#include "ZFImpl_ZFLua.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
static int _ZFP_ZFImpl_ZFLua_zfl_luaValue(ZF_IN lua_State *L)
{
    int count = (int)lua_gettop(L);
    if(count != 1)
    {
        return ZFImpl_ZFLua_luaError(L,
            "[zfl_luaValue] takes only one param, got %zi",
            (zfindex)count);
    }

    if(!lua_isuserdata(L, 1) && lua_islightuserdata(L, 1))
    {
        lua_pushvalue(L, 1);
        return 1;
    }

    return ZFImpl_ZFLua_toLuaValue(L, ZFImpl_ZFLua_luaGet(L, 1));
}

ZFImpl_ZFLua_implSetupCallback_DEFINE(zfl_luaValue, {
        ZFImpl_ZFLua_luaCFunctionRegister(L, "zfl_luaValue", _ZFP_ZFImpl_ZFLua_zfl_luaValue);
    }, {
    })

ZF_NAMESPACE_GLOBAL_END

