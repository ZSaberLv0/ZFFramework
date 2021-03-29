#include "ZFUIWidget_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUIWidget_ZFUIDialogForChoice_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFUIWidget_ZFUIDialogForChoice_test, ZFFramework_test_TestCase)

    ZFPROPERTY_RETAIN_READONLY(ZFUIDialogForChoice *, dialog, zflineAlloc(ZFUIDialogForChoice))

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
        ZFLISTENER_LOCAL(choiceOnConfirm, {
            ZFUIDialogForChoice *dialog = userData->objectHolded();
            zfLogT() << "onConfirm" << dialog->choiceSelectedNameList();
        })
        ZFLISTENER_LOCAL(choiceOnChange, {
            ZFUIDialogForChoice *dialog = userData->objectHolded();
            zfLogT() << "onChange" << dialog->choiceSelectedNameList();
        })
        this->dialog()->observerAdd(ZFUIDialogForChoice::EventChoiceOnConfirm(), choiceOnConfirm, this->dialog()->objectHolder());
        this->dialog()->observerAdd(ZFUIDialogForChoice::EventChoiceOnChange(), choiceOnChange, this->dialog()->objectHolder());
        this->dialog()->dialogTitleText("i'm title");

        this->dialog()->choiceAdd("id 0", "choice 0");
        this->dialog()->choiceAdd("id 1", "choice 1");
        this->dialog()->choiceAdd("id 2", "choice 2");

        zfblockedAlloc(ZFUIKit_test_Button, showButton);
        container->childAdd(showButton);
        showButton->layoutParam()->sizeParam(ZFUISizeParamFillFill());
        ZFLISTENER_LOCAL(showButtonOnClick, {
            userData->objectHolded<ZFUIDialogForChoice *>()->dialogShow();
        })
        showButton->observerAdd(ZFUIButton::EventButtonOnClick(), showButtonOnClick, this->dialog()->objectHolder());

        this->prepareSettingButton(window, this->dialog());
    }

private:
    void prepareSettingButton(ZF_IN ZFUIWindow *window,
                              ZF_IN ZFUIDialogForChoice *dialog)
    {
        zfblockedAlloc(ZFArray, settings);

        ZFUIKit_test_prepareSettingButtonWithTestWindow(window, settings);
    }
};
ZFOBJECT_REGISTER(ZFUIWidget_ZFUIDialogForChoice_test)

ZF_NAMESPACE_GLOBAL_END

