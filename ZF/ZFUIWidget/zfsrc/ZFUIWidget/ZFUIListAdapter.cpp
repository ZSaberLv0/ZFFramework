#include "ZFUIListAdapter.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFOBJECT_REGISTER(ZFUIListAdapter)

ZFOBSERVER_EVENT_REGISTER(ZFUIListAdapter, ListCellOnUpdate)
ZFOBSERVER_EVENT_REGISTER(ZFUIListAdapter, ListCellCacheOnRecycle)

// ============================================================
ZFMETHOD_DEFINE_0(ZFUIListAdapter, ZFUIOrientationEnum, listOrientation)
{
    return this->_ZFP_ZFUIListAdapter_listOrientation;
}
ZFMETHOD_DEFINE_0(ZFUIListAdapter, const ZFUISize &, listContainerSize)
{
    return this->_ZFP_ZFUIListAdapter_listContainerSize;
}

ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIListAdapter, zffloat, cellSizeHint)
{
    this->listAdapterNotifyReload();
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIListAdapter, zfbool, cellSizeFill)
{
    this->listAdapterNotifyReload();
}

ZFMETHOD_DEFINE_1(ZFUIListAdapter, void, listAdapterNotifyReload,
                  ZFMP_IN_OPT(zfindex, atIndexOrMax, zfindexMax()))
{
    this->listAdapterOnReload(atIndexOrMax);
}

ZFMETHOD_DEFINE_2(ZFUIListAdapter, zffloat, cellSizeAtIndex,
                  ZFMP_IN(zfindex, index),
                  ZFMP_IN(ZFUIListCell *, cell))
{
    if(this->cellSizeFill())
    {
        switch(this->listOrientation())
        {
            case ZFUIOrientation::e_Left:
            case ZFUIOrientation::e_Right:
                return this->listContainerSize().width;
            case ZFUIOrientation::e_Top:
            case ZFUIOrientation::e_Bottom:
                return this->listContainerSize().height;
            default:
                zfCoreCriticalShouldNotGoHere();
                return -1;
        }
    }
    else
    {
        return this->cellSizeHint();
    }
}

// ============================================================
#define _ZFP_ZFUIListAdapter_cacheKey(cacheKey, key) \
    zfchar *cacheKey = zfsConnect("_ZFP_ZFUIListAdapter_cacheKey", key); \
    zfblockedFree(cacheKey)
ZFMETHOD_DEFINE_1(ZFUIListAdapter, zfautoObjectT<ZFUIListCell *>, cellCacheDefaultAccess,
                  ZFMP_IN(const zfchar *, key))
{
    _ZFP_ZFUIListAdapter_cacheKey(cacheKey, key);
    ZFArray *cacheList = this->toObject()->objectTag<ZFArray *>(cacheKey);
    if(cacheList != zfnull && !cacheList->isEmpty())
    {
        zfautoObjectT<ZFUIListCell *> ret = cacheList->getLast();
        cacheList->removeLast();
        return ret;
    }
    else
    {
        return zfnull;
    }
}
ZFMETHOD_DEFINE_2(ZFUIListAdapter, void, cellCacheDefaultRecycle,
                  ZFMP_IN(const zfchar *, key),
                  ZFMP_IN(ZFUIListCell *, cell))
{
    _ZFP_ZFUIListAdapter_cacheKey(cacheKey, key);
    ZFArray *cacheList = this->toObject()->objectTag<ZFArray *>(cacheKey);
    if(cacheList == zfnull)
    {
        cacheList = zfAlloc(ZFArray);
        this->toObject()->objectTag(cacheKey, cacheList);
        zfRelease(cacheList);
    }
    cacheList->add(cell);
}

// ============================================================
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFUIListAdapter, zfindex, cellCount)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFUIListAdapter, zfautoObjectT<ZFUIListCell *>, cellAtIndex, ZFMP_IN(zfindex, index))

ZF_NAMESPACE_GLOBAL_END

