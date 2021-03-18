#include "ZFTextTemplateType.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFENUM_DEFINE(ZFTextTemplateIndexFlag)

// ============================================================
void ZFTextTemplateIndexData::objectInfoT(ZF_IN_OUT zfstring &ret) const
{
    ret += ZFTOKEN_ZFObjectInfoLeft;

    zfstringAppend(ret, "%%%zi[%zi%s]",
        this->indexWidth,
        this->indexRadix,
        this->indexUpperCase ? "x" : "X");

    if(this->indexOffset >= 0)
    {
        zfstringAppend(ret, "[%zi, %d)", this->indexStart, this->indexOffset);
    }
    else
    {
        zfstringAppend(ret, "(%d, %zi]", this->indexOffset, this->indexStart);
    }

    ret += " ";
    ZFTextTemplateIndexFlagEnumToString(ret, this->indexFlag);

    ret += ZFTOKEN_ZFObjectInfoRight;
}

// ============================================================
ZFTYPEID_DEFINE_BY_SERIALIZABLE_CONVERTER(ZFTextTemplateIndexData, ZFTextTemplateIndexData, {
        if(ZFSerializableUtil::requireItemClass(serializableData, ZFTypeId_ZFTextTemplateIndexData(), outErrorHint, outErrorPos) == zfnull)
        {
            return zffalse;
        }

        v.indexStart = 0;
        v.indexOffset = 1;
        v.indexRadix = 10;
        v.indexUpperCase = zftrue;
        v.indexWidth = 0;
        v.indexFlag = ZFTextTemplateIndexFlag::EnumDefault();

        const zfchar *element = zfnull;

        element = ZFSerializableUtil::checkAttribute(serializableData, ZFSerializableKeyword_ZFTextTemplateIndexData_indexStart);
        if(element != zfnull && !zfindexFromString(v.indexStart, element))
        {
            ZFSerializableUtil::errorOccurredWhile(outErrorHint, outErrorPos, serializableData, ZFSerializableKeyword_ZFTextTemplateIndexData_indexStart, element);
            return zffalse;
        }

        element = ZFSerializableUtil::checkAttribute(serializableData, ZFSerializableKeyword_ZFTextTemplateIndexData_indexOffset);
        if(element != zfnull && !zfintFromString(v.indexOffset, element))
        {
            ZFSerializableUtil::errorOccurredWhile(outErrorHint, outErrorPos, serializableData, ZFSerializableKeyword_ZFTextTemplateIndexData_indexOffset, element);
            return zffalse;
        }

        element = ZFSerializableUtil::checkAttribute(serializableData, ZFSerializableKeyword_ZFTextTemplateIndexData_indexRadix);
        if(element != zfnull && !zfindexFromString(v.indexRadix, element))
        {
            ZFSerializableUtil::errorOccurredWhile(outErrorHint, outErrorPos, serializableData, ZFSerializableKeyword_ZFTextTemplateIndexData_indexRadix, element);
            return zffalse;
        }

        element = ZFSerializableUtil::checkAttribute(serializableData, ZFSerializableKeyword_ZFTextTemplateIndexData_indexUpperCase);
        if(element != zfnull && !zfboolFromString(v.indexUpperCase, element))
        {
            ZFSerializableUtil::errorOccurredWhile(outErrorHint, outErrorPos, serializableData, ZFSerializableKeyword_ZFTextTemplateIndexData_indexUpperCase, element);
            return zffalse;
        }

        element = ZFSerializableUtil::checkAttribute(serializableData, ZFSerializableKeyword_ZFTextTemplateIndexData_indexWidth);
        if(element != zfnull && !zfindexFromString(v.indexWidth, element))
        {
            ZFSerializableUtil::errorOccurredWhile(outErrorHint, outErrorPos, serializableData, ZFSerializableKeyword_ZFTextTemplateIndexData_indexWidth, element);
            return zffalse;
        }

        element = ZFSerializableUtil::checkAttribute(serializableData, ZFSerializableKeyword_ZFTextTemplateIndexData_indexFlag);
        if(element != zfnull && !ZFTextTemplateIndexFlagEnumFromString(v.indexFlag, element))
        {
            ZFSerializableUtil::errorOccurredWhile(outErrorHint, outErrorPos, serializableData, ZFSerializableKeyword_ZFTextTemplateIndexData_indexFlag, element);
            return zffalse;
        }

        serializableData.resolveMark();
        return zftrue;
    }, {
        serializableData.itemClass(ZFTypeId_ZFTextTemplateIndexData());

        if(v.indexStart != 0)
        {
            serializableData.attributeForName(ZFSerializableKeyword_ZFTextTemplateIndexData_indexStart, zfindexToString(v.indexStart));
        }

        if(v.indexOffset != 1)
        {
            serializableData.attributeForName(ZFSerializableKeyword_ZFTextTemplateIndexData_indexOffset, zfintToString(v.indexOffset));
        }

        if(v.indexRadix != 10)
        {
            serializableData.attributeForName(ZFSerializableKeyword_ZFTextTemplateIndexData_indexRadix, zfindexToString(v.indexRadix));
        }

        if(!v.indexUpperCase)
        {
            serializableData.attributeForName(ZFSerializableKeyword_ZFTextTemplateIndexData_indexUpperCase, zfboolToString(v.indexUpperCase));
        }

        if(v.indexWidth != 0)
        {
            serializableData.attributeForName(ZFSerializableKeyword_ZFTextTemplateIndexData_indexWidth, zfindexToString(v.indexWidth));
        }

        if(v.indexFlag != ZFTextTemplateIndexFlag::EnumDefault())
        {
            serializableData.attributeForName(ZFSerializableKeyword_ZFTextTemplateIndexData_indexFlag, ZFTextTemplateIndexFlagEnumToString(v.indexFlag));
        }

        return zftrue;
    })
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFTextTemplateIndexData, zfindex, indexStart)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFTextTemplateIndexData, zfint, indexOffset)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFTextTemplateIndexData, zfindex, indexRadix)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFTextTemplateIndexData, zfbool, indexUpperCase)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFTextTemplateIndexData, zfindex, indexWidth)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFTextTemplateIndexData, ZFTextTemplateIndexFlagEnum, indexFlag)

// ============================================================
zfclassNotPOD _ZFP_ZFTextTemplateReplaceData
{
public:
    zfstring key;
    zfstring value;
};
zfclassNotPOD _ZFP_ZFTextTemplateEnableData
{
public:
    zfstring key;
    zfbool value;
};
zfclassNotPOD _ZFP_ZFTextTemplateIndexData
{
public:
    zfstring key;
    ZFTextTemplateIndexData value;
};
zfclassNotPOD _ZFP_ZFTextTemplateParamPrivate
{
public:
    zfuint refCount;

    ZFCoreArrayPOD<_ZFP_ZFTextTemplateReplaceData *> replaceDataList;
    ZFCoreMap replaceDataMap; // map to _ZFP_ZFTextTemplateReplaceData *

    zfbool enableDataDefault;
    ZFCoreArrayPOD<_ZFP_ZFTextTemplateEnableData *> enableDataList;
    ZFCoreMap enableDataMap; // map to _ZFP_ZFTextTemplateEnableData *

    ZFTextTemplateIndexData indexDataDefault;
    ZFCoreArrayPOD<_ZFP_ZFTextTemplateIndexData *> indexDataList;
    ZFCoreMap indexDataMap; // map to _ZFP_ZFTextTemplateIndexData *

public:
    _ZFP_ZFTextTemplateParamPrivate(void)
    : refCount(1)
    , replaceDataList()
    , replaceDataMap()
    , enableDataDefault(zffalse)
    , enableDataList()
    , enableDataMap()
    , indexDataDefault()
    , indexDataList()
    , indexDataMap()
    {
    }
};
// ============================================================
ZFTextTemplateParam::ZFTextTemplateParam(void)
: d(zfnew(_ZFP_ZFTextTemplateParamPrivate))
{
}
ZFTextTemplateParam::ZFTextTemplateParam(ZF_IN const ZFTextTemplateParam &ref)
: d(ref.d)
{
    ++(d->refCount);
}
ZFTextTemplateParam::~ZFTextTemplateParam(void)
{
    --(d->refCount);
    if(d->refCount == 0)
    {
        zfdelete(d);
        d = zfnull;
    }
}
ZFTextTemplateParam &ZFTextTemplateParam::operator = (ZF_IN const ZFTextTemplateParam &ref)
{
    if(d != ref.d)
    {
        _ZFP_ZFTextTemplateParamPrivate *t = d;
        d = ref.d;
        ++(d->refCount);
        --(t->refCount);
        if(t->refCount == 0)
        {
            zfdelete(t);
        }
    }
    return *this;
}

// ============================================================
void ZFTextTemplateParam::replaceDataAdd(ZF_IN const zfchar *key, ZF_IN const zfchar *value)
{
    if(key == zfnull)
    {
        return ;
    }
    zfiterator it = d->replaceDataMap.iteratorForKey(key);
    if(d->replaceDataMap.iteratorIsValid(it))
    {
        _ZFP_ZFTextTemplateReplaceData *replaceData = d->replaceDataMap.iteratorValue<_ZFP_ZFTextTemplateReplaceData *>(it);
        if(value == zfnull)
        {
            d->replaceDataList.removeElement(replaceData);
            d->replaceDataMap.iteratorRemove(it);
        }
        else
        {
            replaceData->value = value;
        }
    }
    else
    {
        if(value != zfnull)
        {
            _ZFP_ZFTextTemplateReplaceData *replaceData = zfnew(_ZFP_ZFTextTemplateReplaceData);
            replaceData->key = key;
            replaceData->value = value;
            d->replaceDataList.add(replaceData);
            d->replaceDataMap.set(key, ZFCorePointerForObject<_ZFP_ZFTextTemplateReplaceData *>(replaceData));
        }
    }
}
const zfchar *ZFTextTemplateParam::replaceData(ZF_IN const zfchar *key) const
{
    _ZFP_ZFTextTemplateReplaceData *replaceData = d->replaceDataMap.get<_ZFP_ZFTextTemplateReplaceData *>(key);
    if(replaceData != zfnull)
    {
        return replaceData->value;
    }
    else
    {
        return zfnull;
    }
}
zfindex ZFTextTemplateParam::replaceDataCount(void) const
{
    return d->replaceDataMap.count();
}
const zfchar *ZFTextTemplateParam::replaceDataNameAtIndex(ZF_IN zfindex index) const
{
    return d->replaceDataList.get(index)->key;
}
const zfchar *ZFTextTemplateParam::replaceDataAtIndex(ZF_IN zfindex index) const
{
    return d->replaceDataList.get(index)->value;
}
void ZFTextTemplateParam::replaceDataRemove(ZF_IN const zfchar *key)
{
    zfiterator it = d->replaceDataMap.iteratorForKey(key);
    if(d->replaceDataMap.iteratorIsValid(it))
    {
        d->replaceDataList.removeElement(d->replaceDataMap.iteratorValue<_ZFP_ZFTextTemplateReplaceData *>(it));
        d->replaceDataMap.iteratorRemove(it);
    }
}
void ZFTextTemplateParam::replaceDataRemoveAtIndex(ZF_IN zfindex index)
{
    const zfchar *key = d->replaceDataList.get(index)->key;
    d->replaceDataList.remove(index);
    d->replaceDataMap.remove(key);
}
void ZFTextTemplateParam::replaceDataRemoveAll(void)
{
    d->replaceDataList.removeAll();
    d->replaceDataMap.removeAll();
}

// ============================================================
void ZFTextTemplateParam::enableDataDefault(ZF_IN zfbool enableDataDefault)
{
    d->enableDataDefault = enableDataDefault;
}
zfbool ZFTextTemplateParam::enableDataDefault(void) const
{
    return d->enableDataDefault;
}

void ZFTextTemplateParam::enableDataAdd(ZF_IN const zfchar *key, ZF_IN zfbool value)
{
    if(key == zfnull)
    {
        return ;
    }
    zfiterator it = d->enableDataMap.iteratorForKey(key);
    if(d->enableDataMap.iteratorIsValid(it))
    {
        _ZFP_ZFTextTemplateEnableData *enableData = d->enableDataMap.iteratorValue<_ZFP_ZFTextTemplateEnableData *>(it);
        enableData->value = value;
    }
    else
    {
        _ZFP_ZFTextTemplateEnableData *enableData = zfnew(_ZFP_ZFTextTemplateEnableData);
        enableData->key = key;
        enableData->value = value;
        d->enableDataList.add(enableData);
        d->enableDataMap.set(key, ZFCorePointerForObject<_ZFP_ZFTextTemplateEnableData *>(enableData));
    }
}
const zfbool *ZFTextTemplateParam::enableData(ZF_IN const zfchar *key) const
{
    _ZFP_ZFTextTemplateEnableData *enableData = d->enableDataMap.get<_ZFP_ZFTextTemplateEnableData *>(key);
    if(enableData != zfnull)
    {
        return &(enableData->value);
    }
    else
    {
        return zfnull;
    }
}
zfbool ZFTextTemplateParam::enableDataValue(ZF_IN const zfchar *key) const
{
    const zfbool *value = this->enableData(key);
    if(value == zfnull)
    {
        return this->enableDataDefault();
    }
    else
    {
        return *value;
    }
}
zfindex ZFTextTemplateParam::enableDataCount(void) const
{
    return d->enableDataMap.count();
}
const zfchar *ZFTextTemplateParam::enableDataNameAtIndex(ZF_IN zfindex index) const
{
    return d->enableDataList.get(index)->key;
}
zfbool ZFTextTemplateParam::enableDataAtIndex(ZF_IN zfindex index) const
{
    return d->enableDataList.get(index)->value;
}
void ZFTextTemplateParam::enableDataRemove(ZF_IN const zfchar *key)
{
    zfiterator it = d->enableDataMap.iteratorForKey(key);
    if(d->enableDataMap.iteratorIsValid(it))
    {
        d->enableDataList.removeElement(d->enableDataMap.iteratorValue<_ZFP_ZFTextTemplateEnableData *>(it));
        d->enableDataMap.iteratorRemove(it);
    }
}
void ZFTextTemplateParam::enableDataRemoveAtIndex(ZF_IN zfindex index)
{
    const zfchar *key = d->enableDataList.get(index)->key;
    d->enableDataList.remove(index);
    d->enableDataMap.remove(key);
}
void ZFTextTemplateParam::enableDataRemoveAll(void)
{
    d->enableDataList.removeAll();
    d->enableDataMap.removeAll();
}

// ============================================================
void ZFTextTemplateParam::indexDataDefault(ZF_IN const ZFTextTemplateIndexData &indexDataDefault)
{
    d->indexDataDefault = indexDataDefault;
}
const ZFTextTemplateIndexData &ZFTextTemplateParam::indexDataDefault(void) const
{
    return d->indexDataDefault;
}

void ZFTextTemplateParam::indexDataAdd(ZF_IN const zfchar *key, ZF_IN const ZFTextTemplateIndexData &value)
{
    if(key == zfnull)
    {
        return ;
    }
    zfiterator it = d->indexDataMap.iteratorForKey(key);
    if(d->indexDataMap.iteratorIsValid(it))
    {
        _ZFP_ZFTextTemplateIndexData *indexData = d->indexDataMap.iteratorValue<_ZFP_ZFTextTemplateIndexData *>(it);
        indexData->value = value;
    }
    else
    {
        _ZFP_ZFTextTemplateIndexData *indexData = zfnew(_ZFP_ZFTextTemplateIndexData);
        indexData->key = key;
        indexData->value = value;
        d->indexDataList.add(indexData);
        d->indexDataMap.set(key, ZFCorePointerForObject<_ZFP_ZFTextTemplateIndexData *>(indexData));
    }
}
const ZFTextTemplateIndexData *ZFTextTemplateParam::indexData(ZF_IN const zfchar *key) const
{
    _ZFP_ZFTextTemplateIndexData *indexData = d->indexDataMap.get<_ZFP_ZFTextTemplateIndexData *>(key);
    if(indexData != zfnull)
    {
        return &(indexData->value);
    }
    else
    {
        return zfnull;
    }
}
ZFTextTemplateIndexData *ZFTextTemplateParam::indexData(ZF_IN const zfchar *key)
{
    _ZFP_ZFTextTemplateIndexData *indexData = d->indexDataMap.get<_ZFP_ZFTextTemplateIndexData *>(key);
    if(indexData != zfnull)
    {
        return &(indexData->value);
    }
    else
    {
        return zfnull;
    }
}
zfindex ZFTextTemplateParam::indexDataCount(void) const
{
    return d->indexDataMap.count();
}
const zfchar *ZFTextTemplateParam::indexDataNameAtIndex(ZF_IN zfindex index) const
{
    return d->indexDataList.get(index)->key;
}
const ZFTextTemplateIndexData *ZFTextTemplateParam::indexDataAtIndex(ZF_IN zfindex index) const
{
    return &(d->indexDataList.get(index)->value);
}
ZFTextTemplateIndexData *ZFTextTemplateParam::indexDataAtIndex(ZF_IN zfindex index)
{
    return &(d->indexDataList.get(index)->value);
}
void ZFTextTemplateParam::indexDataRemove(ZF_IN const zfchar *key)
{
    zfiterator it = d->indexDataMap.iteratorForKey(key);
    if(d->indexDataMap.iteratorIsValid(it))
    {
        d->indexDataList.removeElement(d->indexDataMap.iteratorValue<_ZFP_ZFTextTemplateIndexData *>(it));
        d->indexDataMap.iteratorRemove(it);
    }
}
void ZFTextTemplateParam::indexDataRemoveAtIndex(ZF_IN zfindex index)
{
    const zfchar *key = d->indexDataList.get(index)->key;
    d->indexDataList.remove(index);
    d->indexDataMap.remove(key);
}
void ZFTextTemplateParam::indexDataRemoveAll(void)
{
    d->indexDataList.removeAll();
    d->indexDataMap.removeAll();
}

// ============================================================
void ZFTextTemplateParam::copyFrom(ZF_IN const ZFTextTemplateParam &ref)
{
    if(d == ref.d)
    {
        return ;
    }

    d->replaceDataList.removeAll();
    d->replaceDataMap.removeAll();
    for(zfindex i = 0; i < ref.d->replaceDataList.count(); ++i)
    {
        const _ZFP_ZFTextTemplateReplaceData *replaceDataRef = ref.d->replaceDataList[i];
        _ZFP_ZFTextTemplateReplaceData *replaceData = zfnew(_ZFP_ZFTextTemplateReplaceData);
        replaceData->key = replaceDataRef->key;
        replaceData->value = replaceDataRef->value;
        d->replaceDataList.add(replaceData);
        d->replaceDataMap.set(replaceData->key, ZFCorePointerForObject<_ZFP_ZFTextTemplateReplaceData *>(replaceData));
    }

    d->enableDataDefault = ref.d->enableDataDefault;

    d->enableDataList.removeAll();
    d->enableDataMap.removeAll();
    for(zfindex i = 0; i < ref.d->enableDataList.count(); ++i)
    {
        const _ZFP_ZFTextTemplateEnableData *enableDataRef = ref.d->enableDataList[i];
        _ZFP_ZFTextTemplateEnableData *enableData = zfnew(_ZFP_ZFTextTemplateEnableData);
        enableData->key = enableDataRef->key;
        enableData->value = enableDataRef->value;
        d->enableDataList.add(enableData);
        d->enableDataMap.set(enableData->key, ZFCorePointerForObject<_ZFP_ZFTextTemplateEnableData *>(enableData));
    }

    d->indexDataDefault.copyFrom(ref.d->indexDataDefault);

    d->indexDataList.removeAll();
    d->indexDataMap.removeAll();
    for(zfindex i = 0; i < ref.d->indexDataList.count(); ++i)
    {
        const _ZFP_ZFTextTemplateIndexData *indexDataRef = ref.d->indexDataList[i];
        _ZFP_ZFTextTemplateIndexData *indexData = zfnew(_ZFP_ZFTextTemplateIndexData);
        indexData->key = indexDataRef->key;
        indexData->value.copyFrom(indexDataRef->value);
        d->indexDataList.add(indexData);
        d->indexDataMap.set(indexData->key, ZFCorePointerForObject<_ZFP_ZFTextTemplateIndexData *>(indexData));
    }
}

zfbool ZFTextTemplateParam::operator == (ZF_IN const ZFTextTemplateParam &ref) const
{
    if(d == ref.d)
    {
        return zftrue;
    }
    if(d->replaceDataList.count() != ref.d->replaceDataList.count()
        || d->enableDataDefault != ref.d->enableDataDefault
        || d->enableDataList.count() != ref.d->enableDataList.count()
        || d->indexDataDefault != ref.d->indexDataDefault
        || d->indexDataList.count() != ref.d->indexDataList.count()
        )
    {
        return zffalse;
    }
    for(zfindex i = 0; i < d->replaceDataList.count(); ++i)
    {
        const _ZFP_ZFTextTemplateReplaceData *replaceData = d->replaceDataList[i];
        const _ZFP_ZFTextTemplateReplaceData *replaceDataRef = ref.d->replaceDataList[i];
        if(replaceData->key != replaceDataRef->key || replaceData->value != replaceDataRef->value)
        {
            return zffalse;
        }
    }
    for(zfindex i = 0; i < d->enableDataList.count(); ++i)
    {
        const _ZFP_ZFTextTemplateEnableData *enableData = d->enableDataList[i];
        const _ZFP_ZFTextTemplateEnableData *enableDataRef = ref.d->enableDataList[i];
        if(enableData->key != enableDataRef->key || enableData->value != enableDataRef->value)
        {
            return zffalse;
        }
    }
    for(zfindex i = 0; i < d->indexDataList.count(); ++i)
    {
        const _ZFP_ZFTextTemplateIndexData *indexData = d->indexDataList[i];
        const _ZFP_ZFTextTemplateIndexData *indexDataRef = ref.d->indexDataList[i];
        if(indexData->key != indexDataRef->key || indexData->value != indexDataRef->value)
        {
            return zffalse;
        }
    }
    return zftrue;
}

void ZFTextTemplateParam::objectInfoT(ZF_IN_OUT zfstring &ret) const
{
    ret += ZFTOKEN_ZFObjectInfoLeft;
    zfbool first = zftrue;

    if(!d->replaceDataList.isEmpty())
    {
        if(first) {first = zffalse;}
        else {ret += ", ";}

        ret += "replace: [";
        for(zfindex i = 0; i < d->replaceDataList.count(); ++i)
        {
            if(i > 0)
            {
                ret += ", ";
            }
            const _ZFP_ZFTextTemplateReplaceData *replaceData = d->replaceDataList[i];
            zfstringAppend(ret, "<%s, %s>", replaceData->key.cString(), replaceData->value.cString());
        }
        ret += "]";
    }

    if(!d->enableDataList.isEmpty())
    {
        if(first) {first = zffalse;}
        else {ret += ", ";}

        ret += "enable: [";
        for(zfindex i = 0; i < d->enableDataList.count(); ++i)
        {
            if(i > 0)
            {
                ret += ", ";
            }
            const _ZFP_ZFTextTemplateEnableData *enableData = d->enableDataList[i];
            zfstringAppend(ret, "<%s, %b>", enableData->key.cString(), enableData->value);
        }
        ret += "]";
    }

    {
        if(first) {first = zffalse;}
        else {ret += ", ";}

        ret += "enableDataDefault: ";
        zfboolToString(ret, this->enableDataDefault());
    }

    if(!d->indexDataList.isEmpty())
    {
        if(first) {first = zffalse;}
        else {ret += ", ";}

        ret += "index: ";
        for(zfindex i = 0; i < d->indexDataList.count(); ++i)
        {
            if(i > 0)
            {
                ret += ", ";
            }
            const _ZFP_ZFTextTemplateIndexData *indexData = d->indexDataList[i];
            ret += "<";
            ret += indexData->key;
            ret += ", ";
            indexData->value.objectInfoT(ret);
            ret += ">";
        }
        ret += "]";
    }

    {
        if(first) {first = zffalse;}
        else {ret += ", ";}

        ret += "indexDataDefault: ";
        d->indexDataDefault.objectInfoT(ret);
    }

    ret += ZFTOKEN_ZFObjectInfoRight;

    ZFUNUSED(first);
}

// ============================================================
ZFTYPEID_DEFINE_BY_SERIALIZABLE_CONVERTER(ZFTextTemplateParam, ZFTextTemplateParam, {
        if(ZFSerializableUtil::requireItemClass(serializableData, ZFTypeId_ZFTextTemplateParam(), outErrorHint, outErrorPos) == zfnull)
        {
            return zffalse;
        }

        v.replaceDataRemoveAll();
        v.enableDataRemoveAll();
        v.enableDataDefault(zffalse);
        v.indexDataRemoveAll();
        v.indexDataDefault(ZFTextTemplateIndexData());

        const ZFSerializableData *element = zfnull;

        element = ZFSerializableUtil::checkElementByCategory(serializableData, ZFSerializableKeyword_ZFTextTemplateParam_replace);
        if(element != zfnull)
        {
            for(zfindex i = 0; i < element->elementCount(); ++i)
            {
                const ZFSerializableData &item = element->elementAtIndex(i);
                const zfchar *key = item.propertyName();
                if(key == zfnull)
                {
                    ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, item, "missing item name");
                    return zffalse;
                }
                item.resolvePropertyNameMark();
                zfstring value;
                if(!zfstringFromData(value, item, outErrorHint, outErrorPos))
                {
                    return zffalse;
                }
                v.replaceDataAdd(key, value);
            }
            element->resolveMark();
        }

        element = ZFSerializableUtil::checkElementByCategory(serializableData, ZFSerializableKeyword_ZFTextTemplateParam_enable);
        if(element != zfnull)
        {
            for(zfindex i = 0; i < element->elementCount(); ++i)
            {
                const ZFSerializableData &item = element->elementAtIndex(i);
                const zfchar *key = item.propertyName();
                if(key == zfnull)
                {
                    ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, item, "missing item name");
                    return zffalse;
                }
                item.resolvePropertyNameMark();
                zfbool value = zffalse;
                if(!zfboolFromData(value, item, outErrorHint, outErrorPos))
                {
                    return zffalse;
                }
                v.enableDataAdd(key, value);
            }
            element->resolveMark();
        }

        element = ZFSerializableUtil::checkElementByCategory(serializableData, ZFSerializableKeyword_ZFTextTemplateParam_enableDataDefault);
        if(element != zfnull)
        {
            zfbool enableDataDefault = zffalse;
            if(!zfboolFromData(enableDataDefault, *element, outErrorHint, outErrorPos))
            {
                return zffalse;
            }
            v.enableDataDefault(enableDataDefault);
        }

        element = ZFSerializableUtil::checkElementByCategory(serializableData, ZFSerializableKeyword_ZFTextTemplateParam_index);
        if(element != zfnull)
        {
            for(zfindex i = 0; i < element->elementCount(); ++i)
            {
                const ZFSerializableData &item = element->elementAtIndex(i);
                const zfchar *key = item.propertyName();
                if(key == zfnull)
                {
                    ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, item, "missing item name");
                    return zffalse;
                }
                item.resolvePropertyNameMark();
                ZFTextTemplateIndexData value;
                if(!ZFTextTemplateIndexDataFromData(value, item, outErrorHint, outErrorPos))
                {
                    return zffalse;
                }
                v.indexDataAdd(key, value);
            }
            element->resolveMark();
        }

        element = ZFSerializableUtil::checkElementByCategory(serializableData, ZFSerializableKeyword_ZFTextTemplateParam_indexDataDefault);
        if(element != zfnull)
        {
            ZFTextTemplateIndexData indexDataDefault;
            if(!ZFTextTemplateIndexDataFromData(indexDataDefault, *element, outErrorHint, outErrorPos))
            {
                return zffalse;
            }
            v.indexDataDefault(indexDataDefault);
        }

        serializableData.resolveMark();
        return zftrue;
    }, {
        serializableData.itemClass(ZFTypeId_ZFTextTemplateParam());

        if(v.replaceDataCount() > 0)
        {
            ZFSerializableData nodeData;
            for(zfindex i = 0; i < v.replaceDataCount(); ++i)
            {
                ZFSerializableData itemData;
                if(!zfstringToData(itemData, v.replaceDataAtIndex(i), outErrorHint))
                {
                    return zffalse;
                }
                itemData.propertyName(v.replaceDataNameAtIndex(i));
                nodeData.elementAdd(itemData);
            }

            nodeData.itemClass(ZFSerializableKeyword_node);
            nodeData.category(ZFSerializableKeyword_ZFTextTemplateParam_replace);
            serializableData.elementAdd(nodeData);
        }

        if(v.enableDataCount() > 0)
        {
            ZFSerializableData nodeData;
            for(zfindex i = 0; i < v.enableDataCount(); ++i)
            {
                ZFSerializableData itemData;
                if(!zfboolToData(itemData, v.enableDataAtIndex(i), outErrorHint))
                {
                    return zffalse;
                }
                itemData.propertyName(v.enableDataNameAtIndex(i));
                nodeData.elementAdd(itemData);
            }

            nodeData.itemClass(ZFSerializableKeyword_node);
            nodeData.category(ZFSerializableKeyword_ZFTextTemplateParam_enable);
            serializableData.elementAdd(nodeData);
        }

        if(v.enableDataDefault())
        {
            ZFSerializableData nodeData;
            if(!zfboolToData(nodeData, v.enableDataDefault(), outErrorHint))
            {
                return zffalse;
            }
            nodeData.category(ZFSerializableKeyword_ZFTextTemplateParam_enableDataDefault);
            serializableData.elementAdd(nodeData);
        }

        if(v.indexDataCount() > 0)
        {
            ZFSerializableData nodeData;
            for(zfindex i = 0; i < v.indexDataCount(); ++i)
            {
                ZFSerializableData itemData;
                if(!ZFTextTemplateIndexDataToData(itemData, *(v.indexDataAtIndex(i)), outErrorHint))
                {
                    return zffalse;
                }
                itemData.propertyName(v.indexDataNameAtIndex(i));
                nodeData.elementAdd(itemData);
            }

            nodeData.itemClass(ZFSerializableKeyword_node);
            nodeData.category(ZFSerializableKeyword_ZFTextTemplateParam_index);
            serializableData.elementAdd(nodeData);
        }

        {
            ZFSerializableData nodeData;
            if(!ZFTextTemplateIndexDataToData(nodeData, v.indexDataDefault(), outErrorHint))
            {
                return zffalse;
            }
            if(nodeData.attributeCount() > 0 || nodeData.elementCount() > 0)
            {
                nodeData.category(ZFSerializableKeyword_ZFTextTemplateParam_indexDataDefault);
                serializableData.elementAdd(nodeData);
            }
        }

        return zftrue;
    })
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFTextTemplateParam, void, replaceDataAdd, ZFMP_IN(const zfchar *, key), ZFMP_IN(const zfchar *, value))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFTextTemplateParam, const zfchar *, replaceData, ZFMP_IN(const zfchar *, key))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFTextTemplateParam, zfindex, replaceDataCount)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFTextTemplateParam, const zfchar *, replaceDataNameAtIndex, ZFMP_IN(zfindex, index))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFTextTemplateParam, const zfchar *, replaceDataAtIndex, ZFMP_IN(zfindex, index))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFTextTemplateParam, void, replaceDataRemove, ZFMP_IN(const zfchar *, key))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFTextTemplateParam, void, replaceDataRemoveAtIndex, ZFMP_IN(zfindex, index))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFTextTemplateParam, void, replaceDataRemoveAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFTextTemplateParam, void, enableDataDefault, ZFMP_IN(zfbool, enableDataDefault))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFTextTemplateParam, zfbool, enableDataDefault)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFTextTemplateParam, void, enableDataAdd, ZFMP_IN(const zfchar *, key), ZFMP_IN(zfbool, value))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFTextTemplateParam, const zfbool *, enableData, ZFMP_IN(const zfchar *, key))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFTextTemplateParam, zfbool, enableDataValue, ZFMP_IN(const zfchar *, key))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFTextTemplateParam, zfindex, enableDataCount)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFTextTemplateParam, const zfchar *, enableDataNameAtIndex, ZFMP_IN(zfindex, index))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFTextTemplateParam, zfbool, enableDataAtIndex, ZFMP_IN(zfindex, index))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFTextTemplateParam, void, enableDataRemove, ZFMP_IN(const zfchar *, key))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFTextTemplateParam, void, enableDataRemoveAtIndex, ZFMP_IN(zfindex, index))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFTextTemplateParam, void, enableDataRemoveAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFTextTemplateParam, void, indexDataDefault, ZFMP_IN(const ZFTextTemplateIndexData &, indexDataDefault))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFTextTemplateParam, const ZFTextTemplateIndexData &, indexDataDefault)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFTextTemplateParam, void, indexDataAdd, ZFMP_IN(const zfchar *, key), ZFMP_IN(const ZFTextTemplateIndexData &, value))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFTextTemplateParam, const ZFTextTemplateIndexData *, indexData, ZFMP_IN(const zfchar *, key))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFTextTemplateParam, zfindex, indexDataCount)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFTextTemplateParam, const zfchar *, indexDataNameAtIndex, ZFMP_IN(zfindex, index))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFTextTemplateParam, const ZFTextTemplateIndexData *, indexDataAtIndex, ZFMP_IN(zfindex, index))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFTextTemplateParam, void, indexDataRemove, ZFMP_IN(const zfchar *, key))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFTextTemplateParam, void, indexDataRemoveAtIndex, ZFMP_IN(zfindex, index))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFTextTemplateParam, void, indexDataRemoveAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFTextTemplateParam, void, copyFrom, ZFMP_IN(const ZFTextTemplateParam &, ref))

ZF_NAMESPACE_GLOBAL_END

