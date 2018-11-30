#pragma once

#include "NetworkPawnActor.h"

namespace gx {
namespace network {

/**
 * @brief FCharacterActor class.
 */
class GX_NETWORK_EXPORT FCharacterActor : public FPawnActor
{

	GX_NETWORK_OBJECT(FCharacterActor)

public:

	/**
	 * @brief FCharacterActor super class decl.
	 */
	typedef FPawnActor Super;

	/**
	 * @brief Destructor.
	 */
	virtual ~FCharacterActor();
};

}
}