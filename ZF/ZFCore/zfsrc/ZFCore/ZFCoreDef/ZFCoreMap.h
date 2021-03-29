/**
 * @file ZFCoreMap.h
 * @brief core map type for private use only
 */

#ifndef _ZFI_ZFCoreMap_h_
#define _ZFI_ZFCoreMap_h_

#include "ZFCorePointer.h"
#include "ZFCoreArray.h"
#include "ZFIterator.h"
#include "ZFToken.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief key value pair for #ZFCoreMap
 */
zfclassPOD ZF_ENV_EXPORT ZFCoreMapPair
{
public:
    const zfchar *key; /**< @brief key of map */
    ZFCorePointerBase *value; /**< @brief value of map, ensured not null as smart pointer */
};
/**
 * @brief an empty pair
 */
extern ZF_ENV_EXPORT const ZFCoreMapPair ZFCoreMapPairZero;

zfclassFwd _ZFP_ZFCoreMapPrivate;
/**
 * @brief core map type for private use only
 *
 * used to reduce dependency of stl\n
 * use string as key,
 * null key is considered same as empty string\n
 * use #ZFCorePointerBase as value,
 * which use retain logic and can hold many types
 */
zffinal zfclassLikePOD ZF_ENV_EXPORT ZFCoreMap
{
public:
    /**
     * @brief construct an empty map
     */
    ZFCoreMap(void);
    /**
     * @brief retain the ref, to copy, use #copyFrom
     */
    ZFCoreMap(ZF_IN const ZFCoreMap &ref);
    /**
     * @brief retain the ref, to copy, use #copyFrom
     */
    zffinal ZFCoreMap &operator = (ZF_IN const ZFCoreMap &ref);
    /**
     * @brief true if same ref
     */
    zffinal zfbool operator == (ZF_IN const ZFCoreMap &ref) const;
    /**
     * @brief true if not same ref
     */
    zffinal zfbool operator != (ZF_IN const ZFCoreMap &ref) const {return !this->operator == (ref);}
    ~ZFCoreMap(void);

public:
    /** @brief see #objectInfo */
    zffinal void objectInfoT(ZF_IN_OUT zfstring &ret) const;
    /** @brief return object info */
    zffinal zfstring objectInfo(void) const
    {
        zfstring ret;
        this->objectInfoT(ret);
        return ret;
    }

public:
    /** @brief see #objectInfoOfContent */
    zffinal void objectInfoOfContentT(ZF_IN_OUT zfstring &ret,
                                      ZF_IN_OPT zfindex maxCount = zfindexMax(),
                                      ZF_IN_OPT const ZFTokenForKeyValueContainer &token = ZFTokenForKeyValueContainerDefault()) const;
    /** @brief return contents info */
    zffinal zfstring objectInfoOfContent(ZF_IN_OPT zfindex maxCount = zfindexMax(),
                                         ZF_IN_OPT const ZFTokenForKeyValueContainer &token = ZFTokenForKeyValueContainerDefault()) const
    {
        zfstring ret;
        this->objectInfoOfContentT(ret, maxCount, token);
        return ret;
    }

public:
    /**
     * @brief copy all contents from ref, remove all before copy
     */
    zffinal void copyFrom(ZF_IN const ZFCoreMap &ref);

    /**
     * @brief get current retain count
     */
    zffinal zfindex objectRetainCount(void) const;

public:
    /**
     * @brief return count
     */
    zffinal zfindex count(void) const;

    /**
     * @brief true if empty
     */
    zffinal zfbool isEmpty(void) const;

    /**
     * @brief true if contains the key
     */
    zffinal zfbool isContain(ZF_IN const zfchar *key) const;

    /**
     * @brief add elements from ref
     */
    zffinal void addFrom(ZF_IN const ZFCoreMap &ref);

    /**
     * @brief change value or create if not exist,
     *   value would be retained by this method
     *
     * null key is considered same as empty string\n
     * assert fail if value is null (use an empty smart pointer to store null value)
     */
    zffinal void set(ZF_IN const zfchar *key,
                     ZF_IN const ZFCorePointerBase &value);
    /**
     * @brief get value or null if not exist
     */
    zffinal ZFCorePointerBase *get(ZF_IN const zfchar *key) const;
    /**
     * @brief get value or null if not exist
     */
    template<typename T_Element>
    T_Element get(ZF_IN const zfchar *key) const
    {
        ZFCorePointerBase *t = this->get(key);
        if(t != zfnull)
        {
            return t->pointerValueT<T_Element>();
        }
        return zfnull;
    }

    /** @brief see #allKey */
    zffinal void allKeyT(ZF_IN_OUT ZFCoreArray<const zfchar *> &ret) const;
    /**
     * @brief return a copy of all keys
     */
    inline ZFCoreArrayPOD<const zfchar *> allKey(void) const
    {
        ZFCoreArrayPOD<const zfchar *> ret;
        this->allKeyT(ret);
        return ret;
    }

    /** @brief see #allValue */
    zffinal void allValueT(ZF_IN_OUT ZFCoreArray<ZFCorePointerBase *> &ret) const;
    /**
     * @brief return a copy of all values
     */
    inline ZFCoreArrayPOD<ZFCorePointerBase *> allValue(void) const
    {
        ZFCoreArrayPOD<ZFCorePointerBase *> ret;
        this->allValueT(ret);
        return ret;
    }

    /** @brief see #allPair */
    zffinal void allPairT(ZF_IN_OUT ZFCoreArray<ZFCoreMapPair> &ret) const;
    /**
     * @brief return a copy of all keys and values
     */
    inline ZFCoreArrayPOD<ZFCoreMapPair> allPair(void) const
    {
        ZFCoreArrayPOD<ZFCoreMapPair> ret;
        this->allPairT(ret);
        return ret;
    }

    /** @brief see #allValue */
    template<typename T_Element>
    void allValueT(ZF_IN_OUT ZFCoreArray<T_Element> &ret) const
    {
        ret.capacity(ret.count() + this->count());
        for(zfiterator it = this->iterator(); this->iteratorValid(it); this->iteratorNext(it))
        {
            ret.add(this->iteratorValue<T_Element>(it));
        }
    }

    /**
     * @brief remove or do nothing if not exist
     */
    zffinal void remove(ZF_IN const zfchar *key);

    /**
     * @brief remove all content
     */
    zffinal void removeAll(void);

    // ============================================================
    // iterator access
public:
    /** @brief see #zfiterator */
    zffinal zfiterator iterator(void) const;

    /** @brief see #zfiterator */
    zffinal zfiterator iteratorFind(ZF_IN const zfchar *key) const;

    /** @brief see #zfiterator */
    zffinal zfbool iteratorValid(ZF_IN const zfiterator &it) const;
    /** @brief see #zfiterator */
    zffinal zfbool iteratorEqual(ZF_IN const zfiterator &it0,
                                 ZF_IN const zfiterator &it1) const;

    /** @brief see #zfiterator */
    zffinal void iteratorNext(ZF_IN_OUT zfiterator &it) const;
    /** @brief see #zfiterator */
    zffinal void iteratorPrev(ZF_IN_OUT zfiterator &it) const;

    /** @brief see #zfiterator */
    zffinal ZFCorePointerBase *iteratorValue(ZF_IN const zfiterator &it) const;
    /** @brief see #zfiterator */
    zffinal ZFCoreMapPair iteratorPair(ZF_IN const zfiterator &it) const;

    /** @brief see #zfiterator */
    zffinal void iteratorValue(ZF_IN_OUT zfiterator &it,
                               ZF_IN const ZFCorePointerBase &newValue);
    /** @brief see #zfiterator */
    zffinal void iteratorRemove(ZF_IN_OUT zfiterator &it);

    /** @brief see #zfiterator */
    zffinal void iteratorAdd(ZF_IN const zfchar *key,
                             ZF_IN const ZFCorePointerBase &value);

    /** @brief see #zfiterator */
    template<typename T_Element>
    T_Element iteratorValue(ZF_IN const zfiterator &it) const
    {
        ZFCorePointerBase *t = this->iteratorValue(it);
        if(t != zfnull)
        {
            return t->pointerValueT<T_Element>();
        }
        return zfnull;
    }

private:
    _ZFP_ZFCoreMapPrivate *d;
};

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreMap_h_

