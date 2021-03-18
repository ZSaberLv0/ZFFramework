/**
 * @file ZFCallback_lambda.h
 * @brief header for ZFCallback
 */

#ifndef _ZFI_ZFCallback_lambda_h_
#define _ZFI_ZFCallback_lambda_h_

#include "ZFCallback.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief create a callback that simulate lambda function
 *
 * usage:
 * @code
 *   CaptureParam0 c0;
 *   CaptureParam1 c1;
 *   CaptureParam2 c2;
 *
 *   ZFLAMBDA_3(myCallback
 *           , CaptureParam0, c0
 *           , CaptureParam1 &, c1
 *           , CaptureParam2 const &, c2
 *       , {
 *           c1 = param1;
 *       }, ReturnType
 *           , ZFMP_IN(ParamType0, param0)
 *           , ZFMP_IN_OPT(ParamType1, param1, zfnull)
 *       )
 *
 *   myCallback.executeExact<ReturnType, ParamType0, ParamType1>(p0, p1);
 * @endcode
 */
#define ZFLAMBDA_0(name \
    , lambdaBody, ReturnType, ...) \
    _ZFP_ZFLAMBDA(name \
        , _ZFP_ZFLambdaCapture_EMPTY , CaptureParam0, capture0 \
        , _ZFP_ZFLambdaCapture_EMPTY , CaptureParam1, capture1 \
        , _ZFP_ZFLambdaCapture_EMPTY , CaptureParam2, capture2 \
        , _ZFP_ZFLambdaCapture_EMPTY , CaptureParam3, capture3 \
        , _ZFP_ZFLambdaCapture_EMPTY , CaptureParam4, capture4 \
        , _ZFP_ZFLambdaCapture_EMPTY , CaptureParam5, capture5 \
        , _ZFP_ZFLambdaCapture_EMPTY , CaptureParam6, capture6 \
        , _ZFP_ZFLambdaCapture_EMPTY , CaptureParam7, capture7 \
        , lambdaBody, ReturnType _ZFP_ZFLAMBDA_N_VA_EXPAND(__VA_ARGS__) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
    )

/** @brief see #ZFLAMBDA_0 */
#define ZFLAMBDA_1(name \
    , CaptureParam0, capture0 \
    , lambdaBody, ReturnType, ...) \
    _ZFP_ZFLAMBDA(name \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam0, capture0 \
        , _ZFP_ZFLambdaCapture_EMPTY , CaptureParam1, capture1 \
        , _ZFP_ZFLambdaCapture_EMPTY , CaptureParam2, capture2 \
        , _ZFP_ZFLambdaCapture_EMPTY , CaptureParam3, capture3 \
        , _ZFP_ZFLambdaCapture_EMPTY , CaptureParam4, capture4 \
        , _ZFP_ZFLambdaCapture_EMPTY , CaptureParam5, capture5 \
        , _ZFP_ZFLambdaCapture_EMPTY , CaptureParam6, capture6 \
        , _ZFP_ZFLambdaCapture_EMPTY , CaptureParam7, capture7 \
        , lambdaBody, ReturnType _ZFP_ZFLAMBDA_N_VA_EXPAND(__VA_ARGS__) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
    )

/** @brief see #ZFLAMBDA_0 */
#define ZFLAMBDA_2(name \
    , CaptureParam0, capture0 \
    , CaptureParam1, capture1 \
    , lambdaBody, ReturnType, ...) \
    _ZFP_ZFLAMBDA(name \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam0, capture0 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam1, capture1 \
        , _ZFP_ZFLambdaCapture_EMPTY , CaptureParam2, capture2 \
        , _ZFP_ZFLambdaCapture_EMPTY , CaptureParam3, capture3 \
        , _ZFP_ZFLambdaCapture_EMPTY , CaptureParam4, capture4 \
        , _ZFP_ZFLambdaCapture_EMPTY , CaptureParam5, capture5 \
        , _ZFP_ZFLambdaCapture_EMPTY , CaptureParam6, capture6 \
        , _ZFP_ZFLambdaCapture_EMPTY , CaptureParam7, capture7 \
        , lambdaBody, ReturnType _ZFP_ZFLAMBDA_N_VA_EXPAND(__VA_ARGS__) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
    )

/** @brief see #ZFLAMBDA_0 */
#define ZFLAMBDA_3(name \
    , CaptureParam0, capture0 \
    , CaptureParam1, capture1 \
    , CaptureParam2, capture2 \
    , lambdaBody, ReturnType, ...) \
    _ZFP_ZFLAMBDA(name \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam0, capture0 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam1, capture1 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam2, capture2 \
        , _ZFP_ZFLambdaCapture_EMPTY , CaptureParam3, capture3 \
        , _ZFP_ZFLambdaCapture_EMPTY , CaptureParam4, capture4 \
        , _ZFP_ZFLambdaCapture_EMPTY , CaptureParam5, capture5 \
        , _ZFP_ZFLambdaCapture_EMPTY , CaptureParam6, capture6 \
        , _ZFP_ZFLambdaCapture_EMPTY , CaptureParam7, capture7 \
        , lambdaBody, ReturnType _ZFP_ZFLAMBDA_N_VA_EXPAND(__VA_ARGS__) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
    )

/** @brief see #ZFLAMBDA_0 */
#define ZFLAMBDA_4(name \
    , CaptureParam0, capture0 \
    , CaptureParam1, capture1 \
    , CaptureParam2, capture2 \
    , CaptureParam3, capture3 \
    , lambdaBody, ReturnType, ...) \
    _ZFP_ZFLAMBDA(name \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam0, capture0 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam1, capture1 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam2, capture2 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam3, capture3 \
        , _ZFP_ZFLambdaCapture_EMPTY , CaptureParam4, capture4 \
        , _ZFP_ZFLambdaCapture_EMPTY , CaptureParam5, capture5 \
        , _ZFP_ZFLambdaCapture_EMPTY , CaptureParam6, capture6 \
        , _ZFP_ZFLambdaCapture_EMPTY , CaptureParam7, capture7 \
        , lambdaBody, ReturnType _ZFP_ZFLAMBDA_N_VA_EXPAND(__VA_ARGS__) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
    )

/** @brief see #ZFLAMBDA_0 */
#define ZFLAMBDA_5(name \
    , CaptureParam0, capture0 \
    , CaptureParam1, capture1 \
    , CaptureParam2, capture2 \
    , CaptureParam3, capture3 \
    , CaptureParam4, capture4 \
    , lambdaBody, ReturnType, ...) \
    _ZFP_ZFLAMBDA(name \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam0, capture0 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam1, capture1 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam2, capture2 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam3, capture3 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam4, capture4 \
        , _ZFP_ZFLambdaCapture_EMPTY , CaptureParam5, capture5 \
        , _ZFP_ZFLambdaCapture_EMPTY , CaptureParam6, capture6 \
        , _ZFP_ZFLambdaCapture_EMPTY , CaptureParam7, capture7 \
        , lambdaBody, ReturnType _ZFP_ZFLAMBDA_N_VA_EXPAND(__VA_ARGS__) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
    )

/** @brief see #ZFLAMBDA_0 */
#define ZFLAMBDA_6(name \
    , CaptureParam0, capture0 \
    , CaptureParam1, capture1 \
    , CaptureParam2, capture2 \
    , CaptureParam3, capture3 \
    , CaptureParam4, capture4 \
    , CaptureParam5, capture5 \
    , lambdaBody, ReturnType, ...) \
    _ZFP_ZFLAMBDA(name \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam0, capture0 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam1, capture1 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam2, capture2 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam3, capture3 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam4, capture4 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam5, capture5 \
        , _ZFP_ZFLambdaCapture_EMPTY , CaptureParam6, capture6 \
        , _ZFP_ZFLambdaCapture_EMPTY , CaptureParam7, capture7 \
        , lambdaBody, ReturnType _ZFP_ZFLAMBDA_N_VA_EXPAND(__VA_ARGS__) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
    )

/** @brief see #ZFLAMBDA_0 */
#define ZFLAMBDA_7(name \
    , CaptureParam0, capture0 \
    , CaptureParam1, capture1 \
    , CaptureParam2, capture2 \
    , CaptureParam3, capture3 \
    , CaptureParam4, capture4 \
    , CaptureParam5, capture5 \
    , CaptureParam6, capture6 \
    , lambdaBody, ReturnType, ...) \
    _ZFP_ZFLAMBDA(name \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam0, capture0 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam1, capture1 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam2, capture2 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam3, capture3 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam4, capture4 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam5, capture5 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam6, capture6 \
        , _ZFP_ZFLambdaCapture_EMPTY , CaptureParam7, capture7 \
        , lambdaBody, ReturnType _ZFP_ZFLAMBDA_N_VA_EXPAND(__VA_ARGS__) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
    )

/** @brief see #ZFLAMBDA_0 */
#define ZFLAMBDA_8(name \
    , CaptureParam0, capture0 \
    , CaptureParam1, capture1 \
    , CaptureParam2, capture2 \
    , CaptureParam3, capture3 \
    , CaptureParam4, capture4 \
    , CaptureParam5, capture5 \
    , CaptureParam6, capture6 \
    , CaptureParam7, capture7 \
    , lambdaBody, ReturnType, ...) \
    _ZFP_ZFLAMBDA(name \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam0, capture0 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam1, capture1 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam2, capture2 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam3, capture3 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam4, capture4 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam5, capture5 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam6, capture6 \
        , _ZFP_ZFLambdaCapture_EXPAND, CaptureParam7, capture7 \
        , lambdaBody, ReturnType _ZFP_ZFLAMBDA_N_VA_EXPAND(__VA_ARGS__) \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
        , _ZFP_ZFMP_DUMMY() \
    )

#define _ZFP_ZFLAMBDA_N_VA_EXPAND(...) , ##__VA_ARGS__

// ============================================================
#define _ZFP_ZFLambdaCapture_EXPAND(...) __VA_ARGS__
#define _ZFP_ZFLambdaCapture_EMPTY(...)

#define _ZFP_ZFLambdaBegin_EXPAND(...) __VA_ARGS__
#define _ZFP_ZFLambdaBegin(...) \
    _ZFP_ZFLambdaBegin_EXPAND(_ZFP_ZFLambdaBegin_(__VA_ARGS__))
#define _ZFP_ZFLambdaBegin_(ReturnType, name \
        , ParamExpandOrEmpty0, ParamType0, param0, DefaultExpandOrEmpty0, DefaultValueFix0 \
        , ParamExpandOrEmpty1, ParamType1, param1, DefaultExpandOrEmpty1, DefaultValueFix1 \
        , ParamExpandOrEmpty2, ParamType2, param2, DefaultExpandOrEmpty2, DefaultValueFix2 \
        , ParamExpandOrEmpty3, ParamType3, param3, DefaultExpandOrEmpty3, DefaultValueFix3 \
        , ParamExpandOrEmpty4, ParamType4, param4, DefaultExpandOrEmpty4, DefaultValueFix4 \
        , ParamExpandOrEmpty5, ParamType5, param5, DefaultExpandOrEmpty5, DefaultValueFix5 \
        , ParamExpandOrEmpty6, ParamType6, param6, DefaultExpandOrEmpty6, DefaultValueFix6 \
        , ParamExpandOrEmpty7, ParamType7, param7, DefaultExpandOrEmpty7, DefaultValueFix7 \
        , ... \
    ) \
    zfclassNotPOD _ZFP_ZFLambdaI_##name : zfextendsNotPOD _ZFP_ZFCallbackLambda \
    { \
    public: \
        typedef ReturnType _ZFP_Ret; \
        static void _ZFP_D(ZF_IN _ZFP_ZFCallbackLambda *impl) \
        { \
            delete impl; \
        } \
        static _ZFP_Ret _ZFP_I(ZF_IN _ZFP_ZFCallbackLambda *_ZFP_this \
            ParamExpandOrEmpty0(ZFM_COMMA() ParamType0 param0) \
            ParamExpandOrEmpty1(ZFM_COMMA() ParamType1 param1) \
            ParamExpandOrEmpty2(ZFM_COMMA() ParamType2 param2) \
            ParamExpandOrEmpty3(ZFM_COMMA() ParamType3 param3) \
            ParamExpandOrEmpty4(ZFM_COMMA() ParamType4 param4) \
            ParamExpandOrEmpty5(ZFM_COMMA() ParamType5 param5) \
            ParamExpandOrEmpty6(ZFM_COMMA() ParamType6 param6) \
            ParamExpandOrEmpty7(ZFM_COMMA() ParamType7 param7) \
        ) { \
            return ((_ZFP_ZFLambdaI_##name *)_ZFP_this)->_ZFP_i( \
                ParamExpandOrEmpty0(ZFM_EMPTY() param0) \
                ParamExpandOrEmpty1(ZFM_COMMA() param1) \
                ParamExpandOrEmpty2(ZFM_COMMA() param2) \
                ParamExpandOrEmpty3(ZFM_COMMA() param3) \
                ParamExpandOrEmpty4(ZFM_COMMA() param4) \
                ParamExpandOrEmpty5(ZFM_COMMA() param5) \
                ParamExpandOrEmpty6(ZFM_COMMA() param6) \
                ParamExpandOrEmpty7(ZFM_COMMA() param7) \
            ); \
        } \
        zffinal _ZFP_Ret _ZFP_i( \
            ParamExpandOrEmpty0(ZFM_EMPTY() ParamType0 param0) \
            ParamExpandOrEmpty1(ZFM_COMMA() ParamType1 param1) \
            ParamExpandOrEmpty2(ZFM_COMMA() ParamType2 param2) \
            ParamExpandOrEmpty3(ZFM_COMMA() ParamType3 param3) \
            ParamExpandOrEmpty4(ZFM_COMMA() ParamType4 param4) \
            ParamExpandOrEmpty5(ZFM_COMMA() ParamType5 param5) \
            ParamExpandOrEmpty6(ZFM_COMMA() ParamType6 param6) \
            ParamExpandOrEmpty7(ZFM_COMMA() ParamType7 param7) \
        ) {
#define _ZFP_ZFLambdaEnd_EXPAND(...) __VA_ARGS__
#define _ZFP_ZFLambdaEnd(...) \
    _ZFP_ZFLambdaEnd_EXPAND(_ZFP_ZFLambdaEnd_(__VA_ARGS__))
#define _ZFP_ZFLambdaEnd_(name \
    , CaptureExpandOrEmpty0, CaptureType0, capture0 \
    , CaptureExpandOrEmpty1, CaptureType1, capture1 \
    , CaptureExpandOrEmpty2, CaptureType2, capture2 \
    , CaptureExpandOrEmpty3, CaptureType3, capture3 \
    , CaptureExpandOrEmpty4, CaptureType4, capture4 \
    , CaptureExpandOrEmpty5, CaptureType5, capture5 \
    , CaptureExpandOrEmpty6, CaptureType6, capture6 \
    , CaptureExpandOrEmpty7, CaptureType7, capture7 \
    , ...) \
        } \
        virtual ZFFuncAddrType _ZFP_ivk(void) const \
        { \
            return (ZFFuncAddrType)_ZFP_I; \
        } \
        CaptureExpandOrEmpty0(CaptureType0 capture0;) \
        CaptureExpandOrEmpty1(CaptureType1 capture1;) \
        CaptureExpandOrEmpty2(CaptureType2 capture2;) \
        CaptureExpandOrEmpty3(CaptureType3 capture3;) \
        CaptureExpandOrEmpty4(CaptureType4 capture4;) \
        CaptureExpandOrEmpty5(CaptureType5 capture5;) \
        CaptureExpandOrEmpty6(CaptureType6 capture6;) \
        CaptureExpandOrEmpty7(CaptureType7 capture7;) \
        _ZFP_ZFLambdaI_##name( \
            CaptureExpandOrEmpty0(ZFM_EMPTY() CaptureType0 capture0) \
            CaptureExpandOrEmpty1(ZFM_COMMA() CaptureType1 capture1) \
            CaptureExpandOrEmpty2(ZFM_COMMA() CaptureType2 capture2) \
            CaptureExpandOrEmpty3(ZFM_COMMA() CaptureType3 capture3) \
            CaptureExpandOrEmpty4(ZFM_COMMA() CaptureType4 capture4) \
            CaptureExpandOrEmpty5(ZFM_COMMA() CaptureType5 capture5) \
            CaptureExpandOrEmpty6(ZFM_COMMA() CaptureType6 capture6) \
            CaptureExpandOrEmpty7(ZFM_COMMA() CaptureType7 capture7) \
        ) \
        CaptureExpandOrEmpty0(          : capture0(capture0)) \
        CaptureExpandOrEmpty1(ZFM_COMMA() capture1(capture1)) \
        CaptureExpandOrEmpty2(ZFM_COMMA() capture2(capture2)) \
        CaptureExpandOrEmpty3(ZFM_COMMA() capture3(capture3)) \
        CaptureExpandOrEmpty4(ZFM_COMMA() capture4(capture4)) \
        CaptureExpandOrEmpty5(ZFM_COMMA() capture5(capture5)) \
        CaptureExpandOrEmpty6(ZFM_COMMA() capture6(capture6)) \
        CaptureExpandOrEmpty7(ZFM_COMMA() capture7(capture7)) \
        { \
        } \
    }; \
    ZFCallback name = ZFCallback::_ZFP_ZFCallbackCreate( \
        ZFCallbackTypeLambda, \
        zfnull, \
        zfnull, \
        zfnull, \
        new _ZFP_ZFLambdaI_##name( \
            CaptureExpandOrEmpty0(ZFM_EMPTY() capture0) \
            CaptureExpandOrEmpty1(ZFM_COMMA() capture1) \
            CaptureExpandOrEmpty2(ZFM_COMMA() capture2) \
            CaptureExpandOrEmpty3(ZFM_COMMA() capture3) \
            CaptureExpandOrEmpty4(ZFM_COMMA() capture4) \
            CaptureExpandOrEmpty5(ZFM_COMMA() capture5) \
            CaptureExpandOrEmpty6(ZFM_COMMA() capture6) \
            CaptureExpandOrEmpty7(ZFM_COMMA() capture7) \
        ), \
        _ZFP_ZFLambdaI_##name::_ZFP_D);

#define _ZFP_ZFLAMBDA_EXPAND(...) __VA_ARGS__
#define _ZFP_ZFLAMBDA(...) \
    _ZFP_ZFLAMBDA_EXPAND(_ZFP_ZFLAMBDA_(__VA_ARGS__))
#define _ZFP_ZFLAMBDA_(name \
        , CaptureExpandOrEmpty0, CaptureType0, capture0 \
        , CaptureExpandOrEmpty1, CaptureType1, capture1 \
        , CaptureExpandOrEmpty2, CaptureType2, capture2 \
        , CaptureExpandOrEmpty3, CaptureType3, capture3 \
        , CaptureExpandOrEmpty4, CaptureType4, capture4 \
        , CaptureExpandOrEmpty5, CaptureType5, capture5 \
        , CaptureExpandOrEmpty6, CaptureType6, capture6 \
        , CaptureExpandOrEmpty7, CaptureType7, capture7 \
        , lambdaBody, ReturnType \
        , ParamExpandOrEmpty0, ParamType0, param0, DefaultExpandOrEmpty0, DefaultValueFix0 \
        , ParamExpandOrEmpty1, ParamType1, param1, DefaultExpandOrEmpty1, DefaultValueFix1 \
        , ParamExpandOrEmpty2, ParamType2, param2, DefaultExpandOrEmpty2, DefaultValueFix2 \
        , ParamExpandOrEmpty3, ParamType3, param3, DefaultExpandOrEmpty3, DefaultValueFix3 \
        , ParamExpandOrEmpty4, ParamType4, param4, DefaultExpandOrEmpty4, DefaultValueFix4 \
        , ParamExpandOrEmpty5, ParamType5, param5, DefaultExpandOrEmpty5, DefaultValueFix5 \
        , ParamExpandOrEmpty6, ParamType6, param6, DefaultExpandOrEmpty6, DefaultValueFix6 \
        , ParamExpandOrEmpty7, ParamType7, param7, DefaultExpandOrEmpty7, DefaultValueFix7 \
        , ... \
    ) \
    _ZFP_ZFLambdaBegin(ReturnType, name \
            , ParamExpandOrEmpty0, ParamType0, param0, DefaultExpandOrEmpty0, DefaultValueFix0 \
            , ParamExpandOrEmpty1, ParamType1, param1, DefaultExpandOrEmpty1, DefaultValueFix1 \
            , ParamExpandOrEmpty2, ParamType2, param2, DefaultExpandOrEmpty2, DefaultValueFix2 \
            , ParamExpandOrEmpty3, ParamType3, param3, DefaultExpandOrEmpty3, DefaultValueFix3 \
            , ParamExpandOrEmpty4, ParamType4, param4, DefaultExpandOrEmpty4, DefaultValueFix4 \
            , ParamExpandOrEmpty5, ParamType5, param5, DefaultExpandOrEmpty5, DefaultValueFix5 \
            , ParamExpandOrEmpty6, ParamType6, param6, DefaultExpandOrEmpty6, DefaultValueFix6 \
            , ParamExpandOrEmpty7, ParamType7, param7, DefaultExpandOrEmpty7, DefaultValueFix7 \
        ) \
    lambdaBody \
    _ZFP_ZFLambdaEnd(name \
            , CaptureExpandOrEmpty0, CaptureType0, capture0 \
            , CaptureExpandOrEmpty1, CaptureType1, capture1 \
            , CaptureExpandOrEmpty2, CaptureType2, capture2 \
            , CaptureExpandOrEmpty3, CaptureType3, capture3 \
            , CaptureExpandOrEmpty4, CaptureType4, capture4 \
            , CaptureExpandOrEmpty5, CaptureType5, capture5 \
            , CaptureExpandOrEmpty6, CaptureType6, capture6 \
            , CaptureExpandOrEmpty7, CaptureType7, capture7 \
        )

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFCallback_lambda_h_

