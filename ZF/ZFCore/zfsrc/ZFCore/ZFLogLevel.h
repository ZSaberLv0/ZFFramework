/**
 * @file ZFLogLevel.h
 * @brief log level define
 */

#ifndef _ZFI_ZFLogLevel_h_
#define _ZFI_ZFLogLevel_h_

#include "ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief log level
 *
 * similar to Android's Log.v() to Log.e()\n
 * app's global log level can be set or get by #ZFLogLevelDefault,
 * you may achieve your own log level specified behavior\n
 * it's ensured lower level has smaller value
 */
ZFENUM_BEGIN(ZFLogLevel)
    ZFENUM_VALUE(Verbose)
    ZFENUM_VALUE(Debug)
    ZFENUM_VALUE(Info)
    ZFENUM_VALUE(Warning)
    ZFENUM_VALUE(Error)
    ZFENUM_VALUE(Assert)
ZFENUM_SEPARATOR(ZFLogLevel)
    ZFENUM_VALUE_REGISTER(Verbose)
    ZFENUM_VALUE_REGISTER(Debug)
    ZFENUM_VALUE_REGISTER(Info)
    ZFENUM_VALUE_REGISTER(Warning)
    ZFENUM_VALUE_REGISTER(Error)
    ZFENUM_VALUE_REGISTER(Assert)
ZFENUM_END_WITH_DEFAULT(ZFLogLevel, ZFLogLevel::e_Warning)

ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
/**
 * @brief see #ZFObject::observerNotify
 *
 * notified when #ZFLogLevelDefault changed
 */
ZFOBSERVER_EVENT_GLOBAL(LogLevelOnChange)
ZF_NAMESPACE_END(ZFGlobalEvent)

/** @brief see #ZFLogLevel, #ZFLogLevel::e_Warning by default */
ZFMETHOD_FUNC_DECLARE_1(void, ZFLogLevelDefault,
                        ZFMP_IN(ZFLogLevelEnum, level))
/** @brief see #ZFLogLevel */
ZFMETHOD_FUNC_DECLARE_0(ZFLogLevelEnum, ZFLogLevelDefault)

/**
 * @brief true if specified level is active
 *   (i.e. #ZFLogLevelDefault not greater than the specified level)
 */
ZFMETHOD_FUNC_INLINE_DECLARE_1(zfbool, ZFLogLevelIsActive,
                               ZFMP_IN(ZFLogLevelEnum, level))
{
    return (ZFLogLevelDefault() <= level);
}

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFLogLevel_h_

