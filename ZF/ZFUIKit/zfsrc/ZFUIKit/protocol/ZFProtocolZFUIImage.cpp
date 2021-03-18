#include "ZFProtocolZFUIImage.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_INTERFACE_REGISTER(ZFUIImage)

ZFENUM_DEFINE(ZFUIImageImplNinePatchPos)

ZFOUTPUT_TYPE_DEFINE(ZFUIImageImplNinePatchDrawData, {
    output << zfstringWithFormat("(%s)<%s, %s>",
        ZFUIImageImplNinePatchPos::EnumNameForValue(v.position),
        ZFUIRectToString(v.src).cString(),
        ZFUIRectToString(v.dst).cString()
        );
})

zfindex ZFUIImageImplNinePatchCalc(ZF_OUT ZFUIImageImplNinePatchDrawData *outBuf,
                                   ZF_IN const ZFUISize &srcSize,
                                   ZF_IN const ZFUIMargin &srcNinePatch,
                                   ZF_IN const ZFUISize &dstSize)
{
    zfindex count = 0;

    // calculate drawing areas
    const ZFUIMargin &srcMargin = srcNinePatch;
    ZFUIMargin srcMarginFixed = srcNinePatch;
    ZFUISize srcCenter = ZFUISizeMake(srcSize.width - srcMargin.left - srcMargin.right, srcSize.height - srcMargin.top - srcMargin.bottom);
    if(srcCenter.width == 0)
    {
        srcCenter.width = 1;
    }
    if(srcCenter.height == 0)
    {
        srcCenter.height = 1;
    }

    ZFUIMargin dstMargin = srcNinePatch;
    if(dstMargin.left + dstMargin.right > dstSize.width)
    {
        zffloat scale = (zffloat)dstSize.width / (dstMargin.left + dstMargin.right);
        dstMargin.left = ZFUISizeApplyScale(dstMargin.left, scale);
        dstMargin.right = ZFUISizeApplyScale(dstMargin.right, scale);
        if(dstMargin.left + dstMargin.right > dstSize.width)
        {
            dstMargin.right = dstSize.width - dstMargin.left;
        }
        srcMarginFixed.left = dstMargin.left;
        srcMarginFixed.right = dstMargin.right;
    }
    if(dstMargin.top + dstMargin.bottom > dstSize.height)
    {
        zffloat scale = (zffloat)dstSize.height / (dstMargin.top + dstMargin.bottom);
        dstMargin.top = ZFUISizeApplyScale(dstMargin.top, scale);
        dstMargin.bottom = ZFUISizeApplyScale(dstMargin.top, scale);
        if(dstMargin.top + dstMargin.bottom > dstSize.height)
        {
            dstMargin.bottom = dstSize.height - dstMargin.top;
        }
        srcMarginFixed.top = dstMargin.top;
        srcMarginFixed.bottom = dstMargin.bottom;
    }
    ZFUISize dstCenter = ZFUISizeMake(dstSize.width - dstMargin.left - dstMargin.right, dstSize.height - dstMargin.top - dstMargin.bottom);

    // top
    if(srcMargin.top > 0 && dstMargin.top > 0)
    {
        if(srcMargin.left > 0 && dstMargin.left > 0)
        { // left
            ZFUIImageImplNinePatchDrawData *drawData = outBuf + count; ++count;
            drawData->position = ZFUIImageImplNinePatchPos::e_TopLeft;
            drawData->src = ZFUIRectMake(0, 0, srcMarginFixed.left, srcMarginFixed.top);
            drawData->dst = ZFUIRectMake(0, 0, dstMargin.left, dstMargin.top);
        }
        if(srcCenter.width > 0 && dstCenter.width > 0)
        { // center
            ZFUIImageImplNinePatchDrawData *drawData = outBuf + count; ++count;
            drawData->position = ZFUIImageImplNinePatchPos::e_TopCenter;
            drawData->src = ZFUIRectMake(srcMargin.left, 0, srcCenter.width, srcMarginFixed.top);
            drawData->dst = ZFUIRectMake(dstMargin.left, 0, dstCenter.width, dstMargin.top);
        }
        if(srcMargin.right > 0 && dstMargin.right > 0)
        { // right
            ZFUIImageImplNinePatchDrawData *drawData = outBuf + count; ++count;
            drawData->position = ZFUIImageImplNinePatchPos::e_TopRight;
            drawData->src = ZFUIRectMake(srcSize.width - srcMarginFixed.right, 0, srcMarginFixed.right, srcMarginFixed.top);
            drawData->dst = ZFUIRectMake(dstMargin.left + dstCenter.width, 0, dstMargin.right, dstMargin.top);
        }
    }
    // center
    if(srcMargin.top > 0 && dstMargin.top > 0)
    {
        if(srcMargin.left > 0 && dstMargin.left > 0)
        { // left
            ZFUIImageImplNinePatchDrawData *drawData = outBuf + count; ++count;
            drawData->position = ZFUIImageImplNinePatchPos::e_CenterLeft;
            drawData->src = ZFUIRectMake(0, srcMargin.top, srcMarginFixed.left, srcCenter.height);
            drawData->dst = ZFUIRectMake(0, dstMargin.top, dstMargin.left, dstCenter.height);
        }
        if(srcCenter.width > 0 && dstCenter.width > 0)
        { // center
            ZFUIImageImplNinePatchDrawData *drawData = outBuf + count; ++count;
            drawData->position = ZFUIImageImplNinePatchPos::e_CenterCenter;
            drawData->src = ZFUIRectMake(srcMargin.left, srcMargin.top, srcCenter.width, srcCenter.height);
            drawData->dst = ZFUIRectMake(dstMargin.left, dstMargin.top, dstCenter.width, dstCenter.height);
        }
        if(srcMargin.right > 0 && dstMargin.right > 0)
        { // right
            ZFUIImageImplNinePatchDrawData *drawData = outBuf + count; ++count;
            drawData->position = ZFUIImageImplNinePatchPos::e_CenterRight;
            drawData->src = ZFUIRectMake(srcSize.width - srcMarginFixed.right, srcMargin.top, srcMarginFixed.right, srcCenter.height);
            drawData->dst = ZFUIRectMake(dstMargin.left + dstCenter.width, dstMargin.top, dstMargin.right, dstCenter.height);
        }
    }
    // bottom
    if(srcMargin.top > 0 && dstMargin.top > 0)
    {
        if(srcMargin.left > 0 && dstMargin.left > 0)
        { // left
            ZFUIImageImplNinePatchDrawData *drawData = outBuf + count; ++count;
            drawData->position = ZFUIImageImplNinePatchPos::e_BottomLeft;
            drawData->src = ZFUIRectMake(0, srcSize.height - srcMarginFixed.bottom, srcMarginFixed.left, srcMarginFixed.bottom);
            drawData->dst = ZFUIRectMake(0, dstMargin.top + dstCenter.height, dstMargin.left, dstMargin.bottom);
        }
        if(srcCenter.width > 0 && dstCenter.width > 0)
        { // center
            ZFUIImageImplNinePatchDrawData *drawData = outBuf + count; ++count;
            drawData->position = ZFUIImageImplNinePatchPos::e_BottomCenter;
            drawData->src = ZFUIRectMake(srcMargin.left, srcSize.height - srcMarginFixed.bottom, srcCenter.width, srcMarginFixed.bottom);
            drawData->dst = ZFUIRectMake(dstMargin.left, dstMargin.top + dstCenter.height, dstCenter.width, dstMargin.bottom);
        }
        if(srcMargin.right > 0 && dstMargin.right > 0)
        { // right
            ZFUIImageImplNinePatchDrawData *drawData = outBuf + count; ++count;
            drawData->position = ZFUIImageImplNinePatchPos::e_BottomRight;
            drawData->src = ZFUIRectMake(srcSize.width - srcMarginFixed.right, srcSize.height - srcMarginFixed.bottom, srcMarginFixed.right, srcMarginFixed.bottom);
            drawData->dst = ZFUIRectMake(dstMargin.left + dstCenter.width, dstMargin.top + dstCenter.height, dstMargin.right, dstMargin.bottom);
        }
    }

    return count;
}

ZF_NAMESPACE_GLOBAL_END

