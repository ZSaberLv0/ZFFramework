#include "ZFUIKit_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUIKit_ZFUIViewTransform_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFUIKit_ZFUIViewTransform_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView);
        ZFUIWindow *window = zfnull;
        ZFUIView *container = zfnull;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        zfblockedAlloc(ZFUIImageView, view);
        container->childAdd(view, ZFUISizeParamFillFill(), ZFUIAlign::e_TopInner, ZFUIMarginMake(0, 100, 0, 0));
        view->layoutParam()->sizeHint(ZFUISizeMake(100));
        view->image(zfRes("test_normal.png"));

        zffloat margin = 120;

        // viewTranslateX
        zfblockedAlloc(ZFUIKit_test_Button, viewTranslateXDec);
        window->childAdd(viewTranslateXDec, ZFUISizeParamWrapWrap(), ZFUIAlign::e_BottomInner, ZFUIMarginMake(0, 0, margin, 200));
        viewTranslateXDec->buttonLabelText("viewTranslateX -");
        ZFLISTENER_LOCAL(viewTranslateXOnDec, {
            ZFUIImageView *view = userData->objectHolded();
            view->viewTranslateX(view->viewTranslateX() - 10);
        })
        viewTranslateXDec->onClick(viewTranslateXOnDec, view->objectHolder());

        zfblockedAlloc(ZFUIKit_test_Button, viewTranslateXInc);
        window->childAdd(viewTranslateXInc, ZFUISizeParamWrapWrap(), ZFUIAlign::e_BottomInner, ZFUIMarginMake(margin, 0, 0, 200));
        viewTranslateXInc->buttonLabelText("viewTranslateX +");
        ZFLISTENER_LOCAL(viewTranslateXOnInc, {
            ZFUIImageView *view = userData->objectHolded();
            view->viewTranslateX(view->viewTranslateX() + 10);
        })
        viewTranslateXInc->onClick(viewTranslateXOnInc, view->objectHolder());

        // viewTranslateY
        zfblockedAlloc(ZFUIKit_test_Button, viewTranslateYDec);
        window->childAdd(viewTranslateYDec, ZFUISizeParamWrapWrap(), ZFUIAlign::e_BottomInner, ZFUIMarginMake(0, 0, margin, 150));
        viewTranslateYDec->buttonLabelText("viewTranslateY -");
        ZFLISTENER_LOCAL(viewTranslateYOnDec, {
            ZFUIImageView *view = userData->objectHolded();
            view->viewTranslateY(view->viewTranslateY() - 10);
        })
        viewTranslateYDec->onClick(viewTranslateYOnDec, view->objectHolder());

        zfblockedAlloc(ZFUIKit_test_Button, viewTranslateYInc);
        window->childAdd(viewTranslateYInc, ZFUISizeParamWrapWrap(), ZFUIAlign::e_BottomInner, ZFUIMarginMake(margin, 0, 0, 150));
        viewTranslateYInc->buttonLabelText("viewTranslateY +");
        ZFLISTENER_LOCAL(viewTranslateYOnInc, {
            ZFUIImageView *view = userData->objectHolded();
            view->viewTranslateY(view->viewTranslateY() + 10);
        })
        viewTranslateYInc->onClick(viewTranslateYOnInc, view->objectHolder());

        // viewScaleX
        zfblockedAlloc(ZFUIKit_test_Button, viewScaleXDec);
        window->childAdd(viewScaleXDec, ZFUISizeParamWrapWrap(), ZFUIAlign::e_BottomInner, ZFUIMarginMake(0, 0, margin, 100));
        viewScaleXDec->buttonLabelText("viewScaleX -");
        ZFLISTENER_LOCAL(viewScaleXOnDec, {
            ZFUIImageView *view = userData->objectHolded();
            view->viewScaleX(view->viewScaleX() - 0.1f);
        })
        viewScaleXDec->onClick(viewScaleXOnDec, view->objectHolder());

        zfblockedAlloc(ZFUIKit_test_Button, viewScaleXInc);
        window->childAdd(viewScaleXInc, ZFUISizeParamWrapWrap(), ZFUIAlign::e_BottomInner, ZFUIMarginMake(margin, 0, 0, 100));
        viewScaleXInc->buttonLabelText("viewScaleX +");
        ZFLISTENER_LOCAL(viewScaleXOnInc, {
            ZFUIImageView *view = userData->objectHolded();
            view->viewScaleX(view->viewScaleX() + 0.1f);
        })
        viewScaleXInc->onClick(viewScaleXOnInc, view->objectHolder());

        // viewScaleY
        zfblockedAlloc(ZFUIKit_test_Button, viewScaleYDec);
        window->childAdd(viewScaleYDec, ZFUISizeParamWrapWrap(), ZFUIAlign::e_BottomInner, ZFUIMarginMake(0, 0, margin, 50));
        viewScaleYDec->buttonLabelText("viewScaleY -");
        ZFLISTENER_LOCAL(viewScaleYOnDec, {
            ZFUIImageView *view = userData->objectHolded();
            view->viewScaleY(view->viewScaleY() - 0.1f);
        })
        viewScaleYDec->onClick(viewScaleYOnDec, view->objectHolder());

        zfblockedAlloc(ZFUIKit_test_Button, viewScaleYInc);
        window->childAdd(viewScaleYInc, ZFUISizeParamWrapWrap(), ZFUIAlign::e_BottomInner, ZFUIMarginMake(margin, 0, 0, 50));
        viewScaleYInc->buttonLabelText("viewScaleY +");
        ZFLISTENER_LOCAL(viewScaleYOnInc, {
            ZFUIImageView *view = userData->objectHolded();
            view->viewScaleY(view->viewScaleY() + 0.1f);
        })
        viewScaleYInc->onClick(viewScaleYOnInc, view->objectHolder());

        // viewRotate
        zfblockedAlloc(ZFUIKit_test_Button, viewRotateDec);
        window->childAdd(viewRotateDec, ZFUISizeParamWrapWrap(), ZFUIAlign::e_BottomInner, ZFUIMarginMake(0, 0, margin, 0));
        viewRotateDec->buttonLabelText("viewRotate -");
        ZFLISTENER_LOCAL(viewRotateOnDec, {
            ZFUIImageView *view = userData->objectHolded();
            view->viewRotate(view->viewRotate() - 10);
        })
        viewRotateDec->onClick(viewRotateOnDec, view->objectHolder());

        zfblockedAlloc(ZFUIKit_test_Button, viewRotateInc);
        window->childAdd(viewRotateInc, ZFUISizeParamWrapWrap(), ZFUIAlign::e_BottomInner, ZFUIMarginMake(margin, 0, 0, 0));
        viewRotateInc->buttonLabelText("viewRotate +");
        ZFLISTENER_LOCAL(viewRotateOnInc, {
            ZFUIImageView *view = userData->objectHolded();
            view->viewRotate(view->viewRotate() + 10);
        })
        viewRotateInc->onClick(viewRotateOnInc, view->objectHolder());
    }
};
ZFOBJECT_REGISTER(ZFUIKit_ZFUIViewTransform_test)

ZF_NAMESPACE_GLOBAL_END

