#include "ZFUIWidget_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUIWidget_ZFUIDialogForInput_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFUIWidget_ZFUIDialogForInput_test, ZFFramework_test_TestCase)

    ZFPROPERTY_RETAIN_READONLY(ZFUIDialogForInput *, dialog, zflineAlloc(ZFUIDialogForInput))

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView);
        ZFUIWindow *window = zfnull;
        ZFUIView *container = zfnull;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        this->dialog()->dialogApplyAutoHide(this->dialog()->dialogButtonCancel());
        this->dialog()->dialogButtonCancelText("cancel");
        this->dialog()->dialogApplyAutoHide(this->dialog()->dialogButtonYes());
        this->dialog()->dialogButtonYesText("confirm");
        ZFLISTENER_LOCAL(yesOnClick, {
            ZFUIDialogForInput *dialog = userData->objectHolded();
            zfLogT() << "onConfirm" << listenerData.sender()
                << "text:" << dialog->inputText();
        })
        this->dialog()->dialogButtonYes()->observerAdd(ZFUIButton::EventButtonOnClick(), yesOnClick, this->dialog()->objectHolder());
        this->dialog()->dialogTitleText("i'm title");
        this->dialog()->dialogContentText("i'm hint");
        this->dialog()->inputHintText("input something here");

        zfblockedAlloc(ZFUIKit_test_Button, showButton);
        container->childAdd(showButton);
        showButton->layoutParam()->sizeParam(ZFUISizeParamFillFill());
        ZFLISTENER_LOCAL(showButtonOnClick, {
            userData->objectHolded<ZFUIDialogForInput *>()->dialogShow();
        })
        showButton->observerAdd(ZFUIButton::EventButtonOnClick(), showButtonOnClick, this->dialog()->objectHolder());

        this->prepareSettingButton(window, this->dialog());
    }

private:
    void prepareSettingButton(ZF_IN ZFUIWindow *window,
                              ZF_IN ZFUIDialogForInput *dialog)
    {
        zfblockedAlloc(ZFArray, settings);

        ZFUIKit_test_prepareSettingButtonWithTestWindow(window, settings);
    }
};
ZFOBJECT_REGISTER(ZFUIWidget_ZFUIDialogForInput_test)

ZF_NAMESPACE_GLOBAL_END

