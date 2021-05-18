/**
 * @file ZFResCache.h
 * @brief utility to load serializable from resource with cache logic
 */

#ifndef _ZFI_ZFResCache_h_
#define _ZFI_ZFResCache_h_

#include "ZFCache.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief cache for serializable from resources
 *
 * note: res cache would be clean up and disabled during #ZFFrameworkCleanup
 * of level #ZFLevelZFFrameworkPostNormal to prevent dependency broken,
 * by setting #ZFCache::cacheMaxSize to zero
 */
zfclass ZF_ENV_EXPORT ZFResCache : zfextends ZFCache
{
    ZFOBJECT_DECLARE(ZFResCache, ZFCache)
    ZFOBJECT_SINGLETON_DECLARE(ZFResCache, instance)

    ZFPROPERTY_ON_INIT_DECLARE(zfindex, cacheMaxSize)
};

/**
 * @brief load resource by #ZFObjectIOLoad
 *
 * automatically cache the resource in #ZFResCache,
 * you should not modified the returned object if cache enabled\n
 * to remove cache, use #ZFCache::cacheRemove
 */
ZFMETHOD_FUNC_DECLARE_1(zfautoObject, zfRes,
                        ZFMP_IN(const zfchar *, resFilePath))

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFResCache_h_

