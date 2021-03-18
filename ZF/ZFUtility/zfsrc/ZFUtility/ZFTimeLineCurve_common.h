/**
 * @file ZFTimeLineCurve_common.h
 * @brief see #ZFTimeLineCurve
 */

#ifndef _ZFI_ZFTimeLineCurve_common_h_
#define _ZFI_ZFTimeLineCurve_common_h_

#include "ZFTimeLineCurve.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief linear time line, see #ZFTimeLineCurve
 */
zfclass ZF_ENV_EXPORT ZFTimeLineCurveLinear : zfextends ZFTimeLineCurve
{
    ZFOBJECT_DECLARE(ZFTimeLineCurveLinear, ZFTimeLineCurve)
    ZFOBJECT_SINGLETON_DECLARE(zfself, instance)

protected:
    zfoverride
    virtual zffloat progressOnUpdate(ZF_IN zffloat time)
    {
        return time;
    }
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFTimeLineCurve_common_h_

