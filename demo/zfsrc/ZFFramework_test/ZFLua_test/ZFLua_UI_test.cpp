#include "ZFLua_test.h"
#include "ZFUIKit.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFLua_UI_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFLua_UI_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();

        zfautoObject result = ZFLuaExecute(
                "local window = ZFUIWindow()\n"
                "window:windowShow()\n"
                "window:viewBackgroundColor(ZFUIColorRandom())\n"
                "local button = ZFUIButtonBasic()\n"
                "window:childAdd(button)\n"
                "button:layoutParam():layoutAlign(ZFUIAlign.e_TopInner() | ZFUIAlign.e_RightInner())\n"
                "button:viewBackgroundColor(ZFUIColorRandom())\n"
                "button:buttonLabelText('close')\n"
                "button:observerAdd(ZFUIButton.EventButtonOnClick(), function (listenerData, userData)\n"
                "        userData:objectHolded():windowHide()\n"
                "    end, window:objectHolder())\n"
                "return window\n"
            );
        zfCoreAssert(result != zfnull);

        ZFLISTENER_LOCAL(windowOnHide, {
            ZFLISTENER_LOCAL(testCaseStopDelay, {
                ZFLuaGC();
                ZFTestCase *testCase = userData->objectHolded();
                testCase->testCaseStop();
            })
            ZFThreadTaskRequest(testCaseStopDelay, userData);
        })
        result->observerAdd(ZFUIWindow::EventWindowOnHide(), windowOnHide, this->objectHolder());
    }
};
ZFOBJECT_REGISTER(ZFLua_UI_test)

ZF_NAMESPACE_GLOBAL_END

