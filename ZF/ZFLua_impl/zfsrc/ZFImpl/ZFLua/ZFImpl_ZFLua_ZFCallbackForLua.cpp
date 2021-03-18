#include "ZFImpl_ZFLua.h"

ZF_NAMESPACE_GLOBAL_BEGIN

static int _ZFP_ZFImpl_ZFLua_ZFCallbackForLua(ZF_IN lua_State *L)
{
    int count = (int)lua_gettop(L);
    if(count != 1)
    {
        ZFLuaErrorOccurredTrim(
            "[ZFCallbackForLua] expect one param, got %zi",
            (zfindex)count);
        return ZFImpl_ZFLua_luaError(L);
    }

    zfautoObject ret;
    if(!ZFImpl_ZFLua_toCallback(ret, L, 1))
    {
        ZFLuaErrorOccurredTrim(
            "[ZFCallbackForLua] unable to access the callback, got %s",
            ZFImpl_ZFLua_luaObjectInfo(L, 1, zftrue).cString());
        return ZFImpl_ZFLua_luaError(L);
    }
    else
    {
        ZFImpl_ZFLua_luaPush(L, ret);
        return 1;
    }
}

// ============================================================
ZFImpl_ZFLua_implSetupCallback_DEFINE(ZFCallbackForLua, {
        ZFImpl_ZFLua_luaCFunctionRegister(L, "ZFCallbackForLua", _ZFP_ZFImpl_ZFLua_ZFCallbackForLua);
    }, {
    })

ZF_NAMESPACE_GLOBAL_END

