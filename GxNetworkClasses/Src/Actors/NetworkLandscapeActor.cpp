#include "../../Include/Actors/NetworkLandscapeActor.h"
#include "../../../GxNetwork/Include/Engine/NetworkEngine.h"

namespace gx {
namespace network {

GX_NETWORK_OBJECT_IMPL(FLandscapeActor)

FLandscapeActor::~FLandscapeActor()
{

}

uint32_t FLandscapeActor::GetHeightmapSizeX() const
{
	return this->HeightmapSizeX;
}

uint32_t FLandscapeActor::GetHeightmapSizeY() const
{
	return this->HeightmapSizeY;
}

uint32_t FLandscapeActor::GetHeightmapPixelFormat() const
{
	return this->HeightmapPixelFormat;
}

const std::vector<uint8_t>& FLandscapeActor::GetHeightmap() const
{
	return this->Heightmap;
}

void FLandscapeActor::SetHeightmapSizeX(uint32_t heightmapSizeX)
{
	this->HeightmapSizeX = heightmapSizeX;
}

void FLandscapeActor::SetHeightmapSizeY(uint32_t heightmapSizeY)
{
	this->HeightmapSizeY = heightmapSizeY;
}

void FLandscapeActor::SetHeightmap(std::vector<uint8_t>&& heightmap)
{
	this->Heightmap = heightmap;
}

void FLandscapeActor::SetHeightmapPixelFormat(uint32_t heightmapPixelFormat)
{
	this->HeightmapPixelFormat = heightmapPixelFormat;
}

}
}