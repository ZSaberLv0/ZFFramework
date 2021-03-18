/**
 * @file ZFProtocolZFFile.h
 * @brief protocol for ZFFile
 */

#ifndef _ZFI_ZFProtocolZFFile_h_
#define _ZFI_ZFProtocolZFFile_h_

#include "ZFCore/ZFProtocol.h"
#include "ZFCore/ZFFile.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for ZFFile
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFFile)
public:
    /**
     * @brief see #ZFFileFileIsExist
     */
    virtual zfbool fileIsExist(ZF_IN const zfchar *path) zfpurevirtual;
    /**
     * @brief see #ZFFileFileIsDir
     */
    virtual zfbool fileIsDir(ZF_IN const zfchar *path) zfpurevirtual;

    /**
     * @brief see #ZFFileFilePathCreate
     */
    virtual zfbool filePathCreate(ZF_IN const zfchar *path,
                                  ZF_IN_OPT zfbool autoMakeParent = zffalse,
                                  ZF_OUT_OPT zfstring *errPos = zfnull) zfpurevirtual;
    /**
     * @brief see #ZFFileFileCopy
     */
    virtual zfbool fileCopy(ZF_IN const zfchar *srcPath,
                            ZF_IN const zfchar *dstPath,
                            ZF_IN_OPT zfbool isRecursive = zftrue,
                            ZF_IN_OPT zfbool isForce = zffalse,
                            ZF_IN_OPT zfstring *errPos = zfnull) zfpurevirtual;
    /**
     * @brief see #ZFFileFileMove
     */
    virtual zfbool fileMove(ZF_IN const zfchar *srcPath,
                            ZF_IN const zfchar *dstPath,
                            ZF_IN_OPT zfbool isRecursive = zftrue,
                            ZF_IN_OPT zfbool isForce = zffalse,
                            ZF_IN_OPT zfstring *errPos = zfnull) zfpurevirtual;
    /**
     * @brief see #ZFFileFileRemove
     */
    virtual zfbool fileRemove(ZF_IN const zfchar *path,
                              ZF_IN_OPT zfbool isRecursive = zftrue,
                              ZF_IN_OPT zfbool isForce = zffalse,
                              ZF_IN_OPT zfstring *errPos = zfnull) zfpurevirtual;

    /**
     * @brief see #ZFFileFileFindFirst
     *
     * use ZFFileFindData::Impl to store find result\n
     * remember that you must remove the dir "." and "..",
     * and if there is only two dir named "." and "..",
     * you should return false for fileFindFirst\n
     * if returned zffalse, no fileFindClose would be called anymore
     * @warning it's the implementation's responsibility
     *   to allocate and deallocate the nativeFd,
     *   and, if fileFindFirst would return zffalse,
     *   you must deallocate the nativeFd (if allocated) before return
     */
    virtual zfbool fileFindFirst(ZF_IN_OUT ZFFileFindData::Impl &fd,
                                 ZF_IN const zfchar *path) zfpurevirtual;
    /**
     * @brief see #ZFFileFileFindNext, fileFindFirst
     */
    virtual zfbool fileFindNext(ZF_IN_OUT ZFFileFindData::Impl &fd) zfpurevirtual;
    /**
     * @brief see #ZFFileFileFindClose, fileFindFirst
     */
    virtual void fileFindClose(ZF_IN_OUT ZFFileFindData::Impl &fd) zfpurevirtual;
ZFPROTOCOL_INTERFACE_END(ZFFile)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFFile_h_

