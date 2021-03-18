/**
 * @file ZFCoreTypeDef_CorePrimitiveType.h
 * @brief types for ZFFramework
 */

#ifndef _ZFI_ZFCoreTypeDef_CorePrimitiveType_h_
#define _ZFI_ZFCoreTypeDef_CorePrimitiveType_h_

#include "ZFCoreEnvDef.h"
#include <stddef.h>

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// must ensure these types differs from each other
/** @cond ZFPrivateDoc */
ZF_ENV_SENSITIVE("must ensure these types differs from each other")

#ifndef _ZFT_zftrue
    #define _ZFT_t_zftrue true
#else
    #define _ZFT_t_zftrue _ZFT_zftrue
#endif

#ifndef _ZFT_zffalse
    #define _ZFT_t_zffalse false
#else
    #define _ZFT_t_zffalse _ZFT_zffalse
#endif

#ifndef _ZFT_zfbool
    typedef bool _ZFT_t_zfbool;
#else
    typedef _ZFT_zfbool _ZFT_t_zfbool;
#endif

#ifndef _ZFT_zfchar
    typedef char _ZFT_t_zfchar;
#else
    typedef _ZFT_zfchar _ZFT_t_zfchar;
#endif

#ifndef _ZFT_zfbyte
    typedef unsigned char _ZFT_t_zfbyte;
#else
    typedef _ZFT_zfbyte _ZFT_t_zfbyte;
#endif

#ifndef _ZFT_zfint
    typedef int _ZFT_t_zfint;
#else
    typedef _ZFT_zfint _ZFT_t_zfint;
#endif

#ifndef _ZFT_zfuint
    typedef unsigned int _ZFT_t_zfuint;
#else
    typedef _ZFT_zfuint _ZFT_t_zfuint;
#endif

#ifndef _ZFT_zfindex
    typedef unsigned long int _ZFT_t_zfindex;
#else
    typedef _ZFT_zfindex _ZFT_t_zfindex;
#endif

#ifndef _ZFT_zffloat
    typedef float _ZFT_t_zffloat;
#else
    typedef _ZFT_zffloat _ZFT_t_zffloat;
#endif

/** @endcond */
// must ensure these types differs from each other
// ============================================================

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreTypeDef_CorePrimitiveType_h_

