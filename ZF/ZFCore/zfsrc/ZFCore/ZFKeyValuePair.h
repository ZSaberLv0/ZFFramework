/**
 * @file ZFKeyValuePair.h
 * @brief ZFObject key value pair
 */

#ifndef _ZFI_ZFKeyValuePair_h_
#define _ZFI_ZFKeyValuePair_h_

#include "ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief key value pair as POD type, no auto retain logic, designed for performance
 */
zffinal zfclassPOD ZF_ENV_EXPORT ZFKeyValuePair
{
public:
    ZFObject *key; /**< @brief key, no auto retain */
    ZFObject *value; /**< @brief value, no auto retain */
};
/**
 * @brief make a ZFKeyValuePair
 */
inline ZFKeyValuePair ZFKeyValuePairMake(ZF_IN ZFObject *key,
                                         ZF_IN ZFObject *value)
{
    ZFKeyValuePair ret = {key, value};
    return ret;
}
ZFCORE_POD_COMPARER_DECLARE(ZFKeyValuePair)

/**
 * @brief an empty pair
 */
ZFEXPORT_VAR_READONLY_DECLARE(ZFKeyValuePair, ZFKeyValuePairZero)

// ============================================================
/**
 * @brief key value pair with auto retain logic
 */
zffinal zfclassLikePOD ZF_ENV_EXPORT ZFKeyValuePairHolder
{
public:
    zfautoObject key; /**< @brief key */
    zfautoObject value; /**< @brief value */
public:
    /** @cond ZFPrivateDoc */
    ZFKeyValuePairHolder(void) : key(), value() {}
    ZFKeyValuePairHolder(ZF_IN ZFObject *key, ZF_IN ZFObject *value) : key(key), value(value) {}
    ZFKeyValuePairHolder(ZF_IN ZFKeyValuePairHolder const &ref) : key(ref.key), value(ref.value) {}
    ZFKeyValuePairHolder(ZF_IN ZFKeyValuePair const &ref) : key(ref.key), value(ref.value) {}
    zfbool operator == (ZF_IN ZFKeyValuePairHolder const &ref) const {return (this->key == ref.key && this->value == ref.value);}
    zfbool operator != (ZF_IN ZFKeyValuePairHolder const &ref) const {return (this->key != ref.key || this->value != ref.value);}
    zfbool operator == (ZF_IN ZFKeyValuePair const &ref) const {return (this->key == ref.key && this->value == ref.value);}
    zfbool operator != (ZF_IN ZFKeyValuePair const &ref) const {return (this->key != ref.key || this->value != ref.value);}
    ZFKeyValuePairHolder &operator = (ZF_IN ZFKeyValuePairHolder const &ref) {this->key = ref.key; this->value = ref.value; return *this;}
    ZFKeyValuePairHolder &operator = (ZF_IN ZFKeyValuePair const &ref) {this->key = ref.key; this->value = ref.value; return *this;}
    operator ZFKeyValuePair (void) const {return ZFKeyValuePairMake(this->key, this->value);}
    /** @endcond */
};

// ============================================================
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFKeyValuePair_key "key"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFKeyValuePair_value "value"

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFKeyValuePair>
 *       <ObjectType name="key" /> // optional
 *       <ObjectType name="value" /> // optional
 *   </ZFKeyValuePair>
 * @endcode
 */
ZFTYPEID_DECLARE(ZFKeyValuePairHolder, ZFKeyValuePairHolder)
ZFTYPEID_ALIAS_DECLARE(ZFKeyValuePairHolder, ZFKeyValuePairHolder, ZFKeyValuePair, ZFKeyValuePair)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFKeyValuePair_h_

