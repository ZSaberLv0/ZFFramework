/**
 * @file ZFProtocolZFFilePath.h
 * @brief protocol for ZFFile
 */

#ifndef _ZFI_ZFProtocolZFFilePath_h_
#define _ZFI_ZFProtocolZFFilePath_h_

#include "ZFCore/ZFProtocol.h"
#include "ZFCore/ZFFile.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for ZFFile
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFFilePath)
public:
    /**
     * @brief see #ZFFilePathForModule
     */
    virtual const zfchar *pathForModule(void) zfpurevirtual;
    /**
     * @brief see #ZFFilePathForModuleFile
     */
    virtual const zfchar *pathForModuleFile(void) zfpurevirtual;

    /**
     * @brief see #ZFFilePathForSetting
     */
    virtual const zfchar *pathForSetting(void) zfpurevirtual;
    /**
     * @brief see #ZFFilePathForSetting
     */
    virtual void pathForSetting(ZF_IN const zfchar *path) zfpurevirtual;

    /**
     * @brief see #ZFFilePathForStorage
     */
    virtual const zfchar *pathForStorage(void) zfpurevirtual;
    /**
     * @brief see #ZFFilePathForStorage
     */
    virtual void pathForStorage(ZF_IN const zfchar *path) zfpurevirtual;

    /**
     * @brief see #ZFFilePathForStorageShared
     */
    virtual const zfchar *pathForStorageShared(void) zfpurevirtual;
    /**
     * @brief see #ZFFilePathForStorageShared
     */
    virtual void pathForStorageShared(ZF_IN const zfchar *path) zfpurevirtual;

    /**
     * @brief see #ZFFilePathForCache
     */
    virtual const zfchar *pathForCache(void) zfpurevirtual;
    /**
     * @brief see #ZFFilePathForCache
     */
    virtual void pathForCache(ZF_IN const zfchar *path) zfpurevirtual;
    /**
     * @brief see #ZFFilePathForCacheClear
     */
    virtual void pathForCacheClear(void) zfpurevirtual;
ZFPROTOCOL_INTERFACE_END(ZFFilePath)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFFilePath_h_

