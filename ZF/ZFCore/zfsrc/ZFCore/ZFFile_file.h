/**
 * @file ZFFile_file.h
 * @brief file utility
 */

#ifndef _ZFI_ZFFile_file_h_
#define _ZFI_ZFFile_file_h_

#include "ZFFile_fwd.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief return true if file specified by path is exist
 * @note path must be well formed, use #ZFFilePathFormat if necessary
 */
ZFMETHOD_FUNC_DECLARE_1(zfbool, ZFFileFileIsExist,
                        ZFMP_IN(const zfchar *, path))
/**
 * @brief return true if file specified by path is a directory
 * @note path must be well formed, use #ZFFilePathFormat if necessary
 */
ZFMETHOD_FUNC_DECLARE_1(zfbool, ZFFileFileIsDir,
                        ZFMP_IN(const zfchar *, path))

/**
 * @brief make directory
 * @note path must be well formed, use #ZFFilePathFormat if necessary
 */
ZFMETHOD_FUNC_DECLARE_3(zfbool, ZFFileFilePathCreate,
                        ZFMP_IN(const zfchar *, path),
                        ZFMP_IN_OPT(zfbool, autoMakeParent, zftrue),
                        ZFMP_OUT_OPT(zfstring *, errPos, zfnull))

/**
 * @brief copy a file or directory from srcPath to dstPath
 *
 * if both src and dst exist, but one is file and another is dir,
 * return zffalse\n
 * merge directory if dst is an existing dir
 * (if isForce not set,
 * would return false if dst has a child file with the same path in src)\n
 * override file if dst is an existing file and isForce is zftrue
 * @note path must be well formed, use #ZFFilePathFormat if necessary
 */
ZFMETHOD_FUNC_DECLARE_5(zfbool, ZFFileFileCopy,
                        ZFMP_IN(const zfchar *, srcPath),
                        ZFMP_IN(const zfchar *, dstPath),
                        ZFMP_IN_OPT(zfbool, isRecursive, zftrue),
                        ZFMP_IN_OPT(zfbool, isForce, zffalse),
                        ZFMP_IN_OPT(zfstring *, errPos, zfnull))
/**
 * @brief move a file or directory from srcPath to dstPath
 * @note path must be well formed, use #ZFFilePathFormat if necessary
 */
ZFMETHOD_FUNC_DECLARE_5(zfbool, ZFFileFileMove,
                        ZFMP_IN(const zfchar *, srcPath),
                        ZFMP_IN(const zfchar *, dstPath),
                        ZFMP_IN_OPT(zfbool, isRecursive, zftrue),
                        ZFMP_IN_OPT(zfbool, isForce, zffalse),
                        ZFMP_IN_OPT(zfstring *, errPos, zfnull))
/**
 * @brief delete a file or directory from srcPath to dstPath
 *
 * fail if isRecursive is zffalse and dst is a dir
 * @note path must be well formed, use #ZFFilePathFormat if necessary
 */
ZFMETHOD_FUNC_DECLARE_4(zfbool, ZFFileFileRemove,
                        ZFMP_IN(const zfchar *, path),
                        ZFMP_IN_OPT(zfbool, isRecursive, zftrue),
                        ZFMP_IN_OPT(zfbool, isForce, zffalse),
                        ZFMP_IN_OPT(zfstring *, errPos, zfnull))

/**
 * @brief find file or directory, similar to FindFirstFile under Windows
 *
 * path supports path only, without wildcard support,
 * e.g. "/path/" or "/path",
 * "." and ".." won't be included\n
 * typical usage:
 * @code
 *   ZFFileFindData fd;
 *   if(ZFFileFileFindFirst(fd, path))
 *   {
 *     do
 *     {
 *       // do something
 *     } while(ZFFileFileFindNext(fd));
 *     ZFFileFileFindClose(fd);
 *   }
 * @endcode
 * @return false if error or nothing found
 * @warning strings in #ZFFileFindData would be deleted
 *   after each findXXX call,
 *   you must save it if need future use
 * @see ZFFileFindData, ZFFileFileFindFirst, ZFFileFileFindNext, ZFFileFileFindClose
 * @note path must be well formed, use #ZFFilePathFormat if necessary
 */
ZFMETHOD_FUNC_DECLARE_2(zfbool, ZFFileFileFindFirst,
                        ZFMP_IN_OUT(ZFFileFindData &, fd),
                        ZFMP_IN(const zfchar *, path))

/**
 * @see ZFFileFindData, ZFFileFileFindFirst, ZFFileFileFindClose
 */
ZFMETHOD_FUNC_DECLARE_1(zfbool, ZFFileFileFindNext,
                        ZFMP_IN_OUT(ZFFileFindData &, fd))

/**
 * @see ZFFileFindData, ZFFileFileFindFirst, ZFFileFileFindNext
 */
ZFMETHOD_FUNC_DECLARE_1(void, ZFFileFileFindClose,
                        ZFMP_IN_OUT(ZFFileFindData &, fd))

/**
 * @brief open a file for read or write
 * @note path must be well formed, use #ZFFilePathFormat if necessary
 */
ZFMETHOD_FUNC_DECLARE_3(void *, ZFFileFileOpen,
                        ZFMP_IN(const zfchar *, filePath),
                        ZFMP_IN_OPT(ZFFileOpenOptionFlags, flag, ZFFileOpenOption::e_Read),
                        ZFMP_IN_OPT(zfbool, autoCreateParent, zftrue))
/**
 * @brief close and save the file if need, return false if save failed
 */
ZFMETHOD_FUNC_DECLARE_1(zfbool, ZFFileFileClose,
                        ZFMP_IN(void *, token))

/**
 * @brief get current file's position or zfindexMax() if error
 */
ZFMETHOD_FUNC_DECLARE_1(zfindex, ZFFileFileTell,
                        ZFMP_IN(void *, token))
/**
 * @brief similar to fseek, return false if seek out of range
 */
ZFMETHOD_FUNC_DECLARE_3(zfbool, ZFFileFileSeek,
                        ZFMP_IN(void *, token),
                        ZFMP_IN(zfindex, byteSize),
                        ZFMP_IN_OPT(ZFSeekPos, position, ZFSeekPosBegin))

/**
 * @brief read file
 *
 * return size read, even if error occurred
 * (whether error occurred or eof,
 * could be accessed by #ZFFileFileIsError and #ZFFileFileIsEof)\n
 * typical usage:
 * @code
 *   zfindex sizeRead = 0;
 *   while((sizeRead = ZFFileFileRead(token, buf)) > 0
 *       && !ZFFileFileIsError(token))
 *   {
 *       // do something with buf
 *   }
 *   if(ZFFileFileIsError(token))
 *   {
 *       // exit because of error occurred, read fail
 *   }
 *   else if(ZFFileFileIsEof(token))
 *   {
 *       // exit because of eof, read success
 *   }
 * @endcode
 *
 * note that the tail '\0' won't be written to buf
 * @warning ZFFile always read files in binary mode,
 *   for text files,
 *   you must make sure the file is UTF8 encoded without BOM
 */
ZFMETHOD_FUNC_DECLARE_3(zfindex, ZFFileFileRead,
                        ZFMP_IN(void *, token),
                        ZFMP_IN(void *, buf),
                        ZFMP_IN(zfindex, maxByteSize))

/**
 * @brief write file, see #ZFFileFileRead
 *
 * return size written, even if error occurred
 * (whether error occurred or not,
 * could be accessed by #ZFFileFileIsError)\n
 * typical usage:
 * @code
 *   zfindex sizeWritten = ZFFileFileWrite(token, src, size);
 *   if(ZFFileFileIsError(token))
 *   {
 *       // write fail
 *   }
 *   if(sizeWritten != size)
 *   { // you can also check by sizeWritten
 *       // write fail
 *   }
 * @endcode
 * \n
 * maxByteSize could be zfindexMax(),
 * which means #ZFFileFileWrite should be stopped when reached 0x00 in src,
 * usually to output a UTF8 string
 */
ZFMETHOD_FUNC_DECLARE_3(zfindex, ZFFileFileWrite,
                        ZFMP_IN(void *, token),
                        ZFMP_IN(const void *, src),
                        ZFMP_IN_OPT(zfindex, maxByteSize, zfindexMax()))
/**
 * @brief flush the file, useful only for files opened for write
 */
ZFMETHOD_FUNC_DECLARE_1(void, ZFFileFileFlush,
                        ZFMP_IN(void *, token))
/**
 * @brief see #ZFFileFileRead
 */
ZFMETHOD_FUNC_DECLARE_1(zfbool, ZFFileFileIsEof,
                        ZFMP_IN(void *, token))
/**
 * @brief see #ZFFileFileRead
 */
ZFMETHOD_FUNC_DECLARE_1(zfbool, ZFFileFileIsError,
                        ZFMP_IN(void *, token))

/**
 * @brief util method to get file's total size (not left size)
 *
 * #ZFFileFileSeek to end, #ZFFileFileTell, then #ZFFileFileSeek to restore,
 * return zfindexMax() if error\n
 * note that result is not ensured if file is opened in append mode
 */
ZFMETHOD_FUNC_DECLARE_1(zfindex, ZFFileFileSize,
                        ZFMP_IN(void *, token))

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFFile_file_h_

