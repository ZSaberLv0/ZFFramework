#include "ZFMainEntry.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_NAMESPACE_BEGIN(ZFApp)

ZFMETHOD_FUNC_DEFINE_0(const ZFCoreArray<zfstring> &, appParams)
{
    return _ZFP_ZFApp_appParams();
}

ZFMETHOD_FUNC_DEFINE_1(void, appExitCode, ZFMP_IN(zfint, exitCode))
{
    _ZFP_ZFApp_appExitCode() = exitCode;
}
ZFMETHOD_FUNC_DEFINE_0(zfint, appExitCode)
{
    return _ZFP_ZFApp_appExitCode();
}

ZF_NAMESPACE_END(ZFApp)

// ============================================================
_ZFP_ZFMainFuncType &_ZFP_ZFMainFunc(void)
{
    static _ZFP_ZFMainFuncType _func = zfnull;
    return _func;
}

ZFCoreArray<zfstring> &_ZFP_ZFApp_appParams(void)
{
    static ZFCoreArray<zfstring> d;
    return d;
}
zfint &_ZFP_ZFApp_appExitCode(void)
{
    static zfint d = 0;
    return d;
}

ZF_NAMESPACE_GLOBAL_END

