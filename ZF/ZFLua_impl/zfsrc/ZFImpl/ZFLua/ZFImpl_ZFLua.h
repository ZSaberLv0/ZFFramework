/**
 * @file ZFImpl_ZFLua.h
 * @brief global header for lua impl
 */

#ifndef _ZFI_ZFImpl_ZFLua_h_
#define _ZFI_ZFImpl_ZFLua_h_

#include "ZFLua.h"

#include "_impl/lua/ELuna.h"

ZF_NAMESPACE_GLOBAL_BEGIN

extern ZF_ENV_EXPORT lua_State *_ZFP_ZFImpl_ZFLua_luaState(void);
/**
 * @brief global lua state for lua impl
 *
 * lua state would be initialized during #ZFFrameworkInit as level #ZFLevelAppEssential\n
 * \n
 * if you want to setup existing lua_State instead of #ZFImpl_ZFLua_luaState,
 * you must first invoke #ZFImpl_ZFLua_luaStateAttach\n
 * \n
 * \n
 * ADVANCED: how the impl works:
 * -  all attach steps can be registered by #ZFImpl_ZFLua_implSetupCallback_DEFINE,
 *   when #ZFImpl_ZFLua_luaStateAttach,
 *   all necessary register steps would be performed
 * -  in ZFLua, all types are wrapped by #zfautoObject
 *   (wrapped directly for ZFObject type,
 *   wrapped by #ZFTypeIdWrapper for non-ZFObject type)
 * -  all global variables are also wrapped by #zfautoObject
 * -  class and topmost namespace are wrapped by lua raw string value as a global variable,
 *   and should be registered by #ZFImpl_ZFLua_implSetupScope\n
 *   these things are equal:
 *   -  `MyClass.myFunc(param);`
 *   -  `zfl_callStatic("MyClass.myFunc", param);`
 *   -  `local cls = MyClass; cls.myFunc(param);`
 * -  member functions are dispatched as `obj:myFunc(param)`,
 *   which equals to `obj.myFunc(obj, param)`
 * -  all #zfautoObject must be registered by #ZFImpl_ZFLua_implSetupObject,
 *   and all methods would be dispatched internally,
 *   you should not modify #zfautoObject's "__index" by other lua bind tools
 */
#define ZFImpl_ZFLua_luaState _ZFP_ZFImpl_ZFLua_luaState()

/**
 * @brief change #ZFImpl_ZFLua_luaState, can not undo
 */
extern ZF_ENV_EXPORT void ZFImpl_ZFLua_luaStateChange(ZF_IN lua_State *L);

/**
 * @brief see #ZFImpl_ZFLua_luaState
 *
 * must not be attached more than one time
 */
extern ZF_ENV_EXPORT void *ZFImpl_ZFLua_luaStateOpen(void);
/**
 * @brief see #ZFImpl_ZFLua_luaState
 */
extern ZF_ENV_EXPORT void ZFImpl_ZFLua_luaStateClose(ZF_IN lua_State *L);

/**
 * @brief see #ZFImpl_ZFLua_luaState
 *
 * must not be attached more than one time
 */
extern ZF_ENV_EXPORT void ZFImpl_ZFLua_luaStateAttach(ZF_IN lua_State *L);
/**
 * @brief see #ZFImpl_ZFLua_luaStateAttach
 *
 * note, this method only detach L from ZFFramework,
 * all methods registered to L can not be undo
 */
extern ZF_ENV_EXPORT void ZFImpl_ZFLua_luaStateDetach(ZF_IN lua_State *L);

/**
 * @brief return all lua_State that currently registered, for impl or debug use only
 */
extern ZF_ENV_EXPORT const ZFCoreArrayPOD<lua_State *> &ZFImpl_ZFLua_luaStateList(void);
/**
 * @brief see #ZFImpl_ZFLua_luaStateList
 */
extern ZF_ENV_EXPORT void ZFImpl_ZFLua_luaStateListT(ZF_IN_OUT ZFCoreArray<lua_State *> &ret);

// ============================================================
typedef void (*_ZFP_ZFImpl_ZFLua_ImplSetupCallback)(ZF_IN_OUT lua_State *L);
extern ZF_ENV_EXPORT void _ZFP_ZFImpl_ZFLua_implSetupCallbackRegister(ZF_IN _ZFP_ZFImpl_ZFLua_ImplSetupCallback setupAttachCallback,
                                                                      ZF_IN _ZFP_ZFImpl_ZFLua_ImplSetupCallback setupDetachCallback);
extern ZF_ENV_EXPORT void _ZFP_ZFImpl_ZFLua_implSetupCallbackUnregister(ZF_IN _ZFP_ZFImpl_ZFLua_ImplSetupCallback setupAttachCallback,
                                                                        ZF_IN _ZFP_ZFImpl_ZFLua_ImplSetupCallback setupDetachCallback);

/**
 * @brief for impl to add attach and detach actions
 *
 * usage:
 * @code
 *   ZFImpl_ZFLua_implSetupCallback_DEFINE(YourSetupSig, {
 *           // your own attach action, callback proto type:
 *           //   zfbool implSetupAttach(ZF_IN_OUT lua_State *L);
 *       }, {
 *           // your own detach action, callback proto type:
 *           //   zfbool implSetupDetach(ZF_IN_OUT lua_State *L);
 *       })
 * @endcode
 */
#define ZFImpl_ZFLua_implSetupCallback_DEFINE(SetupSig, setupAttachAction, setupDetachAction) \
    ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFImpl_ZFLua_implSetupAction_##SetupSig, ZFLevelZFFrameworkLow) \
    { \
        _ZFP_ZFImpl_ZFLua_implSetupCallbackRegister(zfself::implSetupAttach, zfself::implSetupDetach); \
    } \
    ZF_GLOBAL_INITIALIZER_DESTROY(ZFImpl_ZFLua_implSetupAction_##SetupSig) \
    { \
        _ZFP_ZFImpl_ZFLua_implSetupCallbackUnregister(zfself::implSetupAttach, zfself::implSetupDetach); \
    } \
    public: \
        static void implSetupAttach(ZF_IN_OUT lua_State *L) \
        { \
            setupAttachAction \
        } \
        static void implSetupDetach(ZF_IN_OUT lua_State *L) \
        { \
            setupDetachAction \
        } \
    ZF_GLOBAL_INITIALIZER_END(ZFImpl_ZFLua_implSetupAction_##SetupSig)

// ============================================================
/** @brief see #ZFImpl_ZFLua_luaState */
extern ZF_ENV_EXPORT void ZFImpl_ZFLua_implSetupScope(ZF_IN_OUT lua_State *L, ZF_IN const zfchar *scopeName);
/** @brief see #ZFImpl_ZFLua_luaState */
extern ZF_ENV_EXPORT void ZFImpl_ZFLua_implSetupScope(ZF_IN_OUT lua_State *L, ZF_IN const zfchar **scopeNameList);
/** @brief see #ZFImpl_ZFLua_luaState */
extern ZF_ENV_EXPORT void ZFImpl_ZFLua_implSetupObject(ZF_IN_OUT lua_State *L, ZF_IN_OPT int objIndex = -1);

// ============================================================
/** @brief class prefix for #ZFTypeIdWrapper impl */
#define ZFImpl_ZFLua_PropTypePrefix ZFTypeIdWrapperPrefixName
/** @brief see #ZFImpl_ZFLua_PropTypePrefix */
#define ZFImpl_ZFLua_PropTypePrefixLen ZFTypeIdWrapperPrefixNameLen

// ============================================================
/**
 * @brief run lua code, L must be first initialized by #ZFImpl_ZFLua_luaStateAttach
 *
 * NOTE: the lua code can return any number of values (any type that #ZFImpl_ZFLua_toGeneric supports),
 * when more than one value returned,
 * the luaResult would holds a #v_ZFCoreArray
 */
extern ZF_ENV_EXPORT zfbool ZFImpl_ZFLua_execute(ZF_IN lua_State *L,
                                                 ZF_IN const zfchar *buf,
                                                 ZF_IN_OPT zfindex bufLen = zfindexMax(),
                                                 ZF_OUT_OPT zfautoObject *luaResult = zfnull,
                                                 ZF_IN_OPT const ZFCoreArray<zfautoObject> *luaParams = zfnull,
                                                 ZF_OUT_OPT zfstring *errHint = zfnull,
                                                 ZF_IN_OPT const zfchar *chunkInfo = zfnull);

// ============================================================
// utils
/** @brief see #ZFImpl_ZFLua_luaObjectInfo */
extern ZF_ENV_EXPORT void ZFImpl_ZFLua_luaObjectInfoT(ZF_OUT zfstring &ret,
                                                      ZF_IN lua_State *L,
                                                      ZF_IN int luaStackOffset,
                                                      ZF_IN_OPT zfbool printLuaType = zffalse);
/**
 * @brief get raw lua object info
 */
inline zfstring ZFImpl_ZFLua_luaObjectInfo(ZF_IN lua_State *L,
                                           ZF_IN int luaStackOffset,
                                           ZF_IN_OPT zfbool printLuaType = zffalse)
{
    zfstring ret;
    ZFImpl_ZFLua_luaObjectInfoT(ret, L, luaStackOffset, printLuaType);
    return ret;
}

/**
 * @brief get params from lua
 */
extern ZF_ENV_EXPORT zfbool ZFImpl_ZFLua_toObject(ZF_OUT zfautoObject &param,
                                                  ZF_IN lua_State *L,
                                                  ZF_IN int luaStackOffset);

/**
 * @brief get params from lua
 *
 * supports these types:
 * -  zfautoObject
 * -  any types that can be converted by #ZFImpl_ZFLua_toCallback
 * -  any type that can be converted to string by #ZFImpl_ZFLua_toString,
 *   result would be stored as #ZFDI_WrapperBase,
 *   and would be converted to #ZFTypeIdWrapper during function call
 */
extern ZF_ENV_EXPORT zfbool ZFImpl_ZFLua_toGeneric(ZF_OUT zfautoObject &param,
                                                   ZF_IN lua_State *L,
                                                   ZF_IN int luaStackOffset);

/**
 * @brief get params from lua
 *
 * supports these types:
 * -  #v_ZFCallback
 * -  lua function, converted by ZFCallbackForLua (available in lua code only)
 */
extern ZF_ENV_EXPORT zfbool ZFImpl_ZFLua_toCallback(ZF_OUT zfautoObject &param,
                                                    ZF_IN lua_State *L,
                                                    ZF_IN int luaStackOffset);

/**
 * @brief get params from lua
 *
 * support types:
 * -  normal lua string
 * -  #v_zfstring
 */
extern ZF_ENV_EXPORT zfbool ZFImpl_ZFLua_toString(ZF_IN_OUT zfstring &s,
                                                  ZF_IN lua_State *L,
                                                  ZF_IN int luaStackOffset,
                                                  ZF_IN_OPT zfbool allowEmpty = zffalse,
                                                  ZF_OUT_OPT const ZFClass **holderCls = zfnull);
/** @brief see #ZFImpl_ZFLua_toString */
extern ZF_ENV_EXPORT zfbool ZFImpl_ZFLua_toString(ZF_IN_OUT zfstring &s,
                                                  ZF_IN ZFObject *obj,
                                                  ZF_IN_OPT zfbool allowEmpty = zffalse,
                                                  ZF_OUT_OPT const ZFClass **holderCls = zfnull);

/** @brief see #ZFImpl_ZFLua_toString */
extern ZF_ENV_EXPORT zfbool ZFImpl_ZFLua_toString(ZF_IN_OUT const zfchar *&s,
                                                  ZF_IN lua_State *L,
                                                  ZF_IN int luaStackOffset,
                                                  ZF_IN_OPT zfbool allowEmpty = zffalse,
                                                  ZF_OUT_OPT const ZFClass **holderCls = zfnull);
/** @brief see #ZFImpl_ZFLua_toString */
extern ZF_ENV_EXPORT zfbool ZFImpl_ZFLua_toString(ZF_IN_OUT const zfchar *&s,
                                                  ZF_IN ZFObject *obj,
                                                  ZF_IN_OPT zfbool allowEmpty = zffalse,
                                                  ZF_OUT_OPT const ZFClass **holderCls = zfnull);

/** @brief see #ZFImpl_ZFLua_toNumber */
extern ZF_ENV_EXPORT zfbool ZFImpl_ZFLua_toNumberT(ZF_OUT zfautoObject &ret,
                                                   ZF_IN lua_State *L,
                                                   ZF_IN int luaStackOffset,
                                                   ZF_IN_OPT zfbool allowEmpty = zffalse,
                                                   ZF_OUT_OPT const ZFClass **holderCls = zfnull);
/**
 * @brief get params from lua
 *
 * support types:
 * -  normal lua number
 * -  #v_zfbool
 * -  #v_zfindex
 * -  #v_zfint
 * -  #v_zfuint
 * -  #v_zffloat
 * -  #v_zfdouble
 * -  #v_zflongdouble
 * -  #v_zfbyte
 * -  #v_zftimet
 * -  #v_zfflags
 * -  #v_zfidentity
 * -  all #ZFEnum types
 *
 * return proper #v_zflongdouble if success, or empty if fail
 */
inline zfautoObject ZFImpl_ZFLua_toNumber(ZF_IN lua_State *L,
                                          ZF_IN int luaStackOffset,
                                          ZF_IN_OPT zfbool allowEmpty = zffalse,
                                          ZF_OUT_OPT const ZFClass **holderCls = zfnull)
{
    zfautoObject ret;
    ZFImpl_ZFLua_toNumberT(ret, L, luaStackOffset, allowEmpty, holderCls);
    return ret;
}

/** @brief see #ZFImpl_ZFLua_toNumber */
extern ZF_ENV_EXPORT zfbool ZFImpl_ZFLua_toNumberT(ZF_OUT zfautoObject &ret,
                                                   ZF_IN ZFObject *obj,
                                                   ZF_IN_OPT zfbool allowEmpty = zffalse,
                                                   ZF_OUT_OPT const ZFClass **holderCls = zfnull);

/**
 * @brief convert native type to lua type
 *
 * support all types that can be converted by #ZFImpl_ZFLua_toString
 * and #ZFImpl_ZFLua_toNumber\n
 * if success, push result value to top of lua's stack and return true,
 * otherwise, push nothing and return false\n
 * if allowEmpty, push 0 number if obj is null
 */
extern ZF_ENV_EXPORT zfbool ZFImpl_ZFLua_toLuaValue(ZF_IN lua_State *L,
                                                    ZF_IN ZFObject *obj,
                                                    ZF_IN_OPT zfbool allowEmpty = zffalse);

/**
 * @brief append with va_arg, see #ZFLuaExecute
 */
extern ZF_ENV_EXPORT zfbool ZFImpl_ZFLua_zfstringAppend(ZF_IN lua_State *L,
                                                        ZF_IN_OUT zfstring &s,
                                                        ZF_IN_OPT int luaParamOffset = 1);

// ============================================================
// wrapper for impl
/** @brief util for impl */
inline lua_State *ZFImpl_ZFLua_luaOpen(void)
{
    return ELuna::openLua();
}
/** @brief util for impl */
inline void ZFImpl_ZFLua_luaClose(ZF_IN lua_State *L)
{
    ELuna::closeLua(L);
}

/** @brief util for impl */
template<typename T>
inline void ZFImpl_ZFLua_luaClassRegister(ZF_IN lua_State *L, ZF_IN const zfchar *name)
{
    ELuna::registerClass<T>(L, name, ELuna::constructor<zfautoObject>);
}
/** @brief util for impl */
template<typename F>
inline void ZFImpl_ZFLua_luaFunctionRegister(ZF_IN lua_State *L, ZF_IN const zfchar *name, ZF_IN F f)
{
    ELuna::registerFunction(L, name, f);
}
/** @brief util for impl */
inline void ZFImpl_ZFLua_luaCFunctionRegister(ZF_IN lua_State *L, ZF_IN const zfchar *name, ZF_IN int (*f)(lua_State *))
{
    lua_register(L, name, f);
}

/** @brief util for impl */
inline void ZFImpl_ZFLua_luaPush(ZF_IN lua_State *L, ZF_IN zfautoObject &v)
{
    ELuna::convert2LuaType<zfautoObject>::convertType(L, v);
    ZFImpl_ZFLua_implSetupObject(L);
}
/** @brief util for impl */
inline void ZFImpl_ZFLua_luaPush(ZF_IN lua_State *L, ZF_IN const zfautoObject &v)
{
    zfautoObject t = v;
    ZFImpl_ZFLua_luaPush(L, t);
}
/** @brief util for impl */
inline zfautoObject &ZFImpl_ZFLua_luaGet(ZF_IN lua_State *L, ZF_IN int luaStackOffset)
{
    return ELuna::convert2CppType<zfautoObject &>::convertType(L, luaStackOffset);
}
/** @brief util for impl */
inline int ZFImpl_ZFLua_luaError(ZF_IN lua_State *L, ZF_IN const zfchar *fmt, ...)
{
    zfstring errHint;
    va_list vaList;
    va_start(vaList, fmt);
    zfstringAppendV(errHint, fmt, vaList);
    va_end(vaList);
    return luaL_error(L, "%s", errHint.cString());
}

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFImpl_ZFLua_h_

