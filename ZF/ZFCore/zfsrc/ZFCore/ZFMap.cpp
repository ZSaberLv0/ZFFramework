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
ZFMETHOD_DEFINE_0(ZFMap, ZFCoreArrayPOD<ZFObject *>, allKey)
{
    ZFCoreArrayPOD<ZFObject *> ret;
    this->allKeyT(ret);
    return ret;
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
ZFMETHOD_DEFINE_0(ZFMap, ZFCoreArrayPOD<ZFObject *>, allValue)
{
    ZFCoreArrayPOD<ZFObject *> ret;
    this->allValueT(ret);
    return ret;
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
ZFMETHOD_DEFINE_0(ZFMap, ZFCoreArrayPOD<ZFKeyValuePair>, allPair)
{
    ZFCoreArrayPOD<ZFKeyValuePair> ret;
    this->allPairT(ret);
    return ret;
}

ZFMETHOD_DEFINE_1(ZFMap, void, addFrom,
                  ZFMP_IN(ZFKeyValueContainer *, another))
{
    if(another == this || another == zfnull)
    {
        return ;
    }

    ZFKeyValuePair pair = ZFKeyValuePairZero();
    for(zfiterator it = another->iterator(); another->iteratorValid(it); another->iteratorNext(it))
    {
        pair = another->iteratorPair(it);

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

ZFMETHOD_DEFINE_2(ZFMap, void, set,
                  ZFMP_IN(ZFObject *, pKey),
                  ZFMP_IN(ZFObject *, pValue))
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

ZFMETHOD_DEFINE_1(ZFMap, void, remove,
                  ZFMP_IN(ZFObject *, pKey))
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
ZFMETHOD_DEFINE_1(ZFMap, zfautoObject, removeAndGet,
                  ZFMP_IN(ZFObject *, pKey))
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
ZFMETHOD_DEFINE_1(ZFMap, ZFKeyValuePairHolder, removeAndGetPair,
                  ZFMP_IN(ZFObject *, pKey))
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
ZFMETHOD_DEFINE_0(ZFMap, void, removeAll)
{
    if(!d->data.empty())
    {
        _ZFP_ZFMapPrivate::MapType tmp;
        tmp.swap(d->data);
        for(_ZFP_ZFMapPrivate::MapType::iterator it = d->data.begin(); it != d->data.end(); ++it)
        {
            this->contentOnRemove(it->first, it->second);
            zfRelease(it->first);
            zfRelease(it->second);
        }
        this->contentOnChange();
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
ZFMETHOD_DEFINE_1(ZFMap, zfiterator, iteratorFind,
                  ZFMP_IN(ZFObject *, key))
{
    return zfiterator(zfnew(_ZFP_ZFMapPrivate::MapType::iterator, d->data.find(key)),
        _ZFP_ZFMap_iteratorDeleteCallback,
        _ZFP_ZFMap_iteratorCopyCallback);
}

ZFMETHOD_DEFINE_1(ZFMap, zfbool, iteratorValid,
                  ZFMP_IN(const zfiterator &, it))
{
    _ZFP_ZFMapPrivate::MapType::iterator *data = it.data<_ZFP_ZFMapPrivate::MapType::iterator *>();
    return (data != zfnull && *data != d->data.end());
}
ZFMETHOD_DEFINE_2(ZFMap, zfbool, iteratorEqual,
                  ZFMP_IN(const zfiterator &, it0),
                  ZFMP_IN(const zfiterator &, it1))
{
    return zfiterator::iteratorEqual<_ZFP_ZFMapPrivate::MapType::iterator *>(it0, it1);
}

ZFMETHOD_DEFINE_1(ZFMap, void, iteratorNext,
                  ZFMP_IN_OUT(zfiterator &, it))
{
    _ZFP_ZFMapPrivate::MapType::iterator *data = it.data<_ZFP_ZFMapPrivate::MapType::iterator *>();
    if(data != zfnull && *data != d->data.end())
    {
        data->operator ++ ();
    }
}
ZFMETHOD_DEFINE_1(ZFMap, void, iteratorPrev,
                  ZFMP_IN_OUT(zfiterator &, it))
{
    _ZFP_ZFMapPrivate::MapType::iterator *data = it.data<_ZFP_ZFMapPrivate::MapType::iterator *>();
    if(data != zfnull && *data != d->data.end())
    {
        data->operator -- ();
    }
}
ZFMETHOD_DEFINE_1(ZFMap, ZFObject *, iteratorValue,
                  ZFMP_IN(const zfiterator &, it))
{
    _ZFP_ZFMapPrivate::MapType::iterator *data = it.data<_ZFP_ZFMapPrivate::MapType::iterator *>();
    return ((data != zfnull && *data != d->data.end()) ? (*data)->second : zfnull);
}

ZFMETHOD_DEFINE_2(ZFMap, void, iteratorValue,
                  ZFMP_IN_OUT(zfiterator &, it),
                  ZFMP_IN(ZFObject *, value))
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
ZFMETHOD_DEFINE_1(ZFMap, void, iteratorRemove,
                  ZFMP_IN_OUT(zfiterator &, it))
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
ZFMETHOD_DEFINE_2(ZFMap, void, iteratorAdd,
                  ZFMP_IN(ZFObject *, key),
                  ZFMP_IN(ZFObject *, value))
{
    this->set(key, value);
}

ZF_NAMESPACE_GLOBAL_END

