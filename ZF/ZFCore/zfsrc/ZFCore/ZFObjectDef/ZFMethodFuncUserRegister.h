/**
 * @file ZFMethodFuncUserRegister.h
 * @brief user registered ZFMethod
 */

#ifndef _ZFI_ZFMethodFuncUserRegister_h_
#define _ZFI_ZFMethodFuncUserRegister_h_

#include "ZFMethod.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/** @brief see #ZFMethodFuncUserRegister_0 */
extern ZF_ENV_EXPORT void ZFMethodFuncUserUnregister(ZF_IN const ZFMethod *method);

// ============================================================
#define _ZFP_ZFMethodFuncUserRegisterParamExpand( \
        GenericInvokerOwner, methodInvoker, methodNamespaceString, \
        ReturnType, methodNameString \
        , ParamExpandOrEmpty0, ParamType0, param0, DefaultExpandOrEmpty0, DefaultValueFix0 \
        , ParamExpandOrEmpty1, ParamType1, param1, DefaultExpandOrEmpty1, DefaultValueFix1 \
        , ParamExpandOrEmpty2, ParamType2, param2, DefaultExpandOrEmpty2, DefaultValueFix2 \
        , ParamExpandOrEmpty3, ParamType3, param3, DefaultExpandOrEmpty3, DefaultValueFix3 \
        , ParamExpandOrEmpty4, ParamType4, param4, DefaultExpandOrEmpty4, DefaultValueFix4 \
        , ParamExpandOrEmpty5, ParamType5, param5, DefaultExpandOrEmpty5, DefaultValueFix5 \
        , ParamExpandOrEmpty6, ParamType6, param6, DefaultExpandOrEmpty6, DefaultValueFix6 \
        , ParamExpandOrEmpty7, ParamType7, param7, DefaultExpandOrEmpty7, DefaultValueFix7 \
    ) \
    ( \
        zftrue \
        , zffalse \
        , zfnull \
        , ZFCastReinterpret(ZFFuncAddrType, methodInvoker) \
        , _ZFP_ZFMETHOD_GENERIC_INVOKER_ADDR(GenericInvokerOwner) \
        , _ZFP_ZFMethodTypeText(ZFMethodTypeStatic) \
        , zfnull \
        , ZFMethodPrivilegeTypePublic \
        , methodNamespaceString \
        , methodNameString \
        , ZFTypeId<zftTraits<ReturnType>::TrNoRef>::TypeId() \
        , ZFM_TOSTRING_DIRECT(ReturnType) \
        ParamExpandOrEmpty0(ZFM_EMPTY() \
                , ZFTypeId<zftTraits<ParamType0>::TrNoRef>::TypeId() \
                , ZFM_TOSTRING(ParamType0) \
                , ZFM_TOSTRING(param0) \
                , _ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS_ADDR(GenericInvokerOwner, DefaultExpandOrEmpty0, 0) \
            ) \
        ParamExpandOrEmpty1(ZFM_EMPTY() \
                , ZFTypeId<zftTraits<ParamType1>::TrNoRef>::TypeId() \
                , ZFM_TOSTRING(ParamType1) \
                , ZFM_TOSTRING(param1) \
                , _ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS_ADDR(GenericInvokerOwner, DefaultExpandOrEmpty1, 1) \
            ) \
        ParamExpandOrEmpty2(ZFM_EMPTY() \
                , ZFTypeId<zftTraits<ParamType2>::TrNoRef>::TypeId() \
                , ZFM_TOSTRING(ParamType2) \
                , ZFM_TOSTRING(param2) \
                , _ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS_ADDR(GenericInvokerOwner, DefaultExpandOrEmpty2, 2) \
            ) \
        ParamExpandOrEmpty3(ZFM_EMPTY() \
                , ZFTypeId<zftTraits<ParamType3>::TrNoRef>::TypeId() \
                , ZFM_TOSTRING(ParamType3) \
                , ZFM_TOSTRING(param3) \
                , _ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS_ADDR(GenericInvokerOwner, DefaultExpandOrEmpty3, 3) \
            ) \
        ParamExpandOrEmpty4(ZFM_EMPTY() \
                , ZFTypeId<zftTraits<ParamType4>::TrNoRef>::TypeId() \
                , ZFM_TOSTRING(ParamType4) \
                , ZFM_TOSTRING(param4) \
                , _ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS_ADDR(GenericInvokerOwner, DefaultExpandOrEmpty4, 4) \
            ) \
        ParamExpandOrEmpty5(ZFM_EMPTY() \
                , ZFTypeId<zftTraits<ParamType5>::TrNoRef>::TypeId() \
                , ZFM_TOSTRING(ParamType5) \
                , ZFM_TOSTRING(param5) \
                , _ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS_ADDR(GenericInvokerOwner, DefaultExpandOrEmpty5, 5) \
            ) \
        ParamExpandOrEmpty6(ZFM_EMPTY() \
                , ZFTypeId<zftTraits<ParamType6>::TrNoRef>::TypeId() \
                , ZFM_TOSTRING(ParamType6) \
                , ZFM_TOSTRING(param6) \
                , _ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS_ADDR(GenericInvokerOwner, DefaultExpandOrEmpty6, 6) \
            ) \
        ParamExpandOrEmpty7(ZFM_EMPTY() \
                , ZFTypeId<zftTraits<ParamType7>::TrNoRef>::TypeId() \
                , ZFM_TOSTRING(ParamType7) \
                , ZFM_TOSTRING(param7) \
                , _ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS_ADDR(GenericInvokerOwner, DefaultExpandOrEmpty7, 7) \
            ) \
        , zfnull \
    )

#define _ZFP_ZFMethodFuncUserRegister(...) \
    _ZFP_MtdD_EXPAND(_ZFP_ZFMethodFuncUserRegister_(__VA_ARGS__))
#define _ZFP_ZFMethodFuncUserRegister_(resultMethod, methodInvokerAction, \
        methodNamespaceString, RegSig, \
        ReturnType, methodNameString \
        , ParamExpandOrEmpty0, ParamType0, param0, DefaultExpandOrEmpty0, DefaultValueFix0 \
        , ParamExpandOrEmpty1, ParamType1, param1, DefaultExpandOrEmpty1, DefaultValueFix1 \
        , ParamExpandOrEmpty2, ParamType2, param2, DefaultExpandOrEmpty2, DefaultValueFix2 \
        , ParamExpandOrEmpty3, ParamType3, param3, DefaultExpandOrEmpty3, DefaultValueFix3 \
        , ParamExpandOrEmpty4, ParamType4, param4, DefaultExpandOrEmpty4, DefaultValueFix4 \
        , ParamExpandOrEmpty5, ParamType5, param5, DefaultExpandOrEmpty5, DefaultValueFix5 \
        , ParamExpandOrEmpty6, ParamType6, param6, DefaultExpandOrEmpty6, DefaultValueFix6 \
        , ParamExpandOrEmpty7, ParamType7, param7, DefaultExpandOrEmpty7, DefaultValueFix7 \
    ) \
    const ZFMethod *resultMethod = zfnull; \
    { \
        zfclassNotPOD _ZFP_MtdFUR \
        { \
        public: \
            static ReturnType invoker(ZF_IN const ZFMethod *invokerMethod, ZF_IN ZFObject *invokerObject \
                    ParamExpandOrEmpty0(ZFM_COMMA() ParamType0 param0) \
                    ParamExpandOrEmpty1(ZFM_COMMA() ParamType1 param1) \
                    ParamExpandOrEmpty2(ZFM_COMMA() ParamType2 param2) \
                    ParamExpandOrEmpty3(ZFM_COMMA() ParamType3 param3) \
                    ParamExpandOrEmpty4(ZFM_COMMA() ParamType4 param4) \
                    ParamExpandOrEmpty5(ZFM_COMMA() ParamType5 param5) \
                    ParamExpandOrEmpty6(ZFM_COMMA() ParamType6 param6) \
                    ParamExpandOrEmpty7(ZFM_COMMA() ParamType7 param7) \
                ) \
            { \
                methodInvokerAction \
            } \
            _ZFP_ZFMETHOD_GENERIC_INVOKER_DECLARE( \
                    ReturnType \
                    , ParamExpandOrEmpty0, ParamType0, param0, DefaultExpandOrEmpty0, DefaultValueFix0 \
                    , ParamExpandOrEmpty1, ParamType1, param1, DefaultExpandOrEmpty1, DefaultValueFix1 \
                    , ParamExpandOrEmpty2, ParamType2, param2, DefaultExpandOrEmpty2, DefaultValueFix2 \
                    , ParamExpandOrEmpty3, ParamType3, param3, DefaultExpandOrEmpty3, DefaultValueFix3 \
                    , ParamExpandOrEmpty4, ParamType4, param4, DefaultExpandOrEmpty4, DefaultValueFix4 \
                    , ParamExpandOrEmpty5, ParamType5, param5, DefaultExpandOrEmpty5, DefaultValueFix5 \
                    , ParamExpandOrEmpty6, ParamType6, param6, DefaultExpandOrEmpty6, DefaultValueFix6 \
                    , ParamExpandOrEmpty7, ParamType7, param7, DefaultExpandOrEmpty7, DefaultValueFix7 \
                ) \
        }; \
        resultMethod = _ZFP_ZFMethodRegister _ZFP_ZFMethodFuncUserRegisterParamExpand( \
                _ZFP_MtdFUR, _ZFP_MtdFUR::invoker, methodNamespaceString, \
                ReturnType, methodNameString \
                , ParamExpandOrEmpty0, ParamType0, param0, DefaultExpandOrEmpty0, DefaultValueFix0 \
                , ParamExpandOrEmpty1, ParamType1, param1, DefaultExpandOrEmpty1, DefaultValueFix1 \
                , ParamExpandOrEmpty2, ParamType2, param2, DefaultExpandOrEmpty2, DefaultValueFix2 \
                , ParamExpandOrEmpty3, ParamType3, param3, DefaultExpandOrEmpty3, DefaultValueFix3 \
                , ParamExpandOrEmpty4, ParamType4, param4, DefaultExpandOrEmpty4, DefaultValueFix4 \
                , ParamExpandOrEmpty5, ParamType5, param5, DefaultExpandOrEmpty5, DefaultValueFix5 \
                , ParamExpandOrEmpty6, ParamType6, param6, DefaultExpandOrEmpty6, DefaultValueFix6 \
                , ParamExpandOrEmpty7, ParamType7, param7, DefaultExpandOrEmpty7, DefaultValueFix7 \
            ); \
    } \
    ZFUNUSED(resultMethod)

#define _ZFP_ZFMETHOD_FUNC_USER_REGISTER(...) \
    _ZFP_MtdD_EXPAND(_ZFP_ZFMETHOD_FUNC_USER_REGISTER_(__VA_ARGS__))
#define _ZFP_ZFMETHOD_FUNC_USER_REGISTER_(methodInvokerAction, \
        methodNamespaceString, RegSig, \
        ReturnType, methodName \
        , ParamExpandOrEmpty0, ParamType0, param0, DefaultExpandOrEmpty0, DefaultValueFix0 \
        , ParamExpandOrEmpty1, ParamType1, param1, DefaultExpandOrEmpty1, DefaultValueFix1 \
        , ParamExpandOrEmpty2, ParamType2, param2, DefaultExpandOrEmpty2, DefaultValueFix2 \
        , ParamExpandOrEmpty3, ParamType3, param3, DefaultExpandOrEmpty3, DefaultValueFix3 \
        , ParamExpandOrEmpty4, ParamType4, param4, DefaultExpandOrEmpty4, DefaultValueFix4 \
        , ParamExpandOrEmpty5, ParamType5, param5, DefaultExpandOrEmpty5, DefaultValueFix5 \
        , ParamExpandOrEmpty6, ParamType6, param6, DefaultExpandOrEmpty6, DefaultValueFix6 \
        , ParamExpandOrEmpty7, ParamType7, param7, DefaultExpandOrEmpty7, DefaultValueFix7 \
    ) \
    zfclassNotPOD _ZFP_MtdFURIvk_##methodName##_##RegSig \
    { \
    public: \
        static ReturnType invoker(ZF_IN const ZFMethod *invokerMethod, ZF_IN ZFObject *invokerObject \
                ParamExpandOrEmpty0(ZFM_COMMA() ParamType0 param0) \
                ParamExpandOrEmpty1(ZFM_COMMA() ParamType1 param1) \
                ParamExpandOrEmpty2(ZFM_COMMA() ParamType2 param2) \
                ParamExpandOrEmpty3(ZFM_COMMA() ParamType3 param3) \
                ParamExpandOrEmpty4(ZFM_COMMA() ParamType4 param4) \
                ParamExpandOrEmpty5(ZFM_COMMA() ParamType5 param5) \
                ParamExpandOrEmpty6(ZFM_COMMA() ParamType6 param6) \
                ParamExpandOrEmpty7(ZFM_COMMA() ParamType7 param7) \
            ) \
        { \
            methodInvokerAction \
        } \
        _ZFP_ZFMETHOD_GENERIC_INVOKER_DECLARE( \
                ReturnType \
                , ParamExpandOrEmpty0, ParamType0, param0, DefaultExpandOrEmpty0, DefaultValueFix0 \
                , ParamExpandOrEmpty1, ParamType1, param1, DefaultExpandOrEmpty1, DefaultValueFix1 \
                , ParamExpandOrEmpty2, ParamType2, param2, DefaultExpandOrEmpty2, DefaultValueFix2 \
                , ParamExpandOrEmpty3, ParamType3, param3, DefaultExpandOrEmpty3, DefaultValueFix3 \
                , ParamExpandOrEmpty4, ParamType4, param4, DefaultExpandOrEmpty4, DefaultValueFix4 \
                , ParamExpandOrEmpty5, ParamType5, param5, DefaultExpandOrEmpty5, DefaultValueFix5 \
                , ParamExpandOrEmpty6, ParamType6, param6, DefaultExpandOrEmpty6, DefaultValueFix6 \
                , ParamExpandOrEmpty7, ParamType7, param7, DefaultExpandOrEmpty7, DefaultValueFix7 \
            ) \
    }; \
    static _ZFP_ZFMethodRegisterHolder _ZFP_MtdFURH_##methodName##_##RegSig \
        _ZFP_ZFMethodFuncUserRegisterParamExpand( \
            _ZFP_MtdFURIvk_##methodName##_##RegSig, \
            _ZFP_MtdFURIvk_##methodName##_##RegSig::invoker, \
            methodNamespaceString, \
            ReturnType, ZFM_TOSTRING(methodName) \
            , ParamExpandOrEmpty0, ParamType0, param0, DefaultExpandOrEmpty0, DefaultValueFix0 \
            , ParamExpandOrEmpty1, ParamType1, param1, DefaultExpandOrEmpty1, DefaultValueFix1 \
            , ParamExpandOrEmpty2, ParamType2, param2, DefaultExpandOrEmpty2, DefaultValueFix2 \
            , ParamExpandOrEmpty3, ParamType3, param3, DefaultExpandOrEmpty3, DefaultValueFix3 \
            , ParamExpandOrEmpty4, ParamType4, param4, DefaultExpandOrEmpty4, DefaultValueFix4 \
            , ParamExpandOrEmpty5, ParamType5, param5, DefaultExpandOrEmpty5, DefaultValueFix5 \
            , ParamExpandOrEmpty6, ParamType6, param6, DefaultExpandOrEmpty6, DefaultValueFix6 \
            , ParamExpandOrEmpty7, ParamType7, param7, DefaultExpandOrEmpty7, DefaultValueFix7 \
        );

#define _ZFP_ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC(...) \
    _ZFP_MtdD_EXPAND(_ZFP_ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_(__VA_ARGS__))
#define _ZFP_ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_( \
        ReturnType, methodName \
        , ParamExpandOrEmpty0, ParamType0, param0, DefaultExpandOrEmpty0, DefaultValueFix0 \
        , ParamExpandOrEmpty1, ParamType1, param1, DefaultExpandOrEmpty1, DefaultValueFix1 \
        , ParamExpandOrEmpty2, ParamType2, param2, DefaultExpandOrEmpty2, DefaultValueFix2 \
        , ParamExpandOrEmpty3, ParamType3, param3, DefaultExpandOrEmpty3, DefaultValueFix3 \
        , ParamExpandOrEmpty4, ParamType4, param4, DefaultExpandOrEmpty4, DefaultValueFix4 \
        , ParamExpandOrEmpty5, ParamType5, param5, DefaultExpandOrEmpty5, DefaultValueFix5 \
        , ParamExpandOrEmpty6, ParamType6, param6, DefaultExpandOrEmpty6, DefaultValueFix6 \
        , ParamExpandOrEmpty7, ParamType7, param7, DefaultExpandOrEmpty7, DefaultValueFix7 \
    ) \
    _ZFP_ZFMETHOD_FUNC_USER_REGISTER({ \
                return (ReturnType)ZFM_EXPAND(methodName ZFM_BRACKET_L() \
                        ParamExpandOrEmpty0(ZFM_EMPTY() param0) \
                        ParamExpandOrEmpty1(ZFM_COMMA() param1) \
                        ParamExpandOrEmpty2(ZFM_COMMA() param2) \
                        ParamExpandOrEmpty3(ZFM_COMMA() param3) \
                        ParamExpandOrEmpty4(ZFM_COMMA() param4) \
                        ParamExpandOrEmpty5(ZFM_COMMA() param5) \
                        ParamExpandOrEmpty6(ZFM_COMMA() param6) \
                        ParamExpandOrEmpty7(ZFM_COMMA() param7) \
                    ZFM_BRACKET_R()); \
            }, ZF_NAMESPACE_CURRENT(), ZF_CALLER_LINE, \
            ReturnType, methodName \
            , ParamExpandOrEmpty0, ParamType0, param0, DefaultExpandOrEmpty0, DefaultValueFix0 \
            , ParamExpandOrEmpty1, ParamType1, param1, DefaultExpandOrEmpty1, DefaultValueFix1 \
            , ParamExpandOrEmpty2, ParamType2, param2, DefaultExpandOrEmpty2, DefaultValueFix2 \
            , ParamExpandOrEmpty3, ParamType3, param3, DefaultExpandOrEmpty3, DefaultValueFix3 \
            , ParamExpandOrEmpty4, ParamType4, param4, DefaultExpandOrEmpty4, DefaultValueFix4 \
            , ParamExpandOrEmpty5, ParamType5, param5, DefaultExpandOrEmpty5, DefaultValueFix5 \
            , ParamExpandOrEmpty6, ParamType6, param6, DefaultExpandOrEmpty6, DefaultValueFix6 \
            , ParamExpandOrEmpty7, ParamType7, param7, DefaultExpandOrEmpty7, DefaultValueFix7 \
        )

// ============================================================
// 0 param
/**
 * @brief register a custom method to global scope, for advanced reflection use only
 *
 * example:
 * @code
 *   // register it, using ZF_GLOBAL_INITIALIZER_INIT is recommended
 *   ZF_GLOBAL_INITIALIZER_INIT(MyMethodRegister)
 *   {
 *       ZFMethodFuncUserRegisterDetail_2(resultMethod, {
 *               return (ReturnType)yourInvoker(invokerMethod, invokerObject, param0, param1);
 *           }, "MyMethodNamespace",
 *           ReturnType, "myMethodName"
 *           , ZFMP_IN(ParamType0, param0)
 *           , ZFMP_IN(ParamType1, param1));
 *       _method = resultMethod;
 *       zfLogTrimT() << resultMethod;
 *   }
 *   ZF_GLOBAL_INITIALIZER_DESTROY(MyMethodRegister)
 *   {
 *       ZFMethodFuncUserUnregister(_method);
 *   }
 *   const ZFMethod *_method;
 *   ZF_GLOBAL_INITIALIZER_END(MyMethodRegister)
 *
 *   // or, you may use this macro for short, at cpp files only
 *   ZFMETHOD_FUNC_USER_REGISTER_DETAIL_2({
 *           return (ReturnType)yourInvoker(invokerMethod, invokerObject, param0, param1);
 *       }, "MyMethodNamespace", YourRegSig,
 *       ReturnType, "myMethodName"
 *       , ZFMP_IN(ParamType0, param0)
 *       , ZFMP_IN(ParamType1, param1))
 *
 *   // or, if you already have raw function
 *   static ReturnType rawFunc(ParamType0 param0, ParamType1 param1) {...}
 *   ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(
 *       ReturnType, rawFunc
 *       , ZFMP_IN(ParamType0, param0)
 *       , ZFMP_IN(ParamType1, param1))
 * @endcode
 *
 * note:
 * -  if there is already a method exist with same method name and method id,
 *   register would fail
 * -  the registered method must be unregister manually at proper time,
 *   use #ZF_GLOBAL_INITIALIZER_INIT is recommeded
 * -  once registered, the method can be reflected by #ZFClass::methodForName,
 *   but there's no way to invoke the method directly,
 *   it can only be invoked by #ZFMethod::execute
 */
#define ZFMethodFuncUserRegister_0(resultMethod, methodInvokerAction, \
    methodNamespaceString, \
    ReturnType, methodNameString \
    ) \
    _ZFP_ZFMethodFuncUserRegister(resultMethod, methodInvokerAction, \
        methodNamespaceString, ZF_CALLER_LINE, \
        ReturnType, methodNameString \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethodFuncUserRegister_0 */
#define ZFMethodFuncUserRegisterDetail_0(resultMethod, methodInvokerAction, \
    methodNamespaceString, RegSig, \
    ReturnType, methodNameString \
    ) \
    _ZFP_ZFMethodFuncUserRegister(resultMethod, methodInvokerAction, \
        methodNamespaceString, RegSig, \
        ReturnType, methodNameString \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethodFuncUserRegister_0 */
#define ZFMETHOD_FUNC_USER_REGISTER_0(methodInvokerAction, \
    ReturnType, methodName \
    ) \
    _ZFP_ZFMETHOD_FUNC_USER_REGISTER(methodInvokerAction, \
        ZF_NAMESPACE_CURRENT(), ZF_CALLER_LINE, \
        ReturnType, methodName \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethodFuncUserRegister_0 */
#define ZFMETHOD_FUNC_USER_REGISTER_DETAIL_0(methodInvokerAction, \
    methodNamespaceString, RegSig, \
    ReturnType, methodName \
    ) \
    _ZFP_ZFMETHOD_FUNC_USER_REGISTER(methodInvokerAction, \
        methodNamespaceString, RegSig, \
        ReturnType, methodName \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethodFuncUserRegister_0 */
#define ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_0( \
    ReturnType, methodName \
    ) \
    _ZFP_ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC( \
        ReturnType, methodName \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )

// ============================================================
// 1 param
/** @brief see #ZFMethodFuncUserRegister_0 */
#define ZFMethodFuncUserRegister_1(resultMethod, methodInvokerAction, \
    methodNamespaceString, \
    ReturnType, methodNameString \
    , ZFMP_0 \
    ) \
    _ZFP_ZFMethodFuncUserRegister(resultMethod, methodInvokerAction, \
        methodNamespaceString, ZF_CALLER_LINE, \
        ReturnType, methodNameString \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethodFuncUserRegister_0 */
#define ZFMethodFuncUserRegisterDetail_1(resultMethod, methodInvokerAction, \
    methodNamespaceString, RegSig, \
    ReturnType, methodNameString \
    , ZFMP_0 \
    ) \
    _ZFP_ZFMethodFuncUserRegister(resultMethod, methodInvokerAction, \
        methodNamespaceString, RegSig, \
        ReturnType, methodNameString \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethodFuncUserRegister_0 */
#define ZFMETHOD_FUNC_USER_REGISTER_1(methodInvokerAction, \
    ReturnType, methodName \
    , ZFMP_0 \
    ) \
    _ZFP_ZFMETHOD_FUNC_USER_REGISTER(methodInvokerAction, \
        ZF_NAMESPACE_CURRENT(), ZF_CALLER_LINE, \
        ReturnType, methodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethodFuncUserRegister_0 */
#define ZFMETHOD_FUNC_USER_REGISTER_DETAIL_1(methodInvokerAction, \
    methodNamespaceString, RegSig, \
    ReturnType, methodName \
    , ZFMP_0 \
    ) \
    _ZFP_ZFMETHOD_FUNC_USER_REGISTER(methodInvokerAction, \
        methodNamespaceString, RegSig, \
        ReturnType, methodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethodFuncUserRegister_0 */
#define ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1( \
    ReturnType, methodName \
    , ZFMP_0 \
    ) \
    _ZFP_ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC( \
        ReturnType, methodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )

// ============================================================
// 2 param
/** @brief see #ZFMethodFuncUserRegister_0 */
#define ZFMethodFuncUserRegister_2(resultMethod, methodInvokerAction, \
    methodNamespaceString, \
    ReturnType, methodNameString \
    , ZFMP_0 \
    , ZFMP_1 \
    ) \
    _ZFP_ZFMethodFuncUserRegister(resultMethod, methodInvokerAction, \
        methodNamespaceString, ZF_CALLER_LINE, \
        ReturnType, methodNameString \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethodFuncUserRegister_0 */
#define ZFMethodFuncUserRegisterDetail_2(resultMethod, methodInvokerAction, \
    methodNamespaceString, RegSig, \
    ReturnType, methodNameString \
    , ZFMP_0 \
    , ZFMP_1 \
    ) \
    _ZFP_ZFMethodFuncUserRegister(resultMethod, methodInvokerAction, \
        methodNamespaceString, RegSig, \
        ReturnType, methodNameString \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethodFuncUserRegister_0 */
#define ZFMETHOD_FUNC_USER_REGISTER_2(methodInvokerAction, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    ) \
    _ZFP_ZFMETHOD_FUNC_USER_REGISTER(methodInvokerAction, \
        ZF_NAMESPACE_CURRENT(), ZF_CALLER_LINE, \
        ReturnType, methodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethodFuncUserRegister_0 */
#define ZFMETHOD_FUNC_USER_REGISTER_DETAIL_2(methodInvokerAction, \
    methodNamespaceString, RegSig, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    ) \
    _ZFP_ZFMETHOD_FUNC_USER_REGISTER(methodInvokerAction, \
        methodNamespaceString, RegSig, \
        ReturnType, methodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethodFuncUserRegister_0 */
#define ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2( \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    ) \
    _ZFP_ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC( \
        ReturnType, methodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )

// ============================================================
// 3 param
/** @brief see #ZFMethodFuncUserRegister_0 */
#define ZFMethodFuncUserRegister_3(resultMethod, methodInvokerAction, \
    methodNamespaceString, \
    ReturnType, methodNameString \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    ) \
    _ZFP_ZFMethodFuncUserRegister(resultMethod, methodInvokerAction, \
        methodNamespaceString, ZF_CALLER_LINE, \
        ReturnType, methodNameString \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethodFuncUserRegister_0 */
#define ZFMethodFuncUserRegisterDetail_3(resultMethod, methodInvokerAction, \
    methodNamespaceString, RegSig, \
    ReturnType, methodNameString \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    ) \
    _ZFP_ZFMethodFuncUserRegister(resultMethod, methodInvokerAction, \
        methodNamespaceString, RegSig, \
        ReturnType, methodNameString \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethodFuncUserRegister_0 */
#define ZFMETHOD_FUNC_USER_REGISTER_3(methodInvokerAction, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    ) \
    _ZFP_ZFMETHOD_FUNC_USER_REGISTER(methodInvokerAction, \
        ZF_NAMESPACE_CURRENT(), ZF_CALLER_LINE, \
        ReturnType, methodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethodFuncUserRegister_0 */
#define ZFMETHOD_FUNC_USER_REGISTER_DETAIL_3(methodInvokerAction, \
    methodNamespaceString, RegSig, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    ) \
    _ZFP_ZFMETHOD_FUNC_USER_REGISTER(methodInvokerAction, \
        methodNamespaceString, RegSig, \
        ReturnType, methodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethodFuncUserRegister_0 */
#define ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3( \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    ) \
    _ZFP_ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC( \
        ReturnType, methodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )

// ============================================================
// 4 param
/** @brief see #ZFMethodFuncUserRegister_0 */
#define ZFMethodFuncUserRegister_4(resultMethod, methodInvokerAction, \
    methodNamespaceString, \
    ReturnType, methodNameString \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    ) \
    _ZFP_ZFMethodFuncUserRegister(resultMethod, methodInvokerAction, \
        methodNamespaceString, ZF_CALLER_LINE, \
        ReturnType, methodNameString \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethodFuncUserRegister_0 */
#define ZFMethodFuncUserRegisterDetail_4(resultMethod, methodInvokerAction, \
    methodNamespaceString, RegSig, \
    ReturnType, methodNameString \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    ) \
    _ZFP_ZFMethodFuncUserRegister(resultMethod, methodInvokerAction, \
        methodNamespaceString, RegSig, \
        ReturnType, methodNameString \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethodFuncUserRegister_0 */
#define ZFMETHOD_FUNC_USER_REGISTER_4(methodInvokerAction, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    ) \
    _ZFP_ZFMETHOD_FUNC_USER_REGISTER(methodInvokerAction, \
        ZF_NAMESPACE_CURRENT(), ZF_CALLER_LINE, \
        ReturnType, methodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethodFuncUserRegister_0 */
#define ZFMETHOD_FUNC_USER_REGISTER_DETAIL_4(methodInvokerAction, \
    methodNamespaceString, RegSig, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    ) \
    _ZFP_ZFMETHOD_FUNC_USER_REGISTER(methodInvokerAction, \
        methodNamespaceString, RegSig, \
        ReturnType, methodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethodFuncUserRegister_0 */
#define ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_4( \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    ) \
    _ZFP_ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC( \
        ReturnType, methodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )

// ============================================================
// 5 param
/** @brief see #ZFMethodFuncUserRegister_0 */
#define ZFMethodFuncUserRegister_5(resultMethod, methodInvokerAction, \
    methodNamespaceString, \
    ReturnType, methodNameString \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    ) \
    _ZFP_ZFMethodFuncUserRegister(resultMethod, methodInvokerAction, \
        methodNamespaceString, ZF_CALLER_LINE, \
        ReturnType, methodNameString \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethodFuncUserRegister_0 */
#define ZFMethodFuncUserRegisterDetail_5(resultMethod, methodInvokerAction, \
    methodNamespaceString, RegSig, \
    ReturnType, methodNameString \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    ) \
    _ZFP_ZFMethodFuncUserRegister(resultMethod, methodInvokerAction, \
        methodNamespaceString, RegSig, \
        ReturnType, methodNameString \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethodFuncUserRegister_0 */
#define ZFMETHOD_FUNC_USER_REGISTER_5(methodInvokerAction, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    ) \
    _ZFP_ZFMETHOD_FUNC_USER_REGISTER(methodInvokerAction, \
        ZF_NAMESPACE_CURRENT(), ZF_CALLER_LINE, \
        ReturnType, methodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethodFuncUserRegister_0 */
#define ZFMETHOD_FUNC_USER_REGISTER_DETAIL_5(methodInvokerAction, \
    methodNamespaceString, RegSig, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    ) \
    _ZFP_ZFMETHOD_FUNC_USER_REGISTER(methodInvokerAction, \
        methodNamespaceString, RegSig, \
        ReturnType, methodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethodFuncUserRegister_0 */
#define ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_5( \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    ) \
    _ZFP_ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC( \
        ReturnType, methodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )

// ============================================================
// 6 param
/** @brief see #ZFMethodFuncUserRegister_0 */
#define ZFMethodFuncUserRegister_6(resultMethod, methodInvokerAction, \
    methodNamespaceString, \
    ReturnType, methodNameString \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    ) \
    _ZFP_ZFMethodFuncUserRegister(resultMethod, methodInvokerAction, \
        methodNamespaceString, ZF_CALLER_LINE, \
        ReturnType, methodNameString \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethodFuncUserRegister_0 */
#define ZFMethodFuncUserRegisterDetail_6(resultMethod, methodInvokerAction, \
    methodNamespaceString, RegSig, \
    ReturnType, methodNameString \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    ) \
    _ZFP_ZFMethodFuncUserRegister(resultMethod, methodInvokerAction, \
        methodNamespaceString, RegSig, \
        ReturnType, methodNameString \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethodFuncUserRegister_0 */
#define ZFMETHOD_FUNC_USER_REGISTER_6(methodInvokerAction, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    ) \
    _ZFP_ZFMETHOD_FUNC_USER_REGISTER(methodInvokerAction, \
        ZF_NAMESPACE_CURRENT(), ZF_CALLER_LINE, \
        ReturnType, methodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethodFuncUserRegister_0 */
#define ZFMETHOD_FUNC_USER_REGISTER_DETAIL_6(methodInvokerAction, \
    methodNamespaceString, RegSig, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    ) \
    _ZFP_ZFMETHOD_FUNC_USER_REGISTER(methodInvokerAction, \
        methodNamespaceString, RegSig, \
        ReturnType, methodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethodFuncUserRegister_0 */
#define ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_6( \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    ) \
    _ZFP_ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC( \
        ReturnType, methodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )

// ============================================================
// 7 param
/** @brief see #ZFMethodFuncUserRegister_0 */
#define ZFMethodFuncUserRegister_7(resultMethod, methodInvokerAction, \
    methodNamespaceString, \
    ReturnType, methodNameString \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    ) \
    _ZFP_ZFMethodFuncUserRegister(resultMethod, methodInvokerAction, \
        methodNamespaceString, ZF_CALLER_LINE, \
        ReturnType, methodNameString \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        , _ZFP_MtdP_EXPAND(ZFMP_6) \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethodFuncUserRegister_0 */
#define ZFMethodFuncUserRegisterDetail_7(resultMethod, methodInvokerAction, \
    methodNamespaceString, RegSig, \
    ReturnType, methodNameString \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    ) \
    _ZFP_ZFMethodFuncUserRegister(resultMethod, methodInvokerAction, \
        methodNamespaceString, RegSig, \
        ReturnType, methodNameString \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        , _ZFP_MtdP_EXPAND(ZFMP_6) \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethodFuncUserRegister_0 */
#define ZFMETHOD_FUNC_USER_REGISTER_7(methodInvokerAction, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    ) \
    _ZFP_ZFMETHOD_FUNC_USER_REGISTER(methodInvokerAction, \
        ZF_NAMESPACE_CURRENT(), ZF_CALLER_LINE, \
        ReturnType, methodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        , _ZFP_MtdP_EXPAND(ZFMP_6) \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethodFuncUserRegister_0 */
#define ZFMETHOD_FUNC_USER_REGISTER_DETAIL_7(methodInvokerAction, \
    methodNamespaceString, RegSig, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    ) \
    _ZFP_ZFMETHOD_FUNC_USER_REGISTER(methodInvokerAction, \
        methodNamespaceString, RegSig, \
        ReturnType, methodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        , _ZFP_MtdP_EXPAND(ZFMP_6) \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMethodFuncUserRegister_0 */
#define ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_7( \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    ) \
    _ZFP_ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC( \
        ReturnType, methodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        , _ZFP_MtdP_EXPAND(ZFMP_6) \
        , _ZFP_ZFMP_DUMMY() \
        )

// ============================================================
// 8 param
/** @brief see #ZFMethodFuncUserRegister_0 */
#define ZFMethodFuncUserRegister_8(resultMethod, methodInvokerAction, \
    methodNamespaceString, \
    ReturnType, methodNameString \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    , ZFMP_7 \
    ) \
    _ZFP_ZFMethodFuncUserRegister(resultMethod, methodInvokerAction, \
        methodNamespaceString, ZF_CALLER_LINE, \
        ReturnType, methodNameString \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        , _ZFP_MtdP_EXPAND(ZFMP_6) \
        , _ZFP_MtdP_EXPAND(ZFMP_7) \
        )
/** @brief see #ZFMethodFuncUserRegister_0 */
#define ZFMethodFuncUserRegisterDetail_8(resultMethod, methodInvokerAction, \
    methodNamespaceString, RegSig, \
    ReturnType, methodNameString \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    , ZFMP_7 \
    ) \
    _ZFP_ZFMethodFuncUserRegister(resultMethod, methodInvokerAction, \
        methodNamespaceString, RegSig, \
        ReturnType, methodNameString \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        , _ZFP_MtdP_EXPAND(ZFMP_6) \
        , _ZFP_MtdP_EXPAND(ZFMP_7) \
        )
/** @brief see #ZFMethodFuncUserRegister_0 */
#define ZFMETHOD_FUNC_USER_REGISTER_8(methodInvokerAction, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    , ZFMP_7 \
    ) \
    _ZFP_ZFMETHOD_FUNC_USER_REGISTER(methodInvokerAction, \
        ZF_NAMESPACE_CURRENT(), ZF_CALLER_LINE, \
        ReturnType, methodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        , _ZFP_MtdP_EXPAND(ZFMP_6) \
        , _ZFP_MtdP_EXPAND(ZFMP_7) \
        )
/** @brief see #ZFMethodFuncUserRegister_0 */
#define ZFMETHOD_FUNC_USER_REGISTER_DETAIL_8(methodInvokerAction, \
    methodNamespaceString, RegSig, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    , ZFMP_7 \
    ) \
    _ZFP_ZFMETHOD_FUNC_USER_REGISTER(methodInvokerAction, \
        methodNamespaceString, RegSig, \
        ReturnType, methodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        , _ZFP_MtdP_EXPAND(ZFMP_6) \
        , _ZFP_MtdP_EXPAND(ZFMP_7) \
        )
/** @brief see #ZFMethodFuncUserRegister_0 */
#define ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_8( \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    , ZFMP_7 \
    ) \
    _ZFP_ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC( \
        ReturnType, methodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        , _ZFP_MtdP_EXPAND(ZFMP_6) \
        , _ZFP_MtdP_EXPAND(ZFMP_7) \
        )

/* ZFMETHOD_MAX_PARAM */

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFMethodFuncUserRegister_h_

