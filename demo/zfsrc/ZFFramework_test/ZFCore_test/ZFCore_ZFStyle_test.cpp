#include "ZFCore_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass _ZFP_ZFCore_ZFStyle_test_Object : zfextends ZFStyleableObject
{
    ZFOBJECT_DECLARE(_ZFP_ZFCore_ZFStyle_test_Object, ZFStyleableObject)

    ZFPROPERTY_ASSIGN(zfint, myProperty)
    ZFPROPERTY_ON_ATTACH_INLINE(zfint, myProperty)
    {
        zfLogT() << propertyValue;
    }
};

zfclass ZFCore_ZFStyle_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFCore_ZFStyle_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();

        this->testCaseOutputSeparator();
        this->testCaseOutput("prepare style");

        const zfchar *styleKey = _ZFP_ZFCore_ZFStyle_test_Object::ClassData()->classNameFull();
        {
            zfblockedAlloc(_ZFP_ZFCore_ZFStyle_test_Object, styleValue);
            styleValue->myProperty(123);
            ZFStyleSet(styleKey, styleValue);
        }

        this->testCaseOutputSeparator();
        this->testCaseOutput("attach object to style");
        zfblockedAlloc(_ZFP_ZFCore_ZFStyle_test_Object, obj);
        obj->styleKey(_ZFP_ZFCore_ZFStyle_test_Object::ClassData()->classNameFull());

        this->testCaseOutputSeparator();
        this->testCaseOutput("notify change style");
        ZFStyleChangeBegin();
        ZFStyleSet("ZFCore_ZFStyle_test", zflineAlloc(v_zfstring));
        ZFStyleSet("ZFCore_ZFStyle_test", zfnull);
        ZFStyleChangeEnd();

        this->testCaseStop();
    }
};
ZFOBJECT_REGISTER(ZFCore_ZFStyle_test)

ZF_NAMESPACE_GLOBAL_END

