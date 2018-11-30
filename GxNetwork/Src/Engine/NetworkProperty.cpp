#include "../../Include/Engine/NetworkProperty.h"
#include "../../Include/Engine/NetworkObject.h"

namespace gx {
namespace network {

FProperty::FProperty(FObject* object, const char* name, FProperty::EType type)
	: _type(type)
{
	object->RegisterProperty(name, this);
}

FProperty::~FProperty()
{

}

FProperty::EType FProperty::GetType() const
{
	return _type;
}

FVectorProperty::FVectorProperty(FObject* object, const char* name, EType elementType)
	: FProperty(object, name, EType::Vector)
	, _elementType(elementType)
{
}

FVectorProperty::~FVectorProperty()
{
}

FProperty::EType FVectorProperty::GetElementType() const
{
	return _elementType;
}

}
}