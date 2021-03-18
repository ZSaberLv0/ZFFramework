/**
 * @file ZFMainEntry.h
 * @brief app's main entry wrapper
 */

#ifndef _ZFI_ZFMainEntry_h_
#define _ZFI_ZFMainEntry_h_

#include "ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief app's main entry wrapper
 *
 * typical usage:
 * @code
 *   // in your main.cpp
 *   ZFMAIN_ENTRY()
 *   {
 *       // params hold the params from certain app routine,
 *       // it's not ensured each implementation would have same params format
 *       ZFCoreArray<zfstring> const &params = ZFApp::appParams();
 *
 *       // do your work,
 *       // which usually is creating your main window
 *
 *       // specify exit code if necessary
 *       ZFApp::appExitCode(-1);
 *   }
 * @endcode
 */
#define ZFMAIN_ENTRY() \
    static void _ZFMain(void); \
    ZF_STATIC_REGISTER_INIT(ZFMainEntryRegister) \
    { \
        zfCoreAssertWithMessageTrim(_ZFP_ZFMainFunc() == zfnull, "ZFMAIN_ENTRY already registered"); \
        _ZFP_ZFMainFunc() = _ZFMain; \
    } \
    ZF_STATIC_REGISTER_END(ZFMainEntryRegister) \
    static void _ZFMain(void)

// ============================================================
ZF_NAMESPACE_BEGIN(ZFApp)

/**
 * @brief get the app params
 *
 * NOTE: the app params only avaialble after #ZFMAIN_ENTRY
 */
ZFMETHOD_FUNC_DECLARE_0(const ZFCoreArray<zfstring> &, appParams)

/** @brief exit code for app, 0 by default */
ZFMETHOD_FUNC_DECLARE_1(void, appExitCode, ZFMP_IN(zfint, exitCode))
/** @brief exit code for app, 0 by default */
ZFMETHOD_FUNC_DECLARE_0(zfint, appExitCode)

ZF_NAMESPACE_END(ZFApp)

// ============================================================
typedef void (*_ZFP_ZFMainFuncType)(void);
extern ZF_ENV_EXPORT _ZFP_ZFMainFuncType &_ZFP_ZFMainFunc(void);
extern ZF_ENV_EXPORT ZFCoreArray<zfstring> &_ZFP_ZFApp_appParams(void);
extern ZF_ENV_EXPORT zfint &_ZFP_ZFApp_appExitCode(void);

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFMainEntry_h_

