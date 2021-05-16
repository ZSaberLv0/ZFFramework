#include "ZFUIFlowLayout.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFSTYLE_DEFAULT_DEFINE(ZFUIFlowLayout)

// ============================================================
// ZFUIFlowLayoutParam
ZFOBJECT_REGISTER(ZFUIFlowLayoutParam)

// ============================================================
// ZFUIFlowLayout
ZFOBJECT_REGISTER(ZFUIFlowLayout)

ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIFlowLayout, ZFUIOrientationEnum, layoutOrientationMain)
{
    if(this->layoutOrientationMain() != propertyValueOld)
    {
        this->layoutRequest();
    }
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIFlowLayout, ZFUIOrientationEnum, layoutOrientationSecondary)
{
    if(this->layoutOrientationSecondary() != propertyValueOld)
    {
        this->layoutRequest();
    }
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIFlowLayout, ZFUIMargin, layoutChildMargin)
{
    if(this->layoutChildMargin() != propertyValueOld)
    {
        this->layoutRequest();
    }
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIFlowLayout, zffloat, layoutChildSpaceX)
{
    if(this->layoutChildSpaceX() != propertyValueOld)
    {
        this->layoutRequest();
    }
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIFlowLayout, zffloat, layoutChildSpaceY)
{
    if(this->layoutChildSpaceY() != propertyValueOld)
    {
        this->layoutRequest();
    }
}

// ============================================================
// override ZFUIView
static ZFUISize _ZFP_ZFUIFlowLayout_measureHorizontal(ZF_IN ZFUIFlowLayout *parent,
                                                      ZF_IN const ZFUISize &sizeHint);
static ZFUISize _ZFP_ZFUIFlowLayout_measureVertical(ZF_IN ZFUIFlowLayout *parent,
                                                    ZF_IN const ZFUISize &sizeHint);
void ZFUIFlowLayout::layoutOnMeasure(ZF_OUT ZFUISize &ret,
                                     ZF_IN const ZFUISize &sizeHint,
                                     ZF_IN const ZFUISizeParam &sizeParam)
{
    switch(this->layoutOrientationMain())
    {
        case ZFUIOrientation::e_Left:
        case ZFUIOrientation::e_Right:
            ret = _ZFP_ZFUIFlowLayout_measureHorizontal(this, sizeHint);
            break;
        case ZFUIOrientation::e_Top:
        case ZFUIOrientation::e_Bottom:
            ret = _ZFP_ZFUIFlowLayout_measureVertical(this, sizeHint);
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            return ;
    }
}

static void _ZFP_ZFUIFlowLayout_layoutHorizontal(ZF_IN ZFUIFlowLayout *parent,
                                                 ZF_IN const ZFUISize &size);
static void _ZFP_ZFUIFlowLayout_layoutVertical(ZF_IN ZFUIFlowLayout *parent,
                                               ZF_IN const ZFUISize &size);
void ZFUIFlowLayout::layoutOnLayout(ZF_IN const ZFUIRect &bounds)
{
    switch(this->layoutOrientationMain())
    {
        case ZFUIOrientation::e_Left:
        case ZFUIOrientation::e_Right:
            _ZFP_ZFUIFlowLayout_layoutHorizontal(this, ZFUIRectGetSize(bounds));
            break;
        case ZFUIOrientation::e_Top:
        case ZFUIOrientation::e_Bottom:
            _ZFP_ZFUIFlowLayout_layoutVertical(this, ZFUIRectGetSize(bounds));
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            return ;
    }
}

// ============================================================
// util
ZFUIOrientationEnum _ZFP_ZFUIFlowLayout_layoutOrientationSecondary(ZF_IN ZFUIFlowLayout *parent)
{
    switch(parent->layoutOrientationMain())
    {
        case ZFUIOrientation::e_Left:
        case ZFUIOrientation::e_Right:
            switch(parent->layoutOrientationSecondary())
            {
                case ZFUIOrientation::e_Left:
                case ZFUIOrientation::e_Right:
                    return ZFUIOrientation::e_Top;
                case ZFUIOrientation::e_Top:
                case ZFUIOrientation::e_Bottom:
                    return parent->layoutOrientationSecondary();
                default:
                    zfCoreCriticalShouldNotGoHere();
                    return ZFUIOrientation::e_Left;
            }
        case ZFUIOrientation::e_Top:
        case ZFUIOrientation::e_Bottom:
            switch(parent->layoutOrientationSecondary())
            {
                case ZFUIOrientation::e_Left:
                case ZFUIOrientation::e_Right:
                    return parent->layoutOrientationSecondary();
                case ZFUIOrientation::e_Top:
                case ZFUIOrientation::e_Bottom:
                    return ZFUIOrientation::e_Left;
                default:
                    zfCoreCriticalShouldNotGoHere();
                    return ZFUIOrientation::e_Left;
            }
        default:
            zfCoreCriticalShouldNotGoHere();
            return ZFUIOrientation::e_Left;
    }
}

// ============================================================
// measure line
static ZFUISize _ZFP_ZFUIFlowLayout_measureHorizontalLine(ZF_IN ZFUIFlowLayout *parent,
                                                          ZF_IN const ZFUISize &sizeHint,
                                                          ZF_IN zfindex childIndexStart,
                                                          ZF_OUT zfindex &childIndexStop,
                                                          ZF_OUT zffloat &fixedSize,
                                                          ZF_OUT zffloat &totalWeight)
{
    ZFUISize lineSize = ZFUISizeZero();
    childIndexStop = parent->childCount();
    for(zfindex i = childIndexStart; i < parent->childCount(); ++i)
    {
        ZFUIView *child = parent->childAtIndex(i);
        ZFUIFlowLayoutParam *layoutParam = child->layoutParam()->toAny();
        if(!child->viewVisible() && !layoutParam->layoutReserveSpaceWhenNotVisible())
        {
            continue ;
        }
        zffloat prevSpace = (i > childIndexStart ? parent->layoutChildSpaceX() : 0);
        totalWeight += layoutParam->layoutWeight();

        zffloat marginX = ZFUIMarginGetWidth(layoutParam->layoutMargin());
        zffloat marginY = ZFUIMarginGetHeight(layoutParam->layoutMargin());
        child->layoutMeasure(
            ZFUISizeMake(
                ZFUILayoutParam::sizeHintMerge(
                    layoutParam->sizeHint().width,
                    ZFUILayoutParam::sizeHintOffset(sizeHint.width, 0 - marginX)),
                ZFUILayoutParam::sizeHintMerge(
                    layoutParam->sizeHint().height,
                    ZFUILayoutParam::sizeHintOffset(sizeHint.height, 0 - marginY))),
            ZFUISizeParamMake(ZFUISizeType::e_Wrap, layoutParam->sizeParam().height));
        if(sizeHint.width >= 0 && lineSize.width + prevSpace + child->layoutMeasuredSize().width + marginX > sizeHint.width)
        {
            childIndexStop = i;
            break;
        }
        lineSize.width += prevSpace + child->layoutMeasuredSize().width + marginX;
        lineSize.height = zfmMax(lineSize.height, child->layoutMeasuredSize().height + marginY);
        fixedSize += prevSpace + marginX;
        if(layoutParam->layoutWeight() == 0)
        {
            fixedSize += child->layoutMeasuredSize().width;
        }
        if(layoutParam->sizeParam().width == ZFUISizeType::e_Fill
            || (sizeHint.width >= 0 && lineSize.width == sizeHint.width))
        {
            childIndexStop = i + 1;
            break;
        }
    }
    return lineSize;
}
static ZFUISize _ZFP_ZFUIFlowLayout_measureVerticalLine(ZF_IN ZFUIFlowLayout *parent,
                                                        ZF_IN const ZFUISize &sizeHint,
                                                        ZF_IN zfindex childIndexStart,
                                                        ZF_OUT zfindex &childIndexStop,
                                                        ZF_OUT zffloat &fixedSize,
                                                        ZF_OUT zffloat &totalWeight)
{
    ZFUISize lineSize = ZFUISizeZero();
    childIndexStop = parent->childCount();
    for(zfindex i = childIndexStart; i < parent->childCount(); ++i)
    {
        ZFUIView *child = parent->childAtIndex(i);
        ZFUIFlowLayoutParam *layoutParam = child->layoutParam()->toAny();
        if(!child->viewVisible() && !layoutParam->layoutReserveSpaceWhenNotVisible())
        {
            continue ;
        }
        zffloat prevSpace = (i > childIndexStart ? parent->layoutChildSpaceY() : 0);
        totalWeight += layoutParam->layoutWeight();

        zffloat marginX = ZFUIMarginGetWidth(layoutParam->layoutMargin());
        zffloat marginY = ZFUIMarginGetHeight(layoutParam->layoutMargin());
        child->layoutMeasure(
            ZFUISizeMake(
                ZFUILayoutParam::sizeHintMerge(
                    layoutParam->sizeHint().width,
                    ZFUILayoutParam::sizeHintOffset(sizeHint.width, 0 - marginX)),
                ZFUILayoutParam::sizeHintMerge(
                    layoutParam->sizeHint().height,
                    ZFUILayoutParam::sizeHintOffset(sizeHint.height, 0 - marginY))),
            ZFUISizeParamMake(layoutParam->sizeParam().width, ZFUISizeType::e_Wrap));
        if(sizeHint.height >= 0 && lineSize.height + prevSpace + child->layoutMeasuredSize().height + marginY > sizeHint.height)
        {
            childIndexStop = i;
            break;
        }
        lineSize.width = zfmMax(lineSize.width, child->layoutMeasuredSize().width + marginX);
        lineSize.height += prevSpace + child->layoutMeasuredSize().height + marginY;
        fixedSize += prevSpace + marginY;
        if(layoutParam->layoutWeight() == 0)
        {
            fixedSize += child->layoutMeasuredSize().height;
        }
        if(layoutParam->sizeParam().height == ZFUISizeType::e_Fill
            || (sizeHint.height >= 0 && lineSize.height == sizeHint.height))
        {
            childIndexStop = i + 1;
            break;
        }
    }
    return lineSize;
}

// ============================================================
// measure
static ZFUISize _ZFP_ZFUIFlowLayout_measureHorizontal(ZF_IN ZFUIFlowLayout *parent,
                                                      ZF_IN const ZFUISize &sizeHint)
{
    zffloat parentMarginX = ZFUIMarginGetWidth(parent->layoutChildMargin());
    zffloat parentMarginY = ZFUIMarginGetHeight(parent->layoutChildMargin());
    ZFUISize ret = ZFUISizeMake(0, parentMarginY);
    zfindex childIndex = 0;
    ZFUISize sizeHintTmp = ZFUISizeMake(
        ZFUILayoutParam::sizeHintOffset(sizeHint.width, 0 - parentMarginX),
        sizeHint.height);
    for(zfindex lineIndex = 0; childIndex < parent->childCount(); ++lineIndex)
    {
        zffloat fixedSize = 0;
        zffloat totalWeight = 0;
        zffloat prevLineSpace = (lineIndex > 0 ? parent->layoutChildSpaceY() : 0);
        ZFUISize lineSizeHint = ZFUISizeMake(
            sizeHintTmp.width,
            ZFUILayoutParam::sizeHintOffset(sizeHintTmp.height, 0 - prevLineSpace - ret.height));
        ZFUISize lineSize = _ZFP_ZFUIFlowLayout_measureHorizontalLine(
            parent, lineSizeHint, childIndex, childIndex, fixedSize, totalWeight);
        ret.width = zfmMax(ret.width, lineSize.width + parentMarginX);
        ret.height += prevLineSpace + lineSize.height;
    }
    return ret;
}
static ZFUISize _ZFP_ZFUIFlowLayout_measureVertical(ZF_IN ZFUIFlowLayout *parent,
                                                    ZF_IN const ZFUISize &sizeHint)
{
    zffloat parentMarginX = ZFUIMarginGetWidth(parent->layoutChildMargin());
    zffloat parentMarginY = ZFUIMarginGetHeight(parent->layoutChildMargin());
    ZFUISize ret = ZFUISizeMake(parentMarginX, 0);
    zfindex childIndex = 0;
    ZFUISize sizeHintTmp = ZFUISizeMake(
        sizeHint.width,
        ZFUILayoutParam::sizeHintOffset(sizeHint.height, 0 - parentMarginY));
    for(zfindex lineIndex = 0; childIndex < parent->childCount(); ++lineIndex)
    {
        zffloat fixedSize = 0;
        zffloat totalWeight = 0;
        zffloat prevLineSpace = (lineIndex > 0 ? parent->layoutChildSpaceX() : 0);
        ZFUISize lineSizeHint = ZFUISizeMake(
            ZFUILayoutParam::sizeHintOffset(sizeHintTmp.width, 0 - prevLineSpace - ret.width),
            sizeHintTmp.width);
        ZFUISize lineSize = _ZFP_ZFUIFlowLayout_measureVerticalLine(
            parent, lineSizeHint, childIndex, childIndex, fixedSize, totalWeight);
        ret.width += prevLineSpace + lineSize.width;
        ret.height = zfmMax(ret.height, lineSize.height + parentMarginY);
    }
    return ret;
}

// ============================================================
// layout
static void _ZFP_ZFUIFlowLayout_layoutHorizontal(ZF_IN ZFUIFlowLayout *parent,
                                                 ZF_IN const ZFUISize &size)
{
    zffloat parentMarginX = ZFUIMarginGetWidth(parent->layoutChildMargin());
    zfbool positiveDirectionX = (parent->layoutOrientationMain() == ZFUIOrientation::e_Left);
    zfbool positiveDirectionY = (_ZFP_ZFUIFlowLayout_layoutOrientationSecondary(parent) == ZFUIOrientation::e_Top);
    zffloat lineSizeUsed = (positiveDirectionY ? parent->layoutChildMargin().top : parent->layoutChildMargin().bottom);
    zffloat lineTailMargin = (positiveDirectionY ? parent->layoutChildMargin().bottom : parent->layoutChildMargin().top);
    zfindex childIndex = 0;
    for(zfindex lineIndex = 0; childIndex < parent->childCount(); ++lineIndex)
    {
        zffloat fixedSize = 0;
        zffloat totalWeight = 0;
        zffloat prevLineSpace = (lineIndex > 0 ? parent->layoutChildSpaceY() : 0);
        zfindex childIndexStart = childIndex;
        ZFUISize lineSizeHint = ZFUISizeMake(
            size.width - parentMarginX,
            size.height - lineSizeUsed - lineTailMargin);
        ZFUISize lineSize = _ZFP_ZFUIFlowLayout_measureHorizontalLine(
            parent, lineSizeHint, childIndexStart, childIndex, fixedSize, totalWeight);

        zffloat flexibleSize = size.width - parentMarginX - fixedSize;
        zffloat usedSize = (positiveDirectionX ? parent->layoutChildMargin().left : parent->layoutChildMargin().right);
        for(zfindex i = childIndexStart; i < childIndex; ++i)
        {
            ZFUIView *child = parent->childAtIndex(i);
            ZFUIFlowLayoutParam *layoutParam = child->layoutParam()->toAny();
            if(!child->viewVisible() && !layoutParam->layoutReserveSpaceWhenNotVisible())
            {
                continue ;
            }
            zffloat prevSpace = (i > childIndexStart ? parent->layoutChildSpaceX() : 0);
            zffloat childSize = 0;
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
                childSize = child->layoutMeasuredSize().width;
            }
            child->viewFrame(ZFUIAlignApply(
                layoutParam->layoutAlign(),
                ZFUIRectMake(
                    positiveDirectionX
                        ? usedSize + prevSpace
                        : size.width - usedSize - prevSpace - childSize,
                    positiveDirectionY
                        ? lineSizeUsed + prevLineSpace
                        : size.height - lineSizeUsed - prevLineSpace - lineSize.height,
                    childSize,
                    lineSize.height),
                ZFUISizeMake(childSize, child->layoutMeasuredSize().height),
                layoutParam->layoutMargin()));
            usedSize += prevSpace + childSize;
        }

        lineSizeUsed += prevLineSpace + lineSize.height;
    }
}
static void _ZFP_ZFUIFlowLayout_layoutVertical(ZF_IN ZFUIFlowLayout *parent,
                                               ZF_IN const ZFUISize &size)
{
    zffloat parentMarginY = ZFUIMarginGetHeight(parent->layoutChildMargin());
    zfbool positiveDirectionX = (_ZFP_ZFUIFlowLayout_layoutOrientationSecondary(parent) == ZFUIOrientation::e_Left);
    zfbool positiveDirectionY = (parent->layoutOrientationMain() == ZFUIOrientation::e_Top);
    zffloat lineSizeUsed = (positiveDirectionX ? parent->layoutChildMargin().left : parent->layoutChildMargin().right);
    zffloat lineTailMargin = (positiveDirectionX ? parent->layoutChildMargin().right : parent->layoutChildMargin().left);
    zfindex childIndex = 0;
    for(zfindex lineIndex = 0; childIndex < parent->childCount(); ++lineIndex)
    {
        zffloat fixedSize = 0;
        zffloat totalWeight = 0;
        zffloat prevLineSpace = (lineIndex > 0 ? parent->layoutChildSpaceX() : 0);
        zfindex childIndexStart = childIndex;
        ZFUISize lineSizeHint = ZFUISizeMake(
            size.width - lineSizeUsed - lineTailMargin,
            size.height - parentMarginY);
        ZFUISize lineSize = _ZFP_ZFUIFlowLayout_measureVerticalLine(
            parent, lineSizeHint, childIndexStart, childIndex, fixedSize, totalWeight);

        zffloat flexibleSize = size.height - parentMarginY - fixedSize;
        zffloat usedSize = (positiveDirectionY ? parent->layoutChildMargin().top : parent->layoutChildMargin().bottom);
        for(zfindex i = childIndexStart; i < childIndex; ++i)
        {
            ZFUIView *child = parent->childAtIndex(i);
            ZFUIFlowLayoutParam *layoutParam = child->layoutParam()->toAny();
            if(!child->viewVisible() && !layoutParam->layoutReserveSpaceWhenNotVisible())
            {
                continue ;
            }
            zffloat prevSpace = (i > childIndexStart ? parent->layoutChildSpaceY() : 0);
            zffloat childSize = 0;
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
                childSize = child->layoutMeasuredSize().height;
            }
            child->viewFrame(ZFUIAlignApply(
                layoutParam->layoutAlign(),
                ZFUIRectMake(
                    positiveDirectionX
                        ? lineSizeUsed + prevLineSpace
                        : size.width - lineSizeUsed - prevLineSpace - lineSize.width,
                    positiveDirectionY
                        ? usedSize + prevSpace
                        : size.height - usedSize - prevSpace - childSize,
                    lineSize.width,
                    childSize),
                ZFUISizeMake(child->layoutMeasuredSize().width, childSize),
                layoutParam->layoutMargin()));
            usedSize += prevSpace + childSize;
        }

        lineSizeUsed += prevLineSpace + lineSize.width;
    }
}

ZF_NAMESPACE_GLOBAL_END

