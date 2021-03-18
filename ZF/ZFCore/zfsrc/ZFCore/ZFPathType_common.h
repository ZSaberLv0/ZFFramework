/**
 * @file ZFPathType_common.h
 * @brief common #ZFPathInfo
 */

#ifndef _ZFI_ZFPathType_common_h_
#define _ZFI_ZFPathType_common_h_

#include "ZFPathType_file.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief see #ZFPathInfo
 *
 * pathData is file's relative path within #ZFFilePathForModule
 * @note path must be well formed, use #ZFFilePathFormat if necessary
 */
ZFPATHTYPE_DECLARE(modulePath)

/**
 * @brief see #ZFPathInfo
 *
 * pathData is file's relative path within #ZFFilePathForStorage
 * @note path must be well formed, use #ZFFilePathFormat if necessary
 */
ZFPATHTYPE_DECLARE(storagePath)

/**
 * @brief see #ZFPathInfo
 *
 * pathData is file's relative path within #ZFFilePathForStorageShared
 * @note path must be well formed, use #ZFFilePathFormat if necessary
 */
ZFPATHTYPE_DECLARE(storageSharedPath)

/**
 * @brief see #ZFPathInfo
 *
 * pathData is file's relative path within #ZFFilePathForCache
 *
 * note, the cache path's content would be removed according to #ZFFilePathForCacheClear
 * @note path must be well formed, use #ZFFilePathFormat if necessary
 */
ZFPATHTYPE_DECLARE(cachePath)

/**
 * @brief see #ZFPathInfo
 *
 * pathData is plain text
 */
ZFPATHTYPE_DECLARE(text)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFPathType_common_h_

