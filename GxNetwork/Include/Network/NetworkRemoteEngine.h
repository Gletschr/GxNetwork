#pragma once

#include "NetworkEvent.h"

namespace gx {
namespace network {

/**
 * @brief FRemoteEngine class.
 */
class GX_NETWORK_EXPORT FRemoteEngine
{

public:

	/**
	 * @brief Constructor.
	 * @param GUID - remote engine GUID.
	 */
	FRemoteEngine(const FGuid& GUID);

	/**
	 * @brief Destructor.
	 */
	virtual ~FRemoteEngine();

	/**
	 * @brief Get remote engine GUID.
	 * @return remote engine GUID.
	 */
	const FGuid& GetGUID() const;

	/**
	 * @brief Get remote engine events frame.
	 * @return events frame reference.
	 */
	FBuffer& GetEventsFrame();

	/**
	 * @brief Push event for remote engine.
	 * @param event - event object.
	 */
	template <EEvent Event>
	void PushEvent(const FEvent<Event>& event) 
	{
		_eventsFrame.Lock();
		FOStream stream(_eventsFrame);
		stream << Event;
		stream << event;
		_eventsFrame.UnLock();
	}

private:

	FGuid _GUID;
	FBuffer _eventsFrame;

};

/**
 * @brief FRemoteEngine class shared pointer decl.
 */
typedef std::shared_ptr<FRemoteEngine> FRemoteEnginePtr;


}
}