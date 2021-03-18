/**
 * @file ZFLog.h
 * @brief thread-safe log utility
 */

#ifndef _ZFI_ZFLog_h_
#define _ZFI_ZFLog_h_

#include "ZFLogLevel.h"
#include "ZFMutex.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief mutex used by #zfLogTrimT,
 *   you may use this lock to prevent your log actions from being interrupted
 *
 * note, the mutex would be initialized only after #ZFLevelZFFrameworkEssential
 */
ZFMETHOD_FUNC_DECLARE_0(ZFMutex *, ZFLogMutex)

extern ZF_ENV_EXPORT zfstring _ZFP_ZFLogHeaderString(ZF_IN const ZFCallerInfo &callerInfo);
/**
 * @brief a convenient debug header string
 *
 * typically looks like this (not ensured):\n
 *   "timestamp [file scope::function (line)] "
 */
#define ZFLOG_HEADER_STRING (_ZFP_ZFLogHeaderString(ZFCallerInfoMake()).cString())

// ============================================================
/**
 * @brief thread-safe log utility
 *
 * typical usage:
 * @code
 *   // output anything with ZFLOG_HEADER_STRING
 *   zfLogT() << anything;
 *   zfLog("formated: %s", someText);
 *
 *   // or, trim version without ZFLOG_HEADER_STRING
 *   zfLogTrimT() << anything;
 *   zfLogTrim("formated: %s", someText);
 * @endcode
 *
 * ZFLog is actually an output callback,
 * you can output anything that #ZFOutput supported\n
 * final log content would be written to #ZFOutputDefault,
 * which can be easily redirected to other output destinations\n
 * ZFLog is thread-safe, implemented by #ZFLogMutex\n
 * \n
 * by default, ZFLog would automatically:
 * -  append space between each output
 * -  append endl after last output
 *
 * these behavior can be changed by:
 * @code
 *   zfLogT()
 *       << ZFLogAutoSpaceOn
 *       << ZFLogAutoSpaceOff
 *       << ZFLogAutoEndlOn
 *       << ZFLogAutoEndlOff
 *       ;
 * @endcode
 * and they would be reset to default state after each zfLogT call
 */
extern ZF_ENV_EXPORT ZFOutput zfLogTrimT(void);

/** @brief see #zfLogTrimT */
#define zfLogT() (zfLogTrimT() << ZFLOG_HEADER_STRING)

/** @brief see #zfLogTrimT */
#define zfLogTrim(fmt, ...) (void)(zfLogTrimT() << zfstringWithFormat(fmt, ##__VA_ARGS__))

/** @brief see #zfLogTrimT */
#define zfLog(fmt, ...) (void)(zfLogTrimT() << ZFLOG_HEADER_STRING << zfstringWithFormat(fmt, ##__VA_ARGS__))

// ============================================================
/** @cond ZFPrivateDoc */
zfclassNotPOD _ZFP_ZFLogAutoSpaceOn {};
zfclassNotPOD _ZFP_ZFLogAutoSpaceOff {};
zfclassNotPOD _ZFP_ZFLogAutoEndlOn {};
zfclassNotPOD _ZFP_ZFLogAutoEndlOff {};
extern ZF_ENV_EXPORT const ZFOutput &operator << (const ZFOutput &output, _ZFP_ZFLogAutoSpaceOn const &v);
extern ZF_ENV_EXPORT const ZFOutput &operator << (const ZFOutput &output, _ZFP_ZFLogAutoSpaceOff const &v);
extern ZF_ENV_EXPORT const ZFOutput &operator << (const ZFOutput &output, _ZFP_ZFLogAutoEndlOn const &v);
extern ZF_ENV_EXPORT const ZFOutput &operator << (const ZFOutput &output, _ZFP_ZFLogAutoEndlOff const &v);
/** @endcond */

/** @brief see #zfLogTrimT */
extern ZF_ENV_EXPORT const _ZFP_ZFLogAutoSpaceOn ZFLogAutoSpaceOn;
/** @brief see #zfLogTrimT */
extern ZF_ENV_EXPORT const _ZFP_ZFLogAutoSpaceOff ZFLogAutoSpaceOff;
/** @brief see #zfLogTrimT */
extern ZF_ENV_EXPORT const _ZFP_ZFLogAutoEndlOn ZFLogAutoEndlOn;
/** @brief see #zfLogTrimT */
extern ZF_ENV_EXPORT const _ZFP_ZFLogAutoEndlOff ZFLogAutoEndlOff;

// ============================================================
// other convenient method
extern ZF_ENV_EXPORT zfstring _ZFP_zfLogCurTimeString(void);
/** @cond ZFPrivateDoc */
#define zfLogCurTimeString() (_ZFP_zfLogCurTimeString().cString())
/** @endcond */
/**
 * @brief get a string contains current time with format HH:mm:ss.SSS
 */
ZFMETHOD_FUNC_DECLARE_0(const zfchar *, zfLogCurTimeString)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFLog_h_

