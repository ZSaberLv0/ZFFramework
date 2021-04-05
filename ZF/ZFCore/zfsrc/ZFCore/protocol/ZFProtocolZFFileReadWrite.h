/**
 * @file ZFProtocolZFFileReadWrite.h
 * @brief protocol for ZFFile
 */

#ifndef _ZFI_ZFProtocolZFFileReadWrite_h_
#define _ZFI_ZFProtocolZFFileReadWrite_h_

#include "ZFCore/ZFProtocol.h"
#include "ZFCore/ZFFile.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for ZFFile
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFFileReadWrite)
public:
    /** @brief see #ZFFileFileOpen */
    virtual void *fileOpen(ZF_IN const zfchar *filePath,
                           ZF_IN_OPT ZFFileOpenOptionFlags flag = ZFFileOpenOption::e_Read) zfpurevirtual;
    /** @brief see #ZFFileFileClose */
    virtual zfbool fileClose(ZF_IN void *token) zfpurevirtual;

    /** @brief see #ZFFileFileTell */
    virtual zfindex fileTell(ZF_IN void *token) zfpurevirtual;
    /** @brief see #ZFFileFileSeek */
    virtual zfbool fileSeek(ZF_IN void *token,
                            ZF_IN zfindex byteSize,
                            ZF_IN_OPT ZFSeekPos position = ZFSeekPosBegin) zfpurevirtual;

    /** @brief see #ZFFileFileRead */
    virtual zfindex fileRead(ZF_IN void *token,
                             ZF_IN void *buf,
                             ZF_IN zfindex maxByteSize) zfpurevirtual;

    /** @brief see #ZFFileFileWrite */
    virtual zfindex fileWrite(ZF_IN void *token,
                              ZF_IN const void *src,
                              ZF_IN zfindex maxByteSize) zfpurevirtual;
    /** @brief see #ZFFileFileFlush */
    virtual void fileFlush(ZF_IN void *token) zfpurevirtual;
    /** @brief see #ZFFileFileIsEof */
    virtual zfbool fileIsEof(ZF_IN void *token) zfpurevirtual;
    /** @brief see #ZFFileFileIsError */
    virtual zfbool fileIsError(ZF_IN void *token) zfpurevirtual;
ZFPROTOCOL_INTERFACE_END(ZFFileReadWrite)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFFileReadWrite_h_

