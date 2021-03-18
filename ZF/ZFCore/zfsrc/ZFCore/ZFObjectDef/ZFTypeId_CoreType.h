/**
 * @file ZFTypeId_CoreType.h
 * @brief type define for #ZFTYPEID_DECLARE
 */

#ifndef _ZFI_ZFTypeId_CoreType_h_
#define _ZFI_ZFTypeId_CoreType_h_

#include "ZFTypeId_spec.h"
#include "ZFFilterForZFClass.h"
#include "ZFFilterForZFObject.h"
#include "ZFFilterForZFMethod.h"
#include "ZFFilterForZFProperty.h"
#include "ZFMethodDynamicRegister.h"
#include "ZFPropertyDynamicRegister.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// string conversion spec
/** @cond ZFPrivateDoc */
inline zfbool zfstringToString(ZF_IN_OUT zfstring &s,
                               ZF_IN const zfchar *v)
{
    s += v;
    return zftrue;
}
inline zfstring zfstringToString(ZF_IN const zfchar *v)
{
    return zfstring(v);
}
inline zfbool zfstringFromString(ZF_OUT const zfchar *&v,
                                 ZF_IN const zfchar *src,
                                 ZF_IN_OPT zfindex srcLen = zfindexMax())
{
    if(srcLen != zfindexMax() && src[srcLen] != '\0')
    {
        return zffalse;
    }
    v = src;
    return zftrue;
}
/** @endcond */

/**
 * @brief see #zfstringFromData, designed for performance
 *
 * returned (const zfchar *) propertyValue is the pointer to serializableData's value,
 * you should ensure it's alive while using the propertyValue
 */
extern ZF_ENV_EXPORT zfbool zfstringFromData(ZF_OUT const zfchar * &propertyValue,
                                             ZF_IN const ZFSerializableData &serializableData,
                                             ZF_OUT_OPT zfstring *outErrorHint = zfnull,
                                             ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull);
/**
 * @brief see #zfstringToData, designed for performance
 */
extern ZF_ENV_EXPORT zfbool zfstringToData(ZF_OUT ZFSerializableData &serializableData,
                                           ZF_IN const zfchar * const &propertyValue,
                                           ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */);

/**
 * @brief convert flags to string using flagList and nameList
 *
 * result would be like "Flag0|Flag1|0x123ABC"\n
 * if flags is 0 and there's no 0 value in flagList, or error occurred, "0x0" would be returned
 * params:
 * -  includeNotConverted
 *   if true, values not recognized would be appended to result as HEX value
 * -  exclusiveMode if true, when a flag recognized,
 *   flags would be unset and can't be recognized any more\n
 *   for example, if we have two flag 0101 and 0011 in flagList which shares 0001 bit,
 *   then if we are converting 0111, since 0101 would be recognized first
 *   and leave 0010, 0011 won't be recognized any more
 * -  notConverted if all value recognized, notConverted would be set to 0,
 *   otherwise, the not converted value would be set
 */
extern ZF_ENV_EXPORT zfbool zfflagsToString(ZF_IN_OUT zfstring &ret,
                                            ZF_IN const zfflags *flagList,
                                            ZF_IN const zfchar **nameList,
                                            ZF_IN zfindex listCount,
                                            ZF_IN zfflags const &value,
                                            ZF_IN_OPT zfbool includeNotConverted = zftrue,
                                            ZF_IN_OPT zfbool exclusiveMode = zffalse,
                                            ZF_OUT_OPT zfflags *notConverted = zfnull,
                                            ZF_IN_OPT zfchar separatorToken = '|');
/** @brief see #zfflagsToString */
inline zfstring zfflagsToString(ZF_IN const zfflags *flagList,
                                ZF_IN const zfchar **nameList,
                                ZF_IN zfindex listCount,
                                ZF_IN zfflags const &value,
                                ZF_IN_OPT zfbool includeNotConverted = zftrue,
                                ZF_IN_OPT zfbool exclusiveMode = zffalse,
                                ZF_OUT_OPT zfflags *notConverted = zfnull,
                                ZF_IN_OPT zfchar separatorToken = '|')
{
    zfstring ret;
    zfflagsToString(ret, flagList, nameList, listCount, value, includeNotConverted, exclusiveMode, notConverted, separatorToken);
    return ret;
}

/**
 * @brief convert string to zfflags
 *
 * params:
 * -  pErrPos pointer to first char where error occurred,
 *   a non converted format is not regarded as error
 */
extern ZF_ENV_EXPORT zfbool zfflagsFromString(ZF_OUT zfflags &ret,
                                              ZF_IN const zfflags *flagList,
                                              ZF_IN const zfchar **nameList,
                                              ZF_IN zfindex listCount,
                                              ZF_IN const zfchar *src,
                                              ZF_IN_OPT zfindex srcLen = zfindexMax(),
                                              ZF_IN_OPT zfchar separatorToken = '|',
                                              ZF_OUT_OPT const zfchar **outErrorPos = zfnull);

// ============================================================
// property type
/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <zfbool value="value" />
 * @endcode
 */
ZFTYPEID_DECLARE(zfbool, zfbool)
ZFOUTPUT_TYPE(zfbool, {output.execute(v ? ZFTOKEN_zfbool_zftrue : ZFTOKEN_zfbool_zffalse);})

extern ZF_ENV_EXPORT v_zfbool *_ZFP_v_zftrue(void);
extern ZF_ENV_EXPORT v_zfbool *_ZFP_v_zffalse(void);

/** @brief cached #v_zfbool */
#define v_zftrue (_ZFP_v_zftrue())
/** @brief cached #v_zfbool */
#define v_zffalse (_ZFP_v_zffalse())

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <zfbyte value="value" />
 * @endcode
 */
ZFTYPEID_DECLARE(zfbyte, zfbyte)
ZFOUTPUT_TYPE(zfbyte, {output.execute(zfbyteToString(v));})

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <zfchar value="value" />
 * @endcode
 */
ZFTYPEID_DECLARE(zfchar, zfchar)
ZFOUTPUT_TYPE(zfchar, {output.execute(zfcharToString(v));})

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <zfstring value="value" />
 * @endcode
 */
ZFTYPEID_DECLARE(zfstring, zfstring)
ZFOUTPUT_TYPE(zfstring, {output.execute(zfstringToString(v));})

#define _ZFP_ZFTYPEID_ALIAS_EXPAND_cString(AliasToTypeName, AliasToType, TypeName, Type) \
    template<typename T_Access = _ZFP_PropTypeW_##TypeName \
        , int T_IsPointer = ((zftTraits<typename zftTraits<T_Access>::TrNoRef>::TrIsPtr \
            && zftTypeIsSame< \
                    typename zftTraits<T_Access>::TrNoRef, \
                    _ZFP_PropTypeW_##TypeName \
                >::TypeIsSame != 1) \
            ? 1 : 0) \
        , typename T_Fix = void \
        > \
    zfclassNotPOD Value \
    { \
    public: \
        static zfbool zfvAccessAvailable(ZF_IN_OUT zfautoObject &obj) \
        { \
            return (ZFCastZFObject(v_zfstring *, obj) != zfnull); \
        } \
        static T_Access zfvAccess(ZF_IN_OUT zfautoObject &obj) \
        { \
            return ZFCastZFObject(v_zfstring *, obj)->zfv.cString(); \
        } \
        static void zfvAccessFinish(ZF_IN_OUT zfautoObject &obj) \
        { \
        } \
    }; \
    template<typename T_Access> \
    zfclassNotPOD Value<T_Access const &, 0> \
    { \
    public: \
        static zfbool zfvAccessAvailable(ZF_IN_OUT zfautoObject &obj) \
        { \
            return (ZFCastZFObject(v_zfstring *, obj) != zfnull); \
        } \
        static T_Access zfvAccess(ZF_IN_OUT zfautoObject &obj) \
        { \
            return ZFCastZFObject(v_zfstring *, obj)->zfv.cString(); \
        } \
        static void zfvAccessFinish(ZF_IN_OUT zfautoObject &obj) \
        { \
        } \
    }; \
    template<typename T_Access> \
    zfclassNotPOD Value<T_Access &, 0> \
    { \
    private: \
         typedef typename zftTraits<T_Access>::TrNoRef _TrNoRef; \
    public: \
        static zfbool zfvAccessAvailable(ZF_IN_OUT zfautoObject &obj) \
        { \
            return (ZFCastZFObject(v_zfstring *, obj) != zfnull); \
        } \
        static _TrNoRef zfvAccess(ZF_IN_OUT zfautoObject &obj) \
        { \
            return ZFCastZFObject(v_zfstring *, obj)->zfv.cString(); \
        } \
        static void zfvAccessFinish(ZF_IN_OUT zfautoObject &obj) \
        { \
        } \
    }; \
    template<typename T_Access> \
    zfclassNotPOD Value<T_Access, 1> \
    { \
    private: \
         typedef typename zftTraits<T_Access>::TrNoRef _TrNoRef; \
    public: \
        static zfbool zfvAccessAvailable(ZF_IN_OUT zfautoObject &obj) \
        { \
            return zffalse; \
        } \
        static _TrNoRef zfvAccess(ZF_IN_OUT zfautoObject &obj) \
        { \
            return zfnull; \
        } \
        static void zfvAccessFinish(ZF_IN_OUT zfautoObject &obj) \
        { \
        } \
    };
ZFTYPEID_ALIAS_DECLARE_CUSTOM(zfstring, zfstring, cString, const zfchar *, _ZFP_ZFTYPEID_ALIAS_EXPAND_cString)

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <zfint value="value" />
 * @endcode
 */
ZFTYPEID_DECLARE(zfint, zfint)
ZFOUTPUT_TYPE(zfint, {output.execute(zfintToString(v));})
/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <zfuint value="value" />
 * @endcode
 * @note for portability of unsigned types,
 *   we'll use "-1" to store max unsigned value
 */
ZFTYPEID_DECLARE(zfuint, zfuint)
ZFOUTPUT_TYPE(zfuint, {output.execute(zfuintToString(v));})

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <zfindex value="value" />
 * @endcode
 * @note for portability of unsigned types,
 *   we'll use "-1" to store max unsigned value
 */
ZFTYPEID_DECLARE(zfindex, zfindex)
ZFOUTPUT_TYPE(zfindex, {output.execute(zfindexToString(v));})

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <zffloat value="value" />
 * @endcode
 */
ZFTYPEID_DECLARE(zffloat, zffloat)
ZFOUTPUT_TYPE(zffloat, {output.execute(zffloatToString(v));})

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <zfdouble value="value" />
 * @endcode
 */
ZFTYPEID_DECLARE(zfdouble, zfdouble)
ZFOUTPUT_TYPE(zfdouble, {output.execute(zfdoubleToString(v));})

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <zflongdouble value="value" />
 * @endcode
 */
ZFTYPEID_DECLARE(zflongdouble, zflongdouble)
ZFOUTPUT_TYPE(zflongdouble, {output.execute(zflongdoubleToString(v));})

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <zftimet value="value" />
 * @endcode
 */
ZFTYPEID_DECLARE(zftimet, zftimet)
ZFOUTPUT_TYPE(zftimet, {output.execute(zftimetToString(v));})

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <zfflags value="value" />
 * @endcode
 * @note for portability of unsigned types,
 *   we'll use "-1" to store max unsigned value
 */
ZFTYPEID_DECLARE(zfflags, zfflags)
ZFOUTPUT_TYPE(zfflags, {output.execute(zfflagsToString(v));})

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <zfidentity value="value" />
 * @endcode
 * @note for portability of unsigned types,
 *   we'll use "-1" to store max unsigned value
 */
ZFTYPEID_DECLARE(zfidentity, zfidentity)
ZFOUTPUT_TYPE(zfidentity, {output.execute(zfidentityToString(v));})

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFCompareResult value="value" />
 * @endcode
 */
ZFTYPEID_DECLARE(ZFCompareResult, ZFCompareResult)
ZFOUTPUT_TYPE(ZFCompareResult, {output.execute(ZFCompareResultToString(v));})

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFSeekPos value="value" />
 * @endcode
 */
ZFTYPEID_DECLARE(ZFSeekPos, ZFSeekPos)
ZFOUTPUT_TYPE(ZFSeekPos, {output.execute(ZFSeekPosToString(v));})

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFIndexRange
 *       value="(0, 0)" // optional, 0 by default
 *   />
 * @endcode
 */
ZFTYPEID_DECLARE(ZFIndexRange, ZFIndexRange)
ZFOUTPUT_TYPE(ZFIndexRange, {output.execute(ZFIndexRangeToString(v));})

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFLevel value="value" />
 * @endcode
 */
ZFTYPEID_DECLARE(ZFLevel, ZFLevel)
ZFOUTPUT_TYPE(ZFLevel, {output.execute(ZFLevelToString(v));})

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFFrameworkState value="value" />
 * @endcode
 */
ZFTYPEID_DECLARE(ZFFrameworkState, ZFFrameworkState)
ZFOUTPUT_TYPE(ZFFrameworkState, {output.execute(ZFFrameworkStateToString(v));})

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFFilterType value="value" />
 * @endcode
 */
ZFTYPEID_DECLARE(ZFFilterType, ZFFilterType)
ZFOUTPUT_TYPE(ZFFilterType, {output.execute(ZFFilterTypeToString(v));})

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFFilterCallbackResult value="value" />
 * @endcode
 */
ZFTYPEID_DECLARE(ZFFilterCallbackResult, ZFFilterCallbackResult)
ZFOUTPUT_TYPE(ZFFilterCallbackResult, {output.execute(ZFFilterCallbackResultToString(v));})

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFPathInfo value="pathType:pathData" />
 * @endcode
 */
ZFTYPEID_DECLARE(ZFPathInfo, ZFPathInfo)
ZFOUTPUT_TYPE(ZFPathInfo, {output.execute(ZFPathInfoToString(v));})
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFPathInfo_separator ":"

/**
 * @brief util method to parse path info for performance
 */
extern ZF_ENV_EXPORT zfbool ZFPathInfoParse(ZF_IN const zfchar *pathInfo,
                                            ZF_OUT zfstring &pathType,
                                            ZF_OUT const zfchar *&pathData);

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFTokenForContainer // all tokens are optional
 *       tokenLeft=""
 *       tokenRight=""
 *       tokenSeparator=""
 *       tokenValueLeft=""
 *       tokenValueRight=""
 *       tokenEtc=""
 *   />
 * @endcode
 */
ZFTYPEID_DECLARE(ZFTokenForContainer, ZFTokenForContainer)
ZFOUTPUT_TYPE(ZFTokenForContainer, {output.execute(ZFTokenForContainerToString(v));})
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTokenForContainer_tokenLeft "tokenLeft"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTokenForContainer_tokenRight "tokenRight"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTokenForContainer_tokenSeparator "tokenSeparator"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTokenForContainer_tokenValueLeft "tokenValueLeft"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTokenForContainer_tokenValueRight "tokenValueRight"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTokenForContainer_tokenEtc "tokenEtc"

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFTokenForKeyValueContainer // all tokens are optional
 *       tokenLeft=""
 *       tokenRight=""
 *       tokenSeparator=""
 *       tokenPairLeft=""
 *       tokenPairRight=""
 *       tokenPairSeparator=""
 *       tokenKeyLeft=""
 *       tokenKeyRight=""
 *       tokenValueLeft=""
 *       tokenValueRight=""
 *       tokenEtc=""
 *   />
 * @endcode
 */
ZFTYPEID_DECLARE(ZFTokenForKeyValueContainer, ZFTokenForKeyValueContainer)
ZFOUTPUT_TYPE(ZFTokenForKeyValueContainer, {output.execute(ZFTokenForKeyValueContainerToString(v));})
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTokenForKeyValueContainer_tokenLeft "tokenLeft"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTokenForKeyValueContainer_tokenRight "tokenRight"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTokenForKeyValueContainer_tokenSeparator "tokenSeparator"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTokenForKeyValueContainer_tokenPairLeft "tokenPairLeft"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTokenForKeyValueContainer_tokenPairRight "tokenPairRight"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTokenForKeyValueContainer_tokenPairSeparator "tokenPairSeparator"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTokenForKeyValueContainer_tokenKeyLeft "tokenKeyLeft"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTokenForKeyValueContainer_tokenKeyRight "tokenKeyRight"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTokenForKeyValueContainer_tokenValueLeft "tokenValueLeft"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTokenForKeyValueContainer_tokenValueRight "tokenValueRight"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTokenForKeyValueContainer_tokenEtc "tokenEtc"

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFFilterForZFClassType value="value" />
 * @endcode
 */
ZFTYPEID_DECLARE(ZFFilterForZFClassType, ZFFilterForZFClassType)
ZFOUTPUT_TYPE(ZFFilterForZFClassType, {output.execute(ZFFilterForZFClassTypeToString(v));})

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFMethodPrivilegeType value="value" />
 * @endcode
 */
ZFTYPEID_DECLARE(ZFMethodPrivilegeType, ZFMethodPrivilegeType)
ZFOUTPUT_TYPE(ZFMethodPrivilegeType, {output.execute(ZFMethodPrivilegeTypeToString(v));})

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFMethodType value="value" />
 * @endcode
 */
ZFTYPEID_DECLARE(ZFMethodType, ZFMethodType)
ZFOUTPUT_TYPE(ZFMethodType, {output.execute(ZFMethodTypeToString(v));})

ZFTYPEID_ACCESS_ONLY_DECLARE(ZFMethodParamDefaultValueCallback, ZFMethodParamDefaultValueCallback)

ZFTYPEID_ACCESS_ONLY_DECLARE(ZFMethodDynamicRegisterParam, ZFMethodDynamicRegisterParam)

ZFTYPEID_ACCESS_ONLY_DECLARE(ZFPropertyDynamicRegisterInitValueCallback, ZFPropertyDynamicRegisterInitValueCallback)
ZFTYPEID_ACCESS_ONLY_DECLARE(ZFPropertyDynamicRegisterParam, ZFPropertyDynamicRegisterParam)

ZFTYPEID_ACCESS_ONLY_DECLARE(ZFComparer_ZFObject, ZFComparer<ZFObject *>::Comparer)

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFObjectInstanceState value="value" />
 * @endcode
 */
ZFTYPEID_DECLARE(ZFObjectInstanceState, ZFObjectInstanceState)
ZFOUTPUT_TYPE(ZFObjectInstanceState, {output.execute(ZFObjectInstanceStateToString(v));})

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFCallbackType value="value" />
 * @endcode
 */
ZFTYPEID_DECLARE(ZFCallbackType, ZFCallbackType)
ZFOUTPUT_TYPE(ZFCallbackType, {output.execute(ZFCallbackTypeToString(v));})

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFSerializableData />
 * @endcode
 */
ZFTYPEID_DECLARE(ZFSerializableData, ZFSerializableData)
ZFOUTPUT_TYPE_DECLARE(ZFSerializableData)

// ============================================================
ZFTYPEID_ACCESS_ONLY_DECLARE(ZFFuncAddrType, ZFFuncAddrType)
ZFTYPEID_ACCESS_ONLY_DECLARE(zfiterator, zfiterator)
ZFTYPEID_ACCESS_ONLY_DECLARE(ZFBuffer, ZFBuffer)
ZFTYPEID_ACCESS_ONLY_DECLARE(ZFFilterForNumber, ZFFilterForNumber)
ZFTYPEID_ACCESS_ONLY_DECLARE(ZFFilterForIndex, ZFFilterForIndex)
ZFTYPEID_ACCESS_ONLY_DECLARE(ZFFilterForIdentity, ZFFilterForIdentity)
ZFTYPEID_ACCESS_ONLY_DECLARE(ZFFilterForString, ZFFilterForString)
ZFTYPEID_ACCESS_ONLY_DECLARE(ZFIdentityGenerator, ZFIdentityGenerator)
ZFTYPEID_ACCESS_ONLY_DECLARE(ZFListenerData, ZFListenerData)
ZFTYPEID_ACCESS_ONLY_DECLARE(ZFObserverAddParam, ZFObserverAddParam)
ZFTYPEID_ACCESS_ONLY_DECLARE(ZFObserverHolder, ZFObserverHolder)
ZFTYPEID_ACCESS_ONLY_DECLARE(ZFFilterForZFClass, ZFFilterForZFClass)
ZFTYPEID_ACCESS_ONLY_DECLARE(ZFFilterForZFMethod, ZFFilterForZFMethod)
ZFTYPEID_ACCESS_ONLY_DECLARE(ZFFilterForZFProperty, ZFFilterForZFProperty)
ZFTYPEID_ACCESS_ONLY_DECLARE(ZFFilterForZFObject, ZFFilterForZFObject)
ZFTYPEID_ACCESS_ONLY_DECLARE(ZFMethodGenericInvoker, ZFMethodGenericInvoker)

// ============================================================
// void *
ZFTYPEID_ACCESS_ONLY_DECLARE(VoidPointer, void *)
ZFTYPEID_ACCESS_ONLY_DECLARE(VoidPointerConst, const void *)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFTypeId_CoreType_h_

