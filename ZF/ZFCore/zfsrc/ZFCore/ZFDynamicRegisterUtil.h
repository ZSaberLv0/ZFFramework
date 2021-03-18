/**
 * @file ZFDynamicRegisterUtil.h
 * @brief user registered ZFProperty
 */

#ifndef _ZFI_ZFDynamicRegisterUtil_h_
#define _ZFI_ZFDynamicRegisterUtil_h_

#include "ZFObject.h"

ZF_NAMESPACE_GLOBAL_BEGIN
/* ZFMETHOD_MAX_PARAM */

/** @brief data to hold params for #ZFPropertyDynamicRegisterInitValueCallback */
zfclass ZF_ENV_EXPORT ZFDynamicPropertyData : zfextends ZFObject
{
    ZFOBJECT_DECLARE_WITH_CUSTOM_CTOR(ZFDynamicPropertyData, ZFObject)

public:
    zfautoObject ret; /**< @brief data to hold params for #ZFPropertyDynamicRegisterInitValueCallback */
    const ZFProperty *property; /**< @brief data to hold params for #ZFPropertyDynamicRegisterInitValueCallback */
protected:
    /** @cond ZFPrivateDoc */
    ZFDynamicPropertyData(void)
    : ZFObject()
    , ret()
    , property(zfnull)
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

// ============================================================
zfclassFwd _ZFP_ZFDynamicPrivate;
/**
 * @brief util class to dynamic register class/method/property
 *
 * usage:
 * @code
 *   ZFDynamic()
 *       .classBegin(classNameFull [, parent, userData])
 *           .event(eventName)
 *           .method(callback, userData, returnTypeId, methodName [, paramTypeId0, ...])
 *           .property(typeIdOrRetainClass, propertyName [, propertyInitValue])
 *       .classEnd()
 *       .NSBegin([methodNamespace])
 *           .event(eventName)
 *           .method(callback, userData, returnTypeId, methodName [, paramTypeId0, ...])
 *       .NSEnd()
 *       .enumBegin(enumClassName)
 *           .enumIsFlags(isFlag)
 *           .enumValue(enumName [, enumValue])
 *           .enumValue(enumName [, enumValue])
 *       .enumEnd([enumDefault])
 *   ;
 * @endcode
 *
 * when any steps failed, #errorCallbackNotify would be called,
 * and all further call would be ignored\n
 * \n
 * you may store the returned ZFDynamic object,
 * and use #removeAll to remove all registered items at once\n
 * to make it more convenient for script language,
 * you may also use #regTag to make the registration looks like singleton registration
 */
zfclassLikePOD ZF_ENV_EXPORT ZFDynamic
{
public:
    /** @brief main constructor */
    ZFDynamic(void);
    /** @brief construct with #regTag */
    ZFDynamic(ZF_IN const zfchar *regTag);

    /** @cond ZFPrivateDoc */
    ZFDynamic(ZF_IN const ZFDynamic &ref);
    ~ZFDynamic(void);
    ZFDynamic &operator = (ZF_IN const ZFDynamic &ref);
    zfbool operator == (ZF_IN const ZFDynamic &ref) const;
    zfbool operator != (ZF_IN const ZFDynamic &ref) const {return !this->operator == (ref);}
    /** @endcond */

public:
    /**
     * @brief util method to export all symbols to a tag file
     *
     * the tag file contains all of these in plain text format, one line for each:
     * -  class name
     * -  method namespace
     * -  method name (including property)
     * -  type id name
     *
     * output ensured unique, while order are not ensured\n
     * anything that starts with "_ZFP_" would be ignored\n
     * \n
     * this is useful to use ZFFramework in some script language
     * which has no semantic completion,
     * so that you can use keyword completion by using the tag file
     */
    static void exportTag(ZF_IN_OUT const ZFOutput &output);

public:
    /**
     * @brief util to make the registration able to be called more than once
     *
     * by default, dynamic register would fail if contents already exists,
     * that's not very convenient for script languages\n
     * to solve this, you may use this method to mark the registration,
     * which would automatically unregister old ones if exists,
     * identified by tha regTag
     */
    ZFDynamic &regTag(ZF_IN const zfchar *regTag);
    /** @brief see #regTag */
    const zfchar *regTag(void) const;

public:
    /** @brief see #ZFDynamic */
    void removeAll(void);
    /** @brief see #ZFDynamic */
    const ZFCoreArrayPOD<const ZFClass *> &allClass(void) const;
    /** @brief see #ZFDynamic */
    const ZFCoreArrayPOD<const ZFClass *> &allEnum(void) const;
    /** @brief see #ZFDynamic */
    const ZFCoreArrayPOD<const ZFMethod *> &allMethod(void) const;
    /** @brief see #ZFDynamic */
    const ZFCoreArrayPOD<const ZFProperty *> &allProperty(void) const;
    /** @brief see #ZFDynamic */
    const ZFCoreArrayPOD<zfidentity> &allEvent(void) const;

public:
    /** @brief see #ZFDynamic */
    ZFDynamic &classBegin(ZF_IN const zfchar *classNameFull,
                          ZF_IN_OPT const ZFClass *classParent = ZFObject::ClassData(),
                          ZF_IN_OPT ZFObject *classDynamicRegisterUserData = zfnull);
    /** @brief see #ZFDynamic */
    ZFDynamic &classBegin(ZF_IN const zfchar *classNameFull,
                          ZF_IN const zfchar *parentClassNameFull,
                          ZF_IN_OPT ZFObject *classDynamicRegisterUserData = zfnull);
    /** @brief see #ZFDynamic */
    ZFDynamic &classBegin(ZF_IN const ZFClass *cls);
    /** @brief see #ZFDynamic */
    ZFDynamic &classEnd(void);

    /** @brief see #ZFDynamic */
    ZFDynamic &classOnInit(ZF_IN const ZFListener &classOnInitCallback,
                           ZF_IN_OPT ZFObject *userData = zfnull);
    /** @brief see #ZFDynamic */
    ZFDynamic &classOnDealloc(ZF_IN const ZFListener &classOnDeallocCallback,
                              ZF_IN_OPT ZFObject *userData = zfnull);

public:
    /** @brief see #ZFDynamic */
    ZFDynamic &NSBegin(ZF_IN_OPT const zfchar *methodNamespace = ZF_NAMESPACE_GLOBAL_NAME);
    /** @brief see #ZFDynamic */
    ZFDynamic &NSEnd(void);

public:
    /** @brief see #ZFDynamic */
    ZFDynamic &enumBegin(ZF_IN const zfchar *enumClassName);
    /** @brief see #ZFDynamic */
    ZFDynamic &enumIsFlags(ZF_IN zfbool enumIsFlags);
    /** @brief see #ZFDynamic */
    ZFDynamic &enumValue(ZF_IN const zfchar *enumName,
                         ZF_IN_OPT zfuint enumValue = ZFEnumInvalid());
    /** @brief see #ZFDynamic */
    ZFDynamic &enumEnd(ZF_IN_OPT zfuint enumDefault = ZFEnumInvalid());

public:
    /**
     * @brief register a event, see also #ZFOBSERVER_EVENT #ZFOBSERVER_EVENT_GLOBAL
     *
     * if within class scope (#classBegin),
     * `YourClassName::EventYourEvent` would be registered,
     * otherwise, `YourNamespace::EventYourEvent` would be registered\n
     * registered event would include:
     * -  an event registered by #ZFIdMapDynamicRegister
     * -  a #ZFMethod to access the event
     */
    ZFDynamic &event(ZF_IN const zfchar *eventName);

public:
    /**
     * @brief see #ZFDynamic
     *
     * util method to register method (global method or class member method),
     * methodImpl's param0 is #ZFMethodInvokeData,
     * userData is methodImplUserData
     */
    ZFDynamic &method(ZF_IN const ZFListener &methodImpl
                      , ZF_IN ZFObject *methodImplUserData
                      , ZF_IN const zfchar *methodReturnTypeId
                      , ZF_IN const zfchar *methodName
                      , ZF_IN_OPT const zfchar *methodParamTypeId0 = zfnull
                      , ZF_IN_OPT const zfchar *methodParamTypeId1 = zfnull
                      , ZF_IN_OPT const zfchar *methodParamTypeId2 = zfnull
                      , ZF_IN_OPT const zfchar *methodParamTypeId3 = zfnull
                      , ZF_IN_OPT const zfchar *methodParamTypeId4 = zfnull
                      , ZF_IN_OPT const zfchar *methodParamTypeId5 = zfnull
                      , ZF_IN_OPT const zfchar *methodParamTypeId6 = zfnull
                      , ZF_IN_OPT const zfchar *methodParamTypeId7 = zfnull
                      );
    /**
     * @brief see #ZFDynamic
     *
     * util method to register method (global method or class static method),
     * methodImpl's param0 is #ZFMethodInvokeData,
     * userData is methodImplUserData
     */
    ZFDynamic &methodStatic(ZF_IN const ZFListener &methodImpl
                            , ZF_IN ZFObject *methodImplUserData
                            , ZF_IN const zfchar *methodReturnTypeId
                            , ZF_IN const zfchar *methodName
                            , ZF_IN_OPT const zfchar *methodParamTypeId0 = zfnull
                            , ZF_IN_OPT const zfchar *methodParamTypeId1 = zfnull
                            , ZF_IN_OPT const zfchar *methodParamTypeId2 = zfnull
                            , ZF_IN_OPT const zfchar *methodParamTypeId3 = zfnull
                            , ZF_IN_OPT const zfchar *methodParamTypeId4 = zfnull
                            , ZF_IN_OPT const zfchar *methodParamTypeId5 = zfnull
                            , ZF_IN_OPT const zfchar *methodParamTypeId6 = zfnull
                            , ZF_IN_OPT const zfchar *methodParamTypeId7 = zfnull
                            );
    /** @brief see #ZFDynamic */
    ZFDynamic &method(ZF_IN const ZFMethodDynamicRegisterParam &param);

public:
    /**
     * @brief see #ZFDynamic
     *
     * initValueCallback can be specified to setup property's init value,
     * param0 is #ZFDynamicPropertyData,
     * userData is the initValueCallbackUserData
     */
    ZFDynamic &property(ZF_IN const zfchar *propertyTypeId,
                        ZF_IN const zfchar *propertyName,
                        ZF_IN_OPT ZFObject *propertyInitValue = zfnull,
                        ZF_IN_OPT ZFMethodPrivilegeType setterPrivilegeType = ZFMethodPrivilegeTypePublic,
                        ZF_IN_OPT ZFMethodPrivilegeType getterPrivilegeType = ZFMethodPrivilegeTypePublic);
    /** @brief see #ZFDynamic */
    ZFDynamic &property(ZF_IN const ZFClass *propertyClassOfRetainProperty,
                        ZF_IN const zfchar *propertyName,
                        ZF_IN_OPT ZFObject *propertyInitValue = zfnull,
                        ZF_IN_OPT ZFMethodPrivilegeType setterPrivilegeType = ZFMethodPrivilegeTypePublic,
                        ZF_IN_OPT ZFMethodPrivilegeType getterPrivilegeType = ZFMethodPrivilegeTypePublic);
    /** @brief see #ZFDynamic */
    ZFDynamic &property(ZF_IN const ZFPropertyDynamicRegisterParam &param);

public:
    /** @brief error callback which would be called if error occurred */
    ZFDynamic &errorCallbackAdd(ZF_IN const ZFOutput &errorCallback = ZFOutputDefault());
    /** @brief see #errorCallbackAdd */
    ZFDynamic &errorCallbackRemove(ZF_IN const ZFOutput &errorCallback);
    /** @brief see #errorCallbackAdd */
    zfindex errorCallbackCount(void) const;
    /** @brief see #errorCallbackAdd */
    const ZFOutput &errorCallbackAtIndex(ZF_IN zfindex index) const;
    /** @brief see #errorCallbackAdd */
    void errorCallbackNotify(ZF_IN const zfchar *errorHint) const;

private:
    _ZFP_ZFDynamicPrivate *d;
};
ZFTYPEID_ACCESS_ONLY_DECLARE(ZFDynamic, ZFDynamic)

// ============================================================
/**
 * @brief util method to remove all contents registered by #ZFDynamic
 *
 * ensured called during #ZFFrameworkCleanup as level #ZFLevelZFFrameworkNormal
 */
ZFMETHOD_FUNC_DECLARE_0(void, ZFDynamicRemoveAll)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFDynamicRegisterUtil_h_

