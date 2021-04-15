#include "ZFCompress.h"
#include "protocol/ZFProtocolZFCompress.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFENUM_DEFINE(ZFCompressLevel)

// ============================================================
// base api
ZFMETHOD_FUNC_DEFINE_2(void *, ZFCompressBegin,
                       ZFMP_IN_OUT(const ZFOutput &, outputZip),
                       ZFMP_IN_OPT(ZFCompressLevelEnum, compressLevel, ZFCompressLevel::EnumDefault()))
{
    if(!outputZip.callbackIsValid())
    {
        return zfnull;
    }
    else
    {
        return ZFPROTOCOL_ACCESS(ZFCompress)->compressBegin(outputZip, compressLevel);
    }
}
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFCompressEnd,
                       ZFMP_IN(void *, compressToken))
{
    if(compressToken == zfnull)
    {
        return zffalse;
    }
    else
    {
        return ZFPROTOCOL_ACCESS(ZFCompress)->compressEnd(compressToken);
    }
}
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFCompressContent,
                       ZFMP_IN_OUT(void *, compressToken),
                       ZFMP_IN_OUT(const ZFInput &, inputRaw),
                       ZFMP_IN(const zfchar *, filePathInZip))
{
    if(compressToken == zfnull || !inputRaw.callbackIsValid() || zfsIsEmpty(filePathInZip))
    {
        return zffalse;
    }
    else
    {
        return ZFPROTOCOL_ACCESS(ZFCompress)->compressContent(compressToken, inputRaw, filePathInZip);
    }
}
ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFCompressContentDir,
                       ZFMP_IN_OUT(void *, compressToken),
                       ZFMP_IN(const zfchar *, filePathInZip))
{
    if(compressToken == zfnull || zfsIsEmpty(filePathInZip))
    {
        return zffalse;
    }
    else
    {
        return ZFPROTOCOL_ACCESS(ZFCompress)->compressContentDir(compressToken, filePathInZip);
    }
}

ZFMETHOD_FUNC_DEFINE_1(void *, ZFDecompressBegin,
                       ZFMP_IN_OUT(const ZFInput &, inputZip))
{
    if(!inputZip.callbackIsValid())
    {
        return zfnull;
    }
    else
    {
        return ZFPROTOCOL_ACCESS(ZFCompress)->decompressBegin(inputZip);
    }
}
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFDecompressEnd,
                       ZFMP_IN(void *, decompressToken))
{
    if(decompressToken == zfnull)
    {
        return zffalse;
    }
    else
    {
        return ZFPROTOCOL_ACCESS(ZFCompress)->decompressEnd(decompressToken);
    }
}
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFDecompressContentAtIndex,
                       ZFMP_IN_OUT(void *, decompressToken),
                       ZFMP_IN_OUT(const ZFOutput &, outputRaw),
                       ZFMP_IN(zfindex, fileIndexInZip))
{
    if(decompressToken == zfnull || !outputRaw.callbackIsValid() || fileIndexInZip == zfindexMax())
    {
        return zffalse;
    }
    else
    {
        return ZFPROTOCOL_ACCESS(ZFCompress)->decompressContent(decompressToken, outputRaw, fileIndexInZip);
    }
}
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFDecompressContent,
                       ZFMP_IN_OUT(void *, decompressToken),
                       ZFMP_IN_OUT(const ZFOutput &, outputRaw),
                       ZFMP_IN(const zfchar *, filePathInZip))
{
    if(decompressToken == zfnull || !outputRaw.callbackIsValid() || zfsIsEmpty(filePathInZip))
    {
        return zffalse;
    }
    else
    {
        return ZFPROTOCOL_ACCESS(ZFCompress)->decompressContent(
            decompressToken, outputRaw, ZFDecompressContentIndex(decompressToken, filePathInZip));
    }
}
ZFMETHOD_FUNC_DEFINE_1(zfindex, ZFDecompressContentCount,
                       ZFMP_IN(void *, decompressToken))
{
    if(decompressToken == zfnull)
    {
        return zffalse;
    }
    else
    {
        return ZFPROTOCOL_ACCESS(ZFCompress)->decompressContentCount(decompressToken);
    }
}
ZFMETHOD_FUNC_DEFINE_2(zfindex, ZFDecompressContentIndex,
                       ZFMP_IN(void *, decompressToken),
                       ZFMP_IN(const zfchar *, filePathInZip))
{
    if(decompressToken == zfnull || zfsIsEmpty(filePathInZip))
    {
        return zffalse;
    }
    else
    {
        return ZFPROTOCOL_ACCESS(ZFCompress)->decompressContentIndex(decompressToken, filePathInZip);
    }
}
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFDecompressContentPathT,
                       ZFMP_IN(void *, decompressToken),
                       ZFMP_IN_OUT(zfstring &, filePathInZip),
                       ZFMP_IN(zfindex, fileIndexInZip))
{
    if(decompressToken == zfnull || fileIndexInZip == zfindexMax())
    {
        return zffalse;
    }
    else
    {
        return ZFPROTOCOL_ACCESS(ZFCompress)->decompressContentPath(decompressToken, filePathInZip, fileIndexInZip);
    }
}
ZFMETHOD_FUNC_DEFINE_2(zfstring, ZFDecompressContentPath,
                       ZFMP_IN(void *, decompressToken),
                       ZFMP_IN(zfindex, fileIndexInZip))
{
    zfstring ret;
    ZFDecompressContentPathT(decompressToken, ret, fileIndexInZip);
    return ret;
}

// ============================================================
// util
ZFMETHOD_FUNC_DEFINE_4(zfbool, ZFCompress,
                       ZFMP_IN_OUT(const ZFOutput &, outputZip),
                       ZFMP_IN_OUT(const ZFInput &, inputRaw),
                       ZFMP_IN_OPT(ZFCompressLevelEnum, compressLevel, ZFCompressLevel::EnumDefault()),
                       ZFMP_IN_OPT(const zfchar *, filePathInZip, _ZFP_ZFCompressFilePathDefault))
{
    void *compressToken = ZFCompressBegin(outputZip, compressLevel);
    if(compressToken == zfnull) {return zffalse;}
    zfbool success = zftrue;
    success &= ZFCompressContent(compressToken, inputRaw, filePathInZip);
    success &= ZFCompressEnd(compressToken);
    return success;
}
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFDecompress,
                       ZFMP_IN_OUT(const ZFOutput &, outputRaw),
                       ZFMP_IN_OUT(const ZFInput &, inputZip),
                       ZFMP_IN_OPT(const zfchar *, filePathInZip, _ZFP_ZFCompressFilePathDefault))
{
    void *decompressToken = ZFDecompressBegin(inputZip);
    if(decompressToken == zfnull) {return zffalse;}
    zfbool success = zftrue;
    success &= ZFDecompressContent(decompressToken, outputRaw, filePathInZip);
    success &= ZFDecompressEnd(decompressToken);
    return success;
}

static zfbool _ZFP_ZFCompressDir(ZF_IN_OUT void *compressToken,
                                 ZF_IN const ZFFilePathInfoImpl &fileImpl,
                                 ZF_IN const zfchar *pathType,
                                 ZF_IN const zfchar *pathData,
                                 ZF_IN const zfchar *parentPathInZip)
{
    // prepare param
    zfstring inputName;
    if(!fileImpl.callbackToFileName(pathData, inputName))
    {
        return zffalse;
    }

    zfstring filePathInZip = parentPathInZip;
    if(!filePathInZip.isEmpty())
    {
        filePathInZip += ZFFileSeparator();
    }
    filePathInZip += inputName;

    if(!filePathInZip.isEmpty())
    {
        // file
        if(fileImpl.callbackIsExist(pathData)
            && !fileImpl.callbackIsDir(pathData))
        {
            ZFInput inputRaw;
            if(!ZFInputForPathInfoT(inputRaw,
                pathType, pathData,
                ZFFileOpenOption::e_Read))
            {
                return zffalse;
            }
            return ZFCompressContent(compressToken, inputRaw, filePathInZip);
        }
        else
        {
            if(!ZFCompressContentDir(compressToken, filePathInZip))
            {
                return zffalse;
            }
        }
    }

    // dir
    ZFFileFindData fd;
    zfbool success = zftrue;
    if(fileImpl.callbackFindFirst(fd, pathData))
    {
        do
        {
            zfstring pathDataChild;
            if(!fileImpl.callbackToChild(pathData, pathDataChild, fd.fileName()))
            {
                success = zffalse;
                break;
            }
            if(!_ZFP_ZFCompressDir(compressToken, fileImpl, pathType, pathDataChild, filePathInZip))
            {
                success = zffalse;
                break;
            }
        } while(fileImpl.callbackFindNext(fd));
        fileImpl.callbackFindClose(fd);
    }
    return success;
}
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFCompressDir,
                       ZFMP_IN_OUT(const ZFOutput &, outputZip),
                       ZFMP_IN(const ZFPathInfo &, inputPathInfo),
                       ZFMP_IN_OPT(ZFCompressLevelEnum, compressLevel, ZFCompressLevel::EnumDefault()))
{
    const ZFFilePathInfoImpl *fileImpl = ZFFilePathInfoImplForPathType(inputPathInfo.pathType);
    if(fileImpl == zfnull)
    {
        return zffalse;
    }

    void *compressToken = ZFCompressBegin(outputZip, compressLevel);
    if(compressToken == zfnull)
    {
        return zffalse;
    }

    zfbool success = zftrue;
    success &= _ZFP_ZFCompressDir(compressToken, *fileImpl, inputPathInfo.pathType, inputPathInfo.pathData, "");
    success &= ZFCompressEnd(compressToken);
    return success;
}
ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFDecompressDir,
                       ZFMP_IN(const ZFPathInfo &, outputPathInfo),
                       ZFMP_IN_OUT(const ZFInput &, inputZip))
{
    const ZFFilePathInfoImpl *fileImpl = ZFFilePathInfoImplForPathType(outputPathInfo.pathType);
    if(fileImpl == zfnull)
    {
        return zffalse;
    }

    void *decompressToken = ZFDecompressBegin(inputZip);
    if(decompressToken == zfnull)
    {
        return zffalse;
    }

    zfindex fileCountInZip = ZFDecompressContentCount(decompressToken);
    zfbool success = zftrue;
    do
    {
        for(zfindex fileIndexInZip = 0; fileIndexInZip < fileCountInZip; ++fileIndexInZip)
        {
            zfstring filePathInZip;
            if(!ZFDecompressContentPathT(decompressToken, filePathInZip, fileIndexInZip))
            {
                success = zffalse;
                break;
            }

            zfstring outputPath;
            if(!fileImpl->callbackToChild(outputPathInfo.pathData, outputPath, filePathInZip))
            {
                success = zffalse;
                break;
            }

            if(!filePathInZip.isEmpty() && filePathInZip[filePathInZip.length() - 1] == ZFFileSeparator())
            {
                if(!fileImpl->callbackPathCreate(outputPath, zftrue, zfnull))
                {
                    success = zffalse;
                    break;
                }
                continue;
            }

            ZFOutput outputRaw;
            if(!ZFOutputForPathInfoT(outputRaw, outputPathInfo.pathType, outputPath))
            {
                success = zffalse;
                break;
            }
            if(!ZFDecompressContentAtIndex(decompressToken, outputRaw, fileIndexInZip))
            {
                success = zffalse;
                break;
            }
        }
    } while(zffalse);
    success &= ZFDecompressEnd(decompressToken);
    return success;
}

ZF_NAMESPACE_GLOBAL_END

