#include "ZFImpl_sys_Posix_ZFCore_impl.h"
#include "ZFCore/protocol/ZFProtocolZFLogStackTrace.h"
#include "ZFCore/ZFLog_StackTrace.h"
#include "ZFCore/ZFString.h"

#if (ZF_ENV_sys_Posix || ZF_ENV_sys_unknown) && !ZF_ENV_sys_Android && defined(__GLIBC__)

#include <execinfo.h>
#include <memory.h>

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFLogStackTraceImpl_sys_Posix, ZFLogStackTrace, ZFProtocolLevel::e_SystemLow)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Posix:backtrace")
public:
    virtual void stackTrace(ZF_IN_OUT zfstring &ret,
                            ZF_IN_OPT const zfchar *prefix = zfnull,
                            ZF_IN_OPT zfindex ignoreLevel = 0,
                            ZF_IN_OPT zfindex maxLevel = 20)
    {
        const zfchar *fixedPrefix = ((prefix == zfnull) ? "" : prefix);
        if(maxLevel > 200)
        {
            maxLevel = 200;
        }

        void **array = (void **)zfmalloc(sizeof(void *) * (maxLevel + 1));
        zfindex size = backtrace(array, (int)(maxLevel + 1));
        zfchar **strings = backtrace_symbols(array, (int)size);

        ret += fixedPrefix;
        ret += "======================== stack begin =======================";
        ret += '\n';
        for(zfindex i = ignoreLevel; i < size; ++i)
        {
            ret += fixedPrefix;
            ret += strings[i];
            ret += '\n';
        }
        ret += fixedPrefix;
        ret += "========================  stack end  =======================";

        zffree(strings);
        zffree(array);
    }
    virtual void callerInfo(ZF_IN_OUT zfstring &ret,
                            ZF_IN_OPT zfindex ignoreLevel = 0)
    {
        static const zfindex dummyLevel = 0;
        void **array = (void **)zfmalloc(sizeof(void *) * (dummyLevel + ignoreLevel + 1));
        zfindex size = backtrace(array, 3);
        zfchar **strings = backtrace_symbols(array, (int)size);

        ret += strings[dummyLevel + ignoreLevel];

        zffree(strings);
        zffree(array);
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFLogStackTraceImpl_sys_Posix)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFLogStackTraceImpl_sys_Posix)

ZF_NAMESPACE_GLOBAL_END

#endif // #if (ZF_ENV_sys_Posix || ZF_ENV_sys_unknown) && !ZF_ENV_sys_Android

