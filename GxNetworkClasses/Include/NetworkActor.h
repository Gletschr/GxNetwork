#pragma once

#include "../../GxNetwork/Include/Engine/NetworkObject.h"

namespace gx {
namespace network {

/**
 * @brief FActor class.
 */
class GX_NETWORK_EXPORT FActor : public FObject
{

	GX_NETWORK_OBJECT(FActor)

public:

	/**
	 * @brief FActor super class decl.
	 */
	typedef FObject Super;

	/**
	 * @brief Destructor.
	 */
	virtual ~FActor();

	/**
	 * @brief Get actor scale.
	 * @return actor scale.
	 */
	const FVec3f& GetScale() const;

	/**
	 * @brief Get actor location.
	 * @return actor location.
	 */
	const FVec3f& GetLocation() const;

	/**
	 * @brief Set actor scale.
	 * @param scale - actor scale.
	 */
	void SetScale(const FVec3f& scale);

	/**
	 * @brief Set actor location.
	 * @param location - actor location.
	 */
	void SetLocation(const FVec3f& location);

protected:

	GX_NETWORK_PROPERTY(FVec3f, Scale,		{ 0.0f, 0.0f, 0.0f });
	GX_NETWORK_PROPERTY(FVec3f, Location,	{ 0.0f, 0.0f, 0.0f });

};

}
}