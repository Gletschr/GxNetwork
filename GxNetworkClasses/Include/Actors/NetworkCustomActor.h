#pragma once

#include "../NetworkActor.h"

namespace gx {
namespace network {

/**
 * @brief FCustomActor class.
 */
class GX_NETWORK_EXPORT FCustomActor : public FActor
{

	GX_NETWORK_OBJECT(FCustomActor)

public:

	/**
	 * @brief FCustomActor super class decl.
	 */
	typedef FActor Super;

	/**
	 * @brief Destructor.
	 */
	virtual ~FCustomActor();
	
	/**
	 * @brief Get custom actor type name.
	 * @return custom actor type name.
	 */
	const std::string& GetType() const;

	/**
	 * @brief Set custom actor type name.
	 * @param custom actor - custom actor type name.
	 */
	void SetType(const std::string& type);

protected:

	GX_NETWORK_PROPERTY(std::string, Type);

};

}
}
