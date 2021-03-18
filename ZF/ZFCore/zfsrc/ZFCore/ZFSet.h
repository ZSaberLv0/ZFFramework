/**
 * @file ZFSet.h
 * @brief container of ZFObject
 */

#ifndef _ZFI_ZFSet_h_
#define _ZFI_ZFSet_h_

#include "ZFContainer.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd ZFMapEditable;
/**
 * @brief container of ZFObject, see #ZFContainer
 */
zfclass ZF_ENV_EXPORT ZFSet : zfextends ZFContainer
{
    ZFOBJECT_DECLARE(ZFSet, ZFContainer)

protected:
    zfoverride
    virtual void copyableOnCopyFrom(ZF_IN ZFObject *anotherObj)
    {
        zfsuperI(ZFCopyable)::copyableOnCopyFrom(anotherObj);
        this->removeAll();
        this->addFrom(ZFCastZFObjectUnchecked(zfself *, anotherObj));
    }

protected:
    /** @brief see #ZFObject::objectOnInit */
    virtual void objectOnInit(ZF_IN ZFContainer *another);

    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);

public:
    /**
     * @brief return number of content
     */
    ZFMETHOD_DECLARE_0(zfindex, count)

    /**
     * @brief return true if empty or false if not empty
     */
    ZFMETHOD_DECLARE_0(zfbool, isEmpty)

    /**
     * @brief return true if contains the object,
     *   compared by #ZFObject::objectCompare
     */
    ZFMETHOD_DECLARE_1(zfbool, isContain,
                       ZFMP_IN(ZFObject *, obj))

protected:
    /**
     * @brief insert object to last, assert failure if obj = zfnull, see #add
     */
    virtual void add(ZF_IN ZFObject *obj);
    /**
     * @brief add objects from another container
     */
    virtual void addFrom(ZF_IN ZFContainer *another);

    /**
     * @brief remove object, do nothing if not exist
     */
    virtual void removeElement(ZF_IN ZFObject *obj);
    /**
     * @brief remove all object
     */
    virtual void removeAll(void);

    // ============================================================
    // ZFIterable
public:
    /** @brief see #zfiterator */
    ZFMETHOD_DECLARE_0(zfiterator, iterator)

    /** @brief see #zfiterator */
    ZFMETHOD_DECLARE_1(zfiterator, iteratorFind,
                       ZFMP_IN(ZFObject *, value))

    /** @brief see #zfiterator */
    ZFMETHOD_DECLARE_1(zfbool, iteratorIsValid,
                       ZFMP_IN(const zfiterator &, it))
    /** @brief see #zfiterator */
    ZFMETHOD_DECLARE_2(zfbool, iteratorIsEqual,
                       ZFMP_IN(const zfiterator &, it0),
                       ZFMP_IN(const zfiterator &, it1))

    /** @brief see #zfiterator */
    ZFMETHOD_DECLARE_1(ZFObject *, iteratorValue,
                       ZFMP_IN(const zfiterator &, it))

    /** @brief see #zfiterator */
    ZFMETHOD_DECLARE_1(ZFObject *, iteratorNextValue,
                       ZFMP_IN_OUT(zfiterator &, it))

    /** @brief see #zfiterator */
    ZFMETHOD_DECLARE_1(ZFObject *, iteratorPrevValue,
                       ZFMP_IN_OUT(zfiterator &, it))

protected:
    /** @brief see #zfiterator */
    virtual void iteratorValue(ZF_IN_OUT zfiterator &it,
                               ZF_IN ZFObject *value);
    /** @brief see #zfiterator */
    virtual void iteratorRemove(ZF_IN_OUT zfiterator &it);

    /** @brief see #zfiterator */
    virtual void iteratorAdd(ZF_IN ZFObject *value);
    /** @brief see #zfiterator */
    virtual void iteratorAdd(ZF_IN ZFObject *value,
                             ZF_IN_OUT zfiterator &it);

private:
    ZFMapEditable *d;
};

// ============================================================
/**
 * @brief editable container of ZFObject
 */
zfclass ZF_ENV_EXPORT ZFSetEditable : zfextends ZFSet, zfimplements ZFIterableEditable
{
    ZFOBJECT_DECLARE(ZFSetEditable, ZFSet)
    ZFIMPLEMENTS_DECLARE(ZFIterableEditable)

public:
    ZFMETHOD_INLINE_1(void, add,
                      ZFMP_IN(ZFObject *, obj))
    {
        zfsuper::add(obj);
    }
    ZFMETHOD_INLINE_1(void, addFrom,
                      ZFMP_IN(ZFContainer *, another))
    {
        zfsuper::addFrom(another);
    }

    ZFMETHOD_INLINE_1(void, removeElement,
                      ZFMP_IN(ZFObject *, obj))
    {
        zfsuper::removeElement(obj);
    }
    ZFMETHOD_INLINE_0(void, removeAll)
    {
        zfsuper::removeAll();
    }

public:
    /** @brief see #zfiterator */
    ZFMETHOD_INLINE_2(void, iteratorValue,
                      ZFMP_IN_OUT(zfiterator &, it),
                      ZFMP_IN(ZFObject *, value))
    {
        zfsuper::iteratorValue(it, value);
    }
    /** @brief see #zfiterator */
    ZFMETHOD_INLINE_1(void, iteratorRemove,
                      ZFMP_IN_OUT(zfiterator &, it))
    {
        zfsuper::iteratorRemove(it);
    }

    /** @brief see #zfiterator */
    ZFMETHOD_INLINE_1(void, iteratorAdd,
                      ZFMP_IN(ZFObject *, value))
    {
        zfsuper::iteratorAdd(value);
    }
    /** @brief see #zfiterator */
    ZFMETHOD_INLINE_2(void, iteratorAdd,
                      ZFMP_IN(ZFObject *, value),
                      ZFMP_IN_OUT(zfiterator &, it))
    {
        zfsuper::iteratorAdd(value, it);
    }
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFSet_h_

