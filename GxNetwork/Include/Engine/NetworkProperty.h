#pragma once

#include "../../Include/Common/NetworkTypes.h"

namespace gx {
namespace network {

/**
 * @brief FObject class forward decl.
 */
class FObject;

/**
 * @brief FProperty class.
 */
class GX_NETWORK_EXPORT FProperty
{

public:

	/**
	 * @brief EType enum.
	 */
	enum class EType : uint8_t
	{
		Int8		= 0,	//<! int8_t
		UInt8		= 1,	//<! uint8_t
		Int16		= 2,	//<! int16_t
		UInt16		= 3,	//<! uint16_t
		Int32		= 4,	//<! int32_t
		UInt32		= 5,	//<! uint32_t
		Float		= 6,	//<! float
		Double		= 7,	//<! double
		String		= 8,	//<! std::string
		Vector		= 9,	//<! std::vector<T>
		Vec3f		= 10,	//<! gx::network::FVec3f
		GUID		= 11,	//<! gx::network::FGuid
	};

	/**
	 * @brief Constructor.
	 * @param object - owner object.
	 * @param name - property name.
	 * @param type - property type.
	 */
	FProperty(FObject* object, const char *name, EType type);

	/**
	 * @brief Destructor.
	 */
	virtual ~FProperty();

	/**
	 * @brief Get property type.
	 * @return property type.
	 */
	FProperty::EType GetType() const;

	/**
	 * @brief Deserialize property.
	 * @param stream - input stream.
	 */
	virtual void operator<<(FIStream& stream) = 0;

	/**
	 * @brief Serialize property.
	 * @param stream - output stream.
	 */
	virtual void operator>>(FOStream& stream) const = 0;

private:

	EType _type;

protected:

	template <class T>
	struct FTypeToPropertyType
	{
		enum { Defined = 0 };
		static FProperty::EType Type()
		{
			static_assert(FTypeToPropertyType<T>::Defined, "FTypeToPropertyType<T> struct is not defined for this type.");
			return T();
		}
	};

	template <>
	struct FTypeToPropertyType <int8_t>
	{
		static FProperty::EType Type() { return EType::Int8; }
	};

	template <>
	struct FTypeToPropertyType <uint8_t>
	{
		static FProperty::EType Type() { return EType::UInt8; }
	};

	template <>
	struct FTypeToPropertyType <int16_t>
	{
		static FProperty::EType Type() { return EType::Int16; }
	};

	template <>
	struct FTypeToPropertyType <uint16_t>
	{
		static FProperty::EType Type() { return EType::UInt16; }
	};

	template <>
	struct FTypeToPropertyType <int32_t>
	{
		static FProperty::EType Type() { return EType::Int32; }
	};

	template <>
	struct FTypeToPropertyType <uint32_t>
	{
		static FProperty::EType Type() { return EType::UInt32; }
	};

	template <>
	struct FTypeToPropertyType <float>
	{
		static FProperty::EType Type() { return EType::Float; }
	};

	template <>
	struct FTypeToPropertyType <double>
	{
		static FProperty::EType Type() { return EType::Double; }
	};

	template <>
	struct FTypeToPropertyType <std::string>
	{
		static FProperty::EType Type() { return EType::String; }
	};

	template <class T>
	struct FTypeToPropertyType <std::vector<T>>
	{
		static FProperty::EType Type() { return EType::Vector; }
	};

	template <>
	struct FTypeToPropertyType <FVec3f>
	{
		static FProperty::EType Type() { return EType::Vec3f; }
	};

	template <>
	struct FTypeToPropertyType <FGuid>
	{
		static FProperty::EType Type() { return EType::GUID; }
	};

};

/**
 * @brief FVectorProperty class.
 */
class GX_NETWORK_EXPORT FVectorProperty : public FProperty
{

public:

	/**
	 * @brief Constructor.
	 * @param object - owner object.
	 * @param name - property name.
	 * @param elementType -  vector property element type.
	 */
	FVectorProperty(FObject* object, const char* name, EType elementType);

	/**
	 * @brief Destructor.
	 */
	virtual ~FVectorProperty();

	/**
	 * @brief Get vector property type element type.
	 * @return vector property type element type.
	 */
	FProperty::EType GetElementType() const;

private:

	EType _elementType;

};

/**
 * @brief FPropertyTemplate class.
 */
template <class T>
class FPropertyTemplate : public FProperty
{

public:

	/**
	 * @brief Constructor.
	 * @param object - owner object.
	 * @param name - property name.
	 * @param target - target property object reference.
	 */
	FPropertyTemplate(FObject* object, const char* name, T& target)
		: FProperty(object, name, FProperty::FTypeToPropertyType<T>::Type())
		, _target(target)
	{
	}

	/**
	 * @brief Destructor.
	 */
	virtual ~FPropertyTemplate()
	{
	}

	/**
	 * @brief See FProperty::operator<<().
	 */
	virtual void operator<<(FIStream& stream) override
	{
		stream >> _target;
	}

	/**
	 * @brief See FProperty::operator>>().
	 */
	virtual void operator>>(FOStream& stream) const override
	{
		stream << _target;
	}

private:

	T& _target;

};

/**
 * @brief FPropertyTemplate class.
 */
template <class T>
class FPropertyTemplate <std::vector<T>> : public FVectorProperty
{

public:

	/**
	 * @brief Constructor.
	 * @param object - owner object.
	 * @param name - property name.
	 * @param target - target property object reference.
	 */
	FPropertyTemplate(FObject* object, const char* name, std::vector<T>& target)
		: FVectorProperty(object, name, FProperty::FTypeToPropertyType<T>::Type())
		, _target(target)
	{
	}

	/**
	 * @brief Destructor.
	 */
	virtual ~FPropertyTemplate()
	{
	}

	/**
	 * @brief See FProperty::operator<<().
	 */
	virtual void operator<<(FIStream& stream) override
	{
		stream >> _target;
	}

	/**
	 * @brief See FProperty::operator>>().
	 */
	virtual void operator>>(FOStream& stream) const override
	{
		stream << _target;
	}

private:

	std::vector<T>& _target;

};

/**
 * @brief Network function macro.
 * @param T - network property type.
 * @param Name - network property name.
 */
#define GX_NETWORK_PROPERTY(T, Name, ...) \
	T Name = T(__VA_ARGS__); \
	FPropertyTemplate<T> Property ## Name ## = FPropertyTemplate<T>(this, #Name, this-> ## Name ## );

}
}
