#include "ZFHashSet.h"
#include "ZFHashMap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFHashSet
ZFOBJECT_REGISTER(ZFHashSet)

ZFOBJECT_ON_INIT_DEFINE_1(ZFHashSet,
                          ZFMP_IN(ZFContainer *, another))
{
    this->objectOnInit();
    zfself::addFrom(another);
}
void ZFHashSet::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfAlloc(ZFHashMap);
}
void ZFHashSet::objectOnDealloc(void)
{
    zfRelease(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}

ZFMETHOD_DEFINE_0(ZFHashSet, zfindex, count)
{
    return d->count();
}
ZFMETHOD_DEFINE_0(ZFHashSet, zfbool, isEmpty)
{
    return d->isEmpty();
}
ZFMETHOD_DEFINE_1(ZFHashSet, zfbool, isContain,
                  ZFMP_IN(ZFObject *, obj))
{
    return d->isContain(obj);
}

ZFMETHOD_DEFINE_1(ZFHashSet, void, add,
                  ZFMP_IN(ZFObject *, obj))
{
    zfCoreAssertWithMessage(obj != zfnull, "insert null object");
    if(!d->isContain(obj))
    {
        d->set(obj, zfnullObject());
        this->contentOnAdd(obj);
        this->contentOnChange();
    }
}
ZFMETHOD_DEFINE_1(ZFHashSet, void, addFrom,
                  ZFMP_IN(ZFContainer *, another))
{
    if(another == this || another == zfnull)
    {
        return ;
    }

    zfbool changed = zffalse;
    for(zfiterator it = another->iterator(); another->iteratorValid(it); another->iteratorNext(it))
    {
        ZFObject *obj = another->iteratorValue(it);
        if(!d->isContain(obj))
        {
            changed = zftrue;
            d->set(obj, zfnullObject());
            this->contentOnAdd(obj);
        }
    }

    if(changed)
    {
        this->contentOnChange();
    }
}

ZFMETHOD_DEFINE_1(ZFHashSet, void, removeElement,
                  ZFMP_IN(ZFObject *, obj))
{
    ZFKeyValuePairHolder tmp = d->removeAndGetPair(obj);
    if(tmp.key != zfnull)
    {
        this->contentOnRemove(tmp.key.toObject());
        this->contentOnChange();
    }
}
ZFMETHOD_DEFINE_0(ZFHashSet, void, removeAll)
{
    if(!d->isEmpty())
    {
        ZFCoreArray<zfautoObject> tmp;
        tmp.capacity(d->count());
        for(zfiterator it = d->iterator(); d->iteratorValid(it); d->iteratorNext(it))
        {
            tmp.add(d->iteratorValue(it));
        }
        d->removeAll();

        for(zfindex i = 0; i < tmp.count(); ++i)
        {
            this->contentOnRemove(tmp[i]);
        }
        this->contentOnChange();
    }
}

// ============================================================
ZFMETHOD_DEFINE_0(ZFHashSet, zfiterator, iterator)
{
    return d->iterator();
}

ZFMETHOD_DEFINE_1(ZFHashSet, zfiterator, iteratorFind,
                  ZFMP_IN(ZFObject *, key))
{
    return d->iteratorFind(key);
}

ZFMETHOD_DEFINE_1(ZFHashSet, zfbool, iteratorValid,
                  ZFMP_IN(const zfiterator &, it))
{
    return d->iteratorValid(it);
}
ZFMETHOD_DEFINE_2(ZFHashSet, zfbool, iteratorEqual,
                  ZFMP_IN(const zfiterator &, it0),
                  ZFMP_IN(const zfiterator &, it1))
{
    return d->iteratorEqual(it0, it1);
}

ZFMETHOD_DEFINE_1(ZFHashSet, void, iteratorNext,
                  ZFMP_IN_OUT(zfiterator &, it))
{
    d->iteratorNext(it);
}
ZFMETHOD_DEFINE_1(ZFHashSet, void, iteratorPrev,
                  ZFMP_IN_OUT(zfiterator &, it))
{
    d->iteratorPrev(it);
}

ZFMETHOD_DEFINE_1(ZFHashSet, ZFObject *, iteratorValue,
                  ZFMP_IN(const zfiterator &, it))
{
    return d->iteratorPair(it).key;
}

ZFMETHOD_DEFINE_2(ZFHashSet, void, iteratorValue,
                  ZFMP_IN_OUT(zfiterator &, it),
                  ZFMP_IN(ZFObject *, value))
{
    zfRetain(value);
    this->iteratorRemove(it);
    this->add(value);
    zfRelease(value);
}
ZFMETHOD_DEFINE_1(ZFHashSet, void, iteratorRemove,
                  ZFMP_IN_OUT(zfiterator &, it))
{
    ZFKeyValuePair tmp = d->iteratorPair(it);
    if(tmp.key != zfnull)
    {
        d->iteratorRemove(it);
        this->contentOnRemove(tmp.key);
        this->contentOnChange();
    }
}
ZFMETHOD_DEFINE_1(ZFHashSet, void, iteratorAdd,
                  ZFMP_IN(ZFObject *, value))
{
    this->add(value);
}
ZFMETHOD_DEFINE_2(ZFHashSet, void, iteratorAdd,
                  ZFMP_IN(ZFObject *, value),
                  ZFMP_IN_OUT(zfiterator &, it))
{
    this->add(value);
}

ZF_NAMESPACE_GLOBAL_END

