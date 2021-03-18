#include "ZFImpl_default_ZFAlgorithm_impl.h"
#include "ZFAlgorithm/protocol/ZFProtocolZFRegExp.h"

#undef min
#undef max
#include "../_repo/deelx/deelx.h"
#undef min
#undef max

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFRegExpImpl_default, ZFRegExp, ZFProtocolLevel::e_Default)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("deelx")
public:
    virtual void *nativeRegExpCreate(ZF_IN ZFRegExp *regExp)
    {
        return zfnew(CRegexpT<zfchar>);
    }
    virtual void nativeRegExpDestroy(ZF_IN ZFRegExp *regExp,
                                     ZF_IN void *nativeRegExp)
    {
        zfdelete(ZFCastStatic(CRegexpT<zfchar> *, nativeRegExp));
    }

    virtual void regExpCompile(ZF_IN ZFRegExp *regExp,
                               ZF_IN const zfchar *pattern,
                               ZF_IN_OPT ZFRegExpOptionFlags flag = ZFRegExpOptionFlags::EnumDefault())
    {
        CRegexpT<zfchar> *regexp = ZFCastStatic(CRegexpT<zfchar> *, regExp->nativeRegExp());

        int tmp = NO_FLAG;
        if(ZFBitTest(flag, ZFRegExpOption::e_IgnoreCase))
        {
            ZFBitSet(tmp, IGNORECASE);
        }
        if(ZFBitTest(flag, ZFRegExpOption::e_SingleLine))
        {
            ZFBitSet(tmp, SINGLELINE);
        }
        if(ZFBitTest(flag, ZFRegExpOption::e_MultiLine))
        {
            ZFBitSet(tmp, MULTILINE);
        }
        regexp->Compile(pattern, tmp);
    }

    virtual zfindex regExpNamedGroupIndexForName(ZF_IN ZFRegExp *regExp,
                                                 ZF_IN const zfchar *name)
    {
        CRegexpT<zfchar> *regexp = ZFCastStatic(CRegexpT<zfchar> *, regExp->nativeRegExp());

        zfint ret = regexp->GetNamedGroupNumber(name);
        return ((ret < 0) ? zfindexMax() : (zfindex)ret);
    }

    virtual void regExpMatch(ZF_IN ZFRegExp *regExp,
                             ZF_OUT ZFRegExpResult &result,
                             ZF_IN const zfchar *src,
                             ZF_IN_OPT zfindex srcLength = zfindexMax())
    {
        CRegexpT<zfchar> *regexp = ZFCastStatic(CRegexpT<zfchar> *, regExp->nativeRegExp());

        MatchResult regexpResult = ((srcLength == zfindexMax())
            ? regexp->Match(src)
            : regexp->Match(src, (zfint)srcLength, 0, zfnull));
        this->fillResult(result, regexpResult);
    }
    virtual void regExpMatchExact(ZF_IN ZFRegExp *regExp,
                                  ZF_OUT ZFRegExpResult &result,
                                  ZF_IN const zfchar *src,
                                  ZF_IN_OPT zfindex srcLength = zfindexMax())
    {
        CRegexpT<zfchar> *regexp = ZFCastStatic(CRegexpT<zfchar> *, regExp->nativeRegExp());

        MatchResult regexpResult = ((srcLength == zfindexMax())
            ? regexp->MatchExact(src)
            : regexp->MatchExact(src, (zfint)srcLength, zfnull));
        this->fillResult(result, regexpResult);
    }

    virtual void regExpReplace(ZF_IN ZFRegExp *regExp,
                               ZF_OUT zfstring &ret,
                               ZF_OUT ZFRegExpResult &result,
                               ZF_IN const zfchar *src,
                               ZF_IN const zfchar *replacePattern,
                               ZF_IN_OPT zfindex maxReplaceCount = zfindexMax(),
                               ZF_IN_OPT zfindex srcLength = zfindexMax())
    {
        CRegexpT<zfchar> *regexp = ZFCastStatic(CRegexpT<zfchar> *, regExp->nativeRegExp());

        zfint dummy;
        MatchResult regexpResult;
        zfchar *pResult = regexp->Replace(
            src, (zfint)((srcLength == zfindexMax()) ? zfslen(src) : srcLength),
            replacePattern, (zfint)zfslen(replacePattern),
            dummy,
            -1,
            (zfint)((maxReplaceCount == zfindexMax()) ? -1 : maxReplaceCount),
            &regexpResult);
        if(pResult != zfnull)
        {
            ret += pResult;
            regexp->ReleaseString(pResult);
        }

        this->fillResult(result, regexpResult);
    }

private:
    void fillResult(ZF_OUT ZFRegExpResult &result,
                    ZF_IN const MatchResult &regexpResult)
    {
        result.matched = (regexpResult.IsMatched() != 0);
        if(regexpResult.GetEnd() > regexpResult.GetStart())
        {
            result.matchedRange = ZFIndexRangeMake(regexpResult.GetStart(), regexpResult.GetEnd() - regexpResult.GetStart());
        }
        zfindex n = regexpResult.MaxGroupNumber() + 1;
        for(zfindex i = 1; i < n; ++i)
        {
            ZFIndexRange indexPair;
            indexPair.start = regexpResult.GetGroupStart((zfint)i);
            indexPair.count = regexpResult.GetGroupEnd((zfint)i) - indexPair.start;
            if(indexPair.start != zfindexMax() && indexPair.count > 0)
            {
                result.namedGroups.add(indexPair);
            }
        }
        if(result.namedGroups.isEmpty())
        {
            result.matchedRange = ZFIndexRangeZero();
        }
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFRegExpImpl_default)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFRegExpImpl_default)

ZF_NAMESPACE_GLOBAL_END

