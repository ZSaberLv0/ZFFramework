#include "ZFUIListView.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFSTYLE_DEFAULT_DEFINE(ZFUIListView)

// ============================================================
// _ZFP_ZFUIListViewPrivate
zfclassNotPOD _ZFP_ZFUIListViewPrivate
{
public:
    ZFUIListView *pimplOwner;
    zfbool childAddOverrideFlag;
    zfbool scrollContentFrameOverrideFlag;
    ZFUIListAdapter *listAdapter;
    ZFListener listAdapterOnReloadListener;
    zfbool listReloadRequested;
    zfbool listQuickReloadRequested;
    zfbool cellNeedUpdate;
    zfindex cellCount;
    ZFCoreArrayPOD<zfint> cellSizeList;
    ZFCoreArrayPOD<ZFUIListCell *> listVisibleCell; // retain manually
    ZFIndexRange listVisibleCellIndexRange;
    /*
     * left: left most cell's x
     * top: top most cell's y
     * right: right most cell's (x + width)
     * bottom: bottom most cell's (y + height)
     */
    zfint listVisibleCellOffset;
    zfbool listVisibleCellOffsetNeedUpdate;
    zfbool listReloadByChangeListOrientation;

    /*
     * used by scrollListCellToHead/Tail
     * if activating, we will recursively scrollByPoint until reach desired position
     * task would be canceled if content frame changed manually
     */
    zfindex scrollListCellIndex; // zfindexMax() if not activating
    zfint scrollListCellOffset;
    zfbool scrollListCellToHead;
    zfbool scrollListCellAnimated;
    zfint scrollListCellDesiredPosSaved;

public:
    _ZFP_ZFUIListViewPrivate(void)
    : pimplOwner(zfnull)
    , childAddOverrideFlag(zffalse)
    , scrollContentFrameOverrideFlag(zffalse)
    , listAdapter(zfnull)
    , listAdapterOnReloadListener(ZFCallbackForFunc(listAdapterOnReload))
    , listReloadRequested(zftrue)
    , listQuickReloadRequested(zftrue)
    , cellNeedUpdate(zftrue)
    , cellCount(0)
    , cellSizeList()
    , listVisibleCell()
    , listVisibleCellIndexRange(ZFIndexRangeZero())
    , listVisibleCellOffset(0)
    , listVisibleCellOffsetNeedUpdate(zftrue)
    , listReloadByChangeListOrientation(zftrue)
    , scrollListCellIndex(zfindexMax())
    , scrollListCellOffset(0)
    , scrollListCellToHead(zftrue)
    , scrollListCellAnimated(zftrue)
    , scrollListCellDesiredPosSaved(0)
    {
    }

public:
    static ZFLISTENER_PROTOTYPE_EXPAND(listAdapterOnReload)
    {
        ZFUIListView *listView = userData->objectHolded();
        v_zfindex *atIndex = listenerData.param0<v_zfindex *>();
        if(atIndex == zfnull || atIndex->zfv == zfindexMax())
        {
            listView->listReload();
        }
        else
        {
            listView->listReloadCellAtIndex(atIndex->zfv);
        }
    }
    void listBounceUpdate(void)
    {
        if(this->pimplOwner->listBounce())
        {
            switch(this->pimplOwner->listOrientation())
            {
                case ZFUIOrientation::e_Left:
                case ZFUIOrientation::e_Right:
                    this->pimplOwner->scrollBounceVerticalAlways(zffalse);
                    this->pimplOwner->scrollBounceHorizontalAlways(zftrue);
                    break;
                case ZFUIOrientation::e_Top:
                case ZFUIOrientation::e_Bottom:
                    this->pimplOwner->scrollBounceVerticalAlways(zftrue);
                    this->pimplOwner->scrollBounceHorizontalAlways(zffalse);
                    break;
                default:
                    zfCoreCriticalShouldNotGoHere();
                    return ;
            }
        }
    }
    void listAdaptertingUpdate(void)
    {
        if(this->listAdapter != zfnull)
        {
            this->listAdapter->_ZFP_ZFUIListAdapter_listOrientation = this->pimplOwner->listOrientation();
            this->listAdapter->_ZFP_ZFUIListAdapter_listContainerSize = ZFUIRectGetSize(this->pimplOwner->scrollArea());
        }
    }
    void listAdapterAfterAttach(void)
    {
        if(this->listAdapter != zfnull)
        {
            this->listAdapter->toObject()->observerAdd(
                ZFUIListAdapter::EventListAdapterOnReload(),
                this->listAdapterOnReloadListener,
                this->pimplOwner->objectHolder());
            this->listAdaptertingUpdate();
        }
    }
    void listAdapterBeforeDetach(void)
    {
        if(this->listAdapter)
        {
            this->listAdapter->toObject()->observerRemove(
                ZFUIListAdapter::EventListAdapterOnReload(),
                this->listAdapterOnReloadListener);
        }
    }

public:
    void childAdd(ZF_IN ZFUIView *child)
    {
        this->childAddOverrideFlag = zftrue;
        this->pimplOwner->childAdd(child);
        this->childAddOverrideFlag = zffalse;
    }
    void childAdd(ZF_IN ZFUIView *child, ZFUIViewLayoutParam *layoutParam, ZF_IN zfindex index)
    {
        this->childAddOverrideFlag = zftrue;
        this->pimplOwner->childAdd(child, layoutParam, index);
        this->childAddOverrideFlag = zffalse;
    }
    void childRemoveAtIndex(ZF_IN zfindex index)
    {
        this->childAddOverrideFlag = zftrue;
        this->pimplOwner->childRemoveAtIndex(index);
        this->childAddOverrideFlag = zffalse;
    }
    zfautoObject cellLoadAtIndex(ZF_IN zfindex index)
    {
        zfautoObject ret = this->listAdapter->cellCacheOnAccess(index);
        if(ret != zfnull)
        {
            zfCoreAssertWithMessage(ZFCastZFObject(ZFUIListCell *, ret.toObject()) != zfnull, "list cell %s not type of %s",
                ret.toObject()->classData()->classNameFull(),
                ZFUIListCell::ClassData()->classNameFull());
            return ret;
        }
        ret = this->listAdapter->cellAtIndex(index);
        zfCoreAssertWithMessage(ret != zfnull, "cellAtIndex must return a %s", ZFUIListCell::ClassData()->classNameFull());
        ZFUIListCell *cell = ZFCastZFObject(ZFUIListCell *, ret.toObject());
        zfCoreAssertWithMessage(cell != zfnull, "list cell %s not type of %s",
            ret.toObject()->classData()->classNameFull(),
            ZFUIListCell::ClassData()->classNameFull());
        return ret;
    }
    void cellOnUpdate(ZF_IN zfindex index, ZF_IN ZFUIListCell *cell)
    {
        if(!this->pimplOwner->cellUpdater()->isEmpty())
        {
            ZFUIListCellUpdaterParam updateParam;
            updateParam.cell = cell;
            updateParam.cellIndex = index;
            updateParam.cellCount = this->cellCount;
            updateParam.listOrientation = this->listAdapter->listOrientation();
            updateParam.listContainerSize = this->listAdapter->listContainerSize();
            updateParam.cellSizeHint = this->listAdapter->cellSizeHint();
            for(zfindex i = 0; i < this->pimplOwner->cellUpdater()->count(); ++i)
            {
                ZFObject *cellUpdater = this->pimplOwner->cellUpdater()->get(i);
                cellUpdater->to<ZFUIListCellUpdater *>()->cellOnUpdate(updateParam);
                if(cellUpdater->observerHasAdd(ZFUIListCellUpdater::EventCellOnUpdate()))
                {
                    zfblockedAlloc(v_ZFUIListCellUpdaterParam, param0, updateParam);
                    cellUpdater->observerNotify(ZFUIListCellUpdater::EventCellOnUpdate(), param0);
                }
            }
        }
        this->listAdapter->cellOnUpdate(index, cell);
    }
    void cellOnRecycle(ZF_IN ZFUIListCell *cell)
    {
        if(!this->pimplOwner->cellUpdater()->isEmpty())
        {
            for(zfindex i = 0; i < this->pimplOwner->cellUpdater()->count(); ++i)
            {
                ZFObject *cellUpdater = this->pimplOwner->cellUpdater()->get(i);
                cellUpdater->to<ZFUIListCellUpdater *>()->cellOnRecycle(cell);
                cellUpdater->observerNotify(ZFUIListCellUpdater::EventCellOnRecycle(), cell);
            }
        }
        this->listAdapter->cellCacheOnRecycle(cell);
    }
    zfint cellSizeAtIndex(ZF_IN zfindex index, ZF_IN ZFUIListCell *cell)
    {
        zfint ret = this->listAdapter->cellSizeAtIndex(index, cell);
        if(ret < 0)
        {
            switch(this->pimplOwner->listOrientation())
            {
                case ZFUIOrientation::e_Left:
                case ZFUIOrientation::e_Right:
                    ret = cell->layoutMeasure(
                            ZFUISizeMake(-1, this->pimplOwner->scrollArea().height),
                            ZFUISizeParamWrapFill()
                        ).width;
                    break;
                case ZFUIOrientation::e_Top:
                case ZFUIOrientation::e_Bottom:
                    ret = cell->layoutMeasure(
                            ZFUISizeMake(this->pimplOwner->scrollArea().width, -1),
                            ZFUISizeParamFillWrap()
                        ).height;
                    break;
                default:
                    zfCoreCriticalShouldNotGoHere();
                    break;
            }
        }
        return ret;
    }
    void removeAll(void)
    {
        this->cellCount = 0;
        this->cellSizeList.removeAll();
        this->listVisibleCellIndexRange = ZFIndexRangeZero();
        this->listVisibleCellOffset = 0;
        if(!this->listVisibleCell.isEmpty())
        {
            for(zfindex i = this->listVisibleCell.count() - 1; i != zfindexMax(); --i)
            {
                ZFUIListCell *cell = this->listVisibleCell[i];
                this->childRemoveAtIndex(i);
                if(this->listAdapter != zfnull)
                {
                    this->cellOnRecycle(cell);
                }
                this->pimplOwner->cellOnDetach(cell);
                zfRelease(cell);
            }
            this->listVisibleCell.removeAll();
        }
    }
    void cellRemoveBefore(ZF_IN zfindex index)
    {
        if(index < this->listVisibleCellIndexRange.start || index == zfindexMax())
        {
            return ;
        }

        zfindex indexOfVisibleCell = index - this->listVisibleCellIndexRange.start;
        for(zfindex i = indexOfVisibleCell; i != zfindexMax(); --i)
        {
            this->childRemoveAtIndex(i);
            ZFUIListCell *cell = this->listVisibleCell[i];
            this->cellOnRecycle(cell);
            this->pimplOwner->cellOnDetach(cell);
            zfRelease(cell);
        }
        this->listVisibleCell.remove(0, indexOfVisibleCell + 1);
        this->listVisibleCellIndexRange.start = index + 1;
        this->listVisibleCellIndexRange.count = this->listVisibleCell.count();
        if(this->listVisibleCellIndexRange.count == 0)
        {
            this->listVisibleCellIndexRange.start = 0;
        }
    }
    void cellRemoveAfter(ZF_IN zfindex index)
    {
        if(index >= this->listVisibleCellIndexRange.start + this->listVisibleCellIndexRange.count)
        {
            return ;
        }

        zfindex indexOfVisibleCell = index - this->listVisibleCellIndexRange.start;
        for(zfindex i = this->listVisibleCellIndexRange.count - 1; i != zfindexMax() && i >= indexOfVisibleCell; --i)
        {
            this->childRemoveAtIndex(i);
            ZFUIListCell *cell = this->listVisibleCell[i];
            this->cellOnRecycle(cell);
            this->pimplOwner->cellOnDetach(cell);
            zfRelease(cell);
        }
        this->listVisibleCell.remove(indexOfVisibleCell, zfindexMax());
        this->listVisibleCellIndexRange.count = this->listVisibleCell.count();
        if(this->listVisibleCellIndexRange.count == 0)
        {
            this->listVisibleCellIndexRange.start = 0;
        }
    }
    void updateHeadCellBeforeIndex(ZF_IN zfindex index,
                                   ZF_IN const ZFUIRect &cellFrame)
    {
        this->scrollContentFrameOverrideFlag = zftrue;

        ZFUIMargin scrollMargin = ZFUIMarginInc(
            this->pimplOwner->nativeImplViewMargin(),
            this->pimplOwner->scrollAreaMargin());
        const ZFUIRect &scrollContentFrame = this->pimplOwner->scrollContentFrame();
        const ZFUIRect &viewFrame = this->pimplOwner->viewFrame();

        switch(this->pimplOwner->listOrientation())
        {
            case ZFUIOrientation::e_Left:
            {
                zfint offset = cellFrame.x + cellFrame.width;
                zfint offsetEnd = zfmMax(
                    -scrollContentFrame.x - scrollMargin.left,
                    0);
                zfint offsetBegin = zfmMin(
                    offsetEnd + viewFrame.width,
                    scrollContentFrame.width);
                // remove cells exceeds visible range
                for( ; index >= this->listVisibleCellIndexRange.start && index < this->listVisibleCellIndexRange.start + this->listVisibleCellIndexRange.count; --index)
                {
                    if(offset < offsetEnd)
                    {
                        this->cellNeedUpdate = zftrue;
                        this->cellRemoveBefore(index);
                        break;
                    }
                    offset -= this->cellSizeList[index];
                }
                // skip
                for( ; index != zfindexMax() && offset - this->cellSizeList[index] > offsetBegin; --index)
                {
                    offset -= this->cellSizeList[index];
                }
                // load
                if(index != zfindexMax())
                {
                    zfint sizeDelta = 0;
                    for( ; index != zfindexMax() && offset >= offsetEnd; --index)
                    {
                        if(index >= this->listVisibleCellIndexRange.start && index < this->listVisibleCellIndexRange.start + this->listVisibleCellIndexRange.count)
                        {
                            offset -= this->cellSizeList[index];
                            continue;
                        }
                        this->cellNeedUpdate = zftrue;
                        ZFUIListCell *cellNew = zfRetain(this->cellLoadAtIndex(index).to<ZFUIListCell *>());
                        zfint cellSizeNew = this->cellSizeAtIndex(index, cellNew);
                        if(cellSizeNew != this->cellSizeList[index])
                        {
                            sizeDelta += cellSizeNew - this->cellSizeList[index];
                            this->cellSizeList[index] = cellSizeNew;
                        }
                        this->listVisibleCell.add(0, cellNew);
                        this->listVisibleCellIndexRange.start = index;
                        ++(this->listVisibleCellIndexRange.count);
                        this->childAdd(cellNew, zfnull, 0);
                        this->pimplOwner->cellOnAttach(cellNew);
                        this->cellOnUpdate(index, cellNew);
                        offset -= cellSizeNew;
                    }
                    // update content
                    if(sizeDelta != 0)
                    {
                        ZFUIRect contentFrame = scrollContentFrame;
                        contentFrame.x -= sizeDelta;
                        contentFrame.width += sizeDelta;
                        this->pimplOwner->scrollContentFrameUpdate(contentFrame);
                    }
                    if(index == zfindexMax())
                    {
                        this->listVisibleCellOffset = 0;
                    }
                    else
                    {
                        this->listVisibleCellOffset = offset + sizeDelta;
                    }
                }
            }
                break;
            case ZFUIOrientation::e_Top:
            {
                zfint offset = cellFrame.y + cellFrame.height;
                zfint offsetEnd = zfmMax(
                    -scrollContentFrame.y - scrollMargin.top,
                    0);
                zfint offsetBegin = zfmMin(
                    offsetEnd + viewFrame.height,
                    scrollContentFrame.height);
                // remove cells exceeds visible range
                for( ; index >= this->listVisibleCellIndexRange.start && index < this->listVisibleCellIndexRange.start + this->listVisibleCellIndexRange.count; --index)
                {
                    if(offset < offsetEnd)
                    {
                        this->cellNeedUpdate = zftrue;
                        this->cellRemoveBefore(index);
                        break;
                    }
                    offset -= this->cellSizeList[index];
                }
                // skip
                for( ; index != zfindexMax() && offset - this->cellSizeList[index] > offsetBegin; --index)
                {
                    offset -= this->cellSizeList[index];
                }
                // load
                if(index != zfindexMax())
                {
                    zfint sizeDelta = 0;
                    for( ; index != zfindexMax() && offset >= offsetEnd; --index)
                    {
                        if(index >= this->listVisibleCellIndexRange.start && index < this->listVisibleCellIndexRange.start + this->listVisibleCellIndexRange.count)
                        {
                            offset -= this->cellSizeList[index];
                            continue;
                        }
                        this->cellNeedUpdate = zftrue;
                        ZFUIListCell *cellNew = zfRetain(this->cellLoadAtIndex(index).to<ZFUIListCell *>());
                        zfint cellSizeNew = this->cellSizeAtIndex(index, cellNew);
                        if(cellSizeNew != this->cellSizeList[index])
                        {
                            sizeDelta += cellSizeNew - this->cellSizeList[index];
                            this->cellSizeList[index] = cellSizeNew;
                        }
                        this->listVisibleCell.add(0, cellNew);
                        this->listVisibleCellIndexRange.start = index;
                        ++(this->listVisibleCellIndexRange.count);
                        this->childAdd(cellNew, zfnull, 0);
                        this->pimplOwner->cellOnAttach(cellNew);
                        this->cellOnUpdate(index, cellNew);
                        offset -= cellSizeNew;
                    }
                    // update content
                    if(sizeDelta != 0)
                    {
                        ZFUIRect contentFrame = scrollContentFrame;
                        contentFrame.y -= sizeDelta;
                        contentFrame.height += sizeDelta;
                        this->pimplOwner->scrollContentFrameUpdate(contentFrame);
                    }
                    if(index == zfindexMax())
                    {
                        this->listVisibleCellOffset = 0;
                    }
                    else
                    {
                        this->listVisibleCellOffset = offset + sizeDelta;
                    }
                }
            }
                break;
            case ZFUIOrientation::e_Right:
            {
                zfint offset = cellFrame.x;
                zfint offsetEnd = zfmMin(
                    -scrollContentFrame.x + viewFrame.width - scrollMargin.left,
                    scrollContentFrame.width);
                zfint offsetBegin = zfmMax(
                    offsetEnd - viewFrame.width,
                    0);
                // remove cells exceeds visible range
                for( ; index >= this->listVisibleCellIndexRange.start && index < this->listVisibleCellIndexRange.start + this->listVisibleCellIndexRange.count; --index)
                {
                    if(offset > offsetEnd)
                    {
                        this->cellNeedUpdate = zftrue;
                        this->cellRemoveBefore(index);
                        break;
                    }
                    offset += this->cellSizeList[index];
                }
                // skip
                for( ; index != zfindexMax() && offset + this->cellSizeList[index] < offsetBegin; --index)
                {
                    offset += this->cellSizeList[index];
                }
                // load
                if(index != zfindexMax())
                {
                    zfint sizeDelta = 0;
                    for( ; index != zfindexMax() && offset <= offsetEnd; --index)
                    {
                        if(index >= this->listVisibleCellIndexRange.start && index < this->listVisibleCellIndexRange.start + this->listVisibleCellIndexRange.count)
                        {
                            offset += this->cellSizeList[index];
                            continue;
                        }
                        this->cellNeedUpdate = zftrue;
                        ZFUIListCell *cellNew = zfRetain(this->cellLoadAtIndex(index).to<ZFUIListCell *>());
                        zfint cellSizeNew = this->cellSizeAtIndex(index, cellNew);
                        if(cellSizeNew != this->cellSizeList[index])
                        {
                            sizeDelta += cellSizeNew - this->cellSizeList[index];
                            this->cellSizeList[index] = cellSizeNew;
                        }
                        this->listVisibleCell.add(0, cellNew);
                        this->listVisibleCellIndexRange.start = index;
                        ++(this->listVisibleCellIndexRange.count);
                        this->childAdd(cellNew, zfnull, 0);
                        this->pimplOwner->cellOnAttach(cellNew);
                        this->cellOnUpdate(index, cellNew);
                        offset += cellSizeNew;
                    }
                    // update content
                    if(sizeDelta != 0)
                    {
                        ZFUIRect contentFrame = scrollContentFrame;
                        contentFrame.width += sizeDelta;
                        this->pimplOwner->scrollContentFrameUpdate(contentFrame);
                    }
                    this->listVisibleCellOffset = offset;
                }
            }
                break;
            case ZFUIOrientation::e_Bottom:
            {
                zfint offset = cellFrame.y;
                zfint offsetEnd = zfmMin(
                    -scrollContentFrame.y + viewFrame.height - scrollMargin.top,
                    scrollContentFrame.height);
                zfint offsetBegin = zfmMax(
                    offsetEnd - viewFrame.height,
                    0);
                // remove cells exceeds visible range
                for( ; index >= this->listVisibleCellIndexRange.start && index < this->listVisibleCellIndexRange.start + this->listVisibleCellIndexRange.count; --index)
                {
                    if(offset > offsetEnd)
                    {
                        this->cellNeedUpdate = zftrue;
                        this->cellRemoveBefore(index);
                        break;
                    }
                    offset += this->cellSizeList[index];
                }
                // skip
                for( ; index != zfindexMax() && offset + this->cellSizeList[index] < offsetBegin; --index)
                {
                    offset += this->cellSizeList[index];
                }
                // load
                if(index != zfindexMax())
                {
                    zfint sizeDelta = 0;
                    for( ; index != zfindexMax() && offset <= offsetEnd; --index)
                    {
                        if(index >= this->listVisibleCellIndexRange.start && index < this->listVisibleCellIndexRange.start + this->listVisibleCellIndexRange.count)
                        {
                            offset += this->cellSizeList[index];
                            continue;
                        }
                        this->cellNeedUpdate = zftrue;
                        ZFUIListCell *cellNew = zfRetain(this->cellLoadAtIndex(index).to<ZFUIListCell *>());
                        zfint cellSizeNew = this->cellSizeAtIndex(index, cellNew);
                        if(cellSizeNew != this->cellSizeList[index])
                        {
                            sizeDelta += cellSizeNew - this->cellSizeList[index];
                            this->cellSizeList[index] = cellSizeNew;
                        }
                        this->listVisibleCell.add(0, cellNew);
                        this->listVisibleCellIndexRange.start = index;
                        ++(this->listVisibleCellIndexRange.count);
                        this->childAdd(cellNew, zfnull, 0);
                        this->pimplOwner->cellOnAttach(cellNew);
                        this->cellOnUpdate(index, cellNew);
                        offset += cellSizeNew;
                    }
                    // update content
                    if(sizeDelta != 0)
                    {
                        ZFUIRect contentFrame = scrollContentFrame;
                        contentFrame.height += sizeDelta;
                        this->pimplOwner->scrollContentFrameUpdate(contentFrame);
                    }
                    this->listVisibleCellOffset = offset;
                }
            }
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                return ;
        }

        this->listVisibleCellOffsetNeedUpdate = zffalse;
        this->scrollContentFrameOverrideFlag = zffalse;
    }
    void updateTailCellAfterIndex(ZF_IN zfindex index,
                                  ZF_IN const ZFUIRect &cellFrame)
    {
        this->scrollContentFrameOverrideFlag = zftrue;

        ZFUIMargin scrollMargin = ZFUIMarginInc(
            this->pimplOwner->nativeImplViewMargin(),
            this->pimplOwner->scrollAreaMargin());
        const ZFUIRect &scrollContentFrame = this->pimplOwner->scrollContentFrame();
        const ZFUIRect &viewFrame = this->pimplOwner->viewFrame();

        switch(this->pimplOwner->listOrientation())
        {
            case ZFUIOrientation::e_Left:
            {
                zfint offset = cellFrame.x;
                zfint offsetEnd = zfmMin(
                    -scrollContentFrame.x + viewFrame.width - scrollMargin.left,
                    scrollContentFrame.width);
                zfint offsetBegin = zfmMax(
                    offsetEnd - viewFrame.width,
                    0);
                // remove cells exceeds visible range
                for( ; index >= this->listVisibleCellIndexRange.start && index < this->listVisibleCellIndexRange.start + this->listVisibleCellIndexRange.count; ++index)
                {
                    if(offset > offsetEnd)
                    {
                        this->cellNeedUpdate = zftrue;
                        this->cellRemoveAfter(index);
                        break;
                    }
                    offset += this->cellSizeList[index];
                }
                // skip
                for( ; index < this->cellCount && offset + this->cellSizeList[index] < offsetBegin; ++index)
                {
                    offset += this->cellSizeList[index];
                }
                // load
                if(index < this->cellCount)
                {
                    zfint sizeDelta = 0;
                    for( ; index < this->cellCount && offset <= offsetEnd; ++index)
                    {
                        if(index >= this->listVisibleCellIndexRange.start && index < this->listVisibleCellIndexRange.start + this->listVisibleCellIndexRange.count)
                        {
                            offset += this->cellSizeList[index];
                            continue;
                        }
                        this->cellNeedUpdate = zftrue;
                        ZFUIListCell *cellNew = zfRetain(this->cellLoadAtIndex(index).to<ZFUIListCell *>());
                        zfint cellSizeNew = this->cellSizeAtIndex(index, cellNew);
                        if(cellSizeNew != this->cellSizeList[index])
                        {
                            sizeDelta += cellSizeNew - this->cellSizeList[index];
                            this->cellSizeList[index] = cellSizeNew;
                        }
                        this->listVisibleCell.add(cellNew);
                        ++(this->listVisibleCellIndexRange.count);
                        this->childAdd(cellNew);
                        this->pimplOwner->cellOnAttach(cellNew);
                        this->cellOnUpdate(index, cellNew);
                        if(this->listVisibleCellIndexRange.count == 1)
                        {
                            this->listVisibleCellIndexRange.start = index;
                            if(index == 0)
                            {
                                this->listVisibleCellOffset = 0;
                            }
                            else
                            {
                                this->listVisibleCellOffset = offset;
                            }
                        }
                        offset += cellSizeNew;
                    }
                    // update content
                    if(sizeDelta != 0)
                    {
                        ZFUIRect contentFrame = this->pimplOwner->scrollContentFrame();
                        contentFrame.width += sizeDelta;
                        this->pimplOwner->scrollContentFrameUpdate(contentFrame);
                    }
                }
            }
                break;
            case ZFUIOrientation::e_Top:
            {
                zfint offset = cellFrame.y;
                zfint offsetEnd = zfmMin(
                    -scrollContentFrame.y + viewFrame.height - scrollMargin.top,
                    scrollContentFrame.height);
                zfint offsetBegin = zfmMax(
                    offsetEnd - viewFrame.height,
                    0);
                // remove cells exceeds visible range
                for( ; index >= this->listVisibleCellIndexRange.start && index < this->listVisibleCellIndexRange.start + this->listVisibleCellIndexRange.count; ++index)
                {
                    if(offset > offsetEnd)
                    {
                        this->cellNeedUpdate = zftrue;
                        this->cellRemoveAfter(index);
                        break;
                    }
                    offset += this->cellSizeList[index];
                }
                // skip
                for( ; index < this->cellCount && offset + this->cellSizeList[index] < offsetBegin; ++index)
                {
                    offset += this->cellSizeList[index];
                }
                // load
                if(index < this->cellCount)
                {
                    zfint sizeDelta = 0;
                    for( ; index < this->cellCount && offset <= offsetEnd; ++index)
                    {
                        if(index >= this->listVisibleCellIndexRange.start && index < this->listVisibleCellIndexRange.start + this->listVisibleCellIndexRange.count)
                        {
                            offset += this->cellSizeList[index];
                            continue;
                        }
                        this->cellNeedUpdate = zftrue;
                        ZFUIListCell *cellNew = zfRetain(this->cellLoadAtIndex(index).to<ZFUIListCell *>());
                        zfint cellSizeNew = this->cellSizeAtIndex(index, cellNew);
                        if(cellSizeNew != this->cellSizeList[index])
                        {
                            sizeDelta += cellSizeNew - this->cellSizeList[index];
                            this->cellSizeList[index] = cellSizeNew;
                        }
                        this->listVisibleCell.add(cellNew);
                        ++(this->listVisibleCellIndexRange.count);
                        this->childAdd(cellNew);
                        this->pimplOwner->cellOnAttach(cellNew);
                        this->cellOnUpdate(index, cellNew);
                        if(this->listVisibleCellIndexRange.count == 1)
                        {
                            this->listVisibleCellIndexRange.start = index;
                            if(index == 0)
                            {
                                this->listVisibleCellOffset = 0;
                            }
                            else
                            {
                                this->listVisibleCellOffset = offset;
                            }
                        }
                        offset += cellSizeNew;
                    }
                    // update content
                    if(sizeDelta != 0)
                    {
                        ZFUIRect contentFrame = this->pimplOwner->scrollContentFrame();
                        contentFrame.height += sizeDelta;
                        this->pimplOwner->scrollContentFrameUpdate(contentFrame);
                    }
                }
            }
                break;
            case ZFUIOrientation::e_Right:
            {
                zfint offset = cellFrame.x + cellFrame.width;
                zfint offsetEnd = zfmMax(
                    -scrollContentFrame.x - scrollMargin.left,
                    0);
                zfint offsetBegin = zfmMin(
                    offsetEnd + viewFrame.width,
                    scrollContentFrame.width);
                // remove cells exceeds visible range
                for( ; index >= this->listVisibleCellIndexRange.start && index < this->listVisibleCellIndexRange.start + this->listVisibleCellIndexRange.count; ++index)
                {
                    if(offset < offsetEnd)
                    {
                        this->cellNeedUpdate = zftrue;
                        this->cellRemoveAfter(index);
                        break;
                    }
                    offset -= this->cellSizeList[index];
                }
                // skip
                for( ; index < this->cellCount && offset - this->cellSizeList[index] > offsetBegin; ++index)
                {
                    offset -= this->cellSizeList[index];
                }
                // load
                if(index < this->cellCount)
                {
                    zfint sizeDelta = 0;
                    for( ; index < this->cellCount && offset >= offsetEnd; ++index)
                    {
                        if(index >= this->listVisibleCellIndexRange.start && index < this->listVisibleCellIndexRange.start + this->listVisibleCellIndexRange.count)
                        {
                            offset -= this->cellSizeList[index];
                            continue;
                        }
                        this->cellNeedUpdate = zftrue;
                        ZFUIListCell *cellNew = zfRetain(this->cellLoadAtIndex(index).to<ZFUIListCell *>());
                        zfint cellSizeNew = this->cellSizeAtIndex(index, cellNew);
                        if(cellSizeNew != this->cellSizeList[index])
                        {
                            sizeDelta += cellSizeNew - this->cellSizeList[index];
                            this->cellSizeList[index] = cellSizeNew;
                        }
                        this->listVisibleCell.add(cellNew);
                        ++(this->listVisibleCellIndexRange.count);
                        this->childAdd(cellNew);
                        this->pimplOwner->cellOnAttach(cellNew);
                        this->cellOnUpdate(index, cellNew);
                        if(this->listVisibleCellIndexRange.count == 1)
                        {
                            this->listVisibleCellOffset = offset;
                        }
                        offset -= cellSizeNew;
                    }
                    // update content
                    if(sizeDelta != 0)
                    {
                        this->listVisibleCellOffset += sizeDelta;
                        ZFUIRect contentFrame = this->pimplOwner->scrollContentFrame();
                        contentFrame.x -= sizeDelta;
                        contentFrame.width += sizeDelta;
                        this->pimplOwner->scrollContentFrameUpdate(contentFrame);
                    }
                }
            }
                break;
            case ZFUIOrientation::e_Bottom:
            {
                zfint offset = cellFrame.y + cellFrame.height;
                zfint offsetEnd = zfmMax(
                    -scrollContentFrame.y - scrollMargin.top,
                    0);
                zfint offsetBegin = zfmMin(
                    offsetEnd + viewFrame.height,
                    scrollContentFrame.height);
                // remove cells exceeds visible range
                for( ; index >= this->listVisibleCellIndexRange.start && index < this->listVisibleCellIndexRange.start + this->listVisibleCellIndexRange.count; ++index)
                {
                    if(offset < offsetEnd)
                    {
                        this->cellNeedUpdate = zftrue;
                        this->cellRemoveAfter(index);
                        break;
                    }
                    offset -= this->cellSizeList[index];
                }
                // skip
                for( ; index < this->cellCount && offset - this->cellSizeList[index] > offsetBegin; ++index)
                {
                    offset -= this->cellSizeList[index];
                }
                // load
                if(index < this->cellCount)
                {
                    zfint sizeDelta = 0;
                    for( ; index < this->cellCount && offset >= offsetEnd; ++index)
                    {
                        if(index >= this->listVisibleCellIndexRange.start && index < this->listVisibleCellIndexRange.start + this->listVisibleCellIndexRange.count)
                        {
                            offset -= this->cellSizeList[index];
                            continue;
                        }
                        this->cellNeedUpdate = zftrue;
                        ZFUIListCell *cellNew = zfRetain(this->cellLoadAtIndex(index).to<ZFUIListCell *>());
                        zfint cellSizeNew = this->cellSizeAtIndex(index, cellNew);
                        if(cellSizeNew != this->cellSizeList[index])
                        {
                            sizeDelta += cellSizeNew - this->cellSizeList[index];
                            this->cellSizeList[index] = cellSizeNew;
                        }
                        this->listVisibleCell.add(cellNew);
                        ++(this->listVisibleCellIndexRange.count);
                        this->childAdd(cellNew);
                        this->pimplOwner->cellOnAttach(cellNew);
                        this->cellOnUpdate(index, cellNew);
                        if(this->listVisibleCellIndexRange.count == 1)
                        {
                            this->listVisibleCellOffset = offset;
                        }
                        offset -= cellSizeNew;
                    }
                    // update content
                    if(sizeDelta != 0)
                    {
                        this->listVisibleCellOffset += sizeDelta;
                        ZFUIRect contentFrame = this->pimplOwner->scrollContentFrame();
                        contentFrame.y -= sizeDelta;
                        contentFrame.height += sizeDelta;
                        this->pimplOwner->scrollContentFrameUpdate(contentFrame);
                    }
                }
            }
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                return ;
        }

        this->listVisibleCellOffsetNeedUpdate = zffalse;
        this->scrollContentFrameOverrideFlag = zffalse;
    }
    void updateFromFirstCell(void)
    {
        if(this->cellCount == 0)
        {
            return ;
        }

        zfint cellSize = this->cellSizeList[0];
        switch(this->pimplOwner->listOrientation())
        {
            case ZFUIOrientation::e_Left:
                this->updateTailCellAfterIndex(0, ZFUIRectMake(
                    0,
                    0,
                    cellSize,
                    this->pimplOwner->scrollArea().height));
                break;
            case ZFUIOrientation::e_Top:
                this->updateTailCellAfterIndex(0, ZFUIRectMake(
                    0,
                    0,
                    this->pimplOwner->scrollArea().width,
                    cellSize));
                break;
            case ZFUIOrientation::e_Right:
                this->updateTailCellAfterIndex(0, ZFUIRectMake(
                    this->pimplOwner->scrollContentFrame().width - cellSize,
                    0,
                    cellSize,
                    this->pimplOwner->scrollArea().height));
                break;
            case ZFUIOrientation::e_Bottom:
                this->updateTailCellAfterIndex(0, ZFUIRectMake(
                    0,
                    this->pimplOwner->scrollContentFrame().height - cellSize,
                    this->pimplOwner->scrollArea().width,
                    cellSize));
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                return ;
        }
    }
    ZFUIRect listVisibleCellFrame(ZF_IN zfindex cellIndex)
    {
        ZFUIRect ret = ZFUIRectZero();
        switch(this->pimplOwner->listOrientation())
        {
            case ZFUIOrientation::e_Left:
                ret.x = this->listVisibleCellOffset;
                ret.height = this->pimplOwner->scrollArea().height;
                for(zfindex i = this->listVisibleCellIndexRange.start; ; ++i)
                {
                    if(i == cellIndex)
                    {
                        ret.width = this->cellSizeList[i];
                        break;
                    }
                    ret.x += this->cellSizeList[i];
                }
                break;
            case ZFUIOrientation::e_Top:
                ret.y = this->listVisibleCellOffset;
                ret.width = this->pimplOwner->scrollArea().width;
                for(zfindex i = this->listVisibleCellIndexRange.start; ; ++i)
                {
                    if(i == cellIndex)
                    {
                        ret.height = this->cellSizeList[i];
                        break;
                    }
                    ret.y += this->cellSizeList[i];
                }
                break;
            case ZFUIOrientation::e_Right:
                ret.x = this->listVisibleCellOffset;
                ret.height = this->pimplOwner->scrollArea().height;
                for(zfindex i = this->listVisibleCellIndexRange.start; ; ++i)
                {
                    ret.x -= this->cellSizeList[i];
                    if(i == cellIndex)
                    {
                        ret.width = this->cellSizeList[i];
                        break;
                    }
                }
                break;
            case ZFUIOrientation::e_Bottom:
                ret.y = this->listVisibleCellOffset;
                ret.width = this->pimplOwner->scrollArea().width;
                for(zfindex i = this->listVisibleCellIndexRange.start; ; ++i)
                {
                    ret.y -= this->cellSizeList[i];
                    if(i == cellIndex)
                    {
                        ret.height = this->cellSizeList[i];
                        break;
                    }
                }
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                return ZFUIRectZero();
        }
        return ret;
    }
    void listCheckReload(void)
    {
        if(!this->listReloadRequested)
        {
            if(this->listQuickReloadRequested)
            {
                this->listQuickReloadRequested = zffalse;
                ZFUIRect scrollContentFrame = this->pimplOwner->scrollContentFrame();
                switch(this->pimplOwner->listOrientation())
                {
                    case ZFUIOrientation::e_Left:
                    case ZFUIOrientation::e_Right:
                        scrollContentFrame.height = this->pimplOwner->scrollArea().height;
                        break;
                    case ZFUIOrientation::e_Top:
                    case ZFUIOrientation::e_Bottom:
                        scrollContentFrame.width = this->pimplOwner->scrollArea().width;
                        break;
                    default:
                        zfCoreCriticalShouldNotGoHere();
                        return ;
                }
                if(scrollContentFrame != this->pimplOwner->scrollContentFrame())
                {
                    this->scrollContentFrameOverrideFlag = zftrue;
                    this->pimplOwner->scrollContentFrameUpdate(scrollContentFrame);
                    this->scrollContentFrameOverrideFlag = zffalse;
                }
                if(!this->listVisibleCell.isEmpty())
                {
                    zfindex cellIndex = this->listVisibleCellIndexRange.start;
                    this->updateTailCellAfterIndex(
                        cellIndex,
                        this->listVisibleCellFrame(cellIndex));
                }
                else
                {
                    this->updateFromFirstCell();
                }
            }
            return ;
        }

        if(this->listAdapter == zfnull)
        {
            this->removeAll();
            this->scrollContentFrameOverrideFlag = zftrue;
            this->pimplOwner->scrollContentFrame(ZFUIRectZero());
            this->scrollContentFrameOverrideFlag = zffalse;
            this->listReloadRequested = zffalse;
            this->listQuickReloadRequested = zffalse;
            this->pimplOwner->listVisibleCellOnChange();
            return ;
        }

        this->cellNeedUpdate = zftrue;

        this->cellCount = this->listAdapter->cellCount();
        if(this->cellSizeList.count() > this->cellCount)
        {
            this->cellSizeList.remove(this->cellCount, zfindexMax());
        }
        else
        {
            zfint cellSizeHint = this->listAdapter->cellSizeHint();
            if(this->listAdapter->cellSizeFill())
            {
                switch(this->listAdapter->listOrientation())
                {
                    case ZFUIOrientation::e_Left:
                    case ZFUIOrientation::e_Right:
                        cellSizeHint = this->listAdapter->listContainerSize().width;
                        break;
                    case ZFUIOrientation::e_Top:
                    case ZFUIOrientation::e_Bottom:
                        cellSizeHint = this->listAdapter->listContainerSize().height;
                        break;
                    default:
                        zfCoreCriticalShouldNotGoHere();
                        break;
                }
            }
            else if(cellSizeHint < 0)
            {
                cellSizeHint = ZFUIGlobalStyle::DefaultStyle()->itemSizeListCell();
            }
            this->cellSizeList.capacity(this->cellCount);
            for(zfindex i = this->cellSizeList.count(); i < this->cellCount; ++i)
            {
                this->cellSizeList.add(cellSizeHint);
            }
        }
        zfint totalSize = 0;
        {
            const zfint *buf = this->cellSizeList.arrayBuf();
            for(zfindex i = 0, iEnd = this->cellSizeList.count(); i < iEnd; ++i)
            {
                totalSize += *buf;
                ++buf;
            }
        }
        this->scrollContentFrameOverrideFlag = zftrue;
        switch(this->pimplOwner->listOrientation())
        {
            case ZFUIOrientation::e_Left:
            case ZFUIOrientation::e_Right:
            {
                ZFUIRect scrollContentFrameNew = ZFUIRectMake(
                    this->pimplOwner->scrollContentFrame().x,
                    0,
                    zfmMax(totalSize, this->pimplOwner->scrollArea().width),
                    this->pimplOwner->scrollArea().height);
                if(this->listReloadByChangeListOrientation)
                {
                    if(this->pimplOwner->listOrientation() == ZFUIOrientation::e_Left)
                    {
                        scrollContentFrameNew.x = 0;
                    }
                    else
                    {
                        scrollContentFrameNew.x = this->pimplOwner->scrollArea().width - scrollContentFrameNew.width;
                    }
                }
                this->pimplOwner->scrollContentFrameUpdate(scrollContentFrameNew);
            }
                break;
            case ZFUIOrientation::e_Top:
            case ZFUIOrientation::e_Bottom:
            {
                ZFUIRect scrollContentFrameNew = ZFUIRectMake(
                    0,
                    this->pimplOwner->scrollContentFrame().y,
                    this->pimplOwner->scrollArea().width,
                    zfmMax(totalSize, this->pimplOwner->scrollArea().height));
                if(this->listReloadByChangeListOrientation)
                {
                    if(this->pimplOwner->listOrientation() == ZFUIOrientation::e_Top)
                    {
                        scrollContentFrameNew.y = 0;
                    }
                    else
                    {
                        scrollContentFrameNew.y = this->pimplOwner->scrollArea().height - scrollContentFrameNew.height;
                    }
                }
                this->pimplOwner->scrollContentFrameUpdate(scrollContentFrameNew);
            }
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                break;
        }
        this->scrollContentFrameOverrideFlag = zffalse;

        zfindex cellIndex = zfindexMax();
        if(!this->listVisibleCell.isEmpty())
        {
            cellIndex = this->listVisibleCellIndexRange.start;
        }

        this->listVisibleCellIndexRange = ZFIndexRangeZero();
        for(zfindex i = this->listVisibleCell.count() - 1; i != zfindexMax(); --i)
        {
            ZFUIListCell *cell = this->listVisibleCell[i];
            this->childRemoveAtIndex(i);
            this->cellOnRecycle(cell);
            this->pimplOwner->cellOnDetach(cell);
            zfRelease(cell);
        }
        this->listVisibleCell.removeAll();

        if(cellIndex != zfindexMax() && !this->listVisibleCellOffsetNeedUpdate)
        {
            ZFUIRect cellFrame = this->listVisibleCellFrame(cellIndex);
            this->updateHeadCellBeforeIndex(cellIndex, cellFrame);
            if(!this->listVisibleCell.isEmpty())
            {
                cellIndex = this->listVisibleCellIndexRange.start;
                cellFrame = this->listVisibleCellFrame(cellIndex);
                this->updateTailCellAfterIndex(cellIndex, cellFrame);
            }
            else
            {
                this->updateFromFirstCell();
            }
        }
        else
        {
            this->updateFromFirstCell();
        }

        this->listReloadRequested = zffalse;
        this->listQuickReloadRequested = zffalse;
        this->listReloadByChangeListOrientation = zffalse;
        this->pimplOwner->listVisibleCellOnChange();

        // fix content range
        ZFUIRect scrollContentFrame = this->pimplOwner->scrollContentFrame();
        if(scrollContentFrame.x > 0)
        {
            scrollContentFrame.x = 0;
        }
        if(scrollContentFrame.y > 0)
        {
            scrollContentFrame.y = 0;
        }
        if(scrollContentFrame.width > this->pimplOwner->scrollArea().width
            && scrollContentFrame.x + scrollContentFrame.width < this->pimplOwner->scrollArea().width)
        {
            scrollContentFrame.x = this->pimplOwner->scrollArea().width - scrollContentFrame.width;
        }
        if(scrollContentFrame.height > this->pimplOwner->scrollArea().height
            && scrollContentFrame.y + scrollContentFrame.height < this->pimplOwner->scrollArea().height)
        {
            scrollContentFrame.y = this->pimplOwner->scrollArea().height - scrollContentFrame.height;
        }
        this->pimplOwner->scrollContentFrameUpdate(scrollContentFrame);

        this->pimplOwner->scrollToFitRange();
    }
    void updateCellLayout(void)
    {
        if(!this->cellNeedUpdate)
        {
            return ;
        }
        this->cellNeedUpdate = zffalse;

        zfint offset = 0;
        zfint fillSize = 0;
        switch(this->pimplOwner->listOrientation())
        {
            case ZFUIOrientation::e_Left:
                offset = this->listVisibleCellOffset;
                fillSize = this->pimplOwner->scrollArea().height;
                for(zfindex i = 0; i < this->listVisibleCell.count(); ++i)
                {
                    zfint cellWidth = this->cellSizeList[this->listVisibleCellIndexRange.start + i];
                    this->listVisibleCell[i]->viewFrame(ZFUIRectMake(
                            offset,
                            0,
                            cellWidth,
                            fillSize
                        ));
                    offset += cellWidth;
                }
                break;
            case ZFUIOrientation::e_Top:
                offset = this->listVisibleCellOffset;
                fillSize = this->pimplOwner->scrollArea().width;
                for(zfindex i = 0; i < this->listVisibleCell.count(); ++i)
                {
                    zfint cellHeight = this->cellSizeList[this->listVisibleCellIndexRange.start + i];
                    this->listVisibleCell[i]->viewFrame(ZFUIRectMake(
                            0,
                            offset,
                            fillSize,
                            cellHeight
                        ));
                    offset += cellHeight;
                }
                break;
            case ZFUIOrientation::e_Right:
                offset = this->listVisibleCellOffset;
                fillSize = this->pimplOwner->scrollArea().height;
                for(zfindex i = 0; i < this->listVisibleCell.count(); ++i)
                {
                    zfint cellWidth = this->cellSizeList[this->listVisibleCellIndexRange.start + i];
                    offset -= cellWidth;
                    this->listVisibleCell[i]->viewFrame(ZFUIRectMake(
                            offset,
                            0,
                            cellWidth,
                            fillSize
                        ));
                }
                break;
            case ZFUIOrientation::e_Bottom:
                offset = this->listVisibleCellOffset;
                fillSize = this->pimplOwner->scrollArea().width;
                for(zfindex i = 0; i < this->listVisibleCell.count(); ++i)
                {
                    zfint cellHeight = this->cellSizeList[this->listVisibleCellIndexRange.start + i];
                    offset -= cellHeight;
                    this->listVisibleCell[i]->viewFrame(ZFUIRectMake(
                            0,
                            offset,
                            fillSize,
                            cellHeight
                        ));
                }
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                return ;
        }
    }

public:
    void scrollListCellCheckUpdate(void)
    {
        if(this->scrollListCellIndex == zfindexMax())
        {
            return ;
        }

        zfint desiredPos = this->scrollListCellDesiredPosCalc();
        if(!this->scrollListCellAnimated)
        {
            while(this->scrollListCellIndex != zfindexMax() && desiredPos != this->scrollListCellDesiredPosSaved)
            {
                this->scrollListCellScrollToPos(desiredPos, zffalse);
            }
            this->scrollListCellIndex = zfindexMax();
            return ;
        }
        if(desiredPos != this->scrollListCellDesiredPosSaved)
        {
            this->scrollListCellScrollToPos(desiredPos, zftrue);
        }
        else
        {
            this->scrollListCellIndex = zfindexMax();
        }
    }
private:
    zfint scrollListCellDesiredPosCalc(void)
    {
        zfint offset = 0;
        for(const zfint *p = this->cellSizeList.arrayBuf(), *pEnd = p + this->scrollListCellIndex; p != pEnd; ++p)
        {
            offset += *p;
        }
        if(this->scrollListCellToHead)
        {
            return this->scrollListCellOffset - offset;
        }
        else
        {
            if(ZFUIOrientationIsHorizontal(this->pimplOwner->listOrientation()))
            {
                return this->pimplOwner->scrollArea().width - (offset + this->cellSizeList[this->scrollListCellIndex] + this->scrollListCellOffset);
            }
            else
            {
                return this->pimplOwner->scrollArea().height - (offset + this->cellSizeList[this->scrollListCellIndex] + this->scrollListCellOffset);
            }
        }
    }
    void scrollListCellScrollToPos(ZF_IN zfint pos,
                                   ZF_IN zfbool animated)
    {
        this->scrollListCellDesiredPosSaved = pos;
        this->pimplOwner->scrollOverride(zftrue);
        const ZFUIRect &scrollContentFrame = this->pimplOwner->scrollContentFrame();
        zfint posX = 0;
        zfint posY = 0;
        switch(this->pimplOwner->listOrientation())
        {
            case ZFUIOrientation::e_Left:
                posX = pos;
                posY = scrollContentFrame.y;
                break;
            case ZFUIOrientation::e_Top:
                posX = scrollContentFrame.x;
                posY = pos;
                break;
            case ZFUIOrientation::e_Right:
                posX = this->pimplOwner->scrollArea().width - pos - scrollContentFrame.width;
                posY = scrollContentFrame.y;
                break;
            case ZFUIOrientation::e_Bottom:
                posX = scrollContentFrame.x;
                posY = this->pimplOwner->scrollArea().height - pos - scrollContentFrame.height;
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                break;
        }
        if(animated)
        {
            this->pimplOwner->scrollByPoint(posX, posY);
        }
        else
        {
            this->pimplOwner->scrollContentFrame(ZFUIRectMake(posX, posY, scrollContentFrame.width, scrollContentFrame.height));
        }
        this->pimplOwner->scrollOverride(zffalse);
    }
};

// ============================================================
// ZFUIListView
ZFOBJECT_REGISTER(ZFUIListView)

ZFOBSERVER_EVENT_REGISTER(ZFUIListView, ListCellOnAttach)
ZFOBSERVER_EVENT_REGISTER(ZFUIListView, ListCellOnDetach)
ZFOBSERVER_EVENT_REGISTER(ZFUIListView, ListVisibleCellOnChange)

#define _ZFP_ZFUIListView_listAdapterHolderTag "_ZFP_ZFUIListView_listAdapterHolderTag"
ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUIListView, ZFUIListAdapter *, listAdapter)
{
    d->listAdapter = this->listAdapter();
    d->listAdapterAfterAttach();

    if(this->listAdapter() != propertyValueOld)
    {
        this->listReload();
    }
}
ZFPROPERTY_OVERRIDE_ON_DETACH_DEFINE(ZFUIListView, ZFUIListAdapter *, listAdapter)
{
    d->listAdapterBeforeDetach();
    d->listAdapter = zfnull;
    this->objectTagRemove(_ZFP_ZFUIListView_listAdapterHolderTag);
}
ZFMETHOD_DEFINE_1(ZFUIListView, void, listAdapterAutoRetain,
                  ZFMP_IN(ZFUIListAdapter *, listAdapter))
{
    zfRetain(listAdapter);
    this->listAdapter(listAdapter);
    this->toObject()->objectTag(_ZFP_ZFUIListView_listAdapterHolderTag, this->listAdapter()->toObject());
    zfRelease(listAdapter);
}

ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUIListView, ZFUIOrientationEnum, listOrientation)
{
    if(this->listOrientation() != propertyValueOld)
    {
        d->listBounceUpdate();
        d->listReloadByChangeListOrientation = zftrue;
        this->listReload();
    }
}
ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUIListView, zfbool, listBounce)
{
    if(this->listBounce() != propertyValueOld)
    {
        d->listBounceUpdate();
        if(!this->listBounce())
        {
            this->scrollBounceVerticalAlways(zffalse);
            this->scrollBounceHorizontalAlways(zffalse);
        }
    }
}

// ============================================================
void ZFUIListView::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFUIListViewPrivate);
    d->pimplOwner = this;
    d->listBounceUpdate();

    ZFLISTENER_LOCAL(layoutOnLayoutRequest, {
            _ZFP_ZFUIListViewPrivate *d = userData->to<ZFTypeHolder *>()->holdedDataPointer<_ZFP_ZFUIListViewPrivate *>();
            d->cellNeedUpdate = zftrue;
        })
    this->observerAdd(
        ZFUIView::EventViewLayoutOnLayoutRequest(),
        layoutOnLayoutRequest,
        zflineAlloc(ZFTypeHolder, d, ZFTypeHolderTypePointerRef));
}
void ZFUIListView::objectOnDealloc(void)
{
    zfpoolDelete(d);
    zfsuper::objectOnDealloc();
}
void ZFUIListView::objectOnDeallocPrepare(void)
{
    d->childAddOverrideFlag = zftrue;
    this->listAdapter(zfnull);
    zfsuper::objectOnDeallocPrepare();
    d->childAddOverrideFlag = zffalse;
}

ZFSerializablePropertyType ZFUIListView::serializableOnCheckPropertyType(ZF_IN const ZFProperty *property)
{
    if(property == ZFPropertyAccess(ZFUIScrollView, scrollContentFrame)
        || property == ZFPropertyAccess(ZFUIScrollView, scrollBounceVertical)
        || property == ZFPropertyAccess(ZFUIScrollView, scrollBounceHorizontal)
        || property == ZFPropertyAccess(ZFUIScrollView, scrollBounceVerticalAlways)
        || property == ZFPropertyAccess(ZFUIScrollView, scrollBounceHorizontalAlways)
        )
    {
        return ZFSerializablePropertyTypeNotSerializable;
    }
    else
    {
        return zfsuperI(ZFSerializable)::serializableOnCheckPropertyType(property);
    }
}

zfbool ZFUIListView::serializableOnSerializeFromData(ZF_IN const ZFSerializableData &serializableData,
                                                     ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */,
                                                     ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */)
{
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeFromData(serializableData, outErrorHint, outErrorPos)) {return zffalse;}

    this->listAdapter(zfnull);

    for(zfindex i = 0; i < serializableData.elementCount(); ++i)
    {
        const ZFSerializableData &categoryData = serializableData.elementAtIndex(i);
        if(categoryData.resolved()) {continue;}
        const zfchar *category = ZFSerializableUtil::checkCategory(categoryData);
        if(!zfscmpTheSame(category, ZFSerializableKeyword_ZFUIListView_listAdapter)) {continue;}

        zfautoObject element;
        if(!ZFObjectFromData(element, categoryData, outErrorHint, outErrorPos))
        {
            return zffalse;
        }
        if(element != zfnull
            && !element.toObject()->classData()->classIsTypeOf(ZFUIListAdapter::ClassData()))
        {
            ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, categoryData,
                "%s not type of %s",
                element.toObject()->objectInfoOfInstance().cString(), ZFUIListAdapter::ClassData()->classNameFull());
            return zffalse;
        }

        this->listAdapter(element);
        categoryData.resolveMark();
    }
    return zftrue;
}
zfbool ZFUIListView::serializableOnSerializeToData(ZF_IN_OUT ZFSerializableData &serializableData,
                                                   ZF_IN ZFSerializable *referencedOwnerOrNull,
                                                   ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */)
{
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeToData(serializableData, referencedOwnerOrNull, outErrorHint)) {return zffalse;}
    zfself *ref = ZFCastZFObject(zfself *, referencedOwnerOrNull);

    if(!this->listAdapterSerializable() || this->listAdapter() == zfnull)
    {
        return zftrue;
    }

    ZFSerializableData categoryData;
    if(!ZFObjectToData(categoryData, this->listAdapter()->toObject(), outErrorHint,
        ref ? ZFCastZFObject(ZFSerializable *, ref->listAdapter()) : zfnull))
    {
        return zffalse;
    }

    if(!(
        ref != zfnull && ref->listAdapter() != zfnull
        && this->listAdapter()->classData()->classIsTypeOf(ref->listAdapter()->classData())
        && categoryData.elementCount() == 0 && categoryData.attributeCount() == 0
        ))
    {
        categoryData.category(ZFSerializableKeyword_ZFUIListView_listAdapter);
        serializableData.elementAdd(categoryData);
    }

    return zftrue;
}

void ZFUIListView::layoutOnLayoutPrepare(ZF_IN const ZFUIRect &bounds)
{
    if(!d->listQuickReloadRequested && (
        this->viewFrame().width != this->viewFramePrev().width
        || this->viewFrame().height != this->viewFramePrev().height
    )) {
        d->listQuickReloadRequested = zftrue;
        d->cellNeedUpdate = zftrue;
    }
    if(d->listReloadRequested || d->listQuickReloadRequested)
    {
        d->listAdaptertingUpdate();
    }
    zfsuper::layoutOnLayoutPrepare(bounds);
    d->listCheckReload();
}
void ZFUIListView::layoutOnLayout(ZF_IN const ZFUIRect &bounds)
{
    d->updateCellLayout();
}

void ZFUIListView::viewChildOnAdd(ZF_IN ZFUIView *child,
                                  ZF_IN ZFUIViewChildLayerEnum layer)
{
    if(layer == ZFUIViewChildLayer::e_Normal)
    {
        zfCoreAssertWithMessage(d->childAddOverrideFlag || layer != ZFUIViewChildLayer::e_Normal, "you must not add child to a list view");
    }
    zfsuper::viewChildOnAdd(child, layer);
}
void ZFUIListView::viewChildOnRemove(ZF_IN ZFUIView *child,
                                     ZF_IN ZFUIViewChildLayerEnum layer)
{
    if(layer == ZFUIViewChildLayer::e_Normal)
    {
        zfCoreAssertWithMessage(d->childAddOverrideFlag || layer != ZFUIViewChildLayer::e_Normal, "you must not remove child from a list view");
    }
    zfsuper::viewChildOnRemove(child, layer);
}

void ZFUIListView::scrollAreaOnChange(void)
{
    zfsuper::scrollAreaOnChange();
    if(!d->listQuickReloadRequested)
    {
        d->listQuickReloadRequested = zftrue;
    }
    d->listAdaptertingUpdate();
}
void ZFUIListView::scrollContentFrameOnChange(void)
{
    zfsuper::scrollContentFrameOnChange();
    if(d->scrollContentFrameOverrideFlag)
    {
        return ;
    }

    d->listCheckReload();

    if(d->listAdapter == zfnull)
    {
        return ;
    }

    if(!d->listVisibleCell.isEmpty() && !d->listVisibleCellOffsetNeedUpdate)
    {
        zfindex cellIndex = d->listVisibleCellIndexRange.start + d->listVisibleCellIndexRange.count - 1;
        ZFUIRect cellFrame = d->listVisibleCellFrame(cellIndex);
        d->updateHeadCellBeforeIndex(cellIndex, cellFrame);
        if(!d->listVisibleCell.isEmpty())
        {
            cellIndex = d->listVisibleCellIndexRange.start;
            cellFrame = d->listVisibleCellFrame(cellIndex);
            d->updateTailCellAfterIndex(cellIndex, cellFrame);
        }
        else
        {
            d->updateFromFirstCell();
        }
    }
    else
    {
        d->updateFromFirstCell();
    }

    if(d->cellNeedUpdate)
    {
        this->listVisibleCellOnChange();
    }
}
void ZFUIListView::scrollOnScrolledByUser(void)
{
    zfsuper::scrollOnScrolledByUser();

    // cancel scrollListCellToHead/Tail task
    d->scrollListCellIndex = zfindexMax();
}
void ZFUIListView::scrollOnScrollEnd(void)
{
    zfsuper::scrollOnScrollEnd();

    // cancel scrollListCellToHead/Tail task
    d->scrollListCellIndex = zfindexMax();
}

// ============================================================
ZFMETHOD_DEFINE_0(ZFUIListView, void, listReload)
{
    if(!d->listReloadRequested)
    {
        d->listReloadRequested = zftrue;
        d->listQuickReloadRequested = zftrue;
        d->cellNeedUpdate = zftrue;
        d->listVisibleCellOffsetNeedUpdate = zftrue;
        this->layoutRequest();
    }
}
ZFMETHOD_DEFINE_0(ZFUIListView, zfbool, listReloadRequested)
{
    return d->listReloadRequested;
}
ZFMETHOD_DEFINE_1(ZFUIListView, void, listReloadCellAtIndex,
                  ZFMP_IN(zfindex, index))
{
    if(d->listReloadRequested || !ZFIndexRangeContain(d->listVisibleCellIndexRange, index))
    {
        return ;
    }

    d->cellNeedUpdate = zftrue;

    zfindex indexOfVisibleCell = index - d->listVisibleCellIndexRange.start;
    ZFUIListCell *cellOld = d->listVisibleCell[indexOfVisibleCell];
    ZFUIRect cellOldFrame = d->listVisibleCellFrame(index);

    ZFUIListCell *cellNew = zfRetain(d->cellLoadAtIndex(index).to<ZFUIListCell *>());
    d->listVisibleCell[indexOfVisibleCell] = cellNew;

    this->childReplaceAtIndex(indexOfVisibleCell, cellNew);

    d->cellOnRecycle(cellOld);
    this->cellOnDetach(cellOld);
    this->cellOnAttach(cellNew);
    d->cellOnUpdate(index, cellNew);

    // update cell size at index
    zfint cellNewSize = d->cellSizeAtIndex(index, cellNew);
    d->cellSizeList[index] = cellNewSize;

    // update all cells after the reloaded one
    switch(this->listOrientation())
    {
        case ZFUIOrientation::e_Left:
        case ZFUIOrientation::e_Right:
            cellOldFrame.width = cellNewSize;
            break;
        case ZFUIOrientation::e_Top:
        case ZFUIOrientation::e_Bottom:
            cellOldFrame.height = cellNewSize;
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            return ;
    }
    d->updateTailCellAfterIndex(index, cellOldFrame);

    // finally notify visible cell changed
    this->listVisibleCellOnChange();

    zfRelease(cellOld);
}

ZFMETHOD_DEFINE_0(ZFUIListView, ZFCoreArrayPOD<ZFUIListCell *>, listVisibleCell)
{
    return d->listVisibleCell;
}
ZFMETHOD_DEFINE_0(ZFUIListView, const ZFIndexRange &, listVisibleCellIndexRange)
{
    return d->listVisibleCellIndexRange;
}

ZFMETHOD_DEFINE_3(ZFUIListView, void, scrollListCellToHead,
                  ZFMP_IN(zfindex, cellIndex),
                  ZFMP_IN_OPT(zfint, offset, 0),
                  ZFMP_IN_OPT(zfbool, animated, zftrue))
{
    if(cellIndex >= d->cellCount)
    {
        d->scrollListCellIndex = zfindexMax();
    }
    else
    {
        d->scrollListCellIndex = cellIndex;
    }
    d->scrollListCellOffset = offset;
    d->scrollListCellAnimated = animated;
    d->scrollListCellToHead = zftrue;
    d->scrollListCellDesiredPosSaved = 30000;
    d->scrollListCellCheckUpdate();
}
ZFMETHOD_DEFINE_3(ZFUIListView, void, scrollListCellToTail,
                  ZFMP_IN(zfindex, cellIndex),
                  ZFMP_IN_OPT(zfint, offset, 0),
                  ZFMP_IN_OPT(zfbool, animated, zftrue))
{
    if(cellIndex >= d->cellCount)
    {
        d->scrollListCellIndex = zfindexMax();
    }
    else
    {
        d->scrollListCellIndex = cellIndex;
    }
    d->scrollListCellOffset = offset;
    d->scrollListCellAnimated = animated;
    d->scrollListCellToHead = zffalse;
    d->scrollListCellDesiredPosSaved = 30000;
    d->scrollListCellCheckUpdate();
}

void ZFUIListView::listVisibleCellOnChange(void)
{
    this->observerNotify(ZFUIListView::EventListVisibleCellOnChange());

    if(!this->scrollOverride())
    {
        d->scrollListCellCheckUpdate();
    }
}

ZF_NAMESPACE_GLOBAL_END

