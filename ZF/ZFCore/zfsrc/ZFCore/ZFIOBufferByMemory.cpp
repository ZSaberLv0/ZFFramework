#include "ZFIOBufferByMemory.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassNotPOD _ZFP_ZFIOBufferByMemoryData
{
public:
    zfstring ioBuf;
    zfindex inputIndex;
    zfindex outputIndex;
public:
    _ZFP_ZFIOBufferByMemoryData(void)
    : ioBuf()
    , inputIndex(0)
    , outputIndex(0)
    {
    }
};

zfclass _ZFP_ZFIOBufferByMemory_input : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_ZFIOBufferByMemory_input, ZFObject)
public:
    _ZFP_ZFIOBufferByMemoryData *d;

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

zfclass _ZFP_ZFIOBufferByMemory_output : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_ZFIOBufferByMemory_output, ZFObject)
public:
    _ZFP_ZFIOBufferByMemoryData *d;

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
ZFOBJECT_REGISTER(ZFIOBufferByMemory)

void ZFIOBufferByMemory::objectOnInit(void)
{
    zfsuper::objectOnInit();
    _ZFP_ZFIOBufferByMemoryData *d = zfpoolNew(_ZFP_ZFIOBufferByMemoryData);

    zfblockedAlloc(_ZFP_ZFIOBufferByMemory_input, iOwner);
    iOwner->d = d;
    this->iOwner = iOwner;

    zfblockedAlloc(_ZFP_ZFIOBufferByMemory_output, oOwner);
    oOwner->d = d;
    this->oOwner = oOwner;
}
void ZFIOBufferByMemory::objectOnDealloc(void)
{
    _ZFP_ZFIOBufferByMemoryData *d = this->iOwner->to<_ZFP_ZFIOBufferByMemory_input *>()->d;
    zfpoolDelete(d);
    zfsuper::objectOnDealloc();
}

ZFInput ZFIOBufferByMemory::implInput(void)
{
    return ZFCallbackForMemberMethod(this->iOwner, ZFMethodAccess(_ZFP_ZFIOBufferByMemory_input, onInput));
}
ZFOutput ZFIOBufferByMemory::implOutput(void)
{
    return ZFCallbackForMemberMethod(this->oOwner, ZFMethodAccess(_ZFP_ZFIOBufferByMemory_output, onOutput));
}
void ZFIOBufferByMemory::implRemoveAll(void)
{
    _ZFP_ZFIOBufferByMemoryData *d = this->iOwner->to<_ZFP_ZFIOBufferByMemory_input *>()->d;
    d->ioBuf.removeAll();
    d->inputIndex = 0;
    d->outputIndex = 0;
}

// ============================================================
ZFMETHOD_DEFINE_2(_ZFP_ZFIOBufferByMemory_input, zfindex, onInput,
                  ZFMP_IN(void *, buf),
                  ZFMP_IN(zfindex, count))
{
    if(buf == zfnull)
    {
        return d->ioBuf.length() - d->inputIndex;
    }
    else
    {
        count = zfmMin(count, d->ioBuf.length() - d->inputIndex);
        zfmemcpy(buf, d->ioBuf.cString() + d->inputIndex, count);
        d->inputIndex += count;
        return count;
    }
}
ZFMETHOD_DEFINE_2(_ZFP_ZFIOBufferByMemory_input, zfbool, ioSeek,
                  ZFMP_IN(zfindex, byteSize),
                  ZFMP_IN(ZFSeekPos, pos))
{
    d->inputIndex = ZFIOCallbackCalcFSeek(0, d->ioBuf.length(), d->inputIndex, byteSize, pos);
    return zftrue;
}
ZFMETHOD_DEFINE_0(_ZFP_ZFIOBufferByMemory_input, zfindex, ioTell)
{
    return d->inputIndex;
}
ZFMETHOD_DEFINE_0(_ZFP_ZFIOBufferByMemory_input, zfindex, ioSize)
{
    return d->ioBuf.length();
}

// ============================================================
ZFMETHOD_DEFINE_2(_ZFP_ZFIOBufferByMemory_output, zfindex, onOutput,
                  ZFMP_IN(const void *, buf),
                  ZFMP_IN(zfindex, count))
{
    if(count == zfindexMax())
    {
        count = zfslen((const zfchar *)buf) * sizeof(zfchar);
    }
    d->ioBuf.replace(d->outputIndex, zfmMin(count, d->ioBuf.length() - d->outputIndex), (const zfchar *)buf, count);
    d->outputIndex += count;
    return count;
}

ZFMETHOD_DEFINE_2(_ZFP_ZFIOBufferByMemory_output, zfbool, ioSeek,
                  ZFMP_IN(zfindex, byteSize),
                  ZFMP_IN(ZFSeekPos, pos))
{
    d->outputIndex = ZFIOCallbackCalcFSeek(0, d->ioBuf.length(), d->outputIndex, byteSize, pos);
    return zftrue;
}
ZFMETHOD_DEFINE_0(_ZFP_ZFIOBufferByMemory_output, zfindex, ioTell)
{
    return d->outputIndex;
}
ZFMETHOD_DEFINE_0(_ZFP_ZFIOBufferByMemory_output, zfindex, ioSize)
{
    return d->ioBuf.length();
}

ZF_NAMESPACE_GLOBAL_END

