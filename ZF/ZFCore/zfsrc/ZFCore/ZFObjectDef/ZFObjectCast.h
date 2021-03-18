/**
 * @file ZFObjectCast.h
 * @brief ZFCastZFObject declarations
 */

#ifndef _ZFI_ZFObjectCast_h_
#define _ZFI_ZFObjectCast_h_

#include "ZFObjectClassTypeFwd.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd ZFClass;
zfclassFwd ZFObject;
zfclassFwd ZFInterface;

// ============================================================
/**
 * @brief safely cast ZFObject types, return null if not desired type
 *
 * types can only be ZFObject or ZFInterface types\n
 * \n
 * ADVANCED:\n
 * -  cast between ZFInterface or it's children types may take much time for dynamic checking,
 *   use with caution
 * -  try not to cast from and to the base ZFInterface * type
 */
#define ZFCastZFObject(T_To, obj) _ZFP_ObjCast<T_To>::c(obj)

/**
 * @brief cast ZFObject types without safe check, use only for performance
 *
 * ADVANCED:\n
 * -  this method would have better performance while cast ZFObject types to ZFObject types,
 *   while cast ZFInterface types would always consume some CPU for dynamic check
 * -  try not to cast from and to the base ZFInterface * type
 */
#define ZFCastZFObjectUnchecked(T_To, obj) _ZFP_ObjCastUnchecked<T_To>::c(obj)

#define _ZFP_ObjCastInternal(T_To, obj) \
    static_cast<T_To>(obj)


// ============================================================
// for impl
/** @cond ZFPrivateDoc */
template<typename T_ZFObjectTo, typename T_ZFObjectFrom>
inline T_ZFObjectTo _ZFP_ObjCastExplicit(T_ZFObjectFrom const &obj)
{
    return ((obj->classData()->classIsTypeOf(zftTraits<T_ZFObjectTo>::TrType::ClassData()))
        ? static_cast<T_ZFObjectTo>(obj)
        : zfnull);
}
/** @endcond */



// ============================================================
#define _ZFP_ObjCastTypeZFObject 1
#define _ZFP_ObjCastTypeZFInterface 2
#define _ZFP_ObjCastTypeUnknown 3

#define _ZFP_ObjCastType(TrType) \
    (zftIsZFObject(TrType) \
        ? (zftTypeIsTypeOf<TrType, ZFObject>::TypeIsTypeOf \
                ? _ZFP_ObjCastTypeZFObject \
                : _ZFP_ObjCastTypeZFInterface \
            ) \
        : _ZFP_ObjCastTypeUnknown)


// ============================================================
// ZFCastZFObject
template<typename T_To>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_ObjCast
{
public:
    template<typename T_From>
    static inline T_To c(T_From const &obj);
};
template<int T_CanCastDirectly, typename T_To, typename T_From, int T_ToType, int T_FromType>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_ObjCastHolder
{
};

// can cast directly
template<typename T_To, typename T_From, int T_ToType, int T_FromType>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_ObjCastHolder<1, T_To, T_From, T_ToType, T_FromType>
{
public:
    static inline T_To c(T_From const &obj)
    {
        return static_cast<T_To>(obj);
    }
};
// object to object
template<typename T_To, typename T_From>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_ObjCastHolder<0, T_To, T_From, _ZFP_ObjCastTypeZFObject, _ZFP_ObjCastTypeZFObject>
{
public:
    static inline T_To c(T_From const &obj)
    {
        return (obj
            ? _ZFP_ObjCastExplicit<T_To>(obj)
            : zfnull);
    }
};
// object to interface
template<typename T_To, typename T_From>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_ObjCastHolder<0, T_To, T_From, _ZFP_ObjCastTypeZFInterface, _ZFP_ObjCastTypeZFObject>
{
public:
    static inline T_To c(T_From const &obj)
    {
        return (obj
            ? static_cast<T_To>(obj->classData()->_ZFP_ZFClass_interfaceCast(
                obj,
                zftTraits<T_To>::TrType::ClassData()))
            : zfnull);
    }
};
// interface to object
template<typename T_To, typename T_From>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_ObjCastHolder<0, T_To, T_From, _ZFP_ObjCastTypeZFObject, _ZFP_ObjCastTypeZFInterface>
{
public:
    static inline T_To c(T_From const &obj)
    {
        return (obj
            ? _ZFP_ObjCastExplicit<T_To>(obj->toObject())
            : zfnull);
    }
};
// interface to interface
template<typename T_To, typename T_From>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_ObjCastHolder<0, T_To, T_From, _ZFP_ObjCastTypeZFInterface, _ZFP_ObjCastTypeZFInterface>
{
public:
    static inline T_To c(T_From const &obj)
    {
        return (obj
            ? static_cast<T_To>(obj->classData()->_ZFP_ZFClass_interfaceCast(
                obj->toObject(),
                zftTraits<T_To>::TrType::ClassData()))
            : zfnull);
    }
};

// invoker
template<typename T_To>
template<typename T_From>
inline T_To _ZFP_ObjCast<T_To>::c(T_From const &obj)
{
    typedef typename zftTraits<T_To>::TrType T_ToTmp;
    typedef typename zftTraits<T_From>::TrType T_FromTmp;
    return _ZFP_ObjCastHolder<
            zftTypeIsTypeOf<typename zftTraits<T_From>::TrType, typename zftTraits<T_To>::TrType>::TypeIsTypeOf,
            T_To, T_From,
            _ZFP_ObjCastType(T_ToTmp), _ZFP_ObjCastType(T_FromTmp)
        >::c(obj);
}
template<>
template<typename T_From>
inline ZFInterface *_ZFP_ObjCast<ZFInterface *>::c(T_From const &obj)
{
    zfCoreCriticalMessageTrim("[ZFCastZFObjectUnchecked] direct cast to ZFInterface is not allowed");
    return zfnull;
}


// ============================================================
// ZFCastZFObjectUnchecked
template<typename T_To>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_ObjCastUnchecked
{
public:
    template<typename T_From>
    static inline T_To c(T_From const &obj);
};
template<int T_CanCastDirectly, typename T_To, typename T_From, int T_ToType, int T_FromType>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_ObjCastUncheckedHolder
{
};

// can cast directly
template<typename T_To, typename T_From, int T_ToType, int T_FromType>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_ObjCastUncheckedHolder<1, T_To, T_From, T_ToType, T_FromType>
{
public:
    static inline T_To c(T_From const &obj)
    {
        return static_cast<T_To>(obj);
    }
};
// object to object
template<typename T_To, typename T_From>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_ObjCastUncheckedHolder<0, T_To, T_From, _ZFP_ObjCastTypeZFObject, _ZFP_ObjCastTypeZFObject>
{
public:
    static inline T_To c(T_From const &obj)
    {
        return static_cast<T_To>(obj);
    }
};
// object to interface
template<typename T_To, typename T_From>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_ObjCastUncheckedHolder<0, T_To, T_From, _ZFP_ObjCastTypeZFInterface, _ZFP_ObjCastTypeZFObject>
{
public:
    static inline T_To c(T_From const &obj)
    {
        return (obj
            ? static_cast<T_To>(obj->classData()->_ZFP_ZFClass_interfaceCast(
                obj,
                zftTraits<T_To>::TrType::ClassData()))
            : zfnull);
    }
};
// interface to object
template<typename T_To, typename T_From>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_ObjCastUncheckedHolder<0, T_To, T_From, _ZFP_ObjCastTypeZFObject, _ZFP_ObjCastTypeZFInterface>
{
public:
    static inline T_To c(T_From const &obj)
    {
        return (obj
            ? static_cast<T_To>(obj->toObject())
            : zfnull);
    }
};
// interface to interface
template<typename T_To, typename T_From>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_ObjCastUncheckedHolder<0, T_To, T_From, _ZFP_ObjCastTypeZFInterface, _ZFP_ObjCastTypeZFInterface>
{
public:
    static inline T_To c(T_From const &obj)
    {
        return (obj
            ? static_cast<T_To>(obj->classData()->_ZFP_ZFClass_interfaceCast(
                obj->toObject(),
                zftTraits<T_To>::TrType::ClassData()))
            : zfnull);
    }
};

// invoker
template<typename T_To>
template<typename T_From>
inline T_To _ZFP_ObjCastUnchecked<T_To>::c(T_From const &obj)
{
    typedef typename zftTraits<T_To>::TrType T_ToTmp;
    typedef typename zftTraits<T_From>::TrType T_FromTmp;
    return _ZFP_ObjCastUncheckedHolder<
            zftTypeIsTypeOf<typename zftTraits<T_From>::TrType, typename zftTraits<T_To>::TrType>::TypeIsTypeOf,
            T_To, T_From,
            _ZFP_ObjCastType(T_ToTmp), _ZFP_ObjCastType(T_FromTmp)
        >::c(obj);
}
template<>
template<typename T_From>
inline ZFInterface *_ZFP_ObjCastUnchecked<ZFInterface *>::c(T_From const &obj)
{
    zfCoreCriticalMessageTrim("[ZFCastZFObjectUnchecked] direct cast to ZFInterface is not allowed");
    return zfnull;
}

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFObjectCast_h_

