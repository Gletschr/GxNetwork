#pragma once

#include "Network.h"

#include <mutex>
#include <vector>

namespace gx {
namespace network {

/**
 * @brief FBuffer class.
 */
class GX_NETWORK_EXPORT FBuffer
{

public:
	
	/**
	 * @brief Constructor.
	 * @param c - buffer capacity.
	 */
	FBuffer(uint32_t c = 0);
	
	/**
	 * @brief Destructor.
	 */
	virtual ~FBuffer();
	
	/**
	 * @brief Lock data.
	 */
	void Lock() const;
	
	/**
	 * @brief Unlock data.
	 */
	void UnLock() const;
	
	/**
	 * @brief Clear data.
	 */
	void Clear();
	
	/**
	 * @brief Resize buffer.
	 * @param size - new size.
	 */
	void Resize(uint32_t size);
	
	/**
	 * @brief Reserve buffer.
	 * @param size - new capacity.
	 */
	void Reserve(uint32_t size);
	
	/**
	 * @brief Get size of buffer.
	 * @return size of buffer.
	 */
	uint32_t Size() const;
	
	/**
	 * @brief Get capacity of buffer.
	 * @return capacity of buffer.
	 */
	uint32_t Capacity() const;
	
	/**
	 * @brief Get buffer data pointer.
	 * @return buffer data pointer.
	 */
	uint8_t* Data();

	/**
	 * @brief Get buffer const data pointer.
	 * @return buffer const data pointer.
	 */
	const uint8_t* Data() const;

	/**
	 * @brief Append data to the end of the buffer.
	 * @param data - data chunk pointer.
	 * @param size - data chunk size.
	 */
	void Append(const uint8_t* data, uint32_t size);

	/**
	 * @brief Insert chunk of data to the given position of the buffer.
	 * @param data - data chunk pointer.
	 * @param size - data chunk size.
	 * @param position - position in buffer.
	 */
	void Insert(const uint8_t* data, uint32_t size, uint32_t position);

private:
	
	mutable std::mutex _mutex;
	std::vector<uint8_t> _data;	

};

/**
 * @brief FBuffer shared pointer class decl.
 */
typedef std::shared_ptr<FBuffer> FBufferPtr;

}	
}

