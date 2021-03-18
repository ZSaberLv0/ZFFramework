#include "ZFAutoReleasePool.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassPOD _ZFP_ZFAutoReleasePoolData
{
public:
    ZFObject *obj;
};

zfclassNotPOD _ZFP_ZFAutoReleasePoolPrivate
{
public:
    zfindex maxSize;
    ZFCoreArrayPOD<_ZFP_ZFAutoReleasePoolData> array;
public:
    _ZFP_ZFAutoReleasePoolPrivate(void)
    : maxSize(zfindexMax())
    , array()
    {
    }
};

ZFOBJECT_REGISTER(ZFAutoReleasePool)
ZFOBJECT_SINGLETON_DEFINE_DETAIL(ZFAutoReleasePool, ZFAutoReleasePool, ZFAutoReleasePool, instance, ZFLevelZFFrameworkHigh)
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFAutoReleasePoolCleanup, ZFLevelZFFrameworkNormal)
{
    ZFAutoReleasePool::instance()->poolDrain();
}
ZF_GLOBAL_INITIALIZER_END(ZFAutoReleasePoolCleanup)

ZFOBJECT_ON_INIT_DEFINE_1(ZFAutoReleasePool,
                          ZFMP_IN(zfindex, maxSize))
{
    this->objectOnInit();
    d->maxSize = maxSize;
}
void ZFAutoReleasePool::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFAutoReleasePoolPrivate);
}
void ZFAutoReleasePool::objectOnDealloc(void)
{
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}
void ZFAutoReleasePool::objectOnDeallocPrepare(void)
{
    this->poolDrain();
    zfsuper::objectOnDeallocPrepare();
}
void ZFAutoReleasePool::objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
{
    zfsuper::objectInfoOnAppend(ret);
    ret += ", count: ";
    zfsFromIntT(ret, d->array.count());
}
void ZFAutoReleasePool::poolAdd(ZF_IN ZFObject *obj)
{
    zfCoreAssertWithMessage(obj != this, "add autorelease pool to itself isn't allowed");

    if(obj != zfnull)
    {
        if(d->array.count() >= d->maxSize)
        {
            zfCoreLogTrim("warning, auto release pool full, trying to release old object");
            _ZFP_ZFAutoReleasePoolData data = d->array.get(0);
            d->array.remove(0);
            zfRelease(data.obj);
        }
        _ZFP_ZFAutoReleasePoolData data;
        data.obj = obj;
        d->array.add(data);
    }
}
void ZFAutoReleasePool::poolDrain()
{
    if(!d->array.isEmpty())
    {
        zfindex count = d->array.count();
        _ZFP_ZFAutoReleasePoolData *tmp = (_ZFP_ZFAutoReleasePoolData *)zfmalloc(sizeof(_ZFP_ZFAutoReleasePoolData) * count);
        zfmemcpy(tmp, d->array.arrayBuf(), sizeof(_ZFP_ZFAutoReleasePoolData) * count);
        d->array.removeAll();

        for(zfindex i = 0; i < count; ++i)
        {
            zfRelease(tmp[i].obj);
        }
        zffree(tmp);
    }
}

ZF_NAMESPACE_GLOBAL_END

