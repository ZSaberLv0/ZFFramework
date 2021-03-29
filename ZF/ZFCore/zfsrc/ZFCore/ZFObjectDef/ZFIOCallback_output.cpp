#include "ZFIOCallback_output.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFOutputDummy
static zfindex _ZFP_ZFOutputDummy(ZF_IN const void *s, ZF_IN zfindex count)
{
    return count;
}
ZFOutput ZFOutputDummy(void)
{
    ZFOutput ret = ZFCallbackForFunc(_ZFP_ZFOutputDummy);
    ret.callbackSerializeCustomType(ZFCallbackSerializeCustomType_ZFOutputDummy);
    ret.callbackSerializeCustomData(ZFSerializableData());
    return ret;
}

// ============================================================
// ZFOutputForString
zfclass _ZFP_I_ZFOutputForStringOwner : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_I_ZFOutputForStringOwner, ZFObject)

    ZFALLOC_CACHE_RELEASE({
    })

public:
    zfstring *pString;
    zfindex savedLength;
    zfindex curPos;
    ZFMETHOD_DECLARE_2(zfindex, onOutput,
                      ZFMP_IN(const void *, s),
                      ZFMP_IN(zfindex, count))
    ZFMETHOD_DECLARE_2(zfbool, ioSeek,
                      ZFMP_IN(zfindex, byteSize),
                      ZFMP_IN(ZFSeekPos, pos))
    ZFMETHOD_DECLARE_0(zfindex, ioTell)
    ZFMETHOD_DECLARE_0(zfindex, ioSize)
};
ZFMETHOD_DEFINE_2(_ZFP_I_ZFOutputForStringOwner, zfindex, onOutput,
                  ZFMP_IN(const void *, s),
                  ZFMP_IN(zfindex, count))
{
    if(count == zfindexMax())
    {
        count = zfslen((const zfchar *)s);
    }
    else
    {
        count /= sizeof(zfchar);
    }
    this->pString->replace(this->curPos, zfmMin(this->pString->length() - curPos, count), (const zfchar *)s, count);
    this->curPos += count;
    return count * sizeof(zfchar);
}
ZFMETHOD_DEFINE_2(_ZFP_I_ZFOutputForStringOwner, zfbool, ioSeek,
                  ZFMP_IN(zfindex, byteSize),
                  ZFMP_IN(ZFSeekPos, pos))
{
    this->curPos = ZFIOCallbackCalcFSeek(this->savedLength, this->pString->length(), this->curPos, byteSize, pos);
    return zftrue;
}
ZFMETHOD_DEFINE_0(_ZFP_I_ZFOutputForStringOwner, zfindex, ioTell)
{
    return ((this->pString->length() >= this->savedLength) ? (this->pString->length() - this->savedLength) : zfindexMax());
}
ZFMETHOD_DEFINE_0(_ZFP_I_ZFOutputForStringOwner, zfindex, ioSize)
{
    return this->pString->length() - this->curPos;
}
ZFOutput ZFOutputForString(ZF_IN zfstring &s)
{
    _ZFP_I_ZFOutputForStringOwner *owner = zfAllocWithCache(_ZFP_I_ZFOutputForStringOwner);
    owner->pString = &s;
    owner->savedLength = s.length();
    owner->curPos = s.length();
    ZFOutput ret = ZFCallbackForMemberMethod(
        owner, ZFMethodAccess(_ZFP_I_ZFOutputForStringOwner, onOutput));
    ret.callbackTag(ZFCallbackTagKeyword_ioOwner, owner);
    zfRelease(owner);
    return ret;
}

// ============================================================
// ZFOutputForBufferUnsafe
zfclass _ZFP_I_ZFOutputForBufferUnsafeOwner : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_I_ZFOutputForBufferUnsafeOwner, ZFObject)

    ZFALLOC_CACHE_RELEASE({
    })

public:
    zfbool autoAppendNullToken;
    zfbyte *pStart;
    zfbyte *pEnd; // autoAppendNullToken ? (buf + maxCount - 1) : (buf + maxCount)
    zfbyte *p;

public:
    ZFMETHOD_DECLARE_2(zfindex, onOutput,
                      ZFMP_IN(const void *, s),
                      ZFMP_IN(zfindex, count))
    ZFMETHOD_DECLARE_2(zfbool, ioSeek,
                      ZFMP_IN(zfindex, byteSize),
                      ZFMP_IN(ZFSeekPos, pos))
    ZFMETHOD_DECLARE_0(zfindex, ioTell)
    ZFMETHOD_DECLARE_0(zfindex, ioSize)
};
ZFMETHOD_DEFINE_2(_ZFP_I_ZFOutputForBufferUnsafeOwner, zfindex, onOutput,
                  ZFMP_IN(const void *, s),
                  ZFMP_IN(zfindex, count))
{
    const zfbyte *pSrc = (const zfbyte *)s;
    if(count == zfindexMax())
    {
        while(*pSrc && p < pEnd)
        {
            *p = *pSrc;
            ++p;
            ++pSrc;
        }
        if(autoAppendNullToken)
        {
            *p = '\0';
        }
        return p - (const zfbyte *)s;
    }
    else
    {
        const zfbyte *pSrcEnd = (const zfbyte *)s + count;
        while(*pSrc && pSrc < pSrcEnd && p < pEnd)
        {
            *p = *pSrc;
            ++p;
            ++pSrc;
        }
        if(autoAppendNullToken)
        {
            *p = '\0';
        }
        return p - (const zfbyte *)s;
    }
}
ZFMETHOD_DEFINE_2(_ZFP_I_ZFOutputForBufferUnsafeOwner, zfbool, ioSeek,
                  ZFMP_IN(zfindex, byteSize),
                  ZFMP_IN(ZFSeekPos, pos))
{
    p = pStart + ZFIOCallbackCalcFSeek(0, pEnd - pStart, p - pStart, byteSize, pos);
    return zftrue;
}
ZFMETHOD_DEFINE_0(_ZFP_I_ZFOutputForBufferUnsafeOwner, zfindex, ioTell)
{
    return p - pStart;
}
ZFMETHOD_DEFINE_0(_ZFP_I_ZFOutputForBufferUnsafeOwner, zfindex, ioSize)
{
    return pEnd - pStart;
}
ZFOutput ZFOutputForBufferUnsafe(ZF_IN void *buf,
                                 ZF_IN_OPT zfindex maxCount /* = zfindexMax() */,
                                 ZF_IN_OPT zfbool autoAppendNullToken /* = zftrue */)
{
    if(buf == zfnull || maxCount == 0 || (maxCount == 1 && autoAppendNullToken))
    {
        return ZFCallbackNull();
    }
    _ZFP_I_ZFOutputForBufferUnsafeOwner *owner = zfAllocWithCache(_ZFP_I_ZFOutputForBufferUnsafeOwner);
    owner->autoAppendNullToken = autoAppendNullToken;
    owner->pStart = (zfbyte *)buf;
    if(maxCount == zfindexMax())
    {
        owner->pEnd = zfnull;
        --(owner->pEnd);
    }
    else
    {
        owner->pEnd = owner->pStart + maxCount;
        if(autoAppendNullToken)
        {
            --(owner->pEnd);
        }
    }
    owner->p = owner->pStart;
    ZFOutput ret = ZFCallbackForMemberMethod(
        owner, ZFMethodAccess(_ZFP_I_ZFOutputForBufferUnsafeOwner, onOutput));
    ret.callbackTag(ZFCallbackTagKeyword_ioOwner, owner);
    zfRelease(owner);
    return ret;
}

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_USER_REGISTER_2({
        ZFOutput output = invokerObject->to<v_ZFCallback *>()->zfv;
        return output.output(src, size);
    }, v_ZFCallback, zfindex, output, ZFMP_IN(const zfchar *, src), ZFMP_IN_OPT(zfindex, size, zfindexMax()))

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_0(ZFOutput, ZFOutputDummy)
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(ZFOutput, ZFOutputForString, ZFMP_IN_OUT(zfstring &, s))

ZF_NAMESPACE_GLOBAL_END
#endif

