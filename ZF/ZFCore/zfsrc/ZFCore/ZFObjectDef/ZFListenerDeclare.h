/**
 * @file ZFListenerDeclare.h
 * @brief util macros to declare #ZFListener as #ZFCallback
 */

#ifndef _ZFI_ZFListenerDeclare_h_
#define _ZFI_ZFListenerDeclare_h_

#include "ZFObjectCore.h"
#include "ZFMethodDeclare.h"
#include "ZFMethodFuncDeclare.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief util to create #ZFListener "inline"
 *   by using lambda,
 *   require #ZF_ENV_LAMBDA
 *
 * usage:
 * @code
 *   obj->observerAdd(eventId, ZFListenerForLambda({
 *       zfLogT() << listenerData.sender();
 *   }));
 * @endcode
 */
#if ZF_ENV_LAMBDA
    #define ZFListenerForLambda(content) \
        ZFCallbackForFunc(((void (*)(const ZFListener &, ZFObject *))[](const ZFListener &listenerData, ZFObject *userData) {content}))
#else
    #define ZFListenerForLambda(content) ZF_ENV_LAMBDA_NOT_AVAILABLE
#endif

// ============================================================
/**
 * @brief util method to declare a #ZFListener
 *
 * usage:
 * @code
 *   // proto type:\n
 *   // void listenerName(ZF_IN const ZFListenerData &listenerData, ZFObject *userData);
 *   ZFLISTENER_LOCAL_BEGIN(yourListener)
 *   {
 *       // your code
 *   }
 *   ZFLISTENER_LOCAL_END(yourListener)
 *   obj->observerAdd(eventId, yourListener);
 * @endcode
 */
#define ZFLISTENER_LOCAL_BEGIN(listenerName) \
    ZFCALLBACK_LOCAL_BEGIN_2(void, listenerName, const ZFListenerData &, listenerData, ZFObject *, userData)
/** @brief see #ZFLISTENER_LOCAL_BEGIN */
#define ZFLISTENER_LOCAL_END(listenerName) \
    ZFCALLBACK_LOCAL_END_WITH_TYPE(ZFListener, listenerName)
/** @brief see #ZFLISTENER_LOCAL_BEGIN */
#define ZFLISTENER_LOCAL(listenerName, listenerContent) \
    ZFLISTENER_LOCAL_BEGIN(listenerName) \
    listenerContent \
    ZFLISTENER_LOCAL_END(listenerName)

// ============================================================
#define _ZFP_ZFLISTENER_LAMBDA_EXPAND(...) __VA_ARGS__
/**
 * @brief declare #ZFListener with #ZFLAMBDA_0
 */
#define ZFLISTENER_LAMBDA_0(name \
    , lambdaBody) \
    ZFLAMBDA_0(name \
        , lambdaBody, void \
        , _ZFP_ZFLISTENER_LAMBDA_EXPAND(ZFMP_IN(const ZFListenerData &, listenerData)) \
        , _ZFP_ZFLISTENER_LAMBDA_EXPAND(ZFMP_IN(ZFObject *, userData)) \
        )

/** @brief see #ZFLISTENER_LAMBDA_0 */
#define ZFLISTENER_LAMBDA_1(name \
    , CaptureParam0, capture0 \
    , lambdaBody) \
    ZFLAMBDA_1(name \
        , CaptureParam0, capture0 \
        , lambdaBody, void \
        , _ZFP_ZFLISTENER_LAMBDA_EXPAND(ZFMP_IN(const ZFListenerData &, listenerData)) \
        , _ZFP_ZFLISTENER_LAMBDA_EXPAND(ZFMP_IN(ZFObject *, userData)) \
        )

/** @brief see #ZFLISTENER_LAMBDA_0 */
#define ZFLISTENER_LAMBDA_2(name \
    , CaptureParam0, capture0 \
    , CaptureParam1, capture1 \
    , lambdaBody) \
    ZFLAMBDA_2(name \
        , CaptureParam0, capture0 \
        , CaptureParam1, capture1 \
        , lambdaBody, void \
        , _ZFP_ZFLISTENER_LAMBDA_EXPAND(ZFMP_IN(const ZFListenerData &, listenerData)) \
        , _ZFP_ZFLISTENER_LAMBDA_EXPAND(ZFMP_IN(ZFObject *, userData)) \
        )

/** @brief see #ZFLISTENER_LAMBDA_0 */
#define ZFLISTENER_LAMBDA_3(name \
    , CaptureParam0, capture0 \
    , CaptureParam1, capture1 \
    , CaptureParam2, capture2 \
    , lambdaBody) \
    ZFLAMBDA_3(name \
        , CaptureParam0, capture0 \
        , CaptureParam1, capture1 \
        , CaptureParam2, capture2 \
        , lambdaBody, void \
        , _ZFP_ZFLISTENER_LAMBDA_EXPAND(ZFMP_IN(const ZFListenerData &, listenerData)) \
        , _ZFP_ZFLISTENER_LAMBDA_EXPAND(ZFMP_IN(ZFObject *, userData)) \
        )

/** @brief see #ZFLISTENER_LAMBDA_0 */
#define ZFLISTENER_LAMBDA_4(name \
    , CaptureParam0, capture0 \
    , CaptureParam1, capture1 \
    , CaptureParam2, capture2 \
    , CaptureParam3, capture3 \
    , lambdaBody) \
    ZFLAMBDA_4(name \
        , CaptureParam0, capture0 \
        , CaptureParam1, capture1 \
        , CaptureParam2, capture2 \
        , CaptureParam3, capture3 \
        , lambdaBody, void \
        , _ZFP_ZFLISTENER_LAMBDA_EXPAND(ZFMP_IN(const ZFListenerData &, listenerData)) \
        , _ZFP_ZFLISTENER_LAMBDA_EXPAND(ZFMP_IN(ZFObject *, userData)) \
        )

/** @brief see #ZFLISTENER_LAMBDA_0 */
#define ZFLISTENER_LAMBDA_5(name \
    , CaptureParam0, capture0 \
    , CaptureParam1, capture1 \
    , CaptureParam2, capture2 \
    , CaptureParam3, capture3 \
    , CaptureParam4, capture4 \
    , lambdaBody) \
    ZFLAMBDA_5(name \
        , CaptureParam0, capture0 \
        , CaptureParam1, capture1 \
        , CaptureParam2, capture2 \
        , CaptureParam3, capture3 \
        , CaptureParam4, capture4 \
        , lambdaBody, void \
        , _ZFP_ZFLISTENER_LAMBDA_EXPAND(ZFMP_IN(const ZFListenerData &, listenerData)) \
        , _ZFP_ZFLISTENER_LAMBDA_EXPAND(ZFMP_IN(ZFObject *, userData)) \
        )

/** @brief see #ZFLISTENER_LAMBDA_0 */
#define ZFLISTENER_LAMBDA_6(name \
    , CaptureParam0, capture0 \
    , CaptureParam1, capture1 \
    , CaptureParam2, capture2 \
    , CaptureParam3, capture3 \
    , CaptureParam4, capture4 \
    , CaptureParam5, capture5 \
    , lambdaBody) \
    ZFLAMBDA_6(name \
        , CaptureParam0, capture0 \
        , CaptureParam1, capture1 \
        , CaptureParam2, capture2 \
        , CaptureParam3, capture3 \
        , CaptureParam4, capture4 \
        , CaptureParam5, capture5 \
        , lambdaBody, void \
        , _ZFP_ZFLISTENER_LAMBDA_EXPAND(ZFMP_IN(const ZFListenerData &, listenerData)) \
        , _ZFP_ZFLISTENER_LAMBDA_EXPAND(ZFMP_IN(ZFObject *, userData)) \
        )

/** @brief see #ZFLISTENER_LAMBDA_0 */
#define ZFLISTENER_LAMBDA_7(name \
    , CaptureParam0, capture0 \
    , CaptureParam1, capture1 \
    , CaptureParam2, capture2 \
    , CaptureParam3, capture3 \
    , CaptureParam4, capture4 \
    , CaptureParam5, capture5 \
    , CaptureParam6, capture6 \
    , lambdaBody) \
    ZFLAMBDA_7(name \
        , CaptureParam0, capture0 \
        , CaptureParam1, capture1 \
        , CaptureParam2, capture2 \
        , CaptureParam3, capture3 \
        , CaptureParam4, capture4 \
        , CaptureParam5, capture5 \
        , CaptureParam6, capture6 \
        , lambdaBody, void \
        , _ZFP_ZFLISTENER_LAMBDA_EXPAND(ZFMP_IN(const ZFListenerData &, listenerData)) \
        , _ZFP_ZFLISTENER_LAMBDA_EXPAND(ZFMP_IN(ZFObject *, userData)) \
        )

/** @brief see #ZFLISTENER_LAMBDA_0 */
#define ZFLISTENER_LAMBDA_8(name \
    , CaptureParam0, capture0 \
    , CaptureParam1, capture1 \
    , CaptureParam2, capture2 \
    , CaptureParam3, capture3 \
    , CaptureParam4, capture4 \
    , CaptureParam5, capture5 \
    , CaptureParam6, capture6 \
    , CaptureParam7, capture7 \
    , lambdaBody) \
    ZFLAMBDA_8(name \
        , CaptureParam0, capture0 \
        , CaptureParam1, capture1 \
        , CaptureParam2, capture2 \
        , CaptureParam3, capture3 \
        , CaptureParam4, capture4 \
        , CaptureParam5, capture5 \
        , CaptureParam6, capture6 \
        , CaptureParam7, capture7 \
        , lambdaBody, void \
        , _ZFP_ZFLISTENER_LAMBDA_EXPAND(ZFMP_IN(const ZFListenerData &, listenerData)) \
        , _ZFP_ZFLISTENER_LAMBDA_EXPAND(ZFMP_IN(ZFObject *, userData)) \
        )

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFListenerDeclare_h_

