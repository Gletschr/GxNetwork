#pragma once

#include "NetworkCustomActor.h"

namespace gx {
namespace network {

/**
 * @brief FMobActor class.
 */
class GX_NETWORK_EXPORT FMobActor : public FCustomActor
{

	GX_NETWORK_OBJECT(FMobActor)

public:

	/**
	 * @brief FMobActor super class decl.
	 */
	typedef FCustomActor Super;

	/**
	 * @brief Destructor.
	 */
	virtual ~FMobActor();

	/**
	 * @brief Get mob velocity.
	 * @return mob velocity.
	 */
	float GetVelocity() const;

	/**
	 * @brief Get mob direction.
	 * @return mob direction.
	 */
	const FVec3f& GetDirection() const;

	/**
	 * @brief Set mob velocity.
	 * @param velocity - mob velocity.
	 */
	void SetVelocity(float velocity);

	/**
	 * @brief Set mob direction.
	 * @return direction - mob direction.
	 */
	void SetDirection(const FVec3f &direction);

protected:

	GX_NETWORK_PROPERTY(float, Velocity, 0.0f);
	GX_NETWORK_PROPERTY(FVec3f, Direction, { 0.0f, 0.0f, 0.0f });

};

}
}
