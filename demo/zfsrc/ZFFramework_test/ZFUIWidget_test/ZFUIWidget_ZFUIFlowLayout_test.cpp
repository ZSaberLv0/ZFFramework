#include "ZFUIWidget_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUIWidget_ZFUIFlowLayout_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFUIWidget_ZFUIFlowLayout_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView);
        ZFUIWindow *window = zfnull;
        ZFUIView *container = zfnull;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        zfblockedAlloc(ZFUIFlowLayout, layout);
        container->childAdd(layout);
        layout->layoutParam()->layoutMargin(ZFUIMarginMake(40, 40 + ZFUIGlobalStyle::DefaultStyle()->itemSizeButton(), 40, 40));
        layout->viewBackgroundColor(ZFUIColorRed());

        this->prepareAddRemoveButton(container, layout);

        this->prepareSettingButton(window, layout);
    }

private:
    void prepareAddRemoveButton(ZFUIView *container, ZFUIView *layout)
    {
        zfblockedAlloc(ZFUIKit_test_Button, addButton);
        container->childAdd(addButton);
        ZFLISTENER_LOCAL(addButtonOnClick, {
            ZFUIView *layout = userData->objectHolded();
            zfblockedAlloc(ZFUITextView, view);
            layout->childAdd(view);
            view->viewBackgroundColor(ZFUIColorRandom());
            zfindex textLength = zfmRand(1, 10);
            textLength = layout->childCount() + 1;
            zfstring text;
            text.capacity(textLength);
            zfchar c = '0' + ((layout->childCount() - 1) % 10);
            for(zfindex i = 0; i < textLength; ++i)
            {
                text += c;
            }
            view->text(text);
        })
        addButton->observerAdd(ZFUIButton::EventButtonOnClick(), addButtonOnClick, layout->objectHolder());
        addButton->buttonLabelText("add");

        zfblockedAlloc(ZFUIKit_test_Button, removeButton);
        container->childAdd(removeButton);
        removeButton->layoutParam()->layoutMargin(ZFUIMarginMake(70, 0, 0, 0));
        ZFLISTENER_LOCAL(removeButtonOnClick, {
            ZFUIView *layout = userData->objectHolded();
            if(layout->childCount() > 0)
            {
                layout->childRemoveAtIndex(layout->childCount() - 1);
            }
        })
        removeButton->observerAdd(ZFUIButton::EventButtonOnClick(), removeButtonOnClick, layout->objectHolder());
        removeButton->buttonLabelText("remove");
    }
    void prepareSettingButton(ZF_IN ZFUIWindow *window,
                              ZF_IN ZFUIFlowLayout *layout)
    {
        zfblockedAlloc(ZFArray, settings);

        ZFUIKit_test_prepareSettingForLayoutRequest(settings, layout);

        ZFUIKit_test_prepareSettingForNormalProperty(settings, layout, ZFUIOrientationEnum, ZFPropertyAccess(ZFUIFlowLayout, layoutOrientationMain),
            ZFCoreArrayPODCreate(ZFUIOrientationEnum
                , ZFUIOrientation::e_Right
                , ZFUIOrientation::e_Bottom
                , ZFUIOrientation::e_Left
                , ZFUIOrientation::e_Top
                ));

        ZFUIKit_test_prepareSettingForNormalProperty(settings, layout, ZFUIOrientationEnum, ZFPropertyAccess(ZFUIFlowLayout, layoutOrientationSecondary),
            ZFCoreArrayPODCreate(ZFUIOrientationEnum
                , ZFUIOrientation::e_Right
                , ZFUIOrientation::e_Bottom
                , ZFUIOrientation::e_Left
                , ZFUIOrientation::e_Top
                ));

        ZFUIKit_test_prepareSettingForNormalProperty(settings, layout, ZFUIMargin, ZFPropertyAccess(ZFUIFlowLayout, layoutChildMargin),
            ZFCoreArrayPODCreate(ZFUIMargin
                , ZFUIMarginZero()
                , ZFUIMarginMake(8)
                , ZFUIMarginMake(12)
                , ZFUIMarginMake(24)
                ));

        ZFUIKit_test_prepareSettingForNormalProperty(settings, layout, zffloat, ZFPropertyAccess(ZFUIFlowLayout, layoutChildSpaceX),
            ZFCoreArrayPODCreate(zffloat
                , 0
                , 8
                , 12
                , 24
                ));

        ZFUIKit_test_prepareSettingForNormalProperty(settings, layout, zffloat, ZFPropertyAccess(ZFUIFlowLayout, layoutChildSpaceY),
            ZFCoreArrayPODCreate(zffloat
                , 0
                , 8
                , 12
                , 24
                ));

        ZFUIKit_test_prepareSettingForNormalProperty(settings, layout->layoutParam(), ZFUISizeParam, ZFPropertyAccess(ZFUILayoutParam, sizeParam),
            ZFCoreArrayPODCreate(ZFUISizeParam
                , ZFUISizeParamWrapWrap()
                , ZFUISizeParamFillWrap()
                , ZFUISizeParamWrapFill()
                , ZFUISizeParamFillFill()
                ));

        ZFUIKit_test_prepareSettingButtonWithTestWindow(window, settings);
    }
};
ZFOBJECT_REGISTER(ZFUIWidget_ZFUIFlowLayout_test)

ZF_NAMESPACE_GLOBAL_END

