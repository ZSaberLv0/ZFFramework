/**
 * @file ZFImplOutput.h
 * @brief interact with native log output
 */

#ifndef _ZFI_ZFImplOutput_h_
#define _ZFI_ZFImplOutput_h_

#include "ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// core log
/**
 * @brief callback to extend the native log output
 */
typedef void (*ZFImplOutputCoreLogCallback)(ZF_IN const zfchar *src);
/**
 * @brief callback to extend the native log output
 */
#define ZFImplOutputCoreLogCallbackList _ZFP_ZFImplOutputCoreLogCallbackList()
extern ZF_ENV_EXPORT ZFCoreArrayPOD<ZFImplOutputCoreLogCallback> &_ZFP_ZFImplOutputCoreLogCallbackList(void);

/**
 * @brief used to output log to native env
 */
extern ZF_ENV_EXPORT void ZFImplOutputCoreLog(ZF_IN const zfchar *src);

// ============================================================
// normal log
/**
 * @brief callback to extend the native log output
 */
typedef void (*ZFImplOutputCallback)(ZF_IN const zfchar *src,
                                     ZF_IN_OPT zfindex srcLen /* = zfindexMax() */);
/**
 * @brief callback to extend the native log output
 */
#define ZFImplOutputCallbackList _ZFP_ZFImplOutputCallbackList()
extern ZF_ENV_EXPORT ZFCoreArrayPOD<ZFImplOutputCallback> &_ZFP_ZFImplOutputCallbackList(void);

/**
 * @brief used to output log to native env
 */
extern ZF_ENV_EXPORT void ZFImplOutput(ZF_IN const zfchar *src,
                                       ZF_IN_OPT zfindex srcLen = zfindexMax());

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFImplOutput_h_

