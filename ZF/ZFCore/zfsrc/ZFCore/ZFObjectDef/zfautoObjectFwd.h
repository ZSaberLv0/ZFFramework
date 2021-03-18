/**
 * @file zfautoObjectFwd.h
 * @brief smart pointer for ZFObject
 */

#ifndef _ZFI_zfautoObjectFwd_h_
#define _ZFI_zfautoObjectFwd_h_

#include "ZFAny.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// zfautoObject
zfclassNotPOD ZF_ENV_EXPORT _ZFP_zfautoObjectPrivate
{
public:
    zfuint refCount;
    ZFObject *obj;
public:
    _ZFP_zfautoObjectPrivate(void) : refCount(1), obj(zfnull) {}
    _ZFP_zfautoObjectPrivate(ZF_IN ZFObject *obj) : refCount(1), obj(obj) {}
};

/**
 * @brief a ZFObject holder which would release content object automatically when destroyed
 *
 * useful to hold a ZFObject as temp return value:
 * @code
 *   zfautoObject yourFunc(void)
 *   {
 *       // alloc an object for return
 *       zfblockedAlloc(ZFObject, obj);
 *
 *       // use zfautoObject to wrap the returned object
 *       return zfautoObject(obj);
 *   }
 *
 *   {
 *       zfautoObject value = yourFunc();
 *       ZFObject *obj = value;
 *   } // content obj would be released when zfautoObject destroyed
 * @endcode
 */
zffinal zfclassLikePOD ZF_ENV_EXPORT zfautoObject
{
    /** @cond ZFPrivateDoc */
public:
    zfautoObject(void) : d(zfnull) {}
    zfautoObject(ZF_IN zfautoObject const &ref);
    template<typename T_ZFObject>
    zfautoObject(ZF_IN T_ZFObject *obj);
    template<typename T_ZFObject>
    zfautoObject(ZF_IN T_ZFObject const &obj);
    zffinal ~zfautoObject(void);

public:
    zfautoObject &operator = (ZF_IN zfautoObject const &ref);
    template<typename T_ZFObject>
    zfautoObject &operator = (ZF_IN T_ZFObject *obj);
    template<typename T_ZFObject>
    zfautoObject &operator = (ZF_IN T_ZFObject const &obj);

public:
    template<typename T_ZFObject>
    zfbool operator == (ZF_IN T_ZFObject *obj) const
    {
        return (this->toObject() == (obj ? obj->toObject() : zfnull));
    }
    template<typename T_ZFObject>
    zfbool operator != (ZF_IN T_ZFObject *obj) const
    {
        return (this->toObject() != (obj ? obj->toObject() : zfnull));
    }
    template<typename T_ZFObject>
    zfbool operator == (ZF_IN T_ZFObject const &obj) const
    {
        return (this->toObject() == _ZFP_ZFAnyCast(T_ZFObject, obj));
    }
    template<typename T_ZFObject>
    zfbool operator != (ZF_IN T_ZFObject const &obj) const
    {
        return (this->toObject() != _ZFP_ZFAnyCast(T_ZFObject, obj));
    }

public:
    ZFObject *operator -> (void) const
    {
        return this->toObject();
    }
    operator bool (void) const
    {
        return (this->toObject() != zfnull);
    }
    template<typename T_ZFObject>
    operator T_ZFObject * (void) const
    {
        return ZFCastZFObject(T_ZFObject *, this->toObject());
    }
    /** @endcond */

public:
    /**
     * @brief no lock version of object assign, for low level impl only
     */
    void zflockfree_assign(ZF_IN ZFObject *obj);
    /**
     * @brief no lock version of object assign, for low level impl only
     */
    void zflockfree_assign(ZF_IN zfautoObject const &ref);
    /**
     * @brief get current retain count
     */
    zfindex objectRetainCount(void) const
    {
        return (zfindex)(d ? d->refCount : 0);
    }
    /**
     * @brief get the holded object
     */
    inline ZFObject *toObject(void) const
    {
        return (d ? d->obj : zfnull);
    }
    /**
     * @brief cast by #ZFCastZFObjectUnchecked
     */
    template<typename T_ZFObject>
    inline T_ZFObject to(void) const
    {
        return ZFCastZFObjectUnchecked(T_ZFObject, this->toObject());
    }

private:
    _ZFP_zfautoObjectPrivate *d;
};

// ============================================================
// zfautoObjectT
/**
 * @brief type restrict version of #zfautoObject
 */
template<typename T_ZFObjectBase>
zffinal zfclassLikePOD ZF_ENV_EXPORT zfautoObjectT : zfextendsLikePOD zfautoObject
{
    /** @cond ZFPrivateDoc */
public:
    zfautoObjectT(void) : zfautoObject() {}
    zfautoObjectT(ZF_IN zfautoObject const &ref);
    template<typename T_ZFObject>
    zfautoObjectT(ZF_IN T_ZFObject *obj);
    template<typename T_ZFObject>
    zfautoObjectT(ZF_IN T_ZFObject const &obj);

public:
    zfautoObjectT<T_ZFObjectBase> &operator = (ZF_IN zfautoObject const &ref);
    template<typename T_ZFObject>
    zfautoObjectT<T_ZFObjectBase> &operator = (ZF_IN T_ZFObject *obj);
    template<typename T_ZFObject>
    zfautoObjectT<T_ZFObjectBase> &operator = (ZF_IN T_ZFObject const &obj);

public:
    template<typename T_ZFObject>
    zfbool operator == (ZF_IN T_ZFObject *obj) const
    {
        return (this->toObject() == (obj ? obj->toObject() : zfnull));
    }
    template<typename T_ZFObject>
    zfbool operator != (ZF_IN T_ZFObject *obj) const
    {
        return (this->toObject() != (obj ? obj->toObject() : zfnull));
    }
    template<typename T_ZFObject>
    zfbool operator == (ZF_IN T_ZFObject const &obj) const
    {
        return (this->toObject() == _ZFP_ZFAnyCast(T_ZFObject, obj));
    }
    template<typename T_ZFObject>
    zfbool operator != (ZF_IN T_ZFObject const &obj) const
    {
        return (this->toObject() != _ZFP_ZFAnyCast(T_ZFObject, obj));
    }

public:
    T_ZFObjectBase operator -> (void) const;
    operator bool (void) const
    {
        return (this->toObject() != zfnull);
    }
    template<typename T_ZFObject>
    operator T_ZFObject * (void) const
    {
        return ZFCastZFObject(T_ZFObject *, this->toObject());
    }
    inline ZFObject *toObject(void) const
    { // required for _ZFP_ZFAnyCast to work
        return zfautoObject::toObject();
    }
    /** @endcond */

public:
    /**
     * @brief cast by #ZFCastZFObjectUnchecked
     */
    template<typename T_ZFObject>
    inline T_ZFObject to(void) const
    {
        return ZFCastZFObjectUnchecked(T_ZFObject, this->toObject());
    }
};

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_zfautoObjectFwd_h_

