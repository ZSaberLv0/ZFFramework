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

protected:
    zfoverride
    virtual void copyableOnCopyFrom(ZF_IN ZFObject *anotherObj)
    {
        zfsuperI(ZFCopyable)::copyableOnCopyFrom(anotherObj);
        this->removeAll();
        this->addFrom(ZFCastZFObjectUnchecked(zfself *, anotherObj));
    }

protected:
    /**
     * @brief init from a existing container
     */
    ZFOBJECT_ON_INIT_DECLARE_1(ZFMP_IN(ZFKeyValueContainer *, another))

    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);

public:
    zfoverride
    virtual ZFCompareResult objectCompare(ZF_IN ZFObject *anotherObj);

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
        for(zfiterator it = this->iterator(); this->iteratorIsValid(it); )
        {
            ret.add(this->iteratorNextKey(it)->to<T_ZFObject *>());
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
        for(zfiterator it = this->iterator(); this->iteratorIsValid(it); )
        {
            ret.add(this->iteratorNextValue(it)->to<T_ZFObject *>());
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

protected:
    /**
     * @brief add data from another container
     */
    virtual void addFrom(ZF_IN ZFKeyValueContainer *another);

    /**
     * @brief set a key value pair
     *
     * create new one if not exist,
     * replace old one if already exist\n
     * null key is not allowed,
     * use null value to remove the pair
     */
    virtual void set(ZF_IN ZFObject *pKey,
                     ZF_IN ZFObject *pValue);

    /**
     * @brief remove value associated with pKey
     */
    virtual void remove(ZF_IN ZFObject *pKey);
    /**
     * @brief remove and return removed value or null if not exist
     *
     * use this method for performance, instead of "get then remove",
     * since the latter one have two search step
     */
    virtual zfautoObject removeAndGet(ZF_IN ZFObject *pKey);
    /**
     * @brief remove and get pair
     */
    virtual ZFKeyValuePairHolder removeAndGetPair(ZF_IN ZFObject *pKey);
    /**
     * @brief remove all content
     */
    virtual void removeAll(void);

protected:
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
                       ZFMP_IN(ZFObject *, value))

    /** @brief see #zfiterator */
    ZFMETHOD_DECLARE_1(zfbool, iteratorIsValid,
                       ZFMP_IN(const zfiterator &, it))
    /** @brief see #zfiterator */
    ZFMETHOD_DECLARE_2(zfbool, iteratorIsEqual,
                       ZFMP_IN(const zfiterator &, it0),
                       ZFMP_IN(const zfiterator &, it1))

protected:
    /** @brief see #zfiterator */
    virtual void iteratorValue(ZF_IN_OUT zfiterator &it,
                               ZF_IN ZFObject *value);
    /** @brief see #zfiterator */
    virtual void iteratorRemove(ZF_IN_OUT zfiterator &it);

    /** @brief see #zfiterator */
    virtual void iteratorAdd(ZF_IN ZFObject *value)
    {
        zfCoreCriticalNotSupported();
    }
    /** @brief see #zfiterator */
    virtual void iteratorAdd(ZF_IN ZFObject *value,
                             ZF_IN_OUT zfiterator &it)
    {
        zfCoreCriticalNotSupported();
    }

    // ============================================================
    // ZFIterableKeyValue
public:
    /** @brief see #zfiterator */
    ZFMETHOD_DECLARE_1(zfiterator, iteratorForKey,
                       ZFMP_IN(ZFObject *, key))

    /** @brief see #zfiterator */
    ZFMETHOD_DECLARE_1(ZFObject *, iteratorKey,
                       ZFMP_IN(const zfiterator &, it))

    /** @brief see #zfiterator */
    ZFMETHOD_DECLARE_1(ZFObject *, iteratorValue,
                       ZFMP_IN(const zfiterator &, it))

    /** @brief see #zfiterator */
    ZFMETHOD_DECLARE_1(ZFKeyValuePair, iteratorPair,
                       ZFMP_IN(const zfiterator &, it))

    /** @brief see #zfiterator */
    ZFMETHOD_DECLARE_1(ZFObject *, iteratorNextKey,
                       ZFMP_IN_OUT(zfiterator &, it))

    /** @brief see #zfiterator */
    ZFMETHOD_DECLARE_1(ZFObject *, iteratorNextValue,
                       ZFMP_IN_OUT(zfiterator &, it))

    /** @brief see #zfiterator */
    ZFMETHOD_DECLARE_1(ZFKeyValuePair, iteratorNextPair,
                       ZFMP_IN_OUT(zfiterator &, it))

    /** @brief see #zfiterator */
    ZFMETHOD_DECLARE_1(ZFObject *, iteratorPrevKey,
                       ZFMP_IN_OUT(zfiterator &, it))

    /** @brief see #zfiterator */
    ZFMETHOD_DECLARE_1(ZFObject *, iteratorPrevValue,
                       ZFMP_IN_OUT(zfiterator &, it))

    /** @brief see #zfiterator */
    ZFMETHOD_DECLARE_1(ZFKeyValuePair, iteratorPrevPair,
                       ZFMP_IN_OUT(zfiterator &, it))

protected:
    /** @brief see #zfiterator */
    virtual void iteratorAddKeyValue(ZF_IN ZFObject *key,
                                     ZF_IN ZFObject *value);
    /** @brief see #zfiterator */
    virtual void iteratorAddKeyValue(ZF_IN ZFObject *key,
                                     ZF_IN ZFObject *value,
                                     ZF_IN_OUT zfiterator &it);

private:
    _ZFP_ZFMapPrivate *d;
};

// ============================================================
/**
 * @brief editable container
 */
zfclass ZF_ENV_EXPORT ZFMapEditable : zfextends ZFMap, zfimplements ZFIterableEditable, zfimplements ZFIterableKeyValueEditable
{
    ZFOBJECT_DECLARE(ZFMapEditable, ZFMap)
    ZFIMPLEMENTS_DECLARE(ZFIterableEditable, ZFIterableKeyValueEditable)

public:
    ZFMETHOD_INLINE_1(void, addFrom,
                      ZFMP_IN(ZFKeyValueContainer *, another))
    {
        zfsuper::addFrom(another);
    }

    ZFMETHOD_INLINE_2(void, set,
                      ZFMP_IN(ZFObject *, pKey),
                      ZFMP_IN(ZFObject *, pValue))
    {
        zfsuper::set(pKey, pValue);
    }

    ZFMETHOD_INLINE_1(void, remove,
                      ZFMP_IN(ZFObject *, pKey))
    {
        zfsuper::remove(pKey);
    }
    ZFMETHOD_INLINE_1(zfautoObject, removeAndGet,
                      ZFMP_IN(ZFObject *, pKey))
    {
        return zfsuper::removeAndGet(pKey);
    }
    ZFMETHOD_INLINE_1(ZFKeyValuePairHolder, removeAndGetPair,
                      ZFMP_IN(ZFObject *, pKey))
    {
        return zfsuper::removeAndGetPair(pKey);
    }
    ZFMETHOD_INLINE_0(void, removeAll)
    {
        zfsuper::removeAll();
    }

public:
    /**
     * @brief util method to get and cast to desired type
     */
    template<typename T_ZFObject>
    T_ZFObject removeAndGet(ZF_IN ZFObject *pKey)
    {
        return zfsuper::removeAndGet<T_ZFObject>(pKey);
    }

    // ============================================================
    // ZFIterable
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

    // ============================================================
    // ZFIterableKeyValue
public:
    /** @brief see #zfiterator */
    ZFMETHOD_INLINE_2(void, iteratorAddKeyValue,
                      ZFMP_IN(ZFObject *, key),
                      ZFMP_IN(ZFObject *, value))
    {
        zfsuper::iteratorAddKeyValue(key, value);
    }
    /** @brief see #zfiterator */
    ZFMETHOD_INLINE_3(void, iteratorAddKeyValue,
                      ZFMP_IN(ZFObject *, key),
                      ZFMP_IN(ZFObject *, value),
                      ZFMP_IN_OUT(zfiterator &, it))
    {
        zfsuper::iteratorAddKeyValue(key, value, it);
    }
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFMap_h_

