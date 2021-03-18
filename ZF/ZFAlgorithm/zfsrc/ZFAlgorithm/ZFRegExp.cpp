#include "ZFRegExp.h"
#include "protocol/ZFProtocolZFRegExp.h"

ZF_NAMESPACE_GLOBAL_BEGIN

void ZFRegExpResult::objectInfoT(ZF_IN_OUT zfstring &ret) const
{
    ret += ZFTOKEN_ZFObjectInfoLeft;
    if(this->matched)
    {
        ret += "matched";
        if(this->matchedRange != ZFIndexRangeZero())
        {
            ret += " in ";
            ZFIndexRangeToString(ret, this->matchedRange);
        }
        if(!this->namedGroups.isEmpty())
        {
            ret += ", named groups: ";
            for(zfindex i = 0; i < this->namedGroups.count(); ++i)
            {
                if(i != 0)
                {
                    ret += ", ";
                }
                ZFIndexRangeToString(ret, this->namedGroups[i]);
            }
        }
    }
    else
    {
        ret += "no match";
    }
    ret += ZFTOKEN_ZFObjectInfoRight;
}

ZFTYPEID_DEFINE_BY_SERIALIZABLE_CONVERTER(ZFRegExpResult, ZFRegExpResult, {
        if(ZFSerializableUtil::requireItemClass(serializableData, ZFTypeId_ZFRegExpResult(), outErrorHint, outErrorPos) == zfnull)
        {
            return zffalse;
        }

        const ZFSerializableData *element = zfnull;

        v.matched = zffalse;
        element = ZFSerializableUtil::checkElementByName(serializableData, ZFSerializableKeyword_ZFRegExpResult_matched);
        if(element != zfnull && !zfboolFromData(v.matched, *element, outErrorHint, outErrorPos))
        {
            return zffalse;
        }

        v.matchedRange = ZFIndexRangeZero();
        element = ZFSerializableUtil::checkElementByName(serializableData, ZFSerializableKeyword_ZFRegExpResult_matchedRange);
        if(element != zfnull && !ZFIndexRangeFromData(v.matchedRange, serializableData, outErrorHint, outErrorPos))
        {
            return zffalse;
        }

        v.namedGroups.removeAll();
        element = ZFSerializableUtil::checkElementByName(serializableData, ZFSerializableKeyword_ZFRegExpResult_namedGroups);
        if(element != zfnull && !ZFCoreArrayFromData(
            v.namedGroups,
            ZFIndexRangeFromData,
            serializableData,
            outErrorHint,
            outErrorPos))
        {
            return zffalse;
        }
        return zftrue;
    }, {
        serializableData.itemClass(ZFTypeId_ZFRegExpResult());

        if(v.matched)
        {
            ZFSerializableData element;
            if(!zfboolToData(element, v.matched, outErrorHint))
            {
                return zffalse;
            }
            element.propertyName(ZFSerializableKeyword_ZFRegExpResult_matched);
            serializableData.elementAdd(element);
        }

        if(v.matchedRange != ZFIndexRangeZero())
        {
            ZFSerializableData element;
            if(!ZFIndexRangeToData(element, v.matchedRange, outErrorHint))
            {
                return zffalse;
            }
            element.propertyName(ZFSerializableKeyword_ZFRegExpResult_matchedRange);
            serializableData.elementAdd(element);
        }

        if(!v.namedGroups.isEmpty())
        {
            ZFSerializableData element;
            if(!ZFCoreArrayToData(element, ZFIndexRangeToData, v.namedGroups, outErrorHint))
            {
                return zffalse;
            }
            element.propertyName(ZFSerializableKeyword_ZFRegExpResult_namedGroups);
            serializableData.elementAdd(element);
        }

        return zftrue;
    })
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFRegExpResult, zfbool, matched)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFRegExpResult, ZFIndexRange, matchedRange)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFRegExpResult, ZFCoreArrayPOD<ZFIndexRange>, namedGroups)

// ============================================================
// global
ZFENUM_DEFINE_FLAGS(ZFRegExpOption, ZFRegExpOptionFlags)

// ============================================================
// _ZFP_ZFRegExpPrivate
zfclassNotPOD _ZFP_ZFRegExpPrivate
{
public:
    void *nativeRegExp;
    zfstring pattern;
    ZFRegExpOptionFlags flag;

public:
    _ZFP_ZFRegExpPrivate(void)
    : nativeRegExp(zfnull)
    , pattern()
    , flag()
    {
    }
};

// ============================================================
// ZFRegExp
ZFOBJECT_REGISTER(ZFRegExp)

zfbool ZFRegExp::serializableOnSerializeFromData(ZF_IN const ZFSerializableData &serializableData,
                                                 ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */,
                                                 ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */)
{
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeFromData(serializableData, outErrorHint, outErrorPos)) {return zffalse;}

    const zfchar *pattern = zfnull;
    ZFSerializableUtilSerializeAttributeFromData(serializableData, outErrorHint, outErrorPos,
        check, ZFSerializableKeyword_ZFRegExp_pattern, zfstring, pattern);

    ZFRegExpOptionFlags flag = ZFRegExpOptionFlags::EnumDefault();
    ZFSerializableUtilSerializeAttributeFromData(serializableData, outErrorHint, outErrorPos,
        check, ZFSerializableKeyword_ZFRegExp_flag, ZFRegExpOptionFlags, flag);

    this->regExpCompile(pattern ? pattern : "", flag);

    return zftrue;
}
zfbool ZFRegExp::serializableOnSerializeToData(ZF_IN_OUT ZFSerializableData &serializableData,
                                               ZF_IN ZFSerializable *referencedOwnerOrNull,
                                               ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */)
{
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeToData(serializableData, referencedOwnerOrNull, outErrorHint)) {return zffalse;}
    zfself *ref = ZFCastZFObject(zfself *, referencedOwnerOrNull);

    ZFSerializableUtilSerializeAttributeToData(serializableData, outErrorHint, ref,
        ZFSerializableKeyword_ZFRegExp_pattern, zfstring, this->regExpPattern(), ref->regExpPattern(), "");

    ZFSerializableUtilSerializeAttributeToData(serializableData, outErrorHint, ref,
        ZFSerializableKeyword_ZFRegExp_flag, ZFRegExpOptionFlags, this->regExpFlag(), ref->regExpFlag(), ZFRegExpOptionFlags::EnumDefault());

    return zftrue;
}

ZFOBJECT_ON_INIT_DEFINE_2(ZFRegExp,
                          ZFMP_IN(const zfchar *, pattern),
                          ZFMP_IN_OPT(ZFRegExpOptionFlags, flag, ZFRegExpOptionFlags::EnumDefault()))
{
    this->objectOnInit();
    zfself::regExpCompile(pattern, flag);
}
void ZFRegExp::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFRegExpPrivate);
    d->nativeRegExp = ZFPROTOCOL_ACCESS(ZFRegExp)->nativeRegExpCreate(this);
}
void ZFRegExp::objectOnDealloc(void)
{
    ZFPROTOCOL_ACCESS(ZFRegExp)->nativeRegExpDestroy(this, d->nativeRegExp);
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}

void ZFRegExp::objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
{
    zfsuper::objectInfoOnAppend(ret);
    ZFClassUtil::objectPropertyInfo(ret, this);
}

zfidentity ZFRegExp::objectHash(void)
{
    ZFRegExpOptionFlags flag = this->regExpFlag();
    return zfidentityHash(zfidentityCalcString(this->regExpPattern()),
                         zfidentityCalcPOD(flag));
}
ZFCompareResult ZFRegExp::objectCompare(ZF_IN ZFObject *anotherObj)
{
    if(this == anotherObj) {return ZFCompareTheSame;}
    zfself *another = ZFCastZFObject(zfself *, anotherObj);
    if(another == zfnull) {return ZFCompareUncomparable;}

    if(zfscmpTheSame(this->regExpPattern(), another->regExpPattern())
        && this->regExpFlag() == another->regExpFlag())
    {
        return ZFCompareTheSame;
    }
    return ZFCompareUncomparable;
}

ZFMETHOD_DEFINE_0(ZFRegExp, void *, nativeRegExp)
{
    return d->nativeRegExp;
}

ZFMETHOD_DEFINE_0(ZFRegExp, const zfchar *, regExpPattern)
{
    return (d->pattern.isEmpty() ? zfnull : d->pattern.cString());
}
ZFMETHOD_DEFINE_0(ZFRegExp, ZFRegExpOptionFlags, regExpFlag)
{
    return d->flag;
}
ZFMETHOD_DEFINE_1(ZFRegExp, zfindex, regExpNamedGroupIndexForName, ZFMP_IN(const zfchar *, name))
{
    if(name == zfnull)
    {
        return zfindexMax();
    }
    return ZFPROTOCOL_ACCESS(ZFRegExp)->regExpNamedGroupIndexForName(this, name);
}

ZFMETHOD_DEFINE_2(ZFRegExp, void, regExpCompile,
                  ZFMP_IN(const zfchar *, pattern),
                  ZFMP_IN_OPT(ZFRegExpOptionFlags, flag, ZFRegExpOptionFlags::EnumDefault()))
{
    if(pattern == zfnull)
    {
        pattern = "";
    }
    d->pattern = pattern;
    d->flag = flag;
    ZFPROTOCOL_ACCESS(ZFRegExp)->regExpCompile(this, pattern, flag);
}
ZFMETHOD_DEFINE_3(ZFRegExp, void, regExpMatch,
                  ZFMP_OUT(ZFRegExpResult &, result),
                  ZFMP_IN(const zfchar *, src),
                  ZFMP_IN_OPT(zfindex, srcLength, zfindexMax()))
{
    result.namedGroups.removeAll();
    ZFPROTOCOL_ACCESS(ZFRegExp)->regExpMatch(this, result, src, srcLength);
}
ZFMETHOD_DEFINE_3(ZFRegExp, void, regExpMatchExact,
                  ZFMP_OUT(ZFRegExpResult &, result),
                  ZFMP_IN(const zfchar *, src),
                  ZFMP_IN_OPT(zfindex, srcLength, zfindexMax()))
{
    result.namedGroups.removeAll();
    ZFPROTOCOL_ACCESS(ZFRegExp)->regExpMatchExact(this, result, src, srcLength);
}
ZFMETHOD_DEFINE_6(ZFRegExp, void, regExpReplace,
                  ZFMP_OUT(zfstring &, ret),
                  ZFMP_OUT(ZFRegExpResult &, result),
                  ZFMP_IN(const zfchar *, src),
                  ZFMP_IN(const zfchar *, replacePattern),
                  ZFMP_IN_OPT(zfindex, maxReplaceCount, zfindexMax()),
                  ZFMP_IN_OPT(zfindex, srcLength, zfindexMax()))
{
    result.namedGroups.removeAll();
    ZFPROTOCOL_ACCESS(ZFRegExp)->regExpReplace(this, ret, result, src, replacePattern, maxReplaceCount, srcLength);
}

ZF_NAMESPACE_GLOBAL_END

