#include "ZFUIKit_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUIKit_ZFUITextEdit_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFUIKit_ZFUITextEdit_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView);
        ZFUIWindow *window = zfnull;
        ZFUIView *container = zfnull;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        ZFLISTENER_LOCAL(onClickReturn, {
            ZFUITextEdit *textEdit = listenerData.sender()->toAny();
            textEdit->viewFocusRequest(zffalse);
        })

        zfblockedAlloc(ZFUITextEdit, view);
        container->childAdd(view);
        view->layoutParam()->sizeParam(ZFUISizeParamFillWrap());
        view->layoutParam()->layoutMargin(ZFUIMarginMake(40));
        view->viewBackgroundColor(ZFUIColorRed());
        view->observerAdd(ZFUITextEdit::EventTextOnReturnClick(), onClickReturn);
        view->textPlaceHolder()->text("first text edit");

        zfblockedAlloc(ZFUITextEdit, view2);
        container->childAdd(view2);
        view2->layoutParam()->sizeParam(ZFUISizeParamFillFill());
        view2->layoutParam()->layoutMargin(ZFUIMarginMake(40, 100, 40, 40));
        view2->viewBackgroundColor(ZFUIColorBlue());
        view2->observerAdd(ZFUITextEdit::EventTextOnReturnClick(), onClickReturn);
        view2->textPlaceHolder()->text("second text edit");

        this->prepareSettingButton(window, view);
    }

private:
    void prepareSettingButton(ZF_IN ZFUIWindow *window,
                              ZF_IN ZFUITextEdit *view)
    {
        zfblockedAlloc(ZFArray, settings);
        ZFUIKit_test_prepareSettingForBoolProperty(settings, view, ZFPropertyAccess(ZFUITextEdit, textEditEnable));

        ZFUIKit_test_prepareSettingButtonWithTestWindow(window, settings);
    }
};
ZFOBJECT_REGISTER(ZFUIKit_ZFUITextEdit_test)

ZF_NAMESPACE_GLOBAL_END

