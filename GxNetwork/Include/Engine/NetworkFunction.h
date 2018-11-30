#pragma once

#include "../../Include/Common/NetworkTypes.h"

#include <type_traits>

namespace gx {
namespace network {

/**
 * @brief FObject class forward decl.
 */
class FObject;

/**
 * @brief FFunction class.
 */
class GX_NETWORK_EXPORT FFunction
{

public:

	/**
	 * @brief Constructor.
	 * @param object - owner object.
	 * @param name - function name.
	 */
	FFunction(FObject* object, const char* name);

	/**
	 * @brief Destructor.
	 */
	virtual ~FFunction();

	/**
	 * @brief Execute function.
	 * @param stream - input parameters stream.
	 */
	virtual void Exec(FIStream& stream) = 0;

protected:

	FObject* _object = nullptr;

protected:

	template <class T>
	struct RemoveTraits
	{
		typedef T Type;
	};

	template <class T>
	struct RemoveTraits <T &>
	{
		typedef T Type;
	};

	template <class T>
	struct RemoveTraits <T *>
	{
		typedef T Type;
	};

	template <class T>
	struct RemoveTraits <const T &>
	{
		typedef T Type;
	};

	template <class T>
	struct RemoveTraits <const T *>
	{
		typedef T Type;
	};

	template <class T>
	struct Packer
	{
		Packer(FOStream& stream, const T& value)
		{
			stream << value;
		}
	};

	template <class... Args>
	void PackArguments(FOStream& stream, Args... args)
	{
		std::tuple<Packer<typename RemoveTraits<Args>::Type>...> packer = 
		std::tuple<Packer<typename RemoveTraits<Args>::Type>...>({ Packer<typename RemoveTraits<Args>::Type>(stream, args)... });
	}

	template <class T>
	typename RemoveTraits<T>::Type UnPackArgument(FIStream& stream)
	{
		typename RemoveTraits<T>::Type argument;
		stream >> argument;
		return argument;	
	}

};

/**
 * @brief Network function macro.
 * @param Name - network function name.
 */
#define GX_NETWORK_FUNCTION(Name, ...) \
	void Name ## (__VA_ARGS__); \
	template <class... Args> \
	class FFunction ## Name ## Template : public FFunction \
	{ \
	public: \
		FFunction ## Name ## Template(This* object, const char* name) \
			: FFunction(object, name) \
		{} \
		virtual ~FFunction ## Name ## Template() override \
		{} \
		virtual void Exec(FIStream& stream) override \
		{ \
			((This *)(_object))-> ## Name ## (UnPackArgument<Args>(stream)...); \
		} \
		void ExecRemote(Args... args) \
		{ \
			FBuffer parameters; \
			FOStream stream(parameters); \
			PackArguments(stream, args...); \
			((This *)(_object))->ExecFunctionRemote(#Name, parameters);\
		} \
	}; \
	FFunction ## Name ## Template<__VA_ARGS__> Function ## Name ## = FFunction ## Name ## Template<__VA_ARGS__>(this, #Name); \
	template <class... Args> \
	void Name ## Remote(const Args&... args) \
	{ \
		Function ## Name ## .ExecRemote(args...); \
	}
}
}
