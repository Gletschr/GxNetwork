#pragma once

#include "NetworkAPI.h"
#include "NetworkRemoteEngine.h"
#include "NetworkCommand.h"

#include <mutex>

namespace gx {
namespace network {

/**
 * @brief FManager class.
 */
class GX_NETWORK_EXPORT FManager
{

public:

	/**
	 * @brief Destructor.
	 */
	virtual ~FManager();

	/**
	 * @brief Initialize network manager.
	 */
	bool Init();

	/**
	 * @brief Shutdown network manager.
	 */
	void Shutdown();

	/**
	 * @brief Broadcast event for clients.
	 * @param event - event object.
	 */
	template <EEvent Event>
	void BroadcastEvent(const FEvent<Event>& event)
	{
		std::vector<FRemoteEnginePtr>& remoteEngines = LockRemoteEngines();
		for (FRemoteEnginePtr& remoteEngine : remoteEngines)
			remoteEngine->PushEvent(event);
		UnLockRemoteEngines();
	}

	/**
	 * @brief Process network response.
	 * @param remoteEngineGUID - remote engine GUID.
	 * @param input - input packet.
	 * @param output - output packet.
	 * @return true - on processed succesfully, false - otherwise.
	 */
	bool ProcessResponse(const FGuid& remoteEngineGUID, const FBuffer& input, FBuffer& output);

	/**
	 * @brief Function called while new remote engine connected.
	 * @param remoteEngineGUID - remote engine GUID.
	 */
	void RemoteEngineConnected(const FGuid& remoteEngineGUID);

	/**
	 * @brief Function called while remote engine disconnected.
	 * @param remoteEngineGUID - remote engine GUID.
	 */
	void RemoteEngineDisconnected(const FGuid& remoteEngineGUID);

private:

	FRemoteEnginePtr CreateRemoteEngine(const FGuid& remoteEngineGUID);
	FRemoteEnginePtr RemoveRemoteEngine(const FGuid& remoteEngineGUID);

	bool ProcessResponsePing(const FRemoteEnginePtr& remoteEngine, const FCommand<ECommand::Ping>& inCommand, FOStream& stream);
	bool ProcessResponsePong(const FRemoteEnginePtr& remoteEngine, const FCommand<ECommand::Pong>& inCommand, FOStream& stream);

	bool ProcessResponseEventsFrameRequest(const FRemoteEnginePtr& remoteEngine, const FCommand<ECommand::EventsFrameRequest>& inCommand, FOStream& stream);
	bool ProcessResponseEventsFrameRecieve(const FRemoteEnginePtr& remoteEngine, const FCommand<ECommand::EventsFrameRecieve>& inCommand, FOStream& stream);

	bool ProcessResponseReplicationFrameRequest(const FRemoteEnginePtr& remoteEngine, const FCommand<ECommand::ReplicationFrameRequest>& inCommand, FOStream& stream);
	bool ProcessResponseReplicationFrameRecieve(const FRemoteEnginePtr& remoteEngine, const FCommand<ECommand::ReplicationFrameRecieve>& inCommand, FOStream& stream);

protected:

	/**
	 * @brief See FManager::Init().
	 */
	virtual bool OnInit() = 0;

	/**
	 * @brief See FManager::Shutdown().
	 */
	virtual void OnShutdown() = 0;

	/**
	 * @brief Function called while new remote engine connected.
	 * @param remoteEngine - remote engine object.
	 */
	virtual void OnRemoteEngineConnected(const FRemoteEnginePtr& remoteEngine);

	/**
	 * @brief Function called while remote engine disconnected.
	 * @param remoteEngine - remote engine object.
	 */
	virtual void OnRemoteEngineDisconnected(const FRemoteEnginePtr& remoteEngine);

	/**
	 * @brief Process 'Ping' command response.
	 * @param remoteEngine - remote engine object.
	 * @param inCommand - command data.
	 * @param stream - output stream.
	 * @return true - on processed succesfully, false - otherwise.
	 */
	virtual bool OnProcessResponsePing(const FRemoteEnginePtr& remoteEngine, const FCommand<ECommand::Ping>& inCommand, FOStream& stream) = 0;
	
	/**
	 * @brief Process 'Pong' command response.
	 * @param remoteEngine - remote engine object.
	 * @param inCommand - command data.
	 * @param stream - output stream.
	 * @return true - on processed succesfully, false - otherwise.
	 */
	virtual bool OnProcessResponsePong(const FRemoteEnginePtr& remoteEngine, const FCommand<ECommand::Pong>& inCommand, FOStream& stream) = 0;

	/**
	 * @brief Process 'EventsFrameRequest' command response.
	 * @param remoteEngine - remote engine object.
	 * @param inCommand - command data.
	 * @param stream - output stream.
	 * @return true - on processed succesfully, false - otherwise.
	 */
	virtual bool OnProcessResponseEventsFrameRequest(const FRemoteEnginePtr& remoteEngine, const FCommand<ECommand::EventsFrameRequest>& inCommand, FOStream& stream) = 0;
	
	/**
	 * @brief Process 'EventsFrameRecieve' command response.
	 * @param remoteEngine - remote engine object.
	 * @param inCommand - command data.
	 * @param stream - output stream.
	 * @return true - on processed succesfully, false - otherwise.
	 */
	virtual bool OnProcessResponseEventsFrameRecieve(const FRemoteEnginePtr& remoteEngine, const FCommand<ECommand::EventsFrameRecieve>& inCommand, FOStream& stream) = 0;

	/**
	 * @brief Process 'ReplicationFrameRequest' command response.
	 * @param remoteEngine - remote engine object.
	 * @param inCommand - command data.
	 * @param stream - output stream.
	 * @return true - on processed succesfully, false - otherwise.
	 */
	virtual bool OnProcessResponseReplicationFrameRequest(const FRemoteEnginePtr& remoteEngine, const FCommand<ECommand::ReplicationFrameRequest>& inCommand, FOStream& stream) = 0;
	
	/**
	 * @brief Process 'ReplicationFrameRecieve' command response.
	 * @param remoteEngine - remote engine object.
	 * @param inCommand - command data.
	 * @param stream - output stream.
	 * @return true - on processed succesfully, false - otherwise.
	 */
	virtual bool OnProcessResponseReplicationFrameRecieve(const FRemoteEnginePtr& remoteEngine, const FCommand<ECommand::ReplicationFrameRecieve>& inCommand, FOStream& stream) = 0;

private:

	mutable std::mutex _remoteEnginesLock;
	std::vector<FRemoteEnginePtr> _remoteEngines;

protected:

	/**
	 * @brief Release remote engines array lock.
	 */
	void UnLockRemoteEngines();

	/**
	 * @brief Acquire remote engines array locked access.
	 * @return remote engines array reference.
	 */
	std::vector<FRemoteEnginePtr>& LockRemoteEngines();

	/**
	 * @brief Find connected remote engine.
	 * @param remoteEngineGUID - remote engine GUID.
	 * @return remote engine object - on success, nullptr - otherwise.
	 */
	FRemoteEnginePtr FindRemoteEngine(const FGuid& remoteEngineGUID);

};

/**
 * @brief FManager class shared pointer decl.
 */
typedef std::shared_ptr<FManager> FManagerPtr;

}
}
