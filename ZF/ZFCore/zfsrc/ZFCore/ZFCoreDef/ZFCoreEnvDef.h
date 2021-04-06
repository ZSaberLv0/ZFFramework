/**
 * @file ZFCoreEnvDef.h
 * @brief used to define the env used by ZFFramework
 */
#ifndef _ZFI_ZFCoreEnvDef_h_
#define _ZFI_ZFCoreEnvDef_h_

#include "ZFCoreEnvConfig.h"

// ============================================================
/**
 * @brief begin namespace
 */
#define ZF_NAMESPACE_BEGIN(NameSpace) \
    /** @brief \n */ \
    namespace NameSpace { \
        extern const char *_ZFP_ZF_NAMESPACE_NOT_REGISTERED(void);

/**
 * @brief end namespace
 */
#define ZF_NAMESPACE_END(NameSpace) \
    }
/**
 * @brief end namespace
 */
#define ZF_NAMESPACE_END_WITH_REGISTER(NameSpace, ParentNameSpace) \
    ZF_NAMESPACE_REGISTER(NameSpace, ParentNameSpace) \
    ZF_NAMESPACE_END(NameSpace)

/**
 * @brief use namespace
 */
#define ZF_NAMESPACE_USE(NameSpace) \
    using namespace NameSpace;

/**
 * @brief global namespace id for impl
 */
#define ZF_NAMESPACE_GLOBAL_ID ZF
/**
 * @brief global namespace id for impl
 */
#define ZF_NAMESPACE_GLOBAL_NAME ZFM_TOSTRING(ZF_NAMESPACE_GLOBAL_ID)
/**
 * @brief abbreviation for global namespace id for impl
 */
#define ZF_NAMESPACE_GLOBAL_ABBR_ID zf
/**
 * @brief global namespace id for impl
 */
#define ZF_NAMESPACE_GLOBAL_ABBR_NAME ZFM_TOSTRING(ZF_NAMESPACE_GLOBAL_ABBR_ID)
/**
 * @def ZF_NAMESPACE_GLOBAL
 * @brief namespace of ZFFramework
 * @def ZF_NAMESPACE_GLOBAL_BEGIN
 * @brief begin namespace ZFFramework
 * @def ZF_NAMESPACE_GLOBAL_END
 * @brief end namespace ZFFramework
 * @def ZF_NAMESPACE_GLOBAL_USE
 * @brief use namespace ZFFramework
 */
#if !ZF_ENV_NAMESPACE_GLOBAL_DISABLE
    #define ZF_NAMESPACE_GLOBAL ZF_NAMESPACE_GLOBAL_ID
    #define ZF_NAMESPACE_GLOBAL_BEGIN ZF_NAMESPACE_BEGIN(ZF_NAMESPACE_GLOBAL)
    #define ZF_NAMESPACE_GLOBAL_END ZF_NAMESPACE_END(ZF_NAMESPACE_GLOBAL)
    #define ZF_NAMESPACE_GLOBAL_USE ZF_NAMESPACE_USE(ZF_NAMESPACE_GLOBAL)
#else
    #define ZF_NAMESPACE_GLOBAL
    #define ZF_NAMESPACE_GLOBAL_BEGIN
    #define ZF_NAMESPACE_GLOBAL_END
    #define ZF_NAMESPACE_GLOBAL_USE
    extern const char *_ZFP_ZF_NAMESPACE_NOT_REGISTERED(void);
#endif

ZF_NAMESPACE_GLOBAL_BEGIN
ZF_NAMESPACE_GLOBAL_END
ZF_NAMESPACE_GLOBAL_USE

// ============================================================

ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief dummy macro to show the code is env sensitive
 */
#define ZF_ENV_SENSITIVE(hint)

// ============================================================
/**
 * @brief true to enable lambda (which require C++11), auto detect by default
 *
 * add -DZF_ENV_LAMBDA=1 to compiler to override
 */
#ifndef ZF_ENV_LAMBDA
    #if defined(__cplusplus) && (__cplusplus >= 201103L)
        #define ZF_ENV_LAMBDA 1
    #else
        #define ZF_ENV_LAMBDA 0
    #endif
#endif

// ============================================================
/**
 * @brief used to export symbols
 *
 * for example, add -DZF_ENV_EXPORT=__declspec(dllexport)
 * to export symbols under Windows
 */
#ifndef ZF_ENV_EXPORT
    #define ZF_ENV_EXPORT
#endif

// ============================================================
/**
 * @brief whether in debug mode
 *
 * add -DZF_ENV_DEBUG=1 to compiler to make this macro take effect
 */
#ifndef ZF_ENV_DEBUG
    #if defined(DEBUG) || defined(_DEBUG)
        #define ZF_ENV_DEBUG 1
    #else
        #define ZF_ENV_DEBUG 0
    #endif
#endif

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFCoreEnvDef_h_

