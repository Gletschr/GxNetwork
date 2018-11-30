#pragma once

#include "NetworkCustomActor.h"

namespace gx {
namespace network {

/**
 * @brief FTreeActor class.
 */
class GX_NETWORK_EXPORT FTreeActor : public FCustomActor
{

	GX_NETWORK_OBJECT(FTreeActor)

public:

	/**
	 * @brief FTreeActor super class decl.
	 */
	typedef FCustomActor Super;

	/**
	 * @brief Destructor.
	 */
	virtual ~FTreeActor();

};

}
}
