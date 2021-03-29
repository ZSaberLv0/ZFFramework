/**
 * @file ZFHashSet.h
 * @brief container of ZFObject
 */

#ifndef _ZFI_ZFHashSet_h_
#define _ZFI_ZFHashSet_h_

#include "ZFContainer.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd ZFHashMap;
/**
 * @brief container of ZFObject, see #ZFContainer
 */
zfclass ZF_ENV_EXPORT ZFHashSet : zfextends ZFContainer
{
    ZFOBJECT_DECLARE(ZFHashSet, ZFContainer)

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

public:
    /**
     * @brief insert object to last, assert failure if obj = zfnull, see #add
     */
    ZFMETHOD_DECLARE_1(void, add,
                       ZFMP_IN(ZFObject *, obj))
    /**
     * @brief add objects from another container
     */
    ZFMETHOD_DECLARE_1(void, addFrom,
                       ZFMP_IN(ZFContainer *, another))

    /**
     * @brief remove object, do nothing if not exist
     */
    ZFMETHOD_DECLARE_1(void, removeElement,
                       ZFMP_IN(ZFObject *, obj))
    /**
     * @brief remove all object
     */
    ZFMETHOD_DECLARE_0(void, removeAll)

    // ============================================================
    // ZFIterable
public:
    /** @brief see #zfiterator */
    ZFMETHOD_DECLARE_0(zfiterator, iterator)

    /** @brief see #zfiterator */
    ZFMETHOD_DECLARE_1(zfiterator, iteratorFind,
                       ZFMP_IN(ZFObject *, key))

    /** @brief see #zfiterator */
    ZFMETHOD_DECLARE_1(zfbool, iteratorValid,
                       ZFMP_IN(const zfiterator &, it))
    /** @brief see #zfiterator */
    ZFMETHOD_DECLARE_2(zfbool, iteratorEqual,
                       ZFMP_IN(const zfiterator &, it0),
                       ZFMP_IN(const zfiterator &, it1))

    /** @brief see #zfiterator */
    ZFMETHOD_DECLARE_1(void, iteratorNext,
                       ZFMP_IN_OUT(zfiterator &, it))

    /** @brief see #zfiterator */
    ZFMETHOD_DECLARE_1(void, iteratorPrev,
                       ZFMP_IN_OUT(zfiterator &, it))

    /** @brief see #zfiterator */
    ZFMETHOD_DECLARE_1(ZFObject *, iteratorValue,
                       ZFMP_IN(const zfiterator &, it))

public:
    /** @brief see #zfiterator */
    ZFMETHOD_DECLARE_2(void, iteratorValue,
                       ZFMP_IN_OUT(zfiterator &, it),
                       ZFMP_IN(ZFObject *, value))
    /** @brief see #zfiterator */
    ZFMETHOD_DECLARE_1(void, iteratorRemove,
                       ZFMP_IN_OUT(zfiterator &, it))

    /** @brief see #zfiterator */
    ZFMETHOD_DECLARE_1(void, iteratorAdd,
                       ZFMP_IN(ZFObject *, value))
    /** @brief see #zfiterator */
    ZFMETHOD_DECLARE_2(void, iteratorAdd,
                       ZFMP_IN(ZFObject *, value),
                       ZFMP_IN_OUT(zfiterator &, it))

protected:
    /** @brief see #ZFObject::objectOnInit */
    ZFOBJECT_ON_INIT_DECLARE_1(ZFMP_IN(ZFContainer *, another))
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);

private:
    ZFHashMap *d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFHashSet_h_

