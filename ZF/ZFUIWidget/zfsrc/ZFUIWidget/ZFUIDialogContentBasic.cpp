#include "ZFUIDialogContentBasic.h"
#include "ZFUIDialogContentBasic_Buttons.h"
#include "ZFUILinearLayout.h"
#include "ZFUIViewLayout.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFSTYLE_DEFAULT_DEFINE(ZFUIDialogContentBasic)

// ============================================================
ZFOBJECT_REGISTER(ZFUIDialogContentBasic)

// ============================================================
zfclassNotPOD _ZFP_ZFUIDialogContentBasicPrivate
{
public:
    ZFUILinearLayout *mainContainer;

    ZFUIViewLayout *dialogTitleContainer;
    ZFUIViewLayout *dialogContentContainer;
    ZFUIViewLayout *dialogButtonContainer;

    // button order (left to right):
    // Normal(s), Destructive, Cancel, No, Yes
    zfindex dialogButtonInternalCount;
    ZFUILinearLayout *dialogButtonLayout;
    ZFUIDialogContentBasicButtonYes *dialogButtonYes;
    ZFUIDialogContentBasicButtonNo *dialogButtonNo;
    ZFUIDialogContentBasicButtonCancel *dialogButtonCancel;
    ZFUIDialogContentBasicButtonDestructive *dialogButtonDestructive;
public:
    _ZFP_ZFUIDialogContentBasicPrivate(void)
    : mainContainer(zfnull)
    , dialogTitleContainer(zfnull)
    , dialogContentContainer(zfnull)
    , dialogButtonContainer(zfnull)
    , dialogButtonInternalCount(0)
    , dialogButtonLayout(zfnull)
    , dialogButtonYes(zfnull)
    , dialogButtonNo(zfnull)
    , dialogButtonCancel(zfnull)
    , dialogButtonDestructive(zfnull)
    {
    }
};

// ============================================================
// title
ZFUIView *ZFUIDialogContentBasic::dialogTitleContainer(void)
{
    return d->dialogTitleContainer;
}
ZFUITextView *ZFUIDialogContentBasic::dialogTitleView(void)
{
    return this->titleTextView();
}

// ============================================================
// content
ZFUIView *ZFUIDialogContentBasic::dialogContentContainer(void)
{
    return d->dialogContentContainer;
}
ZFUITextView *ZFUIDialogContentBasic::dialogContentView(void)
{
    return this->contentTextView();
}

// ============================================================
// button
ZFUIView *ZFUIDialogContentBasic::dialogButtonContainer(void)
{
    return d->dialogButtonContainer;
}
ZFUIButton *ZFUIDialogContentBasic::dialogButton(ZF_IN ZFUIDialogButtonTypeEnum dialogButtonType,
                                                 ZF_IN_OPT zfbool autoCreateIfNotExist /* = zftrue */)
{
    switch(dialogButtonType)
    {
        case ZFUIDialogButtonType::e_Normal:
            return zfnull;
        case ZFUIDialogButtonType::e_Yes:
            if(autoCreateIfNotExist && d->dialogButtonYes == zfnull)
            {
                ++(d->dialogButtonInternalCount);
                d->dialogButtonYes = zfAlloc(ZFUIDialogContentBasicButtonYes);
                zfindex index = 0;
                d->dialogButtonLayout->childAdd(d->dialogButtonYes, zfnull, index);
                this->dialogButtonOnAdd(d->dialogButtonYes);
            }
            return d->dialogButtonYes;
        case ZFUIDialogButtonType::e_No:
            if(autoCreateIfNotExist && d->dialogButtonNo == zfnull)
            {
                ++(d->dialogButtonInternalCount);
                d->dialogButtonNo = zfAlloc(ZFUIDialogContentBasicButtonNo);
                zfindex index = 0;
                if(d->dialogButtonYes != zfnull) {++index;}
                d->dialogButtonLayout->childAdd(d->dialogButtonNo, zfnull, index);
                this->dialogButtonOnAdd(d->dialogButtonNo);
            }
            return d->dialogButtonNo;
        case ZFUIDialogButtonType::e_Cancel:
            if(autoCreateIfNotExist && d->dialogButtonCancel == zfnull)
            {
                ++(d->dialogButtonInternalCount);
                d->dialogButtonCancel = zfAlloc(ZFUIDialogContentBasicButtonCancel);
                zfindex index = 0;
                if(d->dialogButtonYes != zfnull) {++index;}
                if(d->dialogButtonNo != zfnull) {++index;}
                d->dialogButtonLayout->childAdd(d->dialogButtonCancel, zfnull, index);
                this->dialogButtonOnAdd(d->dialogButtonCancel);
            }
            return d->dialogButtonCancel;
        case ZFUIDialogButtonType::e_Destructive:
            if(autoCreateIfNotExist && d->dialogButtonDestructive == zfnull)
            {
                ++(d->dialogButtonInternalCount);
                d->dialogButtonDestructive = zfAlloc(ZFUIDialogContentBasicButtonDestructive);
                zfindex index = 0;
                if(d->dialogButtonYes != zfnull) {++index;}
                if(d->dialogButtonNo != zfnull) {++index;}
                if(d->dialogButtonCancel != zfnull) {++index;}
                d->dialogButtonLayout->childAdd(d->dialogButtonDestructive, zfnull, index);
                this->dialogButtonOnAdd(d->dialogButtonDestructive);
            }
            return d->dialogButtonDestructive;
        default:
            zfCoreCriticalShouldNotGoHere();
            return zfnull;
    }
}
const zfchar *ZFUIDialogContentBasic::dialogButtonText(ZF_IN ZFUIDialogButtonTypeEnum dialogButtonType)
{
    ZFUIButtonBasic *button = ZFCastZFObjectUnchecked(ZFUIButtonBasic *, this->dialogButton(dialogButtonType, zffalse));
    return ((button != zfnull) ? button->buttonLabelText() : zfnull);
}
void ZFUIDialogContentBasic::dialogButtonText(ZF_IN ZFUIDialogButtonTypeEnum dialogButtonType,
                                              ZF_IN const zfchar *text)
{
    this->dialogButton(dialogButtonType)->to<ZFUIButtonBasic *>()->buttonLabelText(text);
}
void ZFUIDialogContentBasic::dialogButtonRemove(ZF_IN ZFUIDialogButtonTypeEnum dialogButtonType)
{
    switch(dialogButtonType)
    {
        case ZFUIDialogButtonType::e_Normal:
            break;
        case ZFUIDialogButtonType::e_Yes:
            if(d->dialogButtonYes != zfnull)
            {
                this->dialogButtonOnRemove(d->dialogButtonYes);
                --(d->dialogButtonInternalCount);
                zfindex index = 0;
                d->dialogButtonLayout->childRemoveAtIndex(index);
                zfRelease(d->dialogButtonYes);
                d->dialogButtonYes = zfnull;
            }
            break;
        case ZFUIDialogButtonType::e_No:
            if(d->dialogButtonNo != zfnull)
            {
                this->dialogButtonOnRemove(d->dialogButtonNo);
                --(d->dialogButtonInternalCount);
                zfindex index = 0;
                if(d->dialogButtonYes != zfnull) {++index;}
                d->dialogButtonLayout->childRemoveAtIndex(index);
                zfRelease(d->dialogButtonNo);
                d->dialogButtonNo = zfnull;
            }
            break;
        case ZFUIDialogButtonType::e_Cancel:
            if(d->dialogButtonCancel != zfnull)
            {
                this->dialogButtonOnRemove(d->dialogButtonCancel);
                --(d->dialogButtonInternalCount);
                zfindex index = 0;
                if(d->dialogButtonYes != zfnull) {++index;}
                if(d->dialogButtonNo != zfnull) {++index;}
                d->dialogButtonLayout->childRemoveAtIndex(index);
                zfRelease(d->dialogButtonCancel);
                d->dialogButtonCancel = zfnull;
            }
            break;
        case ZFUIDialogButtonType::e_Destructive:
            if(d->dialogButtonDestructive != zfnull)
            {
                this->dialogButtonOnRemove(d->dialogButtonDestructive);
                --(d->dialogButtonInternalCount);
                zfindex index = 0;
                if(d->dialogButtonYes != zfnull) {++index;}
                if(d->dialogButtonNo != zfnull) {++index;}
                if(d->dialogButtonCancel != zfnull) {++index;}
                d->dialogButtonLayout->childRemoveAtIndex(index);
                zfRelease(d->dialogButtonDestructive);
                d->dialogButtonDestructive = zfnull;
            }
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            break;
    }
}

// ============================================================
// button
zfindex ZFUIDialogContentBasic::dialogButtonCount(void)
{
    return (d->dialogButtonLayout->childCount() - d->dialogButtonInternalCount);
}
ZFUIButton *ZFUIDialogContentBasic::dialogButtonAtIndex(ZF_IN zfindex index)
{
    if(index >= this->dialogButtonCount())
    {
        zfCoreCriticalIndexOutOfRange(index, this->dialogButtonCount());
        return zfnull;
    }
    return d->dialogButtonLayout->childAtIndex(d->dialogButtonInternalCount + index)->to<ZFUIButton *>();
}
zfindex ZFUIDialogContentBasic::dialogButtonFind(ZF_IN ZFUIButton *dialogButton)
{
    zfindex ret = d->dialogButtonLayout->childFind(dialogButton);
    if(ret == zfindexMax() || ret < d->dialogButtonInternalCount)
    {
        return zfindexMax();
    }
    return (ret - d->dialogButtonInternalCount);
}
void ZFUIDialogContentBasic::dialogButtonAdd(ZF_IN ZFUIButton *button,
                                             ZF_IN_OPT zfindex atIndex /* = zfindexMax() */)
{
    if(atIndex == zfindexMax())
    {
        atIndex = this->dialogButtonCount();
    }
    else if(atIndex > this->dialogButtonCount())
    {
        zfCoreCriticalIndexOutOfRange(atIndex, this->dialogButtonCount() + 1);
        return ;
    }
    d->dialogButtonLayout->childAdd(button, zfnull, atIndex + d->dialogButtonInternalCount);
    this->dialogButtonOnAdd(button);
}
void ZFUIDialogContentBasic::dialogButtonRemove(ZF_IN ZFUIButton *button)
{
    this->dialogButtonRemoveAtIndex(this->dialogButtonFind(button));
}
void ZFUIDialogContentBasic::dialogButtonRemoveAtIndex(ZF_IN zfindex index)
{
    if(index == zfindexMax())
    {
        return ;
    }
    else if(index >= this->dialogButtonCount())
    {
        zfCoreCriticalIndexOutOfRange(index, this->dialogButtonCount());
        return ;
    }
    this->dialogButtonOnRemove(d->dialogButtonLayout->childAtIndex(d->dialogButtonInternalCount + index)->to<ZFUIButton *>());
    d->dialogButtonLayout->childRemoveAtIndex(d->dialogButtonInternalCount + index);
}
void ZFUIDialogContentBasic::dialogButtonRemoveAll(void)
{
    while(d->dialogButtonLayout->childCount() > d->dialogButtonInternalCount)
    {
        this->dialogButtonOnRemove(d->dialogButtonLayout->childAtIndex(d->dialogButtonLayout->childCount() - 1)->to<ZFUIButton *>());
        d->dialogButtonLayout->childRemoveAtIndex(d->dialogButtonLayout->childCount() - 1);
    }
}

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIDialogContentBasicDataHolder, ZFLevelZFFrameworkEssential)
{
    this->textChangeListener = ZFCallbackForFunc(zfself::textChange);
    this->containerChildChangeListener = ZFCallbackForFunc(zfself::containerChildChange);
}
public:
    ZFListener textChangeListener;
    ZFListener containerChildChangeListener;
    static ZFLISTENER_PROTOTYPE_EXPAND(textChange)
    {
        const ZFProperty *property = listenerData.param0<v_ZFProperty *>()->zfv;
        if(property != ZFPropertyAccess(ZFUITextView, text))
        {
            return ;
        }
        ZFUITextView *textView = listenerData.sender<ZFUITextView *>();
        textView->viewVisible(!textView->text().isEmpty());
    }
    static ZFLISTENER_PROTOTYPE_EXPAND(containerChildChange)
    {
        ZFUIView *containerView = listenerData.sender<ZFUIView *>();
        containerView->viewVisible(containerView->childCount() > 0);
    }
ZF_GLOBAL_INITIALIZER_END(ZFUIDialogContentBasicDataHolder)

void ZFUIDialogContentBasic::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFUIDialogContentBasicPrivate);

    d->mainContainer = zfAlloc(ZFUILinearLayout);
    this->internalFgViewAdd(d->mainContainer);
    d->mainContainer->layoutOrientation(ZFUIOrientation::e_Top);
    d->mainContainer->layoutChildSpace(ZFUIGlobalStyle::DefaultStyle()->itemSpace());

    { // title
        ZFUITextView *titleTextView = this->titleTextView()->to<ZFUITextView *>();
        d->mainContainer->childAdd(titleTextView);
        titleTextView->layoutParam()->layoutAlign(ZFUIAlign::e_Center);
        titleTextView->observerAdd(ZFObject::EventObjectPropertyValueOnUpdate(),
            ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIDialogContentBasicDataHolder)->textChangeListener);
        titleTextView->viewVisible(!titleTextView->text().isEmpty());
        titleTextView->textColor(ZFUIGlobalStyle::DefaultStyle()->textColorSecondary());
        titleTextView->textSingleLine(zffalse);

        d->dialogTitleContainer = zfAlloc(ZFUIViewLayout);
        d->mainContainer->childAdd(d->dialogTitleContainer);
        d->dialogTitleContainer->layoutParam()->layoutAlign(ZFUIAlign::e_Center);
        d->dialogTitleContainer->observerAdd(ZFUIView::EventViewChildOnChange(),
            ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIDialogContentBasicDataHolder)->containerChildChangeListener);
    }

    { // content
        ZFUITextView *contentTextView = this->contentTextView()->to<ZFUITextView *>();
        d->mainContainer->childAdd(contentTextView);
        contentTextView->layoutParam()->layoutAlign(ZFUIAlign::e_LeftInner);
        contentTextView->observerAdd(ZFObject::EventObjectPropertyValueOnUpdate(),
            ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIDialogContentBasicDataHolder)->textChangeListener);
        contentTextView->viewVisible(!contentTextView->text().isEmpty());

        d->dialogContentContainer = zfAlloc(ZFUIViewLayout);
        d->mainContainer->childAdd(d->dialogContentContainer);
        d->dialogContentContainer->layoutParam()->layoutAlign(ZFUIAlign::e_LeftInner);
        d->dialogContentContainer->layoutParam<ZFUILinearLayoutParam *>()->layoutWeight(1);
        d->dialogContentContainer->observerAdd(ZFUIView::EventViewChildOnChange(),
            ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIDialogContentBasicDataHolder)->containerChildChangeListener);
    }

    { // button
        d->dialogButtonContainer = zfAlloc(ZFUIViewLayout);
        d->mainContainer->childAdd(d->dialogButtonContainer);
        d->dialogButtonContainer->layoutParam()->layoutAlign(ZFUIAlign::e_RightInner);

        d->dialogButtonLayout = zfAlloc(ZFUILinearLayout);
        d->dialogButtonContainer->childAdd(d->dialogButtonLayout);
        d->dialogButtonLayout->layoutParam()->layoutAlign(ZFUIAlign::e_RightInner);
        d->dialogButtonLayout->layoutOrientation(ZFUIOrientation::e_Right);
        d->dialogButtonLayout->layoutChildSpace(ZFUIGlobalStyle::DefaultStyle()->itemSpace());
    }
}
void ZFUIDialogContentBasic::objectOnDealloc(void)
{
    zfCoreAssert(d->dialogButtonYes == zfnull);
    zfCoreAssert(d->dialogButtonNo == zfnull);
    zfCoreAssert(d->dialogButtonCancel == zfnull);
    zfCoreAssert(d->dialogButtonDestructive == zfnull);

    zfRelease(d->dialogButtonLayout);
    zfRelease(d->dialogButtonContainer);
    zfRelease(d->dialogContentContainer);
    zfRelease(d->dialogTitleContainer);
    zfRelease(d->mainContainer);

    zfpoolDelete(d);
    zfsuper::objectOnDealloc();
}
void ZFUIDialogContentBasic::objectOnDeallocPrepare(void)
{
    this->dialogButtonRemove(ZFUIDialogButtonType::e_Yes);
    this->dialogButtonRemove(ZFUIDialogButtonType::e_No);
    this->dialogButtonRemove(ZFUIDialogButtonType::e_Cancel);
    this->dialogButtonRemove(ZFUIDialogButtonType::e_Destructive);
    zfsuper::objectOnDeallocPrepare();
}

void ZFUIDialogContentBasic::layoutOnMeasure(ZF_OUT ZFUISize &ret,
                                             ZF_IN const ZFUISize &sizeHint,
                                             ZF_IN const ZFUISizeParam &sizeParam)
{
    ret = d->mainContainer->layoutMeasure(sizeHint, sizeParam);
}
void ZFUIDialogContentBasic::internalFgViewOnLayout(ZF_IN const ZFUIRect &bounds)
{
    zfsuper::internalFgViewOnLayout(bounds);
    d->mainContainer->viewFrame(bounds);
}
zfbool ZFUIDialogContentBasic::internalViewShouldLayout(ZF_IN ZFUIView *internalView)
{
    if(!zfsuper::internalViewShouldLayout(internalView))
    {
        return zffalse;
    }
    if(internalView == d->mainContainer)
    {
        return zffalse;
    }
    return zftrue;
}

ZF_NAMESPACE_GLOBAL_END

