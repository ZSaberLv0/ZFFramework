#include "ZFThread_observerNotifyInMainThread.h"

#include "ZFCore/ZFSTLWrapper/zfstl_list.h"
#include "ZFCore/ZFSTLWrapper/zfstl_string.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclass _ZFP_I_ZFObserverNotifyInMainThreadTaskData : zfextends ZFObject
{
    ZFOBJECT_DECLARE_WITH_CUSTOM_CTOR(_ZFP_I_ZFObserverNotifyInMainThreadTaskData, ZFObject)

    ZFALLOC_CACHE_RELEASE({
        cache->removeAll();
    })

public:
    _ZFP_I_ZFObserverNotifyInMainThreadTaskData(void)
    : obj(zfnull)
    , customSender(zfnull)
    , param0(zfnull)
    , param1(zfnull)
    {
    }

public: // use direct pointer for performance
    ZFObject *obj;
    ZFObject *customSender;
    zfidentity eventId;
    ZFObject *param0;
    ZFObject *param1;

private:
    inline void removeAll(void)
    {
        zfRetainChange(this->obj, zfnull);
        zfRetainChange(this->customSender, zfnull);
        zfRetainChange(this->param0, zfnull);
        zfRetainChange(this->param1, zfnull);
    }

public:
    zfoverride
    virtual void objectOnDealloc(void)
    {
        this->removeAll();
        zfsuper::objectOnDealloc();
    }
};
ZFOBJECT_REGISTER(_ZFP_I_ZFObserverNotifyInMainThreadTaskData)

// ============================================================
static ZFListener *_ZFP_ZFObserverNotifyInMainThreadCallback = zfnull;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFObserverNotifyInMainThreadDataHolder, ZFLevelZFFrameworkEssential)
{
    this->callback = ZFCallbackForFunc(zfself::callbackAction);
    _ZFP_ZFObserverNotifyInMainThreadCallback = &this->callback;
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFObserverNotifyInMainThreadDataHolder)
{
    _ZFP_ZFObserverNotifyInMainThreadCallback = zfnull;
}
private:
    ZFListener callback;
    static void callbackAction(ZF_IN const ZFListenerData &listenerData, ZF_IN ZFObject *userData)
    {
        _ZFP_I_ZFObserverNotifyInMainThreadTaskData *taskData = userData->to<_ZFP_I_ZFObserverNotifyInMainThreadTaskData *>();
        taskData->obj->observerNotifyWithCustomSender(
            taskData->customSender,
            taskData->eventId,
            taskData->param0,
            taskData->param1);
    }
ZF_GLOBAL_INITIALIZER_END(ZFObserverNotifyInMainThreadDataHolder)

ZFMETHOD_FUNC_DEFINE_5(zfidentity, ZFObserverNotifyInMainThreadWithCustomSender,
                       ZFMP_IN(ZFObject *, customSender),
                       ZFMP_IN(ZFObject *, obj),
                       ZFMP_IN(zfidentity, eventId),
                       ZFMP_IN_OPT(ZFObject *, param0, zfnull),
                       ZFMP_IN_OPT(ZFObject *, param1, zfnull))
{
    if(obj == zfnull)
    {
        return zfidentityInvalid();
    }
    if(_ZFP_ZFObserverNotifyInMainThreadCallback)
    {
        zfblockedAllocWithCache(_ZFP_I_ZFObserverNotifyInMainThreadTaskData, taskData);
        zfRetainChange(taskData->obj, obj);
        zfRetainChange(taskData->customSender, customSender);
        taskData->eventId = eventId;
        zfRetainChange(taskData->param0, param0);
        zfRetainChange(taskData->param1, param1);
        return ZFThreadTaskRequest(*_ZFP_ZFObserverNotifyInMainThreadCallback,
            taskData,
            zfnull,
            zfnull,
            zfnull,
            ZFThreadTaskRequestMergeCallbackDoNotMerge());
    }
    else
    {
        obj->observerNotifyWithCustomSender(customSender, eventId, param0, param1);
        return zfidentityInvalid();
    }
}
ZFMETHOD_FUNC_INLINE_DEFINE_4(zfidentity, ZFObserverNotifyInMainThread,
                              ZFMP_IN(ZFObject *, obj),
                              ZFMP_IN(zfidentity, eventId),
                              ZFMP_IN_OPT(ZFObject *, param0, zfnull),
                              ZFMP_IN_OPT(ZFObject *, param1, zfnull))
ZFMETHOD_FUNC_DEFINE_1(void, ZFObserverNotifyInMainThreadCancel,
                       ZFMP_IN(zfidentity, taskId))
{
    ZFThreadTaskCancel(taskId);
}

ZF_NAMESPACE_GLOBAL_END

