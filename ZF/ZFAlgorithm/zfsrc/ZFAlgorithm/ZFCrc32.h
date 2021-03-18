/**
 * @file ZFCrc32.h
 * @brief CRC32 utility
 */

#ifndef _ZFI_ZFCrc32_h_
#define _ZFI_ZFCrc32_h_

#include "ZFAlgorithmDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief init value for continous calculation, see #zfCrc32Calc
 */
ZFEXPORT_VAR_READONLY_DECLARE(zfflags, ZFCrc32ValueZero)

/**
 * @brief invalid value for CRC32
 */
ZFEXPORT_VAR_READONLY_DECLARE(zfflags, ZFCrc32ValueInvalid)

/**
 * @brief calculate CRC32, return #ZFCrc32ValueInvalid if failed
 *
 * prevResult is used for continous calculation for performance,
 * you may separate big buffer to small ones,
 * and calculate separately
 */
extern ZF_ENV_EXPORT zfflags zfCrc32Calc(ZF_IN const void *src,
                                         ZF_IN zfindex len,
                                         ZF_IN_OPT zfflags prevResult = ZFCrc32ValueZero());
/**
 * @brief calculate CRC32, return #ZFCrc32ValueInvalid if failed
 *
 * prevResult is used for continous calculation for performance,
 * you may separate big buffer to small ones,
 * and calculate separately
 */
ZFMETHOD_FUNC_DECLARE_2(zfflags, zfCrc32Calc,
                        ZFMP_IN(const ZFInput &, callback),
                        ZFMP_IN_OPT(zfflags, prevResult, ZFCrc32ValueZero()))
/**
 * @brief see #zfCrc32Calc
 */
ZFMETHOD_FUNC_DECLARE_3(zfflags, zfCrc32Calc,
                        ZFMP_IN(const zfchar *, src),
                        ZFMP_IN_OPT(zfindex, len, zfindexMax()),
                        ZFMP_IN_OPT(zfflags, prevResult, ZFCrc32ValueZero()))

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFCrc32_h_

