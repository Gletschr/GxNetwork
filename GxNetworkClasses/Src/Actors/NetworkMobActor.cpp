#include "../../Include/Actors/NetworkMobActor.h"
#include "../../../GxNetwork/Include/Engine/NetworkEngine.h"

namespace gx {
namespace network {

GX_NETWORK_OBJECT_IMPL(FMobActor)

FMobActor::~FMobActor()
{

}

float FMobActor::GetVelocity() const
{
	return this->Velocity;
}

const FVec3f& FMobActor::GetDirection() const
{
	return this->Direction;
}

void FMobActor::SetVelocity(float velocity)
{
	this->Velocity = velocity;
}

void FMobActor::SetDirection(const FVec3f& direction)
{
	this->Direction = direction;
}

}
}