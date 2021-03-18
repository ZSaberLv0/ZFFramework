#include "ZFOutputDefault.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
static void _ZFP_ZFOutputDefault_ZFCoreLogExt(ZF_IN const zfchar *s)
{
    ZFOutputDefault().execute(s);
}

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFOutputDefault_ZFCoreLogExt, ZFLevelZFFrameworkEssential)
{
    zfCoreLogOutputCallback(_ZFP_ZFOutputDefault_ZFCoreLogExt);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFOutputDefault_ZFCoreLogExt)
{
    zfCoreLogOutputCallback(ZFCoreLogOutputCallbackDefault);
}
ZF_GLOBAL_INITIALIZER_END(ZFOutputDefault_ZFCoreLogExt)

ZF_NAMESPACE_GLOBAL_END

