/**
 * @file ZFObjectClassTypeFwd.h
 * @brief types for ZFFramework
 */

#ifndef _ZFI_ZFObjectClassTypeFwd_h_
#define _ZFI_ZFObjectClassTypeFwd_h_

#include "../ZFCoreDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief same as class, shows that this class is a #ZFObject type
 *
 * we use some dummy macro to show the basic type of class:
 * -  zfclassFwd: forward declare for a type
 * -  zfclass: shows that it's a subclass of ZFObject,
 *   note that a ZFObject is always not a POD type
 *   and always can't be declared in stack
 * -  zfabstract: shows that it's a abstract class of ZFObject type
 * -  zfclassPOD: shows that it's a POD type
 * -  zfclassLikePOD: not POD type, but you may use it like POD,
 *   e.g. declare in stack, copy value by operator = (if available),
 *   except one limitation: must not memset to 0
 * -  zfclassNotPOD: shows that it's neither ZFObject nor POD type
 *   (or not designed as POD type),
 *   and usually should not be declared in stack
 *   or copy it by copy constructor or operator =
 *
 * @warning zfclassLikePOD is not actually a POD,
 *   usually has it's own constructor or destructor,
 *   so it's highly deprecated to declare a zfclassLikePOD as static variable,
 *   use ZF_GLOBAL_INITIALIZER_INIT or ZF_STATIC_INITIALIZER_INIT instead
 *   to declare in global
 */
#define zfclass class
/**
 * @brief typename for class showing that its abstract
 */
#define zfabstract class

/**
 * @brief declare a class inherit from other class
 */
#define zfextends public

// ============================================================
/**
 * @brief ZFObject auto retain version of #ZFCORE_PARAM
 */
#define ZFCORE_PARAM_RETAIN(T_ParamType, paramName) \
    ZFCORE_PARAM_RETAIN_WITH_INIT(T_ParamType, paramName, ZFM_EMPTY())
/**
 * @brief see #ZFCORE_PARAM_RETAIN
 */
#define ZFCORE_PARAM_RETAIN_WITH_INIT(T_ParamType, paramName, initValue) \
    public: \
        T_ParamType const &paramName(void) const \
        { \
            return this->paramName##_.value; \
        } \
    private: \
        /** @cond ZFPrivateDoc */ \
        zfclassLikePOD ZF_ENV_EXPORT paramName##_ZFCoreParam \
        { \
        public: \
            paramName##_ZFCoreParam(void) \
            : value(zfnull) \
            { \
                T_ParamType *t = zfnull; \
                this->value = zfRetain(*(t = zfnew(T_ParamType, initValue))); \
                zfdelete(t); \
            } \
            paramName##_ZFCoreParam(ZF_IN const paramName##_ZFCoreParam &ref) \
            { \
                this->value = zfRetain(ref.value); \
            } \
            ~paramName##_ZFCoreParam(void) \
            { \
                zfRelease(this->value); \
            } \
            paramName##_ZFCoreParam &operator = (ZF_IN const paramName##_ZFCoreParam &ref) \
            { \
                zfCoreMutexLock(); \
                zflockfree_zfRetain(ref.value); \
                zflockfree_zfRelease(this->value); \
                zfCoreMutexUnlock(); \
                this->value = ref.value; \
                return *this; \
            } \
        public: \
            T_ParamType value; \
        }; \
        paramName##_ZFCoreParam paramName##_; \
        /** @endcond */ \
    public: \
        /** @brief see @ref paramName */ \
        _ZFP_ZFCoreParam_self &paramName(ZF_IN T_ParamType const &value) \
        { \
            zfCoreMutexLock(); \
            zflockfree_zfRetain(value); \
            zflockfree_zfRelease(this->paramName##_.value); \
            zfCoreMutexUnlock(); \
            this->paramName##_.value = value; \
            return *this; \
        }

// ============================================================
#define _ZFP_ZFCORE_PARAM_RETAIN_DECLARE(T_ParamType, paramName) \
    public: \
        T_ParamType const &paramName(void) const; \
    private: \
        /** @cond ZFPrivateDoc */ \
        zfclassLikePOD ZF_ENV_EXPORT paramName##_ZFCoreParam \
        { \
        public: \
            paramName##_ZFCoreParam(void); \
            paramName##_ZFCoreParam(ZF_IN const paramName##_ZFCoreParam &ref); \
            ~paramName##_ZFCoreParam(void); \
            paramName##_ZFCoreParam &operator = (ZF_IN const paramName##_ZFCoreParam &ref); \
        public: \
            T_ParamType value; \
        }; \
        paramName##_ZFCoreParam paramName##_; \
        /** @endcond */ \
    public: \
        /** @brief see @ref paramName */ \
        _ZFP_ZFCoreParam_self &paramName(ZF_IN T_ParamType const &value);
#define _ZFP_ZFCORE_PARAM_RETAIN_DEFINE(T_Owner, T_ParamType, paramName, initValue) \
    T_ParamType const &T_Owner::paramName(void) const \
    { \
        return this->paramName##_.value; \
    } \
    /** @cond ZFPrivateDoc */ \
    T_Owner::paramName##_ZFCoreParam::paramName##_ZFCoreParam(void) \
    { \
        T_ParamType *t = zfnull; \
        this->value = zfRetain(*(t = zfnew(T_ParamType, initValue))); \
        zfdelete(t); \
    } \
    T_Owner::paramName##_ZFCoreParam::paramName##_ZFCoreParam(ZF_IN const T_Owner::paramName##_ZFCoreParam &ref) \
    { \
        this->value = zfRetain(ref.value); \
    } \
    T_Owner::paramName##_ZFCoreParam::~paramName##_ZFCoreParam(void) \
    { \
        zfRelease(this->value); \
    } \
    T_Owner::paramName##_ZFCoreParam &T_Owner::paramName##_ZFCoreParam::operator = (ZF_IN const T_Owner::paramName##_ZFCoreParam &ref) \
    { \
        zfCoreMutexLock(); \
        zflockfree_zfRetain(ref.value); \
        zflockfree_zfRelease(this->value); \
        zfCoreMutexUnlock(); \
        this->value = ref.value; \
        return *this; \
    } \
    /** @endcond */ \
    T_Owner::_ZFP_ZFCoreParam_self &T_Owner::paramName(ZF_IN T_ParamType const &value) \
    { \
        zfCoreMutexLock(); \
        zflockfree_zfRetain(value); \
        zflockfree_zfRelease(this->paramName##_.value); \
        zfCoreMutexUnlock(); \
        this->paramName##_.value = value; \
        return *this; \
    }

// ============================================================
zfclassFwd ZFObject;
zfclassFwd ZFInterface;
zfclassFwd ZFClass;
zfclassFwd ZFProperty;
zfclassFwd ZFMethod;
zfclassFwd zfautoObject;

// ============================================================
/**
 * @brief function type to create object
 */
typedef zfautoObject (*ZFObjectCreator)(void);

// ============================================================
/**
 * @brief true if Type is #ZFObject type
 */
#define zftIsZFObject(Type) (ZFM_CLASS_HAS_MEMBER(_ZFP_zftIsZFObjectCheck, _ZFP_zftIsZFObject, Type) ? 1 : 0)
ZFM_CLASS_HAS_MEMBER_DECLARE(_ZFP_zftIsZFObjectCheck, _ZFP_zftIsZFObject, void (*F)(void))

// ============================================================
/** @brief type for #ZFGlobalEvent::EventClassDataChange */
typedef enum {
    ZFClassDataChangeTypeAttach, /**< @brief attach */
    ZFClassDataChangeTypeDetach, /**< @brief detach */
    ZFClassDataChangeTypeUpdate, /**< @brief update */
} ZFClassDataChangeType;
/** @brief data holder for #ZFGlobalEvent::EventClassDataChange */
zfclassPOD ZF_ENV_EXPORT ZFClassDataChangeData
{
public:
    ZFClassDataChangeType changeType; /**< @brief change type */
    const ZFClass *changedClass; /**< @brief changed class */
    const ZFProperty *changedProperty; /**< @brief changed property */
    const ZFMethod *changedMethod; /**< @brief changed method */
};
extern ZF_ENV_EXPORT void _ZFP_ZFClassDataChangeNotify(ZF_IN ZFClassDataChangeType changeType,
                                                       ZF_IN const ZFClass *changedClass,
                                                       ZF_IN const ZFProperty *changedProperty,
                                                       ZF_IN const ZFMethod *changedMethod);

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFObjectClassTypeFwd_h_

