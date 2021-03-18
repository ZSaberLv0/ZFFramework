#include "ZFLog.h"
#include "ZFLog_StackTrace.h"
#include "ZFImplOutput.h"
#include "ZFString.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
static void _ZFP_ZFLog_ZFCoreLogExt(ZF_IN const ZFCallerInfo &callerInfo)
{
    if(zfLogStackTraceAvailable())
    {
        zfCoreLogTrim(zfLogStackTrace(zfnull, 2).cString());
    }
}

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFLog_ZFCoreLogExt, ZFLevelZFFrameworkEssential)
{
    zfCoreCriticalErrorCallbackAdd(_ZFP_ZFLog_ZFCoreLogExt);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFLog_ZFCoreLogExt)
{
    zfCoreCriticalErrorCallbackRemove(_ZFP_ZFLog_ZFCoreLogExt);
}
ZF_GLOBAL_INITIALIZER_END(ZFLog_ZFCoreLogExt)

ZF_NAMESPACE_GLOBAL_END

