/**
 * @file ZFFile_res.h
 * @brief file utility
 */

#ifndef _ZFI_ZFFile_res_h_
#define _ZFI_ZFFile_res_h_

#include "ZFFile_fwd.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief add an additional path to search as resource files
 *
 * by default, resource files are placed in a fixed builtin location,
 * you may add additional path to search as resource files\n
 * to make things clear, while accessing resource files using a res path,
 * we would:
 * -# search all additional resource path to find whether the file exists
 * -# if exist, use the additional resource file
 * -# if not exist, use builtin resource file
 * -# if builtin resource file not exist, fail
 *
 *
 * this method would affect all methods that process with resource files,
 * such as #ZFFileResCopy, #ZFFileResOpen, #ZFFileResRead\n
 * typically, additional resource path should be modified during startup only
 */
ZFMETHOD_FUNC_DECLARE_1(void, ZFFileResAdditionalPathAdd,
                        ZFMP_IN(const zfchar *, path))
/** @brief see #ZFFileResAdditionalPathAdd */
ZFMETHOD_FUNC_DECLARE_1(void, ZFFileResAdditionalPathRemove,
                        ZFMP_IN(const zfchar *, path))
/** @brief see #ZFFileResAdditionalPathAdd */
ZFMETHOD_FUNC_DECLARE_0(ZFCoreArray<zfstring>, ZFFileResAdditionalPathList)
/**
 * @brief see #ZFFileResAdditionalPathAdd
 *
 * return the registered additional resource path if file exist,
 * or null if not exist\n
 * if return value is not null,
 * the final file path should be "return value + '/' + resPath"
 * @note path must be well formed, use #ZFFilePathFormat if necessary
 */
ZFMETHOD_FUNC_DECLARE_1(const zfchar *, ZFFileResAdditionalPathCheck,
                        ZFMP_IN(const zfchar *, resPath))

/**
 * @brief return true if res file specified by path is exist
 * @note path must be well formed, use #ZFFilePathFormat if necessary
 */
ZFMETHOD_FUNC_DECLARE_1(zfbool, ZFFileResIsExist,
                        ZFMP_IN(const zfchar *, resPath))
/**
 * @brief return true if res file specified by path is a directory
 * @note path must be well formed, use #ZFFilePathFormat if necessary
 */
ZFMETHOD_FUNC_DECLARE_1(zfbool, ZFFileResIsDir,
                        ZFMP_IN(const zfchar *, resPath))

/**
 * @brief res version of #ZFFileFileCopy
 * @note path must be well formed, use #ZFFilePathFormat if necessary
 */
ZFMETHOD_FUNC_DECLARE_5(zfbool, ZFFileResCopy,
                        ZFMP_IN(const zfchar *, resPath),
                        ZFMP_IN(const zfchar *, dstPath),
                        ZFMP_IN_OPT(zfbool, isRecursive, zftrue),
                        ZFMP_IN_OPT(zfbool, isForce, zffalse),
                        ZFMP_IN_OPT(zfstring *, errPos, zfnull))

/**
 * @see see #ZFFileResOpen, #ZFFileFileFindFirst
 *
 * use empty string to find resource's root directory
 * @note path must be well formed, use #ZFFilePathFormat if necessary
 */
ZFMETHOD_FUNC_DECLARE_2(zfbool, ZFFileResFindFirst,
                        ZFMP_IN_OUT(ZFFileFindData &, fd),
                        ZFMP_IN(const zfchar *, resPath))
/**
 * @see see #ZFFileResOpen, #ZFFileFileFindNext
 */
ZFMETHOD_FUNC_DECLARE_1(zfbool, ZFFileResFindNext,
                        ZFMP_IN_OUT(ZFFileFindData &, fd))
/**
 * @see see #ZFFileResOpen, #ZFFileFileFindClose
 */
ZFMETHOD_FUNC_DECLARE_1(void, ZFFileResFindClose,
                        ZFMP_IN_OUT(ZFFileFindData &, fd))

/**
 * @brief open a resource file for read only, see #ZFFileFileOpen
 *
 * since the resource files may or may not be opened by normal method
 * (i.e. #ZFFileFileOpen), such as those in Android project which are located in
 * apk/assets, so use #ZFFileResOpen and #ZFFileResFindFirst to access it for cross-platform\n
 * also, since the resource files may or may not be compressed into app,
 * accessing them may or may not take much time
 * \n
 * a resource file is a file in "zfres" directory of native project,
 * such as "project_path/zfres/1.png" and "project_path/zfres/subdir/1.png",
 * which should be "1.png" and "subdir/1.png" when ZFFileResOpen\n
 * file name could be the same if under different directories,
 * and is recommended to keep all lower case for cross-platform\n
 * \n
 * note that the token is defined same type as the one used by #ZFFileFileOpen,
 * it's your responsibility to make sure not to misuse it
 * @note path must be well formed, use #ZFFilePathFormat if necessary
 */
ZFMETHOD_FUNC_DECLARE_1(void *, ZFFileResOpen,
                        ZFMP_IN(const zfchar *, resPath))
/**
 * @brief see #ZFFileResOpen #ZFFileFileClose
 */
ZFMETHOD_FUNC_DECLARE_1(zfbool, ZFFileResClose,
                        ZFMP_IN(void *, token))

/**
 * @brief see #ZFFileResOpen #ZFFileFileTell
 */
ZFMETHOD_FUNC_DECLARE_1(zfindex, ZFFileResTell,
                        ZFMP_IN(void *, token))
/**
 * @brief see #ZFFileResOpen #ZFFileFileSeek
 */
ZFMETHOD_FUNC_DECLARE_3(zfbool, ZFFileResSeek,
                        ZFMP_IN(void *, token),
                        ZFMP_IN(zfindex, byteSize),
                        ZFMP_IN_OPT(ZFSeekPos, position, ZFSeekPosBegin))

/**
 * @brief see #ZFFileResOpen #ZFFileFileRead
 */
ZFMETHOD_FUNC_DECLARE_3(zfindex, ZFFileResRead,
                        ZFMP_IN(void *, token),
                        ZFMP_IN(void *, buf),
                        ZFMP_IN(zfindex, maxByteSize))

/**
 * @brief see #ZFFileResOpen #ZFFileFileIsEof
 */
ZFMETHOD_FUNC_DECLARE_1(zfbool, ZFFileResIsEof,
                        ZFMP_IN(void *, token))
/**
 * @brief see #ZFFileResOpen #ZFFileFileIsEof
 */
ZFMETHOD_FUNC_DECLARE_1(zfbool, ZFFileResIsError,
                        ZFMP_IN(void *, token))

/**
 * @brief see #ZFFileResOpen, #ZFFileFileSize
 */
ZFMETHOD_FUNC_DECLARE_1(zfindex, ZFFileResSize,
                        ZFMP_IN(void *, token))

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFFile_res_h_

