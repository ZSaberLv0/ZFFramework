#include "ZFUIWidget_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUIWidget_ZFUIOnScreenKeyboardAutoResize_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFUIWidget_ZFUIOnScreenKeyboardAutoResize_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView);
        ZFUIWindow *window = zfnull;
        ZFUIView *container = zfnull;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        ZFUIOnScreenKeyboardAutoResizeStart(window);

        container->viewBackgroundColor(ZFUIColorGreen());
        for(zfindex i = 0; i < 3; ++i)
        {
            zfblockedAlloc(ZFUITextEdit, view);
            container->childAdd(view);
            view->layoutParam()->sizeParam(ZFUISizeParamFillWrap());
            view->viewBackgroundColor(ZFUIColorRandom());
            view->layoutParam()->layoutMargin(ZFUIMarginMake(10));
        }
        container->childAtIndex(0)->layoutParam()->layoutAlign(ZFUIAlign::e_TopInner);
        container->childAtIndex(1)->layoutParam()->layoutAlign(ZFUIAlign::e_Center);
        container->childAtIndex(2)->layoutParam()->layoutAlign(ZFUIAlign::e_BottomInner);
    }
};
ZFOBJECT_REGISTER(ZFUIWidget_ZFUIOnScreenKeyboardAutoResize_test)

ZF_NAMESPACE_GLOBAL_END

