#pragma once

#include <assert.h>
#include <memory>
#include <inttypes.h>

#pragma warning(disable: 4251)
#pragma warning(disable: 4005)

#if defined(GX_NETWORK_DLL)
#	define GX_NETWORK_EXPORT __declspec(dllexport)
#elif defined(GX_NETWORK_BUILTIN)
#	define GX_NETWORK_EXPORT
#else
#	define GX_NETWORK_EXPORT __declspec(dllimport)
#endif

#define GX_NETWORK_ASSERT
#define GX_NETWORK_UNUSED(x) (void)(x)
#define GX_NETWORK_SIZE_T_TO_UINT_32_T(s) (uint32_t)(s)
