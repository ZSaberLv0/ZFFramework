#include "ZFFile_impl.cpp"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFFileFileIsExist,
                       ZFMP_IN(const zfchar *, path))
{
    if(path == zfnull)
    {
        return zffalse;
    }
    return _ZFP_ZFFileImpl->fileIsExist(path);
}

ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFFileFileIsDir,
                       ZFMP_IN(const zfchar *, path))
{
    if(path == zfnull)
    {
        return zffalse;
    }
    return _ZFP_ZFFileImpl->fileIsDir(path);
}

ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFFileFilePathCreate,
                       ZFMP_IN(const zfchar *, path),
                       ZFMP_IN_OPT(zfbool, autoMakeParent, zftrue),
                       ZFMP_OUT_OPT(zfstring *, errPos, zfnull))
{
    if(path == zfnull)
    {
        return zffalse;
    }
    if(*path == '\0')
    {
        return zftrue;
    }
    return _ZFP_ZFFileImpl->filePathCreate(path, autoMakeParent, errPos);
}

ZFMETHOD_FUNC_DEFINE_5(zfbool, ZFFileFileCopy,
                       ZFMP_IN(const zfchar *, srcPath),
                       ZFMP_IN(const zfchar *, dstPath),
                       ZFMP_IN_OPT(zfbool, isRecursive, zftrue),
                       ZFMP_IN_OPT(zfbool, isForce, zffalse),
                       ZFMP_IN_OPT(zfstring *, errPos, zfnull))
{
    if(srcPath == zfnull || dstPath == zfnull)
    {
        return zffalse;
    }
    return _ZFP_ZFFileImpl->fileCopy(srcPath, dstPath, isRecursive, isForce, errPos);
}

ZFMETHOD_FUNC_DEFINE_5(zfbool, ZFFileFileMove,
                       ZFMP_IN(const zfchar *, srcPath),
                       ZFMP_IN(const zfchar *, dstPath),
                       ZFMP_IN_OPT(zfbool, isRecursive, zftrue),
                       ZFMP_IN_OPT(zfbool, isForce, zffalse),
                       ZFMP_IN_OPT(zfstring *, errPos, zfnull))
{
    if(srcPath == zfnull || dstPath == zfnull)
    {
        return zffalse;
    }
    return _ZFP_ZFFileImpl->fileMove(srcPath, dstPath, isRecursive, isForce, errPos);
}
ZFMETHOD_FUNC_DEFINE_4(zfbool, ZFFileFileRemove,
                       ZFMP_IN(const zfchar *, path),
                       ZFMP_IN_OPT(zfbool, isRecursive, zftrue),
                       ZFMP_IN_OPT(zfbool, isForce, zffalse),
                       ZFMP_IN_OPT(zfstring *, errPos, zfnull))
{
    if(path == zfnull)
    {
        return zffalse;
    }
    return _ZFP_ZFFileImpl->fileRemove(path, isRecursive, isForce, errPos);
}

#define _ZFP_ZFFileFindType_file "ZFFileFileFindFirst"

ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFFileFileFindFirst,
                       ZFMP_IN_OUT(ZFFileFindData &, fd),
                       ZFMP_IN(const zfchar *, path))
{
    if(path == zfnull)
    {
        return zffalse;
    }
    fd.implAttach(_ZFP_ZFFileFindType_file);
    if(!_ZFP_ZFFileImpl->fileFindFirst(fd.impl(), path))
    {
        fd.implDetach();
        return zffalse;
    }
    return zftrue;
}
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFFileFileFindNext,
                       ZFMP_IN_OUT(ZFFileFindData &, fd))
{
    fd.implCheck(_ZFP_ZFFileFindType_file);
    return _ZFP_ZFFileImpl->fileFindNext(fd.impl());
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFFileFileFindClose,
                       ZFMP_IN_OUT(ZFFileFindData &, fd))
{
    fd.implCheck(_ZFP_ZFFileFindType_file);
    _ZFP_ZFFileImpl->fileFindClose(fd.impl());
    fd.implDetach();
}

ZFMETHOD_FUNC_DEFINE_3(void *, ZFFileFileOpen,
                       ZFMP_IN(const zfchar *, filePath),
                       ZFMP_IN_OPT(ZFFileOpenOptionFlags, flag, ZFFileOpenOption::e_Read),
                       ZFMP_IN_OPT(zfbool, autoCreateParent, zftrue))
{
    if(autoCreateParent && (zffalse
        || ZFBitTest(flag, ZFFileOpenOption::e_Create)
        || ZFBitTest(flag, ZFFileOpenOption::e_Write)
        || ZFBitTest(flag, ZFFileOpenOption::e_Append)
        ))
    {
        zfstring parentPath;
        if(ZFFilePathParentOf(parentPath, filePath))
        {
            ZFFileFilePathCreate(parentPath);
        }
    }
    return _ZFP_ZFFileReadWriteImpl->fileOpen(filePath, flag);
}
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFFileFileClose,
                       ZFMP_IN(void *, token))
{
    return _ZFP_ZFFileReadWriteImpl->fileClose(token);
}
ZFMETHOD_FUNC_DEFINE_1(zfindex, ZFFileFileTell,
                       ZFMP_IN(void *, token))
{
    return _ZFP_ZFFileReadWriteImpl->fileTell(token);
}
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFFileFileSeek,
                       ZFMP_IN(void *, token),
                       ZFMP_IN(zfindex, byteSize),
                       ZFMP_IN_OPT(ZFSeekPos, position, ZFSeekPosBegin))
{
    return _ZFP_ZFFileReadWriteImpl->fileSeek(token, byteSize, position);
}
ZFMETHOD_FUNC_DEFINE_3(zfindex, ZFFileFileRead,
                       ZFMP_IN(void *, token),
                       ZFMP_IN(void *, buf),
                       ZFMP_IN(zfindex, maxByteSize))
{
    if(maxByteSize == zfindexMax())
    {
        return 0;
    }
    if(buf == zfnull)
    {
        zfindex curPos = _ZFP_ZFFileReadWriteImpl->fileTell(token);
        _ZFP_ZFFileReadWriteImpl->fileSeek(token, 0, ZFSeekPosEnd);
        zfindex endPos = _ZFP_ZFFileReadWriteImpl->fileTell(token);
        _ZFP_ZFFileReadWriteImpl->fileSeek(token, curPos, ZFSeekPosBegin);
        return endPos - curPos;
    }
    return _ZFP_ZFFileReadWriteImpl->fileRead(token, buf, maxByteSize);
}
ZFMETHOD_FUNC_DEFINE_3(zfindex, ZFFileFileWrite,
                       ZFMP_IN(void *, token),
                       ZFMP_IN(const void *, src),
                       ZFMP_IN_OPT(zfindex, maxByteSize, zfindexMax()))
{
    if(src == zfnull)
    {
        return 0;
    }
    return _ZFP_ZFFileReadWriteImpl->fileWrite(token, src,
        (maxByteSize == zfindexMax()) ? (sizeof(zfchar) * zfslen((const zfchar *)src)) : maxByteSize);
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFFileFileFlush,
                       ZFMP_IN(void *, token))
{
    return _ZFP_ZFFileReadWriteImpl->fileFlush(token);
}
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFFileFileIsEof,
                       ZFMP_IN(void *, token))
{
    return _ZFP_ZFFileReadWriteImpl->fileIsEof(token);
}
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFFileFileIsError,
                       ZFMP_IN(void *, token))
{
    return _ZFP_ZFFileReadWriteImpl->fileIsError(token);
}
ZFMETHOD_FUNC_DEFINE_1(zfindex, ZFFileFileSize,
                       ZFMP_IN(void *, token))
{
    if(token == zfnull)
    {
        return zfindexMax();
    }
    zfindex saved = ZFFileFileTell(token);
    if(saved == zfindexMax())
    {
        return zfindexMax();
    }
    ZFFileFileSeek(token, 0, ZFSeekPosEnd);
    zfindex size = ZFFileFileTell(token);
    ZFFileFileSeek(token, saved, ZFSeekPosBegin);
    return size;
}

ZF_NAMESPACE_GLOBAL_END

