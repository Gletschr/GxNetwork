#include "../../Include/Actors/NetworkCustomActor.h"
#include "../../../GxNetwork/Include/Engine/NetworkEngine.h"

namespace gx {
namespace network {

GX_NETWORK_OBJECT_IMPL(FCustomActor)

FCustomActor::~FCustomActor()
{

}

const std::string& FCustomActor::GetType() const
{
	return this->Type;
}

void FCustomActor::SetType(const std::string& type)
{
	this->Type = type;
}

}
}