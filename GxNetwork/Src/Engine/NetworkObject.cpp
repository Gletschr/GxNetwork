#include "../../Include/Engine/NetworkObject.h"
#include "../../Include/Engine/NetworkEngine.h"
#include "../../Include/Common/NetworkLog.h"

namespace gx {
namespace network {
	
FObject::FObject(FEngine* engine, const FGuid& GUID, uint16_t role)
	: FReplicable()
	, _role(role)
	, _GUID(GUID)
	, _engine(engine)
{
	GX_NETWORK_ASSERT(_role != ERole::Undefined);
	GX_NETWORK_ASSERT(
		(_role & ERole::Proxy && !(_role & ERole::Authority)) ||
		(_role & ERole::Authority && !(_role & ERole::Proxy))
	);
	GX_NETWORK_ASSERT(
		(_role & ERole::RemoteProxy && !(_role & ERole::RemoteAuthority)) ||
		(_role & ERole::RemoteAuthority && !(_role & ERole::RemoteProxy))
	);
	GX_NETWORK_ASSERT(_engine);
}

FObject::~FObject()
{
	
}

const FGuid& FObject::GetGUID() const
{
	return _GUID;
}

uint16_t FObject::GetNetworkRole() const
{
	return _role;
}

FFunction* FObject::GetFunction(const char* name)
{
	for (const auto& item : _functions)
	{
		if (item.first == name)
			return item.second;
	}
	return nullptr;
}

FProperty* FObject::GetProperty(const char* name)
{
	for (const auto& item : _properties)
	{
		if (item.first == name)
			return item.second;
	}
	return nullptr;
}

bool FObject::RegisterFunction(const char* name, FFunction* function)
{
	if (GetFunction(name))
		return false;
	_functions[name] = function;
	return true;
}

bool FObject::RegisterProperty(const char* name, FProperty* property)
{
	if (GetProperty(name))
		return false;
	_properties[name] = property;
	return true;
}

bool FObject::ExecFunction(const char* name, const FBuffer& parameters)
{
	FFunction* function = GetFunction(name);
	if (!function)
		return false;
	// TODO: Validate parameters.
	FIStream stream(parameters);
	function->Exec(stream);
	return true;
}

void FObject::ExecFunctionRemote(const char* name, const FBuffer& parameters)
{
	_engine->ExecFunctionRemote(this->GetGUID(), name, parameters);
}

void FObject::operator<<(FIStream& stream)
{
	uint32_t propertiesDataSize = 0;
	stream >> propertiesDataSize;
	uint32_t propertiesStartPos = stream.Pos();
	uint32_t bytesRead = 0;
	while (bytesRead < propertiesDataSize)
	{
		uint32_t bytesReadOffset = stream.Pos();
		std::string propertyName;
		stream >> propertyName;
		FProperty::EType type;
		stream >> type;
		bool isVectorProperty = type == FProperty::EType::Vector;
		FProperty::EType elementType;
		if (isVectorProperty)
		{
			stream >> elementType;
		}
		uint32_t propertyDataSize = 0;
		stream >> propertyDataSize;
		uint32_t propertyStartPos = stream.Pos();
		FProperty* property = GetProperty(propertyName.c_str());
		if  (property && property->GetType() == type && 
			(isVectorProperty ? static_cast<FVectorProperty*>(property)->GetElementType() == elementType : true))
		{
			property->operator<<(stream);
		}
		else
		{
			FLogger::PrintWarning(
				"Unable to replicate property '",
				propertyName,
				"' at <",
				GetClassName(),
				"> [",
				_GUID.A,
				"-",
				_GUID.B,
				"-",
				_GUID.C,
				"-",
				_GUID.D,
				"] object.");
		}
		stream.SetPos(propertyStartPos);
		stream.Read(propertyDataSize);
		bytesRead += stream.Pos() - bytesReadOffset;
	}
	stream.SetPos(propertiesStartPos);
	stream.Read(propertiesDataSize);
}

void FObject::operator>>(FOStream& stream) const
{
	uint32_t propertiesStartPos = stream.Pos();
	for (const auto& item : _properties)
	{
		stream << item.first;
		FProperty::EType type = item.second->GetType();
		stream << type;
		if (type == FProperty::EType::Vector)
		{
			stream << static_cast<FVectorProperty*>(item.second)->GetElementType();
		}
		uint32_t propertyStartPos = stream.Pos();
		item.second->operator>>(stream);
		uint32_t propertyDataSize = stream.Pos() - propertyStartPos;
		stream.SetPos(propertyStartPos);
		stream << propertyDataSize;
		stream.SetPos(propertyStartPos + propertyDataSize + sizeof(propertyDataSize));
	}
	uint32_t propertiesDataSize = stream.Pos() - propertiesStartPos;
	stream.SetPos(propertiesStartPos);
	stream << propertiesDataSize;
	stream.SetPos(propertiesStartPos + propertiesDataSize + sizeof(propertiesDataSize));
}

}
}