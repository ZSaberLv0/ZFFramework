#include "ZFArray.h"
#include "ZFSTLWrapper/zfstl_deque.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// _ZFP_ZFArrayPrivate
zfclassNotPOD _ZFP_ZFArrayPrivate
{
public:
    zfstldeque<ZFObject *> data;
};

// ============================================================
// ZFArray
ZFOBJECT_REGISTER(ZFArray)

ZFOBJECT_ON_INIT_DEFINE_1(ZFArray,
                          ZFMP_IN(ZFContainer *, another))
{
    this->objectOnInit();
    zfself::addFrom(another);
}
void ZFArray::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFArrayPrivate);
}
void ZFArray::objectOnDealloc(void)
{
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}

// ============================================================
ZFMETHOD_DEFINE_0(ZFArray, zfindex, count)
{
    return (zfindex)(d->data.size());
}
ZFMETHOD_DEFINE_0(ZFArray, zfbool, isEmpty)
{
    return d->data.empty();
}
ZFMETHOD_DEFINE_2(ZFArray, zfbool, isContain,
                  ZFMP_IN(ZFObject *, obj),
                  ZFMP_IN_OPT(ZFComparer<ZFObject *>::Comparer, comparer, ZFComparerCheckEqual))
{
    return (this->find(obj, comparer) != zfindexMax());
}
ZFMETHOD_DEFINE_1(ZFArray, ZFObject *, get,
                  ZFMP_IN(zfindex, index))
{
    if(index >= d->data.size())
    {
        zfCoreCriticalIndexOutOfRange(index, d->data.size());
        return zfnull;
    }
    return d->data[index];
}
ZFMETHOD_DEFINE_0(ZFArray, ZFObject *, getFirst)
{
    if(d->data.empty())
    {
        return zfnull;
    }
    return d->data[0];
}
ZFMETHOD_DEFINE_0(ZFArray, ZFObject *, getLast)
{
    if(d->data.empty())
    {
        return zfnull;
    }
    return d->data[d->data.size() - 1];
}
ZFMETHOD_DEFINE_2(ZFArray, zfindex, find,
                  ZFMP_IN(ZFObject *, obj),
                  ZFMP_IN_OPT(ZFComparer<ZFObject *>::Comparer, comparer, ZFComparerCheckEqual))
{
    if(comparer == zfnull)
    {
        return zfindexMax();
    }
    for(zfstlsize i = 0; i < d->data.size(); ++i)
    {
        if(comparer(d->data[i], obj) == ZFCompareTheSame)
        {
            return (zfindex)i;
        }
    }
    return zfindexMax();
}
ZFMETHOD_DEFINE_2(ZFArray, zfindex, findReversely,
                  ZFMP_IN(ZFObject *, obj),
                  ZFMP_IN_OPT(ZFComparer<ZFObject *>::Comparer, comparer, ZFComparerCheckEqual))
{
    if(comparer == zfnull)
    {
        return zfindexMax();
    }
    for(zfstlsize i = d->data.size() - 1; i != (zfstlsize)-1; --i)
    {
        if(comparer(d->data[i], obj) == ZFCompareTheSame)
        {
            return (zfindex)i;
        }
    }
    return zfindexMax();
}
ZFMETHOD_DEFINE_2(ZFArray, void, add,
                  ZFMP_IN(zfindex, indexAddTo),
                  ZFMP_IN(ZFObject *, obj))
{
    zfCoreAssertWithMessage(obj != zfnull, "insert null object");
    zfRetain(obj);
    d->data.insert(d->data.begin() + indexAddTo, obj);

    this->contentOnAdd(obj);
    this->contentOnChange();
}
ZFMETHOD_DEFINE_1(ZFArray, void, add,
                  ZFMP_IN(ZFObject *, obj))
{
    zfCoreAssertWithMessage(obj != zfnull, "insert null object");
    zfRetain(obj);
    d->data.push_back(obj);

    this->contentOnAdd(obj);
    this->contentOnChange();
}
ZFMETHOD_DEFINE_1(ZFArray, void, addFrom,
                  ZFMP_IN(ZFContainer *, another))
{
    if(another == this || another == zfnull)
    {
        return ;
    }
    ZFObject *obj = zfnull;
    for(zfiterator it = another->iterator(); another->iteratorValid(it); another->iteratorNext(it))
    {
        obj = another->iteratorValue(it);
        zfRetain(obj);
        d->data.push_back(obj);
        this->contentOnAdd(obj);
    }
    if(obj != zfnull)
    {
        this->contentOnChange();
    }
}

ZFMETHOD_DEFINE_2(ZFArray, void, set,
                  ZFMP_IN(zfindex, index),
                  ZFMP_IN(ZFObject *, obj))
{
    zfCoreAssertWithMessage(obj != zfnull, "set null object");
    zfRetain(obj);

    ZFObject *old = d->data[index];
    d->data[index] = obj;

    this->contentOnRemove(old);
    this->contentOnAdd(obj);
    this->contentOnChange();

    zfRelease(old);
}

ZFMETHOD_DEFINE_1(ZFArray, zfbool, removeElement,
                  ZFMP_IN(ZFObject *, obj))
{
    if(obj)
    {
        for(zfstldeque<ZFObject *>::iterator it = d->data.begin(); it != d->data.end(); )
        {
            if((*it)->objectCompare(obj) == ZFCompareTheSame)
            {
                ZFObject *toRelease = *it;
                it = d->data.erase(it);
                zfRelease(toRelease);

                this->contentOnRemove(toRelease);
                this->contentOnChange();
                return zftrue;
            }
            else
            {
                ++it;
            }
        }
    }
    return zffalse;
}
ZFMETHOD_DEFINE_2(ZFArray, zfbool, removeElement,
                  ZFMP_IN(ZFObject *, obj),
                  ZFMP_IN(ZFComparer<ZFObject *>::Comparer, comparer))
{
    if(obj && comparer)
    {
        for(zfstldeque<ZFObject *>::iterator it = d->data.begin(); it != d->data.end(); )
        {
            if(comparer(*it, obj) == ZFCompareTheSame)
            {
                ZFObject *toRelease = *it;
                it = d->data.erase(it);
                zfRelease(toRelease);

                this->contentOnRemove(toRelease);
                this->contentOnChange();
                return zftrue;
            }
            else
            {
                ++it;
            }
        }
    }
    return zffalse;
}
ZFMETHOD_DEFINE_1(ZFArray, zfbool, removeElementRevsersely,
                  ZFMP_IN(ZFObject *, obj))
{
    if(obj)
    {
        for(zfstlsize i = d->data.size() - 1; i != (zfstlsize)-1; --i)
        {
            if(d->data[i]->objectCompare(obj) == ZFCompareTheSame)
            {
                ZFObject *toRelease = d->data[i];
                d->data.erase(d->data.begin() + i);
                zfRelease(toRelease);

                this->contentOnRemove(toRelease);
                this->contentOnChange();
                return zftrue;
            }
        }
    }
    return zffalse;
}
ZFMETHOD_DEFINE_2(ZFArray, zfbool, removeElementRevsersely,
                  ZFMP_IN(ZFObject *, obj),
                  ZFMP_IN(ZFComparer<ZFObject *>::Comparer, comparer))
{
    if(obj && comparer)
    {
        for(zfstlsize i = d->data.size() - 1; i != (zfstlsize)-1; --i)
        {
            if(comparer(d->data[i], obj) == ZFCompareTheSame)
            {
                ZFObject *toRelease = d->data[i];
                d->data.erase(d->data.begin() + i);
                zfRelease(toRelease);

                this->contentOnRemove(toRelease);
                this->contentOnChange();
                return zftrue;
            }
        }
    }
    return zffalse;
}
ZFMETHOD_DEFINE_1(ZFArray, zfindex, removeElementAll,
                  ZFMP_IN(ZFObject *, obj))
{
    zfindex removedCount = 0;
    if(obj)
    {
        for(zfstldeque<ZFObject *>::iterator it = d->data.begin(); it != d->data.end(); )
        {
            if((*it)->objectCompare(obj) == ZFCompareTheSame)
            {
                ++removedCount;
                ZFObject *toRelease = *it;
                it = d->data.erase(it);
                zfRelease(toRelease);

                this->contentOnRemove(toRelease);
            }
            else
            {
                ++it;
            }
        }
    }
    if(removedCount > 0)
    {
        this->contentOnChange();
    }
    return removedCount;
}
ZFMETHOD_DEFINE_2(ZFArray, zfindex, removeElementAll,
                  ZFMP_IN(ZFObject *, obj),
                  ZFMP_IN(ZFComparer<ZFObject *>::Comparer, comparer))
{
    zfindex removedCount = 0;
    if(obj && comparer)
    {
        for(zfstldeque<ZFObject *>::iterator it = d->data.begin(); it != d->data.end(); )
        {
            if(comparer(*it, obj) == ZFCompareTheSame)
            {
                ++removedCount;
                ZFObject *toRelease = *it;
                it = d->data.erase(it);
                zfRelease(toRelease);

                this->contentOnRemove(toRelease);
            }
            else
            {
                ++it;
            }
        }
    }
    if(removedCount > 0)
    {
        this->contentOnChange();
    }
    return removedCount;
}

ZFMETHOD_DEFINE_2(ZFArray, void, remove,
                  ZFMP_IN(zfindex, index),
                  ZFMP_IN_OPT(zfindex, count, 1))
{
    if(count == 1)
    {
        ZFObject *tmp = d->data[index];
        d->data.erase(d->data.begin() + index);
        zfRelease(tmp);

        this->contentOnRemove(tmp);
        this->contentOnChange();
    }
    else if(count > 1)
    {
        if(count > this->count() - index)
        {
            count = this->count() - index;
        }
        zfstldeque<ZFObject *> tmp(
            d->data.begin() + index,
            d->data.begin() + (index + count));
        d->data.erase(d->data.begin() + index, d->data.begin() + (index + count));
        for(zfstldeque<ZFObject *>::iterator it = tmp.begin(); it != tmp.end(); ++it)
        {
            this->contentOnRemove(*it);
            zfRelease(*it);
        }

        if(!tmp.empty())
        {
            this->contentOnChange();
        }
    }
}
ZFMETHOD_DEFINE_0(ZFArray, void, removeFirst)
{
    if(!d->data.empty())
    {
        ZFObject *tmp = d->data[0];
        d->data.pop_front();
        zfRelease(tmp);

        this->contentOnRemove(tmp);
        this->contentOnChange();
    }
}
ZFMETHOD_DEFINE_0(ZFArray, void, removeLast)
{
    if(!d->data.empty())
    {
        ZFObject *tmp = d->data[d->data.size() - 1];
        d->data.pop_back();
        zfRelease(tmp);

        this->contentOnRemove(tmp);
        this->contentOnChange();
    }
}
ZFMETHOD_DEFINE_0(ZFArray, void, removeAll)
{
    if(!d->data.empty())
    {
        zfstldeque<ZFObject *> tmp;
        tmp.swap(d->data);

        this->contentOnChange();

        for(zfstldeque<ZFObject *>::iterator it = tmp.begin(); it != tmp.end(); ++it)
        {
            this->contentOnRemove(*it);
            zfRelease(*it);
        }
    }
}

ZFMETHOD_DEFINE_2(ZFArray, void, move,
                  ZFMP_IN(zfindex, fromIndex),
                  ZFMP_IN(zfindex, toIndexOrIndexMax))
{
    if(fromIndex >= (zfindex)d->data.size())
    {
        zfCoreCriticalIndexOutOfRange(fromIndex, (zfindex)d->data.size());
        return ;
    }
    if(toIndexOrIndexMax == zfindexMax())
    {
        toIndexOrIndexMax = (zfindex)(d->data.size() - 1);
    }
    if(toIndexOrIndexMax >= (zfindex)d->data.size())
    {
        zfCoreCriticalIndexOutOfRange(toIndexOrIndexMax, (zfindex)d->data.size());
        return ;
    }
    if(fromIndex == toIndexOrIndexMax)
    {
        return ;
    }
    ZFObject *t = d->data[fromIndex];
    if(fromIndex < toIndexOrIndexMax)
    {
        for(zfindex i = fromIndex; i < toIndexOrIndexMax; ++i)
        {
            d->data[i] = d->data[i + 1];
        }
    }
    else
    {
        for(zfindex i = fromIndex; i > toIndexOrIndexMax; --i)
        {
            d->data[i] = d->data[i - 1];
        }
    }
    d->data[toIndexOrIndexMax] = t;

    this->contentOnChange();
}

ZFMETHOD_DEFINE_4(ZFArray, void, sort,
                  ZFMP_IN_OPT(zfbool, ascending, zftrue),
                  ZFMP_IN_OPT(zfindex, start, 0),
                  ZFMP_IN_OPT(zfindex, count, zfindexMax()),
                  ZFMP_IN_OPT(ZFComparer<ZFObject *>::Comparer, comparer, ZFComparerCheckEqual))
{
    if(d->data.size() > 0 && start + 1 < d->data.size() && count > 1)
    {
        zfmSort<ZFObject *>(
            d->data,
            comparer,
            start,
            (count > d->data.size() - start) ? (d->data.size() - 1) : (start + count - 1),
            ascending);

        this->contentOnChange();
    }
}

// ============================================================
static void _ZFP_ZFArray_iteratorDeleteCallback(ZF_IN void *data)
{
    zfdelete((zfindex *)data);
}
static void *_ZFP_ZFArray_iteratorCopyCallback(ZF_IN void *data)
{
    return zfnew(zfindex, *(zfindex *)data);
}

ZFMETHOD_DEFINE_1(ZFArray, zfiterator, iteratorForIndex,
                  ZFMP_IN(zfindex, index))
{
    return zfiterator(zfnew(zfindex, index),
        _ZFP_ZFArray_iteratorDeleteCallback,
        _ZFP_ZFArray_iteratorCopyCallback);
}

ZFMETHOD_DEFINE_0(ZFArray, zfiterator, iterator)
{
    return zfiterator(zfnew(zfindex, 0),
        _ZFP_ZFArray_iteratorDeleteCallback,
        _ZFP_ZFArray_iteratorCopyCallback);
}

ZFMETHOD_DEFINE_1(ZFArray, zfiterator, iteratorFind,
                  ZFMP_IN(ZFObject *, element))
{
    return this->iteratorForIndex(this->find(element));
}

ZFMETHOD_DEFINE_1(ZFArray, zfbool, iteratorValid,
                  ZFMP_IN(const zfiterator &, it))
{
    zfindex *index = it.data<zfindex *>();
    return (index != zfnull && *index < d->data.size());
}
ZFMETHOD_DEFINE_2(ZFArray, zfbool, iteratorEqual,
                  ZFMP_IN(const zfiterator &, it0),
                  ZFMP_IN(const zfiterator &, it1))
{
    return zfiterator::iteratorEqual<zfindex *>(it0, it1);
}

ZFMETHOD_DEFINE_1(ZFArray, void, iteratorNext,
                  ZFMP_IN_OUT(zfiterator &, it))
{
    zfindex *index = it.data<zfindex *>();
    if(index != zfnull && *index < d->data.size())
    {
        ++(*index);
    }
}
ZFMETHOD_DEFINE_1(ZFArray, void, iteratorPrev,
                  ZFMP_IN_OUT(zfiterator &, it))
{
    zfindex *index = it.data<zfindex *>();
    if(index != zfnull && *index < d->data.size())
    {
        --(*index);
    }
}

ZFMETHOD_DEFINE_1(ZFArray, ZFObject *, iteratorValue,
                  ZFMP_IN(const zfiterator &, it))
{
    zfindex *index = it.data<zfindex *>();
    if(index != zfnull && *index < d->data.size())
    {
        return d->data[*index];
    }
    return zfnull;
}

ZFMETHOD_DEFINE_2(ZFArray, void, iteratorValue,
                  ZFMP_IN_OUT(zfiterator &, it),
                  ZFMP_IN(ZFObject *, value))
{
    zfindex *index = it.data<zfindex *>();
    if(index != zfnull && *index < d->data.size())
    {
        this->set(*index, value);
    }
}
ZFMETHOD_DEFINE_1(ZFArray, void, iteratorRemove,
                  ZFMP_IN_OUT(zfiterator &, it))
{
    zfindex *index = it.data<zfindex *>();
    if(index != zfnull && *index < d->data.size())
    {
        this->remove(*index);
        if(*index >= (zfindex)(d->data.size()))
        {
            *index = (zfindex)(d->data.size()) - 1;
        }
    }
}
ZFMETHOD_DEFINE_1(ZFArray, void, iteratorAdd,
                  ZFMP_IN(ZFObject *, value))
{
    this->add(value);
}
ZFMETHOD_DEFINE_2(ZFArray, void, iteratorAdd,
                  ZFMP_IN(ZFObject *, value),
                  ZFMP_IN_OUT(zfiterator &, it))
{
    zfindex *index = it.data<zfindex *>();
    if(index != zfnull && *index < d->data.size())
    {
        this->add((*index)++, value);
    }
    else
    {
        this->add(value);
    }
}

ZF_NAMESPACE_GLOBAL_END

