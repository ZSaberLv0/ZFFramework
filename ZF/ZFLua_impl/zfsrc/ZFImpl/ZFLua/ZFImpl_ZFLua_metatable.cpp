#include "ZFImpl_ZFLua.h"

#include "ZFCore/ZFSTLWrapper/zfstl_string.h"

ZF_NAMESPACE_GLOBAL_BEGIN

#define _ZFP_ZFImpl_ZFLua_metatable_PrepareParam(param, luaStackOffset) \
    zfautoObject _##param; \
    const ZFClass *param##Cls = zfnull; \
    if(!ZFImpl_ZFLua_toNumberT(_##param, L, luaStackOffset, zftrue, &param##Cls)) \
    { \
        return ZFImpl_ZFLua_luaError(L, \
            "[LuaMetatable] unknown param type: %s", \
            ZFImpl_ZFLua_luaObjectInfo(L, luaStackOffset, zftrue).cString()); \
    } \
    v_zflongdouble *param = _##param.to<v_zflongdouble *>()

// ============================================================
static int _ZFP_ZFImpl_ZFLua_metatableStoreResult(ZF_IN lua_State *L,
                                                  ZF_IN lua_Number const &n,
                                                  ZF_IN v_zflongdouble *param0,
                                                  ZF_IN const ZFClass *paramClass0,
                                                  ZF_IN v_zflongdouble *param1,
                                                  ZF_IN const ZFClass *paramClass1);

// ============================================================
static int _ZFP_ZFImpl_ZFLua_metatable_add(ZF_IN lua_State *L)
{
    _ZFP_ZFImpl_ZFLua_metatable_PrepareParam(v1, 1);
    _ZFP_ZFImpl_ZFLua_metatable_PrepareParam(v2, 2);
    return _ZFP_ZFImpl_ZFLua_metatableStoreResult(L,
        (lua_Number)(v1->zfv + v2->zfv),
        v1, v1Cls, v2, v2Cls);
}
static int _ZFP_ZFImpl_ZFLua_metatable_sub(ZF_IN lua_State *L)
{
    _ZFP_ZFImpl_ZFLua_metatable_PrepareParam(v1, 1);
    _ZFP_ZFImpl_ZFLua_metatable_PrepareParam(v2, 2);
    return _ZFP_ZFImpl_ZFLua_metatableStoreResult(L,
        (lua_Number)(v1->zfv - v2->zfv),
        v1, v1Cls, v2, v2Cls);
}
static int _ZFP_ZFImpl_ZFLua_metatable_mul(ZF_IN lua_State *L)
{
    _ZFP_ZFImpl_ZFLua_metatable_PrepareParam(v1, 1);
    _ZFP_ZFImpl_ZFLua_metatable_PrepareParam(v2, 2);
    return _ZFP_ZFImpl_ZFLua_metatableStoreResult(L,
        (lua_Number)(v1->zfv * v2->zfv),
        v1, v1Cls, v2, v2Cls);
}
static int _ZFP_ZFImpl_ZFLua_metatable_div(ZF_IN lua_State *L)
{
    _ZFP_ZFImpl_ZFLua_metatable_PrepareParam(v1, 1);
    _ZFP_ZFImpl_ZFLua_metatable_PrepareParam(v2, 2);
    return _ZFP_ZFImpl_ZFLua_metatableStoreResult(L,
        (lua_Number)(v1->zfv / v2->zfv),
        v1, v1Cls, v2, v2Cls);
}
static int _ZFP_ZFImpl_ZFLua_metatable_mod(ZF_IN lua_State *L)
{
    _ZFP_ZFImpl_ZFLua_metatable_PrepareParam(v1, 1);
    _ZFP_ZFImpl_ZFLua_metatable_PrepareParam(v2, 2);
    return _ZFP_ZFImpl_ZFLua_metatableStoreResult(L,
        (lua_Number)((zfint)v1->zfv % (zfint)v2->zfv),
        v1, v1Cls, v2, v2Cls);
}
static int _ZFP_ZFImpl_ZFLua_metatable_unm(ZF_IN lua_State *L)
{
    _ZFP_ZFImpl_ZFLua_metatable_PrepareParam(v1, 1);
    return _ZFP_ZFImpl_ZFLua_metatableStoreResult(L,
        (lua_Number)(-(v1->zfv)),
        v1, v1Cls, zfnull, zfnull);
}
static int _ZFP_ZFImpl_ZFLua_metatable_band(ZF_IN lua_State *L)
{
    _ZFP_ZFImpl_ZFLua_metatable_PrepareParam(v1, 1);
    _ZFP_ZFImpl_ZFLua_metatable_PrepareParam(v2, 2);
    return _ZFP_ZFImpl_ZFLua_metatableStoreResult(L,
        (lua_Number)((zfuint)v1->zfv & (zfuint)v2->zfv),
        v1, v_zfflags::ClassData(), v2, v_zfflags::ClassData());
}
static int _ZFP_ZFImpl_ZFLua_metatable_bor(ZF_IN lua_State *L)
{
    _ZFP_ZFImpl_ZFLua_metatable_PrepareParam(v1, 1);
    _ZFP_ZFImpl_ZFLua_metatable_PrepareParam(v2, 2);
    const ZFClass *cls = v_zfflags::ClassData();
    if(v1Cls != zfnull && v1Cls->classIsTypeOf(ZFEnum::ClassData()))
    {
        cls = v1Cls;
    }
    else if(v2Cls != zfnull && v2Cls->classIsTypeOf(ZFEnum::ClassData()))
    {
        cls = v2Cls;
    }
    return _ZFP_ZFImpl_ZFLua_metatableStoreResult(L,
        (lua_Number)((zfflags)v1->zfv | (zfflags)v2->zfv),
        v1, cls, v2, cls);
}
static int _ZFP_ZFImpl_ZFLua_metatable_bxor(ZF_IN lua_State *L)
{
    _ZFP_ZFImpl_ZFLua_metatable_PrepareParam(v1, 1);
    _ZFP_ZFImpl_ZFLua_metatable_PrepareParam(v2, 2);
    return _ZFP_ZFImpl_ZFLua_metatableStoreResult(L,
        (lua_Number)((zfflags)v1->zfv ^ (zfflags)v2->zfv),
        v1, v_zfflags::ClassData(), v2, v_zfflags::ClassData());
}
static int _ZFP_ZFImpl_ZFLua_metatable_bnot(ZF_IN lua_State *L)
{
    _ZFP_ZFImpl_ZFLua_metatable_PrepareParam(v1, 1);
    return _ZFP_ZFImpl_ZFLua_metatableStoreResult(L,
        (lua_Number)(zfflags)(~((zfflags)v1->zfv)),
        v1, v_zfflags::ClassData(), zfnull, zfnull);
}
static int _ZFP_ZFImpl_ZFLua_metatable_shl(ZF_IN lua_State *L)
{
    _ZFP_ZFImpl_ZFLua_metatable_PrepareParam(v1, 1);
    _ZFP_ZFImpl_ZFLua_metatable_PrepareParam(v2, 2);
    return _ZFP_ZFImpl_ZFLua_metatableStoreResult(L,
        (lua_Number)((zfint)v1->zfv << (zfint)v2->zfv),
        v1, v1Cls, v2, v2Cls);
}
static int _ZFP_ZFImpl_ZFLua_metatable_shr(ZF_IN lua_State *L)
{
    _ZFP_ZFImpl_ZFLua_metatable_PrepareParam(v1, 1);
    _ZFP_ZFImpl_ZFLua_metatable_PrepareParam(v2, 2);
    return _ZFP_ZFImpl_ZFLua_metatableStoreResult(L,
        (lua_Number)((zfint)v1->zfv >> (zfint)v2->zfv),
        v1, v1Cls, v2, v2Cls);
}
static zfbool _ZFP_ZFImpl_ZFLua_metatable_concat_action(ZF_IN_OUT zfstring &v, ZF_OUT zfstring &errorHint, ZF_IN lua_State *L, ZF_IN int luaStackOffset)
{
    if(ZFImpl_ZFLua_toString(v, L, luaStackOffset, zftrue))
    {
        return zftrue;
    }
    if(lua_isuserdata(L, luaStackOffset))
    {
        zfautoObject const &param = ZFImpl_ZFLua_luaGet(L, luaStackOffset);
        ZFTypeIdWrapper *wrapper = param;
        if(wrapper != zfnull)
        {
            if(wrapper->wrappedValueToString(v))
            {
                return zftrue;
            }
        }
        ZFObjectInfoT(v, param);
        return zftrue;
    }
    zfstringAppend(errorHint, "[LuaMetatable] unknown param type: %s",
        ZFImpl_ZFLua_luaObjectInfo(L, luaStackOffset, zftrue).cString());
    return zffalse;
}
static int _ZFP_ZFImpl_ZFLua_metatable_concat(ZF_IN lua_State *L)
{
    zfstring v;
    zfstring errorHint;
    if(_ZFP_ZFImpl_ZFLua_metatable_concat_action(v, errorHint, L, 1)
        && _ZFP_ZFImpl_ZFLua_metatable_concat_action(v, errorHint, L, 2))
    {
        lua_pushstring(L, v.cString());
        return 1;
    }
    else
    {
        return ZFImpl_ZFLua_luaError(L,
            "%s",
            errorHint.cString());
    }
}
static int _ZFP_ZFImpl_ZFLua_metatable_len(ZF_IN lua_State *L)
{
    const zfchar *v = zfnull;
    if(!ZFImpl_ZFLua_toString(v, L, 1, zftrue))
    {
        return ZFImpl_ZFLua_luaError(L,
            "[LuaMetatable] unknown param type: %s",
            ZFImpl_ZFLua_luaObjectInfo(L, 1, zftrue).cString());
    }
    lua_pushinteger(L, (lua_Integer)zfslen(v));
    return 1;
}
static zfbool _ZFP_ZFImpl_ZFLua_metatable_cmp(ZF_OUT ZFCompareResult &ret, ZF_OUT zfstring &errorHint, ZF_IN lua_State *L)
{
    if(lua_isuserdata(L, 1) && lua_isuserdata(L, 2))
    {
        zfautoObject v1;
        if(!ZFImpl_ZFLua_toObject(v1, L, 1))
        {
            zfstringAppend(errorHint, "[LuaMetatable] unknown param type: %s",
                ZFImpl_ZFLua_luaObjectInfo(L, 1, zftrue).cString());
            return zffalse;
        }
        zfautoObject v2;
        if(!ZFImpl_ZFLua_toObject(v2, L, 2))
        {
            zfstringAppend(errorHint, "[LuaMetatable] unknown param type: %s",
                ZFImpl_ZFLua_luaObjectInfo(L, 2, zftrue).cString());
            return zffalse;
        }
        ret = ZFComparerDefault(v1, v2);
        return zftrue;
    }
    else
    {
        if(lua_isnumber(L, 1) || lua_isnumber(L, 2))
        {
            zfautoObject v1;
            zfautoObject v2;
            if(ZFImpl_ZFLua_toNumberT(v1, L, 1)
                && ZFImpl_ZFLua_toNumberT(v2, L, 2))
            {
                ret = ZFComparerDefault(v1, v2);
                return zftrue;
            }
        }
        if(lua_isstring(L, 1) || lua_isstring(L, 2))
        {
            const zfchar *v1 = zfnull;
            const zfchar *v2 = zfnull;
            if(ZFImpl_ZFLua_toString(v1, L, 1, zftrue)
               && ZFImpl_ZFLua_toString(v2, L, 2, zftrue))
            {
                ret = ZFComparerDefault(v1, v2);
                return zftrue;
            }
        }
    }

    zfstringAppend(errorHint, "[LuaMetatable] unknown param type: %s and %s",
        ZFImpl_ZFLua_luaObjectInfo(L, 1, zftrue).cString(),
        ZFImpl_ZFLua_luaObjectInfo(L, 2, zftrue).cString());
    return zffalse;
}
static int _ZFP_ZFImpl_ZFLua_metatable_eq(ZF_IN lua_State *L)
{
    ZFCompareResult result = ZFCompareUncomparable;
    zfstring errorHint;
    if(!_ZFP_ZFImpl_ZFLua_metatable_cmp(result, errorHint, L))
    {
        return ZFImpl_ZFLua_luaError(L,
            "%s",
            errorHint.cString());
    }
    lua_pushboolean(L, (result == ZFCompareTheSame));
    return 1;
}
static int _ZFP_ZFImpl_ZFLua_metatable_lt(ZF_IN lua_State *L)
{
    ZFCompareResult result = ZFCompareUncomparable;
    zfstring errorHint;
    if(!_ZFP_ZFImpl_ZFLua_metatable_cmp(result, errorHint, L))
    {
        return ZFImpl_ZFLua_luaError(L,
            "%s",
            errorHint.cString());
    }
    lua_pushboolean(L, (result == ZFCompareSmaller));
    return 1;
}
static int _ZFP_ZFImpl_ZFLua_metatable_le(ZF_IN lua_State *L)
{
    ZFCompareResult result = ZFCompareUncomparable;
    zfstring errorHint;
    if(!_ZFP_ZFImpl_ZFLua_metatable_cmp(result, errorHint, L))
    {
        return ZFImpl_ZFLua_luaError(L,
            "%s",
            errorHint.cString());
    }
    lua_pushboolean(L, (result == ZFCompareSmaller || result == ZFCompareTheSame));
    return 1;
}
static int _ZFP_ZFImpl_ZFLua_metatable_tostring(ZF_IN lua_State *L)
{
    zfautoObject obj;
    if(!ZFImpl_ZFLua_toObject(obj, L, 1))
    {
        return ZFImpl_ZFLua_luaError(L,
            "[__tostring] unknown object type: %s",
            ZFImpl_ZFLua_luaObjectInfo(L, 1, zftrue).cString());
    }

    zfstring ret;
    ZFObjectInfoT(ret, obj.toObject());
    lua_pushstring(L, ret.cString());
    return 1;
}

// ============================================================
void ZFImpl_ZFLua_implSetupObject_metatable(ZF_IN_OUT lua_State *L, ZF_IN_OPT int objIndex /* = -1 */)
{
    lua_getglobal(L, "_ZFP_ZFImpl_ZFLua_implSetupObject");
    if(!lua_isfunction(L, -1))
    {
        lua_pop(L, 1);

        ZFImpl_ZFLua_luaCFunctionRegister(L, "_ZFP_ZFImpl_ZFLua_metatable_add", _ZFP_ZFImpl_ZFLua_metatable_add);
        ZFImpl_ZFLua_luaCFunctionRegister(L, "_ZFP_ZFImpl_ZFLua_metatable_sub", _ZFP_ZFImpl_ZFLua_metatable_sub);
        ZFImpl_ZFLua_luaCFunctionRegister(L, "_ZFP_ZFImpl_ZFLua_metatable_mul", _ZFP_ZFImpl_ZFLua_metatable_mul);
        ZFImpl_ZFLua_luaCFunctionRegister(L, "_ZFP_ZFImpl_ZFLua_metatable_div", _ZFP_ZFImpl_ZFLua_metatable_div);
        ZFImpl_ZFLua_luaCFunctionRegister(L, "_ZFP_ZFImpl_ZFLua_metatable_mod", _ZFP_ZFImpl_ZFLua_metatable_mod);
        ZFImpl_ZFLua_luaCFunctionRegister(L, "_ZFP_ZFImpl_ZFLua_metatable_unm", _ZFP_ZFImpl_ZFLua_metatable_unm);
        ZFImpl_ZFLua_luaCFunctionRegister(L, "_ZFP_ZFImpl_ZFLua_metatable_band", _ZFP_ZFImpl_ZFLua_metatable_band);
        ZFImpl_ZFLua_luaCFunctionRegister(L, "_ZFP_ZFImpl_ZFLua_metatable_bor", _ZFP_ZFImpl_ZFLua_metatable_bor);
        ZFImpl_ZFLua_luaCFunctionRegister(L, "_ZFP_ZFImpl_ZFLua_metatable_bxor", _ZFP_ZFImpl_ZFLua_metatable_bxor);
        ZFImpl_ZFLua_luaCFunctionRegister(L, "_ZFP_ZFImpl_ZFLua_metatable_bnot", _ZFP_ZFImpl_ZFLua_metatable_bnot);
        ZFImpl_ZFLua_luaCFunctionRegister(L, "_ZFP_ZFImpl_ZFLua_metatable_shl", _ZFP_ZFImpl_ZFLua_metatable_shl);
        ZFImpl_ZFLua_luaCFunctionRegister(L, "_ZFP_ZFImpl_ZFLua_metatable_shr", _ZFP_ZFImpl_ZFLua_metatable_shr);
        ZFImpl_ZFLua_luaCFunctionRegister(L, "_ZFP_ZFImpl_ZFLua_metatable_concat", _ZFP_ZFImpl_ZFLua_metatable_concat);
        ZFImpl_ZFLua_luaCFunctionRegister(L, "_ZFP_ZFImpl_ZFLua_metatable_len", _ZFP_ZFImpl_ZFLua_metatable_len);
        ZFImpl_ZFLua_luaCFunctionRegister(L, "_ZFP_ZFImpl_ZFLua_metatable_eq", _ZFP_ZFImpl_ZFLua_metatable_eq);
        ZFImpl_ZFLua_luaCFunctionRegister(L, "_ZFP_ZFImpl_ZFLua_metatable_lt", _ZFP_ZFImpl_ZFLua_metatable_lt);
        ZFImpl_ZFLua_luaCFunctionRegister(L, "_ZFP_ZFImpl_ZFLua_metatable_le", _ZFP_ZFImpl_ZFLua_metatable_le);
        ZFImpl_ZFLua_luaCFunctionRegister(L, "_ZFP_ZFImpl_ZFLua_metatable_tostring", _ZFP_ZFImpl_ZFLua_metatable_tostring);

        ZFImpl_ZFLua_execute(L,
                "function _ZFP_ZFImpl_ZFLua_implSetupObject_index(obj, k)\n"
                "    return function(obj, ...)\n"
                "        return zfl_call(obj, k, ...)\n"
                "    end\n"
                "end\n"
                "\n"
                "function _ZFP_ZFImpl_ZFLua_implSetupObject(obj)\n"
                "    local tbl = debug.getmetatable(obj)\n"
                "    tbl.__index = _ZFP_ZFImpl_ZFLua_implSetupObject_index\n"
                "    \n"
                "    tbl.__add = _ZFP_ZFImpl_ZFLua_metatable_add\n"
                "    tbl.__sub = _ZFP_ZFImpl_ZFLua_metatable_sub\n"
                "    tbl.__mul = _ZFP_ZFImpl_ZFLua_metatable_mul\n"
                "    tbl.__div = _ZFP_ZFImpl_ZFLua_metatable_div\n"
                "    tbl.__mod = _ZFP_ZFImpl_ZFLua_metatable_mod\n"
                "    tbl.__unm = _ZFP_ZFImpl_ZFLua_metatable_unm\n"
                "    tbl.__band = _ZFP_ZFImpl_ZFLua_metatable_band\n"
                "    tbl.__bor = _ZFP_ZFImpl_ZFLua_metatable_bor\n"
                "    tbl.__bxor = _ZFP_ZFImpl_ZFLua_metatable_bxor\n"
                "    tbl.__bnot = _ZFP_ZFImpl_ZFLua_metatable_bnot\n"
                "    tbl.__shl = _ZFP_ZFImpl_ZFLua_metatable_shl\n"
                "    tbl.__shr = _ZFP_ZFImpl_ZFLua_metatable_shr\n"
                "    tbl.__concat = _ZFP_ZFImpl_ZFLua_metatable_concat\n"
                "    tbl.__len = _ZFP_ZFImpl_ZFLua_metatable_len\n"
                "    tbl.__eq = _ZFP_ZFImpl_ZFLua_metatable_eq\n"
                "    tbl.__lt = _ZFP_ZFImpl_ZFLua_metatable_lt\n"
                "    tbl.__le = _ZFP_ZFImpl_ZFLua_metatable_le\n"
                "    tbl.__tostring = _ZFP_ZFImpl_ZFLua_metatable_tostring\n"
                "    \n"
                "    debug.setmetatable(obj, tbl)\n"
                "end\n"
            );

        lua_getglobal(L, "_ZFP_ZFImpl_ZFLua_implSetupObject");
    }

    lua_pushvalue(L, (objIndex > 0) ? objIndex : (objIndex - 1));

    int error = lua_pcall(L, 1, 0, 0);
    if(error)
    {
        zfstring errorHint = lua_tostring(L, -1);
        lua_pop(L, 1);

        ZFLuaErrorOccurredTrim(
            "[ZFLua] failed to setup object metatable: %s",
            errorHint.cString());
    }
}

// ============================================================
static int _ZFP_ZFImpl_ZFLua_metatableStoreResult(ZF_IN lua_State *L,
                                                  ZF_IN lua_Number const &n,
                                                  ZF_IN v_zflongdouble *param0,
                                                  ZF_IN const ZFClass *paramClass0,
                                                  ZF_IN v_zflongdouble *param1,
                                                  ZF_IN const ZFClass *paramClass1)
{
    if(paramClass0 == zfnull)
    {
        paramClass0 = paramClass1;
    }
    if(paramClass0 == zfnull)
    {
        lua_pushnumber(L, n);
        return 1;
    }
    if(paramClass1 == zfnull)
    {
        paramClass1 = paramClass0;
    }

    if(paramClass0->classIsTypeOf(ZFEnum::ClassData()))
    {
        zfautoObject ret = paramClass0->newInstance();
        ret.toObject()->classData()->propertySetterForName("enumValue")->execute<void, zfuint>(ret, (zfuint)n);
        ZFImpl_ZFLua_luaPush(L, ret);
        return zftrue;
    }
    if(paramClass1->classIsTypeOf(ZFEnum::ClassData()))
    {
        zfautoObject ret = paramClass1->newInstance();
        ret.toObject()->classData()->propertySetterForName("enumValue")->execute<void, zfuint>(ret, (zfuint)n);
        ZFImpl_ZFLua_luaPush(L, ret);
        return zftrue;
    }

    if(zffalse)
    {
    }
    else if(paramClass0->classIsTypeOf(v_zfbool::ClassData()) || paramClass1->classIsTypeOf(v_zfbool::ClassData()))
    {
        zfblockedAllocWithCache(v_zfbool, ret);
        ret->zfv = (zfmAbs(n) <= zffloatEpsilon);
        ZFImpl_ZFLua_luaPush(L, ret);
        return 1;
    }
    else if(paramClass0->classIsTypeOf(v_zfflags::ClassData()) || paramClass1->classIsTypeOf(v_zfflags::ClassData()))
    {
        zfblockedAllocWithCache(v_zfflags, ret);
        ret->zfv = (zfflags)n;
        ZFImpl_ZFLua_luaPush(L, ret);
        return 1;
    }
    else if(paramClass0->classIsTypeOf(v_zfidentity::ClassData()) || paramClass1->classIsTypeOf(v_zfidentity::ClassData()))
    {
        zfblockedAllocWithCache(v_zfidentity, ret);
        ret->zfv = (zfidentity)n;
        ZFImpl_ZFLua_luaPush(L, ret);
        return 1;
    }
    else if(paramClass0->classIsTypeOf(v_zfindex::ClassData()) || paramClass1->classIsTypeOf(v_zfindex::ClassData()))
    {
        zfblockedAllocWithCache(v_zfindex, ret);
        ret->zfv = (zfindex)n;
        ZFImpl_ZFLua_luaPush(L, ret);
        return 1;
    }
    else if(paramClass0->classIsTypeOf(v_zftimet::ClassData()) || paramClass1->classIsTypeOf(v_zftimet::ClassData()))
    {
        zfblockedAllocWithCache(v_zftimet, ret);
        ret->zfv = (zftimet)n;
        ZFImpl_ZFLua_luaPush(L, ret);
        return 1;
    }
    else if(paramClass0->classIsTypeOf(v_zfbyte::ClassData()) || paramClass1->classIsTypeOf(v_zfbyte::ClassData()))
    {
        zfblockedAllocWithCache(v_zfbyte, ret);
        ret->zfv = (zfbyte)n;
        ZFImpl_ZFLua_luaPush(L, ret);
        return 1;
    }

    lua_pushnumber(L, n);
    return 1;
}

ZF_NAMESPACE_GLOBAL_END

