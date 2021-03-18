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
    d = zfAlloc(ZFHashMapEditable);
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
void ZFHashSet::add(ZF_IN ZFObject *obj)
{
    zfCoreAssertWithMessage(obj != zfnull, "insert null object");
    if(!d->isContain(obj))
    {
        d->set(obj, zfnullObject());
        this->contentOnAdd(obj);
        this->contentOnChange();
    }
}
void ZFHashSet::addFrom(ZF_IN ZFContainer *another)
{
    if(another == this || another == zfnull)
    {
        return ;
    }

    for(zfiterator it = another->iterator(); another->iteratorIsValid(it);)
    {
        ZFObject *obj = another->iteratorNextValue(it);
        if(!d->isContain(obj))
        {
            d->set(obj, zfnullObject());
            this->contentOnAdd(obj);
        }
    }

    this->contentOnChange();
}

void ZFHashSet::removeElement(ZF_IN ZFObject *obj)
{
    ZFKeyValuePairHolder tmp = d->removeAndGetPair(obj);
    if(tmp.key != zfnull)
    {
        this->contentOnRemove(tmp.key.toObject());
        this->contentOnChange();
    }
}
void ZFHashSet::removeAll(void)
{
    if(!d->isEmpty())
    {
        for(zfiterator it = d->iterator(); d->iteratorIsValid(it); )
        {
            this->contentOnRemove(d->iteratorNextKey(it));
        }
        d->removeAll();

        this->contentOnChange();
    }
}

// ============================================================
ZFMETHOD_DEFINE_0(ZFHashSet, zfiterator, iterator)
{
    return d->iterator();
}

ZFMETHOD_DEFINE_1(ZFHashSet, zfiterator, iteratorFind,
                  ZFMP_IN(ZFObject *, value))
{
    return d->iteratorForKey(value);
}

ZFMETHOD_DEFINE_1(ZFHashSet, zfbool, iteratorIsValid,
                  ZFMP_IN(const zfiterator &, it))
{
    return d->iteratorIsValid(it);
}
ZFMETHOD_DEFINE_2(ZFHashSet, zfbool, iteratorIsEqual,
                  ZFMP_IN(const zfiterator &, it0),
                  ZFMP_IN(const zfiterator &, it1))
{
    return d->iteratorIsEqual(it0, it1);
}

ZFMETHOD_DEFINE_1(ZFHashSet, ZFObject *, iteratorValue,
                  ZFMP_IN(const zfiterator &, it))
{
    return d->iteratorKey(it);
}

ZFMETHOD_DEFINE_1(ZFHashSet, ZFObject *, iteratorNextValue,
                  ZFMP_IN_OUT(zfiterator &, it))
{
    return d->iteratorNextKey(it);
}
ZFMETHOD_DEFINE_1(ZFHashSet, ZFObject *, iteratorPrevValue,
                  ZFMP_IN_OUT(zfiterator &, it))
{
    return d->iteratorPrevKey(it);
}

void ZFHashSet::iteratorValue(ZF_IN_OUT zfiterator &it,
                              ZF_IN ZFObject *value)
{
    this->add(value);
}
void ZFHashSet::iteratorRemove(ZF_IN_OUT zfiterator &it)
{
    ZFKeyValuePair tmp = d->iteratorPair(it);
    if(tmp.key != zfnull)
    {
        this->contentOnRemove(tmp.key);
        d->iteratorRemove(it);
        this->contentOnChange();
    }
}
void ZFHashSet::iteratorAdd(ZF_IN ZFObject *value)
{
    this->add(value);
}
void ZFHashSet::iteratorAdd(ZF_IN ZFObject *value,
                            ZF_IN_OUT zfiterator &it)
{
    this->add(value);
}

ZF_NAMESPACE_GLOBAL_END

