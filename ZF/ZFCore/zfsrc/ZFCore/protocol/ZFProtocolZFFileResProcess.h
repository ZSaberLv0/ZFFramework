/**
 * @file ZFProtocolZFFileResProcess.h
 * @brief protocol for ZFFile
 */

#ifndef _ZFI_ZFProtocolZFFileResProcess_h_
#define _ZFI_ZFProtocolZFFileResProcess_h_

#include "ZFProtocolZFFile.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for ZFFile
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFFileResProcess)

public:
    /**
     * @brief util method to access root path for file res impl
     *
     * for most file res impl, res processing is the same as normal file processing,
     * except the root path differs,
     * so we supply an default impl that process like normal files,
     * and let different impl to specify different root path\n
     * by default, this method would return "./zfres/"
     */
    virtual const zfstring &resRootPath(void);

public:
    /**
     * @brief see #ZFFileResIsExist
     */
    virtual zfbool resIsExist(ZF_IN const zfchar *resPath);
    /**
     * @brief see #ZFFileResIsDir
     */
    virtual zfbool resIsDir(ZF_IN const zfchar *resPath);

    /**
     * @brief see #ZFFileResCopy
     */
    virtual zfbool resCopy(ZF_IN const zfchar *resPath,
                           ZF_IN const zfchar *dstPath,
                           ZF_IN_OPT zfbool isRecursive = zftrue,
                           ZF_IN_OPT zfbool isForce = zffalse,
                           ZF_IN_OPT zfstring *errPos = zfnull);
    /**
     * @brief see #ZFFileResOpen
     */
    virtual void *resOpen(ZF_IN const zfchar *resPath);
    /**
     * @brief see #ZFFileResClose
     */
    virtual zfbool resClose(ZF_IN void *token);

    /**
     * @brief see #ZFFileResTell
     */
    virtual zfindex resTell(ZF_IN void *token);
    /**
     * @brief see #ZFFileResSeek
     */
    virtual zfbool resSeek(ZF_IN void *token,
                           ZF_IN zfindex byteSize,
                           ZF_IN_OPT ZFSeekPos position = ZFSeekPosBegin);

    /**
     * @brief see #ZFFileResRead
     */
    virtual zfindex resRead(ZF_IN void *token,
                            ZF_IN void *buf,
                            ZF_IN zfindex maxByteSize);

    /**
     * @brief see #ZFFileResIsEof
     */
    virtual zfbool resIsEof(ZF_IN void *token);
    /**
     * @brief see #ZFFileResIsError
     */
    virtual zfbool resIsError(ZF_IN void *token);

    /**
     * @brief see #ZFFileResFindFirst, #ZFFileFileFindFirst
     */
    virtual zfbool resFindFirst(ZF_IN_OUT ZFFileFindData::Impl &fd,
                                ZF_IN const zfchar *resPath);
    /**
     * @brief see #ZFFileResFindNext, #ZFFileFileFindNext
     */
    virtual zfbool resFindNext(ZF_IN_OUT ZFFileFindData::Impl &fd);
    /**
     * @brief see #ZFFileResFindClose, #ZFFileFileFindClose
     */
    virtual void resFindClose(ZF_IN_OUT ZFFileFindData::Impl &fd);
ZFPROTOCOL_INTERFACE_END(ZFFileResProcess)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFFileResProcess_h_

