#include "ZFPathType_lua.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPATHTYPE_DEFINE(lua)

// ============================================================
zfclassNotPOD _ZFP_ZFPathType_lua
{
public:
    zfclassNotPOD _Token
    {
    public:
        zfbyte *buf;
        zfindex bufSize;
        zfindex pos;
    public:
        _Token(void) : buf(zfnull), bufSize(0), pos(0) {}
        ~_Token(void)
        {
            zffree(this->buf);
        }
    };
public:
    static zfbool callbackIsExist(ZF_IN const zfchar *pathData)
    {
        return zffalse;
    }
    static zfbool callbackIsDir(ZF_IN const zfchar *pathData)
    {
        return zffalse;
    }
    static zfbool callbackToFileName(ZF_IN const zfchar *pathData,
                                     ZF_IN_OUT zfstring &fileName)
    {
        return zffalse;
    }
    static zfbool callbackToChild(ZF_IN const zfchar *pathData,
                                  ZF_IN_OUT zfstring &pathDataChild,
                                  ZF_IN const zfchar *childName)
    {
        return zffalse;
    }
    static zfbool callbackToParent(ZF_IN const zfchar *pathData,
                                   ZF_IN_OUT zfstring &pathDataParent)
    {
        return zffalse;
    }
    static zfbool callbackPathCreate(ZF_IN const zfchar *pathData,
                                     ZF_IN_OPT zfbool autoMakeParent,
                                     ZF_OUT_OPT zfstring *errPos)
    {
        return zffalse;
    }
    static zfbool callbackRemove(ZF_IN const zfchar *pathData,
                                 ZF_IN_OPT zfbool isRecursive,
                                 ZF_IN_OPT zfbool isForce,
                                 ZF_IN_OPT zfstring *errPos)
    {
        return zffalse;
    }
    static zfbool callbackFindFirst(ZF_IN_OUT ZFFileFindData &fd,
                                    ZF_IN const zfchar *pathData)
    {
        return zffalse;
    }
    static zfbool callbackFindNext(ZF_IN_OUT ZFFileFindData &fd)
    {
        return zffalse;
    }
    static void callbackFindClose(ZF_IN_OUT ZFFileFindData &fd)
    {
    }
    static void *callbackOpen(ZF_IN const zfchar *pathData,
                              ZF_IN_OPT ZFFileOpenOptionFlags flag,
                              ZF_IN_OPT zfbool autoCreateParent)
    {
        if(flag != ZFFileOpenOption::e_Read)
        {
            return zfnull;
        }
        _Token *d = zfnew(_Token);
        d->bufSize = zfslen(pathData) * sizeof(zfchar);
        d->buf = (zfbyte *)zfmalloc(d->bufSize);
        zfmemcpy(d->buf, pathData, d->bufSize);
        return d;
    }
    static zfbool callbackClose(ZF_IN void *token)
    {
        zfdelete((_Token *)token);
        return zftrue;
    }
    static zfindex callbackTell(ZF_IN void *token)
    {
        _Token *d = (_Token *)token;
        return d->pos;
    }
    static zfbool callbackSeek(ZF_IN void *token,
                               ZF_IN zfindex byteSize,
                               ZF_IN_OPT ZFSeekPos position)
    {
        _Token *d = (_Token *)token;
        d->pos = ZFIOCallbackCalcFSeek(0, d->bufSize, d->pos, byteSize, position);
        return zftrue;
    }
    static zfindex callbackRead(ZF_IN void *token,
                                ZF_IN void *buf,
                                ZF_IN zfindex maxByteSize)
    {
        _Token *d = (_Token *)token;
        if(maxByteSize > d->bufSize - d->pos)
        {
            maxByteSize = d->bufSize - d->pos;
        }
        zfmemcpy(buf, d->buf + d->pos, maxByteSize);
        return maxByteSize;
    }
    static zfindex callbackWrite(ZF_IN void *token,
                                 ZF_IN const void *src,
                                 ZF_IN_OPT zfindex maxByteSize)
    {
        return 0;
    }
    static void callbackFlush(ZF_IN void *token)
    {
    }
    static zfbool callbackIsEof(ZF_IN void *token)
    {
        _Token *d = (_Token *)token;
        return (d->pos >= d->bufSize);
    }
    static zfbool callbackIsError(ZF_IN void *token)
    {
        return zffalse;
    }
    static zfindex callbackSize(ZF_IN void *token)
    {
        _Token *d = (_Token *)token;
        return d->bufSize - d->pos;
    }
};
ZFPATHTYPE_FILEIO_REGISTER(lua, ZFPathType_lua()
        , _ZFP_ZFPathType_lua::callbackIsExist
        , _ZFP_ZFPathType_lua::callbackIsDir
        , _ZFP_ZFPathType_lua::callbackToFileName
        , _ZFP_ZFPathType_lua::callbackToChild
        , _ZFP_ZFPathType_lua::callbackToParent
        , _ZFP_ZFPathType_lua::callbackPathCreate
        , _ZFP_ZFPathType_lua::callbackRemove
        , _ZFP_ZFPathType_lua::callbackFindFirst
        , _ZFP_ZFPathType_lua::callbackFindNext
        , _ZFP_ZFPathType_lua::callbackFindClose
        , _ZFP_ZFPathType_lua::callbackOpen
        , _ZFP_ZFPathType_lua::callbackClose
        , _ZFP_ZFPathType_lua::callbackTell
        , _ZFP_ZFPathType_lua::callbackSeek
        , _ZFP_ZFPathType_lua::callbackRead
        , _ZFP_ZFPathType_lua::callbackWrite
        , _ZFP_ZFPathType_lua::callbackFlush
        , _ZFP_ZFPathType_lua::callbackIsEof
        , _ZFP_ZFPathType_lua::callbackIsError
        , _ZFP_ZFPathType_lua::callbackSize
    )

ZF_NAMESPACE_GLOBAL_END

