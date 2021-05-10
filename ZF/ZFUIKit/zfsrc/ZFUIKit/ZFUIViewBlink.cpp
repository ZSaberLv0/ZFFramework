#include "ZFUIViewBlink.h"
#include "ZFUIImageView.h"
#include "ZFAnimationNativeView.h"

#include "ZFUIKit/protocol/ZFProtocolZFAnimationNativeView.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIImage.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIImageIO.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
ZFOBSERVER_EVENT_GLOBAL_REGISTER(ViewBlinkOn)
ZFOBSERVER_EVENT_GLOBAL_REGISTER(ViewBlinkOff)
ZF_NAMESPACE_END(ZFGlobalEvent)

// ============================================================
#define _ZFP_ZFUIViewBlink_DEBUG_noAni 0
#define _ZFP_ZFUIViewBlink_DEBUG_duration 0

#define _ZFP_ZFUIViewBlink_tag_ani "_ZFP_ZFUIViewBlink_tag_ani"
#define _ZFP_ZFUIViewBlink_tag_blinkView "_ZFP_ZFUIViewBlink_tag_blinkView"
#define _ZFP_ZFUIViewBlink_tag_delayTaskId "_ZFP_ZFUIViewBlink_tag_delayTaskId"
#define _ZFP_ZFUIViewBlink_tag_delayId "_ZFP_ZFUIViewBlink_tag_delayId"
#define _ZFP_ZFUIViewBlink_tag_blinkCountLeft "_ZFP_ZFUIViewBlink_tag_blinkCountLeft"

zfclass _ZFP_ZFUIViewBlinkView : zfextends ZFUIImageView
{
    ZFOBJECT_DECLARE(_ZFP_ZFUIViewBlinkView, ZFUIImageView)
};

static void _ZFP_ZFUIViewBlinkDoOn(ZF_IN ZFUIView *view, ZF_IN const ZFUIViewBlinkParam &blinkParam);
static void _ZFP_ZFUIViewBlinkDoOff(ZF_IN ZFUIView *view);
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIViewBlinkDataHolder, ZFLevelZFFrameworkEssential)
{
    this->viewOnDeallocListener = ZFCallbackForFunc(zfself::viewOnDealloc);
}
public:
    ZFListener viewOnDeallocListener;
    ZFCoreArrayPOD<ZFUIView *> blinkingViews;
    ZFIdentityGenerator delayTaskIdGenerator;
    static void viewOnDealloc(ZF_IN const ZFListenerData &listenerData, ZF_IN ZFObject *userData)
    {
        _ZFP_ZFUIViewBlinkDoOff(listenerData.sender<ZFUIView *>());
    }
ZF_GLOBAL_INITIALIZER_END(ZFUIViewBlinkDataHolder)

// ============================================================
ZFTYPEID_ACCESS_ONLY_DEFINE(ZFUIViewBlinkParam, ZFUIViewBlinkParam)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_SETTER_GETTER(v_ZFUIViewBlinkParam, ZFUIImage *, blinkImage)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_SETTER_GETTER(v_ZFUIViewBlinkParam, zftimet, blinkDuration)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_SETTER_GETTER(v_ZFUIViewBlinkParam, zfindex, blinkCount)

// ============================================================
ZFMETHOD_FUNC_DEFINE_2(void, ZFUIViewBlink,
                       ZFMP_IN(ZFUIView *, view),
                       ZFMP_IN_OPT(const ZFUIViewBlinkParam &, blinkParam, ZFUIViewBlinkParam()))
{
    if(blinkParam.blinkCount() <= 0)
    {
        return ;
    }
    if(view)
    {
        _ZFP_ZFUIViewBlinkDoOn(view, blinkParam);
    }
}
ZFMETHOD_FUNC_INLINE_DEFINE_4(void, ZFUIViewBlink,
                              ZFMP_IN(ZFUIView *, view),
                              ZFMP_IN(ZFUIImage *, blinkImage),
                              ZFMP_IN_OPT(zftimet, blinkDuration, zftimetZero()),
                              ZFMP_IN_OPT(zfindex, blinkCount, 1))

// ============================================================
// blink action
static void _ZFP_ZFUIViewBlink_noAni_doOff(ZF_IN ZFUIView *view);
static void _ZFP_ZFUIViewBlinkDoOn(ZF_IN ZFUIView *view, ZF_IN const ZFUIViewBlinkParam &blinkParam)
{
    _ZFP_ZFUIViewBlinkView *blinkView = view->objectTag<_ZFP_ZFUIViewBlinkView *>(_ZFP_ZFUIViewBlink_tag_blinkView);
    if(blinkView != zfnull)
    {
        return ;
    }

    {
        zfblockedAlloc(_ZFP_ZFUIViewBlinkView, t);
        view->internalFgViewAdd(t);
        view->objectTag(_ZFP_ZFUIViewBlink_tag_blinkView, t);
        t->layoutParam()->sizeParam(ZFUISizeParamFillFill());
        blinkView = t;
    }
    blinkView->image((blinkParam.blinkImage() != zfnull)
        ? blinkParam.blinkImage()
        : ZFUIViewBlinkImageDefault().to<ZFUIImage *>());

    if(ZFPROTOCOL_IS_AVAILABLE(ZFAnimationNativeView) && !_ZFP_ZFUIViewBlink_DEBUG_noAni)
    {
        if(blinkParam.blinkCount() > 1)
        {
            view->objectTag(_ZFP_ZFUIViewBlink_tag_blinkCountLeft, zflineAlloc(v_zfindex, blinkParam.blinkCount() - 1));
        }

        zfblockedAlloc(ZFAnimationNativeView, ani);
        view->objectTag(_ZFP_ZFUIViewBlink_tag_ani, ani);
        ani->aniAlphaTo(0);
        #if _ZFP_ZFUIViewBlink_DEBUG_duration
            ani->aniDuration(5000);
        #else
            ani->aniDuration(blinkParam.blinkDuration());
        #endif

        ZFLISTENER_LOCAL(aniOnStopListener, {
            ZFAnimation *ani = listenerData.sender<ZFAnimation *>();
            ZFUIView *blinkView = ani->aniTarget()->to<ZFUIView *>();
            ZFUIView *view = userData->objectHolded();

            v_zfindex *blinkCountLeft = view->objectTag<v_zfindex *>(_ZFP_ZFUIViewBlink_tag_blinkCountLeft);
            if(blinkCountLeft != zfnull)
            {
                if(blinkCountLeft->zfv <= 1)
                {
                    view->objectTagRemove(_ZFP_ZFUIViewBlink_tag_blinkCountLeft);
                }
                else
                {
                    blinkCountLeft->zfv = blinkCountLeft->zfv - 1;
                }

                ani->aniStart();
                return ;
            }

            view->observerRemove(ZFObject::EventObjectBeforeDealloc(), ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewBlinkDataHolder)->viewOnDeallocListener);

            view->objectTagRemove(_ZFP_ZFUIViewBlink_tag_ani);
            view->objectTagRemove(_ZFP_ZFUIViewBlink_tag_blinkView);
            blinkView->viewRemoveFromParent();
            ani->aniTarget(zfnull);

            ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewBlinkDataHolder)->blinkingViews.removeElement(view);
            ZFGlobalObserver().observerNotifyWithCustomSender(view, ZFGlobalEvent::EventViewBlinkOff());
        })
        view->observerAdd(ZFObject::EventObjectBeforeDealloc(), ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewBlinkDataHolder)->viewOnDeallocListener);
        ani->observerAdd(ZFAnimation::EventAniOnStopOrInvalid(), aniOnStopListener, view->objectHolder());
        ani->aniTarget(blinkView);

        ZFGlobalObserver().observerNotifyWithCustomSender(view, ZFGlobalEvent::EventViewBlinkOn());
        ani->aniStart();
    }
    else
    {
        view->observerAdd(ZFObject::EventObjectBeforeDealloc(), ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewBlinkDataHolder)->viewOnDeallocListener);

        ZFGlobalObserver().observerNotifyWithCustomSender(view, ZFGlobalEvent::EventViewBlinkOn());
        zfidentity delayTaskId = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewBlinkDataHolder)->delayTaskIdGenerator.idAcquire();
        zfblockedAlloc(v_zfidentity, delayTaskIdTag, delayTaskId);
        view->objectTag(_ZFP_ZFUIViewBlink_tag_delayTaskId, delayTaskIdTag);

        ZFLISTENER_LAMBDA_1(blinkDelayOnFinish,
            zfautoObject, delayTaskIdTag, {
            ZFUIView *view = userData->objectHolded();
            v_zfidentity *delayTaskId = delayTaskIdTag;
            v_zfidentity *delayTaskIdCur = view->objectTag<v_zfidentity *>(_ZFP_ZFUIViewBlink_tag_delayTaskId);
            if(delayTaskId != delayTaskIdCur)
            {
                return ;
            }

            ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewBlinkDataHolder)->delayTaskIdGenerator.idRelease(delayTaskId->zfv);
            _ZFP_ZFUIViewBlink_noAni_doOff(view);
        })
        zfidentity delayId = ZFExecuteAfterDelay(
            #if _ZFP_ZFUIViewBlink_DEBUG_duration
                (zftimet)5000
            #else
                (zftimet)(blinkParam.blinkDuration() * blinkParam.blinkCount())
            #endif
            ,
            blinkDelayOnFinish,
            view->objectHolder());
        view->objectTag(_ZFP_ZFUIViewBlink_tag_delayId, zflineAlloc(v_zfidentity, delayId));
    }
}
static void _ZFP_ZFUIViewBlink_noAni_doOff(ZF_IN ZFUIView *view)
{
    ZFUIView *blinkView = view->objectTag<ZFUIView *>(_ZFP_ZFUIViewBlink_tag_blinkView);

    view->objectTagRemove(_ZFP_ZFUIViewBlink_tag_blinkView);
    blinkView->viewRemoveFromParent();

    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewBlinkDataHolder)->blinkingViews.removeElement(view);
    view->objectTagRemove(_ZFP_ZFUIViewBlink_tag_delayTaskId);
    view->objectTagRemove(_ZFP_ZFUIViewBlink_tag_delayId);
    ZFGlobalObserver().observerNotifyWithCustomSender(view, ZFGlobalEvent::EventViewBlinkOff());
}
static void _ZFP_ZFUIViewBlinkDoOff(ZF_IN ZFUIView *view)
{
    _ZFP_ZFUIViewBlinkView *blinkView = view->objectTag<_ZFP_ZFUIViewBlinkView *>(_ZFP_ZFUIViewBlink_tag_blinkView);
    if(blinkView == zfnull)
    {
        return ;
    }

    ZFAnimation *ani = view->objectTag<ZFAnimation *>(_ZFP_ZFUIViewBlink_tag_ani);
    if(ani != zfnull)
    {
        view->objectTagRemove(_ZFP_ZFUIViewBlink_tag_blinkCountLeft);
        ani->aniStop();
    }
    else
    {
        _ZFP_ZFUIViewBlink_noAni_doOff(view);
    }
}

// ============================================================
// other
ZFEXPORT_VAR_DEFINE(zfautoObject, ZFUIViewBlinkImageDefault, zfnull)
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIViewBlinkInitSetting, ZFLevelZFFrameworkNormal)
{
    (void)ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewBlinkDataHolder);
    if(ZFPROTOCOL_IS_AVAILABLE(ZFUIImage))
    {
        ZFUIViewBlinkImageDefault(zfRes("ZFUIKit/ZFUIViewBlinkImage.xml"));
    }
    else if(ZFPROTOCOL_IS_AVAILABLE(ZFUIImageIO))
    {
        ZFUIViewBlinkImageDefault(ZFUIImageLoadFromColor(ZFUIColorMake(1, 0, 0)));
    }
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIViewBlinkInitSetting)
{
    ZFUIViewBlinkImageDefault(zfnull);
}
ZF_GLOBAL_INITIALIZER_END(ZFUIViewBlinkInitSetting)

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIViewBlinkCleanup, ZFLevelZFFrameworkLow)
{
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIViewBlinkCleanup)
{
    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIViewBlinkDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewBlinkDataHolder);
    while(!d->blinkingViews.isEmpty())
    {
        _ZFP_ZFUIViewBlinkDoOff(d->blinkingViews[d->blinkingViews.count() - 1]);
    }
}
ZF_GLOBAL_INITIALIZER_END(ZFUIViewBlinkCleanup)

ZF_NAMESPACE_GLOBAL_END

