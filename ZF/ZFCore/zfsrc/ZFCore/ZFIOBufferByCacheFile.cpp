#include "ZFIOBufferByCacheFile.h"
#include "ZFFile.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassNotPOD _ZFP_ZFIOBufferByCacheFileData
{
public:
    void *token;
    zfstring tmpFilePath;
    zfindex outputIndex;
    zfindex inputIndex;
    zfindex fileSize;
public:
    _ZFP_ZFIOBufferByCacheFileData(void)
    : token(zfnull)
    , tmpFilePath()
    , outputIndex(0)
    , inputIndex(0)
    , fileSize(0)
    {
        zfstringAppend(this->tmpFilePath, "%s%cZFIOBufferByCacheFile_%s",
            ZFFilePathForCache(),
            ZFFileSeparator(),
            zfsFromInt(zfidentityCalcPointer(this)).cString());
        this->token = ZFFileFileOpen(this->tmpFilePath.cString(),
            ZFFileOpenOption::e_Create | ZFFileOpenOption::e_Read | ZFFileOpenOption::e_Write);
    }
    ~_ZFP_ZFIOBufferByCacheFileData(void)
    {
        if(this->token != zfnull)
        {
            ZFFileFileClose(this->token);
            this->token = zfnull;
        }
        ZFFileFileRemove(this->tmpFilePath.cString(), zfHint("recursive")zffalse, zfHint("force")zftrue);
    }
};

zfclass _ZFP_ZFIOBufferByCacheFile_input : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_ZFIOBufferByCacheFile_input, ZFObject)
public:
    _ZFP_ZFIOBufferByCacheFileData *d;

public:
    ZFMETHOD_DECLARE_2(zfindex, onInput,
                       ZFMP_IN(void *, buf),
                       ZFMP_IN(zfindex, count))

    ZFMETHOD_DECLARE_2(zfbool, ioSeek,
                       ZFMP_IN(zfindex, byteSize),
                       ZFMP_IN(ZFSeekPos, pos))
    ZFMETHOD_DECLARE_0(zfindex, ioTell)
    ZFMETHOD_DECLARE_0(zfindex, ioSize)
};

zfclass _ZFP_ZFIOBufferByCacheFile_output : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_ZFIOBufferByCacheFile_output, ZFObject)
public:
    _ZFP_ZFIOBufferByCacheFileData *d;

public:
    ZFMETHOD_DECLARE_2(zfindex, onOutput,
                       ZFMP_IN(const void *, buf),
                       ZFMP_IN(zfindex, count))

    ZFMETHOD_DECLARE_2(zfbool, ioSeek,
                       ZFMP_IN(zfindex, byteSize),
                       ZFMP_IN(ZFSeekPos, pos))
    ZFMETHOD_DECLARE_0(zfindex, ioTell)
    ZFMETHOD_DECLARE_0(zfindex, ioSize)
};

// ============================================================
ZFOBJECT_REGISTER(ZFIOBufferByCacheFile)

void ZFIOBufferByCacheFile::objectOnInit(void)
{
    zfsuper::objectOnInit();
    _ZFP_ZFIOBufferByCacheFileData *d = zfpoolNew(_ZFP_ZFIOBufferByCacheFileData);

    zfblockedAlloc(_ZFP_ZFIOBufferByCacheFile_input, iOwner);
    iOwner->d = d;
    this->iOwner = iOwner;

    zfblockedAlloc(_ZFP_ZFIOBufferByCacheFile_output, oOwner);
    oOwner->d = d;
    this->oOwner = oOwner;
}
void ZFIOBufferByCacheFile::objectOnDealloc(void)
{
    _ZFP_ZFIOBufferByCacheFileData *d = this->iOwner->to<_ZFP_ZFIOBufferByCacheFile_input *>()->d;
    zfpoolDelete(d);
    zfsuper::objectOnDealloc();
}

ZFInput ZFIOBufferByCacheFile::implInput(void)
{
    return ZFCallbackForMemberMethod(this->iOwner, ZFMethodAccess(_ZFP_ZFIOBufferByCacheFile_input, onInput));
}
ZFOutput ZFIOBufferByCacheFile::implOutput(void)
{
    return ZFCallbackForMemberMethod(this->oOwner, ZFMethodAccess(_ZFP_ZFIOBufferByCacheFile_output, onOutput));
}
void ZFIOBufferByCacheFile::implRemoveAll(void)
{
    _ZFP_ZFIOBufferByCacheFileData *d = this->iOwner->to<_ZFP_ZFIOBufferByCacheFile_input *>()->d;
    d->inputIndex = 0;
    d->outputIndex = 0;
    d->fileSize = 0;
}

// ============================================================
ZFMETHOD_DEFINE_2(_ZFP_ZFIOBufferByCacheFile_input, zfindex, onInput,
                  ZFMP_IN(void *, buf),
                  ZFMP_IN(zfindex, count))
{
    if(d->token == zfnull)
    {
        return 0;
    }
    if(buf == zfnull)
    {
        return d->fileSize - d->inputIndex;
    }
    else
    {
        ZFFileFileSeek(d->token, d->inputIndex);
        zfindex read = ZFFileFileRead(d->token, buf, zfmMin(count, d->fileSize - d->inputIndex));
        d->inputIndex += read;
        return read;
    }
}
ZFMETHOD_DEFINE_2(_ZFP_ZFIOBufferByCacheFile_input, zfbool, ioSeek,
                  ZFMP_IN(zfindex, byteSize),
                  ZFMP_IN(ZFSeekPos, pos))
{
    if(d->token == zfnull)
    {
        return zffalse;
    }
    d->inputIndex = ZFIOCallbackCalcFSeek(0, d->fileSize, d->inputIndex, byteSize, pos);
    return zftrue;
}
ZFMETHOD_DEFINE_0(_ZFP_ZFIOBufferByCacheFile_input, zfindex, ioTell)
{
    return d->inputIndex;
}
ZFMETHOD_DEFINE_0(_ZFP_ZFIOBufferByCacheFile_input, zfindex, ioSize)
{
    return d->fileSize;
}

// ============================================================
ZFMETHOD_DEFINE_2(_ZFP_ZFIOBufferByCacheFile_output, zfindex, onOutput,
                  ZFMP_IN(const void *, buf),
                  ZFMP_IN(zfindex, count))
{
    if(d->token == zfnull)
    {
        return 0;
    }
    ZFFileFileSeek(d->token, d->outputIndex);
    zfindex written = ZFFileFileWrite(d->token, buf, count);
    d->outputIndex += written;
    if(d->outputIndex > d->fileSize)
    {
        d->fileSize = d->outputIndex;
    }
    return written;
}

ZFMETHOD_DEFINE_2(_ZFP_ZFIOBufferByCacheFile_output, zfbool, ioSeek,
                  ZFMP_IN(zfindex, byteSize),
                  ZFMP_IN(ZFSeekPos, pos))
{
    if(d->token == zfnull)
    {
        return zffalse;
    }
    d->outputIndex = ZFIOCallbackCalcFSeek(0, d->fileSize, d->outputIndex, byteSize, pos);
    return zftrue;
}
ZFMETHOD_DEFINE_0(_ZFP_ZFIOBufferByCacheFile_output, zfindex, ioTell)
{
    return d->outputIndex;
}
ZFMETHOD_DEFINE_0(_ZFP_ZFIOBufferByCacheFile_output, zfindex, ioSize)
{
    return d->fileSize;
}

ZF_NAMESPACE_GLOBAL_END

