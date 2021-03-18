#include "ZFUIListCellView.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFOBJECT_REGISTER(ZFUIListCellView)

void ZFUIListCellView::objectOnInitFinish(void)
{
    zfsuper::objectOnInitFinish();

    zfint itemMargin = ZFUIGlobalStyle::DefaultStyle()->itemMargin();

    this->internalBgViewAdd(this->cellIconContainer());
    this->cellIconContainer()->layoutParam()->layoutAlign(ZFUIAlign::e_Center);
    this->cellIconContainer()->layoutParam()->layoutMargin(ZFUIMarginMake(itemMargin, itemMargin, 0, itemMargin));

    this->internalBgViewAdd(this->cellCenterContainer());
    this->cellCenterContainer()->layoutParam()->layoutAlign(ZFUIAlign::e_Center);
    this->cellCenterContainer()->layoutParam()->layoutMargin(ZFUIMarginMake(itemMargin));

    this->internalBgViewAdd(this->cellAccessoryContainer());
    this->cellAccessoryContainer()->layoutParam()->layoutAlign(ZFUIAlign::e_Center);
    this->cellAccessoryContainer()->layoutParam()->layoutMargin(ZFUIMarginMake(0, itemMargin, itemMargin, itemMargin));
}

static void _ZFP_ZFUIListCellView_measureContent(ZF_IN ZFUIListCellView *view,
                                                 ZF_IN const ZFUISize &sizeHint,
                                                 ZF_OUT ZFUISize &cellIconSize,
                                                 ZF_OUT ZFUISize &cellCenterSize,
                                                 ZF_OUT ZFUISize &cellAccessorySize)
{
    view->cellIconContainer()->layoutMeasure(sizeHint, ZFUISizeParamWrapWrap());
    cellIconSize = view->cellIconContainer()->layoutMeasuredSize();
    if(cellIconSize.width > 0)
    {
        cellIconSize = ZFUISizeApplyMarginReversely(cellIconSize, view->cellIconContainer()->layoutParam()->layoutMargin());
    }

    view->cellAccessoryContainer()->layoutMeasure(sizeHint, ZFUISizeParamWrapWrap());
    cellAccessorySize = view->cellAccessoryContainer()->layoutMeasuredSize();
    if(cellAccessorySize.width > 0)
    {
        cellAccessorySize = ZFUISizeApplyMarginReversely(cellAccessorySize, view->cellAccessoryContainer()->layoutParam()->layoutMargin());
    }

    ZFUISize cellCenterSizeHint = sizeHint;
    if(cellCenterSizeHint.width >= 0)
    {
        cellCenterSizeHint.width -= cellIconSize.width + cellAccessorySize.width;
        cellCenterSizeHint.width -= ZFUIMarginGetWidth(view->cellCenterContainer()->layoutParam()->layoutMargin());
        if(cellCenterSizeHint.width < 0)
        {
            cellCenterSizeHint.width = 0;
        }
    }
    view->cellCenterContainer()->layoutMeasure(cellCenterSizeHint, ZFUISizeParamFillWrap());
    cellCenterSize = view->cellCenterContainer()->layoutMeasuredSize();
    cellCenterSize.width += ZFUIMarginGetWidth(view->cellCenterContainer()->layoutParam()->layoutMargin());
}
void ZFUIListCellView::layoutOnMeasure(ZF_OUT ZFUISize &ret,
                                       ZF_IN const ZFUISize &sizeHint,
                                       ZF_IN const ZFUISizeParam &sizeParam)
{
    ZFUISize cellIconSize = ZFUISizeZero();
    ZFUISize cellCenterSize = ZFUISizeZero();
    ZFUISize cellAccessorySize = ZFUISizeZero();
    _ZFP_ZFUIListCellView_measureContent(this, sizeHint, cellIconSize, cellCenterSize, cellAccessorySize);
    ret.width = cellIconSize.width + cellCenterSize.width + cellAccessorySize.width;
    ret.height = zfmMax(zfmMax(cellIconSize.height, cellCenterSize.height), cellAccessorySize.height);
}
void ZFUIListCellView::internalBgViewOnLayout(ZF_IN const ZFUIRect &bounds)
{
    zfsuper::internalBgViewOnLayout(bounds);

    ZFUISize cellIconSize = ZFUISizeZero();
    ZFUISize cellCenterSize = ZFUISizeZero();
    ZFUISize cellAccessorySize = ZFUISizeZero();
    _ZFP_ZFUIListCellView_measureContent(this, ZFUIRectGetSize(bounds), cellIconSize, cellCenterSize, cellAccessorySize);

    this->cellIconContainer()->viewFrame(ZFUIAlignApply(
        ZFUIAlign::e_LeftInner | ZFUIAlignGetY(this->cellIconContainer()->layoutParam()->layoutAlign()),
        bounds,
        this->cellIconContainer()->layoutMeasuredSize(),
        this->cellIconContainer()->layoutParam()->layoutMargin()));

    ZFUIMargin cellCenterMargin = this->cellCenterContainer()->layoutParam()->layoutMargin();
    cellCenterMargin.left += cellIconSize.width;
    cellCenterMargin.right += cellAccessorySize.width;
    this->cellCenterContainer()->viewFrame(ZFUIAlignApply(
        ZFUIAlign::e_LeftInner | ZFUIAlignGetY(this->cellCenterContainer()->layoutParam()->layoutAlign()),
        bounds,
        this->cellCenterContainer()->layoutMeasuredSize(),
        cellCenterMargin));

    this->cellAccessoryContainer()->viewFrame(ZFUIAlignApply(
        ZFUIAlign::e_RightInner | ZFUIAlignGetY(this->cellAccessoryContainer()->layoutParam()->layoutAlign()),
        bounds,
        this->cellAccessoryContainer()->layoutMeasuredSize(),
        this->cellAccessoryContainer()->layoutParam()->layoutMargin()));
}
zfbool ZFUIListCellView::internalViewShouldLayout(ZF_IN ZFUIView *internalView)
{
    if(internalView == this->cellIconContainer()
        || internalView == this->cellCenterContainer()
        || internalView == this->cellAccessoryContainer())
    {
        return zffalse;
    }
    else
    {
        return zfsuper::internalViewShouldLayout(internalView);
    }
}

ZF_NAMESPACE_GLOBAL_END

