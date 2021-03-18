#include "ZFUIKit_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUIKit_ZFUIViewPositionOnScreen_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFUIKit_ZFUIViewPositionOnScreen_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView);
        ZFFramework_test_protocolCheck(ZFUIViewPositionOnScreen);
        ZFUIWindow *window = zfnull;
        ZFUIView *container = zfnull;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        zfblockedAlloc(ZFUIKit_test_Button, button);
        container->childAdd(button);
        button->layoutParam()->layoutAlign(ZFUIAlign::e_Center);
        button->buttonLabelText("click me");

        ZFLISTENER_LOCAL(buttonOnClick, {
            zfLogTrimT() << "window size:" << ZFUIRectGetSize(ZFUIViewUtil::viewRoot(listenerData.sender<ZFUIView *>())->viewFrame());
            zfLogTrimT() << "clicked view's position:" << ZFUIViewPositionOnScreen(listenerData.sender()->toAny());
        })
        button->observerAdd(ZFUIButton::EventButtonOnClick(), buttonOnClick);
    }
};
ZFOBJECT_REGISTER(ZFUIKit_ZFUIViewPositionOnScreen_test)

ZF_NAMESPACE_GLOBAL_END

