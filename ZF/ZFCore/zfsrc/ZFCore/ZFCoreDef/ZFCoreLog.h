/**
 * @file ZFCoreLog.h
 * @brief internal log utility
 *
 * used internally, you should use zfLog instead
 */

#ifndef _ZFI_ZFCoreLog_h_
#define _ZFI_ZFCoreLog_h_

#include "ZFCallerInfo.h"
#include <assert.h>
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief same as assert defined for future use
 *
 * active only if ZF_ENV_DEBUG is zftrue
 * @see zfCoreAssert
 */
#if ZF_ENV_DEBUG
    #define zfassert(b) assert(b)
#else
    #define zfassert(b) ((void)0)
#endif

/**
 * @brief assert at compile time
 */
#define ZFASSERT_STATIC(cond, name) typedef char assert_failed_##name[(cond) ? 1 : -1];

// ============================================================
/**
 * @brief output callback for zfCoreLog
 */
typedef void (*ZFCoreLogOutputCallbackType)(ZF_IN const zfchar *s);
extern ZF_ENV_EXPORT void _ZFP_ZFCoreLogOutputCallbackDefault(ZF_IN const zfchar *s);
/**
 * @brief default output callback for zfCoreLog
 */
#define ZFCoreLogOutputCallbackDefault _ZFP_ZFCoreLogOutputCallbackDefault

/**
 * @brief set the output callback for zfCoreLog
 * @warning within the callback, you must not access any other ZFFramework's module,
 *   otherwise dead lock may be occurred
 */
extern ZF_ENV_EXPORT void zfCoreLogOutputCallback(ZF_IN ZFCoreLogOutputCallbackType callback);
/**
 * @brief get the output callback for zfCoreLog
 */
extern ZF_ENV_EXPORT ZFCoreLogOutputCallbackType zfCoreLogOutputCallback(void);

// ============================================================
extern ZF_ENV_EXPORT void _ZFP_zfCoreLog(ZF_IN const ZFCallerInfo &callerInfo,
                                         ZF_IN zfbool isAutoEndl,
                                         ZF_IN const zfchar *format,
                                         ...);
extern ZF_ENV_EXPORT void _ZFP_zfCoreLogV(ZF_IN const ZFCallerInfo &callerInfo,
                                          ZF_IN zfbool isAutoEndl,
                                          ZF_IN const zfchar *format,
                                          ZF_IN va_list vaList);
/**
 * @brief log utility used internally
 *
 * quite similar to printf, except that this function would automatically add header info to head,
 * which includes file name, function name and file line info,
 * and an extra '\\n' to tail\n
 * for all the format supported, please refer to #zfstringAppend
 * @warning this function is for internal use,
 *   usually to output critical error messages,
 *   you should not use other log method in your app
 * @warning this method is not thread safe
 * @see zfCoreLogOutputCallback, zfCoreCriticalError
 */
#define zfCoreLog(format, ...) _ZFP_zfCoreLog(ZFCallerInfoMake(), zftrue, format, ##__VA_ARGS__)
/** @brief see #zfCoreLog */
#define zfCoreLogV(format, vaList) _ZFP_zfCoreLogV(ZFCallerInfoMake(), zftrue, format, vaList)

/**
 * @brief same as #zfCoreLog except that no '\\n' is added to tail, see #zfCoreLog
 */
#define zfCoreLogNoEndl(format, ...) _ZFP_zfCoreLog(ZFCallerInfoMake(), zffalse, format, ##__VA_ARGS__)
/** @brief see #zfCoreLogNoEndl */
#define zfCoreLogNoEndlV(format, vaList) _ZFP_zfCoreLogV(ZFCallerInfoMake(), zffalse, format, vaList)

/**
 * @brief log without header info, see #zfCoreLog
 */
#define zfCoreLogTrim(format, ...) _ZFP_zfCoreLog(ZFCallerInfoEmpty(), zftrue, format, ##__VA_ARGS__)
/** @brief see #zfCoreLogTrim */
#define zfCoreLogTrimV(format, vaList) _ZFP_zfCoreLogV(ZFCallerInfoEmpty(), zftrue, format, vaList)

/**
 * @brief log without header info or '\\n' to tail, see #zfCoreLog
 */
#define zfCoreLogTrimNoEndl(format, ...) _ZFP_zfCoreLog(ZFCallerInfoEmpty(), zffalse, format, ##__VA_ARGS__)
/** @brief see #zfCoreLogTrimNoEndl */
#define zfCoreLogTrimNoEndlV(format, vaList) _ZFP_zfCoreLogV(ZFCallerInfoEmpty(), zffalse, format, vaList)

/**
 * @brief log with custom header info, see #zfCoreLog
 */
#define zfCoreLogDetail(callerInfo, format, ...) _ZFP_zfCoreLog(callerInfo, zftrue, format, ##__VA_ARGS__)
/** @brief see #zfCoreLogDetail */
#define zfCoreLogDetailV(callerInfo, format, vaList) _ZFP_zfCoreLogV(callerInfo, zftrue, format, vaList)

/**
 * @brief log with custom header info, see #zfCoreLog
 */
#define zfCoreLogDetailNoEndl(callerInfo, format, ...) _ZFP_zfCoreLog(callerInfo, zffalse, format, ##__VA_ARGS__)
/** @brief see #zfCoreLogDetailNoEndl */
#define zfCoreLogDetailNoEndlV(callerInfo, format, vaList) _ZFP_zfCoreLogV(callerInfo, zffalse, format, vaList)

// ============================================================
/**
 * @brief callback which would be called if zfCoreCriticalError is called
 */
typedef void (*ZFCoreCriticalErrorCallback)(ZF_IN const ZFCallerInfo &callerInfo);

/**
 * @brief see #ZFCoreCriticalErrorCallback
 */
extern ZF_ENV_EXPORT void zfCoreCriticalErrorPrepareCallbackAdd(ZF_IN ZFCoreCriticalErrorCallback callback);
/**
 * @brief see #ZFCoreCriticalErrorCallback
 */
extern ZF_ENV_EXPORT void zfCoreCriticalErrorPrepareCallbackRemove(ZF_IN ZFCoreCriticalErrorCallback callback);

/**
 * @brief see #ZFCoreCriticalErrorCallback
 */
extern ZF_ENV_EXPORT void zfCoreCriticalErrorCallbackAdd(ZF_IN ZFCoreCriticalErrorCallback callback);
/**
 * @brief see #ZFCoreCriticalErrorCallback
 */
extern ZF_ENV_EXPORT void zfCoreCriticalErrorCallbackRemove(ZF_IN ZFCoreCriticalErrorCallback callback);

extern ZF_ENV_EXPORT void _ZFP_zfCoreCriticalErrorPrepare(ZF_IN const ZFCallerInfo &callerInfo);
extern ZF_ENV_EXPORT void _ZFP_zfCoreCriticalError(ZF_IN const ZFCallerInfo &callerInfo);
/**
 * @brief ready to output critical error, see #zfCoreCriticalError
 *
 * usage:
 * @code
 *   zfCoreCriticalErrorPrepare();
 *   zfCoreLog(xxx);
 *   zfCoreCriticalError();
 * @endcode
 * this is useful to redirect #zfCoreLog by #zfCoreLogOutputCallback,
 * to save critical error messages to proper location
 */
#define zfCoreCriticalErrorPrepare() _ZFP_zfCoreCriticalErrorPrepare(ZFCallerInfoMake())
/** @brief see #zfCoreCriticalErrorPrepare */
#define zfCoreCriticalErrorPrepareDetail(callerInfo) _ZFP_zfCoreCriticalErrorPrepare(callerInfo)

/**
 * @brief used when error occurred, to terminate the application
 *
 * @warning this function is for internal use,
 *   calling this method would cause app to be terminated
 */
#define zfCoreCriticalError() _ZFP_zfCoreCriticalError(ZFCallerInfoMake())
/** @brief see #zfCoreCriticalError */
#define zfCoreCriticalErrorDetail(callerInfo) _ZFP_zfCoreCriticalError(callerInfo)

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreLog_h_

