#include "../../Include/Common/NetworkBuffer.h"

#include <iterator>

namespace gx {
namespace network {

FBuffer::FBuffer(uint32_t capacity)
{
	_data.reserve(capacity);
}

FBuffer::~FBuffer()
{
	
}

void FBuffer::Lock() const
{
	_mutex.lock();
}

void FBuffer::UnLock() const
{
	_mutex.unlock();
}

void FBuffer::Clear()
{
	_data.clear();
}

void FBuffer::Resize(uint32_t size)
{
	_data.resize(size);
}

void FBuffer::Reserve(uint32_t size)
{
	_data.reserve(size);
}

uint32_t FBuffer::Size() const
{
	return GX_NETWORK_SIZE_T_TO_UINT_32_T(_data.size());
}

uint32_t FBuffer::Capacity() const
{
	return GX_NETWORK_SIZE_T_TO_UINT_32_T(_data.capacity());
}

uint8_t* FBuffer::Data()
{
	return _data.data();
}

const uint8_t* FBuffer::Data() const
{
	return _data.data();
}

void FBuffer::Append(const uint8_t* data, uint32_t size)
{
	std::copy(data, data + size, std::back_inserter(_data));
}

void FBuffer::Insert(const uint8_t* data, uint32_t size, uint32_t position)
{
	_data.insert(_data.begin() + position, data, data + size);
}

}
}