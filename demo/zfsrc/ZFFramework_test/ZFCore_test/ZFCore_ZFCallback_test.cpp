#include "ZFCore_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass _ZFP_ZFCore_ZFCallback_test_Class : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_ZFCore_ZFCallback_test_Class, ZFObject)

    ZFMETHOD_INLINE_0(void, classMember)
    {
        zfLogT();
    }

    ZFMETHOD_INLINE_STATIC_0(void, classStaticMember)
    {
        zfLogT();
    }
};

static void _ZFP_ZFCore_ZFCallback_test_StaticFunction(void)
{
    zfLogT();
}

zfclass ZFCore_ZFCallback_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFCore_ZFCallback_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();

        this->testCaseOutput("try execute %s", this->callbackClassMember.objectInfo().cString());
        this->callbackClassMember.executeExact<void>();

        this->testCaseOutputSeparator();
        this->testCaseOutput("try execute %s", this->callbackClassStaticMember.objectInfo().cString());
        this->callbackClassStaticMember.executeExact<void>();

        this->testCaseOutputSeparator();
        this->testCaseOutput("try execute %s", this->callbackFunction.objectInfo().cString());
        this->callbackFunction.executeExact<void>();

        ZFCALLBACK_LOCAL_BEGIN_1(void, localCallback, const zfstring &, param)
        {
            zfLogT() << param;
        }
        ZFCALLBACK_LOCAL_END(localCallback)
        this->testCaseOutputSeparator();
        this->testCaseOutput("try execute %s", localCallback.objectInfo().cString());
        localCallback.executeExact<void, const zfstring &>("param");

        this->testCaseStop();
    }

private:
    _ZFP_ZFCore_ZFCallback_test_Class *ownerClass;
    ZFCallback callbackClassMember;
    ZFCallback callbackClassStaticMember;
    ZFCallback callbackFunction;

protected:
    zfoverride
    virtual void objectOnInit(void)
    {
        zfsuper::objectOnInit();
        this->ownerClass = zfAlloc(_ZFP_ZFCore_ZFCallback_test_Class);

        this->callbackClassMember = ZFCallbackForMemberMethod(
            this->ownerClass,
            this->ownerClass->classData()->methodForName("classMember"));

        this->callbackClassStaticMember = ZFCallbackForMethod(
            _ZFP_ZFCore_ZFCallback_test_Class::ClassData()->methodForName("classStaticMember"));

        this->callbackFunction = ZFCallbackForFunc(_ZFP_ZFCore_ZFCallback_test_StaticFunction);
    }
    zfoverride
    virtual void objectOnDealloc(void)
    {
        zfRelease(this->ownerClass);
        this->ownerClass = zfnull;
        zfsuper::objectOnDealloc();
    }
};
ZFOBJECT_REGISTER(ZFCore_ZFCallback_test)

ZF_NAMESPACE_GLOBAL_END

