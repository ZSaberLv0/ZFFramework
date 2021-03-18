#include "ZFCore_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_DEFINE_0(void, ZFCore_ZFMethodFunc_test_func)
{
    zfLogT();
}

ZF_NAMESPACE_BEGIN(ZFCore_ZFMethodFunc_test_namespace)
ZFMETHOD_FUNC_DEFINE_0(void, ZFCore_ZFMethodFunc_test_func)
{
    zfLogT();
}
ZF_NAMESPACE_END_WITH_REGISTER(ZFCore_ZFMethodFunc_test_namespace, ZF_NAMESPACE_GLOBAL)

ZFMETHOD_FUNC_DEFINE_0(zfstring, ZFCore_ZFMethodFunc_test_func0)
{
    zfLogT();
    return "ReturnValue";
}

ZFMETHOD_FUNC_DEFINE_1(zfstring, ZFCore_ZFMethodFunc_test_func1,
                       ZFMP_IN(zfstring, param0))
{
    zfLogT();
    return "ReturnValue";
}

ZFMETHOD_FUNC_DEFINE_2(zfstring, ZFCore_ZFMethodFunc_test_func2,
                       ZFMP_IN(zfstring, param0),
                       ZFMP_IN(zfstring, param1))
{
    zfLogT();
    return "ReturnValue";
}

ZFMETHOD_FUNC_DEFINE_3(zfstring, ZFCore_ZFMethodFunc_test_func3,
                       ZFMP_IN(zfstring, param0),
                       ZFMP_IN(zfstring, param1),
                       ZFMP_IN(zfstring, param2))
{
    zfLogT();
    return "ReturnValue";
}

ZFMETHOD_FUNC_DEFINE_4(zfstring, ZFCore_ZFMethodFunc_test_func4,
                       ZFMP_IN(zfstring, param0),
                       ZFMP_IN(zfstring, param1),
                       ZFMP_IN(zfstring, param2),
                       ZFMP_IN(zfstring, param3))
{
    zfLogT();
    return "ReturnValue";
}

ZFMETHOD_FUNC_DEFINE_5(zfstring, ZFCore_ZFMethodFunc_test_func5,
                       ZFMP_IN(zfstring, param0),
                       ZFMP_IN(zfstring, param1),
                       ZFMP_IN(zfstring, param2),
                       ZFMP_IN(zfstring, param3),
                       ZFMP_IN(zfstring, param4))
{
    zfLogT();
    return "ReturnValue";
}

ZFMETHOD_FUNC_DEFINE_6(zfstring, ZFCore_ZFMethodFunc_test_func6,
                       ZFMP_IN(zfstring, param0),
                       ZFMP_IN(zfstring, param1),
                       ZFMP_IN(zfstring, param2),
                       ZFMP_IN(zfstring, param3),
                       ZFMP_IN(zfstring, param4),
                       ZFMP_IN(zfstring, param5))
{
    zfLogT();
    return "ReturnValue";
}

ZFMETHOD_FUNC_DEFINE_7(zfstring, ZFCore_ZFMethodFunc_test_func7,
                       ZFMP_IN(zfstring, param0),
                       ZFMP_IN(zfstring, param1),
                       ZFMP_IN(zfstring, param2),
                       ZFMP_IN(zfstring, param3),
                       ZFMP_IN(zfstring, param4),
                       ZFMP_IN(zfstring, param5),
                       ZFMP_IN(zfstring, param6))
{
    zfLogT();
    return "ReturnValue";
}

ZFMETHOD_FUNC_DEFINE_8(zfstring, ZFCore_ZFMethodFunc_test_func8,
                       ZFMP_IN(zfstring, param0),
                       ZFMP_IN(zfstring, param1),
                       ZFMP_IN(zfstring, param2),
                       ZFMP_IN(zfstring, param3),
                       ZFMP_IN(zfstring, param4),
                       ZFMP_IN(zfstring, param5),
                       ZFMP_IN(zfstring, param6),
                       ZFMP_IN(zfstring, param7))
{
    zfLogT();
    return "ReturnValue";
}

// ============================================================
zfclass ZFCore_ZFMethodFunc_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFCore_ZFMethodFunc_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();

        ZFCoreArrayPOD<const ZFMethod *> methods;
        ZFFilterForZFMethod filter;
        {
            zfclassNotPOD _Holder
            {
                public:
                    static ZFFilterCallbackResult filter(ZF_IN const ZFMethod *const &e)
                    {
                        const zfchar *prefix = "ZFCore_ZFMethodFunc_test_";
                        if(zfsncmp(e->methodName(), prefix, zfslen(prefix)) == 0)
                        {
                            return ZFFilterCallbackResultActive;
                        }
                        else
                        {
                            return ZFFilterCallbackResultNotActive;
                        }
                    }
            };
            filter.customFilterCallbackAdd(_Holder::filter);
        }
        ZFMethodGetAllFuncT(methods, &filter);
        this->testCaseOutput(methods.objectInfoOfContent(
                zfindexMax(),
                ZFTokenForContainerPlainList()
            ));

        this->testCaseStop();
    }
};
ZFOBJECT_REGISTER(ZFCore_ZFMethodFunc_test)

ZF_NAMESPACE_GLOBAL_END

