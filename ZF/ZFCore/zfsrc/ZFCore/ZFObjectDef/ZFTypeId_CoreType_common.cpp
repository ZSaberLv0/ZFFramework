#include "ZFTypeId_CoreType.h"
#include "ZFObjectImpl.h"
#include "ZFSerializableDataSerializableConverter.h"

ZF_NAMESPACE_GLOBAL_BEGIN

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
ZFTYPEID_ACCESS_ONLY_DEFINE(ZFPtr, void *)
ZFTYPEID_ACCESS_ONLY_DEFINE(ZFPtrConst, const void *)

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
ZFTYPEID_ACCESS_ONLY_DEFINE(ZFFuncAddrType, ZFFuncAddrType)

// ============================================================
ZFTYPEID_ACCESS_ONLY_DEFINE_UNCOMPARABLE(zfiterator, zfiterator)

// ============================================================
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

// ============================================================
ZFTYPEID_ACCESS_ONLY_DEFINE(ZFIdentityGenerator, ZFIdentityGenerator)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFIdentityGenerator, zfidentity, idAcquire)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFIdentityGenerator, void, idRelease, ZFMP_IN(zfidentity, identity))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFIdentityGenerator, zfbool, idUsed, ZFMP_IN(zfidentity, identity))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFIdentityGenerator, void, idUsedGetAll, ZFMP_IN_OUT(ZFCoreArray<zfidentity> &, ret))

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

// ============================================================
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

// ============================================================
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

// ============================================================
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

ZF_NAMESPACE_GLOBAL_END

