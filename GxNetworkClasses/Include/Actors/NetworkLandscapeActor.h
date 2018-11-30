#pragma once

#include "../NetworkActor.h"

namespace gx {
namespace network {

/**
 * @brief FLandscapeActor class.
 */
class GX_NETWORK_EXPORT FLandscapeActor : public FActor
{

	GX_NETWORK_OBJECT(FLandscapeActor)

public:

	/**
	 * @brief FLandscapeActor super class decl.
	 */
	typedef FActor Super;

	/**
	 * @brief Destructor.
	 */
	virtual ~FLandscapeActor();

	/**
	 * @brief Get heightmap width.
	 * @return heightmap width.
	 */
	uint32_t GetHeightmapSizeX() const;

	/**
	 * @brief Get heightmap height.
	 * @return heightmap height.
	 */
	uint32_t GetHeightmapSizeY() const;

	/**
	 * @brief Get heightmap pixel format.
	 * @return heightmap pixel format.
	 */
	uint32_t GetHeightmapPixelFormat() const;

	/**
	 * @brief Get heightmap data frame.
	 * @return heightmap data frame.
	 */
	const std::vector<uint8_t>& GetHeightmap() const;

	/**
	 * @brief Set heightmap width.
	 * @param heightmapSizeX - heightmap width.
	 */
	void SetHeightmapSizeX(uint32_t heightmapSizeX);

	/**
	 * @brief Set heightmap height.
	 * @param heightmapSizeY - heightmap height.
	 */
	void SetHeightmapSizeY(uint32_t heightmapSizeY);

	/**
	 * @brief Set heightmap data frame.
	 * @param heightmap - heightmap data frame reference.
	 */
	void SetHeightmap(std::vector<uint8_t>&& heightmap);

	/**
	 * @brief Set heightmap pixel format.
	 * @param heightmapPixelFormat - heightmap pixel format.
	 */
	void SetHeightmapPixelFormat(uint32_t heightmapPixelFormat);	

protected:

	GX_NETWORK_PROPERTY(uint32_t, HeightmapSizeX, 0);
	GX_NETWORK_PROPERTY(uint32_t, HeightmapSizeY, 0);
	GX_NETWORK_PROPERTY(uint32_t, HeightmapPixelFormat, 0);
	GX_NETWORK_PROPERTY(std::vector<uint8_t>, Heightmap, {});

};

}
}