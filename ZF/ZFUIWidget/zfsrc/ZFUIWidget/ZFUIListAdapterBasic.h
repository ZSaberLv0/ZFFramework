/**
 * @file ZFUIListAdapterBasic.h
 * @brief basic list adapter that allow dynamic extension
 */

#ifndef _ZFI_ZFUIListAdapterBasic_h_
#define _ZFI_ZFUIListAdapterBasic_h_

#include "ZFUIListAdapter.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/** @brief see #ZFUIListAdapterBasic::EventListCellAtIndex */
zfclass ZF_ENV_EXPORT ZFUIListAdapterBasicParam : zfextends ZFObject
{
    ZFOBJECT_DECLARE(ZFUIListAdapterBasicParam, ZFObject)

public:
    /** @brief see #ZFUIListAdapterBasic::EventListCellAtIndex */
    ZFPROPERTY_RETAIN(ZFUIListCell *, cell)
    /** @brief see #ZFUIListAdapterBasic::EventListCellAtIndex */
    ZFPROPERTY_ASSIGN(zfindex, cellIndex)
    /** @brief see #ZFUIListAdapterBasic::EventListCellAtIndex */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfint, cellSize, -1)
};

// ============================================================
/**
 * @brief basic list adapter that allow dynamic extension
 *
 * this class is designed for script file to implement list adapter dynamically,
 * typical usage in lua:
 * @code
 *   listView:listAdapterAutoRetain(ZFUIListAdapterBasic());
 *
 *   -- required
 *   listView:listAdapter():observerAdd(ZFUIListAdapterBasic.EventListCellCount(), function(listenerData, userData)
 *       listenerData:param0():zfv(yourListCellCount());
 *   end);
 *   listView:listAdapter():observerAdd(ZFUIListAdapterBasic.EventListCellAtIndex(), function(listenerData, userData)
 *       local param = listenerData:param0();
 *       param:cell(yourListCellAtIndex(param:cellIndex()));
 *   end);
 *
 *   -- optional
 *   listView:listAdapter():observerAdd(ZFUIListAdapterBasic.EventListCellSizeAtIndex(), function(listenerData, userData)
 *       local param = listenerData:param0();
 *       param:cellSize(yourListCellSizeAtIndex(param:cellIndex(), param:cell()));
 *   end);
 *   listView:listAdapter():observerAdd(ZFUIListAdapterBasic.EventListCellCacheOnAccess(), function(listenerData, userData)
 *       local param = listenerData:param0();
 *       param:cell(yourListCellCacheAtIndex(param:cellIndex()));
 *   end);
 *   listView:listAdapter():observerAdd(ZFUIListAdapterBasic.EventListCellCacheOnRecycle(), function(listenerData, userData)
 *       local cell = listenerData:param0();
 *       yourListCellRecycle(cell);
 *   end);
 * @endcode
 */
zfclass ZF_ENV_EXPORT ZFUIListAdapterBasic : zfextends ZFStyleableObject, zfimplements ZFUIListAdapter
{
    ZFOBJECT_DECLARE(ZFUIListAdapterBasic, ZFStyleableObject)
    ZFIMPLEMENTS_DECLARE(ZFUIListAdapter)

public:
    // ============================================================
    // events
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when #cellCount,
     * param0 is #v_zfindex that holds the cell count
     */
    ZFOBSERVER_EVENT(ListCellCount)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when #cellAtIndex,
     * param0 is #ZFUIListAdapterBasicParam
     */
    ZFOBSERVER_EVENT(ListCellAtIndex)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when #cellSizeAtIndex,
     * param0 is #ZFUIListAdapterBasicParam
     */
    ZFOBSERVER_EVENT(ListCellSizeAtIndex)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when #cellCacheOnAccess,
     * param0 is #ZFUIListAdapterBasicParam
     */
    ZFOBSERVER_EVENT(ListCellCacheOnAccess)

public:
    zfoverride
    virtual zfindex cellCount(void)
    {
        zfblockedAlloc(v_zfindex, ret);
        this->observerNotify(zfself::EventListCellCount(), ret);
        return ret->zfv;
    }
    zfoverride
    virtual zfautoObject cellAtIndex(ZF_IN zfindex index)
    {
        zfblockedAlloc(ZFUIListAdapterBasicParam, param);
        param->cellIndex(index);
        this->observerNotify(zfself::EventListCellAtIndex(), param);
        return param->cell();
    }
    zfoverride
    virtual zfint cellSizeAtIndex(ZF_IN zfindex index,
                                  ZF_IN ZFUIListCell *cell)
    {
        if(this->observerHasAdd(zfself::EventListCellSizeAtIndex()))
        {
            zfblockedAlloc(ZFUIListAdapterBasicParam, param);
            param->cellIndex(index);
            param->cell(cell);
            this->observerNotify(zfself::EventListCellSizeAtIndex(), param);
            return param->cellSize();
        }
        else
        {
            return zfsuperI(ZFUIListAdapter)::cellSizeAtIndex(index, cell);
        }
    }

protected:
    zfoverride
    virtual zfautoObject cellCacheOnAccess(ZF_IN zfindex index)
    {
        if(this->observerHasAdd(zfself::EventListCellCacheOnAccess()))
        {
            zfblockedAlloc(ZFUIListAdapterBasicParam, param);
            param->cellIndex(index);
            this->observerNotify(zfself::EventListCellCacheOnAccess(), param);
            return param->cell();
        }
        else
        {
            return zfsuperI(ZFUIListAdapter)::cellCacheOnAccess(index);
        }
    }
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIListAdapterBasic_h_

