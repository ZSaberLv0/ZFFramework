#include "ZFImpl_ZFLua.h"

ZF_NAMESPACE_GLOBAL_BEGIN

static int _ZFP_ZFImpl_ZFLua_ZFCoreArrayCreate(ZF_IN lua_State *L)
{
    zfblockedAlloc(v_ZFCoreArray, ret);
    int count = (int)lua_gettop(L);

    for(int i = 0; i < count; ++i)
    {
        zfautoObject p;
        if(ZFImpl_ZFLua_toObject(p, L, i + 1)
            || ZFImpl_ZFLua_toCallback(p, L, i + 1))
        {
            ret->zfv.add(p);
            continue;
        }

        zfblockedAlloc(v_zfstring, pTmp);
        if(ZFImpl_ZFLua_toString(pTmp->zfv, L, i + 1, zftrue))
        {
            ret->zfv.add(pTmp);
            continue;
        }

        return ZFImpl_ZFLua_luaError(L,
            "[ZFCoreArrayCreate] unknown param type: %s",
            ZFImpl_ZFLua_luaObjectInfo(L, i + 1, zftrue).cString());
    }

    ZFImpl_ZFLua_luaPush(L, ret);
    return 1;
}

// ============================================================
ZFImpl_ZFLua_implSetupCallback_DEFINE(ZFCoreArrayCreate, {
        ZFImpl_ZFLua_luaCFunctionRegister(L, "ZFCoreArrayCreate", _ZFP_ZFImpl_ZFLua_ZFCoreArrayCreate);
    }, {
    })

ZF_NAMESPACE_GLOBAL_END

