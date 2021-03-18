/**
 * @file ZFMethodUserRegister.h
 * @brief user registered ZFMethod
 */

#ifndef _ZFI_ZFMethodUserRegister_h_
#define _ZFI_ZFMethodUserRegister_h_

#include "ZFMethod.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/** @brief see #ZFMethodUserRegister_0 */
extern ZF_ENV_EXPORT void ZFMethodUserUnregister(ZF_IN const ZFMethod *method);

// ============================================================
#define _ZFP_ZFMethodUserRegisterParamExpand( \
        GenericInvokerOwner, methodInvoker, ownerClass, \
        PublicOrProtectedOrPrivate, ZFMethodType_, \
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
        , _ZFP_ZFMethodTypeText(ZFMethodType_) \
        , ownerClass \
        , _ZFP_ZFMethod_initClassMemberType_privilege(PublicOrProtectedOrPrivate) \
        , zfnull \
        , methodNameString \
        , ZFTypeId<zftTraits<ReturnType>::TrNoRef>::TypeId() \
        , ZFM_TOSTRING_DIRECT(ReturnType) \
        ParamExpandOrEmpty0(ZFM_EMPTY() \
                , ZFTypeId<zftTraits<ParamType0>::TrNoRef>::TypeId() \
                , ZFM_TOSTRING(ParamType0) \
                , _ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS_ADDR(GenericInvokerOwner, DefaultExpandOrEmpty0, 0) \
            ) \
        ParamExpandOrEmpty1(ZFM_EMPTY() \
                , ZFTypeId<zftTraits<ParamType1>::TrNoRef>::TypeId() \
                , ZFM_TOSTRING(ParamType1) \
                , _ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS_ADDR(GenericInvokerOwner, DefaultExpandOrEmpty1, 1) \
            ) \
        ParamExpandOrEmpty2(ZFM_EMPTY() \
                , ZFTypeId<zftTraits<ParamType2>::TrNoRef>::TypeId() \
                , ZFM_TOSTRING(ParamType2) \
                , _ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS_ADDR(GenericInvokerOwner, DefaultExpandOrEmpty2, 2) \
            ) \
        ParamExpandOrEmpty3(ZFM_EMPTY() \
                , ZFTypeId<zftTraits<ParamType3>::TrNoRef>::TypeId() \
                , ZFM_TOSTRING(ParamType3) \
                , _ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS_ADDR(GenericInvokerOwner, DefaultExpandOrEmpty3, 3) \
            ) \
        ParamExpandOrEmpty4(ZFM_EMPTY() \
                , ZFTypeId<zftTraits<ParamType4>::TrNoRef>::TypeId() \
                , ZFM_TOSTRING(ParamType4) \
                , _ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS_ADDR(GenericInvokerOwner, DefaultExpandOrEmpty4, 4) \
            ) \
        ParamExpandOrEmpty5(ZFM_EMPTY() \
                , ZFTypeId<zftTraits<ParamType5>::TrNoRef>::TypeId() \
                , ZFM_TOSTRING(ParamType5) \
                , _ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS_ADDR(GenericInvokerOwner, DefaultExpandOrEmpty5, 5) \
            ) \
        ParamExpandOrEmpty6(ZFM_EMPTY() \
                , ZFTypeId<zftTraits<ParamType6>::TrNoRef>::TypeId() \
                , ZFM_TOSTRING(ParamType6) \
                , _ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS_ADDR(GenericInvokerOwner, DefaultExpandOrEmpty6, 6) \
            ) \
        ParamExpandOrEmpty7(ZFM_EMPTY() \
                , ZFTypeId<zftTraits<ParamType7>::TrNoRef>::TypeId() \
                , ZFM_TOSTRING(ParamType7) \
                , _ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS_ADDR(GenericInvokerOwner, DefaultExpandOrEmpty7, 7) \
            ) \
        , zfnull \
    )

#define _ZFP_ZFMethodUserRegister(...) \
    _ZFP_MtdD_EXPAND(_ZFP_ZFMethodUserRegister_(__VA_ARGS__))
#define _ZFP_ZFMethodUserRegister_(resultMethod, methodInvokerAction, ownerClass, \
        PublicOrProtectedOrPrivate, ZFMethodType_, \
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
        zfclassNotPOD _ZFP_MtdUR \
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
        resultMethod = _ZFP_ZFMethodRegister _ZFP_ZFMethodUserRegisterParamExpand( \
                _ZFP_MtdUR, _ZFP_MtdUR::invoker, ownerClass, \
                PublicOrProtectedOrPrivate, ZFMethodType_, \
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

// ============================================================
#define _ZFP_ZFMETHOD_USER_REGISTER(...) \
    _ZFP_MtdD_EXPAND(_ZFP_ZFMETHOD_USER_REGISTER_(__VA_ARGS__))
#define _ZFP_ZFMETHOD_USER_REGISTER_( \
        methodInvokerAction, ownerClassSig, \
        PublicOrProtectedOrPrivate, ZFMethodType_, RegSig, \
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
    zfclassNotPOD _ZFP_MtdURIvk_##ownerClassSig##_##methodName##_##RegSig \
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
    static _ZFP_ZFMethodRegisterHolder _ZFP_MtdURH_##ownerClassSig##_##methodName##_##RegSig \
        _ZFP_ZFMethodUserRegisterParamExpand( \
            _ZFP_MtdURIvk_##ownerClassSig##_##methodName##_##RegSig, \
            _ZFP_MtdURIvk_##ownerClassSig##_##methodName##_##RegSig::invoker, \
            ownerClassSig::ClassData(), \
            PublicOrProtectedOrPrivate, ZFMethodType_, \
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

#define _ZFP_ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC(...) \
    _ZFP_MtdD_EXPAND(_ZFP_ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_(__VA_ARGS__))
#define _ZFP_ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_( \
        ownerClassSig, \
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
    _ZFP_ZFMETHOD_USER_REGISTER({ \
                return (ReturnType)ZFCastZFObject(ownerClassSig *, invokerObject)->methodName( \
                        ParamExpandOrEmpty0(ZFM_EMPTY() param0) \
                        ParamExpandOrEmpty1(ZFM_COMMA() param1) \
                        ParamExpandOrEmpty2(ZFM_COMMA() param2) \
                        ParamExpandOrEmpty3(ZFM_COMMA() param3) \
                        ParamExpandOrEmpty4(ZFM_COMMA() param4) \
                        ParamExpandOrEmpty5(ZFM_COMMA() param5) \
                        ParamExpandOrEmpty6(ZFM_COMMA() param6) \
                        ParamExpandOrEmpty7(ZFM_COMMA() param7) \
                    ); \
            }, ownerClassSig, \
            public, ZFMethodTypeVirtual, ZF_CALLER_LINE, \
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

template<typename T_Wrapper>
inline T_Wrapper &_ZFP_MtdUR_zfv(T_Wrapper &zfv)
{
    return zfv;
};
template<typename T_Wrapper>
inline T_Wrapper const &_ZFP_MtdUR_zfv(T_Wrapper const &zfv)
{
    return zfv;
};
template<typename T_Wrapper>
inline T_Wrapper &_ZFP_MtdUR_zfv(T_Wrapper *zfv)
{
    return *zfv;
};
template<typename T_Wrapper>
inline T_Wrapper const &_ZFP_MtdUR_zfv(const T_Wrapper *zfv)
{
    return *zfv;
};
#define _ZFP_ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC(...) \
    _ZFP_MtdD_EXPAND(_ZFP_ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_(__VA_ARGS__))
#define _ZFP_ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_(WrapperClass, \
        RegSig, \
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
    _ZFP_ZFMETHOD_USER_REGISTER({ \
                return (ReturnType)_ZFP_MtdUR_zfv(invokerObject->to<WrapperClass *>()->zfv).methodName( \
                        ParamExpandOrEmpty0(ZFM_EMPTY() param0) \
                        ParamExpandOrEmpty1(ZFM_COMMA() param1) \
                        ParamExpandOrEmpty2(ZFM_COMMA() param2) \
                        ParamExpandOrEmpty3(ZFM_COMMA() param3) \
                        ParamExpandOrEmpty4(ZFM_COMMA() param4) \
                        ParamExpandOrEmpty5(ZFM_COMMA() param5) \
                        ParamExpandOrEmpty6(ZFM_COMMA() param6) \
                        ParamExpandOrEmpty7(ZFM_COMMA() param7) \
                    ); \
            }, WrapperClass, \
            public, ZFMethodTypeVirtual, RegSig, \
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

#define _ZFP_ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_STATIC(...) \
    _ZFP_MtdD_EXPAND(_ZFP_ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_STATIC_(__VA_ARGS__))
#define _ZFP_ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_STATIC_(OriginalClass, WrapperClass, \
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
    _ZFP_ZFMETHOD_USER_REGISTER({ \
                return (ReturnType)OriginalClass::methodName( \
                        ParamExpandOrEmpty0(ZFM_EMPTY() param0) \
                        ParamExpandOrEmpty1(ZFM_COMMA() param1) \
                        ParamExpandOrEmpty2(ZFM_COMMA() param2) \
                        ParamExpandOrEmpty3(ZFM_COMMA() param3) \
                        ParamExpandOrEmpty4(ZFM_COMMA() param4) \
                        ParamExpandOrEmpty5(ZFM_COMMA() param5) \
                        ParamExpandOrEmpty6(ZFM_COMMA() param6) \
                        ParamExpandOrEmpty7(ZFM_COMMA() param7) \
                    ); \
            }, WrapperClass, \
            public, ZFMethodTypeStatic, ZF_CALLER_LINE, \
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
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR_READONLY(ownerClassSig, VarType, VarName) \
    _ZFP_ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR_GETTER(ownerClassSig, VarType, VarName)
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ownerClassSig, VarType, VarName) \
    _ZFP_ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR_GETTER(ownerClassSig, VarType, VarName) \
    _ZFP_ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR_SETTER(ownerClassSig, VarType, VarName)

#define _ZFP_ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR_GETTER(ownerClassSig, VarType, VarName) \
    _ZFP_ZFMETHOD_USER_REGISTER({ \
            return (VarType const &)(invokerObject->to<ownerClassSig *>()->VarName); \
        }, ownerClassSig, public, ZFMethodTypeVirtual, G, VarType const &, VarName \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
#define _ZFP_ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR_SETTER(ownerClassSig, VarType, VarName) \
    _ZFP_ZFMETHOD_USER_REGISTER({ \
            invokerObject->to<ownerClassSig *>()->VarName = param0; \
        }, ownerClassSig, public, ZFMethodTypeVirtual, S, void, VarName \
        , _ZFP_MtdP_EXPAND(ZFMP_IN(VarType const &, param0)) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )

// ============================================================
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR_READONLY(WrapperClass, VarType, VarName) \
    _ZFP_ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR_GETTER(WrapperClass, VarType, VarName)
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(WrapperClass, VarType, VarName) \
    _ZFP_ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR_GETTER(WrapperClass, VarType, VarName) \
    _ZFP_ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR_SETTER(WrapperClass, VarType, VarName)

#define _ZFP_ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR_GETTER(WrapperClass, VarType, VarName) \
    _ZFP_ZFMETHOD_USER_REGISTER({ \
            return (VarType const &)(_ZFP_MtdUR_zfv(invokerObject->to<WrapperClass *>()->zfv).VarName); \
        }, WrapperClass, public, ZFMethodTypeVirtual, G, VarType const &, VarName \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
#define _ZFP_ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR_SETTER(WrapperClass, VarType, VarName) \
    _ZFP_ZFMETHOD_USER_REGISTER({ \
            _ZFP_MtdUR_zfv(invokerObject->to<WrapperClass *>()->zfv).VarName = param0; \
        }, WrapperClass, public, ZFMethodTypeVirtual, S, void, VarName \
        , _ZFP_MtdP_EXPAND(ZFMP_IN(VarType const &, param0)) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )

// ============================================================
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_FOR_WRAPPER_SETTER_GETTER(WrapperClass, VarType, VarName) \
    _ZFP_ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC(WrapperClass, G, \
        VarType const &, VarName \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        ) \
    _ZFP_ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC(WrapperClass, S, \
        void, VarName \
        , _ZFP_MtdP_EXPAND(ZFMP_IN(VarType const &, v)) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )

// ============================================================
// 0 param
/**
 * @brief register a custom method to existing class, for advanced reflection use only
 *
 * example:
 * @code
 *   // register it, using ZF_GLOBAL_INITIALIZER_INIT is recommended
 *   ZF_GLOBAL_INITIALIZER_INIT(MyMethodRegister)
 *   {
 *       ZFMethodUserRegisterDetail_2(resultMethod, {
 *               return (ReturnType)yourInvoker(invokerMethod, invokerObject, param0, param1);
 *           }, MyClassToAttachTheMethod::ClassData(),
 *           public, ZFMethodTypeStatic,
 *           ReturnType, "myMethodName"
 *           , ZFMP_IN(ParamType0, param0)
 *           , ZFMP_IN(ParamType1, param1));
 *       _method = resultMethod;
 *       zfLogTrimT() << resultMethod;
 *   }
 *   ZF_GLOBAL_INITIALIZER_DESTROY(MyMethodRegister)
 *   {
 *       ZFMethodUserUnregister(_method);
 *   }
 *   const ZFMethod *_method;
 *   ZF_GLOBAL_INITIALIZER_END(MyMethodRegister)
 *
 *   // or, you may use this macro for short, at cpp files only
 *   ZFMETHOD_USER_REGISTER_DETAIL_2({
 *           return (ReturnType)yourInvoker(invokerMethod, invokerObject, param0, param1);
 *       }, MyClassToAttachTheMethod::ClassData(),
 *       public, ZFMethodTypeStatic, ZF_CALLER_LINE,
 *       ReturnType, "myMethodName"
 *       , ZFMP_IN(ParamType0, param0)
 *       , ZFMP_IN(ParamType1, param1))
 * @endcode
 *
 * note:
 * -  if there is already a method exist with same method name and method id:
 *   -  if registered to same class, register would fail
 *   -  if registered to different class in inherit tree, child class's one would replace parent class
 * -  the registered method must be unregister manually at proper time,
 *   use #ZF_GLOBAL_INITIALIZER_INIT is recommeded
 * -  once registered, the method can be reflected by #ZFClass::methodForName,
 *   but there's no way to invoke the method by object directly,
 *   it can only be invoked by #ZFMethod::execute
 *
 * @see ZFMethodDynamicRegister
 */
#define ZFMethodUserRegister_0(resultMethod, methodInvokerAction, ownerClass, \
    ReturnType, methodNameString \
    ) \
    _ZFP_ZFMethodUserRegister(resultMethod, methodInvokerAction, ownerClass, \
        public, ZFMethodTypeVirtual, \
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
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMethodUserRegisterDetail_0(resultMethod, methodInvokerAction, ownerClass, \
    PublicOrProtectedOrPrivate, ZFMethodType_, \
    ReturnType, methodNameString \
    ) \
    _ZFP_ZFMethodUserRegister(resultMethod, methodInvokerAction, ownerClass, \
        PublicOrProtectedOrPrivate, ZFMethodType_, \
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
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_0(methodInvokerAction, ownerClassSig, \
    ReturnType, methodName \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER(methodInvokerAction, ownerClassSig, \
        public, ZFMethodTypeVirtual, ZF_CALLER_LINE, \
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
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_DETAIL_0(methodInvokerAction, ownerClassSig, \
    PublicOrProtectedOrPrivate, ZFMethodType_, RegSig, \
    ReturnType, methodName \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER(methodInvokerAction, ownerClassSig, \
        PublicOrProtectedOrPrivate, ZFMethodType_, RegSig, \
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
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ownerClassSig, \
    ReturnType, methodName \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC(ownerClassSig, \
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
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(WrapperClass, \
    ReturnType, methodName \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC(WrapperClass, ZF_CALLER_LINE, \
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
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_STATIC_0(OriginalClass, WrapperClass, \
    ReturnType, methodName \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_STATIC(OriginalClass, WrapperClass, \
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
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMethodUserRegister_1(resultMethod, methodInvokerAction, ownerClass, \
    ReturnType, methodNameString \
    , ZFMP_0 \
    ) \
    _ZFP_ZFMethodUserRegister(resultMethod, methodInvokerAction, ownerClass, \
        public, ZFMethodTypeVirtual, \
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
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMethodUserRegisterDetail_1(resultMethod, methodInvokerAction, ownerClass, \
    PublicOrProtectedOrPrivate, ZFMethodType_, \
    ReturnType, methodNameString \
    , ZFMP_0 \
    ) \
    _ZFP_ZFMethodUserRegister(resultMethod, methodInvokerAction, ownerClass, \
        PublicOrProtectedOrPrivate, ZFMethodType_, \
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
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_1(methodInvokerAction, ownerClassSig, \
    ReturnType, methodName \
    , ZFMP_0 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER(methodInvokerAction, ownerClassSig, \
        public, ZFMethodTypeVirtual, ZF_CALLER_LINE, \
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
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_DETAIL_1(methodInvokerAction, ownerClassSig, \
    PublicOrProtectedOrPrivate, ZFMethodType_, RegSig, \
    ReturnType, methodName \
    , ZFMP_0 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER(methodInvokerAction, ownerClassSig, \
        PublicOrProtectedOrPrivate, ZFMethodType_, RegSig, \
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
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ownerClassSig, \
    ReturnType, methodName \
    , ZFMP_0 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC(ownerClassSig, \
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
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(WrapperClass, \
    ReturnType, methodName \
    , ZFMP_0 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC(WrapperClass, ZF_CALLER_LINE, \
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
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_STATIC_1(OriginalClass, WrapperClass, \
    ReturnType, methodName \
    , ZFMP_0 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_STATIC(OriginalClass, WrapperClass, \
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
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMethodUserRegister_2(resultMethod, methodInvokerAction, ownerClass, \
    ReturnType, methodNameString \
    , ZFMP_0 \
    , ZFMP_1 \
    ) \
    _ZFP_ZFMethodUserRegister(resultMethod, methodInvokerAction, ownerClass, \
        public, ZFMethodTypeVirtual, \
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
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMethodUserRegisterDetail_2(resultMethod, methodInvokerAction, ownerClass, \
    PublicOrProtectedOrPrivate, ZFMethodType_, \
    ReturnType, methodNameString \
    , ZFMP_0 \
    , ZFMP_1 \
    ) \
    _ZFP_ZFMethodUserRegister(resultMethod, methodInvokerAction, ownerClass, \
        PublicOrProtectedOrPrivate, ZFMethodType_, \
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
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_2(methodInvokerAction, ownerClassSig, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER(methodInvokerAction, ownerClassSig, \
        public, ZFMethodTypeVirtual, ZF_CALLER_LINE, \
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
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_DETAIL_2(methodInvokerAction, ownerClassSig, \
    PublicOrProtectedOrPrivate, ZFMethodType_, RegSig, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER(methodInvokerAction, ownerClassSig, \
        PublicOrProtectedOrPrivate, ZFMethodType_, RegSig, \
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
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(ownerClassSig, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC(ownerClassSig, \
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
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(WrapperClass, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC(WrapperClass, ZF_CALLER_LINE, \
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
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_STATIC_2(OriginalClass, WrapperClass, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_STATIC(OriginalClass, WrapperClass, \
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
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMethodUserRegister_3(resultMethod, methodInvokerAction, ownerClass, \
    ReturnType, methodNameString \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    ) \
    _ZFP_ZFMethodUserRegister(resultMethod, methodInvokerAction, ownerClass, \
        public, ZFMethodTypeVirtual, \
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
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMethodUserRegisterDetail_3(resultMethod, methodInvokerAction, ownerClass, \
    PublicOrProtectedOrPrivate, ZFMethodType_, \
    ReturnType, methodNameString \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    ) \
    _ZFP_ZFMethodUserRegister(resultMethod, methodInvokerAction, ownerClass, \
        PublicOrProtectedOrPrivate, ZFMethodType_, \
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
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_3(methodInvokerAction, ownerClassSig, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER(methodInvokerAction, ownerClassSig, \
        public, ZFMethodTypeVirtual, ZF_CALLER_LINE, \
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
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_DETAIL_3(methodInvokerAction, ownerClassSig, \
    PublicOrProtectedOrPrivate, ZFMethodType_, RegSig, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER(methodInvokerAction, ownerClassSig, \
        PublicOrProtectedOrPrivate, ZFMethodType_, RegSig, \
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
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_3(ownerClassSig, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC(ownerClassSig, \
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
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_3(WrapperClass, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC(WrapperClass, ZF_CALLER_LINE, \
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
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_STATIC_3(OriginalClass, WrapperClass, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_STATIC(OriginalClass, WrapperClass, \
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
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMethodUserRegister_4(resultMethod, methodInvokerAction, ownerClass, \
    ReturnType, methodNameString \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    ) \
    _ZFP_ZFMethodUserRegister(resultMethod, methodInvokerAction, ownerClass, \
        public, ZFMethodTypeVirtual, \
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
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMethodUserRegisterDetail_4(resultMethod, methodInvokerAction, ownerClass, \
    PublicOrProtectedOrPrivate, ZFMethodType_, \
    ReturnType, methodNameString \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    ) \
    _ZFP_ZFMethodUserRegister(resultMethod, methodInvokerAction, ownerClass, \
        PublicOrProtectedOrPrivate, ZFMethodType_, \
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
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_4(methodInvokerAction, ownerClassSig, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER(methodInvokerAction, ownerClassSig, \
        public, ZFMethodTypeVirtual, ZF_CALLER_LINE, \
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
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_DETAIL_4(methodInvokerAction, ownerClassSig, \
    PublicOrProtectedOrPrivate, ZFMethodType_, RegSig, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER(methodInvokerAction, ownerClassSig, \
        PublicOrProtectedOrPrivate, ZFMethodType_, RegSig, \
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
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_4(ownerClassSig, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC(ownerClassSig, \
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
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_4(WrapperClass, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC(WrapperClass, ZF_CALLER_LINE, \
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
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_STATIC_4(OriginalClass, WrapperClass, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_STATIC(OriginalClass, WrapperClass, \
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
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMethodUserRegister_5(resultMethod, methodInvokerAction, ownerClass, \
    ReturnType, methodNameString \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    ) \
    _ZFP_ZFMethodUserRegister(resultMethod, methodInvokerAction, ownerClass, \
        public, ZFMethodTypeVirtual, \
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
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMethodUserRegisterDetail_5(resultMethod, methodInvokerAction, ownerClass, \
    PublicOrProtectedOrPrivate, ZFMethodType_, \
    ReturnType, methodNameString \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    ) \
    _ZFP_ZFMethodUserRegister(resultMethod, methodInvokerAction, ownerClass, \
        PublicOrProtectedOrPrivate, ZFMethodType_, \
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
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_5(methodInvokerAction, ownerClassSig, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER(methodInvokerAction, ownerClassSig, \
        public, ZFMethodTypeVirtual, ZF_CALLER_LINE, \
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
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_DETAIL_5(methodInvokerAction, ownerClassSig, \
    PublicOrProtectedOrPrivate, ZFMethodType_, RegSig, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER(methodInvokerAction, ownerClassSig, \
        PublicOrProtectedOrPrivate, ZFMethodType_, RegSig, \
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
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_5(ownerClassSig, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC(ownerClassSig, \
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
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_5(WrapperClass, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC(WrapperClass, ZF_CALLER_LINE, \
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
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_STATIC_5(OriginalClass, WrapperClass, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_STATIC(OriginalClass, WrapperClass, \
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
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMethodUserRegister_6(resultMethod, methodInvokerAction, ownerClass, \
    ReturnType, methodNameString \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    ) \
    _ZFP_ZFMethodUserRegister(resultMethod, methodInvokerAction, ownerClass, \
        public, ZFMethodTypeVirtual, \
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
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMethodUserRegisterDetail_6(resultMethod, methodInvokerAction, ownerClass, \
    PublicOrProtectedOrPrivate, ZFMethodType_, \
    ReturnType, methodNameString \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    ) \
    _ZFP_ZFMethodUserRegister(resultMethod, methodInvokerAction, ownerClass, \
        PublicOrProtectedOrPrivate, ZFMethodType_, \
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
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_6(methodInvokerAction, ownerClassSig, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER(methodInvokerAction, ownerClassSig, \
        public, ZFMethodTypeVirtual, ZF_CALLER_LINE, \
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
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_DETAIL_6(methodInvokerAction, ownerClassSig, \
    PublicOrProtectedOrPrivate, ZFMethodType_, RegSig, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER(methodInvokerAction, ownerClassSig, \
        PublicOrProtectedOrPrivate, ZFMethodType_, RegSig, \
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
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_6(ownerClassSig, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC(ownerClassSig, \
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
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_6(WrapperClass, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC(WrapperClass, ZF_CALLER_LINE, \
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
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_STATIC_6(OriginalClass, WrapperClass, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_STATIC(OriginalClass, WrapperClass, \
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
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMethodUserRegister_7(resultMethod, methodInvokerAction, ownerClass, \
    ReturnType, methodNameString \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    ) \
    _ZFP_ZFMethodUserRegister(resultMethod, methodInvokerAction, ownerClass, \
        public, ZFMethodTypeVirtual, \
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
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMethodUserRegisterDetail_7(resultMethod, methodInvokerAction, ownerClass, \
    PublicOrProtectedOrPrivate, ZFMethodType_, \
    ReturnType, methodNameString \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    ) \
    _ZFP_ZFMethodUserRegister(resultMethod, methodInvokerAction, ownerClass, \
        PublicOrProtectedOrPrivate, ZFMethodType_, \
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
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_7(methodInvokerAction, ownerClassSig, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER(methodInvokerAction, ownerClassSig, \
        public, ZFMethodTypeVirtual, ZF_CALLER_LINE, \
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
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_DETAIL_7(methodInvokerAction, ownerClassSig, \
    PublicOrProtectedOrPrivate, ZFMethodType_, RegSig, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER(methodInvokerAction, ownerClassSig, \
        PublicOrProtectedOrPrivate, ZFMethodType_, RegSig, \
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
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_7(ownerClassSig, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC(ownerClassSig, \
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
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_7(WrapperClass, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC(WrapperClass, ZF_CALLER_LINE, \
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
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_STATIC_7(OriginalClass, WrapperClass, \
    ReturnType, methodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    ) \
    _ZFP_ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_STATIC(OriginalClass, WrapperClass, \
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
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMethodUserRegister_8(resultMethod, methodInvokerAction, ownerClass, \
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
    _ZFP_ZFMethodUserRegister(resultMethod, methodInvokerAction, ownerClass, \
        public, ZFMethodTypeVirtual, \
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
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMethodUserRegisterDetail_8(resultMethod, methodInvokerAction, ownerClass, \
    PublicOrProtectedOrPrivate, ZFMethodType_, \
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
    _ZFP_ZFMethodUserRegister(resultMethod, methodInvokerAction, ownerClass, \
        PublicOrProtectedOrPrivate, ZFMethodType_, \
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
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_8(methodInvokerAction, ownerClassSig, \
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
    _ZFP_ZFMETHOD_USER_REGISTER(methodInvokerAction, ownerClassSig, \
        public, ZFMethodTypeVirtual, ZF_CALLER_LINE, \
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
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_DETAIL_8(methodInvokerAction, ownerClassSig, \
    PublicOrProtectedOrPrivate, ZFMethodType_, RegSig, \
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
    _ZFP_ZFMETHOD_USER_REGISTER(methodInvokerAction, ownerClassSig, \
        PublicOrProtectedOrPrivate, ZFMethodType_, RegSig, \
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
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_8(ownerClassSig, \
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
    _ZFP_ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC(ownerClassSig, \
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
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_8(WrapperClass, \
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
    _ZFP_ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC(WrapperClass, ZF_CALLER_LINE, \
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
/** @brief see #ZFMethodUserRegister_0 */
#define ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_STATIC_8(OriginalClass, WrapperClass, \
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
    _ZFP_ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_STATIC(OriginalClass, WrapperClass, \
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
#endif // #ifndef _ZFI_ZFMethodUserRegister_h_

