#pragma once

#include "NetworkReplicable.h"
#include "NetworkProperty.h"
#include "NetworkFunction.h"
#include "../Common/NetworkTypes.h"

namespace gx {
namespace network {

/**
 * @brief FEngine class forward decl.
 */
class FEngine;

/**
 * @brief FObject class forward decl.
 */
class FObject;

/**
 * @brief FObject shared pointer class decl.
 */
typedef std::shared_ptr<FObject> FObjectPtr;

/**
 * @brief FObject class.
 */
class GX_NETWORK_EXPORT FObject : public FReplicable
{

	/**
	 * @brief GX_NETWORK_OBJECT macro. Should be defined in headers (.h) of all classes derrived from FObject.
	 * @param FClass derrived class name.
	 * @param RemoteRole - object remote role.
	 */
	#define GX_NETWORK_OBJECT(FClass) \
	public: \
		FClass ## (FEngine* engine, const FGuid& GUID, uint16_t role);\
		static  FObjectPtr	Create(FEngine* engine, const FGuid& GUID, uint16_t role) { return FObjectPtr(new FClass(engine, GUID, role)); } \
		static  const char* GetClassNameStatic() { return &(#FClass [1]); } \
		virtual const char* GetClassName() const override { return FClass::GetClassNameStatic(); } \
	private: \
		typedef FClass This;

	/**
	 * @brief GX_NETWORK_OBJECT_IMPL macro. Should be defined in sources (.cpp) of all classes derrived from FObject.
	 * @param FClass derrived class name.
	 */
	#define GX_NETWORK_OBJECT_IMPL(FClass) \
	FClass ## :: ## FClass(FEngine* engine, const FGuid& GUID, uint16_t role) \
		: Super(engine, GUID, role) \
	{ \
	} \
	struct FClass ## StaticRegister \
	{ \
		FClass ## StaticRegister() \
		{ \
			FEngine::RegisterClass(FClass::GetClassNameStatic(), &FClass::Create); \
		} \
	}; \
	static FClass ## StaticRegister _ ## FClass ## StaticRegister;

public:

	/**
	 * @brief FObject::ERole enum.
	 */
	enum ERole
	{
		Undefined		= 0x00,
		Proxy			= 0x01,
		Authority		= 0x02,
		RemoteProxy		= 0x04,
		RemoteAuthority = 0x08,
	};
	
	/**
	 * @brief Constructor.
	 * @param engine - network engine pointer.
	 * @param GUID - object guid.
	 * @param role - object role.
	 */
	FObject(FEngine* engine, const FGuid& GUID, uint16_t role);
	
	/**
	 * @brief Destructor.
	 */
	virtual ~FObject();
	
	/**
	 * @brief Get object network role.
	 * @return object network role.
	 */
	uint16_t GetNetworkRole() const;

	/**
	 * @brief Get object GUID.
	 * @return object GUID reference.
	 */
	const FGuid& GetGUID() const;

	/**
	 * @brief See FReplicable::operator<<(FIStream&).
	 */
	virtual void operator<<(FIStream& stream) override;

	/**
	 * @brief See FReplicable::operator>>(FOStream&).
	 */
	virtual void operator>>(FOStream& stream) const override;
	
	/**
	 * @brief Get object class name.
	 * @return object class name.
	 */
	virtual const char* GetClassName() const = 0;

	/**
	 * @brief Static creator type decl.
	 */
	typedef FObjectPtr (*FCreator)(FEngine*, const FGuid&, uint16_t);

	/**
	 * @brief Get function by name.
	 * @param name - function name.
	 * @return function object on success, nullptr - otherwise.
	 */
	FFunction* GetFunction(const char* name);

	/**
	 * @brief Get property by name.
	 * @param name - property name.
	 * @return property object on success, nullptr - otherwise.
	 */
	FProperty* GetProperty(const char* name);

	/**
	 * @brief Register object function.
	 * @param name - function name.
	 * @param function - function object pointer.
	 * @return true - on success, false - otherwise.
	 */
	bool RegisterFunction(const char* name, FFunction* function);

	/**
	 * @brief Register object property.
	 * @param name - property name.
	 * @param property - property object pointer.
	 * @return true - on success, false - otherwise.
	 */
	bool RegisterProperty(const char* name, FProperty* property);

	/**
	 * @brief Exec function by name.
	 * @param parameters - input parameters.
	 * @return true - on success, false - otherwise.
	 */
	bool ExecFunction(const char* name, const FBuffer& parameters);

	/**
	 * @brief Exec remote function by name.
	 * @param parameters - input parameters.
	 */
	void ExecFunctionRemote(const char* name, const FBuffer& parameters);

private:

	FGuid _GUID;
	uint16_t _role;

	std::map<std::string, FFunction*> _functions;
	std::map<std::string, FProperty*> _properties;

protected:

	FEngine* _engine = nullptr;

};
 
}
}
