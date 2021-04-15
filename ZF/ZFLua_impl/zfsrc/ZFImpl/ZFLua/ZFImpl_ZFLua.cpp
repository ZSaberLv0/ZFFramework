#include "ZFImpl_ZFLua.h"
#include "ZFImpl_ZFLua_metatable.h"

#include "ZFCore/ZFSTLWrapper/zfstl_string.h"
#include "ZFCore/ZFSTLWrapper/zfstl_vector.h"
#include "ZFCore/ZFSTLWrapper/zfstl_map.h"

#define _ZFP_ZFCallbackForLua_DEBUG ZF_ENV_DEBUG

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFImpl_ZFLua_luaStateHolder, ZFLevelAppEssential)
{
    L = (lua_State *)ZFLuaStateOpen();
    this->builtinLuaEnv = zftrue;
    ZFLuaStateAttach(L);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFImpl_ZFLua_luaStateHolder)
{
    ZFLuaStateDetach(L);
    if(this->builtinLuaEnv)
    {
        ZFLuaStateClose(L);
    }
}
lua_State *L;
zfbool builtinLuaEnv;
ZF_GLOBAL_INITIALIZER_END(ZFImpl_ZFLua_luaStateHolder)
lua_State *_ZFP_ZFImpl_ZFLua_luaState(void)
{
    return ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_ZFLua_luaStateHolder)->L;
}
void ZFImpl_ZFLua_luaStateChange(ZF_IN lua_State *L)
{
    zfCoreMutexLocker();
    ZF_GLOBAL_INITIALIZER_CLASS(ZFImpl_ZFLua_luaStateHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_ZFLua_luaStateHolder);

    zfCoreAssert(L != zfnull);
    if(L == d->L)
    {
        return ;
    }

    ZFLuaStateDetach(d->L);
    if(d->builtinLuaEnv)
    {
        d->builtinLuaEnv = zffalse;
        ZFLuaStateClose(d->L);
    }
    d->L = L;
    ZFLuaStateAttach(d->L);
}

void *ZFImpl_ZFLua_luaStateOpen(void)
{
    return ZFImpl_ZFLua_luaOpen();
}
void ZFImpl_ZFLua_luaStateClose(ZF_IN lua_State *L)
{
    ZFImpl_ZFLua_luaClose(L);
}

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFImpl_ZFLua_luaStateGlobalHolder, ZFLevelZFFrameworkLow)
{
}
zfstlmap<lua_State *, zfbool> attachedState;
ZFCoreArrayPOD<lua_State *> attachedStateList;
ZFCoreArrayPOD<_ZFP_ZFImpl_ZFLua_ImplSetupCallback> setupAttach;
ZFCoreArrayPOD<_ZFP_ZFImpl_ZFLua_ImplSetupCallback> setupDetach;
ZF_GLOBAL_INITIALIZER_END(ZFImpl_ZFLua_luaStateGlobalHolder)

void ZFImpl_ZFLua_luaStateAttach(ZF_IN lua_State *L)
{
    zfCoreAssert(L != zfnull);
    zfCoreMutexLocker();

    ZF_GLOBAL_INITIALIZER_CLASS(ZFImpl_ZFLua_luaStateGlobalHolder) *d
        = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_ZFLua_luaStateGlobalHolder);

    zfCoreAssertWithMessageTrim(d->attachedState.find(L) == d->attachedState.end(),
        "[ZFImpl_ZFLua_luaStateAttach] lua state %p already attached",
        L);
    d->attachedState[L] = zftrue;
    d->attachedStateList.add(L);

    ZFImpl_ZFLua_luaClassRegister<zfautoObject>(L, "zfautoObject");

    // zfnull
    zfclassNotPOD _ZFP_ZFImpl_ZFLua_zfnullHolder
    {
    public:
        static zfautoObject get_zfnull(void)
        {
            return zfnull;
        }
    };
    ZFImpl_ZFLua_luaFunctionRegister(L, "_ZFP_ZFImpl_ZFLua_zfnull", _ZFP_ZFImpl_ZFLua_zfnullHolder::get_zfnull);
    ZFImpl_ZFLua_execute(L,
            "zfnull = _ZFP_ZFImpl_ZFLua_zfnull()\n"
        );

    // zftrue, zffalse
    zfclassNotPOD _ZFP_ZFImpl_ZFLua_zfboolHolder
    {
    public:
        static zfautoObject get_zftrue(void)
        {
            return v_zftrue;
        }
        static zfautoObject get_zffalse(void)
        {
            return v_zffalse;
        }
    };
    ZFImpl_ZFLua_luaFunctionRegister(L, "_ZFP_ZFImpl_ZFLua_zftrue", _ZFP_ZFImpl_ZFLua_zfboolHolder::get_zftrue);
    ZFImpl_ZFLua_luaFunctionRegister(L, "_ZFP_ZFImpl_ZFLua_zffalse", _ZFP_ZFImpl_ZFLua_zfboolHolder::get_zffalse);
    ZFImpl_ZFLua_execute(L,
            "zftrue = _ZFP_ZFImpl_ZFLua_zftrue()\n"
            "zffalse = _ZFP_ZFImpl_ZFLua_zffalse()\n"
        );

    // zfl_L
    zfclassNotPOD _ZFP_ZFImpl_ZFLua_LHolder
    {
    public:
        static int f(ZF_IN lua_State *L)
        {
            zfblockedAllocWithCache(v_ZFPtr, ret);
            ret->zfv = (void *)L;
            zfautoObject tmp = ret;
            ZFImpl_ZFLua_luaPush(L, tmp);
            return 1;
        }
    };
    ZFImpl_ZFLua_luaCFunctionRegister(L, "zfl_L", _ZFP_ZFImpl_ZFLua_LHolder::f);

    // zfl_call metatable
    ZFImpl_ZFLua_execute(L,
            "_ZFP_zfl_index = function(tbl, k)\n"
            "    local t = {ZFNS=tbl.ZFNS .. '.' .. k}\n" // ZFNamespaceSeparator
            "    local d = {}\n"
            "    d.__index = _ZFP_zfl_index\n"
            "    d.__call = _ZFP_zfl_call\n"
            "    debug.setmetatable(t, d)\n"
            "    return t\n"
            "end\n"
            "_ZFP_zfl_call = function(tbl, ...)\n"
            "    return zfl_callStatic(tbl.ZFNS, ...)\n"
            "end\n"
        );

    // each impl setup callback
    for(zfindex i = 0; i < d->setupAttach.count(); ++i)
    {
        d->setupAttach[i](L);
    }
}
void ZFImpl_ZFLua_luaStateDetach(ZF_IN lua_State *L)
{
    zfCoreMutexLocker();

    ZF_GLOBAL_INITIALIZER_CLASS(ZFImpl_ZFLua_luaStateGlobalHolder) *d
        = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_ZFLua_luaStateGlobalHolder);

    zfCoreAssertWithMessageTrim(d->attachedState.find(L) != d->attachedState.end(),
        "[ZFImpl_ZFLua_luaStateDetach] lua state %p not attached",
        L);

    // each impl setup callback
    for(zfindex i = 0; i < d->setupDetach.count(); ++i)
    {
        d->setupDetach[i](L);
    }

    d->attachedState.erase(L);
    d->attachedStateList.removeElement(L);
}
void ZFImpl_ZFLua_luaStateListT(ZF_IN_OUT ZFCoreArray<lua_State *> &ret)
{
    ret.addFrom(ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_ZFLua_luaStateGlobalHolder)->attachedStateList);
}
const ZFCoreArrayPOD<lua_State *> &ZFImpl_ZFLua_luaStateList(void)
{
    return ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_ZFLua_luaStateGlobalHolder)->attachedStateList;
}

// ============================================================
void _ZFP_ZFImpl_ZFLua_implSetupCallbackRegister(ZF_IN _ZFP_ZFImpl_ZFLua_ImplSetupCallback setupAttachCallback,
                                                 ZF_IN _ZFP_ZFImpl_ZFLua_ImplSetupCallback setupDetachCallback)
{
    zfCoreMutexLocker();

    ZF_GLOBAL_INITIALIZER_CLASS(ZFImpl_ZFLua_luaStateGlobalHolder) *d
        = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_ZFLua_luaStateGlobalHolder);
    d->setupAttach.add(setupAttachCallback);
    d->setupDetach.add(setupDetachCallback);
}
void _ZFP_ZFImpl_ZFLua_implSetupCallbackUnregister(ZF_IN _ZFP_ZFImpl_ZFLua_ImplSetupCallback setupAttachCallback,
                                                   ZF_IN _ZFP_ZFImpl_ZFLua_ImplSetupCallback setupDetachCallback)
{
    zfCoreMutexLocker();

    ZF_GLOBAL_INITIALIZER_CLASS(ZFImpl_ZFLua_luaStateGlobalHolder) *d
        = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_ZFLua_luaStateGlobalHolder);
    d->setupAttach.removeElement(setupAttachCallback);
    d->setupDetach.removeElement(setupDetachCallback);
}

// ============================================================
void ZFImpl_ZFLua_implSetupScope(ZF_IN_OUT lua_State *L, ZF_IN const zfchar *scopeName)
{
    const zfchar *scopeNameList[2] = {scopeName, zfnull};
    ZFImpl_ZFLua_implSetupScope(L, scopeNameList);
}
static void _ZFP_ZFImpl_ZFLua_implSetupScope(ZF_IN_OUT zfstring &code,
                                             ZF_IN const zfchar *scopeName)
{
    zfstringAppend(code,
            "%s = {ZFNS='%s'}\n"
            "local tbl = {}\n"
            "tbl.__index = _ZFP_zfl_index\n"
            "tbl.__call = _ZFP_zfl_call\n"
            "debug.setmetatable(%s, tbl)\n"
        , scopeName, scopeName, scopeName);
}
void ZFImpl_ZFLua_implSetupScope(ZF_IN_OUT lua_State *L, ZF_IN const zfchar **scopeNameList)
{
    zfstring code;
    for(zfindex i = 0; *scopeNameList != zfnull; ++scopeNameList, ++i)
    {
        _ZFP_ZFImpl_ZFLua_implSetupScope(code, *scopeNameList);
        if(zfsncmp(*scopeNameList, ZFImpl_ZFLua_PropTypePrefix, ZFImpl_ZFLua_PropTypePrefixLen) == 0
            && !zfsIsEmpty(*scopeNameList + ZFImpl_ZFLua_PropTypePrefixLen))
        {
            _ZFP_ZFImpl_ZFLua_implSetupScope(code, *scopeNameList + ZFImpl_ZFLua_PropTypePrefixLen);
            ++i;
        }
        if(i >= 100)
        {
            ZFImpl_ZFLua_execute(L, code);
            code.removeAll();
            i = 0;
        }
    }
    if(!code.isEmpty())
    {
        ZFImpl_ZFLua_execute(L, code);
    }
}
void ZFImpl_ZFLua_implSetupObject(ZF_IN_OUT lua_State *L, ZF_IN_OPT int objIndex /* = -1 */)
{
    ZFImpl_ZFLua_implSetupObject_metatable(L, objIndex);
}

// ============================================================
zfbool ZFImpl_ZFLua_execute(ZF_IN lua_State *L,
                            ZF_IN const zfchar *buf,
                            ZF_IN_OPT zfindex bufLen /* = zfindexMax() */,
                            ZF_OUT_OPT zfautoObject *luaResult /* = zfnull */,
                            ZF_IN_OPT const ZFCoreArray<zfautoObject> *luaParams /* = zfnull */,
                            ZF_OUT_OPT zfstring *errHint /* = zfnull */,
                            ZF_IN_OPT const zfchar *chunkInfo /* = zfnull */)
{
    #if ZF_ENV_DEBUG
    int DEBUG_luaStackNum = lua_gettop(L);
    #endif

    ZF_GLOBAL_INITIALIZER_CLASS(ZFImpl_ZFLua_luaStateGlobalHolder) *d
        = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_ZFLua_luaStateGlobalHolder);
    if(d->attachedState.find(L) == d->attachedState.end())
    {
        if(errHint != zfnull)
        {
            zfstringAppend(*errHint, "lua_State %p not attached", L);
        }
        return zffalse;
    }

    int luaStackNum = lua_gettop(L);
    int error = luaL_loadbuffer(L, buf, (bufLen == zfindexMax()) ? zfslen(buf) : bufLen, zfnull);
    if(error == 0)
    {
        if(luaParams != zfnull && !luaParams->isEmpty())
        {
            for(zfindex i = 0; i < luaParams->count(); ++i)
            {
                ZFImpl_ZFLua_luaPush(L, luaParams->get(i));
            }
        }
        error = lua_pcall(
            L,
            (luaParams != zfnull ? (int)luaParams->count() : 0),
            LUA_MULTRET,
            0);
    }

    if(error != 0)
    {
        // [string "?"]:123: xxx
        const char *nativeError = lua_tostring(L, -1);
        zfindex tokenL = zfindexMax();
        zfindex tokenR = zfindexMax();
        tokenL = zfstringFind((const zfchar *)nativeError, zfindexMax(), "]:");
        if(tokenL != zfindexMax())
        {
            tokenL += 2;
            tokenR = zfstringFind((const zfchar *)nativeError + tokenL, zfindexMax(), ":");
        }

        // detect error line
        zfindex errorLine = zfindexMax();
        if(tokenL != zfindexMax() && tokenR != zfindexMax())
        {
            zfindexFromString(errorLine, (const zfchar *)nativeError + tokenL, tokenR);
            nativeError = nativeError + tokenL + tokenR + 1;
            while(*nativeError == ' ') {++nativeError;}
        }

        zfstring errHintTmp;
        errHintTmp += nativeError;
        if(!zfsIsEmpty(chunkInfo))
        {
            errHintTmp += ", at: [";
            errHintTmp += chunkInfo;
            errHintTmp += "]";
        }
        if(errorLine != zfindexMax())
        {
            errHintTmp += ", line: ";
            zfindexToString(errHintTmp, errorLine);
        }
        if(errHint != zfnull)
        {
            *errHint += errHintTmp;
        }
        ZFLuaErrorOccurredTrim("%s", errHintTmp.cString());

#if !ZF_ENV_ZFLUA_USE_EXCEPTION
        zfCoreCriticalMessageTrim(
                "| [ZFLua]\n"
                "|     native lua error occurred with no exception support\n"
                "|     (which would cause unrecoverable C++ memory leak or logic error)\n"
                "|     to enable exception support\n"
                "|     add ZF_ENV_ZFLUA_USE_EXCEPTION to your compiler"
            );
#endif

        lua_pop(L, 1);
        return zffalse;
    }

    int luaResultNum = lua_gettop(L) - luaStackNum;
    if(luaResult != zfnull)
    {
        if(luaResultNum == 0)
        {
            *luaResult = zfnull;
        }
        else if(luaResultNum == 1)
        {
            ZFImpl_ZFLua_toGeneric(*luaResult, L, -1);
        }
        else
        {
            zfblockedAlloc(v_ZFCoreArray, ret);
            *luaResult = ret;
            ret->zfv.capacity((zfindex)luaResultNum);
            for(int i = 0; i < luaResultNum; ++i)
            {
                zfautoObject tmp;
                ZFImpl_ZFLua_toGeneric(tmp, L, -1 - (luaResultNum - 1 - i));
                ret->zfv.add(tmp);
            }
        }
    }
    if(luaResultNum > 0)
    {
        lua_pop(L, luaResultNum);
    }

    #if ZF_ENV_DEBUG
    if(DEBUG_luaStackNum != lua_gettop(L))
    {
        zfCoreCriticalMessage("[ZFLua] stack messed up after execute: %d => %d",
            (zfint)DEBUG_luaStackNum, (zfint)lua_gettop(L));
    }
    #endif

    return zftrue;
}

// ============================================================
// utils
void ZFImpl_ZFLua_luaObjectInfoT(ZF_OUT zfstring &ret,
                                 ZF_IN lua_State *L,
                                 ZF_IN int luaStackOffset,
                                 ZF_IN_OPT zfbool printLuaType /* = zffalse */)
{
    if(printLuaType)
    {
        ret += "(";
        ret += luaL_typename(L, luaStackOffset);
        ret += ")";
    }

    switch(lua_type(L, luaStackOffset))
    {
        case LUA_TNIL:
            ret += "<nil>";
            break;
        case LUA_TBOOLEAN:
            zfboolToString(ret, (lua_toboolean(L, luaStackOffset) != 0));
            break;
        case LUA_TLIGHTUSERDATA:
        case LUA_TUSERDATA:
            {
                zfautoObject obj;
                if(ZFImpl_ZFLua_toObject(obj, L, luaStackOffset))
                {
                    ZFObjectInfoT(ret, obj);
                }
                else
                {
                    zfsFromPointerT(ret, lua_topointer(L, luaStackOffset));
                }
            }
            break;
        case LUA_TNUMBER:
            zfdoubleToString(ret, (zfdouble)lua_tonumber(L, luaStackOffset));
            break;
        case LUA_TSTRING:
            ret += lua_tostring(L, luaStackOffset);
            break;
        case LUA_TTABLE:
            lua_getglobal(L, "zfl_tableInfo");
            if(lua_isnil(L, -1))
            {
                lua_pop(L, 1);
                zfsFromPointerT(ret, lua_topointer(L, luaStackOffset));
            }
            else
            {
                if(luaStackOffset < 0)
                {
                    lua_pushvalue(L, luaStackOffset - 1);
                }
                else
                {
                    lua_pushvalue(L, luaStackOffset);
                }
                lua_call(L, 1, 1);
                ret += lua_tostring(L, -1);
                lua_pop(L, 1);
            }
            break;
        case LUA_TFUNCTION:
            zfsFromPointerT(ret, lua_topointer(L, luaStackOffset));
            break;
        case LUA_TTHREAD:
            zfsFromPointerT(ret, lua_topointer(L, luaStackOffset));
            break;
        default:
            ret += "<Unknown>";
            break;
    }
}
zfbool ZFImpl_ZFLua_toObject(ZF_OUT zfautoObject &param,
                             ZF_IN lua_State *L,
                             ZF_IN int luaStackOffset)
{
    if(lua_isuserdata(L, luaStackOffset))
    {
        param = ZFImpl_ZFLua_luaGet(L, luaStackOffset);
        return zftrue;
    }
    else
    {
        return zffalse;
    }
}

zfbool ZFImpl_ZFLua_toGeneric(ZF_OUT zfautoObject &param,
                              ZF_IN lua_State *L,
                              ZF_IN int luaStackOffset)
{
    if(ZFImpl_ZFLua_toObject(param, L, luaStackOffset))
    {
        return zftrue;
    }
    else if(ZFImpl_ZFLua_toCallback(param, L, luaStackOffset))
    {
        return zftrue;
    }

    zfblockedAllocWithCache(ZFDI_WrapperRaw, wrapper);
    if(lua_isstring(L, luaStackOffset))
    {
        wrapper->zfv(lua_tostring(L, luaStackOffset));
        param = wrapper;
        return zftrue;
    }
    if(lua_isboolean(L, luaStackOffset))
    {
        wrapper->zfv((zfbool)lua_toboolean(L, luaStackOffset) ? ZFTOKEN_zfbool_zftrue : ZFTOKEN_zfbool_zffalse);
        param = wrapper;
        return zftrue;
    }
    if(!lua_isuserdata(L, luaStackOffset))
    {
        return zffalse;
    }

    zfautoObject const &obj = ZFImpl_ZFLua_luaGet(L, luaStackOffset);
    if(obj == zfnull)
    {
        wrapper->zfv("");
        return zftrue;
    }
    const ZFClass *cls = obj->classData();
    if(cls->classIsTypeOf(v_zfstring::ClassData()))
    {
        wrapper->zfv(obj->to<v_zfstring *>()->zfv);
        return zftrue;
    }
    else if(cls->classIsTypeOf(ZFDI_WrapperBase::ClassData()))
    {
        wrapper->zfv(obj->to<ZFDI_WrapperBase *>()->zfv());
        return zftrue;
    }
    else
    {
        return zffalse;
    }
}

zfclassFwd _ZFP_I_ZFImpl_ZFLua_ZFCallbackForLuaHolder;
static void _ZFP_ZFImpl_ZFLua_ZFCallbackAutoClean_callback(ZF_IN const ZFListenerData &listenerData, ZF_IN ZFObject *userData);
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFImpl_ZFLua_ZFCallbackAutoClean, ZFLevelZFFrameworkNormal)
{
    this->luaStateOnDetachListener = ZFCallbackForFunc(_ZFP_ZFImpl_ZFLua_ZFCallbackAutoClean_callback);
    ZFGlobalObserver().observerAdd(ZFGlobalEvent::EventLuaStateOnDetach(), this->luaStateOnDetachListener);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFImpl_ZFLua_ZFCallbackAutoClean)
{
    ZFGlobalObserver().observerRemove(ZFGlobalEvent::EventLuaStateOnDetach(), this->luaStateOnDetachListener);
}
ZFListener luaStateOnDetachListener;
ZFCoreArrayPOD<_ZFP_I_ZFImpl_ZFLua_ZFCallbackForLuaHolder *> attachList;
ZF_GLOBAL_INITIALIZER_END(ZFImpl_ZFLua_ZFCallbackAutoClean)

zfclass _ZFP_I_ZFImpl_ZFLua_ZFCallbackForLuaHolder : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_I_ZFImpl_ZFLua_ZFCallbackForLuaHolder, ZFObject)
public:
    lua_State *L;
    int luaFunc;
#if _ZFP_ZFCallbackForLua_DEBUG
    zfstring luaFuncInfo;
#endif

    ZFMETHOD_INLINE_2(void, callback,
                      ZFMP_IN(const ZFListenerData &, listenerData),
                      ZFMP_IN(ZFObject *, userData))
    {
        if(L == zfnull)
        {
            return;
        }
        lua_rawgeti(L, LUA_REGISTRYINDEX, luaFunc);
        if(lua_isfunction(L, -1))
        {
            zfblockedAllocWithCache(v_ZFListenerData, listenerDataTmp);
            listenerDataTmp->zfv = listenerData;
            ZFImpl_ZFLua_luaPush(L, listenerDataTmp);

            ZFImpl_ZFLua_luaPush(L, userData);

            lua_pcall(L, 2, 0, 0);
        }
    }
protected:
    zfoverride
    virtual void objectOnInitFinish(void)
    {
        zfsuper::objectOnInitFinish();
        ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_ZFLua_ZFCallbackAutoClean)->attachList.add(this);
        this->L = zfnull;
    }
    zfoverride
    virtual void objectOnDeallocPrepare(void)
    {
        if(L != zfnull)
        {
            ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_ZFLua_ZFCallbackAutoClean)->attachList.removeElement(this);
            luaL_unref(L, LUA_REGISTRYINDEX, luaFunc);
            L = zfnull;
        }
        zfsuper::objectOnDeallocPrepare();
    }
};

static void _ZFP_ZFImpl_ZFLua_ZFCallbackAutoClean_callback(ZF_IN const ZFListenerData &listenerData, ZF_IN ZFObject *userData)
{
    lua_State *L = (lua_State *)listenerData.param0<v_ZFPtr *>()->zfv;
    ZFCoreArrayPOD<_ZFP_I_ZFImpl_ZFLua_ZFCallbackForLuaHolder *> &attachList = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_ZFLua_ZFCallbackAutoClean)->attachList;
    for(zfindex i = attachList.count() - 1; i != zfindexMax(); --i)
    {
        _ZFP_I_ZFImpl_ZFLua_ZFCallbackForLuaHolder *p = attachList[i];
        if(p->L == L)
        {
            attachList.remove(i);
            luaL_unref(p->L, LUA_REGISTRYINDEX, p->luaFunc);
            p->L = zfnull;
        }
    }
}

zfbool ZFImpl_ZFLua_toCallback(ZF_OUT zfautoObject &param,
                               ZF_IN lua_State *L,
                               ZF_IN int luaStackOffset)
{
    if(ZFImpl_ZFLua_toObject(param, L, luaStackOffset))
    {
        v_ZFCallback *callbackTmp = param;
        if(callbackTmp != zfnull)
        {
            return zftrue;
        }
        else
        {
            return zffalse;
        }
    }

    if(lua_isfunction(L, luaStackOffset))
    {
        zfblockedAlloc(v_ZFCallback, ret);
        zfblockedAlloc(_ZFP_I_ZFImpl_ZFLua_ZFCallbackForLuaHolder, holder);
        holder->L = L;
        lua_pushvalue(L, luaStackOffset);
        holder->luaFunc = luaL_ref(L, LUA_REGISTRYINDEX);
        ret->zfv = ZFCallbackForMemberMethod(holder, ZFMethodAccess(_ZFP_I_ZFImpl_ZFLua_ZFCallbackForLuaHolder, callback));
        ret->zfv.callbackOwnerObjectRetain();
        param = ret;

        #if _ZFP_ZFCallbackForLua_DEBUG
        {
            const zfchar *buf =
                    "local arg={...}\n"
                    "local info=debug.getinfo(arg[1])\n"
                    "return info['source'] .. ':' .. info['linedefined']\n"
                ;
            int error = luaL_loadbuffer(L, buf, zfslen(buf), "[ZFLuaDebug]");
            if(error == 0)
            {
                lua_pushvalue(L, luaStackOffset);
                error = lua_pcall(L, 1, 1, 0);

                if(error == 0 && ZFImpl_ZFLua_toString(holder->luaFuncInfo, L, -1, zftrue))
                {
                    lua_pop(L, 1);
                }
            }

            if(error != 0)
            {
                zfstring errHintTmp;
                const char *nativeError = lua_tostring(L, -1);
                zfCoreCriticalMessageTrim("[ZFLuaDebug] %s", nativeError);
                lua_pop(L, 1);
            }
        }
        #endif
        return zftrue;
    }
    else
    {
        return zffalse;
    }
}

zfbool ZFImpl_ZFLua_toString(ZF_IN_OUT zfstring &s,
                             ZF_IN lua_State *L,
                             ZF_IN int luaStackOffset,
                             ZF_IN_OPT zfbool allowEmpty /* = zffalse */,
                             ZF_OUT_OPT const ZFClass **holderCls /* = zfnull */)
{
    const zfchar *t = zfnull;
    if(ZFImpl_ZFLua_toString(t, L, luaStackOffset, allowEmpty,  holderCls))
    {
        s += t;
        return zftrue;
    }
    else
    {
        return zffalse;
    }
}
zfbool ZFImpl_ZFLua_toString(ZF_IN_OUT zfstring &s,
                             ZF_IN ZFObject *obj,
                             ZF_IN_OPT zfbool allowEmpty /* = zffalse */,
                             ZF_OUT_OPT const ZFClass **holderCls /* = zfnull */)
{
    const zfchar *t = zfnull;
    if(ZFImpl_ZFLua_toString(t, obj, allowEmpty,  holderCls))
    {
        s += t;
        return zftrue;
    }
    else
    {
        return zffalse;
    }
}
zfbool ZFImpl_ZFLua_toString(ZF_IN_OUT const zfchar *&s,
                             ZF_IN lua_State *L,
                             ZF_IN int luaStackOffset,
                             ZF_IN_OPT zfbool allowEmpty /* = zffalse */,
                             ZF_OUT_OPT const ZFClass **holderCls /* = zfnull */)
{
    if(holderCls != zfnull) {*holderCls = zfnull;}
    if(lua_isstring(L, luaStackOffset))
    {
        s = lua_tostring(L, luaStackOffset);
        return zftrue;
    }
    if(lua_isboolean(L, luaStackOffset))
    {
        if((zfbool)lua_toboolean(L, luaStackOffset))
        {
            s = ZFTOKEN_zfbool_zftrue;
        }
        else
        {
            s = ZFTOKEN_zfbool_zffalse;
        }
        return zftrue;
    }
    if(!lua_isuserdata(L, luaStackOffset))
    {
        return zffalse;
    }

    zfautoObject const &param = ZFImpl_ZFLua_luaGet(L, luaStackOffset);
    return ZFImpl_ZFLua_toString(s, param.toObject(), allowEmpty, holderCls);
}
zfbool ZFImpl_ZFLua_toString(ZF_IN_OUT const zfchar *&s,
                             ZF_IN ZFObject *obj,
                             ZF_IN_OPT zfbool allowEmpty /* = zffalse */,
                             ZF_OUT_OPT const ZFClass **holderCls /* = zfnull */)
{
    if(holderCls != zfnull) {*holderCls = zfnull;}
    if(obj == zfnull)
    {
        s = "";
        return allowEmpty;
    }

    const ZFClass *cls = obj->classData();
    if(cls->classIsTypeOf(v_zfstring::ClassData()))
    {
        if(holderCls != zfnull) {*holderCls = v_zfstring::ClassData();}
        s = obj->to<v_zfstring *>()->zfv;
        return zftrue;
    }
    else if(cls->classIsTypeOf(ZFDI_WrapperBase::ClassData()))
    {
        if(holderCls != zfnull) {*holderCls = ZFDI_WrapperBase::ClassData();}
        s = obj->to<ZFDI_WrapperBase *>()->zfv();
        return zftrue;
    }
    else
    {
        return zffalse;
    }
}

zfbool ZFImpl_ZFLua_toNumberT(ZF_OUT zfautoObject &ret,
                              ZF_IN lua_State *L,
                              ZF_IN int luaStackOffset,
                              ZF_IN_OPT zfbool allowEmpty /* = zffalse */,
                              ZF_OUT_OPT const ZFClass **holderCls /* = zfnull */)
{
    if(holderCls != zfnull) {*holderCls = zfnull;}
    int success = 0;
    lua_Number num = lua_tonumberx(L, luaStackOffset, &success);
    if(success)
    {
        ret = zflineAlloc(v_zflongdouble, (zflongdouble)num);
        return zftrue;
    }
    if(!lua_isuserdata(L, luaStackOffset))
    {
        return zffalse;
    }

    zfautoObject const &param = ZFImpl_ZFLua_luaGet(L, luaStackOffset);
    return ZFImpl_ZFLua_toNumberT(ret, param.toObject(), allowEmpty, holderCls);
}

zfbool ZFImpl_ZFLua_toNumberT(ZF_OUT zfautoObject &ret,
                              ZF_IN ZFObject *obj,
                              ZF_IN_OPT zfbool allowEmpty /* = zffalse */,
                              ZF_OUT_OPT const ZFClass **holderCls /* = zfnull */)
{
    if(holderCls != zfnull) {*holderCls = zfnull;}
    if(obj == zfnull)
    {
        if(allowEmpty)
        {
            ret = zflineAlloc(v_zflongdouble, 0);
            return zftrue;
        }
        else
        {
            return zffalse;
        }
    }

    const ZFClass *cls = obj->classData();
    if(holderCls != zfnull) {*holderCls = cls;}
    if(cls->classIsTypeOf(v_zfbool::ClassData()))
    {
        ret = zflineAlloc(v_zflongdouble, (zft_zflongdouble)(ZFCastZFObjectUnchecked(v_zfbool *, obj)->zfv ? 1 : 0));
        return zftrue;
    }
    else if(cls->classIsTypeOf(v_zfindex::ClassData()))
    {
        ret = zflineAlloc(v_zflongdouble, (zft_zflongdouble)(ZFCastZFObjectUnchecked(v_zfindex *, obj)->zfv));
        return zftrue;
    }
    else if(cls->classIsTypeOf(v_zfint::ClassData()))
    {
        ret = zflineAlloc(v_zflongdouble, (zft_zflongdouble)(ZFCastZFObjectUnchecked(v_zfint *, obj)->zfv));
        return zftrue;
    }
    else if(cls->classIsTypeOf(v_zfuint::ClassData()))
    {
        ret = zflineAlloc(v_zflongdouble, (zft_zflongdouble)(ZFCastZFObjectUnchecked(v_zfuint *, obj)->zfv));
        return zftrue;
    }
    else if(cls->classIsTypeOf(v_zffloat::ClassData()))
    {
        ret = zflineAlloc(v_zflongdouble, (zft_zflongdouble)(ZFCastZFObjectUnchecked(v_zffloat *, obj)->zfv));
        return zftrue;
    }
    else if(cls->classIsTypeOf(v_zfdouble::ClassData()))
    {
        ret = zflineAlloc(v_zflongdouble, (zft_zflongdouble)(ZFCastZFObjectUnchecked(v_zfdouble *, obj)->zfv));
        return zftrue;
    }
    else if(cls->classIsTypeOf(v_zflongdouble::ClassData()))
    {
        ret = obj;
        return zftrue;
    }
    else if(cls->classIsTypeOf(v_zfbyte::ClassData()))
    {
        ret = zflineAlloc(v_zflongdouble, (zft_zflongdouble)(ZFCastZFObjectUnchecked(v_zfbyte *, obj)->zfv));
        return zftrue;
    }
    else if(cls->classIsTypeOf(v_zftimet::ClassData()))
    {
        ret = zflineAlloc(v_zflongdouble, (zft_zflongdouble)(ZFCastZFObjectUnchecked(v_zftimet *, obj)->zfv));
        return zftrue;
    }
    else if(cls->classIsTypeOf(v_zfflags::ClassData()))
    {
        ret = zflineAlloc(v_zflongdouble, (zft_zflongdouble)(ZFCastZFObjectUnchecked(v_zfflags *, obj)->zfv));
        return zftrue;
    }
    else if(cls->classIsTypeOf(v_zfidentity::ClassData()))
    {
        ret = zflineAlloc(v_zflongdouble, (zft_zflongdouble)(ZFCastZFObjectUnchecked(v_zfidentity *, obj)->zfv));
        return zftrue;
    }
    else if(cls->classIsTypeOf(ZFEnum::ClassData()))
    {
        ret = zflineAlloc(v_zflongdouble, (zft_zflongdouble)(ZFCastZFObjectUnchecked(ZFEnum *, obj)->enumValue()));
        return zftrue;
    }
    else
    {
        return zffalse;
    }
}

zfbool ZFImpl_ZFLua_toLuaValue(ZF_IN lua_State *L,
                               ZF_IN ZFObject *obj,
                               ZF_IN_OPT zfbool allowEmpty /* = zffalse */)
{
    if(obj == zfnull)
    {
        if(allowEmpty)
        {
            lua_pushnumber(L, 0);
            return zftrue;
        }
        else
        {
            return zffalse;
        }
    }

    if(obj->classData()->classIsTypeOf(v_zfbool::ClassData()))
    {
        lua_pushboolean(L, ZFCastZFObjectUnchecked(v_zfbool *, obj)->zfv);
        return zftrue;
    }
    zfautoObject t;
    if(ZFImpl_ZFLua_toNumberT(t, obj, allowEmpty))
    {
        lua_pushnumber(L, (lua_Number)t.to<v_zflongdouble *>()->zfv);
        return zftrue;
    }

    const zfchar *s = zfnull;
    if(ZFImpl_ZFLua_toString(s, obj, allowEmpty))
    {
        lua_pushstring(L, s);
        return zftrue;
    }

    ZFImpl_ZFLua_luaError(L,
        "[zfl_luaValue] unknown param type, got %s",
        obj->objectInfo().cString());
    return zffalse;
}

// ============================================================
// ZFImpl_ZFLua_zfstringAppend
static zfbool _ZFP_ZFImpl_ZFLua_zfstringAppend_bool(ZF_IN lua_State *L, ZF_IN int i)
{
    zfbool ret = zftrue;
    zfautoObject t;
    if(ZFImpl_ZFLua_toObject(t, L, i))
    {
        if(t == zfnull)
        {
            ret = zffalse;
        }
        else if(t->classData()->classIsTypeOf(v_zfbool::ClassData()))
        {
            ret = t->to<v_zfbool *>()->zfv;
        }
        else if(ZFImpl_ZFLua_toNumberT(t, L, i))
        {
            ret = t->to<v_zflongdouble *>()->zfv;
        }
    }
    else if(lua_isboolean(L, i))
    {
        ret = (zfbool)lua_toboolean(L, i);
    }
    else if(lua_isnoneornil(L, i))
    {
        ret = zffalse;
    }
    else if(lua_isnumber(L, i))
    {
        ret = (lua_tonumber(L, i) != 0);
    }
    return ret;
}
static zflongdouble _ZFP_ZFImpl_ZFLua_zfstringAppend_number(ZF_IN lua_State *L, ZF_IN int i)
{
    zfautoObject t;
    if(ZFImpl_ZFLua_toNumberT(t, L, i, zftrue))
    {
        return t->to<v_zflongdouble *>()->zfv;
    }
    else
    {
        return 0;
    }
}
static zfstring _ZFP_ZFImpl_ZFLua_zfstringAppend_pointer(ZF_IN lua_State *L, ZF_IN int i)
{
    zfstring ret;
    zfautoObject t;
    if(ZFImpl_ZFLua_toObject(t, L, i))
    {
        zfstringAppend(ret, "%p", t.toObject());
    }
    else
    {
        ZFImpl_ZFLua_luaObjectInfoT(ret, L, i);
    }
    return ret;
}
static zfstring _ZFP_ZFImpl_ZFLua_zfstringAppend_string(ZF_IN lua_State *L, ZF_IN int i, ZF_IN zfbool toString)
{
    zfstring ret;
    zfautoObject t;
    if(ZFImpl_ZFLua_toObject(t, L, i))
    {
        ZFObjectInfoT(ret, t.toObject());
    }
    else if(ZFImpl_ZFLua_toString(ret, L, i))
    {
        // nothing to do
    }
    else if(toString)
    {
        ZFImpl_ZFLua_luaObjectInfoT(ret, L, i);
    }
    return ret;
}
zfbool ZFImpl_ZFLua_zfstringAppend(ZF_IN lua_State *L,
                                   ZF_IN_OUT zfstring &s,
                                   ZF_IN_OPT int luaParamOffset /* = 1 */)
{
    int count = (int)lua_gettop(L);
    if(count <= luaParamOffset - 1)
    {
        return zftrue;
    }

    const zfchar *fmt = zfnull;
    if(!ZFImpl_ZFLua_toString(fmt, L, luaParamOffset))
    {
        return zffalse;
    }

    zfstring fmtTmp;
    const zfchar *pFmt = fmt;
    const zfchar *pFmtL = fmt;
    const zfchar *pToken = zfnull;
    for(int i = luaParamOffset + 1; i <= count; ++i)
    {
        do {
            while(*pFmt != '\0' && *pFmt != '%') {++pFmt;}
            if(*pFmt == '\0' || *(pFmt + 1) == '\0')
            {
                return zffalse;
            }
            if(*(pFmt + 1) == '%')
            {
                pFmt += 2;
                continue;
            }
        } while(zffalse);
        s.append(pFmtL, pFmt - pFmtL);

        pToken = pFmt + 1;
        while(zffalse
            || *pToken == '-'
            || *pToken == '+'
            || *pToken == ' '
            || *pToken == '.'
            || (*pToken >= '0' && *pToken <= '9')
        ) {
            ++pToken;
        }

        switch(*pToken)
        {
            case 'b':
                pFmtL = pToken + 1;
                fmtTmp.assign(pFmt, pFmtL - pFmt);
                zfstringAppend(s, fmtTmp, _ZFP_ZFImpl_ZFLua_zfstringAppend_bool(L, i));
                break;
            case 'z':
                if(*(pToken + 1) != 'i')
                {
                    return zffalse;
                }
                pFmtL = pToken + 2;
                fmtTmp.assign(pFmt, pFmtL - pFmt);
                zfstringAppend(s, fmtTmp, (zfindex)_ZFP_ZFImpl_ZFLua_zfstringAppend_number(L, i));
                break;
            case 'd':
            case 'i':
                pFmtL = pToken + 1;
                fmtTmp.assign(pFmt, pFmtL - pFmt);
                zfstringAppend(s, fmtTmp, (zfint)_ZFP_ZFImpl_ZFLua_zfstringAppend_number(L, i));
                break;
            case 'u':
            case 'o':
            case 'x':
            case 'X':
                pFmtL = pToken + 1;
                fmtTmp.assign(pFmt, pFmtL - pFmt);
                zfstringAppend(s, fmtTmp, (zfuint)_ZFP_ZFImpl_ZFLua_zfstringAppend_number(L, i));
                break;
            case 'f':
                pFmtL = pToken + 1;
                fmtTmp.assign(pFmt, pFmtL - pFmt);
                zfstringAppend(s, fmtTmp, (zffloat)_ZFP_ZFImpl_ZFLua_zfstringAppend_number(L, i));
                break;
            case 'p':
                pFmtL = pToken + 1;
                fmtTmp.assign(pFmt, pFmtL - pFmt);
                fmtTmp[fmtTmp.length() - 1] = 's';
                zfstringAppend(s, fmtTmp, _ZFP_ZFImpl_ZFLua_zfstringAppend_pointer(L, i).cString());
                break;
            case 'c':
            case 'C':
                pFmtL = pToken + 1;
                fmtTmp.assign(pFmt, pFmtL - pFmt);
                {
                    zfstring tmp = _ZFP_ZFImpl_ZFLua_zfstringAppend_string(L, i, zffalse);
                    zfstringAppend(s, fmtTmp, (zfchar)(tmp.isEmpty() ? '?' : tmp[0]));
                }
                break;
            case 's':
            case 'S':
                pFmtL = pToken + 1;
                fmtTmp.assign(pFmt, pFmtL - pFmt);
                zfstringAppend(s, fmtTmp, _ZFP_ZFImpl_ZFLua_zfstringAppend_string(L, i, zftrue).cString());
                break;
            default:
                return zffalse;
        }
        pFmt = pFmtL;
    }
    s.append(pFmtL, (fmt + zfslen(fmt)) - pFmtL);

    return zftrue;
}

ZF_NAMESPACE_GLOBAL_END

