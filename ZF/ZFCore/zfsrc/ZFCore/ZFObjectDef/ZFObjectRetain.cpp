#include "ZFObjectCore.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassPOD _ZFP_zfAllocWithCache_Data
{
public:
    zfbool *enableFlag;
    ZFObject **cache;
    zfindex *cacheCount;
};
static ZFCoreArrayPOD<_ZFP_zfAllocWithCache_Data> &_ZFP_zfAllocWithCache_DataList(void)
{
    static ZFCoreArrayPOD<_ZFP_zfAllocWithCache_Data> d;
    return d;
}

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(zfAllocWithCache, ZFLevelZFFrameworkNormal)
{
    ZFCoreArrayPOD<_ZFP_zfAllocWithCache_Data> &d = _ZFP_zfAllocWithCache_DataList();
    for(zfindex i = d.count() - 1; i != zfindexMax(); --i)
    {
        *(d[i].enableFlag) = zftrue;
    }
}
ZF_GLOBAL_INITIALIZER_DESTROY(zfAllocWithCache)
{
    ZFCoreArrayPOD<ZFObject *> toRelease;
    ZFCoreArrayPOD<_ZFP_zfAllocWithCache_Data> &d = _ZFP_zfAllocWithCache_DataList();
    for(zfindex i = d.count() - 1; i != zfindexMax(); --i)
    {
        *(d[i].enableFlag) = zffalse;
        toRelease.addFrom(d[i].cache, *(d[i].cacheCount));
        *(d[i].cacheCount) = 0;
    }
    for(zfindex i = toRelease.count() - 1; i != zfindexMax(); --i)
    {
        zflockfree_zfRelease(toRelease[i]);
    }
}
ZF_GLOBAL_INITIALIZER_END(zfAllocWithCache)

void zfAllocCacheRemoveAll(void)
{
    zfCoreMutexLocker();
    ZFCoreArrayPOD<ZFObject *> toRelease;
    ZFCoreArrayPOD<_ZFP_zfAllocWithCache_Data> &d = _ZFP_zfAllocWithCache_DataList();
    for(zfindex i = d.count() - 1; i != zfindexMax(); --i)
    {
        toRelease.addFrom(d[i].cache, *(d[i].cacheCount));
        *(d[i].cacheCount) = 0;
    }
    for(zfindex i = toRelease.count() - 1; i != zfindexMax(); --i)
    {
        zflockfree_zfRelease(toRelease[i]);
    }
}

void _ZFP_zfAllocWithCache_register(ZF_IN_OUT zfbool &enableFlag,
                                    ZF_IN_OUT ZFObject **cache,
                                    ZF_IN_OUT zfindex &cacheCount)
{
    ZFCoreArrayPOD<_ZFP_zfAllocWithCache_Data> &d = _ZFP_zfAllocWithCache_DataList();
    _ZFP_zfAllocWithCache_Data item;
    item.enableFlag = &enableFlag;
    item.cache = cache;
    item.cacheCount = &cacheCount;
    d.add(item);
    if(ZFFrameworkStateCheck((ZFLevel)(ZFLevelZFFrameworkNormal - 1)) == ZFFrameworkStateAvailable)
    {
        enableFlag = zftrue;
    }
}
void _ZFP_zfAllocWithCache_unregister(ZF_IN_OUT zfbool &enableFlag)
{
    ZFCoreArrayPOD<_ZFP_zfAllocWithCache_Data> &d = _ZFP_zfAllocWithCache_DataList();
    for(zfindex i = 0; i < d.count(); ++i)
    {
        if(&enableFlag == d[i].enableFlag)
        {
            ZFCoreArrayPOD<ZFObject *> toRelease;
            toRelease.addFrom(d[i].cache, *(d[i].cacheCount));

            *(d[i].cacheCount) = 0;
            enableFlag = zffalse;
            d.remove(i);

            for(zfindex i = toRelease.count() - 1; i != zfindexMax(); --i)
            {
                zflockfree_zfRelease(toRelease[i]);
            }
            break;
        }
    }
}

ZF_NAMESPACE_GLOBAL_END

