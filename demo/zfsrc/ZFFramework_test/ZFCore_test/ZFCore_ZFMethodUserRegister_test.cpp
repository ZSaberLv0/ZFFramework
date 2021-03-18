#include "ZFCore_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclass ZFCore_ZFMethodUserRegister_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFCore_ZFMethodUserRegister_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();

        {
            this->testCaseOutputSeparator();
            this->testCaseOutput("try execute dynamically registered method");

            ZFMethodUserRegisterDetail_1(method, {
                    zfLogT();
                    return "returnValue";
                }, zfself::ClassData(), public, ZFMethodTypeNormal,
                zfstring, "funcDynamic"
                , ZFMP_IN(zfstring const &, param0)
                );

            ZFMethodUserRegisterDetail_0(method0, {
                    zfLogT();
                    return "returnValue";
                }, zfself::ClassData(), public, ZFMethodTypeNormal,
                zfstring, "funcDynamic0"
                );

            ZFMethodUserRegisterDetail_1(method1, {
                    zfLogT();
                    return "returnValue";
                }, zfself::ClassData(), public, ZFMethodTypeNormal,
                zfstring, "funcDynamic1"
                , ZFMP_IN(zfstring const &, param0)
                );

            ZFMethodUserRegisterDetail_2(method2, {
                    zfLogT();
                    return "returnValue";
                }, zfself::ClassData(), public, ZFMethodTypeNormal,
                zfstring, "funcDynamic2"
                , ZFMP_IN(zfstring const &, param0)
                , ZFMP_IN(zfstring const &, param1)
                );

            ZFMethodUserRegisterDetail_3(method3, {
                    zfLogT();
                    return "returnValue";
                }, zfself::ClassData(), public, ZFMethodTypeNormal,
                zfstring, "funcDynamic3"
                , ZFMP_IN(zfstring const &, param0)
                , ZFMP_IN(zfstring const &, param1)
                , ZFMP_IN(zfstring const &, param2)
                );

            ZFMethodUserRegisterDetail_4(method4, {
                    zfLogT();
                    return "returnValue";
                }, zfself::ClassData(), public, ZFMethodTypeNormal,
                zfstring, "funcDynamic4"
                , ZFMP_IN(zfstring const &, param0)
                , ZFMP_IN(zfstring const &, param1)
                , ZFMP_IN(zfstring const &, param2)
                , ZFMP_IN(zfstring const &, param3)
                );

            ZFMethodUserRegisterDetail_5(method5, {
                    zfLogT();
                    return "returnValue";
                }, zfself::ClassData(), public, ZFMethodTypeNormal,
                zfstring, "funcDynamic5"
                , ZFMP_IN(zfstring const &, param0)
                , ZFMP_IN(zfstring const &, param1)
                , ZFMP_IN(zfstring const &, param2)
                , ZFMP_IN(zfstring const &, param3)
                , ZFMP_IN(zfstring const &, param4)
                );

            ZFMethodUserRegisterDetail_6(method6, {
                    zfLogT();
                    return "returnValue";
                }, zfself::ClassData(), public, ZFMethodTypeNormal,
                zfstring, "funcDynamic6"
                , ZFMP_IN(zfstring const &, param0)
                , ZFMP_IN(zfstring const &, param1)
                , ZFMP_IN(zfstring const &, param2)
                , ZFMP_IN(zfstring const &, param3)
                , ZFMP_IN(zfstring const &, param4)
                , ZFMP_IN(zfstring const &, param5)
                );

            ZFMethodUserRegisterDetail_7(method7, {
                    zfLogT();
                    return "returnValue";
                }, zfself::ClassData(), public, ZFMethodTypeNormal,
                zfstring, "funcDynamic7"
                , ZFMP_IN(zfstring const &, param0)
                , ZFMP_IN(zfstring const &, param1)
                , ZFMP_IN(zfstring const &, param2)
                , ZFMP_IN(zfstring const &, param3)
                , ZFMP_IN(zfstring const &, param4)
                , ZFMP_IN(zfstring const &, param5)
                , ZFMP_IN(zfstring const &, param6)
                );

            ZFMethodUserRegisterDetail_8(method8, {
                    zfLogT();
                    return "returnValue";
                }, zfself::ClassData(), public, ZFMethodTypeNormal,
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

            zfstring ret = zfself::ClassData()->methodForName("funcDynamic")->execute<zfstring, zfstring const &>(this, "paramValue");
            this->testCaseOutput("execute result: %s", ret.cString());

            ZFMethodUserUnregister(zfself::ClassData()->methodForName("funcDynamic"));

            zfLogTrimT() << "after unregister, method:" << zfself::ClassData()->methodForName("funcDynamic");

            this->testCaseOutput("all list:");
            for(zfindex i = 0; i <= 8; ++i)
            {
                this->testCaseOutput("  %s",
                    zfself::ClassData()->methodForName(zfstringWithFormat("funcDynamic%zi", i))->objectInfo().cString());
            }

            for(zfindex i = 0; i <= 8; ++i)
            {
                ZFMethodUserUnregister(zfself::ClassData()->methodForName(zfstringWithFormat("funcDynamic%zi", i)));
            }
        }

        {
            this->testCaseOutputSeparator();
            this->testCaseOutput("try execute statically registered method");

            zfstring ret = zfself::ClassData()->methodForName("funcStatic")->execute<zfstring, zfstring const &>(this, "paramValue");
            this->testCaseOutput("execute result: %s", ret.cString());

            this->testCaseOutput("all list:");
            for(zfindex i = 0; i <= ZFMETHOD_MAX_PARAM; ++i)
            {
                this->testCaseOutput("  %s",
                    zfself::ClassData()->methodForName(zfstringWithFormat("funcStatic%zi", i))->objectInfo().cString());
            }
        }

        this->testCaseStop();
    }
};
ZFOBJECT_REGISTER(ZFCore_ZFMethodUserRegister_test)

ZFMETHOD_USER_REGISTER_DETAIL_1({
        zfLogT();
        return "returnValue";
    }, ZFCore_ZFMethodUserRegister_test, public, ZFMethodTypeStatic, ZF_CALLER_LINE,
    zfstring, funcStatic
    , ZFMP_IN(zfstring const &, param0)
    )

ZFMETHOD_USER_REGISTER_DETAIL_0({
        zfLogT();
        return "returnValue";
    }, ZFCore_ZFMethodUserRegister_test, public, ZFMethodTypeStatic, ZF_CALLER_LINE,
    zfstring, funcStatic0
    )

ZFMETHOD_USER_REGISTER_DETAIL_1({
        zfLogT();
        return "returnValue";
    }, ZFCore_ZFMethodUserRegister_test, public, ZFMethodTypeStatic, ZF_CALLER_LINE,
    zfstring, funcStatic1
    , ZFMP_IN(zfstring const &, param0)
    )

ZFMETHOD_USER_REGISTER_DETAIL_2({
        zfLogT();
        return "returnValue";
    }, ZFCore_ZFMethodUserRegister_test, public, ZFMethodTypeStatic, ZF_CALLER_LINE,
    zfstring, funcStatic2
    , ZFMP_IN(zfstring const &, param0)
    , ZFMP_IN(zfstring const &, param1)
    )

ZFMETHOD_USER_REGISTER_DETAIL_3({
        zfLogT();
        return "returnValue";
    }, ZFCore_ZFMethodUserRegister_test, public, ZFMethodTypeStatic, ZF_CALLER_LINE,
    zfstring, funcStatic3
    , ZFMP_IN(zfstring const &, param0)
    , ZFMP_IN(zfstring const &, param1)
    , ZFMP_IN(zfstring const &, param2)
    )

ZFMETHOD_USER_REGISTER_DETAIL_4({
        zfLogT();
        return "returnValue";
    }, ZFCore_ZFMethodUserRegister_test, public, ZFMethodTypeStatic, ZF_CALLER_LINE,
    zfstring, funcStatic4
    , ZFMP_IN(zfstring const &, param0)
    , ZFMP_IN(zfstring const &, param1)
    , ZFMP_IN(zfstring const &, param2)
    , ZFMP_IN(zfstring const &, param3)
    )

ZFMETHOD_USER_REGISTER_DETAIL_5({
        zfLogT();
        return "returnValue";
    }, ZFCore_ZFMethodUserRegister_test, public, ZFMethodTypeStatic, ZF_CALLER_LINE,
    zfstring, funcStatic5
    , ZFMP_IN(zfstring const &, param0)
    , ZFMP_IN(zfstring const &, param1)
    , ZFMP_IN(zfstring const &, param2)
    , ZFMP_IN(zfstring const &, param3)
    , ZFMP_IN(zfstring const &, param4)
    )

ZFMETHOD_USER_REGISTER_DETAIL_6({
        zfLogT();
        return "returnValue";
    }, ZFCore_ZFMethodUserRegister_test, public, ZFMethodTypeStatic, ZF_CALLER_LINE,
    zfstring, funcStatic6
    , ZFMP_IN(zfstring const &, param0)
    , ZFMP_IN(zfstring const &, param1)
    , ZFMP_IN(zfstring const &, param2)
    , ZFMP_IN(zfstring const &, param3)
    , ZFMP_IN(zfstring const &, param4)
    , ZFMP_IN(zfstring const &, param5)
    )

ZFMETHOD_USER_REGISTER_DETAIL_7({
        zfLogT();
        return "returnValue";
    }, ZFCore_ZFMethodUserRegister_test, public, ZFMethodTypeStatic, ZF_CALLER_LINE,
    zfstring, funcStatic7
    , ZFMP_IN(zfstring const &, param0)
    , ZFMP_IN(zfstring const &, param1)
    , ZFMP_IN(zfstring const &, param2)
    , ZFMP_IN(zfstring const &, param3)
    , ZFMP_IN(zfstring const &, param4)
    , ZFMP_IN(zfstring const &, param5)
    , ZFMP_IN(zfstring const &, param6)
    )

ZFMETHOD_USER_REGISTER_DETAIL_8({
        zfLogT();
        return "returnValue";
    }, ZFCore_ZFMethodUserRegister_test, public, ZFMethodTypeStatic, ZF_CALLER_LINE,
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

ZF_NAMESPACE_GLOBAL_END

