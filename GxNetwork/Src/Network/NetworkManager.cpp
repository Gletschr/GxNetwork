#include "../../Include/Network/NetworkManager.h"
#include "../../Include/Common/NetworkLog.h"

#include <algorithm>

namespace gx {
namespace network {

FManager::~FManager()
{
	
}

bool FManager::Init()
{
	return OnInit();
}

void FManager::Shutdown()
{
	OnShutdown();
}

bool FManager::ProcessResponse(const FGuid& remoteEngineGUID, const FBuffer& input, FBuffer& output)
{
	FIStream istream(input);
	FOStream ostream(output);

	FRemoteEnginePtr remoteEngine = FindRemoteEngine(remoteEngineGUID);

	if (!remoteEngine)
	{
		FLogger::PrintError("Access denied. Request from unknown engine [",
			remoteEngineGUID.A,
			"-",
			remoteEngineGUID.B,
			"-",
			remoteEngineGUID.C,
			"-",
			remoteEngineGUID.D,
			"].");
		return false;
	}

	bool result = true;

	while (result && !istream.IsEOF())
	{
		uint8_t    v;
		istream >> v;

		if (v >= static_cast<uint8_t>(ECommand::MaxValue))
		{
			result = false;
		}
		else
		{
			ECommand c = static_cast<ECommand>(v);

			switch  (c)
			{
				case ECommand::Ping:
				{
					FCommand<ECommand::Ping> command;
					istream >> command;
					result = result && ProcessResponsePing(remoteEngine, command, ostream);
					break;
				}

				case ECommand::Pong:
				{
					FCommand<ECommand::Pong> command;
					istream >> command;
					result = result && ProcessResponsePong(remoteEngine, command, ostream);
					break;
				}

				case ECommand::EventsFrameRequest:
				{
					FCommand<ECommand::EventsFrameRequest> command;
					istream >> command;
					result = result && ProcessResponseEventsFrameRequest(remoteEngine, command, ostream);
					break;
				}

				case ECommand::EventsFrameRecieve:
				{
					FCommand<ECommand::EventsFrameRecieve> command;
					istream >> command;
					result = result && ProcessResponseEventsFrameRecieve(remoteEngine, command, ostream);
					break;
				}

				case ECommand::ReplicationFrameRequest:
				{
					FCommand<ECommand::ReplicationFrameRequest> command;
					istream >> command;
					result = result && ProcessResponseReplicationFrameRequest(remoteEngine, command, ostream);
					break;
				}

				case ECommand::ReplicationFrameRecieve:
				{
					FCommand<ECommand::ReplicationFrameRecieve> command;
					istream >> command;
					result = result && ProcessResponseReplicationFrameRecieve(remoteEngine, command, ostream);
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

	return result;
}

void FManager::RemoteEngineConnected(const FGuid& remoteEngineGUID)
{
	if (!FindRemoteEngine(remoteEngineGUID))
	{
		FRemoteEnginePtr remoteEngine = CreateRemoteEngine(remoteEngineGUID);
		OnRemoteEngineConnected(remoteEngine);
	}
}

void FManager::RemoteEngineDisconnected(const FGuid& remoteEngineGUID)
{
	FRemoteEnginePtr remoteEngine = RemoveRemoteEngine(remoteEngineGUID);
	if (remoteEngine)
	{
		OnRemoteEngineDisconnected(remoteEngine);
	}
}

FRemoteEnginePtr FManager::FindRemoteEngine(const FGuid & remoteEngineGUID)
{
	std::vector<FRemoteEnginePtr>& remoteEngines = LockRemoteEngines();
	auto item = std::find_if(remoteEngines.begin(), remoteEngines.end(), [remoteEngineGUID](FRemoteEnginePtr& remoteEngine) {
		return remoteEngine->GetGUID() == remoteEngineGUID;
	});
	FRemoteEnginePtr remoteEngine = item != remoteEngines.end() ? *item : nullptr;
	UnLockRemoteEngines();
	return remoteEngine;
}

FRemoteEnginePtr FManager::CreateRemoteEngine(const FGuid& remoteEngineGUID)
{
	std::vector<FRemoteEnginePtr>& remoteEngines = LockRemoteEngines();
	FRemoteEnginePtr remoteEngine = FRemoteEnginePtr(new FRemoteEngine(remoteEngineGUID));
	remoteEngines.push_back(remoteEngine);
	UnLockRemoteEngines();
	return remoteEngine;
}

FRemoteEnginePtr FManager::RemoveRemoteEngine(const FGuid& remoteEngineGUID)
{
	std::vector<FRemoteEnginePtr>& remoteEngines = LockRemoteEngines();
	FRemoteEnginePtr remoteEngine;
	auto i = std::remove_if(remoteEngines.begin(), remoteEngines.end(), [&](FRemoteEnginePtr& item) {
		if (item->GetGUID() == remoteEngineGUID)
		{
			remoteEngine = item;
			return true;
		}
		return false;
	});
	remoteEngines.erase(i, remoteEngines.end());
	UnLockRemoteEngines();
	return remoteEngine;
}

bool FManager::ProcessResponsePing(const FRemoteEnginePtr& remote, const FCommand<ECommand::Ping>& inCommand, FOStream& stream)
{
	return OnProcessResponsePing(remote, inCommand, stream);
}

bool FManager::ProcessResponsePong(const FRemoteEnginePtr& remote, const FCommand<ECommand::Pong>& inCommand, FOStream& stream)
{
	return OnProcessResponsePong(remote, inCommand, stream);
}

bool FManager::ProcessResponseEventsFrameRequest(const FRemoteEnginePtr& remote, const FCommand<ECommand::EventsFrameRequest>& inCommand, FOStream& stream)
{
	return OnProcessResponseEventsFrameRequest(remote, inCommand, stream);
}

bool FManager::ProcessResponseEventsFrameRecieve(const FRemoteEnginePtr& remote, const FCommand<ECommand::EventsFrameRecieve>& inCommand, FOStream& stream)
{
	return OnProcessResponseEventsFrameRecieve(remote, inCommand, stream);
}

bool FManager::ProcessResponseReplicationFrameRequest(const FRemoteEnginePtr& remote, const FCommand<ECommand::ReplicationFrameRequest>& inCommand, FOStream& stream)
{
	return OnProcessResponseReplicationFrameRequest(remote, inCommand, stream);
}

bool FManager::ProcessResponseReplicationFrameRecieve(const FRemoteEnginePtr& remote, const FCommand<ECommand::ReplicationFrameRecieve>& inCommand, FOStream& stream)
{
	return OnProcessResponseReplicationFrameRecieve(remote, inCommand, stream);
}

void FManager::OnRemoteEngineConnected(const FRemoteEnginePtr & remoteEngine)
{
}

void FManager::OnRemoteEngineDisconnected(const FRemoteEnginePtr & remoteEngine)
{
}


/**
* @brief Acquire remote engines array locked access.
* @return remote engines array reference.
*/

std::vector<FRemoteEnginePtr>& FManager::LockRemoteEngines()
{
	_remoteEnginesLock.lock();
	return _remoteEngines;
}

/**
* @brief Release remote engines array lock.
*/

void FManager::UnLockRemoteEngines()
{
	_remoteEnginesLock.unlock();
}

}
}
