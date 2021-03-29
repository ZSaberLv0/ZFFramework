#include "ZFUIKit_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFUIKit_test_Window)
ZFOBJECT_REGISTER(ZFUIKit_test_Button)
ZFOBJECT_REGISTER(ZFUIKit_test_ListView)

void ZFUIKit_test_prepareTestWindow(ZF_OUT ZFUIWindow *&window,
                                    ZF_OUT ZFUIView *&container,
                                    ZF_IN ZFTestCase *testCaseToStop)
{
    window = zfAlloc(ZFUIKit_test_Window);
    zfblockedRelease(window);
    window->windowShow();

    // close button
    zfblockedAlloc(ZFUIKit_test_Button, closeButton);
    window->childAdd(closeButton);
    closeButton->layoutParam()->layoutAlign(ZFUIAlign::e_TopInner | ZFUIAlign::e_RightInner);
    closeButton->buttonLabelText("close");
    ZFLISTENER_LOCAL(onClickCloseButton, {
        ZFUIWindow *window = userData->objectTag("window")->objectHolded();
        ZFTestCase *testCase = userData->objectTag("testCaseToStop")->objectHolded();
        window->windowHide();
        testCase->testCaseStop();
    })
    zfblockedAlloc(ZFObject, closeButtonUserData);
    closeButtonUserData->objectTag("window", window->objectHolder());
    closeButtonUserData->objectTag("testCaseToStop", testCaseToStop->objectHolder());
    closeButton->observerAdd(ZFUIButton::EventButtonOnClick(), onClickCloseButton, closeButtonUserData);

    // container
    container = zfAlloc(ZFUIView);
    zfblockedRelease(container);
    window->childAdd(container);
    container->layoutParam()->sizeParam(ZFUISizeParamFillFill());
    container->layoutParam()->layoutMargin(ZFUIMarginMake(0, 50, 0, 0));
}

zfautoObject ZFUIKit_test_prepareSettingButton(ZF_IN ZFArray *settings)
{
    zfblockedAlloc(ZFUIKit_test_Button, settingsButton);
    settingsButton->buttonLabelText("settings");
    settingsButton->objectTag("settingsHolder", settings);

    zfblockedAlloc(ZFUIKit_test_Window, window);
    settingsButton->objectTag("setting window", window);
    window->viewAlpha(0.8f);
    ZFLISTENER_LOCAL(onClickSetting, {
        userData->objectHolded<ZFUIWindow *>()->windowShow();
    })
    settingsButton->observerAdd(ZFUIButton::EventButtonOnClick(), onClickSetting, window->objectHolder());

    zfblockedAlloc(ZFUIKit_test_Button, closeButton);
    window->childAdd(closeButton);
    closeButton->layoutParam()->layoutAlign(ZFUIAlign::e_TopInner);
    closeButton->buttonLabelStyle()->text("done");
    ZFLISTENER_LOCAL(onClickCloseButton, {
        userData->objectHolded<ZFUIWindow *>()->windowHide();
    })
    closeButton->observerAdd(ZFUIButton::EventButtonOnClick(), onClickCloseButton, window->objectHolder());

    zfblockedAlloc(ZFUIKit_test_ListView, listView);
    window->childAdd(listView);
    listView->layoutParam()->sizeParam(ZFUISizeParamFillFill());
    listView->layoutParam()->layoutMargin(ZFUIMarginMake(0, 50, 0, 0));
    for(zfindex i = 0; i < settings->count(); ++i)
    {
        ZFUIKit_test_SettingData *setting = settings->get<ZFUIKit_test_SettingData *>(i);
        zfCoreAssert(setting->buttonTextGetter().callbackIsValid());
        zfCoreAssert(setting->buttonClickListener().callbackIsValid());

        zfblockedAlloc(ZFUIKit_test_Button, button);
        listView->childAdd(button);
        ZFLISTENER_LOCAL(onButtonClick, {
            ZFUIKit_test_SettingData *setting = ZFAny(userData);
            ZFUIButtonBasic *button = ZFAny(listenerData.sender());

            setting->buttonClickListener().execute(ZFListenerData().sender(button), setting->userData());
            setting->settingUpdate();
        })
        button->observerAdd(ZFUIButton::EventButtonOnClick(), onButtonClick, setting);

        zfblockedAlloc(ZFObject, settingChangeUserData);
        settingChangeUserData->objectTag("setting", setting);
        settingChangeUserData->objectTag("button", button);
        ZFLISTENER_LOCAL(settingOnChange, {
            ZFUIKit_test_SettingData *setting = userData->objectTag<ZFUIKit_test_SettingData *>("setting");
            ZFUIButtonBasic *button = userData->objectTag<ZFUIButtonBasic *>("button");

            zfblockedAlloc(v_zfstring, buttonText);
            setting->buttonTextGetter().execute(
                ZFListenerData().sender(button).param0(buttonText),
                setting->userData());
            button->buttonLabelText(buttonText->zfv);
        })
        setting->observerAdd(ZFUIKit_test_SettingData::EventSettingOnChange(), settingOnChange, settingChangeUserData);

        zfblockedAlloc(v_zfstring, buttonText);
        setting->buttonTextGetter().execute(
            ZFListenerData().sender(button).param0(buttonText),
            setting->userData());
        button->buttonLabelText(buttonText->zfv);
    }

    return settingsButton;
}

void ZFUIKit_test_prepareSettingButtonWithTestWindow(ZF_IN ZFUIWindow *window,
                                                     ZF_IN ZFArray *settings)
{
    zfautoObject buttonHolder = ZFUIKit_test_prepareSettingButton(settings);
    ZFUIButton *button = buttonHolder;
    window->childAdd(button);
    button->layoutParam()->layoutAlign(ZFUIAlign::e_TopInner);
}

void ZFUIKit_test_prepareSettingForProperty(ZF_IN_OUT ZFArray *settings,
                                            ZF_IN ZFObject *obj,
                                            ZF_IN const ZFProperty *property,
                                            ZF_IN const ZFListener &nextCallback,
                                            ZF_IN ZFObject *userData)
{
    zfCoreAssert(settings != zfnull);
    zfCoreAssert(nextCallback.callbackIsValid());

    zfblockedAlloc(ZFObject, holder);
    holder->objectTag("obj", obj->objectHolder());
    holder->objectTag("property", zflineAlloc(v_ZFProperty, property));
    holder->objectTag("nextCallback", zflineAlloc(ZFTypeHolder, zfnew(ZFListener, nextCallback), ZFTypeHolderTypeObject<ZFListener *>));
    holder->objectTag("userData", userData);

    ZFLISTENER_LOCAL(buttonTextGetter, {
        ZFObject *obj = userData->objectTag("obj")->objectHolded();
        const ZFProperty *property = userData->objectTag<v_ZFProperty *>("property")->zfv;

        v_zfstring *text = listenerData.param0<v_zfstring *>();
        text->zfv = zfstringWithFormat("%s : %s", property->propertyName(), ZFPropertyGetInfo(property, obj).cString());
    })
    ZFLISTENER_LOCAL(buttonClickListener, {
        const ZFListener &nextCallback = userData->objectTag<ZFTypeHolder *>("nextCallback")->holdedDataRef<const ZFListener &>();
        nextCallback.execute(ZFListenerData(), userData->objectTag("userData"));
    })

    settings->add(zflineAlloc(ZFUIKit_test_SettingData, buttonTextGetter, buttonClickListener, holder));
}

void ZFUIKit_test_prepareSettingForBoolProperty(ZF_IN_OUT ZFArray *settings,
                                                ZF_IN ZFObject *obj,
                                                ZF_IN const ZFProperty *property)
{
    zfCoreAssert(settings != zfnull);
    zfCoreAssert(obj != zfnull && property != zfnull);
    zfCoreAssert(obj->classData()->classIsTypeOf(property->propertyOwnerClass()));
    zfCoreAssert(zfscmpTheSame(property->propertyTypeId(), ZFTypeId_zfbool()));

    zfbool value = property->getterMethod()->execute<zfbool const &>(obj);
    ZFUIKit_test_prepareSettingForNormalProperty(settings, obj, zfbool, property, ZFCoreArrayPODCreate(zfbool, value, !value));
}

void ZFUIKit_test_prepareSettingForLayoutRequest(ZF_IN_OUT ZFArray *settings,
                                                 ZF_IN ZFUIView *view)
{
    zfCoreAssert(settings != zfnull);
    zfCoreAssert(view != zfnull);

    ZFLISTENER_LOCAL(buttonTextGetter, {
        v_zfstring *text = listenerData.param0<v_zfstring *>();
        text->zfv = "layoutRequest";
    })
    ZFLISTENER_LOCAL(buttonClickListener, {
        userData->objectHolded<ZFUIView *>()->layoutRequest();
    })

    settings->add(zflineAlloc(ZFUIKit_test_SettingData, buttonTextGetter, buttonClickListener, view->objectHolder()));
}

void ZFUIKit_test_prepareSettingForResetProperty(ZF_IN_OUT ZFArray *settings,
                                                 ZF_IN ZFObject *obj,
                                                 ZF_IN const ZFCoreArrayPOD<const ZFProperty *> &propertyList)
{
    zfblockedAlloc(ZFUIKit_test_SettingData, setting);
    settings->add(setting);
    setting->userData(zflineAlloc(ZFObject));
    setting->userData()->objectTag("obj", obj->objectHolder());
    setting->userData()->objectTag("settings", settings->objectHolder());
    zfblockedAlloc(ZFTypeHolder, propertyListHolder,
        zfnew(ZFCoreArrayPOD<const ZFProperty *>, propertyList),
        ZFTypeHolderTypeObject<ZFCoreArrayPOD<const ZFProperty *> *>);
    setting->userData()->objectTag("propertyList", propertyListHolder);

    ZFLISTENER_LOCAL(buttonTextGetter, {
        v_zfstring *text = listenerData.param0<v_zfstring *>();
        text->zfv = "reset setting";
    })
    setting->buttonTextGetter(buttonTextGetter);

    ZFLISTENER_LOCAL(buttonClickListener, {
        ZFObject *obj = userData->objectTag("obj")->objectHolded();
        ZFArray *settings = userData->objectTag("settings")->objectHolded();
        const ZFCoreArrayPOD<const ZFProperty *> &toReset = userData->objectTag<ZFTypeHolder *>("propertyList")->holdedDataRef<const ZFCoreArrayPOD<const ZFProperty *> &>();

        if(obj == zfnull || settings == zfnull || toReset.isEmpty())
        {
            return ;
        }

        zfautoObject fromObj = obj->classData()->newInstance();
        for(zfindex i = 0; i < toReset.count(); ++i)
        {
            ZFPropertyCopy(toReset[i], obj, fromObj);
        }
        for(zfindex i = 0; i < settings->count(); ++i)
        {
            settings->get<ZFUIKit_test_SettingData *>(i)->settingUpdate();
        }
    })
    setting->buttonClickListener(buttonClickListener);
}

ZF_NAMESPACE_GLOBAL_END

