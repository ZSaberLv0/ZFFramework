#include "ZFCore_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclass ZFCore_ZFMethodFuncUserRegister_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFCore_ZFMethodFuncUserRegister_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();

        {
            this->testCaseOutputSeparator();
            this->testCaseOutput("try execute dynamically registered method");

            ZFMethodFuncUserRegisterDetail_1(method, {
                    zfLogT();
                    return "returnValue";
                }, "ZFCore_ZFMethodFuncUserRegister_testNamespace", ZF_CALLER_LINE,
                zfstring, "funcDynamic"
                , ZFMP_IN(zfstring const &, param0)
                );

            ZFMethodFuncUserRegisterDetail_0(method0, {
                    zfLogT();
                    return "returnValue";
                }, "ZFCore_ZFMethodFuncUserRegister_testNamespace", ZF_CALLER_LINE,
                zfstring, "funcDynamic0"
                );

            ZFMethodFuncUserRegisterDetail_1(method1, {
                    zfLogT();
                    return "returnValue";
                }, "ZFCore_ZFMethodFuncUserRegister_testNamespace", ZF_CALLER_LINE,
                zfstring, "funcDynamic1"
                , ZFMP_IN(zfstring const &, param0)
                );

            ZFMethodFuncUserRegisterDetail_2(method2, {
                    zfLogT();
                    return "returnValue";
                }, "ZFCore_ZFMethodFuncUserRegister_testNamespace", ZF_CALLER_LINE,
                zfstring, "funcDynamic2"
                , ZFMP_IN(zfstring const &, param0)
                , ZFMP_IN(zfstring const &, param1)
                );

            ZFMethodFuncUserRegisterDetail_3(method3, {
                    zfLogT();
                    return "returnValue";
                }, "ZFCore_ZFMethodFuncUserRegister_testNamespace", ZF_CALLER_LINE,
                zfstring, "funcDynamic3"
                , ZFMP_IN(zfstring const &, param0)
                , ZFMP_IN(zfstring const &, param1)
                , ZFMP_IN(zfstring const &, param2)
                );

            ZFMethodFuncUserRegisterDetail_4(method4, {
                    zfLogT();
                    return "returnValue";
                }, "ZFCore_ZFMethodFuncUserRegister_testNamespace", ZF_CALLER_LINE,
                zfstring, "funcDynamic4"
                , ZFMP_IN(zfstring const &, param0)
                , ZFMP_IN(zfstring const &, param1)
                , ZFMP_IN(zfstring const &, param2)
                , ZFMP_IN(zfstring const &, param3)
                );

            ZFMethodFuncUserRegisterDetail_5(method5, {
                    zfLogT();
                    return "returnValue";
                }, "ZFCore_ZFMethodFuncUserRegister_testNamespace", ZF_CALLER_LINE,
                zfstring, "funcDynamic5"
                , ZFMP_IN(zfstring const &, param0)
                , ZFMP_IN(zfstring const &, param1)
                , ZFMP_IN(zfstring const &, param2)
                , ZFMP_IN(zfstring const &, param3)
                , ZFMP_IN(zfstring const &, param4)
                );

            ZFMethodFuncUserRegisterDetail_6(method6, {
                    zfLogT();
                    return "returnValue";
                }, "ZFCore_ZFMethodFuncUserRegister_testNamespace", ZF_CALLER_LINE,
                zfstring, "funcDynamic6"
                , ZFMP_IN(zfstring const &, param0)
                , ZFMP_IN(zfstring const &, param1)
                , ZFMP_IN(zfstring const &, param2)
                , ZFMP_IN(zfstring const &, param3)
                , ZFMP_IN(zfstring const &, param4)
                , ZFMP_IN(zfstring const &, param5)
                );

            ZFMethodFuncUserRegisterDetail_7(method7, {
                    zfLogT();
                    return "returnValue";
                }, "ZFCore_ZFMethodFuncUserRegister_testNamespace", ZF_CALLER_LINE,
                zfstring, "funcDynamic7"
                , ZFMP_IN(zfstring const &, param0)
                , ZFMP_IN(zfstring const &, param1)
                , ZFMP_IN(zfstring const &, param2)
                , ZFMP_IN(zfstring const &, param3)
                , ZFMP_IN(zfstring const &, param4)
                , ZFMP_IN(zfstring const &, param5)
                , ZFMP_IN(zfstring const &, param6)
                );

            ZFMethodFuncUserRegisterDetail_8(method8, {
                    zfLogT();
                    return "returnValue";
                }, "ZFCore_ZFMethodFuncUserRegister_testNamespace", ZF_CALLER_LINE,
                zfstring, "funcDynamic8"
                , ZFMP_IN(zfstring const &, param0)
                , ZFMP_IN(zfstring const &, param1)
                , ZFMP_IN(zfstring const &, param2)
                , ZFMP_IN(zfstring const &, param3)
                , ZFMP_IN(zfstring const &, param4)
                , ZFMP_IN(zfstring const &, param5)
                , ZFMP_IN(zfstring const &, param6)
                , ZFMP_IN(zfstring const &, param7)
                );

            zfstring ret = ZFMethodForName("ZFCore_ZFMethodFuncUserRegister_testNamespace", "funcDynamic")
                ->execute<zfstring, zfstring const &>(this, "paramValue");
            this->testCaseOutput("execute result: %s", ret.cString());

            ZFMethodFuncUserUnregister(ZFMethodForName("ZFCore_ZFMethodFuncUserRegister_testNamespace", "funcDynamic"));

            zfLogTrimT() << "after unregister, method:"
                << ZFMethodForName("ZFCore_ZFMethodFuncUserRegister_testNamespace", "funcDynamic");

            this->testCaseOutput("all list:");
            for(zfindex i = 0; i <= 8; ++i)
            {
                this->testCaseOutput("  %s",
                    ZFMethodForName(
                            "ZFCore_ZFMethodFuncUserRegister_testNamespace",
                            zfstringWithFormat("funcDynamic%zi", i)
                        )->objectInfo().cString()
                    );
            }

            for(zfindex i = 0; i <= 8; ++i)
            {
                ZFMethodFuncUserUnregister(ZFMethodForName("ZFCore_ZFMethodFuncUserRegister_testNamespace",
                        zfstringWithFormat("funcDynamic%zi", i))
                    );
            }
        }

        {
            this->testCaseOutputSeparator();
            this->testCaseOutput("try execute statically registered method");

            zfstring ret = ZFMethodForName("ZFCore_ZFMethodFuncUserRegister_testNamespace", "funcStatic")
                ->execute<zfstring, zfstring const &>(this, "paramValue");
            this->testCaseOutput("execute result: %s", ret.cString());

            this->testCaseOutput("all list:");
            for(zfindex i = 0; i <= ZFMETHOD_MAX_PARAM; ++i)
            {
                this->testCaseOutput("  %s",
                    ZFMethodForName(
                            "ZFCore_ZFMethodFuncUserRegister_testNamespace",
                            zfstringWithFormat("funcStatic%zi", i)
                        )->objectInfo().cString()
                    );
            }
        }

        this->testCaseStop();
    }
};
ZFOBJECT_REGISTER(ZFCore_ZFMethodFuncUserRegister_test)

ZF_NAMESPACE_BEGIN(ZFCore_ZFMethodFuncUserRegister_testNamespace)

ZFMETHOD_FUNC_USER_REGISTER_DETAIL_1({
        zfLogT();
        return "returnValue";
    }, ZF_NAMESPACE_CURRENT(), ZF_CALLER_LINE,
    zfstring, funcStatic
    , ZFMP_IN(zfstring const &, param0)
    )

ZFMETHOD_FUNC_USER_REGISTER_DETAIL_0({
        zfLogT();
        return "returnValue";
    }, ZF_NAMESPACE_CURRENT(), ZF_CALLER_LINE,
    zfstring, funcStatic0
    )

ZFMETHOD_FUNC_USER_REGISTER_DETAIL_1({
        zfLogT();
        return "returnValue";
    }, ZF_NAMESPACE_CURRENT(), ZF_CALLER_LINE,
    zfstring, funcStatic1
    , ZFMP_IN(zfstring const &, param0)
    )

ZFMETHOD_FUNC_USER_REGISTER_DETAIL_2({
        zfLogT();
        return "returnValue";
    }, ZF_NAMESPACE_CURRENT(), ZF_CALLER_LINE,
    zfstring, funcStatic2
    , ZFMP_IN(zfstring const &, param0)
    , ZFMP_IN(zfstring const &, param1)
    )

ZFMETHOD_FUNC_USER_REGISTER_DETAIL_3({
        zfLogT();
        return "returnValue";
    }, ZF_NAMESPACE_CURRENT(), ZF_CALLER_LINE,
    zfstring, funcStatic3
    , ZFMP_IN(zfstring const &, param0)
    , ZFMP_IN(zfstring const &, param1)
    , ZFMP_IN(zfstring const &, param2)
    )

ZFMETHOD_FUNC_USER_REGISTER_DETAIL_4({
        zfLogT();
        return "returnValue";
    }, ZF_NAMESPACE_CURRENT(), ZF_CALLER_LINE,
    zfstring, funcStatic4
    , ZFMP_IN(zfstring const &, param0)
    , ZFMP_IN(zfstring const &, param1)
    , ZFMP_IN(zfstring const &, param2)
    , ZFMP_IN(zfstring const &, param3)
    )

ZFMETHOD_FUNC_USER_REGISTER_DETAIL_5({
        zfLogT();
        return "returnValue";
    }, ZF_NAMESPACE_CURRENT(), ZF_CALLER_LINE,
    zfstring, funcStatic5
    , ZFMP_IN(zfstring const &, param0)
    , ZFMP_IN(zfstring const &, param1)
    , ZFMP_IN(zfstring const &, param2)
    , ZFMP_IN(zfstring const &, param3)
    , ZFMP_IN(zfstring const &, param4)
    )

ZFMETHOD_FUNC_USER_REGISTER_DETAIL_6({
        zfLogT();
        return "returnValue";
    }, ZF_NAMESPACE_CURRENT(), ZF_CALLER_LINE,
    zfstring, funcStatic6
    , ZFMP_IN(zfstring const &, param0)
    , ZFMP_IN(zfstring const &, param1)
    , ZFMP_IN(zfstring const &, param2)
    , ZFMP_IN(zfstring const &, param3)
    , ZFMP_IN(zfstring const &, param4)
    , ZFMP_IN(zfstring const &, param5)
    )

ZFMETHOD_FUNC_USER_REGISTER_DETAIL_7({
        zfLogT();
        return "returnValue";
    }, ZF_NAMESPACE_CURRENT(), ZF_CALLER_LINE,
    zfstring, funcStatic7
    , ZFMP_IN(zfstring const &, param0)
    , ZFMP_IN(zfstring const &, param1)
    , ZFMP_IN(zfstring const &, param2)
    , ZFMP_IN(zfstring const &, param3)
    , ZFMP_IN(zfstring const &, param4)
    , ZFMP_IN(zfstring const &, param5)
    , ZFMP_IN(zfstring const &, param6)
    )

ZFMETHOD_FUNC_USER_REGISTER_DETAIL_8({
        zfLogT();
        return "returnValue";
    }, ZF_NAMESPACE_CURRENT(), ZF_CALLER_LINE,
    zfstring, funcStatic8
    , ZFMP_IN(zfstring const &, param0)
    , ZFMP_IN(zfstring const &, param1)
    , ZFMP_IN(zfstring const &, param2)
    , ZFMP_IN(zfstring const &, param3)
    , ZFMP_IN(zfstring const &, param4)
    , ZFMP_IN(zfstring const &, param5)
    , ZFMP_IN(zfstring const &, param6)
    , ZFMP_IN(zfstring const &, param7)
    )

ZF_NAMESPACE_END_WITH_REGISTER(ZFCore_ZFMethodFuncUserRegister_testNamespace, ZF_NAMESPACE_GLOBAL)

ZF_NAMESPACE_GLOBAL_END

