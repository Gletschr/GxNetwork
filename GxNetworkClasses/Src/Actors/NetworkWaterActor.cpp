#include "../../Include/Actors/NetworkWaterActor.h"
#include "../../../GxNetwork/Include/Engine/NetworkEngine.h"

namespace gx {
namespace network {

GX_NETWORK_OBJECT_IMPL(FWaterActor)

FWaterActor::~FWaterActor()
{

}

float FWaterActor::GetWaterHeight() const
{
	return this->WaterHeight;
}

uint32_t FWaterActor::GetHeightmapSizeX() const
{
	return this->HeightmapSizeX;
}

uint32_t FWaterActor::GetHeightmapSizeY() const
{
	return this->HeightmapSizeY;
}

void FWaterActor::SetWaterHeight(float waterHeight)
{
	this->WaterHeight = waterHeight;
}

void FWaterActor::SetHeightmapSizeX(uint32_t heightmapSizeX)
{
	this->HeightmapSizeX = heightmapSizeX;
}

void FWaterActor::SetHeightmapSizeY(uint32_t heightmapSizeY)
{
	this->HeightmapSizeY = heightmapSizeY;
}

}
}