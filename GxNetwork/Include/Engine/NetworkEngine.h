#pragma once

#include "NetworkObject.h"

#include <map>
#include <string>

namespace gx {
namespace network {

/**
 * @brief FManager class forward decl.
 */
class FManager;

/**
 * @brief FManager class shared pointer forward decl.
 */
typedef std::shared_ptr<FManager> FManagerPtr;

/**
 * @brief FEngine class.
 */
class GX_NETWORK_EXPORT FEngine
{

public:

	/**
	 * @brief FEngine::EMode enum.
	 */
	enum EMode
	{
		Client,
		Server,
	};
	
	/**
	 * @brief Constructor.
	 * @param GUID - engine GUID.
	 * @param manager - engine network manager.
	 */
	FEngine(const FGuid& GUID, const FManagerPtr& manager);

	/**
	 * @brief Destructor.
	 */
	virtual ~FEngine();
	
	/**
	 * @brief Initialize engine.
	 * @param mode - engine mode (server or client).
	 * @return true on initialized succesfully, false - otherwise.
	 */
	bool Init(EMode mode);

	/**
	 * @brief Network engine tick.
	 * @param dt - delta time from last tick.
	 */
	void Tick(float dt);
	
	/**
	 * @brief Shutdown engine.
	 */
	void Shutdown();

	/**
	 * @brief Get engine GUID.
	 * @return engine GUID.
	 */
	const FGuid& GetGUID() const;

	/**
	 * @brief Get engine network manager.
	 * @return network manager pointer on success, nullptr - otherwise.
	 */
	FManagerPtr GetNetworkManager() const;

	/**
	 * @brief Replicate engine state (deserialize).
	 * @param stream - input stream.
	 */
	void Replicate(FIStream& stream);
	
	/**
	 * @brief Replicate engine state (serialize).
	 * @param stream - output stream.
	 */
	void Replicate(FOStream& stream) const;

	/**
	 * @brief Replicate engine events.
	 * @param stream - input stream.
	 */
	void ReplicateEvents(FIStream& stream);

	/**
	 * @brief Called while player joined game.
	 * @param playerGUID - player GUID.
	 */
	void PlayerJoinGame(const FGuid& playerGUID);

	/**
	 * @brief Called while player quited game.
	 * @param playerGUID - player GUID.
	 */
	void PlayerQuitGame(const FGuid& playerGUID);

	/**
	 * @brief Exec remote function by name.
	 * @param GUID - caller object GUID.
	 * @param name - function name.
	 * @param parameters - input parameters.
	 */
	void ExecFunctionRemote(const FGuid& GUID, const char* name, const FBuffer& parameters);

	/**
	 * @brief Get engine replication frame.
	 * @return replication frame const reference.
	 */
	const FBuffer& GetReplicationFrame() const;

	/**
	 * @brief Get object by GUID.
	 * @param GUID - object GUID.
	 * @return object pointer on success, nullptr - otherwise.
	 */
	FObjectPtr GetObjectByGUID(const FGuid& GUID) const;

	/**
	 * @brief Create object by class name.
	 * @param GUID - object GUID.
	 * @param className - object class name.
	 * @return object pointer on success, nullptr - otherwise.
	 */
	FObjectPtr CreateObjectStatic(const FGuid& GUID, const char* className);

	/**
	 * @brief Remove object by GUID.
	 * @param GUID - object GUID.
	 * @return object pointer on success, nullptr - otherwise.
	 */
	FObjectPtr RemoveObjectStatic(const FGuid& GUID);

	/**
	 * @brief Register class.
	 * @param className - class name.
	 * @param creator - static creator function.
	 * @return true on success, false - otherwise.
	 */
	static bool RegisterClass(const char* className, const FObject::FCreator& creator);

private:

	bool CheckInitialized(const char* context = "") const;

	FObjectPtr CreateObjectByClassName(const FGuid& GUID, const char* className, uint16_t role);
	FObjectPtr RemoveObjectByGUID(const FGuid& GUID);

	FObjectPtr CreateObjectDynamic(const FGuid& GUID, const FGuid& ownerGUID, const char* className);
	FObjectPtr RemoveObjectDynamic(const FGuid& GUID);

	bool ProcessEventExecFunctionRemote(const FGuid& GUID, const char* name, const FBuffer& parameters);
	void BroadcastEventExecFunctionRemote(const FGuid& GUID, const char* name, const FBuffer& parameters);

	bool ProcessEventCreateObject(const FGuid& GUID, const FGuid& ownerGUID, const char* className);
	bool ProcessEventRemoveObject(const FGuid& GUID);

	void BroadcastEventCreateObject(const FGuid& GUID, const FGuid& ownerGUID, const char* className);
	void BroadcastEventRemoveObject(const FGuid& GUID);

protected:

	/**
	 * @brief See FEngine::Init().
	 */
	virtual bool OnInit();

	/**
	 * @brief See FEngine::OnShutdown().
	 */
	virtual void OnShutdown();

	/**
	 * @brief See FEngine::Tick().
	 */
	virtual void OnTick(float dt);

	/**
	 * @brief Callback, called while object created.
	 * @param object - created object.
	 */
	virtual void OnObjectCreated(const FObjectPtr& object);

	/**
	 * @brief Callback, called while object removed.
	 * @param object - removed object.
	 */
	virtual void OnObjectRemoved(const FObjectPtr& object);

private:
	
	FGuid _GUID;
	EMode _mode;

	bool _bInitialized = false;

	FBuffer _replicationFrame;
	std::vector<FObjectPtr> _objects;

protected:

	FManagerPtr _manager;

	
};

}
}
