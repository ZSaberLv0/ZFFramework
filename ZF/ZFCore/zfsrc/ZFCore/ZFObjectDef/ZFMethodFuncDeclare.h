/**
 * @file ZFMethodFuncDeclare.h
 * @brief ZFMethod declaration
 */

#ifndef _ZFI_ZFMethodFuncDeclare_h_
#define _ZFI_ZFMethodFuncDeclare_h_

#include "ZFMethod.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
extern ZF_ENV_EXPORT void _ZFP_ZFMethodFuncRegister(ZF_IN const ZFMethod *method);
extern ZF_ENV_EXPORT void _ZFP_ZFMethodFuncUnregister(ZF_IN const ZFMethod *method);
extern ZF_ENV_EXPORT const ZFMethod *_ZFP_ZFMethodFuncForName(ZF_IN const zfchar *methodNamespace,
                                                              ZF_IN const zfchar *methodName);
extern ZF_ENV_EXPORT const ZFMethod *_ZFP_ZFMethodFuncForName(ZF_IN const zfchar *methodNamespace,
                                                              ZF_IN const zfchar *methodName
                                                              , ZF_IN_OPT const zfchar *methodParamTypeId0
                                                              , ZF_IN_OPT const zfchar *methodParamTypeId1 = zfnull
                                                              , ZF_IN_OPT const zfchar *methodParamTypeId2 = zfnull
                                                              , ZF_IN_OPT const zfchar *methodParamTypeId3 = zfnull
                                                              , ZF_IN_OPT const zfchar *methodParamTypeId4 = zfnull
                                                              , ZF_IN_OPT const zfchar *methodParamTypeId5 = zfnull
                                                              , ZF_IN_OPT const zfchar *methodParamTypeId6 = zfnull
                                                              , ZF_IN_OPT const zfchar *methodParamTypeId7 = zfnull
                                                              );
extern ZF_ENV_EXPORT void _ZFP_ZFMethodFuncForNameGetAllT(ZF_IN_OUT ZFCoreArray<const ZFMethod *> &ret,
                                                          ZF_IN const zfchar *methodNamespace,
                                                          ZF_IN const zfchar *methodName);

/** @brief see #ZFMethodGetAllFunc */
extern ZF_ENV_EXPORT void ZFMethodGetAllFuncT(ZF_IN_OUT ZFCoreArray<const ZFMethod *> &ret,
                                              ZF_IN_OPT const ZFFilterForZFMethod *filter = zfnull);
/**
 * @brief get all function type method currently registered, for debug use only
 */
inline ZFCoreArrayPOD<const ZFMethod *> ZFMethodGetAllFunc(ZF_IN_OPT const ZFFilterForZFMethod *filter = zfnull)
{
    ZFCoreArrayPOD<const ZFMethod *> ret;
    ZFMethodGetAllFuncT(ret, filter);
    return ret;
}

// ============================================================
#define _ZFP_ZFMethodFuncAccess(MethodNamespace, MethodName) \
    _ZFP_ZFMethodFuncAccess_(MethodNamespace, MethodName)
#define _ZFP_ZFMethodFuncAccess_(MethodNamespace, MethodName) \
    (MethodNamespace::_ZFP_MtdFA_##MethodName(zfnull))
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMethodFuncAccess(MethodName) \
    _ZFP_ZFMethodFuncAccess(ZF_NAMESPACE_GLOBAL_ID, MethodName)

#define _ZFP_ZFMethodFuncAccessDetail(...) \
    _ZFP_MtdD_EXPAND(_ZFP_ZFMethodFuncAccessDetail_(__VA_ARGS__))
#define _ZFP_ZFMethodFuncAccessDetail_(MethodNamespace, MethodName \
        , ParamExpandOrEmpty0, ParamType0, param0, DefaultExpandOrEmpty0, DefaultValueFix0 \
        , ParamExpandOrEmpty1, ParamType1, param1, DefaultExpandOrEmpty1, DefaultValueFix1 \
        , ParamExpandOrEmpty2, ParamType2, param2, DefaultExpandOrEmpty2, DefaultValueFix2 \
        , ParamExpandOrEmpty3, ParamType3, param3, DefaultExpandOrEmpty3, DefaultValueFix3 \
        , ParamExpandOrEmpty4, ParamType4, param4, DefaultExpandOrEmpty4, DefaultValueFix4 \
        , ParamExpandOrEmpty5, ParamType5, param5, DefaultExpandOrEmpty5, DefaultValueFix5 \
        , ParamExpandOrEmpty6, ParamType6, param6, DefaultExpandOrEmpty6, DefaultValueFix6 \
        , ParamExpandOrEmpty7, ParamType7, param7, DefaultExpandOrEmpty7, DefaultValueFix7 \
    ) \
    (MethodNamespace::_ZFP_MtdFA_##MethodName((void (*)( \
           ParamExpandOrEmpty0(ZFM_EMPTY() ParamType0) \
           ParamExpandOrEmpty1(ZFM_COMMA() ParamType1) \
           ParamExpandOrEmpty2(ZFM_COMMA() ParamType2) \
           ParamExpandOrEmpty3(ZFM_COMMA() ParamType3) \
           ParamExpandOrEmpty4(ZFM_COMMA() ParamType4) \
           ParamExpandOrEmpty5(ZFM_COMMA() ParamType5) \
           ParamExpandOrEmpty6(ZFM_COMMA() ParamType6) \
           ParamExpandOrEmpty7(ZFM_COMMA() ParamType7) \
        ))zfnull))
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMethodFuncAccessDetail_0(MethodNamespace, MethodName \
    ) \
    _ZFP_ZFMethodFuncAccessDetail(MethodNamespace, MethodName \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMethodFuncAccessDetail_1(MethodNamespace, MethodName \
        , ZFMP_0 \
    ) \
    _ZFP_ZFMethodFuncAccessDetail(MethodNamespace, MethodName \
            , _ZFP_MtdP_EXPAND(ZFMP_0) \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMethodFuncAccessDetail_2(MethodNamespace, MethodName \
        , ZFMP_0 \
        , ZFMP_1 \
    ) \
    _ZFP_ZFMethodFuncAccessDetail(MethodNamespace, MethodName \
            , _ZFP_MtdP_EXPAND(ZFMP_0) \
            , _ZFP_MtdP_EXPAND(ZFMP_1) \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMethodFuncAccessDetail_3(MethodNamespace, MethodName \
        , ZFMP_0 \
        , ZFMP_1 \
        , ZFMP_2 \
    ) \
    _ZFP_ZFMethodFuncAccessDetail(MethodNamespace, MethodName \
            , _ZFP_MtdP_EXPAND(ZFMP_0) \
            , _ZFP_MtdP_EXPAND(ZFMP_1) \
            , _ZFP_MtdP_EXPAND(ZFMP_2) \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMethodFuncAccessDetail_4(MethodNamespace, MethodName \
        , ZFMP_0 \
        , ZFMP_1 \
        , ZFMP_2 \
        , ZFMP_3 \
    ) \
    _ZFP_ZFMethodFuncAccessDetail(MethodNamespace, MethodName \
            , _ZFP_MtdP_EXPAND(ZFMP_0) \
            , _ZFP_MtdP_EXPAND(ZFMP_1) \
            , _ZFP_MtdP_EXPAND(ZFMP_2) \
            , _ZFP_MtdP_EXPAND(ZFMP_3) \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMethodFuncAccessDetail_5(MethodNamespace, MethodName \
        , ZFMP_0 \
        , ZFMP_1 \
        , ZFMP_2 \
        , ZFMP_3 \
        , ZFMP_4 \
    ) \
    _ZFP_ZFMethodFuncAccessDetail(MethodNamespace, MethodName \
            , _ZFP_MtdP_EXPAND(ZFMP_0) \
            , _ZFP_MtdP_EXPAND(ZFMP_1) \
            , _ZFP_MtdP_EXPAND(ZFMP_2) \
            , _ZFP_MtdP_EXPAND(ZFMP_3) \
            , _ZFP_MtdP_EXPAND(ZFMP_4) \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMethodFuncAccessDetail_6(MethodNamespace, MethodName \
        , ZFMP_0 \
        , ZFMP_1 \
        , ZFMP_2 \
        , ZFMP_3 \
        , ZFMP_4 \
        , ZFMP_5 \
    ) \
    _ZFP_ZFMethodFuncAccessDetail(MethodNamespace, MethodName \
            , _ZFP_MtdP_EXPAND(ZFMP_0) \
            , _ZFP_MtdP_EXPAND(ZFMP_1) \
            , _ZFP_MtdP_EXPAND(ZFMP_2) \
            , _ZFP_MtdP_EXPAND(ZFMP_3) \
            , _ZFP_MtdP_EXPAND(ZFMP_4) \
            , _ZFP_MtdP_EXPAND(ZFMP_5) \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMethodFuncAccessDetail_7(MethodNamespace, MethodName \
        , ZFMP_0 \
        , ZFMP_1 \
        , ZFMP_2 \
        , ZFMP_3 \
        , ZFMP_4 \
        , ZFMP_5 \
        , ZFMP_6 \
    ) \
    _ZFP_ZFMethodFuncAccessDetail(MethodNamespace, MethodName \
            , _ZFP_MtdP_EXPAND(ZFMP_0) \
            , _ZFP_MtdP_EXPAND(ZFMP_1) \
            , _ZFP_MtdP_EXPAND(ZFMP_2) \
            , _ZFP_MtdP_EXPAND(ZFMP_3) \
            , _ZFP_MtdP_EXPAND(ZFMP_4) \
            , _ZFP_MtdP_EXPAND(ZFMP_5) \
            , _ZFP_MtdP_EXPAND(ZFMP_6) \
            , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMethodFuncAccessDetail_8(MethodNamespace, MethodName \
        , ZFMP_0 \
        , ZFMP_1 \
        , ZFMP_2 \
        , ZFMP_3 \
        , ZFMP_4 \
        , ZFMP_5 \
        , ZFMP_6 \
        , ZFMP_7 \
    ) \
    _ZFP_ZFMethodFuncAccessDetail(MethodNamespace, MethodName \
            , _ZFP_MtdP_EXPAND(ZFMP_0) \
            , _ZFP_MtdP_EXPAND(ZFMP_1) \
            , _ZFP_MtdP_EXPAND(ZFMP_2) \
            , _ZFP_MtdP_EXPAND(ZFMP_3) \
            , _ZFP_MtdP_EXPAND(ZFMP_4) \
            , _ZFP_MtdP_EXPAND(ZFMP_5) \
            , _ZFP_MtdP_EXPAND(ZFMP_6) \
            , _ZFP_MtdP_EXPAND(ZFMP_7) \
        )

// ============================================================
#define _ZFP_ZFMethodFuncIsInline isInline
#define _ZFP_ZFMethodFuncNotInline notInline

#define _ZFP_ZFMETHOD_FUNC_DECLARE_inlinePrefix_isInline() inline
#define _ZFP_ZFMETHOD_FUNC_DECLARE_inlinePrefix_notInline() extern ZF_ENV_EXPORT
#define _ZFP_ZFMETHOD_FUNC_DECLARE_inlinePrefix(isInlineOrNot) _ZFP_ZFMETHOD_FUNC_DECLARE_inlinePrefix_##isInlineOrNot()

#define _ZFP_ZFMETHOD_FUNC_DECLARE_inlinePostfix_isInline()
#define _ZFP_ZFMETHOD_FUNC_DECLARE_inlinePostfix_notInline() ;
#define _ZFP_ZFMETHOD_FUNC_DECLARE_inlinePostfix(isInlineOrNot) _ZFP_ZFMETHOD_FUNC_DECLARE_inlinePostfix_##isInlineOrNot()

#define _ZFP_ZFMETHOD_FUNC_DEFINE_inlinePrefix_isInline() inline
#define _ZFP_ZFMETHOD_FUNC_DEFINE_inlinePrefix_notInline() extern ZF_ENV_EXPORT
#define _ZFP_ZFMETHOD_FUNC_DEFINE_inlinePrefix(isInlineOrNot) _ZFP_ZFMETHOD_FUNC_DEFINE_inlinePrefix_##isInlineOrNot()

#define _ZFP_ZFMETHOD_FUNC_DEFINE_inlinePostfix_isInline() ;
#define _ZFP_ZFMETHOD_FUNC_DEFINE_inlinePostfix_notInline()
#define _ZFP_ZFMETHOD_FUNC_DEFINE_inlinePostfix(isInlineOrNot) _ZFP_ZFMETHOD_FUNC_DEFINE_inlinePostfix_##isInlineOrNot()

// ============================================================
#define _ZFP_ZFMETHOD_FUNC_DECLARE(...) \
    _ZFP_MtdD_EXPAND(_ZFP_ZFMETHOD_FUNC_DECLARE_(__VA_ARGS__))
#define _ZFP_ZFMETHOD_FUNC_DECLARE_( \
        isInlineOrNot, RegSig, \
        ReturnType, MethodName \
        , ParamExpandOrEmpty0, ParamType0, param0, DefaultExpandOrEmpty0, DefaultValueFix0 \
        , ParamExpandOrEmpty1, ParamType1, param1, DefaultExpandOrEmpty1, DefaultValueFix1 \
        , ParamExpandOrEmpty2, ParamType2, param2, DefaultExpandOrEmpty2, DefaultValueFix2 \
        , ParamExpandOrEmpty3, ParamType3, param3, DefaultExpandOrEmpty3, DefaultValueFix3 \
        , ParamExpandOrEmpty4, ParamType4, param4, DefaultExpandOrEmpty4, DefaultValueFix4 \
        , ParamExpandOrEmpty5, ParamType5, param5, DefaultExpandOrEmpty5, DefaultValueFix5 \
        , ParamExpandOrEmpty6, ParamType6, param6, DefaultExpandOrEmpty6, DefaultValueFix6 \
        , ParamExpandOrEmpty7, ParamType7, param7, DefaultExpandOrEmpty7, DefaultValueFix7 \
    ) \
    /** @cond ZFPrivateDoc */ \
    extern ZF_ENV_EXPORT const ZFMethod *_ZFP_MtdFA_##MethodName(void (*)( \
            ParamExpandOrEmpty0(ZFM_EMPTY() ParamType0) \
            ParamExpandOrEmpty1(ZFM_COMMA() ParamType1) \
            ParamExpandOrEmpty2(ZFM_COMMA() ParamType2) \
            ParamExpandOrEmpty3(ZFM_COMMA() ParamType3) \
            ParamExpandOrEmpty4(ZFM_COMMA() ParamType4) \
            ParamExpandOrEmpty5(ZFM_COMMA() ParamType5) \
            ParamExpandOrEmpty6(ZFM_COMMA() ParamType6) \
            ParamExpandOrEmpty7(ZFM_COMMA() ParamType7) \
        )); \
    /** @endcond */ \
    _ZFP_ZFMETHOD_FUNC_DECLARE_inlinePrefix(isInlineOrNot) ReturnType (MethodName)( \
            ParamExpandOrEmpty0(            ParamType0 param0 DefaultValueFix0()) \
            ParamExpandOrEmpty1(ZFM_COMMA() ParamType1 param1 DefaultValueFix1()) \
            ParamExpandOrEmpty2(ZFM_COMMA() ParamType2 param2 DefaultValueFix2()) \
            ParamExpandOrEmpty3(ZFM_COMMA() ParamType3 param3 DefaultValueFix3()) \
            ParamExpandOrEmpty4(ZFM_COMMA() ParamType4 param4 DefaultValueFix4()) \
            ParamExpandOrEmpty5(ZFM_COMMA() ParamType5 param5 DefaultValueFix5()) \
            ParamExpandOrEmpty6(ZFM_COMMA() ParamType6 param6 DefaultValueFix6()) \
            ParamExpandOrEmpty7(ZFM_COMMA() ParamType7 param7 DefaultValueFix7()) \
        ) _ZFP_ZFMETHOD_FUNC_DECLARE_inlinePostfix(isInlineOrNot)

#define _ZFP_ZFMETHOD_FUNC_DEFINE(...) \
    _ZFP_MtdD_EXPAND(_ZFP_ZFMETHOD_FUNC_DEFINE_(__VA_ARGS__))
#define _ZFP_ZFMETHOD_FUNC_DEFINE_( \
        isInlineOrNot, RegSig, \
        ReturnType, MethodName \
        , ParamExpandOrEmpty0, ParamType0, param0, DefaultExpandOrEmpty0, DefaultValueFix0 \
        , ParamExpandOrEmpty1, ParamType1, param1, DefaultExpandOrEmpty1, DefaultValueFix1 \
        , ParamExpandOrEmpty2, ParamType2, param2, DefaultExpandOrEmpty2, DefaultValueFix2 \
        , ParamExpandOrEmpty3, ParamType3, param3, DefaultExpandOrEmpty3, DefaultValueFix3 \
        , ParamExpandOrEmpty4, ParamType4, param4, DefaultExpandOrEmpty4, DefaultValueFix4 \
        , ParamExpandOrEmpty5, ParamType5, param5, DefaultExpandOrEmpty5, DefaultValueFix5 \
        , ParamExpandOrEmpty6, ParamType6, param6, DefaultExpandOrEmpty6, DefaultValueFix6 \
        , ParamExpandOrEmpty7, ParamType7, param7, DefaultExpandOrEmpty7, DefaultValueFix7 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE_inlinePrefix(isInlineOrNot) ReturnType (MethodName)( \
            ParamExpandOrEmpty0(            ParamType0 param0) \
            ParamExpandOrEmpty1(ZFM_COMMA() ParamType1 param1) \
            ParamExpandOrEmpty2(ZFM_COMMA() ParamType2 param2) \
            ParamExpandOrEmpty3(ZFM_COMMA() ParamType3 param3) \
            ParamExpandOrEmpty4(ZFM_COMMA() ParamType4 param4) \
            ParamExpandOrEmpty5(ZFM_COMMA() ParamType5 param5) \
            ParamExpandOrEmpty6(ZFM_COMMA() ParamType6 param6) \
            ParamExpandOrEmpty7(ZFM_COMMA() ParamType7 param7) \
        ); \
    zfclassNotPOD ZF_ENV_EXPORT _ZFP_MtdFH_##MethodName##_##RegSig \
    { \
    public: \
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
        static ReturnType methodInvoker( \
                ZF_IN const ZFMethod *invokerMethod, \
                ZF_IN ZFObject *invokerObject \
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
            return (MethodName)( \
                    ParamExpandOrEmpty0(            param0) \
                    ParamExpandOrEmpty1(ZFM_COMMA() param1) \
                    ParamExpandOrEmpty2(ZFM_COMMA() param2) \
                    ParamExpandOrEmpty3(ZFM_COMMA() param3) \
                    ParamExpandOrEmpty4(ZFM_COMMA() param4) \
                    ParamExpandOrEmpty5(ZFM_COMMA() param5) \
                    ParamExpandOrEmpty6(ZFM_COMMA() param6) \
                    ParamExpandOrEmpty7(ZFM_COMMA() param7) \
                ); \
        } \
    }; \
    const ZFMethod *_ZFP_MtdFA_##MethodName(void (*)( \
            ParamExpandOrEmpty0(ZFM_EMPTY() ParamType0) \
            ParamExpandOrEmpty1(ZFM_COMMA() ParamType1) \
            ParamExpandOrEmpty2(ZFM_COMMA() ParamType2) \
            ParamExpandOrEmpty3(ZFM_COMMA() ParamType3) \
            ParamExpandOrEmpty4(ZFM_COMMA() ParamType4) \
            ParamExpandOrEmpty5(ZFM_COMMA() ParamType5) \
            ParamExpandOrEmpty6(ZFM_COMMA() ParamType6) \
            ParamExpandOrEmpty7(ZFM_COMMA() ParamType7) \
        )) \
    { \
        static _ZFP_ZFMethodRegisterHolder _methodHolder(zffalse \
                , zffalse \
                , zfnull \
                , ZFCastReinterpret(ZFFuncAddrType, &_ZFP_MtdFH_##MethodName##_##RegSig::methodInvoker) \
                , _ZFP_ZFMETHOD_GENERIC_INVOKER_ADDR(_ZFP_MtdFH_##MethodName##_##RegSig) \
                , _ZFP_ZFMethodTypeText(ZFMethodTypeStatic) \
                , zfnull \
                , ZFMethodPrivilegeTypePublic \
                , ZF_NAMESPACE_CURRENT() \
                , ZFM_TOSTRING(MethodName) \
                , ZFTypeId<zftTraits<ReturnType>::TrNoRef>::TypeId() \
                , ZFM_TOSTRING_DIRECT(ReturnType) \
                ParamExpandOrEmpty0(ZFM_EMPTY() \
                        , ZFTypeId<zftTraits<ParamType0>::TrNoRef>::TypeId() \
                        , ZFM_TOSTRING(ParamType0) \
                        , ZFM_TOSTRING(param0) \
                        , _ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS_ADDR(_ZFP_MtdFH_##MethodName##_##RegSig, DefaultExpandOrEmpty0, 0) \
                    ) \
                ParamExpandOrEmpty1(ZFM_EMPTY() \
                        , ZFTypeId<zftTraits<ParamType1>::TrNoRef>::TypeId() \
                        , ZFM_TOSTRING(ParamType1) \
                        , ZFM_TOSTRING(param1) \
                        , _ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS_ADDR(_ZFP_MtdFH_##MethodName##_##RegSig, DefaultExpandOrEmpty1, 1) \
                    ) \
                ParamExpandOrEmpty2(ZFM_EMPTY() \
                        , ZFTypeId<zftTraits<ParamType2>::TrNoRef>::TypeId() \
                        , ZFM_TOSTRING(ParamType2) \
                        , ZFM_TOSTRING(param2) \
                        , _ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS_ADDR(_ZFP_MtdFH_##MethodName##_##RegSig, DefaultExpandOrEmpty2, 2) \
                    ) \
                ParamExpandOrEmpty3(ZFM_EMPTY() \
                        , ZFTypeId<zftTraits<ParamType3>::TrNoRef>::TypeId() \
                        , ZFM_TOSTRING(ParamType3) \
                        , ZFM_TOSTRING(param3) \
                        , _ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS_ADDR(_ZFP_MtdFH_##MethodName##_##RegSig, DefaultExpandOrEmpty3, 3) \
                    ) \
                ParamExpandOrEmpty4(ZFM_EMPTY() \
                        , ZFTypeId<zftTraits<ParamType4>::TrNoRef>::TypeId() \
                        , ZFM_TOSTRING(ParamType4) \
                        , ZFM_TOSTRING(param4) \
                        , _ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS_ADDR(_ZFP_MtdFH_##MethodName##_##RegSig, DefaultExpandOrEmpty4, 4) \
                    ) \
                ParamExpandOrEmpty5(ZFM_EMPTY() \
                        , ZFTypeId<zftTraits<ParamType5>::TrNoRef>::TypeId() \
                        , ZFM_TOSTRING(ParamType5) \
                        , ZFM_TOSTRING(param5) \
                        , _ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS_ADDR(_ZFP_MtdFH_##MethodName##_##RegSig, DefaultExpandOrEmpty5, 5) \
                    ) \
                ParamExpandOrEmpty6(ZFM_EMPTY() \
                        , ZFTypeId<zftTraits<ParamType6>::TrNoRef>::TypeId() \
                        , ZFM_TOSTRING(ParamType6) \
                        , ZFM_TOSTRING(param6) \
                        , _ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS_ADDR(_ZFP_MtdFH_##MethodName##_##RegSig, DefaultExpandOrEmpty6, 6) \
                    ) \
                ParamExpandOrEmpty7(ZFM_EMPTY() \
                        , ZFTypeId<zftTraits<ParamType7>::TrNoRef>::TypeId() \
                        , ZFM_TOSTRING(ParamType7) \
                        , ZFM_TOSTRING(param7) \
                        , _ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS_ADDR(_ZFP_MtdFH_##MethodName##_##RegSig, DefaultExpandOrEmpty7, 7) \
                    ) \
                , zfnull \
            ); \
        return _methodHolder.method; \
    } \
    ZF_STATIC_REGISTER_INIT(MtdFR_##MethodName##_##RegSig) \
    { \
        (_ZFP_MtdFA_##MethodName((void (*)( \
               ParamExpandOrEmpty0(ZFM_EMPTY() ParamType0) \
               ParamExpandOrEmpty1(ZFM_COMMA() ParamType1) \
               ParamExpandOrEmpty2(ZFM_COMMA() ParamType2) \
               ParamExpandOrEmpty3(ZFM_COMMA() ParamType3) \
               ParamExpandOrEmpty4(ZFM_COMMA() ParamType4) \
               ParamExpandOrEmpty5(ZFM_COMMA() ParamType5) \
               ParamExpandOrEmpty6(ZFM_COMMA() ParamType6) \
               ParamExpandOrEmpty7(ZFM_COMMA() ParamType7) \
            ))zfnull)); \
    } \
    ZF_STATIC_REGISTER_END(MtdFR_##MethodName##_##RegSig) \
    _ZFP_ZFMETHOD_FUNC_DEFINE_inlinePrefix(isInlineOrNot) ReturnType (MethodName)( \
            ParamExpandOrEmpty0(            ParamType0 param0) \
            ParamExpandOrEmpty1(ZFM_COMMA() ParamType1 param1) \
            ParamExpandOrEmpty2(ZFM_COMMA() ParamType2 param2) \
            ParamExpandOrEmpty3(ZFM_COMMA() ParamType3 param3) \
            ParamExpandOrEmpty4(ZFM_COMMA() ParamType4 param4) \
            ParamExpandOrEmpty5(ZFM_COMMA() ParamType5 param5) \
            ParamExpandOrEmpty6(ZFM_COMMA() ParamType6 param6) \
            ParamExpandOrEmpty7(ZFM_COMMA() ParamType7 param7) \
        ) _ZFP_ZFMETHOD_FUNC_DEFINE_inlinePostfix(isInlineOrNot)

// ============================================================
// 0 param method declare
/**
 * @brief declare function type of #ZFMethod
 *
 * usage:
 * @code
 *   // in header file
 *   ZFMETHOD_FUNC_DECLARE_0(void, myFunc)
 *
 *   // in source file
 *   ZFMETHOD_FUNC_DEFINE_0(void, myFunc)
 *   {
 *       // func body
 *   }
 * @endcode
 *
 * similar to #ZFMETHOD_INLINE_0, but declared in global scope instead of class scope,
 * declared method can be reflected by #ZFMethodForName,
 * and can be invoked by by #ZFMethod::execute with null as first param\n
 * function type method support at most one level namespace
 */
#define ZFMETHOD_FUNC_DECLARE_0( \
    ReturnType, MethodName \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE( \
        _ZFP_ZFMethodFuncNotInline, ZF_CALLER_LINE, \
        ReturnType, MethodName \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_DECLARE_DETAIL_0(RegSig, \
    ReturnType, MethodName \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE( \
        _ZFP_ZFMethodFuncNotInline, RegSig, \
        ReturnType, MethodName \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_DEFINE_0( \
    ReturnType, MethodName \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncNotInline, ZF_CALLER_LINE, \
        ReturnType, MethodName \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_DEFINE_DETAIL_0(RegSig, \
    ReturnType, MethodName \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncNotInline, RegSig, \
        ReturnType, MethodName \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DECLARE_0( \
    ReturnType, MethodName \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE( \
        _ZFP_ZFMethodFuncIsInline, ZF_CALLER_LINE, \
        ReturnType, MethodName \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DECLARE_DETAIL_0(RegSig, \
    ReturnType, MethodName \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE( \
        _ZFP_ZFMethodFuncIsInline, RegSig, \
        ReturnType, MethodName \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DEFINE_0( \
    ReturnType, MethodName \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncIsInline, ZF_CALLER_LINE, \
        ReturnType, MethodName \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DEFINE_DETAIL_0(RegSig, \
    ReturnType, MethodName \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncIsInline, RegSig, \
        ReturnType, MethodName \
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
// 1 param method declare
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_DECLARE_1( \
    ReturnType, MethodName \
    , ZFMP_0 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE( \
        _ZFP_ZFMethodFuncNotInline, ZF_CALLER_LINE, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_DECLARE_DETAIL_1(RegSig, \
    ReturnType, MethodName \
    , ZFMP_0 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE( \
        _ZFP_ZFMethodFuncNotInline, RegSig, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_DEFINE_1( \
    ReturnType, MethodName \
    , ZFMP_0 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncNotInline, ZF_CALLER_LINE, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_DEFINE_DETAIL_1(RegSig, \
    ReturnType, MethodName \
    , ZFMP_0 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncNotInline, RegSig, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DECLARE_1( \
    ReturnType, MethodName \
    , ZFMP_0 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE( \
        _ZFP_ZFMethodFuncIsInline, ZF_CALLER_LINE, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DECLARE_DETAIL_1(RegSig, \
    ReturnType, MethodName \
    , ZFMP_0 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE( \
        _ZFP_ZFMethodFuncIsInline, RegSig, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DEFINE_1( \
    ReturnType, MethodName \
    , ZFMP_0 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncIsInline, ZF_CALLER_LINE, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DEFINE_DETAIL_1(RegSig, \
    ReturnType, MethodName \
    , ZFMP_0 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncIsInline, RegSig, \
        ReturnType, MethodName \
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
// 2 param method declare
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_DECLARE_2( \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE( \
        _ZFP_ZFMethodFuncNotInline, ZF_CALLER_LINE, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_DECLARE_DETAIL_2(RegSig, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE( \
        _ZFP_ZFMethodFuncNotInline, RegSig, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_DEFINE_2( \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncNotInline, ZF_CALLER_LINE, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_DEFINE_DETAIL_2(RegSig, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncNotInline, RegSig, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DECLARE_2( \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE( \
        _ZFP_ZFMethodFuncIsInline, ZF_CALLER_LINE, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DECLARE_DETAIL_2(RegSig, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE( \
        _ZFP_ZFMethodFuncIsInline, RegSig, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DEFINE_2( \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncIsInline, ZF_CALLER_LINE, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DEFINE_DETAIL_2(RegSig, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncIsInline, RegSig, \
        ReturnType, MethodName \
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
// 3 param method declare
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_DECLARE_3( \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE( \
        _ZFP_ZFMethodFuncNotInline, ZF_CALLER_LINE, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_DECLARE_DETAIL_3(RegSig, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE( \
        _ZFP_ZFMethodFuncNotInline, RegSig, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_DEFINE_3( \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncNotInline, ZF_CALLER_LINE, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_DEFINE_DETAIL_3(RegSig, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncNotInline, RegSig, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DECLARE_3( \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE( \
        _ZFP_ZFMethodFuncIsInline, ZF_CALLER_LINE, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DECLARE_DETAIL_3(RegSig, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE( \
        _ZFP_ZFMethodFuncIsInline, RegSig, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DEFINE_3( \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncIsInline, ZF_CALLER_LINE, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DEFINE_DETAIL_3(RegSig, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncIsInline, RegSig, \
        ReturnType, MethodName \
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
// 4 param method declare
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_DECLARE_4( \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE( \
        _ZFP_ZFMethodFuncNotInline, ZF_CALLER_LINE, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_DECLARE_DETAIL_4(RegSig, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE( \
        _ZFP_ZFMethodFuncNotInline, RegSig, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_DEFINE_4( \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncNotInline, ZF_CALLER_LINE, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_DEFINE_DETAIL_4(RegSig, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncNotInline, RegSig, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DECLARE_4( \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE( \
        _ZFP_ZFMethodFuncIsInline, ZF_CALLER_LINE, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DECLARE_DETAIL_4(RegSig, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE( \
        _ZFP_ZFMethodFuncIsInline, RegSig, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DEFINE_4( \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncIsInline, ZF_CALLER_LINE, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DEFINE_DETAIL_4(RegSig, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncIsInline, RegSig, \
        ReturnType, MethodName \
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
// 5 param method declare
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_DECLARE_5( \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE( \
        _ZFP_ZFMethodFuncNotInline, ZF_CALLER_LINE, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_DECLARE_DETAIL_5(RegSig, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE( \
        _ZFP_ZFMethodFuncNotInline, RegSig, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_DEFINE_5( \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncNotInline, ZF_CALLER_LINE, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_DEFINE_DETAIL_5(RegSig, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncNotInline, RegSig, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DECLARE_5( \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE( \
        _ZFP_ZFMethodFuncIsInline, ZF_CALLER_LINE, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DECLARE_DETAIL_5(RegSig, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE( \
        _ZFP_ZFMethodFuncIsInline, RegSig, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DEFINE_5( \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncIsInline, ZF_CALLER_LINE, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DEFINE_DETAIL_5(RegSig, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncIsInline, RegSig, \
        ReturnType, MethodName \
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
// 6 param method declare
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_DECLARE_6( \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE( \
        _ZFP_ZFMethodFuncNotInline, ZF_CALLER_LINE, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_DECLARE_DETAIL_6(RegSig, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE( \
        _ZFP_ZFMethodFuncNotInline, RegSig, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_DEFINE_6( \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncNotInline, ZF_CALLER_LINE, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_DEFINE_DETAIL_6(RegSig, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncNotInline, RegSig, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DECLARE_6( \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE( \
        _ZFP_ZFMethodFuncIsInline, ZF_CALLER_LINE, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DECLARE_DETAIL_6(RegSig, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE( \
        _ZFP_ZFMethodFuncIsInline, RegSig, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DEFINE_6( \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncIsInline, ZF_CALLER_LINE, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DEFINE_DETAIL_6(RegSig, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncIsInline, RegSig, \
        ReturnType, MethodName \
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
// 7 param method declare
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_DECLARE_7( \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE( \
        _ZFP_ZFMethodFuncNotInline, ZF_CALLER_LINE, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        , _ZFP_MtdP_EXPAND(ZFMP_6) \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_DECLARE_DETAIL_7(RegSig, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE( \
        _ZFP_ZFMethodFuncNotInline, RegSig, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        , _ZFP_MtdP_EXPAND(ZFMP_6) \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_DEFINE_7( \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncNotInline, ZF_CALLER_LINE, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        , _ZFP_MtdP_EXPAND(ZFMP_6) \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_DEFINE_DETAIL_7(RegSig, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncNotInline, RegSig, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        , _ZFP_MtdP_EXPAND(ZFMP_6) \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DECLARE_7( \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE( \
        _ZFP_ZFMethodFuncIsInline, ZF_CALLER_LINE, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        , _ZFP_MtdP_EXPAND(ZFMP_6) \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DECLARE_DETAIL_7(RegSig, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE( \
        _ZFP_ZFMethodFuncIsInline, RegSig, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        , _ZFP_MtdP_EXPAND(ZFMP_6) \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DEFINE_7( \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncIsInline, ZF_CALLER_LINE, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        , _ZFP_MtdP_EXPAND(ZFMP_6) \
        , _ZFP_ZFMP_DUMMY() \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DEFINE_DETAIL_7(RegSig, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncIsInline, RegSig, \
        ReturnType, MethodName \
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
// 8 param method declare
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_DECLARE_8( \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    , ZFMP_7 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE( \
        _ZFP_ZFMethodFuncNotInline, ZF_CALLER_LINE, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        , _ZFP_MtdP_EXPAND(ZFMP_6) \
        , _ZFP_MtdP_EXPAND(ZFMP_7) \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_DECLARE_DETAIL_8(RegSig, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    , ZFMP_7 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE( \
        _ZFP_ZFMethodFuncNotInline, RegSig, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        , _ZFP_MtdP_EXPAND(ZFMP_6) \
        , _ZFP_MtdP_EXPAND(ZFMP_7) \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_DEFINE_8( \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    , ZFMP_7 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncNotInline, ZF_CALLER_LINE, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        , _ZFP_MtdP_EXPAND(ZFMP_6) \
        , _ZFP_MtdP_EXPAND(ZFMP_7) \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_DEFINE_DETAIL_8(RegSig, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    , ZFMP_7 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncNotInline, RegSig, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        , _ZFP_MtdP_EXPAND(ZFMP_6) \
        , _ZFP_MtdP_EXPAND(ZFMP_7) \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DECLARE_8( \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    , ZFMP_7 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE( \
        _ZFP_ZFMethodFuncIsInline, ZF_CALLER_LINE, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        , _ZFP_MtdP_EXPAND(ZFMP_6) \
        , _ZFP_MtdP_EXPAND(ZFMP_7) \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DECLARE_DETAIL_8(RegSig, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    , ZFMP_7 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DECLARE( \
        _ZFP_ZFMethodFuncIsInline, RegSig, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        , _ZFP_MtdP_EXPAND(ZFMP_6) \
        , _ZFP_MtdP_EXPAND(ZFMP_7) \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DEFINE_8( \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    , ZFMP_7 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncIsInline, ZF_CALLER_LINE, \
        ReturnType, MethodName \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        , _ZFP_MtdP_EXPAND(ZFMP_6) \
        , _ZFP_MtdP_EXPAND(ZFMP_7) \
        )
/** @brief see #ZFMETHOD_FUNC_DECLARE_0 */
#define ZFMETHOD_FUNC_INLINE_DEFINE_DETAIL_8(RegSig, \
    ReturnType, MethodName \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    , ZFMP_7 \
    ) \
    _ZFP_ZFMETHOD_FUNC_DEFINE( \
        _ZFP_ZFMethodFuncIsInline, RegSig, \
        ReturnType, MethodName \
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
#endif // #ifndef _ZFI_ZFMethodFuncDeclare_h_

