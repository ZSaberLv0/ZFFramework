#include "ZFSerializableData.h"
#include "ZFObjectImpl.h"
#include "ZFSerializableUtil.h"
#include "ZFSerializableDataSerializableConverter.h"

#include "ZFCore/ZFSTLWrapper/zfstl_string.h"
#include "ZFCore/ZFSTLWrapper/zfstl_deque.h"
#include "ZFCore/ZFSTLWrapper/zfstl_map.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfbool ZFSerializableDataResolveCheckEnable = zftrue;

// ============================================================
// _ZFP_ZFSerializableDataPrivate
zfclassNotPOD _ZFP_ZFSerializableDataAttributeData
{
public:
    zfstlstringZ attrValue;
    zfbool resolved;
public:
    _ZFP_ZFSerializableDataAttributeData(void)
    : attrValue()
    , resolved(zffalse)
    {
    }
    _ZFP_ZFSerializableDataAttributeData(ZF_IN const zfchar *attrValue,
                                         ZF_IN_OPT zfbool resolved = zffalse)
    : attrValue(attrValue)
    , resolved(resolved)
    {
    }
};
typedef zfstlmap<zfstlstringZ, _ZFP_ZFSerializableDataAttributeData> _ZFP_ZFSerializableDataAttributeMapType;
typedef zfstlmap<zfstlstringZ, zfautoObject> _ZFP_ZFSerializableDataTagMapType;
zfclassNotPOD _ZFP_ZFSerializableDataPrivate
{
public:
    zfuint refCount;
    _ZFP_ZFSerializableDataPrivate *serializableDataParent;
    zfstlstringZ classNameFull;
    zfbool resolved;
    ZFPathInfo *pathInfo;
    _ZFP_ZFSerializableDataAttributeMapType attributes;
    zfstldeque<ZFSerializableData> elements;
    _ZFP_ZFSerializableDataTagMapType serializableDataTagMap;

public:
    void removeAll(void)
    {
        this->classNameFull.clear();
        this->attributes.clear();
        if(!this->elements.empty())
        {
            for(zfstlsize i = this->elements.size() - 1; i != (zfstlsize)-1; --i)
            {
                this->elements[i].d->serializableDataParent = zfnull;
            }
            this->elements.clear();
        }

        this->serializableDataTagMap.clear();
    }

public:
    _ZFP_ZFSerializableDataPrivate(void)
    : refCount(1)
    , serializableDataParent(zfnull)
    , classNameFull()
    , resolved(zffalse)
    , pathInfo(zfnull)
    , attributes()
    , elements()
    , serializableDataTagMap()
    {
    }
    ~_ZFP_ZFSerializableDataPrivate(void)
    {
        if(!this->elements.empty())
        {
            for(zfstlsize i = 0; i < this->elements.size(); ++i)
            {
                this->elements[i].d->serializableDataParent = zfnull;
            }
        }

        zfdelete(this->pathInfo);
    }
};

// ============================================================
// ZFSerializableData
ZFSerializableData::ZFSerializableData(ZF_IN _ZFP_ZFSerializableDataPrivate *d)
{
    this->d = d;
    ++(d->refCount);
}
ZFSerializableData::ZFSerializableData(void)
{
    d = zfnew(_ZFP_ZFSerializableDataPrivate);
}
ZFSerializableData::ZFSerializableData(ZF_IN const ZFSerializableData &ref)
{
    d = ref.d;
    ++(d->refCount);
}
ZFSerializableData &ZFSerializableData::operator = (ZF_IN const ZFSerializableData &ref)
{
    _ZFP_ZFSerializableDataPrivate *dTmp = d;
    d = ref.d;
    ++(ref.d->refCount);
    --(dTmp->refCount);
    if(dTmp->refCount == 0)
    {
        zfdelete(dTmp);
    }
    return *this;
}
zfbool ZFSerializableData::operator == (ZF_IN const ZFSerializableData &ref) const
{
    return (d == ref.d);
}
ZFSerializableData::~ZFSerializableData(void)
{
    --(d->refCount);
    if(d->refCount == 0)
    {
        zfdelete(d);
    }
}

void ZFSerializableData::copyFrom(ZF_IN const ZFSerializableData &ref)
{
    if(d == ref.d)
    {
        return ;
    }

    d->classNameFull = ref.d->classNameFull;
    d->attributes = ref.d->attributes;

    if(d->pathInfo == zfnull)
    {
        if(ref.d->pathInfo != zfnull)
        {
            d->pathInfo = zfnew(ZFPathInfo, *(ref.d->pathInfo));
        }
    }
    else
    {
        if(ref.d->pathInfo == zfnull)
        {
            zfdelete(d->pathInfo);
            d->pathInfo = zfnull;
        }
        else
        {
            *(d->pathInfo) = *(ref.d->pathInfo);
        }
    }

    d->serializableDataTagMap = ref.d->serializableDataTagMap;

    this->elementRemoveAll();
    for(zfstlsize i = 0; i < ref.d->elements.size(); ++i)
    {
        this->elementAdd(ref.d->elements[i].copy());
    }
}

zfindex ZFSerializableData::objectRetainCount(void) const
{
    return d->refCount;
}

// ============================================================
// local path logic
const ZFPathInfo *ZFSerializableData::pathInfo(void) const
{
    return d->pathInfo;
}
void ZFSerializableData::pathInfo(ZF_IN const ZFPathInfo *pathInfo)
{
    if(pathInfo != zfnull && !(pathInfo->pathType.isEmpty() && pathInfo->pathData.isEmpty()))
    {
        if(d->pathInfo == zfnull)
        {
            d->pathInfo = zfnew(ZFPathInfo, *pathInfo);
        }
        else
        {
            *(d->pathInfo) = *pathInfo;
        }
    }
    else
    {
        if(d->pathInfo != zfnull)
        {
            zfdelete(d->pathInfo);
            d->pathInfo = zfnull;
        }
    }
}
void ZFSerializableData::pathInfo(ZF_IN const zfchar *pathType, ZF_IN const zfchar *pathData)
{
    if(!zfsIsEmpty(pathType) || !zfsIsEmpty(pathData))
    {
        if(d->pathInfo == zfnull)
        {
            d->pathInfo = zfnew(ZFPathInfo, pathType, pathData);
        }
        else
        {
            d->pathInfo->pathType = pathType;
            d->pathInfo->pathData = pathData;
        }
    }
    else
    {
        if(d->pathInfo != zfnull)
        {
            zfdelete(d->pathInfo);
            d->pathInfo = zfnull;
        }
    }
}
const ZFPathInfo *ZFSerializableData::pathInfoCheck(void) const
{
    _ZFP_ZFSerializableDataPrivate *check = d;
    do
    {
        if(check->pathInfo != zfnull && !check->pathInfo->pathData.isEmpty())
        {
            return check->pathInfo;
        }
        check = check->serializableDataParent;
    } while(check != zfnull);
    return zfnull;
}

// ============================================================
// parent
zfbool ZFSerializableData::serializableDataParent(ZF_OUT ZFSerializableData &ret) const
{
    if(d->serializableDataParent != zfnull)
    {
        ret = ZFSerializableData(d->serializableDataParent);
        return zftrue;
    }
    else
    {
        return zffalse;
    }
}

// ============================================================
// class
void ZFSerializableData::itemClass(ZF_IN const zfchar *classNameFull)
{
    if(classNameFull == zfnull)
    {
        d->classNameFull.clear();
    }
    else
    {
        d->classNameFull = classNameFull;
    }
}
const zfchar *ZFSerializableData::itemClass(void) const
{
    return d->classNameFull.empty() ? zfnull : d->classNameFull.c_str();
}

// ============================================================
// other types
void ZFSerializableData::removeAll(void)
{
    d->removeAll();
}

// ============================================================
void ZFSerializableData::serializableDataTag(ZF_IN const zfchar *key,
                                             ZF_IN ZFObject *tag)
{
    if(key == zfnull)
    {
        return ;
    }

    _ZFP_ZFSerializableDataTagMapType &m = d->serializableDataTagMap;
    _ZFP_ZFSerializableDataTagMapType::iterator it = m.find(key);
    if(it == m.end())
    {
        if(tag != zfnull)
        {
            m[key] = tag;
        }
    }
    else
    {
        if(tag == zfnull)
        {
            zfautoObject holder = it->second;
            ZFUNUSED(holder);
            m.erase(it);
        }
        else
        {
            zfautoObject holder = it->second;
            ZFUNUSED(holder);
            it->second = tag;
        }
    }
}
ZFObject *ZFSerializableData::serializableDataTag(ZF_IN const zfchar *key) const
{
    if(key != zfnull)
    {
        _ZFP_ZFSerializableDataTagMapType &m = d->serializableDataTagMap;
        _ZFP_ZFSerializableDataTagMapType::iterator it = m.find(key);
        if(it != m.end())
        {
            return it->second.toObject();
        }
    }
    return zfnull;
}
void ZFSerializableData::serializableDataTagGetAllKeyValue(ZF_IN_OUT ZFCoreArray<const zfchar *> &allKey,
                                                           ZF_IN_OUT ZFCoreArray<ZFObject *> &allValue) const
{
    _ZFP_ZFSerializableDataTagMapType &m = d->serializableDataTagMap;
    allKey.capacity(allKey.count() + m.size());
    allValue.capacity(allValue.count() + m.size());
    for(_ZFP_ZFSerializableDataTagMapType::iterator it = m.begin(); it != m.end(); ++it)
    {
        allKey.add(it->first.c_str());
        allValue.add(it->second.toObject());
    }
}
void ZFSerializableData::serializableDataTagRemove(ZF_IN const zfchar *key)
{
    if(key != zfnull)
    {
        _ZFP_ZFSerializableDataTagMapType &m = d->serializableDataTagMap;
        _ZFP_ZFSerializableDataTagMapType::iterator it = m.find(key);
        if(it != m.end())
        {
            m.erase(it);
        }
    }
}
zfautoObject ZFSerializableData::serializableDataTagRemoveAndGet(ZF_IN const zfchar *key)
{
    if(key != zfnull)
    {
        _ZFP_ZFSerializableDataTagMapType &m = d->serializableDataTagMap;
        _ZFP_ZFSerializableDataTagMapType::iterator it = m.find(key);
        if(it != m.end())
        {
            zfautoObject ret = it->second;
            m.erase(it);
            return ret;
        }
    }
    return zfnull;
}
void ZFSerializableData::serializableDataTagRemoveAll(void)
{
    if(!d->serializableDataTagMap.empty())
    {
        _ZFP_ZFSerializableDataTagMapType tmp;
        tmp.swap(d->serializableDataTagMap);
    }
}

// ============================================================
// attribute
void ZFSerializableData::attributeForName(ZF_IN const zfchar *name,
                                          ZF_IN const zfchar *value)
{
    if(name != zfnull)
    {
        if(value != zfnull)
        {
            _ZFP_ZFSerializableDataAttributeMapType::iterator it = d->attributes.find(name);
            if(it != d->attributes.end())
            {
                it->second = _ZFP_ZFSerializableDataAttributeData(value);
            }
            else
            {
                d->attributes[name] = _ZFP_ZFSerializableDataAttributeData(value);
            }
        }
        else
        {
            d->attributes.erase(name);
        }
    }
}
const zfchar *ZFSerializableData::attributeForName(ZF_IN const zfchar *name) const
{
    if(name != zfnull)
    {
        _ZFP_ZFSerializableDataAttributeMapType::iterator it = d->attributes.find(name);
        if(it != d->attributes.end())
        {
            return it->second.attrValue.c_str();
        }
    }
    return zfnull;
}

zfindex ZFSerializableData::attributeCount(void) const
{
    return (zfindex)d->attributes.size();
}
void ZFSerializableData::attributeRemove(ZF_IN const zfchar *name)
{
    if(name != zfnull)
    {
        d->attributes.erase(name);
    }
}
void ZFSerializableData::attributeRemoveAll(void)
{
    d->attributes.clear();
}

static void _ZFP_ZFSerializableData_iteratorDeleteCallback(void *data)
{
    zfdelete(ZFCastStatic(_ZFP_ZFSerializableDataAttributeMapType::iterator *, data));
}
static void *_ZFP_ZFSerializableData_iteratorCopyCallback(void *data)
{
    return zfnew(_ZFP_ZFSerializableDataAttributeMapType::iterator,
        *ZFCastStatic(_ZFP_ZFSerializableDataAttributeMapType::iterator *, data));
}
zfiterator ZFSerializableData::attributeIteratorForName(ZF_IN const zfchar *name) const
{
    return zfiterator(
        zfnew(_ZFP_ZFSerializableDataAttributeMapType::iterator, name ? d->attributes.find(name) : d->attributes.end()),
        _ZFP_ZFSerializableData_iteratorDeleteCallback,
        _ZFP_ZFSerializableData_iteratorCopyCallback);
}
zfiterator ZFSerializableData::attributeIterator(void) const
{
    return zfiterator(
        zfnew(_ZFP_ZFSerializableDataAttributeMapType::iterator, d->attributes.begin()),
        _ZFP_ZFSerializableData_iteratorDeleteCallback,
        _ZFP_ZFSerializableData_iteratorCopyCallback);
}
zfbool ZFSerializableData::attributeIteratorIsValid(ZF_IN const zfiterator &it) const
{
    _ZFP_ZFSerializableDataAttributeMapType::iterator *data = it.data<_ZFP_ZFSerializableDataAttributeMapType::iterator *>();
    return (data != zfnull && *data != d->attributes.end());
}
zfbool ZFSerializableData::attributeIteratorIsEqual(ZF_IN const zfiterator &it0,
                                                    ZF_IN const zfiterator &it1) const
{
    return zfiterator::iteratorIsEqual<_ZFP_ZFSerializableDataAttributeMapType::iterator *>(it0, it1);
}
void ZFSerializableData::attributeIteratorValue(ZF_IN_OUT zfiterator &it,
                                                ZF_IN const zfchar *value)
{
    if(value == zfnull)
    {
        this->attributeIteratorRemove(it);
        return ;
    }
    _ZFP_ZFSerializableDataAttributeMapType::iterator *data = it.data<_ZFP_ZFSerializableDataAttributeMapType::iterator *>();
    if(data)
    {
        (*data)->second = _ZFP_ZFSerializableDataAttributeData(value);
    }
}
void ZFSerializableData::attributeIteratorRemove(ZF_IN_OUT zfiterator &it)
{
    _ZFP_ZFSerializableDataAttributeMapType::iterator *data = it.data<_ZFP_ZFSerializableDataAttributeMapType::iterator *>();
    if(data != zfnull)
    {
        d->attributes.erase((*data)++);
    }
}
const zfchar *ZFSerializableData::attributeIteratorKey(ZF_IN const zfiterator &it) const
{
    _ZFP_ZFSerializableDataAttributeMapType::iterator *data = it.data<_ZFP_ZFSerializableDataAttributeMapType::iterator *>();
    if(data != zfnull)
    {
        return (*data)->first.c_str();
    }
    return zfnull;
}
const zfchar *ZFSerializableData::attributeIteratorValue(ZF_IN const zfiterator &it) const
{
    _ZFP_ZFSerializableDataAttributeMapType::iterator *data = it.data<_ZFP_ZFSerializableDataAttributeMapType::iterator *>();
    if(data != zfnull)
    {
        return (*data)->second.attrValue.c_str();
    }
    return zfnull;
}
const zfchar *ZFSerializableData::attributeIteratorNextKey(ZF_IN_OUT zfiterator &it) const
{
    _ZFP_ZFSerializableDataAttributeMapType::iterator *data = it.data<_ZFP_ZFSerializableDataAttributeMapType::iterator *>();
    if(data != zfnull)
    {
        const zfchar *ret = (*data)->first.c_str();
        ++(*data);
        return ret;
    }
    return zfnull;
}
const zfchar *ZFSerializableData::attributeIteratorNextValue(ZF_IN_OUT zfiterator &it) const
{
    _ZFP_ZFSerializableDataAttributeMapType::iterator *data = it.data<_ZFP_ZFSerializableDataAttributeMapType::iterator *>();
    if(data != zfnull)
    {
        _ZFP_ZFSerializableDataAttributeData &ret = (*data)->second;
        ++(*data);
        return ret.attrValue.c_str();
    }
    return zfnull;
}

zfbool ZFSerializableData::attributeIteratorResolved(ZF_IN const zfiterator &it) const
{
    if(ZFSerializableDataResolveCheckEnable)
    {
        _ZFP_ZFSerializableDataAttributeMapType::iterator *data = it.data<_ZFP_ZFSerializableDataAttributeMapType::iterator *>();
        if(data != zfnull)
        {
            return (*data)->second.resolved;
        }
    }
    return zffalse;
}
void ZFSerializableData::attributeIteratorResolveMark(ZF_IN const zfiterator &it) const
{
    if(ZFSerializableDataResolveCheckEnable)
    {
        _ZFP_ZFSerializableDataAttributeMapType::iterator *data = it.data<_ZFP_ZFSerializableDataAttributeMapType::iterator *>();
        if(data != zfnull)
        {
            (*data)->second.resolved = zftrue;
        }
    }
}
void ZFSerializableData::attributeIteratorResolveUnmark(ZF_IN const zfiterator &it) const
{
    if(ZFSerializableDataResolveCheckEnable)
    {
        _ZFP_ZFSerializableDataAttributeMapType::iterator *data = it.data<_ZFP_ZFSerializableDataAttributeMapType::iterator *>();
        if(data != zfnull)
        {
            (*data)->second.resolved = zftrue;
        }
    }
}

// ============================================================
// element
void ZFSerializableData::elementAdd(ZF_IN const ZFSerializableData &element)
{
    zfCoreAssertWithMessage(d != element.d, "adding self is not allowed");
    zfCoreAssertWithMessage(d->serializableDataParent == zfnull, "adding a data which already has parent");
    d->elements.push_back(element);
    element.d->serializableDataParent = d;
}
void ZFSerializableData::elementAdd(ZF_IN const ZFSerializableData &element,
                                    ZF_IN zfindex atIndex)
{
    zfCoreAssertWithMessage(d != element.d, "adding self is not allowed");
    zfCoreAssertWithMessage(d->serializableDataParent == zfnull, "adding a data which already has parent");
    if(atIndex == zfindexMax())
    {
        atIndex = (zfindex)d->elements.size();
    }
    if(atIndex > (zfindex)d->elements.size())
    {
        zfCoreCriticalIndexOutOfRange(atIndex, (zfindex)(d->elements.size() + 1));
        return ;
    }

    d->elements.insert(d->elements.begin() + atIndex, element);
    element.d->serializableDataParent = d;
}

void ZFSerializableData::elementSetAtIndex(ZF_IN zfindex index,
                                           ZF_IN const ZFSerializableData &element)
{
    d->elements[(zfstlsize)index].d->serializableDataParent = zfnull;
    d->elements[(zfstlsize)index] = element;
    element.d->serializableDataParent = d;
}

zfindex ZFSerializableData::elementFindByName(ZF_IN const zfchar *name) const
{
    if(name != zfnull && *name != '\0')
    {
        for(zfstlsize i = 0; i < d->elements.size(); ++i)
        {
            if(zfscmpTheSame(d->elements[i].propertyName(), name))
            {
                return i;
            }
        }
    }
    return zfindexMax();
}
zfindex ZFSerializableData::elementFindByCategory(ZF_IN const zfchar *category) const
{
    if(category != zfnull && *category != '\0')
    {
        for(zfstlsize i = 0; i < d->elements.size(); ++i)
        {
            if(zfscmpTheSame(d->elements[i].category(), category))
            {
                return i;
            }
        }
    }
    return zfindexMax();
}

zfindex ZFSerializableData::elementCount(void) const
{
    return (zfindex)d->elements.size();
}
const ZFSerializableData &ZFSerializableData::elementAtIndex(ZF_IN zfindex index) const
{
    return d->elements[(zfstlsize)index];
}
ZFSerializableData &ZFSerializableData::elementAtIndex(ZF_IN zfindex index)
{
    return d->elements[(zfstlsize)index];
}

void ZFSerializableData::elementRemoveAtIndex(ZF_IN zfindex index)
{
    d->elements[(zfstlsize)index].d->serializableDataParent = zfnull;
    d->elements.erase(d->elements.begin() + (zfstlsize)index);
}
void ZFSerializableData::elementRemoveAll(void)
{
    if(!d->elements.empty())
    {
        for(zfstlsize i = 0; i < d->elements.size(); ++i)
        {
            d->elements[i].d->serializableDataParent = zfnull;
        }
        d->elements.clear();
    }
}

// ============================================================
// resolved state
zfbool ZFSerializableData::resolved(void) const
{
    return ZFSerializableDataResolveCheckEnable && d->resolved;
}
void ZFSerializableData::resolveMark(void) const
{
    if(ZFSerializableDataResolveCheckEnable)
    {
        d->resolved = zftrue;
    }
}
void ZFSerializableData::resolveUnmark(void) const
{
    if(ZFSerializableDataResolveCheckEnable)
    {
        d->resolved = zffalse;
    }
}

zfbool ZFSerializableData::resolvedAttribute(ZF_IN const zfchar *name) const
{
    if(ZFSerializableDataResolveCheckEnable && name != zfnull)
    {
        _ZFP_ZFSerializableDataAttributeMapType::iterator it = d->attributes.find(name);
        if(it != d->attributes.end())
        {
            return it->second.resolved;
        }
    }
    return zffalse;
}
void ZFSerializableData::resolveAttributeMark(ZF_IN const zfchar *name) const
{
    if(ZFSerializableDataResolveCheckEnable && name != zfnull)
    {
        _ZFP_ZFSerializableDataAttributeMapType::iterator it = d->attributes.find(name);
        if(it != d->attributes.end())
        {
            it->second.resolved = zftrue;
        }
    }
}
void ZFSerializableData::resolveAttributeUnmark(ZF_IN const zfchar *name) const
{
    if(ZFSerializableDataResolveCheckEnable && name != zfnull)
    {
        _ZFP_ZFSerializableDataAttributeMapType::iterator it = d->attributes.find(name);
        if(it != d->attributes.end())
        {
            it->second.resolved = zffalse;
        }
    }
}

zfbool ZFSerializableData::resolvedAll(ZF_OUT_OPT const ZFSerializableData **firstNotResolvedElement /* = zfnull */,
                                       ZF_OUT_OPT zfstring *firstNotResolvedAttribute /* = zfnull */) const
{
    if(!ZFSerializableDataResolveCheckEnable)
    {
        return zffalse;
    }
    if(!this->resolved())
    {
        if(firstNotResolvedElement != zfnull)
        {
            *firstNotResolvedElement = this;
        }
        return zffalse;
    }
    for(_ZFP_ZFSerializableDataAttributeMapType::iterator it = d->attributes.begin(); it != d->attributes.end(); ++it)
    {
        if(!it->second.resolved)
        {
            if(firstNotResolvedElement != zfnull)
            {
                *firstNotResolvedElement = this;
            }
            if(firstNotResolvedAttribute != zfnull)
            {
                *firstNotResolvedAttribute += it->first.c_str();
            }
            return zffalse;
        }
    }
    for(zfindex i = 0; i < this->elementCount(); ++i)
    {
        if(!this->elementAtIndex(i).resolvedAll(firstNotResolvedElement, firstNotResolvedAttribute))
        {
            return zffalse;
        }
    }
    return zftrue;
}
void ZFSerializableData::resolveMarkAll(void) const
{
    if(ZFSerializableDataResolveCheckEnable)
    {
        this->resolveMark();
        this->resolveAttributeMarkAll();
        for(zfindex i = 0; i < this->elementCount(); ++i)
        {
            this->elementAtIndex(i).resolveMarkAll();
        }
    }
}
void ZFSerializableData::resolveUnmarkAll(void) const
{
    if(ZFSerializableDataResolveCheckEnable)
    {
        this->resolveUnmark();
        this->resolveAttributeUnmarkAll();
        for(zfindex i = 0; i < this->elementCount(); ++i)
        {
            this->elementAtIndex(i).resolveUnmarkAll();
        }
    }
}
void ZFSerializableData::resolveAttributeMarkAll(void) const
{
    if(ZFSerializableDataResolveCheckEnable)
    {
        for(_ZFP_ZFSerializableDataAttributeMapType::iterator it = d->attributes.begin(); it != d->attributes.end(); ++it)
        {
            it->second.resolved = zftrue;
        }
    }
}
void ZFSerializableData::resolveAttributeUnmarkAll(void) const
{
    if(ZFSerializableDataResolveCheckEnable)
    {
        for(_ZFP_ZFSerializableDataAttributeMapType::iterator it = d->attributes.begin(); it != d->attributes.end(); ++it)
        {
            it->second.resolved = zftrue;
        }
    }
}

// ============================================================
// other functions
void ZFSerializableData::objectInfoT(ZF_IN_OUT zfstring &ret) const
{
    ZFSerializableDataToZfsd(ret, *this);
}

ZFCompareResult ZFSerializableData::objectCompare(ZF_IN const ZFSerializableData &another) const
{
    if(d == another.d)
    {
        return ZFCompareTheSame;
    }

    if(!zfscmpTheSame(this->itemClass(), another.itemClass())
        || this->attributeCount() != another.attributeCount()
        || this->elementCount() != another.elementCount())
    {
        return ZFCompareUncomparable;
    }

    for(zfiterator it = another.attributeIterator(); another.attributeIteratorIsValid(it); another.attributeIteratorNextValue(it))
    {
        const zfchar *value = this->attributeForName(another.attributeIteratorKey(it));
        if(value == zfnull || !zfscmpTheSame(another.attributeIteratorValue(it), value))
        {
            return ZFCompareUncomparable;
        }
    }
    for(zfindex i = 0; i < this->elementCount(); ++i)
    {
        if(this->elementAtIndex(i).objectCompare(another.elementAtIndex(i)) != ZFCompareTheSame)
        {
            return ZFCompareUncomparable;
        }
    }
    return ZFCompareTheSame;
}

zfbool ZFSerializableData::isEmpty(void) const
{
    return (
        d->classNameFull.empty()
        && d->attributes.empty()
        && d->elements.empty()
        );
}

ZF_NAMESPACE_GLOBAL_END

