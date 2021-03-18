#include "ZFMap.h"
#include "ZFSTLWrapper/zfstl_map.h"
#include "ZFSTLWrapper/zfstl_vector.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// _ZFP_ZFMapPrivate
zfclassNotPOD _ZFP_ZFMapKeyComparer
{
public:
    inline zfbool operator () (ZF_IN ZFObject *obj1,
                               ZF_IN ZFObject *obj2) const
    {
        ZFCompareResult cmp = ZFObjectCompare(obj1, obj2);
        zfCoreAssertWithMessageTrim(cmp != ZFCompareUncomparable, "[ZFMap] key must comparable: %s, %s",
            ZFObjectInfo(obj1).cString(),
            ZFObjectInfo(obj2).cString());
        return (cmp == ZFCompareSmaller);
    }
};
zfclassNotPOD _ZFP_ZFMapPrivate
{
public:
    typedef zfstlmap<ZFObject *, ZFObject *, _ZFP_ZFMapKeyComparer> MapType;

public:
    MapType data;

public:
    _ZFP_ZFMapPrivate(void)
    : data()
    {
    }
};

// ============================================================
// ZFMap
ZFOBJECT_REGISTER(ZFMap)

ZFOBJECT_ON_INIT_DEFINE_1(ZFMap,
                          ZFMP_IN(ZFKeyValueContainer *, another))
{
    this->objectOnInit();
    if(another != zfnull)
    {
        zfself::addFrom(another);
    }
}
void ZFMap::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFMapPrivate);
}
void ZFMap::objectOnDealloc(void)
{
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}

ZFCompareResult ZFMap::objectCompare(ZF_IN ZFObject *anotherObj)
{
    if(this == anotherObj) {return ZFCompareTheSame;}
    zfself *another = ZFCastZFObject(zfself *, anotherObj);
    if(another == zfnull) {return ZFCompareUncomparable;}

    if(this->count() != another->count()
       || this->objectHash() != another->objectHash())
    {
        return ZFCompareUncomparable;
    }

    // ZFMap is sorted, compare by order for performance
    for(zfiterator it0 = this->iterator(),
        it1 = another->iterator();
        this->iteratorIsValid(it0);)
    {
        ZFKeyValuePair pair0 = this->iteratorNextPair(it0);
        ZFKeyValuePair pair1 = another->iteratorNextPair(it1);
        if(ZFObjectCompare(pair0.key, pair1.key) != ZFCompareTheSame
            || ZFObjectCompare(pair0.value, pair1.value) != ZFCompareTheSame)
        {
            return ZFCompareUncomparable;
        }
    }
    return ZFCompareTheSame;
}

ZFMETHOD_DEFINE_0(ZFMap, zfindex, count)
{
    return d->data.size();
}

ZFMETHOD_DEFINE_0(ZFMap, zfbool, isEmpty)
{
    return d->data.empty();
}

ZFMETHOD_DEFINE_1(ZFMap, zfbool, isContain,
                  ZFMP_IN(ZFObject *, pKey))
{
    return (pKey != zfnull && d->data.find(pKey) != d->data.end());
}

ZFMETHOD_DEFINE_1(ZFMap, ZFObject *, get,
                  ZFMP_IN(ZFObject *, pKey))
{
    if(pKey != zfnull)
    {
        _ZFP_ZFMapPrivate::MapType::const_iterator it = d->data.find(pKey);
        if(it != d->data.end())
        {
            return it->second;
        }
    }
    return zfnull;
}
ZFMETHOD_DEFINE_1(ZFMap, ZFKeyValuePair, getPair,
                  ZFMP_IN(ZFObject *, pKey))
{
    ZFKeyValuePair ret = ZFKeyValuePairZero();
    if(pKey != zfnull)
    {
        _ZFP_ZFMapPrivate::MapType::const_iterator it = d->data.find(pKey);
        if(it != d->data.end())
        {
            ret.key = it->first;
            ret.value = it->second;
        }
    }
    return ret;
}

ZFMETHOD_DEFINE_1(ZFMap, void, allKeyT,
                  ZFMP_IN_OUT(ZFCoreArray<ZFObject *> &, ret))
{
    if(!this->isEmpty())
    {
        ret.capacity(ret.count() + this->count());
        for(_ZFP_ZFMapPrivate::MapType::const_iterator it = d->data.begin(); it != d->data.end(); ++it)
        {
            ret.add(it->first);
        }
    }
}
ZFMETHOD_DEFINE_1(ZFMap, void, allValueT,
                  ZFMP_IN_OUT(ZFCoreArray<ZFObject *> &, ret))
{
    if(!this->isEmpty())
    {
        ret.capacity(ret.count() + this->count());
        for(_ZFP_ZFMapPrivate::MapType::const_iterator it = d->data.begin(); it != d->data.end(); ++it)
        {
            ret.add(it->second);
        }
    }
}
ZFMETHOD_DEFINE_1(ZFMap, void, allPairT,
                  ZFMP_IN_OUT(ZFCoreArray<ZFKeyValuePair> &, ret))
{
    if(!this->isEmpty())
    {
        ret.capacity(ret.count() + this->count());
        ZFKeyValuePair pair;
        for(_ZFP_ZFMapPrivate::MapType::const_iterator it = d->data.begin(); it != d->data.end(); ++it)
        {
            pair.key = it->first;
            pair.value = it->second;
            ret.add(pair);
        }
    }
}

void ZFMap::addFrom(ZF_IN ZFKeyValueContainer *another)
{
    if(another == this || another == zfnull)
    {
        return ;
    }

    ZFKeyValuePair pair = ZFKeyValuePairZero();
    for(zfiterator it = another->iterator(); another->iteratorIsValid(it); )
    {
        pair = another->iteratorNextPair(it);

        _ZFP_ZFMapPrivate::MapType::iterator itExisting = d->data.find(pair.key);
        if(itExisting != d->data.end())
        {
            this->contentOnRemove(itExisting->first, itExisting->second);
            ZFObject *tmp = itExisting->second;
            zfRetain(pair.value);
            itExisting->second = pair.value;
            this->contentOnAdd(itExisting->first, itExisting->second);
            zfRelease(tmp);
        }
        else
        {
            zfRetain(pair.key);
            zfRetain(pair.value);
            (d->data)[pair.key] = pair.value;
        }
    }

    if(pair.key != zfnull)
    {
        this->contentOnChange();
    }
}

void ZFMap::set(ZF_IN ZFObject *pKey,
                ZF_IN ZFObject *pValue)
{
    if(pKey == zfnull)
    {
        return ;
    }
    if(pValue == zfnull)
    {
        this->remove(pKey);
        return ;
    }

    _ZFP_ZFMapPrivate::MapType::iterator it = d->data.find(pKey);
    if(it != d->data.end())
    {
        this->contentOnRemove(it->first, it->second);
        ZFObject *tmp = it->second;
        zfRetain(pValue);
        it->second = pValue;
        this->contentOnAdd(it->first, it->second);
        zfRelease(tmp);
    }
    else
    {
        zfRetain(pKey);
        zfRetain(pValue);
        (d->data)[pKey] = pValue;
    }

    this->contentOnChange();
}

void ZFMap::remove(ZF_IN ZFObject *pKey)
{
    if(pKey != zfnull)
    {
        _ZFP_ZFMapPrivate::MapType::iterator it = d->data.find(pKey);
        if(it != d->data.end())
        {
            ZFObject *tmpKey = it->first;
            ZFObject *tmpValue = it->second;
            d->data.erase(it);
            this->contentOnRemove(tmpKey, tmpValue);
            zfRelease(tmpKey);
            zfRelease(tmpValue);

            this->contentOnChange();
        }
    }
}
zfautoObject ZFMap::removeAndGet(ZF_IN ZFObject *pKey)
{
    if(pKey != zfnull)
    {
        _ZFP_ZFMapPrivate::MapType::iterator it = d->data.find(pKey);
        if(it != d->data.end())
        {
            ZFObject *tmpKey = it->first;
            ZFObject *tmpValue = it->second;
            d->data.erase(it);

            this->contentOnRemove(tmpKey, tmpValue);
            this->contentOnChange();

            zfRelease(tmpKey);
            zfblockedRelease(tmpValue);
            return tmpValue;
        }
    }
    return zfnull;
}
ZFKeyValuePairHolder ZFMap::removeAndGetPair(ZF_IN ZFObject *pKey)
{
    ZFKeyValuePairHolder ret;
    if(pKey != zfnull)
    {
        _ZFP_ZFMapPrivate::MapType::iterator it = d->data.find(pKey);
        if(it != d->data.end())
        {
            zfblockedRelease(it->first);
            zfblockedRelease(it->second);
            ret.key = it->first;
            ret.value = it->second;
            d->data.erase(it);

            this->contentOnRemove(ret.key.toObject(), ret.value.toObject());
            this->contentOnChange();
            return ret;
        }
    }
    return ret;
}
void ZFMap::removeAll(void)
{
    if(!d->data.empty())
    {
        zfstlvector<ZFObject *> tmp;
        tmp.reserve(d->data.size() * 2);
        for(_ZFP_ZFMapPrivate::MapType::iterator it = d->data.begin(); it != d->data.end(); ++it)
        {
            tmp.push_back(it->first);
            tmp.push_back(it->second);
        }

        d->data.clear();
        this->contentOnChange();

        for(zfstlsize i = 0; i < tmp.size(); ++i)
        {
            zfRelease(tmp[i]);
        }
    }
}

// ============================================================
static void _ZFP_ZFMap_iteratorDeleteCallback(ZF_IN void *data)
{
    zfdelete(ZFCastStatic(_ZFP_ZFMapPrivate::MapType::iterator *, data));
}
static void *_ZFP_ZFMap_iteratorCopyCallback(ZF_IN void *data)
{
    return zfnew(_ZFP_ZFMapPrivate::MapType::iterator,
        *ZFCastStatic(_ZFP_ZFMapPrivate::MapType::iterator *, data));
}
ZFMETHOD_DEFINE_0(ZFMap, zfiterator, iterator)
{
    return zfiterator(zfnew(_ZFP_ZFMapPrivate::MapType::iterator, d->data.begin()),
        _ZFP_ZFMap_iteratorDeleteCallback,
        _ZFP_ZFMap_iteratorCopyCallback);
}

ZFMETHOD_DEFINE_1(ZFMap, zfbool, iteratorIsValid,
                  ZFMP_IN(const zfiterator &, it))
{
    _ZFP_ZFMapPrivate::MapType::iterator *data = it.data<_ZFP_ZFMapPrivate::MapType::iterator *>();
    return (data != zfnull && *data != d->data.end());
}
ZFMETHOD_DEFINE_2(ZFMap, zfbool, iteratorIsEqual,
                  ZFMP_IN(const zfiterator &, it0),
                  ZFMP_IN(const zfiterator &, it1))
{
    return zfiterator::iteratorIsEqual<_ZFP_ZFMapPrivate::MapType::iterator *>(it0, it1);
}

void ZFMap::iteratorValue(ZF_IN_OUT zfiterator &it,
                          ZF_IN ZFObject *value)
{
    _ZFP_ZFMapPrivate::MapType::iterator *data = it.data<_ZFP_ZFMapPrivate::MapType::iterator *>();
    if(data != zfnull && *data != d->data.end())
    {
        this->contentOnRemove((*data)->first, (*data)->second);
        ZFObject *tmp = (*data)->second;
        zfRetain(value);
        (*data)->second = value;
        this->contentOnAdd((*data)->first, (*data)->second);
        zfRelease(tmp);

        this->contentOnChange();
    }
}
void ZFMap::iteratorRemove(ZF_IN_OUT zfiterator &it)
{
    _ZFP_ZFMapPrivate::MapType::iterator *data = it.data<_ZFP_ZFMapPrivate::MapType::iterator *>();
    if(data != zfnull && *data != d->data.end())
    {
        ZFObject *tmpKey = (*data)->first;
        ZFObject *tmpValue = (*data)->second;
        d->data.erase((*data)++);

        this->contentOnRemove(tmpKey, tmpValue);
        this->contentOnChange();

        zfRelease(tmpKey);
        zfRelease(tmpValue);
    }
}

ZFMETHOD_DEFINE_1(ZFMap, zfiterator, iteratorForKey,
                  ZFMP_IN(ZFObject *, key))
{
    return zfiterator(zfnew(_ZFP_ZFMapPrivate::MapType::iterator, d->data.find(key)),
        _ZFP_ZFMap_iteratorDeleteCallback,
        _ZFP_ZFMap_iteratorCopyCallback);
}
ZFMETHOD_DEFINE_1(ZFMap, ZFObject *, iteratorKey,
                  ZFMP_IN(const zfiterator &, it))
{
    _ZFP_ZFMapPrivate::MapType::iterator *data = it.data<_ZFP_ZFMapPrivate::MapType::iterator *>();
    return ((data != zfnull && *data != d->data.end()) ? (*data)->first : zfnull);
}
ZFMETHOD_DEFINE_1(ZFMap, ZFObject *, iteratorValue,
                  ZFMP_IN(const zfiterator &, it))
{
    _ZFP_ZFMapPrivate::MapType::iterator *data = it.data<_ZFP_ZFMapPrivate::MapType::iterator *>();
    return ((data != zfnull && *data != d->data.end()) ? (*data)->second : zfnull);
}
ZFMETHOD_DEFINE_1(ZFMap, ZFKeyValuePair, iteratorPair,
                  ZFMP_IN(const zfiterator &, it))
{
    ZFKeyValuePair ret = ZFKeyValuePairZero();
    _ZFP_ZFMapPrivate::MapType::iterator *data = it.data<_ZFP_ZFMapPrivate::MapType::iterator *>();
    if(data != zfnull && *data != d->data.end())
    {
        ret.key = (*data)->first;
        ret.value = (*data)->second;
    }
    return ret;
}
ZFMETHOD_DEFINE_1(ZFMap, ZFObject *, iteratorNextKey,
                  ZFMP_IN_OUT(zfiterator &, it))
{
    _ZFP_ZFMapPrivate::MapType::iterator *data = it.data<_ZFP_ZFMapPrivate::MapType::iterator *>();
    if(data != zfnull && *data != d->data.end())
    {
        ZFObject *ret = (*data)->first;
        data->operator ++ ();
        return ret;
    }
    return zfnull;
}
ZFMETHOD_DEFINE_1(ZFMap, ZFObject *, iteratorNextValue,
                  ZFMP_IN_OUT(zfiterator &, it))
{
    _ZFP_ZFMapPrivate::MapType::iterator *data = it.data<_ZFP_ZFMapPrivate::MapType::iterator *>();
    if(data != zfnull && *data != d->data.end())
    {
        ZFObject *ret = (*data)->second;
        data->operator ++ ();
        return ret;
    }
    return zfnull;
}
ZFMETHOD_DEFINE_1(ZFMap, ZFKeyValuePair, iteratorNextPair,
                  ZFMP_IN_OUT(zfiterator &, it))
{
    ZFKeyValuePair ret = ZFKeyValuePairZero();
    _ZFP_ZFMapPrivate::MapType::iterator *data = it.data<_ZFP_ZFMapPrivate::MapType::iterator *>();
    if(data != zfnull && *data != d->data.end())
    {
        ret.key = (*data)->first;
        ret.value = (*data)->second;
        data->operator ++ ();
        return ret;
    }
    return ret;
}
ZFMETHOD_DEFINE_1(ZFMap, ZFObject *, iteratorPrevKey,
                  ZFMP_IN_OUT(zfiterator &, it))
{
    _ZFP_ZFMapPrivate::MapType::iterator *data = it.data<_ZFP_ZFMapPrivate::MapType::iterator *>();
    if(data != zfnull && *data != d->data.end())
    {
        ZFObject *ret = (*data)->first;
        data->operator -- ();
        return ret;
    }
    return zfnull;
}
ZFMETHOD_DEFINE_1(ZFMap, ZFObject *, iteratorPrevValue,
                  ZFMP_IN_OUT(zfiterator &, it))
{
    _ZFP_ZFMapPrivate::MapType::iterator *data = it.data<_ZFP_ZFMapPrivate::MapType::iterator *>();
    if(data != zfnull && *data != d->data.end())
    {
        ZFObject *ret = (*data)->second;
        data->operator -- ();
        return ret;
    }
    return zfnull;
}
ZFMETHOD_DEFINE_1(ZFMap, ZFKeyValuePair, iteratorPrevPair,
                  ZFMP_IN_OUT(zfiterator &, it))
{
    ZFKeyValuePair ret = ZFKeyValuePairZero();
    _ZFP_ZFMapPrivate::MapType::iterator *data = it.data<_ZFP_ZFMapPrivate::MapType::iterator *>();
    if(data != zfnull && *data != d->data.end())
    {
        ret.key = (*data)->first;
        ret.value = (*data)->second;
        data->operator -- ();
        return ret;
    }
    return ret;
}
void ZFMap::iteratorAddKeyValue(ZF_IN ZFObject *key,
                                ZF_IN ZFObject *value)
{
    this->set(key, value);
}
void ZFMap::iteratorAddKeyValue(ZF_IN ZFObject *key,
                                ZF_IN ZFObject *value,
                                ZF_IN_OUT zfiterator &it)
{
    this->set(key, value);
}

ZF_NAMESPACE_GLOBAL_END

