#include "ZFUIKit_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUIKit_ZFUIDraw_test_DrawableView : zfextends ZFUIDrawableView
{
    ZFOBJECT_DECLARE(ZFUIKit_ZFUIDraw_test_DrawableView, ZFUIDrawableView)

public:
    static void testDraw(ZF_IN void *context,
                         ZF_IN const ZFUISize &ownerSizePixel)
    {
        ZFUIDraw::antialiasing(context, zftrue);
        ZFUIDraw::drawColor(context,
            ZFUIColorBlue(),
            ZFUIRectMake(0, 0, ownerSizePixel.width / 2, ownerSizePixel.height / 2));
        ZFUIDraw::drawImage(context,
            zfRes("test_normal.png"),
            ZFUIRectZero(),
            ZFUIRectMake(ownerSizePixel.width / 2, ownerSizePixel.height / 2, ownerSizePixel.width / 2, ownerSizePixel.height / 2));
    }

protected:
    zfoverride
    virtual void onDraw(void)
    {
        zfsuper::onDraw();
        void *context = ZFUIDraw::beginForView(this);
        ZFUIKit_ZFUIDraw_test_DrawableView::testDraw(
            context,
            ZFUISizeApplyScale(ZFUIRectGetSize(this->viewFrame()), this->scaleFixed()));
        ZFUIDraw::endForView(context);
    }
};

zfclass ZFUIKit_ZFUIDraw_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFUIKit_ZFUIDraw_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView);
        ZFUIWindow *window = zfnull;
        ZFUIView *container = zfnull;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        container->viewBackgroundColor(ZFUIColorRandom(1));

        zfblockedAlloc(ZFUIKit_ZFUIDraw_test_DrawableView, view0);
        container->childAdd(view0, ZFUISizeParamFillFill(), ZFUIAlign::e_LeftInner);
        view0->layoutParam()->sizeHint(ZFUISizeMake(100, 100));

        zfblockedAlloc(ZFUIImageView, view1);
        container->childAdd(view1, ZFUISizeParamFillFill(), ZFUIAlign::e_RightInner);
        view1->layoutParam()->sizeHint(ZFUISizeMake(100, 100));
        void *context = ZFUIDraw::beginForImage(ZFUISizeApplyScale(ZFUISizeMake(100), view1->scaleFixed()));
        ZFUIKit_ZFUIDraw_test_DrawableView::testDraw(
            context,
            ZFUISizeApplyScale(ZFUISizeMake(100), view1->scaleFixed()));
        zfautoObjectT<ZFUIImage *> image = ZFUIDraw::endForImage(context);
        view1->image(image);

        zfblockedAlloc(ZFArray, views);
        views->add(view0);
        views->add(view1);
        this->prepareSettingButton(window, views);
    }

private:
    void prepareSettingButton(ZF_IN ZFUIWindow *window,
                              ZF_IN ZFArray *views)
    {
        zfblockedAlloc(ZFArray, settings);

        { // imageScaleType
            zfblockedAlloc(ZFUIKit_test_SettingData, setting);
            settings->add(setting);
            setting->userData(views);
            ZFLISTENER_LOCAL(buttonTextGetter, {
                v_zfstring *text = listenerData.param0<v_zfstring *>();
                text->zfv = "change layout";
            })
            setting->buttonTextGetter(buttonTextGetter);
            ZFLISTENER_LOCAL(buttonClickListener, {
                ZFArray *views = userData->to<ZFArray *>();
                ZFUISize sizeHint = views->get<ZFUIView *>(0)->layoutParam()->sizeHint();
                if(sizeHint.height == 100)
                {
                    sizeHint.height = 150;
                }
                else
                {
                    sizeHint.height = 100;
                }
                for(zfindex i = 0; i < views->count(); ++i)
                {
                    views->get<ZFUIView *>(i)->layoutParam()->sizeHint(sizeHint);
                }
            })
            setting->buttonClickListener(buttonClickListener);
        }

        ZFUIKit_test_prepareSettingButtonWithTestWindow(window, settings);
    }
};
ZFOBJECT_REGISTER(ZFUIKit_ZFUIDraw_test)

ZF_NAMESPACE_GLOBAL_END

