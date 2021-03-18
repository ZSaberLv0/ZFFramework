#include "ZFLog_StackTrace.h"
#include "protocol/ZFProtocolZFLogStackTrace.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_DEFINE_0(zfbool, zfLogStackTraceAvailable)
{
    return (ZFPROTOCOL_TRY_ACCESS(ZFLogStackTrace) != zfnull);
}

ZFMETHOD_FUNC_DEFINE_4(void, zfLogStackTrace,
                       ZFMP_OUT(zfstring &, ret),
                       ZFMP_IN_OPT(const zfchar *, prefix, zfnull),
                       ZFMP_IN_OPT(zfindex, ignoreLevel, 0),
                       ZFMP_IN_OPT(zfindex, maxLevel, 20))
{
    if(ZFPROTOCOL_TRY_ACCESS(ZFLogStackTrace) != zfnull)
    {
        ZFPROTOCOL_TRY_ACCESS(ZFLogStackTrace)->stackTrace(ret, prefix, ignoreLevel + 2, maxLevel);
    }
    else
    {
        const zfchar *fixedPrefix = ((prefix == zfnull) ? "" : prefix);

        ret += fixedPrefix;
        ret += "======================== stack begin =======================";
        ret += '\n';

        ret += fixedPrefix;
        ret += "| zfLogStackTrace is currently unsupported";
        ret += '\n';

        ret += fixedPrefix;
        ret += "========================  stack end  =======================";
        ret += '\n';
    }
}
ZFMETHOD_FUNC_DEFINE_3(zfstring, zfLogStackTrace,
                       ZFMP_IN_OPT(const zfchar *, prefix, zfnull),
                       ZFMP_IN_OPT(zfindex, ignoreLevel, 0),
                       ZFMP_IN_OPT(zfindex, maxLevel, 20))
{
    zfstring ret;
    if(ZFPROTOCOL_TRY_ACCESS(ZFLogStackTrace) != zfnull)
    {
        ZFPROTOCOL_TRY_ACCESS(ZFLogStackTrace)->stackTrace(ret, prefix, ignoreLevel + 2, maxLevel);
    }
    else
    {
        const zfchar *fixedPrefix = ((prefix == zfnull) ? "" : prefix);

        ret += fixedPrefix;
        ret += "======================== stack begin =======================";
        ret += '\n';

        ret += fixedPrefix;
        ret += "| zfLogStackTrace is currently unsupported";
        ret += '\n';

        ret += fixedPrefix;
        ret += "========================  stack end  =======================";
        ret += '\n';
    }
    return ret;
}

ZFMETHOD_FUNC_DEFINE_2(void, zfLogCallerInfo,
                       ZFMP_OUT(zfstring &, ret),
                       ZFMP_IN_OPT(zfindex, ignoreLevel, 0))
{
    if(ZFPROTOCOL_TRY_ACCESS(ZFLogStackTrace) != zfnull)
    {
        ZFPROTOCOL_TRY_ACCESS(ZFLogStackTrace)->callerInfo(ret, ignoreLevel + 2);
    }
    else
    {
        ret += "<zfLogCallerInfo is currently unsupported>";
    }
}
ZFMETHOD_FUNC_DEFINE_1(zfstring, zfLogCallerInfo,
                       ZFMP_IN_OPT(zfindex, ignoreLevel, 0))
{
    zfstring ret;
    if(ZFPROTOCOL_TRY_ACCESS(ZFLogStackTrace) != zfnull)
    {
        ZFPROTOCOL_TRY_ACCESS(ZFLogStackTrace)->callerInfo(ret, ignoreLevel + 2);
    }
    else
    {
        ret += "<zfLogCallerInfo is currently unsupported>";
    }
    return ret;
}

ZF_NAMESPACE_GLOBAL_END

