#include "ZFPathType_res.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPATHTYPE_DEFINE(res)

// ============================================================
static ZFToken _ZFP_ZFFilePathInfoOpenForRes(ZF_IN const zfchar *filePath,
                                             ZF_IN_OPT ZFFileOpenOptionFlags flag = ZFFileOpenOption::e_Read,
                                             ZF_IN_OPT zfbool autoCreateParent = zftrue)
{
    if(flag != ZFFileOpenOption::e_Read)
    {
        return ZFTokenInvalid();
    }
    return ZFFileResOpen(filePath);
}
ZFPATHTYPE_FILEIO_REGISTER(res, ZFPathType_res()
        , ZFFileResIsExist
        , ZFFileResIsDir
        , ZFFilePathInfoCallbackToFileNameDefault
        , ZFFilePathInfoCallbackToChildDefault
        , ZFFilePathInfoCallbackToParentDefault
        , ZFFilePathInfoCallbackPathCreateDefault
        , ZFFilePathInfoCallbackRemoveDefault
        , ZFFileResFindFirst
        , ZFFileResFindNext
        , ZFFileResFindClose
        , _ZFP_ZFFilePathInfoOpenForRes
        , ZFFileResClose
        , ZFFileResTell
        , ZFFileResSeek
        , ZFFileResRead
        , ZFFilePathInfoCallbackWriteDefault
        , ZFFilePathInfoCallbackFlushDefault
        , ZFFileResIsEof
        , ZFFileResIsError
        , ZFFileResSize
    )

// ============================================================
// ZFInputForResFile
ZFMETHOD_FUNC_INLINE_DEFINE_1(ZFInput, ZFInputForResFile,
                              ZFMP_IN(const zfchar *, resFilePath))

ZF_NAMESPACE_GLOBAL_END

