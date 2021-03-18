#include "ZFPathType_file.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPATHTYPE_DEFINE(file)

// ============================================================
ZFPATHTYPE_FILEIO_REGISTER(file, ZFPathType_file()
        , ZFFileFileIsExist
        , ZFFileFileIsDir
        , ZFFilePathInfoCallbackToFileNameDefault
        , ZFFilePathInfoCallbackToChildDefault
        , ZFFilePathInfoCallbackToParentDefault
        , ZFFileFilePathCreate
        , ZFFileFileRemove
        , ZFFileFileFindFirst
        , ZFFileFileFindNext
        , ZFFileFileFindClose
        , ZFFileFileOpen
        , ZFFileFileClose
        , ZFFileFileTell
        , ZFFileFileSeek
        , ZFFileFileRead
        , ZFFileFileWrite
        , ZFFileFileFlush
        , ZFFileFileIsEof
        , ZFFileFileIsError
        , ZFFileFileSize
    )

// ============================================================
// ZFInputForFile
ZFMETHOD_FUNC_INLINE_DEFINE_2(ZFInput, ZFInputForFile,
                              ZFMP_IN(const zfchar *, filePath),
                              ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, ZFFileOpenOption::e_Read))

// ============================================================
// ZFOutputForFile
ZFMETHOD_FUNC_INLINE_DEFINE_2(ZFOutput, ZFOutputForFile,
                              ZFMP_IN(const zfchar *, filePath),
                              ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, ZFFileOpenOption::e_Create))

ZF_NAMESPACE_GLOBAL_END

