#include "ZFUIKit_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

#if 0
ZF_GLOBAL_INITIALIZER_INIT(ZFUIKit_ZFUIView_debug_LogUIEvent)
{
    this->onUIEventListener = ZFCallbackForFunc(zfself::onUIEvent);
    ZFGlobalObserver().observerAdd(
            ZFUIView::EventViewOnEvent(),
            this->onUIEventListener
        );
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIKit_ZFUIView_debug_LogUIEvent)
{
    ZFGlobalObserver().observerRemove(
            ZFUIView::EventViewOnEvent(),
            this->onUIEventListener
        );
}
private:
    ZFListener onUIEventListener;
    static void onUIEvent(ZF_IN const ZFListenerData &listenerData, ZF_IN ZFObject *userData)
    {
        zfLogTrimT() << listenerData.sender()->objectInfoOfInstance() << listenerData.param0();
    }
ZF_GLOBAL_INITIALIZER_END(ZFUIKit_ZFUIView_debug_LogUIEvent)
#endif

ZF_NAMESPACE_GLOBAL_END

