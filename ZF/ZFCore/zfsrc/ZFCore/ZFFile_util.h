/**
 * @file ZFFile_util.h
 * @brief file utility
 */

#ifndef _ZFI_ZFFile_util_h_
#define _ZFI_ZFFile_util_h_

#include "ZFFile_file.h"
#include "ZFFile_res.h"
#include "ZFFile_pathInfo.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief format path according #ZFFileSeparator
 *
 * format rules:
 * -  ensured no '/' at tail
 * -  all extra '/' and '\\' would be replaced with single '/'
 * -  strip spaces and tabs at head and tail
 * -  replace these escape chars:
 *   -  `\"` to double quote
 *   -  `\ ` to space
 * -  remove "./" at head
 * -  remove "/." at tail
 * -  remove "/./" at anywhere
 * -  null src is treated as error, while empty string src is not an error
 * -  single "/" src would result empty string
 */
ZFMETHOD_FUNC_DECLARE_3(zfbool, ZFFilePathFormat,
                        ZFMP_IN_OUT(zfstring &, ret),
                        ZFMP_IN(const zfchar *, src),
                        ZFMP_IN_OPT(zfindex, srcLen, zfindexMax()))

/**
 * @brief util method to resolve ".." in path
 */
ZFMETHOD_FUNC_DECLARE_1(void, ZFFilePathFormatRelative,
                        ZFMP_IN_OUT(zfstring &, ret))

/**
 * @brief get file name from path or src if error,
 *   e.g. "file.ext" from "/path/file.ext"
 * @note path must be well formed, use #ZFFilePathFormat if necessary
 */
ZFMETHOD_FUNC_DECLARE_2(zfbool, ZFFileNameOf,
                        ZFMP_OUT(zfstring &, ret),
                        ZFMP_IN(const zfchar *, src))
/** @brief see #ZFFileNameOf */
ZFMETHOD_FUNC_DECLARE_1(zfstring, ZFFileNameOf,
                        ZFMP_IN(const zfchar *, src))
/**
 * @brief get file name without extension from path or src if error,
 *   e.g. "file.ext0" from "/path/file.ext0.ext1"
 * @note path must be well formed, use #ZFFilePathFormat if necessary
 */
ZFMETHOD_FUNC_DECLARE_2(zfbool, ZFFileNameOfWithoutExt,
                        ZFMP_OUT(zfstring &, ret),
                        ZFMP_IN(const zfchar *, src))
/** @brief see #ZFFileNameOfWithoutExt */
ZFMETHOD_FUNC_DECLARE_1(zfstring, ZFFileNameOfWithoutExt,
                        ZFMP_IN(const zfchar *, src))
/**
 * @brief get file extension from path or empty if error,
 *   e.g. "ext1" from "/path/file.ext0.ext1",
 *   or empty if no extension
 * @note path must be well formed, use #ZFFilePathFormat if necessary
 */
ZFMETHOD_FUNC_DECLARE_2(zfbool, ZFFileExtOf,
                        ZFMP_OUT(zfstring &, ret),
                        ZFMP_IN(const zfchar *, src))
/** @brief see #ZFFileExtOf */
ZFMETHOD_FUNC_DECLARE_1(zfstring, ZFFileExtOf,
                        ZFMP_IN(const zfchar *, src))
/**
 * @brief get file path without ext,
 *   e.g. "/path/file.ext0" from "/path/file.ext0.ext1",
 *   or original string if no extension
 * @note path must be well formed, use #ZFFilePathFormat if necessary
 */
ZFMETHOD_FUNC_DECLARE_2(zfbool, ZFFilePathOfWithoutExt,
                        ZFMP_OUT(zfstring &, ret),
                        ZFMP_IN(const zfchar *, src))
/** @brief see #ZFFileExtOf */
ZFMETHOD_FUNC_DECLARE_1(zfstring, ZFFilePathOfWithoutExt,
                        ZFMP_IN(const zfchar *, src))
/**
 * @brief get parent path
 *
 * return empty string if no parent found
 * @note path must be well formed, use #ZFFilePathFormat if necessary
 */
ZFMETHOD_FUNC_DECLARE_2(zfbool, ZFFilePathParentOf,
                        ZFMP_OUT(zfstring &, ret),
                        ZFMP_IN(const zfchar *, src))
/** @brief see #ZFFilePathParentOf */
ZFMETHOD_FUNC_DECLARE_1(zfstring, ZFFilePathParentOf,
                        ZFMP_IN(const zfchar *, src))
/**
 * @brief get each file components from path or empty if error,
 *   e.g. {"path", "file.ext"}(as array) from "/path/file.ext"
 * @note path must be well formed, use #ZFFilePathFormat if necessary
 */
ZFMETHOD_FUNC_DECLARE_2(zfbool, ZFFilePathComponentsOf,
                        ZFMP_IN_OUT(ZFCoreArray<zfstring> &, ret),
                        ZFMP_IN(const zfchar *, src))
/** @brief see #ZFFilePathComponentsOf */
ZFMETHOD_FUNC_DECLARE_1(ZFCoreArray<zfstring>, ZFFilePathComponentsOf,
                        ZFMP_IN(const zfchar *, src))

// ============================================================
zfclassLikePOD ZF_ENV_EXPORT _ZFP_ZFFileFileCloseHolder
{
public:
    _ZFP_ZFFileFileCloseHolder(ZF_IN ZFToken token) : token(token) {}
    ~_ZFP_ZFFileFileCloseHolder(void)
    {
        if(this->token != ZFTokenInvalid())
        {
            ZFFileFileClose(this->token);
        }
    }
private:
    ZFToken token;
};
/**
 * @brief util method to call #ZFFileFileClose after code block
 */
#define ZFFileFileCloseHolder(token) _ZFP_ZFFileFileCloseHolder ZFUniqueName(ZFFileFileCloseHolder)(token)

// ============================================================
zfclassLikePOD ZF_ENV_EXPORT _ZFP_ZFFileResCloseHolder
{
public:
    _ZFP_ZFFileResCloseHolder(ZF_IN ZFToken token) : token(token) {}
    ~_ZFP_ZFFileResCloseHolder(void)
    {
        if(this->token != ZFTokenInvalid())
        {
            ZFFileResClose(this->token);
        }
    }
private:
    ZFToken token;
};
/**
 * @brief util method to call #ZFFileResClose after code block
 */
#define ZFFileResCloseHolder(token) _ZFP_ZFFileResCloseHolder ZFUniqueName(ZFFileResCloseHolder)(token)

// ============================================================
zfclassLikePOD ZF_ENV_EXPORT _ZFP_ZFFilePathInfoCloseHolder
{
public:
    _ZFP_ZFFilePathInfoCloseHolder(ZF_IN const ZFPathInfo &pathInfo, ZF_IN ZFToken token) : pathInfo(pathInfo), token(token) {}
    ~_ZFP_ZFFilePathInfoCloseHolder(void)
    {
        if(this->token != ZFTokenInvalid())
        {
            ZFFilePathInfoClose(this->pathInfo, this->token);
        }
    }
private:
    const ZFPathInfo &pathInfo;
    ZFToken token;
};
/**
 * @brief util method to call #ZFFilePathInfoClose after code block
 */
#define ZFFilePathInfoCloseHolder(pathInfo, token) _ZFP_ZFFilePathInfoCloseHolder ZFUniqueName(ZFFilePathInfoCloseHolder)(pathInfo, token)

// ============================================================
/**
 * @brief list all files, usually for debug use
 */
ZFMETHOD_FUNC_DECLARE_4(void, ZFFilePathInfoTreePrint,
                        ZFMP_IN(const ZFPathInfo &, pathInfo),
                        ZFMP_IN_OPT(const ZFOutput &, outputCallback, ZFOutputDefault()),
                        ZFMP_IN_OPT(const zfchar *, headToken, zfnull),
                        ZFMP_IN_OPT(const zfchar *, indentToken, "  "))

// ============================================================
/**
 * @brief util method to loop each child file in specified pathInfo
 *
 * usage:
 * @code
 *   ZFLISTENER_LOCAL(fileCallback, {
 *       const ZFPathInfo &pathInfo = listenerData.param0<v_ZFPathInfo *>()->zfv;
 *       const ZFFileFindData &fd = listenerData.param1<v_ZFFileFindData *>()->zfv;
 *       // do your things with the file
 *   })
 *   ZFFilePathInfoForEach(pathInfo, fileCallback);
 * @endcode
 *
 * fileCallback's param0 is #v_ZFPathInfo that describe the child file or dir,
 * and param1 is #v_ZFFileFindData
 */
ZFMETHOD_FUNC_DECLARE_3(zfbool, ZFFilePathInfoForEach,
                        ZFMP_IN(const ZFPathInfo &, pathInfo),
                        ZFMP_IN(const ZFListener &, fileCallback),
                        ZFMP_IN_OPT(ZFObject *, userData, zfnull))

// ============================================================
// ZFIOBufferedCallbackUsingTmpFile
zfclassFwd _ZFP_ZFIOBufferedCallbackUsingTmpFilePrivate;
/**
 * @brief see #ZFIOBufferedCallback
 *
 * note, use this class only for large IO cache,
 * otherwise, #ZFIOBufferedCallbackUsingBuffer would be better
 */
zfclassLikePOD ZF_ENV_EXPORT ZFIOBufferedCallbackUsingTmpFile : zfextendsLikePOD ZFIOBufferedCallback
{
public:
    /** @cond ZFPrivateDoc */
    ZFIOBufferedCallbackUsingTmpFile(void);
    ZFIOBufferedCallbackUsingTmpFile(ZF_IN const ZFIOBufferedCallbackUsingTmpFile &ref);
    ZFIOBufferedCallbackUsingTmpFile &operator = (ZF_IN const ZFIOBufferedCallbackUsingTmpFile &ref);
    /** @endcond */
    virtual ~ZFIOBufferedCallbackUsingTmpFile(void);

public:
    zfoverride
    virtual ZFInput inputCallback(void);
    zfoverride
    virtual ZFOutput outputCallback(void);

public:
    zfoverride
    virtual void resetInput(void);
    zfoverride
    virtual void resetOutput(void);

private:
    _ZFP_ZFIOBufferedCallbackUsingTmpFilePrivate *d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFFile_util_h_

