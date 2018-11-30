#pragma once

#include "NetworkBuffer.h"

namespace gx {
namespace network {

/**
 * @brief FStream class.
 */
class GX_NETWORK_EXPORT FStream
{

public:
	
	/**
	 * @brief Destructor.
	 */
	virtual ~FStream();
	
	/**
	 * @brief Get stream position.
	 * @return stream position.
	 */
	uint32_t Pos() const;

	/**
	 * @brief Set stream position.
	 * @param pos - new stream position.
	 */
	void SetPos(uint32_t pos);
	
protected:

	uint32_t _pos = 0;

};

/**
 * @brief FIStream class.
 */
class GX_NETWORK_EXPORT FIStream : public FStream
{

public:
	
	/**
	 * @brief Constructor.
	 * @param buffer - data storage.
	 */
	FIStream(const FBuffer& buffer);
	
	/**
	 * @brief Destructor.
	 */
	virtual ~FIStream();
	
	/**
	 * @brief Check EOF stream state.
	 * @return true if stream is in EOF state, false - otherwise.
	 */
	bool IsEOF() const;
	
	/**
	 * @brief Read chunk of data.
	 * @param size - chunk size.
	 * @return data const pointer.
	 */
	const uint8_t* Read(uint32_t size);
	
	/**
	 * @brief Read data into value.
	 * @param value - value to read into.
	 * @return stream reference.
	 */
	template <class T>
	FIStream& operator>>(T& value)
	{
		value = *(const T *)(Read(sizeof(T)));
		return  *this;
	}
	
private:
	
	const FBuffer& _buffer;

};

/**
 * @brief FOStream class.
 */
class GX_NETWORK_EXPORT FOStream : public FStream
{

public:
	
	/**
	 * @brief Constructor.
	 * @param buffer - data storage.
	 */
	FOStream(FBuffer& buffer);
	
	/**
	 * @brief Destructor.
	 */
	virtual ~FOStream();
	
	/**
	 * @brief Write chunk of data.
	 * @param size - chunk size.
	 * @param data - data const pointer.
	 */
	void Write(const uint8_t* data, uint32_t size);
	
	/**
	 * @brief Read data into value.
	 * @param value - value to read into.
	 * @return stream reference.
	 */
	template <class T>
	FOStream& operator<<(const T& value)
	{
		Write((const uint8_t*)(&value), sizeof(T));
		return *this;
	}
	
private:

	FBuffer& _buffer;
	
};
	
}
}