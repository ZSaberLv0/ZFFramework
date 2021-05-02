/**
 * @file zfautoObject.h
 * @brief smart pointer for ZFObject
 */

#ifndef _ZFI_zfautoObject_h_
#define _ZFI_zfautoObject_h_

#include "ZFObjectRetain.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// zfautoObject
/** @cond ZFPrivateDoc */
template<typename T_ZFObject>
zfautoObject::zfautoObject(ZF_IN T_ZFObject *p)
: d(zfnull)
{
    ZFObject *obj = (p ? p->toObject() : zfnull);
    if(obj)
    {
        zfCoreMutexLock();
        d = zfpoolNew(_ZFP_zfautoObjectPrivate,
            zflockfree_zfRetain(obj));
        zfCoreMutexUnlock();
    }
}
template<typename T_ZFObject>
zfautoObject::zfautoObject(ZF_IN T_ZFObject const &p)
: d(zfnull)
{
    ZFObject *obj = _ZFP_ZFAnyCast(T_ZFObject, p);
    if(obj)
    {
        zfCoreMutexLock();
        d = zfpoolNew(_ZFP_zfautoObjectPrivate,
            zflockfree_zfRetain(obj));
        zfCoreMutexUnlock();
    }
}

template<typename T_ZFObject>
zfautoObject &zfautoObject::operator = (ZF_IN T_ZFObject *p)
{
    zfCoreMutexLock();
    this->zflockfree_assign(p ? p->toObject() : zfnull);
    zfCoreMutexUnlock();
    return *this;
}
template<typename T_ZFObject>
zfautoObject &zfautoObject::operator = (ZF_IN T_ZFObject const &p)
{
    zfCoreMutexLock();
    this->zflockfree_assign(_ZFP_ZFAnyCast(T_ZFObject, p));
    zfCoreMutexUnlock();
    return *this;
}
/** @endcond */

// ============================================================
/** @cond ZFPrivateDoc */
extern ZF_ENV_EXPORT void _ZFP_zfautoObjectTError(void);
template<typename T_ZFObjectBase>
zfautoObjectT<T_ZFObjectBase>::zfautoObjectT(ZF_IN zfautoObject const &ref)
: zfautoObject(ref)
{
    if(this->toObject() != zfnull && ZFCastZFObject(T_ZFObjectBase, this->toObject()) == zfnull)
    {
        _ZFP_zfautoObjectTError();
    }
}
template<typename T_ZFObjectBase>
template<typename T_ZFObject>
zfautoObjectT<T_ZFObjectBase>::zfautoObjectT(ZF_IN T_ZFObject *obj)
: zfautoObject(obj)
{
    if(this->toObject() != zfnull && ZFCastZFObject(T_ZFObjectBase, this->toObject()) == zfnull)
    {
        _ZFP_zfautoObjectTError();
    }
}
template<typename T_ZFObjectBase>
template<typename T_ZFObject>
zfautoObjectT<T_ZFObjectBase>::zfautoObjectT(ZF_IN T_ZFObject const &obj)
{
    if(this->toObject() != zfnull && ZFCastZFObject(T_ZFObjectBase, this->toObject()) == zfnull)
    {
        _ZFP_zfautoObjectTError();
    }
}

template<typename T_ZFObjectBase>
T_ZFObjectBase zfautoObjectT<T_ZFObjectBase>::operator -> (void) const
{
    return ZFCastZFObjectUnchecked(T_ZFObjectBase, this->toObject());
}
/** @endcond */

// ============================================================
template<typename T_To, int T_ToType>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_ObjCastHolder<0, T_To, zfautoObject, T_ToType, _ZFP_ObjCastTypeUnknown>
{
public:
    static inline T_To c(zfautoObject const &obj)
    {
        return ZFCastZFObject(T_To, obj.toObject());
    }
};
template<typename T_From, int T_FromType>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_ObjCastHolder<0, zfautoObject, T_From, _ZFP_ObjCastTypeUnknown, T_FromType>
{
public:
    static inline zfautoObject c(T_From obj)
    {
        return ZFCastZFObject(ZFObject *, obj);
    }
};

template<typename T_To, int T_ToType>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_ObjCastUncheckedHolder<0, T_To, zfautoObject, T_ToType, _ZFP_ObjCastTypeUnknown>
{
public:
    static inline T_To c(zfautoObject const &obj)
    {
        return ZFCastZFObjectUnchecked(T_To, obj.toObject());
    }
};
template<typename T_From, int T_FromType>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_ObjCastUncheckedHolder<0, zfautoObject, T_From, _ZFP_ObjCastTypeUnknown, T_FromType>
{
public:
    static inline zfautoObject c(T_From obj)
    {
        return ZFCastZFObjectUnchecked(ZFObject *, obj);
    }
};

// ============================================================
template<typename T_To, int T_ToType, typename T_From>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_ObjCastHolder<0, T_To, zfautoObjectT<T_From>, T_ToType, _ZFP_ObjCastTypeUnknown>
{
public:
    static inline T_To c(zfautoObjectT<T_From> const &obj)
    {
        return ZFCastZFObject(T_To, obj.toObject());
    }
};
template<typename T_To, typename T_From, int T_FromType>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_ObjCastHolder<0, zfautoObjectT<T_To>, T_From, _ZFP_ObjCastTypeUnknown, T_FromType>
{
public:
    static inline zfautoObjectT<T_To> c(T_From obj)
    {
        return ZFCastZFObject(ZFObject *, obj);
    }
};

template<typename T_To, int T_ToType, typename T_From>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_ObjCastUncheckedHolder<0, T_To, zfautoObjectT<T_From>, T_ToType, _ZFP_ObjCastTypeUnknown>
{
public:
    static inline T_To c(zfautoObjectT<T_From> const &obj)
    {
        return ZFCastZFObjectUnchecked(T_To, obj.toObject());
    }
};
template<typename T_To, typename T_From, int T_FromType>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_ObjCastUncheckedHolder<0, zfautoObjectT<T_To>, T_From, _ZFP_ObjCastTypeUnknown, T_FromType>
{
public:
    static inline zfautoObjectT<T_To> c(T_From obj)
    {
        return ZFCastZFObjectUnchecked(ZFObject *, obj);
    }
};

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_zfautoObject_h_

