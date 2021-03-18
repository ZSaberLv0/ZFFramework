#include "ZFUtility_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

#if 0
ZF_GLOBAL_INITIALIZER_INIT(ZFUtility_ZFAnimation_debug_LogEvent)
{
    this->taskOwner = zfAlloc(ZFObject);

    ZFLISTENER_LOCAL(aniOnAlloc, {
        if(listenerData.sender()->classData()->classIsTypeOf(ZFAnimation::ClassData()))
        {
            zfLogTrimT() << zfLogCurTimeString() << listenerData.sender() << "alloc";
        }
    })
    ZFObjectGlobalEventObserver().observerAdd(ZFObject::EventObjectAfterAlloc(), aniOnAlloc);

    ZFLISTENER_LOCAL(aniOnDealloc, {
        if(listenerData.sender()->classData()->classIsTypeOf(ZFAnimation::ClassData()))
        {
            zfLogTrimT() << zfLogCurTimeString() << listenerData.sender() << "dealloc";
        }
    })
    ZFObjectGlobalEventObserver().observerAdd(ZFObject::EventObjectBeforeDealloc(), aniOnDealloc);

    ZFLISTENER_LOCAL(aniOnInvalid, {
        zfLogTrimT() << zfLogCurTimeString() << listenerData.sender() << "invalid";
    })
    ZFObjectGlobalEventObserver().observerAdd(ZFAnimation::EventAniOnInvalid(), aniOnInvalid);

    ZFLISTENER_LOCAL(aniOnStart, {
        zfLogTrimT() << zfLogCurTimeString() << listenerData.sender() << "start";
    })
    ZFObjectGlobalEventObserver().observerAdd(ZFAnimation::EventAniOnStart(), aniOnStart);

    ZFLISTENER_LOCAL(aniOnStop, {
        zfLogTrimT() << zfLogCurTimeString() << listenerData.sender() << "stop";
    })
    ZFObjectGlobalEventObserver().observerAdd(ZFAnimation::EventAniOnStop(), aniOnStop);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUtility_ZFAnimation_debug_LogEvent)
{
    ZFObjectGlobalEventObserver().observerRemoveByOwner(this->taskOwner);
    zfRelease(this->taskOwner);
}
private:
    ZFObject *taskOwner;
ZF_GLOBAL_INITIALIZER_END(ZFUtility_ZFAnimation_debug_LogEvent)
#endif

ZF_NAMESPACE_GLOBAL_END

