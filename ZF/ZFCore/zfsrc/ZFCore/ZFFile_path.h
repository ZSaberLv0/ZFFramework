/**
 * @file ZFFile_path.h
 * @brief file utility
 */

#ifndef _ZFI_ZFFile_path_h_
#define _ZFI_ZFFile_path_h_

#include "ZFFile_fwd.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// cwd
/**
 * @brief get current working directory
 *
 * return null if not available
 */
ZFMETHOD_FUNC_DECLARE_0(const zfchar *, ZFFilePathForCwd)
/**
 * @brief change current working directory
 *
 * return false if failed
 */
ZFMETHOD_FUNC_DECLARE_1(zfbool, ZFFilePathForCwdChange,
                        ZFMP_IN(const zfchar *, path))
/**
 * @brief util method to access #ZFFilePathForCwd,
 *   return #ZFFilePathForModule if not available
 */
ZFMETHOD_FUNC_DECLARE_0(const zfchar *, ZFFilePathForCwdOrModule)

// ============================================================
// module
/**
 * @brief get module's parent's path, e.g. "/path"
 *
 * path is ensured to use ZFFileSeparator as separator,
 * and no extra separator would be added to tail
 */
ZFMETHOD_FUNC_DECLARE_0(const zfchar *, ZFFilePathForModule)
/**
 * @brief get module's path, e.g. "/path/module.exe"
 *
 * path is ensured to use ZFFileSeparator as separator\n
 * it's not ensured module path is actually executable's path,
 * use other path types for data storage such as #ZFFilePathForSetting
 */
ZFMETHOD_FUNC_DECLARE_0(const zfchar *, ZFFilePathForModuleFile)

// ============================================================
// setting
ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
/**
 * @brief see #ZFObject::observerNotify
 *
 * notified after change, param0 is the old path value
 * (as #v_zfstring)\n
 * notified to #ZFGlobalEventCenter only
 */
ZFOBSERVER_EVENT_GLOBAL(ZFFilePathForSettingOnChange)
ZF_NAMESPACE_END(ZFGlobalEvent)

/**
 * @brief get a proper data path that app can save settings to
 *
 * path is ensured to use ZFFileSeparator as separator,
 * and no extra separator would be added to tail
 */
ZFMETHOD_FUNC_DECLARE_0(const zfchar *, ZFFilePathForSetting)
/**
 * @brief change the data path, null to use defalut path
 */
ZFMETHOD_FUNC_DECLARE_1(void, ZFFilePathForSetting,
                        ZFMP_IN(const zfchar *, path))

// ============================================================
// storage
ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
/**
 * @brief see #ZFObject::observerNotify
 *
 * notified after change, param0 is the old path value
 * (as #v_zfstring)\n
 * notified to #ZFGlobalEventCenter only
 */
ZFOBSERVER_EVENT_GLOBAL(ZFFilePathForStorageOnChange)
ZF_NAMESPACE_END(ZFGlobalEvent)

/**
 * @brief get a proper storage path that app can write files to
 *
 * path is ensured to use ZFFileSeparator as separator,
 * and no extra separator would be added to tail
 */
ZFMETHOD_FUNC_DECLARE_0(const zfchar *, ZFFilePathForStorage)
/**
 * @brief change the storage path, null to use defalut path
 */
ZFMETHOD_FUNC_DECLARE_1(void, ZFFilePathForStorage,
                        ZFMP_IN(const zfchar *, path))

// ============================================================
// storage shared
ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
/**
 * @brief see #ZFObject::observerNotify
 *
 * notified after change, param0 is the old path value
 * (as #v_zfstring)\n
 * notified to #ZFGlobalEventCenter only
 */
ZFOBSERVER_EVENT_GLOBAL(ZFFilePathForStorageSharedOnChange)
ZF_NAMESPACE_END(ZFGlobalEvent)

/**
 * @brief get a proper shared storage path that app can write files to,
 *   and other app can access
 *
 * note, this path may or may not be writable,
 * maybe external storage path or user's home path,
 * write with caution and only if necessary\n
 * path is ensured to use ZFFileSeparator as separator,
 * and no extra separator would be added to tail
 */
ZFMETHOD_FUNC_DECLARE_0(const zfchar *, ZFFilePathForStorageShared)
/**
 * @brief change the storage path, null to use defalut path
 */
ZFMETHOD_FUNC_DECLARE_1(void, ZFFilePathForStorageShared,
                        ZFMP_IN(const zfchar *, path))

// ============================================================
// cache
ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
/**
 * @brief see #ZFObject::observerNotify
 *
 * notified after change, param0 is the old path value
 * (as #v_zfstring)\n
 * notified to #ZFGlobalEventCenter only
 */
ZFOBSERVER_EVENT_GLOBAL(ZFFilePathForCacheOnChange)
/**
 * @brief see #ZFObject::observerNotify
 *
 * notified to #ZFGlobalEventCenter only
 */
ZFOBSERVER_EVENT_GLOBAL(ZFFilePathForCacheBeforeClear)
/**
 * @brief see #ZFObject::observerNotify
 *
 * notified to #ZFGlobalEventCenter only
 */
ZFOBSERVER_EVENT_GLOBAL(ZFFilePathForCacheAfterClear)
ZF_NAMESPACE_END(ZFGlobalEvent)

/**
 * @brief get a proper cache path that app can write files to
 *
 * cache is used for temp files only,
 * may or may not be deleted automatically\n
 * path is ensured to use ZFFileSeparator as separator,
 * and no extra separator would be added to tail
 */
ZFMETHOD_FUNC_DECLARE_0(const zfchar *, ZFFilePathForCache)
/**
 * @brief change the cache path, null to use defalut path
 */
ZFMETHOD_FUNC_DECLARE_1(void, ZFFilePathForCache,
                        ZFMP_IN(const zfchar *, path))
/**
 * @brief clear all caches under #ZFFilePathForCache
 *
 * note, this method is ensured called during #ZFFrameworkCleanup
 * as level #ZFLevelZFFrameworkLow
 */
ZFMETHOD_FUNC_DECLARE_0(void, ZFFilePathForCacheClear)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFFile_path_h_

