#include "ZFUIWidget_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUIWidget_ZFUIAniImageView_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFUIWidget_ZFUIAniImageView_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView);
        ZFUIWindow *window = zfnull;
        ZFUIView *container = zfnull;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        zfblockedAlloc(ZFUIAniImageView, animatedImageView);
        container->childAdd(animatedImageView, ZFUISizeParamWrapWrap(), ZFUIAlign::e_Center);
        ZFLISTENER_LOCAL(testCaseOnStop, {
            ZFUIAniImageView *animatedImageView = userData->objectHolded();
            animatedImageView->aniStop();
        })
        this->observerAdd(ZFTestCase::EventTestCaseOnStop(), testCaseOnStop, animatedImageView->objectHolder());

        zfautoObjectT<ZFUIImage *> frameSrc = zfRes("test_normal.png");
        animatedImageView->aniLoad(frameSrc, ZFUISizeMake(
                frameSrc->imageSizeFixed().width / 2,
                frameSrc->imageSizeFixed().height / 2
            ),
            4,
            ZFCoreArrayPODCreate(zftimet, 100, 500, 1000));

        this->prepareActionLog(animatedImageView);
        this->prepareActionButton(container, animatedImageView);
        this->prepareSettingButton(window, animatedImageView);
    }

private:
    void prepareActionLog(ZF_IN ZFUIAniImageView *animatedImageView)
    {
        ZFLISTENER_LOCAL(onFrame, {
            userData->objectHolded<ZFTestCase *>()->testCaseOutput(
                "onFrame: %zi",
                listenerData.sender<ZFUIAniImageView *>()->aniFrame());
        })
        animatedImageView->observerAdd(ZFUIAniImageView::EventAniOnFrame(), onFrame, this->objectHolder());

        ZFLISTENER_LOCAL(onLoop, {
            userData->objectHolded<ZFTestCase *>()->testCaseOutput(
                "onLoop: %zi",
                listenerData.sender<ZFUIAniImageView *>()->aniFrame());
        })
        animatedImageView->observerAdd(ZFUIAniImageView::EventAniOnLoop(), onLoop, this->objectHolder());

        ZFLISTENER_LOCAL(onStop, {
            userData->objectHolded<ZFTestCase *>()->testCaseOutput(
                "onStop: %zi",
                listenerData.sender<ZFUIAniImageView *>()->aniFrame());
        })
        animatedImageView->observerAdd(ZFUIAniImageView::EventAniOnStop(), onStop, this->objectHolder());
    }
    void prepareActionButton(ZF_IN ZFUIView *container,
                             ZF_IN ZFUIAniImageView *animatedImageView)
    {
        ZFLISTENER_LOCAL(manualOnClick, {
            ZFUIAniImageView *animatedImageView = userData->objectHolded();
            animatedImageView->aniFrameNext();
        })
        zfblockedAlloc(ZFUIKit_test_Button, manualButton);
        container->childAdd(manualButton);
        manualButton->layoutParam()->layoutAlign(ZFUIAlign::e_LeftInner | ZFUIAlign::e_BottomInner);
        manualButton->buttonLabelText("manual");
        manualButton->observerAdd(ZFUIButton::EventButtonOnClick(), manualOnClick, animatedImageView->objectHolder());

        ZFLISTENER_LOCAL(startOnClick, {
            ZFUIAniImageView *animatedImageView = userData->objectHolded();
            animatedImageView->aniStart();
        })
        zfblockedAlloc(ZFUIKit_test_Button, startButton);
        container->childAdd(startButton);
        startButton->layoutParam()->layoutAlign(ZFUIAlign::e_Center | ZFUIAlign::e_BottomInner);
        startButton->buttonLabelText("start");
        startButton->observerAdd(ZFUIButton::EventButtonOnClick(), startOnClick, animatedImageView->objectHolder());

        ZFLISTENER_LOCAL(stopOnClick, {
            ZFUIAniImageView *animatedImageView = userData->objectHolded();
            animatedImageView->aniStop();
        })
        zfblockedAlloc(ZFUIKit_test_Button, stopButton);
        container->childAdd(stopButton);
        stopButton->layoutParam()->layoutAlign(ZFUIAlign::e_RightInner | ZFUIAlign::e_BottomInner);
        stopButton->buttonLabelText("stop");
        stopButton->observerAdd(ZFUIButton::EventButtonOnClick(), stopOnClick, animatedImageView->objectHolder());
    }
    void prepareSettingButton(ZF_IN ZFUIWindow *window,
                              ZF_IN ZFUIAniImageView *animatedImageView)
    {
        zfblockedAlloc(ZFArray, settings);

        ZFUIKit_test_prepareSettingButtonWithTestWindow(window, settings);
    }
};
ZFOBJECT_REGISTER(ZFUIWidget_ZFUIAniImageView_test)

ZF_NAMESPACE_GLOBAL_END

