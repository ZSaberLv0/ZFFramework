#include "ZFImpl_ZFLua.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/*
 * zfl_call(obj, "classInstanceMethodName"/ZFMethod, param0, param1, ...)
 */
static int _ZFP_ZFImpl_ZFLua_zfl_call(ZF_IN lua_State *L);
/*
 * zfl_callStatic("Scope1.Scope2.methodName"/ZFMethod/ZFClass, param0, param1, ...)
 */
static int _ZFP_ZFImpl_ZFLua_zfl_callStatic(ZF_IN lua_State *L);
/*
 * zfl_callStatic2("Scope", "methodName", param0, param1, ...)
 */
static int _ZFP_ZFImpl_ZFLua_zfl_callStatic2(ZF_IN lua_State *L);

ZFImpl_ZFLua_implSetupCallback_DEFINE(zfl_call, {
        ZFImpl_ZFLua_luaCFunctionRegister(L, "zfl_call", _ZFP_ZFImpl_ZFLua_zfl_call);
        ZFImpl_ZFLua_luaCFunctionRegister(L, "zfl_callStatic", _ZFP_ZFImpl_ZFLua_zfl_callStatic);
        ZFImpl_ZFLua_luaCFunctionRegister(L, "zfl_callStatic2", _ZFP_ZFImpl_ZFLua_zfl_callStatic2);
    }, {
    })

// ============================================================
/*
 * type can be:
 * -  string type
 * -  #v_ZFMethod
 * -  #v_ZFClass
 */
static int _ZFP_ZFImpl_ZFLua_zfl_call_impl(ZF_IN lua_State *L,
                                           ZF_IN const zfchar *zfl_funcName,
                                           ZF_IN ZFObject *obj,
                                           ZF_IN const zfchar *NS,
                                           ZF_IN ZFObject *type,
                                           ZF_IN int paramCount,
                                           ZF_IN int luaParamOffset)
{
    zfautoObject paramList[ZFMETHOD_MAX_PARAM];
    for(zfindex i = 0; i < ZFMETHOD_MAX_PARAM; ++i)
    {
        paramList[i].zflockfree_assign(ZFMethodGenericInvokerDefaultParamHolder());
    }

    for(int i = 0; i < paramCount; ++i)
    {
        if(!ZFImpl_ZFLua_toGeneric(paramList[i], L, luaParamOffset + i))
        {
            return ZFImpl_ZFLua_luaError(L,
                "[%s] failed to get param%d, got %s, while executing: %s",
                zfl_funcName,
                i,
                ZFImpl_ZFLua_luaObjectInfo(L, luaParamOffset + i, zftrue).cString(),
                ZFObjectInfo(type).cString());
        }
    }

    zfautoObject ret;
    zfstring errorHint;
    if(ZFDI_invoke(ret, &errorHint, obj, NS, type, (zfindex)paramCount, paramList))
    {
        ZFImpl_ZFLua_luaPush(L, ret);
        return 1;
    }
    return ZFImpl_ZFLua_luaError(L,
        "[%s] %s",
        zfl_funcName,
        errorHint.cString());
}

// ============================================================
/*
 * zfl_call(obj, "classInstanceMethodName"/ZFMethod, param0, param1, ...)
 */
static int _ZFP_ZFImpl_ZFLua_zfl_call(ZF_IN lua_State *L)
{
    static const int luaParamOffset = 3;
    static const zfchar *zfl_funcName = "zfl_call";
    int count = (int)lua_gettop(L);
    if(count < luaParamOffset - 1 || count > ZFMETHOD_MAX_PARAM + luaParamOffset - 1)
    {
        return ZFImpl_ZFLua_luaError(L,
            "[%s] invalid param, expect zfl_call(obj, \"methodName\", param0, param1, ...), got %zi param",
            zfl_funcName,
            (zfindex)count);
    }
    int paramCount = count - (luaParamOffset - 1);

    zfautoObject obj;
    if(!ZFImpl_ZFLua_toObject(obj, L, 1))
    {
        return ZFImpl_ZFLua_luaError(L,
            "[%s] failed to access caller object, expect zfautoObject, got %s, while executing: %s",
            zfl_funcName,
            ZFImpl_ZFLua_luaObjectInfo(L, 1, zftrue).cString(),
            ZFImpl_ZFLua_luaObjectInfo(L, 2).cString());
    }
    if(obj == zfnull)
    {
        return ZFImpl_ZFLua_luaError(L,
            "[%s] caller object must not be null, while executing: %s",
            zfl_funcName,
            ZFImpl_ZFLua_luaObjectInfo(L, 2).cString());
    }
    zfautoObject type;
    if(!ZFImpl_ZFLua_toGeneric(type, L, 2))
    {
        return ZFImpl_ZFLua_luaError(L,
            "[%s] unable to access method, got: %s",
            zfl_funcName,
            ZFImpl_ZFLua_luaObjectInfo(L, 2).cString());
    }

    return _ZFP_ZFImpl_ZFLua_zfl_call_impl(
        L,
        zfl_funcName,
        obj,
        zfnull,
        type,
        paramCount,
        luaParamOffset);
}

/*
 * zfl_callStatic("Scope1.Scope2.methodName"/ZFMethod/ZFClass, param0, param1, ...)
 */
static int _ZFP_ZFImpl_ZFLua_zfl_callStatic(ZF_IN lua_State *L)
{
    static const int luaParamOffset = 2;
    static const zfchar *zfl_funcName = "zfl_callStatic";
    int count = (int)lua_gettop(L);
    if(count < luaParamOffset - 1 || count > ZFMETHOD_MAX_PARAM + luaParamOffset - 1)
    {
        return ZFImpl_ZFLua_luaError(L,
            "[%s] invalid param, expect zfl_callStatic(\"Scope1.Scope2.methodName\", param0, param1, ...), got %zi param",
            zfl_funcName,
            (zfindex)count);
    }
    int paramCount = count - (luaParamOffset - 1);

    zfautoObject type;
    if(!ZFImpl_ZFLua_toGeneric(type, L, 1))
    {
        return ZFImpl_ZFLua_luaError(L,
            "[%s] unable to access method, got: %s",
            zfl_funcName,
            ZFImpl_ZFLua_luaObjectInfo(L, 1).cString());
    }

    return _ZFP_ZFImpl_ZFLua_zfl_call_impl(
        L,
        zfl_funcName,
        zfnull,
        zfnull,
        type,
        paramCount,
        luaParamOffset);
}

/*
 * zfl_callStatic2("Scope", "methodName", param0, param1, ...)
 */
static int _ZFP_ZFImpl_ZFLua_zfl_callStatic2(ZF_IN lua_State *L)
{
    static int luaParamOffset = 3;
    static const zfchar *zfl_funcName = "zfl_callStatic2";
    int count = (int)lua_gettop(L);
    if(count < luaParamOffset - 1 || count > ZFMETHOD_MAX_PARAM + luaParamOffset - 1)
    {
        return ZFImpl_ZFLua_luaError(L,
            "[%s] invalid param, expect zfl_callStatic2(\"Scope\", \"methodName\", param0, param1, ...), got %zi param",
            zfl_funcName,
            (zfindex)count);
    }
    int paramCount = count - (luaParamOffset - 1);

    const zfchar *classOrNamespace = zfnull;
    if(!ZFImpl_ZFLua_toString(classOrNamespace, L, 1, zfHint("allowEmpty")zftrue))
    {
        return ZFImpl_ZFLua_luaError(L,
            "[%s] failed to access method scope, expect string type, got %s",
            zfl_funcName,
            ZFImpl_ZFLua_luaObjectInfo(L, 1, zftrue).cString());
    }
    zfautoObject type;
    if(!ZFImpl_ZFLua_toGeneric(type, L, 2))
    {
        return ZFImpl_ZFLua_luaError(L,
            "[%s] failed to access method name, expect string type, got %s",
            zfl_funcName,
            ZFImpl_ZFLua_luaObjectInfo(L, 2, zftrue).cString());
    }

    return _ZFP_ZFImpl_ZFLua_zfl_call_impl(
        L,
        zfl_funcName,
        zfnull,
        classOrNamespace,
        type,
        paramCount,
        luaParamOffset);
}

ZF_NAMESPACE_GLOBAL_END

