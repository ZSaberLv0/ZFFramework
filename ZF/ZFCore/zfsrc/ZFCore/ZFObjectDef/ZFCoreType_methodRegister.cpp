#include "ZFCoreType_methodRegister.h"
#include "ZFTypeId_CoreType.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFEXPORT_VAR_READONLY_DEFINE(zfint, ZF_ENV_ZFVERSION_MAIN, ZF_ENV_ZFVERSION_MAIN())
ZFEXPORT_VAR_READONLY_DEFINE(zfint, ZF_ENV_ZFVERSION_SUB, ZF_ENV_ZFVERSION_SUB())
ZFEXPORT_VAR_READONLY_DEFINE(zfint, ZF_ENV_ZFVERSION_MINOR, ZF_ENV_ZFVERSION_MINOR())
ZFEXPORT_VAR_READONLY_DEFINE(const zfchar *, ZF_ENV_ZFVERSION, ZF_ENV_ZFVERSION())

// ============================================================
// zfCoreArgSplit
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(void, zfCoreArgSplit, ZFMP_IN_OUT(ZFCoreArray<zfstring> &, result), ZFMP_IN(const zfchar *, cmdLine))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(ZFCoreArray<zfstring>, zfCoreArgSplit, ZFMP_IN(const zfchar *, cmdLine))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(void, zfCoreArgMerge, ZFMP_OUT(zfstring &, result), ZFMP_IN(const ZFCoreArray<zfstring> &, argList))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(zfstring, zfCoreArgMerge, ZFMP_IN(const ZFCoreArray<zfstring> &, argList))

// ============================================================
// zfCoreDataEncode
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_0(const zfchar *, zfCoreDataEncodeCharMapDefault)
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_5(void, zfCoreDataEncode, ZFMP_OUT(zfstring &, result), ZFMP_IN(const zfchar *, src), ZFMP_IN_OPT(zfindex, srcLen, zfindexMax()), ZFMP_IN_OPT(const zfchar *, charMap, zfCoreDataEncodeCharMapDefault()), ZFMP_IN_OPT(zfchar, escapeToken, '%'))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_4(void, zfCoreDataDecode, ZFMP_OUT(zfstring &, result), ZFMP_IN(const zfchar *, src), ZFMP_IN_OPT(zfindex, srcLen, zfindexMax()), ZFMP_IN_OPT(zfchar, escapeToken, '%'))

// ============================================================
// zfCoreDataPairSplit
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_8(zfbool, zfCoreDataPairSplitString, ZFMP_IN_OUT(ZFCoreArray<ZFIndexRange> &, outData), ZFMP_IN(zfindex, desiredCountOrIndexMax), ZFMP_IN(const zfchar *, src), ZFMP_IN_OPT(zfindex, srcLen, zfindexMax()), ZFMP_IN_OPT(const zfchar *, separatorTokens, ","), ZFMP_IN_OPT(const zfchar *, leftTokens, "("), ZFMP_IN_OPT(const zfchar *, rightTokens, ")"), ZFMP_IN_OPT(zfbool, allowEmptyItem, zffalse))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_8(zfbool, zfCoreDataPairSplitInt, ZFMP_IN_OUT(ZFCoreArrayPOD<zfint> &, outData), ZFMP_IN(zfindex, desiredCountOrIndexMax), ZFMP_IN(const zfchar *, src), ZFMP_IN_OPT(zfindex, srcLen, zfindexMax()), ZFMP_IN_OPT(const zfchar *, separatorTokens, ","), ZFMP_IN_OPT(const zfchar *, leftTokens, "("), ZFMP_IN_OPT(const zfchar *, rightTokens, ")"), ZFMP_IN_OPT(zfbool, allowEmptyItem, zffalse))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_8(zfbool, zfCoreDataPairSplitFloat, ZFMP_IN_OUT(ZFCoreArrayPOD<zffloat> &, outData), ZFMP_IN(zfindex, desiredCountOrIndexMax), ZFMP_IN(const zfchar *, src), ZFMP_IN_OPT(zfindex, srcLen, zfindexMax()), ZFMP_IN_OPT(const zfchar *, separatorTokens, ","), ZFMP_IN_OPT(const zfchar *, leftTokens, "("), ZFMP_IN_OPT(const zfchar *, rightTokens, ")"), ZFMP_IN_OPT(zfbool, allowEmptyItem, zffalse))

// ============================================================
// ZFIdentityGenerator
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfidentity, zfidentityCalcString, ZFMP_IN(const zfchar *, src), ZFMP_IN_OPT(zfindex, srcLen, zfindexMax()))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfidentity, zfidentityCalcBuf, ZFMP_IN(const void *, src), ZFMP_IN(zfindex, srcLen))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(zfidentity, zfidentityCalcPointer, ZFMP_IN(const void *, p))

// ============================================================
// ZFLevel
ZFEXPORT_ENUM_DEFINE(ZFLevel
    , ZFLevelZFFrameworkStatic
    , ZFLevelZFFrameworkEssential
    , ZFLevelZFFrameworkHigh
    , ZFLevelZFFrameworkNormal
    , ZFLevelZFFrameworkLow
    , ZFLevelAppEssential
    , ZFLevelAppHigh
    , ZFLevelAppNormal
    , ZFLevelAppLow
    , ZFLevelZFFrameworkPostLow
    , ZFLevelZFFrameworkPostNormal
    , ZFLevelZFFrameworkPostHigh
    , ZFLevelZFFrameworkPostEssential
    , ZFLevelZFFrameworkPostStatic
    )

// ============================================================
// ZFCoreStatistic
ZF_NAMESPACE_BEGIN(ZFCoreStatistic)
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(void, invokeCountLog, ZFMP_IN(const zfchar *, key))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(void, invokeCountRemove, ZFMP_IN(const zfchar *, key))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_0(void, invokeCountRemoveAll)
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(zfindex, invokeCountGet, ZFMP_IN(const zfchar *, key))
ZF_NAMESPACE_END(ZFCoreStatistic)

// ============================================================
// zfstringFind
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_4(zfindex, zfstringFind, ZFMP_IN(const zfchar *, src), ZFMP_IN(zfindex, srcLen), ZFMP_IN(const zfchar *, find), ZFMP_IN_OPT(zfindex, findLen, zfindexMax()))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfindex, zfstringFind, ZFMP_IN(const zfstring &, src), ZFMP_IN(const zfstring &, find))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zfindex, zfstringFind, ZFMP_IN(const zfstring &, src), ZFMP_IN(const zfchar *, find), ZFMP_IN_OPT(zfindex, findLen, zfindexMax()))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfindex, zfstringFind, ZFMP_IN(const zfstring &, src), ZFMP_IN(zfchar, find))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_4(zfindex, zfstringFindReversely, ZFMP_IN(const zfchar *, src), ZFMP_IN(zfindex, srcLen), ZFMP_IN(const zfchar *, find), ZFMP_IN_OPT(zfindex, findLen, zfindexMax()))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfindex, zfstringFindReversely, ZFMP_IN(const zfstring &, src), ZFMP_IN(const zfstring &, find))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zfindex, zfstringFindReversely, ZFMP_IN(const zfstring &, src), ZFMP_IN(const zfchar *, find), ZFMP_IN_OPT(zfindex, findLen, zfindexMax()))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfindex, zfstringFindReversely, ZFMP_IN(const zfstring &, src), ZFMP_IN(zfchar, find))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_4(zfindex, zfstringFindCaseInsensitive, ZFMP_IN(const zfchar *, src), ZFMP_IN(zfindex, srcLen), ZFMP_IN(const zfchar *, find), ZFMP_IN_OPT(zfindex, findLen, zfindexMax()))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfindex, zfstringFindCaseInsensitive, ZFMP_IN(const zfstring &, src), ZFMP_IN(const zfstring &, find))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zfindex, zfstringFindCaseInsensitive, ZFMP_IN(const zfstring &, src), ZFMP_IN(const zfchar *, find), ZFMP_IN_OPT(zfindex, findLen, zfindexMax()))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfindex, zfstringFindCaseInsensitive, ZFMP_IN(const zfstring &, src), ZFMP_IN(zfchar, find))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_4(zfindex, zfstringFindCaseInsensitiveReversely, ZFMP_IN(const zfchar *, src), ZFMP_IN(zfindex, srcLen), ZFMP_IN(const zfchar *, find), ZFMP_IN_OPT(zfindex, findLen, zfindexMax()))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfindex, zfstringFindCaseInsensitiveReversely, ZFMP_IN(const zfstring &, src), ZFMP_IN(const zfstring &, find))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zfindex, zfstringFindCaseInsensitiveReversely, ZFMP_IN(const zfstring &, src), ZFMP_IN(const zfchar *, find), ZFMP_IN_OPT(zfindex, findLen, zfindexMax()))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfindex, zfstringFindCaseInsensitiveReversely, ZFMP_IN(const zfstring &, src), ZFMP_IN(zfchar, find))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_4(zfindex, zfstringFindFirstOf, ZFMP_IN(const zfchar *, src), ZFMP_IN(zfindex, srcLen), ZFMP_IN(const zfchar *, find), ZFMP_IN_OPT(zfindex, findLen, zfindexMax()))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfindex, zfstringFindFirstOf, ZFMP_IN(const zfstring &, src), ZFMP_IN(const zfstring &, find))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zfindex, zfstringFindFirstOf, ZFMP_IN(const zfstring &, src), ZFMP_IN(const zfchar *, find), ZFMP_IN_OPT(zfindex, findLen, zfindexMax()))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfindex, zfstringFindFirstOf, ZFMP_IN(const zfstring &, src), ZFMP_IN(zfchar, find))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_4(zfindex, zfstringFindFirstNotOf, ZFMP_IN(const zfchar *, src), ZFMP_IN(zfindex, srcLen), ZFMP_IN(const zfchar *, find), ZFMP_IN_OPT(zfindex, findLen, zfindexMax()))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfindex, zfstringFindFirstNotOf, ZFMP_IN(const zfstring &, src), ZFMP_IN(const zfstring &, find))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zfindex, zfstringFindFirstNotOf, ZFMP_IN(const zfstring &, src), ZFMP_IN(const zfchar *, find), ZFMP_IN_OPT(zfindex, findLen, zfindexMax()))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfindex, zfstringFindFirstNotOf, ZFMP_IN(const zfstring &, src), ZFMP_IN(zfchar, find))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_4(zfindex, zfstringFindLastOf, ZFMP_IN(const zfchar *, src), ZFMP_IN(zfindex, srcLen), ZFMP_IN(const zfchar *, find), ZFMP_IN_OPT(zfindex, findLen, zfindexMax()))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfindex, zfstringFindLastOf, ZFMP_IN(const zfstring &, src), ZFMP_IN(const zfstring &, find))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zfindex, zfstringFindLastOf, ZFMP_IN(const zfstring &, src), ZFMP_IN(const zfchar *, find), ZFMP_IN_OPT(zfindex, findLen, zfindexMax()))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfindex, zfstringFindLastOf, ZFMP_IN(const zfstring &, src), ZFMP_IN(zfchar, find))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_4(zfindex, zfstringFindLastNotOf, ZFMP_IN(const zfchar *, src), ZFMP_IN(zfindex, srcLen), ZFMP_IN(const zfchar *, find), ZFMP_IN_OPT(zfindex, findLen, zfindexMax()))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfindex, zfstringFindLastNotOf, ZFMP_IN(const zfstring &, src), ZFMP_IN(const zfstring &, find))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zfindex, zfstringFindLastNotOf, ZFMP_IN(const zfstring &, src), ZFMP_IN(const zfchar *, find), ZFMP_IN_OPT(zfindex, findLen, zfindexMax()))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfindex, zfstringFindLastNotOf, ZFMP_IN(const zfstring &, src), ZFMP_IN(zfchar, find))

// ============================================================
// namespace
ZFEXPORT_VAR_READONLY_DEFINE(const zfchar *, ZFNamespaceSeparator, ZFNamespaceSeparator())
ZFEXPORT_VAR_READONLY_DEFINE(zfindex, ZFNamespaceSeparatorLen, ZFNamespaceSeparatorLen())
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(const zfchar *, ZFNamespaceSkipGlobal, ZFMP_IN(const zfchar *, ns))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zfbool, ZFNamespaceSplit, ZFMP_IN_OUT(ZFCoreArray<ZFIndexRange> &, ret), ZFMP_IN(const zfchar *, src), ZFMP_IN_OPT(zfindex, srcLen, zfindexMax()))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(void, ZFNamespaceGetAllT, ZFMP_IN_OUT(ZFCoreArray<const zfchar *> &, ret))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_0(ZFCoreArrayPOD<const zfchar *>, ZFNamespaceGetAll)
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(void, ZFNamespaceGetAllT, ZFMP_IN_OUT(ZFCoreArray<const zfchar *> &, ret), ZFMP_IN(const zfchar *, parent), ZFMP_IN_OPT(zfbool, recursive, zffalse))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(ZFCoreArrayPOD<const zfchar *>, ZFNamespaceGetAll, ZFMP_IN(const zfchar *, parent), ZFMP_IN_OPT(zfbool, recursive, zffalse))

// ============================================================
// core type
ZFEXPORT_VAR_READONLY_DEFINE(zfindex, zfindexMax, zfindexMax())
ZFEXPORT_VAR_READONLY_DEFINE(zfindex, zfindexZero, zfindexZero())
ZFEXPORT_VAR_READONLY_DEFINE(zfint, zfintZero, zfintZero())
ZFEXPORT_VAR_READONLY_DEFINE(zfuint, zfuintZero, zfuintZero())
ZFEXPORT_VAR_READONLY_DEFINE(zffloat, zffloatZero, zffloatZero())
ZFEXPORT_VAR_READONLY_DEFINE(zfdouble, zfdoubleZero, zfdoubleZero())
ZFEXPORT_VAR_READONLY_DEFINE(zflongdouble, zflongdoubleZero, zflongdoubleZero())
ZFEXPORT_VAR_READONLY_DEFINE(zfbyte, zfbyteZero, zfbyteZero())
ZFEXPORT_VAR_READONLY_DEFINE(zftimet, zftimetZero, zftimetZero())
ZFEXPORT_VAR_READONLY_DEFINE(zfflags, zfflagsZero, zfflagsZero())
ZFEXPORT_VAR_READONLY_DEFINE(zfidentity, zfidentityZero, zfidentityZero())
ZFEXPORT_VAR_READONLY_DEFINE(zfidentity, zfidentityInvalid, zfidentityInvalid())
ZFEXPORT_ENUM_DEFINE(ZFCompareResult, ZFCompareUncomparable, ZFCompareSmaller, ZFCompareTheSame, ZFCompareGreater)
ZFEXPORT_ENUM_DEFINE(ZFSeekPos, ZFSeekPosBegin, ZFSeekPosCur, ZFSeekPosCurReversely, ZFSeekPosEnd)
ZFEXPORT_ENUM_DEFINE(ZFFilterType, ZFFilterTypeInclude, ZFFilterTypeExclude)
ZFEXPORT_ENUM_DEFINE(ZFFilterCallbackResult, ZFFilterCallbackResultNotSpecified, ZFFilterCallbackResultActive, ZFFilterCallbackResultNotActive)

ZFEXPORT_VAR_READONLY_VALUEREF_DEFINE(zfiterator, zfiteratorInvalid, zfiteratorInvalid())

ZFEXPORT_VAR_READONLY_VALUEREF_DEFINE(ZFIndexRange, ZFIndexRangeZero, ZFIndexRangeZero())
ZFEXPORT_VAR_READONLY_VALUEREF_DEFINE(ZFIndexRange, ZFIndexRangeMax, ZFIndexRangeMax())
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(ZFIndexRange, ZFIndexRangeMake, ZFMP_IN(zfindex, start), ZFMP_IN(zfindex, count))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(ZFCompareResult, ZFIndexRangeIsEqual, ZFMP_IN(const ZFIndexRange &, e0), ZFMP_IN(const ZFIndexRange &, e1))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfbool, ZFIndexRangeContain, ZFMP_IN(const ZFIndexRange &, range), ZFMP_IN(zfindex, index))

ZFEXPORT_VAR_READONLY_DEFINE(ZFToken, ZFTokenInvalid, ZFTokenInvalid())

// ============================================================
// ZFCoreUtilMath
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfint, zfmMax, ZFMP_IN(zfint const &, n0), ZFMP_IN(zfint const &, n1))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zffloat, zfmMax, ZFMP_IN(zffloat const &, n0), ZFMP_IN(zffloat const &, n1))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfint, zfmMin, ZFMP_IN(zfint const &, n0), ZFMP_IN(zfint const &, n1))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zffloat, zfmMin, ZFMP_IN(zffloat const &, n0), ZFMP_IN(zffloat const &, n1))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(zfint, zfmAbs, ZFMP_IN(zfint const &, n0))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(zffloat, zfmAbs, ZFMP_IN(zffloat const &, n0))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zfint, zfmApplyRange, ZFMP_IN(zfint const &, n), ZFMP_IN(zfint const &, nMin), ZFMP_IN(zfint const &, nMax))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zffloat, zfmApplyRange, ZFMP_IN(zffloat const &, n), ZFMP_IN(zffloat const &, nMin), ZFMP_IN(zffloat const &, nMax))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zfint, zfmApplyProgress, ZFMP_IN(zfint const &, start), ZFMP_IN(zfint const &, end), ZFMP_IN(zffloat const &, progress))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zffloat, zfmApplyProgress, ZFMP_IN(zffloat const &, start), ZFMP_IN(zffloat const &, end), ZFMP_IN(zffloat const &, progress))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(zffloat, zfmRoundUp, ZFMP_IN(zffloat const &, v))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(zffloat, zfmRoundDown, ZFMP_IN(zffloat const &, v))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(zffloat, zfmRound, ZFMP_IN(zffloat const &, v))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_0(zfuint, zfmRand)
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(zfint, zfmRand, ZFMP_IN(zfint, range))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfint, zfmRand, ZFMP_IN(zfint, start), ZFMP_IN(zfint, end))

// ============================================================
// zfVersion
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(void, zfVersionGet, ZFMP_OUT(zfstring &, ret), ZFMP_IN(const zfchar *, version), ZFMP_IN(zfindex, subVersionIndex))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfstring, zfVersionGet, ZFMP_IN(const zfchar *, version), ZFMP_IN(zfindex, subVersionIndex))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(void, zfVersionGetInt, ZFMP_OUT(zfuint &, ret), ZFMP_IN(const zfchar *, version), ZFMP_IN(zfindex, subVersionIndex))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfuint, zfVersionGetInt, ZFMP_IN(const zfchar *, version), ZFMP_IN(zfindex, subVersionIndex))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_4(void, zfVersionSet, ZFMP_IN_OUT(zfstring &, version), ZFMP_IN(zfindex, subVersionIndex), ZFMP_IN(const zfchar *, subVersion), ZFMP_IN_OPT(const zfchar *, emptySubVersion, "0"))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_4(void, zfVersionSetInt, ZFMP_IN_OUT(zfstring &, version), ZFMP_IN(zfindex, subVersionIndex), ZFMP_IN(zfuint, subVersion), ZFMP_IN_OPT(const zfchar *, emptySubVersion, "0"))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(ZFCompareResult, zfVersionCompare, ZFMP_IN(const zfchar *, version0), ZFMP_IN(const zfchar *, version1))
ZFEXPORT_VAR_READONLY_DEFINE(zfindex, ZFVERSION_MAIN, ZFVERSION_MAIN())
ZFEXPORT_VAR_READONLY_DEFINE(zfindex, ZFVERSION_SUB, ZFVERSION_SUB())
ZFEXPORT_VAR_READONLY_DEFINE(zfindex, ZFVERSION_MINOR, ZFVERSION_MINOR())
ZFEXPORT_VAR_READONLY_DEFINE(zfindex, ZFVERSION_BUILD, ZFVERSION_BUILD())

// ============================================================
// ZFTokenForContainer
ZFEXPORT_VAR_READONLY_VALUEREF_DEFINE(ZFTokenForContainer, ZFTokenForContainerTrim, ZFTokenForContainerTrim())
ZFEXPORT_VAR_READONLY_VALUEREF_DEFINE(ZFTokenForContainer, ZFTokenForContainerDetail, ZFTokenForContainerDetail())
ZFEXPORT_VAR_READONLY_VALUEREF_DEFINE(ZFTokenForContainer, ZFTokenForContainerPlainList, ZFTokenForContainerPlainList())
ZFEXPORT_VAR_VALUEREF_DEFINE(ZFTokenForContainer, ZFTokenForContainerDefault, ZFTokenForContainerDefault())

ZFEXPORT_VAR_READONLY_VALUEREF_DEFINE(ZFTokenForKeyValueContainer, ZFTokenForKeyValueContainerTrim, ZFTokenForKeyValueContainerTrim())
ZFEXPORT_VAR_READONLY_VALUEREF_DEFINE(ZFTokenForKeyValueContainer, ZFTokenForKeyValueContainerDetail, ZFTokenForKeyValueContainerDetail())
ZFEXPORT_VAR_VALUEREF_DEFINE(ZFTokenForKeyValueContainer, ZFTokenForKeyValueContainerDefault, ZFTokenForKeyValueContainerDefault())

ZF_NAMESPACE_GLOBAL_END
#endif

