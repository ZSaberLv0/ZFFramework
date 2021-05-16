/**
 * @file ZFPropertyDynamicRegisterExtra.h
 * @brief user registered ZFProperty
 */

#ifndef _ZFI_ZFPropertyDynamicRegisterExtra_h_
#define _ZFI_ZFPropertyDynamicRegisterExtra_h_

#include "ZFPropertyDynamicRegister.h"
#include "ZFObjectCore.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassFwd ZFListener;

/**
 * @brief property life cycle, see #ZFPROPERTY_ON_INIT_DECLARE
 */
typedef enum {
    ZFPropertyLifeCycleOnInit, /**< @brief see #ZFPROPERTY_ON_INIT_DECLARE */
    ZFPropertyLifeCycleOnVerify, /**< @brief see #ZFPROPERTY_ON_INIT_DECLARE */
    ZFPropertyLifeCycleOnAttach, /**< @brief see #ZFPROPERTY_ON_INIT_DECLARE */
    ZFPropertyLifeCycleOnDetach, /**< @brief see #ZFPROPERTY_ON_INIT_DECLARE */
} ZFPropertyLifeCycle;

/**
 * @brief dynamic version of #ZFPROPERTY_ON_INIT_DECLARE
 *
 * can not be registered if the property already has custom life cycle for ownerClass\n
 * \n
 * callback's param0 is #ZFPropertyInvokeData,
 * sender is the invokerObject
 */
extern ZF_ENV_EXPORT zfbool ZFPropertyDynamicRegisterLifeCycle(ZF_IN const ZFProperty *property,
                                                               ZF_IN const ZFClass *ownerClassOrNull,
                                                               ZF_IN ZFPropertyLifeCycle lifeCycle,
                                                               ZF_IN const ZFListener &callback,
                                                               ZF_IN_OPT ZFObject *userData = zfnull,
                                                               ZF_OUT_OPT zfstring *errorHint = zfnull);
/** @brief see #ZFPropertyDynamicRegisterLifeCycle */
extern ZF_ENV_EXPORT zfbool ZFPropertyDynamicUnregisterLifeCycle(ZF_IN const ZFProperty *property,
                                                                 ZF_IN const ZFClass *ownerClassOrNull,
                                                                 ZF_IN ZFPropertyLifeCycle lifeCycle);

/** @brief data to hold params for property, see #ZFPropertyDynamicRegisterLifeCycle */
zfclass ZF_ENV_EXPORT ZFPropertyInvokeData : zfextends ZFObject
{
    ZFOBJECT_DECLARE_WITH_CUSTOM_CTOR(ZFPropertyInvokeData, ZFObject)

public:
    /**
     * @brief holds the owner object of the property
     */
    ZFObject *invokerObject;
    /**
     * @brief holds the owner property
     */
    const ZFProperty *invokerProperty;
    /**
     * @brief holds the property's value
     *
     * for retain property, it's the property's value,
     * for assign property, it's a #ZFTypeIdWrapper
     */
    zfautoObject propertyValue;
    /**
     * @brief see #propertyValue
     */
    zfautoObject propertyValueOld;

protected:
    /** @cond ZFPrivateDoc */
    ZFPropertyInvokeData(void) : ZFObject()
    , invokerObject(zfnull)
    , invokerProperty(zfnull)
    , propertyValue()
    , propertyValueOld()
    {
    }
    /** @endcond */
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFPropertyDynamicRegisterExtra_h_

