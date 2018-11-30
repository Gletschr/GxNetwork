#include "../../Include/Engine/NetworkEngine.h"
#include "../../Include/Common/NetworkLog.h"
#include "../../Include/Network/NetworkManager.h"

#include <algorithm>

namespace gx {
namespace network {

/**
 * @brief FEngineClassFactory class.
 */
class FEngineClassFactory
{

	FEngineClassFactory()
	{
	}

	~FEngineClassFactory()
	{
	}

public:

	/**
	 * @brief Get class factory instance.
	 * @return class factory instance reference.
	 */
	static FEngineClassFactory& Instance()
	{
		static FEngineClassFactory instance;
		return instance;
	}

	/**
	 * @brief Find class creator by given class name.
	 * @param className - class name.
	 * @param creator - creator object.
	 * @return true on found, false - otherwise.
	 */
	bool FindClass(const char* className, FObject::FCreator& creator)
	{
		std::lock_guard<std::mutex> lock(_mutex);
		auto item = std::find_if(_factory.begin(), _factory.end(), [className](const std::pair<std::string, FObject::FCreator>& item) {
			return item.first == className;
		});
		if (item != _factory.end())
		{
			creator = item->second;
		}
		return item != _factory.end();
	}

	/**
	 * @brief Register class creator.
	 * @param className - class name.
	 * @param creator - creator object.
	 * @return true on registered succesfully, false - otherwise.
	 */
	bool RegisterClass(const char* className, const FObject::FCreator& creator)
	{
		std::lock_guard<std::mutex> lock(_mutex);
		auto item = std::find_if(_factory.begin(), _factory.end(), [className](const std::pair<std::string, FObject::FCreator>& item) {
			return item.first == className;
		});
		if (item == _factory.end())
		{
			_factory[className] = creator;
		}
		return item == _factory.end();
	}

private:

	std::mutex _mutex;
	std::map<std::string, FObject::FCreator> _factory;

};

FEngine::FEngine(const FGuid& GUID, const FManagerPtr& manager)
	: _GUID(GUID)
	, _manager(manager)
{
	GX_NETWORK_ASSERT(_manager);
}

FEngine::~FEngine()
{
}

bool FEngine::Init(EMode mode)
{
	GX_NETWORK_ASSERT(_bInitialized == false);
	_mode = mode;
	_bInitialized = true;
	_bInitialized = _bInitialized && OnInit();
	_bInitialized = _bInitialized && _manager->Init();
	return _bInitialized;
}

void FEngine::Tick(float dt)
{
	if (!CheckInitialized(__FUNCTION__))
		return;

	_replicationFrame.Lock();
	_replicationFrame.Clear();
	network::FOStream stream(_replicationFrame);
	Replicate(stream);
	_replicationFrame.UnLock();
	OnTick(dt);
}

void FEngine::Shutdown()
{
	GX_NETWORK_ASSERT(_bInitialized == true);
	OnShutdown();
	_objects.clear();
	_manager->Shutdown();
	_bInitialized = false;
}

const FGuid& FEngine::GetGUID() const
{
	return _GUID;
}

void FEngine::Replicate(FIStream& stream)
{
	if (!CheckInitialized(__FUNCTION__))
		return;

	while (!stream.IsEOF())
	{
		FGuid GUID;
		stream >> GUID;

		std::string className;
		stream >> className;

		uint32_t  objectDataSize = 0;
		stream >> objectDataSize;
		
		uint32_t  objectStartPos = stream.Pos();

		FObjectPtr object = GetObjectByGUID(GUID);

		if (object)
		{
			if (className != object->GetClassName())
			{
				FLogger::PrintWarning(
					"Unable to replicate object <", 
					object->GetClassName(), 
					">[", 
					GUID.A, 
					"-", 
					GUID.B, 
					"-", 
					GUID.C, 
					"-", 
					GUID.D, 
					"]. Class name mismatch: <", 
					className, ">.");
			}
			else
			{
				if (_mode == EMode::Client)
				{
					stream >> object;
				}
				else // _mode == EMode::Server
				{
					if (object->GetNetworkRole() & FObject::ERole::RemoteAuthority)
					{
						stream >> object;
					}
				}
			}			
		}
		else
		{
			if (_mode == EMode::Server)
			{
				FLogger::PrintError(
					"Unable to replicate object <",
					className,
					">[",
					GUID.A,
					"-",
					GUID.B,
					"-",
					GUID.C,
					"-",
					GUID.D,
					"]. Object not found.");
			}
			else // _mode == EMode::Client
			{
				FObjectPtr object = CreateObjectStatic(GUID, className.c_str());

				// Replicate immediately after creation.
				// TODO: Experimental.
				stream >> object;

				if (object)
				{
					OnObjectCreated(object);
				}				
			}
		}
		
		stream.SetPos(objectStartPos);
		stream.Read(objectDataSize);
	}
}

// Object semantic
//
// 1. Object GUID				| uint32_t[4]
// 3. Object class name size	| uint32_t
// 4. Object class name			| char[]
// 5. Object data size			| uint32_t
// 6. Object data				| uint8_t[]

void FEngine::Replicate(FOStream& stream) const
{
	if (!CheckInitialized(__FUNCTION__))
		return;

	for (const FObjectPtr& object : _objects)
	{
		if (object->GetNetworkRole() & FObject::ERole::Authority || object->GetNetworkRole() & FObject::ERole::RemoteAuthority)
		{
			stream << object->GetGUID();
			stream << std::string(object->GetClassName());
			uint32_t objectStartPos = stream.Pos();
			stream << object;
			uint32_t objectDataSize = stream.Pos() - objectStartPos;
			stream.SetPos(objectStartPos);
			stream << objectDataSize;
			stream.SetPos(objectStartPos + objectDataSize + sizeof(objectDataSize));
		}		
	}
}

void FEngine::ReplicateEvents(FIStream& stream)
{
	if (!CheckInitialized(__FUNCTION__))
		return;

	bool result = true;

	while (result && !stream.IsEOF())
	{
		uint8_t   v;
		stream >> v;
		if (v >= static_cast<uint8_t>(EEvent::MaxValue))
		{
			result = false;
		}
		else
		{
			EEvent  e = static_cast<EEvent>(v);
			switch (e) 
			{
				case EEvent::CreateObject: 
				{
					FEvent<EEvent::CreateObject> event;
					stream >> event;
					result = result && ProcessEventCreateObject(event.GUID, event.OwnerGUID, event.ClassName.c_str());
					break;
				}

				case EEvent::RemoveObject:
				{
					FEvent<EEvent::RemoveObject> event;
					stream >> event;
					result = result && ProcessEventRemoveObject(event.GUID);
					break;
				}

				case EEvent::ExecFunctionRemote:
				{
					FEvent<EEvent::ExecFunctionRemote> event;
					stream >> event;
					FBuffer parameters;
					parameters.Append(event.ParametersData, event.ParametersSize);
					result = result && ProcessEventExecFunctionRemote(event.GUID, event.FunctionName.c_str(), parameters);
					break;
				}

				default:
				{
					result = false;
					break;
				}
			}
		}
	}
	if (!result)
	{
		FLogger::PrintError("An error occured during processing events.");
	}
}

void FEngine::PlayerJoinGame(const FGuid& playerGUID)
{
	if (!CheckInitialized(__FUNCTION__))
		return;

	if (_mode == EMode::Server)
	{
		CreateObjectDynamic(playerGUID, playerGUID, "CharacterActor");
	}
	else // _mode == EMode::Client
	{
		FLogger::PrintError("Unable to join game at client engine.");
	}
}

void FEngine::PlayerQuitGame(const FGuid& playerGUID)
{
	if (!CheckInitialized(__FUNCTION__))
		return;

	if (_mode == EMode::Server)
	{
		RemoveObjectDynamic(playerGUID);
	}
	else // _mode == EMode::Client
	{
		FLogger::PrintError("Unable to quit game at client engine.");
	}
}

void FEngine::ExecFunctionRemote(const FGuid& GUID, const char* name, const FBuffer& parameters)
{
	BroadcastEventExecFunctionRemote(GUID, name, parameters);
}

const FBuffer& FEngine::GetReplicationFrame() const
{
	return _replicationFrame;
}

FObjectPtr FEngine::GetObjectByGUID(const FGuid& GUID) const
{
	for (const FObjectPtr& object : _objects)
	{
		if (object->GetGUID() == GUID)
		{
			return object;
		}
	}
	return nullptr;
}

FManagerPtr FEngine::GetNetworkManager() const
{
	return _manager;
}

bool FEngine::RegisterClass(const char* className, const FObject::FCreator& creator)
{
	return FEngineClassFactory::Instance().RegisterClass(className, creator);
}

FObjectPtr FEngine::CreateObjectStatic(const FGuid& GUID, const char* className)
{
	if (!CheckInitialized(__FUNCTION__))
		return nullptr;

	uint16_t role = FObject::ERole::Undefined;

	if (_mode == EMode::Client)
	{
		role = FObject::ERole::Proxy | FObject::ERole::RemoteProxy;
	}
	else // _mode == EMode::Server
	{
		role = FObject::ERole::Authority | FObject::ERole::RemoteProxy;
	}

	FObjectPtr object = CreateObjectByClassName(GUID, className, role);

	if (object && _mode == EMode::Server)
	{
		BroadcastEventCreateObject(GUID, this->_GUID, className);
	}

	return object;
}

FObjectPtr FEngine::RemoveObjectStatic(const FGuid& GUID)
{
	if (!CheckInitialized(__FUNCTION__))
		return nullptr;

	FObjectPtr object = RemoveObjectByGUID(GUID);

	if (object && _mode == EMode::Server)
	{
		BroadcastEventRemoveObject(GUID);
	}

	return object;
}

FObjectPtr FEngine::CreateObjectDynamic(const FGuid& GUID, const FGuid& ownerGUID, const char* className)
{
	if (!CheckInitialized(__FUNCTION__))
		return nullptr;

	uint16_t role = FObject::ERole::Undefined;

	if (_mode == EMode::Client)
	{
		if (ownerGUID != this->_GUID)
		{
			role = FObject::ERole::Proxy | FObject::ERole::RemoteProxy;
		}
		else
		{
			role = FObject::ERole::Proxy | FObject::ERole::RemoteAuthority;
		}
	}
	else // _mode == EMode::Server
	{
		if (ownerGUID == this->_GUID)
		{
			role = FObject::ERole::Authority | FObject::ERole::RemoteProxy;
		}
		else
		{
			role = FObject::ERole::Authority | FObject::ERole::RemoteAuthority;
		}
	}

	FObjectPtr object = CreateObjectByClassName(GUID, className, role);

	if (object)
	{
		OnObjectCreated(object);
		if (_mode == EMode::Server)
		{
			BroadcastEventCreateObject(GUID, ownerGUID, className);
		}
	}

	return object;
}

FObjectPtr FEngine::RemoveObjectDynamic(const FGuid& GUID)
{
	if (!CheckInitialized(__FUNCTION__))
		return nullptr;

	FObjectPtr object = RemoveObjectByGUID(GUID);

	if (object)
	{
		OnObjectRemoved(object);
		if (_mode == EMode::Server)
		{
			BroadcastEventRemoveObject(GUID);
		}
	}

	return object;
}

bool FEngine::ProcessEventExecFunctionRemote(const FGuid& GUID, const char* name, const FBuffer& parameters)
{
	if (!CheckInitialized(__FUNCTION__))
		return false;
	FObjectPtr object = GetObjectByGUID(GUID);
	if (object)
	{
		return object->ExecFunction(name, parameters);
	}
	return false;
}

void FEngine::BroadcastEventExecFunctionRemote(const FGuid& GUID, const char* name, const FBuffer& parameters)
{
	if (!CheckInitialized(__FUNCTION__))
		return;
	FEvent<EEvent::ExecFunctionRemote> execFunctionRemoteEvent;
	execFunctionRemoteEvent.GUID = GUID;
	execFunctionRemoteEvent.FunctionName = name;
	execFunctionRemoteEvent.ParametersSize = parameters.Size();
	execFunctionRemoteEvent.ParametersData = parameters.Data();
	_manager->BroadcastEvent(execFunctionRemoteEvent);
}

bool FEngine::ProcessEventCreateObject(const FGuid& GUID, const FGuid& ownerGUID, const char* className)
{
	if (!CheckInitialized(__FUNCTION__))
		return nullptr;

	FObjectPtr object;

	if (_mode == EMode::Server)
	{
		FLogger::PrintError(
			"Access denied (create object dynamic on server side <",
			className,
			">[",
			GUID.A,
			"-",
			GUID.B,
			"-",
			GUID.C,
			"-",
			GUID.D,
			"] with owner [",
			ownerGUID.A,
			"-",
			ownerGUID.B,
			"-",
			ownerGUID.C,
			"-",
			ownerGUID.D,
			"]).");
	}
	else // _mode == EMode::Client
	{
		object = CreateObjectDynamic(GUID, ownerGUID, className);
	}

	return object != nullptr;
}

bool FEngine::ProcessEventRemoveObject(const FGuid& GUID)
{
	if (!CheckInitialized(__FUNCTION__))
		return nullptr;

	FObjectPtr object;

	if (_mode == EMode::Server)
	{
		FLogger::PrintError(
			"Access denied (remove object dynamic on server side <",
			">[",
			GUID.A,
			"-",
			GUID.B,
			"-",
			GUID.C,
			"-",
			GUID.D,
			"]).");
	}
	else // _mode == EMode::Client
	{
		object = RemoveObjectDynamic(GUID);
	}

	return object != nullptr;
}

void FEngine::BroadcastEventCreateObject(const FGuid& GUID, const FGuid& ownerGUID, const char* className)
{
	if (!CheckInitialized(__FUNCTION__))
		return;

	GX_NETWORK_ASSERT(_mode == EMode::Server);
	FEvent<EEvent::CreateObject> createObjectEvent;
	createObjectEvent.GUID = GUID;
	createObjectEvent.ClassName = className;
	createObjectEvent.OwnerGUID = ownerGUID;
	_manager->BroadcastEvent(createObjectEvent);
}

void FEngine::BroadcastEventRemoveObject(const FGuid& GUID)
{
	if (!CheckInitialized(__FUNCTION__))
		return;

	GX_NETWORK_ASSERT(_mode == EMode::Server);
	FEvent<EEvent::RemoveObject> removeObjectEvent;
	removeObjectEvent.GUID = GUID;
	_manager->BroadcastEvent(removeObjectEvent);
}

bool FEngine::CheckInitialized(const char* context) const
{
	if (!_bInitialized)
		FLogger::PrintError("Engine not initialized (", context, ").");
	return _bInitialized;
}

FObjectPtr FEngine::CreateObjectByClassName(const FGuid& GUID, const char* className, uint16_t role)
{
	if (!CheckInitialized(__FUNCTION__))
		return nullptr;

	FObject::FCreator creator = nullptr;

	if (!FEngineClassFactory::Instance().FindClass(className, creator))
	{
		FLogger::PrintError(
			"Unable to create object '", 
			className, 
			"' ",
			GUID.A, 
			"-", 
			GUID.B, 
			"-", 
			GUID.C, 
			"-", 
			GUID.D, 
			". Class not found.");
		return nullptr;
	}

	if (GetObjectByGUID(GUID) != nullptr)
	{
		FLogger::PrintError(
			"Unable to create object '", 
			className, 
			"' ", 
			GUID.A, 
			"-", 
			GUID.B, 
			"-", 
			GUID.C, 
			"-", 
			GUID.D, 
			". Object already exists.");
		return nullptr;
	}

	FObjectPtr object = creator(this, GUID, role);

	if (object)
	{
		_objects.push_back(object);
	}

	return object;
}

FObjectPtr FEngine::RemoveObjectByGUID(const FGuid& GUID)
{
	if (!CheckInitialized(__FUNCTION__))
		return nullptr;

	FObjectPtr object;
	auto i = std::remove_if(_objects.begin(), _objects.end(), [&](FObjectPtr& item) {
		if (item->GetGUID() == GUID)
		{
			object = item;
			return true;
		}
		return false;
	});
	_objects.erase(i, _objects.end());
	return object;
}

bool FEngine::OnInit()
{
	return true;
}

void FEngine::OnShutdown()
{
}

void FEngine::OnTick(float dt)
{
	GX_NETWORK_UNUSED(dt);
}

void FEngine::OnObjectCreated(const FObjectPtr& object)
{
	GX_NETWORK_UNUSED(object);
}

void FEngine::OnObjectRemoved(const FObjectPtr& object)
{
	GX_NETWORK_UNUSED(object);
}

}
}
