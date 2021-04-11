#include "ZFTypeId_CoreType.h"
#include "ZFObjectImpl.h"
#include "ZFSerializableDataSerializableConverter.h"
#include "ZFIdMap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// string conversion spec
#define _ZFP_zfflagsInvalidToken ("0x0")
zfbool zfflagsToString(ZF_IN_OUT zfstring &ret,
                       ZF_IN const zfflags *flagList,
                       ZF_IN const zfchar **nameList,
                       ZF_IN zfindex listCount,
                       ZF_IN zfflags const &value,
                       ZF_IN_OPT zfbool includeNotConverted /* = zftrue */,
                       ZF_IN_OPT zfbool exclusiveMode /* = zffalse */,
                       ZF_OUT_OPT zfflags *notConverted /* = zfnull */,
                       ZF_IN_OPT zfchar separatorToken /* = '|' */)
{
    zfflags flagsLeft = value;
    if(exclusiveMode)
    {
        for(zfindex i = 0; i < listCount; ++i)
        {
            if(flagList[i] && ZFBitTest(flagsLeft, flagList[i]))
            {
                ret += nameList[i];
                ret += separatorToken;
                ZFBitUnset(flagsLeft, flagList[i]);
            }
            if(flagsLeft == 0)
            {
                break;
            }
        }
    }
    else
    {
        for(zfindex i = 0; i < listCount; ++i)
        {
            if(flagList[i] && ZFBitTest(value, flagList[i]))
            {
                ret += nameList[i];
                ret += separatorToken;
                ZFBitUnset(flagsLeft, flagList[i]);
            }
        }
    }

    // check flagsLeft and remove tailing separatorToken from ret
    if(flagsLeft == 0)
    {
        if(ret.isEmpty())
        {
            zfindex zeroTokenIndex = zfindexMax();
            for(zfindex i = 0; i < listCount; ++i)
            {
                if(flagList[i] == 0)
                {
                    zeroTokenIndex = i;
                    break;
                }
            }
            if(zeroTokenIndex == zfindexMax())
            {
                ret = _ZFP_zfflagsInvalidToken;
            }
            else
            {
                ret = nameList[zeroTokenIndex];
            }
        }
        else
        {
            ret.remove(ret.length() - 1);
        }
    }
    else if(includeNotConverted)
    {
        ret += "0x";
        zfsFromIntT(ret, flagsLeft,
            zfHint("radix")16,
            zfHint("upper case?")zftrue);
    }

    if(notConverted != zfnull)
    {
        *notConverted = flagsLeft;
    }

    return zftrue;
}
zfbool zfflagsFromString(ZF_OUT zfflags &ret,
                         ZF_IN const zfflags *flagList,
                         ZF_IN const zfchar **nameList,
                         ZF_IN zfindex listCount,
                         ZF_IN const zfchar *src,
                         ZF_IN_OPT zfindex srcLen /* = zfindexMax() */,
                         ZF_IN_OPT zfchar separatorToken /* = '|' */,
                         ZF_OUT_OPT const zfchar **outErrorPos /* = zfnull */)
{
    if(src == zfnull)
    {
        if(outErrorPos != zfnull)
        {
            *outErrorPos = src;
        }
        return zffalse;
    }
    ZFCoreArrayPOD<ZFIndexRange> pos;
    zfstring separatorTokens;
    separatorTokens += separatorToken;
    if(!zfCoreDataPairSplitString(pos, zfindexMax(), src, srcLen, separatorTokens, zfnull, zfnull, zffalse, outErrorPos))
    {
        return zffalse;
    }
    ret = 0;
    for(zfindex iSrc = 0; iSrc < pos.count(); ++iSrc)
    {
        zfbool recognized = zffalse;
        for(zfindex iList = 0; iList < listCount; ++iList)
        {
            if(zfsncmp(src + pos[iSrc].start, nameList[iList], pos[iSrc].count) == 0)
            {
                recognized = zftrue;
                ZFBitSet(ret, flagList[iList]);
                break;
            }
        }
        if(!recognized)
        {
            if(pos[iSrc].count > 2 && *(src + pos[iSrc].start) == '0' && *(src + pos[iSrc].start + 1) == 'x')
            {
                zfflags tmp = zfflagsZero();
                if(!zfsToIntT(tmp, src + pos[iSrc].start + 2, pos[iSrc].count - 2, 16, zftrue, outErrorPos))
                {
                    return zffalse;
                }
                ZFBitSet(ret, tmp);
            }
            else
            {
                if(outErrorPos != zfnull)
                {
                    *outErrorPos = (src + pos[iSrc].start);
                }
                return zffalse;
            }
        }
    }
    return zftrue;
}

zfbool zfstringFromData(ZF_OUT const zfchar * &v,
                        ZF_IN const ZFSerializableData &serializableData,
                        ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */,
                        ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */)
{
    if(ZFSerializableUtil::requireItemClass(serializableData, ZFTypeId_zfstring(), outErrorHint, outErrorPos) == zfnull)
    {
        return zffalse;
    }
    const zfchar *valueString = ZFSerializableUtil::checkPropertyValue(serializableData);
    if(valueString == zfnull)
    {
        v = zfnull;
        return zftrue;
    }
    serializableData.resolveMark();
    v = valueString;
    return zftrue;
}
zfbool zfstringToData(ZF_OUT ZFSerializableData &serializableData,
                      ZF_IN const zfchar * const &v,
                      ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */)
{
    if(v == zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHint,
            "null string");
        return zffalse;
    }

    serializableData.itemClass(ZFTypeId_zfstring());
    serializableData.propertyValue(v);
    return zftrue;
}

// ============================================================
// utils
#define _ZFP_ZFTYPEID_DEFINE_int_allow_negative(TypeName, Type) \
    ZFTYPEID_DEFINE_BY_STRING_CONVERTER(TypeName, Type, { \
            return zfsToIntT(v, src, srcLen, 10, zftrue); \
        }, { \
            return zfsFromIntT(s, v); \
        })
#define _ZFP_ZFTYPEID_DEFINE_int_disallow_negative(TypeName, Type) \
    ZFTYPEID_DEFINE_BY_STRING_CONVERTER(TypeName, Type, { \
            if(src != zfnull && zfsncmp(src, "-1", 2) == 0) \
            { \
                v = (Type)-1; \
                return zftrue; \
            } \
            else \
            { \
                return zfsToIntT(v, src, srcLen, 10, zffalse); \
            } \
        }, { \
            if(v == ((Type)-1)) \
            { \
                s += "-1"; \
                return zftrue; \
            } \
            else \
            { \
                return zfsFromIntT(s, v); \
            } \
        })
#define _ZFP_ZFTYPEID_DEFINE_float(TypeName, Type) \
    ZFTYPEID_DEFINE_BY_STRING_CONVERTER(TypeName, Type, { \
            return zfsToFloatT(v, src, srcLen); \
        }, { \
            return zfsFromFloatT(s, v); \
        })

// ============================================================
ZFTYPEID_DEFINE_BY_STRING_CONVERTER(zfbool, zfbool, {
        const zfchar *tokens[] = ZFM_EXPAND({
            ZFTOKEN_zfbool_zftrue,
            ZFTOKEN_zfbool_zffalse,
        });
        zfindex matched = zfsCheckMatch(tokens, ZFM_ARRAY_SIZE(tokens), src, srcLen);
        v = zffalse;
        switch(matched)
        {
            case 0:
                v = zftrue;
                return zftrue;
            case 1:
                v = zffalse;
                return zftrue;
            default:
                return zffalse;
        }
    }, {
        s += (v ? ZFTOKEN_zfbool_zftrue : ZFTOKEN_zfbool_zffalse);
        return zftrue;
    })

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(v_zfbool, ZFLevelZFFrameworkEssential)
{
    this->obj_zftrue = zflockfree_zfAlloc(v_zfbool, zftrue);
    this->obj_zffalse = zflockfree_zfAlloc(v_zfbool, zffalse);
}
ZF_GLOBAL_INITIALIZER_DESTROY(v_zfbool)
{
    zflockfree_zfRelease(this->obj_zftrue);
    zflockfree_zfRelease(this->obj_zffalse);
}
v_zfbool *obj_zftrue;
v_zfbool *obj_zffalse;
ZF_GLOBAL_INITIALIZER_END(v_zfbool)
v_zfbool *_ZFP_v_zftrue(void)
{
    return ZF_GLOBAL_INITIALIZER_INSTANCE(v_zfbool)->obj_zftrue;
}
v_zfbool *_ZFP_v_zffalse(void)
{
    return ZF_GLOBAL_INITIALIZER_INSTANCE(v_zfbool)->obj_zffalse;
}

// ============================================================
ZFTYPEID_DEFINE_BY_STRING_CONVERTER(zfbyte, zfbyte, {
        return zfsToIntT(v, src, srcLen, 16, zffalse);
    }, {
        return zfsFromIntT(s, v, 16);
    })

// ============================================================
ZFTYPEID_DEFINE_BY_STRING_CONVERTER(zfchar, zfchar, {
        if(src == zfnull || srcLen == 0)
        {
            return zffalse;
        }
        v = *src;
        return zftrue;
    }, {
        s += v;
        return zftrue;
    })

// ============================================================
ZFTYPEID_DEFINE_BY_STRING_CONVERTER(zfstring, zfstring, {
        v.append(src, srcLen);
        return zftrue;
    }, {
        s += v;
        return zftrue;
    })
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_zfstring, void, append, ZFMP_IN(const zfstring &, s))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_zfstring, void, append, ZFMP_IN(const zfchar *, s), ZFMP_IN(zfindex, len))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_zfstring, void, assign, ZFMP_IN(const zfstring &, s))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_zfstring, void, assign, ZFMP_IN(const zfchar *, s), ZFMP_IN(zfindex, len))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_zfstring, void, insert, ZFMP_IN(zfindex, insertAt), ZFMP_IN(const zfstring &, s))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_3(v_zfstring, void, insert, ZFMP_IN(zfindex, insertAt), ZFMP_IN(const zfchar *, s), ZFMP_IN(zfindex, len))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_3(v_zfstring, void, replace, ZFMP_IN(zfindex, replacePos), ZFMP_IN(zfindex, replaceLen), ZFMP_IN(const zfstring &, s))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_4(v_zfstring, void, replace, ZFMP_IN(zfindex, replacePos), ZFMP_IN(zfindex, replaceLen), ZFMP_IN(const zfchar *, s), ZFMP_IN(zfindex, len))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_zfstring, zfindex, length)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_zfstring, zfbool, isEmpty)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_zfstring, void, capacity, ZFMP_IN(zfindex, capacity))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_zfstring, zfindex, capacity)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_zfstring, void, remove, ZFMP_IN_OPT(zfindex, pos, 0), ZFMP_IN_OPT(zfindex, len, zfindexMax()))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_zfstring, void, removeAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_zfstring, zfint, compare, ZFMP_IN(const zfstring &, s))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_zfstring, zfint, compare, ZFMP_IN(const zfchar *, s), ZFMP_IN_OPT(zfindex, len, zfindexMax()))

ZFTYPEID_ALIAS_DEFINE(zfstring, zfstring, cString, const zfchar *)

// ============================================================
_ZFP_ZFTYPEID_DEFINE_int_allow_negative(zfint, zfint)
ZFTYPEID_PROGRESS_DEFINE_BY_VALUE(zfint, zfint)
// ============================================================
_ZFP_ZFTYPEID_DEFINE_int_disallow_negative(zfuint, zfuint)
ZFTYPEID_PROGRESS_DEFINE_BY_VALUE(zfuint, zfuint)

// ============================================================
_ZFP_ZFTYPEID_DEFINE_int_disallow_negative(zfindex, zfindex)
ZFTYPEID_PROGRESS_DEFINE_BY_VALUE(zfindex, zfindex)

// ============================================================
_ZFP_ZFTYPEID_DEFINE_float(zffloat, zffloat)
ZFTYPEID_PROGRESS_DEFINE_BY_VALUE(zffloat, zffloat)

// ============================================================
_ZFP_ZFTYPEID_DEFINE_float(zfdouble, zfdouble)
ZFTYPEID_PROGRESS_DEFINE_BY_VALUE(zfdouble, zfdouble)

// ============================================================
_ZFP_ZFTYPEID_DEFINE_float(zflongdouble, zflongdouble)
ZFTYPEID_PROGRESS_DEFINE_BY_VALUE(zflongdouble, zflongdouble)

// ============================================================
_ZFP_ZFTYPEID_DEFINE_int_allow_negative(zftimet, zftimet)
ZFTYPEID_PROGRESS_DEFINE(zftimet, zftimet, {
        ret = (zft_zftimet)(from + (zft_zftimet)((to - from) * progress));
    })

// ============================================================
ZFTYPEID_DEFINE(zfidentity, zfidentity, {
        if(ZFSerializableUtil::requireItemClass(serializableData, ZFTypeId_zfidentity(), outErrorHint, outErrorPos) == zfnull)
        {
            return zffalse;
        }
        const zfchar *valueString = ZFSerializableUtil::checkPropertyValue(serializableData);
        if(valueString == zfnull)
        {
            v = zfidentityInvalid();
            return zftrue;
        }
        if(!zfidentityFromString(v, valueString))
        {
            ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
                "invalid value: \"%s\"", valueString);
            return zffalse;
        }
        serializableData.resolveMark();
        return zftrue;
    }, {
        serializableData.itemClass(ZFTypeId_zfidentity());
        zfstring s;
        if(!zfidentityToString(s, v))
        {
            ZFSerializableUtil::errorOccurred(outErrorHint,
                "unable to convert value to string");
            return zffalse;
        }
        serializableData.propertyValue(s.isEmpty() ? zfnull : s.cString());
        return zftrue;
    }, {
        if(src == zfnull
            || srcLen == 0
            || zfsncmp(src, ZFTOKEN_zfidentityInvalid, srcLen) == 0)
        {
            v = zfidentityInvalid();
            return zftrue;
        }
        v = ZFIdMapIdForName(srcLen == zfindexMax() || src[srcLen] == '\0' ? src : zfstring(src, srcLen).cString());
        return (v != zfidentityInvalid());
    }, {
        if(v == zfidentityInvalid())
        {
            s += ZFTOKEN_zfidentityInvalid;
            return zftrue;
        }
        else
        {
            const zfchar *name = ZFIdMapNameForId(v);
            if(name == zfnull)
            {
                return zffalse;
            }
            else
            {
                s += name;
                return zftrue;
            }
        }
    })

// ============================================================
_ZFP_ZFTYPEID_DEFINE_int_disallow_negative(zfflags, zfflags)

// ============================================================
ZFTYPEID_DEFINE_BY_STRING_CONVERTER(ZFCompareResult, ZFCompareResult, {
        if(src == zfnull) {return zffalse;}
        if(zfsncmp(src, ZFTOKEN_ZFCompareTheSame, srcLen) == 0)
        {
            v = ZFCompareTheSame;
            return zftrue;
        }
        else if(zfsncmp(src, ZFTOKEN_ZFCompareSmaller, srcLen) == 0)
        {
            v = ZFCompareSmaller;
            return zftrue;
        }
        else if(zfsncmp(src, ZFTOKEN_ZFCompareGreater, srcLen) == 0)
        {
            v = ZFCompareGreater;
            return zftrue;
        }
        else if(zfsncmp(src, ZFTOKEN_ZFCompareUncomparable, srcLen) == 0)
        {
            v = ZFCompareUncomparable;
            return zftrue;
        }
        else
        {
            return zffalse;
        }
    }, {
        switch(v)
        {
            case ZFCompareUncomparable:
                s += ZFTOKEN_ZFCompareUncomparable;
                return zftrue;
            case ZFCompareSmaller:
                s += ZFTOKEN_ZFCompareSmaller;
                return zftrue;
            case ZFCompareTheSame:
                s += ZFTOKEN_ZFCompareTheSame;
                return zftrue;
            case ZFCompareGreater:
                s += ZFTOKEN_ZFCompareGreater;
                return zftrue;
            default:
                zfCoreCriticalShouldNotGoHere();
                return zffalse;
        }
    })

// ============================================================
ZFTYPEID_DEFINE_BY_STRING_CONVERTER(ZFSeekPos, ZFSeekPos, {
        const zfchar *tokens[] = ZFM_EXPAND({
            ZFTOKEN_ZFSeekPosBegin,
            ZFTOKEN_ZFSeekPosCur,
            ZFTOKEN_ZFSeekPosCurReversely,
            ZFTOKEN_ZFSeekPosEnd,
        });
        zfindex matched = zfsCheckMatch(tokens, ZFM_ARRAY_SIZE(tokens), src, srcLen);
        v = ZFSeekPosBegin;
        switch(matched)
        {
            case 0:
                v = ZFSeekPosBegin;
                return zftrue;
            case 1:
                v = ZFSeekPosCur;
                return zftrue;
            case 2:
                v = ZFSeekPosCurReversely;
                return zftrue;
            case 3:
                v = ZFSeekPosEnd;
                return zftrue;
            default:
                return zffalse;
        }
    }, {
        switch(v)
        {
            case ZFSeekPosBegin:
                s += ZFTOKEN_ZFSeekPosBegin;
                return zftrue;
            case ZFSeekPosCur:
                s += ZFTOKEN_ZFSeekPosCur;
                return zftrue;
            case ZFSeekPosCurReversely:
                s += ZFTOKEN_ZFSeekPosCurReversely;
                return zftrue;
            case ZFSeekPosEnd:
                s += ZFTOKEN_ZFSeekPosEnd;
                return zftrue;
            default:
                zfCoreCriticalShouldNotGoHere();
                return zffalse;
        }
    })

// ============================================================
ZFTYPEID_DEFINE_BY_STRING_CONVERTER(ZFIndexRange, ZFIndexRange, {
        v = ZFIndexRangeZero();
        ZFCoreArrayPOD<zfindex> pair;
        if(!zfCoreDataPairSplitInt(pair, 2, src, srcLen))
        {
            return zffalse;
        }
        v.start = pair[0];
        v.count = pair[1];
        return zftrue;
    }, {
        zfstringAppend(s, "(%zi, %zi)", v.start, v.count);
        return zftrue;
    })
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFIndexRange, zfindex, start)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFIndexRange, zfindex, count)

// ============================================================
ZFTYPEID_DEFINE_BY_STRING_CONVERTER(ZFLevel, ZFLevel, {
        const zfchar *tokens[] = ZFM_EXPAND({
            ZFTOKEN_ZFLevelZFFrameworkStatic,
            ZFTOKEN_ZFLevelZFFrameworkEssential,
            ZFTOKEN_ZFLevelZFFrameworkHigh,
            ZFTOKEN_ZFLevelZFFrameworkNormal,
            ZFTOKEN_ZFLevelZFFrameworkLow,

            ZFTOKEN_ZFLevelAppEssential,
            ZFTOKEN_ZFLevelAppHigh,
            ZFTOKEN_ZFLevelAppNormal,
            ZFTOKEN_ZFLevelAppLow,

            ZFTOKEN_ZFLevelZFFrameworkPostLow,
            ZFTOKEN_ZFLevelZFFrameworkPostNormal,
            ZFTOKEN_ZFLevelZFFrameworkPostHigh,
            ZFTOKEN_ZFLevelZFFrameworkPostEssential,
            ZFTOKEN_ZFLevelZFFrameworkPostStatic,
        });
        zfindex matched = zfsCheckMatch(tokens, ZFM_ARRAY_SIZE(tokens), src, srcLen);
        v = ZFLevelAppLow;
        switch(matched)
        {
            case 0:
                v = ZFLevelZFFrameworkStatic;
                return zftrue;
            case 1:
                v = ZFLevelZFFrameworkEssential;
                return zftrue;
            case 2:
                v = ZFLevelZFFrameworkHigh;
                return zftrue;
            case 3:
                v = ZFLevelZFFrameworkNormal;
                return zftrue;
            case 4:
                v = ZFLevelZFFrameworkLow;
                return zftrue;

            case 5:
                v = ZFLevelAppEssential;
                return zftrue;
            case 6:
                v = ZFLevelAppHigh;
                return zftrue;
            case 7:
                v = ZFLevelAppNormal;
                return zftrue;
            case 8:
                v = ZFLevelAppLow;
                return zftrue;

            case 9:
                v = ZFLevelZFFrameworkPostLow;
                return zftrue;
            case 10:
                v = ZFLevelZFFrameworkPostNormal;
                return zftrue;
            case 11:
                v = ZFLevelZFFrameworkPostHigh;
                return zftrue;
            case 12:
                v = ZFLevelZFFrameworkPostEssential;
                return zftrue;
            case 13:
                v = ZFLevelZFFrameworkPostStatic;
                return zftrue;

            default:
                return zffalse;
        }
    }, {
        switch(v)
        {
            case ZFLevelZFFrameworkStatic:
                s += ZFTOKEN_ZFLevelZFFrameworkStatic;
                return zftrue;
            case ZFLevelZFFrameworkEssential:
                s += ZFTOKEN_ZFLevelZFFrameworkEssential;
                return zftrue;
            case ZFLevelZFFrameworkHigh:
                s += ZFTOKEN_ZFLevelZFFrameworkHigh;
                return zftrue;
            case ZFLevelZFFrameworkNormal:
                s += ZFTOKEN_ZFLevelZFFrameworkNormal;
                return zftrue;
            case ZFLevelZFFrameworkLow:
                s += ZFTOKEN_ZFLevelZFFrameworkLow;
                return zftrue;

            case ZFLevelAppEssential:
                s += ZFTOKEN_ZFLevelAppEssential;
                return zftrue;
            case ZFLevelAppHigh:
                s += ZFTOKEN_ZFLevelAppHigh;
                return zftrue;
            case ZFLevelAppNormal:
                s += ZFTOKEN_ZFLevelAppNormal;
                return zftrue;
            case ZFLevelAppLow:
                s += ZFTOKEN_ZFLevelAppLow;
                return zftrue;

            case ZFLevelZFFrameworkPostLow:
                s += ZFTOKEN_ZFLevelZFFrameworkPostLow;
                return zftrue;
            case ZFLevelZFFrameworkPostNormal:
                s += ZFTOKEN_ZFLevelZFFrameworkPostNormal;
                return zftrue;
            case ZFLevelZFFrameworkPostHigh:
                s += ZFTOKEN_ZFLevelZFFrameworkPostHigh;
                return zftrue;
            case ZFLevelZFFrameworkPostEssential:
                s += ZFTOKEN_ZFLevelZFFrameworkPostEssential;
                return zftrue;
            case ZFLevelZFFrameworkPostStatic:
                s += ZFTOKEN_ZFLevelZFFrameworkPostStatic;
                return zftrue;

            default:
                return zffalse;
        }
    })

// ============================================================
ZFTYPEID_DEFINE_BY_STRING_CONVERTER(ZFFrameworkState, ZFFrameworkState, {
        const zfchar *tokens[] = ZFM_EXPAND({
            ZFTOKEN_ZFFrameworkStateNotAvailable,
            ZFTOKEN_ZFFrameworkStateInitProcessing,
            ZFTOKEN_ZFFrameworkStateAvailable,
            ZFTOKEN_ZFFrameworkStateCleanupProcessing,
        });
        zfindex matched = zfsCheckMatch(tokens, ZFM_ARRAY_SIZE(tokens), src, srcLen);
        v = ZFFrameworkStateNotAvailable;
        switch(matched)
        {
            case 0:
                v = ZFFrameworkStateNotAvailable;
                return zftrue;
            case 1:
                v = ZFFrameworkStateInitProcessing;
                return zftrue;
            case 2:
                v = ZFFrameworkStateAvailable;
                return zftrue;
            case 3:
                v = ZFFrameworkStateCleanupProcessing;
                return zftrue;
            default:
                return zffalse;
        }
    }, {
        switch(v)
        {
            case ZFFrameworkStateNotAvailable:
                s += ZFTOKEN_ZFFrameworkStateNotAvailable;
                return zftrue;
            case ZFFrameworkStateInitProcessing:
                s += ZFTOKEN_ZFFrameworkStateInitProcessing;
                return zftrue;
            case ZFFrameworkStateAvailable:
                s += ZFTOKEN_ZFFrameworkStateAvailable;
                return zftrue;
            case ZFFrameworkStateCleanupProcessing:
                s += ZFTOKEN_ZFFrameworkStateCleanupProcessing;
                return zftrue;
            default:
                return zffalse;
        }
    })

// ============================================================
ZFTYPEID_DEFINE_BY_STRING_CONVERTER(ZFFilterType, ZFFilterType, {
        const zfchar *tokens[] = ZFM_EXPAND({
            ZFTOKEN_ZFFilterTypeInclude,
            ZFTOKEN_ZFFilterTypeExclude,
        });
        zfindex matched = zfsCheckMatch(tokens, ZFM_ARRAY_SIZE(tokens), src, srcLen);
        v = ZFFilterTypeInclude;
        switch(matched)
        {
            case 0:
                v = ZFFilterTypeInclude;
                return zftrue;
            case 1:
                v = ZFFilterTypeExclude;
                return zftrue;
            default:
                return zffalse;
        }
    }, {
        switch(v)
        {
            case ZFFilterTypeInclude:
                s += ZFTOKEN_ZFFilterTypeInclude;
                return zftrue;
            case ZFFilterTypeExclude:
                s += ZFTOKEN_ZFFilterTypeExclude;
                return zftrue;
            default:
                zfCoreCriticalShouldNotGoHere();
                return zffalse;
        }
    })

// ============================================================
ZFTYPEID_DEFINE_BY_STRING_CONVERTER(ZFFilterCallbackResult, ZFFilterCallbackResult, {
        const zfchar *tokens[] = ZFM_EXPAND({
            ZFTOKEN_ZFFilterCallbackResultNotSpecified,
            ZFTOKEN_ZFFilterCallbackResultActive,
            ZFTOKEN_ZFFilterCallbackResultNotActive,
        });
        zfindex matched = zfsCheckMatch(tokens, ZFM_ARRAY_SIZE(tokens), src, srcLen);
        v = ZFFilterCallbackResultNotSpecified;
        switch(matched)
        {
            case 0:
                v = ZFFilterCallbackResultNotSpecified;
                return zftrue;
            case 1:
                v = ZFFilterCallbackResultActive;
                return zftrue;
            case 2:
                v = ZFFilterCallbackResultNotActive;
                return zftrue;
            default:
                return zffalse;
        }
    }, {
        switch(v)
        {
            case ZFFilterCallbackResultNotSpecified:
                s += ZFTOKEN_ZFFilterCallbackResultNotSpecified;
                return zftrue;
            case ZFFilterCallbackResultActive:
                s += ZFTOKEN_ZFFilterCallbackResultActive;
                return zftrue;
            case ZFFilterCallbackResultNotActive:
                s += ZFTOKEN_ZFFilterCallbackResultNotActive;
                return zftrue;
            default:
                zfCoreCriticalShouldNotGoHere();
                return zffalse;
        }
    })

// ============================================================
zfbool ZFPathInfoParse(ZF_IN const zfchar *pathInfo,
                       ZF_OUT zfstring &pathType,
                       ZF_OUT const zfchar *&pathData)
{
    pathData = pathInfo;
    while(*pathData != ZFSerializableKeyword_ZFPathInfo_separator[0] && *pathData != '\0') {++pathData;}
    if(*pathData != ZFSerializableKeyword_ZFPathInfo_separator[0])
    {
        return zffalse;
    }
    pathType.append(pathInfo, pathData - pathInfo);
    ++pathData;
    return zftrue;
}
ZFTYPEID_DEFINE_BY_STRING_CONVERTER(ZFPathInfo, ZFPathInfo, {
        if(srcLen == zfindexMax())
        {
            const zfchar *pathData = zfnull;
            if(!ZFPathInfoParse(src, v.pathType, pathData))
            {
                return zffalse;
            }
            v.pathData = pathData;
            return zftrue;
        }
        else
        {
            const zfchar *srcEnd = src + (srcLen == zfindexMax() ? zfslen(src) : srcLen);
            const zfchar *p = src;
            while(*p != ZFSerializableKeyword_ZFPathInfo_separator[0] && p < srcEnd) {++p;}
            if(*p != ZFSerializableKeyword_ZFPathInfo_separator[0]) {return zffalse;}
            v.pathType.assign(src, p - src);
            ++p;
            v.pathData.assign(p, srcEnd - p);
            return zftrue;
        }
    }, {
        s += v.pathType;
        s += ZFSerializableKeyword_ZFPathInfo_separator[0];
        s += v.pathData;
        return zftrue;
    })
ZFOBJECT_ON_INIT_USER_REGISTER_2({
        ZFPathInfo &zfv = invokerObject->to<v_ZFPathInfo *>()->zfv;
        zfv.pathType = pathType;
        zfv.pathData = pathData;
    }, v_ZFPathInfo
    , ZFMP_IN(const zfchar *, pathType)
    , ZFMP_IN(const zfchar *, pathData)
    )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFPathInfo, zfstring, pathType)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFPathInfo, zfstring, pathData)

// ============================================================
ZFTYPEID_DEFINE_BY_SERIALIZABLE_CONVERTER(ZFTokenForContainer, ZFTokenForContainer, {
        v.tokenLeft = ZFSerializableUtil::checkAttribute(serializableData, ZFSerializableKeyword_ZFTokenForContainer_tokenLeft);
        v.tokenRight = ZFSerializableUtil::checkAttribute(serializableData, ZFSerializableKeyword_ZFTokenForContainer_tokenRight);
        v.tokenSeparator = ZFSerializableUtil::checkAttribute(serializableData, ZFSerializableKeyword_ZFTokenForContainer_tokenSeparator);
        v.tokenValueLeft = ZFSerializableUtil::checkAttribute(serializableData, ZFSerializableKeyword_ZFTokenForContainer_tokenValueLeft);
        v.tokenValueRight = ZFSerializableUtil::checkAttribute(serializableData, ZFSerializableKeyword_ZFTokenForContainer_tokenValueRight);
        v.tokenEtc = ZFSerializableUtil::checkAttribute(serializableData, ZFSerializableKeyword_ZFTokenForContainer_tokenEtc);
        return zftrue;
    }, {
        if(!v.tokenLeft.isEmpty()) {serializableData.attributeForName(ZFSerializableKeyword_ZFTokenForContainer_tokenLeft, v.tokenLeft);}
        if(!v.tokenRight.isEmpty()) {serializableData.attributeForName(ZFSerializableKeyword_ZFTokenForContainer_tokenRight, v.tokenRight);}
        if(!v.tokenSeparator.isEmpty()) {serializableData.attributeForName(ZFSerializableKeyword_ZFTokenForContainer_tokenSeparator, v.tokenSeparator);}
        if(!v.tokenValueLeft.isEmpty()) {serializableData.attributeForName(ZFSerializableKeyword_ZFTokenForContainer_tokenValueLeft, v.tokenValueLeft);}
        if(!v.tokenValueRight.isEmpty()) {serializableData.attributeForName(ZFSerializableKeyword_ZFTokenForContainer_tokenValueRight, v.tokenValueRight);}
        if(!v.tokenEtc.isEmpty()) {serializableData.attributeForName(ZFSerializableKeyword_ZFTokenForContainer_tokenEtc, v.tokenEtc);}
        return zftrue;
    })
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFTokenForContainer, zfstring, tokenLeft)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFTokenForContainer, zfstring, tokenRight)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFTokenForContainer, zfstring, tokenSeparator)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFTokenForContainer, zfstring, tokenValueLeft)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFTokenForContainer, zfstring, tokenValueRight)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFTokenForContainer, zfstring, tokenEtc)

// ============================================================
ZFTYPEID_DEFINE_BY_SERIALIZABLE_CONVERTER(ZFTokenForKeyValueContainer, ZFTokenForKeyValueContainer, {
        v.tokenLeft = ZFSerializableUtil::checkAttribute(serializableData, ZFSerializableKeyword_ZFTokenForKeyValueContainer_tokenLeft);
        v.tokenRight = ZFSerializableUtil::checkAttribute(serializableData, ZFSerializableKeyword_ZFTokenForKeyValueContainer_tokenRight);
        v.tokenSeparator = ZFSerializableUtil::checkAttribute(serializableData, ZFSerializableKeyword_ZFTokenForKeyValueContainer_tokenSeparator);
        v.tokenPairLeft = ZFSerializableUtil::checkAttribute(serializableData, ZFSerializableKeyword_ZFTokenForKeyValueContainer_tokenPairLeft);
        v.tokenPairRight = ZFSerializableUtil::checkAttribute(serializableData, ZFSerializableKeyword_ZFTokenForKeyValueContainer_tokenPairRight);
        v.tokenPairSeparator = ZFSerializableUtil::checkAttribute(serializableData, ZFSerializableKeyword_ZFTokenForKeyValueContainer_tokenPairSeparator);
        v.tokenKeyLeft = ZFSerializableUtil::checkAttribute(serializableData, ZFSerializableKeyword_ZFTokenForKeyValueContainer_tokenKeyLeft);
        v.tokenKeyRight = ZFSerializableUtil::checkAttribute(serializableData, ZFSerializableKeyword_ZFTokenForKeyValueContainer_tokenKeyRight);
        v.tokenValueLeft = ZFSerializableUtil::checkAttribute(serializableData, ZFSerializableKeyword_ZFTokenForKeyValueContainer_tokenValueLeft);
        v.tokenValueRight = ZFSerializableUtil::checkAttribute(serializableData, ZFSerializableKeyword_ZFTokenForKeyValueContainer_tokenValueRight);
        v.tokenEtc = ZFSerializableUtil::checkAttribute(serializableData, ZFSerializableKeyword_ZFTokenForKeyValueContainer_tokenEtc);
        return zftrue;
    }, {
        if(!v.tokenLeft.isEmpty()) {serializableData.attributeForName(ZFSerializableKeyword_ZFTokenForKeyValueContainer_tokenLeft, v.tokenLeft);}
        if(!v.tokenRight.isEmpty()) {serializableData.attributeForName(ZFSerializableKeyword_ZFTokenForKeyValueContainer_tokenRight, v.tokenRight);}
        if(!v.tokenSeparator.isEmpty()) {serializableData.attributeForName(ZFSerializableKeyword_ZFTokenForKeyValueContainer_tokenSeparator, v.tokenSeparator);}
        if(!v.tokenPairLeft.isEmpty()) {serializableData.attributeForName(ZFSerializableKeyword_ZFTokenForKeyValueContainer_tokenPairLeft, v.tokenPairLeft);}
        if(!v.tokenPairRight.isEmpty()) {serializableData.attributeForName(ZFSerializableKeyword_ZFTokenForKeyValueContainer_tokenPairRight, v.tokenPairRight);}
        if(!v.tokenPairSeparator.isEmpty()) {serializableData.attributeForName(ZFSerializableKeyword_ZFTokenForKeyValueContainer_tokenPairSeparator, v.tokenPairSeparator);}
        if(!v.tokenKeyLeft.isEmpty()) {serializableData.attributeForName(ZFSerializableKeyword_ZFTokenForKeyValueContainer_tokenKeyLeft, v.tokenKeyLeft);}
        if(!v.tokenKeyRight.isEmpty()) {serializableData.attributeForName(ZFSerializableKeyword_ZFTokenForKeyValueContainer_tokenKeyRight, v.tokenKeyRight);}
        if(!v.tokenValueLeft.isEmpty()) {serializableData.attributeForName(ZFSerializableKeyword_ZFTokenForKeyValueContainer_tokenValueLeft, v.tokenValueLeft);}
        if(!v.tokenValueRight.isEmpty()) {serializableData.attributeForName(ZFSerializableKeyword_ZFTokenForKeyValueContainer_tokenValueRight, v.tokenValueRight);}
        if(!v.tokenEtc.isEmpty()) {serializableData.attributeForName(ZFSerializableKeyword_ZFTokenForKeyValueContainer_tokenEtc, v.tokenEtc);}
        return zftrue;
    })
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFTokenForKeyValueContainer, zfstring, tokenLeft)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFTokenForKeyValueContainer, zfstring, tokenRight)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFTokenForKeyValueContainer, zfstring, tokenSeparator)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFTokenForKeyValueContainer, zfstring, tokenPairLeft)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFTokenForKeyValueContainer, zfstring, tokenPairRight)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFTokenForKeyValueContainer, zfstring, tokenPairSeparator)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFTokenForKeyValueContainer, zfstring, tokenKeyLeft)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFTokenForKeyValueContainer, zfstring, tokenKeyRight)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFTokenForKeyValueContainer, zfstring, tokenValueLeft)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFTokenForKeyValueContainer, zfstring, tokenValueRight)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFTokenForKeyValueContainer, zfstring, tokenEtc)

// ============================================================
ZFTYPEID_DEFINE_BY_STRING_CONVERTER(ZFFilterForZFClassType, ZFFilterForZFClassType, {
        const zfchar *tokens[] = ZFM_EXPAND({
            ZFTOKEN_ZFFilterForZFClassTypeInclude,
            ZFTOKEN_ZFFilterForZFClassTypeExclude,
            ZFTOKEN_ZFFilterForZFClassTypeIncludeChildOf,
            ZFTOKEN_ZFFilterForZFClassTypeIncludeParentOf,
            ZFTOKEN_ZFFilterForZFClassTypeExcludeChildOf,
            ZFTOKEN_ZFFilterForZFClassTypeExcludeParentOf,
        });
        zfindex matched = zfsCheckMatch(tokens, ZFM_ARRAY_SIZE(tokens), src, srcLen);
        v = ZFFilterForZFClassTypeInclude;
        switch(matched)
        {
            case 0:
                v = ZFFilterForZFClassTypeInclude;
                return zftrue;
            case 1:
                v = ZFFilterForZFClassTypeExclude;
                return zftrue;
            case 2:
                v = ZFFilterForZFClassTypeIncludeChildOf;
                return zftrue;
            case 3:
                v = ZFFilterForZFClassTypeIncludeParentOf;
                return zftrue;
            case 4:
                v = ZFFilterForZFClassTypeExcludeChildOf;
                return zftrue;
            case 5:
                v = ZFFilterForZFClassTypeExcludeParentOf;
                return zftrue;
            default:
                return zffalse;
        }
    }, {
        switch(v)
        {
            case ZFFilterForZFClassTypeInclude:
                s += ZFTOKEN_ZFFilterForZFClassTypeInclude;
                return zftrue;
            case ZFFilterForZFClassTypeExclude:
                s += ZFTOKEN_ZFFilterForZFClassTypeExclude;
                return zftrue;
            case ZFFilterForZFClassTypeIncludeChildOf:
                s += ZFTOKEN_ZFFilterForZFClassTypeIncludeChildOf;
                return zftrue;
            case ZFFilterForZFClassTypeIncludeParentOf:
                s += ZFTOKEN_ZFFilterForZFClassTypeIncludeParentOf;
                return zftrue;
            case ZFFilterForZFClassTypeExcludeChildOf:
                s += ZFTOKEN_ZFFilterForZFClassTypeExcludeChildOf;
                return zftrue;
            case ZFFilterForZFClassTypeExcludeParentOf:
                s += ZFTOKEN_ZFFilterForZFClassTypeExcludeParentOf;
                return zftrue;
            default:
                zfCoreCriticalShouldNotGoHere();
                return zffalse;
        }
    })
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForZFClass, void, copyFrom, ZFMP_IN(ZFFilterForZFClass const &, ref))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFFilterForZFClass, void, filterAdd, ZFMP_IN(const ZFClass * const &, e), ZFMP_IN_OPT(ZFFilterForZFClassType, filterType, ZFFilterForZFClassTypeExclude))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFFilterForZFClass, void, filterRemove, ZFMP_IN(const ZFClass * const &, e), ZFMP_IN_OPT(ZFFilterForZFClassType, filterType, ZFFilterForZFClassTypeExclude))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForZFClass, void, filterRemoveAtIndex, ZFMP_IN(zfindex, index))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFFilterForZFClass, void, filterRemoveAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFFilterForZFClass, zfindex, filterCount)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForZFClass, const ZFClass *, filterElementAtIndex, ZFMP_IN(zfindex, index))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForZFClass, ZFFilterForZFClassType, filterTypeAtIndex, ZFMP_IN(zfindex, index))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForZFClass, zfbool, filterCheckActive, ZFMP_IN(const ZFClass * const &, e))

// ============================================================
ZFTYPEID_DEFINE_BY_STRING_CONVERTER(ZFMethodPrivilegeType, ZFMethodPrivilegeType, {
        const zfchar *tokens[] = ZFM_EXPAND({
            ZFTOKEN_ZFMethodPrivilegeTypePublic,
            ZFTOKEN_ZFMethodPrivilegeTypeProtected,
            ZFTOKEN_ZFMethodPrivilegeTypePrivate,
        });
        zfindex matched = zfsCheckMatch(tokens, ZFM_ARRAY_SIZE(tokens), src, srcLen);
        v = ZFMethodPrivilegeTypePublic;
        switch(matched)
        {
            case 0:
                v = ZFMethodPrivilegeTypePublic;
                return zftrue;
            case 1:
                v = ZFMethodPrivilegeTypeProtected;
                return zftrue;
            case 2:
                v = ZFMethodPrivilegeTypePrivate;
                return zftrue;
            default:
                return zffalse;
        }
    }, {
        switch(v)
        {
            case ZFMethodPrivilegeTypePublic:
                s += ZFTOKEN_ZFMethodPrivilegeTypePublic;
                return zftrue;
            case ZFMethodPrivilegeTypeProtected:
                s += ZFTOKEN_ZFMethodPrivilegeTypeProtected;
                return zftrue;
            case ZFMethodPrivilegeTypePrivate:
                s += ZFTOKEN_ZFMethodPrivilegeTypePrivate;
                return zftrue;
            default:
                zfCoreCriticalShouldNotGoHere();
                return zffalse;
        }
    })

// ============================================================
ZFTYPEID_DEFINE_BY_STRING_CONVERTER(ZFMethodType, ZFMethodType, {
        const zfchar *tokens[] = ZFM_EXPAND({
            ZFTOKEN_ZFMethodTypeNormal,
            ZFTOKEN_ZFMethodTypeStatic,
            ZFTOKEN_ZFMethodTypeVirtual,
        });
        zfindex matched = zfsCheckMatch(tokens, ZFM_ARRAY_SIZE(tokens), src, srcLen);
        v = ZFMethodTypeNormal;
        switch(matched)
        {
            case 0:
                v = ZFMethodTypeNormal;
                return zftrue;
            case 1:
                v = ZFMethodTypeStatic;
                return zftrue;
            case 2:
                v = ZFMethodTypeVirtual;
                return zftrue;
            default:
                return zffalse;
        }
    }, {
        switch(v)
        {
            case ZFMethodTypeNormal:
                s += ZFTOKEN_ZFMethodTypeNormal;
                return zftrue;
            case ZFMethodTypeStatic:
                s += ZFTOKEN_ZFMethodTypeStatic;
                return zftrue;
            case ZFMethodTypeVirtual:
                s += ZFTOKEN_ZFMethodTypeVirtual;
                return zftrue;
            default:
                zfCoreCriticalShouldNotGoHere();
                return zffalse;
        }
    })

// ============================================================
ZFTYPEID_ACCESS_ONLY_DEFINE(ZFMethodParamDefaultValueCallback, ZFMethodParamDefaultValueCallback)

// ============================================================
ZFTYPEID_ACCESS_ONLY_DEFINE(ZFMethodDynamicRegisterParam, ZFMethodDynamicRegisterParam)

ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFMethodDynamicRegisterParam, void, methodDynamicRegisterUserData, ZFMP_IN(ZFObject *, methodDynamicRegisterUserData))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFMethodDynamicRegisterParam, ZFObject *, methodDynamicRegisterUserData)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFMethodDynamicRegisterParam, void, methodOwnerClass, ZFMP_IN(const ZFClass *, methodOwnerClass))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFMethodDynamicRegisterParam, const ZFClass *, methodOwnerClass)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFMethodDynamicRegisterParam, void, methodNamespace, ZFMP_IN(const zfchar *, methodNamespace))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFMethodDynamicRegisterParam, const zfchar *, methodNamespace)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFMethodDynamicRegisterParam, void, methodGenericInvoker, ZFMP_IN(ZFMethodGenericInvoker, methodGenericInvoker))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFMethodDynamicRegisterParam, ZFMethodGenericInvoker, methodGenericInvoker)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFMethodDynamicRegisterParam, void, methodImpl, ZFMP_IN(const ZFListener &, methodImpl))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFMethodDynamicRegisterParam, const ZFListener &, methodImpl)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFMethodDynamicRegisterParam, void, methodImplUserData, ZFMP_IN(ZFObject *, methodImplUserData))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFMethodDynamicRegisterParam, ZFObject *, methodImplUserData)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFMethodDynamicRegisterParam, void, methodType, ZFMP_IN(ZFMethodType, methodType))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFMethodDynamicRegisterParam, ZFMethodType, methodType)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFMethodDynamicRegisterParam, void, methodPrivilegeType, ZFMP_IN(ZFMethodPrivilegeType, methodPrivilegeType))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFMethodDynamicRegisterParam, ZFMethodPrivilegeType, methodPrivilegeType)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFMethodDynamicRegisterParam, void, methodName, ZFMP_IN(const zfchar *, methodName))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFMethodDynamicRegisterParam, const zfchar *, methodName)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFMethodDynamicRegisterParam, void, methodReturnTypeId, ZFMP_IN(const zfchar *, methodReturnTypeId))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFMethodDynamicRegisterParam, const zfchar *, methodReturnTypeId)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFMethodDynamicRegisterParam, void, methodReturnTypeName, ZFMP_IN(const zfchar *, methodReturnTypeName))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFMethodDynamicRegisterParam, const zfchar *, methodReturnTypeName)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_3(v_ZFMethodDynamicRegisterParam, void, methodParamAdd, ZFMP_IN(const zfchar *, methodParamTypeId), ZFMP_IN_OPT(const zfchar *, methodParamTypeName, zfnull), ZFMP_IN_OPT(ZFMethodParamDefaultValueCallback, methodParamDefaultValueCallback, zfnull))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFMethodDynamicRegisterParam, zfindex, methodParamCount)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFMethodDynamicRegisterParam, const zfchar *, methodParamTypeIdAtIndex, ZFMP_IN(zfindex, index))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFMethodDynamicRegisterParam, const zfchar *, methodParamTypeNameAtIndex, ZFMP_IN(zfindex, index))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFMethodDynamicRegisterParam, ZFMethodParamDefaultValueCallback, methodParamDefaultValueCallbackAtIndex, ZFMP_IN(zfindex, index))

// ============================================================
ZFTYPEID_ACCESS_ONLY_DEFINE(ZFPropertyDynamicRegisterInitValueCallback, ZFPropertyDynamicRegisterInitValueCallback)
ZFTYPEID_ACCESS_ONLY_DEFINE(ZFPropertyDynamicRegisterParam, ZFPropertyDynamicRegisterParam)

ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFPropertyDynamicRegisterParam, void, propertyDynamicRegisterUserData, ZFMP_IN(ZFObject *, propertyDynamicRegisterUserData))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFPropertyDynamicRegisterParam, ZFObject *, propertyDynamicRegisterUserData)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFPropertyDynamicRegisterParam, void, propertyOwnerClass, ZFMP_IN(const ZFClass *, propertyOwnerClass))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFPropertyDynamicRegisterParam, const ZFClass *, propertyOwnerClass)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFPropertyDynamicRegisterParam, void, propertyTypeId, ZFMP_IN(const zfchar *, propertyTypeId))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFPropertyDynamicRegisterParam, const zfchar *, propertyTypeId)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFPropertyDynamicRegisterParam, void, propertyTypeName, ZFMP_IN(const zfchar *, propertyTypeName))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFPropertyDynamicRegisterParam, const zfchar *, propertyTypeName)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFPropertyDynamicRegisterParam, void, propertyName, ZFMP_IN(const zfchar *, propertyName))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFPropertyDynamicRegisterParam, const zfchar *, propertyName)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFPropertyDynamicRegisterParam, void, propertyClassOfRetainProperty, ZFMP_IN(const ZFClass *, propertyClassOfRetainProperty))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFPropertyDynamicRegisterParam, const ZFClass *, propertyClassOfRetainProperty)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFPropertyDynamicRegisterParam, void, propertyInitValueCallback, ZFMP_IN(ZFPropertyDynamicRegisterInitValueCallback, propertyInitValueCallback))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFPropertyDynamicRegisterParam, ZFPropertyDynamicRegisterInitValueCallback, propertyInitValueCallback)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFPropertyDynamicRegisterParam, void, propertySetterType, ZFMP_IN(ZFMethodPrivilegeType, propertySetterType))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFPropertyDynamicRegisterParam, ZFMethodPrivilegeType, propertySetterType)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFPropertyDynamicRegisterParam, void, propertyGetterType, ZFMP_IN(ZFMethodPrivilegeType, propertyGetterType))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFPropertyDynamicRegisterParam, ZFMethodPrivilegeType, propertyGetterType)

// ============================================================
ZFTYPEID_ACCESS_ONLY_DEFINE(ZFComparer_ZFObject, ZFComparer<ZFObject *>::Comparer)

// ============================================================
ZFTYPEID_DEFINE_BY_STRING_CONVERTER(ZFObjectInstanceState, ZFObjectInstanceState, {
        const zfchar *tokens[] = ZFM_EXPAND({
            ZFTOKEN_ZFObjectInstanceStateOnInit,
            ZFTOKEN_ZFObjectInstanceStateOnInitFinish,
            ZFTOKEN_ZFObjectInstanceStateIdle,
            ZFTOKEN_ZFObjectInstanceStateOnDeallocPrepare,
            ZFTOKEN_ZFObjectInstanceStateOnDealloc,
        });
        zfindex matched = zfsCheckMatch(tokens, ZFM_ARRAY_SIZE(tokens), src, srcLen);
        v = ZFObjectInstanceStateOnInit;
        switch(matched)
        {
            case 0:
                v = ZFObjectInstanceStateOnInit;
                return zftrue;
            case 1:
                v = ZFObjectInstanceStateOnInitFinish;
                return zftrue;
            case 2:
                v = ZFObjectInstanceStateIdle;
                return zftrue;
            case 3:
                v = ZFObjectInstanceStateOnDeallocPrepare;
                return zftrue;
            case 4:
                v = ZFObjectInstanceStateOnDealloc;
                return zftrue;
            default:
                return zffalse;
        }
    }, {
        switch(v)
        {
            case ZFObjectInstanceStateOnInit:
                s += ZFTOKEN_ZFObjectInstanceStateOnInit;
                return zftrue;
            case ZFObjectInstanceStateOnInitFinish:
                s += ZFTOKEN_ZFObjectInstanceStateOnInitFinish;
                return zftrue;
            case ZFObjectInstanceStateIdle:
                s += ZFTOKEN_ZFObjectInstanceStateIdle;
                return zftrue;
            case ZFObjectInstanceStateOnDeallocPrepare:
                s += ZFTOKEN_ZFObjectInstanceStateOnDeallocPrepare;
                return zftrue;
            case ZFObjectInstanceStateOnDealloc:
                s += ZFTOKEN_ZFObjectInstanceStateOnDealloc;
                return zftrue;
            default:
                zfCoreCriticalShouldNotGoHere();
                return zffalse;
        }
    })

// ============================================================
ZFTYPEID_DEFINE_BY_STRING_CONVERTER(ZFCallbackType, ZFCallbackType, {
        const zfchar *tokens[] = ZFM_EXPAND({
            ZFTOKEN_ZFCallbackTypeDummy,
            ZFTOKEN_ZFCallbackTypeMethod,
            ZFTOKEN_ZFCallbackTypeMemberMethod,
            ZFTOKEN_ZFCallbackTypeRawFunction,
            ZFTOKEN_ZFCallbackTypeLambda,
        });
        zfindex matched = zfsCheckMatch(tokens, ZFM_ARRAY_SIZE(tokens), src, srcLen);
        v = ZFCallbackTypeDummy;
        switch(matched)
        {
            case 0:
                v = ZFCallbackTypeDummy;
                return zftrue;
            case 1:
                v = ZFCallbackTypeMethod;
                return zftrue;
            case 2:
                v = ZFCallbackTypeMemberMethod;
                return zftrue;
            case 3:
                v = ZFCallbackTypeRawFunction;
                return zftrue;
            default:
                return zffalse;
        }
    }, {
        switch(v)
        {
            case ZFCallbackTypeDummy:
                s += ZFTOKEN_ZFCallbackTypeDummy;
                return zftrue;
            case ZFCallbackTypeMethod:
                s += ZFTOKEN_ZFCallbackTypeMethod;
                return zftrue;
            case ZFCallbackTypeMemberMethod:
                s += ZFTOKEN_ZFCallbackTypeMemberMethod;
                return zftrue;
            case ZFCallbackTypeRawFunction:
                s += ZFTOKEN_ZFCallbackTypeRawFunction;
                return zftrue;
            case ZFCallbackTypeLambda:
                s += ZFTOKEN_ZFCallbackTypeLambda;
                return zftrue;
            default:
                zfCoreCriticalShouldNotGoHere();
                return zffalse;
        }
    })

// ============================================================
ZFTYPEID_DEFINE(ZFSerializableData, ZFSerializableData, {
        v = serializableData;
        return zftrue;
    }, {
        serializableData = v;
        return zftrue;
    }, {
        return ZFSerializableDataFromZfsd(v, src, srcLen);
    }, {
        return ZFSerializableDataToZfsd(s, v, zfnull, zffalse);
    })

ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, void, copyFrom, ZFMP_IN(const ZFSerializableData &, ref))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, ZFSerializableData, copy)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, const ZFPathInfo *, pathInfo)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, void, pathInfo, ZFMP_IN(const ZFPathInfo *, pathInfo))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFSerializableData, void, pathInfo, ZFMP_IN(const zfchar *, pathType), ZFMP_IN(const zfchar *, pathData))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, const ZFPathInfo *, pathInfoCheck)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, zfbool, serializableDataParent, ZFMP_OUT(ZFSerializableData &, ret))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, void, itemClass, ZFMP_IN(const zfchar *, classNameFull))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, const zfchar *, itemClass)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, void, propertyName, ZFMP_IN(const zfchar *, name))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, const zfchar *, propertyName)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, void, propertyValue, ZFMP_IN(const zfchar *, value))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, const zfchar *, propertyValue)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, void, category, ZFMP_IN(const zfchar *, category))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, const zfchar *, category)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, void, removeAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFSerializableData, void, serializableDataTag, ZFMP_IN(const zfchar *, key), ZFMP_IN(ZFObject *, tag))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, ZFObject *, serializableDataTag, ZFMP_IN(const zfchar *, key))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFSerializableData, void, serializableDataTagGetAllKeyValue, ZFMP_IN_OUT(ZFCoreArray<const zfchar *> &, allKey), ZFMP_IN_OUT(ZFCoreArray<ZFObject *> &, allValue))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, void, serializableDataTagRemove, ZFMP_IN(const zfchar *, key))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, zfautoObject, serializableDataTagRemoveAndGet, ZFMP_IN(const zfchar *, key))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, void, serializableDataTagRemoveAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFSerializableData, void, attributeForName, ZFMP_IN(const zfchar *, name), ZFMP_IN(const zfchar *, value))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, const zfchar *, attributeForName, ZFMP_IN(const zfchar *, name))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, zfindex, attributeCount)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, void, attributeRemove, ZFMP_IN(const zfchar *, name))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, void, attributeRemoveAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, zfiterator, attributeIteratorForName, ZFMP_IN(const zfchar *, name))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, zfiterator, attributeIterator)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, zfbool, attributeIteratorIsValid, ZFMP_IN(const zfiterator &, it))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFSerializableData, zfbool, attributeIteratorIsEqual, ZFMP_IN(const zfiterator &, it0), ZFMP_IN(const zfiterator &, it1))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFSerializableData, void, attributeIteratorValue, ZFMP_IN_OUT(zfiterator &, it), ZFMP_IN(const zfchar *, value))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, void, attributeIteratorRemove, ZFMP_IN_OUT(zfiterator &, it))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, const zfchar *, attributeIteratorKey, ZFMP_IN(const zfiterator &, it))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, const zfchar *, attributeIteratorValue, ZFMP_IN(const zfiterator &, it))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, const zfchar *, attributeIteratorNextKey, ZFMP_IN_OUT(zfiterator &, it))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, const zfchar *, attributeIteratorNextValue, ZFMP_IN_OUT(zfiterator &, it))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, zfbool, attributeIteratorResolved, ZFMP_IN(const zfiterator &, it))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, void, attributeIteratorResolveMark, ZFMP_IN(const zfiterator &, it))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, void, attributeIteratorResolveUnmark, ZFMP_IN(const zfiterator &, it))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, void, elementAdd, ZFMP_IN(const ZFSerializableData &, element))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFSerializableData, void, elementAdd, ZFMP_IN(const ZFSerializableData &, element), ZFMP_IN(zfindex, atIndex))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFSerializableData, void, elementSetAtIndex, ZFMP_IN(zfindex, index), ZFMP_IN(const ZFSerializableData &, element))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, zfindex, elementFindByName, ZFMP_IN(const zfchar *, name))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, zfindex, elementFindByCategory, ZFMP_IN(const zfchar *, name))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, zfindex, elementCount)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, const ZFSerializableData &, elementAtIndex, ZFMP_IN(zfindex, index))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, void, elementRemoveAtIndex, ZFMP_IN(zfindex, index))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, void, elementRemoveAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, zfbool, resolved)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, void, resolveMark)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, void, resolveUnmark)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, zfbool, resolvedAttribute, ZFMP_IN(const zfchar *, name))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, void, resolveAttributeMark, ZFMP_IN(const zfchar *, name))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, void, resolveAttributeUnmark, ZFMP_IN(const zfchar *, name))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, zfbool, resolvedPropertyName)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, void, resolvePropertyNameMark)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, void, resolvePropertyNameUnmark)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, zfbool, resolvedPropertyValue)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, void, resolvePropertyValueMark)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, void, resolvePropertyValueUnmark)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, zfbool, resolvedCategory)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, void, resolveCategoryMark)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, void, resolveCategoryUnmark)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, void, resolveMarkAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, void, resolveUnmarkAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, void, resolveAttributeMarkAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, void, resolveAttributeUnmarkAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, zfbool, isEmpty)

ZFOUTPUT_TYPE_DEFINE(ZFSerializableData, {
        output.execute(v.objectInfo());
    })

// ============================================================
ZFTYPEID_ACCESS_ONLY_DEFINE(ZFFuncAddrType, ZFFuncAddrType)
ZFTYPEID_ACCESS_ONLY_DEFINE_UNCOMPARABLE(zfiterator, zfiterator)

ZFTYPEID_ACCESS_ONLY_DEFINE(ZFBuffer, ZFBuffer)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFBuffer, zfbool, bufferMalloc, ZFMP_IN(zfindex, bufferSize))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFBuffer, zfbool, bufferRealloc, ZFMP_IN(zfindex, bufferSize))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFBuffer, void, bufferFree)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFBuffer, void *, bufferGiveUp)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFBuffer, void, bufferSize, ZFMP_IN(zfindex, bufferSize))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_3(v_ZFBuffer, void, bufferChange, ZFMP_IN(void *, buffer), ZFMP_IN(zfindex, bufferSize), ZFMP_IN(zfbool, bufferAutoFree))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFBuffer, zfbool, bufferCopy, ZFMP_IN(const void *, buffer), ZFMP_IN(zfindex, bufferSize))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFBuffer, zfbool, bufferCopy, ZFMP_IN(const zfchar *, s), ZFMP_IN_OPT(zfindex, length, zfindexMax()))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFBuffer, zfbool, bufferCopy, ZFMP_IN(const zfstring &, s))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFBuffer, void *, buffer)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFBuffer, const zfchar *, bufferAsString)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFBuffer, zfindex, bufferAsStringLength)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFBuffer, zfindex, bufferSize)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFBuffer, zfbool, bufferAutoFree)

ZFTYPEID_ACCESS_ONLY_DEFINE(ZFFilterForNumber, ZFFilterForNumber)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForNumber, void, copyFrom, ZFMP_IN(ZFFilterForNumber const &, ref))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFFilterForNumber, void, filterAdd, ZFMP_IN(zfint const &, e), ZFMP_IN_OPT(ZFFilterType, filterType, ZFFilterTypeExclude))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFFilterForNumber, void, filterRemove, ZFMP_IN(zfint const &, e), ZFMP_IN_OPT(ZFFilterType, filterType, ZFFilterTypeExclude))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForNumber, void, filterRemoveAtIndex, ZFMP_IN(zfindex, index))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFFilterForNumber, void, filterRemoveAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFFilterForNumber, zfindex, filterCount)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForNumber, zfint, filterElementAtIndex, ZFMP_IN(zfindex, index))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForNumber, ZFFilterType, filterTypeAtIndex, ZFMP_IN(zfindex, index))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForNumber, zfbool, filterCheckActive, ZFMP_IN(zfint const &, e))

ZFTYPEID_ACCESS_ONLY_DEFINE(ZFFilterForIndex, ZFFilterForIndex)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForIndex, void, copyFrom, ZFMP_IN(ZFFilterForIndex const &, ref))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFFilterForIndex, void, filterAdd, ZFMP_IN(zfindex const &, e), ZFMP_IN_OPT(ZFFilterType, filterType, ZFFilterTypeExclude))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFFilterForIndex, void, filterRemove, ZFMP_IN(zfindex const &, e), ZFMP_IN_OPT(ZFFilterType, filterType, ZFFilterTypeExclude))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForIndex, void, filterRemoveAtIndex, ZFMP_IN(zfindex, index))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFFilterForIndex, void, filterRemoveAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFFilterForIndex, zfindex, filterCount)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForIndex, zfindex, filterElementAtIndex, ZFMP_IN(zfindex, index))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForIndex, ZFFilterType, filterTypeAtIndex, ZFMP_IN(zfindex, index))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForIndex, zfbool, filterCheckActive, ZFMP_IN(zfindex const &, e))

ZFTYPEID_ACCESS_ONLY_DEFINE(ZFFilterForIdentity, ZFFilterForIdentity)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForIdentity, void, copyFrom, ZFMP_IN(ZFFilterForIdentity const &, ref))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFFilterForIdentity, void, filterAdd, ZFMP_IN(zfidentity const &, e), ZFMP_IN_OPT(ZFFilterType, filterType, ZFFilterTypeExclude))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFFilterForIdentity, void, filterRemove, ZFMP_IN(zfidentity const &, e), ZFMP_IN_OPT(ZFFilterType, filterType, ZFFilterTypeExclude))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForIdentity, void, filterRemoveAtIndex, ZFMP_IN(zfindex, index))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFFilterForIdentity, void, filterRemoveAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFFilterForIdentity, zfindex, filterCount)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForIdentity, zfidentity, filterElementAtIndex, ZFMP_IN(zfindex, index))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForIdentity, ZFFilterType, filterTypeAtIndex, ZFMP_IN(zfindex, index))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForIdentity, zfbool, filterCheckActive, ZFMP_IN(zfidentity const &, e))

ZFTYPEID_ACCESS_ONLY_DEFINE(ZFFilterForString, ZFFilterForString)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForString, void, copyFrom, ZFMP_IN(ZFFilterForString const &, ref))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFFilterForString, void, filterAdd, ZFMP_IN(const zfchar *, e), ZFMP_IN_OPT(ZFFilterType, filterType, ZFFilterTypeExclude))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFFilterForString, void, filterRemove, ZFMP_IN(const zfchar *, e), ZFMP_IN_OPT(ZFFilterType, filterType, ZFFilterTypeExclude))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForString, void, filterRemoveAtIndex, ZFMP_IN(zfindex, index))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFFilterForString, void, filterRemoveAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFFilterForString, zfindex, filterCount)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForString, const zfchar *, filterElementAtIndex, ZFMP_IN(zfindex, index))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForString, ZFFilterType, filterTypeAtIndex, ZFMP_IN(zfindex, index))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForString, zfbool, filterCheckActive, ZFMP_IN(const zfchar *, e))

ZFTYPEID_ACCESS_ONLY_DEFINE(ZFIdentityGenerator, ZFIdentityGenerator)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFIdentityGenerator, zfidentity, idAcquire)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFIdentityGenerator, void, idRelease, ZFMP_IN(zfidentity, identity))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFIdentityGenerator, zfbool, idUsed, ZFMP_IN(zfidentity, identity))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFIdentityGenerator, void, idUsedGetAll, ZFMP_IN_OUT(ZFCoreArray<zfidentity> &, ret))

ZFTYPEID_ACCESS_ONLY_DEFINE(ZFListenerData, ZFListenerData)
ZFOBJECT_ON_INIT_USER_REGISTER_4({
        invokerObject->to<v_ZFListenerData *>()->zfv
            .eventId(eventId)
            .sender(sender)
            .param0(param0)
            .param1(param1)
            ;
    }, v_ZFListenerData
    , ZFMP_IN(zfidentity, eventId)
    , ZFMP_IN(ZFObject *, sender)
    , ZFMP_IN_OPT(ZFObject *, param0, zfnull)
    , ZFMP_IN_OPT(ZFObject *, param1, zfnull)
    )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_SETTER_GETTER(v_ZFListenerData, zfidentity, eventId)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_SETTER_GETTER(v_ZFListenerData, ZFObject *, sender)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_SETTER_GETTER(v_ZFListenerData, ZFObject *, param0)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_SETTER_GETTER(v_ZFListenerData, ZFObject *, param1)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFListenerData, void, eventFiltered, ZFMP_IN(zfbool, eventFiltered))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFListenerData, zfbool, eventFiltered)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFListenerData, ZFListenerData &, eventFilterEnable)

ZFTYPEID_ACCESS_ONLY_DEFINE(ZFObserverAddParam, ZFObserverAddParam)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_SETTER_GETTER(v_ZFObserverAddParam, zfidentity, eventId)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_SETTER_GETTER(v_ZFObserverAddParam, ZFListener, observer)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_SETTER_GETTER(v_ZFObserverAddParam, ZFObject *, userData)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_SETTER_GETTER(v_ZFObserverAddParam, ZFObject *, owner)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_SETTER_GETTER(v_ZFObserverAddParam, zfbool, autoRemoveAfterActivate)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_SETTER_GETTER(v_ZFObserverAddParam, ZFLevel, observerLevel)

ZFTYPEID_ACCESS_ONLY_DEFINE(ZFObserverHolder, ZFObserverHolder)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_6(v_ZFObserverHolder, zfidentity, observerAdd, ZFMP_IN(zfidentity, eventId), ZFMP_IN(const ZFListener &, observer), ZFMP_IN_OPT(ZFObject *, userData, zfnull), ZFMP_IN_OPT(ZFObject *, owner, zfnull), ZFMP_IN_OPT(zfbool, autoRemoveAfterActivate, zffalse), ZFMP_IN_OPT(ZFLevel, observerLevel, ZFLevelAppNormal))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFObserverHolder, zfidentity, observerAdd, ZFMP_IN(const ZFObserverAddParam &, param))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_5(v_ZFObserverHolder, zfidentity, observerAddForOnce, ZFMP_IN(zfidentity, eventId), ZFMP_IN(const ZFListener &, observer), ZFMP_IN_OPT(ZFObject *, userData, zfnull), ZFMP_IN_OPT(ZFObject *, owner, zfnull), ZFMP_IN_OPT(ZFLevel, observerLevel, ZFLevelAppNormal))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFObserverHolder, void, observerMoveToFirst, ZFMP_IN(zfidentity, taskId))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFObserverHolder, void, observerRemove, ZFMP_IN(zfidentity, eventId), ZFMP_IN(const ZFListener &, callback))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_4(v_ZFObserverHolder, void, observerRemove, ZFMP_IN(zfidentity, eventId), ZFMP_IN(const ZFListener &, callback), ZFMP_IN(ZFObject *, userData), ZFMP_IN_OPT(ZFComparer<ZFObject *>::Comparer, userDataComparer, ZFComparerCheckEqual))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFObserverHolder, void, observerRemoveByTaskId, ZFMP_IN(zfidentity, taskId))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFObserverHolder, void, observerRemoveByOwner, ZFMP_IN(ZFObject *, owner))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFObserverHolder, void, observerRemoveAll, ZFMP_IN(zfidentity, eventId))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFObserverHolder, void, observerRemoveAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFObserverHolder, zfbool, observerHasAdd)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFObserverHolder, zfbool, observerHasAdd, ZFMP_IN(zfidentity, eventId))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_3(v_ZFObserverHolder, void, observerNotify, ZFMP_IN(zfidentity, eventId), ZFMP_IN_OPT(ZFObject *, param0, zfnull), ZFMP_IN_OPT(ZFObject *, param1, zfnull))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_4(v_ZFObserverHolder, void, observerNotifyWithCustomSender, ZFMP_IN(ZFObject *, customSender), ZFMP_IN(zfidentity, eventId), ZFMP_IN_OPT(ZFObject *, param0, zfnull), ZFMP_IN_OPT(ZFObject *, param1, zfnull))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFObserverHolder, ZFObject *, observerOwner)

ZFTYPEID_ACCESS_ONLY_DEFINE(ZFFilterForZFClass, ZFFilterForZFClass)

ZFTYPEID_ACCESS_ONLY_DEFINE(ZFFilterForZFMethod, ZFFilterForZFMethod)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForZFMethod, void, copyFrom, ZFMP_IN(ZFFilterForZFMethod const &, ref))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFFilterForZFMethod, void, filterAdd, ZFMP_IN(const ZFMethod * const &, e), ZFMP_IN_OPT(ZFFilterType, filterType, ZFFilterTypeExclude))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFFilterForZFMethod, void, filterRemove, ZFMP_IN(const ZFMethod * const &, e), ZFMP_IN_OPT(ZFFilterType, filterType, ZFFilterTypeExclude))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForZFMethod, void, filterRemoveAtIndex, ZFMP_IN(zfindex, index))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFFilterForZFMethod, void, filterRemoveAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFFilterForZFMethod, zfindex, filterCount)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForZFMethod, const ZFMethod *, filterElementAtIndex, ZFMP_IN(zfindex, index))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForZFMethod, ZFFilterType, filterTypeAtIndex, ZFMP_IN(zfindex, index))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForZFMethod, zfbool, filterCheckActive, ZFMP_IN(const ZFMethod * const &, e))

ZFTYPEID_ACCESS_ONLY_DEFINE(ZFFilterForZFProperty, ZFFilterForZFProperty)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForZFProperty, void, copyFrom, ZFMP_IN(ZFFilterForZFProperty const &, ref))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFFilterForZFProperty, void, filterAdd, ZFMP_IN(const ZFProperty * const &, e), ZFMP_IN_OPT(ZFFilterType, filterType, ZFFilterTypeExclude))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFFilterForZFProperty, void, filterRemove, ZFMP_IN(const ZFProperty * const &, e), ZFMP_IN_OPT(ZFFilterType, filterType, ZFFilterTypeExclude))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForZFProperty, void, filterRemoveAtIndex, ZFMP_IN(zfindex, index))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFFilterForZFProperty, void, filterRemoveAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFFilterForZFProperty, zfindex, filterCount)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForZFProperty, const ZFProperty *, filterElementAtIndex, ZFMP_IN(zfindex, index))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForZFProperty, ZFFilterType, filterTypeAtIndex, ZFMP_IN(zfindex, index))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForZFProperty, zfbool, filterCheckActive, ZFMP_IN(const ZFProperty * const &, e))

ZFTYPEID_ACCESS_ONLY_DEFINE(ZFFilterForZFObject, ZFFilterForZFObject)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForZFObject, void, copyFrom, ZFMP_IN(ZFFilterForZFObject const &, ref))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFFilterForZFObject, void, filterAdd, ZFMP_IN(ZFObject * const &, e), ZFMP_IN_OPT(ZFFilterType, filterType, ZFFilterTypeExclude))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFFilterForZFObject, void, filterRemove, ZFMP_IN(ZFObject * const &, e), ZFMP_IN_OPT(ZFFilterType, filterType, ZFFilterTypeExclude))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForZFObject, void, filterRemoveAtIndex, ZFMP_IN(zfindex, index))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFFilterForZFObject, void, filterRemoveAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFFilterForZFObject, zfindex, filterCount)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForZFObject, ZFObject *, filterElementAtIndex, ZFMP_IN(zfindex, index))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForZFObject, ZFFilterType, filterTypeAtIndex, ZFMP_IN(zfindex, index))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFFilterForZFObject, zfbool, filterCheckActive, ZFMP_IN(ZFObject * const &, e))

ZFTYPEID_ACCESS_ONLY_DEFINE(ZFMethodGenericInvoker, ZFMethodGenericInvoker)

ZFTYPEID_ACCESS_ONLY_DEFINE(ZFPtr, void *)
ZFTYPEID_ACCESS_ONLY_DEFINE(ZFPtrConst, const void *)

ZF_NAMESPACE_GLOBAL_END

