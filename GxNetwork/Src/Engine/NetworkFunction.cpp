#include "../../Include/Engine/NetworkFunction.h"
#include "../../Include/Engine/NetworkObject.h"

namespace gx {
namespace network {

FFunction::FFunction(FObject* object, const char *name)
	: _object(object)
{
	GX_NETWORK_ASSERT(_object->RegisterFunction(name, this));
}

FFunction::~FFunction()
{
}

}
}
