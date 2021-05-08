/**
 * @file ZFUITypeDef.h
 * @brief necessary header file for all ZFUIKit member
 */

#ifndef _ZFI_ZFUITypeDef_h_
#define _ZFI_ZFUITypeDef_h_

#include "ZFUIKitDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// basic size
/**
 * @brief return a scaled int
 */
ZFMETHOD_FUNC_DECLARE_3(void, ZFUISizeApplyScale,
                        ZFMP_OUT(zffloat &, ret),
                        ZFMP_IN(zffloat const &, v),
                        ZFMP_IN(zffloat, scale))
/**
 * @brief return a scaled int
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(zffloat, ZFUISizeApplyScale,
                               ZFMP_IN(zffloat const &, v),
                               ZFMP_IN(zffloat, scale))
{
    zffloat ret = 0;
    ZFUISizeApplyScale(ret, v, scale);
    return ret;
}
/**
 * @brief return a scaled int reversely
 */
ZFMETHOD_FUNC_DECLARE_3(void, ZFUISizeApplyScaleReversely,
                        ZFMP_OUT(zffloat &, ret),
                        ZFMP_IN(zffloat const &, v),
                        ZFMP_IN(zffloat, scale))
/**
 * @brief return a scaled int reversely
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(zffloat, ZFUISizeApplyScaleReversely,
                               ZFMP_IN(zffloat const &, v),
                               ZFMP_IN(zffloat, scale))
{
    zffloat ret = 0;
    ZFUISizeApplyScaleReversely(ret, v, scale);
    return ret;
}

// ============================================================
// ZFUIPoint
/**
 * @brief 2D point
 */
zfclassPOD ZF_ENV_EXPORT ZFUIPoint
{
public:
    zffloat x;   /**< @brief x */
    zffloat y;   /**< @brief y */
};

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFUIPoint
 *       value="(0, 0)" // optional, 0 by default
 *   />
 * @endcode
 */
ZFTYPEID_DECLARE(ZFUIPoint, ZFUIPoint)

ZFOUTPUT_TYPE(ZFUIPoint, {output << ZFUIPointToString(v);})

ZFCORE_POD_COMPARER_DECLARE(ZFUIPoint)

/**
 * @brief (0, 0)
 */
ZFEXPORT_VAR_READONLY_DECLARE(ZFUIPoint, ZFUIPointZero)

/**
 * @brief make a ZFUIPoint
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFUIPoint, ZFUIPointMake,
                               ZFMP_IN(zffloat const &, x),
                               ZFMP_IN(zffloat const &, y))
{
    ZFUIPoint ret = {x, y};
    return ret;
}

/**
 * @brief return a scaled point
 */
ZFMETHOD_FUNC_INLINE_DECLARE_3(void, ZFUIPointApplyScale,
                               ZFMP_OUT(ZFUIPoint &, ret),
                               ZFMP_IN(const ZFUIPoint &, point),
                               ZFMP_IN(zffloat, scale))
{
    ret.x = ZFUISizeApplyScale(point.x, scale);
    ret.y = ZFUISizeApplyScale(point.y, scale);
}
/**
 * @brief return a scaled point
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFUIPoint, ZFUIPointApplyScale,
                               ZFMP_IN(const ZFUIPoint &, point),
                               ZFMP_IN(zffloat, scale))
{
    ZFUIPoint ret = {ZFUISizeApplyScale(point.x, scale), ZFUISizeApplyScale(point.y, scale)};
    return ret;
}
/**
 * @brief return a scaled point reversely
 */
ZFMETHOD_FUNC_INLINE_DECLARE_3(void, ZFUIPointApplyScaleReversely,
                               ZFMP_OUT(ZFUIPoint &, ret),
                               ZFMP_IN(const ZFUIPoint &, point),
                               ZFMP_IN(zffloat, scale))
{
    ret.x = ZFUISizeApplyScaleReversely(point.x, scale);
    ret.y = ZFUISizeApplyScaleReversely(point.y, scale);
}
/**
 * @brief return a scaled point reversely
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFUIPoint, ZFUIPointApplyScaleReversely,
                               ZFMP_IN(const ZFUIPoint &, point),
                               ZFMP_IN(zffloat, scale))
{
    ZFUIPoint ret = {ZFUISizeApplyScaleReversely(point.x, scale), ZFUISizeApplyScaleReversely(point.y, scale)};
    return ret;
}

// ============================================================
// ZFUIMargin
/**
 * @brief margin
 */
zfclassPOD ZF_ENV_EXPORT ZFUIMargin
{
public:
    zffloat left;    /**< @brief left margin */
    zffloat top;     /**< @brief top margin */
    zffloat right;   /**< @brief right margin */
    zffloat bottom;  /**< @brief bottom margin */
};

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFUIMargin
 *       value="(0, 0, 0, 0)" // optional, 0 by default
 *   />
 * @endcode
 */
ZFTYPEID_DECLARE(ZFUIMargin, ZFUIMargin)

ZFOUTPUT_TYPE(ZFUIMargin, {output << ZFUIMarginToString(v);})

ZFCORE_POD_COMPARER_DECLARE(ZFUIMargin)

/**
 * @brief (0, 0, 0, 0)
 */
ZFEXPORT_VAR_READONLY_DECLARE(ZFUIMargin, ZFUIMarginZero)

/**
 * @brief make a margin
 */
ZFMETHOD_FUNC_INLINE_DECLARE_4(ZFUIMargin, ZFUIMarginMake,
                               ZFMP_IN(zffloat const &, left),
                               ZFMP_IN(zffloat const &, top),
                               ZFMP_IN(zffloat const &, right),
                               ZFMP_IN(zffloat const &, bottom))
{
    ZFUIMargin ret = {left, top, right, bottom};
    return ret;
}
/**
 * @brief make a margin with all sides set to margin
 */
ZFMETHOD_FUNC_INLINE_DECLARE_1(ZFUIMargin, ZFUIMarginMake,
                               ZFMP_IN(zffloat const &, margin))
{
    ZFUIMargin ret = {margin, margin, margin, margin};
    return ret;
}
/**
 * @brief make a margin with left/right to x and top/bottom to y
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFUIMargin, ZFUIMarginMake,
                               ZFMP_IN(zffloat const &, x),
                               ZFMP_IN(zffloat const &, y))
{
    ZFUIMargin ret = {x, y, x, y};
    return ret;
}

/**
 * @brief return a scaled margin
 */
ZFMETHOD_FUNC_INLINE_DECLARE_3(void, ZFUIMarginApplyScale,
                               ZFMP_OUT(ZFUIMargin &, ret),
                               ZFMP_IN(const ZFUIMargin &, margin),
                               ZFMP_IN(zffloat, scale))
{
    ret.left = ZFUISizeApplyScale(margin.left, scale);
    ret.top = ZFUISizeApplyScale(margin.top, scale);
    ret.right = ZFUISizeApplyScale(margin.right, scale);
    ret.bottom = ZFUISizeApplyScale(margin.bottom, scale);
}
/**
 * @brief return a scaled margin
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFUIMargin, ZFUIMarginApplyScale,
                               ZFMP_IN(const ZFUIMargin &, margin),
                               ZFMP_IN(zffloat, scale))
{
    ZFUIMargin ret = {
        ZFUISizeApplyScale(margin.left, scale),
        ZFUISizeApplyScale(margin.top, scale),
        ZFUISizeApplyScale(margin.right, scale),
        ZFUISizeApplyScale(margin.bottom, scale)
    };
    return ret;
}
/**
 * @brief return a scaled margin reversely
 */
ZFMETHOD_FUNC_INLINE_DECLARE_3(void, ZFUIMarginApplyScaleReversely,
                               ZFMP_OUT(ZFUIMargin &, ret),
                               ZFMP_IN(const ZFUIMargin &, margin),
                               ZFMP_IN(zffloat, scale))
{
    ret.left = ZFUISizeApplyScaleReversely(margin.left, scale);
    ret.top = ZFUISizeApplyScaleReversely(margin.top, scale);
    ret.right = ZFUISizeApplyScaleReversely(margin.right, scale);
    ret.bottom = ZFUISizeApplyScaleReversely(margin.bottom, scale);
}
/**
 * @brief return a scaled margin reversely
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFUIMargin, ZFUIMarginApplyScaleReversely,
                               ZFMP_IN(const ZFUIMargin &, margin),
                               ZFMP_IN(zffloat, scale))
{
    ZFUIMargin ret = {
        ZFUISizeApplyScaleReversely(margin.left, scale),
        ZFUISizeApplyScaleReversely(margin.top, scale),
        ZFUISizeApplyScaleReversely(margin.right, scale),
        ZFUISizeApplyScaleReversely(margin.bottom, scale)
    };
    return ret;
}
/**
 * @brief left + right margin
 */
ZFMETHOD_FUNC_INLINE_DECLARE_1(zffloat, ZFUIMarginGetWidth,
                               ZFMP_IN(const ZFUIMargin &, margin))
{
    return margin.left + margin.right;
}
/**
 * @brief top + bottom margin
 */
ZFMETHOD_FUNC_INLINE_DECLARE_1(zffloat, ZFUIMarginGetHeight,
                               ZFMP_IN(const ZFUIMargin &, margin))
{
    return margin.top + margin.bottom;
}
/**
 * @brief v0 + v1
 */
ZFMETHOD_FUNC_INLINE_DECLARE_3(void, ZFUIMarginInc,
                               ZFMP_IN_OUT(ZFUIMargin &, ret),
                               ZFMP_IN(const ZFUIMargin &, v0),
                               ZFMP_IN(const ZFUIMargin &, v1))
{
    ret.left = v0.left + v1.left;
    ret.top = v0.top + v1.top;
    ret.right = v0.right + v1.right;
    ret.bottom = v0.bottom + v1.bottom;
}
/**
 * @brief v0 + v1
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFUIMargin, ZFUIMarginInc,
                               ZFMP_IN(const ZFUIMargin &, v0),
                               ZFMP_IN(const ZFUIMargin &, v1))
{
    ZFUIMargin ret = {
        v0.left + v1.left,
        v0.top + v1.top,
        v0.right + v1.right,
        v0.bottom + v1.bottom
    };
    return ret;
}
/**
 * @brief v0 + v1
 */
inline ZFUIMargin operator + (ZF_IN const ZFUIMargin &v0,
                              ZF_IN const ZFUIMargin &v1)
{
    ZFUIMargin ret = {
        v0.left + v1.left,
        v0.top + v1.top,
        v0.right + v1.right,
        v0.bottom + v1.bottom
    };
    return ret;
}
/**
 * @brief v0 += v1
 */
inline ZFUIMargin &operator += (ZF_IN_OUT ZFUIMargin &v0,
                                ZF_IN const ZFUIMargin &v1)
{
    v0.left += v1.left;
    v0.top += v1.top;
    v0.right += v1.right;
    v0.bottom += v1.bottom;
    return v0;
}
/**
 * @brief v0 - v1
 */
ZFMETHOD_FUNC_INLINE_DECLARE_3(void, ZFUIMarginDec,
                               ZFMP_IN_OUT(ZFUIMargin &, ret),
                               ZFMP_IN(const ZFUIMargin &, v0),
                               ZFMP_IN(const ZFUIMargin &, v1))
{
    ret.left = v0.left - v1.left;
    ret.top = v0.top - v1.top;
    ret.right = v0.right - v1.right;
    ret.bottom = v0.bottom - v1.bottom;
}
/**
 * @brief v0 - v1
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFUIMargin, ZFUIMarginDec,
                               ZFMP_IN(const ZFUIMargin &, v0),
                               ZFMP_IN(const ZFUIMargin &, v1))
{
    ZFUIMargin ret = {
        v0.left - v1.left,
        v0.top - v1.top,
        v0.right - v1.right,
        v0.bottom - v1.bottom
    };
    return ret;
}
/**
 * @brief v0 - v1
 */
inline ZFUIMargin operator - (ZF_IN const ZFUIMargin &v0,
                              ZF_IN const ZFUIMargin &v1)
{
    ZFUIMargin ret = {
        v0.left - v1.left,
        v0.top - v1.top,
        v0.right - v1.right,
        v0.bottom - v1.bottom
    };
    return ret;
}
/**
 * @brief v0 -= v1
 */
inline ZFUIMargin &operator -= (ZF_IN_OUT ZFUIMargin &v0,
                                ZF_IN const ZFUIMargin &v1)
{
    v0.left -= v1.left;
    v0.top -= v1.top;
    v0.right -= v1.right;
    v0.bottom -= v1.bottom;
    return v0;
}

// ============================================================
// ZFUISize
/**
 * @brief 2D size
 */
zfclassPOD ZF_ENV_EXPORT ZFUISize
{
public:
    zffloat width;   /**< @brief width */
    zffloat height;  /**< @brief height */
};

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFUISize
 *       value="(0, 0)" // optional, 0 by default
 *   />
 * @endcode
 */
ZFTYPEID_DECLARE(ZFUISize, ZFUISize)

ZFOUTPUT_TYPE(ZFUISize, {output << ZFUISizeToString(v);})

ZFCORE_POD_COMPARER_DECLARE(ZFUISize)

/**
 * @brief (0, 0)
 */
ZFEXPORT_VAR_READONLY_DECLARE(ZFUISize, ZFUISizeZero)
/**
 * @brief (-1, -1)
 *
 * since zero size is valid on some cases,
 * we supply -1 as special size to indicate an invalid size
 */
ZFEXPORT_VAR_READONLY_DECLARE(ZFUISize, ZFUISizeInvalid)

/**
 * @brief make a ZFUISize
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFUISize, ZFUISizeMake,
                               ZFMP_IN(zffloat const &, width),
                               ZFMP_IN(zffloat const &, height))
{
    ZFUISize ret = {width, height};
    return ret;
}
/**
 * @brief make a ZFUISize
 */
ZFMETHOD_FUNC_INLINE_DECLARE_1(ZFUISize, ZFUISizeMake,
                               ZFMP_IN(zffloat const &, v))
{
    ZFUISize ret = {v, v};
    return ret;
}

/**
 * @brief check and apply size range
 *
 * return value in range [minSize, maxSize] if minSize < maxSize,
 * or return value in range [minSize, infinite) minSize > maxSize
 */
ZFMETHOD_FUNC_INLINE_DECLARE_4(void, ZFUISizeApplyRange,
                               ZFMP_OUT(ZFUISize &, ret),
                               ZFMP_IN(const ZFUISize &, orgSize),
                               ZFMP_IN(const ZFUISize &, minSize),
                               ZFMP_IN(const ZFUISize &, maxSize))
{
    ret.width = zfmApplyRange(orgSize.width, minSize.width, maxSize.width);
    ret.height = zfmApplyRange(orgSize.height, minSize.height, maxSize.height);
}
/**
 * @brief check and apply size range
 *
 * return value in range [minSize, maxSize] if minSize < maxSize,
 * or return value in range [minSize, infinite) minSize > maxSize
 */
ZFMETHOD_FUNC_INLINE_DECLARE_3(ZFUISize, ZFUISizeApplyRange,
                               ZFMP_IN(const ZFUISize &, orgSize),
                               ZFMP_IN(const ZFUISize &, minSize),
                               ZFMP_IN(const ZFUISize &, maxSize))
{
    ZFUISize ret = {
        zfmApplyRange(orgSize.width, minSize.width, maxSize.width),
        zfmApplyRange(orgSize.height, minSize.height, maxSize.height)
    };
    return ret;
}
/**
 * @brief return a scaled size
 */
ZFMETHOD_FUNC_INLINE_DECLARE_3(void, ZFUISizeApplyScale,
                               ZFMP_OUT(ZFUISize &, ret),
                               ZFMP_IN(const ZFUISize &, size),
                               ZFMP_IN(zffloat, scale))
{
    ret.width = ZFUISizeApplyScale(size.width, scale);
    ret.height = ZFUISizeApplyScale(size.height, scale);
}
/**
 * @brief return a scaled size
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFUISize, ZFUISizeApplyScale,
                               ZFMP_IN(const ZFUISize &, size),
                               ZFMP_IN(zffloat, scale))
{
    ZFUISize ret = {
        ZFUISizeApplyScale(size.width, scale),
        ZFUISizeApplyScale(size.height, scale)
    };
    return ret;
}
/**
 * @brief return a scaled size reversely
 */
ZFMETHOD_FUNC_INLINE_DECLARE_3(void, ZFUISizeApplyScaleReversely,
                               ZFMP_OUT(ZFUISize &, ret),
                               ZFMP_IN(const ZFUISize &, size),
                               ZFMP_IN(zffloat, scale))
{
    ret.width = ZFUISizeApplyScaleReversely(size.width, scale);
    ret.height = ZFUISizeApplyScaleReversely(size.height, scale);
}
/**
 * @brief return a scaled size reversely
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFUISize, ZFUISizeApplyScaleReversely,
                               ZFMP_IN(const ZFUISize &, size),
                               ZFMP_IN(zffloat, scale))
{
    ZFUISize ret = {
        ZFUISizeApplyScaleReversely(size.width, scale),
        ZFUISizeApplyScaleReversely(size.height, scale)
    };
    return ret;
}

/**
 * @brief scale size to fit refSize and keep aspect ratio
 *
 * input size must be valid, while negative refSize means no limit
 */
ZFMETHOD_FUNC_DECLARE_3(void, ZFUISizeApplyAspectRatio,
                        ZFMP_OUT(ZFUISize &, ret),
                        ZFMP_IN(const ZFUISize &, size),
                        ZFMP_IN(const ZFUISize &, refSize))
/**
 * @brief scale size to fit refSize and keep aspect ratio
 *
 * input size must be valid, while negative refSize means no limit
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFUISize, ZFUISizeApplyAspectRatio,
                               ZFMP_IN(const ZFUISize &, size),
                               ZFMP_IN(const ZFUISize &, refSize))
{
    ZFUISize ret = ZFUISizeZero();
    ZFUISizeApplyAspectRatio(ret, size, refSize);
    return ret;
}

/**
 * @brief return a size excluding the margin
 */
ZFMETHOD_FUNC_INLINE_DECLARE_3(void, ZFUISizeApplyMargin,
                               ZFMP_OUT(ZFUISize &, ret),
                               ZFMP_IN(const ZFUISize &, size),
                               ZFMP_IN(const ZFUIMargin &, margin))
{
    ret.width = size.width - margin.left - margin.right;
    ret.height = size.height - margin.top - margin.bottom;
}
/**
 * @brief return a size excluding the margin
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFUISize, ZFUISizeApplyMargin,
                               ZFMP_IN(const ZFUISize &, size),
                               ZFMP_IN(const ZFUIMargin &, margin))
{
    ZFUISize ret = {
        size.width - margin.left - margin.right,
        size.height - margin.top - margin.bottom
    };
    return ret;
}
/**
 * @brief revert excluding the margin
 */
ZFMETHOD_FUNC_INLINE_DECLARE_3(void, ZFUISizeApplyMarginReversely,
                               ZFMP_OUT(ZFUISize &, ret),
                               ZFMP_IN(const ZFUISize &, size),
                               ZFMP_IN(const ZFUIMargin &, margin))
{
    ret.width = size.width + margin.left + margin.right;
    ret.height = size.height + margin.top + margin.bottom;
}
/**
 * @brief revert excluding the margin
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFUISize, ZFUISizeApplyMarginReversely,
                               ZFMP_IN(const ZFUISize &, size),
                               ZFMP_IN(const ZFUIMargin &, margin))
{
    ZFUISize ret = {
        size.width + margin.left + margin.right,
        size.height + margin.top + margin.bottom
    };
    return ret;
}

// ============================================================
// ZFUIRect
/**
 * @brief 2D rectangle
 */
zfclassPOD ZF_ENV_EXPORT ZFUIRect
{
public:
    zffloat x; /**< @brief x */
    zffloat y; /**< @brief y */
    zffloat width; /**< @brief width */
    zffloat height; /**< @brief height */
};

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFUIRect
 *       value="(0, 0, 0, 0)" // optional, 0 by default
 *   />
 * @endcode
 */
ZFTYPEID_DECLARE(ZFUIRect, ZFUIRect)

ZFOUTPUT_TYPE(ZFUIRect, {output << ZFUIRectToString(v);})

ZFCORE_POD_COMPARER_DECLARE(ZFUIRect)

/**
 * @brief (0, 0, 0, 0)
 */
ZFEXPORT_VAR_READONLY_DECLARE(ZFUIRect, ZFUIRectZero)

/**
 * @brief make a ZFUIRect
 */
ZFMETHOD_FUNC_INLINE_DECLARE_4(ZFUIRect, ZFUIRectMake,
                               ZFMP_IN(zffloat const &, x),
                               ZFMP_IN(zffloat const &, y),
                               ZFMP_IN(zffloat const &, w),
                               ZFMP_IN(zffloat const &, h))
{
    ZFUIRect ret = {x, y, w, h};
    return ret;
}
/**
 * @brief make a ZFUIRect
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFUIRect, ZFUIRectMake,
                               ZFMP_IN(const ZFUIPoint &, point),
                               ZFMP_IN(const ZFUISize &, size))
{
    ZFUIRect ret = {point.x, point.y, size.width, size.height};
    return ret;
}

/**
 * @brief is rect empty
 *
 * rect is empty if width or height <= 0
 */
ZFMETHOD_FUNC_INLINE_DECLARE_1(zfbool, ZFUIRectIsEmpty,
                               ZFMP_IN(const ZFUIRect &, rect))
{
    return (rect.width <= 0 || rect.height <= 0);
}
/**
 * @brief union two rect, return a rect that contains both rect1 and rect2
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFUIRect, ZFUIRectUnion,
                               ZFMP_IN(const ZFUIRect &, rect1),
                               ZFMP_IN(const ZFUIRect &, rect2))
{
    ZFUIRect ret = {
        (rect1.x < rect2.x) ? rect1.x : rect2.x,
        (rect1.y < rect2.y) ? rect1.y : rect2.y,
        (rect1.x + rect1.width > rect2.x + rect2.width)
            ? (rect1.x + rect1.width)
            : (rect2.x + rect2.width),
        (rect1.y + rect1.height > rect2.y + rect2.height)
            ? (rect1.y + rect1.height)
            : (rect2.y + rect2.height)
    };
    if(ZFUIRectIsEmpty(ret))
    {
        return ZFUIRectZero();
    }
    else
    {
        return ret;
    }
}
/**
 * @brief intersect two rect, return #ZFUIRectZero if not intersected
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFUIRect, ZFUIRectIntersection,
                               ZFMP_IN(const ZFUIRect &, rect1),
                               ZFMP_IN(const ZFUIRect &, rect2))
{
    ZFUIRect ret = {
        (rect1.x > rect2.x) ? rect1.x : rect2.x,
        (rect1.y > rect2.y) ? rect1.y : rect2.y,
        (rect1.x + rect1.width < rect2.x + rect2.width)
            ? (rect1.x + rect1.width)
            : (rect2.x + rect2.width),
        (rect1.y + rect1.height < rect2.y + rect2.height)
            ? (rect1.y + rect1.height)
            : (rect2.y + rect2.height)
    };
    if(ZFUIRectIsEmpty(ret))
    {
        return ZFUIRectZero();
    }
    else
    {
        return ret;
    }
}
/**
 * @brief is two rect intersected,
 *   two tangented rect is not intersected
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(zfbool, ZFUIRectIsIntersectToRect,
                               ZFMP_IN(const ZFUIRect &, rect1),
                               ZFMP_IN(const ZFUIRect &, rect2))
{
    return !ZFUIRectIsEmpty(ZFUIRectIntersection(rect1, rect2));
}
/**
 * @brief is rect contains point
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(zfbool, ZFUIRectIsContainPoint,
                               ZFMP_IN(const ZFUIRect &, rect),
                               ZFMP_IN(const ZFUIPoint &, point))
{
    return (point.x >= rect.x
        && point.x <= rect.x + rect.width
        && point.y >= rect.y
        && point.y <= rect.y + rect.height);
}
/**
 * @brief see #ZFUIRectIsContainPoint
 *
 * if margin is positive, rect's area is reduced by margin,
 * if margin is negative, rect's area is increased by margin
 */
ZFMETHOD_FUNC_INLINE_DECLARE_3(zfbool, ZFUIRectIsContainPoint,
                               ZFMP_IN(const ZFUIRect &, rect),
                               ZFMP_IN(const ZFUIPoint &, point),
                               ZFMP_IN(const ZFUIMargin &, margin))
{
    return (point.x >= rect.x + margin.left
        && point.x <= rect.x + rect.width - margin.right
        && point.y >= rect.y + margin.top
        && point.y <= rect.y + rect.height - margin.bottom);
}
/**
 * @brief return true if rect2 is completely contained by rect1,
 *   two equal rect is regarded as contain each other
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(zfbool, ZFUIRectIsContainRect,
                               ZFMP_IN(const ZFUIRect &, rect1),
                               ZFMP_IN(const ZFUIRect &, rect2))
{
    return (rect2.x >= rect1.x
        && rect2.x + rect2.width <= rect1.x + rect1.width
        && rect2.y >= rect1.y
        && rect2.y + rect2.height <= rect1.y + rect1.height);
}
/**
 * @brief see #ZFUIRectIsContainRect
 *
 * if margin is positive, rect's area is reduced by margin,
 * if margin is negative, rect's area is increased by margin
 */
ZFMETHOD_FUNC_INLINE_DECLARE_3(zfbool, ZFUIRectIsContainRect,
                               ZFMP_IN(const ZFUIRect &, rect1),
                               ZFMP_IN(const ZFUIRect &, rect2),
                               ZFMP_IN(const ZFUIMargin &, margin))
{
    return (rect2.x >= rect1.x + margin.left
        && rect2.x + rect2.width <= rect1.x + rect1.width - margin.right
        && rect2.y >= rect1.y + margin.top
        && rect2.y + rect2.height <= rect1.y + rect1.height - margin.bottom);
}
/**
 * @brief return a scaled rect
 */
ZFMETHOD_FUNC_INLINE_DECLARE_3(void, ZFUIRectApplyScale,
                               ZFMP_OUT(ZFUIRect &, ret),
                               ZFMP_IN(const ZFUIRect &, rect),
                               ZFMP_IN(zffloat, scale))
{
    ret.x = ZFUISizeApplyScale(rect.x, scale);
    ret.y = ZFUISizeApplyScale(rect.y, scale);
    ret.width = ZFUISizeApplyScale(rect.width, scale);
    ret.height = ZFUISizeApplyScale(rect.height, scale);
}
/**
 * @brief return a scaled rect
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFUIRect, ZFUIRectApplyScale,
                               ZFMP_IN(const ZFUIRect &, rect),
                               ZFMP_IN(zffloat, scale))
{
    ZFUIRect ret = {
        ZFUISizeApplyScale(rect.x, scale),
        ZFUISizeApplyScale(rect.y, scale),
        ZFUISizeApplyScale(rect.width, scale),
        ZFUISizeApplyScale(rect.height, scale)
    };
    return ret;
}
/**
 * @brief return a scaled rect reversely
 */
ZFMETHOD_FUNC_INLINE_DECLARE_3(void, ZFUIRectApplyScaleReversely,
                               ZFMP_OUT(ZFUIRect &, ret),
                               ZFMP_IN(const ZFUIRect &, rect),
                               ZFMP_IN(zffloat, scale))
{
    ret.x = ZFUISizeApplyScaleReversely(rect.x, scale);
    ret.y = ZFUISizeApplyScaleReversely(rect.y, scale);
    ret.width = ZFUISizeApplyScaleReversely(rect.width, scale);
    ret.height = ZFUISizeApplyScaleReversely(rect.height, scale);
}
/**
 * @brief return a scaled rect reversely
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFUIRect, ZFUIRectApplyScaleReversely,
                               ZFMP_IN(const ZFUIRect &, rect),
                               ZFMP_IN(zffloat, scale))
{
    ZFUIRect ret = {
        ZFUISizeApplyScaleReversely(rect.x, scale),
        ZFUISizeApplyScaleReversely(rect.y, scale),
        ZFUISizeApplyScaleReversely(rect.width, scale),
        ZFUISizeApplyScaleReversely(rect.height, scale)
    };
    return ret;
}
/**
 * @brief get bounds for rect, i.e. (0, 0, width, height)
 */
ZFMETHOD_FUNC_INLINE_DECLARE_1(ZFUIRect, ZFUIRectGetBounds,
                               ZFMP_IN(const ZFUIRect &, rect))
{
    ZFUIRect ret = {0, 0, rect.width, rect.height};
    return ret;
}
/**
 * @brief get size of the rect
 */
ZFMETHOD_FUNC_INLINE_DECLARE_1(ZFUISize, ZFUIRectGetSize,
                               ZFMP_IN(const ZFUIRect &, rect))
{
    ZFUISize ret = {rect.width, rect.height};
    return ret;
}
/**
 * @brief get center point of the rect
 */
ZFMETHOD_FUNC_INLINE_DECLARE_1(ZFUIPoint, ZFUIRectGetCenter,
                               ZFMP_IN(const ZFUIRect &, rect))
{
    ZFUIPoint ret = {rect.x + rect.width / 2, rect.y + rect.height / 2};
    return ret;
}
/**
 * @brief get left edge of the rect
 */
ZFMETHOD_FUNC_INLINE_DECLARE_1(zffloat, ZFUIRectGetLeft,
                               ZFMP_IN(const ZFUIRect &, rect))
{
    return rect.x;
}
/**
 * @brief get top edge of the rect
 */
ZFMETHOD_FUNC_INLINE_DECLARE_1(zffloat, ZFUIRectGetTop,
                               ZFMP_IN(const ZFUIRect &, rect))
{
    return rect.y;
}
/**
 * @brief get right edge of the rect
 */
ZFMETHOD_FUNC_INLINE_DECLARE_1(zffloat, ZFUIRectGetRight,
                               ZFMP_IN(const ZFUIRect &, rect))
{
    return (rect.x + rect.width);
}
/**
 * @brief get bottom edge of the rect
 */
ZFMETHOD_FUNC_INLINE_DECLARE_1(zffloat, ZFUIRectGetBottom,
                               ZFMP_IN(const ZFUIRect &, rect))
{
    return (rect.y + rect.height);
}

/**
 * @brief return a rect excluding the margin
 */
ZFMETHOD_FUNC_INLINE_DECLARE_3(void, ZFUIRectApplyMargin,
                               ZFMP_OUT(ZFUIRect &, ret),
                               ZFMP_IN(const ZFUIRect &, rect),
                               ZFMP_IN(const ZFUIMargin &, margin))
{
    ret.x = rect.x + margin.left;
    ret.y = rect.y + margin.top;
    ret.width = rect.width - margin.left - margin.right;
    ret.height = rect.height - margin.top - margin.bottom;
}
/**
 * @brief return a rect excluding the margin
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFUIRect, ZFUIRectApplyMargin,
                               ZFMP_IN(const ZFUIRect &, rect),
                               ZFMP_IN(const ZFUIMargin &, margin))
{
    ZFUIRect ret = {
        rect.x + margin.left,
        rect.y + margin.top,
        rect.width - margin.left - margin.right,
        rect.height - margin.top - margin.bottom
    };
    return ret;
}
/**
 * @brief revert excluding the margin
 */
ZFMETHOD_FUNC_INLINE_DECLARE_3(void, ZFUIRectApplyMarginReversely,
                               ZFMP_OUT(ZFUIRect &, ret),
                               ZFMP_IN(const ZFUIRect &, rect),
                               ZFMP_IN(const ZFUIMargin &, margin))
{
    ret.x = rect.x - margin.left;
    ret.y = rect.y - margin.top;
    ret.width = rect.width + margin.left + margin.right;
    ret.height = rect.height + margin.top + margin.bottom;
}
/**
 * @brief revert excluding the margin
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFUIRect, ZFUIRectApplyMarginReversely,
                               ZFMP_IN(const ZFUIRect &, rect),
                               ZFMP_IN(const ZFUIMargin &, margin))
{
    ZFUIRect ret = {
        rect.x - margin.left,
        rect.y - margin.top,
        rect.width + margin.left + margin.right,
        rect.height + margin.top + margin.bottom
    };
    return ret;
}

// ============================================================
// ZFUIAlignFlags
/**
 * @brief align types for UI in ZFFramework
 * @see ZFUIAlignFlags
 */
ZFENUM_BEGIN(ZFUIAlign)
    ZFENUM_VALUE_WITH_INIT(Center, 0) /**< @brief ensured 0 */
    ZFENUM_VALUE_WITH_INIT(Left, 1)
    ZFENUM_VALUE_WITH_INIT(LeftInner, 2)
    ZFENUM_VALUE_WITH_INIT(Top, 4)
    ZFENUM_VALUE_WITH_INIT(TopInner, 8)
    ZFENUM_VALUE_WITH_INIT(Right, 16)
    ZFENUM_VALUE_WITH_INIT(RightInner, 32)
    ZFENUM_VALUE_WITH_INIT(Bottom, 64)
    ZFENUM_VALUE_WITH_INIT(BottomInner, 128)
ZFENUM_SEPARATOR(ZFUIAlign)
    ZFENUM_VALUE_REGISTER(Center)
    ZFENUM_VALUE_REGISTER(Left)
    ZFENUM_VALUE_REGISTER(LeftInner)
    ZFENUM_VALUE_REGISTER(Top)
    ZFENUM_VALUE_REGISTER(TopInner)
    ZFENUM_VALUE_REGISTER(Right)
    ZFENUM_VALUE_REGISTER(RightInner)
    ZFENUM_VALUE_REGISTER(Bottom)
    ZFENUM_VALUE_REGISTER(BottomInner)
ZFENUM_END_FLAGS(ZFUIAlign, ZFUIAlignFlags)

/**
 * @brief true if align is valid
 *
 * a align is valid only if:
 * -  only bits in #ZFUIAlign are set
 * -  x or y align type must not conflict, i.e.
 *   "Left, LeftInner, Right, RightInner" could only exist at most one,
 *   "Top, TopInner, Bottom, BottomInner" could only exist at most one
 */
ZFMETHOD_FUNC_DECLARE_1(zfbool, ZFUIAlignIsValid,
                        ZFMP_IN(const ZFUIAlignFlags &, align))
/**
 * @brief apply align and calculate result, or #ZFUIRectZero if error
 */
ZFMETHOD_FUNC_DECLARE_5(void, ZFUIAlignApply,
                        ZFMP_OUT(ZFUIRect &, ret),
                        ZFMP_IN(const ZFUIAlignFlags &, align),
                        ZFMP_IN(const ZFUIRect &, refRect),
                        ZFMP_IN(const ZFUISize &, itemSize),
                        ZFMP_IN_OPT(const ZFUIMargin &, margin, ZFUIMarginZero()))
/**
 * @brief apply align and calculate result, or #ZFUIRectZero if error
 */
ZFMETHOD_FUNC_INLINE_DECLARE_4(ZFUIRect, ZFUIAlignApply,
                               ZFMP_IN(const ZFUIAlignFlags &, align),
                               ZFMP_IN(const ZFUIRect &, refRect),
                               ZFMP_IN(const ZFUISize &, itemSize),
                               ZFMP_IN_OPT(const ZFUIMargin &, margin, ZFUIMarginZero()))
{
    ZFUIRect ret = ZFUIRectZero();
    ZFUIAlignApply(ret, align, refRect, itemSize, margin);
    return ret;
}
/**
 * @brief get horizontal align or #ZFUIAlign::e_Center if invalid
 */
ZFMETHOD_FUNC_DECLARE_1(ZFUIAlignEnum, ZFUIAlignGetX,
                        ZFMP_IN(const ZFUIAlignFlags &, align))
/**
 * @brief get vertical align or #ZFUIAlign::e_Center if invalid
 */
ZFMETHOD_FUNC_DECLARE_1(ZFUIAlignEnum, ZFUIAlignGetY,
                        ZFMP_IN(const ZFUIAlignFlags &, align))

// ============================================================
// ZFUIOrientation
/**
 * @brief orientation types for UI in ZFFramework
 *
 * for example, for a mobile phone,
 * Top means user hold the phone normally,
 * Left means user rotate the phone 90 degrees counter clockwise,
 * Right means user rotate the phone 90 degrees clockwise,
 * and Bottom means user rotate the phone 180 degrees
 */
ZFENUM_BEGIN(ZFUIOrientation)
    ZFENUM_VALUE_WITH_INIT(Left, 1)
    ZFENUM_VALUE_WITH_INIT(Top, 2)
    ZFENUM_VALUE_WITH_INIT(Right, 4)
    ZFENUM_VALUE_WITH_INIT(Bottom, 8)
ZFENUM_SEPARATOR(ZFUIOrientation)
    ZFENUM_VALUE_REGISTER(Left)
    ZFENUM_VALUE_REGISTER(Top)
    ZFENUM_VALUE_REGISTER(Right)
    ZFENUM_VALUE_REGISTER(Bottom)
ZFENUM_END_FLAGS(ZFUIOrientation, ZFUIOrientationFlags)

/** @brief (ZFUIOrientation::e_Left | ZFUIOrientation::e_Top | ZFUIOrientation::e_Right | ZFUIOrientation::e_Bottom) */
ZFEXPORT_VAR_READONLY_DECLARE(ZFUIOrientationFlags, ZFUIOrientationFlagsAll)
/** @brief (ZFUIOrientation::e_Left | ZFUIOrientation::e_Right) */
ZFEXPORT_VAR_READONLY_DECLARE(ZFUIOrientationFlags, ZFUIOrientationFlagsHorizontal)
/** @brief (ZFUIOrientation::e_Top | ZFUIOrientation::e_Bottom) */
ZFEXPORT_VAR_READONLY_DECLARE(ZFUIOrientationFlags, ZFUIOrientationFlagsVertical)

/**
 * @brief true if vertical orientation
 */
ZFMETHOD_FUNC_INLINE_DECLARE_1(zfbool, ZFUIOrientationIsVertical,
                               ZFMP_IN(ZFUIOrientationEnum, orientation))
{
    return (orientation == ZFUIOrientation::e_Top || orientation == ZFUIOrientation::e_Bottom);
}
/**
 * @brief true if vertical orientation
 */
ZFMETHOD_FUNC_INLINE_DECLARE_1(zfbool, ZFUIOrientationIsHorizontal,
                               ZFMP_IN(ZFUIOrientationEnum, orientation))
{
    return (orientation == ZFUIOrientation::e_Left || orientation == ZFUIOrientation::e_Right);
}

// ============================================================
// ZFUIColor
/**
 * @brief color with `AARRGGBB` format
 *
 * note, color is ensured as POD type,
 * but the actual color type is not ensured,
 * use float to make and store a/r/g/b element,
 * and use string converter (#ZFUIColorToString) if necessary\n
 * for now, the color is 32bit integer with `AARRGGBB` format,
 * but may extend to 64bit for future update
 */
ZFT_INT_STRONG(zft_zfuint32, ZFUIColor)

/** @brief zero color */
ZFEXPORT_VAR_READONLY_DECLARE(ZFUIColor, ZFUIColorZero)

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFUIColor
 *       value="value" // optional, 0 by default
 *   />
 * @endcode
 *
 * valid value format:
 * -  "AARRGGBB"
 * -  "RRGGBB"
 * -  "ARGB"
 * -  "RGB"
 *
 * valid prefix:
 * -  empty ("AARRGGBB")
 * -  "#" ("#AARRGGBB")
 * -  "0x" ("0xAARRGGBB")
 *
 * when serialize back to string,
 * it's ensured to use "#" as prefix,
 * and will try to use the shortest form if available
 * (e.g. "RGB" instead of "AARRGGBB")
 */
ZFTYPEID_DECLARE(ZFUIColor, ZFUIColor)

ZFOUTPUT_TYPE(ZFUIColor, {output << ZFUIColorToString(v);})

/**
 * @brief true if two color is equal
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(zfbool, ZFUIColorIsEqual,
                               ZFMP_IN(ZFUIColor const &, v0),
                               ZFMP_IN(ZFUIColor const &, v1))
{
    return (v0 == v1);
}

/** @cond ZFPrivateDoc */
#define _ZFP_ZFUIColorMake(r, g, b, a, ...) \
    ZFUIColor( \
          (((zfuint)((a) * 0xFF) & 0xFF) << 24) \
        | (((zfuint)((r) * 0xFF) & 0xFF) << 16) \
        | (((zfuint)((g) * 0xFF) & 0xFF) << 8) \
        | ((zfuint)((b) * 0xFF) & 0xFF))
#define ZFUIColorMake(r, g, b, ...) _ZFP_ZFUIColorMake(r, g, b, ##__VA_ARGS__, 1.0f)

#define ZFUIColorMakeARGB(argb) ZFUIColor((zft_ZFUIColor)(argb))
#define ZFUIColorMakeRGB(rgb) ZFUIColor((zft_ZFUIColor)(0xFF000000 | (rgb)))

#define ZFUIColorGetA(c) ((zffloat)((((c) >> 24) & 0xFF) / (zft_zffloat)0xFF))
#define ZFUIColorGetR(c) ((zffloat)((((c) >> 16) & 0xFF) / (zft_zffloat)0xFF))
#define ZFUIColorGetG(c) ((zffloat)((((c) >>  8) & 0xFF) / (zft_zffloat)0xFF))
#define ZFUIColorGetB(c) ((zffloat)((((c) >>  0) & 0xFF) / (zft_zffloat)0xFF))
#define ZFUIColorWithA(c, v) ((zft_ZFUIColor)(((c) & 0x00FFFFFF) | (((zfuint)((v) * 0xFF) & 0xFF) << 24)))
#define ZFUIColorWithR(c, v) ((zft_ZFUIColor)(((c) & 0xFF00FFFF) | (((zfuint)((v) * 0xFF) & 0xFF) << 16)))
#define ZFUIColorWithG(c, v) ((zft_ZFUIColor)(((c) & 0xFFFF00FF) | (((zfuint)((v) * 0xFF) & 0xFF) <<  8)))
#define ZFUIColorWithB(c, v) ((zft_ZFUIColor)(((c) & 0xFFFFFF00) | (((zfuint)((v) * 0xFF) & 0xFF) <<  0)))
#define ZFUIColorSetA(c, v) ((c) = ZFUIColorWithA(c, v))
#define ZFUIColorSetR(c, v) ((c) = ZFUIColorWithR(c, v))
#define ZFUIColorSetG(c, v) ((c) = ZFUIColorWithG(c, v))
#define ZFUIColorSetB(c, v) ((c) = ZFUIColorWithB(c, v))
/** @endcond */

/**
 * @brief make a color
 */
ZFMETHOD_FUNC_INLINE_DECLARE_4(ZFUIColor, ZFUIColorMake,
                               ZFMP_IN(zffloat, r),
                               ZFMP_IN(zffloat, g),
                               ZFMP_IN(zffloat, b),
                               ZFMP_IN_OPT(zffloat, a, 1.0f))
{
    return ZFUIColorMake(r, g, b, a);
}

/**
 * @brief get alpha from a #ZFUIColor as zffloat
 */
ZFMETHOD_FUNC_INLINE_DECLARE_1(zffloat, ZFUIColorGetA,
                               ZFMP_IN(ZFUIColor const &, c))
{
    return ZFUIColorGetA(c);
}
/**
 * @brief get red from a #ZFUIColor as zffloat
 */
ZFMETHOD_FUNC_INLINE_DECLARE_1(zffloat, ZFUIColorGetR,
                               ZFMP_IN(ZFUIColor const &, c))
{
    return ZFUIColorGetR(c);
}
/**
 * @brief get green from a #ZFUIColor as zffloat
 */
ZFMETHOD_FUNC_INLINE_DECLARE_1(zffloat, ZFUIColorGetG,
                               ZFMP_IN(ZFUIColor const &, c))
{
    return ZFUIColorGetG(c);
}
/**
 * @brief get blue from a #ZFUIColor as zffloat
 */
ZFMETHOD_FUNC_INLINE_DECLARE_1(zffloat, ZFUIColorGetB,
                               ZFMP_IN(ZFUIColor const &, c))
{
    return ZFUIColorGetB(c);
}

/**
 * @brief set alpha to a #ZFUIColor
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFUIColor &, ZFUIColorSetA,
                               ZFMP_IN_OUT(ZFUIColor &, c),
                               ZFMP_IN(zffloat, a))
{
    return ZFUIColorSetA(c, a);
}
/**
 * @brief set red from a #ZFUIColor
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFUIColor &, ZFUIColorSetR,
                               ZFMP_IN_OUT(ZFUIColor &, c),
                               ZFMP_IN(zffloat, r))
{
    return ZFUIColorSetR(c, r);
}
/**
 * @brief set green from a #ZFUIColor
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFUIColor &, ZFUIColorSetG,
                               ZFMP_IN_OUT(ZFUIColor &, c),
                               ZFMP_IN(zffloat, g))
{
    return ZFUIColorSetG(c, g);
}
/**
 * @brief set blue from a #ZFUIColor
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFUIColor &, ZFUIColorSetB,
                               ZFMP_IN_OUT(ZFUIColor &, c),
                               ZFMP_IN(zffloat, b))
{
    return ZFUIColorSetB(c, b);
}

/**
 * @brief return a copy of color with changed alpha
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFUIColor, ZFUIColorWithA,
                               ZFMP_IN(ZFUIColor const &, c),
                               ZFMP_IN(zffloat, a))
{
    return ZFUIColorWithA(c, a);
}
/**
 * @brief return a copy of color with changed red
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFUIColor, ZFUIColorWithR,
                               ZFMP_IN(ZFUIColor const &, c),
                               ZFMP_IN(zffloat, r))
{
    return ZFUIColorWithR(c, r);
}
/**
 * @brief return a copy of color with changed green
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFUIColor, ZFUIColorWithG,
                               ZFMP_IN(ZFUIColor const &, c),
                               ZFMP_IN(zffloat, g))
{
    return ZFUIColorWithG(c, g);
}
/**
 * @brief return a copy of color with changed blue
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFUIColor, ZFUIColorWithB,
                               ZFMP_IN(ZFUIColor const &, c),
                               ZFMP_IN(zffloat, b))
{
    return ZFUIColorWithB(c, b);
}

/**
 * @brief create a random color with specified alpha
 */
ZFMETHOD_FUNC_INLINE_DECLARE_1(ZFUIColor, ZFUIColorRandom,
                               ZFMP_IN_OPT(zffloat, alpha, 1.0f))
{
    return ZFUIColorMake(zfmRand(0xFF) / (zft_zffloat)0xFF, zfmRand(0xFF) / (zft_zffloat)0xFF, zfmRand(0xFF) / (zft_zffloat)0xFF, alpha);
}

// ============================================================
// ZFUITextAppearance
/**
 * @brief text appearance type, normal, bold or italic
 */
ZFENUM_BEGIN(ZFUITextAppearance)
    ZFENUM_VALUE(Normal) /**< @brief normal */
    ZFENUM_VALUE(Bold) /**< @brief bold */
    ZFENUM_VALUE(Italic) /**< @brief italic */
    ZFENUM_VALUE(BoldItalic) /**< @brief bold and italic */
ZFENUM_SEPARATOR(ZFUITextAppearance)
    ZFENUM_VALUE_REGISTER(Normal)
    ZFENUM_VALUE_REGISTER(Bold)
    ZFENUM_VALUE_REGISTER(Italic)
    ZFENUM_VALUE_REGISTER(BoldItalic)
ZFENUM_END(ZFUITextAppearance)

// ============================================================
// ZFUITextTruncateMode
/**
 * @brief text truncate mode
 */
ZFENUM_BEGIN(ZFUITextTruncateMode)
    ZFENUM_VALUE(Disable) /**< @brief never truncate */
    ZFENUM_VALUE(Head) /**< @brief truncate at head */
    ZFENUM_VALUE(Middle) /**< @brief truncate at middle */
    ZFENUM_VALUE(Tail) /**< @brief truncate at end */
ZFENUM_SEPARATOR(ZFUITextTruncateMode)
    ZFENUM_VALUE_REGISTER(Disable)
    ZFENUM_VALUE_REGISTER(Head)
    ZFENUM_VALUE_REGISTER(Middle)
    ZFENUM_VALUE_REGISTER(Tail)
ZFENUM_END(ZFUITextTruncateMode)

// ============================================================
/**
 * @brief scale type when stretch items
 */
ZFENUM_BEGIN(ZFUIContentScaleType)
    /**
     * @brief simply fill to parent
     */
    ZFENUM_VALUE(Fill)
    /**
     * @brief simply center the content without scale,
     *   may clip if content too large
     */
    ZFENUM_VALUE(Center)
    /**
     * @brief always keep aspect ratio and fill to min bounds,
     *   may leave blank area if aspect ratio differs from parent
     */
    ZFENUM_VALUE(FillCenter)
    /**
     * @brief always keep aspect ratio and fill to max bounds,
     *   may clip if aspect ratio differs from parent
     */
    ZFENUM_VALUE(FillCenterClipped)
    /**
     * @brief same as #e_Center if content not exceeds parent,
     *   same as #e_FillCenter if content exceeds parent
     */
    ZFENUM_VALUE(FitCenter)
    /**
     * @brief always fill x, may clip or leave blank if aspect ratio differs from parent
     */
    ZFENUM_VALUE(FillX)
    /**
     * @brief same as #e_Center if x not exceeds parent,
     *   same as #e_FillX if x exceeds parent
     */
    ZFENUM_VALUE(FitX)
    /**
     * @brief always fill y, may clip or leave blank if aspect ratio differs from parent
     */
    ZFENUM_VALUE(FillY)
    /**
     * @brief same as #e_Center if y not exceeds parent,
     *   same as #e_FillY if y exceeds parent
     */
    ZFENUM_VALUE(FitY)
ZFENUM_SEPARATOR(ZFUIContentScaleType)
    ZFENUM_VALUE_REGISTER(Fill)
    ZFENUM_VALUE_REGISTER(Center)
    ZFENUM_VALUE_REGISTER(FillCenter)
    ZFENUM_VALUE_REGISTER(FillCenterClipped)
    ZFENUM_VALUE_REGISTER(FitCenter)
    ZFENUM_VALUE_REGISTER(FillX)
    ZFENUM_VALUE_REGISTER(FitX)
    ZFENUM_VALUE_REGISTER(FillY)
    ZFENUM_VALUE_REGISTER(FitY)
ZFENUM_END(ZFUIContentScaleType)

/**
 * @brief apply scale for #ZFUIContentScaleType
 */
ZFMETHOD_FUNC_DECLARE_4(void, ZFUIContentScaleTypeApply,
                        ZFMP_OUT(ZFUIRect &, ret),
                        ZFMP_IN(ZFUIContentScaleTypeEnum, scaleType),
                        ZFMP_IN(const ZFUIRect &, bounds),
                        ZFMP_IN(const ZFUISize &, contentSize))
/**
 * @brief apply scale for #ZFUIContentScaleType
 */
ZFMETHOD_FUNC_INLINE_DECLARE_3(ZFUIRect, ZFUIContentScaleTypeApply,
                               ZFMP_IN(ZFUIContentScaleTypeEnum, scaleType),
                               ZFMP_IN(const ZFUIRect &, bounds),
                               ZFMP_IN(const ZFUISize &, contentSize))
{
    ZFUIRect ret = ZFUIRectZero();
    ZFUIContentScaleTypeApply(ret, scaleType, bounds, contentSize);
    return ret;
}

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUITypeDef_h_

#include "ZFUIColor_common.h"

