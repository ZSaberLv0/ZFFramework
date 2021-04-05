/**
 * @file ZFProtocolZFCompress.h
 * @brief protocol for ZFCompress
 */

#ifndef _ZFI_ZFProtocolZFCompress_h_
#define _ZFI_ZFProtocolZFCompress_h_

#include "ZFCore/ZFProtocol.h"
#include "ZFAlgorithm/ZFCompress.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for ZFCompress
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFCompress)
public:
    /** @brief see #ZFCompressBegin */
    virtual void *compressBegin(ZF_IN_OUT const ZFOutput &outputZip,
                                ZF_IN ZFCompressLevelEnum compressLevel) zfpurevirtual;
    /** @brief see #ZFCompressBegin */
    virtual zfbool compressEnd(ZF_IN_OUT void *compressToken) zfpurevirtual;
    /** @brief see #ZFCompressBegin */
    virtual zfbool compressContent(ZF_IN_OUT void *compressToken,
                                   ZF_IN_OUT const ZFInput &inputRaw,
                                   ZF_IN const zfchar *filePathInZip) zfpurevirtual;
    /** @brief see #ZFCompressBegin */
    virtual zfbool compressContentDir(ZF_IN_OUT void *compressToken,
                                      ZF_IN const zfchar *filePathInZip) zfpurevirtual;

    /** @brief see #ZFCompressBegin */
    virtual void *decompressBegin(ZF_IN_OUT const ZFInput &inputZip) zfpurevirtual;
    /** @brief see #ZFCompressBegin */
    virtual zfbool decompressEnd(ZF_IN_OUT void *decompressToken) zfpurevirtual;
    /** @brief see #ZFCompressBegin */
    virtual zfbool decompressContent(ZF_IN_OUT void *decompressToken,
                                     ZF_IN_OUT const ZFOutput &outputRaw,
                                     ZF_IN zfindex fileIndexInZip) zfpurevirtual;
    /** @brief see #ZFCompressBegin */
    virtual zfindex decompressContentCount(ZF_IN void *decompressToken) zfpurevirtual;
    /** @brief see #ZFCompressBegin */
    virtual zfindex decompressContentIndex(ZF_IN void *decompressToken,
                                           ZF_IN const zfchar *filePathInZip) zfpurevirtual;
    /** @brief see #ZFCompressBegin */
    virtual zfbool decompressContentPath(ZF_IN void *decompressToken,
                                         ZF_IN_OUT zfstring &filePathInZip,
                                         ZF_IN zfindex fileIndexInZip) zfpurevirtual;
ZFPROTOCOL_INTERFACE_END(ZFCompress)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFCompress_h_

