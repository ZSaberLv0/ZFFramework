/**
 * @file ZFCorePointer.h
 * @brief light weight smart pointer
 */
#ifndef _ZFI_ZFCorePointer_h_
#define _ZFI_ZFCorePointer_h_

#include "ZFCoreSPrintf.h"
#include "ZFCoreElementInfoGetter.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
template<typename T_Pointer>
zfclassNotPOD _ZFP_ZFCorePointerHelper;
template<typename T_Pointer>
zfclassNotPOD _ZFP_ZFCorePointerHelper<T_Pointer *>
{
public:
    static inline void *toNonConstRaw(ZF_IN T_Pointer *p)
    {
        return p;
    }
    template<typename T_ZFCorePointer>
    static inline T_Pointer *toPointer(ZF_IN T_ZFCorePointer p)
    {
        return ZFCastStatic(T_Pointer *, p->pointerValueAccessNonConst());
    }
};
template<typename T_Pointer>
zfclassNotPOD _ZFP_ZFCorePointerHelper<const T_Pointer *>
{
public:
    static inline void *toNonConstRaw(ZF_IN const T_Pointer *p)
    {
        return zfnull;
    }
    template<typename T_ZFCorePointer>
    static inline const T_Pointer *toPointer(ZF_IN T_ZFCorePointer p)
    {
        return ZFCastStatic(const T_Pointer *, p->pointerValueAccess());
    }
};

// ============================================================
/**
 * @brief dummy base class for ZFCorePointer
 *
 * designed for hiding type from ZFCorePointer
 */
zfclassLikePOD ZF_ENV_EXPORT ZFCorePointerBase
{
    ZFCLASS_DISALLOW_COPY_CONSTRUCTOR(ZFCorePointerBase)
public:
    /** @cond ZFPrivateDoc */
    ZFCorePointerBase(void) {}
    virtual ~ZFCorePointerBase(void) {}
    /** @endcond */

public:
    /** @brief see #objectInfo */
    virtual void objectInfoT(ZF_IN_OUT zfstring &ret) const
    {
        if(this->pointerValueAccess() == zfnull)
        {
            ret += ZFTOKEN_zfnull;
        }
        else
        {
            this->objectInfoOfContentT(ret);
        }
    }
    /** @brief return object info */
    virtual inline zfstring objectInfo(void) const
    {
        zfstring ret;
        this->objectInfoT(ret);
        return ret;
    }

public:
    /** @brief see #objectInfoOfContent */
    virtual void objectInfoOfContentT(ZF_IN_OUT zfstring &ret) const zfpurevirtual;
    /**
     * @brief get content info or #ZFTOKEN_ZFCoreInfoGetterNotAvailable if not available
     */
    virtual inline zfstring objectInfoOfContent(void) const
    {
        zfstring ret;
        this->objectInfoOfContentT(ret);
        return ret;
    }

public:
    /**
     * @brief compare by internal pointer value
     */
    virtual ZFCompareResult objectCompare(ZF_IN const ZFCorePointerBase &another) const
    {
        return ((this->pointerValueAccess() == another.pointerValueAccess())
            ? ZFCompareTheSame
            : ZFCompareUncomparable);
    }

public:
    /**
     * @brief create a new reference to the smart pointer
     *
     * increase object reference and return a new smart pointer,
     * old one should be deleted manually\n
     * after the newly created instance and the old one both been deleted,
     * the object would be deleted finally
     * @note should be (and recommended to be) paired with #refDelete instead of #zfdelete,
     *   if you override default #zfnew and #zfdelete
     */
    virtual ZFCorePointerBase *refNew(void) const zfpurevirtual;
    /**
     * @brief see #refNew
     */
    virtual void refDelete(void)
    {
        zfdelete(this);
    }
    /**
     * @brief get the internal pointer
     */
    virtual const void *pointerValueAccess(void) const zfpurevirtual;
    /**
     * @brief get the internal pointer
     */
    virtual void *pointerValueAccessNonConst(void) const zfpurevirtual;

    /**
     * @brief util method to get and cast to desired type
     */
    template<typename T_PointerDesired>
    inline T_PointerDesired pointerValueT(void) const
    {
        return _ZFP_ZFCorePointerHelper<T_PointerDesired>::toPointer(this);
    }

    /**
     * @brief get current retain count
     */
    virtual zfindex objectRetainCount(void) const zfpurevirtual;
};

// ============================================================
template<typename T_Pointer>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_ZFCorePointerPrivate
{
public:
    zfuint refCount;
    T_Pointer pointerValue;
public:
    _ZFP_ZFCorePointerPrivate(void)
    : refCount(1)
    , pointerValue(zfnull)
    {
    }
};
/**
 * @brief pointer container with reference count logic
 *
 * usage:
 * @code
 *   {
 *       // create
 *       YourClass *obj = zfnew(YourClass);
 *       ZFCorePointerForObject<YourClass *> container(obj);
 *
 *       // get
 *       container->pointerValue()->funcInYourClass();
 *       // or cast it
 *       ((YourClass *)container)->funcInYourClass();
 *       // or implicit
 *       container->funcInYourClass();
 *
 *       // or as r-value
 *       YourClass *p = container;
 *       // or set as l-value
 *       container = p;
 *
 *       // or copy it
 *       ZFCorePointerForObject<YourClass *> another0(container);
 *       another0 = container;
 *   } // obj would be automatically deleted after this block,
 *     // i.e. all smart pointer container were deleted
 * @endcode
 *
 * you may store any types in ZFCorePointer, if you are able to supply a custom delete callback,
 * to declare your own type, see #ZFCOREPOINTER_DECLARE
 */
template<typename T_Pointer, typename T_ZFCorePointerType>
zffinal zfclassLikePOD ZF_ENV_EXPORT ZFCorePointer : zfextendsLikePOD ZFCorePointerBase
{
public:
    /**
     * @brief set the pointer value
     */
    inline void pointerValue(ZF_IN T_Pointer const &value)
    {
        if(d->pointerValue != zfnull)
        {
            T_ZFCorePointerType::pointerOnDelete(d->pointerValue);
        }
        d->pointerValue = value;
        if(value)
        {
            T_ZFCorePointerType::pointerOnRetain(d->pointerValue);
        }
    }
    /**
     * @brief get the pointer value
     */
    inline T_Pointer const &pointerValue(void) const
    {
        return d->pointerValue;
    }

public:
    /** @cond ZFPrivateDoc */
    inline T_Pointer const & operator -> (void) const
    {
        return d->pointerValue;
    }
    inline zfbool operator == (ZF_IN T_Pointer const &p) const
    {
        return (d->pointerValue == p);
    }
    inline zfbool operator != (ZF_IN T_Pointer const &p) const
    {
        return (d->pointerValue != p);
    }
    inline zfbool operator == (ZF_IN const ZFCorePointer<T_Pointer, T_ZFCorePointerType> &ref)
    {
        return (d->pointerValue == ref.d->pointerValue);
    }
    inline zfbool operator != (ZF_IN const ZFCorePointer<T_Pointer, T_ZFCorePointerType> &ref)
    {
        return (d->pointerValue == ref.d->pointerValue);
    }
    /** @endcond */

public:
    /** @cond ZFPrivateDoc */
    ZFCorePointer(void)
    : d(zfnew(_ZFP_ZFCorePointerPrivate<T_Pointer>))
    {
    }
    ZFCorePointer(ZF_IN T_Pointer const &value)
    : d(zfnew(_ZFP_ZFCorePointerPrivate<T_Pointer>))
    {
        this->pointerValue(value);
    }
    ZFCorePointer(ZF_IN const ZFCorePointer<T_Pointer, T_ZFCorePointerType> &ref)
    : d(ref.d)
    {
        ++(d->refCount);
    }
    virtual ~ZFCorePointer(void)
    {
        --(d->refCount);
        if(d->refCount == 0)
        {
            if(d->pointerValue != zfnull)
            {
                T_ZFCorePointerType::pointerOnDelete(d->pointerValue);
            }
            zfdelete(d);
        }
    }
    ZFCorePointer<T_Pointer, T_ZFCorePointerType> &operator = (ZF_IN const ZFCorePointer<T_Pointer, T_ZFCorePointerType> &ref)
    {
        _ZFP_ZFCorePointerPrivate<T_Pointer> *dTmp = d;
        d = ref.d;
        ++(ref.d->refCount);
        --(dTmp->refCount);
        if(dTmp->refCount == 0)
        {
            if(dTmp->pointerValue != zfnull)
            {
                T_ZFCorePointerType::pointerOnDelete(dTmp->pointerValue);
            }
            zfdelete(dTmp);
        }
        return *this;
    }
    ZFCorePointer<T_Pointer, T_ZFCorePointerType> &operator = (ZF_IN T_Pointer const &value)
    {
        this->pointerValue(value);
        return *this;
    }
    operator T_Pointer const & (void) const
    {
        return d->pointerValue;
    }
    /** @endcond */

public:
    zfoverride
    virtual void objectInfoOfContentT(ZF_IN_OUT zfstring &ret) const
    {
        this->objectInfoOfContentT(ret, zfnull);
    }
    /** @brief see #objectInfoOfContent */
    virtual void objectInfoOfContentT(ZF_IN_OUT zfstring &ret, ZF_IN typename ZFCoreInfoGetter<T_Pointer>::InfoGetter elementInfoGetter) const
    {
        if(elementInfoGetter != zfnull)
        {
            elementInfoGetter(ret, d->pointerValue);
        }
        else
        {
            ZFCoreElementInfoGetter<T_Pointer>::elementInfoGetter(ret, d->pointerValue);
        }
    }

public:
    zfoverride
    virtual ZFCorePointerBase *refNew(void) const
    {
        return zfnew((ZFCorePointer<T_Pointer, T_ZFCorePointerType>), *this);
    }
    zfoverride
    virtual inline const void *pointerValueAccess(void) const
    {
        return d->pointerValue;
    }
    zfoverride
    virtual inline void *pointerValueAccessNonConst(void) const
    {
        return _ZFP_ZFCorePointerHelper<T_Pointer>::toNonConstRaw(d->pointerValue);
    }

    zfoverride
    virtual zfindex objectRetainCount(void) const
    {
        return d->refCount;
    }

    /** @cond ZFPrivateDoc */
    template<typename T_PointerDesired>
    inline T_PointerDesired pointerValueT(void) const
    {
        return _ZFP_ZFCorePointerHelper<T_PointerDesired>::toPointer(this);
    }
    /** @endcond */

private:
    _ZFP_ZFCorePointerPrivate<T_Pointer> *d;
};

// ============================================================
/**
 * @brief declare your custom #ZFCorePointer type
 *
 * usage
 * @code
 *   ZFCOREPOINTER_DECLARE(YourCorePointer, {
 *           logRetained(p); // define your own retain action here
 *       }, {
 *           delete p; // define your own delete action here
 *       })
 * @endcode
 * once declared, you can use as normal #ZFCorePointer
 * @code
 *   YourCorePointer sp(zfnew(YourType));
 * @endcode
 */
#define ZFCOREPOINTER_DECLARE(T_ZFCorePointer, pointerRetainAction, pointerDeleteAction) \
    template<typename T_Type> \
    zfclassNotPOD ZF_ENV_EXPORT _ZFP_CPT_##T_ZFCorePointer \
    { \
    public: \
        static inline void pointerOnRetain(T_Type const &p) \
        pointerRetainAction \
        static inline void pointerOnDelete(T_Type const &p) \
        pointerDeleteAction \
    }; \
    /** @brief see #ZFCorePointer */ \
    template<typename T_Type> \
    zfclassLikePOD ZF_ENV_EXPORT T_ZFCorePointer : zfextendsLikePOD ZFCorePointer<T_Type, _ZFP_CPT_##T_ZFCorePointer<T_Type> > \
    { \
    public: \
        /** @cond ZFPrivateDoc */ \
        T_ZFCorePointer(void) \
        : ZFCorePointer<T_Type, _ZFP_CPT_##T_ZFCorePointer<T_Type> >() \
        { \
        } \
        T_ZFCorePointer(T_Type const &value) \
        : ZFCorePointer<T_Type, _ZFP_CPT_##T_ZFCorePointer<T_Type> >(value) \
        { \
        } \
        T_ZFCorePointer(ZFCorePointer<T_Type, _ZFP_CPT_##T_ZFCorePointer<T_Type> > const &ref) \
        : ZFCorePointer<T_Type, _ZFP_CPT_##T_ZFCorePointer<T_Type> >(ref) \
        { \
        } \
        T_ZFCorePointer<T_Type> &operator = (T_Type const &value) \
        { \
            ZFCorePointer<T_Type, _ZFP_CPT_##T_ZFCorePointer<T_Type> >::operator = (value); \
            return *this; \
        } \
        T_ZFCorePointer<T_Type> &operator = (ZFCorePointer<T_Type, _ZFP_CPT_##T_ZFCorePointer<T_Type> > const &ref) \
        { \
            ZFCorePointer<T_Type, _ZFP_CPT_##T_ZFCorePointer<T_Type> >::operator = (ref); \
            return *this; \
        } \
        template<typename T_PointerDesired> \
        inline T_PointerDesired pointerValueT(void) const \
        { \
            return _ZFP_ZFCorePointerHelper<T_PointerDesired>::toPointer(this); \
        } \
        template<typename T_Ref> \
        inline T_Ref operator *(void) const \
        { \
            return *(this->pointerValue()); \
        } \
        /** @endcond */ \
    };

ZFCOREPOINTER_DECLARE(ZFCorePointerForPointerRef, {}, {})
ZFCOREPOINTER_DECLARE(ZFCorePointerForPOD, {}, {zffree(p);})
ZFCOREPOINTER_DECLARE(ZFCorePointerForObject, {}, {zfdelete(p);})

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFCorePointer_h_

