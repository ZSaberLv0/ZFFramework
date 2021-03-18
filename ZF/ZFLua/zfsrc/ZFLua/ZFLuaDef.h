/**
 * @file ZFLuaDef.h
 * @brief global header for ZFLua module
 */

#ifndef _ZFI_ZFLuaDef_h_
#define _ZFI_ZFLuaDef_h_

#include "ZFCore.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
extern ZF_ENV_EXPORT ZFCoreArray<ZFOutput> &_ZFP_ZFLuaErrorCallbacks(void);
/**
 * @brief callbacks that would be called when lua related error occurred,
 *   for debug use only
 *
 * by default, #ZFOutputDefault would be added during #ZFLevelZFFrameworkNormal
 */
#define ZFLuaErrorCallbacks _ZFP_ZFLuaErrorCallbacks()
extern ZF_ENV_EXPORT void _ZFP_ZFLuaErrorOccurred(ZF_IN const ZFCallerInfo &callerInfo,
                                                  ZF_IN const zfchar *fmt,
                                                  ...);
/**
 * @brief util method to notify #ZFLuaErrorCallbacks
 */
#define ZFLuaErrorOccurredTrim(fmt, ...) \
    ZFLuaErrorOccurredDetail(ZFCallerInfoEmpty(), fmt, ##__VA_ARGS__)
/**
 * @brief util method to notify #ZFLuaErrorCallbacks
 */
#define ZFLuaErrorOccurred(fmt, ...) \
    ZFLuaErrorOccurredDetail(ZFCallerInfoMake(), fmt, ##__VA_ARGS__)
/**
 * @brief util method to notify #ZFLuaErrorCallbacks
 */
#define ZFLuaErrorOccurredDetail(callerInfo, fmt, ...) \
    _ZFP_ZFLuaErrorOccurred(callerInfo, fmt, ##__VA_ARGS__)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFLuaDef_h_

