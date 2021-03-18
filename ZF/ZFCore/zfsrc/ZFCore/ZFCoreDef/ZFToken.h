/**
 * @file ZFToken.h
 * @brief string token util
 */

#ifndef _ZFI_ZFToken_h_
#define _ZFI_ZFToken_h_

#include "ZFCoreTypeDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief util class to hold string tokens
 */
zfclassLikePOD ZF_ENV_EXPORT ZFTokenForContainer
{
public:
    zfstring tokenLeft; /**< @brief "[" by default */
    zfstring tokenRight; /**< @brief "]" by default */
    zfstring tokenSeparator; /**< @brief ", " by default */
    zfstring tokenValueLeft; /**< @brief "" by default */
    zfstring tokenValueRight; /**< @brief "" by default */
    zfstring tokenEtc; /**< @brief "..." by default */

public:
    /** @cond ZFPrivateDoc */
    ZFTokenForContainer(void)
    : tokenLeft("[")
    , tokenRight("]")
    , tokenSeparator(", ")
    , tokenValueLeft("")
    , tokenValueRight("")
    , tokenEtc("...")
    {
    }
    ZFTokenForContainer(
          ZF_IN zfstring const &tokenLeft
        , ZF_IN zfstring const &tokenRight
        , ZF_IN zfstring const &tokenSeparator
        , ZF_IN zfstring const &tokenValueLeft
        , ZF_IN zfstring const &tokenValueRight
        , ZF_IN zfstring const &tokenEtc
        )
    : tokenLeft(tokenLeft)
    , tokenRight(tokenRight)
    , tokenSeparator(tokenSeparator)
    , tokenValueLeft(tokenValueLeft)
    , tokenValueRight(tokenValueRight)
    , tokenEtc(tokenEtc)
    {
    }
    zfbool operator == (ZF_IN const ZFTokenForContainer &ref) const
    {
        return (this->tokenLeft.compare(ref.tokenLeft) == 0
                && this->tokenRight.compare(ref.tokenRight) == 0
                && this->tokenSeparator.compare(ref.tokenSeparator) == 0
                && this->tokenValueLeft.compare(ref.tokenValueLeft) == 0
                && this->tokenValueRight.compare(ref.tokenValueRight) == 0
                && this->tokenEtc.compare(ref.tokenEtc) == 0
            );
    }
    inline zfbool operator != (ZF_IN const ZFTokenForContainer &ref) const {return !this->operator == (ref);}
    /** @endcond */
};
extern ZF_ENV_EXPORT const ZFTokenForContainer _ZFP_ZFTokenForContainerTrim;
extern ZF_ENV_EXPORT const ZFTokenForContainer _ZFP_ZFTokenForContainerDetail;
extern ZF_ENV_EXPORT const ZFTokenForContainer _ZFP_ZFTokenForContainerPlainList;
extern ZF_ENV_EXPORT ZFTokenForContainer _ZFP_ZFTokenForContainerDefault;
/**
 * @brief see #ZFTokenForContainer
 *
 * output format:
 * @code
 *   [e0, e1, e2, ...]
 * @endcode
 */
#define ZFTokenForContainerTrim() _ZFP_ZFTokenForContainerTrim
/**
 * @brief see #ZFTokenForContainer
 *
 * output format:
 * @code
 *   [
 *       e0,
 *       e1,
 *       e2,
 *       ...
 *   ]
 * @endcode
 */
#define ZFTokenForContainerDetail() _ZFP_ZFTokenForContainerDetail
/**
 * @brief see #ZFTokenForContainer
 *
 * output format:
 * @code
 *   e0
 *   e1
 *   e2
 *   ...
 * @endcode
 */
#define ZFTokenForContainerPlainList() _ZFP_ZFTokenForContainerPlainList
/**
 * @brief see #ZFTokenForContainer, modifyable, #ZFTokenForContainerTrim by default
 */
#define ZFTokenForContainerDefault() _ZFP_ZFTokenForContainerDefault

// ============================================================
/**
 * @brief util class to hold string tokens
 */
zfclassLikePOD ZF_ENV_EXPORT ZFTokenForKeyValueContainer
{
public:
    zfstring tokenLeft; /**< @brief "{" by default */
    zfstring tokenRight; /**< @brief "}" by default */
    zfstring tokenSeparator; /**< @brief ", " by default */
    zfstring tokenPairLeft; /**< @brief "" by default */
    zfstring tokenPairRight; /**< @brief "" by default */
    zfstring tokenPairSeparator; /**< @brief " = " by default */
    zfstring tokenKeyLeft; /**< @brief "" by default */
    zfstring tokenKeyRight; /**< @brief "" by default */
    zfstring tokenValueLeft; /**< @brief "" by default */
    zfstring tokenValueRight; /**< @brief "" by default */
    zfstring tokenEtc; /**< @brief "..." by default */

public:
    /** @cond ZFPrivateDoc */
    ZFTokenForKeyValueContainer(void)
    : tokenLeft("{")
    , tokenRight("}")
    , tokenSeparator(", ")
    , tokenPairLeft("")
    , tokenPairRight("")
    , tokenPairSeparator(" = ")
    , tokenKeyLeft("")
    , tokenKeyRight("")
    , tokenValueLeft("")
    , tokenValueRight("")
    , tokenEtc("...")
    {
    }
    ZFTokenForKeyValueContainer(
          ZF_IN zfstring const &tokenLeft
        , ZF_IN zfstring const &tokenRight
        , ZF_IN zfstring const &tokenSeparator
        , ZF_IN zfstring const &tokenPairLeft
        , ZF_IN zfstring const &tokenPairRight
        , ZF_IN zfstring const &tokenPairSeparator
        , ZF_IN zfstring const &tokenKeyLeft
        , ZF_IN zfstring const &tokenKeyRight
        , ZF_IN zfstring const &tokenValueLeft
        , ZF_IN zfstring const &tokenValueRight
        , ZF_IN zfstring const &tokenEtc
        )
    : tokenLeft(tokenLeft)
    , tokenRight(tokenRight)
    , tokenSeparator(tokenSeparator)
    , tokenPairLeft(tokenPairLeft)
    , tokenPairRight(tokenPairRight)
    , tokenPairSeparator(tokenPairSeparator)
    , tokenKeyLeft(tokenKeyLeft)
    , tokenKeyRight(tokenKeyRight)
    , tokenValueLeft(tokenValueLeft)
    , tokenValueRight(tokenValueRight)
    , tokenEtc(tokenEtc)
    {
    }
    zfbool operator == (ZF_IN const ZFTokenForKeyValueContainer &ref) const
    {
        return (this->tokenLeft.compare(ref.tokenLeft) == 0
                && this->tokenRight.compare(ref.tokenRight) == 0
                && this->tokenSeparator.compare(ref.tokenSeparator) == 0
                && this->tokenPairLeft.compare(ref.tokenPairLeft) == 0
                && this->tokenPairRight.compare(ref.tokenPairRight) == 0
                && this->tokenPairSeparator.compare(ref.tokenPairSeparator) == 0
                && this->tokenKeyLeft.compare(ref.tokenKeyLeft) == 0
                && this->tokenKeyRight.compare(ref.tokenKeyRight) == 0
                && this->tokenValueLeft.compare(ref.tokenValueLeft) == 0
                && this->tokenValueRight.compare(ref.tokenValueRight) == 0
                && this->tokenEtc.compare(ref.tokenEtc) == 0
            );
    }
    inline zfbool operator != (ZF_IN const ZFTokenForKeyValueContainer &ref) const {return !this->operator == (ref);}
    /** @endcond */
};
extern ZF_ENV_EXPORT const ZFTokenForKeyValueContainer _ZFP_ZFTokenForKeyValueContainerTrim;
extern ZF_ENV_EXPORT const ZFTokenForKeyValueContainer _ZFP_ZFTokenForKeyValueContainerDetail;
extern ZF_ENV_EXPORT ZFTokenForKeyValueContainer _ZFP_ZFTokenForKeyValueContainerDefault;
/**
 * @brief see #ZFTokenForKeyValueContainer
 *
 * output format:
 * @code
 *   {k0 = v0, k1 = v1, ...}
 * @endcode
 */
#define ZFTokenForKeyValueContainerTrim() _ZFP_ZFTokenForKeyValueContainerTrim
/**
 * @brief see #ZFTokenForKeyValueContainer
 *
 * output format:
 * @code
 *   {
 *       k0 = v0,
 *       k1 = v1,
 *       ...
 *   }
 * @endcode
 */
#define ZFTokenForKeyValueContainerDetail() _ZFP_ZFTokenForKeyValueContainerDetail
/**
 * @brief see #ZFTokenForKeyValueContainer, modifyable, #ZFTokenForKeyValueContainerTrim by default
 */
#define ZFTokenForKeyValueContainerDefault() _ZFP_ZFTokenForKeyValueContainerDefault

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFToken_h_

