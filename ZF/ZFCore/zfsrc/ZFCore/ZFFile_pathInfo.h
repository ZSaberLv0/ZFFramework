/**
 * @file ZFFile_pathInfo.h
 * @brief file utility
 */

#ifndef _ZFI_ZFFile_pathInfo_h_
#define _ZFI_ZFFile_pathInfo_h_

#include "ZFFile_fwd.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
typedef zfbool (*ZFFilePathInfoCallbackIsExist)(ZF_IN const zfchar *pathData);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
typedef zfbool (*ZFFilePathInfoCallbackIsDir)(ZF_IN const zfchar *pathData);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
typedef zfbool (*ZFFilePathInfoCallbackToFileName)(ZF_IN const zfchar *pathData,
                                                   ZF_IN_OUT zfstring &fileName);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
typedef zfbool (*ZFFilePathInfoCallbackToChild)(ZF_IN const zfchar *pathData,
                                                ZF_IN_OUT zfstring &pathDataChild,
                                                ZF_IN const zfchar *childName);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
typedef zfbool (*ZFFilePathInfoCallbackToParent)(ZF_IN const zfchar *pathData,
                                                 ZF_IN_OUT zfstring &pathDataParent);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
typedef zfbool (*ZFFilePathInfoCallbackPathCreate)(ZF_IN const zfchar *pathData,
                                                   ZF_IN_OPT zfbool autoMakeParent,
                                                   ZF_OUT_OPT zfstring *errPos);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
typedef zfbool (*ZFFilePathInfoCallbackRemove)(ZF_IN const zfchar *pathData,
                                               ZF_IN_OPT zfbool isRecursive,
                                               ZF_IN_OPT zfbool isForce,
                                               ZF_IN_OPT zfstring *errPos);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
typedef zfbool (*ZFFilePathInfoCallbackFindFirst)(ZF_IN_OUT ZFFileFindData &fd,
                                                  ZF_IN const zfchar *pathData);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
typedef zfbool (*ZFFilePathInfoCallbackFindNext)(ZF_IN_OUT ZFFileFindData &fd);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
typedef void (*ZFFilePathInfoCallbackFindClose)(ZF_IN_OUT ZFFileFindData &fd);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
typedef void *(*ZFFilePathInfoCallbackOpen)(ZF_IN const zfchar *pathData,
                                            ZF_IN_OPT ZFFileOpenOptionFlags flag,
                                            ZF_IN_OPT zfbool autoCreateParent);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
typedef zfbool (*ZFFilePathInfoCallbackClose)(ZF_IN void *token);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
typedef zfindex (*ZFFilePathInfoCallbackTell)(ZF_IN void *token);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
typedef zfbool (*ZFFilePathInfoCallbackSeek)(ZF_IN void *token,
                                             ZF_IN zfindex byteSize,
                                             ZF_IN_OPT ZFSeekPos position);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
typedef zfindex (*ZFFilePathInfoCallbackRead)(ZF_IN void *token,
                                              ZF_IN void *buf,
                                              ZF_IN zfindex maxByteSize);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
typedef zfindex (*ZFFilePathInfoCallbackWrite)(ZF_IN void *token,
                                               ZF_IN const void *src,
                                               ZF_IN_OPT zfindex maxByteSize);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
typedef void (*ZFFilePathInfoCallbackFlush)(ZF_IN void *token);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
typedef zfbool (*ZFFilePathInfoCallbackIsEof)(ZF_IN void *token);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
typedef zfbool (*ZFFilePathInfoCallbackIsError)(ZF_IN void *token);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
typedef zfindex (*ZFFilePathInfoCallbackSize)(ZF_IN void *token);

// ============================================================
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
extern ZF_ENV_EXPORT zfbool ZFFilePathInfoCallbackIsExistDefault(ZF_IN const zfchar *pathData);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
extern ZF_ENV_EXPORT zfbool ZFFilePathInfoCallbackIsDirDefault(ZF_IN const zfchar *pathData);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
extern ZF_ENV_EXPORT zfbool ZFFilePathInfoCallbackToFileNameDefault(ZF_IN const zfchar *pathData,
                                                                    ZF_IN_OUT zfstring &fileName);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
extern ZF_ENV_EXPORT zfbool ZFFilePathInfoCallbackToChildDefault(ZF_IN const zfchar *pathData,
                                                                 ZF_IN_OUT zfstring &pathDataChild,
                                                                 ZF_IN const zfchar *childName);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
extern ZF_ENV_EXPORT zfbool ZFFilePathInfoCallbackToParentDefault(ZF_IN const zfchar *pathData,
                                                                  ZF_IN_OUT zfstring &pathDataParent);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
extern ZF_ENV_EXPORT zfbool ZFFilePathInfoCallbackPathCreateDefault(ZF_IN const zfchar *pathData,
                                                                    ZF_IN_OPT zfbool autoMakeParent = zftrue,
                                                                    ZF_OUT_OPT zfstring *errPos = zfnull);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
extern ZF_ENV_EXPORT zfbool ZFFilePathInfoCallbackRemoveDefault(ZF_IN const zfchar *pathData,
                                                                ZF_IN_OPT zfbool isRecursive = zftrue,
                                                                ZF_IN_OPT zfbool isForce = zffalse,
                                                                ZF_IN_OPT zfstring *errPos = zfnull);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
extern ZF_ENV_EXPORT zfbool ZFFilePathInfoCallbackFindFirstDefault(ZF_IN_OUT ZFFileFindData &fd,
                                                                   ZF_IN const zfchar *pathData);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
extern ZF_ENV_EXPORT zfbool ZFFilePathInfoCallbackFindNextDefault(ZF_IN_OUT ZFFileFindData &fd);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
extern ZF_ENV_EXPORT void ZFFilePathInfoCallbackFindCloseDefault(ZF_IN_OUT ZFFileFindData &fd);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
extern ZF_ENV_EXPORT void *ZFFilePathInfoCallbackOpenDefault(ZF_IN const zfchar *pathData,
                                                             ZF_IN_OPT ZFFileOpenOptionFlags flag = ZFFileOpenOption::e_Read,
                                                             ZF_IN_OPT zfbool autoCreateParent = zftrue);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
extern ZF_ENV_EXPORT zfbool ZFFilePathInfoCallbackCloseDefault(ZF_IN void *token);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
extern ZF_ENV_EXPORT zfindex ZFFilePathInfoCallbackTellDefault(ZF_IN void *token);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
extern ZF_ENV_EXPORT zfbool ZFFilePathInfoCallbackSeekDefault(ZF_IN void *token,
                                                              ZF_IN zfindex byteSize,
                                                              ZF_IN_OPT ZFSeekPos position = ZFSeekPosBegin);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
extern ZF_ENV_EXPORT zfindex ZFFilePathInfoCallbackReadDefault(ZF_IN void *token,
                                                               ZF_IN void *buf,
                                                               ZF_IN zfindex maxByteSize);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
extern ZF_ENV_EXPORT zfindex ZFFilePathInfoCallbackWriteDefault(ZF_IN void *token,
                                                                ZF_IN const void *src,
                                                                ZF_IN_OPT zfindex maxByteSize = zfindexMax());
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
extern ZF_ENV_EXPORT void ZFFilePathInfoCallbackFlushDefault(ZF_IN void *token);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
extern ZF_ENV_EXPORT zfbool ZFFilePathInfoCallbackIsEofDefault(ZF_IN void *token);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
extern ZF_ENV_EXPORT zfbool ZFFilePathInfoCallbackIsErrorDefault(ZF_IN void *token);
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
extern ZF_ENV_EXPORT zfindex ZFFilePathInfoCallbackSizeDefault(ZF_IN void *token);

// ============================================================
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
ZFMETHOD_FUNC_DECLARE_1(zfbool, ZFFilePathInfoIsExist,
                        ZFMP_IN(const ZFPathInfo &, pathInfo))
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
ZFMETHOD_FUNC_DECLARE_1(zfbool, ZFFilePathInfoIsDir,
                        ZFMP_IN(const ZFPathInfo &, pathInfo))
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
ZFMETHOD_FUNC_DECLARE_2(zfbool, ZFFilePathInfoToFileName,
                        ZFMP_IN(const ZFPathInfo &, pathInfo),
                        ZFMP_IN_OUT(zfstring &, fileName))
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
ZFMETHOD_FUNC_DECLARE_3(zfbool, ZFFilePathInfoToChild,
                        ZFMP_IN(const ZFPathInfo &, pathInfo),
                        ZFMP_IN_OUT(zfstring &, pathDataChild),
                        ZFMP_IN(const zfchar *, childName))
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
ZFMETHOD_FUNC_DECLARE_2(zfbool, ZFFilePathInfoToParent,
                        ZFMP_IN(const ZFPathInfo &, pathInfo),
                        ZFMP_IN_OUT(zfstring &, pathDataParent))
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
ZFMETHOD_FUNC_DECLARE_3(zfbool, ZFFilePathInfoPathCreate,
                        ZFMP_IN(const ZFPathInfo &, pathInfo),
                        ZFMP_IN_OPT(zfbool, autoMakeParent, zftrue),
                        ZFMP_OUT_OPT(zfstring *, errPos, zfnull))
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
ZFMETHOD_FUNC_DECLARE_4(zfbool, ZFFilePathInfoRemove,
                        ZFMP_IN(const ZFPathInfo &, pathInfo),
                        ZFMP_IN_OPT(zfbool, isRecursive, zftrue),
                        ZFMP_IN_OPT(zfbool, isForce, zffalse),
                        ZFMP_IN_OPT(zfstring *, errPos, zfnull))
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
ZFMETHOD_FUNC_DECLARE_2(zfbool, ZFFilePathInfoFindFirst,
                        ZFMP_IN(const ZFPathInfo &, pathInfo),
                        ZFMP_IN_OUT(ZFFileFindData &, fd))
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
ZFMETHOD_FUNC_DECLARE_2(zfbool, ZFFilePathInfoFindNext,
                        ZFMP_IN(const ZFPathInfo &, pathInfo),
                        ZFMP_IN_OUT(ZFFileFindData &, fd))
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
ZFMETHOD_FUNC_DECLARE_2(void, ZFFilePathInfoFindClose,
                        ZFMP_IN(const ZFPathInfo &, pathInfo),
                        ZFMP_IN_OUT(ZFFileFindData &, fd))
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
ZFMETHOD_FUNC_DECLARE_3(void *, ZFFilePathInfoOpen,
                        ZFMP_IN(const ZFPathInfo &, pathInfo),
                        ZFMP_IN_OPT(ZFFileOpenOptionFlags, flag, ZFFileOpenOption::e_Read),
                        ZFMP_IN_OPT(zfbool, autoCreateParent, zftrue))
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
ZFMETHOD_FUNC_DECLARE_2(zfbool, ZFFilePathInfoClose,
                        ZFMP_IN(const ZFPathInfo &, pathInfo),
                        ZFMP_IN(void *, token))
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
ZFMETHOD_FUNC_DECLARE_2(zfindex, ZFFilePathInfoTell,
                        ZFMP_IN(const ZFPathInfo &, pathInfo),
                        ZFMP_IN(void *, token))
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
ZFMETHOD_FUNC_DECLARE_4(zfbool, ZFFilePathInfoSeek,
                        ZFMP_IN(const ZFPathInfo &, pathInfo),
                        ZFMP_IN(void *, token),
                        ZFMP_IN(zfindex, byteSize),
                        ZFMP_IN_OPT(ZFSeekPos, position, ZFSeekPosBegin))
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
ZFMETHOD_FUNC_DECLARE_4(zfindex, ZFFilePathInfoRead,
                        ZFMP_IN(const ZFPathInfo &, pathInfo),
                        ZFMP_IN(void *, token),
                        ZFMP_IN(void *, buf),
                        ZFMP_IN(zfindex, maxByteSize))
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
ZFMETHOD_FUNC_DECLARE_4(zfindex, ZFFilePathInfoWrite,
                        ZFMP_IN(const ZFPathInfo &, pathInfo),
                        ZFMP_IN(void *, token),
                        ZFMP_IN(const void *, src),
                        ZFMP_IN_OPT(zfindex, maxByteSize, zfindexMax()))
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
ZFMETHOD_FUNC_DECLARE_2(void, ZFFilePathInfoFlush,
                        ZFMP_IN(const ZFPathInfo &, pathInfo),
                        ZFMP_IN(void *, token))
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
ZFMETHOD_FUNC_DECLARE_2(zfbool, ZFFilePathInfoIsEof,
                        ZFMP_IN(const ZFPathInfo &, pathInfo),
                        ZFMP_IN(void *, token))
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
ZFMETHOD_FUNC_DECLARE_2(zfbool, ZFFilePathInfoIsError,
                        ZFMP_IN(const ZFPathInfo &, pathInfo),
                        ZFMP_IN(void *, token))
/** @brief see #ZFPATHTYPE_FILEIO_REGISTER */
ZFMETHOD_FUNC_DECLARE_2(zfindex, ZFFilePathInfoSize,
                        ZFMP_IN(const ZFPathInfo &, pathInfo),
                        ZFMP_IN(void *, token))

// ============================================================
/** @brief callback data for #ZFPATHTYPE_FILEIO_REGISTER */
zfclassPOD ZFFilePathInfoData
{
public:
    ZFFilePathInfoCallbackIsExist callbackIsExist; /**< @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    ZFFilePathInfoCallbackIsDir callbackIsDir; /**< @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    ZFFilePathInfoCallbackToFileName callbackToFileName; /**< @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    ZFFilePathInfoCallbackToChild callbackToChild; /**< @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    ZFFilePathInfoCallbackToParent callbackToParent; /**< @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    ZFFilePathInfoCallbackPathCreate callbackPathCreate; /**< @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    ZFFilePathInfoCallbackRemove callbackRemove; /**< @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    ZFFilePathInfoCallbackFindFirst callbackFindFirst; /**< @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    ZFFilePathInfoCallbackFindNext callbackFindNext; /**< @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    ZFFilePathInfoCallbackFindClose callbackFindClose; /**< @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    ZFFilePathInfoCallbackOpen callbackOpen; /**< @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    ZFFilePathInfoCallbackClose callbackClose; /**< @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    ZFFilePathInfoCallbackTell callbackTell; /**< @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    ZFFilePathInfoCallbackSeek callbackSeek; /**< @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    ZFFilePathInfoCallbackRead callbackRead; /**< @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    ZFFilePathInfoCallbackWrite callbackWrite; /**< @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    ZFFilePathInfoCallbackFlush callbackFlush; /**< @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    ZFFilePathInfoCallbackIsEof callbackIsEof; /**< @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    ZFFilePathInfoCallbackIsError callbackIsError; /**< @brief see #ZFPATHTYPE_FILEIO_REGISTER */
    ZFFilePathInfoCallbackSize callbackSize; /**< @brief see #ZFPATHTYPE_FILEIO_REGISTER */
};

extern ZF_ENV_EXPORT void _ZFP_ZFFilePathInfoRegister(ZF_IN const zfchar *pathType,
                                                      ZF_IN const ZFFilePathInfoData &data);
extern ZF_ENV_EXPORT void _ZFP_ZFFilePathInfoUnregister(ZF_IN const zfchar *pathType);

/**
 * @brief register pathType for generic file processing
 *
 * there may be many different file processing methods
 * (normal file, resource file, ftp file, etc),
 * which have quite similar read/write functions,
 * so we introduced this generic version to achieve advanced file processing\n
 * to use, register your path type by this macro,
 * then use #ZFFilePathInfoOpen series to process files
 */
#define ZFPATHTYPE_FILEIO_REGISTER(registerSig, pathType \
        , callbackIsExist_ \
        , callbackIsDir_ \
        , callbackToFileName_ \
        , callbackToChild_ \
        , callbackToParent_ \
        , callbackPathCreate_ \
        , callbackRemove_ \
        , callbackFindFirst_ \
        , callbackFindNext_ \
        , callbackFindClose_ \
        , callbackOpen_ \
        , callbackClose_ \
        , callbackTell_ \
        , callbackSeek_ \
        , callbackRead_ \
        , callbackWrite_ \
        , callbackFlush_ \
        , callbackIsEof_ \
        , callbackIsError_ \
        , callbackSize_ \
    ) \
    ZF_STATIC_REGISTER_INIT(ZFFilePathInfoReg_##registerSig) \
    { \
        ZFFilePathInfoData data; \
        data.callbackIsExist = callbackIsExist_; \
        data.callbackIsDir = callbackIsDir_; \
        data.callbackToFileName = callbackToFileName_; \
        data.callbackToChild = callbackToChild_; \
        data.callbackToParent = callbackToParent_; \
        data.callbackPathCreate = callbackPathCreate_; \
        data.callbackRemove = callbackRemove_; \
        data.callbackFindFirst = callbackFindFirst_; \
        data.callbackFindNext = callbackFindNext_; \
        data.callbackFindClose = callbackFindClose_; \
        data.callbackOpen = callbackOpen_; \
        data.callbackClose = callbackClose_; \
        data.callbackTell = callbackTell_; \
        data.callbackSeek = callbackSeek_; \
        data.callbackRead = callbackRead_; \
        data.callbackWrite = callbackWrite_; \
        data.callbackFlush = callbackFlush_; \
        data.callbackIsEof = callbackIsEof_; \
        data.callbackIsError = callbackIsError_; \
        data.callbackSize = callbackSize_; \
        _ZFP_ZFFilePathInfoRegister(pathType, data); \
    } \
    ZF_STATIC_REGISTER_DESTROY(ZFFilePathInfoReg_##registerSig) \
    { \
        _ZFP_ZFFilePathInfoUnregister(pathType); \
    } \
    ZF_STATIC_REGISTER_END(ZFFilePathInfoReg_##registerSig)

/**
 * @brief get data registered by #ZFPATHTYPE_FILEIO_REGISTER
 */
extern ZF_ENV_EXPORT const ZFFilePathInfoData *ZFFilePathInfoDataForPathType(ZF_IN const zfchar *pathType);
/** @brief see #ZFFilePathInfoDataGetAll */
extern ZF_ENV_EXPORT void ZFFilePathInfoDataGetAllT(ZF_IN_OUT ZFCoreArrayPOD<const zfchar *> &ret);
/**
 * @brief get all data registered by #ZFPATHTYPE_FILEIO_REGISTER
 */
inline ZFCoreArrayPOD<const zfchar *> ZFFilePathInfoDataGetAll(void)
{
    ZFCoreArrayPOD<const zfchar *> ret;
    ZFFilePathInfoDataGetAllT(ret);
    return ret;
}

// ============================================================
// utils
/**
 * @brief util to make a child path info relative to existing one,
 *   see also #ZFFilePathInfoCallbackToChild
 */
ZFMETHOD_FUNC_DECLARE_3(zfbool, ZFPathInfoForLocalFileT,
                        ZFMP_OUT(ZFPathInfo &, ret),
                        ZFMP_IN(const ZFPathInfo &, pathInfo),
                        ZFMP_IN(const zfchar *, childPath))
/**
 * @brief see #ZFPathInfoForLocalFileT
 */
ZFMETHOD_FUNC_DECLARE_2(ZFPathInfo, ZFPathInfoForLocalFile,
                        ZFMP_IN(const ZFPathInfo &, pathInfo),
                        ZFMP_IN(const zfchar *, childPath))

// ============================================================
// ZFInputForPathInfo
/**
 * @brief see #ZFCALLBACK_SERIALIZE_CUSTOM_TYPE_DEFINE
 *
 * serializable data:
 * @code
 *   <node>
 *       <ZFPathInfo category="pathInfo" ... />
 *       <ZFFileOpenOptionFlags category="flags" ... /> // optional, ZFFileOpenOption::e_Read by default
 *   </node>
 * @endcode
 */
#define ZFCallbackSerializeCustomType_ZFInputForPathInfo "ZFInputForPathInfo"

/**
 * @brief util to create a file input callback
 *
 * param:
 * -  (const ZFPathInfo &)pathInfo: see #ZFPathInfo
 * -  (ZFFileOpenOption)flags: flags to open file
 *
 * auto open and auto close files, may return a null callback if open file error\n
 * \n
 * note, additional impl can be attached by #ZFPATHTYPE_FILEIO_REGISTER
 */
ZFMETHOD_FUNC_DECLARE_2(ZFInput, ZFInputForPathInfo,
                        ZFMP_IN(const ZFPathInfo &, pathInfo),
                        ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, ZFFileOpenOption::e_Read))
/**
 * @brief see #ZFInputForPathInfo
 */
ZFMETHOD_FUNC_DECLARE_2(ZFInput, ZFInputForPathInfoString,
                        ZFMP_IN(const zfchar *, pathInfoString),
                        ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, ZFFileOpenOption::e_Read))
/**
 * @brief see #ZFInputForPathInfo
 */
ZFMETHOD_FUNC_DECLARE_3(zfbool, ZFInputForPathInfoStringT,
                        ZFMP_IN_OUT(ZFCallback &, ret),
                        ZFMP_IN(const zfchar *, pathInfoString),
                        ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, ZFFileOpenOption::e_Read))
/**
 * @brief see #ZFInputForPathInfo
 *
 * this method is designed for performance for further impl, by:
 * -  allow set #ZFCallbackSerializeCustomTypeDisable
 * -  less extra copy on pathInfo
 */
ZFMETHOD_FUNC_DECLARE_4(zfbool, ZFInputForPathInfoT,
                        ZFMP_IN_OUT(ZFCallback &, ret),
                        ZFMP_IN(const zfchar *, pathType),
                        ZFMP_IN(const zfchar *, pathData),
                        ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, ZFFileOpenOption::e_Read))

// ============================================================
// ZFOutputForPathInfo
/**
 * @brief see #ZFCALLBACK_SERIALIZE_CUSTOM_TYPE_DEFINE
 *
 * serializable data:
 * @code
 *   <node>
 *       <ZFPathInfo category="pathInfo" ... />
 *       <ZFFileOpenOptionFlags category="flags" ... /> // optional, ZFFileOpenOption::e_Create by default
 *   </node>
 * @endcode
 */
#define ZFCallbackSerializeCustomType_ZFOutputForPathInfo "ZFOutputForPathInfo"

/**
 * @brief util to create a file output callback
 *
 * param:
 * -  (const ZFPathInfo &)pathInfo: see #ZFPathInfo
 * -  (ZFFileOpenOption)flags: flags to open file
 *
 * auto open and auto close files, may return a null callback if open file error\n
 * \n
 * note, additional impl can be attached by #ZFPATHTYPE_FILEIO_REGISTER
 */
ZFMETHOD_FUNC_DECLARE_2(ZFOutput, ZFOutputForPathInfo,
                        ZFMP_IN(const ZFPathInfo &, pathInfo),
                        ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, ZFFileOpenOption::e_Create))
/**
 * @brief see #ZFOutputForPathInfo
 */
ZFMETHOD_FUNC_DECLARE_2(ZFOutput, ZFOutputForPathInfoString,
                        ZFMP_IN(const zfchar *, pathInfoString),
                        ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, ZFFileOpenOption::e_Create))
/**
 * @brief see #ZFOutputForPathInfo
 */
ZFMETHOD_FUNC_DECLARE_3(zfbool, ZFOutputForPathInfoStringT,
                        ZFMP_IN_OUT(ZFCallback &, ret),
                        ZFMP_IN(const zfchar *, pathInfoString),
                        ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, ZFFileOpenOption::e_Create))
/**
 * @brief see #ZFOutputForPathInfo
 *
 * this method is designed for performance for further impl, by:
 * -  allow set #ZFCallbackSerializeCustomTypeDisable
 * -  less extra copy on pathInfo
 */
ZFMETHOD_FUNC_DECLARE_4(zfbool, ZFOutputForPathInfoT,
                        ZFMP_IN_OUT(ZFCallback &, ret),
                        ZFMP_IN(const zfchar *, pathType),
                        ZFMP_IN(const zfchar *, pathData),
                        ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, ZFFileOpenOption::e_Create))

// ============================================================
// ZFInputForLocalFile
/**
 * @brief see #ZFCALLBACK_SERIALIZE_CUSTOM_TYPE_DEFINE
 *
 * serializable data:
 * @code
 *   <node>
 *       <zfstring category="filePath" ... />
 *       <ZFFileOpenOptionFlags category="flags" ... /> // optional, ZFFileOpenOption::e_Read by default
 *   </node>
 * @endcode
 */
#define ZFCallbackSerializeCustomType_ZFInputForLocalFile "ZFInputForLocalFile"

/**
 * @brief util to create a file input callback
 *
 * param:
 * -  (const ZFPathInfo &)pathInfo: see #ZFPathInfo
 * -  (const zfchar *)localPath: local file path to use
 * -  (ZFFileOpenOption)flags: flags to open file
 *
 * auto open and auto close files, may return a null callback if open file error\n
 * \n
 * note, additional impl can be attached by #ZFPATHTYPE_FILEIO_REGISTER
 */
ZFMETHOD_FUNC_DECLARE_3(ZFInput, ZFInputForLocalFile,
                        ZFMP_IN(const ZFPathInfo &, pathInfo),
                        ZFMP_IN(const zfchar *, localPath),
                        ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, ZFFileOpenOption::e_Read))
/**
 * @brief see #ZFInputForLocalFile
 */
ZFMETHOD_FUNC_DECLARE_4(zfbool, ZFInputForLocalFileT,
                        ZFMP_OUT(ZFCallback &, ret),
                        ZFMP_IN(const ZFPathInfo &, pathInfo),
                        ZFMP_IN(const zfchar *, localPath),
                        ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, ZFFileOpenOption::e_Read))

// ============================================================
// ZFOutputForLocalFile
/**
 * @brief see #ZFCALLBACK_SERIALIZE_CUSTOM_TYPE_DEFINE
 *
 * serializable data:
 * @code
 *   <node>
 *       <zfstring category="filePath" ... />
 *       <ZFFileOpenOptionFlags category="flags" ... /> // optional, ZFFileOpenOption::e_Create by default
 *   </node>
 * @endcode
 */
#define ZFCallbackSerializeCustomType_ZFOutputForLocalFile "ZFOutputForLocalFile"

/**
 * @brief util to create a file output callback
 *
 * param:
 * -  (const ZFPathInfo &)pathInfo: see #ZFPathInfo
 * -  (const zfchar *)localPath: local file path to use
 * -  (ZFFileOpenOption)flags: flags to open file
 *
 * auto open and auto close files, may return a null callback if open file error\n
 * \n
 * note, additional impl can be attached by #ZFPATHTYPE_FILEIO_REGISTER
 */
ZFMETHOD_FUNC_DECLARE_3(ZFOutput, ZFOutputForLocalFile,
                        ZFMP_IN(const ZFPathInfo &, pathInfo),
                        ZFMP_IN(const zfchar *, localPath),
                        ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, ZFFileOpenOption::e_Create))

// ============================================================
/**
 * @brief see #ZFSTYLE_DECODER_DEFINE
 *
 * styleKey should start with '@' and holds a #ZFPathInfo,
 * the result style object would be created by
 * #ZFInputForPathInfo and #ZFObjectIOLoad
 */
#define ZFStyleDecoder_pathInfo pathInfo

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFFile_pathInfo_h_

