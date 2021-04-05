#include "ZFTextTemplateRun.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFTYPEID_ACCESS_ONLY_DEFINE(ZFTextTemplateRunParam, ZFTextTemplateRunParam)

ZFTextTemplateRunParam &_ZFP_ZFTextTemplateRunParamDefault(void)
{
    static ZFTextTemplateRunParam d;
    return d;
}
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFTextTemplateRunParamCleaner, ZFLevelZFFrameworkEssential)
{
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFTextTemplateRunParamCleaner)
{
    ZFTextTemplateRunParam &d = ZFTextTemplateRunParamDefault();
    d.dirNameFilter = ZFFilterForString();
    d.dirContentFilter = ZFFilterForString();
    d.fileNameFilter = ZFFilterForString();
    d.fileContentFilter = ZFFilterForString();
}
ZF_GLOBAL_INITIALIZER_END(ZFTextTemplateRunParamCleaner)

// ============================================================
static zfbool _ZFP_ZFTextTemplateRun_applyName(ZF_IN_OUT zfstring &path,
                                               ZF_IN const ZFTextTemplateParam &textTemplateParam,
                                               ZF_IN const ZFFilterForString &nameFilter,
                                               ZF_OUT zfstring *outErrorHint);
static zfbool _ZFP_ZFTextTemplateRun_applyDir(ZF_IN_OUT zfstring &path,
                                              ZF_IN const ZFTextTemplateParam &textTemplateParam,
                                              ZF_IN const ZFTextTemplateRunParam &runParam,
                                              ZF_OUT zfstring *outErrorHint);
static zfbool _ZFP_ZFTextTemplateRun_applyFile(ZF_IN_OUT zfstring &path,
                                               ZF_IN const ZFTextTemplateParam &textTemplateParam,
                                               ZF_IN const ZFTextTemplateRunParam &runParam,
                                               ZF_OUT zfstring *outErrorHint);
ZFMETHOD_FUNC_DEFINE_4(zfbool, ZFTextTemplateRun,
                       ZFMP_IN(const zfchar *, path),
                       ZFMP_IN(const ZFTextTemplateParam &, textTemplateParam),
                       ZFMP_IN_OPT(const ZFTextTemplateRunParam &, runParam, ZFTextTemplateRunParam()),
                       ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull))
{
    zfstring pathTmp;
    ZFFilePathFormat(pathTmp, path);
    if(!ZFFileFileIsExist(pathTmp))
    {
        zfstringAppend(outErrorHint, "path not exist: \"%s\"", path);
        return zffalse;
    }

    if(ZFFileFileIsDir(pathTmp))
    {
        return _ZFP_ZFTextTemplateRun_applyDir(pathTmp, textTemplateParam, runParam, outErrorHint);
    }
    else
    {
        return _ZFP_ZFTextTemplateRun_applyFile(pathTmp, textTemplateParam, runParam, outErrorHint);
    }
}

// ============================================================
static zfbool _ZFP_ZFTextTemplateRun_applyName(ZF_IN_OUT zfstring &path,
                                               ZF_IN const ZFTextTemplateParam &textTemplateParam,
                                               ZF_IN const ZFFilterForString &nameFilter,
                                               ZF_OUT zfstring *outErrorHint)
{
    if(!nameFilter.filterCheckActive(path))
    {
        return zftrue;
    }

    zfstring fileName;
    ZFFileNameOf(fileName, path);
    zfstring fileNameNew;
    ZFTextTemplateApply(textTemplateParam, ZFOutputForString(fileNameNew), fileName);
    if(fileName.compare(fileNameNew) == 0)
    {
        return zftrue;
    }
    if(fileNameNew.isEmpty())
    {
        ZFFileFileRemove(path);
        path.removeAll();
        return zftrue;
    }

    zfstring pathNew;
    if(ZFFilePathParentOf(pathNew, path))
    {
        pathNew += ZFFileSeparator();
    }
    pathNew += fileNameNew;

    if(!ZFFileFileMove(path, pathNew))
    {
        zfstringAppend(outErrorHint,
            "failed to move from \"%s\" to \"%s\"",
            path.cString(), pathNew.cString());
        return zffalse;
    }

    path = pathNew;
    return zftrue;
}
static zfbool _ZFP_ZFTextTemplateRun_applyDir(ZF_IN_OUT zfstring &path,
                                              ZF_IN const ZFTextTemplateParam &textTemplateParam,
                                              ZF_IN const ZFTextTemplateRunParam &runParam,
                                              ZF_OUT zfstring *outErrorHint)
{
    if(!_ZFP_ZFTextTemplateRun_applyName(path, textTemplateParam, runParam.dirNameFilter, outErrorHint))
    {
        return zffalse;
    }
    if(path.isEmpty())
    {
        return zftrue;
    }
    if(!runParam.dirContentFilter.filterCheckActive(path))
    {
        return zftrue;
    }

    zfbool ret = zftrue;
    ZFFileFindData fd;
    if(ZFFileFileFindFirst(fd, path))
    {
        do
        {
            zfstring fullPath = path;
            fullPath += ZFFileSeparator();
            fullPath += fd.fileName();
            if(fd.fileIsDir())
            {
                if(!_ZFP_ZFTextTemplateRun_applyDir(fullPath, textTemplateParam, runParam, outErrorHint))
                {
                    ret = zffalse;
                    break;
                }
            }
            else
            {
                if(!_ZFP_ZFTextTemplateRun_applyFile(fullPath, textTemplateParam, runParam, outErrorHint))
                {
                    ret = zffalse;
                    break;
                }
            }
        } while(ZFFileFileFindNext(fd));
        ZFFileFileFindClose(fd);
    }
    return ret;
}
static zfbool _ZFP_ZFTextTemplateRun_applyFile(ZF_IN_OUT zfstring &path,
                                               ZF_IN const ZFTextTemplateParam &textTemplateParam,
                                               ZF_IN const ZFTextTemplateRunParam &runParam,
                                               ZF_OUT zfstring *outErrorHint)
{
    if(!_ZFP_ZFTextTemplateRun_applyName(path, textTemplateParam, runParam.fileNameFilter, outErrorHint))
    {
        return zffalse;
    }
    if(path.isEmpty())
    {
        return zftrue;
    }
    if(!runParam.fileContentFilter.filterCheckActive(path))
    {
        return zftrue;
    }

    // read entire file to buffer
    zfchar *buf = zfnull;
    zfchar *bufEnd = zfnull;
    {
        void *token = ZFFileFileOpen(path, ZFFileOpenOption::e_Read);
        if(token == zfnull)
        {
            zfstringAppend(outErrorHint, "failed to open file %s", path.cString());
            return zffalse;
        }
        ZFFileFileCloseHolder(token);

        zfindex fileSize = ZFFileFileSize(token);
        if(fileSize == 0)
        {
            return zftrue;
        }

        buf = (zfchar *)zfmalloc(fileSize);
        if(buf == zfnull)
        {
            zfstringAppend(outErrorHint, "failed to malloc buffer for size %zi", fileSize);
            return zffalse;
        }

        if(ZFFileFileRead(token, buf, fileSize) != fileSize)
        {
            zfstringAppend(outErrorHint, "failed to read file %s", path.cString());
            zffree(buf);
            return zffalse;
        }
        bufEnd = buf + fileSize;
    }
    zfblockedFree(buf);

    if(ZFTextTemplateApply(textTemplateParam, ZFOutputForFile(path), buf, bufEnd - buf) == zfindexMax())
    {
        zfstringAppend(outErrorHint, "failed to update template for %s", path.cString());
        return zffalse;
    }
    return zftrue;
}

ZF_NAMESPACE_GLOBAL_END

