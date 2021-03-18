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
    ZFOBJECT_ON_INIT_DECLARE_1(ZFMP_IN(ZFContainer *, another))
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
     * @brief return object at index, assert failure if out of range
     */
    ZFMETHOD_DECLARE_1(ZFObject *, get, ZFMP_IN(zfindex, index))

    /**
     * @brief return first object or zfnull if empty
     */
    ZFMETHOD_DECLARE_0(ZFObject *, getFirst)

    /**
     * @brief return last object or zfnull if empty
     */
    ZFMETHOD_DECLARE_0(ZFObject *, getLast)

    /**
     * @brief return true if contains the object,
     *   compared by #ZFObject::objectCompare by default
     */
    ZFMETHOD_DECLARE_2(zfbool, isContain,
                       ZFMP_IN(ZFObject *, obj),
                       ZFMP_IN_OPT(ZFComparer<ZFObject *>::Comparer, comparer, ZFComparerCheckEqual))

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

protected:
    /**
     * @brief insert object at index, assert failure if obj = zfnull
     *   or index out of range [0, count]
     * @note null object is not allowed
     */
    virtual void add(ZF_IN zfindex indexAddTo,
                     ZF_IN ZFObject *obj);
    /**
     * @brief insert object to last, assert failure if obj = zfnull, see #add
     */
    virtual void add(ZF_IN ZFObject *obj);
    /**
     * @brief add objects from another container
     */
    virtual void addFrom(ZF_IN ZFContainer *another);

    /**
     * @brief replace object at index, assert fail if index out of range
     */
    virtual void set(ZF_IN zfindex index,
                     ZF_IN ZFObject *obj);

    /**
     * @brief remove first matched object, return whether the element removed
     */
    virtual zfbool removeElement(ZF_IN ZFObject *obj);
    /**
     * @brief remove first matched object, return whether the element removed
     */
    virtual zfbool removeElement(ZF_IN ZFObject *obj, ZF_IN ZFComparer<ZFObject *>::Comparer comparer);
    /**
     * @brief remove last matched object, return whether the element removed
     */
    virtual zfbool removeElementRevsersely(ZF_IN ZFObject *obj);
    /**
     * @brief remove last matched object, return whether the element removed
     */
    virtual zfbool removeElementRevsersely(ZF_IN ZFObject *obj, ZF_IN ZFComparer<ZFObject *>::Comparer comparer);
    /**
     * @brief remove all matched object, return number of element removed
     */
    virtual zfindex removeElementAll(ZF_IN ZFObject *obj);
    /**
     * @brief remove all matched object, return number of element removed
     */
    virtual zfindex removeElementAll(ZF_IN ZFObject *obj, ZF_IN ZFComparer<ZFObject *>::Comparer comparer);

    /**
     * @brief remove object at index, assert failure if out of range
     */
    virtual void remove(ZF_IN zfindex index,
                        ZF_IN_OPT zfindex count = 1);
    /**
     * @brief remove first object or do nothing if out of range
     */
    virtual void removeFirst(void);
    /**
     * @brief remove last object or do nothing if out of range
     */
    virtual void removeLast(void);
    /**
     * @brief remove all object
     */
    virtual void removeAll(void);

    /**
     * @brief move element
     */
    virtual void move(ZF_IN zfindex fromIndex, ZF_IN zfindex toIndexOrIndexMax);

    /**
     * @brief sort content in range [start, start + count), all of content must be comparable
     */
    virtual void sort(ZF_IN_OPT zfbool ascending = zftrue,
                      ZF_IN_OPT zfindex start = 0,
                      ZF_IN_OPT zfindex count = zfindexMax(),
                      ZF_IN_OPT ZFComparer<ZFObject *>::Comparer comparer = ZFComparerCheckEqual);

    // ============================================================
    // extra iterable
public:
    /** @brief see #zfiterator */
    ZFMETHOD_DECLARE_1(zfiterator, iteratorForIndex,
                       ZFMP_IN(zfindex, index))
    /** @brief see #zfiterator */
    ZFMETHOD_DECLARE_2(zfiterator, iteratorFind,
                       ZFMP_IN(ZFObject *, value),
                       ZFMP_IN(ZFComparer<ZFObject *>::Comparer, comparer))
    /** @brief see #zfiterator */
    ZFMETHOD_DECLARE_2(zfiterator, iteratorFindReversely,
                       ZFMP_IN(ZFObject *, value),
                       ZFMP_IN_OPT(ZFComparer<ZFObject *>::Comparer, comparer, ZFComparerCheckEqual))

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
    _ZFP_ZFArrayPrivate *d;
};

// ============================================================
/**
 * @brief editable container of ZFObject
 */
zfclass ZF_ENV_EXPORT ZFArrayEditable : zfextends ZFArray, zfimplements ZFIterableEditable
{
    ZFOBJECT_DECLARE(ZFArrayEditable, ZFArray)
    ZFIMPLEMENTS_DECLARE(ZFIterableEditable)

public:
    ZFMETHOD_INLINE_2(void, add,
                      ZFMP_IN(zfindex, indexAddTo),
                      ZFMP_IN(ZFObject *, obj))
    {
        zfsuper::add(indexAddTo, obj);
    }
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

    ZFMETHOD_INLINE_2(void, set,
                      ZFMP_IN(zfindex, index),
                      ZFMP_IN(ZFObject *, obj))
    {
        zfsuper::set(index, obj);
    }

    ZFMETHOD_INLINE_1(zfbool, removeElement,
                      ZFMP_IN(ZFObject *, obj))
    {
        return zfsuper::removeElement(obj);
    }
    ZFMETHOD_INLINE_2(zfbool, removeElement,
                      ZFMP_IN(ZFObject *, obj),
                      ZFMP_IN(ZFComparer<ZFObject *>::Comparer, comparer))
    {
        return zfsuper::removeElement(obj, comparer);
    }
    ZFMETHOD_INLINE_1(zfbool, removeElementRevsersely,
                      ZFMP_IN(ZFObject *, obj))
    {
        return zfsuper::removeElementRevsersely(obj);
    }
    ZFMETHOD_INLINE_2(zfbool, removeElementRevsersely,
                      ZFMP_IN(ZFObject *, obj),
                      ZFMP_IN(ZFComparer<ZFObject *>::Comparer, comparer))
    {
        return zfsuper::removeElementRevsersely(obj, comparer);
    }
    ZFMETHOD_INLINE_1(zfindex, removeElementAll,
                      ZFMP_IN(ZFObject *, obj))
    {
        return zfsuper::removeElementAll(obj);
    }
    ZFMETHOD_INLINE_2(zfindex, removeElementAll,
                      ZFMP_IN(ZFObject *, obj),
                      ZFMP_IN(ZFComparer<ZFObject *>::Comparer, comparer))
    {
        return zfsuper::removeElementAll(obj, comparer);
    }

    ZFMETHOD_INLINE_2(void, remove,
                      ZFMP_IN(zfindex , index),
                      ZFMP_IN_OPT(zfindex, count, 1))
    {
        zfsuper::remove(index, count);
    }
    ZFMETHOD_INLINE_0(void, removeFirst)
    {
        zfsuper::removeFirst();
    }
    ZFMETHOD_INLINE_0(void, removeLast)
    {
        zfsuper::removeLast();
    }
    ZFMETHOD_INLINE_0(void, removeAll)
    {
        zfsuper::removeAll();
    }

    ZFMETHOD_INLINE_2(void, move,
                      ZFMP_IN(zfindex, fromIndex),
                      ZFMP_IN(zfindex, toIndexOrIndexMax))
    {
        zfsuper::move(fromIndex, toIndexOrIndexMax);
    }

    ZFMETHOD_INLINE_4(void, sort,
                      ZFMP_IN_OPT(zfbool, ascending, zftrue),
                      ZFMP_IN_OPT(zfindex, start, 0),
                      ZFMP_IN_OPT(zfindex, count, zfindexMax()),
                      ZFMP_IN_OPT(ZFComparer<ZFObject *>::Comparer, comparer, ZFComparerCheckEqual))
    {
        zfsuper::sort(ascending, start, count, comparer);
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
#endif // #ifndef _ZFI_ZFArray_h_

