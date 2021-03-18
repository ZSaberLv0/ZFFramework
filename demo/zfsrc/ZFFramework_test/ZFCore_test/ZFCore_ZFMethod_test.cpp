#include "ZFCore_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclass _ZFP_ZFCore_ZFMethod_test_TestBase : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_ZFCore_ZFMethod_test_TestBase, ZFObject)
public:
    zfstring s;

public:
    ZFMETHOD_INLINE_0(void, methodNormal)
    {
        zfLogT();
    }
    ZFMETHOD_DECLARE_0(void, methodDefinedInCppFile)
    ZFMETHOD_INLINE_0(void, methodOverrided)
    {
        zfLogT();
    }
    ZFMETHOD_INLINE_DETAIL_0(protected, ZFMethodTypeVirtual, ZF_CALLER_LINE,
                             void, methodProtected)
    {
        zfLogT();
    }
    ZFMETHOD_INLINE_DETAIL_0(private, ZFMethodTypeVirtual, ZF_CALLER_LINE,
                             void, methodPrivate)
    {
        zfLogT();
    }
    ZFMETHOD_INLINE_DETAIL_0(public, ZFMethodTypeStatic, ZF_CALLER_LINE,
                             void, methodStatic)
    {
        zfLogT();
    }
    ZFMETHOD_INLINE_1(void, methodOverloaded,
                      ZFMP_IN(zfint, param0))
    {
        zfLogT() << param0;
    }
    ZFMETHOD_INLINE_1(void, methodOverloaded,
                      ZFMP_IN(zffloat, param0))
    {
        zfLogT() << param0;
    }
    ZFMETHOD_INLINE_1(void, methodForDefaultParam,
                      ZFMP_IN_OPT(const zfstring &, param0, "DefaultParam"))
    {
        zfLogT() << param0;
    }
    ZFMETHOD_INLINE_0(zfstring &, methodReturnReference)
    {
        zfLogT();
        return this->s;
    }
    ZFMETHOD_INLINE_1(void, methodParamIsReference,
                      ZFMP_IN(zfstring &, param0))
    {
        zfLogT();
        param0 = "newString";
    }
    ZFMETHOD_INLINE_2(void, methodFor2Param,
                      ZFMP_IN(const zfchar *, param0),
                      ZFMP_IN(const zfchar *, param1))
    {
        zfLogT() << param0 << param1;
    }
    ZFMETHOD_INLINE_3(void, methodFor3Param,
                      ZFMP_IN(const zfchar *, param0),
                      ZFMP_IN(const zfchar *, param1),
                      ZFMP_IN(const zfchar *, param2))
    {
        zfLogT() << param0 << param1 << param2;
    }
    ZFMETHOD_INLINE_4(void, methodFor4Param,
                      ZFMP_IN(const zfchar *, param0),
                      ZFMP_IN(const zfchar *, param1),
                      ZFMP_IN(const zfchar *, param2),
                      ZFMP_IN(const zfchar *, param3))
    {
        zfLogT() << param0 << param1 << param2 << param3;
    }
    ZFMETHOD_INLINE_5(void, methodFor5Param,
                      ZFMP_IN(const zfchar *, param0),
                      ZFMP_IN(const zfchar *, param1),
                      ZFMP_IN(const zfchar *, param2),
                      ZFMP_IN(const zfchar *, param3),
                      ZFMP_IN(const zfchar *, param4))
    {
        zfLogT() << param0 << param1 << param2 << param3 << param4;
    }
    ZFMETHOD_INLINE_6(void, methodFor6Param,
                      ZFMP_IN(const zfchar *, param0),
                      ZFMP_IN(const zfchar *, param1),
                      ZFMP_IN(const zfchar *, param2),
                      ZFMP_IN(const zfchar *, param3),
                      ZFMP_IN(const zfchar *, param4),
                      ZFMP_IN(const zfchar *, param5))
    {
        zfLogT() << param0 << param1 << param2 << param3 << param4 << param5;
    }
    ZFMETHOD_INLINE_7(void, methodFor7Param,
                      ZFMP_IN(const zfchar *, param0),
                      ZFMP_IN(const zfchar *, param1),
                      ZFMP_IN(const zfchar *, param2),
                      ZFMP_IN(const zfchar *, param3),
                      ZFMP_IN(const zfchar *, param4),
                      ZFMP_IN(const zfchar *, param5),
                      ZFMP_IN(const zfchar *, param6))
    {
        zfLogT() << param0 << param1 << param2 << param3 << param4 << param5 << param6;
    }
    ZFMETHOD_INLINE_8(void, methodFor8Param,
                      ZFMP_IN(const zfchar *, param0),
                      ZFMP_IN(const zfchar *, param1),
                      ZFMP_IN(const zfchar *, param2),
                      ZFMP_IN(const zfchar *, param3),
                      ZFMP_IN(const zfchar *, param4),
                      ZFMP_IN(const zfchar *, param5),
                      ZFMP_IN(const zfchar *, param6),
                      ZFMP_IN(const zfchar *, param7))
    {
        zfLogT() << param0 << param1 << param2 << param3 << param4 << param5 << param6 << param7;
    }
};
#if 0 // if opened, all method should be registered statically
    ZFOBJECT_REGISTER(_ZFP_ZFCore_ZFMethod_test_TestBase)
#endif
ZFMETHOD_DEFINE_0(_ZFP_ZFCore_ZFMethod_test_TestBase, void, methodDefinedInCppFile)
{
    zfLogT();
}

// ============================================================
zfclass _ZFP_ZFCore_ZFMethod_test_TestChild : zfextends _ZFP_ZFCore_ZFMethod_test_TestBase
{
    ZFOBJECT_DECLARE(_ZFP_ZFCore_ZFMethod_test_TestChild, _ZFP_ZFCore_ZFMethod_test_TestBase)

public:
    ZFMETHOD_INLINE_0(void, methodOverrided)
    {
        zfsuper::methodOverrided();
        zfLogT();
    }

    ZFMETHOD_INLINE_0(void, methodInChild)
    {
        zfLogT();
    }
};
#if 0 // if opened, all method should be registered statically
    ZFOBJECT_REGISTER(_ZFP_ZFCore_ZFMethod_test_TestChild)
#endif

// ============================================================
zfclass ZFCore_ZFMethod_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFCore_ZFMethod_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();

        const ZFClass *clsBase = _ZFP_ZFCore_ZFMethod_test_TestBase::ClassData();
        const ZFClass *clsChild = _ZFP_ZFCore_ZFMethod_test_TestChild::ClassData();

        // ============================================================
        this->testCaseOutputSeparator();
        this->testCaseOutput("ZFMethod registration");

        this->testCaseOutputSeparator();
        this->testCaseOutput("method registered before any of instances created:");

        this->testCaseOutputSeparator();
        this->testCaseOutput("Base:");
        this->printMethodInfo(_ZFP_ZFCore_ZFMethod_test_TestBase::ClassData());
        this->testCaseOutput("Child:");
        this->printMethodInfo(_ZFP_ZFCore_ZFMethod_test_TestChild::ClassData());

        this->testCaseOutputSeparator();

        this->prepareInstance();
        this->testCaseOutput("method registered after any of instances created:");
        this->testCaseOutputSeparator();
        this->testCaseOutput("Base:");
        this->printMethodInfo(_ZFP_ZFCore_ZFMethod_test_TestBase::ClassData());
        this->testCaseOutput("Child:");
        this->printMethodInfo(_ZFP_ZFCore_ZFMethod_test_TestChild::ClassData());

        // ============================================================
        this->testCaseOutputSeparator();
        this->testCaseOutput("ZFMethod directly access");
        this->testCaseOutput("methodNormal: %s",
                ZFMethodAccess(_ZFP_ZFCore_ZFMethod_test_TestBase, methodNormal)->objectInfo().cString());
        this->testCaseOutput("methodStatic: %s",
                ZFMethodAccess(_ZFP_ZFCore_ZFMethod_test_TestBase, methodStatic)->objectInfo().cString());
        this->testCaseOutput("methodOverloaded(zfint): %s",
                ZFMethodAccessDetail_1(_ZFP_ZFCore_ZFMethod_test_TestBase, methodOverloaded, ZFMP_IN(zfint, param0))
                ->objectInfo().cString());
        this->testCaseOutput("methodOverloaded(zffloat): %s",
                ZFMethodAccessDetail_1(_ZFP_ZFCore_ZFMethod_test_TestBase, methodOverloaded, ZFMP_IN(zffloat, param0))
                ->objectInfo().cString());

        this->testCaseOutput("unlike reflected by ZFClass, ZFMethodAccess would comiple error if method not defined");
#if 0 // this should not able to be compiled
        ZFMethodAccess(_ZFP_ZFCore_ZFMethod_test_TestBase, methodNotDefined);
#endif

        // ============================================================
        this->testCaseOutputSeparator();
        this->testCaseOutput("ZFMethod reflection");
        this->testCaseOutput("call method normally:");
        this->pBase->methodNormal();
        this->pChild->methodNormal();
        this->pBasePointToChild->methodNormal();

        this->testCaseOutputSeparator();
        this->testCaseOutput("call by reflect:");
        clsBase->methodForNameIgnoreParent("methodNormal")->execute<void>(this->pBase);
        clsBase->methodForNameIgnoreParent("methodNormal")->execute<void>(this->pChild);
        clsBase->methodForNameIgnoreParent("methodNormal")->execute<void>(this->pBasePointToChild);

        this->testCaseOutput("should be null since it's in base only: %p",
                clsChild->methodForNameIgnoreParent("methodNormal"));
        this->testCaseOutput("should not be null since we search hierarchically: %p",
                clsChild->methodForName("methodNormal"));
        this->testCaseOutput("should not be null since it's in child only: %p",
                clsChild->methodForNameIgnoreParent("methodInChild"));

        clsChild->methodForName("methodNormal")->execute<void>(this->pBase);
        clsChild->methodForName("methodNormal")->execute<void>(this->pChild);
        clsChild->methodForName("methodNormal")->execute<void>(this->pBasePointToChild);

        // ============================================================
        this->testCaseOutputSeparator();
        this->testCaseOutput("ZFMethod overloaded method");

        clsBase->methodForNameIgnoreParent("methodOverloaded", "zfint")->execute<void, zfint>(this->pBase, 123);
        clsBase->methodForNameIgnoreParent("methodOverloaded", "zffloat")->execute<void, zffloat>(this->pBase, 123.456f);

        // ============================================================
        this->testCaseOutputSeparator();
        this->testCaseOutput("ZFMethod with default param");

        this->pBase->methodForDefaultParam();
        clsBase->methodForNameIgnoreParent("methodForDefaultParam")->execute<void, const zfstring &>(this->pBase,
            "default value not available when reflect");

        // ============================================================
        this->testCaseOutputSeparator();
        this->testCaseOutput("return reference:");
        this->pBase->s = "oldString";
        this->testCaseOutput("before: %s", this->pBase->s.cString());
        clsBase->methodForNameIgnoreParent("methodReturnReference")->execute<zfstring &>(this->pBase) = "newString";
        this->testCaseOutput("after: %s", this->pBase->s.cString());

        this->testCaseOutputSeparator();

        zfstring _paramReference;
        zfstring &paramReference = _paramReference;
        this->testCaseOutput("param is reference:");
        paramReference = "oldString";
        this->testCaseOutput("before: %s", paramReference.cString());
        clsBase->methodForNameIgnoreParent("methodParamIsReference")->execute<void, zfstring &>(this->pBase, paramReference);
        this->testCaseOutput("after: %s", paramReference.cString());

        this->testCaseOutputSeparator();
        this->testCaseOutput("note that you must specify both ReturnType and ParamType, ");
        this->testCaseOutput("if not, no compiler would occurred, but app may crash at runtime");
#if 0
        // invoke method without explicitly specifying the param types
        // this code may or may not crash
        paramReference = "oldString";
        this->testCaseOutput("these code may have wrong result or crash:");
        this->testCaseOutput("before: %s", paramReference.cString()));
        clsBase->methodForNameIgnoreParent("methodParamIsReference")->execute<void>(this->pBase, paramReference);
        this->testCaseOutput("after: %s", paramReference.cString()));
#endif

        this->testCaseStop();
    }

private:
    _ZFP_ZFCore_ZFMethod_test_TestBase *pBase;
    _ZFP_ZFCore_ZFMethod_test_TestChild *pChild;
    _ZFP_ZFCore_ZFMethod_test_TestBase *pBasePointToChild;
protected:
    zfoverride
    virtual void objectOnInit(void)
    {
        zfsuper::objectOnInit();

        this->pBase = zfnull;
        this->pChild = zfnull;
        this->pBasePointToChild = zfnull;
    }
    zfoverride
    virtual void objectOnDealloc(void)
    {
        this->destroyInstance();
        zfsuper::objectOnDealloc();
    }
private:
    void prepareInstance(void)
    {
        this->destroyInstance();

        this->pBase = zfAlloc(_ZFP_ZFCore_ZFMethod_test_TestBase);
        this->pChild = zfAlloc(_ZFP_ZFCore_ZFMethod_test_TestChild);
        this->pBasePointToChild = this->pChild;
    }
    void destroyInstance(void)
    {
        zfRelease(this->pBase);
        zfRelease(this->pChild);

        this->pBase = zfnull;
        this->pChild = zfnull;
        this->pBasePointToChild = zfnull;
    }
    void printMethodInfo(const ZFClass *cls, const zfchar *indent = "  ")
    {
        for(zfindex i = 0; i < cls->methodCount(); ++i)
        {
            this->testCaseOutput("%s%s", indent, cls->methodAtIndex(i)->objectInfo().cString());
        }
    }
};
ZFOBJECT_REGISTER(ZFCore_ZFMethod_test)

ZF_NAMESPACE_GLOBAL_END

