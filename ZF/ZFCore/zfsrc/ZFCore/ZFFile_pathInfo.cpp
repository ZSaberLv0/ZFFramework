#include "ZFFile_impl.cpp"

#include "ZFSTLWrapper/zfstl_map.h"
#include "ZFSTLWrapper/zfstl_string.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfbool ZFFilePathInfoCallbackIsExistDefault(ZF_IN const zfchar *pathData)
{
    return zffalse;
}
zfbool ZFFilePathInfoCallbackIsDirDefault(ZF_IN const zfchar *pathData)
{
    return zffalse;
}
zfbool ZFFilePathInfoCallbackToFileNameDefault(ZF_IN const zfchar *pathData,
                                               ZF_OUT zfstring &fileName)
{
    return ZFFileNameOf(fileName, pathData);
}
zfbool ZFFilePathInfoCallbackToChildDefault(ZF_IN const zfchar *pathData,
                                            ZF_IN_OUT zfstring &pathDataChild,
                                            ZF_IN const zfchar *childName)
{
    if(pathData >= pathDataChild.cString() && pathData < pathDataChild.cString() + pathDataChild.length())
    {
        if(pathData == pathDataChild.cString())
        {
            if(!pathDataChild.isEmpty())
            {
                pathDataChild += ZFFileSeparator();
            }
            pathDataChild += childName;
        }
        else
        {
            zfstring t;
            if(!zfsIsEmpty(pathData))
            {
                t += pathData;
                t += ZFFileSeparator();
            }
            t += childName;
            pathDataChild = t;
        }
    }
    else
    {
        if(!zfsIsEmpty(pathData))
        {
            pathDataChild += pathData;
            pathDataChild += ZFFileSeparator();
        }
        pathDataChild += childName;
    }
    ZFFilePathFormatRelative(pathDataChild);
    return zftrue;
}
zfbool ZFFilePathInfoCallbackToParentDefault(ZF_IN const zfchar *pathData,
                                             ZF_IN_OUT zfstring &pathDataParent)
{
    ZFFilePathParentOf(pathDataParent, pathData);
    // always return true
    // typical case: pathData is file at root path
    return zftrue;
}
zfbool ZFFilePathInfoCallbackPathCreateDefault(ZF_IN const zfchar *pathData,
                                               ZF_IN_OPT zfbool autoMakeParent /* = zftrue */,
                                               ZF_OUT_OPT zfstring *errPos /* = zfnull */)
{
    return zffalse;
}
zfbool ZFFilePathInfoCallbackRemoveDefault(ZF_IN const zfchar *pathData,
                                           ZF_IN_OPT zfbool isRecursive /* = zftrue */,
                                           ZF_IN_OPT zfbool isForce /* = zffalse */,
                                           ZF_IN_OPT zfstring *errPos /* = zfnull */)
{
    return zffalse;
}
zfbool ZFFilePathInfoCallbackFindFirstDefault(ZF_IN_OUT ZFFileFindData &fd,
                                              ZF_IN const zfchar *pathData)
{
    return zffalse;
}
zfbool ZFFilePathInfoCallbackFindNextDefault(ZF_IN_OUT ZFFileFindData &fd)
{
    return zffalse;
}
void ZFFilePathInfoCallbackFindCloseDefault(ZF_IN_OUT ZFFileFindData &fd)
{
}
ZFToken ZFFilePathInfoCallbackOpenDefault(ZF_IN const zfchar *pathData,
                                          ZF_IN_OPT ZFFileOpenOptionFlags flag /* = ZFFileOpenOption::e_Read */,
                                          ZF_IN_OPT zfbool autoCreateParent /* = zftrue */)
{
    return ZFTokenInvalid();
}
zfbool ZFFilePathInfoCallbackCloseDefault(ZF_IN ZFToken token)
{
    return zffalse;
}
zfindex ZFFilePathInfoCallbackTellDefault(ZF_IN ZFToken token)
{
    return zfindexMax();
}
zfbool ZFFilePathInfoCallbackSeekDefault(ZF_IN ZFToken token,
                                         ZF_IN zfindex byteSize,
                                         ZF_IN_OPT ZFSeekPos position /* = ZFSeekPosBegin */)
{
    return zffalse;
}
zfindex ZFFilePathInfoCallbackReadDefault(ZF_IN ZFToken token,
                                          ZF_IN void *buf,
                                          ZF_IN zfindex maxByteSize)
{
    return 0;
}
zfindex ZFFilePathInfoCallbackWriteDefault(ZF_IN ZFToken token,
                                           ZF_IN const void *src,
                                           ZF_IN_OPT zfindex maxByteSize /* = zfindexMax() */)
{
    return 0;
}
void ZFFilePathInfoCallbackFlushDefault(ZF_IN ZFToken token)
{
}
zfbool ZFFilePathInfoCallbackIsEofDefault(ZF_IN ZFToken token)
{
    return zftrue;
}
zfbool ZFFilePathInfoCallbackIsErrorDefault(ZF_IN ZFToken token)
{
    return zftrue;
}
zfindex ZFFilePathInfoCallbackSizeDefault(ZF_IN ZFToken token)
{
    return zfindexMax();
}

// ============================================================
static zfstlmap<zfstlstringZ, ZFFilePathInfoData> &_ZFP_ZFFilePathInfoDataMap(void)
{
    static zfstlmap<zfstlstringZ, ZFFilePathInfoData> d;
    return d;
}
static ZFFilePathInfoData *_ZFP_ZFFilePathInfoDataForPathType(ZF_IN const zfchar *pathType)
{
    zfstlmap<zfstlstringZ, ZFFilePathInfoData> &m = _ZFP_ZFFilePathInfoDataMap();
    zfstlmap<zfstlstringZ, ZFFilePathInfoData>::iterator it = m.find(pathType);
    if(it != m.end())
    {
        return &(it->second);
    }
    else
    {
        return zfnull;
    }
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFFilePathInfoIsExist,
                       ZFMP_IN(const ZFPathInfo &, pathInfo))
{
    ZFFilePathInfoData *data = _ZFP_ZFFilePathInfoDataForPathType(pathInfo.pathType);
    if(data == zfnull)
    {
        return ZFFilePathInfoCallbackIsExistDefault(pathInfo.pathData);
    }
    else
    {
        return data->callbackIsExist(pathInfo.pathData);
    }
}
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFFilePathInfoIsDir,
                       ZFMP_IN(const ZFPathInfo &, pathInfo))
{
    ZFFilePathInfoData *data = _ZFP_ZFFilePathInfoDataForPathType(pathInfo.pathType);
    if(data == zfnull)
    {
        return ZFFilePathInfoCallbackIsDirDefault(pathInfo.pathData);
    }
    else
    {
        return data->callbackIsDir(pathInfo.pathData);
    }
}
ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFFilePathInfoToFileName,
                       ZFMP_IN(const ZFPathInfo &, pathInfo),
                       ZFMP_OUT(zfstring &, fileName))
{
    ZFFilePathInfoData *data = _ZFP_ZFFilePathInfoDataForPathType(pathInfo.pathType);
    if(data == zfnull)
    {
        return ZFFilePathInfoCallbackToFileNameDefault(pathInfo.pathData, fileName);
    }
    else
    {
        return data->callbackToFileName(pathInfo.pathData, fileName);
    }
}
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFFilePathInfoToChild,
                       ZFMP_IN(const ZFPathInfo &, pathInfo),
                       ZFMP_IN_OUT(zfstring &, pathDataChild),
                       ZFMP_IN(const zfchar *, childName))
{
    ZFFilePathInfoData *data = _ZFP_ZFFilePathInfoDataForPathType(pathInfo.pathType);
    if(data == zfnull)
    {
        return ZFFilePathInfoCallbackToChildDefault(pathInfo.pathData, pathDataChild, childName);
    }
    else
    {
        return data->callbackToChild(pathInfo.pathData, pathDataChild, childName);
    }
}
ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFFilePathInfoToParent,
                       ZFMP_IN(const ZFPathInfo &, pathInfo),
                       ZFMP_IN_OUT(zfstring &, pathDataParent))
{
    ZFFilePathInfoData *data = _ZFP_ZFFilePathInfoDataForPathType(pathInfo.pathType);
    if(data == zfnull)
    {
        return ZFFilePathInfoCallbackToParentDefault(pathInfo.pathData, pathDataParent);
    }
    else
    {
        return data->callbackToParent(pathInfo.pathData, pathDataParent);
    }
}
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFFilePathInfoPathCreate,
                       ZFMP_IN(const ZFPathInfo &, pathInfo),
                       ZFMP_IN_OPT(zfbool, autoMakeParent, zftrue),
                       ZFMP_OUT_OPT(zfstring *, errPos, zfnull))
{
    ZFFilePathInfoData *data = _ZFP_ZFFilePathInfoDataForPathType(pathInfo.pathType);
    if(data == zfnull)
    {
        return ZFFilePathInfoCallbackPathCreateDefault(pathInfo.pathData, autoMakeParent, errPos);
    }
    else
    {
        return data->callbackPathCreate(pathInfo.pathData, autoMakeParent, errPos);
    }
}
ZFMETHOD_FUNC_DEFINE_4(zfbool, ZFFilePathInfoRemove,
                       ZFMP_IN(const ZFPathInfo &, pathInfo),
                       ZFMP_IN_OPT(zfbool, isRecursive, zftrue),
                       ZFMP_IN_OPT(zfbool, isForce, zffalse),
                       ZFMP_IN_OPT(zfstring *, errPos, zfnull))
{
    ZFFilePathInfoData *data = _ZFP_ZFFilePathInfoDataForPathType(pathInfo.pathType);
    if(data == zfnull)
    {
        return ZFFilePathInfoCallbackRemoveDefault(pathInfo.pathData, isRecursive, isForce, errPos);
    }
    else
    {
        return data->callbackRemove(pathInfo.pathData, isRecursive, isForce, errPos);
    }
}
ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFFilePathInfoFindFirst,
                       ZFMP_IN(const ZFPathInfo &, pathInfo),
                       ZFMP_IN_OUT(ZFFileFindData &, fd))
{
    ZFFilePathInfoData *data = _ZFP_ZFFilePathInfoDataForPathType(pathInfo.pathType);
    if(data == zfnull)
    {
        return ZFFilePathInfoCallbackFindFirstDefault(fd, pathInfo.pathData);
    }
    else
    {
        return data->callbackFindFirst(fd, pathInfo.pathData);
    }
}
ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFFilePathInfoFindNext,
                       ZFMP_IN(const ZFPathInfo &, pathInfo),
                       ZFMP_IN_OUT(ZFFileFindData &, fd))
{
    ZFFilePathInfoData *data = _ZFP_ZFFilePathInfoDataForPathType(pathInfo.pathType);
    if(data == zfnull)
    {
        return ZFFilePathInfoCallbackFindNextDefault(fd);
    }
    else
    {
        return data->callbackFindNext(fd);
    }
}
ZFMETHOD_FUNC_DEFINE_2(void, ZFFilePathInfoFindClose,
                       ZFMP_IN(const ZFPathInfo &, pathInfo),
                       ZFMP_IN_OUT(ZFFileFindData &, fd))
{
    ZFFilePathInfoData *data = _ZFP_ZFFilePathInfoDataForPathType(pathInfo.pathType);
    if(data == zfnull)
    {
        ZFFilePathInfoCallbackFindCloseDefault(fd);
    }
    else
    {
        data->callbackFindClose(fd);
    }
}
ZFMETHOD_FUNC_DEFINE_3(ZFToken, ZFFilePathInfoOpen,
                       ZFMP_IN(const ZFPathInfo &, pathInfo),
                       ZFMP_IN_OPT(ZFFileOpenOptionFlags, flag, ZFFileOpenOption::e_Read),
                       ZFMP_IN_OPT(zfbool, autoCreateParent, zftrue))
{
    ZFFilePathInfoData *data = _ZFP_ZFFilePathInfoDataForPathType(pathInfo.pathType);
    if(data == zfnull)
    {
        return ZFFilePathInfoCallbackOpenDefault(pathInfo.pathData, flag, autoCreateParent);
    }
    else
    {
        return data->callbackOpen(pathInfo.pathData, flag, autoCreateParent);
    }
}
ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFFilePathInfoClose,
                       ZFMP_IN(const ZFPathInfo &, pathInfo),
                       ZFMP_IN(ZFToken, token))
{
    ZFFilePathInfoData *data = _ZFP_ZFFilePathInfoDataForPathType(pathInfo.pathType);
    if(data == zfnull)
    {
        return ZFFilePathInfoCallbackCloseDefault(token);
    }
    else
    {
        return data->callbackClose(token);
    }
}
ZFMETHOD_FUNC_DEFINE_2(zfindex, ZFFilePathInfoTell,
                       ZFMP_IN(const ZFPathInfo &, pathInfo),
                       ZFMP_IN(ZFToken, token))
{
    ZFFilePathInfoData *data = _ZFP_ZFFilePathInfoDataForPathType(pathInfo.pathType);
    if(data == zfnull)
    {
        return ZFFilePathInfoCallbackTellDefault(token);
    }
    else
    {
        return data->callbackTell(token);
    }
}
ZFMETHOD_FUNC_DEFINE_4(zfbool, ZFFilePathInfoSeek,
                       ZFMP_IN(const ZFPathInfo &, pathInfo),
                       ZFMP_IN(ZFToken, token),
                       ZFMP_IN(zfindex, byteSize),
                       ZFMP_IN_OPT(ZFSeekPos, position, ZFSeekPosBegin))
{
    ZFFilePathInfoData *data = _ZFP_ZFFilePathInfoDataForPathType(pathInfo.pathType);
    if(data == zfnull)
    {
        return ZFFilePathInfoCallbackSeekDefault(token, byteSize, position);
    }
    else
    {
        return data->callbackSeek(token, byteSize, position);
    }
}
ZFMETHOD_FUNC_DEFINE_4(zfindex, ZFFilePathInfoRead,
                       ZFMP_IN(const ZFPathInfo &, pathInfo),
                       ZFMP_IN(ZFToken, token),
                       ZFMP_IN(void *, buf),
                       ZFMP_IN(zfindex, maxByteSize))
{
    ZFFilePathInfoData *data = _ZFP_ZFFilePathInfoDataForPathType(pathInfo.pathType);
    if(data == zfnull)
    {
        return ZFFilePathInfoCallbackReadDefault(token, buf, maxByteSize);
    }
    else
    {
        return data->callbackRead(token, buf, maxByteSize);
    }
}
ZFMETHOD_FUNC_DEFINE_4(zfindex, ZFFilePathInfoWrite,
                       ZFMP_IN(const ZFPathInfo &, pathInfo),
                       ZFMP_IN(ZFToken, token),
                       ZFMP_IN(const void *, src),
                       ZFMP_IN_OPT(zfindex, maxByteSize, zfindexMax()))
{
    ZFFilePathInfoData *data = _ZFP_ZFFilePathInfoDataForPathType(pathInfo.pathType);
    if(data == zfnull)
    {
        return ZFFilePathInfoCallbackWriteDefault(token, src, maxByteSize);
    }
    else
    {
        return data->callbackWrite(token, src, maxByteSize);
    }
}
ZFMETHOD_FUNC_DEFINE_2(void, ZFFilePathInfoFlush,
                       ZFMP_IN(const ZFPathInfo &, pathInfo),
                       ZFMP_IN(ZFToken, token))
{
    ZFFilePathInfoData *data = _ZFP_ZFFilePathInfoDataForPathType(pathInfo.pathType);
    if(data == zfnull)
    {
        ZFFilePathInfoCallbackFlushDefault(token);
    }
    else
    {
        data->callbackFlush(token);
    }
}
ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFFilePathInfoIsEof,
                       ZFMP_IN(const ZFPathInfo &, pathInfo),
                       ZFMP_IN(ZFToken, token))
{
    ZFFilePathInfoData *data = _ZFP_ZFFilePathInfoDataForPathType(pathInfo.pathType);
    if(data == zfnull)
    {
        return ZFFilePathInfoCallbackIsEofDefault(token);
    }
    else
    {
        return data->callbackIsEof(token);
    }
}
ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFFilePathInfoIsError,
                       ZFMP_IN(const ZFPathInfo &, pathInfo),
                       ZFMP_IN(ZFToken, token))
{
    ZFFilePathInfoData *data = _ZFP_ZFFilePathInfoDataForPathType(pathInfo.pathType);
    if(data == zfnull)
    {
        return ZFFilePathInfoCallbackIsErrorDefault(token);
    }
    else
    {
        return data->callbackIsError(token);
    }
}
ZFMETHOD_FUNC_DEFINE_2(zfindex, ZFFilePathInfoSize,
                       ZFMP_IN(const ZFPathInfo &, pathInfo),
                       ZFMP_IN(ZFToken, token))
{
    ZFFilePathInfoData *data = _ZFP_ZFFilePathInfoDataForPathType(pathInfo.pathType);
    if(data == zfnull)
    {
        return ZFFilePathInfoCallbackSizeDefault(token);
    }
    else
    {
        return data->callbackSize(token);
    }
}

// ============================================================
void _ZFP_ZFFilePathInfoRegister(ZF_IN const zfchar *pathType,
                                 ZF_IN const ZFFilePathInfoData &data)
{
    zfstlmap<zfstlstringZ, ZFFilePathInfoData> &m = _ZFP_ZFFilePathInfoDataMap();
    zfCoreAssertWithMessage(m.find(pathType) == m.end(),
        "pathType \"%s\" already registered",
        pathType);
    zfCoreAssert(zftrue
            && data.callbackIsExist != zfnull
            && data.callbackIsDir != zfnull
            && data.callbackToFileName != zfnull
            && data.callbackToChild != zfnull
            && data.callbackToParent != zfnull
            && data.callbackPathCreate != zfnull
            && data.callbackRemove != zfnull
            && data.callbackFindFirst != zfnull
            && data.callbackFindNext != zfnull
            && data.callbackFindClose != zfnull
            && data.callbackOpen != zfnull
            && data.callbackClose != zfnull
            && data.callbackTell != zfnull
            && data.callbackSeek != zfnull
            && data.callbackRead != zfnull
            && data.callbackWrite != zfnull
            && data.callbackFlush != zfnull
            && data.callbackIsEof != zfnull
            && data.callbackIsError != zfnull
            && data.callbackSize != zfnull
        );
    m[pathType] = data;
}
void _ZFP_ZFFilePathInfoUnregister(ZF_IN const zfchar *pathType)
{
    zfstlmap<zfstlstringZ, ZFFilePathInfoData> &m = _ZFP_ZFFilePathInfoDataMap();
    m.erase(pathType);
}
const ZFFilePathInfoData *ZFFilePathInfoDataForPathType(ZF_IN const zfchar *pathType)
{
    zfstlmap<zfstlstringZ, ZFFilePathInfoData> &m = _ZFP_ZFFilePathInfoDataMap();
    zfstlmap<zfstlstringZ, ZFFilePathInfoData>::iterator it = m.find(pathType);
    if(it == m.end())
    {
        return zfnull;
    }
    else
    {
        return &(it->second);
    }
}
void ZFFilePathInfoDataGetAllT(ZF_IN_OUT ZFCoreArrayPOD<const zfchar *> &ret)
{
    zfstlmap<zfstlstringZ, ZFFilePathInfoData> &m = _ZFP_ZFFilePathInfoDataMap();
    for(zfstlmap<zfstlstringZ, ZFFilePathInfoData>::iterator it = m.begin(); it != m.end(); ++it)
    {
        ret.add(it->first.c_str());
    }
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFPathInfoForLocalFileT,
                       ZFMP_OUT(ZFPathInfo &, ret),
                       ZFMP_IN(const ZFPathInfo &, pathInfo),
                       ZFMP_IN(const zfchar *, childPath))
{
    const ZFFilePathInfoData *impl = ZFFilePathInfoDataForPathType(pathInfo.pathType);
    if(impl == zfnull)
    {
        return zffalse;
    }
    ret.pathType = pathInfo.pathType;
    if(!impl->callbackIsDir(pathInfo.pathData))
    {
        zfstring pathData;
        return impl->callbackToParent(pathInfo.pathData, pathData)
            && impl->callbackToChild(pathData, ret.pathData, childPath);
    }
    else
    {
        return impl->callbackToChild(pathInfo.pathData, ret.pathData, childPath);
    }
}
ZFMETHOD_FUNC_DEFINE_2(ZFPathInfo, ZFPathInfoForLocalFile,
                       ZFMP_IN(const ZFPathInfo &, pathInfo),
                       ZFMP_IN(const zfchar *, childPath))
{
    ZFPathInfo ret;
    ZFPathInfoForLocalFileT(ret, pathInfo, childPath);
    return ret;
}

// ============================================================
// ZFInputForPathInfo
zfclass _ZFP_I_ZFInputForPathInfoOwner : zfextends ZFObject
{
    ZFOBJECT_DECLARE_WITH_CUSTOM_CTOR(_ZFP_I_ZFInputForPathInfoOwner, ZFObject)

    ZFALLOC_CACHE_RELEASE({
        cache->_cleanup();
    })
private:
    void _cleanup(void)
    {
        if(this->token != ZFTokenInvalid())
        {
            this->impl->callbackClose(this->token);
            this->impl = zfnull;
            this->token = ZFTokenInvalid();
        }
    }

protected:
    zfoverride
    virtual void objectOnDealloc(void)
    {
        this->_cleanup();
        zfsuper::objectOnDealloc();
    }

public:
    virtual zfbool openFile(ZF_IN const zfchar *pathType,
                            ZF_IN const zfchar *pathData,
                            ZF_IN ZFFileOpenOptionFlags flags)
    {
        this->impl = ZFFilePathInfoDataForPathType(pathType);
        if(this->impl == zfnull)
        {
            return zffalse;
        }

        this->token = this->impl->callbackOpen(pathData, flags, zftrue);
        return (this->token != ZFTokenInvalid());
    }

    ZFMETHOD_DECLARE_2(zfindex, onInput,
                       ZFMP_IN(void *, buf),
                       ZFMP_IN(zfindex, count))
    ZFMETHOD_DECLARE_2(zfbool, ioSeek,
                       ZFMP_IN(zfindex, byteSize),
                       ZFMP_IN(ZFSeekPos, pos))
    ZFMETHOD_DECLARE_0(zfindex, ioTell)
    ZFMETHOD_DECLARE_0(zfindex, ioSize)

private:
    const ZFFilePathInfoData *impl;
    ZFToken token;
protected:
    _ZFP_I_ZFInputForPathInfoOwner(void)
    : impl(zfnull)
    , token(ZFTokenInvalid())
    {
    }
};
ZFMETHOD_DEFINE_2(_ZFP_I_ZFInputForPathInfoOwner, zfindex, onInput,
                  ZFMP_IN(void *, buf),
                  ZFMP_IN(zfindex, count))
{
    return this->impl->callbackRead(this->token, buf, count);
}
ZFMETHOD_DEFINE_2(_ZFP_I_ZFInputForPathInfoOwner, zfbool, ioSeek,
                  ZFMP_IN(zfindex, byteSize),
                  ZFMP_IN(ZFSeekPos, pos))
{
    return this->impl->callbackSeek(this->token, byteSize, pos);
}
ZFMETHOD_DEFINE_0(_ZFP_I_ZFInputForPathInfoOwner, zfindex, ioTell)
{
    return this->impl->callbackTell(this->token);
}
ZFMETHOD_DEFINE_0(_ZFP_I_ZFInputForPathInfoOwner, zfindex, ioSize)
{
    return this->impl->callbackSize(this->token) - this->impl->callbackTell(this->token);
}
ZFMETHOD_FUNC_DEFINE_2(ZFInput, ZFInputForPathInfo,
                       ZFMP_IN(const ZFPathInfo &, pathInfo),
                       ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, ZFFileOpenOption::e_Read))
{
    ZFInput ret;
    ZFInputForPathInfoT(ret, pathInfo.pathType, pathInfo.pathData, flags);
    return ret;
}
ZFMETHOD_FUNC_DEFINE_2(ZFInput, ZFInputForPathInfoString,
                       ZFMP_IN(const zfchar *, pathInfoString),
                       ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, ZFFileOpenOption::e_Read))
{
    ZFInput ret;
    ZFInputForPathInfoStringT(ret, pathInfoString, flags);
    return ret;
}
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFInputForPathInfoStringT,
                       ZFMP_IN_OUT(ZFCallback &, ret),
                       ZFMP_IN(const zfchar *, pathInfoString),
                       ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, ZFFileOpenOption::e_Read))
{
    zfstring pathType;
    const zfchar *pathData = zfnull;
    if(!ZFPathInfoParse(pathInfoString, pathType, pathData))
    {
        return zffalse;
    }
    else
    {
        return ZFInputForPathInfoT(ret, pathType, pathData, flags);
    }
}
ZFMETHOD_FUNC_DEFINE_4(zfbool, ZFInputForPathInfoT,
                       ZFMP_IN_OUT(ZFCallback &, ret),
                       ZFMP_IN(const zfchar *, pathType),
                       ZFMP_IN(const zfchar *, pathData),
                       ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, ZFFileOpenOption::e_Read))
{
    zfblockedAllocWithCache(_ZFP_I_ZFInputForPathInfoOwner, inputOwner);
    if(!inputOwner->openFile(pathType, pathData, flags))
    {
        return zffalse;
    }
    ret = ZFCallbackForMemberMethod(
        inputOwner, ZFMethodAccess(_ZFP_I_ZFInputForPathInfoOwner, onInput));
    ret.callbackTag(ZFCallbackTagKeyword_ioOwner, inputOwner);

    ret.pathInfo(pathType, pathData);

    zfstring callbackId;
    ZFPathInfoToString(callbackId, *ret.pathInfo());
    ret.callbackId(callbackId);

    if(!ret.callbackSerializeCustomDisabled())
    {
        zfbool success = zffalse;
        ZFSerializableData customData;
        customData.itemClass(ZFSerializableKeyword_node);
        do
        {
            ZFSerializableData pathInfoData;
            if(!ZFPathInfoToData(pathInfoData, *ret.pathInfo()))
            {
                break;
            }
            pathInfoData.category(ZFSerializableKeyword_ZFFileCallback_pathInfo);
            customData.elementAdd(pathInfoData);

            if(flags != ZFFileOpenOption::e_Read)
            {
                ZFSerializableData flagsData;
                if(!ZFFileOpenOptionFlagsToData(flagsData, flags))
                {
                    break;
                }
                flagsData.category(ZFSerializableKeyword_ZFFileCallback_flags);
                customData.elementAdd(flagsData);
            }

            success = zftrue;
        } while(zffalse);
        if(success)
        {
            ret.callbackSerializeCustomType(ZFCallbackSerializeCustomType_ZFInputForPathInfo);
            ret.callbackSerializeCustomData(customData);
        }
        else
        {
            ret = ZFCallbackNull();
        }
    }

    return ret.callbackIsValid();
}
ZFCALLBACK_SERIALIZE_CUSTOM_TYPE_DEFINE(ZFInputForPathInfo, ZFCallbackSerializeCustomType_ZFInputForPathInfo)
{
    const ZFSerializableData *pathInfoData = ZFSerializableUtil::requireElementByCategory(
        serializableData, ZFSerializableKeyword_ZFFileCallback_pathInfo, outErrorHint, outErrorPos);
    if(pathInfoData == zfnull)
    {
        return zffalse;
    }
    ZFPathInfo pathInfo;
    if(!ZFPathInfoFromData(pathInfo, *pathInfoData, outErrorHint, outErrorPos))
    {
        return zffalse;
    }

    ZFFileOpenOptionFlags flags = ZFFileOpenOption::e_Read;
    const ZFSerializableData *flagsData = ZFSerializableUtil::checkElementByCategory(
        serializableData, ZFSerializableKeyword_ZFFileCallback_flags);
    if(flagsData != zfnull && !ZFFileOpenOptionFlagsFromData(flags, *flagsData, outErrorHint, outErrorPos))
    {
        return zffalse;
    }

    ret.callbackSerializeCustomDisable(zftrue);
    ZFInputForPathInfoT(ret, pathInfo.pathType, pathInfo.pathData, flags);
    if(!ret.callbackIsValid())
    {
        ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
            "failed to open file: %s", ZFPathInfoToString(pathInfo).cString());
        return zffalse;
    }
    serializableData.resolveMark();
    return zftrue;
}

// ============================================================
// ZFOutputForPathInfo
zfclass _ZFP_I_ZFOutputForPathInfoOwner : zfextends ZFObject
{
    ZFOBJECT_DECLARE_WITH_CUSTOM_CTOR(_ZFP_I_ZFOutputForPathInfoOwner, ZFObject)

    ZFALLOC_CACHE_RELEASE({
        cache->_cleanup();
    })
private:
    void _cleanup(void)
    {
        if(this->token != ZFTokenInvalid())
        {
            this->impl->callbackClose(this->token);
            this->impl = zfnull;
            this->token = ZFTokenInvalid();
        }
    }

protected:
    zfoverride
    virtual void objectOnDealloc(void)
    {
        this->_cleanup();
        zfsuper::objectOnDealloc();
    }

public:
    virtual zfbool openFile(ZF_IN const zfchar *pathType,
                            ZF_IN const zfchar *pathData,
                            ZF_IN ZFFileOpenOptionFlags flags)
    {
        this->impl = ZFFilePathInfoDataForPathType(pathType);
        if(this->impl == zfnull)
        {
            return zffalse;
        }

        this->token = this->impl->callbackOpen(pathData, flags, zftrue);
        return (this->token != ZFTokenInvalid());
    }

    ZFMETHOD_DECLARE_2(zfindex, onOutput,
                       ZFMP_IN(const void *, s),
                       ZFMP_IN(zfindex, count))
    ZFMETHOD_DECLARE_2(zfbool, ioSeek,
                       ZFMP_IN(zfindex, byteSize),
                       ZFMP_IN(ZFSeekPos, pos))
    ZFMETHOD_DECLARE_0(zfindex, ioTell)
    ZFMETHOD_DECLARE_0(zfindex, ioSize)

private:
    const ZFFilePathInfoData *impl;
    ZFToken token;
protected:
    _ZFP_I_ZFOutputForPathInfoOwner(void)
    : impl(zfnull)
    , token(ZFTokenInvalid())
    {
    }
};
ZFMETHOD_DEFINE_2(_ZFP_I_ZFOutputForPathInfoOwner, zfindex, onOutput,
                  ZFMP_IN(const void *, s),
                  ZFMP_IN(zfindex, count))
{
    return this->impl->callbackWrite(this->token, s, count);
}
ZFMETHOD_DEFINE_2(_ZFP_I_ZFOutputForPathInfoOwner, zfbool, ioSeek,
                  ZFMP_IN(zfindex, byteSize),
                  ZFMP_IN(ZFSeekPos, pos))
{
    return this->impl->callbackSeek(this->token, byteSize, pos);
}
ZFMETHOD_DEFINE_0(_ZFP_I_ZFOutputForPathInfoOwner, zfindex, ioTell)
{
    return this->impl->callbackTell(this->token);
}
ZFMETHOD_DEFINE_0(_ZFP_I_ZFOutputForPathInfoOwner, zfindex, ioSize)
{
    return this->impl->callbackSize(this->token) - this->impl->callbackTell(this->token);
}
ZFMETHOD_FUNC_DEFINE_2(ZFOutput, ZFOutputForPathInfo,
                       ZFMP_IN(const ZFPathInfo &, pathInfo),
                       ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, ZFFileOpenOption::e_Create))
{
    ZFOutput ret;
    ZFOutputForPathInfoT(ret, pathInfo.pathType, pathInfo.pathData, flags);
    return ret;
}
ZFMETHOD_FUNC_DEFINE_2(ZFOutput, ZFOutputForPathInfoString,
                       ZFMP_IN(const zfchar *, pathInfoString),
                       ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, ZFFileOpenOption::e_Create))
{
    ZFOutput ret;
    ZFOutputForPathInfoStringT(ret, pathInfoString, flags);
    return ret;
}
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFOutputForPathInfoStringT,
                       ZFMP_IN_OUT(ZFCallback &, ret),
                       ZFMP_IN(const zfchar *, pathInfoString),
                       ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, ZFFileOpenOption::e_Create))
{
    zfstring pathType;
    const zfchar *pathData = zfnull;
    if(!ZFPathInfoParse(pathInfoString, pathType, pathData))
    {
        return zffalse;
    }
    else
    {
        return ZFOutputForPathInfoT(ret, pathType, pathData, flags);
    }
}
ZFMETHOD_FUNC_DEFINE_4(zfbool, ZFOutputForPathInfoT,
                       ZFMP_IN_OUT(ZFCallback &, ret),
                       ZFMP_IN(const zfchar *, pathType),
                       ZFMP_IN(const zfchar *, pathData),
                       ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, ZFFileOpenOption::e_Create))
{
    zfblockedAllocWithCache(_ZFP_I_ZFOutputForPathInfoOwner, outputOwner);
    if(!outputOwner->openFile(pathType, pathData, flags))
    {
        return zffalse;
    }
    ret = ZFCallbackForMemberMethod(
        outputOwner, ZFMethodAccess(_ZFP_I_ZFOutputForPathInfoOwner, onOutput));
    ret.callbackTag(ZFCallbackTagKeyword_ioOwner, outputOwner);

    ret.pathInfo(pathType, pathData);

    zfstring callbackId;
    ZFPathInfoToString(callbackId, *ret.pathInfo());
    ret.callbackId(callbackId);

    if(!ret.callbackSerializeCustomDisabled())
    {
        zfbool success = zffalse;
        ZFSerializableData customData;
        customData.itemClass(ZFSerializableKeyword_node);
        do
        {
            ZFSerializableData pathInfoData;
            if(!ZFPathInfoToData(pathInfoData, *ret.pathInfo()))
            {
                break;
            }
            pathInfoData.category(ZFSerializableKeyword_ZFFileCallback_pathInfo);
            customData.elementAdd(pathInfoData);

            if(flags != ZFFileOpenOption::e_Create)
            {
                ZFSerializableData flagsData;
                if(!ZFFileOpenOptionFlagsToData(flagsData, flags))
                {
                    break;
                }
                flagsData.category(ZFSerializableKeyword_ZFFileCallback_flags);
                customData.elementAdd(flagsData);
            }

            success = zftrue;
        } while(zffalse);
        if(success)
        {
            ret.callbackSerializeCustomType(ZFCallbackSerializeCustomType_ZFOutputForPathInfo);
            ret.callbackSerializeCustomData(customData);
        }
        else
        {
            ret = ZFCallbackNull();
        }
    }

    return ret.callbackIsValid();
}
ZFCALLBACK_SERIALIZE_CUSTOM_TYPE_DEFINE(ZFOutputForPathInfo, ZFCallbackSerializeCustomType_ZFOutputForPathInfo)
{
    const ZFSerializableData *pathInfoData = ZFSerializableUtil::requireElementByCategory(
        serializableData, ZFSerializableKeyword_ZFFileCallback_pathInfo, outErrorHint, outErrorPos);
    if(pathInfoData == zfnull)
    {
        return zffalse;
    }
    ZFPathInfo pathInfo;
    if(!ZFPathInfoFromData(pathInfo, *pathInfoData, outErrorHint, outErrorPos))
    {
        return zffalse;
    }

    ZFFileOpenOptionFlags flags = ZFFileOpenOption::e_Create;
    const ZFSerializableData *flagsData = ZFSerializableUtil::checkElementByCategory(
        serializableData, ZFSerializableKeyword_ZFFileCallback_flags);
    if(flagsData != zfnull && !ZFFileOpenOptionFlagsFromData(flags, *flagsData, outErrorHint, outErrorPos))
    {
        return zffalse;
    }

    ret.callbackSerializeCustomDisable(zftrue);
    ZFOutputForPathInfoT(ret, pathInfo.pathType, pathInfo.pathData, flags);
    if(!ret.callbackIsValid())
    {
        ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
            "failed to open file: %s", ZFPathInfoToString(pathInfo).cString());
        return zffalse;
    }
    serializableData.resolveMark();
    return zftrue;
}

// ============================================================
static zfbool _ZFP_ZFFileCallbackForLocalFileGetAbsPath(ZF_OUT zfstring &pathDataAbs,
                                                        ZF_IN const ZFPathInfo &pathInfo,
                                                        ZF_IN const zfchar *localPath)
{
    const ZFFilePathInfoData *impl = ZFFilePathInfoDataForPathType(pathInfo.pathType);
    if(impl == zfnull)
    {
        return zffalse;
    }
    if(!impl->callbackIsDir(pathInfo.pathData))
    {
        zfstring pathDataParent;
        if(!impl->callbackToParent(pathInfo.pathData, pathDataParent))
        {
            return zffalse;
        }
        return impl->callbackToChild(pathDataParent, pathDataAbs, localPath);
    }
    else
    {
        return impl->callbackToChild(pathInfo.pathData, pathDataAbs, localPath);
    }
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_3(ZFInput, ZFInputForLocalFile,
                       ZFMP_IN(const ZFPathInfo &, pathInfo),
                       ZFMP_IN(const zfchar *, localPath),
                       ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, ZFFileOpenOption::e_Read))
{
    ZFInput ret;
    ZFInputForLocalFileT(ret, pathInfo, localPath, flags);
    return ret;
}
ZFMETHOD_FUNC_DEFINE_4(zfbool, ZFInputForLocalFileT,
                       ZFMP_OUT(ZFCallback &, ret),
                       ZFMP_IN(const ZFPathInfo &, pathInfo),
                       ZFMP_IN(const zfchar *, localPath),
                       ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, ZFFileOpenOption::e_Read))
{
    if(zfsIsEmpty(localPath))
    {
        return zffalse;
    }

    zfstring pathDataAbs;
    if(!_ZFP_ZFFileCallbackForLocalFileGetAbsPath(pathDataAbs, pathInfo, localPath))
    {
        return zffalse;
    }
    ret.callbackSerializeCustomDisable(zftrue);
    if(!ZFInputForPathInfoT(ret, pathInfo.pathType, pathDataAbs, flags))
    {
        return zffalse;
    }

    {
        zfbool success = zffalse;
        ZFSerializableData customData;
        customData.itemClass(ZFSerializableKeyword_node);
        do
        {
            ZFSerializableData filePathData;
            if(!zfstringToData(filePathData, localPath))
            {
                return zffalse;
            }
            filePathData.category(ZFSerializableKeyword_ZFFileCallback_filePath);
            customData.elementAdd(filePathData);

            if(flags != ZFFileOpenOption::e_Read)
            {
                ZFSerializableData flagsData;
                if(!ZFFileOpenOptionFlagsToData(flagsData, flags))
                {
                    break;
                }
                flagsData.category(ZFSerializableKeyword_ZFFileCallback_flags);
                customData.elementAdd(flagsData);
            }

            success = zftrue;
        } while(zffalse);
        if(success)
        {
            ret.callbackSerializeCustomType(ZFCallbackSerializeCustomType_ZFInputForLocalFile);
            ret.callbackSerializeCustomData(customData);
        }
        else
        {
            ret = ZFCallbackNull();
        }
    }

    return zftrue;
}
ZFCALLBACK_SERIALIZE_CUSTOM_TYPE_DEFINE(ZFInputForLocalFile, ZFCallbackSerializeCustomType_ZFInputForLocalFile)
{
    const ZFPathInfo *pathInfo = serializableData.pathInfoCheck();
    if(pathInfo == zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
            "serializableData does not contain path info");
        return zffalse;
    }

    const ZFSerializableData *filePathData = ZFSerializableUtil::requireElementByCategory(
        serializableData, ZFSerializableKeyword_ZFFileCallback_filePath, outErrorHint, outErrorPos);
    if(filePathData == zfnull)
    {
        return zffalse;
    }
    const zfchar *localPath = zfnull;
    if(!zfstringFromData(localPath, *filePathData, outErrorHint, outErrorPos))
    {
        return zffalse;
    }

    ZFFileOpenOptionFlags flags = ZFFileOpenOption::e_Read;
    const ZFSerializableData *flagsData = ZFSerializableUtil::checkElementByCategory(
        serializableData, ZFSerializableKeyword_ZFFileCallback_flags);
    if(flagsData != zfnull && !ZFFileOpenOptionFlagsFromData(flags, *flagsData, outErrorHint, outErrorPos))
    {
        return zffalse;
    }

    ret.callbackSerializeCustomDisable(zftrue);
    zfstring pathDataAbs;
    if(!_ZFP_ZFFileCallbackForLocalFileGetAbsPath(pathDataAbs, *pathInfo, localPath))
    {
        ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
            "failed to get file path: %s, localPath: %s",
            ZFPathInfoToString(*pathInfo).cString(),
            localPath);
        return zffalse;
    }
    ZFInputForPathInfoT(ret, pathInfo->pathType, pathDataAbs, flags);
    if(!ret.callbackIsValid())
    {
        ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
            "failed to open file: %s",
            ZFPathInfoToString(ZFPathInfo(pathInfo->pathType, pathDataAbs)).cString());
        return zffalse;
    }
    serializableData.resolveMark();
    return zftrue;
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_3(ZFOutput, ZFOutputForLocalFile,
                       ZFMP_IN(const ZFPathInfo &, pathInfo),
                       ZFMP_IN(const zfchar *, localPath),
                       ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, ZFFileOpenOption::e_Create))
{
    if(zfsIsEmpty(localPath))
    {
        return ZFCallbackNull();
    }

    zfstring pathDataAbs;
    if(!_ZFP_ZFFileCallbackForLocalFileGetAbsPath(pathDataAbs, pathInfo, localPath))
    {
        return ZFCallbackNull();
    }
    ZFOutput ret;
    ret.callbackSerializeCustomDisable(zftrue);
    if(!ZFOutputForPathInfoT(ret, pathInfo.pathType, pathDataAbs, flags))
    {
        return ZFCallbackNull();
    }

    {
        zfbool success = zffalse;
        ZFSerializableData customData;
        customData.itemClass(ZFSerializableKeyword_node);
        do
        {
            ZFSerializableData filePathData;
            if(!zfstringToData(filePathData, localPath))
            {
                return ZFCallbackNull();
            }
            filePathData.category(ZFSerializableKeyword_ZFFileCallback_filePath);
            customData.elementAdd(filePathData);

            if(flags != ZFFileOpenOption::e_Create)
            {
                ZFSerializableData flagsData;
                if(!ZFFileOpenOptionFlagsToData(flagsData, flags))
                {
                    break;
                }
                flagsData.category(ZFSerializableKeyword_ZFFileCallback_flags);
                customData.elementAdd(flagsData);
            }

            success = zftrue;
        } while(zffalse);
        if(success)
        {
            ret.callbackSerializeCustomType(ZFCallbackSerializeCustomType_ZFOutputForLocalFile);
            ret.callbackSerializeCustomData(customData);
        }
    }

    return ret;
}
ZFCALLBACK_SERIALIZE_CUSTOM_TYPE_DEFINE(ZFOutputForLocalFile, ZFCallbackSerializeCustomType_ZFOutputForLocalFile)
{
    const ZFPathInfo *pathInfo = serializableData.pathInfoCheck();
    if(pathInfo == zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
            "serializableData does not contain path info");
        return zffalse;
    }

    const ZFSerializableData *filePathData = ZFSerializableUtil::requireElementByCategory(
        serializableData, ZFSerializableKeyword_ZFFileCallback_filePath, outErrorHint, outErrorPos);
    if(filePathData == zfnull)
    {
        return zffalse;
    }
    const zfchar *localPath = zfnull;
    if(!zfstringFromData(localPath, *filePathData, outErrorHint, outErrorPos))
    {
        return zffalse;
    }

    ZFFileOpenOptionFlags flags = ZFFileOpenOption::e_Create;
    const ZFSerializableData *flagsData = ZFSerializableUtil::checkElementByCategory(
        serializableData, ZFSerializableKeyword_ZFFileCallback_flags);
    if(flagsData != zfnull && !ZFFileOpenOptionFlagsFromData(flags, *flagsData, outErrorHint, outErrorPos))
    {
        return zffalse;
    }

    ret.callbackSerializeCustomDisable(zftrue);
    zfstring pathDataAbs;
    if(!_ZFP_ZFFileCallbackForLocalFileGetAbsPath(pathDataAbs, *pathInfo, localPath))
    {
        ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
            "failed to get file path: %s, localPath: %s",
            ZFPathInfoToString(*pathInfo).cString(),
            localPath);
        return zffalse;
    }
    ZFOutputForPathInfoT(ret, pathInfo->pathType, pathDataAbs, flags);
    if(!ret.callbackIsValid())
    {
        ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
            "failed to open file: %s",
            ZFPathInfoToString(ZFPathInfo(pathInfo->pathType, pathDataAbs)).cString());
        return zffalse;
    }
    serializableData.resolveMark();
    return zftrue;
}

// ============================================================
ZFSTYLE_DECODER_DEFINE(ZFStyleDecoder_pathInfo, {
    if(*styleKey != '@')
    {
        return zffalse;
    }
    ZFInput input;
    input.callbackSerializeCustomDisable(zftrue);
    return ZFInputForPathInfoStringT(input, styleKey + 1)
        && ZFObjectIOLoadT(ret, input);
})

ZF_NAMESPACE_GLOBAL_END

