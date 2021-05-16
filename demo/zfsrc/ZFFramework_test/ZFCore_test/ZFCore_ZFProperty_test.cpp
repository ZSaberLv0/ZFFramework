#include "ZFCore_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass _ZFP_ZFCore_ZFProperty_test_TestBase : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_ZFCore_ZFProperty_test_TestBase, ZFObject)

public:
    // retain
    ZFPROPERTY_RETAIN(ZFObject *, propertyRetain)
    ZFPROPERTY_RETAIN_READONLY(ZFObject *, propertyRetainReadonly, ZFPropertyNoInitValue)

    // assign
    ZFPROPERTY_ASSIGN(zfstring, propertyAssign)
    ZFPROPERTY_ASSIGN_READONLY(zfstring, propertyAssignReadonly, ZFPropertyNoInitValue)

    // weak
    ZFPROPERTY_ASSIGN(ZFObject *, propertyWeak)

    // custom callback
    ZFPROPERTY_ON_INIT_INLINE(ZFObject *, propertyRetain)
    {
        zfLogT();
    }
    ZFPROPERTY_ON_VERIFY_INLINE(ZFObject *, propertyRetain)
    {
        zfLogT();
    }
    ZFPROPERTY_ON_ATTACH_INLINE(ZFObject *, propertyRetain)
    {
        zfLogT();
    }
    ZFPROPERTY_ON_DETACH_INLINE(ZFObject *, propertyRetain)
    {
        zfLogT();
    }

    ZFPROPERTY_ON_INIT_DECLARE(zfstring, propertyAssign)
    ZFPROPERTY_ON_VERIFY_DECLARE(zfstring, propertyAssign)
    ZFPROPERTY_ON_ATTACH_DECLARE(zfstring, propertyAssign)
    ZFPROPERTY_ON_DETACH_DECLARE(zfstring, propertyAssign)
};
ZFPROPERTY_ON_INIT_DEFINE(_ZFP_ZFCore_ZFProperty_test_TestBase, zfstring, propertyAssign)
{
    zfLogT();
}
ZFPROPERTY_ON_VERIFY_DEFINE(_ZFP_ZFCore_ZFProperty_test_TestBase, zfstring, propertyAssign)
{
    zfLogT();
}
ZFPROPERTY_ON_ATTACH_DEFINE(_ZFP_ZFCore_ZFProperty_test_TestBase, zfstring, propertyAssign)
{
    zfLogT();
}
ZFPROPERTY_ON_DETACH_DEFINE(_ZFP_ZFCore_ZFProperty_test_TestBase, zfstring, propertyAssign)
{
    zfLogT();
}

zfclass _ZFP_ZFCore_ZFProperty_test_TestChild: zfextends _ZFP_ZFCore_ZFProperty_test_TestBase
{
    ZFOBJECT_DECLARE(_ZFP_ZFCore_ZFProperty_test_TestChild, _ZFP_ZFCore_ZFProperty_test_TestBase)

public:
    ZFPROPERTY_ASSIGN(zfstring, propertyInChild)
};

// ============================================================
zfclass ZFCore_ZFProperty_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFCore_ZFProperty_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();

        this->testCaseOutputSeparator();
        this->testCaseOutput("ZFProperty");
        {
            zfblockedAlloc(_ZFP_ZFCore_ZFProperty_test_TestChild, p);

            // property list
            const ZFClass *clsTmp = p->classData();
            zfLogT() << ZFLogAutoEndlOff << "list: ";
            for(zfindex i = 0; i < clsTmp->propertyCount(); ++i)
            {
                if(i > 0)
                {
                    zfLogTrimT() << ZFLogAutoEndlOff << ", ";
                }
                zfLogTrimT() << ZFLogAutoSpaceOff << ZFLogAutoEndlOff
                << clsTmp->propertyAtIndex(i)->propertyName();
            }
            zfLogTrimT();

            // reflect
            p->propertyAssign("oldValue");
            zfLogT() << "before:" << p->propertyAssign();
            p->classData()->propertySetterForName("propertyAssign")->execute<void, zfstring const &>(p, "newValue");
            zfLogT() << "after:" << p->propertyAssign();
            zfLogT() << "access by reflect:" << p->classData()->propertyGetterForName("propertyAssign")->execute<zfstring const &>(p);

            // retain
            zfLogTrimT();
            zfLogTrimT() << "retain";

            p->propertyRetain(zflineAlloc(ZFObject));
            zfLogT() << p->propertyRetain();

#if 0 // this should not compile
            p->propertyRetainReadonly(zflineAlloc(ZFObject));
#endif
            zfLogT() << p->propertyRetainReadonly();

            // assign
            zfLogTrimT();
            zfLogTrimT() << "assign";

            p->propertyAssign(zfstring());
            zfLogT() << p->propertyAssign();

#if 0 // this should not compile
            p->propertyAssignReadonly(zfstring());
#endif
            zfLogT() << p->propertyAssignReadonly();

            // weak
            {
                zfblockedAlloc(ZFObject, value);
                p->propertyWeak(value);
                zfLogT() << p->propertyWeak();
            }
            zfLogT() << p->propertyWeak();

            // copy
            this->testCaseOutputSeparator();
            this->testCaseOutput("copy propertis");
            zfblockedAlloc(_ZFP_ZFCore_ZFProperty_test_TestBase, pBase);
            zfblockedAlloc(_ZFP_ZFCore_ZFProperty_test_TestChild, pChild);
            pBase->propertyAssign("string set in another");
            ZFPropertyCopyAll(pChild, pBase);
            this->testCaseOutputSeparator();
            zfLogTrimT() << "after copy:" << pChild->propertyAssign();

            this->testCaseOutputSeparator();
            this->testCaseOutput("copy by ZFPropertyCopyAll");
            pChild->propertyAssign("");
            ZFPropertyCopyAll(pChild, pBase);
            zfLogTrimT() << "after copy:" << pChild->propertyAssign();
        }

        this->testCaseStop();
    }
};
ZFOBJECT_REGISTER(ZFCore_ZFProperty_test)

ZF_NAMESPACE_GLOBAL_END

