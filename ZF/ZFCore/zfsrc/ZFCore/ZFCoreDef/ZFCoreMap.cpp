#include "ZFCoreMap.h"

#include "ZFCore/ZFSTLWrapper/zfstl_string.h"

#ifndef ZF_ENV_ZFCOREMAP_USE_HASHMAP
    #define ZF_ENV_ZFCOREMAP_USE_HASHMAP 1
#endif

#if ZF_ENV_ZFCOREMAP_USE_HASHMAP
    #include "ZFCore/ZFSTLWrapper/zfstl_hashmap.h"
#else
    #include "ZFCore/ZFSTLWrapper/zfstl_map.h"
#endif

ZF_NAMESPACE_GLOBAL_BEGIN

const ZFCoreMapPair ZFCoreMapPairZero = {0, 0};

// ============================================================
zfclassNotPOD _ZFP_ZFCoreMapPrivate
{
public:
#if ZF_ENV_ZFCOREMAP_USE_HASHMAP
    typedef zfstlhashmap<zfstlstringZ, ZFCorePointerBase *> MapType;
#else
    typedef zfstlmap<zfstlstringZ, ZFCorePointerBase *> MapType;
#endif

public:
    zfuint refCount;
    _ZFP_ZFCoreMapPrivate::MapType m;

public:
    void removeAll(void)
    {
        if(!this->m.empty())
        {
            _ZFP_ZFCoreMapPrivate::MapType tmp;
            tmp.swap(this->m);
            for(_ZFP_ZFCoreMapPrivate::MapType::iterator it = tmp.begin(); it != tmp.end(); ++it)
            {
                zfdelete(it->second);
            }
        }
    }

public:
    _ZFP_ZFCoreMapPrivate(void)
    : refCount(1)
    {
    }
    ~_ZFP_ZFCoreMapPrivate(void)
    {
        this->removeAll();
    }
};

// ============================================================
ZFCoreMap::ZFCoreMap(void)
: d(zfnew(_ZFP_ZFCoreMapPrivate))
{
}
ZFCoreMap::ZFCoreMap(ZF_IN const ZFCoreMap &ref)
: d(ref.d)
{
    ++(d->refCount);
}
ZFCoreMap &ZFCoreMap::operator = (ZF_IN const ZFCoreMap &ref)
{
    _ZFP_ZFCoreMapPrivate *dTmp = d;
    d = ref.d;
    ++(ref.d->refCount);
    --(dTmp->refCount);
    if(dTmp->refCount == 0)
    {
        zfdelete(dTmp);
    }
    return *this;
}
zfbool ZFCoreMap::operator == (ZF_IN const ZFCoreMap &ref) const
{
    return (d == ref.d);
}
ZFCoreMap::~ZFCoreMap(void)
{
    --(d->refCount);
    if(d->refCount == 0)
    {
        zfdelete(d);
    }
}

void ZFCoreMap::objectInfoT(ZF_IN_OUT zfstring &ret) const
{
    this->objectInfoOfContentT(ret, 5);
}
void ZFCoreMap::objectInfoOfContentT(ZF_IN_OUT zfstring &ret,
                                     ZF_IN_OPT zfindex maxCount /* = zfindexMax() */,
                                     ZF_IN_OPT const ZFTokenForKeyValueContainer &token /* = ZFTokenForKeyValueContainerDefault() */) const
{
    zfindex count = 0;
    ret += token.tokenLeft;
    if(!this->isEmpty())
    {
        for(zfiterator it = this->iterator();
            this->iteratorValid(it) && count < maxCount;
            this->iteratorNext(it), ++count)
        {
            if(count > 0)
            {
                ret += token.tokenSeparator;
            }

            ret += token.tokenPairLeft;
            {
                ret += token.tokenKeyLeft;
                ret += this->iteratorPair(it).key;
                ret += token.tokenKeyRight;
            }
            ret += token.tokenPairSeparator;
            {
                ret += token.tokenValueLeft;
                ZFCorePointerBase *value = this->iteratorValue(it);
                if(value == zfnull)
                {
                    ret += ZFTOKEN_zfnull;
                }
                else
                {
                    value->objectInfoOfContentT(ret);
                }
                ret += token.tokenValueRight;
            }
            ret += token.tokenPairRight;
        }
    }
    if(count < this->count())
    {
        if(count > 0)
        {
            ret += token.tokenSeparator;
        }
        ret += token.tokenEtc;
    }
    ret += token.tokenRight;
}

void ZFCoreMap::copyFrom(ZF_IN const ZFCoreMap &ref)
{
    if(d != ref.d)
    {
        this->removeAll();
        this->addFrom(ref);
    }
}

zfindex ZFCoreMap::objectRetainCount(void) const
{
    return d->refCount;
}

zfindex ZFCoreMap::count(void) const
{
    return d->m.size();
}

zfbool ZFCoreMap::isEmpty(void) const
{
    return d->m.empty();
}

zfbool ZFCoreMap::isContain(ZF_IN const zfchar *key) const
{
    if(key == zfnull)
    {
        key = "";
    }
    return (d->m.find(key) != d->m.end());
}

void ZFCoreMap::addFrom(ZF_IN const ZFCoreMap &ref)
{
    if(d != ref.d && !ref.isEmpty())
    {
        for(zfiterator it = ref.iterator(); this->iteratorValid(it); this->iteratorNext(it))
        {
            this->set(this->iteratorPair(it).key, *(this->iteratorValue(it)));
        }
    }
}

void ZFCoreMap::set(ZF_IN const zfchar *key,
                    ZF_IN const ZFCorePointerBase &value)
{
    if(key == zfnull)
    {
        key = "";
    }

    _ZFP_ZFCoreMapPrivate::MapType::iterator it = d->m.find(key);
    if(it == d->m.end())
    {
        d->m[key] = value.refNew();
    }
    else
    {
        ZFCorePointerBase *toDelete = it->second;
        it->second = value.refNew();
        toDelete->refDelete();
    }
}
ZFCorePointerBase *ZFCoreMap::get(ZF_IN const zfchar *key) const
{
    if(key == zfnull)
    {
        key = "";
    }
    _ZFP_ZFCoreMapPrivate::MapType::iterator it = d->m.find(key);
    if(it == d->m.end())
    {
        return zfnull;
    }
    return it->second;
}

void ZFCoreMap::allKeyT(ZF_IN_OUT ZFCoreArray<const zfchar *> &ret) const
{
    ret.capacity(ret.count() + this->count());
    for(_ZFP_ZFCoreMapPrivate::MapType::const_iterator it = d->m.begin();
        it != d->m.end();
        ++it)
    {
        ret.add(it->first.c_str());
    }
}
void ZFCoreMap::allValueT(ZF_IN_OUT ZFCoreArray<ZFCorePointerBase *> &ret) const
{
    ret.capacity(ret.count() + this->count());
    for(_ZFP_ZFCoreMapPrivate::MapType::const_iterator it = d->m.begin();
        it != d->m.end();
        ++it)
    {
        ret.add(it->second);
    }
}
void ZFCoreMap::allPairT(ZF_IN_OUT ZFCoreArray<ZFCoreMapPair> &ret) const
{
    ret.capacity(ret.count() + this->count());
    ZFCoreMapPair tmp;
    for(_ZFP_ZFCoreMapPrivate::MapType::const_iterator it = d->m.begin();
        it != d->m.end();
        ++it)
    {
        tmp.key = it->first.c_str();
        tmp.value = it->second;
        ret.add(tmp);
    }
}

void ZFCoreMap::remove(ZF_IN const zfchar *key)
{
    if(key == zfnull)
    {
        key = "";
    }
    _ZFP_ZFCoreMapPrivate::MapType::iterator it = d->m.find(key);
    if(it != d->m.end())
    {
        ZFCorePointerBase *savedValue = it->second;
        d->m.erase(it);
        savedValue->refDelete();
    }
}

void ZFCoreMap::removeAll(void)
{
    d->removeAll();
}

// ============================================================
// iterator
static void _ZFP_ZFCoreMap_iteratorDeleteCallback(void *data)
{
    zfdelete(ZFCastStatic(_ZFP_ZFCoreMapPrivate::MapType::iterator *, data));
}
static void *_ZFP_ZFCoreMap_iteratorCopyCallback(void *data)
{
    return zfnew(_ZFP_ZFCoreMapPrivate::MapType::iterator,
        *ZFCastStatic(_ZFP_ZFCoreMapPrivate::MapType::iterator *, data));
}
zfiterator ZFCoreMap::iterator(void) const
{
    return zfiterator(
        zfnew(_ZFP_ZFCoreMapPrivate::MapType::iterator, d->m.begin()),
        _ZFP_ZFCoreMap_iteratorDeleteCallback,
        _ZFP_ZFCoreMap_iteratorCopyCallback);
}

zfiterator ZFCoreMap::iteratorFind(ZF_IN const zfchar *key) const
{
    if(key == zfnull)
    {
        key = "";
    }
    return zfiterator(
        zfnew(_ZFP_ZFCoreMapPrivate::MapType::iterator, d->m.find(key)),
        _ZFP_ZFCoreMap_iteratorDeleteCallback,
        _ZFP_ZFCoreMap_iteratorCopyCallback);
}

zfbool ZFCoreMap::iteratorValid(ZF_IN const zfiterator &it) const
{
    _ZFP_ZFCoreMapPrivate::MapType::iterator *data = it.data<_ZFP_ZFCoreMapPrivate::MapType::iterator *>();
    return (data != zfnull && *data != d->m.end());
}
zfbool ZFCoreMap::iteratorEqual(ZF_IN const zfiterator &it0,
                                  ZF_IN const zfiterator &it1) const
{
    return zfiterator::iteratorEqual<_ZFP_ZFCoreMapPrivate::MapType::iterator *>(it0, it1);
}

void ZFCoreMap::iteratorNext(ZF_IN_OUT zfiterator &it) const
{
    _ZFP_ZFCoreMapPrivate::MapType::iterator *data = it.data<_ZFP_ZFCoreMapPrivate::MapType::iterator *>();
    if(data)
    {
        data->operator ++ ();
    }
}
void ZFCoreMap::iteratorPrev(ZF_IN_OUT zfiterator &it) const
{
#if ZF_ENV_ZFCOREMAP_USE_HASHMAP
    zfCoreCriticalNotSupported();
#else
    _ZFP_ZFCoreMapPrivate::MapType::iterator *data = it.data<_ZFP_ZFCoreMapPrivate::MapType::iterator *>();
    if(data)
    {
        data->operator -- ();
    }
#endif
}

ZFCorePointerBase *ZFCoreMap::iteratorValue(ZF_IN const zfiterator &it) const
{
    _ZFP_ZFCoreMapPrivate::MapType::iterator *data = it.data<_ZFP_ZFCoreMapPrivate::MapType::iterator *>();
    return ((data != zfnull) ? (*data)->second : zfnull);
}
ZFCoreMapPair ZFCoreMap::iteratorPair(ZF_IN const zfiterator &it) const
{
    _ZFP_ZFCoreMapPrivate::MapType::iterator *data = it.data<_ZFP_ZFCoreMapPrivate::MapType::iterator *>();
    ZFCoreMapPair ret = ZFCoreMapPairZero;
    if(data)
    {
        ret.key = (*data)->first.c_str();
        ret.value = (*data)->second;
    }
    return ret;
}

void ZFCoreMap::iteratorValue(ZF_IN_OUT zfiterator &it,
                              ZF_IN const ZFCorePointerBase &newValue)
{
    _ZFP_ZFCoreMapPrivate::MapType::iterator *data = it.data<_ZFP_ZFCoreMapPrivate::MapType::iterator *>();
    if(data)
    {
        ZFCorePointerBase *toDelete = (*data)->second;
        (*data)->second = newValue.refNew();
        toDelete->refDelete();
    }
}
void ZFCoreMap::iteratorRemove(ZF_IN_OUT zfiterator &it)
{
    _ZFP_ZFCoreMapPrivate::MapType::iterator *data = it.data<_ZFP_ZFCoreMapPrivate::MapType::iterator *>();
    if(data != zfnull)
    {
        ZFCorePointerBase *savedValue = (*data)->second;
        d->m.erase((*data)++);
        savedValue->refDelete();
    }
}

void ZFCoreMap::iteratorAdd(ZF_IN const zfchar *key,
                            ZF_IN const ZFCorePointerBase &value)
{
    this->set(key, value);
}

ZF_NAMESPACE_GLOBAL_END

