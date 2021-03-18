#include "ZFUILinearLayout.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFSTYLE_DEFAULT_DEFINE(ZFUILinearLayout)

// ============================================================
// ZFUILinearLayoutParam
ZFOBJECT_REGISTER(ZFUILinearLayoutParam)

// ============================================================
// ZFUILinearLayout
ZFOBJECT_REGISTER(ZFUILinearLayout)

ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUILinearLayout, ZFUIOrientationEnum, layoutOrientation)
{
    if(this->layoutOrientation() != propertyValueOld)
    {
        this->layoutRequest();
    }
}
ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUILinearLayout, ZFUIMargin, layoutChildMargin)
{
    if(this->layoutChildMargin() != propertyValueOld)
    {
        this->layoutRequest();
    }
}
ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUILinearLayout, zfint, layoutChildSpace)
{
    if(this->layoutChildSpace() != propertyValueOld)
    {
        this->layoutRequest();
    }
}

// ============================================================
// override ZFUIView
static ZFUISize _ZFP_ZFUILinearLayout_measureHorizontal(ZF_IN ZFUILinearLayout *parent,
                                                        ZF_IN const ZFUISize &sizeHint,
                                                        ZF_OUT_OPT zfint *fixedSize = zfnull,
                                                        ZF_OUT_OPT zfint *totalWeight = zfnull);
static ZFUISize _ZFP_ZFUILinearLayout_measureVertical(ZF_IN ZFUILinearLayout *parent,
                                                      ZF_IN const ZFUISize &sizeHint,
                                                      ZF_OUT_OPT zfint *fixedSize = zfnull,
                                                      ZF_OUT_OPT zfint *totalWeight = zfnull);
void ZFUILinearLayout::layoutOnMeasure(ZF_OUT ZFUISize &ret,
                                       ZF_IN const ZFUISize &sizeHint,
                                       ZF_IN const ZFUISizeParam &sizeParam)
{
    switch(this->layoutOrientation())
    {
        case ZFUIOrientation::e_Left:
        case ZFUIOrientation::e_Right:
            ret = _ZFP_ZFUILinearLayout_measureHorizontal(this, sizeHint);
            break;
        case ZFUIOrientation::e_Top:
        case ZFUIOrientation::e_Bottom:
            ret = _ZFP_ZFUILinearLayout_measureVertical(this, sizeHint);
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            return ;
    }
}

static void _ZFP_ZFUILinearLayout_layoutHorizontal(ZF_IN ZFUILinearLayout *parent,
                                                   ZF_IN const ZFUISize &size);
static void _ZFP_ZFUILinearLayout_layoutVertical(ZF_IN ZFUILinearLayout *parent,
                                                 ZF_IN const ZFUISize &size);
void ZFUILinearLayout::layoutOnLayout(ZF_IN const ZFUIRect &bounds)
{
    switch(this->layoutOrientation())
    {
        case ZFUIOrientation::e_Left:
        case ZFUIOrientation::e_Right:
            _ZFP_ZFUILinearLayout_layoutHorizontal(this, ZFUIRectGetSize(bounds));
            break;
        case ZFUIOrientation::e_Top:
        case ZFUIOrientation::e_Bottom:
            _ZFP_ZFUILinearLayout_layoutVertical(this, ZFUIRectGetSize(bounds));
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            return ;
    }
}

// ============================================================
// measure
static ZFUISize _ZFP_ZFUILinearLayout_measureHorizontal(ZF_IN ZFUILinearLayout *parent,
                                                        ZF_IN const ZFUISize &sizeHint,
                                                        ZF_OUT_OPT zfint *fixedSize /* = zfnull */,
                                                        ZF_OUT_OPT zfint *totalWeight /* = zfnull */)
{
    zfint parentMarginX = ZFUIMarginGetWidth(parent->layoutChildMargin());
    zfint parentMarginY = ZFUIMarginGetHeight(parent->layoutChildMargin());
    ZFUISize ret = ZFUISizeMake(parentMarginX, 0);
    if(fixedSize != zfnull)
    {
        *fixedSize = parentMarginX;
    }
    for(zfindex i = 0; i < parent->childCount(); ++i)
    {
        ZFUIView *child = parent->childAtIndex(i);
        ZFUILinearLayoutParam *layoutParam = child->layoutParam()->toAny();
        if(!child->viewVisible() && !layoutParam->layoutReserveSpaceWhenNotVisible())
        {
            continue ;
        }
        zfint prevSpace = (i > 0 ? parent->layoutChildSpace() : 0);
        if(totalWeight != zfnull)
        {
            *totalWeight += layoutParam->layoutWeight();
        }

        zfint marginX = ZFUIMarginGetWidth(layoutParam->layoutMargin());
        zfint marginY = ZFUIMarginGetHeight(layoutParam->layoutMargin());
        child->layoutMeasure(
            ZFUISizeMake(
                ZFUIViewLayoutParam::sizeHintMerge(
                    layoutParam->sizeHint().width,
                    ZFUIViewLayoutParam::sizeHintOffset(sizeHint.width, 0 - parentMarginX - marginX)),
                ZFUIViewLayoutParam::sizeHintMerge(
                    layoutParam->sizeHint().height,
                    ZFUIViewLayoutParam::sizeHintOffset(sizeHint.height, 0 - parentMarginY - marginY))),
            ZFUISizeParamWrapWrap());
        ret.width += prevSpace + child->layoutMeasuredSize().width + marginX;
        ret.height = zfmMax(ret.height, child->layoutMeasuredSize().height + parentMarginY + marginY);
        if(fixedSize != zfnull)
        {
            *fixedSize += prevSpace + marginX;
            if(layoutParam->layoutWeight() == 0)
            {
                *fixedSize += child->layoutMeasuredSize().width;
            }
        }
    }
    return ret;
}
static ZFUISize _ZFP_ZFUILinearLayout_measureVertical(ZF_IN ZFUILinearLayout *parent,
                                                      ZF_IN const ZFUISize &sizeHint,
                                                      ZF_OUT_OPT zfint *fixedSize /* = zfnull */,
                                                      ZF_OUT_OPT zfint *totalWeight /* = zfnull */)
{
    zfint parentMarginX = ZFUIMarginGetWidth(parent->layoutChildMargin());
    zfint parentMarginY = ZFUIMarginGetHeight(parent->layoutChildMargin());
    ZFUISize ret = ZFUISizeMake(0, parentMarginY);
    if(fixedSize != zfnull)
    {
        *fixedSize = parentMarginY;
    }
    for(zfindex i = 0; i < parent->childCount(); ++i)
    {
        ZFUIView *child = parent->childAtIndex(i);
        ZFUILinearLayoutParam *layoutParam = child->layoutParam()->toAny();
        if(!child->viewVisible() && !layoutParam->layoutReserveSpaceWhenNotVisible())
        {
            continue ;
        }
        zfint prevSpace = (i > 0 ? parent->layoutChildSpace() : 0);
        if(totalWeight != zfnull)
        {
            *totalWeight += layoutParam->layoutWeight();
        }

        zfint marginX = ZFUIMarginGetWidth(layoutParam->layoutMargin());
        zfint marginY = ZFUIMarginGetHeight(layoutParam->layoutMargin());
        child->layoutMeasure(
            ZFUISizeMake(
                ZFUIViewLayoutParam::sizeHintMerge(
                    layoutParam->sizeHint().width,
                    ZFUIViewLayoutParam::sizeHintOffset(sizeHint.width, 0 - parentMarginX - marginX)),
                ZFUIViewLayoutParam::sizeHintMerge(
                    layoutParam->sizeHint().height,
                    ZFUIViewLayoutParam::sizeHintOffset(sizeHint.height, 0 - parentMarginY - marginY))),
            ZFUISizeParamWrapWrap());
        ret.width = zfmMax(ret.width, child->layoutMeasuredSize().width + parentMarginX + marginX);
        ret.height += prevSpace + child->layoutMeasuredSize().height + marginY;
        if(fixedSize != zfnull)
        {
            *fixedSize += prevSpace + marginY;
            if(layoutParam->layoutWeight() == 0)
            {
                *fixedSize += child->layoutMeasuredSize().height;
            }
        }
    }
    return ret;
}

// ============================================================
// layout
static void _ZFP_ZFUILinearLayout_layoutHorizontal(ZF_IN ZFUILinearLayout *parent,
                                                   ZF_IN const ZFUISize &size)
{
    zfint fixedSize = 0;
    zfint totalWeight = 0;
    _ZFP_ZFUILinearLayout_measureHorizontal(
        parent, size, &fixedSize, &totalWeight);
    zfint flexibleSize = zfmMax(0, size.width - fixedSize);
    zfbool positiveDirection = (parent->layoutOrientation() == ZFUIOrientation::e_Left);
    zfint usedSize = (positiveDirection ? parent->layoutChildMargin().left : parent->layoutChildMargin().right);
    for(zfindex i = 0; i < parent->childCount(); ++i)
    {
        ZFUIView *child = parent->childAtIndex(i);
        ZFUILinearLayoutParam *layoutParam = child->layoutParam()->toAny();
        if(!child->viewVisible() && !layoutParam->layoutReserveSpaceWhenNotVisible())
        {
            continue ;
        }
        zfint prevSpace = (i > 0 ? parent->layoutChildSpace() : 0);
        zfint childSize = 0;
        if(layoutParam->layoutWeight() > 0)
        {
            childSize = layoutParam->layoutWeight() * flexibleSize / totalWeight;
        }
        else if(layoutParam->sizeParam().width == ZFUISizeType::e_Fill && totalWeight == 0)
        {
            childSize = flexibleSize + child->layoutMeasuredSize().width;
            flexibleSize = 0;
        }
        else
        {
            childSize = child->layoutMeasuredSize().width + ZFUIMarginGetWidth(layoutParam->layoutMargin());
        }
        child->viewFrame(ZFUIAlignApply(
            layoutParam->layoutAlign(),
            ZFUIRectMake(
                positiveDirection
                    ? usedSize + prevSpace
                    : size.width - usedSize - prevSpace - childSize,
                parent->layoutChildMargin().top,
                childSize,
                size.height - ZFUIMarginGetHeight(parent->layoutChildMargin())),
            ZFUISizeMake(
                childSize - ZFUIMarginGetWidth(layoutParam->layoutMargin()),
                layoutParam->sizeParam().height == ZFUISizeType::e_Fill
                    ? size.height - ZFUIMarginGetHeight(parent->layoutChildMargin())
                    : child->layoutMeasuredSize().height),
            layoutParam->layoutMargin()));
        usedSize += prevSpace + childSize;
    }
}
static void _ZFP_ZFUILinearLayout_layoutVertical(ZF_IN ZFUILinearLayout *parent,
                                                 ZF_IN const ZFUISize &size)
{
    zfint fixedSize = 0;
    zfint totalWeight = 0;
    _ZFP_ZFUILinearLayout_measureVertical(
        parent, size, &fixedSize, &totalWeight);
    zfint flexibleSize = zfmMax(0, size.height - fixedSize);
    zfbool positiveDirection = (parent->layoutOrientation() == ZFUIOrientation::e_Top);
    zfint usedSize = (positiveDirection ? parent->layoutChildMargin().top : parent->layoutChildMargin().bottom);
    for(zfindex i = 0; i < parent->childCount(); ++i)
    {
        ZFUIView *child = parent->childAtIndex(i);
        ZFUILinearLayoutParam *layoutParam = child->layoutParam()->toAny();
        if(!child->viewVisible() && !layoutParam->layoutReserveSpaceWhenNotVisible())
        {
            continue ;
        }
        zfint prevSpace = (i > 0 ? parent->layoutChildSpace() : 0);
        zfint childSize = 0;
        if(layoutParam->layoutWeight() > 0)
        {
            childSize = layoutParam->layoutWeight() * flexibleSize / totalWeight;
        }
        else if(layoutParam->sizeParam().height == ZFUISizeType::e_Fill && totalWeight == 0)
        {
            childSize = flexibleSize + child->layoutMeasuredSize().height;
            flexibleSize = 0;
        }
        else
        {
            childSize = child->layoutMeasuredSize().height + ZFUIMarginGetHeight(layoutParam->layoutMargin());
        }
        child->viewFrame(ZFUIAlignApply(
            layoutParam->layoutAlign(),
            ZFUIRectMake(
                parent->layoutChildMargin().left,
                positiveDirection
                    ? usedSize + prevSpace
                    : size.height - usedSize - prevSpace - childSize,
                size.width - ZFUIMarginGetWidth(parent->layoutChildMargin()),
                childSize),
            ZFUISizeMake(
                layoutParam->sizeParam().width == ZFUISizeType::e_Fill
                    ? size.width - ZFUIMarginGetWidth(parent->layoutChildMargin())
                    : child->layoutMeasuredSize().width,
                childSize - ZFUIMarginGetHeight(layoutParam->layoutMargin())),
            layoutParam->layoutMargin()));
        usedSize += prevSpace + childSize;
    }
}

ZF_NAMESPACE_GLOBAL_END

