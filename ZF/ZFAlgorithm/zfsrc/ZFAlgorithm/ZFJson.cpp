#include "ZFJson.h"
#include "protocol/ZFProtocolZFJson.h"

#include "ZFCore/ZFSTLWrapper/zfstl_deque.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFENUM_DEFINE(ZFJsonType)

// ============================================================
zfbool ZFJsonOutputToken::operator == (ZF_IN ZFJsonOutputToken const &ref) const
{
    return (zftrue
            && this->jsonNewLineToken == ref.jsonNewLineToken
            && this->jsonIndentToken == ref.jsonIndentToken
            && this->jsonValueSeparatorToken == ref.jsonValueSeparatorToken
            && this->jsonSeparatorToken == ref.jsonSeparatorToken
            && this->jsonSeparatorInSameLineToken == ref.jsonSeparatorInSameLineToken
            && this->jsonKeyTagLeft == ref.jsonKeyTagLeft
            && this->jsonKeyTagRight == ref.jsonKeyTagRight
            && this->jsonValueTagLeft == ref.jsonValueTagLeft
            && this->jsonValueTagRight == ref.jsonValueTagRight
            && this->jsonObjectTagLeft == ref.jsonObjectTagLeft
            && this->jsonObjectTagRight == ref.jsonObjectTagRight
            && this->jsonArrayTagLeft == ref.jsonArrayTagLeft
            && this->jsonArrayTagRight == ref.jsonArrayTagRight
        );
}
zfbool ZFJsonOutputFlags::operator == (ZF_IN ZFJsonOutputFlags const &ref) const
{
    return (zftrue
            && this->jsonToken == ref.jsonToken
            && this->jsonGlobalLineBeginToken == ref.jsonGlobalLineBeginToken
            && this->jsonObjectAddNewLineForContent == ref.jsonObjectAddNewLineForContent
            && this->jsonObjectTagInSameLineIfNoContent == ref.jsonObjectTagInSameLineIfNoContent
            && this->jsonArrayAddNewLineForContent == ref.jsonArrayAddNewLineForContent
            && this->jsonArrayContentTagInSameLine == ref.jsonArrayContentTagInSameLine
            && this->jsonArrayTagInSameLineIfNoContent == ref.jsonArrayTagInSameLineIfNoContent
        );
}
ZFTYPEID_ACCESS_ONLY_DEFINE(ZFJsonOutputFlags, ZFJsonOutputFlags)

// ============================================================
ZFEXPORT_VAR_READONLY_DEFINE(ZFJsonOutputFlags, ZFJsonOutputFlagsDefault, ZFJsonOutputFlags())

static const ZFJsonOutputFlags &_ZFP_ZFJsonOutputFlagsTrimInit(void)
{
    static ZFJsonOutputFlags d;
    d.jsonToken.jsonNewLineToken.removeAll();
    d.jsonToken.jsonIndentToken.removeAll();
    d.jsonToken.jsonValueSeparatorToken = ":";
    d.jsonToken.jsonSeparatorInSameLineToken = ",";
    d.jsonObjectAddNewLineForContent = zffalse;
    d.jsonArrayAddNewLineForContent = zffalse;
    return d;
}
ZFEXPORT_VAR_READONLY_DEFINE(ZFJsonOutputFlags, ZFJsonOutputFlagsTrim, _ZFP_ZFJsonOutputFlagsTrimInit())

// ============================================================
static void _ZFP_ZFJsonItemToOutput_outputIndent(ZF_IN_OUT const ZFOutput &output,
                                                 ZF_IN const ZFJsonOutputFlags &outputFlags,
                                                 ZF_IN zfindex indentLevel)
{
    for(zfindex i = 0; i < indentLevel; ++i)
    {
        output << outputFlags.jsonToken.jsonIndentToken;
    }
}
static void _ZFP_ZFJsonItemToOutput_output(ZF_IN_OUT const ZFOutput &output,
                                           ZF_IN const ZFJsonItem &jsonItem,
                                           ZF_IN const ZFJsonOutputFlags &outputFlags,
                                           ZF_IN zfindex indentLevel);
static void _ZFP_ZFJsonItemToOutput_output_JsonValue(ZF_IN_OUT const ZFOutput &output,
                                                     ZF_IN const ZFJsonItem &jsonValue,
                                                     ZF_IN const ZFJsonOutputFlags &outputFlags,
                                                     ZF_IN zfindex indentLevel)
{
    output << outputFlags.jsonToken.jsonValueTagLeft;
    ZFJsonEscapeCharEncode(output, jsonValue.jsonValue());
    output << outputFlags.jsonToken.jsonValueTagRight;
}
static void _ZFP_ZFJsonItemToOutput_output_JsonObject(ZF_IN_OUT const ZFOutput &output,
                                                      ZF_IN const ZFJsonItem &jsonObject,
                                                      ZF_IN const ZFJsonOutputFlags &outputFlags,
                                                      ZF_IN zfindex indentLevel)
{
    output << outputFlags.jsonToken.jsonObjectTagLeft;
    zfbool first = zftrue;
    for(zfiterator it = jsonObject.jsonItemIterator(); jsonObject.jsonItemIteratorIsValid(it); jsonObject.jsonItemIteratorNextValue(it))
    {
        if(outputFlags.jsonObjectAddNewLineForContent)
        {
            if(first)
            {
                first = zffalse;
            }
            else
            {
                output << outputFlags.jsonToken.jsonSeparatorToken;
            }
            output << outputFlags.jsonToken.jsonNewLineToken << outputFlags.jsonGlobalLineBeginToken;
            _ZFP_ZFJsonItemToOutput_outputIndent(output, outputFlags, indentLevel + 1);
        }
        else
        {
            if(first)
            {
                first = zffalse;
            }
            else
            {
                output << outputFlags.jsonToken.jsonSeparatorInSameLineToken;
            }
        }
        output
            << outputFlags.jsonToken.jsonKeyTagLeft
            << jsonObject.jsonItemIteratorKey(it)
            << outputFlags.jsonToken.jsonKeyTagRight;
        output << outputFlags.jsonToken.jsonValueSeparatorToken;
        _ZFP_ZFJsonItemToOutput_output(output, jsonObject.jsonItemIteratorValue(it), outputFlags, indentLevel + 1);
    }
    if(outputFlags.jsonObjectAddNewLineForContent
        && !(outputFlags.jsonObjectTagInSameLineIfNoContent && jsonObject.jsonItemCount() == 0))
    {
        output << outputFlags.jsonToken.jsonNewLineToken << outputFlags.jsonGlobalLineBeginToken;
        _ZFP_ZFJsonItemToOutput_outputIndent(output, outputFlags, indentLevel);
    }
    output << outputFlags.jsonToken.jsonObjectTagRight;
}
static void _ZFP_ZFJsonItemToOutput_output_JsonArray(ZF_IN_OUT const ZFOutput &output,
                                                     ZF_IN const ZFJsonItem &jsonArray,
                                                     ZF_IN const ZFJsonOutputFlags &outputFlags,
                                                     ZF_IN zfindex indentLevel)
{
    output << outputFlags.jsonToken.jsonArrayTagLeft;
    for(zfindex i = 0; i < jsonArray.jsonObjectCount(); ++i)
    {
        if(outputFlags.jsonArrayAddNewLineForContent)
        {
            if(i != 0)
            {
                if(outputFlags.jsonArrayContentTagInSameLine)
                {
                    output << outputFlags.jsonToken.jsonSeparatorInSameLineToken;
                }
                else
                {
                    output << outputFlags.jsonToken.jsonSeparatorToken;
                }
            }

            if(!outputFlags.jsonArrayContentTagInSameLine)
            {
                output << outputFlags.jsonToken.jsonNewLineToken << outputFlags.jsonGlobalLineBeginToken;
                _ZFP_ZFJsonItemToOutput_outputIndent(output, outputFlags, indentLevel);
            }
            _ZFP_ZFJsonItemToOutput_output(output, jsonArray.jsonObjectAtIndex(i), outputFlags, indentLevel + 1);
        }
        else
        {
            if(i != 0)
            {
                output << outputFlags.jsonToken.jsonSeparatorInSameLineToken;
            }
            _ZFP_ZFJsonItemToOutput_output(output, jsonArray.jsonObjectAtIndex(i), outputFlags, indentLevel + 1);
        }
    }
    if(outputFlags.jsonArrayAddNewLineForContent
        && !(outputFlags.jsonArrayTagInSameLineIfNoContent && jsonArray.jsonObjectCount() == 0))
    {
        output << outputFlags.jsonToken.jsonNewLineToken << outputFlags.jsonGlobalLineBeginToken;
        _ZFP_ZFJsonItemToOutput_outputIndent(output, outputFlags, indentLevel);
    }
    output << outputFlags.jsonToken.jsonArrayTagRight;
}
static void _ZFP_ZFJsonItemToOutput_output(ZF_IN_OUT const ZFOutput &output,
                                           ZF_IN const ZFJsonItem &jsonItem,
                                           ZF_IN const ZFJsonOutputFlags &outputFlags,
                                           ZF_IN zfindex indentLevel)
{
    switch(jsonItem.jsonType())
    {
        case ZFJsonType::e_JsonValue:
            _ZFP_ZFJsonItemToOutput_output_JsonValue(output, jsonItem, outputFlags, indentLevel);
            break;
        case ZFJsonType::e_JsonObject:
            _ZFP_ZFJsonItemToOutput_output_JsonObject(output, jsonItem, outputFlags, indentLevel);
            break;
        case ZFJsonType::e_JsonArray:
            _ZFP_ZFJsonItemToOutput_output_JsonArray(output, jsonItem, outputFlags, indentLevel);
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            break;
    }
}

// ============================================================
zfclassLikePOD _ZFP_ZFJsonMemoryPoolString
{
private:
    void *_token;
    union {
        zfchar *_s;
        const zfchar *_sInPool;
    } _s;

private:
    _ZFP_ZFJsonMemoryPoolString(ZF_IN const _ZFP_ZFJsonMemoryPoolString &ref);
    _ZFP_ZFJsonMemoryPoolString &operator = (ZF_IN const _ZFP_ZFJsonMemoryPoolString &ref);
public:
    _ZFP_ZFJsonMemoryPoolString(void)
    : _token(zfnull)
    , _s()
    {
    }
    _ZFP_ZFJsonMemoryPoolString(ZF_IN const zfchar *s)
    : _token(zfnull)
    , _s()
    {
        _s._s = zfsCopy(s);
    }
    _ZFP_ZFJsonMemoryPoolString(ZF_IN const zfchar *s, ZF_IN void *token)
    : _token(token)
    , _s()
    {
        _s._sInPool = s;
    }
    ~_ZFP_ZFJsonMemoryPoolString(void)
    {
        if(_token)
        {
            ZFPROTOCOL_ACCESS(ZFJson)->jsonMemoryPoolRelease(_token, _s._sInPool);
        }
        else
        {
            zffree(_s._s);
        }
    }

public:
    void value(ZF_IN const zfchar *value)
    {
        if(_token)
        {
            ZFPROTOCOL_ACCESS(ZFJson)->jsonMemoryPoolRelease(_token, _s._sInPool);
            _token = zfnull;
            _s._s = zfnull;
        }
        zfsChange(_s._s, value);
    }
    void value(ZF_IN const zfchar *value, ZF_IN void *token)
    {
        if(_token)
        {
            ZFPROTOCOL_ACCESS(ZFJson)->jsonMemoryPoolRelease(_token, _s._sInPool);
            _token = zfnull;
            _s._s = zfnull;
        }
        else
        {
            zffree(_s._s);
        }
        _token = token;
        _s._sInPool = value;
    }
    inline const zfchar *value(void) const
    {
        return _s._sInPool;
    }
};
zfclassNotPOD _ZFP_ZFJsonItemData
{
public:
    _ZFP_ZFJsonMemoryPoolString jsonKey;
    ZFJsonItem jsonItem;
public:
    _ZFP_ZFJsonItemData(ZF_IN const zfchar *jsonKey,
                        ZF_IN const ZFJsonItem &jsonItem)
    : jsonKey(jsonKey)
    , jsonItem(jsonItem)
    {
    }
    _ZFP_ZFJsonItemData(ZF_IN const zfchar *jsonKey,
                        ZF_IN void *jsonKeyToken,
                        ZF_IN const ZFJsonItem &jsonItem)
    : jsonKey(jsonKey, jsonKeyToken)
    , jsonItem(jsonItem)
    {
    }
};
typedef zfstldeque<_ZFP_ZFJsonItemData *> _ZFP_ZFJsonItemMapType;
typedef zfstldeque<ZFJsonItem> _ZFP_ZFJsonObjectArrayType;
zfclassNotPOD _ZFP_ZFJsonItemPrivate
{
public:
    zfuint refCount;
    ZFJsonTypeEnum jsonType;
    _ZFP_ZFJsonItemPrivate *jsonParent;
    const zfchar *jsonKey;
    _ZFP_ZFJsonMemoryPoolString jsonValue;
    _ZFP_ZFJsonItemMapType jsonItemMap;
    _ZFP_ZFJsonObjectArrayType jsonObjectArray;

public:
    _ZFP_ZFJsonItemPrivate(void)
    : refCount(1)
    , jsonType(ZFJsonType::e_JsonNull)
    , jsonParent(zfnull)
    , jsonKey()
    , jsonValue()
    , jsonItemMap()
    , jsonObjectArray()
    {
    }
    ~_ZFP_ZFJsonItemPrivate(void)
    {
        this->jsonItemRemoveAll();
        this->jsonObjectRemoveAll();
    }

public:
    void jsonItemRemoveAll(void)
    {
        for(zfstlsize i = 0; i < this->jsonItemMap.size(); ++i)
        {
            this->jsonItemMap[i]->jsonItem.d->jsonParent = zfnull;
            zfdelete(this->jsonItemMap[i]);
        }
        this->jsonItemMap.clear();
    }
    void jsonObjectRemoveAll(void)
    {
        for(zfstlsize i = 0; i < this->jsonObjectArray.size(); ++i)
        {
            this->jsonObjectArray[i].d->jsonParent = zfnull;
        }
        this->jsonObjectArray.clear();
    }

public:
    void jsonItem(ZF_IN _ZFP_ZFJsonItemData *jsonItemData)
    {
        if(this->jsonItemMap.empty())
        {
            jsonItemData->jsonItem.d->jsonParent = this;
            this->jsonItemMap.push_back(jsonItemData);
            return ;
        }
        zfstlsize l = 0;
        zfstlsize r = this->jsonItemMap.size() - 1;
        zfint cmp = 0;
        do
        {
            zfstlsize i = (l + r) / 2;
            _ZFP_ZFJsonItemData *t = this->jsonItemMap[i];
            cmp = zfscmp(jsonItemData->jsonKey.value(), t->jsonKey.value());
            if(cmp == 0)
            {
                l = i;
                break;
            }
            if(l == r)
            {
                break;
            }

            if(cmp < 0)
            {
                if(i == l)
                {
                    break;
                }
                r = i;
            }
            else
            {
                if(l == i || i + 1 == r)
                {
                    l = i + 1;
                }
                else
                {
                    l = i;
                }
            }
        } while(zftrue);

        jsonItemData->jsonItem.d->jsonParent = this;
        if(cmp == 0)
        {
            this->jsonItemMap[l]->jsonItem.d->jsonParent = zfnull;
            zfdelete(this->jsonItemMap[l]);
            this->jsonItemMap[l] = jsonItemData;
        }
        else
        {
            if(cmp > 0)
            {
                ++l;
            }
            this->jsonItemMap.insert(this->jsonItemMap.begin() + l, jsonItemData);
        }
    }
    zfstlsize jsonItemIndex(ZF_IN const zfchar *key)
    {
        if(key == zfnull || this->jsonItemMap.empty())
        {
            return (zfstlsize)-1;
        }
        zfstlsize l = 0;
        zfstlsize r = this->jsonItemMap.size() - 1;
        zfint cmp = -1;
        do
        {
            zfstlsize i = (l + r) / 2;
            _ZFP_ZFJsonItemData *t = this->jsonItemMap[i];
            cmp = zfscmp(key, t->jsonKey.value());
            if(cmp == 0)
            {
                return i;
            }
            if(l == r)
            {
                break;
            }

            if(cmp < 0)
            {
                if(i == l)
                {
                    break;
                }
                r = i;
            }
            else
            {
                if(l == i || i + 1 == r)
                {
                    l = i + 1;
                }
                else
                {
                    l = i;
                }
            }
        } while(zftrue);
        return (zfstlsize)-1;
    }
};

// ============================================================
// ZFJsonItem
ZFJsonItem::ZFJsonItem(ZF_IN _ZFP_ZFJsonItemPrivate *ref)
{
    if(ref)
    {
        d = ref;
        ++(d->refCount);
    }
    else
    {
        d = zfnew(_ZFP_ZFJsonItemPrivate);
    }
}
ZFJsonItem::ZFJsonItem(void)
: d(zfnew(_ZFP_ZFJsonItemPrivate))
{
}
ZFJsonItem::ZFJsonItem(ZF_IN ZFJsonTypeEnum jsonType)
: d(zfnew(_ZFP_ZFJsonItemPrivate))
{
    d->jsonType = jsonType;
}
ZFJsonItem::ZFJsonItem(ZF_IN const ZFJsonItem &ref)
: d(ref.d)
{
    ++(d->refCount);
}
ZFJsonItem::~ZFJsonItem(void)
{
    --(d->refCount);
    if(d->refCount == 0)
    {
        zfdelete(d);
    }
}

ZFJsonItem &ZFJsonItem::operator = (ZF_IN const ZFJsonItem &ref)
{
    _ZFP_ZFJsonItemPrivate *dTmp = d;
    d = ref.d;
    ++(ref.d->refCount);
    --(dTmp->refCount);
    if(dTmp->refCount == 0)
    {
        zfdelete(dTmp);
    }
    return *this;
}
zfbool ZFJsonItem::operator == (ZF_IN const ZFJsonItem &ref) const
{
    return (d == ref.d || (d->jsonType == ZFJsonType::e_JsonNull && ref.d->jsonType == ZFJsonType::e_JsonNull));
}

// ============================================================
void ZFJsonItem::objectInfoT(ZF_IN_OUT zfstring &ret) const
{
    ret += ZFTOKEN_ZFObjectInfoLeft;
    ret += ZFJsonType::EnumNameForValue(this->jsonType());
    switch(this->jsonType())
    {
        case ZFJsonType::e_JsonNull:
            break;
        case ZFJsonType::e_JsonValue:
            zfstringAppend(ret, "\"%s\"", this->jsonValue());
            break;
        case ZFJsonType::e_JsonObject:
            if(this->jsonItemCount() > 0)
            {
                zfstringAppend(ret, " %zi elements", this->jsonItemCount());
            }
            break;
        case ZFJsonType::e_JsonArray:
            if(this->jsonObjectCount() > 0)
            {
                zfstringAppend(ret, " %zi elements", this->jsonObjectCount());
            }
            break;
        default:
            break;
    }
    ret += ZFTOKEN_ZFObjectInfoRight;
}

zfindex ZFJsonItem::objectRetainCount(void) const
{
    return d->refCount;
}

// ============================================================
ZFJsonTypeEnum ZFJsonItem::jsonType(void) const
{
    return d->jsonType;
}

ZFJsonItem ZFJsonItem::jsonParent(void) const
{
    return ZFJsonItem(d->jsonParent);
}

const zfchar *ZFJsonItem::jsonKey(void) const
{
    return d->jsonKey;
}
void ZFJsonItem::jsonValue(ZF_IN const zfchar *value)
{
    zfCoreAssert(this->jsonType() == ZFJsonType::e_JsonValue);
    d->jsonValue.value(value);
}
const zfchar *ZFJsonItem::jsonValue(void) const
{
    return d->jsonValue.value();
}

void ZFJsonItem::_ZFP_ZFJson_jsonMemoryPool_jsonValue(ZF_IN const zfchar *value, ZF_IN void *token)
{
    d->jsonValue.value(value, token);
}
void ZFJsonItem::_ZFP_ZFJson_jsonMemoryPool_jsonItem(ZF_IN const zfchar *key, ZF_IN void *token, ZF_IN const ZFJsonItem &jsonItem)
{
    zfCoreAssert(this->jsonType() == ZFJsonType::e_JsonObject);
    zfCoreAssert(!jsonItem.jsonIsNull());
    if(key != zfnull)
    {
        d->jsonItem(zfnew(_ZFP_ZFJsonItemData, key, token, jsonItem));
    }
}

// ============================================================
ZFJsonItem ZFJsonItem::jsonCloneTree(void) const
{
    ZFJsonItem ret(this->jsonType());
    switch(this->jsonType())
    {
        case ZFJsonType::e_JsonNull:
            break;
        case ZFJsonType::e_JsonValue:
            ret.jsonValue(this->jsonValue());
            break;
        case ZFJsonType::e_JsonObject:
            for(zfiterator it = this->jsonItemIterator(); this->jsonItemIteratorIsValid(it); this->jsonItemIteratorNextValue(it))
            {
                ret.jsonItem(this->jsonItemIteratorKey(it), this->jsonItemIteratorValue(it).jsonCloneTree());
            }
            break;
        case ZFJsonType::e_JsonArray:
            for(zfindex i = 0; i < this->jsonObjectCount(); ++i)
            {
                ret.jsonObjectAdd(this->jsonObjectAtIndex(i).jsonCloneTree());
            }
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            break;
    }
    return ret;
}

// ============================================================
// for object type
ZFJsonItem ZFJsonItem::jsonItem(ZF_IN const zfchar *key) const
{
    zfstlsize index = d->jsonItemIndex(key);
    if(index != (zfstlsize)-1)
    {
        return d->jsonItemMap[index]->jsonItem;
    }
    else
    {
        return ZFJsonItem();
    }
}
void ZFJsonItem::jsonItem(ZF_IN const zfchar *key,
                          ZF_IN const ZFJsonItem &jsonItem)
{
    zfCoreAssert(this->jsonType() == ZFJsonType::e_JsonObject);
    zfCoreAssert(!jsonItem.jsonIsNull());
    if(key != zfnull)
    {
        d->jsonItem(zfnew(_ZFP_ZFJsonItemData, key, jsonItem));
    }
}

const zfchar *ZFJsonItem::jsonItemValue(ZF_IN const zfchar *key) const
{
    zfstlsize index = d->jsonItemIndex(key);
    if(index != (zfstlsize)-1)
    {
        return d->jsonItemMap[index]->jsonItem.jsonValue();
    }
    else
    {
        return zfnull;
    }
}
void ZFJsonItem::jsonItemValue(ZF_IN const zfchar *key, ZF_IN const zfchar *value)
{
    if(value == zfnull)
    {
        this->jsonItemRemove(key);
    }
    else
    {
        ZFJsonItem jsonValue(ZFJsonType::e_JsonValue);
        jsonValue.jsonValue(value);
        this->jsonItem(key, jsonValue);
    }
}

void ZFJsonItem::jsonItemRemove(ZF_IN const ZFJsonItem &jsonItem)
{
    this->jsonItemRemove(jsonItem.jsonKey());
}
void ZFJsonItem::jsonItemRemove(ZF_IN const zfchar *key)
{
    zfstlsize index = d->jsonItemIndex(key);
    if(index != (zfstlsize)-1)
    {
        d->jsonItemMap[index]->jsonItem.d->jsonParent = zfnull;
        zfdelete(d->jsonItemMap[index]);
        d->jsonItemMap.erase(d->jsonItemMap.begin() + index);
    }
}

void ZFJsonItem::jsonItemRemoveAll(void)
{
    d->jsonItemRemoveAll();
}

zfindex ZFJsonItem::jsonItemCount(void) const
{
    return (zfindex)d->jsonItemMap.size();
}

static void _ZFP_ZFJsonItem_iteratorDeleteCallback(void *data)
{
    zfdelete(ZFCastStatic(zfstlsize *, data));
}
static void *_ZFP_ZFJsonItem_iteratorCopyCallback(void *data)
{
    return zfnew(zfstlsize,
        *ZFCastStatic(zfstlsize *, data));
}
zfiterator ZFJsonItem::jsonItemIterator(void) const
{
    return zfiterator(
        zfnew(zfstlsize, 0),
        _ZFP_ZFJsonItem_iteratorDeleteCallback,
        _ZFP_ZFJsonItem_iteratorCopyCallback);
}

zfiterator ZFJsonItem::jsonItemIteratorForKey(ZF_IN const zfchar *key) const
{
    return zfiterator(
        zfnew(zfstlsize, d->jsonItemIndex(key)),
        _ZFP_ZFJsonItem_iteratorDeleteCallback,
        _ZFP_ZFJsonItem_iteratorCopyCallback);
}

zfbool ZFJsonItem::jsonItemIteratorIsValid(ZF_IN const zfiterator &it) const
{
    zfstlsize *data = it.data<zfstlsize *>();
    return (data != zfnull && *data < d->jsonItemMap.size());
}

zfbool ZFJsonItem::jsonItemIteratorIsEqual(ZF_IN const zfiterator &it0,
                                           ZF_IN const zfiterator &it1) const
{
    return zfiterator::iteratorEqual<zfstlsize *>(it0, it1);
}

void ZFJsonItem::jsonItemIteratorValue(ZF_IN_OUT zfiterator &it, ZF_IN const ZFJsonItem &jsonItem)
{
    if(jsonItem.jsonIsNull())
    {
        this->jsonItemIteratorRemove(it);
        return ;
    }
    zfCoreAssert(this->jsonType() == ZFJsonType::e_JsonObject);
    zfstlsize *data = it.data<zfstlsize *>();
    if(data && *data < d->jsonItemMap.size())
    {
        _ZFP_ZFJsonItemData *jsonItemData = d->jsonItemMap[*data];
        jsonItemData->jsonItem.d->jsonParent = zfnull;
        jsonItem.d->jsonParent = d;
        jsonItemData->jsonItem = jsonItem;
    }
}

void ZFJsonItem::jsonItemIteratorRemove(ZF_IN_OUT zfiterator &it)
{
    zfstlsize *data = it.data<zfstlsize *>();
    if(data && *data < d->jsonItemMap.size())
    {
        d->jsonItemMap[*data]->jsonItem.d->jsonParent = zfnull;
        zfdelete(d->jsonItemMap[*data]);
        d->jsonItemMap.erase(d->jsonItemMap.begin() + (*data));
    }
}

const zfchar *ZFJsonItem::jsonItemIteratorKey(ZF_IN const zfiterator &it) const
{
    zfstlsize *data = it.data<zfstlsize *>();
    if(data && *data < d->jsonItemMap.size())
    {
        return d->jsonItemMap[*data]->jsonKey.value();
    }
    else
    {
        return zfnull;
    }
}
ZFJsonItem ZFJsonItem::jsonItemIteratorValue(ZF_IN const zfiterator &it) const
{
    zfstlsize *data = it.data<zfstlsize *>();
    if(data && *data < d->jsonItemMap.size())
    {
        return d->jsonItemMap[*data]->jsonItem;
    }
    else
    {
        return ZFJsonItem();
    }
}

const zfchar *ZFJsonItem::jsonItemIteratorNextKey(ZF_IN_OUT zfiterator &it) const
{
    zfstlsize *data = it.data<zfstlsize *>();
    if(data && *data < d->jsonItemMap.size())
    {
        const zfchar *ret = d->jsonItemMap[*data]->jsonKey.value();
        ++(*data);
        return ret;
    }
    else
    {
        return zfnull;
    }
}
ZFJsonItem ZFJsonItem::jsonItemIteratorNextValue(ZF_IN_OUT zfiterator &it) const
{
    zfstlsize *data = it.data<zfstlsize *>();
    if(data && *data < d->jsonItemMap.size())
    {
        return d->jsonItemMap[(*data)++]->jsonItem;
    }
    else
    {
        return ZFJsonItem();
    }
}

// ============================================================
zfindex ZFJsonItem::jsonObjectCount(void) const
{
    return (zfindex)d->jsonObjectArray.size();
}
ZFJsonItem ZFJsonItem::jsonObjectAtIndex(ZF_IN zfindex index) const
{
    if(index >= (zfindex)d->jsonObjectArray.size())
    {
        zfCoreCriticalIndexOutOfRange(index, (zfindex)d->jsonObjectArray.size());
    }
    return d->jsonObjectArray[index];
}

void ZFJsonItem::jsonObjectAdd(ZF_IN const ZFJsonItem &jsonObject,
                               ZF_IN_OPT zfindex atIndex /* = zfindexMax() */)
{
    zfCoreAssert(this->jsonType() == ZFJsonType::e_JsonArray);
    if(atIndex == zfindexMax())
    {
        atIndex = (zfindex)d->jsonObjectArray.size();
    }
    else if(atIndex > (zfindex)d->jsonObjectArray.size())
    {
        zfCoreCriticalIndexOutOfRange(atIndex, (zfindex)(d->jsonObjectArray.size() + 1));
        return ;
    }
    zfCoreAssertWithMessage(!jsonObject.jsonIsNull(), "add null object");
    zfCoreAssertWithMessage(jsonObject.jsonType() == ZFJsonType::e_JsonObject, "object not type of %s",
        ZFJsonType::EnumNameForValue(ZFJsonType::e_JsonObject));
    d->jsonObjectArray.insert(d->jsonObjectArray.begin() + atIndex, jsonObject);
    jsonObject.d->jsonParent = d;
}
void ZFJsonItem::jsonObjectRemove(ZF_IN const ZFJsonItem &jsonObject)
{
    if(jsonObject.d->jsonParent != d || jsonObject.d->jsonType != ZFJsonType::e_JsonObject)
    {
        return ;
    }

    for(zfstlsize i = 0; i < d->jsonObjectArray.size(); ++i)
    {
        if(jsonObject == d->jsonObjectArray[i])
        {
            jsonObject.d->jsonParent = zfnull;
            d->jsonObjectArray.erase(d->jsonObjectArray.begin() + i);
            break;
        }
    }
}
void ZFJsonItem::jsonObjectRemoveAtIndex(ZF_IN zfindex index)
{
    if(index >= (zfindex)d->jsonObjectArray.size())
    {
        zfCoreCriticalIndexOutOfRange(index, (zfindex)d->jsonObjectArray.size());
        return ;
    }
    d->jsonObjectArray[index].d->jsonParent = zfnull;
    d->jsonObjectArray.erase(d->jsonObjectArray.begin() + index);
}
void ZFJsonItem::jsonObjectRemoveAll(void)
{
    d->jsonObjectRemoveAll();
}
zfindex ZFJsonItem::jsonObjectFind(ZF_IN const ZFJsonItem &jsonObject) const
{
    if(jsonObject.d->jsonParent == d && jsonObject.d->jsonType == ZFJsonType::e_JsonObject)
    {
        for(zfstlsize i = 0; i < d->jsonObjectArray.size(); ++i)
        {
            if(jsonObject == d->jsonObjectArray[i])
            {
                return (zfindex)i;
            }
        }
    }
    return zfindexMax();
}

// ============================================================
ZFTYPEID_DEFINE_BY_STRING_CONVERTER(ZFJsonItem, ZFJsonItem, {
        v = ZFPROTOCOL_ACCESS(ZFJson)->jsonParse(src, srcLen);
        return !v.jsonIsNull();
    }, {
        return ZFJsonItemToOutput(ZFOutputForString(s), v);
    })
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFJsonItem, void, objectInfoT, ZFMP_IN_OUT(zfstring &, ret))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFJsonItem, zfstring, objectInfo)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFJsonItem, zfindex, objectRetainCount)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFJsonItem, ZFJsonTypeEnum, jsonType)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFJsonItem, zfbool, jsonIsNull)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFJsonItem, ZFJsonItem, jsonParent)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFJsonItem, const zfchar *, jsonKey)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFJsonItem, void, jsonValue, ZFMP_IN(const zfchar *, value))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFJsonItem, const zfchar *, jsonValue)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFJsonItem, ZFJsonItem, jsonCloneTree)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFJsonItem, ZFJsonItem, jsonItem, ZFMP_IN(const zfchar *, key))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFJsonItem, void, jsonItem, ZFMP_IN(const zfchar *, key), ZFMP_IN(const ZFJsonItem &, jsonItem))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFJsonItem, const zfchar *, jsonItemValue, ZFMP_IN(const zfchar *, key))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFJsonItem, void, jsonItemValue, ZFMP_IN(const zfchar *, key), ZFMP_IN(const zfchar *, value))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFJsonItem, void, jsonItemRemove, ZFMP_IN(const ZFJsonItem &, jsonItem))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFJsonItem, void, jsonItemRemove, ZFMP_IN(const zfchar *, key))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFJsonItem, void, jsonItemRemoveAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFJsonItem, zfindex, jsonItemCount)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFJsonItem, zfiterator, jsonItemIterator)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFJsonItem, zfiterator, jsonItemIteratorForKey, ZFMP_IN(const zfchar *, key))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFJsonItem, zfbool, jsonItemIteratorIsValid, ZFMP_IN(const zfiterator &, it))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFJsonItem, zfbool, jsonItemIteratorIsEqual, ZFMP_IN(const zfiterator &, it0), ZFMP_IN(const zfiterator &, it1))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFJsonItem, void, jsonItemIteratorValue, ZFMP_IN_OUT(zfiterator &, it), ZFMP_IN(const ZFJsonItem &, jsonItem))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFJsonItem, void, jsonItemIteratorRemove, ZFMP_IN_OUT(zfiterator &, it))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFJsonItem, const zfchar *, jsonItemIteratorKey, ZFMP_IN(const zfiterator &, it))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFJsonItem, ZFJsonItem, jsonItemIteratorValue, ZFMP_IN(const zfiterator &, it))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFJsonItem, const zfchar *, jsonItemIteratorNextKey, ZFMP_IN_OUT(zfiterator &, it))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFJsonItem, ZFJsonItem, jsonItemIteratorNextValue, ZFMP_IN_OUT(zfiterator &, it))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFJsonItem, zfindex, jsonObjectCount)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFJsonItem, ZFJsonItem, jsonObjectAtIndex, ZFMP_IN(zfindex, index))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFJsonItem, void, jsonObjectAdd, ZFMP_IN(const ZFJsonItem &, jsonObject), ZFMP_IN_OPT(zfindex, atIndex, zfindexMax()))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFJsonItem, void, jsonObjectRemove, ZFMP_IN(const ZFJsonItem &, jsonObject))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFJsonItem, void, jsonObjectRemoveAtIndex, ZFMP_IN(zfindex, index))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFJsonItem, void, jsonObjectRemoveAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFJsonItem, zfindex, jsonObjectFind, ZFMP_IN(const ZFJsonItem &, jsonObject))

// ============================================================
ZFMETHOD_FUNC_DEFINE_0(ZFJsonItem, ZFJsonValue) {return ZFJsonItem(ZFJsonType::e_JsonValue);}
ZFMETHOD_FUNC_DEFINE_0(ZFJsonItem, ZFJsonObject) {return ZFJsonItem(ZFJsonType::e_JsonObject);}
ZFMETHOD_FUNC_DEFINE_0(ZFJsonItem, ZFJsonArray) {return ZFJsonItem(ZFJsonType::e_JsonArray);}

// ============================================================
ZFMETHOD_FUNC_DEFINE_1(ZFJsonItem, ZFJsonItemFromInput, ZFMP_IN(const ZFInput &, input))
{
    return ZFPROTOCOL_ACCESS(ZFJson)->jsonParse(input);
}
ZFMETHOD_FUNC_DEFINE_2(ZFJsonItem, ZFJsonItemFromString,
                       ZFMP_IN(const zfchar *, src),
                       ZFMP_IN_OPT(zfindex, length, zfindexMax()))
{
    return ZFPROTOCOL_ACCESS(ZFJson)->jsonParse(src, length);
}

ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFJsonItemToOutput,
                       ZFMP_IN_OUT(const ZFOutput &, output),
                       ZFMP_IN(const ZFJsonItem &, jsonItem),
                       ZFMP_IN_OPT(const ZFJsonOutputFlags &, outputFlags, ZFJsonOutputFlagsDefault()))
{
    if(output.callbackIsValid() && !jsonItem.jsonIsNull())
    {
        output << outputFlags.jsonGlobalLineBeginToken;
        _ZFP_ZFJsonItemToOutput_output(output, jsonItem, outputFlags, 0);
        return zftrue;
    }
    return zffalse;
}
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFJsonItemToString,
                       ZFMP_IN_OUT(zfstring &, ret),
                       ZFMP_IN(const ZFJsonItem &, jsonItem),
                       ZFMP_IN(const ZFJsonOutputFlags &, outputFlags))
{
    return ZFJsonItemToOutput(ZFOutputForString(ret), jsonItem, outputFlags);
}
ZFMETHOD_FUNC_DEFINE_2(zfstring, ZFJsonItemToString,
                       ZFMP_IN(const ZFJsonItem &, jsonItem),
                       ZFMP_IN(const ZFJsonOutputFlags &, outputFlags))
{
    zfstring ret;
    ZFJsonItemToString(ret, jsonItem, outputFlags);
    return ret;
}

// ============================================================
// escape chars
ZFMETHOD_FUNC_DEFINE_3(void, ZFJsonEscapeCharEncode,
                       ZFMP_OUT(zfstring &, dst),
                       ZFMP_IN(const zfchar *, src),
                       ZFMP_IN_OPT(zfindex, count, zfindexMax()))
{
    ZFJsonEscapeCharEncode(ZFOutputForString(dst), src, count);
}
ZFMETHOD_FUNC_DEFINE_3(void, ZFJsonEscapeCharEncode,
                       ZFMP_OUT(const ZFOutput &, dst),
                       ZFMP_IN(const zfchar *, src),
                       ZFMP_IN_OPT(zfindex, count, zfindexMax()))
{
    ZFPROTOCOL_ACCESS(ZFJson)->jsonEscapeCharEncode(dst, src, count);
}

ZFMETHOD_FUNC_DEFINE_3(void, ZFJsonEscapeCharDecode,
                       ZFMP_OUT(zfstring &, dst),
                       ZFMP_IN(const zfchar *, src),
                       ZFMP_IN_OPT(zfindex, count, zfindexMax()))
{
    ZFJsonEscapeCharDecode(ZFOutputForString(dst), src, count);
}
ZFMETHOD_FUNC_DEFINE_3(void, ZFJsonEscapeCharDecode,
                       ZFMP_OUT(const ZFOutput &, dst),
                       ZFMP_IN(const zfchar *, src),
                       ZFMP_IN_OPT(zfindex, count, zfindexMax()))
{
    ZFPROTOCOL_ACCESS(ZFJson)->jsonEscapeCharDecode(dst, src, count);
}

ZF_NAMESPACE_GLOBAL_END

