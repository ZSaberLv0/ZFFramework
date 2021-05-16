#include "ZFUIDialogForInput.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFSTYLE_DEFAULT_DEFINE(ZFUIDialogForInput)

ZFPROPERTY_ON_INIT_DEFINE(ZFUIDialogForInput, ZFUITextEditWidget *, inputView)
{
    zfblockedAlloc(ZFUITextEditWidget, inputView);
    propertyValue = inputView;
    inputView->to<ZFUIView *>()->viewSizeMin(ZFUISizeMake(
            ZFUIGlobalStyle::DefaultStyle()->itemSizeDialogWidth(),
            ZFUIGlobalStyle::DefaultStyle()->itemSizeControl()
        ));
    inputView->textEditConfirmWhenLostFocus(zffalse);
}

ZFMETHOD_DEFINE_1(ZFUIDialogForInput, void, inputHintText,
                  ZFMP_IN(const zfchar *, text))
{
    this->inputView()->textPlaceHolder()->text(text);
}
ZFMETHOD_DEFINE_0(ZFUIDialogForInput, const zfchar *, inputHintText)
{
    return this->inputView()->textPlaceHolder()->text();
}

ZFMETHOD_DEFINE_1(ZFUIDialogForInput, void, inputText,
                  ZFMP_IN(const zfchar *, text))
{
    this->inputView()->text(text);
}
ZFMETHOD_DEFINE_0(ZFUIDialogForInput, const zfchar *, inputText)
{
    return this->inputView()->text();
}

void ZFUIDialogForInput::objectOnInit(void)
{
    zfsuper::objectOnInit();

    this->dialogContentContainer()->childAdd(this->inputView());

    ZFLISTENER_LOCAL(inputOnConfirm, {
        ZFUIDialogForInput *dialog = userData->objectHolded();
        ZFUIButton *button = dialog->dialogButtonYes(zffalse);
        if(button != zfnull)
        {
            button->buttonSimulateClick();
        }
    })
    this->inputView()->observerAdd(ZFUITextEdit::EventTextOnEditConfirm(), inputOnConfirm, this->objectHolder());
}
void ZFUIDialogForInput::objectOnDealloc(void)
{
    zfsuper::objectOnDealloc();
}

ZF_NAMESPACE_GLOBAL_END

