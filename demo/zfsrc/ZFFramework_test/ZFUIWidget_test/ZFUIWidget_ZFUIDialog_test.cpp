#include "ZFUIWidget_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUIWidget_ZFUIDialog_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFUIWidget_ZFUIDialog_test, ZFFramework_test_TestCase)

    ZFPROPERTY_RETAIN_READONLY(ZFUIDialogBasic *, dialog, zflineAlloc(ZFUIDialogBasic))

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
            zfLogT() << "onConfirm" << listenerData.sender();
        })
        this->dialog()->dialogButtonYes()->observerAdd(ZFUIButton::EventButtonOnClick(), yesOnClick);
        this->dialog()->dialogTitleText("i'm title");
        this->dialog()->dialogContentText("i'm content");

        zfblockedAlloc(ZFUIKit_test_Button, showButton);
        container->childAdd(showButton);
        showButton->layoutParam()->sizeParam(ZFUISizeParamFillFill());
        ZFLISTENER_LOCAL(showButtonOnClick, {
            userData->objectHolded<ZFUIDialog *>()->dialogShow();
        })
        showButton->observerAdd(ZFUIButton::EventButtonOnClick(), showButtonOnClick, this->dialog()->objectHolder());

        this->prepareSettingButton(window, this->dialog());
    }

private:
    void prepareSettingButton(ZF_IN ZFUIWindow *window,
                              ZF_IN ZFUIDialog *dialog)
    {
        zfblockedAlloc(ZFArrayEditable, settings);
        ZFUIKit_test_prepareSettingForBoolProperty(settings, dialog, ZFPropertyAccess(ZFUIDialog, dialogHideWhenTouchOutside));
        ZFUIKit_test_prepareSettingForBoolProperty(settings, dialog, ZFPropertyAccess(ZFUIDialog, dialogHideWhenClickBack));

        ZFUIKit_test_prepareSettingButtonWithTestWindow(window, settings);
    }
};
ZFOBJECT_REGISTER(ZFUIWidget_ZFUIDialog_test)

// ============================================================
#if 1
ZF_GLOBAL_INITIALIZER_INIT(ZFUIWidget_ZFUIDialog_debug_LogEvent)
{
    ZFLISTENER_LOCAL(dialogBeforeShow, {
        zfLogTrimT()
            << zfLogCurTimeString()
            << listenerData.sender()
            << "BeforeShow";
    })
    this->dialogBeforeShowListener = dialogBeforeShow;
    ZFObjectGlobalEventObserver().observerAdd(ZFUIDialog::EventDialogBeforeShow(), this->dialogBeforeShowListener);

    ZFLISTENER_LOCAL(dialogAfterShow, {
        zfLogTrimT()
            << zfLogCurTimeString()
            << listenerData.sender()
            << "AfterShow";
    })
    this->dialogAfterShowListener = dialogAfterShow;
    ZFObjectGlobalEventObserver().observerAdd(ZFUIDialog::EventDialogAfterShow(), this->dialogAfterShowListener);

    ZFLISTENER_LOCAL(dialogBeforeHide, {
        zfLogTrimT()
            << zfLogCurTimeString()
            << listenerData.sender()
            << "BeforeHide";
    })
    this->dialogBeforeHideListener = dialogBeforeHide;
    ZFObjectGlobalEventObserver().observerAdd(ZFUIDialog::EventDialogBeforeHide(), this->dialogBeforeHideListener);

    ZFLISTENER_LOCAL(dialogAfterHide, {
        zfLogTrimT()
            << zfLogCurTimeString()
            << listenerData.sender()
            << "AfterHide";
    })
    this->dialogAfterHideListener = dialogAfterHide;
    ZFObjectGlobalEventObserver().observerAdd(ZFUIDialog::EventDialogAfterHide(), this->dialogAfterHideListener);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIWidget_ZFUIDialog_debug_LogEvent)
{
    ZFObjectGlobalEventObserver().observerRemove(ZFUIDialog::EventDialogBeforeShow(), this->dialogBeforeShowListener);
    ZFObjectGlobalEventObserver().observerRemove(ZFUIDialog::EventDialogAfterShow(), this->dialogAfterShowListener);
    ZFObjectGlobalEventObserver().observerRemove(ZFUIDialog::EventDialogBeforeHide(), this->dialogBeforeHideListener);
    ZFObjectGlobalEventObserver().observerRemove(ZFUIDialog::EventDialogAfterHide(), this->dialogAfterHideListener);
}
private:
    ZFListener dialogBeforeShowListener;
    ZFListener dialogAfterShowListener;
    ZFListener dialogBeforeHideListener;
    ZFListener dialogAfterHideListener;
ZF_GLOBAL_INITIALIZER_END(ZFUIWidget_ZFUIDialog_debug_LogEvent)
#endif

ZF_NAMESPACE_GLOBAL_END

