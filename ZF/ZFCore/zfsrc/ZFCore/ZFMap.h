/**
 * @file ZFMap.h
 * @brief container of ZFObject
 */

#ifndef _ZFI_ZFMap_h_
#define _ZFI_ZFMap_h_

#include "ZFKeyValueContainer.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd _ZFP_ZFMapPrivate;
/**
 * @brief container of ZFObject, see #ZFKeyValueContainer
 */
zfclass ZF_ENV_EXPORT ZFMap: zfextends ZFKeyValueContainer
{
    ZFOBJECT_DECLARE(ZFMap, ZFKeyValueContainer)

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
     * @brief return true if contain pKey
     */
    ZFMETHOD_DECLARE_1(zfbool, isContain,
                       ZFMP_IN(ZFObject *, pKey))

    /**
     * @brief get element by key or null if no such key
     */
    ZFMETHOD_DECLARE_1(ZFObject *, get,
                       ZFMP_IN(ZFObject *, pKey))
    /**
     * @brief get element pair, return all null if not find,
     *   otherwise, original key and value's pair would be returned
     */
    ZFMETHOD_DECLARE_1(ZFKeyValuePair, getPair,
                       ZFMP_IN(ZFObject *, pKey))

    /**
     * @brief get a copy of all key
     */
    ZFMETHOD_DECLARE_1(void, allKeyT,
                       ZFMP_IN_OUT(ZFCoreArray<ZFObject *> &, ret))
    /** @brief see #allKey */
    ZFMETHOD_DECLARE_0(ZFCoreArrayPOD<ZFObject *>, allKey)

    /**
     * @brief get a copy of all value
     */
    ZFMETHOD_DECLARE_1(void, allValueT,
                       ZFMP_IN_OUT(ZFCoreArray<ZFObject *> &, ret))
    /** @brief see #allValue */
    ZFMETHOD_DECLARE_0(ZFCoreArrayPOD<ZFObject *>, allValue)

    /**
     * @brief get a copy of all key value pair
     */
    ZFMETHOD_DECLARE_1(void, allPairT,
                       ZFMP_IN_OUT(ZFCoreArray<ZFKeyValuePair> &, ret))
    /** @brief see #allPair */
    ZFMETHOD_DECLARE_0(ZFCoreArrayPOD<ZFKeyValuePair>, allPair)

public:
    /**
     * @brief util method to get and cast to desired type,
     *   no type safe check and type must be valid
     */
    template<typename T_ZFObject>
    T_ZFObject get(ZF_IN ZFObject *pKey)
    {
        return ZFCastZFObjectUnchecked(T_ZFObject, this->get(pKey));
    }

    /** @brief see #allKey */
    template<typename T_ZFObject>
    void allKeyT(ZF_IN_OUT ZFCoreArray<T_ZFObject> &ret)
    {
        ret.capacity(ret.count() + this->count());
        for(zfiterator it = this->iterator(); this->iteratorValid(it); this->iteratorNext(it))
        {
            ret.add(this->iteratorPair(it).key->to<T_ZFObject *>());
        }
    }
    /** @brief see #allKey */
    template<typename T_ZFObject>
    ZFCoreArrayPOD<T_ZFObject> allKey(void)
    {
        ZFCoreArrayPOD<T_ZFObject> ret;
        this->allKeyT(ret);
        return ret;
    }

    /** @brief see #allValue */
    template<typename T_ZFObject>
    void allValueT(ZF_IN_OUT ZFCoreArray<T_ZFObject> &ret)
    {
        ret.capacity(ret.count() + this->count());
        for(zfiterator it = this->iterator(); this->iteratorValid(it); this->iteratorNext(it))
        {
            ret.add(this->iteratorValue(it)->to<T_ZFObject *>());
        }
    }
    /** @brief see #allValue */
    template<typename T_ZFObject>
    ZFCoreArrayPOD<T_ZFObject> allValue(void)
    {
        ZFCoreArrayPOD<T_ZFObject> ret;
        this->allValueT(ret);
        return ret;
    }

public:
    /**
     * @brief add data from another container
     */
    ZFMETHOD_DECLARE_1(void, addFrom,
                       ZFMP_IN(ZFKeyValueContainer *, another))

    /**
     * @brief set a key value pair
     *
     * create new one if not exist,
     * replace old one if already exist\n
     * null key is not allowed,
     * use null value to remove the pair
     */
    ZFMETHOD_DECLARE_2(void, set,
                       ZFMP_IN(ZFObject *, pKey),
                       ZFMP_IN(ZFObject *, pValue))

    /**
     * @brief remove value associated with pKey
     */
    ZFMETHOD_DECLARE_1(void, remove,
                       ZFMP_IN(ZFObject *, pKey))
    /**
     * @brief remove and return removed value or null if not exist
     *
     * use this method for performance, instead of "get then remove",
     * since the latter one have two search step
     */
    ZFMETHOD_DECLARE_1(zfautoObject, removeAndGet,
                       ZFMP_IN(ZFObject *, pKey))
    /**
     * @brief remove and get pair
     */
    ZFMETHOD_DECLARE_1(ZFKeyValuePairHolder, removeAndGetPair,
                       ZFMP_IN(ZFObject *, pKey))
    /**
     * @brief remove all content
     */
    ZFMETHOD_DECLARE_0(void, removeAll)

public:
    /**
     * @brief util method to get and cast to desired type
     */
    template<typename T_ZFObject>
    T_ZFObject removeAndGet(ZF_IN ZFObject *pKey)
    {
        return ZFCastZFObjectUnchecked(T_ZFObject, this->removeAndGet(pKey));
    }

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

    // ============================================================
    // ZFIterableKeyValue
public:
    /** @brief see #zfiterator */
    ZFMETHOD_DECLARE_1(ZFKeyValuePair, iteratorPair,
                       ZFMP_IN(const zfiterator &, it))

    /** @brief see #zfiterator */
    ZFMETHOD_DECLARE_2(void, iteratorAdd,
                       ZFMP_IN(ZFObject *, key),
                       ZFMP_IN(ZFObject *, value))

protected:
    /**
     * @brief init from a existing container
     */
    ZFOBJECT_ON_INIT_DECLARE_1(ZFMP_IN(ZFKeyValueContainer *, another))
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);

private:
    _ZFP_ZFMapPrivate *d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFMap_h_

