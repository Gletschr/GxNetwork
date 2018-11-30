#pragma once

#include "../Common/Network.h"

namespace gx {
namespace network {

/**
 * ECommand enum.
 */
enum class GX_NETWORK_EXPORT ECommand : uint8_t
{
	Ping = 0,
	Pong,

	EventsFrameRequest,
	EventsFrameRecieve,

	ReplicationFrameRequest,
	ReplicationFrameRecieve,	

	MaxValue,
};

}
}