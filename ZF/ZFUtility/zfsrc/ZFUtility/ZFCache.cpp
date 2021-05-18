#include "ZFCache.h"

#include "ZFCore/ZFSTLWrapper/zfstl_list.h"
#include "ZFCore/ZFSTLWrapper/zfstl_map.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFCacheDataHolder, ZFLevelZFFrameworkStatic)
{
    this->cacheTrimListener = ZFCallbackForFunc(zfself::cacheTrim);
}
public:
    ZFListener cacheTrimListener;
    zfstlmap<ZFCache *, zfbool> attachedObject;
public:
    static void cacheTrim(ZF_IN const ZFListenerData &listenerData, ZF_IN ZFObject *userData)
    {
        const zfstlmap<ZFCache *, zfbool> &attachedObject = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFCacheDataHolder)->attachedObject;
        for(zfstlmap<ZFCache *, zfbool>::const_iterator it = attachedObject.begin(); it != attachedObject.end(); ++it)
        {
            it->first->cacheTrim();
        }
    }
    void cacheTrimListenerSetup(ZF_IN ZFCache *objectCache, ZF_IN zfbool autoTrim)
    {
        zfCoreMutexLocker();

        if(autoTrim)
        {
            this->attachedObject[objectCache] = zftrue;
        }
        else
        {
            this->attachedObject.erase(objectCache);
        }

        if(autoTrim && this->attachedObject.size() == 1)
        {
            ZFGlobalObserver().observerAdd(
                ZFGlobalEvent::EventAppOnMemoryLow(),
                this->cacheTrimListener);
        }
        else if(!autoTrim && this->attachedObject.size() == 0)
        {
            ZFGlobalObserver().observerRemove(
                ZFGlobalEvent::EventAppOnMemoryLow(),
                this->cacheTrimListener);
        }
    }
ZF_GLOBAL_INITIALIZER_END(ZFCacheDataHolder)

// ============================================================
zfclassFwd _ZFP_ZFCacheData;
typedef zfstllist<_ZFP_ZFCacheData *> _ZFP_ZFCacheList;
typedef zfstlmap<const zfchar *, _ZFP_ZFCacheList, zfcharConst_zfstlComparer> _ZFP_ZFCacheMap;
zfclassNotPOD _ZFP_ZFCacheData
{
public:
    zfchar *cacheKey;
    zfautoObject cacheValue;
    _ZFP_ZFCacheList::iterator listIt;
    _ZFP_ZFCacheList::iterator mapIt;
public:
    ~_ZFP_ZFCacheData(void)
    {
        if(this->cacheKey)
        {
            zffree(this->cacheKey);
        }
    }
};
zfclassNotPOD _ZFP_ZFCachePrivate
{
public:
    /*
     * new cache added to tail
     */
    _ZFP_ZFCacheList cacheList;
    _ZFP_ZFCacheMap cacheMap;

public:
    _ZFP_ZFCacheData *popFirst(void)
    {
        if(this->cacheList.empty())
        {
            return zfnull;
        }
        else
        {
            _ZFP_ZFCacheData *cacheData = *(this->cacheList.begin());
            this->cacheList.pop_front();

            _ZFP_ZFCacheMap::iterator mapIt = this->cacheMap.find(cacheData->cacheKey);
            mapIt->second.erase(cacheData->mapIt);
            if(mapIt->second.empty())
            {
                this->cacheMap.erase(mapIt);
            }

            return cacheData;
        }
    }
};

// ============================================================
ZFOBJECT_REGISTER(ZFCache)

ZFOBSERVER_EVENT_REGISTER(ZFCache, CacheOnAdd)
ZFOBSERVER_EVENT_REGISTER(ZFCache, CacheOnRemove)

ZFPROPERTY_ON_ATTACH_DEFINE(ZFCache, zfindex, cacheMaxSize)
{
    this->cacheTrimBySize(this->cacheMaxSize());
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFCache, zfbool, cacheTrimWhenReceiveMemoryWarning)
{
    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFCacheDataHolder)->cacheTrimListenerSetup(
        this, this->cacheTrimWhenReceiveMemoryWarning());
}

ZFMETHOD_DEFINE_2(ZFCache, void, cacheAdd,
                  ZFMP_IN(const zfchar *, cacheKey),
                  ZFMP_IN(ZFObject *, cacheValue))
{
    if(cacheKey == zfnull || cacheValue == zfnull)
    {
        return ;
    }

    zfCoreMutexLock();
    _ZFP_ZFCacheData *cacheData = zfpoolNew(_ZFP_ZFCacheData);
    zfCoreMutexUnlock();

    cacheData->cacheKey = zfsCopy(cacheKey);
    cacheData->cacheValue = cacheValue;

    d->cacheList.push_back(cacheData);
    cacheData->listIt = d->cacheList.end();
    --(cacheData->listIt);

    _ZFP_ZFCacheList &cacheList = d->cacheMap[cacheData->cacheKey];
    cacheList.push_back(cacheData);
    cacheData->mapIt = cacheList.end();
    --(cacheData->mapIt);

    this->cacheOnAdd(cacheValue);

    this->cacheTrimBySize(this->cacheMaxSize());
}
ZFMETHOD_DEFINE_1(ZFCache, zfautoObject, cacheGet,
                  ZFMP_IN(const zfchar *, cacheKey))
{
    if(cacheKey == zfnull)
    {
        return zfnull;
    }

    _ZFP_ZFCacheMap::iterator mapIt = d->cacheMap.find(cacheKey);
    if(mapIt == d->cacheMap.end())
    {
        return zfnull;
    }

    _ZFP_ZFCacheData *cacheData = *(mapIt->second.begin());
    if(mapIt->second.size() != 1)
    {
        mapIt->second.pop_front();
        mapIt->second.push_back(cacheData);
        cacheData->mapIt = mapIt->second.end();
        --(cacheData->mapIt);
    }
    if(d->cacheList.size() != 1)
    {
        d->cacheList.erase(cacheData->listIt);
        d->cacheList.push_back(cacheData);
        cacheData->listIt = d->cacheList.end();
        --(cacheData->listIt);
    }

    return cacheData->cacheValue;
}

ZFMETHOD_DEFINE_1(ZFCache, void, cacheRemove,
                  ZFMP_IN(const zfchar *, cacheKey))
{
    _ZFP_ZFCacheMap::iterator mapIt = d->cacheMap.find(cacheKey);
    if(mapIt == d->cacheMap.end())
    {
        return;
    }

    _ZFP_ZFCacheList cacheList;
    cacheList.swap(mapIt->second);
    d->cacheMap.erase(mapIt);

    for(_ZFP_ZFCacheList::iterator listIt = cacheList.begin(); listIt != cacheList.end(); ++listIt)
    {
        d->cacheList.erase((*listIt)->listIt);
    }

    zfCoreMutexLocker();
    for(_ZFP_ZFCacheList::iterator listIt = cacheList.begin(); listIt != cacheList.end(); ++listIt)
    {
        this->cacheOnRemove((*listIt)->cacheValue);
        zfpoolDelete(*listIt);
    }
}

ZFMETHOD_DEFINE_0(ZFCache, void, cacheRemoveAll)
{
    if(d->cacheList.empty())
    {
        return;
    }
    _ZFP_ZFCacheList cacheList;
    cacheList.swap(d->cacheList);
    d->cacheMap.clear();

    zfCoreMutexLocker();
    for(_ZFP_ZFCacheList::iterator listIt = cacheList.begin(); listIt != cacheList.end(); ++listIt)
    {
        this->cacheOnRemove((*listIt)->cacheValue);
        zfpoolDelete(*listIt);
    }
}

ZFMETHOD_DEFINE_0(ZFCache, void, cacheTrim)
{
    this->cacheTrimBySize(this->cacheTrimThreshold());
}

ZFMETHOD_DEFINE_1(ZFCache, void, cacheTrimBySize,
                  ZFMP_IN(zfindex, size))
{
    zfCoreMutexLocker();
    while(d->cacheList.size() > (zfstlsize)size)
    {
        _ZFP_ZFCacheData *cacheData = d->popFirst();
        this->cacheOnRemove(cacheData->cacheValue);
        zfpoolDelete(cacheData);
    }
}

ZFMETHOD_DEFINE_0(ZFCache, ZFCoreArray<zfautoObject>, cacheGetAll)
{
    ZFCoreArray<zfautoObject> ret;
    this->cacheGetAllT(ret);
    return ret;
}
ZFMETHOD_DEFINE_1(ZFCache, void, cacheGetAllT,
                  ZFMP_IN_OUT(ZFCoreArray<zfautoObject> &, ret))
{
    for(_ZFP_ZFCacheList::iterator listIt = d->cacheList.begin(); listIt != d->cacheList.end(); ++listIt)
    {
        ret.add((*listIt)->cacheValue);
    }
}

void ZFCache::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFCachePrivate);
    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFCacheDataHolder)->cacheTrimListenerSetup(this, this->cacheTrimWhenReceiveMemoryWarning());
}
void ZFCache::objectOnDealloc(void)
{
    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFCacheDataHolder)->cacheTrimListenerSetup(this, zffalse);
    zfpoolDelete(d);
    zfsuper::objectOnDealloc();
}
void ZFCache::objectOnDeallocPrepare(void)
{
    this->cacheRemoveAll();
    zfsuper::objectOnDeallocPrepare();
}

ZF_NAMESPACE_GLOBAL_END

