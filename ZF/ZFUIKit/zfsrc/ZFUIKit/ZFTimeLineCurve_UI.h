/**
 * @file ZFTimeLineCurve_UI.h
 * @brief see #ZFTimeLineCurve
 */

#ifndef _ZFI_ZFTimeLineCurve_UI_h_
#define _ZFI_ZFTimeLineCurve_UI_h_

#include "ZFUITypeDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief ease in time line, see #ZFTimeLineCurve
 */
zfclass ZF_ENV_EXPORT ZFTimeLineCurveEaseIn : zfextends ZFTimeLineCurve
{
    ZFOBJECT_DECLARE(ZFTimeLineCurveEaseIn, ZFTimeLineCurve)
    ZFOBJECT_SINGLETON_DECLARE(zfself, instance)

protected:
    zfoverride
    virtual zffloat progressOnUpdate(ZF_IN zffloat time)
    {
        return ZFBezierEaseIn().y_by_x(time);
    }
};

// ============================================================
/**
 * @brief ease in time line, see #ZFTimeLineCurve
 */
zfclass ZF_ENV_EXPORT ZFTimeLineCurveEaseOut : zfextends ZFTimeLineCurve
{
    ZFOBJECT_DECLARE(ZFTimeLineCurveEaseOut, ZFTimeLineCurve)
    ZFOBJECT_SINGLETON_DECLARE(zfself, instance)

protected:
    zfoverride
    virtual zffloat progressOnUpdate(ZF_IN zffloat time)
    {
        return ZFBezierEaseOut().y_by_x(time);
    }
};

// ============================================================
/**
 * @brief ease in time line, see #ZFTimeLineCurve
 */
zfclass ZF_ENV_EXPORT ZFTimeLineCurveEaseInOut : zfextends ZFTimeLineCurve
{
    ZFOBJECT_DECLARE(ZFTimeLineCurveEaseInOut, ZFTimeLineCurve)
    ZFOBJECT_SINGLETON_DECLARE(zfself, instance)

protected:
    zfoverride
    virtual zffloat progressOnUpdate(ZF_IN zffloat time)
    {
        return ZFBezierEaseInOut().y_by_x(time);
    }
};

// ============================================================
/**
 * @brief ease in time line, see #ZFTimeLineCurve
 */
zfclass ZF_ENV_EXPORT ZFTimeLineCurveBounceIn : zfextends ZFTimeLineCurve
{
    ZFOBJECT_DECLARE(ZFTimeLineCurveBounceIn, ZFTimeLineCurve)
    ZFOBJECT_SINGLETON_DECLARE(zfself, instance)

protected:
    zfoverride
    virtual zffloat progressOnUpdate(ZF_IN zffloat time)
    {
        return ZFBezierBounceIn().y_by_x(time);
    }
};

// ============================================================
/**
 * @brief ease in time line, see #ZFTimeLineCurve
 */
zfclass ZF_ENV_EXPORT ZFTimeLineCurveBounceOut : zfextends ZFTimeLineCurve
{
    ZFOBJECT_DECLARE(ZFTimeLineCurveBounceOut, ZFTimeLineCurve)
    ZFOBJECT_SINGLETON_DECLARE(zfself, instance)

protected:
    zfoverride
    virtual zffloat progressOnUpdate(ZF_IN zffloat time)
    {
        return ZFBezierBounceOut().y_by_x(time);
    }
};

// ============================================================
/**
 * @brief ease in time line, see #ZFTimeLineCurve
 */
zfclass ZF_ENV_EXPORT ZFTimeLineCurveBounceInOut : zfextends ZFTimeLineCurve
{
    ZFOBJECT_DECLARE(ZFTimeLineCurveBounceInOut, ZFTimeLineCurve)
    ZFOBJECT_SINGLETON_DECLARE(zfself, instance)

protected:
    zfoverride
    virtual zffloat progressOnUpdate(ZF_IN zffloat time)
    {
        return ZFBezierBounceInOut().y_by_x(time);
    }
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFTimeLineCurve_UI_h_

