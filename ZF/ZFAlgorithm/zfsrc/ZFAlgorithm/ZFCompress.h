/**
 * @file ZFCompress.h
 * @brief compress util
 */

#ifndef _ZFI_ZFCompress_h_
#define _ZFI_ZFCompress_h_

#include "ZFAlgorithmDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief compress level for #ZFCompress
 */
ZFENUM_BEGIN(ZFCompressLevel)
    ZFENUM_VALUE(NoCompress)
    ZFENUM_VALUE(BestSpeed)
    ZFENUM_VALUE(GoodSpeed)
    ZFENUM_VALUE(DefaultCompress)
    ZFENUM_VALUE(GoodCompress)
    ZFENUM_VALUE(BestCompress)
ZFENUM_SEPARATOR(ZFCompressLevel)
    ZFENUM_VALUE_REGISTER(NoCompress)
    ZFENUM_VALUE_REGISTER(BestSpeed)
    ZFENUM_VALUE_REGISTER(GoodSpeed)
    ZFENUM_VALUE_REGISTER(DefaultCompress)
    ZFENUM_VALUE_REGISTER(GoodCompress)
    ZFENUM_VALUE_REGISTER(BestCompress)
ZFENUM_END_WITH_DEFAULT(ZFCompressLevel, ZFCompressLevel::e_DefaultCompress)

// ============================================================
// base api
/**
 * @brief compress util
 *
 * typical usage:
 * @code
 *   // compress
 *   ZFToken compressToken = ZFCompressBegin(outputZip);
 *   zfbool success = (compressToken != ZFTokenInvalid());
 *   success &= ZFCompressContent(compressToken, inputRaw0, filePathInZip0);
 *   success &= ZFCompressContent(compressToken, inputRaw1, filePathInZip1);
 *   success &= ZFCompressEnd(compressToken);
 *
 *   // decompress
 *   ZFToken decompressToken = ZFDecompressBegin(inputZip);
 *   zfbool success = (decompressToken != ZFTokenInvalid());
 *   success &= ZFDecompressContent(decompressToken, outputRaw0, filePathInZip0);
 *   success &= ZFDecompressContent(decompressToken, outputRaw1, filePathInZip1);
 *   success &= ZFDecompressContentAtIndex(decompressToken, outputRaw0, fileIndexInZip0);
 *   success &= ZFDecompressContentAtIndex(decompressToken, outputRaw1, fileIndexInZip1);
 *   success &= ZFDecompressEnd(decompressToken);
 *
 *   // get zip file content info
 *   zfindex fileCountInZip = ZFDecompressContentCount(decompressToken);
 *   zfindex fileIndexInZip = ZFDecompressContentIndex("filePathInZip");
 *   zfstring filePathInZip = ZFDecompressContentPath(fileIndexInZip);
 * @endcode
 *
 * or, use the util methods:
 * @code
 *   // compress buffer
 *   ZFCompress(outputZip, inputRaw, filePathInZip);
 *   // decompress buffer
 *   ZFDecompress(outputZip, inputRaw, filePathInZip);
 *
 *   // compress dir
 *   ZFCompressDir(outputZip, inputPathInfo);
 *   // decompress dir
 *   ZFDecompressDir(outputPathInfo, inputZip);
 * @endcode
 *
 * note: which compress algorithm would be used, depends on impl
 */
ZFMETHOD_FUNC_DECLARE_2(ZFToken, ZFCompressBegin,
                        ZFMP_IN_OUT(const ZFOutput &, outputZip),
                        ZFMP_IN_OPT(ZFCompressLevelEnum, compressLevel, ZFCompressLevel::EnumDefault()))
/** @brief see #ZFCompressBegin */
ZFMETHOD_FUNC_DECLARE_1(zfbool, ZFCompressEnd,
                        ZFMP_IN_OUT(ZFToken &, compressToken))
/** @brief see #ZFCompressBegin */
ZFMETHOD_FUNC_DECLARE_3(zfbool, ZFCompressContent,
                        ZFMP_IN_OUT(ZFToken, compressToken),
                        ZFMP_IN_OUT(const ZFInput &, inputRaw),
                        ZFMP_IN(const zfchar *, filePathInZip))
/** @brief see #ZFCompressBegin */
ZFMETHOD_FUNC_DECLARE_2(zfbool, ZFCompressContentDir,
                        ZFMP_IN_OUT(ZFToken, compressToken),
                        ZFMP_IN(const zfchar *, filePathInZip))

/** @brief see #ZFCompressBegin */
ZFMETHOD_FUNC_DECLARE_1(ZFToken, ZFDecompressBegin,
                        ZFMP_IN_OUT(const ZFInput &, inputZip))
/** @brief see #ZFCompressBegin */
ZFMETHOD_FUNC_DECLARE_1(zfbool, ZFDecompressEnd,
                        ZFMP_IN_OUT(ZFToken &, decompressToken))
/** @brief see #ZFCompressBegin */
ZFMETHOD_FUNC_DECLARE_3(zfbool, ZFDecompressContentAtIndex,
                        ZFMP_IN_OUT(ZFToken, decompressToken),
                        ZFMP_IN_OUT(const ZFOutput &, outputRaw),
                        ZFMP_IN(zfindex, fileIndexInZip))
/** @brief see #ZFCompressBegin */
ZFMETHOD_FUNC_DECLARE_3(zfbool, ZFDecompressContent,
                        ZFMP_IN_OUT(ZFToken, decompressToken),
                        ZFMP_IN_OUT(const ZFOutput &, outputRaw),
                        ZFMP_IN(const zfchar *, filePathInZip))
/** @brief see #ZFCompressBegin, return #zfindexMax if fail */
ZFMETHOD_FUNC_DECLARE_1(zfindex, ZFDecompressContentCount,
                        ZFMP_IN(ZFToken, decompressToken))
/** @brief see #ZFCompressBegin, return #zfindexMax if fail */
ZFMETHOD_FUNC_DECLARE_2(zfindex, ZFDecompressContentIndex,
                        ZFMP_IN(ZFToken, decompressToken),
                        ZFMP_IN(const zfchar *, filePathInZip))
/** @brief see #ZFCompressBegin */
ZFMETHOD_FUNC_DECLARE_3(zfbool, ZFDecompressContentPathT,
                        ZFMP_IN(ZFToken, decompressToken),
                        ZFMP_IN_OUT(zfstring &, filePathInZip),
                        ZFMP_IN(zfindex, fileIndexInZip))
/** @brief see #ZFCompressBegin */
ZFMETHOD_FUNC_DECLARE_2(zfstring, ZFDecompressContentPath,
                        ZFMP_IN(ZFToken, decompressToken),
                        ZFMP_IN(zfindex, fileIndexInZip))

// ============================================================
// util
#define _ZFP_ZFCompressFilePathDefault "content"

/** @brief see #ZFCompressBegin */
ZFMETHOD_FUNC_DECLARE_4(zfbool, ZFCompress,
                        ZFMP_IN_OUT(const ZFOutput &, outputZip),
                        ZFMP_IN_OUT(const ZFInput &, inputRaw),
                        ZFMP_IN_OPT(ZFCompressLevelEnum, compressLevel, ZFCompressLevel::EnumDefault()),
                        ZFMP_IN_OPT(const zfchar *, filePathInZip, _ZFP_ZFCompressFilePathDefault))
/** @brief see #ZFCompressBegin */
ZFMETHOD_FUNC_DECLARE_3(zfbool, ZFDecompress,
                        ZFMP_IN_OUT(const ZFOutput &, outputRaw),
                        ZFMP_IN_OUT(const ZFInput &, inputZip),
                        ZFMP_IN_OPT(const zfchar *, filePathInZip, _ZFP_ZFCompressFilePathDefault))

/** @brief see #ZFCompressBegin */
ZFMETHOD_FUNC_DECLARE_3(zfbool, ZFCompressDir,
                        ZFMP_IN_OUT(const ZFOutput &, outputZip),
                        ZFMP_IN(const ZFPathInfo &, inputPathInfo),
                        ZFMP_IN_OPT(ZFCompressLevelEnum, compressLevel, ZFCompressLevel::EnumDefault()))
/** @brief see #ZFCompressBegin */
ZFMETHOD_FUNC_DECLARE_2(zfbool, ZFDecompressDir,
                        ZFMP_IN(const ZFPathInfo &, outputPathInfo),
                        ZFMP_IN_OUT(const ZFInput &, inputZip))

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFCompress_h_

