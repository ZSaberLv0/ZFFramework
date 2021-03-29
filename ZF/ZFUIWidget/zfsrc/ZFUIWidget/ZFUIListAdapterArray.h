/**
 * @file ZFUIListAdapterArray.h
 * @brief basic list adapter which hold all list cells directly (with no recycle logic)
 */

#ifndef _ZFI_ZFUIListAdapterArray_h_
#define _ZFI_ZFUIListAdapterArray_h_

#include "ZFUIListAdapter.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUIListAdapterArray_cell "ZFSerializableKeyword_ZFUIListAdapterArray_cell"

/**
 * @brief basic list adapter which hold all list cells directly (with no recycle logic)
 *
 * serializable data:
 * @code
 *   <ZFUIListAdapterArray>
 *       <SomeView category="cell" />
 *       ... // each cell
 *   </ZFUIListAdapterArray>
 * @endcode
 */
zfclass ZF_ENV_EXPORT ZFUIListAdapterArray : zfextends ZFStyleableObject, zfimplements ZFUIListAdapter
{
    ZFOBJECT_DECLARE(ZFUIListAdapterArray, ZFStyleableObject)
    ZFIMPLEMENTS_DECLARE(ZFUIListAdapter)

protected:
    zfoverride
    virtual void objectOnInit(void)
    {
        zfsuper::objectOnInit();
        d = zfAlloc(ZFArray);
    }
    zfoverride
    virtual void objectOnDealloc(void)
    {
        zfRelease(d);
        zfsuper::objectOnDealloc();
    }
    zfoverride
    virtual void objectOnDeallocPrepare(void)
    {
        this->cellRemoveAll();
        zfsuper::objectOnDeallocPrepare();
    }

public:
    /**
     * @brief directly access internal cell array,
     *   you may modify it directly, with caution
     */
    ZFMETHOD_INLINE_0(ZFArray *, cellArray)
    {
        return d;
    }
    /**
     * @brief add cell
     */
    ZFMETHOD_INLINE_1(void, cellAdd,
                      ZFMP_IN(ZFUIListCell *, cell))
    {
        d->add(cell);
    }
    /**
     * @brief add cell at index
     */
    ZFMETHOD_INLINE_2(void, cellAdd,
                      ZFMP_IN(zfindex, index),
                      ZFMP_IN(ZFUIListCell *, cell))
    {
        d->add(index, cell);
    }
    /**
     * @brief remove cell
     */
    ZFMETHOD_INLINE_2(void, cellRemove,
                      ZFMP_IN(zfindex, index),
                      ZFMP_IN_OPT(zfindex, count, 1))
    {
        d->remove(index, count);
    }
    /**
     * @brief remove cell
     */
    ZFMETHOD_INLINE_1(void, cellRemoveElement,
                      ZFMP_IN(ZFUIListCell *, cell))
    {
        d->removeElement(cell);
    }
    /**
     * @brief remove all cell
     */
    ZFMETHOD_INLINE_0(void, cellRemoveAll)
    {
        d->removeAll();
    }

public:
    zfoverride
    virtual zfindex cellCount(void)
    {
        return d->count();
    }
    zfoverride
    virtual zfautoObject cellAtIndex(ZF_IN zfindex index)
    {
        return d->get(index);
    }

protected:
    zfoverride
    virtual zfautoObject cellCacheOnAccess(ZF_IN zfindex index)
    {
        return d->get(index);
    }

protected:
    zfoverride
    virtual zfbool serializableOnSerializeFromData(ZF_IN const ZFSerializableData &serializableData,
                                                   ZF_OUT_OPT zfstring *outErrorHint = zfnull,
                                                   ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull);
    zfoverride
    virtual zfbool serializableOnSerializeToData(ZF_IN_OUT ZFSerializableData &serializableData,
                                                 ZF_IN ZFSerializable *referencedOwnerOrNull,
                                                 ZF_OUT_OPT zfstring *outErrorHint = zfnull);

private:
    ZFArray *d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIListAdapterArray_h_

