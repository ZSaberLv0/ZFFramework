/**
 * @file ZFCache.h
 * @brief util object to hold #ZFObject as cache
 */

#ifndef _ZFI_ZFCache_h_
#define _ZFI_ZFCache_h_

#include "ZFUtilityDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassFwd _ZFP_ZFCachePrivate;
/**
 * @brief util object to hold #ZFObject as cache
 */
zfclass ZF_ENV_EXPORT ZFCache : zfextends ZFObject
{
    ZFOBJECT_DECLARE(ZFCache, ZFObject)

public:
    /** @brief callback for impl */
    typedef zfbool (*CacheOnAddImpl)(ZF_IN ZFObject *cache);

public:
    /** @brief callback for impl */
    ZFCoreArrayPOD<ZFCache::CacheOnAddImpl> cacheOnAddImpl;

public:
    /**
     * @brief max cache size, 10 by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfindex, cacheMaxSize, 10)
    ZFPROPERTY_ON_ATTACH_DECLARE(zfindex, cacheMaxSize)

    /**
     * @brief whether invoke #cacheTrim when receive #ZFGlobalEvent::EventAppOnMemoryLow, true by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfbool, cacheTrimWhenReceiveMemoryWarning, zftrue)
    ZFPROPERTY_ON_ATTACH_DECLARE(zfbool, cacheTrimWhenReceiveMemoryWarning)

    /**
     * @brief leave how many cache alive while #cacheTrim, 3 by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfindex, cacheTrimThreshold, 3)

public:
    /**
     * @brief add cache
     *
     * if cache exceeds #cacheMaxSize,
     * the cacheValue would be discarded
     */
    ZFMETHOD_DECLARE_1(void, cacheAdd,
                       ZFMP_IN(ZFObject *, cacheValue))
    /**
     * @brief access cache, or return null if not exist
     */
    ZFMETHOD_DECLARE_0(zfautoObject, cacheGet)

    /**
     * @brief remove all cache
     */
    ZFMETHOD_DECLARE_0(void, cacheRemoveAll)
    /**
     * @brief trim the cache to reduce memory
     *
     * by default, this method would call #cacheTrimBySize
     */
    ZFMETHOD_DECLARE_0(void, cacheTrim)
    /**
     * @brief util method to trim the cache
     *   so that the cached data won't exceeds specified size
     */
    ZFMETHOD_DECLARE_1(void, cacheTrimBySize,
                       ZFMP_IN(zfindex , size))

public:
    /**
     * @brief access all cache data, used for debug only
     */
    ZFMETHOD_DECLARE_0(ZFCoreArray<zfautoObject>, cacheGetAll)
    /**
     * @brief see #cacheGetAll
     */
    ZFMETHOD_DECLARE_1(void, cacheGetAllT,
                       ZFMP_IN_OUT(ZFCoreArray<zfautoObject> &, ret))

protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);
    zfoverride
    virtual void objectOnDeallocPrepare(void);

private:
    _ZFP_ZFCachePrivate *d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFCache_h_

