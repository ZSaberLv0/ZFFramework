#include "ZFUIKit_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUIKit_ZFUIImageNinePatch_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFUIKit_ZFUIImageNinePatch_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView);
        ZFUIWindow *window = zfnull;
        ZFUIView *container = zfnull;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        zfblockedAlloc(ZFUIImageView, imageView);
        container->childAdd(imageView);
        imageView->layoutParam()->layoutAlign(ZFUIAlign::e_Center);
        imageView->image(zfRes("test_normal.png").to<ZFCopyable *>()->copy());
        imageView->image()->imageNinePatch(ZFUIMarginMake(
            imageView->image()->imageSize().width / 2,
            imageView->image()->imageSize().height / 2));

        this->prepareSettingButton(window, imageView);
    }

private:
    void prepareSettingButton(ZF_IN ZFUIWindow *window,
                              ZF_IN ZFUIImageView *imageView)
    {
        zfblockedAlloc(ZFArray, settings);

        { // wrap or fill
            zfblockedAlloc(ZFUIKit_test_SettingData, setting);
            settings->add(setting);
            setting->userData(zflineAlloc(ZFObject));
            setting->userData()->objectTag("imageView", imageView->objectHolder());
            ZFLISTENER_LOCAL(buttonTextGetter, {
                ZFUIImageView *imageView = userData->objectTag("imageView")->objectHolded();
                v_zfstring *text = listenerData.param0<v_zfstring *>();
                zfbool fill = (imageView->layoutParam()->sizeParam().width == ZFUISizeType::e_Fill);
                text->zfv = fill ? "fill" : "wrap";
            })
            setting->buttonTextGetter(buttonTextGetter);
            ZFLISTENER_LOCAL(buttonClickListener, {
                ZFUIImageView *imageView = userData->objectTag("imageView")->objectHolded();
                zfbool fill = (imageView->layoutParam()->sizeParam().width == ZFUISizeType::e_Fill);
                imageView->layoutParam()->sizeParam(fill ? ZFUISizeParamWrapWrap() : ZFUISizeParamFillFill());
            })
            setting->buttonClickListener(buttonClickListener);
        }

        ZFUIKit_test_prepareSettingButtonWithTestWindow(window, settings);
    }
};
ZFOBJECT_REGISTER(ZFUIKit_ZFUIImageNinePatch_test)

ZF_NAMESPACE_GLOBAL_END

