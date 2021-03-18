/**
 * @file ZFEncrypt.h
 * @brief encrypt util
 */

#ifndef _ZFI_ZFEncrypt_h_
#define _ZFI_ZFEncrypt_h_

#include "ZFAlgorithmDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief encrypt util, result value is ensured to have printable chars only
 *
 * which encrypt algorithm would be used, is depends on ZFEncrypt protocol
 */
ZFMETHOD_FUNC_DECLARE_3(zfbool, ZFEncrypt,
                        ZFMP_IN_OUT(const ZFOutput &, output),
                        ZFMP_IN(const ZFInput &, input),
                        ZFMP_IN(const zfchar *, encryptKey))
/** @brief see #ZFEncrypt */
ZFMETHOD_FUNC_DECLARE_3(zfbool, ZFDecrypt,
                        ZFMP_IN_OUT(const ZFOutput &, output),
                        ZFMP_IN(const ZFInput &, input),
                        ZFMP_IN(const zfchar *, encryptKey))

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFEncrypt_h_

