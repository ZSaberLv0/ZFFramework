#ifndef _ZFI_ZFImpl_h_
#define _ZFI_ZFImpl_h_

#include "ZFCore/ZFCoreDef.h"

#ifdef __APPLE__
    #include "TargetConditionals.h"
#endif

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief force to use Windows env
 *
 * you can add it to precompiler to make it active
 */
#ifndef ZF_ENV_FORCE_sys_Windows
    #define ZF_ENV_FORCE_sys_Windows 0
#else
    #undef ZF_ENV_FORCE_sys_Windows
    #define ZF_ENV_FORCE_sys_Windows 1
#endif

/**
 * @brief force not to use Windows env
 *
 * you can add it to precompiler to make it active
 */
#ifndef ZF_ENV_FORCE_not_sys_Windows
    #define ZF_ENV_FORCE_not_sys_Windows 0
#else
    #undef ZF_ENV_FORCE_not_sys_Windows
    #define ZF_ENV_FORCE_not_sys_Windows 1
#endif

/**
 * @brief force to use Windows CE env
 *
 * you can add it to precompiler to make it active
 */
#ifndef ZF_ENV_FORCE_sys_WindowsCE
    #define ZF_ENV_FORCE_sys_WindowsCE 0
#else
    #undef ZF_ENV_FORCE_sys_WindowsCE
    #define ZF_ENV_FORCE_sys_WindowsCE 1
#endif

/**
 * @brief force not to use Windows CE env
 *
 * you can add it to precompiler to make it active
 */
#ifndef ZF_ENV_FORCE_not_sys_WindowsCE
    #define ZF_ENV_FORCE_not_sys_WindowsCE 0
#else
    #undef ZF_ENV_FORCE_not_sys_WindowsCE
    #define ZF_ENV_FORCE_not_sys_WindowsCE 1
#endif

/**
 * @brief force to use Posix env
 *
 * you can add it to precompiler to make it active
 */
#ifndef ZF_ENV_FORCE_sys_Posix
    #define ZF_ENV_FORCE_sys_Posix 0
#else
    #undef ZF_ENV_FORCE_sys_Posix
    #define ZF_ENV_FORCE_sys_Posix 1
#endif

/**
 * @brief force not to use Posix env
 *
 * you can add it to precompiler to make it active
 */
#ifndef ZF_ENV_FORCE_not_sys_Posix
    #define ZF_ENV_FORCE_not_sys_Posix 0
#else
    #undef ZF_ENV_FORCE_not_sys_Posix
    #define ZF_ENV_FORCE_not_sys_Posix 1
#endif

/**
 * @brief force to use Linux env
 *
 * you can add it to precompiler to make it active
 */
#ifndef ZF_ENV_FORCE_sys_Linux
    #define ZF_ENV_FORCE_sys_Linux 0
#else
    #undef ZF_ENV_FORCE_sys_Linux
    #define ZF_ENV_FORCE_sys_Linux 1
#endif

/**
 * @brief force not to use Linux env
 *
 * you can add it to precompiler to make it active
 */
#ifndef ZF_ENV_FORCE_not_sys_Linux
    #define ZF_ENV_FORCE_not_sys_Linux 0
#else
    #undef ZF_ENV_FORCE_not_sys_Linux
    #define ZF_ENV_FORCE_not_sys_Linux 1
#endif

/**
 * @brief force to use MacOS env
 *
 * you can add it to precompiler to make it active
 */
#ifndef ZF_ENV_FORCE_sys_MacOS
    #define ZF_ENV_FORCE_sys_MacOS 0
#else
    #undef ZF_ENV_FORCE_sys_MacOS
    #define ZF_ENV_FORCE_sys_MacOS 1
#endif

/**
 * @brief force not to use MacOS env
 *
 * you can add it to precompiler to make it active
 */
#ifndef ZF_ENV_FORCE_not_sys_MacOS
    #define ZF_ENV_FORCE_not_sys_MacOS 0
#else
    #undef ZF_ENV_FORCE_not_sys_MacOS
    #define ZF_ENV_FORCE_not_sys_MacOS 1
#endif

/**
 * @brief force to use iOS env
 *
 * you can add it to precompiler to make it active
 */
#ifndef ZF_ENV_FORCE_sys_iOS
    #define ZF_ENV_FORCE_sys_iOS 0
#else
    #undef ZF_ENV_FORCE_sys_iOS
    #define ZF_ENV_FORCE_sys_iOS 1
#endif

/**
 * @brief force not to use iOS env
 *
 * you can add it to precompiler to make it active
 */
#ifndef ZF_ENV_FORCE_not_sys_iOS
    #define ZF_ENV_FORCE_not_sys_iOS 0
#else
    #undef ZF_ENV_FORCE_not_sys_iOS
    #define ZF_ENV_FORCE_not_sys_iOS 1
#endif

/**
 * @brief force to use Android env
 *
 * you can add it to precompiler to make it active
 */
#ifndef ZF_ENV_FORCE_sys_Android
    #define ZF_ENV_FORCE_sys_Android 0
#else
    #undef ZF_ENV_FORCE_sys_Android
    #define ZF_ENV_FORCE_sys_Android 1
#endif

/**
 * @brief force not to use Android env
 *
 * you can add it to precompiler to make it active
 */
#ifndef ZF_ENV_FORCE_not_sys_Android
    #define ZF_ENV_FORCE_not_sys_Android 0
#else
    #undef ZF_ENV_FORCE_not_sys_Android
    #define ZF_ENV_FORCE_not_sys_Android 1
#endif

/**
 * @brief force to use Qt env
 *
 * you can add it to precompiler to make it active
 */
#ifndef ZF_ENV_FORCE_sys_Qt
    #define ZF_ENV_FORCE_sys_Qt 0
#else
    #undef ZF_ENV_FORCE_sys_Qt
    #define ZF_ENV_FORCE_sys_Qt 1
#endif

/**
 * @brief force not to use Qt env
 *
 * you can add it to precompiler to make it active
 */
#ifndef ZF_ENV_FORCE_not_sys_Qt
    #define ZF_ENV_FORCE_not_sys_Qt 0
#else
    #undef ZF_ENV_FORCE_not_sys_Qt
    #define ZF_ENV_FORCE_not_sys_Qt 1
#endif

// ============================================================
#undef _ZFP_ZF_ENV_sys_defined

/**
 * @brief true if under Windows
 */
#if !ZF_ENV_FORCE_not_sys_Windows && (( \
            defined(_WIN32) || defined(WIN32) \
        ) || ZF_ENV_FORCE_sys_Windows || ZF_ENV_FORCE_sys_WindowsCE)
    #define ZF_ENV_sys_Windows 1
    #undef _ZFP_ZF_ENV_sys_defined
    #define _ZFP_ZF_ENV_sys_defined 1
#else
    #define ZF_ENV_sys_Windows 0
#endif
/**
 * @brief true if under Windows CE
 *
 * ZF_ENV_sys_Windows is also set to zftrue
 */
#if !ZF_ENV_FORCE_not_sys_WindowsCE && ZF_ENV_sys_Windows && (( \
            defined(WINCE) \
        ) || ZF_ENV_FORCE_sys_WindowsCE)
    #define ZF_ENV_sys_WindowsCE 1
    #undef _ZFP_ZF_ENV_sys_defined
    #define _ZFP_ZF_ENV_sys_defined 1
#else
    #define ZF_ENV_sys_WindowsCE 0
#endif

/**
 * @brief true if under Posix
 */
#if !ZF_ENV_FORCE_not_sys_Posix && (( \
            defined(__unix__) || defined(__APPLE__) || defined(__linux) \
        ) || ZF_ENV_FORCE_sys_Posix)
    #define ZF_ENV_sys_Posix 1
    #undef _ZFP_ZF_ENV_sys_defined
    #define _ZFP_ZF_ENV_sys_defined 1
#else
    #define ZF_ENV_sys_Posix 0
#endif

/**
 * @brief true if under Linux
 */
#if !ZF_ENV_FORCE_not_sys_Linux && (( \
            defined(__linux) \
        ) || ZF_ENV_FORCE_sys_Linux)
    #define ZF_ENV_sys_Linux 1
    #undef _ZFP_ZF_ENV_sys_defined
    #define _ZFP_ZF_ENV_sys_defined 1
#else
    #define ZF_ENV_sys_Linux 0
#endif

/**
 * @brief true if under MacOS
 */
#if !ZF_ENV_FORCE_not_sys_MacOS && (( \
            defined(__APPLE__) && TARGET_OS_MAC \
        ) || ZF_ENV_FORCE_sys_MacOS)
    #define ZF_ENV_sys_MacOS 1
    #undef _ZFP_ZF_ENV_sys_defined
    #define _ZFP_ZF_ENV_sys_defined 1
#else
    #define ZF_ENV_sys_MacOS 0
#endif

/**
 * @brief true if under iOS
 */
#if !ZF_ENV_FORCE_not_sys_iOS && (( \
            defined(__APPLE__) && (TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE) \
        ) || ZF_ENV_FORCE_sys_iOS)
    #define ZF_ENV_sys_iOS 1
    #undef _ZFP_ZF_ENV_sys_defined
    #define _ZFP_ZF_ENV_sys_defined 1
#else
    #define ZF_ENV_sys_iOS 0
#endif

/**
 * @brief true if under Android
 */
#if !ZF_ENV_FORCE_sys_Android && (( \
            defined(ANDROID) || defined(__ANDROID__) \
        ) || ZF_ENV_FORCE_sys_Android)
    #define ZF_ENV_sys_Android 1
    #undef _ZFP_ZF_ENV_sys_defined
    #define _ZFP_ZF_ENV_sys_defined 1
#else
    #define ZF_ENV_sys_Android 0
#endif

/**
 * @brief true if under QT
 */
#if !ZF_ENV_FORCE_sys_Qt && (( \
            defined(QT_VERSION) || defined(QT_CORE_LIB) \
        ) || ZF_ENV_FORCE_sys_Qt)
    #define ZF_ENV_sys_Qt 1
    #undef _ZFP_ZF_ENV_sys_defined
    #define _ZFP_ZF_ENV_sys_defined 1
#else
    #define ZF_ENV_sys_Qt 0
#endif

/**
 * @brief true if under unknown OS
 *
 * for most case, you can fall back to Posix if unknown
 */
#if !_ZFP_ZF_ENV_sys_defined
    #define ZF_ENV_sys_unknown 1
#else
    #define ZF_ENV_sys_unknown 0
#endif

#undef _ZFP_ZF_ENV_sys_defined

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFImpl_h_

