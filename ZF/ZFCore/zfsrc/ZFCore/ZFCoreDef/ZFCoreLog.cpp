#include "ZFCoreLog.h"
#include "ZFCoreSPrintf.h"
#include "ZFCoreStaticInitializer.h"
#include "ZFCoreArray.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_STATIC_INITIALIZER_INIT(ZFCoreLogDataHolder)
{
    outputCallback = ZFCoreLogOutputCallbackDefault;
}
ZF_STATIC_INITIALIZER_DESTROY(ZFCoreLogDataHolder)
{
    outputCallback = zfnull;
}
ZFCoreLogOutputCallbackType outputCallback;
ZFCoreArrayPOD<ZFCoreCriticalErrorCallback> criticalErrorCallbacks;
ZFCoreArrayPOD<ZFCoreCriticalErrorCallback> criticalErrorPrepareCallbacks;
ZF_STATIC_INITIALIZER_END(ZFCoreLogDataHolder)
#define _ZFP_ZFCoreLogOutputCallback (ZF_STATIC_INITIALIZER_INSTANCE(ZFCoreLogDataHolder)->outputCallback)
#define _ZFP_ZFCoreCriticalErrorCallbacks (ZF_STATIC_INITIALIZER_INSTANCE(ZFCoreLogDataHolder)->criticalErrorCallbacks)
#define _ZFP_ZFCoreCriticalErrorPrepareCallbacks (ZF_STATIC_INITIALIZER_INSTANCE(ZFCoreLogDataHolder)->criticalErrorPrepareCallbacks)

void _ZFP_ZFCoreLogOutputCallbackDefault(ZF_IN const zfchar *s)
{
    // try to print to std output
    fprintf(stderr, "%s", s);
}

void zfCoreLogOutputCallback(ZF_IN_OPT ZFCoreLogOutputCallbackType callback)
{
    _ZFP_ZFCoreLogOutputCallback = callback;
}
ZFCoreLogOutputCallbackType zfCoreLogOutputCallback(void)
{
    return _ZFP_ZFCoreLogOutputCallback;
}

void _ZFP_zfCoreLog(ZF_IN const ZFCallerInfo &callerInfo,
                    ZF_IN zfbool isAutoEndl,
                    ZF_IN const zfchar *format,
                    ...)
{
    va_list vaList;
    va_start(vaList, format);
    _ZFP_zfCoreLogV(callerInfo, isAutoEndl, format, vaList);
    va_end(vaList);
}
void _ZFP_zfCoreLogV(ZF_IN const ZFCallerInfo &callerInfo,
                     ZF_IN zfbool isAutoEndl,
                     ZF_IN const zfchar *format,
                     ZF_IN va_list vaList)
{
    if(_ZFP_ZFCoreLogOutputCallback == zfnull)
    {
        return ;
    }

    zfstring s;

    if(callerInfo.callerInfoT(s))
    {
        s += ' ';
    }
    zfstringAppendV(s, format, vaList);
    if(isAutoEndl)
    {
        zfstringAppend(s, "\n");
    }

    _ZFP_ZFCoreLogOutputCallback(s.cString());
}

void zfCoreCriticalErrorCallbackPrepareAdd(ZF_IN ZFCoreCriticalErrorCallback callback)
{
    if(callback != zfnull)
    {
        _ZFP_ZFCoreCriticalErrorPrepareCallbacks.add(callback);
    }
}
void zfCoreCriticalErrorCallbackPrepareRemove(ZF_IN ZFCoreCriticalErrorCallback callback)
{
    _ZFP_ZFCoreCriticalErrorPrepareCallbacks.removeElement(callback);
}
void zfCoreCriticalErrorCallbackAdd(ZF_IN ZFCoreCriticalErrorCallback callback)
{
    if(callback != zfnull)
    {
        _ZFP_ZFCoreCriticalErrorCallbacks.add(callback);
    }
}
void zfCoreCriticalErrorCallbackRemove(ZF_IN ZFCoreCriticalErrorCallback callback)
{
    _ZFP_ZFCoreCriticalErrorCallbacks.removeElement(callback);
}
void _ZFP_zfCoreCriticalErrorPrepare(ZF_IN const ZFCallerInfo &callerInfo)
{
    const ZFCoreArrayPOD<ZFCoreCriticalErrorCallback> &criticalErrorCallbacks = _ZFP_ZFCoreCriticalErrorPrepareCallbacks;
    for(zfindex i = 0; i < criticalErrorCallbacks.count(); ++i)
    {
        criticalErrorCallbacks[i](callerInfo);
    }
}
void _ZFP_zfCoreCriticalError(ZF_IN const ZFCallerInfo &callerInfo)
{
    const ZFCoreArrayPOD<ZFCoreCriticalErrorCallback> &criticalErrorCallbacks = _ZFP_ZFCoreCriticalErrorCallbacks;
    for(zfindex i = 0; i < criticalErrorCallbacks.count(); ++i)
    {
        criticalErrorCallbacks[i](callerInfo);
    }

    abort();
}

ZF_NAMESPACE_GLOBAL_END

