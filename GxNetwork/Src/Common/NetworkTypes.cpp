#include "../../Include/Common/NetworkTypes.h"

namespace gx {
namespace network {

FVec3f::FVec3f()
	: x(0.0f)
	, y(0.0f)
	, z(0.0f)
{
}

FVec3f::FVec3f(const FVec3f & vec3f)
	: x(vec3f.x)
	, y(vec3f.y)
	, z(vec3f.z)
{
}

FVec3f::FVec3f(float x, float y, float z)
	: x(x)
	, y(y)
	, z(z)
{}

FGuid::FGuid()
	: A(0)
	, B(0)
	, C(0)
	, D(0)
{
}

FGuid::FGuid(const FGuid & guid)
	: A(guid.A)
	, B(guid.B)
	, C(guid.C)
	, D(guid.D)
{
}

FGuid::FGuid(uint32_t a, uint32_t b, uint32_t c, uint32_t d)
	: A(a)
	, B(b)
	, C(c)
	, D(d)
{
}

bool operator==(const FVec3f& a, const FVec3f& b)
{
	return a.x == b.x && a.y == b.y && a.z == b.z;
}

bool operator!=(const FVec3f& a, const FVec3f& b)
{
	return a.x != b.x || a.y != b.y || a.z != b.z;
}

FIStream& operator >> (FIStream& stream, FVec3f& vec3f)
{
	stream >> vec3f.x;
	stream >> vec3f.y;
	stream >> vec3f.z;
	return stream;
}

FOStream& operator<<(FOStream& stream, const FVec3f& vec3f)
{
	stream << vec3f.x;
	stream << vec3f.y;
	stream << vec3f.z;
	return stream;
}

bool operator==(const FGuid& a, const FGuid& b)
{
	return a.A == b.A && a.B == b.B && a.C == b.C && a.D == b.D;
}

bool operator!=(const FGuid& a, const FGuid& b)
{
	return a.A != b.A || a.B != b.B || a.C != b.C || a.D != b.D;
}

FIStream& operator>>(FIStream& stream, FGuid& GUID)
{
	stream >> GUID.A;
	stream >> GUID.B;
	stream >> GUID.C;
	stream >> GUID.D;
	return stream;
}

FOStream& operator<<(FOStream& stream, const FGuid& GUID)
{
	stream << GUID.A;
	stream << GUID.B;
	stream << GUID.C;
	stream << GUID.D;
	return stream;
}

FIStream& operator>>(FIStream& stream, FHeader& header)
{
	stream >> header.GUID[0];
	stream >> header.GUID[1];
	stream >> header.GUID[2];
	stream >> header.GUID[3];
	stream >> header.PacketSize;
	return stream;
}

FOStream& operator<<(FOStream& stream, const FHeader& header)
{
	stream << header.GUID[0];
	stream << header.GUID[1];
	stream << header.GUID[2];
	stream << header.GUID[3];
	stream << header.PacketSize;
	return stream;
}

FIStream& operator>>(FIStream& stream, std::string& string)
{
	static_assert(sizeof(char) == sizeof(uint8_t), "sizeof(char) != sizeof(uint8_t)");
	uint32_t  size = 0;
	stream >> size;
	string = (const char*)(stream.Read(size));
	return stream;
}

FOStream& operator<<(FOStream& stream, const std::string& string)
{
	static_assert(sizeof(char) == sizeof(uint8_t), "sizeof(char) != sizeof(uint8_t)");
	uint32_t  size = GX_NETWORK_SIZE_T_TO_UINT_32_T(string.size() + 1);
	stream << size;
	stream.Write((const uint8_t*)(string.c_str()), size);
	return stream;
}

}
}