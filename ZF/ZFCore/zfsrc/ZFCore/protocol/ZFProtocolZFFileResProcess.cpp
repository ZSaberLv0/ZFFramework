#include "ZFProtocolZFFileResProcess.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_INTERFACE_REGISTER(ZFFileResProcess)

// ============================================================
zfbool _ZFP_ZFImpl_ZFFileResProcess_resPathFix(ZF_IN const zfstring &resRootPath, ZF_IN_OUT zfstring &result, ZF_IN const zfstring &original)
{
    result.assign(original, resRootPath.length());
    return zftrue;
}

// ============================================================
const zfstring &ZFPROTOCOL_INTERFACE_CLASS(ZFFileResProcess)::resRootPath(void)
{
    static zfstring d = zfstringWithFormat(".%cres%c", ZFFileSeparator(), ZFFileSeparator());
    return d;
}
zfbool ZFPROTOCOL_INTERFACE_CLASS(ZFFileResProcess)::resIsExist(ZF_IN const zfchar *resPath)
{
    zfstring tmpPath;
    tmpPath += this->resRootPath();
    tmpPath += resPath;
    return ZFFileFileIsExist(tmpPath.cString());
}
zfbool ZFPROTOCOL_INTERFACE_CLASS(ZFFileResProcess)::resIsDir(ZF_IN const zfchar *resPath)
{
    zfstring tmpPath;
    tmpPath += this->resRootPath();
    tmpPath += resPath;
    return ZFFileFileIsDir(tmpPath.cString());
}
zfbool ZFPROTOCOL_INTERFACE_CLASS(ZFFileResProcess)::resCopy(ZF_IN const zfchar *resPath,
                                                             ZF_IN const zfchar *dstPath,
                                                             ZF_IN_OPT zfbool isRecursive /* = zftrue */,
                                                             ZF_IN_OPT zfbool isForce /* = zffalse */,
                                                             ZF_IN_OPT zfstring *errPos /* = zfnull */)
{
    zfstring tmpPath;
    tmpPath += this->resRootPath();
    tmpPath += resPath;
    if(errPos == zfnull)
    {
        return ZFFileFileCopy(tmpPath, dstPath, isRecursive, isForce, zfnull);
    }
    else
    {
        zfstring errPosTmp;
        zfbool ret = ZFFileFileCopy(tmpPath, dstPath, isRecursive, isForce, &errPosTmp);
        if(!ret)
        {
            if(zfstringFind(errPosTmp, this->resRootPath()) == 0)
            {
                *errPos += errPosTmp.cString() + this->resRootPath().length();
            }
            else
            {
                *errPos += errPosTmp;
            }
        }
        return ret;
    }
}

// ============================================================
// res RW
void *ZFPROTOCOL_INTERFACE_CLASS(ZFFileResProcess)::resOpen(ZF_IN const zfchar *resPath)
{
    zfstring tmpPath;
    tmpPath += this->resRootPath();
    tmpPath += resPath;
    return ZFFileFileOpen(tmpPath.cString(), ZFFileOpenOption::e_Read);
}
zfbool ZFPROTOCOL_INTERFACE_CLASS(ZFFileResProcess)::resClose(ZF_IN void *token)
{
    return ZFFileFileClose(token);
}
zfindex ZFPROTOCOL_INTERFACE_CLASS(ZFFileResProcess)::resTell(ZF_IN void *token)
{
    return ZFFileFileTell(token);
}
zfbool ZFPROTOCOL_INTERFACE_CLASS(ZFFileResProcess)::resSeek(ZF_IN void *token,
                                                             ZF_IN zfindex byteSize,
                                                             ZF_IN_OPT ZFSeekPos position /* = ZFSeekPosBegin */)
{
    return ZFFileFileSeek(token, byteSize, position);
}
zfindex ZFPROTOCOL_INTERFACE_CLASS(ZFFileResProcess)::resRead(ZF_IN void *token,
                                                              ZF_IN void *buf,
                                                              ZF_IN zfindex maxByteSize)
{
    return ZFFileFileRead(token, buf, maxByteSize);
}
zfbool ZFPROTOCOL_INTERFACE_CLASS(ZFFileResProcess)::resIsEof(ZF_IN void *token)
{
    return ZFFileFileIsEof(token);
}
zfbool ZFPROTOCOL_INTERFACE_CLASS(ZFFileResProcess)::resIsError(ZF_IN void *token)
{
    return ZFFileFileIsError(token);
}

// ============================================================
// res find
zfbool ZFPROTOCOL_INTERFACE_CLASS(ZFFileResProcess)::resFindFirst(ZF_IN_OUT ZFFileFindData::Impl &fd,
                                                                  ZF_IN const zfchar *resPath)
{
    ZFFileFindData::Impl *normalFd = zfnew(ZFFileFindData::Impl);
    zfstring findPath;
    findPath += this->resRootPath();
    findPath += resPath;
    if(!ZFPROTOCOL_ACCESS(ZFFile)->fileFindFirst(*normalFd, findPath))
    {
        zfdelete(normalFd);
        return zffalse;
    }
    fd.fileName = normalFd->fileName;
    fd.fileIsDir = normalFd->fileIsDir;
    fd.nativeFd = normalFd;
    return zftrue;
}
zfbool ZFPROTOCOL_INTERFACE_CLASS(ZFFileResProcess)::resFindNext(ZF_IN_OUT ZFFileFindData::Impl &fd)
{
    ZFFileFindData::Impl *normalFd = ZFCastStatic(ZFFileFindData::Impl *, fd.nativeFd);
    if(!ZFPROTOCOL_ACCESS(ZFFile)->fileFindNext(*normalFd))
    {
        return zffalse;
    }
    fd.fileName = normalFd->fileName;
    fd.fileIsDir = normalFd->fileIsDir;
    return zftrue;
}
void ZFPROTOCOL_INTERFACE_CLASS(ZFFileResProcess)::resFindClose(ZF_IN_OUT ZFFileFindData::Impl &fd)
{
    ZFFileFindData::Impl *normalFd = ZFCastStatic(ZFFileFindData::Impl *, fd.nativeFd);
    ZFPROTOCOL_ACCESS(ZFFile)->fileFindClose(*normalFd);
    zfdelete(normalFd);
    fd.nativeFd = zfnull;
}

ZF_NAMESPACE_GLOBAL_END

