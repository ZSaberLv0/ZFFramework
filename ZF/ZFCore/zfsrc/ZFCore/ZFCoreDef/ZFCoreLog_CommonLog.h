/**
 * @file ZFCoreLog_CommonLog.h
 * @brief common log messages for ZFFramework
 *
 * used internally, you should use zfLog instead
 */

#ifndef _ZFI_ZFCoreLog_CommonLog_h_
#define _ZFI_ZFCoreLog_CommonLog_h_

#include "ZFCoreLog.h"
ZF_NAMESPACE_GLOBAL_BEGIN

extern ZF_ENV_EXPORT void _ZFP_zfCoreLogCriticalMessage(ZF_IN const ZFCallerInfo &callerInfo,
                                                        ZF_IN const zfchar *fmt,
                                                        ...);
extern ZF_ENV_EXPORT void _ZFP_zfCoreLogCriticalMessageV(ZF_IN const ZFCallerInfo &callerInfo,
                                                         ZF_IN const zfchar *fmt,
                                                         ZF_IN va_list vaList);
extern ZF_ENV_EXPORT void _ZFP_zfCoreCritical(ZF_IN const ZFCallerInfo &callerInfo,
                                              ZF_IN const zfchar *fmt,
                                              ...);
extern ZF_ENV_EXPORT void _ZFP_zfCoreCriticalV(ZF_IN const ZFCallerInfo &callerInfo,
                                               ZF_IN const zfchar *fmt,
                                               ZF_IN va_list vaList);

// ============================================================
/**
 * @brief print a critical error message which is easy to notice
 *
 * log only, see also #zfCoreCriticalMessage
 */
#define zfCoreLogCriticalMessage(fmt, ...) _ZFP_zfCoreLogCriticalMessage(ZFCallerInfoMake(), fmt, ##__VA_ARGS__)
/** @brief see #zfCoreLogCriticalMessage */
#define zfCoreLogCriticalMessageV(fmt, vaList) _ZFP_zfCoreLogCriticalMessageV(ZFCallerInfoMake(), fmt, vaList)

/** @brief see #zfCoreLogCriticalMessage */
#define zfCoreLogCriticalMessageTrim(fmt, ...) _ZFP_zfCoreLogCriticalMessage(ZFCallerInfoEmpty(), fmt, ##__VA_ARGS__)
/** @brief see #zfCoreLogCriticalMessage */
#define zfCoreLogCriticalMessageTrimV(fmt, vaList) _ZFP_zfCoreLogCriticalMessageV(ZFCallerInfoEmpty(), fmt, vaList)

/** @brief see #zfCoreLogCriticalMessage */
#define zfCoreLogCriticalMessageDetail(callerInfo, fmt, ...) _ZFP_zfCoreLogCriticalMessage(callerInfo, fmt, ##__VA_ARGS__)
/** @brief see #zfCoreLogCriticalMessage */
#define zfCoreLogCriticalMessageDetailV(callerInfo, fmt, vaList) _ZFP_zfCoreLogCriticalMessageV(callerInfo, fmt, vaList)

// ============================================================
/**
 * @brief assert exp is zftrue
 *
 * do nothing if exp is zftrue, otherwise,
 * print a critical error message,
 * then terminate app by #zfCoreCriticalError
 * @note unlike zfassert, this function is always active, even if ZF_ENV_DEBUG is zftrue
 * @see zfassert
 */
#define zfCoreAssert(exp) zfCoreAssertWithMessage(exp, "assert failed for \"%s\"", ZFM_TOSTRING(exp))

/** @brief see #zfCoreAssert */
#define zfCoreAssertTrim(exp) zfCoreAssertWithMessageTrim(exp, "assert failed for \"%s\"", ZFM_TOSTRING(exp))

/** @brief see #zfCoreAssert */
#define zfCoreAssertDetail(exp, callerInfo) zfCoreAssertWithMessageDetail(exp, callerInfo, "assert failed for \"%s\"", ZFM_TOSTRING(exp))

// ============================================================
/**
 * @brief print a critical error message and abort
 */
#define zfCoreCriticalMessage(fmt, ...) _ZFP_zfCoreCritical(ZFCallerInfoMake(), fmt, ##__VA_ARGS__)
/** @brief see #zfCoreCriticalMessage */
#define zfCoreCriticalMessageV(fmt, vaList) _ZFP_zfCoreCriticalV(ZFCallerInfoMake(), fmt, vaList)

/** @brief see #zfCoreCriticalMessage */
#define zfCoreCriticalMessageTrim(fmt, ...) _ZFP_zfCoreCritical(ZFCallerInfoEmpty(), fmt, ##__VA_ARGS__)
/** @brief see #zfCoreCriticalMessage */
#define zfCoreCriticalMessageTrimV(fmt, vaList) _ZFP_zfCoreCriticalV(ZFCallerInfoEmpty(), fmt, vaList)

/** @brief see #zfCoreCriticalMessage */
#define zfCoreCriticalMessageDetail(callerInfo, fmt, ...) _ZFP_zfCoreCritical(callerInfo, fmt, ##__VA_ARGS__)
/** @brief see #zfCoreCriticalMessage */
#define zfCoreCriticalMessageDetailV(callerInfo, fmt, vaList) _ZFP_zfCoreCriticalV(callerInfo, fmt, vaList)

// ============================================================
/**
 * @brief #zfCoreAssert with custom message
 */
#define zfCoreAssertWithMessage(exp, fmt, ...) \
    do \
    { \
        if(!(exp)) \
        { \
            zfCoreCriticalMessage(fmt, ##__VA_ARGS__); \
        } \
    } while(zffalse)

/**
 * @brief see #zfCoreAssertWithMessage
 */
#define zfCoreAssertWithMessageTrim(exp, fmt, ...) \
    do \
    { \
        if(!(exp)) \
        { \
            zfCoreCriticalMessageTrim(fmt, ##__VA_ARGS__); \
        } \
    } while(zffalse)

/**
 * @brief see #zfCoreAssertWithMessage
 */
#define zfCoreAssertWithMessageDetail(exp, callerInfo, fmt, ...) \
    do \
    { \
        if(!(exp)) \
        { \
            zfCoreCriticalMessageDetail(callerInfo, fmt, ##__VA_ARGS__); \
        } \
    } while(zffalse)

// ============================================================
/**
 * @brief log that likes "[file function (line)] index i out of range [0, n)"
 */
#define zfCoreCriticalIndexOutOfRange(index, range) \
    zfCoreCriticalIndexOutOfRangeDetail(ZFCallerInfoMake(), index, range)
/**
 * @brief see #zfCoreCriticalIndexOutOfRange
 */
#define zfCoreCriticalIndexOutOfRangeDetail(callerInfo, index, range) \
    zfCoreCriticalMessageDetail(callerInfo, \
        "index %zi out of range [0, %zi)", \
        (zfindex)(index), \
        (zfindex)(range))

// ============================================================
/**
 * @brief log that likes "[file function (line)] should not go here"
 */
#define zfCoreCriticalShouldNotGoHere() \
    zfCoreCriticalShouldNotGoHereDetail(ZFCallerInfoMake())
/**
 * @brief see #zfCoreCriticalShouldNotGoHere
 */
#define zfCoreCriticalShouldNotGoHereDetail(callerInfo) \
    zfCoreCriticalMessageDetail(callerInfo, "should not go here")

// ============================================================
/**
 * @brief log that likes "[file function (line)] not supported"
 */
#define zfCoreCriticalNotSupported() \
    zfCoreCriticalNotSupportedDetail(ZFCallerInfoMake())
/**
 * @brief see #zfCoreCriticalNotSupported
 */
#define zfCoreCriticalNotSupportedDetail(callerInfo) \
    zfCoreCriticalMessageDetail(callerInfo, "not supported")

ZF_NAMESPACE_GLOBAL_END

#if 0 // test only
    #if (defined(ANDROID) || defined(__ANDROID__))
        #include <jni.h>
        #include <android/log.h>
        #define zfzfzfLog(fmt, ...) \
            ((void)__android_log_print(ANDROID_LOG_ERROR, "test", fmt, ##__VA_ARGS__))
    #elif (defined(QT_VERSION) || defined(QT_CORE_LIB))
        #include <QDebug>
        #define zfzfzfLog(fmt, ...) \
            qDebug(fmt, ##__VA_ARGS__)
    #else
        #include <stdio.h>
        #define zfzfzfLog(fmt, ...) \
            do { \
                printf(fmt, ##__VA_ARGS__); \
                printf("\n"); \
            } while(false)
    #endif
#endif

#endif // #ifndef _ZFI_ZFCoreLog_CommonLog_h_

