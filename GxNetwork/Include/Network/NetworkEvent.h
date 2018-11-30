#pragma once

#include "../Common/NetworkTypes.h"

namespace gx { 
namespace network {

/**
 * EEvent enum.
 */
enum class GX_NETWORK_EXPORT EEvent : uint8_t
{
	CreateObject = 0,
	RemoveObject,
	ExecFunctionRemote,
	MaxValue,
};

/**
 * @brief FEvent struct.
 */
template <EEvent Event>
struct GX_NETWORK_EXPORT FEvent
{
	enum { Defined = 0 };
	
	/**
	 * @brief Deserialize event object.
	 * @param stream - input stream.
	 */
	void operator<<(FIStream& stream)
	{
		static_assert(FEvent<Event>::Defined, "FEvent<...> struct is not defined.");
		return Event;
	}
	
	/**
	 * @brief Serialize event object.
	 * @param stream - output stream.
	 */
	void operator>>(FOStream& stream) const
	{
		static_assert(FEvent<Event>::Defined, "FEvent<...> struct is not defined.");
		return Event;
	}
};

/**
 * @brief Deserialize FEvent object.
 * @param stream - input stream.
 * @param event - object reference.
 * @return stream reference.
 */
template <EEvent Event>
FIStream& operator>>(FIStream& stream, FEvent<Event>& event)
{
	event.operator<<(stream);
	return stream;
}

/**
 * @brief Serialize FEvent object.
 * @param stream - output stream.
 * @param event - object reference.
 * @return stream reference.
 */
template <EEvent Event>
FOStream& operator<<(FOStream& stream, const FEvent<Event>& event)
{
	event.operator>>(stream);
	return stream;
}

/**
 * @brief FEvent<CreateObject> struct.
 */
template <>
struct GX_NETWORK_EXPORT FEvent <EEvent::CreateObject>
{
	FGuid GUID;
	FGuid OwnerGUID;
	std::string ClassName;

	/**
	 * @brief See FEvent::operator<<(FIStream&).
	 */
	void operator<<(FIStream& stream)
	{
		stream >> GUID;
		stream >> ClassName;
		stream >> OwnerGUID;
	}
	
	/**
	 * @brief See FEvent::operator>>(FOStream&).
	 */
	void operator>>(FOStream& stream) const
	{
		stream << GUID;
		stream << ClassName;
		stream << OwnerGUID;
	}
};

/**
 * @brief FEvent<RemoveObject> struct.
 */
template <>
struct GX_NETWORK_EXPORT FEvent <EEvent::RemoveObject>
{
	FGuid GUID;

	/**
	 * @brief See FEvent::operator<<(FIStream&).
	 */
	void operator<<(FIStream& stream)
	{
		stream >> GUID;
	}
	
	/**
	 * @brief See FEvent::operator>>(FOStream&).
	 */
	void operator>>(FOStream& stream) const
	{
		stream << GUID;
	}
};

/**
 * @brief FEvent<ExecFunctionRemote> struct.
 */
template <>
struct GX_NETWORK_EXPORT FEvent <EEvent::ExecFunctionRemote>
{
	FGuid GUID;
	std::string FunctionName;
	uint32_t ParametersSize = 0;
	const uint8_t* ParametersData = nullptr;

	/**
	 * @brief See FEvent::operator<<(FIStream&).
	 */
	void operator<<(FIStream& stream)
	{
		stream >> GUID;
		stream >> FunctionName;
		stream >> ParametersSize;
		ParametersData = stream.Read(ParametersSize);
	}
	
	/**
	 * @brief See FEvent::operator>>(FOStream&).
	 */
	void operator>>(FOStream& stream) const
	{
		stream << GUID;
		stream << FunctionName;
		stream << ParametersSize;
		stream.Write(ParametersData, ParametersSize);
	}
};

}
}