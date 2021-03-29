/**
 * @file ZFIterable.h
 * @brief interface shows a ZFObject is iterable
 */

#ifndef _ZFI_ZFIterable_h_
#define _ZFI_ZFIterable_h_

#include "ZFKeyValuePair.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief iterable, see #zfiterator
 */
zfinterface ZF_ENV_EXPORT ZFIterable : zfextends ZFInterface
{
    ZFINTERFACE_DECLARE(ZFIterable, ZFInterface)

public:
    /**
     * @brief return total count of this iterable
     */
    virtual zfindex count(void) zfpurevirtual;
    /**
     * @brief return a iterator, see #zfiterator
     */
    virtual zfiterator iterator(void) zfpurevirtual;

    /**
     * @brief find element
     */
    virtual zfiterator iteratorFind(ZF_IN ZFObject *element) zfpurevirtual;

    /**
     * @brief true if the iterator is valid, see #zfiterator
     *
     * you may access value by #iteratorValue,
     * or move to next by #iteratorNext
     */
    virtual zfbool iteratorValid(ZF_IN const zfiterator &it) zfpurevirtual;

    /**
     * @brief true if two iterator is considered as equal, see #zfiterator
     *
     * two invalid iterator is considered as equal
     */
    virtual zfbool iteratorEqual(ZF_IN const zfiterator &it0,
                                 ZF_IN const zfiterator &it1) zfpurevirtual;

    /**
     * @brief move to next, see #zfiterator
     *
     * iterator must be valid
     */
    virtual void iteratorNext(ZF_IN_OUT zfiterator &it) zfpurevirtual;

    /**
     * @brief move to prev, see #zfiterator
     *
     * iterator must be valid and this iterable must be able to move prev
     */
    virtual void iteratorPrev(ZF_IN_OUT zfiterator &it) zfpurevirtual;

    /**
     * @brief get value by iterator, see #zfiterator
     *
     * iterator must be valid
     */
    virtual ZFObject *iteratorValue(ZF_IN const zfiterator &it) zfpurevirtual;

public:
    /**
     * @brief set value at iterator, see #zfiterator
     */
    virtual void iteratorValue(ZF_IN_OUT zfiterator &it,
                               ZF_IN ZFObject *value) zfpurevirtual;
    /**
     * @brief remove value at iterator, see #zfiterator
     */
    virtual void iteratorRemove(ZF_IN_OUT zfiterator &it) zfpurevirtual;
    /**
     * @brief remove all contents of this iterable
     */
    virtual void removeAll(void) zfpurevirtual;

    /**
     * @brief add value to tail
     */
    virtual void iteratorAdd(ZF_IN ZFObject *value) zfpurevirtual;
    /**
     * @brief add value before iterator,
     *   see #zfiterator
     */
    virtual void iteratorAdd(ZF_IN ZFObject *value,
                             ZF_IN_OUT zfiterator &it) zfpurevirtual;
};

// ============================================================
/**
 * @brief iterable that with key value pair, see #zfiterator
 */
zfinterface ZF_ENV_EXPORT ZFIterableKeyValue : zfextends ZFInterface
{
    ZFINTERFACE_DECLARE(ZFIterableKeyValue, ZFIterable)

public:
    /**
     * @brief get key value pair with iterator, see #zfiterator
     */
    virtual ZFKeyValuePair iteratorPair(ZF_IN const zfiterator &it) zfpurevirtual;

public:
    /**
     * @brief add key value to tail, see #zfiterator
     *
     * for most key value container,
     * this is same as set key with value
     */
    virtual void iteratorAdd(ZF_IN ZFObject *key,
                             ZF_IN ZFObject *value) zfpurevirtual;
    /**
     * @brief add key value before iterator, see #zfiterator
     *
     * for most key value container,
     * this is same as set key with value
     */
    virtual void iteratorAdd(ZF_IN ZFObject *key,
                             ZF_IN ZFObject *value,
                             ZF_IN_OUT zfiterator &it)
    {
        this->iteratorAdd(key, value);
    }
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFIterable_h_

