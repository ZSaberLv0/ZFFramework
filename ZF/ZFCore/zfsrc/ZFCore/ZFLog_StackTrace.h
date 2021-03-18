/**
 * @file ZFLog_StackTrace.h
 * @brief utility to log stack
 */

#ifndef _ZFI_ZFLog_StackTrace_h_
#define _ZFI_ZFLog_StackTrace_h_

#include "ZFLog.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief true if #zfLogStackTrace and #zfLogCallerInfo is available
 */
ZFMETHOD_FUNC_DECLARE_0(zfbool, zfLogStackTraceAvailable)

/**
 * @brief get the function call stack
 *
 * output may be different depending on the OS and implementation
 */
ZFMETHOD_FUNC_DECLARE_4(void, zfLogStackTrace,
                        ZFMP_OUT(zfstring &, ret),
                        ZFMP_IN_OPT(const zfchar *, prefix, zfnull),
                        ZFMP_IN_OPT(zfindex, ignoreLevel, 0),
                        ZFMP_IN_OPT(zfindex, maxLevel, 20))
/** @brief see #zfLogStackTrace */
ZFMETHOD_FUNC_DECLARE_3(zfstring, zfLogStackTrace,
                        ZFMP_IN_OPT(const zfchar *, prefix, zfnull),
                        ZFMP_IN_OPT(zfindex, ignoreLevel, 0),
                        ZFMP_IN_OPT(zfindex, maxLevel, 20))

/**
 * @brief get the caller's info
 *
 * see #zfLogStackTrace for how to make it available
 */
ZFMETHOD_FUNC_DECLARE_2(void, zfLogCallerInfo,
                        ZFMP_OUT(zfstring &, ret),
                        ZFMP_IN_OPT(zfindex, ignoreLevel, 0))
/** @brief see zfLogCallerInfo */
ZFMETHOD_FUNC_DECLARE_1(zfstring, zfLogCallerInfo,
                        ZFMP_IN_OPT(zfindex, ignoreLevel, 0))

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFLog_StackTrace_h_

