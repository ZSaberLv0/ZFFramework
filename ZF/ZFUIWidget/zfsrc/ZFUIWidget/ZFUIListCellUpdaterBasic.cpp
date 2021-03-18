#include "ZFUIListCellUpdaterBasic.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFUIListCellUpdaterBasic)

#define _ZFP_ZFUIListCellUpdaterBasic_cacheKey_separator "_ZFP_ZFUIListCellUpdaterBasic_cacheKey_separator"
#define _ZFP_ZFUIListCellUpdaterBasic_tag_separator "_ZFP_ZFUIListCellUpdaterBasic_tag_separator"
#define _ZFP_ZFUIListCellUpdaterBasic_tag_separator_head "_ZFP_ZFUIListCellUpdaterBasic_tag_separator_head"
static ZFUIView *_ZFP_ZFUIListCellUpdaterBasic_setupSeparator(ZF_IN ZFUIListCellUpdaterBasic *owner,
                                                              ZF_IN const ZFUIListCellUpdaterParam &updateParam)
{
    zfautoObject separatorViewHolder = owner->itemCacheAccess(_ZFP_ZFUIListCellUpdaterBasic_cacheKey_separator);
    ZFUIView *separatorView = separatorViewHolder.to<ZFUIView *>();
    if(separatorView == zfnull)
    {
        separatorView = zfAlloc(ZFUIView);
        separatorViewHolder = separatorView;
        zfRelease(separatorView);

        separatorView->layoutParam(zflineAlloc(ZFUIViewLayoutParam));
        separatorView->layoutParam()->sizeParam(ZFUISizeParamFillFill());

        separatorView->viewUIEnableTree(zffalse);
    }

    updateParam.cell->internalFgViewAdd(separatorView);
    separatorView->viewBackgroundColor(owner->separatorColor());
    switch(updateParam.listOrientation)
    {
        case ZFUIOrientation::e_Left:
        case ZFUIOrientation::e_Right:
            separatorView->layoutParam()->sizeHint(ZFUISizeMake(owner->separatorSize(), -1));
            separatorView->layoutParam()->layoutMargin(ZFUIMarginMake(0, owner->separatorMargin().top, 0, owner->separatorMargin().bottom));
            break;
        case ZFUIOrientation::e_Top:
        case ZFUIOrientation::e_Bottom:
            separatorView->layoutParam()->sizeHint(ZFUISizeMake(-1, owner->separatorSize()));
            separatorView->layoutParam()->layoutMargin(ZFUIMarginMake(owner->separatorMargin().left, 0, owner->separatorMargin().right, 0));
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            return zfnull;
    }
    return separatorView;
}
void ZFUIListCellUpdaterBasic::cellOnUpdate(ZF_IN const ZFUIListCellUpdaterParam &updateParam)
{
    zfsuperI(ZFUIListCellUpdater)::cellOnUpdate(updateParam);

    if(updateParam.cellIndex == 0)
    { // first cell
        if(this->separatorIncludingHead())
        {
            ZFUIView *separatorView = _ZFP_ZFUIListCellUpdaterBasic_setupSeparator(this, updateParam);
            updateParam.cell->objectTag(_ZFP_ZFUIListCellUpdaterBasic_tag_separator_head, separatorView);
            switch(updateParam.listOrientation)
            {
                case ZFUIOrientation::e_Left:
                    separatorView->layoutParam()->layoutAlign(ZFUIAlign::e_LeftInner);
                    break;
                case ZFUIOrientation::e_Right:
                    separatorView->layoutParam()->layoutAlign(ZFUIAlign::e_RightInner);
                    break;
                case ZFUIOrientation::e_Top:
                    separatorView->layoutParam()->layoutAlign(ZFUIAlign::e_TopInner);
                    break;
                case ZFUIOrientation::e_Bottom:
                    separatorView->layoutParam()->layoutAlign(ZFUIAlign::e_BottomInner);
                    break;
                default:
                    zfCoreCriticalShouldNotGoHere();
                    return ;
            }
        }
    }
    if(updateParam.cellIndex == updateParam.cellCount - 1)
    { // last cell
        if(!this->separatorIncludingTail())
        {
            return ;
        }
    }

    ZFUIView *separatorView = _ZFP_ZFUIListCellUpdaterBasic_setupSeparator(this, updateParam);
    updateParam.cell->objectTag(_ZFP_ZFUIListCellUpdaterBasic_tag_separator, separatorView);
    switch(updateParam.listOrientation)
    {
        case ZFUIOrientation::e_Left:
            separatorView->layoutParam()->layoutAlign(ZFUIAlign::e_RightInner);
            break;
        case ZFUIOrientation::e_Right:
            separatorView->layoutParam()->layoutAlign(ZFUIAlign::e_LeftInner);
            break;
        case ZFUIOrientation::e_Top:
            separatorView->layoutParam()->layoutAlign(ZFUIAlign::e_BottomInner);
            break;
        case ZFUIOrientation::e_Bottom:
            separatorView->layoutParam()->layoutAlign(ZFUIAlign::e_TopInner);
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            return ;
    }
}
void ZFUIListCellUpdaterBasic::cellOnRecycle(ZF_IN ZFUIListCell *cell)
{
    zfsuperI(ZFUIListCellUpdater)::cellOnRecycle(cell);

    ZFObject *separatorHead = cell->objectTag(_ZFP_ZFUIListCellUpdaterBasic_tag_separator_head);
    if(separatorHead != zfnull)
    {
        this->itemCacheRecycle(_ZFP_ZFUIListCellUpdaterBasic_cacheKey_separator, separatorHead);
        cell->internalFgViewRemove(separatorHead->to<ZFUIView *>());
        cell->objectTagRemove(_ZFP_ZFUIListCellUpdaterBasic_tag_separator_head);
    }
    ZFObject *separatorTail = cell->objectTag(_ZFP_ZFUIListCellUpdaterBasic_tag_separator);
    if(separatorTail != zfnull)
    {
        this->itemCacheRecycle(_ZFP_ZFUIListCellUpdaterBasic_cacheKey_separator, separatorTail);
        cell->internalFgViewRemove(separatorTail->to<ZFUIView *>());
        cell->objectTagRemove(_ZFP_ZFUIListCellUpdaterBasic_tag_separator);
    }
}

ZF_NAMESPACE_GLOBAL_END

