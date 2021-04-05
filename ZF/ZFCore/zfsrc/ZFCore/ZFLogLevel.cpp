#include "ZFLogLevel.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFENUM_DEFINE(ZFLogLevel)

ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
ZFOBSERVER_EVENT_GLOBAL_REGISTER(LogLevelOnChange)
ZF_NAMESPACE_END(ZFGlobalEvent)

static ZFLogLevelEnum _ZFP_ZFLogLevelGlobal = ZFLogLevel::EnumDefault();
void ZFLogLevelDefault(ZF_IN ZFLogLevelEnum level)
{
    _ZFP_ZFLogLevelGlobal = level;
    if(ZFFrameworkStateCheck(ZFLevelZFFrameworkEssential) == ZFFrameworkStateAvailable)
    {
        ZFGlobalObserver().observerNotify(ZFGlobalEvent::EventLogLevelOnChange());
    }
}
ZFLogLevelEnum ZFLogLevelDefault(void)
{
    return _ZFP_ZFLogLevelGlobal;
}
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFLogLevelReset, ZFLevelZFFrameworkEssential)
{
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFLogLevelReset)
{
    _ZFP_ZFLogLevelGlobal = ZFLogLevel::EnumDefault();
}
ZF_GLOBAL_INITIALIZER_END(ZFLogLevelReset)

ZF_NAMESPACE_GLOBAL_END

