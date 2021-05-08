#include "ZFUIKit_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUIKit_ZFAnimationNativeView_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFUIKit_ZFAnimationNativeView_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView);
        ZFUIWindow *window = zfnull;
        ZFUIView *container = zfnull;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        zfblockedAlloc(ZFUIView, view);
        container->childAdd(view);
        view->viewSizePrefer(ZFUISizeMake(80, 60));
        view->viewBackgroundColor(ZFUIColorRandom());
        view->layoutParam()->layoutAlign(ZFUIAlign::e_Center);

        this->ani()->aniTarget(view);

        this->prepareSettingButton(window, this->ani());

        ZFLISTENER_LOCAL(startOnClick, {
            userData->objectHolded<ZFAnimation *>()->aniStart();
        })
        zfblockedAlloc(ZFUIKit_test_Button, startButton);
        startButton->buttonLabelText("start");
        startButton->observerAdd(ZFUIButton::EventButtonOnClick(), startOnClick, this->ani()->objectHolder());
        container->childAdd(startButton);
        startButton->layoutParam()->layoutAlign(ZFUIAlign::e_TopInner | ZFUIAlign::e_RightInner);
    }

    ZFPROPERTY_RETAIN_READONLY(ZFAnimationNativeView *, ani, zflineAlloc(ZFAnimationNativeView))

private:
    void prepareSettingButton(ZF_IN ZFUIWindow *window,
                              ZF_IN ZFAnimationNativeView *ani)
    {
        zfblockedAlloc(ZFArray, settings);

        ZFUIKit_test_prepareSettingForResetProperty(settings, ani, ZFCoreArrayPODCreate(const ZFProperty *
                , ZFPropertyAccess(ZFAnimationNativeView, aniCurve)
                , ZFPropertyAccess(ZFAnimationNativeView, aniAlphaFrom)
                , ZFPropertyAccess(ZFAnimationNativeView, aniAlphaTo)
                , ZFPropertyAccess(ZFAnimationNativeView, aniScaleXFrom)
                , ZFPropertyAccess(ZFAnimationNativeView, aniScaleXTo)
                , ZFPropertyAccess(ZFAnimationNativeView, aniScaleYFrom)
                , ZFPropertyAccess(ZFAnimationNativeView, aniScaleYTo)
                , ZFPropertyAccess(ZFAnimationNativeView, aniScaleZFrom)
                , ZFPropertyAccess(ZFAnimationNativeView, aniScaleZTo)
                , ZFPropertyAccess(ZFAnimationNativeView, aniTranslateXFrom)
                , ZFPropertyAccess(ZFAnimationNativeView, aniTranslateXTo)
                , ZFPropertyAccess(ZFAnimationNativeView, aniTranslateYFrom)
                , ZFPropertyAccess(ZFAnimationNativeView, aniTranslateYTo)
                , ZFPropertyAccess(ZFAnimationNativeView, aniTranslateZFrom)
                , ZFPropertyAccess(ZFAnimationNativeView, aniTranslateZTo)
                , ZFPropertyAccess(ZFAnimationNativeView, aniTranslatePixelXFrom)
                , ZFPropertyAccess(ZFAnimationNativeView, aniTranslatePixelXTo)
                , ZFPropertyAccess(ZFAnimationNativeView, aniTranslatePixelYFrom)
                , ZFPropertyAccess(ZFAnimationNativeView, aniTranslatePixelYTo)
                , ZFPropertyAccess(ZFAnimationNativeView, aniTranslatePixelZFrom)
                , ZFPropertyAccess(ZFAnimationNativeView, aniTranslatePixelZTo)
                , ZFPropertyAccess(ZFAnimationNativeView, aniRotateXFrom)
                , ZFPropertyAccess(ZFAnimationNativeView, aniRotateXTo)
                , ZFPropertyAccess(ZFAnimationNativeView, aniRotateYFrom)
                , ZFPropertyAccess(ZFAnimationNativeView, aniRotateYTo)
                , ZFPropertyAccess(ZFAnimationNativeView, aniRotateZFrom)
                , ZFPropertyAccess(ZFAnimationNativeView, aniRotateZTo)
            ));

        ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zftimet, ZFPropertyAccess(ZFAnimationNativeView, aniDuration),
            ZFCoreArrayPODCreate(zftimet
                , (zftimet)0
                , (zftimet)3000
                ));

        ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, ZFAnimationNativeViewCurveEnum, ZFPropertyAccess(ZFAnimationNativeView, aniCurve),
            ZFCoreArrayPODCreate(ZFAnimationNativeViewCurveEnum
                , ZFAnimationNativeViewCurve::e_Linear
                , ZFAnimationNativeViewCurve::e_EaseIn
                , ZFAnimationNativeViewCurve::e_EaseOut
                , ZFAnimationNativeViewCurve::e_EaseInOut
                ));

        { // aniAlpha
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAnimationNativeView, aniAlphaFrom),
                ZFCoreArrayPODCreate(zffloat
                    , 1.0f
                    , 0.0f
                    ));
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAnimationNativeView, aniAlphaTo),
                ZFCoreArrayPODCreate(zffloat
                    , 1.0f
                    , 0.0f
                    ));
        } // aniAlpha

        { // aniScale
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAnimationNativeView, aniScaleXFrom),
                ZFCoreArrayPODCreate(zffloat
                    , 1.0f
                    , 0.0f
                    ));
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAnimationNativeView, aniScaleXTo),
                ZFCoreArrayPODCreate(zffloat
                    , 1.0f
                    , 0.0f
                    ));

            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAnimationNativeView, aniScaleYFrom),
                ZFCoreArrayPODCreate(zffloat
                    , 1.0f
                    , 0.0f
                    ));
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAnimationNativeView, aniScaleYTo),
                ZFCoreArrayPODCreate(zffloat
                    , 1.0f
                    , 0.0f
                    ));

            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAnimationNativeView, aniScaleZFrom),
                ZFCoreArrayPODCreate(zffloat
                    , 1.0f
                    , 0.0f
                    ));
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAnimationNativeView, aniScaleZTo),
                ZFCoreArrayPODCreate(zffloat
                    , 1.0f
                    , 0.0f
                    ));
        } // aniScale

        { // aniTranslate
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAnimationNativeView, aniTranslateXFrom),
                ZFCoreArrayPODCreate(zffloat
                    , 0.0f
                    , 1.0f
                    ));
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAnimationNativeView, aniTranslateXTo),
                ZFCoreArrayPODCreate(zffloat
                    , 0.0f
                    , 1.0f
                    ));

            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAnimationNativeView, aniTranslateYFrom),
                ZFCoreArrayPODCreate(zffloat
                    , 0.0f
                    , 1.0f
                    ));
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAnimationNativeView, aniTranslateYTo),
                ZFCoreArrayPODCreate(zffloat
                    , 0.0f
                    , 1.0f
                    ));

            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAnimationNativeView, aniTranslateZFrom),
                ZFCoreArrayPODCreate(zffloat
                    , 0.0f
                    , 1.0f
                    ));
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAnimationNativeView, aniTranslateZTo),
                ZFCoreArrayPODCreate(zffloat
                    , 0.0f
                    , 1.0f
                    ));
        } // aniTranslate

        { // aniTranslatePixel
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAnimationNativeView, aniTranslatePixelXFrom),
                ZFCoreArrayPODCreate(zffloat
                    , 0
                    , 100
                    ));
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAnimationNativeView, aniTranslatePixelXTo),
                ZFCoreArrayPODCreate(zffloat
                    , 0
                    , 100
                    ));

            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAnimationNativeView, aniTranslatePixelYFrom),
                ZFCoreArrayPODCreate(zffloat
                    , 0
                    , 100
                    ));
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAnimationNativeView, aniTranslatePixelYTo),
                ZFCoreArrayPODCreate(zffloat
                    , 0
                    , 100
                    ));

            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAnimationNativeView, aniTranslatePixelZFrom),
                ZFCoreArrayPODCreate(zffloat
                    , 0
                    , 100
                    ));
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAnimationNativeView, aniTranslatePixelZTo),
                ZFCoreArrayPODCreate(zffloat
                    , 0
                    , 100
                    ));
        } // aniTranslatePixel

        { // aniRotate
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAnimationNativeView, aniRotateXFrom),
                ZFCoreArrayPODCreate(zffloat
                    , 0
                    , 90
                    ));
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAnimationNativeView, aniRotateXTo),
                ZFCoreArrayPODCreate(zffloat
                    , 0
                    , 90
                    ));

            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAnimationNativeView, aniRotateYFrom),
                ZFCoreArrayPODCreate(zffloat
                    , 0
                    , 90
                    ));
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAnimationNativeView, aniRotateYTo),
                ZFCoreArrayPODCreate(zffloat
                    , 0
                    , 90
                    ));

            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAnimationNativeView, aniRotateZFrom),
                ZFCoreArrayPODCreate(zffloat
                    , 0
                    , 90
                    ));
            ZFUIKit_test_prepareSettingForNormalProperty(settings, ani, zffloat, ZFPropertyAccess(ZFAnimationNativeView, aniRotateZTo),
                ZFCoreArrayPODCreate(zffloat
                    , 0
                    , 90
                    ));
        } // aniRotate

        ZFUIKit_test_prepareSettingButtonWithTestWindow(window, settings);
    }
};
ZFOBJECT_REGISTER(ZFUIKit_ZFAnimationNativeView_test)

ZF_NAMESPACE_GLOBAL_END

