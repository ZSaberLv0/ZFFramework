#include "ZFImpl_ZFLua_zfAlloc.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfbool ZFImpl_ZFLua_zfAlloc(ZF_OUT zfautoObject &ret,
                            ZF_IN lua_State *L,
                            ZF_IN const ZFClass *cls,
                            ZF_IN int paramCount,
                            ZF_IN int luaParamOffset)
{
    if(paramCount == 0)
    {
        ret = cls->newInstance();
        return zftrue;
    }

    zfautoObject paramList[ZFMETHOD_MAX_PARAM];
    for(zfindex i = 0; i < ZFMETHOD_MAX_PARAM; ++i)
    {
        paramList[i].zflockfree_assign(ZFMethodGenericInvokerDefaultParamHolder());
    }
    for(int i = 0; i < paramCount; ++i)
    {
        if(!ZFImpl_ZFLua_toGeneric(paramList[i], L, luaParamOffset + i))
        {
            return zffalse;
        }
    }
    return ZFDI_alloc(ret, zfnull, cls, (zfindex)paramCount, paramList);
}

// ============================================================
static int _ZFP_ZFImpl_ZFLua_zfAlloc(ZF_IN lua_State *L)
{
    static const int luaParamOffset = 2;
    int count = (int)lua_gettop(L);
    if(count < luaParamOffset - 1)
    {
        return ZFImpl_ZFLua_luaError(L,
            "[zfAlloc] takes at least one param");
    }
    int paramCount = (count - (luaParamOffset - 1));

    zfautoObject clsHolder;
    if(!ZFImpl_ZFLua_toGeneric(clsHolder, L, 1))
    {
        return ZFImpl_ZFLua_luaError(L,
            "[zfAlloc] unable to access class");
    }

    zfautoObject paramList[ZFMETHOD_MAX_PARAM];
    for(zfindex i = 0; i < ZFMETHOD_MAX_PARAM; ++i)
    {
        paramList[i].zflockfree_assign(ZFMethodGenericInvokerDefaultParamHolder());
    }
    for(int i = 0; i < paramCount; ++i)
    {
        if(!ZFImpl_ZFLua_toGeneric(paramList[i], L, luaParamOffset + i))
        {
            return zffalse;
        }
    }

    zfautoObject ret;
    ZFDI_alloc(ret, zfnull, zfnull, clsHolder, (zfindex)paramCount, paramList);
    ZFImpl_ZFLua_luaPush(L, ret);
    return 1;
}

// ============================================================
ZFImpl_ZFLua_implSetupCallback_DEFINE(zfAlloc, {
        ZFImpl_ZFLua_luaCFunctionRegister(L, "zfAlloc", _ZFP_ZFImpl_ZFLua_zfAlloc);
    }, {
    })

ZF_NAMESPACE_GLOBAL_END

