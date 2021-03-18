#include "ZFUITextEditWidget.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFSTYLE_DEFAULT_DEFINE(ZFUITextEditWidget)

// ============================================================
// ZFUITextEditWidget
ZFOBJECT_REGISTER(ZFUITextEditWidget)

static void _ZFP_ZFUITextEditWidget_updateClearButton(ZF_IN ZFUITextEditWidget *textEditWidget)
{
    if(textEditWidget->textEditClearButtonAutoEnable())
    {
        textEditWidget->textEditClearButton()->to<ZFUIView *>()->viewVisible(
            !textEditWidget->text().isEmpty());
    }
}

ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUITextEditWidget, zfbool, textEditClearButtonAutoEnable)
{
    _ZFP_ZFUITextEditWidget_updateClearButton(this);
}

void ZFUITextEditWidget::objectOnInit(void)
{
    zfsuper::objectOnInit();

    ZFUIImageView *textEditBackgroundView = this->textEditBackgroundView()->to<ZFUIImageView *>();
    this->internalImplViewAdd(textEditBackgroundView);
    textEditBackgroundView->layoutParam()->sizeParam(ZFUISizeParamFillFill());
    textEditBackgroundView->serializableRefLayoutParam()->sizeParam(ZFUISizeParamFillFill());

    ZFUIButtonBasic *textEditClearButton = this->textEditClearButton()->to<ZFUIButtonBasic *>();
    this->internalBgViewAdd(textEditClearButton);
    textEditClearButton->layoutParam()->layoutAlign(ZFUIAlign::e_RightInner);
    textEditClearButton->serializableRefLayoutParam()->layoutAlign(ZFUIAlign::e_RightInner);

    _ZFP_ZFUITextEditWidget_updateClearButton(this);
}
void ZFUITextEditWidget::objectOnDealloc(void)
{
    zfsuper::objectOnDealloc();
}

void ZFUITextEditWidget::layoutOnLayoutPrepare(ZF_IN const ZFUIRect &bounds)
{
    zfsuper::layoutOnLayoutPrepare(bounds);
    this->nativeImplViewMarginUpdate();
}
void ZFUITextEditWidget::nativeImplViewMarginImplUpdate(ZF_IN_OUT ZFUIMargin &nativeImplViewMargin)
{
    zfsuper::nativeImplViewMarginImplUpdate(nativeImplViewMargin);

    ZFUIImageView *textEditBackgroundView = this->textEditBackgroundView()->to<ZFUIImageView *>();
    if(textEditBackgroundView->viewVisible())
    {
        ZFUIImage *image = textEditBackgroundView->image();
        if(image != zfnull)
        {
            ZFUIMarginInc(nativeImplViewMargin, nativeImplViewMargin, image->imageNinePatch());
        }
    }

    ZFUIButtonBasic *textEditClearButton = this->textEditClearButton()->to<ZFUIButtonBasic *>();
    if(textEditClearButton->viewVisible())
    {
        textEditClearButton->layoutMeasure(ZFUISizeInvalid(), ZFUISizeParamWrapWrap());
        zfint size = ZFUIMarginGetWidth(textEditClearButton->layoutParam()->layoutMargin()) + textEditClearButton->layoutMeasuredSize().width;
        if(ZFBitTest(textEditClearButton->layoutParam()->layoutAlign(), ZFUIAlign::e_LeftInner))
        {
            nativeImplViewMargin.left += size;
        }
        else
        {
            nativeImplViewMargin.right += size;
        }
    }
}
zfbool ZFUITextEditWidget::internalViewShouldLayout(ZF_IN ZFUIView *internalView)
{
    if(internalView == this->textEditClearButton()->toObject())
    {
        return zffalse;
    }
    return zfsuper::internalViewShouldLayout(internalView);
}
void ZFUITextEditWidget::internalBgViewOnLayout(ZF_IN const ZFUIRect &bounds)
{
    zfsuper::internalBgViewOnLayout(bounds);

    ZFUIButtonBasic *textEditClearButton = this->textEditClearButton()->to<ZFUIButtonBasic *>();
    if(!textEditClearButton->viewVisible())
    {
        return ;
    }

    ZFUIMargin margin = ZFUIMarginZero();
    ZFUIImageView *textEditBackgroundView = this->textEditBackgroundView()->to<ZFUIImageView *>();
    if(textEditBackgroundView->viewVisible())
    {
        ZFUIImage *image = textEditBackgroundView->image();
        if(image != zfnull)
        {
            margin = image->imageNinePatch();
        }
    }
    textEditClearButton->viewFrame(ZFUIAlignApply(
        textEditClearButton->layoutParam()->layoutAlign(),
        bounds,
        textEditClearButton->layoutMeasuredSize(),
        margin));
}
void ZFUITextEditWidget::textOnChange(ZF_IN const zfchar *oldText)
{
    zfsuper::textOnChange(oldText);
    _ZFP_ZFUITextEditWidget_updateClearButton(this);
}

ZF_NAMESPACE_GLOBAL_END

