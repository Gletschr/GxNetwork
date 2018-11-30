#include "../../Include/Common/NetworkStream.h"

namespace gx {
namespace network {

FStream::~FStream()
{
	
}

uint32_t FStream::Pos() const
{
	return _pos;
}
	
void FStream::SetPos(uint32_t pos)
{
	_pos = pos;
}

FIStream::FIStream(const FBuffer& buffer)
	: FStream()
	, _buffer(buffer)
{
	
}

FIStream::~FIStream()
{
	
}

bool FIStream::IsEOF() const
{
	GX_NETWORK_ASSERT(_pos <= _buffer.Size());
	return _pos == _buffer.Size();
}

const uint8_t* FIStream::Read(uint32_t size)
{
	GX_NETWORK_ASSERT(_pos + size <= _buffer.Size());
	const uint8_t* value = _buffer.Data() + _pos;
	_pos += size;
	return value;
}

FOStream::FOStream(FBuffer& buffer)
	: FStream()
	, _buffer(buffer)
{
	_pos = _buffer.Size();
}

FOStream::~FOStream()
{
	
}

void FOStream::Write(const uint8_t* data, uint32_t size)
{
	_buffer.Insert(data, size, _pos);
	_pos += size;
}

}
}