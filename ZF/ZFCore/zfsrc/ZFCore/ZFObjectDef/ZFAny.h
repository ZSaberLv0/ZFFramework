/**
 * @file ZFAny.h
 * @brief util class to cast ZFObject types freely
 */

#ifndef _ZFI_ZFAny_h_
#define _ZFI_ZFAny_h_

#include "ZFObjectCast.h"
ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd ZFObject;
zfclassFwd ZFInterface;

ZFM_CLASS_HAS_MEMBER_DECLARE(ZFAny, toObject, ZFObject *(T::*F)(void) const)
template<typename T_Type, int has_toObject>
zfclassNotPOD _ZFP_ZFAnyCastT
{
public:
    static ZFObject *c(ZF_IN T_Type obj)
    {
        return (ZFObject *)obj;
    }
};
template<typename T_Type>
zfclassNotPOD _ZFP_ZFAnyCastT<T_Type, 1>
{
public:
    static ZFObject *c(ZF_IN T_Type obj)
    {
        return obj.toObject();
    }
};
#define _ZFP_ZFAnyCast(T_Type, obj) \
    (_ZFP_ZFAnyCastT<T_Type, ZFM_CLASS_HAS_MEMBER(ZFAny, toObject, T_Type)>::c(obj))

// ============================================================
// ZFAny
/**
 * @brief util method to cast ZFObject types freely
 *
 * usage:
 * @code
 *   YourObjectType *obj = ...;
 *   ZFAny any = obj;
 *
 *   // cast to any other object type, return null if not able to cast
 *   AnotherObjectType *another = any;
 *
 *   // or use toObject
 *   ZFObject *orig = any.toObject();
 * @endcode
 */
zfclassLikePOD ZF_ENV_EXPORT ZFAny
{
    /** @cond ZFPrivateDoc */
public:
    ZFAny(void)
    : _obj(zfnull)
    {
    }
    template<typename T_ZFObject>
    ZFAny(ZF_IN T_ZFObject *obj)
    : _obj(obj ? obj->toObject() : zfnull)
    {
    }
    template<typename T_ZFObject>
    ZFAny(ZF_IN T_ZFObject const &obj)
    : _obj(_ZFP_ZFAnyCast(T_ZFObject, obj))
    {
    }

public:
    template<typename T_ZFObject>
    ZFAny &operator = (ZF_IN T_ZFObject *obj)
    {
        this->_obj = (obj ? obj->toObject() : zfnull);
        return *this;
    }
    template<typename T_ZFObject>
    ZFAny &operator = (ZF_IN T_ZFObject const &obj)
    {
        this->_obj = _ZFP_ZFAnyCast(T_ZFObject, obj);
        return *this;
    }

public:
    template<typename T_ZFObject>
    zfbool operator == (ZF_IN T_ZFObject *obj) const
    {
        return (this->_obj == obj);
    }
    template<typename T_ZFObject>
    zfbool operator != (ZF_IN T_ZFObject *obj) const
    {
        return (this->_obj != obj);
    }
    template<typename T_ZFObject>
    zfbool operator == (ZF_IN T_ZFObject const &obj) const
    {
        return (this->_obj == _ZFP_ZFAnyCast(T_ZFObject, obj));
    }
    template<typename T_ZFObject>
    zfbool operator != (ZF_IN T_ZFObject const &obj) const
    {
        return (this->_obj != _ZFP_ZFAnyCast(T_ZFObject, obj));
    }

public:
    ZFObject *operator -> (void) const
    {
        return this->toObject();
    }
    operator bool (void) const
    {
        return (this->_obj != zfnull);
    }
    template<typename T_ZFObject>
    inline operator T_ZFObject * (void) const
    {
        return ZFCastZFObject(T_ZFObject *, this->_obj);
    }
    /** @endcond */

public:
    /**
     * @brief get the holded object
     */
    inline ZFObject *toObject(void) const
    {
        return this->_obj;
    }
    /**
     * @brief cast by #ZFCastZFObjectUnchecked
     */
    template<typename T_ZFObject>
    inline T_ZFObject to(void) const
    {
        return ZFCastZFObjectUnchecked(T_ZFObject, this->_obj);
    }

private:
    ZFObject *_obj;
};

template<typename T_To, int T_ToType>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_ObjCastHolder<0, T_To, ZFAny, T_ToType, _ZFP_ObjCastTypeUnknown>
{
public:
    static inline T_To c(ZFAny const &obj)
    {
        return ZFCastZFObject(T_To, obj.toObject());
    }
};
template<typename T_From, int T_FromType>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_ObjCastHolder<0, ZFAny, T_From, _ZFP_ObjCastTypeUnknown, T_FromType>
{
public:
    static inline ZFAny c(T_From obj)
    {
        return ZFCastZFObject(ZFObject *, obj);
    }
};

template<typename T_To, int T_ToType>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_ObjCastUncheckedHolder<0, T_To, ZFAny, T_ToType, _ZFP_ObjCastTypeUnknown>
{
public:
    static inline T_To c(ZFAny const &obj)
    {
        return ZFCastZFObjectUnchecked(T_To, obj.toObject());
    }
};
template<typename T_From, int T_FromType>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_ObjCastUncheckedHolder<0, ZFAny, T_From, _ZFP_ObjCastTypeUnknown, T_FromType>
{
public:
    static inline ZFAny c(T_From obj)
    {
        return ZFCastZFObjectUnchecked(ZFObject *, obj);
    }
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFAny_h_

