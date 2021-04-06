#include "ZFUIKit_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

#if 1
ZF_GLOBAL_INITIALIZER_INIT(ZFUIViewFocusState_test)
{
    this->listener = ZFCallbackForFunc(zfself::viewFocusOnChange);
    ZFGlobalObserver().observerAdd(ZFUIView::EventViewFocusOnChange(), this->listener);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIViewFocusState_test)
{
    ZFGlobalObserver().observerRemove(ZFUIView::EventViewFocusOnChange(), this->listener);
}
private:
    ZFListener listener;
    static void viewFocusOnChange(ZF_IN const ZFListenerData &listenerData, ZF_IN ZFObject *userData)
    {
        zfLogTrimT() << "[ZFUIViewFocus]"
            << listenerData.sender()
            << "changed to" << listenerData.sender<ZFUIView *>()->viewFocused();
    }
ZF_GLOBAL_INITIALIZER_END(ZFUIViewFocusState_test)
#endif

ZF_NAMESPACE_GLOBAL_END

