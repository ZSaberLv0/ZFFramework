/**
 * @file ZFBezier.h
 * @brief bezier curve
 */

#ifndef _ZFI_ZFBezier_h_
#define _ZFI_ZFBezier_h_

#include "ZFAlgorithmDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief bezier curve
 */
zffinal zfclassLikePOD ZF_ENV_EXPORT ZFBezier
{
public:
    /** @brief construct a linear curve */
    ZFBezier(void)
    {
        this->controlPoint(0, 0, 1, 1);
    }
    /** @brief construct from another bezier */
    ZFBezier(ZF_IN const ZFBezier &ref)
    : p0x(ref.p0x), p0y(ref.p0y), p1x(ref.p1x), p1y(ref.p1y)
    , ax(ref.ax), bx(ref.bx), cx(ref.cx), ay(ref.ay), by(ref.by), cy(ref.cy)
    {
    }
    /** @brief construct with control point */
    ZFBezier(ZF_IN zffloat p0x, ZF_IN zffloat p0y,
             ZF_IN zffloat p1x, ZF_IN zffloat p1y)
    {
        this->controlPoint(p0x, p0y, p1x, p1y);
    }
    /** @cond ZFPrivateDoc */
    ZFBezier &operator = (ZF_IN const ZFBezier &ref)
    {
        this->p0x = ref.p0x;
        this->p0y = ref.p0y;
        this->p1x = ref.p1x;
        this->p1y = ref.p1y;
        this->ax = ref.ax;
        this->bx = ref.bx;
        this->cx = ref.cx;
        this->ay = ref.ay;
        this->by = ref.by;
        this->cy = ref.cy;
        return *this;
    }
    zfbool operator == (ZF_IN const ZFBezier &ref) const
    {
        return (this->p0x == ref.p0x
            && this->p0y == ref.p0y
            && this->p1x == ref.p1x
            && this->p1y == ref.p1y
            );
    }
    inline zfbool operator != (ZF_IN const ZFBezier &ref) const {return !this->operator == (ref);}
    /** @endcond */

public:
    /** @brief change control point */
    ZFBezier &controlPoint(ZF_IN zffloat p0x, ZF_IN zffloat p0y,
                           ZF_IN zffloat p1x, ZF_IN zffloat p1y);

public:
    /**
     * @brief solve y with given t
     *
     * t must in range [0, 1]
     */
    zffloat y_by_t(ZF_IN zffloat t) const;

    /**
     * @brief solve y with given x
     *
     * value would be invalid if out of range [0, 1]
     */
    zffloat y_by_x(ZF_IN zffloat x) const;

    /**
     * @brief solve x with given t
     *
     * t must in range [0, 1]
     */
    zffloat x_by_t(ZF_IN zffloat t) const;

    /**
     * @brief solve x with given y
     *
     * value would be invalid if out of range [0, 1]
     */
    zffloat x_by_y(ZF_IN zffloat y) const;

    /**
     * @brief solve t with given x
     *
     * value would be invalid if out of range [0, 1]
     */
    zffloat t_by_x(ZF_IN zffloat x) const;

    /**
     * @brief solve t with given y
     *
     * value would be invalid if out of range [0, 1]
     */
    zffloat t_by_y(ZF_IN zffloat y) const;

public:
    zffloat p0x; /**< @brief control point */
    zffloat p0y; /**< @brief control point */
    zffloat p1x; /**< @brief control point */
    zffloat p1y; /**< @brief control point */
private:
    zffloat ax;
    zffloat bx;
    zffloat cx;

    zffloat ay;
    zffloat by;
    zffloat cy;
};

// ============================================================
/**
 * @brief linear bezier
 */
ZFEXPORT_VAR_READONLY_DECLARE(ZFBezier, ZFBezierZero)
/**
 * @brief linear bezier
 */
ZFEXPORT_VAR_READONLY_DECLARE(ZFBezier, ZFBezierLinear)
/**
 * @brief ease in bezier
 */
ZFEXPORT_VAR_READONLY_DECLARE(ZFBezier, ZFBezierEaseIn)
/**
 * @brief ease out bezier
 */
ZFEXPORT_VAR_READONLY_DECLARE(ZFBezier, ZFBezierEaseOut)
/**
 * @brief ease in out bezier
 */
ZFEXPORT_VAR_READONLY_DECLARE(ZFBezier, ZFBezierEaseInOut)
/**
 * @brief bounce in bezier
 */
ZFEXPORT_VAR_READONLY_DECLARE(ZFBezier, ZFBezierBounceIn)
/**
 * @brief bounce out bezier
 */
ZFEXPORT_VAR_READONLY_DECLARE(ZFBezier, ZFBezierBounceOut)
/**
 * @brief bounce in out bezier
 */
ZFEXPORT_VAR_READONLY_DECLARE(ZFBezier, ZFBezierBounceInOut)

// ============================================================
/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFBezier
 *       p0x="0" // optional, 0 by default
 *       p0y="0" // optional, 0 by default
 *       p1x="1" // optional, 1 by default
 *       p1y="1" // optional, 1 by default
 *   />
 * @endcode
 *
 * string data:
 * @code
 *   (p0x, p0y, p1x, p1y)
 * @endcode
 */
ZFTYPEID_DECLARE(ZFBezier, ZFBezier)

/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFBezier_p0x "p0x"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFBezier_p0y "p0y"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFBezier_p1x "p1x"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFBezier_p1y "p1y"

// ============================================================
ZFOUTPUT_TYPE(ZFBezier, {output << ZFBezierToString(v);})

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFBezier_h_

