#include "ZFOutputForFormat.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFOBJECT_REGISTER(ZFOutputFormat)

void ZFOutputFormat::format(ZF_IN_OUT zfstring &ret,
                            ZF_IN ZFOutputFormatStepEnum outputStep,
                            ZF_IN const zfchar *src,
                            ZF_IN zfindex srcLen,
                            ZF_IN zfindex outputCount,
                            ZF_IN zfindex writtenLen,
                            ZF_IN_OUT_OPT void *&state)
{
    // nothing to do
}

// ============================================================
zfclass _ZFP_I_ZFOutputForFormatOwner : zfextends ZFObject
{
    ZFOBJECT_DECLARE_WITH_CUSTOM_CTOR(_ZFP_I_ZFOutputForFormatOwner, ZFObject)

public:
    ZFOutput output;
    ZFOutputFormat *format;
    ZFOutputFormatStepEnum outputStep;
    zfindex outputCount;
    zfindex writtenLen;
    void *state;

public:
    ZFALLOC_CACHE_RELEASE({
        if(cache->format != zfnull)
        {
            cache->outputEnd();
        }

        cache->output.callbackClear();
        if(cache->format != zfnull)
        {
            zfRelease(cache->format);
            cache->format = zfnull;
        }
        cache->outputStep = ZFOutputFormatStep::e_OnInit;
        cache->outputCount = 0;
        cache->writtenLen = 0;
        cache->state = zfnull;
    })

protected:
    _ZFP_I_ZFOutputForFormatOwner(void)
    : output()
    , format(zfnull)
    , outputStep(ZFOutputFormatStep::e_OnInit)
    , outputCount(0)
    , writtenLen(0)
    , state(zfnull)
    {
    }

protected:
    zfoverride
    virtual void objectOnDealloc(void)
    {
        if(this->format != zfnull)
        {
            this->outputEnd();
            zfRelease(this->format);
            this->format = zfnull;
        }
        zfsuper::objectOnDealloc();
    }

public:
    void outputBegin(void)
    {
        zfstring buf;
        this->format->_ZFP_format(
            buf, ZFOutputFormatStep::e_OnInit, "", 0, this->outputCount, this->writtenLen, this->state);
        if(!buf.isEmpty())
        {
            this->writtenLen += this->output.execute(buf.cString(), buf.length() * sizeof(zfchar));
        }
    }
    void outputEnd(void)
    {
        zfstring buf;
        this->format->_ZFP_format(
            buf, ZFOutputFormatStep::e_OnOutputEnd, "", 0, this->outputCount, this->writtenLen, this->state);
        if(!buf.isEmpty())
        {
            if(this->output.callbackIsValid())
            {
                this->writtenLen += this->output.execute(buf.cString(), buf.length() * sizeof(zfchar));
            }
            buf.removeAll();
        }

        this->format->_ZFP_format(
            buf, ZFOutputFormatStep::e_OnDealloc, "", 0, this->outputCount, this->writtenLen, this->state);
        if(!buf.isEmpty())
        {
            if(this->output.callbackIsValid())
            {
                this->writtenLen += this->output.execute(buf.cString(), buf.length() * sizeof(zfchar));
            }
        }
    }
    ZFMETHOD_INLINE_2(zfindex, onOutput,
                      ZFMP_IN(const void *, s),
                      ZFMP_IN(zfindex, count))
    {
        if(!this->output.callbackIsValid())
        {
            return 0;
        }

        if(count == zfindexMax())
        {
            count = zfslen((const zfchar *)s);
        }
        else
        {
            count /= sizeof(zfchar);
        }

        zfstring buf;
        this->format->_ZFP_format(
            buf,
            ZFOutputFormatStep::e_OnOutput,
            (const zfchar *)s,
            count,
            this->outputCount,
            this->writtenLen,
            this->state);
        ++(this->outputCount);
        if(buf.isEmpty())
        {
            return count * sizeof(zfchar);
        }

        zfindex written = this->output.execute(buf.cString(), buf.length() * sizeof(zfchar));
        this->writtenLen += written;
        if(written == buf.length() * sizeof(zfchar))
        {
            return count * sizeof(zfchar);
        }
        else
        {
            return 0;
        }
    }
};

ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFOutputForFormatT,
                       ZFMP_OUT(ZFCallback &, ret),
                       ZFMP_IN(const ZFOutput &, output),
                       ZFMP_IN(ZFOutputFormat *, format))
{
    if(!output.callbackIsValid())
    {
        return zffalse;
    }
    if(format == zfnull)
    {
        ret = output;
        return zffalse;
    }

    _ZFP_I_ZFOutputForFormatOwner *outputOwner = zfAllocWithCache(_ZFP_I_ZFOutputForFormatOwner);
    outputOwner->output = output;
    outputOwner->format = zfRetain(format);
    ret = ZFCallbackForMemberMethod(outputOwner, ZFMethodAccess(_ZFP_I_ZFOutputForFormatOwner, onOutput));
    ret.callbackOwnerObjectRetain();
    ret.callbackTag(ZFCallbackTagKeyword_ioOwner, output.callbackTag(ZFCallbackTagKeyword_ioOwner));
    zfRelease(outputOwner);

    if(!ret.callbackSerializeCustomDisabled())
    {
        ZFSerializableData outputData;
        ZFSerializableData formatData;
        if(format->classData()->classIsTypeOf(ZFSerializable::ClassData())
            && ZFCallbackToData(outputData, output)
            && ZFObjectToData(formatData, format->toObject()))
        {
            ZFSerializableData serializableData;
            outputData.category(ZFSerializableKeyword_ZFOutputForFormat_output);
            serializableData.elementAdd(outputData);
            formatData.category(ZFSerializableKeyword_ZFOutputForFormat_format);
            serializableData.elementAdd(formatData);
            ret.callbackSerializeCustomType(ZFCallbackSerializeCustomType_ZFOutputForFormat);
            ret.callbackSerializeCustomData(serializableData);
        }
    }

    outputOwner->outputBegin();
    return zftrue;
}

ZFMETHOD_FUNC_DEFINE_2(ZFOutput, ZFOutputForFormat,
                       ZFMP_IN(const ZFOutput &, output),
                       ZFMP_IN(ZFOutputFormat *, format))
{
    ZFOutput ret;
    ZFOutputForFormatT(ret, output, format);
    return ret;
}

ZFCALLBACK_SERIALIZE_CUSTOM_TYPE_DEFINE(ZFOutputForFormat, ZFCallbackSerializeCustomType_ZFOutputForFormat)
{
    const ZFSerializableData *outputData = ZFSerializableUtil::requireElementByCategory(
        serializableData, ZFSerializableKeyword_ZFOutputForFormat_output, outErrorHint, outErrorPos);
    if(outputData == zfnull)
    {
        return zffalse;
    }
    ZFOutput output;
    if(!ZFCallbackFromData(output, *outputData, outErrorHint, outErrorPos))
    {
        return zffalse;
    }

    const ZFSerializableData *formatData = ZFSerializableUtil::requireElementByCategory(
        serializableData, ZFSerializableKeyword_ZFOutputForFormat_format, outErrorHint, outErrorPos);
    if(formatData == zfnull)
    {
        return zffalse;
    }
    zfautoObject formatHolder;
    if(!zfautoObjectFromData(formatHolder, *formatData, outErrorHint, outErrorPos))
    {
        return zffalse;
    }
    ZFOutputFormat *format = formatHolder;
    if(format == zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, *formatData,
            "format object %s not type of %s",
            ZFObjectInfo(formatHolder.toObject()).cString(),
            ZFOutputFormat::ClassData()->classNameFull());
        return zffalse;
    }


    ZFOutput retTmp;
    retTmp.callbackSerializeCustomDisable(zftrue);
    if(!ZFOutputForFormatT(retTmp, output, format))
    {
        ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, *formatData,
            "unable to create from output %s and format %s",
            output.objectInfo().cString(),
            ZFObjectInfo(formatHolder.toObject()).cString());
        return zffalse;
    }
    ret = retTmp;

    serializableData.resolveMark();
    return zftrue;
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_1(ZFOutputFormat *, ZFOutputForFormatGetFormat,
                       ZFMP_IN(const ZFCallback &, callback))
{
    _ZFP_I_ZFOutputForFormatOwner *owner = ZFCastZFObject(_ZFP_I_ZFOutputForFormatOwner *, callback.callbackOwnerObject());
    if(owner == zfnull)
    {
        return zfnull;
    }
    else
    {
        return owner->format;
    }
}

ZFMETHOD_FUNC_DEFINE_1(ZFOutput, ZFOutputForFormatGetOutput,
                       ZFMP_IN(const ZFCallback &, callback))
{
    _ZFP_I_ZFOutputForFormatOwner *owner = ZFCastZFObject(_ZFP_I_ZFOutputForFormatOwner *, callback.callbackOwnerObject());
    if(owner == zfnull)
    {
        return ZFCallbackNull();
    }
    else
    {
        return owner->output;
    }
}

// ============================================================
ZFOBJECT_REGISTER(ZFOutputFormatBasic)
void ZFOutputFormatBasic::format(ZF_IN_OUT zfstring &ret,
                                 ZF_IN ZFOutputFormatStepEnum outputStep,
                                 ZF_IN const zfchar *src,
                                 ZF_IN zfindex srcLen,
                                 ZF_IN zfindex outputCount,
                                 ZF_IN zfindex writtenLen,
                                 ZF_IN_OUT_OPT void *&state)
{
    switch(outputStep)
    {
        case ZFOutputFormatStep::e_OnInit:
            state = (void *)zfnew(zfbool, zftrue);
            return ;
        case ZFOutputFormatStep::e_OnDealloc:
            zfdelete((zfbool *)state);
            return ;
        case ZFOutputFormatStep::e_OnOutput:
            break;
        case ZFOutputFormatStep::e_OnOutputEnd:
            if(outputCount > 0)
            {
                ret += this->outputPostfix();
            }
            return;
        default:
            zfCoreCriticalShouldNotGoHere();
            return ;
    }
    if(outputCount == 0)
    {
        ret += this->outputPrefix();
    }
    zfbool &needLinePrefix = *(zfbool *)state;
    if(needLinePrefix)
    {
        needLinePrefix = zffalse;
        ret += this->linePrefix();
    }

    const zfchar *srcEnd = src + srcLen;
    const zfchar *p = src;
    while(src < srcEnd)
    {
        if(*src == '\n')
        {
            ret.append(p, src - p);
            ret += this->linePostfix();
            ret += '\n';
            needLinePrefix = zftrue;
            zfcharMoveNext(src);
            p = src;
            continue;
        }
        zfcharMoveNext(src);
    }
    if(p < src)
    {
        needLinePrefix = zffalse;
        ret.append(p, src - p);
    }
}

ZF_NAMESPACE_GLOBAL_END

