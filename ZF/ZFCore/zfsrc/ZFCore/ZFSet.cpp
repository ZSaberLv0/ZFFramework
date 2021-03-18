#include "ZFSet.h"
#include "ZFMap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFSet
ZFOBJECT_REGISTER(ZFSet)

void ZFSet::objectOnInit(ZF_IN ZFContainer *another)
{
    this->objectOnInit();
    zfself::addFrom(another);
}
void ZFSet::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfAlloc(ZFMapEditable);
}
void ZFSet::objectOnDealloc(void)
{
    zfRelease(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}

ZFMETHOD_DEFINE_0(ZFSet, zfindex, count)
{
    return d->count();
}
ZFMETHOD_DEFINE_0(ZFSet, zfbool, isEmpty)
{
    return d->isEmpty();
}
ZFMETHOD_DEFINE_1(ZFSet, zfbool, isContain,
                  ZFMP_IN(ZFObject *, obj))
{
    return d->isContain(obj);
}

void ZFSet::add(ZF_IN ZFObject *obj)
{
    zfCoreAssertWithMessage(obj != zfnull, "insert null object");
    if(!d->isContain(obj))
    {
        d->set(obj, zfnullObject());
        this->contentOnAdd(obj);
        this->contentOnChange();
    }
}
void ZFSet::addFrom(ZF_IN ZFContainer *another)
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

void ZFSet::removeElement(ZF_IN ZFObject *obj)
{
    ZFKeyValuePairHolder tmp = d->removeAndGetPair(obj);
    if(tmp.key != zfnull)
    {
        this->contentOnRemove(tmp.key.toObject());
        this->contentOnChange();
    }
}
void ZFSet::removeAll(void)
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
ZFMETHOD_DEFINE_0(ZFSet, zfiterator, iterator)
{
    return d->iterator();
}

ZFMETHOD_DEFINE_1(ZFSet, zfiterator, iteratorFind,
                  ZFMP_IN(ZFObject *, value))
{
    return d->iteratorForKey(value);
}

ZFMETHOD_DEFINE_1(ZFSet, zfbool, iteratorIsValid,
                  ZFMP_IN(const zfiterator &, it))
{
    return d->iteratorIsValid(it);
}
ZFMETHOD_DEFINE_2(ZFSet, zfbool, iteratorIsEqual,
                  ZFMP_IN(const zfiterator &, it0),
                  ZFMP_IN(const zfiterator &, it1))
{
    return d->iteratorIsEqual(it0, it1);
}

ZFMETHOD_DEFINE_1(ZFSet, ZFObject *, iteratorValue,
                  ZFMP_IN(const zfiterator &, it))
{
    return d->iteratorKey(it);
}

ZFMETHOD_DEFINE_1(ZFSet, ZFObject *, iteratorNextValue,
                  ZFMP_IN_OUT(zfiterator &, it))
{
    return d->iteratorNextKey(it);
}
ZFMETHOD_DEFINE_1(ZFSet, ZFObject *, iteratorPrevValue,
                  ZFMP_IN_OUT(zfiterator &, it))
{
    return d->iteratorPrevKey(it);
}

void ZFSet::iteratorValue(ZF_IN_OUT zfiterator &it,
                          ZF_IN ZFObject *value)
{
    this->add(value);
}
void ZFSet::iteratorRemove(ZF_IN_OUT zfiterator &it)
{
    ZFKeyValuePair tmp = d->iteratorPair(it);
    if(tmp.key != zfnull)
    {
        this->contentOnRemove(tmp.key);
        d->iteratorRemove(it);
        this->contentOnChange();
    }
}
void ZFSet::iteratorAdd(ZF_IN ZFObject *value)
{
    this->add(value);
}
void ZFSet::iteratorAdd(ZF_IN ZFObject *value,
                        ZF_IN_OUT zfiterator &it)
{
    this->add(value);
}

ZF_NAMESPACE_GLOBAL_END

