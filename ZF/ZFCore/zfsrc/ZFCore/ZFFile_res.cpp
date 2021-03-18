#include "ZFFile_impl.cpp"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassLikePOD _ZFP_ZFFileTokenForRes
{
public:
    ZFToken fd; // fd returned by ZFFileFileOpen or ZFFileResOpen
    zfstring resAdditionalPathWithSeparator; // not null if it's located in additional res path

public:
    _ZFP_ZFFileTokenForRes(void)
    : fd(ZFTokenInvalid())
    , resAdditionalPathWithSeparator()
    {
    }
};

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFFileResAdditionalPathDataHolder, ZFLevelZFFrameworkStatic)
{
}
public:
    ZFCoreArray<zfstring> resAdditionalPathList;
ZF_GLOBAL_INITIALIZER_END(ZFFileResAdditionalPathDataHolder)
#define _ZFP_ZFFileResAdditionalPathList (ZF_GLOBAL_INITIALIZER_INSTANCE(ZFFileResAdditionalPathDataHolder)->resAdditionalPathList)

ZFMETHOD_FUNC_DEFINE_1(void, ZFFileResAdditionalPathAdd,
                       ZFMP_IN(const zfchar *, path))
{
    if(path == zfnull || *path == '\0')
    {
        return ;
    }
    zfstring pathFormated;
    ZFFilePathFormat(pathFormated, path);
    _ZFP_ZFFileResAdditionalPathList.add(pathFormated);
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFFileResAdditionalPathRemove,
                       ZFMP_IN(const zfchar *, path))
{
    if(path == zfnull || *path == '\0')
    {
        return ;
    }
    zfstring pathFormated;
    ZFFilePathFormat(pathFormated, path);
    _ZFP_ZFFileResAdditionalPathList.removeElement(pathFormated);
}
ZFMETHOD_FUNC_DEFINE_0(ZFCoreArray<zfstring>, ZFFileResAdditionalPathList)
{
    return _ZFP_ZFFileResAdditionalPathList;
}
ZFMETHOD_FUNC_DEFINE_1(const zfchar *, ZFFileResAdditionalPathCheck,
                       ZFMP_IN(const zfchar *, resPath))
{
    if(zfsIsEmpty(resPath) || zfscmpTheSame(resPath, "."))
    {
        return zfnull;
    }
    for(zfindex i = 0; i < _ZFP_ZFFileResAdditionalPathList.count(); ++i)
    {
        zfstring t = _ZFP_ZFFileResAdditionalPathList[i];
        t += ZFFileSeparator();
        t += resPath;
        if(ZFFileFileIsExist(t))
        {
            return _ZFP_ZFFileResAdditionalPathList[i];
        }
    }
    return zfnull;
}

ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFFileResIsExist,
                       ZFMP_IN(const zfchar *, resPath))
{
    const zfchar *resAdditionalPath = ZFFileResAdditionalPathCheck(resPath);
    if(resAdditionalPath != zfnull)
    {
        return zftrue;
    }
    else
    {
        return ZFPROTOCOL_ACCESS(ZFFileResProcess)->resIsExist(resPath);
    }
}
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFFileResIsDir,
                       ZFMP_IN(const zfchar *, resPath))
{
    const zfchar *resAdditionalPath = ZFFileResAdditionalPathCheck(resPath);
    if(resAdditionalPath != zfnull)
    {
        zfstring tmp = resAdditionalPath;
        tmp += ZFFileSeparator();
        tmp += resPath;
        return ZFFileFileIsDir(tmp);
    }
    else
    {
        return ZFPROTOCOL_ACCESS(ZFFileResProcess)->resIsDir(resPath);
    }
}
ZFMETHOD_FUNC_DEFINE_5(zfbool, ZFFileResCopy,
                       ZFMP_IN(const zfchar *, resPath),
                       ZFMP_IN(const zfchar *, dstPath),
                       ZFMP_IN_OPT(zfbool, isRecursive, zftrue),
                       ZFMP_IN_OPT(zfbool, isForce, zffalse),
                       ZFMP_IN_OPT(zfstring *, errPos, zfnull))
{
    const zfchar *resAdditionalPath = ZFFileResAdditionalPathCheck(resPath);
    if(resAdditionalPath == zfnull)
    {
        return _ZFP_ZFFileResProcessImpl->resCopy(resPath, dstPath, isRecursive, isForce, errPos);
    }
    else
    {
        zfstring resPathTmp;
        resPathTmp += resAdditionalPath;
        resPathTmp += ZFFileSeparator();
        resPathTmp += resPath;
        zfstring errPosTmp;
        zfbool ret = _ZFP_ZFFileImpl->fileCopy(resPathTmp, dstPath, isRecursive, isForce, &errPosTmp);
        if(errPos != zfnull)
        {
            if(!ret)
            {
                zfstring header;
                header += resAdditionalPath;
                header += ZFFileSeparator();
                if(zfstringFind(errPosTmp, header) == 0)
                {
                    errPosTmp.remove(0, header.length());
                }
            }
            *errPos += errPosTmp;
        }
        return ret;
    }
}

#define _ZFP_ZFFileFindType_res "ZFFileResFindFirst"

zfclassNotPOD _ZFP_ZFFileResFindData
{
public:
    zfstring resPathSaved;
    ZFFileFindData resAdditionalFd;
    /*
     * if not empty, the file is find from resAdditionalPath
     * ensured end with file separator
     * ensured clear when find task ends from resAdditionalPath
     */
    zfstring resAdditionalPath;
    zfbool resFindFirstStarted;
public:
    _ZFP_ZFFileResFindData(void)
    : resPathSaved()
    , resAdditionalFd()
    , resAdditionalPath()
    , resFindFirstStarted(zffalse)
    {
    }
public:
    void copyToFd(ZF_OUT ZFFileFindData::Impl &fd) const
    {
        fd.fileName = this->resAdditionalFd.impl().fileName;
        fd.fileIsDir = this->resAdditionalFd.impl().fileIsDir;
    }
};

ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFFileResFindFirst,
                       ZFMP_IN_OUT(ZFFileFindData &, fd),
                       ZFMP_IN(const zfchar *, resPath))
{
    if(resPath == zfnull)
    {
        return zffalse;
    }
    _ZFP_ZFFileResFindData *implUserData = zfnew(_ZFP_ZFFileResFindData);
    fd.implAttach(_ZFP_ZFFileFindType_res, implUserData);
    implUserData->resPathSaved = resPath;

    const zfchar *resAdditionalPath = ZFFileResAdditionalPathCheck(resPath);
    if(resAdditionalPath == zfnull)
    {
        if(!_ZFP_ZFFileResProcessImpl->resFindFirst(fd.impl(), resPath))
        {
            fd.implDetach();
            zfdelete(implUserData);
            return zffalse;
        }
        return zftrue;
    }
    else
    {
        implUserData->resAdditionalPath = resAdditionalPath;
        implUserData->resAdditionalPath += ZFFileSeparator();
        zfstring resPathTmp;
        resPathTmp += implUserData->resAdditionalPath;
        resPathTmp += resPath;
        if(ZFFileFileFindFirst(implUserData->resAdditionalFd, resPathTmp))
        {
            implUserData->copyToFd(fd.impl());
            return zftrue;
        }
        else
        {
            implUserData->resAdditionalPath.removeAll();
            if(!_ZFP_ZFFileResProcessImpl->resFindFirst(fd.impl(), resPath))
            {
                fd.implDetach();
                zfdelete(implUserData);
                return zffalse;
            }
            return zftrue;
        }
    }
}
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFFileResFindNext,
                       ZFMP_IN_OUT(ZFFileFindData &, fd))
{
    _ZFP_ZFFileResFindData *implUserData = (_ZFP_ZFFileResFindData *)fd.implCheck(_ZFP_ZFFileFindType_res);

    if(!implUserData->resAdditionalPath.isEmpty())
    {
        if(ZFFileFileFindNext(implUserData->resAdditionalFd))
        {
            implUserData->copyToFd(fd.impl());
            return zftrue;
        }

        implUserData->resAdditionalPath.removeAll();
        ZFFileFileFindClose(implUserData->resAdditionalFd);

        implUserData->resFindFirstStarted = _ZFP_ZFFileResProcessImpl->resFindFirst(fd.impl(), implUserData->resPathSaved);
        return implUserData->resFindFirstStarted;
    }
    return _ZFP_ZFFileResProcessImpl->resFindNext(fd.impl());
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFFileResFindClose,
                       ZFMP_IN_OUT(ZFFileFindData &, fd))
{
    _ZFP_ZFFileResFindData *implUserData = (_ZFP_ZFFileResFindData *)fd.implCheck(_ZFP_ZFFileFindType_res);

    if(!implUserData->resAdditionalPath.isEmpty())
    {
        ZFFileFileFindClose(implUserData->resAdditionalFd);
    }
    else if(implUserData->resFindFirstStarted)
    {
        _ZFP_ZFFileResProcessImpl->resFindClose(fd.impl());
    }
    fd.implDetach();
    zfdelete(implUserData);
}

ZFMETHOD_FUNC_DEFINE_1(ZFToken, ZFFileResOpen,
                       ZFMP_IN(const zfchar *, resPath))
{
    if(resPath == zfnull)
    {
        return ZFTokenInvalid();
    }

    _ZFP_ZFFileTokenForRes *ret = zfnew(_ZFP_ZFFileTokenForRes);
    const zfchar *resAdditionalPath = ZFFileResAdditionalPathCheck(resPath);
    if(resAdditionalPath == zfnull)
    {
        ret->fd = _ZFP_ZFFileResProcessImpl->resOpen(resPath);
    }
    else
    {
        ret->resAdditionalPathWithSeparator = resAdditionalPath;
        ret->resAdditionalPathWithSeparator += ZFFileSeparator();
        zfstring resPathTmp;
        resPathTmp += ret->resAdditionalPathWithSeparator;
        resPathTmp += resPath;
        ret->fd = ZFFileFileOpen(resPathTmp, ZFFileOpenOption::e_Read);
    }
    if(ret->fd == ZFTokenInvalid())
    {
        zfdelete(ret);
        ret = zfnull;
    }
    return (ZFToken)ret;
}
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFFileResClose,
                       ZFMP_IN(ZFToken, token))
{
    if(token == ZFTokenInvalid())
    {
        return zffalse;
    }

    _ZFP_ZFFileTokenForRes *resToken = (_ZFP_ZFFileTokenForRes *)token;
    zfblockedDelete(resToken);
    if(resToken->resAdditionalPathWithSeparator.isEmpty())
    {
        return _ZFP_ZFFileResProcessImpl->resClose(resToken->fd);
    }
    else
    {
        return ZFFileFileClose(resToken->fd);
    }
}
ZFMETHOD_FUNC_DEFINE_1(zfindex, ZFFileResTell,
                       ZFMP_IN(ZFToken, token))
{
    if(token == ZFTokenInvalid())
    {
        return zffalse;
    }

    _ZFP_ZFFileTokenForRes *resToken = (_ZFP_ZFFileTokenForRes *)token;
    if(resToken->resAdditionalPathWithSeparator.isEmpty())
    {
        return _ZFP_ZFFileResProcessImpl->resTell(resToken->fd);
    }
    else
    {
        return ZFFileFileTell(resToken->fd);
    }
}
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFFileResSeek,
                       ZFMP_IN(ZFToken, token),
                       ZFMP_IN(zfindex, byteSize),
                       ZFMP_IN_OPT(ZFSeekPos, position, ZFSeekPosBegin))
{
    if(token == ZFTokenInvalid())
    {
        return zffalse;
    }

    _ZFP_ZFFileTokenForRes *resToken = (_ZFP_ZFFileTokenForRes *)token;
    if(resToken->resAdditionalPathWithSeparator.isEmpty())
    {
        return _ZFP_ZFFileResProcessImpl->resSeek(resToken->fd, byteSize, position);
    }
    else
    {
        return ZFFileFileSeek(resToken->fd, byteSize, position);
    }
}
ZFMETHOD_FUNC_DEFINE_3(zfindex, ZFFileResRead,
                       ZFMP_IN(ZFToken, token),
                       ZFMP_IN(void *, buf),
                       ZFMP_IN(zfindex, maxByteSize))
{
    if(token == ZFTokenInvalid() || maxByteSize == zfindexMax())
    {
        return 0;
    }

    _ZFP_ZFFileTokenForRes *resToken = (_ZFP_ZFFileTokenForRes *)token;
    if(!resToken->resAdditionalPathWithSeparator.isEmpty())
    {
        return ZFFileFileRead(resToken->fd, buf, maxByteSize);
    }

    if(buf == zfnull)
    {
        zfindex curPos = _ZFP_ZFFileResProcessImpl->resTell(resToken->fd);
        _ZFP_ZFFileResProcessImpl->resSeek(resToken->fd, 0, ZFSeekPosEnd);
        zfindex endPos = _ZFP_ZFFileResProcessImpl->resTell(resToken->fd);
        _ZFP_ZFFileResProcessImpl->resSeek(resToken->fd, curPos, ZFSeekPosBegin);
        return endPos - curPos;
    }
    return _ZFP_ZFFileResProcessImpl->resRead(resToken->fd, buf, maxByteSize);
}
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFFileResIsEof,
                       ZFMP_IN(ZFToken, token))
{
    if(token == ZFTokenInvalid())
    {
        return zffalse;
    }

    _ZFP_ZFFileTokenForRes *resToken = (_ZFP_ZFFileTokenForRes *)token;
    if(resToken->resAdditionalPathWithSeparator.isEmpty())
    {
        return _ZFP_ZFFileResProcessImpl->resIsEof(resToken->fd);
    }
    else
    {
        return ZFFileFileIsEof(resToken->fd);
    }
}
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFFileResIsError,
                       ZFMP_IN(ZFToken, token))
{
    if(token == ZFTokenInvalid())
    {
        return zffalse;
    }

    _ZFP_ZFFileTokenForRes *resToken = (_ZFP_ZFFileTokenForRes *)token;
    if(resToken->resAdditionalPathWithSeparator.isEmpty())
    {
        return _ZFP_ZFFileResProcessImpl->resIsError(resToken->fd);
    }
    else
    {
        return ZFFileFileIsError(resToken->fd);
    }
}

ZFMETHOD_FUNC_DEFINE_1(zfindex, ZFFileResSize,
                       ZFMP_IN(ZFToken, token))
{
    if(token == ZFTokenInvalid())
    {
        return zfindexMax();
    }
    zfindex saved = ZFFileResTell(token);
    ZFFileResSeek(token, 0, ZFSeekPosEnd);
    zfindex size = ZFFileResTell(token);
    ZFFileResSeek(token, saved, ZFSeekPosBegin);
    return size;
}

ZF_NAMESPACE_GLOBAL_END

