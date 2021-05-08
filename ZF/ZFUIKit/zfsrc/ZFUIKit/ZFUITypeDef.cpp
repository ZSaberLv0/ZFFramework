#include "ZFUITypeDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFMETHOD_FUNC_DEFINE_3(void, ZFUISizeApplyScale,
                       ZFMP_OUT(zffloat &, ret),
                       ZFMP_IN(zffloat const &, v),
                       ZFMP_IN(zffloat, scale))
{
    ret = (zffloat)zfmRound(v * scale);
}
ZFMETHOD_FUNC_INLINE_DEFINE_2(zffloat, ZFUISizeApplyScale,
                              ZFMP_IN(zffloat const &, v),
                              ZFMP_IN(zffloat, scale))

ZFMETHOD_FUNC_DEFINE_3(void, ZFUISizeApplyScaleReversely,
                       ZFMP_OUT(zffloat &, ret),
                       ZFMP_IN(zffloat const &, v),
                       ZFMP_IN(zffloat, scale))
{
    ret = (zffloat)zfmRound(v / scale);
}
ZFMETHOD_FUNC_INLINE_DEFINE_2(zffloat, ZFUISizeApplyScaleReversely,
                              ZFMP_IN(zffloat const &, v),
                              ZFMP_IN(zffloat, scale))

// ============================================================
// ZFUIPoint
ZFEXPORT_VAR_READONLY_DEFINE(ZFUIPoint, ZFUIPointZero, ZFUIPointMake(0, 0))
ZFMETHOD_FUNC_INLINE_DEFINE_2(ZFUIPoint, ZFUIPointMake,
                              ZFMP_IN(zffloat const &, x),
                              ZFMP_IN(zffloat const &, y))
ZFMETHOD_FUNC_INLINE_DEFINE_3(void, ZFUIPointApplyScale,
                              ZFMP_OUT(ZFUIPoint &, ret),
                              ZFMP_IN(const ZFUIPoint &, point),
                              ZFMP_IN(zffloat, scale))
ZFMETHOD_FUNC_INLINE_DEFINE_2(ZFUIPoint, ZFUIPointApplyScale,
                              ZFMP_IN(const ZFUIPoint &, point),
                              ZFMP_IN(zffloat, scale))
ZFMETHOD_FUNC_INLINE_DEFINE_3(void, ZFUIPointApplyScaleReversely,
                              ZFMP_OUT(ZFUIPoint &, ret),
                              ZFMP_IN(const ZFUIPoint &, point),
                              ZFMP_IN(zffloat, scale))
ZFMETHOD_FUNC_INLINE_DEFINE_2(ZFUIPoint, ZFUIPointApplyScaleReversely,
                              ZFMP_IN(const ZFUIPoint &, point),
                              ZFMP_IN(zffloat, scale))
ZFTYPEID_DEFINE_BY_STRING_CONVERTER(ZFUIPoint, ZFUIPoint, {
        ZFCoreArrayPOD<zffloat> buf;
        if(!zfCoreDataPairSplitFloat(buf, 2, src, srcLen))
        {
            return zffalse;
        }
        v.x = buf[0];
        v.y = buf[1];
        return zftrue;
    }, {
        zfstringAppend(s, "(%f, %f)", v.x, v.y);
        return zftrue;
    })
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFUIPoint, zffloat, x)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFUIPoint, zffloat, y)

ZFTYPEID_PROGRESS_DEFINE(ZFUIPoint, ZFUIPoint, {
        ret.x = from.x + (zffloat)((to.x - from.x) * progress);
        ret.y = from.y + (zffloat)((to.y - from.y) * progress);
    })

// ============================================================
// ZFUIMargin
ZFEXPORT_VAR_READONLY_DEFINE(ZFUIMargin, ZFUIMarginZero, ZFUIMarginMake(0, 0, 0, 0))
ZFMETHOD_FUNC_INLINE_DEFINE_4(ZFUIMargin, ZFUIMarginMake,
                              ZFMP_IN(zffloat const &, left),
                              ZFMP_IN(zffloat const &, top),
                              ZFMP_IN(zffloat const &, right),
                              ZFMP_IN(zffloat const &, bottom))
ZFMETHOD_FUNC_INLINE_DEFINE_1(ZFUIMargin, ZFUIMarginMake,
                              ZFMP_IN(zffloat const &, margin))
ZFMETHOD_FUNC_INLINE_DEFINE_2(ZFUIMargin, ZFUIMarginMake,
                              ZFMP_IN(zffloat const &, x),
                              ZFMP_IN(zffloat const &, y))
ZFMETHOD_FUNC_INLINE_DEFINE_3(void, ZFUIMarginApplyScale,
                              ZFMP_OUT(ZFUIMargin &, ret),
                              ZFMP_IN(const ZFUIMargin &, margin),
                              ZFMP_IN(zffloat, scale))
ZFMETHOD_FUNC_INLINE_DEFINE_2(ZFUIMargin, ZFUIMarginApplyScale,
                              ZFMP_IN(const ZFUIMargin &, margin),
                              ZFMP_IN(zffloat, scale))
ZFMETHOD_FUNC_INLINE_DEFINE_3(void, ZFUIMarginApplyScaleReversely,
                              ZFMP_OUT(ZFUIMargin &, ret),
                              ZFMP_IN(const ZFUIMargin &, margin),
                              ZFMP_IN(zffloat, scale))
ZFMETHOD_FUNC_INLINE_DEFINE_2(ZFUIMargin, ZFUIMarginApplyScaleReversely,
                              ZFMP_IN(const ZFUIMargin &, margin),
                              ZFMP_IN(zffloat, scale))
ZFMETHOD_FUNC_INLINE_DEFINE_1(zffloat, ZFUIMarginGetWidth,
                              ZFMP_IN(const ZFUIMargin &, margin))
ZFMETHOD_FUNC_INLINE_DEFINE_1(zffloat, ZFUIMarginGetHeight,
                              ZFMP_IN(const ZFUIMargin &, margin))
ZFMETHOD_FUNC_INLINE_DEFINE_3(void, ZFUIMarginInc,
                              ZFMP_IN_OUT(ZFUIMargin &, ret),
                              ZFMP_IN(const ZFUIMargin &, v0),
                              ZFMP_IN(const ZFUIMargin &, v1))
ZFMETHOD_FUNC_INLINE_DEFINE_2(ZFUIMargin, ZFUIMarginInc,
                              ZFMP_IN(const ZFUIMargin &, v0),
                              ZFMP_IN(const ZFUIMargin &, v1))
ZFMETHOD_FUNC_INLINE_DEFINE_3(void, ZFUIMarginDec,
                              ZFMP_IN_OUT(ZFUIMargin &, ret),
                              ZFMP_IN(const ZFUIMargin &, v0),
                              ZFMP_IN(const ZFUIMargin &, v1))
ZFMETHOD_FUNC_INLINE_DEFINE_2(ZFUIMargin, ZFUIMarginDec,
                              ZFMP_IN(const ZFUIMargin &, v0),
                              ZFMP_IN(const ZFUIMargin &, v1))
ZFTYPEID_DEFINE_BY_STRING_CONVERTER(ZFUIMargin, ZFUIMargin, {
        ZFCoreArrayPOD<zffloat> buf;
        if(!zfCoreDataPairSplitFloat(buf, 4, src, srcLen))
        {
            return zffalse;
        }
        v.left = buf[0];
        v.top = buf[1];
        v.right = buf[2];
        v.bottom = buf[3];
        return zftrue;
    }, {
        zfstringAppend(s, "(%f, %f, %f, %f)", v.left, v.top, v.right, v.bottom);
        return zftrue;
    })
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFUIMargin, zffloat, left)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFUIMargin, zffloat, top)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFUIMargin, zffloat, right)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFUIMargin, zffloat, bottom)

ZFTYPEID_PROGRESS_DEFINE(ZFUIMargin, ZFUIMargin, {
        ret.left = from.left + (zffloat)((to.left - from.left) * progress);
        ret.top = from.top + (zffloat)((to.top - from.top) * progress);
        ret.right = from.right + (zffloat)((to.right - from.right) * progress);
        ret.bottom = from.bottom + (zffloat)((to.bottom - from.bottom) * progress);
    })

// ============================================================
// ZFUISize
ZFEXPORT_VAR_READONLY_DEFINE(ZFUISize, ZFUISizeZero, ZFUISizeMake(0, 0))
ZFEXPORT_VAR_READONLY_DEFINE(ZFUISize, ZFUISizeInvalid, ZFUISizeMake(-1, -1))
ZFMETHOD_FUNC_INLINE_DEFINE_2(ZFUISize, ZFUISizeMake,
                              ZFMP_IN(zffloat const &, width),
                              ZFMP_IN(zffloat const &, height))
ZFMETHOD_FUNC_INLINE_DEFINE_1(ZFUISize, ZFUISizeMake,
                              ZFMP_IN(zffloat const &, v))
ZFMETHOD_FUNC_INLINE_DEFINE_4(void, ZFUISizeApplyRange,
                              ZFMP_OUT(ZFUISize &, ret),
                              ZFMP_IN(const ZFUISize &, orgSize),
                              ZFMP_IN(const ZFUISize &, minSize),
                              ZFMP_IN(const ZFUISize &, maxSize))
ZFMETHOD_FUNC_INLINE_DEFINE_3(ZFUISize, ZFUISizeApplyRange,
                              ZFMP_IN(const ZFUISize &, orgSize),
                              ZFMP_IN(const ZFUISize &, minSize),
                              ZFMP_IN(const ZFUISize &, maxSize))
ZFMETHOD_FUNC_INLINE_DEFINE_3(void, ZFUISizeApplyScale,
                              ZFMP_OUT(ZFUISize &, ret),
                              ZFMP_IN(const ZFUISize &, size),
                              ZFMP_IN(zffloat, scale))
ZFMETHOD_FUNC_INLINE_DEFINE_2(ZFUISize, ZFUISizeApplyScale,
                              ZFMP_IN(const ZFUISize &, size),
                              ZFMP_IN(zffloat, scale))
ZFMETHOD_FUNC_INLINE_DEFINE_3(void, ZFUISizeApplyScaleReversely,
                              ZFMP_OUT(ZFUISize &, ret),
                              ZFMP_IN(const ZFUISize &, size),
                              ZFMP_IN(zffloat, scale))
ZFMETHOD_FUNC_INLINE_DEFINE_2(ZFUISize, ZFUISizeApplyScaleReversely,
                              ZFMP_IN(const ZFUISize &, size),
                              ZFMP_IN(zffloat, scale))
ZFMETHOD_FUNC_DEFINE_3(void, ZFUISizeApplyAspectRatio,
                       ZFMP_OUT(ZFUISize &, ret),
                       ZFMP_IN(const ZFUISize &, size),
                       ZFMP_IN(const ZFUISize &, refSize))
{
    if(size.width <= 0 || size.height <= 0
        || refSize.width == 0 || refSize.height == 0)
    {
        ret = ZFUISizeZero();
    }
    else if(refSize.width < 0 && refSize.height < 0)
    {
        ret = size;
    }
    else if(refSize.height < 0 || size.width * refSize.height > size.height * refSize.width)
    {
        ret = ZFUISizeMake(
            refSize.width,
            size.height * refSize.width / size.width
        );
    }
    else
    {
        ret = ZFUISizeMake(
            size.width * refSize.height / size.height,
            refSize.height
        );
    }
}
ZFMETHOD_FUNC_INLINE_DEFINE_2(ZFUISize, ZFUISizeApplyAspectRatio,
                              ZFMP_IN(const ZFUISize &, size),
                              ZFMP_IN(const ZFUISize &, refSize))
ZFMETHOD_FUNC_INLINE_DEFINE_3(void, ZFUISizeApplyMargin,
                              ZFMP_OUT(ZFUISize &, ret),
                              ZFMP_IN(const ZFUISize &, size),
                              ZFMP_IN(const ZFUIMargin &, margin))
ZFMETHOD_FUNC_INLINE_DEFINE_2(ZFUISize, ZFUISizeApplyMargin,
                              ZFMP_IN(const ZFUISize &, size),
                              ZFMP_IN(const ZFUIMargin &, margin))
ZFMETHOD_FUNC_INLINE_DEFINE_3(void, ZFUISizeApplyMarginReversely,
                              ZFMP_OUT(ZFUISize &, ret),
                              ZFMP_IN(const ZFUISize &, size),
                              ZFMP_IN(const ZFUIMargin &, margin))
ZFMETHOD_FUNC_INLINE_DEFINE_2(ZFUISize, ZFUISizeApplyMarginReversely,
                              ZFMP_IN(const ZFUISize &, size),
                              ZFMP_IN(const ZFUIMargin &, margin))

ZFTYPEID_DEFINE_BY_STRING_CONVERTER(ZFUISize, ZFUISize, {
        ZFCoreArrayPOD<zffloat> buf;
        if(!zfCoreDataPairSplitFloat(buf, 2, src, srcLen))
        {
            return zffalse;
        }
        v.width = buf[0];
        v.height = buf[1];
        return zftrue;
    }, {
        zfstringAppend(s, "(%f, %f)", v.width, v.height);
        return zftrue;
    })
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFUISize, zffloat, width)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFUISize, zffloat, height)

ZFTYPEID_PROGRESS_DEFINE(ZFUISize, ZFUISize, {
        ret.width = from.width + (zffloat)((to.width - from.width) * progress);
        ret.height = from.height + (zffloat)((to.height - from.height) * progress);
    })

// ============================================================
// ZFUIRect
ZFEXPORT_VAR_READONLY_DEFINE(ZFUIRect, ZFUIRectZero, ZFUIRectMake(0, 0, 0, 0))
ZFMETHOD_FUNC_INLINE_DEFINE_4(ZFUIRect, ZFUIRectMake,
                              ZFMP_IN(zffloat const &, x),
                              ZFMP_IN(zffloat const &, y),
                              ZFMP_IN(zffloat const &, w),
                              ZFMP_IN(zffloat const &, h))
ZFMETHOD_FUNC_INLINE_DEFINE_2(ZFUIRect, ZFUIRectMake,
                              ZFMP_IN(const ZFUIPoint &, point),
                              ZFMP_IN(const ZFUISize &, size))
ZFMETHOD_FUNC_INLINE_DEFINE_1(zfbool, ZFUIRectIsEmpty,
                              ZFMP_IN(const ZFUIRect &, rect))
ZFMETHOD_FUNC_INLINE_DEFINE_2(ZFUIRect, ZFUIRectUnion,
                              ZFMP_IN(const ZFUIRect &, rect1),
                              ZFMP_IN(const ZFUIRect &, rect2))
ZFMETHOD_FUNC_INLINE_DEFINE_2(ZFUIRect, ZFUIRectIntersection,
                              ZFMP_IN(const ZFUIRect &, rect1),
                              ZFMP_IN(const ZFUIRect &, rect2))
ZFMETHOD_FUNC_INLINE_DEFINE_2(zfbool, ZFUIRectIsIntersectToRect,
                              ZFMP_IN(const ZFUIRect &, rect1),
                              ZFMP_IN(const ZFUIRect &, rect2))
ZFMETHOD_FUNC_INLINE_DEFINE_2(zfbool, ZFUIRectIsContainPoint,
                              ZFMP_IN(const ZFUIRect &, rect),
                              ZFMP_IN(const ZFUIPoint &, point))
ZFMETHOD_FUNC_INLINE_DEFINE_3(zfbool, ZFUIRectIsContainPoint,
                              ZFMP_IN(const ZFUIRect &, rect),
                              ZFMP_IN(const ZFUIPoint &, point),
                              ZFMP_IN(const ZFUIMargin &, margin))
ZFMETHOD_FUNC_INLINE_DEFINE_2(zfbool, ZFUIRectIsContainRect,
                              ZFMP_IN(const ZFUIRect &, rect1),
                              ZFMP_IN(const ZFUIRect &, rect2))
ZFMETHOD_FUNC_INLINE_DEFINE_3(zfbool, ZFUIRectIsContainRect,
                              ZFMP_IN(const ZFUIRect &, rect1),
                              ZFMP_IN(const ZFUIRect &, rect2),
                              ZFMP_IN(const ZFUIMargin &, margin))
ZFMETHOD_FUNC_INLINE_DEFINE_3(void, ZFUIRectApplyScale,
                              ZFMP_OUT(ZFUIRect &, ret),
                              ZFMP_IN(const ZFUIRect &, rect),
                              ZFMP_IN(zffloat, scale))
ZFMETHOD_FUNC_INLINE_DEFINE_2(ZFUIRect, ZFUIRectApplyScale,
                              ZFMP_IN(const ZFUIRect &, rect),
                              ZFMP_IN(zffloat, scale))
ZFMETHOD_FUNC_INLINE_DEFINE_3(void, ZFUIRectApplyScaleReversely,
                              ZFMP_OUT(ZFUIRect &, ret),
                              ZFMP_IN(const ZFUIRect &, rect),
                              ZFMP_IN(zffloat, scale))
ZFMETHOD_FUNC_INLINE_DEFINE_2(ZFUIRect, ZFUIRectApplyScaleReversely,
                              ZFMP_IN(const ZFUIRect &, rect),
                              ZFMP_IN(zffloat, scale))
ZFMETHOD_FUNC_INLINE_DEFINE_1(ZFUIRect, ZFUIRectGetBounds,
                              ZFMP_IN(const ZFUIRect &, rect))
ZFMETHOD_FUNC_INLINE_DEFINE_1(ZFUIPoint, ZFUIRectGetCenter,
                              ZFMP_IN(const ZFUIRect &, rect))
ZFMETHOD_FUNC_INLINE_DEFINE_1(zffloat, ZFUIRectGetLeft,
                              ZFMP_IN(const ZFUIRect &, rect))
ZFMETHOD_FUNC_INLINE_DEFINE_1(zffloat, ZFUIRectGetTop,
                              ZFMP_IN(const ZFUIRect &, rect))
ZFMETHOD_FUNC_INLINE_DEFINE_1(zffloat, ZFUIRectGetRight,
                              ZFMP_IN(const ZFUIRect &, rect))
ZFMETHOD_FUNC_INLINE_DEFINE_1(zffloat, ZFUIRectGetBottom,
                              ZFMP_IN(const ZFUIRect &, rect))
ZFMETHOD_FUNC_INLINE_DEFINE_3(void, ZFUIRectApplyMargin,
                              ZFMP_OUT(ZFUIRect &, ret),
                              ZFMP_IN(const ZFUIRect &, rect),
                              ZFMP_IN(const ZFUIMargin &, margin))
ZFMETHOD_FUNC_INLINE_DEFINE_2(ZFUIRect, ZFUIRectApplyMargin,
                              ZFMP_IN(const ZFUIRect &, rect),
                              ZFMP_IN(const ZFUIMargin &, margin))
ZFMETHOD_FUNC_INLINE_DEFINE_3(void, ZFUIRectApplyMarginReversely,
                              ZFMP_OUT(ZFUIRect &, ret),
                              ZFMP_IN(const ZFUIRect &, rect),
                              ZFMP_IN(const ZFUIMargin &, margin))
ZFMETHOD_FUNC_INLINE_DEFINE_2(ZFUIRect, ZFUIRectApplyMarginReversely,
                              ZFMP_IN(const ZFUIRect &, rect),
                              ZFMP_IN(const ZFUIMargin &, margin))
ZFTYPEID_DEFINE_BY_STRING_CONVERTER(ZFUIRect, ZFUIRect, {
        ZFCoreArrayPOD<zffloat> buf;
        if(!zfCoreDataPairSplitFloat(buf, 4, src, srcLen))
        {
            return zffalse;
        }
        v.x = buf[0];
        v.y = buf[1];
        v.width = buf[2];
        v.height = buf[3];
        return zftrue;
    }, {
        zfstringAppend(s, "(%f, %f, %f, %f)", v.x, v.y, v.width, v.height);
        return zftrue;
    })
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFUIRect, zffloat, x)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFUIRect, zffloat, y)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFUIRect, zffloat, width)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFUIRect, zffloat, height)

ZFTYPEID_PROGRESS_DEFINE(ZFUIRect, ZFUIRect, {
        ret.x = from.x + (zffloat)((to.x - from.x) * progress);
        ret.y = from.y + (zffloat)((to.y - from.y) * progress);
        ret.width = from.width + (zffloat)((to.width - from.width) * progress);
        ret.height = from.height + (zffloat)((to.height - from.height) * progress);
    })

// ============================================================
// ZFUIAlignFlags
ZFENUM_DEFINE_FLAGS(ZFUIAlign, ZFUIAlignFlags)
static const ZFUIAlignFlags &_ZFP_ZFUIAlignMask(void)
{
    static ZFUIAlignFlags _alignMask = (0
        | ZFUIAlign::e_Center
        | ZFUIAlign::e_Left
        | ZFUIAlign::e_LeftInner
        | ZFUIAlign::e_Top
        | ZFUIAlign::e_TopInner
        | ZFUIAlign::e_Right
        | ZFUIAlign::e_RightInner
        | ZFUIAlign::e_Bottom
        | ZFUIAlign::e_BottomInner
        );
    return _alignMask;
}
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFUIAlignIsValid,
                       ZFMP_IN(const ZFUIAlignFlags &, align))
{
    return (
            ZFBitGet(align, ~(_ZFP_ZFUIAlignMask())) == 0
            && ((zffloat)ZFBitTest(align, ZFUIAlign::e_Left)
                + (zffloat)ZFBitTest(align, ZFUIAlign::e_LeftInner)
                + (zffloat)ZFBitTest(align, ZFUIAlign::e_Right)
                + (zffloat)ZFBitTest(align, ZFUIAlign::e_RightInner)
                ) <= 1
            && ((zffloat)ZFBitTest(align, ZFUIAlign::e_Top)
                + (zffloat)ZFBitTest(align, ZFUIAlign::e_TopInner)
                + (zffloat)ZFBitTest(align, ZFUIAlign::e_Bottom)
                + (zffloat)ZFBitTest(align, ZFUIAlign::e_BottomInner)
                ) <= 1
        );
}
ZFMETHOD_FUNC_DEFINE_5(void, ZFUIAlignApply,
                       ZFMP_OUT(ZFUIRect &, ret),
                       ZFMP_IN(const ZFUIAlignFlags &, align),
                       ZFMP_IN(const ZFUIRect &, refRect),
                       ZFMP_IN(const ZFUISize &, itemSize),
                       ZFMP_IN_OPT(const ZFUIMargin &, margin, ZFUIMarginZero()))
{
    ret = ZFUIRectZero();
    if(!ZFUIAlignIsValid(align))
    {
        return ;
    }
    ret.width = itemSize.width;
    ret.height = itemSize.height;

    if(ZFBitTest(align, ZFUIAlign::e_LeftInner))
    {
        ret.x = refRect.x + margin.left;
    }
    else if(ZFBitTest(align, ZFUIAlign::e_RightInner))
    {
        ret.x = refRect.x + refRect.width - margin.right - itemSize.width;
    }
    else if(ZFBitTest(align, ZFUIAlign::e_Left))
    {
        ret.x = refRect.x - margin.right - itemSize.width;
    }
    else if(ZFBitTest(align, ZFUIAlign::e_Right))
    {
        ret.x = refRect.x + refRect.width + margin.left;
    }
    else
    {
        ret.x = refRect.x + margin.left
            + (refRect.width - margin.left - margin.right - itemSize.width) / 2;
    }

    if(ZFBitTest(align, ZFUIAlign::e_TopInner))
    {
        ret.y = refRect.y + margin.top;
    }
    else if(ZFBitTest(align, ZFUIAlign::e_BottomInner))
    {
        ret.y = refRect.y + refRect.height - margin.bottom - itemSize.height;
    }
    else if(ZFBitTest(align, ZFUIAlign::e_Top))
    {
        ret.y = refRect.y - margin.bottom - itemSize.height;
    }
    else if(ZFBitTest(align, ZFUIAlign::e_Bottom))
    {
        ret.y = refRect.y + refRect.height + margin.top;
    }
    else
    {
        ret.y = refRect.y + margin.top
            + (refRect.height - margin.top - margin.bottom - itemSize.height) / 2;
    }
}
ZFMETHOD_FUNC_INLINE_DEFINE_4(ZFUIRect, ZFUIAlignApply,
                              ZFMP_IN(const ZFUIAlignFlags &, align),
                              ZFMP_IN(const ZFUIRect &, refRect),
                              ZFMP_IN(const ZFUISize &, itemSize),
                              ZFMP_IN_OPT(const ZFUIMargin &, margin, ZFUIMarginZero()))
ZFMETHOD_FUNC_DEFINE_1(ZFUIAlignEnum, ZFUIAlignGetX,
                       ZFMP_IN(const ZFUIAlignFlags &, align))
{
    if(!ZFUIAlignIsValid(align))
    {
        return ZFUIAlign::e_Center;
    }

    if(ZFBitTest(align, ZFUIAlign::e_Left))
    {
        return ZFUIAlign::e_Left;
    }
    if(ZFBitTest(align, ZFUIAlign::e_LeftInner))
    {
        return ZFUIAlign::e_LeftInner;
    }
    if(ZFBitTest(align, ZFUIAlign::e_Right))
    {
        return ZFUIAlign::e_Right;
    }
    if(ZFBitTest(align, ZFUIAlign::e_RightInner))
    {
        return ZFUIAlign::e_RightInner;
    }
    return ZFUIAlign::e_Center;
}
ZFMETHOD_FUNC_DEFINE_1(ZFUIAlignEnum, ZFUIAlignGetY,
                       ZFMP_IN(const ZFUIAlignFlags &, align))
{
    if(!ZFUIAlignIsValid(align))
    {
        return ZFUIAlign::e_Center;
    }

    if(ZFBitTest(align, ZFUIAlign::e_Top))
    {
        return ZFUIAlign::e_Top;
    }
    if(ZFBitTest(align, ZFUIAlign::e_TopInner))
    {
        return ZFUIAlign::e_TopInner;
    }
    if(ZFBitTest(align, ZFUIAlign::e_Bottom))
    {
        return ZFUIAlign::e_Bottom;
    }
    if(ZFBitTest(align, ZFUIAlign::e_BottomInner))
    {
        return ZFUIAlign::e_BottomInner;
    }
    return ZFUIAlign::e_Center;
}

// ============================================================
// ZFUIColor
ZFEXPORT_VAR_READONLY_DEFINE(ZFUIColor, ZFUIColorZero, ZFUIColorMake(0, 0, 0, 0))
ZFTYPEID_DEFINE_BY_STRING_CONVERTER(ZFUIColor, ZFUIColor, {
        zft_ZFUIColor c = 0;
        do
        {
            if(src == zfnull)
            {
                return zffalse;
            }
            if(srcLen == zfindexMax())
            {
                srcLen = zfslen(src);
            }
            if(src[0] == '#')
            {
                --srcLen;
                ++src;
            }
            else if(src[0] == '0' && (src[1] == 'x' || src[1] == 'X'))
            {
                srcLen -= 2;
                src += 2;
            }
            if(srcLen != 3 && srcLen != 4 && srcLen != 6 && srcLen != 8)
            {
                return zffalse;
            }

            c = 0xFFFFFFFF;
            zfuint tmp = 0;

            if(srcLen <= 4)
            {
                tmp = 0;
                if(!zfsToIntT(tmp, src, 1, 16)) {return zffalse;} src += 1;
                c = ((c << 8) | (tmp * 16 + tmp));

                tmp = 0;
                if(!zfsToIntT(tmp, src, 1, 16)) {return zffalse;} src += 1;
                c = ((c << 8) | (tmp * 16 + tmp));

                tmp = 0;
                if(!zfsToIntT(tmp, src, 1, 16)) {return zffalse;} src += 1;
                c = ((c << 8) | (tmp * 16 + tmp));

                if(srcLen == 4)
                {
                    tmp = 0;
                    if(!zfsToIntT(tmp, src, 1, 16)) {return zffalse;} src += 1;
                    c = ((c << 8) | (tmp * 16 + tmp));
                }
            }
            else
            {
                tmp = 0;
                if(!zfsToIntT(tmp, src, 2, 16)) {return zffalse;} src += 2;
                c = ((c << 8) | tmp);

                tmp = 0;
                if(!zfsToIntT(tmp, src, 2, 16)) {return zffalse;} src += 2;
                c = ((c << 8) | tmp);

                tmp = 0;
                if(!zfsToIntT(tmp, src, 2, 16)) {return zffalse;} src += 2;
                c = ((c << 8) | tmp);

                if(srcLen == 8)
                {
                    tmp = 0;
                    if(!zfsToIntT(tmp, src, 2, 16)) {return zffalse;} src += 2;
                    c = ((c << 8) | tmp);
                }
            }
        } while(zffalse);

        v = c;

        return zftrue;
    }, {
        if(zftrue
                && ((v >> 28) & 0x0F) == ((v >> 24) & 0x0F)
                && ((v >> 20) & 0x0F) == ((v >> 16) & 0x0F)
                && ((v >> 12) & 0x0F) == ((v >> 8) & 0x0F)
                && ((v >> 4) & 0x0F) == ((v >> 0) & 0x0F)
            )
        {
            if(((v >> 24) & 0xFF) == 0xFF)
            {
                zfstringAppend(s, "#%X%X%X"
                    , ((v >> 16) & 0x0F)
                    , ((v >> 8) & 0x0F)
                    , ((v >> 0) & 0x0F)
                    );
            }
            else
            {
                zfstringAppend(s, "#%X%X%X%X"
                    , ((v >> 24) & 0x0F)
                    , ((v >> 16) & 0x0F)
                    , ((v >> 8) & 0x0F)
                    , ((v >> 0) & 0x0F)
                    );
            }
        }
        else
        {
            if(((v >> 24) & 0xFF) == 0xFF)
            {
                zfstringAppend(s, "#%02X%02X%02X"
                    , ((v >> 16) & 0xFF)
                    , ((v >> 8) & 0xFF)
                    , ((v >> 0) & 0xFF)
                    );
            }
            else
            {
                zfstringAppend(s, "#%02X%02X%02X%02X"
                    , ((v >> 24) & 0xFF)
                    , ((v >> 16) & 0xFF)
                    , ((v >> 8) & 0xFF)
                    , ((v >> 0) & 0xFF)
                    );
            }
        }
        return zftrue;
    })

ZFTYPEID_PROGRESS_DEFINE(ZFUIColor, ZFUIColor, {
        ret = ZFUIColorMake(
            ZFUIColorGetR(from) + (zffloat)((ZFUIColorGetR(to) - ZFUIColorGetR(from)) * progress),
            ZFUIColorGetG(from) + (zffloat)((ZFUIColorGetG(to) - ZFUIColorGetG(from)) * progress),
            ZFUIColorGetB(from) + (zffloat)((ZFUIColorGetB(to) - ZFUIColorGetB(from)) * progress),
            ZFUIColorGetA(from) + (zffloat)((ZFUIColorGetA(to) - ZFUIColorGetA(from)) * progress)
        );
    })

ZFMETHOD_FUNC_INLINE_DEFINE_2(zfbool, ZFUIColorIsEqual,
                              ZFMP_IN(ZFUIColor const &, v0),
                              ZFMP_IN(ZFUIColor const &, v1))

ZFMETHOD_FUNC_INLINE_DEFINE_4(ZFUIColor, ZFUIColorMake,
                              ZFMP_IN(zffloat, r),
                              ZFMP_IN(zffloat, g),
                              ZFMP_IN(zffloat, b),
                              ZFMP_IN_OPT(zffloat, a, 1.0f))

ZFMETHOD_FUNC_INLINE_DEFINE_1(zffloat, ZFUIColorGetA,
                              ZFMP_IN(ZFUIColor const &, c))
ZFMETHOD_FUNC_INLINE_DEFINE_1(zffloat, ZFUIColorGetR,
                              ZFMP_IN(ZFUIColor const &, c))
ZFMETHOD_FUNC_INLINE_DEFINE_1(zffloat, ZFUIColorGetG,
                              ZFMP_IN(ZFUIColor const &, c))
ZFMETHOD_FUNC_INLINE_DEFINE_1(zffloat, ZFUIColorGetB,
                              ZFMP_IN(ZFUIColor const &, c))

ZFMETHOD_FUNC_INLINE_DEFINE_2(ZFUIColor &, ZFUIColorSetA,
                              ZFMP_IN_OUT(ZFUIColor &, c),
                              ZFMP_IN(zffloat, a))
ZFMETHOD_FUNC_INLINE_DEFINE_2(ZFUIColor &, ZFUIColorSetR,
                              ZFMP_IN_OUT(ZFUIColor &, c),
                              ZFMP_IN(zffloat, r))
ZFMETHOD_FUNC_INLINE_DEFINE_2(ZFUIColor &, ZFUIColorSetG,
                              ZFMP_IN_OUT(ZFUIColor &, c),
                              ZFMP_IN(zffloat, g))
ZFMETHOD_FUNC_INLINE_DEFINE_2(ZFUIColor &, ZFUIColorSetB,
                              ZFMP_IN_OUT(ZFUIColor &, c),
                              ZFMP_IN(zffloat, b))

ZFMETHOD_FUNC_INLINE_DEFINE_2(ZFUIColor, ZFUIColorWithA,
                              ZFMP_IN(ZFUIColor const &, c),
                              ZFMP_IN(zffloat, a))
ZFMETHOD_FUNC_INLINE_DEFINE_2(ZFUIColor, ZFUIColorWithR,
                              ZFMP_IN(ZFUIColor const &, c),
                              ZFMP_IN(zffloat, r))
ZFMETHOD_FUNC_INLINE_DEFINE_2(ZFUIColor, ZFUIColorWithG,
                              ZFMP_IN(ZFUIColor const &, c),
                              ZFMP_IN(zffloat, g))
ZFMETHOD_FUNC_INLINE_DEFINE_2(ZFUIColor, ZFUIColorWithB,
                              ZFMP_IN(ZFUIColor const &, c),
                              ZFMP_IN(zffloat, b))

ZFMETHOD_FUNC_INLINE_DEFINE_1(ZFUIColor, ZFUIColorRandom,
                              ZFMP_IN_OPT(zffloat, alpha, 1.0f))

// ============================================================
ZFENUM_DEFINE_FLAGS(ZFUIOrientation, ZFUIOrientationFlags)

ZFEXPORT_VAR_READONLY_DEFINE(ZFUIOrientationFlags, ZFUIOrientationFlagsAll, (ZFUIOrientation::e_Left | ZFUIOrientation::e_Top | ZFUIOrientation::e_Right | ZFUIOrientation::e_Bottom))
ZFEXPORT_VAR_READONLY_DEFINE(ZFUIOrientationFlags, ZFUIOrientationFlagsHorizontal, (ZFUIOrientation::e_Left | ZFUIOrientation::e_Right))
ZFEXPORT_VAR_READONLY_DEFINE(ZFUIOrientationFlags, ZFUIOrientationFlagsVertical, (ZFUIOrientation::e_Top | ZFUIOrientation::e_Bottom))

ZFMETHOD_FUNC_INLINE_DEFINE_1(zfbool, ZFUIOrientationIsVertical,
                              ZFMP_IN(ZFUIOrientationEnum, orientation))
ZFMETHOD_FUNC_INLINE_DEFINE_1(zfbool, ZFUIOrientationIsHorizontal,
                              ZFMP_IN(ZFUIOrientationEnum, orientation))

ZFENUM_DEFINE(ZFUITextAppearance)

ZFENUM_DEFINE(ZFUITextTruncateMode)

// ============================================================
ZFENUM_DEFINE(ZFUIContentScaleType)

static void _ZFP_ZFUIContentScaleTypeApply_Center(ZF_OUT ZFUIRect &ret, ZF_IN const ZFUIRect &bounds, ZF_IN const ZFUISize &contentSize)
{
    ret = ZFUIAlignApply(ZFUIAlign::e_Center, bounds, contentSize);
}
static void _ZFP_ZFUIContentScaleTypeApply_FillX(ZF_OUT ZFUIRect &ret, ZF_IN const ZFUIRect &bounds, ZF_IN const ZFUISize &contentSize)
{
    ret = ZFUIAlignApply(ZFUIAlign::e_Center, bounds, ZFUISizeMake(
        bounds.width,
        bounds.width * contentSize.height / contentSize.width
        ));
}
static void _ZFP_ZFUIContentScaleTypeApply_FillY(ZF_OUT ZFUIRect &ret, ZF_IN const ZFUIRect &bounds, ZF_IN const ZFUISize &contentSize)
{
    ret = ZFUIAlignApply(ZFUIAlign::e_Center, bounds, ZFUISizeMake(
        contentSize.width * bounds.height / contentSize.height,
        bounds.height
        ));
}
static void _ZFP_ZFUIContentScaleTypeApply_FillCenter(ZF_OUT ZFUIRect &ret, ZF_IN const ZFUIRect &bounds, ZF_IN const ZFUISize &contentSize)
{
    if(contentSize.width * bounds.height >= bounds.width * contentSize.height)
    {
        _ZFP_ZFUIContentScaleTypeApply_FillX(ret, bounds, contentSize);
    }
    else
    {
        _ZFP_ZFUIContentScaleTypeApply_FillY(ret, bounds, contentSize);
    }
}
static void _ZFP_ZFUIContentScaleTypeApply_FillCenterClipped(ZF_OUT ZFUIRect &ret, ZF_IN const ZFUIRect &bounds, ZF_IN const ZFUISize &contentSize)
{
    if(contentSize.width * bounds.height >= bounds.width * contentSize.height)
    {
        _ZFP_ZFUIContentScaleTypeApply_FillY(ret, bounds, contentSize);
    }
    else
    {
        _ZFP_ZFUIContentScaleTypeApply_FillX(ret, bounds, contentSize);
    }
}
ZFMETHOD_FUNC_DEFINE_4(void, ZFUIContentScaleTypeApply,
                       ZFMP_OUT(ZFUIRect &, ret),
                       ZFMP_IN(ZFUIContentScaleTypeEnum, scaleType),
                       ZFMP_IN(const ZFUIRect &, bounds),
                       ZFMP_IN(const ZFUISize &, contentSize))
{
    switch(scaleType)
    {
        case ZFUIContentScaleType::e_Fill:
            ret = bounds;
            break;
        case ZFUIContentScaleType::e_Center:
            _ZFP_ZFUIContentScaleTypeApply_Center(ret, bounds, contentSize);
            break;
        case ZFUIContentScaleType::e_FillCenter:
            _ZFP_ZFUIContentScaleTypeApply_FillCenter(ret, bounds, contentSize);
            break;
        case ZFUIContentScaleType::e_FillCenterClipped:
            _ZFP_ZFUIContentScaleTypeApply_FillCenterClipped(ret, bounds, contentSize);
            break;
        case ZFUIContentScaleType::e_FitCenter:
            if(contentSize.width <= bounds.width && contentSize.height <= bounds.height)
            {
                _ZFP_ZFUIContentScaleTypeApply_Center(ret, bounds, contentSize);
            }
            else
            {
                _ZFP_ZFUIContentScaleTypeApply_FillCenter(ret, bounds, contentSize);
            }
            break;
        case ZFUIContentScaleType::e_FillX:
            _ZFP_ZFUIContentScaleTypeApply_FillX(ret, bounds, contentSize);
            break;
        case ZFUIContentScaleType::e_FitX:
            if(contentSize.width <= bounds.width)
            {
                _ZFP_ZFUIContentScaleTypeApply_Center(ret, bounds, contentSize);
            }
            else
            {
                _ZFP_ZFUIContentScaleTypeApply_FillX(ret, bounds, contentSize);
            }
            break;
        case ZFUIContentScaleType::e_FillY:
            _ZFP_ZFUIContentScaleTypeApply_FillY(ret, bounds, contentSize);
            break;
        case ZFUIContentScaleType::e_FitY:
            if(contentSize.height <= bounds.height)
            {
                _ZFP_ZFUIContentScaleTypeApply_Center(ret, bounds, contentSize);
            }
            else
            {
                _ZFP_ZFUIContentScaleTypeApply_FillY(ret, bounds, contentSize);
            }
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            break;
    }
}
ZFMETHOD_FUNC_INLINE_DEFINE_3(ZFUIRect, ZFUIContentScaleTypeApply,
                              ZFMP_IN(ZFUIContentScaleTypeEnum, scaleType),
                              ZFMP_IN(const ZFUIRect &, bounds),
                              ZFMP_IN(const ZFUISize &, contentSize))

ZF_NAMESPACE_GLOBAL_END

