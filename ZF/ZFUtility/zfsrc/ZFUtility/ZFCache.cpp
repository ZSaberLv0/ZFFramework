#include "ZFCache.h"

#include "ZFCore/ZFSTLWrapper/zfstl_list.h"
#include "ZFCore/ZFSTLWrapper/zfstl_string.h"
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
    static ZFLISTENER_PROTOTYPE_EXPAND(cacheTrim)
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
            ZFObjectGlobalEventObserver().observerAdd(
                ZFGlobalEvent::EventAppOnMemoryLow(),
                this->cacheTrimListener);
        }
        else if(!autoTrim && this->attachedObject.size() == 0)
        {
            ZFObjectGlobalEventObserver().observerRemove(
                ZFGlobalEvent::EventAppOnMemoryLow(),
                this->cacheTrimListener);
        }
    }
ZF_GLOBAL_INITIALIZER_END(ZFCacheDataHolder)

// ============================================================
zfclassNotPOD _ZFP_ZFCachePrivate
{
public:
    zfstllist<zfautoObject> cacheList; // add to tail, access or exceeds from head
};

// ============================================================
ZFOBJECT_REGISTER(ZFCache)

ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFCache, zfindex, cacheMaxSize)
{
    this->cacheTrimBySize(this->cacheMaxSize());
}
ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFCache, zfbool, cacheTrimWhenReceiveMemoryWarning)
{
    zfsynchronize(this);
    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFCacheDataHolder)->cacheTrimListenerSetup(
        this, this->cacheTrimWhenReceiveMemoryWarning());
}

ZFMETHOD_DEFINE_1(ZFCache, void, cacheAdd,
                  ZFMP_IN(ZFObject *, cacheValue))
{
    zfsynchronize(this);
    if(cacheValue == zfnull || (zfindex)d->cacheList.size() >= this->cacheMaxSize())
    {
        return ;
    }
    for(zfindex i = this->cacheOnAddImpl.count() - 1; i != zfindexMax(); --i)
    {
        if(!this->cacheOnAddImpl[i](cacheValue))
        {
            return ;
        }
    }
    cacheValue->observerRemoveAll();
    cacheValue->objectTagRemoveAll();
    d->cacheList.push_back(cacheValue);
}
ZFMETHOD_DEFINE_0(ZFCache, zfautoObject, cacheGet)
{
    zfsynchronize(this);
    if(d->cacheList.empty())
    {
        return zfnull;
    }
    else
    {
        zfautoObject ret = *(d->cacheList.begin());
        d->cacheList.pop_front();
        return ret;
    }
}

ZFMETHOD_DEFINE_0(ZFCache, void, cacheRemoveAll)
{
    zfsynchronize(this);
    d->cacheList.clear();
}

ZFMETHOD_DEFINE_0(ZFCache, void, cacheTrim)
{
    this->cacheTrimBySize(this->cacheTrimThreshold());
}

ZFMETHOD_DEFINE_1(ZFCache, void, cacheTrimBySize,
                  ZFMP_IN(zfindex, size))
{
    zfsynchronize(this);
    if(size >= (zfindex)(d->cacheList.size()))
    {
        return ;
    }
    while((zfindex)d->cacheList.size() > size)
    {
        d->cacheList.pop_front();
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
    zfsynchronize(this);
    for(zfstllist<zfautoObject>::iterator it = d->cacheList.begin(); it != d->cacheList.end(); ++it)
    {
        ret.add(*it);
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

