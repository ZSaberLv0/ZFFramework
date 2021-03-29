#include "ZFIOCallback_input.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFInput
zfindex ZFInputReadChar(ZF_OUT zfchar *p, ZF_IN_OUT const ZFInput &input)
{
    if(input.execute(p, 1) != 1)
    {
        p[0] = '\0';
        return 0;
    }
    if((zfbyte)p[0] < 0x7F)
    {
        p[1] = '\0';
        return 1;
    }

    if(input.execute(p + 1, 1) != 1)
    {
        p[1] = '\0';
        return 1;
    }
    if((zfbyte)p[0] <= 0xDF && (zfbyte)p[1] <= 0xBF)
    {
        p[2] = '\0';
        return 2;
    }

    if(input.execute(p + 2, 1) != 1)
    {
        p[2] = '\0';
        return 2;
    }
    if((zfbyte)p[0] <= 0xEF && (zfbyte)p[1] <= 0xBF && (zfbyte)p[2] <= 0xBF)
    {
        p[3] = '\0';
        return 3;
    }

    if(input.execute(p + 3, 1) != 1)
    {
        p[3] = '\0';
        return 3;
    }
    if((zfbyte)p[0] <= 0xF7 && (zfbyte)p[1] <= 0xBF && (zfbyte)p[2] <= 0xBF && (zfbyte)p[3] <= 0xBF)
    {
        p[4] = '\0';
        return 4;
    }

    if(input.execute(p + 4, 1) != 1)
    {
        p[4] = '\0';
        return 4;
    }
    if((zfbyte)p[0] <= 0xFB && (zfbyte)p[1] <= 0xBF && (zfbyte)p[2] <= 0xBF && (zfbyte)p[3] <= 0xBF && (zfbyte)p[4] <= 0xBF)
    {
        p[5] = '\0';
        return 5;
    }

    if(input.execute(p + 5, 1) != 1)
    {
        p[5] = '\0';
        return 5;
    }
    if((zfbyte)p[0] <= 0xFD && (zfbyte)p[1] <= 0xBF && (zfbyte)p[2] <= 0xBF && (zfbyte)p[3] <= 0xBF && (zfbyte)p[4] <= 0xBF && (zfbyte)p[5] <= 0xBF)
    {
        p[6] = '\0';
        return 6;
    }

    if(input.execute(p + 6, 1) != 1)
    {
        p[6] = '\0';
        return 6;
    }
    if((zfbyte)p[0] <= 0xFE && (zfbyte)p[1] <= 0xBF && (zfbyte)p[2] <= 0xBF && (zfbyte)p[3] <= 0xBF && (zfbyte)p[4] <= 0xBF && (zfbyte)p[5] <= 0xBF && (zfbyte)p[6] <= 0xBF)
    {
        p[7] = '\0';
        return 7;
    }

    if(input.execute(p + 7, 1) != 1)
    {
        p[7] = '\0';
        return 7;
    }

    p[8] = '\0';
    return 8;
}
zfindex ZFInputReadChar(ZF_IN_OUT zfstring &buf, ZF_IN_OUT const ZFInput &input)
{
    zfchar tmp[9] = {0};
    zfindex ret = ZFInputReadChar(tmp, input);
    buf += tmp;
    return ret;
}

void ZFInputReadToString(ZF_IN_OUT zfstring &ret, ZF_IN_OUT const ZFInput &input)
{
    if(!input.callbackIsValid())
    {
        return ;
    }

    zfindex totalSize = input.ioSize();
    if(totalSize != zfindexMax())
    {
        ret.capacity(totalSize);
    }

    #define _ZFP_ZFInputReadToString_blockSize 256
    zfindex readCount = 0;
    zfchar bufTmp[_ZFP_ZFInputReadToString_blockSize];
    do
    {
        readCount = input.execute(bufTmp, _ZFP_ZFInputReadToString_blockSize);
        if(readCount > 0)
        {
            ret.append(bufTmp, readCount);
        }
        if(readCount < _ZFP_ZFInputReadToString_blockSize)
        {
            break;
        }
    } while(zftrue);
    #undef _ZFP_ZFInputReadToString_blockSize
}

ZFBuffer ZFInputReadToBuffer(ZF_IN_OUT const ZFInput &input)
{
    if(!input.callbackIsValid())
    {
        return ZFBuffer();
    }

    ZFBuffer ret;
    zfindex totalSize = input.ioSize();
    if(totalSize != zfindexMax())
    {
        ret.bufferMalloc(totalSize + sizeof(zfchar));
        if(input.execute(ret.buffer(), totalSize) != totalSize)
        {
            ret.bufferFree();
        }
        else
        {
            ret.bufferSize(totalSize);
        }
    }
    else
    {
        #define _ZFP_ZFInputReadToBuffer_blockSize 256
        zfindex readCount = 0;
        zfindex size = 0;
        do
        {
            ret.bufferRealloc(ret.bufferSize() + _ZFP_ZFInputReadToBuffer_blockSize);
            readCount = input.execute(ret.bufferT<zfbyte *>() + size, _ZFP_ZFInputReadToBuffer_blockSize - sizeof(zfchar));
            size += readCount;
            if(readCount < _ZFP_ZFInputReadToBuffer_blockSize - sizeof(zfchar))
            {
                ret.bufferRealloc(size + sizeof(zfchar));
                ret.bufferSize(size);
                break;
            }
        } while(zftrue);
        #undef _ZFP_ZFInputReadToBuffer_blockSize
    }
    return ret;
}

zfbool ZFInputSkipChars(ZF_OUT zfchar *buf,
                        ZF_IN_OUT const ZFInput &input,
                        ZF_IN_OPT const zfchar *charSet /* = " \t\r\n" */)
{
    zfindex charSetCount = zfslen(charSet);
    zfbool matched = zffalse;
    do
    {
        switch(ZFInputReadChar(buf, input))
        {
            case 0:
                return zffalse;
            case 1:
                break;
            default:
                return zftrue;
        }
        matched = zffalse;
        for(zfindex i = 0; i < charSetCount; ++i)
        {
            if(charSet[i] == buf[0])
            {
                matched = zftrue;
                break;
            }
        }
        if(!matched)
        {
            return zftrue;
        }
    } while(zftrue);
    return zffalse;
}
zfindex ZFInputReadUntil(ZF_IN_OUT zfstring &ret,
                         ZF_IN_OUT const ZFInput &input,
                         ZF_IN_OPT const zfchar *charSet /* = " \t\r\n" */,
                         ZF_IN_OPT zfindex maxCount /* = zfindexMax() */,
                         ZF_OUT_OPT zfchar *firstMatchedChar /* = zfnull */)
{
    zfindex readCount = 0;
    if(firstMatchedChar != zfnull)
    {
        *firstMatchedChar = '\0';
    }
    if(input.callbackIsValid())
    {
        zfchar buf[9] = {0};
        zfindex charSetCount = zfslen(charSet);
        zfbool matched = zffalse;
        while(readCount < maxCount)
        {
            zfindex t = ZFInputReadChar(buf, input);
            if(t == 0)
            {
                break;
            }
            else if(t == 1)
            {
                matched = zffalse;
                for(zfindex i = 0; i < charSetCount; ++i)
                {
                    if(charSet[i] == buf[0])
                    {
                        matched = zftrue;
                        break;
                    }
                }
                if(matched)
                {
                    if(firstMatchedChar != zfnull)
                    {
                        *firstMatchedChar = buf[0];
                    }
                    break;
                }
            }

            ret += buf;
            readCount += t;
        }
    }
    return readCount;
}
zfindex ZFInputCheckMatch(ZF_IN const zfchar **tokens,
                          ZF_IN zfindex tokenCount,
                          ZF_IN_OUT const ZFInput &input)
{
    zfindex ret = zfindexMax();
    if(input.callbackIsValid())
    {
        zfindex saved = input.ioTell();
        zfindex maxLen = 0;
        for(zfindex i = 0; i < tokenCount; ++i)
        {
            maxLen = zfmMax(maxLen, zfslen(tokens[i]));
        }
        zfchar *buf = (zfchar *)zfmalloc(sizeof(zfchar) * maxLen);
        zfblockedFree(buf);

        zfbool matched = zffalse;
        zfindex firstCharLen = ZFInputSkipChars(buf, input);
        zfindex matchedLen = 0;
        if(firstCharLen < maxLen)
        {
            input.execute(buf + firstCharLen, maxLen - firstCharLen);
            for(zfindex i = 0; i < tokenCount; ++i)
            {
                matchedLen = zfslen(tokens[i]);
                if(zfsncmp(tokens[i], buf, matchedLen) == 0)
                {
                    matched = zftrue;
                    ret = i;
                    break;
                }
            }
        }

        if(matched)
        {
            input.ioSeek(maxLen - matchedLen, ZFSeekPosCurReversely);
        }
        else
        {
            input.ioSeek(saved, ZFSeekPosBegin);
        }
    }
    return ret;
}

// ============================================================
// ZFInputDummy
static zfindex _ZFP_ZFInputDummy(ZF_OUT void *buf, ZF_IN zfindex count)
{
    return 0;
}
ZFInput ZFInputDummy(void)
{
    ZFInput ret = ZFCallbackForFunc(_ZFP_ZFInputDummy);
    ret.callbackSerializeCustomType(ZFCallbackSerializeCustomType_ZFInputDummy);
    ret.callbackSerializeCustomData(ZFSerializableData());
    return ret;
}

// ============================================================
// ZFInputForInputInRange
zfclass _ZFP_I_ZFInputForInputInRangeOwner : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_I_ZFInputForInputInRangeOwner, ZFObject)
public:
    ZFInput src;
    zfindex srcStart;
    zfindex srcCount;
    zfbool autoRestorePos;
    zfindex savedPos;
    zfindex curPos; // ensured init with start

    ZFALLOC_CACHE_RELEASE({
        cache->_cleanup();
        cache->src.callbackClear();
    })
private:
    void _cleanup(void)
    {
        if(autoRestorePos)
        {
            autoRestorePos = zffalse;
            src.ioSeek(savedPos);
        }
    }

protected:
    zfoverride
    virtual void objectOnDeallocPrepare(void)
    {
        this->_cleanup();
        zfsuper::objectOnDeallocPrepare();
    }

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
ZFMETHOD_DEFINE_2(_ZFP_I_ZFInputForInputInRangeOwner, zfindex, onInput,
                  ZFMP_IN(void *, buf),
                  ZFMP_IN(zfindex, count))
{
    if(buf == zfnull)
    {
        return src.execute(buf, count);
    }
    if(curPos - srcStart + count > srcCount)
    {
        count = srcCount - curPos + srcStart;
    }
    count = src.execute(buf, count);
    curPos += count;
    return count;
}
ZFMETHOD_DEFINE_2(_ZFP_I_ZFInputForInputInRangeOwner, zfbool, ioSeek,
                  ZFMP_IN(zfindex, byteSize),
                  ZFMP_IN(ZFSeekPos, pos))
{
    curPos = ZFIOCallbackCalcFSeek(srcStart, srcCount, curPos, byteSize, pos);
    return zftrue;
}
ZFMETHOD_DEFINE_0(_ZFP_I_ZFInputForInputInRangeOwner, zfindex, ioTell)
{
    return curPos - srcStart;
}
ZFMETHOD_DEFINE_0(_ZFP_I_ZFInputForInputInRangeOwner, zfindex, ioSize)
{
    return srcStart + srcCount - curPos;
}
ZFInput ZFInputForInputInRange(ZF_IN const ZFInput &inputCallback,
                               ZF_IN_OPT zfindex start /* = 0 */,
                               ZF_IN_OPT zfindex count /* = zfindexMax() */,
                               ZF_IN_OPT zfbool autoRestorePos /* = zftrue */)
{
    zfbool valid = zffalse;
    zfindex savedPos = zfindexMax();
    zfindex countFixed = count;
    do
    {
        if(!inputCallback.callbackIsValid()) {break;}

        savedPos = inputCallback.ioTell();
        if(savedPos == zfindexMax()) {break;}

        if(!inputCallback.ioSeek(start, ZFSeekPosBegin)) {break;}

        zfindex srcCount = inputCallback.ioSize();
        if(srcCount == zfindexMax()) {break;}
        if(start + countFixed > srcCount)
        {
            countFixed = srcCount - start;
        }

        valid = zftrue;
    } while(zffalse);
    if(!valid)
    {
        if(savedPos != zfindexMax())
        {
            inputCallback.ioSeek(savedPos, ZFSeekPosBegin);
        }
        return ZFCallbackNull();
    }

    _ZFP_I_ZFInputForInputInRangeOwner *owner = zfAllocWithCache(_ZFP_I_ZFInputForInputInRangeOwner);
    owner->src = inputCallback;
    owner->srcStart = start;
    owner->srcCount = countFixed;
    owner->autoRestorePos = autoRestorePos;
    owner->savedPos = savedPos;
    owner->curPos = start;
    ZFInput ret = ZFCallbackForMemberMethod(
        owner, ZFMethodAccess(_ZFP_I_ZFInputForInputInRangeOwner, onInput));
    ret.callbackTag(ZFCallbackTagKeyword_ioOwner, owner);
    zfRelease(owner);

    if(inputCallback.callbackId() != zfnull)
    {
        ret.callbackId(zfstringWithFormat("ZFInputForInputInRange[%zi, %zi]:%@", start, count, inputCallback.callbackId()));
    }

    if(!inputCallback.callbackSerializeCustomDisabled())
    {
        ZFSerializableData inputData;
        if(ZFCallbackToData(inputData, inputCallback))
        {
            ZFSerializableData customData;
            customData.itemClass(ZFSerializableKeyword_node);

            zfbool success = zffalse;
            do {
                inputData.category(ZFSerializableKeyword_ZFInputForInputInRange_input);
                customData.elementAdd(inputData);

                if(start != 0)
                {
                    ZFSerializableData startData;
                    if(!zfindexToData(startData, start))
                    {
                        break;
                    }
                    startData.category(ZFSerializableKeyword_ZFInputForInputInRange_start);
                    customData.elementAdd(startData);
                }

                if(count != zfindexMax())
                {
                    ZFSerializableData countData;
                    if(!zfindexToData(countData, count))
                    {
                        break;
                    }
                    countData.category(ZFSerializableKeyword_ZFInputForInputInRange_count);
                    customData.elementAdd(countData);
                }

                if(!autoRestorePos)
                {
                    ZFSerializableData autoRestorePosData;
                    if(!zfboolToData(autoRestorePosData, autoRestorePos))
                    {
                        break;
                    }
                    autoRestorePosData.category(ZFSerializableKeyword_ZFInputForInputInRange_autoRestorePos);
                    customData.elementAdd(autoRestorePosData);
                }

                success = zftrue;
            } while(zffalse);

            if(success)
            {
                ret.callbackSerializeCustomType(ZFCallbackSerializeCustomType_ZFInputForInputInRange);
                ret.callbackSerializeCustomData(customData);
            }
        }
    }

    return ret;
}
ZFCALLBACK_SERIALIZE_CUSTOM_TYPE_DEFINE(ZFInputForInputInRange, ZFCallbackSerializeCustomType_ZFInputForInputInRange)
{
    const ZFSerializableData *inputData = ZFSerializableUtil::requireElementByCategory(
        serializableData, ZFSerializableKeyword_ZFInputForInputInRange_input, outErrorHint, outErrorPos);
    if(inputData == zfnull)
    {
        return zffalse;
    }
    ZFCallback input;
    if(!ZFCallbackFromData(input, *inputData, outErrorHint, outErrorPos))
    {
        return zffalse;
    }

    zfindex start = 0;
    {
        const ZFSerializableData *startData = ZFSerializableUtil::checkElementByCategory(serializableData, ZFSerializableKeyword_ZFInputForInputInRange_start);
        if(startData != zfnull && !zfindexFromData(start, *startData, outErrorHint, outErrorPos))
        {
            return zffalse;
        }
    }
    zfindex count = zfindexMax();
    {
        const ZFSerializableData *countData = ZFSerializableUtil::checkElementByCategory(serializableData, ZFSerializableKeyword_ZFInputForInputInRange_count);
        if(countData != zfnull && !zfindexFromData(count, *countData, outErrorHint, outErrorPos))
        {
            return zffalse;
        }
    }
    zfbool autoRestorePos = zftrue;
    {
        const ZFSerializableData *autoRestorePosData = ZFSerializableUtil::checkElementByCategory(serializableData, ZFSerializableKeyword_ZFInputForInputInRange_autoRestorePos);
        if(autoRestorePosData != zfnull && !zfboolFromData(autoRestorePos, *autoRestorePosData, outErrorHint, outErrorPos))
        {
            return zffalse;
        }
    }
    serializableData.resolveMark();

    ret = ZFInputForInputInRange(input, start, count, autoRestorePos);
    return zftrue;
}

// ============================================================
// ZFInputForBufferUnsafe
zfclass _ZFP_I_ZFInputForBufferUnsafeOwner : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_I_ZFInputForBufferUnsafeOwner, ZFObject)

public:
    const zfbyte *pStart;
    const zfbyte *pEnd; // ensured valid
    const zfbyte *p;

    ZFALLOC_CACHE_RELEASE({
        cache->pStart = zfnull;
        cache->pEnd = zfnull;
        cache->p = zfnull;
    })

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
ZFMETHOD_DEFINE_2(_ZFP_I_ZFInputForBufferUnsafeOwner, zfindex, onInput,
                  ZFMP_IN(void *, buf),
                  ZFMP_IN(zfindex, count))
{
    if(buf == zfnull)
    {
        return pEnd - p;
    }
    if(p + count > pEnd)
    {
        count = pEnd - p;
    }
    zfmemcpy(buf, p, count);
    p += count;
    return count;
}
ZFMETHOD_DEFINE_2(_ZFP_I_ZFInputForBufferUnsafeOwner, zfbool, ioSeek,
                  ZFMP_IN(zfindex, byteSize),
                  ZFMP_IN(ZFSeekPos, pos))
{
    p = pStart + ZFIOCallbackCalcFSeek(0, pEnd - pStart, p - pStart, byteSize, pos);
    return zftrue;
}
ZFMETHOD_DEFINE_0(_ZFP_I_ZFInputForBufferUnsafeOwner, zfindex, ioTell)
{
    return p - pStart;
}
ZFMETHOD_DEFINE_0(_ZFP_I_ZFInputForBufferUnsafeOwner, zfindex, ioSize)
{
    return pEnd - p;
}
static ZFInput _ZFP_ZFInputForBuffer(ZF_IN zfbool copy,
                                     ZF_IN const void *src,
                                     ZF_IN_OPT zfindex count /* = zfindexMax() */)
{
    if(src == zfnull)
    {
        return ZFCallbackNull();
    }
    zfindex len = count;
    void *srcTmp = zfnull;
    if(len == zfindexMax())
    {
        len = zfslen((const zfchar *)src);
    }
    if(copy)
    {
        srcTmp = zfmalloc(sizeof(zfchar) * len);
        zfmemcpy(srcTmp, src, sizeof(zfchar) * len);
        src = srcTmp;
    }
    if(src == zfnull)
    {
        return ZFCallbackNull();
    }

    _ZFP_I_ZFInputForBufferUnsafeOwner *owner = zfAllocWithCache(_ZFP_I_ZFInputForBufferUnsafeOwner);
    owner->pStart = (const zfbyte *)src;
    owner->pEnd = owner->pStart + len;
    owner->p = owner->pStart;
    ZFInput ret = ZFCallbackForMemberMethod(
        owner, ZFMethodAccess(_ZFP_I_ZFInputForBufferUnsafeOwner, onInput));
    ret.callbackTag(ZFCallbackTagKeyword_ioOwner, owner);
    if(copy)
    {
        ret.callbackTag(
            "ZFInputForBufferCopiedBuffer",
            zflineAlloc(ZFTypeHolder, srcTmp, ZFTypeHolderTypePOD));

    }
    zfRelease(owner);
    return ret;
}
ZFInput ZFInputForBufferUnsafe(ZF_IN const void *src,
                               ZF_IN_OPT zfindex count /* = zfindexMax() */)
{
    return _ZFP_ZFInputForBuffer(zffalse, src, count);
}
ZFInput ZFInputForBuffer(ZF_IN const void *src,
                         ZF_IN_OPT zfindex count /* = zfindexMax() */)
{
    return _ZFP_ZFInputForBuffer(zftrue, src, count);
}
ZFInput ZFInputForString(ZF_IN const zfchar *src,
                         ZF_IN_OPT zfindex count /* = zfindexMax() */)
{
    return _ZFP_ZFInputForBuffer(zftrue, src, count);
}

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_USER_REGISTER_2({
        ZFInput input = invokerObject->to<v_ZFCallback *>()->zfv;
        return input.input(buf, count);
    }, v_ZFCallback, zfindex, input, ZFMP_OUT(zfstring &, buf), ZFMP_IN(zfindex, count))

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(ZFBuffer, ZFInputReadToBuffer, ZFMP_IN_OUT(const ZFInput &, input))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_0(ZFInput, ZFInputDummy)
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_4(ZFInput, ZFInputForInputInRange, ZFMP_IN(const ZFInput &, inputCallback), ZFMP_IN_OPT(zfindex, start, 0), ZFMP_IN_OPT(zfindex, count, zfindexMax()), ZFMP_IN_OPT(zfbool, autoRestorePos, zftrue))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(ZFInput, ZFInputForBufferUnsafe, ZFMP_IN(const zfchar *, buf), ZFMP_IN_OPT(zfindex, count, zfindexMax()))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(ZFInput, ZFInputForBuffer, ZFMP_IN(const zfchar *, buf), ZFMP_IN_OPT(zfindex, count, zfindexMax()))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(ZFInput, ZFInputForString, ZFMP_IN(const zfchar *, buf), ZFMP_IN_OPT(zfindex, count, zfindexMax()))

ZF_NAMESPACE_GLOBAL_END
#endif

