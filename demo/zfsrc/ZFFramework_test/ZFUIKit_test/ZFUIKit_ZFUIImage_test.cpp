#include "ZFUIKit_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUIKit_ZFUIImage_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFUIKit_ZFUIImage_test, ZFFramework_test_TestCase)

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
        container->childAdd(view);
        view->layoutParam()->sizeParam(ZFUISizeParamFillFill());

        // encode image to binary and load it again to test
        zfautoObject imageHolder = zfRes("test_normal.png");
        ZFIOBufferedCallbackUsingBuffer io;
        ZFUIImageEncodeToFile(io, imageHolder);
        zfautoObject imageNew = ZFUIImageEncodeFromFile(io);
        view->image(imageNew);

        this->prepareSettingButton(window, view);
    }

private:
    void prepareSettingButton(ZF_IN ZFUIWindow *window,
                              ZF_IN ZFUIImageView *view)
    {
        zfblockedAlloc(ZFArrayEditable, settings);

        ZFUIKit_test_prepareSettingButtonWithTestWindow(window, settings);
    }
};
ZFOBJECT_REGISTER(ZFUIKit_ZFUIImage_test)

ZF_NAMESPACE_GLOBAL_END

