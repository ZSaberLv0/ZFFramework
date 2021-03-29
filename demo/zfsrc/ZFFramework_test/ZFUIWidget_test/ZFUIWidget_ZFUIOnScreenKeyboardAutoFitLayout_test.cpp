#include "ZFUIWidget_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUIWidget_ZFUIOnScreenKeyboardAutoFitLayout_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFUIWidget_ZFUIOnScreenKeyboardAutoFitLayout_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView);
        ZFUIWindow *window = zfnull;
        ZFUIView *container = zfnull;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        zfblockedAlloc(ZFUIOnScreenKeyboardAutoFitLayout, layout);
        container->childAdd(layout, ZFUISizeParamFillFill());
        this->prepareSettingButton(window, layout);

        layout->viewBackgroundColor(ZFUIColorGreen());
        for(zfindex i = 0; i < 3; ++i)
        {
            zfblockedAlloc(ZFUITextEdit, view);
            layout->childAdd(view);
            view->layoutParam()->sizeParam(ZFUISizeParamFillWrap());
            view->viewBackgroundColor(ZFUIColorRandom());
            view->layoutParam()->layoutMargin(ZFUIMarginMake(10));
        }
        layout->childAtIndex(0)->layoutParam()->layoutAlign(ZFUIAlign::e_TopInner);
        layout->childAtIndex(1)->layoutParam()->layoutAlign(ZFUIAlign::e_Center);
        layout->childAtIndex(2)->layoutParam()->layoutAlign(ZFUIAlign::e_BottomInner);
    }

private:
    void prepareSettingButton(ZF_IN ZFUIWindow *window,
                              ZF_IN ZFUIOnScreenKeyboardAutoFitLayout *layout)
    {
        zfblockedAlloc(ZFArray, settings);

        ZFUIKit_test_prepareSettingForBoolProperty(settings, layout, ZFPropertyAccess(ZFUIOnScreenKeyboardAutoFitLayout, autoFitEnable));
        ZFUIKit_test_prepareSettingForBoolProperty(settings, layout, ZFPropertyAccess(ZFUIOnScreenKeyboardAutoFitLayout, autoFitFocusedViewToVisible));
        ZFUIKit_test_prepareSettingForBoolProperty(settings, layout, ZFPropertyAccess(ZFUIOnScreenKeyboardAutoFitLayout, autoFitScrollEnable));

        ZFUIKit_test_prepareSettingButtonWithTestWindow(window, settings);
    }
};
ZFOBJECT_REGISTER(ZFUIWidget_ZFUIOnScreenKeyboardAutoFitLayout_test)

ZF_NAMESPACE_GLOBAL_END

