#include "ZFLua_test.h"
#include "ZFUIKit.h"

#include "ZFImpl/ZFLua/ZFImpl_ZFLua.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
static int _ZFP_ZFLua_performance_test_alloc(lua_State *L)
{
    zfblockedAlloc(ZFObject, t);
    ZFUNUSED(t);
    return 0;
}

// ============================================================
ZFMETHOD_FUNC_DECLARE_0(void, _ZFP_ZFLua_performance_test_f0)
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFLua_performance_test_f0)
{
}
ZFMETHOD_FUNC_DECLARE_4(void, _ZFP_ZFLua_performance_test_f4
        , ZFMP_IN(const zfchar *, p0)
        , ZFMP_IN(const zfchar *, p1)
        , ZFMP_IN(const zfchar *, p2)
        , ZFMP_IN(const zfchar *, p3)
        )
ZFMETHOD_FUNC_DEFINE_4(void, _ZFP_ZFLua_performance_test_f4
        , ZFMP_IN(const zfchar *, p0)
        , ZFMP_IN(const zfchar *, p1)
        , ZFMP_IN(const zfchar *, p2)
        , ZFMP_IN(const zfchar *, p3)
        )
{
}

static int _ZFP_ZFLua_performance_test_C_f0(lua_State *L)
{
    return 0;
}
static int _ZFP_ZFLua_performance_test_C_f4(lua_State *L)
{
    (void)lua_tostring(L, 1);
    (void)lua_tostring(L, 2);
    (void)lua_tostring(L, 3);
    (void)lua_tostring(L, 4);
    return 0;
}

// ============================================================
static int _ZFP_ZFLua_performance_test_begin(lua_State *L)
{
    ZFCoreStatistic::invokeTimeLogBegin("ZFLua_performance_test");
    return 0;
}
static int _ZFP_ZFLua_performance_test_end(lua_State *L)
{
    ZFCoreStatistic::invokeTimeLogEnd("ZFLua_performance_test");
    zfLogTrimT() << "[ZFLua_performance_test]" << lua_tostring(L, 1)
        << ZFTimeValueToStringFriendly(ZFCoreStatistic::invokeTimeGetTotalTime("ZFLua_performance_test"));
    ZFCoreStatistic::invokeTimeRemove("ZFLua_performance_test");
    return 0;
}

// ============================================================
zfclass ZFLua_performance_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFLua_performance_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();

        ZFImpl_ZFLua_luaCFunctionRegister((lua_State *)ZFLuaState(), "_ZFP_ZFLua_performance_test_begin", _ZFP_ZFLua_performance_test_begin);
        ZFImpl_ZFLua_luaCFunctionRegister((lua_State *)ZFLuaState(), "_ZFP_ZFLua_performance_test_end", _ZFP_ZFLua_performance_test_end);

        ZFImpl_ZFLua_luaCFunctionRegister((lua_State *)ZFLuaState(), "_ZFP_ZFLua_performance_test_alloc", _ZFP_ZFLua_performance_test_alloc);

        ZFImpl_ZFLua_luaCFunctionRegister((lua_State *)ZFLuaState(), "_ZFP_ZFLua_performance_test_C_f0", _ZFP_ZFLua_performance_test_C_f0);
        ZFImpl_ZFLua_luaCFunctionRegister((lua_State *)ZFLuaState(), "_ZFP_ZFLua_performance_test_C_f4", _ZFP_ZFLua_performance_test_C_f4);

        ZFLuaExecute(
                "local loop = 10000;\n"

                "_ZFP_ZFLua_performance_test_begin();\n"
                "for i=1,loop do\n"
                "    _ZFP_ZFLua_performance_test_alloc()\n"
                "end\n"
                "_ZFP_ZFLua_performance_test_end(' obj alloc:');\n"

                "_ZFP_ZFLua_performance_test_begin();\n"
                "for i=1,loop do\n"
                "    _ZFP_ZFLua_performance_test_C_f0()\n"
                "end\n"
                "_ZFP_ZFLua_performance_test_end('    C f0():');\n"

                "_ZFP_ZFLua_performance_test_begin();\n"
                "for i=1,loop do\n"
                "    _ZFP_ZFLua_performance_test_f0()\n"
                "end\n"
                "_ZFP_ZFLua_performance_test_end('ZFLua f0():');\n"

                "_ZFP_ZFLua_performance_test_begin();\n"
                "for i=1,loop do\n"
                "    _ZFP_ZFLua_performance_test_C_f4('1', '2', '3', '4')\n"
                "end\n"
                "_ZFP_ZFLua_performance_test_end('    C f4():');\n"

                "_ZFP_ZFLua_performance_test_begin();\n"
                "for i=1,loop do\n"
                "    _ZFP_ZFLua_performance_test_f4('1', '2', '3', '4')\n"
                "end\n"
                "_ZFP_ZFLua_performance_test_end('ZFLua f4():');\n"
            );

        this->testCaseStop();
    }
};
ZFOBJECT_REGISTER(ZFLua_performance_test)

ZF_NAMESPACE_GLOBAL_END

