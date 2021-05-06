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
        container->childAdd(view, ZFUISizeParamFillFill(), ZFUIAlign::e_Center);
        view->layoutParam()->sizeHint(ZFUISizeMake(100));
        view->image(zfRes("test_normal.png"));

        zfblockedAlloc(ZFUIKit_test_Button, scaleXDec);
        window->childAdd(scaleXDec, ZFUISizeParamWrapWrap(), ZFUIAlign::e_BottomInner, ZFUIMarginMake(0, 0, 100, 100));
        scaleXDec->buttonLabelText("scaleX -");
        ZFLISTENER_LOCAL(scaleXOnDec, {
            ZFUIImageView *view = userData->objectHolded();
            view->viewScaleX(view->viewScaleX() - 0.1f);
        })
        scaleXDec->onClick(scaleXOnDec, view->objectHolder());

        zfblockedAlloc(ZFUIKit_test_Button, scaleXInc);
        window->childAdd(scaleXInc, ZFUISizeParamWrapWrap(), ZFUIAlign::e_BottomInner, ZFUIMarginMake(100, 0, 0, 100));
        scaleXInc->buttonLabelText("scaleX +");
        ZFLISTENER_LOCAL(scaleXOnInc, {
            ZFUIImageView *view = userData->objectHolded();
            view->viewScaleX(view->viewScaleX() + 0.1f);
        })
        scaleXInc->onClick(scaleXOnInc, view->objectHolder());

        zfblockedAlloc(ZFUIKit_test_Button, scaleYDec);
        window->childAdd(scaleYDec, ZFUISizeParamWrapWrap(), ZFUIAlign::e_BottomInner, ZFUIMarginMake(0, 0, 100, 50));
        scaleYDec->buttonLabelText("scaleY -");
        ZFLISTENER_LOCAL(scaleYOnDec, {
            ZFUIImageView *view = userData->objectHolded();
            view->viewScaleY(view->viewScaleY() - 0.1f);
        })
        scaleYDec->onClick(scaleYOnDec, view->objectHolder());

        zfblockedAlloc(ZFUIKit_test_Button, scaleYInc);
        window->childAdd(scaleYInc, ZFUISizeParamWrapWrap(), ZFUIAlign::e_BottomInner, ZFUIMarginMake(100, 0, 0, 50));
        scaleYInc->buttonLabelText("scaleY +");
        ZFLISTENER_LOCAL(scaleYOnInc, {
            ZFUIImageView *view = userData->objectHolded();
            view->viewScaleY(view->viewScaleY() + 0.1f);
        })
        scaleYInc->onClick(scaleYOnInc, view->objectHolder());

        zfblockedAlloc(ZFUIKit_test_Button, rotateDec);
        window->childAdd(rotateDec, ZFUISizeParamWrapWrap(), ZFUIAlign::e_BottomInner, ZFUIMarginMake(0, 0, 100, 0));
        rotateDec->buttonLabelText("rotate -");
        ZFLISTENER_LOCAL(rotateOnDec, {
            ZFUIImageView *view = userData->objectHolded();
            view->viewRotation(view->viewRotation() - 10);
        })
        rotateDec->onClick(rotateOnDec, view->objectHolder());

        zfblockedAlloc(ZFUIKit_test_Button, rotateInc);
        window->childAdd(rotateInc, ZFUISizeParamWrapWrap(), ZFUIAlign::e_BottomInner, ZFUIMarginMake(100, 0, 0, 0));
        rotateInc->buttonLabelText("rotate +");
        ZFLISTENER_LOCAL(rotateOnInc, {
            ZFUIImageView *view = userData->objectHolded();
            view->viewRotation(view->viewRotation() + 10);
        })
        rotateInc->onClick(rotateOnInc, view->objectHolder());
    }
};
ZFOBJECT_REGISTER(ZFUIKit_ZFUIViewTransform_test)

ZF_NAMESPACE_GLOBAL_END

