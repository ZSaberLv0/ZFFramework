#include "ZFUIViewBlinkWhenFocus.h"

#include "ZFUIWindow.h"
#include "ZFUISysWindow.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIViewBlinkWhenFocus_AutoPause_DataHolder, ZFLevelZFFrameworkLow)
{
    this->doActionListener = ZFCallbackForFunc(zfself::doAction);
    ZFObjectGlobalEventObserver().observerAdd(ZFUIWindow::EventWindowOnShow(), this->doActionListener);
    ZFObjectGlobalEventObserver().observerAdd(ZFUIWindow::EventWindowOnHide(), this->doActionListener);
    ZFObjectGlobalEventObserver().observerAdd(ZFUISysWindow::EventSysWindowOnResume(), this->doActionListener);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIViewBlinkWhenFocus_AutoPause_DataHolder)
{
    ZFObjectGlobalEventObserver().observerRemove(ZFUIWindow::EventWindowOnShow(), this->doActionListener);
    ZFObjectGlobalEventObserver().observerRemove(ZFUIWindow::EventWindowOnHide(), this->doActionListener);
    ZFObjectGlobalEventObserver().observerRemove(ZFUISysWindow::EventSysWindowOnResume(), this->doActionListener);
}
public:
    ZFListener doActionListener;
    static ZFLISTENER_PROTOTYPE_EXPAND(doAction)
    {
        ZFUIViewBlinkWhenFocusAutoApplyPauseForTime();
    }
ZF_GLOBAL_INITIALIZER_END(ZFUIViewBlinkWhenFocus_AutoPause_DataHolder)

ZF_NAMESPACE_GLOBAL_END

