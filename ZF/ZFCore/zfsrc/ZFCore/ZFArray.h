/**
 * @file ZFArray.h
 * @brief container of ZFObject
 */

#ifndef _ZFI_ZFArray_h_
#define _ZFI_ZFArray_h_

#include "ZFContainer.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd _ZFP_ZFArrayPrivate;
/**
 * @brief container of ZFObject, see #ZFContainer
 */
zfclass ZF_ENV_EXPORT ZFArray : zfextends ZFContainer
{
    ZFOBJECT_DECLARE(ZFArray, ZFContainer)

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
     *   compared by #ZFObject::objectCompare by default
     */
    ZFMETHOD_DECLARE_2(zfbool, isContain,
                       ZFMP_IN(ZFObject *, obj),
                       ZFMP_IN_OPT(ZFComparer<ZFObject *>::Comparer, comparer, ZFComparerCheckEqual))

    /**
     * @brief return object at index, assert failure if out of range
     */
    ZFMETHOD_DECLARE_1(ZFObject *, get,
                       ZFMP_IN(zfindex, index))

    /**
     * @brief return first object or zfnull if empty
     */
    ZFMETHOD_DECLARE_0(ZFObject *, getFirst)

    /**
     * @brief return last object or zfnull if empty
     */
    ZFMETHOD_DECLARE_0(ZFObject *, getLast)

    /**
     * @brief find element, compared by #ZFObject::objectCompare by default
     */
    ZFMETHOD_DECLARE_2(zfindex, find,
                       ZFMP_IN(ZFObject *, obj),
                       ZFMP_IN_OPT(ZFComparer<ZFObject *>::Comparer, comparer, ZFComparerCheckEqual))
    /**
     * @brief find element, compared by #ZFObject::objectCompare by default
     */
    ZFMETHOD_DECLARE_2(zfindex, findReversely,
                       ZFMP_IN(ZFObject *, obj),
                       ZFMP_IN_OPT(ZFComparer<ZFObject *>::Comparer, comparer, ZFComparerCheckEqual))

public:
    /**
     * @brief util getter to get and cast to desired type
     */
    template<typename T_ZFObject>
    T_ZFObject get(ZF_IN zfindex index)
    {
        return ZFCastZFObjectUnchecked(T_ZFObject, this->get(index));
    }
    /**
     * @brief util getter to get and cast to desired type
     */
    template<typename T_ZFObject>
    T_ZFObject getFirst(void)
    {
        return ZFCastZFObjectUnchecked(T_ZFObject, this->getFirst());
    }
    /**
     * @brief util getter to get and cast to desired type
     */
    template<typename T_ZFObject>
    T_ZFObject getLast(void)
    {
        return ZFCastZFObjectUnchecked(T_ZFObject, this->getLast());
    }

public:
    /**
     * @brief insert object at index, assert failure if obj = zfnull
     *   or index out of range [0, count]
     * @note null object is not allowed
     */
    ZFMETHOD_DECLARE_2(void, add,
                       ZFMP_IN(zfindex, indexAddTo),
                       ZFMP_IN(ZFObject *, obj))
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
     * @brief replace object at index, assert fail if index out of range
     */
    ZFMETHOD_DECLARE_2(void, set,
                       ZFMP_IN(zfindex, index),
                       ZFMP_IN(ZFObject *, obj))

    /**
     * @brief remove first matched object, return whether the element removed
     */
    ZFMETHOD_DECLARE_1(zfbool, removeElement,
                       ZFMP_IN(ZFObject *, obj))
    /**
     * @brief remove first matched object, return whether the element removed
     */
    ZFMETHOD_DECLARE_2(zfbool, removeElement,
                       ZFMP_IN(ZFObject *, obj),
                       ZFMP_IN(ZFComparer<ZFObject *>::Comparer, comparer))
    /**
     * @brief remove last matched object, return whether the element removed
     */
    ZFMETHOD_DECLARE_1(zfbool, removeElementRevsersely,
                       ZFMP_IN(ZFObject *, obj))
    /**
     * @brief remove last matched object, return whether the element removed
     */
    ZFMETHOD_DECLARE_2(zfbool, removeElementRevsersely,
                       ZFMP_IN(ZFObject *, obj),
                       ZFMP_IN(ZFComparer<ZFObject *>::Comparer, comparer))
    /**
     * @brief remove all matched object, return number of element removed
     */
    ZFMETHOD_DECLARE_1(zfindex, removeElementAll,
                       ZFMP_IN(ZFObject *, obj))
    /**
     * @brief remove all matched object, return number of element removed
     */
    ZFMETHOD_DECLARE_2(zfindex, removeElementAll,
                       ZFMP_IN(ZFObject *, obj),
                       ZFMP_IN(ZFComparer<ZFObject *>::Comparer, comparer))

    /**
     * @brief remove object at index, assert failure if out of range
     */
    ZFMETHOD_DECLARE_2(void, remove,
                       ZFMP_IN(zfindex, index),
                       ZFMP_IN_OPT(zfindex, count, 1))
    /**
     * @brief remove first object or do nothing if out of range
     */
    ZFMETHOD_DECLARE_0(void, removeFirst)
    /**
     * @brief remove last object or do nothing if out of range
     */
    ZFMETHOD_DECLARE_0(void, removeLast)
    /**
     * @brief remove all object
     */
    ZFMETHOD_DECLARE_0(void, removeAll)

    /**
     * @brief move element
     */
    ZFMETHOD_DECLARE_2(void, move,
                       ZFMP_IN(zfindex, fromIndex),
                       ZFMP_IN(zfindex, toIndexOrIndexMax))

    /**
     * @brief sort content in range [start, start + count), all of content must be comparable
     */
    ZFMETHOD_DECLARE_4(void, sort,
                       ZFMP_IN_OPT(zfbool, ascending, zftrue),
                       ZFMP_IN_OPT(zfindex, start, 0),
                       ZFMP_IN_OPT(zfindex, count, zfindexMax()),
                       ZFMP_IN_OPT(ZFComparer<ZFObject *>::Comparer, comparer, ZFComparerCheckEqual))

    // ============================================================
    // extra iterable
public:
    /** @brief see #zfiterator */
    ZFMETHOD_DECLARE_1(zfiterator, iteratorForIndex,
                       ZFMP_IN(zfindex, index))

    // ============================================================
    // ZFIterable
public:
    /** @brief see #zfiterator */
    ZFMETHOD_DECLARE_0(zfiterator, iterator)

    /** @brief see #zfiterator */
    ZFMETHOD_DECLARE_1(zfiterator, iteratorFind,
                       ZFMP_IN(ZFObject *, element))

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
    _ZFP_ZFArrayPrivate *d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFArray_h_

