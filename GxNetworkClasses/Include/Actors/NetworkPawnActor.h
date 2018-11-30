#pragma once

#include "../NetworkActor.h"

namespace gx {
namespace network {

/**
 * @brief FPawnActor class.
 */
class GX_NETWORK_EXPORT FPawnActor : public FActor
{

	GX_NETWORK_OBJECT(FPawnActor)

public:

	/**
	 * @brief FPawnActor super class decl.
	 */
	typedef FActor Super;

	/**
	 * @brief Destructor.
	 */
	virtual ~FPawnActor();

};

}
}