#include "ZFUIKit_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUIKit_scale_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFUIKit_scale_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView);
        ZFUIWindow *window = zfnull;
        ZFUIView *container = zfnull;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        zfblockedAlloc(ZFUITextView, view);
        container->childAdd(view);
        view->text("test text");
        view->layoutParam()->layoutAlign(ZFUIAlign::e_Center);

        this->prepareSettingButton(window);
    }

private:
    void prepareSettingButton(ZF_IN ZFUIWindow *window)
    {
        zfblockedAlloc(ZFArray, settings);

        { // UIScale
            zfblockedAlloc(ZFUIKit_test_SettingData, setting);
            settings->add(setting);
            ZFLISTENER_LOCAL(buttonTextGetter, {
                v_zfstring *text = listenerData.param0<v_zfstring *>();
                text->zfv = zfstringWithFormat(
                    "scale: %f",
                    ZFUISysWindow::mainWindow()->rootView()->UIScale());
            })
            setting->buttonTextGetter(buttonTextGetter);
            ZFLISTENER_LOCAL(buttonClickListener, {
                ZFUIRootView *rootView = ZFUISysWindow::mainWindow()->rootView();
                if(rootView->UIScale() == 1)
                {
                    rootView->UIScale(2);
                }
                else
                {
                    rootView->UIScale(1);
                }
            })
            setting->buttonClickListener(buttonClickListener);
        }

        ZFUIKit_test_prepareSettingButtonWithTestWindow(window, settings);
    }
};
ZFOBJECT_REGISTER(ZFUIKit_scale_test)

ZF_NAMESPACE_GLOBAL_END

