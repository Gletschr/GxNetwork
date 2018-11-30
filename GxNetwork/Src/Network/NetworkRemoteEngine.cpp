#include "../../Include/Network/NetworkRemoteEngine.h"

namespace gx {
namespace network {

FRemoteEngine::FRemoteEngine(const FGuid& GUID)
	: _GUID(GUID)
{
}

FRemoteEngine::~FRemoteEngine()
{
}

const FGuid& FRemoteEngine::GetGUID() const
{
	return _GUID;
}

FBuffer& FRemoteEngine::GetEventsFrame()
{
	return _eventsFrame;
}

}
}