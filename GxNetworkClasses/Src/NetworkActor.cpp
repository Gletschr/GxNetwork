#include "../Include/NetworkActor.h"
#include "../../GxNetwork/Include/Engine/NetworkEngine.h"

namespace gx {
namespace network {

GX_NETWORK_OBJECT_IMPL(FActor)

FActor::~FActor()
{

}

const FVec3f& FActor::GetScale() const
{
	return this->Scale;
}

const FVec3f& FActor::GetLocation() const
{
	return this->Location;
}

void FActor::SetScale(const FVec3f& scale)
{
	this->Scale = scale;
}

void FActor::SetLocation(const FVec3f& location)
{
	this->Location = location;
}

}
}