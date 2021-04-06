#include "ZFLua_test.h"
#include "ZFUIWidget.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFLua_app_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFLua_app_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();

        zfautoObject luaResult = ZFLuaExecute(ZFInputForResFile("test_ZFLua_app.lua"));
        ZFUIPageManager *pm = luaResult;
        zfCoreAssert(pm != zfnull);
        pm->observerAdd(
            ZFUIPageManager::EventManagerOnDestroy(),
            ZFCallbackForMemberMethod(this, ZFMethodAccess(zfself, managerOnDestroy)));
    }

private:
    ZFMETHOD_INLINE_2(void, managerOnDestroy,
                      ZFMP_IN(const ZFListenerData &, listenerData),
                      ZFMP_IN(ZFObject *, userData))
    {
        this->testCaseStop(ZFResultType::e_Success);
        ZFLuaGC();
    }
};
ZFOBJECT_REGISTER(ZFLua_app_test)

ZF_NAMESPACE_GLOBAL_END

