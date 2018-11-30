#pragma once

#include "../Common/NetworkStream.h"

namespace gx {
namespace network {

/**
 * @brief FReplicable class.
 */
class GX_NETWORK_EXPORT FReplicable
{

public:

	/**
	 * @brief Destructor.
	 */
	virtual ~FReplicable();
	
	/**
	 * @brief Deserialize replicable object.
	 * @param stream - input stream.
	 */
	virtual void operator<<(FIStream& stream) = 0;
	
	/**
	 * @brief Serialize replicable object.
	 * @param stream - output stream.
	 */
	virtual void operator>>(FOStream& stream) const = 0;

};

/**
 * @brief Deserialize FReplicable object.
 * @param stream - input stream.
 * @param replicable - object reference.
 * @return stream reference.
 */
template <class T, bool IsReplicable = std::is_base_of<FReplicable, T>::value>
FIStream& operator>>(FIStream& stream, std::shared_ptr<T>& replicable)
{
	static_assert(IsReplicable, "Accepted only FReplicable derrived objects.");
	replicable->operator<<(stream);
	return stream;
}

/**
 * @brief Serialize FReplicable object.
 * @param stream - output stream.
 * @param replicable - object reference.
 * @return stream reference.
 */
template <class T, bool IsReplicable = std::is_base_of<FReplicable, T>::value>
FOStream& operator<<(FOStream& stream, const std::shared_ptr<T>& replicable)
{
	static_assert(IsReplicable, "Accepted only FReplicable derrived objects.");
	replicable->operator>>(stream);
	return stream;
}

}
}









































