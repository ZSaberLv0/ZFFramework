/**
 * @file ZFCallback.h
 * @brief header for ZFCallback
 */

#ifndef _ZFI_ZFCallback_h_
#define _ZFI_ZFCallback_h_

#include "ZFMethod.h"
#include "ZFObjectCast.h"
#include "ZFAny.h"
#include "zfautoObjectFwd.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief callback type of ZFCallback
 */
typedef enum
{
    ZFCallbackTypeDummy, /**< @brief dummy that must not be executed */
    ZFCallbackTypeMethod, /**< @brief class static member method described by #ZFMethod */
    ZFCallbackTypeMemberMethod, /**< @brief class memeber method that need a object instance to execute, described by #ZFMethod */
    ZFCallbackTypeRawFunction, /**< @brief static function without #ZFMethod */
    ZFCallbackTypeLambda, /**< @brief lambda function that can capture other vars */
} ZFCallbackType;
/** @brief string tokens */
#define ZFTOKEN_ZFCallbackTypeDummy "TypeDummy"
/** @brief string tokens */
#define ZFTOKEN_ZFCallbackTypeMethod "TypeMethod"
/** @brief string tokens */
#define ZFTOKEN_ZFCallbackTypeMemberMethod "TypeMemeberMethod"
/** @brief string tokens */
#define ZFTOKEN_ZFCallbackTypeRawFunction "TypeRawFunction"
/** @brief string tokens */
#define ZFTOKEN_ZFCallbackTypeLambda "TypeLambda"

// ============================================================
// callback invoker
extern ZF_ENV_EXPORT void _ZFP_ZFCallback_executeNullCallback(void);
/**
 * @brief dummy type for function type pointer for ZFCallback
 */
#define _ZFP_ZFCALLBACK_INVOKER(N) \
    /** @brief see #ZFCallback, you must assign the exact return type and param types for safe */ \
    template<typename T_ReturnType ZFM_REPEAT(N, ZFM_REPEAT_TEMPLATE, ZFM_COMMA, ZFM_COMMA)> \
    T_ReturnType executeExact(ZFM_REPEAT(N, ZFM_REPEAT_PARAM, ZFM_EMPTY, ZFM_COMMA)) const \
    { \
        switch(this->callbackType()) \
        { \
            case ZFCallbackTypeMethod: \
            case ZFCallbackTypeMemberMethod: \
                return this->callbackMethod()->execute<T_ReturnType ZFM_REPEAT(N, ZFM_REPEAT_TYPE, ZFM_COMMA, ZFM_COMMA)>( \
                        this->callbackOwnerObject() \
                        ZFM_REPEAT(N, ZFM_REPEAT_NAME, ZFM_COMMA, ZFM_COMMA) \
                    ); \
            case ZFCallbackTypeRawFunction: \
                return ((T_ReturnType (*)(ZFM_REPEAT(N, ZFM_REPEAT_TYPE, ZFM_EMPTY, ZFM_COMMA)))(this->callbackRawFunction())) \
                    (ZFM_REPEAT(N, ZFM_REPEAT_NAME, ZFM_EMPTY, ZFM_COMMA)); \
            case ZFCallbackTypeLambda: \
                return ((T_ReturnType (*)(_ZFP_ZFCallbackLambda * ZFM_REPEAT(N, ZFM_REPEAT_TYPE, ZFM_COMMA, ZFM_COMMA)))(this->_ZFP_ZFCallback_callbackLambdaInvoker()))( \
                        this->_ZFP_ZFCallback_callbackLambdaImpl() \
                        ZFM_REPEAT(N, ZFM_REPEAT_NAME, ZFM_COMMA, ZFM_COMMA) \
                    ); \
            case ZFCallbackTypeDummy: \
            default: \
                _ZFP_ZFCallback_executeNullCallback(); \
                return ((T_ReturnType (*)(void))(zfnull)) (); \
        } \
    }

zfclassNotPOD _ZFP_ZFCallbackLambda
{
public:
    typedef void (*DestroyCallback)(ZF_IN _ZFP_ZFCallbackLambda *impl);
public:
    virtual ~_ZFP_ZFCallbackLambda(void) {}
public:
    virtual ZFFuncAddrType _ZFP_ivk(void) const zfpurevirtual;
};

// ============================================================
// ZFCallback
/**
 * @brief explicit disable callback serialization
 *
 * manually set to #ZFCallback::callbackSerializeCustomType before create the callback\n
 * serializing would cause additional data storage and processing,
 * explicitly disable serialization may improve performance for some case
 */
#define ZFCallbackSerializeCustomTypeDisable "-"

zfclassFwd ZFSerializableData;
zfclassFwd _ZFP_ZFCallbackPrivate;
/**
 * @brief callback used by ZFFramework
 *
 * a callback may be a class member or static member declared as #ZFMethod,
 * or static raw function,
 * it's a ZFMethod pointer container in fact\n
 * use ZFCallbackForXXX to create a callback,
 * then you may pass it as param or store it for future use\n
 * to execute callback, use #executeExact similar to ZFMethod::execute,
 * while you have no need to take care of the owner object
 * @warning before execute, you must check whether the callback is valid,
 *   by #callbackIsValid, otherwise, assert fail
 * @warning while execute, similar to ZFMethod,
 *   you must explicitly assign the ReturnType
 *   and each ParamType for the template param
 *
 * @note you may also declare a child class of ZFCallback,
 *   by ZFCALLBACK_DECLARE_XXX, see #ZFCALLBACK_DECLARE_BEGIN
 */
zfclassLikePOD ZF_ENV_EXPORT ZFCallback
{
public:
    /** @cond ZFPrivateDoc */
    ZFCallback(void);
    ZFCallback(const ZFCallback &ref);
    ZFCallback &operator = (const ZFCallback &ref);
    virtual ~ZFCallback(void);
    zfbool operator == (ZF_IN const ZFCallback &ref) const {return (this->objectCompare(ref) == ZFCompareTheSame);}
    zfbool operator != (ZF_IN const ZFCallback &ref) const {return (this->objectCompare(ref) != ZFCompareTheSame);}
    static ZFCallback _ZFP_ZFCallbackCreate(ZF_IN ZFCallbackType callbackType,
                                            ZF_IN ZFObject *callbackOwnerObject,
                                            ZF_IN const ZFMethod *callbackMethod,
                                            ZF_IN ZFFuncAddrType callbackRawFunction,
                                            ZF_IN _ZFP_ZFCallbackLambda *callbackLambdaImpl,
                                            ZF_IN _ZFP_ZFCallbackLambda::DestroyCallback callbackLambdaImplDestroy);
    /** @endcond */

    _ZFP_ZFCALLBACK_INVOKER(0)
    _ZFP_ZFCALLBACK_INVOKER(1)
    _ZFP_ZFCALLBACK_INVOKER(2)
    _ZFP_ZFCALLBACK_INVOKER(3)
    _ZFP_ZFCALLBACK_INVOKER(4)
    _ZFP_ZFCALLBACK_INVOKER(5)
    _ZFP_ZFCALLBACK_INVOKER(6)
    _ZFP_ZFCALLBACK_INVOKER(7)
    _ZFP_ZFCALLBACK_INVOKER(8)
    /* ZFMETHOD_MAX_PARAM */

public:
    /**
     * @brief get current retain count
     */
    zffinal zfindex objectRetainCount(void) const;

    /** @brief see #objectInfo */
    zffinal void objectInfoT(ZF_IN_OUT zfstring &ret) const;
    /** @brief return object info */
    zffinal inline zfstring objectInfo(void) const
    {
        zfstring ret;
        this->objectInfoT(ret);
        return ret;
    }

    /**
     * @brief compare with another callback
     *
     * two callback is considered the same if all of these are same:
     * -  #callbackType
     * -  #callbackOwnerObject
     * -  #callbackMethod
     * -  #callbackRawFunction
     */
    zffinal ZFCompareResult objectCompare(ZF_IN const ZFCallback &ref) const;
    /**
     * @brief compare callback by instance only (same callback contents not necessarily to be same instance)
     */
    zffinal ZFCompareResult objectCompareByInstance(ZF_IN const ZFCallback &ref) const
    {
        return ((d == ref.d) ? ZFCompareTheSame : ZFCompareUncomparable);
    }

public:
    /**
     * @brief an unique id for the callback, used for cache logic
     *
     * two callback with same callback id is treated as same callback,
     * which is useful to achieve cache logic\n
     * you must ensure the callback id is unique and verbose enough to describe the callback,
     * otherwise, leave it empty
     */
    zffinal void callbackId(ZF_IN const zfchar *callbackId);
    /**
     * @brief see #callbackId
     */
    zffinal const zfchar *callbackId(void) const;

    /**
     * @brief retain and store a autoreleased object attached to this callback,
     *   or set null to remove
     *
     * recommended to have "ZFCallbackTagKeyword_" as prefix for key name\n
     * usually used to store the owner object,
     * so that it'll be released automatically when all the callback's
     * references were deleted,
     * you can also retain the owner by #callbackOwnerObjectRetain\n
     * you can also save state for the callback as the auto released data
     */
    zffinal void callbackTag(ZF_IN const zfchar *key,
                             ZF_IN ZFObject *tag);
    /** @brief see #callbackTag */
    zffinal ZFObject *callbackTag(ZF_IN const zfchar *key) const;
    /** @brief see #callbackTag */
    template<typename T_ZFObject>
    T_ZFObject callbackTag(ZF_IN const zfchar *key) const
    {
        return ZFCastZFObjectUnchecked(T_ZFObject, this->callbackTag(key));
    }
    /** @brief see #callbackTag */
    zffinal void callbackTagGetAllKeyValue(ZF_IN_OUT ZFCoreArray<const zfchar *> &allKey,
                                           ZF_IN_OUT ZFCoreArray<ZFObject *> &allValue) const;
    /**
     * @brief remove tag, same as set tag to null
     */
    inline void callbackTagRemove(ZF_IN const zfchar *key)
    {
        this->callbackTag(key, zfnull);
    }
    /**
     * @brief remove tag, return removed tag or null if not exist
     */
    zffinal zfautoObject callbackTagRemoveAndGet(ZF_IN const zfchar *key);
    /**
     * @brief see #callbackTag
     *
     * @note it's unsafe to remove all tag manually,
     *   which may break unrelated modules' additional logic,
     *   remove only if necessary\n
     *   typically, you should remove exactly the one you have added
     */
    zffinal void callbackTagRemoveAll(void);

    /**
     * @brief return true if callback is valid
     */
    zffinal inline zfbool callbackIsValid(void) const
    {
        return (this->callbackType() != ZFCallbackTypeDummy);
    }

    /**
     * @brief get the type of callback
     */
    zffinal ZFCallbackType callbackType(void) const;

    /**
     * @brief get the owner object,
     *   valid only if type is class member method
     */
    zffinal ZFObject *callbackOwnerObject(void) const;

    /**
     * @brief get the method or null if not declared by ZFMethod
     */
    zffinal const ZFMethod *callbackMethod(void) const;

    /**
     * @brief get static function, valid only if type is #ZFCallbackTypeRawFunction
     */
    zffinal ZFFuncAddrType callbackRawFunction(void) const;

    /**
     * @brief clear and reset to dummy callback,
     *   as well as all attached tags
     *
     * release reference count only,
     * if other callback is refering to the internal data,
     * the data won't be deleted
     */
    zffinal void callbackClear(void);

    /**
     * @brief retain owner object and auto release it
     *   after callback's retain count reduced to 0
     *
     * by default, owner won't be retained,
     * you may use this method to retain owner,
     * or use #callbackTag to store owner
     */
    zffinal void callbackOwnerObjectRetain(void) const;
    /**
     * @brief manually release the owner, see #callbackOwnerObjectRetain
     */
    zffinal void callbackOwnerObjectRelease(void) const;

    // ============================================================
    // callback serialize logic
public:
    /**
     * @brief see #ZFTypeId_ZFCallback
     *
     * you may set to #ZFCallbackSerializeCustomTypeDisable to explicitly
     * disable callback serialization
     */
    zffinal void callbackSerializeCustomType(ZF_IN const zfchar *customType);
    /**
     * @brief see #ZFTypeId_ZFCallback
     */
    zffinal const zfchar *callbackSerializeCustomType(void) const;
    /**
     * @brief see #ZFTypeId_ZFCallback
     */
    zffinal void callbackSerializeCustomData(ZF_IN const ZFSerializableData *customData);
    /**
     * @brief see #ZFTypeId_ZFCallback
     */
    zffinal void callbackSerializeCustomData(ZF_IN const ZFSerializableData &customData)
    {
        this->callbackSerializeCustomData(&customData);
    }
    /**
     * @brief see #ZFTypeId_ZFCallback
     */
    zffinal const ZFSerializableData *callbackSerializeCustomData(void) const;

    /**
     * @brief see #ZFTypeId_ZFCallback
     */
    zffinal void callbackSerializeCustomDisable(ZF_IN zfbool disable)
    {
        this->callbackSerializeCustomType(disable ? ZFCallbackSerializeCustomTypeDisable : zfnull);
    }
    /**
     * @brief see #ZFTypeId_ZFCallback
     */
    zffinal zfbool callbackSerializeCustomDisabled(void) const
    {
        return zfscmpTheSame(this->callbackSerializeCustomType(), ZFCallbackSerializeCustomTypeDisable);
    }

    // ============================================================
    // local path logic
public:
    /**
     * @brief used for impl to store path related info, see #ZFPathInfo
     */
    zffinal const ZFPathInfo *pathInfo(void) const;
    /** @brief see #pathInfo */
    zffinal void pathInfo(ZF_IN const ZFPathInfo *pathInfo);
    /** @brief see #pathInfo */
    zffinal void pathInfo(ZF_IN const zfchar *pathType, ZF_IN const zfchar *pathData);

private:
    _ZFP_ZFCallbackPrivate *d;
    zffinal _ZFP_ZFCallbackLambda *_ZFP_ZFCallback_callbackLambdaImpl(void) const;
    zffinal ZFFuncAddrType _ZFP_ZFCallback_callbackLambdaInvoker(void) const;
};

// ============================================================
// ZFCallback create
/**
 * @brief an empty callback
 */
#define ZFCallbackNull() \
    ZFCallback::_ZFP_ZFCallbackCreate( \
        ZFCallbackTypeDummy, \
        zfnull, \
        zfnull, \
        zfnull, \
        zfnull, \
        zfnull)

/**
 * @brief create a callback from ZFMethod
 *
 * assert fail if method not valid\n
 * method can be local method or class static member method
 */
#define ZFCallbackForMethod(zfmethod) \
    ZFCallback::_ZFP_ZFCallbackCreate( \
        ZFCallbackTypeMethod, \
        zfnull, \
        zfmethod, \
        zfnull, \
        zfnull, \
        zfnull)

/**
 * @brief create a callback from ZFMethod of an object
 *
 * assert fail if method not valid or obj has no such method\n
 * obj won't be retained unless #ZFCallback::callbackOwnerObjectRetain is called,
 * so you must make sure the obj is alive during callback's invocation
 */
#define ZFCallbackForMemberMethod(obj, zfmethod) \
    ZFCallback::_ZFP_ZFCallbackCreate( \
        ZFCallbackTypeMemberMethod, \
        obj, \
        zfmethod, \
        zfnull, \
        zfnull, \
        zfnull)

/**
 * @brief create a callback from static function
 *
 * assert fail if function address not valid
 */
#define ZFCallbackForFunc(callbackRawFunction) \
    ZFCallback::_ZFP_ZFCallbackCreate( \
        ZFCallbackTypeRawFunction, \
        zfnull, \
        zfnull, \
        (ZFFuncAddrType)(void (*)(const ZFListenerData &, ZFObject *))(callbackRawFunction), \
        zfnull, \
        zfnull)

// ============================================================
// child callback declare
#define _ZFP_ZFCALLBACK_DECLARE_BEGIN(CallbackTypeName, ParentType) \
    zfclassLikePOD ZF_ENV_EXPORT CallbackTypeName : zfextendsLikePOD ParentType \
    { \
        _ZFP_ZFCALLBACK_DECLARE_CONSTRUCTORS(CallbackTypeName, ParentType) \
    public:
#define _ZFP_ZFCALLBACK_DECLARE_END(CallbackTypeName, ParentType) \
    };
#define _ZFP_ZFCALLBACK_DECLARE_CONSTRUCTORS(CallbackTypeName, ParentType) \
    public: \
        /** @cond ZFPrivateDoc */ \
        CallbackTypeName(void) \
        : ParentType() \
        { \
        } \
        CallbackTypeName(ZF_IN const ZFCallback &ref) \
        : ParentType(ref) \
        { \
        } \
        CallbackTypeName &operator = (const CallbackTypeName &ref) \
        { \
            ZFCallback::operator = (ref); \
            return *this; \
        } \
        /** @endcond */
/**
 * @brief see #ZFCALLBACK_DECLARE_BEGIN
 */
#define ZFCALLBACK_DECLARE_END(CallbackTypeName, ParentType) \
    _ZFP_ZFCALLBACK_DECLARE_END(CallbackTypeName, ParentType) \
    ZFTYPEID_ALIAS_DECLARE(ZFCallback, ZFCallback, CallbackTypeName, CallbackTypeName)
#define _ZFP_ZFCALLBACK_DECLARE_END_NO_ALIAS(CallbackTypeName, ParentType) \
    _ZFP_ZFCALLBACK_DECLARE_END(CallbackTypeName, ParentType)

/**
 * @brief util macro to declare a child type of ZFCallback
 *
 * declaration:
 * @code
 *   // in header file
 *   // declare by default:
 *   ZFCALLBACK_DECLARE(CallbackTypeName, ParentType)
 *
 *   // if you need extra functions
 *   ZFCALLBACK_DECLARE_BEGIN(CallbackTypeName, ParentType)
 *       // your extra functions here
 *       // it's not allowed to add member variables
 *   ZFCALLBACK_DECLARE_END(CallbackTypeName, ParentType)
 *
 *   // in source file
 *   ZFCALLBACK_DEFINE(CallbackTypeName, ParentType)
 * @endcode
 */
#define ZFCALLBACK_DECLARE_BEGIN(CallbackTypeName, ParentType) \
    _ZFP_ZFCALLBACK_DECLARE_BEGIN(CallbackTypeName, ParentType)
/**
 * @brief see #ZFCALLBACK_DECLARE_BEGIN
 */
#define ZFCALLBACK_DECLARE(CallbackTypeName, ParentType) \
    ZFCALLBACK_DECLARE_BEGIN(CallbackTypeName, ParentType) \
    ZFCALLBACK_DECLARE_END(CallbackTypeName, ParentType)

/**
 * @brief see #ZFCALLBACK_DECLARE_BEGIN
 */
#define ZFCALLBACK_DEFINE(CallbackTypeName, ParentType) \
    ZFTYPEID_ALIAS_DEFINE(ZFCallback, ZFCallback, CallbackTypeName, CallbackTypeName)

// ============================================================
/**
 * @brief declare callback from a local declared function
 *
 * usage:
 * @code
 *   // declare
 *   ZFCALLBACK_LOCAL_BEGIN_1(ReturnType, callback, ParamType0, paramName0)
 *   {
 *       // your code here
 *       return ReturnType();
 *   }
 *   ZFCALLBACK_LOCAL_END(callback)
 *   // run
 *   ReturnType ret = callback.executeExact<ReturnType, ParamType0>(param0);
 * @endcode
 *
 * note, this method is used for convenient only,
 * it's not reflectable
 */
#define ZFCALLBACK_LOCAL_BEGIN_0(ReturnType, callbackName) \
    zfclassNotPOD ZFM_CAT(_ZFP_ZFCallbackLocal_, callbackName) \
    { \
    public: \
        static ReturnType callbackName(void)

/** @brief see #ZFCALLBACK_LOCAL_BEGIN_0 */
#define ZFCALLBACK_LOCAL_BEGIN_1(ReturnType, callbackName, ParamType0, paramName0) \
    zfclassNotPOD ZFM_CAT(_ZFP_ZFCallbackLocal_, callbackName) \
    { \
    public: \
        static ReturnType callbackName(ParamType0 paramName0)

/** @brief see #ZFCALLBACK_LOCAL_BEGIN_0 */
#define ZFCALLBACK_LOCAL_BEGIN_2(ReturnType, callbackName, ParamType0, paramName0, ParamType1, paramName1) \
    zfclassNotPOD ZFM_CAT(_ZFP_ZFCallbackLocal_, callbackName) \
    { \
    public: \
        static ReturnType callbackName(ParamType0 paramName0, ParamType1 paramName1)

/** @brief see #ZFCALLBACK_LOCAL_BEGIN_0 */
#define ZFCALLBACK_LOCAL_BEGIN_3(ReturnType, callbackName, ParamType0, paramName0, ParamType1, paramName1, ParamType2, paramName2) \
    zfclassNotPOD ZFM_CAT(_ZFP_ZFCallbackLocal_, callbackName) \
    { \
    public: \
        static ReturnType callbackName(ParamType0 paramName0, ParamType1 paramName1, ParamType2 paramName2)

/** @brief see #ZFCALLBACK_LOCAL_BEGIN_0 */
#define ZFCALLBACK_LOCAL_BEGIN_4(ReturnType, callbackName, ParamType0, paramName0, ParamType1, paramName1, ParamType2, paramName2, ParamType3, paramName3) \
    zfclassNotPOD ZFM_CAT(_ZFP_ZFCallbackLocal_, callbackName) \
    { \
    public: \
        static ReturnType callbackName(ParamType0 paramName0, ParamType1 paramName1, ParamType2 paramName2, ParamType3 paramName3)

/** @brief see #ZFCALLBACK_LOCAL_BEGIN_0 */
#define ZFCALLBACK_LOCAL_BEGIN_5(ReturnType, callbackName, ParamType0, paramName0, ParamType1, paramName1, ParamType2, paramName2, ParamType3, paramName3, ParamType4, paramName4) \
    zfclassNotPOD ZFM_CAT(_ZFP_ZFCallbackLocal_, callbackName) \
    { \
    public: \
        static ReturnType callbackName(ParamType0 paramName0, ParamType1 paramName1, ParamType2 paramName2, ParamType3 paramName3, ParamType4 paramName4)

/** @brief see #ZFCALLBACK_LOCAL_BEGIN_0 */
#define ZFCALLBACK_LOCAL_BEGIN_6(ReturnType, callbackName, ParamType0, paramName0, ParamType1, paramName1, ParamType2, paramName2, ParamType3, paramName3, ParamType4, paramName4, ParamType5, paramName5) \
    zfclassNotPOD ZFM_CAT(_ZFP_ZFCallbackLocal_, callbackName) \
    { \
    public: \
        static ReturnType callbackName(ParamType0 paramName0, ParamType1 paramName1, ParamType2 paramName2, ParamType3 paramName3, ParamType4 paramName4, ParamType5 paramName5)

/** @brief see #ZFCALLBACK_LOCAL_BEGIN_0 */
#define ZFCALLBACK_LOCAL_BEGIN_7(ReturnType, callbackName, ParamType0, paramName0, ParamType1, paramName1, ParamType2, paramName2, ParamType3, paramName3, ParamType4, paramName4, ParamType5, paramName5, ParamType6, paramName6) \
    zfclassNotPOD ZFM_CAT(_ZFP_ZFCallbackLocal_, callbackName) \
    { \
    public: \
        static ReturnType callbackName(ParamType0 paramName0, ParamType1 paramName1, ParamType2 paramName2, ParamType3 paramName3, ParamType4 paramName4, ParamType5 paramName5, ParamType6 paramName6)

/** @brief see #ZFCALLBACK_LOCAL_BEGIN_0 */
#define ZFCALLBACK_LOCAL_BEGIN_8(ReturnType, callbackName, ParamType0, paramName0, ParamType1, paramName1, ParamType2, paramName2, ParamType3, paramName3, ParamType4, paramName4, ParamType5, paramName5, ParamType6, paramName6, ParamType7, paramName7) \
    zfclassNotPOD ZFM_CAT(_ZFP_ZFCallbackLocal_, callbackName) \
    { \
    public: \
        static ReturnType callbackName(ParamType0 paramName0, ParamType1 paramName1, ParamType2 paramName2, ParamType3 paramName3, ParamType4 paramName4, ParamType5 paramName5, ParamType6 paramName6, ParamType7 paramName7)
/* ZFMETHOD_MAX_PARAM */

/** @brief see #ZFCALLBACK_LOCAL_BEGIN_0 */
#define ZFCALLBACK_LOCAL_END(callbackName) \
    ZFCALLBACK_LOCAL_END_WITH_TYPE(ZFCallback, callbackName)
/** @brief see #ZFCALLBACK_LOCAL_BEGIN_0 */
#define ZFCALLBACK_LOCAL_END_WITH_TYPE(CallbackType, callbackName) \
    }; \
    CallbackType callbackName = ZFCallbackForFunc((ZFFuncAddrType)(ZFM_CAT(_ZFP_ZFCallbackLocal_, callbackName)::callbackName));

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFCallback_h_

