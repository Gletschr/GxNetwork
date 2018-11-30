#pragma once

#include "NetworkStream.h"

#include <map>
#include <string>

namespace gx {
namespace network {

/**
 * @brief FVec3f struct.
 */
struct GX_NETWORK_EXPORT FVec3f
{
	/**
	 * @brief Constructor.
	 */
	FVec3f();

	/**
	 * @brief Constructor.
	 * @param vec3f - prototype.
	 */
	FVec3f(const FVec3f& vec3f);

	/**
	 * @brief Constructor.
	 * @param x - x component.
	 * @param y - y component.
	 * @param z - z component.
	 */
	FVec3f(float x, float y, float z);

	union
	{
		struct
		{
			float x;
			float y;
			float z;
		};
		uint8_t Data[sizeof(float) * 3];
	};
};

/**
 * @brief FVec3f equal operator.
 * @param a - first value.
 * @param b - second value.
 * @return true if values are equal, false - otherwise.
 */
GX_NETWORK_EXPORT bool operator==(const FVec3f& a, const FVec3f& b);

/**
 * @brief FVec3f notequal operator.
 * @param a - first value.
 * @param b - second value.
 * @return true if values are not equal, false - otherwise.
 */
GX_NETWORK_EXPORT bool operator!=(const FVec3f& a, const FVec3f& b);

/**
 * @brief Deserialize FVec3f object.
 * @param stream - input stream.
 * @param vec3f - object reference.
 * @return stream reference.
 */
GX_NETWORK_EXPORT FIStream& operator>>(FIStream& stream, FVec3f& vec3f);

/**
 * @brief Serialize FVec3f object.
 * @param stream - output stream.
 * @param vec3f - object reference.
 * @return stream reference.
 */
GX_NETWORK_EXPORT FOStream& operator<<(FOStream& stream, const FVec3f& vec3f);

/**
 * @brief FGuid struct.
 */
struct GX_NETWORK_EXPORT FGuid
{
	/**
	 * @brief Constructor.
	 */
	FGuid();

	/**
	 * @brief Constructor.
	 * @param guid - prototype.
	 */
	FGuid(const FGuid& guid);

	/**
	 * @brief Constructor.
	 * @param a - guid part A.
	 * @param b - guid part B.
	 * @param c - guid part C.
	 * @param d - guid part D.
	 */
	FGuid(uint32_t a, uint32_t b, uint32_t c, uint32_t d);

	union
	{
		struct
		{
			uint32_t A;
			uint32_t B;
			uint32_t C;
			uint32_t D;
		};
		uint8_t Data[sizeof(uint32_t) * 4];
	};
};

/**
 * @brief FGuid equal operator.
 * @param a - first value.
 * @param b - second value.
 * @return true if values are equal, false - otherwise.
 */
GX_NETWORK_EXPORT bool operator==(const FGuid& a, const FGuid& b);

/**
 * @brief FGuid notequal operator.
 * @param a - first value.
 * @param b - second value.
 * @return true if values are not equal, false - otherwise.
 */
GX_NETWORK_EXPORT bool operator!=(const FGuid& a, const FGuid& b);

/**
 * @brief Deserialize FGuid object.
 * @param stream - input stream.
 * @param GUID - object reference.
 * @return stream reference.
 */
GX_NETWORK_EXPORT FIStream& operator>>(FIStream& stream, FGuid& GUID);

/**
 * @brief Serialize FGuid object.
 * @param stream - output stream.
 * @param GUID - object reference.
 * @return stream reference.
 */
GX_NETWORK_EXPORT FOStream& operator<<(FOStream& stream, const FGuid& GUID);

/**
 * @brief FHeader struct.
 */
struct GX_NETWORK_EXPORT FHeader
{
	union 
	{
		struct 
		{
			uint32_t GUID[4];
			uint32_t PacketSize;
		};

		uint8_t Data[sizeof(uint32_t) * 5];
	};	
};

/**
 * @brief Deserialize FHeader object.
 * @param stream - input stream.
 * @param header - object reference.
 * @return stream reference.
 */
GX_NETWORK_EXPORT FIStream& operator>>(FIStream& stream, FHeader& header);

/**
 * @brief Serialize FHeader object.
 * @param stream - output stream.
 * @param header - object reference.
 * @return stream reference.
 */
GX_NETWORK_EXPORT FOStream& operator<<(FOStream& stream, const FHeader& header);

/**
* @brief Deserialize std::string object.
* @param stream - input stream.
* @param string - object reference.
* @return stream reference.
*/
GX_NETWORK_EXPORT FIStream& operator>>(FIStream& stream, std::string& string);

/**
* @brief Serialize std::string object.
* @param stream - output stream.
* @param string - object reference.
* @return stream reference.
*/
GX_NETWORK_EXPORT FOStream& operator<<(FOStream& stream, const std::string& string);

/**
 * @brief Deserialize std::vector object.
 * @param stream - input stream.
 * @param vector - object reference.
 * @return stream reference.
 */
template <class T>
FIStream& operator>>(FIStream& stream, std::vector<T>& vector)
{
	static_assert(std::is_integral<T>::value == true, "Only integral types are accepted.");
	uint32_t  size = 0;
	stream >> size;
	vector.resize(size / sizeof(T));
	memcpy(vector.data(), stream.Read(size), size);
	return stream;
}

/**
 * @brief Serialize std::vector object.
 * @param stream - output stream.
 * @param std::vector - object reference.
 * @return stream reference.
 */
template <class T>
FOStream& operator<<(FOStream& stream, const std::vector<T>& vector)
{
	static_assert(std::is_integral<T>::value == true, "Only integral types are accepted.");
	uint32_t  size = GX_NETWORK_SIZE_T_TO_UINT_32_T(vector.size() * sizeof(T));
	stream << size;
	stream.Write((const uint8_t*)(vector.data()), size);
	return stream;
}

}	
}

