#include "ZFHashMap.h"
#include "ZFSTLWrapper/zfstl_hashmap.h"
#include "ZFSTLWrapper/zfstl_vector.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// _ZFP_ZFHashMapPrivate
zfclassNotPOD _ZFP_ZFHashMapKeyHasher
{
public:
    zfstlsize operator () (ZFObject *const &v) const
    {
        return (zfstlsize)v->objectHash();
    }
};
zfclassNotPOD _ZFP_ZFHashMapKeyComparer
{
public:
    zfbool operator () (ZFObject * const &v0, ZFObject * const &v1) const
    {
        return (v0->objectCompare(v1) == ZFCompareTheSame);
    }
};

zfclassNotPOD _ZFP_ZFHashMapPrivate
{
public:
    typedef zfstlhashmap<ZFObject *, ZFObject *, _ZFP_ZFHashMapKeyHasher, _ZFP_ZFHashMapKeyComparer> MapType;

public:
    MapType data;

public:
    _ZFP_ZFHashMapPrivate(void)
    : data()
    {
    }
};

// ============================================================
// ZFHashMap
ZFOBJECT_REGISTER(ZFHashMap)

ZFOBJECT_ON_INIT_DEFINE_1(ZFHashMap,
                          ZFMP_IN(ZFKeyValueContainer *, another))
{
    this->objectOnInit();
    if(another != zfnull)
    {
        zfself::addFrom(another);
    }
}
void ZFHashMap::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFHashMapPrivate);
}
void ZFHashMap::objectOnDealloc(void)
{
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}

ZFMETHOD_DEFINE_0(ZFHashMap, zfindex, count)
{
    return d->data.size();
}

ZFMETHOD_DEFINE_0(ZFHashMap, zfbool, isEmpty)
{
    return d->data.empty();
}

ZFMETHOD_DEFINE_1(ZFHashMap, zfbool, isContain,
                  ZFMP_IN(ZFObject *, pKey))
{
    return (pKey != zfnull && d->data.find(pKey) != d->data.end());
}

ZFMETHOD_DEFINE_1(ZFHashMap, ZFObject *, get,
                  ZFMP_IN(ZFObject *, pKey))
{
    if(pKey != zfnull)
    {
        _ZFP_ZFHashMapPrivate::MapType::const_iterator it = d->data.find(pKey);
        if(it != d->data.end())
        {
            return it->second;
        }
    }
    return zfnull;
}
ZFMETHOD_DEFINE_1(ZFHashMap, ZFKeyValuePair, getPair,
                  ZFMP_IN(ZFObject *, pKey))
{
    ZFKeyValuePair ret = ZFKeyValuePairZero();
    if(pKey != zfnull)
    {
        _ZFP_ZFHashMapPrivate::MapType::const_iterator it = d->data.find(pKey);
        if(it != d->data.end())
        {
            ret.key = it->first;
            ret.value = it->second;
        }
    }
    return ret;
}

ZFMETHOD_DEFINE_1(ZFHashMap, void, allKeyT,
                  ZFMP_IN_OUT(ZFCoreArray<ZFObject *> &, ret))
{
    if(!this->isEmpty())
    {
        ret.capacity(ret.count() + this->count());
        for(_ZFP_ZFHashMapPrivate::MapType::const_iterator it = d->data.begin(); it != d->data.end(); ++it)
        {
            ret.add(it->first);
        }
    }
}
ZFMETHOD_DEFINE_0(ZFHashMap, ZFCoreArrayPOD<ZFObject *>, allKey)
{
    ZFCoreArrayPOD<ZFObject *> ret;
    this->allKeyT(ret);
    return ret;
}

ZFMETHOD_DEFINE_1(ZFHashMap, void, allValueT,
                  ZFMP_IN_OUT(ZFCoreArray<ZFObject *> &, ret))
{
    if(!this->isEmpty())
    {
        ret.capacity(ret.count() + this->count());
        for(_ZFP_ZFHashMapPrivate::MapType::const_iterator it = d->data.begin(); it != d->data.end(); ++it)
        {
            ret.add(it->second);
        }
    }
}
ZFMETHOD_DEFINE_0(ZFHashMap, ZFCoreArrayPOD<ZFObject *>, allValue)
{
    ZFCoreArrayPOD<ZFObject *> ret;
    this->allValueT(ret);
    return ret;
}

ZFMETHOD_DEFINE_1(ZFHashMap, void, allPairT,
                  ZFMP_IN_OUT(ZFCoreArray<ZFKeyValuePair> &, ret))
{
    if(!this->isEmpty())
    {
        ret.capacity(ret.count() + this->count());
        ZFKeyValuePair pair;
        for(_ZFP_ZFHashMapPrivate::MapType::const_iterator it = d->data.begin(); it != d->data.end(); ++it)
        {
            pair.key = it->first;
            pair.value = it->second;
            ret.add(pair);
        }
    }
}
ZFMETHOD_DEFINE_0(ZFHashMap, ZFCoreArrayPOD<ZFKeyValuePair>, allPair)
{
    ZFCoreArrayPOD<ZFKeyValuePair> ret;
    this->allPairT(ret);
    return ret;
}

void ZFHashMap::addFrom(ZF_IN ZFKeyValueContainer *another)
{
    if(another == this || another == zfnull)
    {
        return ;
    }

    ZFKeyValuePair pair = ZFKeyValuePairZero();
    for(zfiterator it = another->iterator(); another->iteratorIsValid(it); )
    {
        pair = another->iteratorNextPair(it);

        _ZFP_ZFHashMapPrivate::MapType::iterator itExisting = d->data.find(pair.key);
        if(itExisting != d->data.end())
        {
            this->contentOnRemove(itExisting->first, itExisting->second);
            ZFObject *tmp = itExisting->second;
            zfRetain(pair.value);
            itExisting->second = pair.value;
            this->contentOnAdd(pair.key, pair.value);
            zfRelease(tmp);
        }
        else
        {
            zfRetain(pair.key);
            zfRetain(pair.value);
            (d->data)[pair.key] = pair.value;
            this->contentOnAdd(pair.key, pair.value);
        }
    }

    if(pair.key != zfnull)
    {
        this->contentOnChange();
    }
}

void ZFHashMap::set(ZF_IN ZFObject *pKey,
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

    _ZFP_ZFHashMapPrivate::MapType::iterator it = d->data.find(pKey);
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
        this->contentOnAdd(it->first, it->second);
    }

    this->contentOnChange();
}

void ZFHashMap::remove(ZF_IN ZFObject *pKey)
{
    if(pKey != zfnull)
    {
        _ZFP_ZFHashMapPrivate::MapType::iterator it = d->data.find(pKey);
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
zfautoObject ZFHashMap::removeAndGet(ZF_IN ZFObject *pKey)
{
    if(pKey != zfnull)
    {
        _ZFP_ZFHashMapPrivate::MapType::iterator it = d->data.find(pKey);
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
ZFKeyValuePairHolder ZFHashMap::removeAndGetPair(ZF_IN ZFObject *pKey)
{
    ZFKeyValuePairHolder ret;
    if(pKey != zfnull)
    {
        _ZFP_ZFHashMapPrivate::MapType::iterator it = d->data.find(pKey);
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
void ZFHashMap::removeAll(void)
{
    if(!d->data.empty())
    {
        zfstlvector<ZFObject *> tmp;
        tmp.reserve(d->data.size() * 2);
        for(_ZFP_ZFHashMapPrivate::MapType::iterator it = d->data.begin(); it != d->data.end(); ++it)
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
static void _ZFP_ZFHashMap_iteratorDeleteCallback(ZF_IN void *data)
{
    zfdelete(ZFCastStatic(_ZFP_ZFHashMapPrivate::MapType::iterator *, data));
}
static void *_ZFP_ZFHashMap_iteratorCopyCallback(ZF_IN void *data)
{
    return zfnew(_ZFP_ZFHashMapPrivate::MapType::iterator,
        *ZFCastStatic(_ZFP_ZFHashMapPrivate::MapType::iterator *, data));
}
ZFMETHOD_DEFINE_0(ZFHashMap, zfiterator, iterator)
{
    return zfiterator(zfnew(_ZFP_ZFHashMapPrivate::MapType::iterator, d->data.begin()),
        _ZFP_ZFHashMap_iteratorDeleteCallback,
        _ZFP_ZFHashMap_iteratorCopyCallback);
}

ZFMETHOD_DEFINE_1(ZFHashMap, zfbool, iteratorIsValid,
                  ZFMP_IN(const zfiterator &, it))
{
    _ZFP_ZFHashMapPrivate::MapType::iterator *data = it.data<_ZFP_ZFHashMapPrivate::MapType::iterator *>();
    return (data != zfnull && *data != d->data.end());
}
ZFMETHOD_DEFINE_2(ZFHashMap, zfbool, iteratorIsEqual,
                  ZFMP_IN(const zfiterator &, it0),
                  ZFMP_IN(const zfiterator &, it1))
{
    return zfiterator::iteratorIsEqual<_ZFP_ZFHashMapPrivate::MapType::iterator *>(it0, it1);
}

void ZFHashMap::iteratorValue(ZF_IN_OUT zfiterator &it,
                              ZF_IN ZFObject *value)
{
    _ZFP_ZFHashMapPrivate::MapType::iterator *data = it.data<_ZFP_ZFHashMapPrivate::MapType::iterator *>();
    if(data != zfnull && *data != d->data.end())
    {
        this->contentOnRemove((*data)->first, (*data)->second);
        ZFObject *tmp = (*data)->second;
        zfRetain(value);
        (*data)->second = value;
        this->contentOnAdd((*data)->first, value);
        this->contentOnChange();
        zfRelease(tmp);
    }
}
void ZFHashMap::iteratorRemove(ZF_IN_OUT zfiterator &it)
{
    _ZFP_ZFHashMapPrivate::MapType::iterator *data = it.data<_ZFP_ZFHashMapPrivate::MapType::iterator *>();
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
ZFMETHOD_DEFINE_1(ZFHashMap, zfiterator, iteratorForKey,
                  ZFMP_IN(ZFObject *, key))
{
    return zfiterator(zfnew(_ZFP_ZFHashMapPrivate::MapType::iterator, d->data.find(key)),
        _ZFP_ZFHashMap_iteratorDeleteCallback,
        _ZFP_ZFHashMap_iteratorCopyCallback);
}
ZFMETHOD_DEFINE_1(ZFHashMap, ZFObject *, iteratorKey,
                  ZFMP_IN(const zfiterator &, it))
{
    _ZFP_ZFHashMapPrivate::MapType::iterator *data = it.data<_ZFP_ZFHashMapPrivate::MapType::iterator *>();
    return ((data != zfnull && *data != d->data.end()) ? (*data)->first : zfnull);
}
ZFMETHOD_DEFINE_1(ZFHashMap, ZFObject *, iteratorValue,
                  ZFMP_IN(const zfiterator &, it))
{
    _ZFP_ZFHashMapPrivate::MapType::iterator *data = it.data<_ZFP_ZFHashMapPrivate::MapType::iterator *>();
    return ((data != zfnull && *data != d->data.end()) ? (*data)->second : zfnull);
}
ZFMETHOD_DEFINE_1(ZFHashMap, ZFKeyValuePair, iteratorPair,
                  ZFMP_IN(const zfiterator &, it))
{
    ZFKeyValuePair ret = ZFKeyValuePairZero();
    _ZFP_ZFHashMapPrivate::MapType::iterator *data = it.data<_ZFP_ZFHashMapPrivate::MapType::iterator *>();
    if(data != zfnull && *data != d->data.end())
    {
        ret.key = (*data)->first;
        ret.value = (*data)->second;
    }
    return ret;
}
ZFMETHOD_DEFINE_1(ZFHashMap, ZFObject *, iteratorNextKey,
                  ZFMP_IN_OUT(zfiterator &, it))
{
    _ZFP_ZFHashMapPrivate::MapType::iterator *data = it.data<_ZFP_ZFHashMapPrivate::MapType::iterator *>();
    if(data != zfnull && *data != d->data.end())
    {
        ZFObject *ret = (*data)->first;
        data->operator ++ ();
        return ret;
    }
    return zfnull;
}
ZFMETHOD_DEFINE_1(ZFHashMap, ZFObject *, iteratorNextValue,
                  ZFMP_IN_OUT(zfiterator &, it))
{
    _ZFP_ZFHashMapPrivate::MapType::iterator *data = it.data<_ZFP_ZFHashMapPrivate::MapType::iterator *>();
    if(data != zfnull && *data != d->data.end())
    {
        ZFObject *ret = (*data)->second;
        data->operator ++ ();
        return ret;
    }
    return zfnull;
}
ZFMETHOD_DEFINE_1(ZFHashMap, ZFKeyValuePair, iteratorNextPair,
                  ZFMP_IN_OUT(zfiterator &, it))
{
    ZFKeyValuePair ret = ZFKeyValuePairZero();
    _ZFP_ZFHashMapPrivate::MapType::iterator *data = it.data<_ZFP_ZFHashMapPrivate::MapType::iterator *>();
    if(data != zfnull && *data != d->data.end())
    {
        ret.key = (*data)->first;
        ret.value = (*data)->second;
        data->operator ++ ();
        return ret;
    }
    return ret;
}
ZFMETHOD_DEFINE_1(ZFHashMap, ZFObject *, iteratorPrevKey,
                  ZFMP_IN_OUT(zfiterator &, it))
{
    zfCoreCriticalNotSupported();
    return zfnull;
}
ZFMETHOD_DEFINE_1(ZFHashMap, ZFObject *, iteratorPrevValue,
                  ZFMP_IN_OUT(zfiterator &, it))
{
    zfCoreCriticalNotSupported();
    return zfnull;
}
ZFMETHOD_DEFINE_1(ZFHashMap, ZFKeyValuePair, iteratorPrevPair,
                  ZFMP_IN_OUT(zfiterator &, it))
{
    ZFKeyValuePair ret = ZFKeyValuePairZero();
    zfCoreCriticalNotSupported();
    return ret;
}
void ZFHashMap::iteratorAddKeyValue(ZF_IN ZFObject *key,
                                    ZF_IN ZFObject *value)
{
    this->set(key, value);
}
void ZFHashMap::iteratorAddKeyValue(ZF_IN ZFObject *key,
                                    ZF_IN ZFObject *value,
                                    ZF_IN_OUT zfiterator &it)
{
    this->set(key, value);
}

ZF_NAMESPACE_GLOBAL_END

