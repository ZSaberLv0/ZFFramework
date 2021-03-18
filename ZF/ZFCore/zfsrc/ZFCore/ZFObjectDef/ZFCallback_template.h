/**
 * @file ZFCallback_template.h
 * @brief common callback defines
 */

#ifndef _ZFI_ZFCallback_template_h_
#define _ZFI_ZFCallback_template_h_

#include "ZFCallback.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/** @brief template version of #ZFCallback with explicit param and return types */
template<typename T_Return
    , typename T_Param0 = void
    , typename T_Param1 = void
    , typename T_Param2 = void
    , typename T_Param3 = void
    , typename T_Param4 = void
    , typename T_Param5 = void
    , typename T_Param6 = void
    , typename T_Param7 = void
    , typename T_Dummy = void
    >
zfclassLikePOD ZF_ENV_EXPORT ZFCallbackT : zfextendsLikePOD ZFCallback {};

/** @cond ZFPrivateDoc */
template<typename T_Return
    >
zfclassLikePOD ZF_ENV_EXPORT ZFCallbackT<T_Return
    , void
    , void
    , void
    , void
    , void
    , void
    , void
    , void
    , void
    > : zfextendsLikePOD ZFCallback
{
    _ZFP_ZFCALLBACK_DECLARE_CONSTRUCTORS(ZFCallbackT, ZFCallback)

public:
    inline T_Return execute(
                            ) const
    {
        return ZFCallback::executeExact<T_Return
            >(
            );
    }
    inline T_Return operator () (
                                 ) const
    {
        return ZFCallback::executeExact<T_Return
            >(
            );
    }
};
template<typename T_Return
    , typename T_Param0
    >
zfclassLikePOD ZF_ENV_EXPORT ZFCallbackT<T_Return
    , T_Param0
    , void
    , void
    , void
    , void
    , void
    , void
    , void
    , void
    > : zfextendsLikePOD ZFCallback
{
    _ZFP_ZFCALLBACK_DECLARE_CONSTRUCTORS(ZFCallbackT, ZFCallback)

public:
    inline T_Return execute(
                              ZF_IN T_Param0 param0
                            ) const
    {
        return ZFCallback::executeExact<T_Return
                , T_Param0
            >(
                  param0
            );
    }
    inline T_Return operator () (
                                   ZF_IN T_Param0 param0
                                 ) const
    {
        return ZFCallback::executeExact<T_Return
                , T_Param0
            >(
                  param0
            );
    }
};
template<typename T_Return
    , typename T_Param0
    , typename T_Param1
    >
zfclassLikePOD ZF_ENV_EXPORT ZFCallbackT<T_Return
    , T_Param0
    , T_Param1
    , void
    , void
    , void
    , void
    , void
    , void
    , void
    > : zfextendsLikePOD ZFCallback
{
    _ZFP_ZFCALLBACK_DECLARE_CONSTRUCTORS(ZFCallbackT, ZFCallback)

public:
    inline T_Return execute(
                              ZF_IN T_Param0 param0
                            , ZF_IN T_Param1 param1
                            ) const
    {
        return ZFCallback::executeExact<T_Return
                , T_Param0
                , T_Param1
            >(
                  param0
                , param1
            );
    }
    inline T_Return operator () (
                                   ZF_IN T_Param0 param0
                                 , ZF_IN T_Param1 param1
                                 ) const
    {
        return ZFCallback::executeExact<T_Return
                , T_Param0
                , T_Param1
            >(
                  param0
                , param1
            );
    }
};
template<typename T_Return
    , typename T_Param0
    , typename T_Param1
    , typename T_Param2
    >
zfclassLikePOD ZF_ENV_EXPORT ZFCallbackT<T_Return
    , T_Param0
    , T_Param1
    , T_Param2
    , void
    , void
    , void
    , void
    , void
    , void
    > : zfextendsLikePOD ZFCallback
{
    _ZFP_ZFCALLBACK_DECLARE_CONSTRUCTORS(ZFCallbackT, ZFCallback)

public:
    inline T_Return execute(
                              ZF_IN T_Param0 param0
                            , ZF_IN T_Param1 param1
                            , ZF_IN T_Param2 param2
                            ) const
    {
        return ZFCallback::executeExact<T_Return
                , T_Param0
                , T_Param1
                , T_Param2
            >(
                  param0
                , param1
                , param2
            );
    }
    inline T_Return operator () (
                                   ZF_IN T_Param0 param0
                                 , ZF_IN T_Param1 param1
                                 , ZF_IN T_Param2 param2
                                 ) const
    {
        return ZFCallback::executeExact<T_Return
                , T_Param0
                , T_Param1
                , T_Param2
            >(
                  param0
                , param1
                , param2
            );
    }
};
template<typename T_Return
    , typename T_Param0
    , typename T_Param1
    , typename T_Param2
    , typename T_Param3
    >
zfclassLikePOD ZF_ENV_EXPORT ZFCallbackT<T_Return
    , T_Param0
    , T_Param1
    , T_Param2
    , T_Param3
    , void
    , void
    , void
    , void
    , void
    > : zfextendsLikePOD ZFCallback
{
    _ZFP_ZFCALLBACK_DECLARE_CONSTRUCTORS(ZFCallbackT, ZFCallback)

public:
    inline T_Return execute(
                              ZF_IN T_Param0 param0
                            , ZF_IN T_Param1 param1
                            , ZF_IN T_Param2 param2
                            , ZF_IN T_Param3 param3
                            ) const
    {
        return ZFCallback::executeExact<T_Return
                , T_Param0
                , T_Param1
                , T_Param2
                , T_Param3
            >(
                  param0
                , param1
                , param2
                , param3
            );
    }
    inline T_Return operator () (
                                   ZF_IN T_Param0 param0
                                 , ZF_IN T_Param1 param1
                                 , ZF_IN T_Param2 param2
                                 , ZF_IN T_Param3 param3
                                 ) const
    {
        return ZFCallback::executeExact<T_Return
                , T_Param0
                , T_Param1
                , T_Param2
                , T_Param3
            >(
                  param0
                , param1
                , param2
                , param3
            );
    }
};
template<typename T_Return
    , typename T_Param0
    , typename T_Param1
    , typename T_Param2
    , typename T_Param3
    , typename T_Param4
    >
zfclassLikePOD ZF_ENV_EXPORT ZFCallbackT<T_Return
    , T_Param0
    , T_Param1
    , T_Param2
    , T_Param3
    , T_Param4
    , void
    , void
    , void
    , void
    > : zfextendsLikePOD ZFCallback
{
    _ZFP_ZFCALLBACK_DECLARE_CONSTRUCTORS(ZFCallbackT, ZFCallback)

public:
    inline T_Return execute(
                              ZF_IN T_Param0 param0
                            , ZF_IN T_Param1 param1
                            , ZF_IN T_Param2 param2
                            , ZF_IN T_Param3 param3
                            , ZF_IN T_Param4 param4
                            ) const
    {
        return ZFCallback::executeExact<T_Return
                , T_Param0
                , T_Param1
                , T_Param2
                , T_Param3
                , T_Param4
            >(
                  param0
                , param1
                , param2
                , param3
                , param4
            );
    }
    inline T_Return operator () (
                                   ZF_IN T_Param0 param0
                                 , ZF_IN T_Param1 param1
                                 , ZF_IN T_Param2 param2
                                 , ZF_IN T_Param3 param3
                                 , ZF_IN T_Param4 param4
                                 ) const
    {
        return ZFCallback::executeExact<T_Return
                , T_Param0
                , T_Param1
                , T_Param2
                , T_Param3
                , T_Param4
            >(
                  param0
                , param1
                , param2
                , param3
                , param4
            );
    }
};
template<typename T_Return
    , typename T_Param0
    , typename T_Param1
    , typename T_Param2
    , typename T_Param3
    , typename T_Param4
    , typename T_Param5
    >
zfclassLikePOD ZF_ENV_EXPORT ZFCallbackT<T_Return
    , T_Param0
    , T_Param1
    , T_Param2
    , T_Param3
    , T_Param4
    , T_Param5
    , void
    , void
    , void
    > : zfextendsLikePOD ZFCallback
{
    _ZFP_ZFCALLBACK_DECLARE_CONSTRUCTORS(ZFCallbackT, ZFCallback)

public:
    inline T_Return execute(
                              ZF_IN T_Param0 param0
                            , ZF_IN T_Param1 param1
                            , ZF_IN T_Param2 param2
                            , ZF_IN T_Param3 param3
                            , ZF_IN T_Param4 param4
                            , ZF_IN T_Param5 param5
                            ) const
    {
        return ZFCallback::executeExact<T_Return
                , T_Param0
                , T_Param1
                , T_Param2
                , T_Param3
                , T_Param4
                , T_Param5
            >(
                  param0
                , param1
                , param2
                , param3
                , param4
                , param5
            );
    }
    inline T_Return operator () (
                                   ZF_IN T_Param0 param0
                                 , ZF_IN T_Param1 param1
                                 , ZF_IN T_Param2 param2
                                 , ZF_IN T_Param3 param3
                                 , ZF_IN T_Param4 param4
                                 , ZF_IN T_Param5 param5
                                 ) const
    {
        return ZFCallback::executeExact<T_Return
                , T_Param0
                , T_Param1
                , T_Param2
                , T_Param3
                , T_Param4
                , T_Param5
            >(
                  param0
                , param1
                , param2
                , param3
                , param4
                , param5
            );
    }
};
template<typename T_Return
    , typename T_Param0
    , typename T_Param1
    , typename T_Param2
    , typename T_Param3
    , typename T_Param4
    , typename T_Param5
    , typename T_Param6
    >
zfclassLikePOD ZF_ENV_EXPORT ZFCallbackT<T_Return
    , T_Param0
    , T_Param1
    , T_Param2
    , T_Param3
    , T_Param4
    , T_Param5
    , T_Param6
    , void
    , void
    > : zfextendsLikePOD ZFCallback
{
    _ZFP_ZFCALLBACK_DECLARE_CONSTRUCTORS(ZFCallbackT, ZFCallback)

public:
    inline T_Return execute(
                              ZF_IN T_Param0 param0
                            , ZF_IN T_Param1 param1
                            , ZF_IN T_Param2 param2
                            , ZF_IN T_Param3 param3
                            , ZF_IN T_Param4 param4
                            , ZF_IN T_Param5 param5
                            , ZF_IN T_Param6 param6
                            ) const
    {
        return ZFCallback::executeExact<T_Return
                , T_Param0
                , T_Param1
                , T_Param2
                , T_Param3
                , T_Param4
                , T_Param5
                , T_Param6
            >(
                  param0
                , param1
                , param2
                , param3
                , param4
                , param5
                , param6
            );
    }
    inline T_Return operator () (
                                   ZF_IN T_Param0 param0
                                 , ZF_IN T_Param1 param1
                                 , ZF_IN T_Param2 param2
                                 , ZF_IN T_Param3 param3
                                 , ZF_IN T_Param4 param4
                                 , ZF_IN T_Param5 param5
                                 , ZF_IN T_Param6 param6
                                 ) const
    {
        return ZFCallback::executeExact<T_Return
                , T_Param0
                , T_Param1
                , T_Param2
                , T_Param3
                , T_Param4
                , T_Param5
                , T_Param6
            >(
                  param0
                , param1
                , param2
                , param3
                , param4
                , param5
                , param6
            );
    }
};
template<typename T_Return
    , typename T_Param0
    , typename T_Param1
    , typename T_Param2
    , typename T_Param3
    , typename T_Param4
    , typename T_Param5
    , typename T_Param6
    , typename T_Param7
    >
zfclassLikePOD ZF_ENV_EXPORT ZFCallbackT<T_Return
    , T_Param0
    , T_Param1
    , T_Param2
    , T_Param3
    , T_Param4
    , T_Param5
    , T_Param6
    , T_Param7
    , void
    > : zfextendsLikePOD ZFCallback
{
    _ZFP_ZFCALLBACK_DECLARE_CONSTRUCTORS(ZFCallbackT, ZFCallback)

public:
    inline T_Return execute(
                              ZF_IN T_Param0 param0
                            , ZF_IN T_Param1 param1
                            , ZF_IN T_Param2 param2
                            , ZF_IN T_Param3 param3
                            , ZF_IN T_Param4 param4
                            , ZF_IN T_Param5 param5
                            , ZF_IN T_Param6 param6
                            , ZF_IN T_Param7 param7
                            ) const
    {
        return ZFCallback::executeExact<T_Return
                , T_Param0
                , T_Param1
                , T_Param2
                , T_Param3
                , T_Param4
                , T_Param5
                , T_Param6
                , T_Param7
            >(
                  param0
                , param1
                , param2
                , param3
                , param4
                , param5
                , param6
                , param7
            );
    }
    inline T_Return operator () (
                                   ZF_IN T_Param0 param0
                                 , ZF_IN T_Param1 param1
                                 , ZF_IN T_Param2 param2
                                 , ZF_IN T_Param3 param3
                                 , ZF_IN T_Param4 param4
                                 , ZF_IN T_Param5 param5
                                 , ZF_IN T_Param6 param6
                                 , ZF_IN T_Param7 param7
                                 ) const
    {
        return ZFCallback::executeExact<T_Return
                , T_Param0
                , T_Param1
                , T_Param2
                , T_Param3
                , T_Param4
                , T_Param5
                , T_Param6
                , T_Param7
            >(
                  param0
                , param1
                , param2
                , param3
                , param4
                , param5
                , param6
                , param7
            );
    }
};
/** @endcond */
/* ZFMETHOD_MAX_PARAM */

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFCallback_template_h_

