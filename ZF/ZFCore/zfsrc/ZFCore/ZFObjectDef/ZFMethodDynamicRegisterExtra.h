/**
 * @file ZFMethodDynamicRegisterExtra.h
 * @brief user registered ZFMethod
 */

#ifndef _ZFI_ZFMethodDynamicRegisterExtra_h_
#define _ZFI_ZFMethodDynamicRegisterExtra_h_

#include "ZFMethodDynamicRegister.h"
#include "ZFObjectCore.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/** @brief data to hold params for #ZFMethodGenericInvoker */
zfclass ZF_ENV_EXPORT ZFMethodInvokeData : zfextends ZFObject
{
    ZFOBJECT_DECLARE_WITH_CUSTOM_CTOR(ZFMethodInvokeData, ZFObject)

public:
    zfbool invokeSuccess; /**< @brief data to hold params for #ZFMethodGenericInvoker */
    const ZFMethod *invokerMethod; /**< @brief data to hold params for #ZFMethodGenericInvoker */
    ZFObject *invokerObject; /**< @brief data to hold params for #ZFMethodGenericInvoker */
    zfstring errorHint; /**< @brief data to hold params for #ZFMethodGenericInvoker */
    zfautoObject ret; /**< @brief data to hold params for #ZFMethodGenericInvoker */
    zfautoObject param0; /**< @brief data to hold params for #ZFMethodGenericInvoker */
    zfautoObject param1; /**< @brief data to hold params for #ZFMethodGenericInvoker */
    zfautoObject param2; /**< @brief data to hold params for #ZFMethodGenericInvoker */
    zfautoObject param3; /**< @brief data to hold params for #ZFMethodGenericInvoker */
    zfautoObject param4; /**< @brief data to hold params for #ZFMethodGenericInvoker */
    zfautoObject param5; /**< @brief data to hold params for #ZFMethodGenericInvoker */
    zfautoObject param6; /**< @brief data to hold params for #ZFMethodGenericInvoker */
    zfautoObject param7; /**< @brief data to hold params for #ZFMethodGenericInvoker */
protected:
    /** @cond ZFPrivateDoc */
    ZFMethodInvokeData(void)
    : ZFObject()
    , invokeSuccess(zftrue)
    , invokerMethod(zfnull)
    , invokerObject(zfnull)
    , errorHint()
    , ret()
    , param0()
    , param1()
    , param2()
    , param3()
    , param4()
    , param5()
    , param6()
    , param7()
    {
    }
    /** @endcond */
public:
    zfoverride
    virtual zfbool objectIsPrivate(void)
    {
        return zftrue;
    }
    zfoverride
    virtual zfbool objectIsInternal(void)
    {
        return zftrue;
    }
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFMethodDynamicRegisterExtra_h_

