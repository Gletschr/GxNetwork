#pragma once

#include "../NetworkActor.h"

namespace gx {
namespace network {

/**
 * @brief FWaterActor class.
 */
class GX_NETWORK_EXPORT FWaterActor : public FActor
{

	GX_NETWORK_OBJECT(FWaterActor)

public:

	/**
	 * @brief FWaterActor super class decl.
	 */
	typedef FActor Super;

	/**
	 * @brief Destructor.
	 */
	virtual ~FWaterActor();

	/**
	 * @brief Get water height.
	 * @return water height.
	 */
	float GetWaterHeight() const;

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
	 * @brief Set water height.
	 * @param waterHeight - water height.
	 */
	void SetWaterHeight(float waterHeight);

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

protected:

	GX_NETWORK_PROPERTY(float, WaterHeight, 0.0f);

	GX_NETWORK_PROPERTY(uint32_t, HeightmapSizeX, 0);
	GX_NETWORK_PROPERTY(uint32_t, HeightmapSizeY, 0);	

};

}
}
