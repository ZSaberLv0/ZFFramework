/**
 * @file ZFJson.h
 * @brief JSON processing
 */

#ifndef _ZFI_ZFJson_h_
#define _ZFI_ZFJson_h_

#include "ZFAlgorithmDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/*
 * all types:
 *     JsonNull
 *     JsonValue
 *     JsonObject
 *     JsonArray
 *
 * here is a typical json data:
    "{"
    "    \"k0\" : \"v0\","
    ""
    "    \"object0\" : {},"
    "    \"object1\" : {"
    "        \"k0\" : \"v0\","
    "        \"k1\" : \"v1\""
    "    },"
    "    \"object2\" : {"
    "        \"k0\" : {\"k0\" : \"v0\"}"
    "    },"
    ""
    "    \"array0\" : [],"
    "    \"array1\" : ["
    "        {\"k0\" : \"v0\"},"
    "        {\"k0\" : \"v0\"}"
    "    ],"
    ""
    "    \"k1\" : \"v1\""
    "}"
 */

// ============================================================
// ZFJsonType
/**
 * @brief json item type
 */
ZFENUM_BEGIN(ZFJsonType)
    ZFENUM_VALUE(JsonNull)
    ZFENUM_VALUE(JsonValue)
    ZFENUM_VALUE(JsonObject)
    ZFENUM_VALUE(JsonArray)
ZFENUM_SEPARATOR(ZFJsonType)
    ZFENUM_VALUE_REGISTER(JsonNull)
    ZFENUM_VALUE_REGISTER(JsonValue)
    ZFENUM_VALUE_REGISTER(JsonObject)
    ZFENUM_VALUE_REGISTER(JsonArray)
ZFENUM_END(ZFJsonType)

// ============================================================
/**
 * @brief token for output json
 */
zfclassLikePOD ZF_ENV_EXPORT ZFJsonOutputToken
{
public:
    zfstring jsonNewLineToken; /**< @brief "\n" by default */
    zfstring jsonIndentToken; /**< @brief "  " by default */

    zfstring jsonValueSeparatorToken; /**< @brief " : " by default */
    zfstring jsonSeparatorToken; /**< @brief "," by default */
    zfstring jsonSeparatorInSameLineToken; /**< @brief ", " by default */

    zfstring jsonKeyTagLeft; /**< @brief "\"" by default */
    zfstring jsonKeyTagRight; /**< @brief "\"" by default */

    zfstring jsonValueTagLeft; /**< @brief "\"" by default */
    zfstring jsonValueTagRight; /**< @brief "\"" by default */

    zfstring jsonObjectTagLeft; /**< @brief "{" by default */
    zfstring jsonObjectTagRight; /**< @brief "}" by default */

    zfstring jsonArrayTagLeft; /**< @brief "[" by default */
    zfstring jsonArrayTagRight; /**< @brief "]" by default */

public:
    /** @cond ZFPrivateDoc */
    ZFJsonOutputToken(void)
    : jsonNewLineToken("\n")
    , jsonIndentToken("  ")
    , jsonValueSeparatorToken(" : ")
    , jsonSeparatorToken(",")
    , jsonSeparatorInSameLineToken(", ")
    , jsonKeyTagLeft("\"")
    , jsonKeyTagRight("\"")
    , jsonValueTagLeft("\"")
    , jsonValueTagRight("\"")
    , jsonObjectTagLeft("{")
    , jsonObjectTagRight("}")
    , jsonArrayTagLeft("[")
    , jsonArrayTagRight("]")
    {
    }
    zfbool operator == (ZF_IN ZFJsonOutputToken const &ref) const;
    zfbool operator != (ZF_IN ZFJsonOutputToken const &ref) const
    {
        return !this->operator == (ref);
    }
    /** @endcond */
};
/**
 * @brief flags to output json items
 */
zfclassLikePOD ZF_ENV_EXPORT ZFJsonOutputFlags
{
public:
    /**
     * @brief token to output json data
     */
    ZFJsonOutputToken jsonToken;

    /**
     * @brief strings added to head of each new line, empty by default
     */
    zfstring jsonGlobalLineBeginToken;

    /**
     * @brief whether add new line for content of a json object, true by default
     */
    zfbool jsonObjectAddNewLineForContent;
    /**
     * @brief whether to put right tag in same line if no content, true by default
     */
    zfbool jsonObjectTagInSameLineIfNoContent;

    /**
     * @brief whether add new line for content of a json array, true by default
     */
    zfbool jsonArrayAddNewLineForContent;
    /**
     * @brief whether trim element tag in same line for a json array,
     *   valid only if jsonArrayAddNewLineForContent is true,
     *   true by default
     */
    zfbool jsonArrayContentTagInSameLine;
    /**
     * @brief whether add new line for content of a json array, true by default
     */
    zfbool jsonArrayTagInSameLineIfNoContent;

public:
    /** @cond ZFPrivateDoc */
    ZFJsonOutputFlags(void)
    : jsonGlobalLineBeginToken()
    , jsonObjectAddNewLineForContent(zftrue)
    , jsonObjectTagInSameLineIfNoContent(zftrue)
    , jsonArrayAddNewLineForContent(zftrue)
    , jsonArrayContentTagInSameLine(zftrue)
    , jsonArrayTagInSameLineIfNoContent(zftrue)
    {
    }
    zfbool operator == (ZF_IN ZFJsonOutputFlags const &ref) const;
    zfbool operator != (ZF_IN ZFJsonOutputFlags const &ref) const
    {
        return !this->operator == (ref);
    }
    /** @endcond */
};
ZFTYPEID_ACCESS_ONLY_DECLARE(ZFJsonOutputFlags, ZFJsonOutputFlags)
/**
 * @brief default output flags for #ZFJsonItemToOutput
 */
ZFEXPORT_VAR_READONLY_DECLARE(ZFJsonOutputFlags, ZFJsonOutputFlagsDefault)
/**
 * @brief trim output flags for #ZFJsonItemToOutput
 */
ZFEXPORT_VAR_READONLY_DECLARE(ZFJsonOutputFlags, ZFJsonOutputFlagsTrim)

// ============================================================
// ZFJsonItem
zfclassFwd _ZFP_ZFJsonItemPrivate;
/**
 * @brief JSON item
 */
zfclassLikePOD ZF_ENV_EXPORT ZFJsonItem
{
    // ============================================================
public:
    /** @brief construct a null item */
    ZFJsonItem(void);
    /** @brief construct with specified type */
    ZFJsonItem(ZF_IN ZFJsonTypeEnum jsonType);
    /** @brief retain from other item */
    ZFJsonItem(ZF_IN const ZFJsonItem &ref);
    virtual ~ZFJsonItem(void);

    /** @cond ZFPrivateDoc */
    ZFJsonItem &operator = (ZF_IN const ZFJsonItem &ref);
    zfbool operator == (ZF_IN const ZFJsonItem &ref) const;
    zfbool operator != (ZF_IN const ZFJsonItem &ref) const {return !(this->operator == (ref));}
    /** @endcond */
private:
    zfbool operator == (ZF_IN const zfchar *ref) const;
    zfbool operator != (ZF_IN const zfchar *ref) const;
    zfbool operator == (ZF_IN zfint ref) const;
    zfbool operator != (ZF_IN zfint ref) const;

    // ============================================================
public:
    /** @brief see #objectInfo */
    void objectInfoT(ZF_IN_OUT zfstring &ret) const;
    /** @brief return object info */
    inline zfstring objectInfo(void) const
    {
        zfstring ret;
        this->objectInfoT(ret);
        return ret;
    }

public:
    /**
     * @brief get current retain count
     */
    zfindex objectRetainCount(void) const;

    // ============================================================
public:
    /**
     * @brief type of this item
     */
    ZFJsonTypeEnum jsonType(void) const;
    /**
     * @brief true if #jsonType is #ZFJsonType::e_JsonNull
     */
    inline zfbool jsonIsNull(void) const
    {
        return (this->jsonType() == ZFJsonType::e_JsonNull);
    }

    /**
     * @brief access parent of this node
     */
    ZFJsonItem jsonParent(void) const;

    // ============================================================
    // for value type
public:
    /**
     * @brief key of the node, valid only if this node belongs to an object node
     */
    const zfchar *jsonKey(void) const;
    /**
     * @brief value of the node, valid only for value type
     */
    void jsonValue(ZF_IN const zfchar *value);
    /**
     * @brief see #jsonValue
     */
    const zfchar *jsonValue(void) const;

    zffinal void _ZFP_ZFJson_jsonMemoryPool_jsonValue(ZF_IN const zfchar *value, ZF_IN void *token);
    zffinal void _ZFP_ZFJson_jsonMemoryPool_jsonItem(ZF_IN const zfchar *key, ZF_IN void *token, ZF_IN const ZFJsonItem &jsonItem);

    // ============================================================
public:
    /**
     * @brief return a copy of this node as well as all of its content
     *
     * the newly created tree has no parent
     */
    ZFJsonItem jsonCloneTree(void) const;

    // ============================================================
    // for object type
public:
    /**
     * @brief get json item for key
     */
    ZFJsonItem jsonItem(ZF_IN const zfchar *key) const;
    /**
     * @brief set json item for key
     */
    void jsonItem(ZF_IN const zfchar *key,
                  ZF_IN const ZFJsonItem &jsonItem);

    /**
     * @brief util method to access json value, return a null type item if no such item or not value type
     */
    const zfchar *jsonItemValue(ZF_IN const zfchar *key) const;
    /**
     * @brief util method to add a json value item
     */
    void jsonItemValue(ZF_IN const zfchar *key, ZF_IN const zfchar *value);

    /**
     * @brief remove json item
     */
    void jsonItemRemove(ZF_IN const ZFJsonItem &jsonItem);
    /**
     * @brief remove json item
     */
    void jsonItemRemove(ZF_IN const zfchar *key);
    /**
     * @brief remove all json item
     */
    void jsonItemRemoveAll(void);

    /**
     * @brief json item count
     */
    zfindex jsonItemCount(void) const;

public:
    /** @brief see #zfiterator */
    zfiterator jsonItemIterator(void) const;
    /** @brief see #zfiterator */
    zfiterator jsonItemIteratorForKey(ZF_IN const zfchar *key) const;
    /** @brief see #zfiterator */
    zfbool jsonItemIteratorIsValid(ZF_IN const zfiterator &it) const;
    /** @brief see #zfiterator */
    zfbool jsonItemIteratorIsEqual(ZF_IN const zfiterator &it0,
                                   ZF_IN const zfiterator &it1) const;
    /** @brief see #zfiterator */
    void jsonItemIteratorValue(ZF_IN_OUT zfiterator &it, ZF_IN const ZFJsonItem &jsonItem);
    /** @brief see #zfiterator */
    void jsonItemIteratorRemove(ZF_IN_OUT zfiterator &it);
    /** @brief see #zfiterator */
    const zfchar *jsonItemIteratorKey(ZF_IN const zfiterator &it) const;
    /** @brief see #zfiterator */
    ZFJsonItem jsonItemIteratorValue(ZF_IN const zfiterator &it) const;
    /** @brief see #zfiterator */
    const zfchar *jsonItemIteratorNextKey(ZF_IN_OUT zfiterator &it) const;
    /** @brief see #zfiterator */
    ZFJsonItem jsonItemIteratorNextValue(ZF_IN_OUT zfiterator &it) const;

    // ============================================================
    // for array type
public:
    /**
     * @brief json object count
     */
    zfindex jsonObjectCount(void) const;
    /**
     * @brief get json object at index
     */
    ZFJsonItem jsonObjectAtIndex(ZF_IN zfindex index) const;

public:
    /**
     * @brief add json object to specified index (ranged in [0, count])
     */
    void jsonObjectAdd(ZF_IN const ZFJsonItem &jsonObject,
                       ZF_IN_OPT zfindex atIndex = zfindexMax());
    /**
     * @brief remove json object
     */
    void jsonObjectRemove(ZF_IN const ZFJsonItem &jsonObject);
    /**
     * @brief remove json object at index
     */
    void jsonObjectRemoveAtIndex(ZF_IN zfindex index);
    /**
     * @brief remove all json object
     */
    void jsonObjectRemoveAll(void);
    /**
     * @brief find json object
     */
    zfindex jsonObjectFind(ZF_IN const ZFJsonItem &jsonObject) const;

    // ============================================================
    // quick access
public:
    /** @brief access #jsonValue */
    operator const zfchar * (void) const {return this->jsonValue();}
    /** @brief access #jsonItem */
    inline ZFJsonItem operator [] (ZF_IN const zfchar *key) const {return this->jsonItem(key);}
    /** @brief access #jsonObjectAtIndex */
    inline ZFJsonItem operator [] (ZF_IN zfindex const &jsonObjectIndex) const {return this->jsonObjectAtIndex(jsonObjectIndex);}

private:
    _ZFP_ZFJsonItemPrivate *d;
    friend zfclassFwd _ZFP_ZFJsonItemPrivate;
private:
    ZFJsonItem(ZF_IN _ZFP_ZFJsonItemPrivate *ref);
};
ZFTYPEID_DECLARE(ZFJsonItem, ZFJsonItem)
ZFOUTPUT_TYPE(ZFJsonItem, {output << v.objectInfo();})

/** @brief util to create #ZFXmlItem */
ZFMETHOD_FUNC_DECLARE_0(ZFJsonItem, ZFJsonValue)
/** @brief util to create #ZFXmlItem */
ZFMETHOD_FUNC_DECLARE_0(ZFJsonItem, ZFJsonObject)
/** @brief util to create #ZFXmlItem */
ZFMETHOD_FUNC_DECLARE_0(ZFJsonItem, ZFJsonArray)

// ============================================================
/**
 * @brief parse json, or return an item with null type if fail
 */
ZFMETHOD_FUNC_DECLARE_1(ZFJsonItem, ZFJsonItemFromInput, ZFMP_IN(const ZFInput &, input))
/**
 * @brief parse json, or return an item with null type if fail
 */
ZFMETHOD_FUNC_DECLARE_2(ZFJsonItem, ZFJsonItemFromString,
                        ZFMP_IN(const zfchar *, src),
                        ZFMP_IN_OPT(zfindex, length, zfindexMax()))

/**
 * @brief convert json to output
 *
 * @note result string is not ensured to be a valid json string
 *   if source is not valid
 */
ZFMETHOD_FUNC_DECLARE_3(zfbool, ZFJsonItemToOutput,
                        ZFMP_IN_OUT(const ZFOutput &, output),
                        ZFMP_IN(const ZFJsonItem &, jsonItem),
                        ZFMP_IN_OPT(const ZFJsonOutputFlags &, outputFlags, ZFJsonOutputFlagsDefault()))
/**
 * @brief convert json to string
 *
 * @note result string is not ensured to be a valid json string
 *   if source is not valid
 */
ZFMETHOD_FUNC_DECLARE_3(zfbool, ZFJsonItemToString,
                        ZFMP_IN_OUT(zfstring &, ret),
                        ZFMP_IN(const ZFJsonItem &, jsonItem),
                        ZFMP_IN(const ZFJsonOutputFlags &, outputFlags))
/** @brief see #ZFJsonItemToString */
ZFMETHOD_FUNC_DECLARE_2(zfstring, ZFJsonItemToString,
                        ZFMP_IN(const ZFJsonItem &, jsonItem),
                        ZFMP_IN(const ZFJsonOutputFlags &, outputFlags))

// ============================================================
// escape chars
/**
 * @brief encode json chars
 *
 * @code
 *   \\b        \b
 *   \\t        \t
 *   \\n        \n
 *   \\f        \f
 *   \\r        \r
 *   \"         "
 *   \\         \
 * @endcode
 *
 * result would be appended to dst
 */
ZFMETHOD_FUNC_DECLARE_3(void, ZFJsonEscapeCharEncode,
                        ZFMP_OUT(zfstring &, dst),
                        ZFMP_IN(const zfchar *, src),
                        ZFMP_IN_OPT(zfindex, count, zfindexMax()))
/**
 * @brief see #ZFJsonEscapeCharEncode
 */
ZFMETHOD_FUNC_DECLARE_3(void, ZFJsonEscapeCharEncode,
                        ZFMP_OUT(const ZFOutput &, dst),
                        ZFMP_IN(const zfchar *, src),
                        ZFMP_IN_OPT(zfindex, count, zfindexMax()))

/**
 * @brief see #ZFJsonEscapeCharEncode
 */
ZFMETHOD_FUNC_DECLARE_3(void, ZFJsonEscapeCharDecode,
                        ZFMP_OUT(zfstring &, dst),
                        ZFMP_IN(const zfchar *, src),
                        ZFMP_IN_OPT(zfindex, count, zfindexMax()))
/**
 * @brief see #ZFJsonEscapeCharEncode
 */
ZFMETHOD_FUNC_DECLARE_3(void, ZFJsonEscapeCharDecode,
                        ZFMP_OUT(const ZFOutput &, dst),
                        ZFMP_IN(const zfchar *, src),
                        ZFMP_IN_OPT(zfindex, count, zfindexMax()))

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFJson_h_

