#include "ZFUIButtonBasic.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// _ZFP_ZFUIButtonBasicPrivate
zfclassNotPOD _ZFP_ZFUIButtonBasicPrivate
{
public:
    ZFUIButtonBasic *pimplOwner;
    ZFUITextView *buttonLabel;
    ZFUIImageView *buttonIcon;
    ZFUIImageView *buttonBackground;

public:
    _ZFP_ZFUIButtonBasicPrivate(void)
    : pimplOwner(zfnull)
    , buttonLabel(zfnull)
    , buttonIcon(zfnull)
    , buttonBackground(zfnull)
    {
    }

public:
    void updateButtonLabel(void)
    {
        const zfchar *value = this->pimplOwner->buttonLabelStyle(this->pimplOwner->buttonState())->text();
        if(*value == '\0')
        {
            if(this->pimplOwner->buttonState() == ZFUIButtonState::e_Checked)
            {
                value = this->pimplOwner->buttonLabelStyleHighlighted()->text();
            }
            else if(this->pimplOwner->buttonState() == ZFUIButtonState::e_CheckedHighlighted)
            {
                value = this->pimplOwner->buttonLabelStyleChecked()->text();
                if(*value == '\0')
                {
                    value = this->pimplOwner->buttonLabelStyleHighlighted()->text();
                }
            }
            if(*value == '\0')
            {
                value = this->pimplOwner->buttonLabelStyleNormal()->text();
            }
        }
        this->buttonLabel->text(value);
    }
    void updateButtonIcon(void)
    {
        ZFUIImage *value = this->pimplOwner->buttonIconStyle(this->pimplOwner->buttonState())->image();
        if(value == zfnull)
        {
            if(this->pimplOwner->buttonState() == ZFUIButtonState::e_Checked)
            {
                value = this->pimplOwner->buttonIconStyleHighlighted()->image();
            }
            else if(this->pimplOwner->buttonState() == ZFUIButtonState::e_CheckedHighlighted)
            {
                value = this->pimplOwner->buttonIconStyleChecked()->image();
                if(value == zfnull)
                {
                    value = this->pimplOwner->buttonIconStyleHighlighted()->image();
                }
            }
            if(value == zfnull)
            {
                value = this->pimplOwner->buttonIconStyleNormal()->image();
            }
        }
        this->buttonIcon->image(value);
    }
    void updateButtonBackground(void)
    {
        ZFUIImage *value = this->pimplOwner->buttonBackgroundStyle(this->pimplOwner->buttonState())->image();
        if(value == zfnull)
        {
            if(this->pimplOwner->buttonState() == ZFUIButtonState::e_Checked)
            {
                value = this->pimplOwner->buttonBackgroundStyleHighlighted()->image();
            }
            else if(this->pimplOwner->buttonState() == ZFUIButtonState::e_CheckedHighlighted)
            {
                value = this->pimplOwner->buttonBackgroundStyleChecked()->image();
                if(value == zfnull)
                {
                    value = this->pimplOwner->buttonBackgroundStyleHighlighted()->image();
                }
            }
            if(value == zfnull)
            {
                value = this->pimplOwner->buttonBackgroundStyleNormal()->image();
            }
        }
        this->buttonBackground->image(value);
    }

public:
    #define _ZFP_ZFUIButtonBasic_LISTENER_EXPAND_STYLE_CHANGED(T_Component, T_State) \
        static void button##T_Component##Style##T_State##Changed(ZF_IN const ZFListenerData &listenerData, ZF_IN ZFObject *userData) \
        { \
            const ZFProperty *property = listenerData.param0<v_ZFProperty *>()->zfv; \
            ZFUIButtonBasic *button = userData->objectHolded(); \
            button->prepareButton##T_Component(); \
            if(button->buttonState() == ZFUIButtonState::e_##T_State) \
            { \
                ZFPropertyCopy(property, button->button##T_Component()->toObject(), listenerData.sender()); \
            } \
            else if(property == ZFPropertyAccess(ZFUITextView, text)) \
            { \
                button->d->updateButton##T_Component(); \
            } \
        }

    _ZFP_ZFUIButtonBasic_LISTENER_EXPAND_STYLE_CHANGED(Label, Normal)
    _ZFP_ZFUIButtonBasic_LISTENER_EXPAND_STYLE_CHANGED(Label, Highlighted)
    _ZFP_ZFUIButtonBasic_LISTENER_EXPAND_STYLE_CHANGED(Label, Checked)
    _ZFP_ZFUIButtonBasic_LISTENER_EXPAND_STYLE_CHANGED(Label, CheckedHighlighted)
    _ZFP_ZFUIButtonBasic_LISTENER_EXPAND_STYLE_CHANGED(Label, Disabled)

    _ZFP_ZFUIButtonBasic_LISTENER_EXPAND_STYLE_CHANGED(Icon, Normal)
    _ZFP_ZFUIButtonBasic_LISTENER_EXPAND_STYLE_CHANGED(Icon, Highlighted)
    _ZFP_ZFUIButtonBasic_LISTENER_EXPAND_STYLE_CHANGED(Icon, Checked)
    _ZFP_ZFUIButtonBasic_LISTENER_EXPAND_STYLE_CHANGED(Icon, CheckedHighlighted)
    _ZFP_ZFUIButtonBasic_LISTENER_EXPAND_STYLE_CHANGED(Icon, Disabled)

    _ZFP_ZFUIButtonBasic_LISTENER_EXPAND_STYLE_CHANGED(Background, Normal)
    _ZFP_ZFUIButtonBasic_LISTENER_EXPAND_STYLE_CHANGED(Background, Highlighted)
    _ZFP_ZFUIButtonBasic_LISTENER_EXPAND_STYLE_CHANGED(Background, Checked)
    _ZFP_ZFUIButtonBasic_LISTENER_EXPAND_STYLE_CHANGED(Background, CheckedHighlighted)
    _ZFP_ZFUIButtonBasic_LISTENER_EXPAND_STYLE_CHANGED(Background, Disabled)
};
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIButtonBasicListenerHolder, ZFLevelZFFrameworkEssential)
{
    this->buttonLabelStyleNormalChangedListener = ZFCallbackForFunc(_ZFP_ZFUIButtonBasicPrivate::buttonLabelStyleNormalChanged);
    this->buttonLabelStyleHighlightedChangedListener = ZFCallbackForFunc(_ZFP_ZFUIButtonBasicPrivate::buttonLabelStyleHighlightedChanged);
    this->buttonLabelStyleCheckedChangedListener = ZFCallbackForFunc(_ZFP_ZFUIButtonBasicPrivate::buttonLabelStyleCheckedChanged);
    this->buttonLabelStyleCheckedHighlightedChangedListener = ZFCallbackForFunc(_ZFP_ZFUIButtonBasicPrivate::buttonLabelStyleCheckedHighlightedChanged);
    this->buttonLabelStyleDisabledChangedListener = ZFCallbackForFunc(_ZFP_ZFUIButtonBasicPrivate::buttonLabelStyleDisabledChanged);
    this->buttonIconStyleNormalChangedListener = ZFCallbackForFunc(_ZFP_ZFUIButtonBasicPrivate::buttonIconStyleNormalChanged);
    this->buttonIconStyleHighlightedChangedListener = ZFCallbackForFunc(_ZFP_ZFUIButtonBasicPrivate::buttonIconStyleHighlightedChanged);
    this->buttonIconStyleCheckedChangedListener = ZFCallbackForFunc(_ZFP_ZFUIButtonBasicPrivate::buttonIconStyleCheckedChanged);
    this->buttonIconStyleCheckedHighlightedChangedListener = ZFCallbackForFunc(_ZFP_ZFUIButtonBasicPrivate::buttonIconStyleCheckedHighlightedChanged);
    this->buttonIconStyleDisabledChangedListener = ZFCallbackForFunc(_ZFP_ZFUIButtonBasicPrivate::buttonIconStyleDisabledChanged);
    this->buttonBackgroundStyleNormalChangedListener = ZFCallbackForFunc(_ZFP_ZFUIButtonBasicPrivate::buttonBackgroundStyleNormalChanged);
    this->buttonBackgroundStyleHighlightedChangedListener = ZFCallbackForFunc(_ZFP_ZFUIButtonBasicPrivate::buttonBackgroundStyleHighlightedChanged);
    this->buttonBackgroundStyleCheckedChangedListener = ZFCallbackForFunc(_ZFP_ZFUIButtonBasicPrivate::buttonBackgroundStyleCheckedChanged);
    this->buttonBackgroundStyleCheckedHighlightedChangedListener = ZFCallbackForFunc(_ZFP_ZFUIButtonBasicPrivate::buttonBackgroundStyleCheckedHighlightedChanged);
    this->buttonBackgroundStyleDisabledChangedListener = ZFCallbackForFunc(_ZFP_ZFUIButtonBasicPrivate::buttonBackgroundStyleDisabledChanged);
}
public:
    ZFListener buttonLabelStyleNormalChangedListener;
    ZFListener buttonLabelStyleHighlightedChangedListener;
    ZFListener buttonLabelStyleCheckedChangedListener;
    ZFListener buttonLabelStyleCheckedHighlightedChangedListener;
    ZFListener buttonLabelStyleDisabledChangedListener;
    ZFListener buttonIconStyleNormalChangedListener;
    ZFListener buttonIconStyleHighlightedChangedListener;
    ZFListener buttonIconStyleCheckedChangedListener;
    ZFListener buttonIconStyleCheckedHighlightedChangedListener;
    ZFListener buttonIconStyleDisabledChangedListener;
    ZFListener buttonBackgroundStyleNormalChangedListener;
    ZFListener buttonBackgroundStyleHighlightedChangedListener;
    ZFListener buttonBackgroundStyleCheckedChangedListener;
    ZFListener buttonBackgroundStyleCheckedHighlightedChangedListener;
    ZFListener buttonBackgroundStyleDisabledChangedListener;
ZF_GLOBAL_INITIALIZER_END(ZFUIButtonBasicListenerHolder)

// ============================================================
// ZFUIButtonBasic
ZFOBJECT_REGISTER(ZFUIButtonBasic)
ZFSTYLE_DEFAULT_DEFINE(ZFUIButtonBasic)

#define _ZFP_ZFUIBUTTONBASIC_BUTTON_COMPONENT_DEFINE(Type, ComponentName, StateName) \
    ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIButtonBasic, Type, button##ComponentName##Style##StateName) \
    { \
        this->button##ComponentName##Style##StateName()->toObject()->observerAdd( \
            ZFObject::EventObjectPropertyValueOnUpdate(), \
            ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIButtonBasicListenerHolder)->button##ComponentName##Style##StateName##ChangedListener, \
            this->objectHolder()); \
    } \
    ZFPROPERTY_ON_DETACH_DEFINE(ZFUIButtonBasic, Type, button##ComponentName##Style##StateName) \
    { \
        this->button##ComponentName##Style##StateName()->toObject()->observerRemove( \
            ZFObject::EventObjectPropertyValueOnUpdate(), \
            ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIButtonBasicListenerHolder)->button##ComponentName##Style##StateName##ChangedListener); \
    }

_ZFP_ZFUIBUTTONBASIC_BUTTON_COMPONENT_DEFINE(ZFUITextView *, Label, Normal)
_ZFP_ZFUIBUTTONBASIC_BUTTON_COMPONENT_DEFINE(ZFUITextView *, Label, Highlighted)
_ZFP_ZFUIBUTTONBASIC_BUTTON_COMPONENT_DEFINE(ZFUITextView *, Label, Checked)
_ZFP_ZFUIBUTTONBASIC_BUTTON_COMPONENT_DEFINE(ZFUITextView *, Label, CheckedHighlighted)
_ZFP_ZFUIBUTTONBASIC_BUTTON_COMPONENT_DEFINE(ZFUITextView *, Label, Disabled)

_ZFP_ZFUIBUTTONBASIC_BUTTON_COMPONENT_DEFINE(ZFUIImageView *, Icon, Normal)
_ZFP_ZFUIBUTTONBASIC_BUTTON_COMPONENT_DEFINE(ZFUIImageView *, Icon, Highlighted)
_ZFP_ZFUIBUTTONBASIC_BUTTON_COMPONENT_DEFINE(ZFUIImageView *, Icon, Checked)
_ZFP_ZFUIBUTTONBASIC_BUTTON_COMPONENT_DEFINE(ZFUIImageView *, Icon, CheckedHighlighted)
_ZFP_ZFUIBUTTONBASIC_BUTTON_COMPONENT_DEFINE(ZFUIImageView *, Icon, Disabled)

_ZFP_ZFUIBUTTONBASIC_BUTTON_COMPONENT_DEFINE(ZFUIImageView *, Background, Normal)
_ZFP_ZFUIBUTTONBASIC_BUTTON_COMPONENT_DEFINE(ZFUIImageView *, Background, Highlighted)
_ZFP_ZFUIBUTTONBASIC_BUTTON_COMPONENT_DEFINE(ZFUIImageView *, Background, Checked)
_ZFP_ZFUIBUTTONBASIC_BUTTON_COMPONENT_DEFINE(ZFUIImageView *, Background, CheckedHighlighted)
_ZFP_ZFUIBUTTONBASIC_BUTTON_COMPONENT_DEFINE(ZFUIImageView *, Background, Disabled)

ZFPROPERTY_ON_INIT_DEFINE(ZFUIButtonBasic, ZFUITextView *, buttonLabelStyleNormal)
{
    ZFUITextView *v = propertyValue.to<ZFUITextView *>();
    v->textColor(ZFUIGlobalStyle::DefaultStyle()->controlColorNormal());

    this->prepareButtonLabel();
}
ZFPROPERTY_ON_INIT_DEFINE(ZFUIButtonBasic, ZFUITextView *, buttonLabelStyleHighlighted)
{
    ZFUITextView *v = propertyValue.to<ZFUITextView *>();
    v->textColor(ZFUIGlobalStyle::DefaultStyle()->controlColorHighlighted());

    this->prepareButtonLabel();
}
ZFPROPERTY_ON_INIT_DEFINE(ZFUIButtonBasic, ZFUITextView *, buttonLabelStyleChecked)
{
    ZFUITextView *v = propertyValue.to<ZFUITextView *>();
    v->textColor(ZFUIGlobalStyle::DefaultStyle()->controlColorChecked());

    this->prepareButtonLabel();
}
ZFPROPERTY_ON_INIT_DEFINE(ZFUIButtonBasic, ZFUITextView *, buttonLabelStyleCheckedHighlighted)
{
    ZFUITextView *v = propertyValue.to<ZFUITextView *>();
    v->textColor(ZFUIGlobalStyle::DefaultStyle()->controlColorCheckedHighlighted());

    this->prepareButtonLabel();
}
ZFPROPERTY_ON_INIT_DEFINE(ZFUIButtonBasic, ZFUITextView *, buttonLabelStyleDisabled)
{
    ZFUITextView *v = propertyValue.to<ZFUITextView *>();
    v->textColor(ZFUIGlobalStyle::DefaultStyle()->controlColorDisabled());

    this->prepareButtonLabel();
}
ZFPROPERTY_ON_INIT_DEFINE(ZFUIButtonBasic, ZFUIImageView *, buttonIconStyleNormal)
{
    this->prepareButtonIcon();
}
ZFPROPERTY_ON_INIT_DEFINE(ZFUIButtonBasic, ZFUIImageView *, buttonIconStyleHighlighted)
{
    this->prepareButtonIcon();
}
ZFPROPERTY_ON_INIT_DEFINE(ZFUIButtonBasic, ZFUIImageView *, buttonIconStyleChecked)
{
    this->prepareButtonIcon();
}
ZFPROPERTY_ON_INIT_DEFINE(ZFUIButtonBasic, ZFUIImageView *, buttonIconStyleCheckedHighlighted)
{
    this->prepareButtonIcon();
}
ZFPROPERTY_ON_INIT_DEFINE(ZFUIButtonBasic, ZFUIImageView *, buttonIconStyleDisabled)
{
    this->prepareButtonIcon();
}
ZFPROPERTY_ON_INIT_DEFINE(ZFUIButtonBasic, ZFUIImageView *, buttonBackgroundStyleNormal)
{
    this->prepareButtonBackground();
}
ZFPROPERTY_ON_INIT_DEFINE(ZFUIButtonBasic, ZFUIImageView *, buttonBackgroundStyleHighlighted)
{
    this->prepareButtonBackground();
}
ZFPROPERTY_ON_INIT_DEFINE(ZFUIButtonBasic, ZFUIImageView *, buttonBackgroundStyleChecked)
{
    this->prepareButtonBackground();
}
ZFPROPERTY_ON_INIT_DEFINE(ZFUIButtonBasic, ZFUIImageView *, buttonBackgroundStyleCheckedHighlighted)
{
    this->prepareButtonBackground();
}
ZFPROPERTY_ON_INIT_DEFINE(ZFUIButtonBasic, ZFUIImageView *, buttonBackgroundStyleDisabled)
{
    this->prepareButtonBackground();
}

ZFMETHOD_DEFINE_1(ZFUIButtonBasic, ZFUITextView *, buttonLabelStyle,
                  ZFMP_IN_OPT(ZFUIButtonStateEnum, forState, ZFUIButtonState::e_Normal))
{
    switch(forState)
    {
        case ZFUIButtonState::e_Normal:
            return this->buttonLabelStyleNormal();
        case ZFUIButtonState::e_Highlighted:
            return this->buttonLabelStyleHighlighted();
        case ZFUIButtonState::e_Checked:
            return this->buttonLabelStyleChecked();
        case ZFUIButtonState::e_CheckedHighlighted:
            return this->buttonLabelStyleCheckedHighlighted();
        case ZFUIButtonState::e_Disabled:
            return this->buttonLabelStyleDisabled();
        default:
            zfCoreCriticalShouldNotGoHere();
            return zfnull;
    }
}
ZFMETHOD_DEFINE_1(ZFUIButtonBasic, ZFUIImageView *, buttonIconStyle,
                  ZFMP_IN_OPT(ZFUIButtonStateEnum, forState, ZFUIButtonState::e_Normal))
{
    switch(forState)
    {
        case ZFUIButtonState::e_Normal:
            return this->buttonIconStyleNormal();
        case ZFUIButtonState::e_Highlighted:
            return this->buttonIconStyleHighlighted();
        case ZFUIButtonState::e_Checked:
            return this->buttonIconStyleChecked();
        case ZFUIButtonState::e_CheckedHighlighted:
            return this->buttonIconStyleCheckedHighlighted();
        case ZFUIButtonState::e_Disabled:
            return this->buttonIconStyleDisabled();
        default:
            zfCoreCriticalShouldNotGoHere();
            return zfnull;
    }
}
ZFMETHOD_DEFINE_1(ZFUIButtonBasic, ZFUIImageView *, buttonBackgroundStyle,
                  ZFMP_IN_OPT(ZFUIButtonStateEnum, forState, ZFUIButtonState::e_Normal))
{
    switch(forState)
    {
        case ZFUIButtonState::e_Normal:
            return this->buttonBackgroundStyleNormal();
        case ZFUIButtonState::e_Highlighted:
            return this->buttonBackgroundStyleHighlighted();
        case ZFUIButtonState::e_Checked:
            return this->buttonBackgroundStyleChecked();
        case ZFUIButtonState::e_CheckedHighlighted:
            return this->buttonBackgroundStyleCheckedHighlighted();
        case ZFUIButtonState::e_Disabled:
            return this->buttonBackgroundStyleDisabled();
        default:
            zfCoreCriticalShouldNotGoHere();
            return zfnull;
    }
}
ZFMETHOD_DEFINE_1(ZFUIButtonBasic, const zfchar *, buttonLabelText,
                  ZFMP_IN_OPT(ZFUIButtonStateEnum, forState, ZFUIButtonState::e_Normal))
{
    return this->buttonLabelStyle(forState)->text();
}
ZFMETHOD_DEFINE_2(ZFUIButtonBasic, void, buttonLabelText,
                  ZFMP_IN(const zfchar *, text),
                  ZFMP_IN_OPT(ZFUIButtonStateEnum, forState, ZFUIButtonState::e_Normal))
{
    this->buttonLabelStyle(forState)->text(text);
}

ZFMETHOD_DEFINE_1(ZFUIButtonBasic, ZFUIImage *, buttonIconImage,
                  ZFMP_IN_OPT(ZFUIButtonStateEnum, forState, ZFUIButtonState::e_Normal))
{
    return this->buttonIconStyle(forState)->image();
}
ZFMETHOD_DEFINE_2(ZFUIButtonBasic, void, buttonIconImage,
                  ZFMP_IN(ZFUIImage *, image),
                  ZFMP_IN_OPT(ZFUIButtonStateEnum, forState, ZFUIButtonState::e_Normal))
{
    this->buttonIconStyle(forState)->image(image);
}

ZFMETHOD_DEFINE_1(ZFUIButtonBasic, ZFUIImage *, buttonBackgroundImage,
                  ZFMP_IN_OPT(ZFUIButtonStateEnum, forState, ZFUIButtonState::e_Normal))
{
    return this->buttonBackgroundStyle(forState)->image();
}
ZFMETHOD_DEFINE_2(ZFUIButtonBasic, void, buttonBackgroundImage,
                  ZFMP_IN(ZFUIImage *, image),
                  ZFMP_IN_OPT(ZFUIButtonStateEnum, forState, ZFUIButtonState::e_Normal))
{
    this->buttonBackgroundStyle(forState)->image(image);
}

void ZFUIButtonBasic::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFUIButtonBasicPrivate);
    d->pimplOwner = this;
}

void ZFUIButtonBasic::objectOnDealloc(void)
{
    zfRelease(d->buttonLabel);
    zfRelease(d->buttonIcon);
    zfRelease(d->buttonBackground);
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}

void ZFUIButtonBasic::objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
{
    zfsuper::objectInfoOnAppend(ret);

    if(this->buttonLabel() != zfnull && !this->buttonLabel()->text().isEmpty())
    {
        zfstringAppend(ret, " \"%s\"", this->buttonLabel()->text().cString());
    }
}

void ZFUIButtonBasic::layoutOnMeasure(ZF_OUT ZFUISize &ret,
                                      ZF_IN const ZFUISize &sizeHint,
                                      ZF_IN const ZFUISizeParam &sizeParam)
{
    ZFUIView *labelView = ZFCastZFObjectUnchecked(ZFUIView *, d->buttonLabel);
    ZFUIView *iconView = ZFCastZFObjectUnchecked(ZFUIView *, d->buttonIcon);
    ZFUIView *backgroundView = ZFCastZFObjectUnchecked(ZFUIView *, d->buttonBackground);
    ZFUISize sizeHintTmp = sizeHint;
    if(sizeHintTmp.width >= 0)
    {
        sizeHintTmp.width -= this->buttonContentMargin().left + this->buttonContentMargin().right;
        if(sizeHintTmp.width < 0)
        {
            sizeHintTmp.width = 0;
        }
    }
    if(sizeHintTmp.height >= 0)
    {
        sizeHintTmp.height -= this->buttonContentMargin().top + this->buttonContentMargin().bottom;
        if(sizeHintTmp.height < 0)
        {
            sizeHintTmp.height = 0;
        }
    }

    ZFUISize labelSize = ZFUISizeZero();
    if(labelView != zfnull && labelView->viewVisible() && !d->buttonLabel->text().isEmpty())
    {
        labelSize = labelView->layoutMeasure(sizeHintTmp, ZFUISizeParamWrapWrap());
    }

    ZFUISize iconSize = ZFUISizeZero();
    if(iconView != zfnull && iconView->viewVisible() && d->buttonIcon->image() != zfnull)
    {
        iconSize = iconView->layoutMeasure(sizeHintTmp, ZFUISizeParamWrapWrap());
    }

    ZFUISize backgroundSize = ZFUISizeZero();
    if(backgroundView != zfnull && backgroundView->viewVisible() && d->buttonBackground->image() != zfnull)
    {
        ZFUISize sizeHintBg = sizeHint;
        if(sizeHintBg.width >= 0)
        {
            sizeHintBg.width -= this->buttonContentMargin().left + this->buttonContentMargin().right;
            if(sizeHintBg.width < 0)
            {
                sizeHintBg.width = 0;
            }
        }
        if(sizeHintBg.height >= 0)
        {
            sizeHintBg.height -= this->buttonContentMargin().top + this->buttonContentMargin().bottom;
            if(sizeHintBg.height < 0)
            {
                sizeHintBg.height = 0;
            }
        }
        backgroundSize = backgroundView->layoutMeasure(sizeHintBg, ZFUISizeParamWrapWrap());
    }

    ZFUISize contentSize = ZFUISizeZero();
    zffloat contentSpace = 0;
    switch(this->buttonContentIconPosition())
    {
        case ZFUIOrientation::e_Left:
        case ZFUIOrientation::e_Right:
            if(iconSize.width > 0 && labelSize.width > 0)
            {
                contentSpace = this->buttonContentSpace();
            }
            contentSize.width = iconSize.width + contentSpace + labelSize.width;
            contentSize.height = zfmMax(iconSize.height, labelSize.height);
            break;
        case ZFUIOrientation::e_Top:
        case ZFUIOrientation::e_Bottom:
            if(iconSize.height > 0 && labelSize.height > 0)
            {
                contentSpace = this->buttonContentSpace();
            }
            contentSize.width = zfmMax(iconSize.width, labelSize.width);
            contentSize.height = iconSize.height + contentSpace + labelSize.height;
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            return ;
    }

    if(backgroundSize.width > 0)
    {
        backgroundSize.width += ZFUIMarginGetWidth(this->buttonBackgroundMargin());
    }
    if(backgroundSize.height > 0)
    {
        backgroundSize.height += ZFUIMarginGetHeight(this->buttonBackgroundMargin());
    }
    if(contentSize.width > 0)
    {
        contentSize.width += ZFUIMarginGetWidth(this->buttonContentMargin());
    }
    if(contentSize.height > 0)
    {
        contentSize.height += ZFUIMarginGetHeight(this->buttonContentMargin());
    }
    ret.width = zfmMax(backgroundSize.width, contentSize.width);
    ret.height = zfmMax(backgroundSize.height, contentSize.height);
}
void ZFUIButtonBasic::internalViewOnLayout(ZF_IN const ZFUIRect &bounds)
{
    zfsuper::internalViewOnLayout(bounds);

    ZFUIView *backgroundView = ZFCastZFObjectUnchecked(ZFUIView *, d->buttonBackground);
    if(backgroundView != zfnull)
    {
        backgroundView->viewFrame(ZFUIRectApplyMargin(bounds, this->buttonBackgroundMargin()));
    }

    ZFUISize sizeHint = ZFUISizeApplyMargin(ZFUIRectGetSize(bounds), this->buttonContentMargin());

    ZFUIView *labelView = ZFCastZFObjectUnchecked(ZFUIView *, d->buttonLabel);
    ZFUIView *iconView = ZFCastZFObjectUnchecked(ZFUIView *, d->buttonIcon);

    ZFUISize labelSize = ZFUISizeZero();
    if(labelView != zfnull && labelView->viewVisible() && !d->buttonLabel->text().isEmpty())
    {
        labelSize = labelView->layoutMeasure(sizeHint, ZFUISizeParamWrapWrap());
    }

    ZFUISize iconSize = ZFUISizeZero();
    if(iconView != zfnull && iconView->viewVisible() && d->buttonIcon->image() != zfnull)
    {
        iconSize = iconView->layoutMeasure(sizeHint, ZFUISizeParamWrapWrap());
    }

    ZFUIRect contentFrame = ZFUIRectZero();
    zffloat contentSpace = 0;
    switch(this->buttonContentIconPosition())
    {
        case ZFUIOrientation::e_Left:
        case ZFUIOrientation::e_Right:
            if(iconSize.width > 0 && labelSize.width > 0)
            {
                contentSpace = this->buttonContentSpace();
            }
            contentFrame = ZFUIAlignApply(
                this->buttonContentAlign(),
                bounds,
                ZFUISizeMake(iconSize.width + contentSpace + labelSize.width, zfmMax(iconSize.height, labelSize.height)),
                this->buttonContentMargin());
            if(this->buttonContentIconPosition() == ZFUIOrientation::e_Left)
            {
                if(iconView != zfnull)
                {
                    iconView->viewFrame(ZFUIAlignApply(ZFUIAlign::e_LeftInner, contentFrame, iconSize));
                }
                if(labelView != zfnull)
                {
                    labelView->viewFrame(ZFUIAlignApply(ZFUIAlign::e_RightInner, contentFrame, labelSize));
                }
            }
            else
            {
                if(iconView != zfnull)
                {
                    iconView->viewFrame(ZFUIAlignApply(ZFUIAlign::e_RightInner, contentFrame, iconSize));
                }
                if(labelView != zfnull)
                {
                    labelView->viewFrame(ZFUIAlignApply(ZFUIAlign::e_LeftInner, contentFrame, labelSize));
                }
            }
            break;
        case ZFUIOrientation::e_Top:
        case ZFUIOrientation::e_Bottom:
            if(iconSize.height > 0 && labelSize.height > 0)
            {
                contentSpace = this->buttonContentSpace();
            }
            contentFrame = ZFUIAlignApply(
                this->buttonContentAlign(),
                bounds,
                ZFUISizeMake(zfmMax(iconSize.width, labelSize.width), iconSize.height + contentSpace + labelSize.height),
                this->buttonContentMargin());
            if(this->buttonContentIconPosition() == ZFUIOrientation::e_Top)
            {
                if(iconView != zfnull)
                {
                    iconView->viewFrame(ZFUIAlignApply(ZFUIAlign::e_TopInner, contentFrame, iconSize));
                }
                if(labelView != zfnull)
                {
                    labelView->viewFrame(ZFUIAlignApply(ZFUIAlign::e_BottomInner, contentFrame, labelSize));
                }
            }
            else
            {
                if(iconView != zfnull)
                {
                    iconView->viewFrame(ZFUIAlignApply(ZFUIAlign::e_BottomInner, contentFrame, iconSize));
                }
                if(labelView != zfnull)
                {
                    labelView->viewFrame(ZFUIAlignApply(ZFUIAlign::e_TopInner, contentFrame, labelSize));
                }
            }
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            return ;
    }
}

zfbool ZFUIButtonBasic::internalViewShouldLayout(ZF_IN ZFUIView *internalView)
{
    if((d->buttonLabel != zfnull && d->buttonLabel->toObject() == internalView)
        || (d->buttonIcon != zfnull && d->buttonIcon->toObject() == internalView)
        || (d->buttonBackground != zfnull && d->buttonBackground->toObject() == internalView))
    {
        return zffalse;
    }
    return zfsuper::internalViewShouldLayout(internalView);
}

void ZFUIButtonBasic::buttonStateOnUpdate(void)
{
    zfsuper::buttonStateOnUpdate();

    #define _ZFP_ZFUIButtonBasic_buttonStateOn(StateName) \
        if(d->buttonLabel != zfnull) \
        { \
            if(ZFPropertyIsValueAccessed(ZFPropertyAccess(ZFUIButtonBasic, buttonLabelStyle##StateName), this)) \
            { \
                ZFCastZFObjectUnchecked(ZFStyleable *, d->buttonLabel)->styleableCopyFrom(ZFCastZFObjectUnchecked(ZFStyleable *, this->buttonLabelStyle##StateName())); \
            } \
            d->updateButtonLabel(); \
        } \
        if(d->buttonIcon != zfnull) \
        { \
            if(ZFPropertyIsValueAccessed(ZFPropertyAccess(ZFUIButtonBasic, buttonIconStyle##StateName), this)) \
            { \
                ZFCastZFObjectUnchecked(ZFStyleable *, d->buttonIcon)->styleableCopyFrom(ZFCastZFObjectUnchecked(ZFStyleable *, this->buttonIconStyle##StateName())); \
            } \
            d->updateButtonIcon(); \
        } \
        if(d->buttonBackground != zfnull) \
        { \
            if(ZFPropertyIsValueAccessed(ZFPropertyAccess(ZFUIButtonBasic, buttonBackgroundStyle##StateName), this)) \
            { \
                ZFCastZFObjectUnchecked(ZFStyleable *, d->buttonBackground)->styleableCopyFrom(ZFCastZFObjectUnchecked(ZFStyleable *, this->buttonBackgroundStyle##StateName())); \
            } \
            d->updateButtonBackground(); \
        }
    switch(this->buttonState())
    {
        case ZFUIButtonState::e_Normal:
            _ZFP_ZFUIButtonBasic_buttonStateOn(Normal)
            break;
        case ZFUIButtonState::e_Highlighted:
            _ZFP_ZFUIButtonBasic_buttonStateOn(Highlighted)
            break;
        case ZFUIButtonState::e_Checked:
            _ZFP_ZFUIButtonBasic_buttonStateOn(Checked)
            break;
        case ZFUIButtonState::e_CheckedHighlighted:
            _ZFP_ZFUIButtonBasic_buttonStateOn(CheckedHighlighted)
            break;
        case ZFUIButtonState::e_Disabled:
            _ZFP_ZFUIButtonBasic_buttonStateOn(Disabled)
            break;
        default:
            break;
    }
}

const ZFClass *ZFUIButtonBasic::buttonLabelClass(void)
{
    return ZFUITextView::ClassData();
}
const ZFClass *ZFUIButtonBasic::buttonIconClass(void)
{
    return ZFUIImageView::ClassData();
}
const ZFClass *ZFUIButtonBasic::buttonBackgroundClass(void)
{
    return ZFUIImageView::ClassData();
}

ZFMETHOD_DEFINE_0(ZFUIButtonBasic, ZFUITextView *, buttonLabel)
{
    this->prepareButtonLabel();
    return d->buttonLabel;
}
ZFMETHOD_DEFINE_0(ZFUIButtonBasic, ZFUIImageView *, buttonIcon)
{
    this->prepareButtonIcon();
    return d->buttonIcon;
}
ZFMETHOD_DEFINE_0(ZFUIButtonBasic, ZFUIImageView *, buttonBackground)
{
    this->prepareButtonBackground();
    return d->buttonBackground;
}

ZFMETHOD_DEFINE_0(ZFUIButtonBasic, void, prepareButtonLabel)
{
    if(d->buttonLabel == zfnull)
    {
        ZFObject *obj = zfRetain(this->buttonLabelClass()->newInstance().toObject());
        d->buttonLabel = ZFCastZFObject(ZFUITextView *, obj);
        zfCoreAssert(d->buttonLabel != zfnull);
        this->internalBgViewAdd(ZFCastZFObjectUnchecked(ZFUIView *, d->buttonLabel));
    }
}
ZFMETHOD_DEFINE_0(ZFUIButtonBasic, void, prepareButtonIcon)
{
    if(d->buttonIcon == zfnull)
    {
        ZFObject *obj = zfRetain(this->buttonIconClass()->newInstance().toObject());
        d->buttonIcon = ZFCastZFObject(ZFUIImageView *, obj);
        zfCoreAssert(d->buttonIcon != zfnull);
        this->internalBgViewAdd(ZFCastZFObjectUnchecked(ZFUIView *, d->buttonIcon));

        if(d->buttonLabel != zfnull)
        {
            ZFUIView *t = d->buttonLabel->to<ZFUIView *>();
            this->internalBgViewRemove(t);
            this->internalBgViewAdd(t);
        }
    }
}
ZFMETHOD_DEFINE_0(ZFUIButtonBasic, void, prepareButtonBackground)
{
    if(d->buttonBackground == zfnull)
    {
        ZFObject *obj = zfRetain(this->buttonBackgroundClass()->newInstance().toObject());
        d->buttonBackground = ZFCastZFObject(ZFUIImageView *, obj);
        zfCoreAssert(d->buttonBackground != zfnull);
        this->internalBgViewAdd(ZFCastZFObjectUnchecked(ZFUIView *, d->buttonBackground));

        if(d->buttonIcon != zfnull)
        {
            ZFUIView *t = d->buttonIcon->to<ZFUIView *>();
            this->internalBgViewRemove(t);
            this->internalBgViewAdd(t);
        }
        if(d->buttonLabel != zfnull)
        {
            ZFUIView *t = d->buttonLabel->to<ZFUIView *>();
            this->internalBgViewRemove(t);
            this->internalBgViewAdd(t);
        }
    }
}

ZF_NAMESPACE_GLOBAL_END

