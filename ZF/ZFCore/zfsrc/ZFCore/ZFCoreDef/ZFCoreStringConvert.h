/**
 * @file ZFCoreStringConvert.h
 * @brief utilities for ZFFramework
 */

#ifndef _ZFI_ZFCoreStringConvert_h_
#define _ZFI_ZFCoreStringConvert_h_

#include "ZFCoreTypeDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief check whether the toCompare matches the tokens, return the index in tokens or zfindexMax() if not matched
 */
extern ZF_ENV_EXPORT zfindex zfsCheckMatch(ZF_IN const zfchar **tokens,
                                           ZF_IN zfindex tokenCount,
                                           ZF_IN const zfchar *toCompare,
                                           ZF_IN_OPT zfindex toCompareLength = zfindexMax());

// ============================================================
/**
 * @brief util macro to declare type string converter for ZFFramework
 */
#define ZFCORETYPE_STRING_CONVERTER_DECLARE(TypeName, Type) \
    /** @brief util method to convert TypeName from string */ \
    extern ZF_ENV_EXPORT zfbool TypeName##FromString(ZF_OUT Type &v, \
                                                     ZF_IN const zfchar *src, \
                                                     ZF_IN_OPT zfindex srcLen = zfindexMax()); \
    /** @brief util method to convert TypeName to string */ \
    extern ZF_ENV_EXPORT zfbool TypeName##ToString(ZF_IN_OUT zfstring &s, ZF_IN Type const &v); \
    /** @brief util method to convert TypeName to string */ \
    inline zfstring TypeName##ToString(ZF_IN Type const &v) \
    { \
        zfstring s; \
        TypeName##ToString(s, v); \
        return s; \
    }
/**
 * @brief util macro to declare type string converter for ZFFramework
 *
 * @code
 *   ZFCORETYPE_STRING_CONVERTER_DEFINE(YourTypeName, YourType, {
 *           // proto type:
 *           //   zfbool YourTypeNameFromString(ZF_OUT YourType &v,
 *           //       ZF_IN const zfchar *src,
 *           //       ZF_IN_OPT zfindex srcLen = zfindexMax());
 *       }, {
 *           // proto type:
 *           //   zfbool YourTypeNameToString(ZF_IN_OUT zfstring &s,
 *           //       ZF_IN YourType const &v);
 *           // append result to s
 *       })
 * @endcode
 */
#define ZFCORETYPE_STRING_CONVERTER_DEFINE(TypeName, Type, convertFromStringAction, convertToStringAction) \
    zfbool TypeName##FromString(ZF_OUT Type &v, \
                                ZF_IN const zfchar *src, \
                                ZF_IN_OPT zfindex srcLen /* = zfindexMax() */) \
    { \
        convertFromStringAction \
    } \
    zfbool TypeName##ToString(ZF_IN_OUT zfstring &s, ZF_IN Type const &v) \
    { \
        convertToStringAction \
    }

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreStringConvert_h_

