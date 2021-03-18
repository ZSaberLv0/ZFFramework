/**
 * @file ZFPropertyFwd.h
 * @brief property for ZFObject type
 */

#ifndef _ZFI_ZFPropertyFwd_h_
#define _ZFI_ZFPropertyFwd_h_

#include "ZFObjectClassTypeFwd.h"
#include "ZFTypeIdFwd.h"
ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd ZFProperty;
zfclassFwd ZFObject;
zfclassFwd ZFSerializableData;

// ============================================================
/**
 * @brief used to check whether the property has been accessed
 *
 * usage:
 * @code
 *   const ZFProperty *propertyInfo = ...;
 *   propertyInfo->callbackIsValueAccessed(propertyInfo, ownerObj);
 * @endcode
 */
typedef zfbool (*ZFPropertyCallbackIsValueAccessed)(ZF_IN const ZFProperty *property,
                                                    ZF_IN ZFObject *ownerObj);
/** @brief change default impl for #ZFPropertyCallbackIsValueAccessed, use with caution */
extern ZF_ENV_EXPORT void ZFPropertyCallbackIsValueAccessedChange(ZF_IN const ZFProperty *property,
                                                                  ZF_IN ZFPropertyCallbackIsValueAccessed callback);

// ============================================================
/**
 * @brief used to check whether the property is in init value state
 *
 * usage:
 * @code
 *   const ZFProperty *propertyInfo = ...;
 *   propertyInfo->callbackIsInitValue(propertyInfo, ownerObj, zfnull);
 * @endcode
 * @note the outInitValue can be set to access the init value (only when the checker returned false)
 */
typedef zfbool (*ZFPropertyCallbackIsInitValue)(ZF_IN const ZFProperty *property,
                                                ZF_IN ZFObject *ownerObj,
                                                ZF_OUT_OPT zfautoObject *outInitValue /* = zfnull */);
/** @brief change default impl for #ZFPropertyCallbackIsInitValue, use with caution */
extern ZF_ENV_EXPORT void ZFPropertyCallbackIsInitValueChange(ZF_IN const ZFProperty *property,
                                                              ZF_IN ZFPropertyCallbackIsInitValue callback);

// ============================================================
/**
 * @brief used to reset the property to its init state
 */
typedef void (*ZFPropertyCallbackValueReset)(ZF_IN const ZFProperty *property,
                                             ZF_IN ZFObject *ownerObj);
/** @brief change default impl for #ZFPropertyCallbackValueReset, use with caution */
extern ZF_ENV_EXPORT void ZFPropertyCallbackValueResetChange(ZF_IN const ZFProperty *property,
                                                             ZF_IN ZFPropertyCallbackValueReset callback);

// ============================================================
/**
 * @brief for user registered property only, used to setup a property's init value
 */
typedef void (*ZFPropertyCallbackUserRegisterInitValueSetup)(ZF_IN const ZFProperty *property,
                                                             ZF_IN_OUT void *v);

// ============================================================
typedef void (*_ZFP_PropLifeCycleWrapper)(ZF_IN ZFObject *propertyOwnerObject,
                                          ZF_IN void *propertyValue,
                                          ZF_IN const void *propertyValueOld);
zfclassPOD ZF_ENV_EXPORT _ZFP_PropLifeCycleData
{
public:
    const ZFClass *propertyOwnerClass;
    _ZFP_PropLifeCycleWrapper propertyLifeCycleWrapper;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFPropertyFwd_h_

