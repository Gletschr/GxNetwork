#pragma once

#include "../../GxNetwork/Include/Engine/NetworkObject.h"

namespace gx {
namespace network {

/**
 * @brief FScene class.
 */
class GX_NETWORK_EXPORT FScene : public FObject
{

	GX_NETWORK_OBJECT(FScene)

public:

	/**
	 * @brief FActor super class decl.
	 */
	typedef FObject Super;

	/**
	 * @brief Destructor.
	 */
	virtual ~FScene();

};

}
}