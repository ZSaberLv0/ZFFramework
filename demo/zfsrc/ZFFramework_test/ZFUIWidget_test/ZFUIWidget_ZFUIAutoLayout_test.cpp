#include "ZFUIWidget_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUIWidget_ZFUIAutoLayout_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFUIWidget_ZFUIAutoLayout_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView);
        ZFUIWindow *window = zfnull;
        ZFUIView *container = zfnull;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        zfblockedAlloc(ZFUIAutoLayout, layout);
        container->childAdd(layout, ZFUISizeParamFillFill());
        layout->layoutParam()->layoutMargin(ZFUIMarginMake(40));
        layout->viewBackgroundColor(ZFUIColorRed());

        ZFCoreArrayPOD<ZFUIView *> v;
        for(zfindex i = 0; i < 5; ++i)
        {
            zfblockedAlloc(ZFUITextView, t);
            layout->childAdd(t);
            v.add(t);
            t->text(zfstringWithFormat("text %zi", i));
            t->viewBackgroundColor(ZFUIColorRandom());
        }

        zfal_maker(v[0]).left().toParentLeft().right().toLeft(v[1]);
        zfal_maker(v[1]).left().toRight(v[0]).right().toLeft(v[2]);
        zfal_maker(v[2]).left().toRight(v[1]).right().toParentRight();

        zfal_maker(v[3]).right().toLeft(v[1]).bottom().toTop(v[1]).height().toWidth(v[1]).scale(2);
        zfal_maker(v[4]).left().toRight(v[1]).top().toBottom(v[1]).height().toWidth(v[1]).scale(3);

        this->prepareSettingButton(window, layout);
    }

private:
    void prepareSettingButton(ZF_IN ZFUIWindow *window,
                              ZF_IN ZFUIAutoLayout *layout)
    {
        zfblockedAlloc(ZFArrayEditable, settings);

        ZFUIKit_test_prepareSettingForLayoutRequest(settings, layout);

        ZFUIKit_test_prepareSettingForNormalProperty(settings, layout->layoutParam(), ZFUISizeParam, ZFPropertyAccess(ZFUIViewLayoutParam, sizeParam),
            ZFCoreArrayPODCreate(ZFUISizeParam
                , ZFUISizeParamWrapWrap()
                , ZFUISizeParamFillWrap()
                , ZFUISizeParamWrapFill()
                , ZFUISizeParamFillFill()
                ));

        ZFUIKit_test_prepareSettingForNormalProperty(settings, layout->childAtIndex(0)->layoutParam<ZFUIAutoLayoutParam *>(), zffloat, ZFPropertyAccess(ZFUIAutoLayoutParam, biasX),
            ZFCoreArrayPODCreate(zffloat
                , 0.5f
                , 0.1f
                , 0.9f
                ));

        ZFUIKit_test_prepareSettingButtonWithTestWindow(window, settings);
    }
};
ZFOBJECT_REGISTER(ZFUIWidget_ZFUIAutoLayout_test)

ZF_NAMESPACE_GLOBAL_END

