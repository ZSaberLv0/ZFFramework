/**
 * @file ZFBase64.h
 * @brief base64 encoder and decoder
 */

#ifndef _ZFI_ZFBase64_h_
#define _ZFI_ZFBase64_h_

#include "ZFAlgorithmDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief char table for ZFBase64, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/" by default
 */
ZFEXPORT_VAR_READONLY_DECLARE(const zfchar *, ZFBase64TableDefault)

/**
 * @brief pad token for ZFBase64, '=' by default
 */
ZFEXPORT_VAR_READONLY_DECLARE(zfchar, ZFBase64PadDefault)

/**
 * @brief standard line break position for ZFBase64, 76 by default
 */
ZFEXPORT_VAR_READONLY_DECLARE(zfindex, ZFBase64LineBreakPosStandard)

/**
 * @brief no line break for ZFBase64
 */
ZFEXPORT_VAR_READONLY_DECLARE(zfindex, ZFBase64LineBreakPosNone)

// ============================================================
// encode
/**
 * @brief calculate required size to store result,
 *   ensured big enough but not ensure exactly the same
 *
 * return zfindexMax() if error
 */
ZFMETHOD_FUNC_DECLARE_2(zfindex, ZFBase64EncodeCalcSize,
                        ZFMP_IN(zfindex, srcLen),
                        ZFMP_IN_OPT(zfindex, lineBreakPos, ZFBase64LineBreakPosNone()))

/**
 * @brief encode base64
 */
extern ZF_ENV_EXPORT zfbool ZFBase64Encode(ZF_OUT zfchar *buf,
                                           ZF_IN const void *src,
                                           ZF_IN_OPT zfindex srcLen = zfindexMax(),
                                           ZF_OUT_OPT zfindex *outResultSize = zfnull,
                                           ZF_IN_OPT const zfchar *table = ZFBase64TableDefault(),
                                           ZF_IN_OPT zfchar pad = ZFBase64PadDefault(),
                                           ZF_IN_OPT zfindex lineBreakPos = ZFBase64LineBreakPosNone());
/**
 * @brief encode base64, return byte size written even if error occurred
 */
ZFMETHOD_FUNC_DECLARE_6(zfbool, ZFBase64Encode,
                        ZFMP_IN_OUT(const ZFOutput &, outputCallback),
                        ZFMP_IN(const ZFInput &, inputCallback),
                        ZFMP_OUT_OPT(zfindex *, outResultSize, zfnull),
                        ZFMP_IN_OPT(const zfchar *, table, ZFBase64TableDefault()),
                        ZFMP_IN_OPT(zfchar, pad, ZFBase64PadDefault()),
                        ZFMP_IN_OPT(zfindex, lineBreakPos, ZFBase64LineBreakPosNone()))

// ============================================================
// decode
/**
 * @brief calculate required size to store result,
 *   ensured big enough but not ensure exactly the same
 *
 * return zfindexMax() if error
 */
ZFMETHOD_FUNC_DECLARE_2(zfindex, ZFBase64DecodeCalcSize,
                        ZFMP_IN(zfindex, srcLen),
                        ZFMP_IN_OPT(zfindex, lineBreakPos, ZFBase64LineBreakPosNone()))
/**
 * @brief encode base64, return byte size written even if error occurred
 *
 * extra space, tab, '\\r', '\\n' is allowed,
 * missing pad is allowed
 */
extern ZF_ENV_EXPORT zfbool ZFBase64Decode(ZF_OUT void *buf,
                                           ZF_IN const zfchar *src,
                                           ZF_IN_OPT zfindex srcLen = zfindexMax(),
                                           ZF_OUT_OPT zfindex *outResultSize = zfnull,
                                           ZF_IN_OPT const zfchar *table = ZFBase64TableDefault(),
                                           ZF_IN_OPT zfchar pad = ZFBase64PadDefault());
/**
 * @brief encode base64, return byte size written even if error occurred
 */
ZFMETHOD_FUNC_DECLARE_5(zfbool, ZFBase64Decode,
                        ZFMP_IN_OUT(const ZFOutput &, outputCallback),
                        ZFMP_IN(const ZFInput &, inputCallback),
                        ZFMP_OUT_OPT(zfindex *, outResultSize, zfnull),
                        ZFMP_IN_OPT(const zfchar *, table, ZFBase64TableDefault()),
                        ZFMP_IN_OPT(zfchar, pad, ZFBase64PadDefault()))

// ============================================================
/**
 * @brief util class to process base64
 */
zfclass ZF_ENV_EXPORT ZFBase64 : zfextends ZFObject, zfimplements ZFSerializable
{
    ZFOBJECT_DECLARE(ZFBase64, ZFObject)
    ZFIMPLEMENTS_DECLARE(ZFSerializable)

public:
    /** @brief see #ZFBase64TableDefault */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfstring, base64Table, ZFBase64TableDefault())
    /** @brief see #ZFBase64PadDefault */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfchar, base64Pad, ZFBase64PadDefault())
    /** @brief see #ZFBase64LineBreakPosNone */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfindex, base64LineBreakPos, ZFBase64LineBreakPosNone())

public:
    /** @brief see #ZFBase64EncodeCalcSize */
    ZFMETHOD_DECLARE_1(zfindex, base64EncodeCalcSize,
                       ZFMP_IN(zfindex, srcLen))
    /** @brief see #ZFBase64Encode */
    virtual inline zfbool base64Encode(ZF_OUT zfchar *buf,
                                       ZF_IN const void *src,
                                       ZF_IN_OPT zfindex srcLen = zfindexMax(),
                                       ZF_OUT_OPT zfindex *outResultSize = zfnull)
    {
        return ZFBase64Encode(buf, src, srcLen, outResultSize, this->base64Table(), this->base64Pad(), this->base64LineBreakPos());
    }
    /** @brief see #ZFBase64Encode */
    ZFMETHOD_DECLARE_3(zfbool, base64Encode,
                       ZFMP_IN_OUT(const ZFOutput &, output),
                       ZFMP_IN(const ZFInput &, input),
                       ZFMP_OUT_OPT(zfindex *, outResultSize, zfnull))
public:
    /** @brief see #ZFBase64DecodeCalcSize */
    ZFMETHOD_DECLARE_1(zfindex, base64DecodeCalcSize,
                       ZFMP_IN(zfindex, srcLen))
    /** @brief see #ZFBase64Decode */
    virtual inline zfbool base64Decode(ZF_OUT void *buf,
                                       ZF_IN const zfchar *src,
                                       ZF_IN_OPT zfindex srcLen = zfindexMax(),
                                       ZF_OUT_OPT zfindex *outResultSize = zfnull)
    {
        return ZFBase64Decode(buf, src, srcLen, outResultSize, this->base64Table(), this->base64Pad());
    }
    /** @brief see #ZFBase64Decode */
    ZFMETHOD_DECLARE_3(zfbool, base64Decode,
                       ZFMP_IN_OUT(const ZFOutput &, output),
                       ZFMP_IN(const ZFInput &, input),
                       ZFMP_OUT_OPT(zfindex *, outResultSize, zfnull))
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFBase64_h_

