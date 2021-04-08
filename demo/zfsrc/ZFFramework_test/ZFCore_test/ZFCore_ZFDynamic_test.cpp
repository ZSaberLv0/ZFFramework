#include "ZFCore_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclass ZFCore_ZFDynamic_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFCore_ZFDynamic_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();

        this->testCaseOutputSeparator();
        this->testCaseOutput("ZFDynamic");

        ZFLISTENER_LOCAL(methodCallback, {
                ZFMethodInvokeData *d = listenerData.param0()->toAny();
                zfblockedAlloc(v_zfstring, ret);
                ret->zfv += d->param0->to<v_zfstring *>()->zfv;
                ret->zfv += "(modified)";
                zfautoRelease(zfRetain(ret)); // take care of this
                d->ret = ret;
            })
        ZFDynamic d = ZFDynamic()
            .classBegin("ZFDynamicTest", "ZFObject")
                .property(ZFTypeId_zfstring(), "testProp")
                .method(methodCallback, zfnull, ZFTypeId_zfstring(), "testMethod", ZFTypeId_zfstring())
            .classEnd()
            .NSBegin("ZFDynamicTestNS")
                .method(methodCallback, zfnull, ZFTypeId_zfstring(), "testMethod", ZFTypeId_zfstring())
            .NSEnd()
            ;
        const ZFClass *cls = ZFClass::classForName("ZFDynamicTest");
        zfautoObject obj = cls->newInstance();

        obj->invoke("testProp", zflineAlloc(v_zfstring, "testValue"));
        zfLogT() << obj->invoke("testProp");
        zfLogT() << obj->invoke("testMethod", zflineAlloc(v_zfstring, "testParam"));

        const ZFMethod *method = ZFMethodForName("ZFDynamicTestNS", "testMethod");
        zfLogT() << method;
        zfLogT() << method->methodGenericInvoke(zfnull, zflineAlloc(v_zfstring, "testParam"));

        d.removeAll();
        this->testCaseStop();
    }
};
ZFOBJECT_REGISTER(ZFCore_ZFDynamic_test)

ZF_NAMESPACE_GLOBAL_END

