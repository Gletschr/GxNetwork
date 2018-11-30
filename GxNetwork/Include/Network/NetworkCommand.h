#pragma once

#include "NetworkAPI.h"
#include "../Common/NetworkTypes.h"

namespace gx { 
namespace network {

/**
 * @brief FCommand struct.
 */
template <ECommand Command>
struct GX_NETWORK_EXPORT FCommand
{
	enum { Defined = 0 };
	
	/**
	 * @brief Deserialize command object.
	 * @param stream - input stream.
	 */
	void operator<<(FIStream& stream)
	{
		static_assert(FCommand<Command>::Defined, "FCommand<...> struct is not defined.");
		return Command;
	}
	
	/**
	 * @brief Serialize command object.
	 * @param stream - output stream.
	 */
	void operator>>(FOStream& stream) const
	{
		static_assert(FCommand<Command>::Defined, "FCommand<...> struct is not defined.");
		return Command;
	}
};

/**
 * @brief Deserialize FCommand object.
 * @param stream - input stream.
 * @param command - object reference.
 * @return stream reference.
 */
template <ECommand Command>
FIStream& operator>>(FIStream& stream, FCommand<Command>& command)
{
	command.operator<<(stream);
	return stream;
}

/**
 * @brief Serialize FCommand object.
 * @param stream - output stream.
 * @param command - object reference.
 * @return stream reference.
 */
template <ECommand Command>
FOStream& operator<<(FOStream& stream, const FCommand<Command>& command)
{
	command.operator>>(stream);
	return stream;
}

/**
 * @brief FCommand<Ping> struct.
 */
template <>
struct GX_NETWORK_EXPORT FCommand <ECommand::Ping>
{
	/**
	 * @brief See FCommand::operator<<(FIStream&).
	 */
	void operator<<(FIStream&)
	{
		
	}
	
	/**
	 * @brief See FCommand::operator>>(FOStream&).
	 */
	void operator>>(FOStream& stream) const
	{

	}
};

/**
 * @brief FCommand<Pong> struct.
 */
template <>
struct GX_NETWORK_EXPORT FCommand <ECommand::Pong>
{
	/**
	 * @brief See FCommand::operator<<(FIStream&).
	 */
	void operator<<(FIStream&)
	{
		
	}
	
	/**
	 * @brief See FCommand::operator>>(FOStream&).
	 */
	void operator>>(FOStream& stream) const
	{

	}
};

/**
 * @brief FCommand<EventsFrameRequest> struct.
 */
template <>
struct GX_NETWORK_EXPORT FCommand <ECommand::EventsFrameRequest>
{
	/**
	 * @brief See FCommand::operator<<(FIStream&).
	 */
	void operator<<(FIStream&)
	{

	}
	
	/**
	 * @brief See FCommand::operator>>(FOStream&).
	 */
	void operator>>(FOStream& stream) const
	{

	}
};

/**
 * @brief FCommand<EventsFrameRecieve> struct.
 */
template <>
struct GX_NETWORK_EXPORT FCommand <ECommand::EventsFrameRecieve>
{
	uint32_t FrameSize = 0;
	const uint8_t* FrameData = nullptr;
	
	/**
	 * @brief See FCommand::operator<<(FIStream&).
	 */
	void operator<<(FIStream& stream)
	{
		stream >> FrameSize;
		FrameData = stream.Read(FrameSize);
	}
	
	/**
	 * @brief See FCommand::operator>>(FOStream&).
	 */
	void operator>>(FOStream& stream) const
	{
		stream << FrameSize;
		stream.Write(FrameData, FrameSize);
	}
};

/**
 * @brief FCommand<ReplicationFrameRequest> struct.
 */
template <>
struct GX_NETWORK_EXPORT FCommand <ECommand::ReplicationFrameRequest>
{
	/**
	 * @brief See FCommand::operator<<(FIStream&).
	 */
	void operator<<(FIStream&)
	{

	}
	
	/**
	 * @brief See FCommand::operator>>(FOStream&).
	 */
	void operator>>(FOStream& stream) const
	{

	}
};

/**
 * @brief FCommand<ReplicationFrameRecieve> struct.
 */
template <>
struct GX_NETWORK_EXPORT FCommand <ECommand::ReplicationFrameRecieve>
{
	uint32_t FrameSize = 0;
	const uint8_t* FrameData = nullptr;
	
	/**
	 * @brief See FCommand::operator<<(FIStream&).
	 */
	void operator<<(FIStream& stream)
	{
		stream >> FrameSize;
		FrameData = stream.Read(FrameSize);
	}
	
	/**
	 * @brief See FCommand::operator>>(FOStream&).
	 */
	void operator>>(FOStream& stream) const
	{
		stream << FrameSize;
		stream.Write(FrameData, FrameSize);
	}
};

}
}