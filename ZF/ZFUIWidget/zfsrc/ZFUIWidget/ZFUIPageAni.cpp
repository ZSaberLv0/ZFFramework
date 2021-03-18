#include "ZFUIPageAni.h"

ZF_NAMESPACE_GLOBAL_BEGIN

static zfbool _ZFP_ZFUIPageAniCheck(ZF_IN ZFUIPage *page, ZFEnum *resumeOrPauseReason, ZFUIPage *siblingPage)
{
    if(zffalse
        || (page == zfnull && siblingPage == zfnull)
        || resumeOrPauseReason == zfnull
    ) {
        return zffalse;
    }
    if(resumeOrPauseReason->classData()->classIsTypeOf(ZFUIPageResumeReason::ClassData()))
    {
        return resumeOrPauseReason->enumValue() != ZFUIPageResumeReason::e_ByManagerResume;
    }
    else if(resumeOrPauseReason->classData()->classIsTypeOf(ZFUIPagePauseReason::ClassData()))
    {
        return resumeOrPauseReason->enumValue() != ZFUIPagePauseReason::e_ByManagerPause;
    }
    else
    {
        return zffalse;
    }
}

ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFUIPageAniPrepareForAlpha,
                       ZFMP_IN(ZFUIPage *, page),
                       ZFMP_IN(ZFEnum *, resumeOrPauseReason),
                       ZFMP_IN(ZFUIPage *, siblingPage))
{
    if(!_ZFP_ZFUIPageAniCheck(page, resumeOrPauseReason, siblingPage)) {return zffalse;}

    ZFUIPage *alphaInPage = zfnull;
    ZFUIPage *alphaOutPage = zfnull;
    if(resumeOrPauseReason->classData()->classIsTypeOf(ZFUIPageResumeReason::ClassData()))
    {
        alphaInPage = page;
        alphaOutPage = siblingPage;
    }
    else
    {
        alphaInPage = siblingPage;
        alphaOutPage = page;
    }

    if(alphaInPage != zfnull)
    {
        zfblockedAlloc(ZFAnimationNativeView, alphaInAni);
        alphaInPage->pageAni(alphaInAni);
        alphaInAni->aniAlphaFrom(0);
    }
    if(alphaOutPage != zfnull)
    {
        zfblockedAlloc(ZFAnimationNativeView, alphaOutAni);
        alphaOutPage->pageAni(alphaOutAni);
        alphaOutAni->aniAlphaTo(0);
    }
    return zftrue;
}

ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFUIPageAniPrepareForSlide,
                       ZFMP_IN(ZFUIPage *, page),
                       ZFMP_IN(ZFEnum *, resumeOrPauseReason),
                       ZFMP_IN(ZFUIPage *, siblingPage))
{
    if(!_ZFP_ZFUIPageAniCheck(page, resumeOrPauseReason, siblingPage)) {return zffalse;}

    if(resumeOrPauseReason->classData()->classIsTypeOf(ZFUIPageResumeReason::ClassData()))
    {
        if(resumeOrPauseReason->enumValue() == ZFUIPageResumeReason::e_ByRequest)
        {
            if(page != zfnull)
            {
                zfblockedAlloc(ZFAnimationNativeView, requestInAni);
                page->pageAni(requestInAni);
                requestInAni->aniTranslateXFrom(1);
                requestInAni->aniCurve(ZFAnimationNativeViewCurve::e_EaseOut);
            }
            if(siblingPage != zfnull)
            {
                zfblockedAlloc(ZFAnimationNativeView, requestOutAni);
                siblingPage->pageAni(requestOutAni);
                requestOutAni->aniTranslateXTo(-1);
                requestOutAni->aniCurve(ZFAnimationNativeViewCurve::e_EaseOut);
            }
        }
        else // ZFUIPageResumeReason::e_FromBackground
        {
            if(page != zfnull)
            {
                zfblockedAlloc(ZFAnimationNativeView, resumeInAni);
                page->pageAni(resumeInAni);
                resumeInAni->aniTranslateXFrom(-1);
                resumeInAni->aniCurve(ZFAnimationNativeViewCurve::e_EaseOut);
            }
            if(siblingPage != zfnull)
            {
                zfblockedAlloc(ZFAnimationNativeView, resumeOutAni);
                siblingPage->pageAni(resumeOutAni);
                resumeOutAni->aniTranslateXTo(1);
                resumeOutAni->aniCurve(ZFAnimationNativeViewCurve::e_EaseOut);
            }
        }
    }
    else
    {
        if(resumeOrPauseReason->enumValue() == ZFUIPagePauseReason::e_ToBackground)
        {
            if(siblingPage != zfnull)
            {
                zfblockedAlloc(ZFAnimationNativeView, requestInAni);
                siblingPage->pageAni(requestInAni);
                requestInAni->aniTranslateXFrom(1);
                requestInAni->aniCurve(ZFAnimationNativeViewCurve::e_EaseOut);
            }
            if(page != zfnull)
            {
                zfblockedAlloc(ZFAnimationNativeView, requestOutAni);
                page->pageAni(requestOutAni);
                requestOutAni->aniTranslateXTo(-1);
                requestOutAni->aniCurve(ZFAnimationNativeViewCurve::e_EaseOut);
            }
        }
        else // ZFUIPagePauseReason::e_BeforeDestroy
        {
            if(siblingPage != zfnull)
            {
                zfblockedAlloc(ZFAnimationNativeView, resumeInAni);
                siblingPage->pageAni(resumeInAni);
                resumeInAni->aniTranslateXFrom(-1);
                resumeInAni->aniCurve(ZFAnimationNativeViewCurve::e_EaseOut);
            }
            if(page != zfnull)
            {
                zfblockedAlloc(ZFAnimationNativeView, resumeOutAni);
                page->pageAni(resumeOutAni);
                resumeOutAni->aniTranslateXTo(1);
                resumeOutAni->aniCurve(ZFAnimationNativeViewCurve::e_EaseOut);
            }
        }
    }
    return zftrue;
}

ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFUIPageAniPrepareForPopup,
                       ZFMP_IN(ZFUIPage *, page),
                       ZFMP_IN(ZFEnum *, resumeOrPauseReason),
                       ZFMP_IN(ZFUIPage *, siblingPage))
{
    if(!_ZFP_ZFUIPageAniCheck(page, resumeOrPauseReason, siblingPage)) {return zffalse;}

    if(resumeOrPauseReason->classData()->classIsTypeOf(ZFUIPageResumeReason::ClassData()))
    {
        if(resumeOrPauseReason->enumValue() == ZFUIPageResumeReason::e_ByRequest)
        {
            if(page != zfnull)
            {
                zfblockedAlloc(ZFAnimationNativeView, requestInAni);
                page->pageAni(requestInAni);
                requestInAni->aniTranslateYFrom(0);
                requestInAni->aniCurve(ZFAnimationNativeViewCurve::e_EaseOut);
            }
            if(siblingPage != zfnull)
            {
                zfblockedAlloc(ZFAnimation, requestOutAni);
                siblingPage->pageAni(requestOutAni);
            }
        }
        else // ZFUIPageResumeReason::e_FromBackground
        {
            if(page != zfnull)
            {
                zfblockedAlloc(ZFAnimation, resumeInAni);
                page->pageAni(resumeInAni);
            }
            if(siblingPage != zfnull)
            {
                zfblockedAlloc(ZFAnimationNativeView, resumeOutAni);
                siblingPage->pageAni(resumeOutAni);
                resumeOutAni->aniTranslateYTo(1);
                resumeOutAni->aniCurve(ZFAnimationNativeViewCurve::e_EaseIn);
            }
        }
    }
    else
    {
        if(resumeOrPauseReason->enumValue() == ZFUIPagePauseReason::e_ToBackground)
        {
            if(siblingPage != zfnull)
            {
                zfblockedAlloc(ZFAnimationNativeView, requestInAni);
                siblingPage->pageAni(requestInAni);
                requestInAni->aniTranslateYFrom(0);
                requestInAni->aniCurve(ZFAnimationNativeViewCurve::e_EaseOut);
            }
            if(page != zfnull)
            {
                zfblockedAlloc(ZFAnimation, requestOutAni);
                page->pageAni(requestOutAni);
            }
        }
        else // ZFUIPagePauseReason::e_BeforeDestroy
        {
            if(siblingPage != zfnull)
            {
                zfblockedAlloc(ZFAnimation, resumeInAni);
                siblingPage->pageAni(resumeInAni);
            }
            if(page != zfnull)
            {
                zfblockedAlloc(ZFAnimationNativeView, resumeOutAni);
                page->pageAni(resumeOutAni);
                resumeOutAni->aniTranslateYTo(1);
                resumeOutAni->aniCurve(ZFAnimationNativeViewCurve::e_EaseIn);
            }
        }
    }
    return zftrue;
}

ZF_NAMESPACE_GLOBAL_END

