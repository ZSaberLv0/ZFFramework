/**
 * @file ZFEnvInfo.h
 * @brief info of native environment
 */

#ifndef _ZFI_ZFEnvInfo_h_
#define _ZFI_ZFEnvInfo_h_

#include "ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/** @brief see #ZFEnvInfo::envSummaryCallbackRegister */
typedef void (*ZFEnvSummaryCallback)(ZF_IN_OUT zfstring &ret);

ZF_NAMESPACE_BEGIN(ZFEnvInfo)

// ============================================================
/**
 * @brief util method to get summary info
 */
ZFMETHOD_FUNC_DECLARE_1(void, envSummary,
                        ZFMP_IN_OUT(zfstring &, ret))
/**
 * @brief see #envSummary
 */
ZFMETHOD_FUNC_DECLARE_0(zfstring, envSummary)

/**
 * @brief register a callback to output custom env info, see #envSummary
 */
extern ZF_ENV_EXPORT void envSummaryCallbackRegister(ZF_IN const zfchar *name, ZF_IN ZFEnvSummaryCallback callback);
/** @brief see #envSummaryCallbackRegister */
extern ZF_ENV_EXPORT void envSummaryCallbackUnregister(ZF_IN const zfchar *name);

// ============================================================
/**
 * @brief name of native system, e.g. "iOS", empty if not available
 */
ZFMETHOD_FUNC_DECLARE_1(void, systemName,
                        ZFMP_IN_OUT(zfstring &, ret))
/**
 * @brief see #systemName
 */
ZFMETHOD_FUNC_DECLARE_0(zfstring, systemName)
/**
 * @brief version of native system, e.g. "9.0.1", empty if not available
 */
ZFMETHOD_FUNC_DECLARE_1(void, systemVersion,
                        ZFMP_IN_OUT(zfstring &, ret))
/**
 * @brief see #systemVersion
 */
ZFMETHOD_FUNC_DECLARE_0(zfstring, systemVersion)

// ============================================================
/**
 * @brief name of native framework, e.g. "Qt", empty if not available
 */
ZFMETHOD_FUNC_DECLARE_1(void, frameworkName,
                        ZFMP_IN_OUT(zfstring &, ret))
/**
 * @brief see #frameworkName
 */
ZFMETHOD_FUNC_DECLARE_0(zfstring, frameworkName)
/**
 * @brief version of native framework, e.g. "9.0.1", empty if not available
 */
ZFMETHOD_FUNC_DECLARE_1(void, frameworkVersion,
                        ZFMP_IN_OUT(zfstring &, ret))
/**
 * @brief see #frameworkVersion
 */
ZFMETHOD_FUNC_DECLARE_0(zfstring, frameworkVersion)

ZF_NAMESPACE_END(ZFEnvInfo)
ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFEnvInfo_h_

