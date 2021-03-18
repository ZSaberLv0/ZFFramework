/**
 * @file ZFMd5.h
 * @brief MD5 utility
 */

#ifndef _ZFI_ZFMd5_h_
#define _ZFI_ZFMd5_h_

#include "ZFAlgorithmDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief calculate MD5, return empty string if failed
 */
extern ZF_ENV_EXPORT void zfMd5Calc(ZF_IN_OUT zfstring &ret,
                                    ZF_IN const void *src,
                                    ZF_IN zfindex len,
                                    ZF_IN_OPT zfbool upperCase = zftrue);
/** @brief see #zfMd5Calc */
inline zfstring zfMd5Calc(ZF_IN const void *src,
                          ZF_IN zfindex len,
                          ZF_IN_OPT zfbool upperCase = zftrue)
{
    zfstring ret;
    zfMd5Calc(ret, src, len, upperCase);
    return ret;
}

/** @brief see #zfMd5Calc */
ZFMETHOD_FUNC_DECLARE_3(void, zfMd5Calc,
                        ZFMP_IN_OUT(zfstring &, ret),
                        ZFMP_IN(const ZFInput &, callback),
                        ZFMP_IN_OPT(zfbool, upperCase, zftrue))
/** @brief see #zfMd5Calc */
ZFMETHOD_FUNC_DECLARE_2(zfstring, zfMd5Calc,
                        ZFMP_IN(const ZFInput &, callback),
                        ZFMP_IN_OPT(zfbool, upperCase, zftrue))

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFMd5_h_

