/**
 * @file AndroidJNIUtil.h
 * @brief Android's JNI utilities
 */

#ifndef _AndroidJNIUtil_h_
#define _AndroidJNIUtil_h_

// need -llog if use logs in Android

#include "JNIUtil.h"
#if (defined(ANDROID) || defined(__ANDROID__))
#include <android/log.h>
namespace AndroidJNIUtil {

/** @brief verbose log level */
#define AndroidLogLevelV ANDROID_LOG_VERBOSE
/** @brief debug log level */
#define AndroidLogLevelD ANDROID_LOG_DEBUG
/** @brief info log level */
#define AndroidLogLevelI ANDROID_LOG_INFO
/** @brief warning log level */
#define AndroidLogLevelW ANDROID_LOG_WARN
/** @brief error log level */
#define AndroidLogLevelE ANDROID_LOG_ERROR
/** @brief error log level */
#define AndroidLogLevelA ANDROID_LOG_FATAL
/** @brief default log level, could be changed by adding to pre-compiler */
#ifndef AndroidLogLevelDefault
    #define AndroidLogLevelDefault AndroidLogLevelV
#endif

/** @cond ZFPrivateDoc */
extern _JNI_EXPORT JNIUtil::JNIString &_AndroidLogTagDefault(void);
/** @endcond */
/**
 * @brief default log tag, could be changed at runtime
 */
inline const char *AndroidLogTagDefault(void)
{
    return _AndroidLogTagDefault().c_str();
}
inline void AndroidLogTagDefault(const char *s)
{
    _AndroidLogTagDefault() = (s ? s : "");
}

/**
 * @brief log with level, tag and fmt
 */
#define AndroidLogDetail(level, tag, fmt, ...) \
    ((void)__android_log_print(level, tag, fmt, ##__VA_ARGS__))

/**
 * @brief log with default level, default tag and fmt
 */
#define AndroidLog(fmt, ...) \
    AndroidLogDetail(AndroidLogLevelDefault, AndroidLogTagDefault(), fmt, ##__VA_ARGS__)

}
#endif // #if (defined(ANDROID) || defined(__ANDROID__))
#endif // #ifndef _AndroidJNIUtil_h_
