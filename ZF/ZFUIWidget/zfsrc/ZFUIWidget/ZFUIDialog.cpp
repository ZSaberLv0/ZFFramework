#include "ZFUIDialog.h"
#include "ZFUIButton.h"
#include "ZFUIViewLayout.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFObjectCreator ZFUIDialogDefaultLayoutParamCreator = ZFUIDialogDefaultLayoutParamCreatorDefault;
zfautoObject ZFUIDialogDefaultLayoutParamCreatorDefault(void)
{
    zfblockedAlloc(ZFUILayoutParam, lp);
    lp->layoutAlign(ZFUIAlign::e_Center);
    lp->layoutMargin(ZFUIMarginMake(ZFUIGlobalStyle::DefaultStyle()->itemMargin()));
    return lp;
}
ZFObjectCreator ZFUIDialogDefaultAniShowCreator = ZFUIDialogDefaultAniShowCreatorDefault;
zfautoObject ZFUIDialogDefaultAniShowCreatorDefault(void)
{
    zfblockedAlloc(ZFAnimationNativeView, ani);
    ani->aniAlphaFrom(0);
    return ani;
}
ZFObjectCreator ZFUIDialogDefaultAniHideCreator = ZFUIDialogDefaultAniHideCreatorDefault;
zfautoObject ZFUIDialogDefaultAniHideCreatorDefault(void)
{
    zfblockedAlloc(ZFAnimationNativeView, ani);
    ani->aniAlphaTo(0);
    return ani;
}

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIDialogDefaultCreatorReset, ZFLevelZFFrameworkNormal)
{
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIDialogDefaultCreatorReset)
{
    ZFUIDialogDefaultLayoutParamCreator = ZFUIDialogDefaultLayoutParamCreatorDefault;
    ZFUIDialogDefaultAniShowCreator = ZFUIDialogDefaultAniShowCreatorDefault;
    ZFUIDialogDefaultAniHideCreator = ZFUIDialogDefaultAniHideCreatorDefault;
}
ZF_GLOBAL_INITIALIZER_END(ZFUIDialogDefaultCreatorReset)

// ============================================================
// ZFUIDialog
ZFSTYLE_DEFAULT_DEFINE(ZFUIDialog)

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIDialogAllDialog, ZFLevelZFFrameworkHigh)
{
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIDialogAllDialog)
{
    ZFCoreArrayPOD<ZFUIDialog *> tmp;
    tmp.copyFrom(this->allDialog);
    for(zfindex i = 0; i < tmp.count(); ++i)
    {
        if(tmp[i]->dialogShowing())
        {
            tmp[i]->dialogHideForce();
        }
    }
}
public:
    ZFCoreArrayPOD<ZFUIDialog *> allDialog;
ZF_GLOBAL_INITIALIZER_END(ZFUIDialogAllDialog)
#define _ZFP_ZFUIDialogAllDialog ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIDialogAllDialog)->allDialog

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIDialogAutoHide, ZFLevelZFFrameworkLow)
{
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIDialogAutoHide)
{
    ZFCoreArrayPOD<ZFUIDialog *> tmp;
    tmp.copyFrom(_ZFP_ZFUIDialogAllDialog);
    for(zfindex i = 0; i < tmp.count(); ++i)
    {
        if(tmp[i]->dialogShowing())
        {
            tmp[i]->dialogHideForce();
        }
    }
}
ZF_GLOBAL_INITIALIZER_END(ZFUIDialogAutoHide)

// ============================================================
// _ZFP_I_ZFUIDialogPrivate
/*
 * memo:
 *
 * _ZFP_I_ZFUIDialogPrivate (ZFUIWindow, handle all key event)
 * ^ dialogWindowBg (internal bg, window ani target)
 *   dialogClickMask (internal bg, focused if dialogContainer has no focusable child)
 *   _ZFP_ZFUIDialogContentHolder (Ui disable, supply layout logic)
 *   ^ dialogBg
 *     ^ dialogContainer (dialogInternalContainer)
 *       ^ dialogView
 *         subclass impl views
 */
zfclass _ZFP_I_ZFUIDialogPrivate : zfextends ZFUIWindow
{
    ZFOBJECT_DECLARE(_ZFP_I_ZFUIDialogPrivate, ZFUIWindow)

public:
    ZFUIDialog *pimplOwner;
    ZFUIView *dialogWindowBg;
    ZFAnimationNativeView *dialogWindowAniShow;
    ZFAnimationNativeView *dialogWindowAniHide;
    ZFUIButton *dialogClickMask;
    ZFUIImageView *dialogBg;
    ZFUIOnScreenKeyboardAutoFitLayout *dialogContainer;
    ZFListener aniShowOnStopListener;
    ZFListener aniHideOnStopListener;

protected:
    zfoverride
    virtual void objectOnInit(void)
    {
        zfsuper::objectOnInit();
        this->windowLevel(ZFUIWindowLevel::e_AppHigh);
        this->sysWindowMarginShouldApply(zffalse);
    }
    zfoverride
    virtual void layoutOnLayout(ZF_IN const ZFUIRect &bounds)
    {
        if(this->pimplOwner->sysWindowMarginShouldApply())
        {
            zfsuper::layoutOnLayout(ZFUIRectApplyMargin(bounds, this->windowOwnerSysWindow()->sysWindowMargin()));
        }
        else
        {
            zfsuper::layoutOnLayout(bounds);
        }
    }

public:
    void aniStop(void)
    {
        if(this->dialogWindowAniShow != zfnull)
        {
            this->dialogWindowAniShow->aniStop();
        }
        if(this->dialogWindowAniHide != zfnull)
        {
            this->dialogWindowAniHide->aniStop();
        }
        if(this->pimplOwner->dialogAniShow() != zfnull)
        {
            this->pimplOwner->dialogAniShow()->aniStop();
        }
        if(this->pimplOwner->dialogAniHide() != zfnull)
        {
            this->pimplOwner->dialogAniHide()->aniStop();
        }
    }

public:
    ZFMETHOD_INLINE_2(void, dialogClickMaskOnClick,
                      ZFMP_IN(const ZFListenerData &, listenerData),
                      ZFMP_IN(ZFObject *, userData))
    {
        if(this->pimplOwner->dialogHideWhenTouchOutside())
        {
            this->pimplOwner->dialogHide();
        }
    }
    ZFMETHOD_INLINE_2(void, aniShowOnStop,
                      ZFMP_IN(const ZFListenerData &, listenerData),
                      ZFMP_IN(ZFObject *, userData))
    {
        if(!this->dialogWindowAniShow->aniRunning()
            && !this->dialogWindowAniHide->aniRunning()
            && (this->pimplOwner->dialogAniShow() == zfnull || !this->pimplOwner->dialogAniShow()->aniRunning())
            && (this->pimplOwner->dialogAniHide() == zfnull || !this->pimplOwner->dialogAniHide()->aniRunning())
            )
        {
            this->viewUIEnableTree(zftrue);
            ZFUIViewBlinkWhenFocusAutoApplyPause();
            if(this->pimplOwner->dialogFocusAutomatically())
            {
                ZFUIView *v = this->pimplOwner->dialogFocusOnUpdate();
                if(this->pimplOwner->observerHasAdd(ZFUIDialog::EventDialogFocusOnUpdate()))
                {
                    zfblockedAlloc(ZFObjectHolder, param0, v);
                    this->pimplOwner->observerNotify(ZFUIDialog::EventDialogFocusOnUpdate(), param0);
                }
                if(v != zfnull)
                {
                    v->viewFocusRequest(zftrue);
                }
                else
                {
                    this->dialogClickMask->viewFocusRequest(zftrue);
                }
            }
            else
            {
                this->dialogClickMask->viewFocusRequest(zftrue);
            }
            ZFUIViewBlinkWhenFocusAutoApplyResume();
            this->pimplOwner->dialogAfterShow();
        }
    }
    ZFMETHOD_INLINE_2(void, aniHideOnStop,
                      ZFMP_IN(const ZFListenerData &, listenerData),
                      ZFMP_IN(ZFObject *, userData))
    {
        if(!this->dialogWindowAniShow->aniRunning()
            && !this->dialogWindowAniHide->aniRunning()
            && (this->pimplOwner->dialogAniShow() == zfnull || !this->pimplOwner->dialogAniShow()->aniRunning())
            && (this->pimplOwner->dialogAniHide() == zfnull || !this->pimplOwner->dialogAniHide()->aniRunning())
            )
        {
            this->viewUIEnableTree(zftrue);
            this->windowHide();
            this->pimplOwner->dialogAfterHide();
            zfRelease(this->pimplOwner);
        }
    }

public:
    void layoutDialogPrepare(ZF_IN const ZFUIRect &bounds)
    {
        if(this->pimplOwner->dialogBackgroundImage() != zfnull)
        {
            this->dialogContainer->layoutParam()->layoutMargin(this->pimplOwner->dialogBackgroundImage()->imageNinePatch());
        }
    }
    void layoutDialog(ZF_IN const ZFUIRect &bounds)
    {
        ZFUIMargin dialogMargin = this->pimplOwner->dialogLayoutParam()->layoutMargin();
        const ZFUIMargin &contentMargin = this->dialogContainer->layoutParam()->layoutMargin();

        ZFUIMargin totalMargin = ZFUIMarginInc(dialogMargin, contentMargin);

        this->dialogContainer->layoutMeasure(
            ZFUISizeApplyMargin(ZFUIRectGetSize(bounds), totalMargin),
            this->pimplOwner->dialogLayoutParam()->sizeParam());
        ZFUISize dialogContainerSize = this->dialogContainer->layoutMeasuredSize();

        ZFUISize dialogBgSize = dialogContainerSize;
        dialogBgSize.width += ZFUIMarginGetWidth(contentMargin);
        dialogBgSize.height += ZFUIMarginGetHeight(contentMargin);
        this->dialogBg->viewFrame(ZFUIAlignApply(
            this->pimplOwner->dialogLayoutParam()->layoutAlign(),
            bounds,
            dialogBgSize,
            dialogMargin));
    }

    // event handle
protected:
    zfoverride
    virtual void viewEventOnKeyEvent(ZF_IN ZFUIKeyEvent *keyEvent)
    {
        // dialog would always resolve key event
        keyEvent->eventResolved(zftrue);

        if(!this->pimplOwner->dialogHideWhenClickBack()
            || keyEvent->keyAction != ZFUIKeyAction::e_KeyUp)
        {
            return ;
        }

        switch(keyEvent->keyCode)
        {
            case ZFUIKeyCode::e_kPhoneBack:
            case ZFUIKeyCode::e_kEsc:
                this->pimplOwner->dialogHide();
                break;
            default:
                break;
        }
    }
};
ZFOBJECT_REGISTER(_ZFP_I_ZFUIDialogPrivate)

zfclass _ZFP_ZFUIDialogContentHolder : zfextends ZFUIView
{
    ZFOBJECT_DECLARE(_ZFP_ZFUIDialogContentHolder, ZFUIView)

public:
    _ZFP_I_ZFUIDialogPrivate *pimplOwner;

protected:
    zfoverride
    virtual void layoutOnLayoutPrepare(ZF_IN const ZFUIRect &bounds)
    {
        this->pimplOwner->layoutDialogPrepare(bounds);
    }
    zfoverride
    virtual void layoutOnLayout(ZF_IN const ZFUIRect &bounds)
    {
        this->pimplOwner->layoutDialog(bounds);
    }
};

zfclass _ZFP_I_ZFUIDialog_DialogClickMask : zfextends ZFUIButton
{
    ZFOBJECT_DECLARE(_ZFP_I_ZFUIDialog_DialogClickMask, ZFUIButton)
};

// ============================================================
// ZFUIDialog
ZFOBJECT_REGISTER(ZFUIDialog)

ZFOBSERVER_EVENT_REGISTER(ZFUIDialog, DialogBeforeShow)
ZFOBSERVER_EVENT_REGISTER(ZFUIDialog, DialogAfterShow)
ZFOBSERVER_EVENT_REGISTER(ZFUIDialog, DialogBeforeHide)
ZFOBSERVER_EVENT_REGISTER(ZFUIDialog, DialogAfterHide)
ZFOBSERVER_EVENT_REGISTER(ZFUIDialog, DialogFocusOnUpdate)

ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIDialog, ZFUIColor, dialogWindowColor)
{
    d->dialogWindowBg->viewBackgroundColor(this->dialogWindowColor());
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIDialog, ZFUIImage *, dialogBackgroundImage)
{
    d->dialogBg->image(this->dialogBackgroundImage());
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIDialog, ZFUIView *, dialogView)
{
    if(this->dialogView() != zfnull)
    {
        d->dialogContainer->childAdd(this->dialogView());
        this->dialogView()->layoutParam()->layoutAlign(ZFUIAlign::e_Center);
    }
}
ZFPROPERTY_ON_DETACH_DEFINE(ZFUIDialog, ZFUIView *, dialogView)
{
    if(this->dialogView() != zfnull)
    {
        d->dialogContainer->childRemove(this->dialogView());
    }
}

ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIDialog, zfbool, dialogWindowAutoResize)
{
    if(this->dialogWindowAutoResize())
    {
        this->dialogWindowAutoFit(zffalse);
    }
}

ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIDialog, zfbool, dialogWindowAutoFit)
{
    if(this->dialogWindowAutoFit())
    {
        this->dialogWindowAutoResize(zffalse);
    }
    this->dialogWindowAutoFitLayout()->autoFitEnable(this->dialogWindowAutoFit());
}

ZFMETHOD_DEFINE_0(ZFUIDialog, ZFUIOnScreenKeyboardAutoFitLayout *, dialogWindowAutoFitLayout)
{
    return d->dialogContainer;
}

ZFMETHOD_DEFINE_0(ZFUIDialog, zfbool, dialogShowing)
{
    return d->windowShowing();
}
ZFMETHOD_DEFINE_0(ZFUIDialog, void, dialogShow)
{
    if(d->windowShowing())
    {
        return ;
    }
    zfRetain(this);
    d->aniStop();
    d->windowShow();

    if(this->dialogWindowAutoResize())
    {
        ZFUIOnScreenKeyboardAutoResizeStart(this->dialogWindow());
    }

    d->viewUIEnableTree(zffalse);
    this->dialogBeforeShow();
    if(this->dialogWindowColor() != ZFUIColorZero() && this->dialogWindowAutoDim())
    {
        d->dialogWindowAniShow->aniTarget(d->dialogWindowBg);
        d->dialogWindowAniShow->observerAddForOnce(
            ZFAnimation::EventAniOnStopOrInvalid(),
            d->aniShowOnStopListener);
        d->dialogWindowAniShow->aniStart();
    }
    if(this->dialogAniShow() != zfnull)
    {
        this->dialogAniShow()->aniTarget(d->dialogBg);
        this->dialogAniShow()->observerAddForOnce(
            ZFAnimation::EventAniOnStopOrInvalid(),
            d->aniShowOnStopListener);
        this->dialogAniShow()->aniStart();
    }
}
ZFMETHOD_DEFINE_0(ZFUIDialog, void, dialogHide)
{
    if(!d->windowShowing())
    {
        return ;
    }
    d->aniStop();
    if(!d->windowShowing())
    {
        return ;
    }

    if(this->dialogWindowAutoResize())
    {
        ZFUIOnScreenKeyboardAutoResizeStop(this->dialogWindow());
    }

    d->viewUIEnableTree(zffalse);
    this->dialogBeforeHide();
    if(this->dialogWindowColor() != ZFUIColorZero() && this->dialogWindowAutoDim())
    {
        d->dialogWindowAniHide->aniTarget(d->dialogWindowBg);
        d->dialogWindowAniHide->observerAddForOnce(
            ZFAnimation::EventAniOnStopOrInvalid(),
            d->aniHideOnStopListener);
        d->dialogWindowAniHide->aniStart();
    }
    if(this->dialogAniHide() != zfnull)
    {
        this->dialogAniHide()->aniTarget(d->dialogBg);
        this->dialogAniHide()->observerAddForOnce(
            ZFAnimation::EventAniOnStopOrInvalid(),
            d->aniHideOnStopListener);
        this->dialogAniHide()->aniStart();
    }
}
ZFMETHOD_DEFINE_0(ZFUIDialog, void, dialogHideForce)
{
    if(d->windowShowing())
    {
        zfRetain(this);
        d->aniStop();
        if(d->windowShowing())
        {
            this->dialogBeforeHide();
            d->windowHide();
            this->dialogAfterHide();
        }
        zfRelease(this);
    }
}

ZFMETHOD_DEFINE_1(ZFUIDialog, void, dialogApplyAutoHide,
                  ZFMP_IN(ZFUIButton *, button))
{
    if(button == zfnull)
    {
        return ;
    }
    ZFLISTENER_LOCAL(buttonOnClick, {
        userData->objectHolded<ZFUIDialog *>()->dialogHide();
    })
    button->observerAdd(ZFObserverAddParam()
            .eventId(ZFUIButton::EventButtonOnClick())
            .observer(buttonOnClick)
            .userData(this->objectHolder())
            .observerLevel(ZFLevelZFFrameworkPostNormal)
        );
}

ZFMETHOD_DEFINE_0(ZFUIDialog, ZFUIWindow *, dialogWindow)
{
    return d;
}

ZFUIView *ZFUIDialog::dialogInternalContainer(void)
{
    return d->dialogContainer;
}
ZFUIView *ZFUIDialog::dialogInternalBackgroundContainer(void)
{
    return d->dialogBg;
}

ZFUIView *ZFUIDialog::dialogFocusOnUpdate(void)
{
    return ZFUIViewFocusNextMove(this->dialogInternalContainer(),
        ZFUIViewFocusNextParam().focusDirection(ZFUIOrientation::e_Left | ZFUIOrientation::e_Top));
}

void ZFUIDialog::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfAlloc(_ZFP_I_ZFUIDialogPrivate);
    d->pimplOwner = this;

    d->dialogWindowBg = zfAlloc(ZFUIView);
    d->internalBgViewAdd(d->dialogWindowBg);
    d->dialogWindowBg->layoutParam()->sizeParam(ZFUISizeParamFillFill());

    d->dialogClickMask = zfAlloc(_ZFP_I_ZFUIDialog_DialogClickMask);
    d->internalBgViewAdd(d->dialogClickMask);
    d->dialogClickMask->layoutParam()->sizeParam(ZFUISizeParamFillFill());
    d->dialogClickMask->observerAdd(ZFUIButton::EventButtonOnClick(),
        ZFCallbackForMemberMethod(d, ZFMethodAccess(_ZFP_I_ZFUIDialogPrivate, dialogClickMaskOnClick)));

    zfblockedAlloc(_ZFP_ZFUIDialogContentHolder, dialogContentHolder);
    d->childAdd(dialogContentHolder);
    dialogContentHolder->pimplOwner = d;
    dialogContentHolder->layoutParam()->sizeParam(ZFUISizeParamFillFill());
    dialogContentHolder->viewUIEnable(zffalse);

    d->dialogBg = zfAlloc(ZFUIImageView);
    dialogContentHolder->childAdd(d->dialogBg);
    d->dialogBg->viewUIEnableTree(zftrue);
    d->dialogBg->viewUIEnable(zftrue);

    d->dialogContainer = zfAlloc(ZFUIOnScreenKeyboardAutoFitLayout);
    d->dialogBg->childAdd(d->dialogContainer);
    d->dialogContainer->layoutParam()->sizeParam(ZFUISizeParamFillFill());

    d->dialogWindowAniShow = zfAlloc(ZFAnimationNativeView);
    d->dialogWindowAniShow->aniAlphaFrom(0);
    d->dialogWindowAniHide = zfAlloc(ZFAnimationNativeView);
    d->dialogWindowAniHide->aniAlphaTo(0);

    d->aniShowOnStopListener = ZFCallbackForMemberMethod(d, ZFMethodAccess(_ZFP_I_ZFUIDialogPrivate, aniShowOnStop));
    d->aniHideOnStopListener = ZFCallbackForMemberMethod(d, ZFMethodAccess(_ZFP_I_ZFUIDialogPrivate, aniHideOnStop));

    d->dialogWindowBg->viewBackgroundColor(this->dialogWindowColor());
    d->dialogBg->image(this->dialogBackgroundImage());

    _ZFP_ZFUIDialogAllDialog.add(this);
}
void ZFUIDialog::objectOnDealloc(void)
{
    _ZFP_ZFUIDialogAllDialog.removeElement(this);

    zfRetainChange(d->dialogWindowAniShow, zfnull);
    zfRetainChange(d->dialogWindowAniHide, zfnull);

    zfRelease(d->dialogContainer);
    zfRelease(d->dialogBg);
    zfRelease(d->dialogClickMask);
    zfRelease(d->dialogWindowBg);
    zfRelease(d);
    d = zfnull;

    zfsuper::objectOnDealloc();
}

ZF_NAMESPACE_GLOBAL_END

