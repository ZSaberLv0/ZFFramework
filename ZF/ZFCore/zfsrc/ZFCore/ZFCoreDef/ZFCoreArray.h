/**
 * @file ZFCoreArray.h
 * @brief light weight array
 */

#ifndef _ZFI_ZFCoreArray_h_
#define _ZFI_ZFCoreArray_h_

#include "ZFCoreLog_CommonLog.h"
#include "ZFComparer.h"
#include "ZFCoreUtilMath.h"
#include "ZFToken.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
#define _ZFP_ZFCoreArrayMinCapacity 8

template<typename T_Element>
static void _ZFP_ZFCoreArray_objCreate(ZF_IN T_Element *p, ZF_IN T_Element *pEnd, ZF_IN zfbool PODType)
{
    if(!PODType)
    {
        while(p != pEnd)
        {
            zfnewPlacement(p, T_Element);
            ++p;
        }
    }
}
template<typename T_Element>
static void _ZFP_ZFCoreArray_objCreate(ZF_IN T_Element *p, ZF_IN T_Element *pEnd,
                                       ZF_IN const T_Element *src,
                                       ZF_IN zfbool PODType)
{
    if(PODType)
    {
        zfmemcpy(p, src, (pEnd - p) * sizeof(T_Element));
    }
    else
    {
        while(p != pEnd)
        {
            zfnewPlacement(p, T_Element, *src);
            ++p;
            ++src;
        }
    }
}
template<typename T_Element>
static void _ZFP_ZFCoreArray_objMove(ZF_IN T_Element *dst,
                                     ZF_IN const T_Element *src,
                                     ZF_IN zfindex count,
                                     ZF_IN zfbool PODType)
{
    if(PODType)
    {
        zfmemmove(dst, src, count * sizeof(T_Element));
    }
    else
    {
        zfmemmoveObject(dst, src, count);
    }
}
template<typename T_Element>
static void _ZFP_ZFCoreArray_objDestroy(ZF_IN T_Element *p, ZF_IN T_Element *pEnd,
                                        ZF_IN zfbool PODType)
{
    if(!PODType)
    {
        while(p != pEnd)
        {
            zfdeletePlacement(p);
            ++p;
        }
    }
}
template<typename T_Element>
zffinal zfclassNotPOD ZF_ENV_EXPORT _ZFP_ZFCoreArrayPrivate
{
public:
    zfuint refCount;
    zfbool PODType;
    T_Element *buf;
    zfuint capacity;
    zfuint count;
public:
    _ZFP_ZFCoreArrayPrivate(void)
    : refCount(1)
    , PODType(zffalse)
    , buf(zfnull)
    , capacity(0)
    , count(0)
    {
    }
    ~_ZFP_ZFCoreArrayPrivate(void)
    {
        _ZFP_ZFCoreArray_objDestroy(this->buf, this->buf + this->count, this->PODType);
        zffree(this->buf);
    }
};

// ============================================================
/**
 * @brief dummy base for #ZFCoreArray
 */
zfclassLikePOD ZF_ENV_EXPORT ZFCoreArrayBase
{
public:
    virtual ~ZFCoreArrayBase(void) {}
    /**
     * @brief new reference
     */
    virtual ZFCoreArrayBase *refNew(void) const zfpurevirtual;
    /**
     * @brief delete reference
     */
    virtual void refDelete(void)
    {
        zfdelete(this);
    }
    /**
     * @brief get the impl
     */
    virtual void *refImpl(void) const zfpurevirtual;

    /** @cond ZFPrivateDoc */
    virtual ZFCoreArrayBase &operator = (ZF_IN const ZFCoreArrayBase &ref) zfpurevirtual;
    virtual zfbool operator == (ZF_IN const ZFCoreArrayBase &ref) const zfpurevirtual;
    virtual zfbool operator != (ZF_IN const ZFCoreArrayBase &ref) const zfpurevirtual;
    /** @endcond */
};

// ============================================================
/**
 * @brief light weight array
 *
 * @warning element must support:
 *   constructor with no argument,
 *   copy constructor to create from existing value,
 *   operator = to copy value
 *
 * REMARKS:
 * -  ZFCoreArray is designed to reduce dependency of STL or other third-party containers,
 *   aimed for light weight
 * -  use other containers such as ZFArray in your app,
 *   if performance is really a matter,
 *   choose any other containers you like,
 *   but try not to abuse them,
 *   since it really bricks the portability of your code
 */
template<typename T_Element>
zffinal zfclassLikePOD ZF_ENV_EXPORT ZFCoreArray : zfextendsLikePOD ZFCoreArrayBase
{
public:
    /**
     * @brief main constructor
     */
    ZFCoreArray(void)
    {
        d = zfnew(_ZFP_ZFCoreArrayPrivate<T_Element>);
    }
    /**
     * @brief main constructor
     */
    ZFCoreArray(ZF_IN zfindex capacity)
    {
        d = zfnew(_ZFP_ZFCoreArrayPrivate<T_Element>);
        this->capacity(capacity);
    }
    /**
     * @brief construct from another array
     */
    ZFCoreArray(ZF_IN const ZFCoreArray<T_Element> &ref)
    : d(ref.d)
    {
        ++(d->refCount);
    }
    virtual ~ZFCoreArray(void)
    {
        --(d->refCount);
        if(d->refCount == 0)
        {
            zfdelete(d);
        }
    }
    /**
     * @brief new reference
     */
    virtual ZFCoreArrayBase *refNew(void) const {return zfnew(ZFCoreArray<T_Element>, *this);}
    /**
     * @brief get the impl
     */
    virtual void *refImpl(void) const {return d;}
    /**
     * @brief retain the array, if you want to copy, use #copyFrom instead
     */
    ZFCoreArray<T_Element> &operator = (ZF_IN const ZFCoreArray<T_Element> &ref)
    {
        _ZFP_ZFCoreArrayPrivate<T_Element> *dTmp = d;
        d = ref.d;
        ++(ref.d->refCount);
        --(dTmp->refCount);
        if(dTmp->refCount == 0)
        {
            zfdelete(dTmp);
        }
        return *this;
    }
    /** @cond ZFPrivateDoc */
    zfbool operator == (ZF_IN const ZFCoreArray<T_Element> &ref) const {return (d == ref.d);}
    inline zfbool operator != (ZF_IN const ZFCoreArray<T_Element> &ref) const {return !this->operator == (ref);}
    zfoverride
    virtual ZFCoreArrayBase &operator = (ZF_IN const ZFCoreArrayBase &ref)
    {
        return this->operator = ((const ZFCoreArray<T_Element> &)ref);
    }
    virtual zfbool operator == (ZF_IN const ZFCoreArrayBase &ref) const
    {
        return this->operator == ((const ZFCoreArray<T_Element> &)ref);
    }
    virtual zfbool operator != (ZF_IN const ZFCoreArrayBase &ref) const
    {
        return this->operator == ((const ZFCoreArray<T_Element> &)ref);
    }
    /** @endcond */
    /**
     * @brief copy all settings and contents from another array
     */
    void copyFrom(ZF_IN const ZFCoreArray<T_Element> &ref)
    {
        if(d != ref.d)
        {
            if(d->buf)
            {
                _ZFP_ZFCoreArray_objDestroy(d->buf, d->buf + d->count, d->PODType);
                d->count = 0;
            }
            this->capacity(ref.count());
            _ZFP_ZFCoreArray_objCreate(d->buf, d->buf + ref.count(), ref.arrayBuf(), d->PODType);
            d->count = (zfuint)ref.count();
        }
    }
    /**
     * @brief get retain count
     */
    zfindex objectRetainCount(void) const {return d->refCount;}
    /**
     * @brief compare by content
     */
    ZFCompareResult objectCompare(ZF_IN const ZFCoreArray<T_Element> &ref,
                                  ZF_IN_OPT typename ZFComparer<T_Element>::Comparer comparer = ZFComparerCheckEqual) const
    {
        if(this->count() != ref.count())
        {
            return ZFCompareUncomparable;
        }
        for(zfindex i = this->count() - 1; i != zfindexMax(); --i)
        {
            if(comparer(this->get(i), ref.get(i)) != ZFCompareTheSame)
            {
                return ZFCompareUncomparable;
            }
        }
        return ZFCompareTheSame;
    }

public:
    /** @brief see #objectInfo */
    void objectInfoT(ZF_IN_OUT zfstring &ret) const
    {
        this->objectInfoOfContentT(ret, 10);
    }
    /** @brief return object info */
    inline zfstring objectInfo(void) const
    {
        zfstring ret;
        this->objectInfoT(ret);
        return ret;
    }

public:
    /** @brief see #objectInfoOfContent */
    void objectInfoOfContentT(ZF_IN_OUT zfstring &ret,
                              ZF_IN_OPT zfindex maxCount = zfindexMax(),
                              ZF_IN_OPT const ZFTokenForContainer &token = ZFTokenForContainerDefault(),
                              ZF_IN_OPT typename ZFCoreInfoGetterType<T_Element>::InfoGetter infoGetter = zfnull) const
    {
        if(infoGetter == zfnull)
        {
            infoGetter = ZFCoreInfoGetter<T_Element>::InfoGetter;
        }

        zfindex count = 0;
        ret += token.tokenLeft;
        for(; count < this->count() && count < maxCount; ++count)
        {
            if(count > 0)
            {
                ret += token.tokenSeparator;
            }
            ret += token.tokenValueLeft;
            infoGetter(ret, this->get(count));
            ret += token.tokenValueRight;
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
    /** @brief return content info */
    zfstring objectInfoOfContent(ZF_IN_OPT zfindex maxCount = zfindexMax(),
                                 ZF_IN_OPT const ZFTokenForContainer &token = ZFTokenForContainerDefault(),
                                 ZF_IN_OPT typename ZFCoreInfoGetterType<T_Element>::InfoGetter infoGetter = zfnull) const
    {
        zfstring ret;
        this->objectInfoOfContentT(ret, maxCount, token, infoGetter);
        return ret;
    }

public:
    /**
     * @brief whether the array contains POD type
     */
    zfbool isPODType(void) const {return d->PODType;}
    /**
     * @brief change capacity to hold at least newCapacity
     *
     * do nothing if newCapacity not changed or less than current capacity
     */
    void capacity(ZF_IN zfindex newCapacity)
    {
        zfindex fixedCapacity = zfmMax<zfindex>(this->capacity(), _ZFP_ZFCoreArrayMinCapacity);
        while(fixedCapacity < newCapacity)
        {
            fixedCapacity *= 2;
        }
        this->capacityDoChange(fixedCapacity);
    }
    /**
     * @brief trim current capacity
     *
     * do nothing if not necessary to trim\n
     * this method would be called automatically when remove elements
     */
    void capacityTrim(void)
    {
        if(this->capacity() > _ZFP_ZFCoreArrayMinCapacity
            && this->capacity() > this->count() * 2)
        {
            zfindex fixedCapacity = this->capacity();
            zfindex end = zfmMax<zfindex>(this->count(), _ZFP_ZFCoreArrayMinCapacity) * 2;
            while(fixedCapacity >= end) {fixedCapacity /= 2;}
            this->capacityDoChange(fixedCapacity);
        }
    }
    /**
     * @brief get capacity
     */
    zfindex capacity(void) const {return (zfindex)d->capacity;}

public:
    /**
     * @brief add element
     */
    void add(ZF_IN T_Element const &e)
    {
        T_Element t = e;
        this->capacity(this->count() + 1);
        _ZFP_ZFCoreArray_objCreate(d->buf + d->count, d->buf + d->count + 1, &t, d->PODType);
        ++(d->count);
    }
    /**
     * @brief add element at index
     */
    void add(ZF_IN zfindex index,
             ZF_IN T_Element const &e)
    {
        if(index > this->count())
        {
            zfCoreCriticalIndexOutOfRange(index, this->count() + 1);
            return ;
        }
        T_Element t = e;
        this->capacity(this->count() + 1);
        _ZFP_ZFCoreArray_objCreate(d->buf + d->count, d->buf + d->count + 1, d->PODType);
        T_Element *pos = d->buf + index;
        _ZFP_ZFCoreArray_objMove(pos + 1, pos, this->count() - index, d->PODType);
        ++(d->count);
        *pos = t;
    }
    /**
     * @brief add elements, src can be part of this array's buffer
     */
    void addFrom(ZF_IN const T_Element *src,
                 ZF_IN zfindex count)
    {
        if(src == zfnull || count == 0)
        {
            return ;
        }
        if(src < d->buf || src >= d->buf + d->capacity)
        {
            this->capacity(this->count() + count);
            _ZFP_ZFCoreArray_objCreate(d->buf + d->count, d->buf + d->count + count, src, d->PODType);
            d->count += count;
        }
        else
        {
            ZFCoreArray<T_Element> tmp(count);
            tmp.addFrom(src, count);
            this->addFrom(tmp.arrayBuf(), count);
        }
    }

    /**
     * @brief add from another array
     */
    void addFrom(ZF_IN const ZFCoreArray<T_Element> &ref)
    {
        this->addFrom(ref.arrayBuf(), ref.count());
    }

    /**
     * @brief find element
     */
    zfindex find(ZF_IN T_Element const &e,
                 ZF_IN_OPT typename ZFComparer<T_Element>::Comparer comparer = ZFComparerCheckEqual) const
    {
        for(T_Element *p = d->buf, *pEnd = d->buf + d->count; p < pEnd; ++p)
        {
            if(comparer(*p, e) == ZFCompareTheSame)
            {
                return (p - d->buf);
            }
        }
        return zfindexMax();
    }
    /**
     * @brief find element reversely
     */
    zfindex findReversely(ZF_IN T_Element const &e,
                          ZF_IN_OPT typename ZFComparer<T_Element>::Comparer comparer = ZFComparerCheckEqual) const
    {
        if(d->buf)
        {
            for(T_Element *p = d->buf + d->count - 1; p >= d->buf; --p)
            {
                if(comparer(*p, e) == ZFCompareTheSame)
                {
                    return (p - d->buf);
                }
            }
        }
        return zfindexMax();
    }
    /**
     * @brief find element
     */
    template<typename T_Another>
    zfindex find(ZF_IN T_Another const &e,
                 ZF_IN typename ZFComparer<T_Element, T_Another>::Comparer comparer) const
    {
        for(T_Element *p = d->buf, *pEnd = d->buf + d->count; p < pEnd; ++p)
        {
            if(comparer(*p, e) == ZFCompareTheSame)
            {
                return (p - d->buf);
            }
        }
        return zfindexMax();
    }
    /**
     * @brief find element reversely
     */
    template<typename T_Another>
    zfindex findReversely(ZF_IN T_Another const &e,
                          ZF_IN typename ZFComparer<T_Element, T_Another>::Comparer comparer) const
    {
        if(d->buf)
        {
            for(T_Element *p = d->buf + d->count - 1; p >= d->buf; --p)
            {
                if(comparer(*p, e) == ZFCompareTheSame)
                {
                    return (p - d->buf);
                }
            }
        }
        return zfindexMax();
    }

    /**
     * @brief remove first matched element, return whether the element removed
     */
    zfbool removeElement(ZF_IN T_Element const &e,
                         ZF_IN_OPT typename ZFComparer<T_Element>::Comparer comparer = ZFComparerCheckEqual)
    {
        for(T_Element *p = d->buf, *pEnd = d->buf + d->count; p < pEnd; ++p)
        {
            if(comparer(*p, e) == ZFCompareTheSame)
            {
                this->remove(p - d->buf);
                return zftrue;
            }
        }
        return zffalse;
    }
    /**
     * @brief remove first matched element, return whether the element removed
     */
    template<typename T_Another>
    zfbool removeElement(ZF_IN T_Another const &e,
                         ZF_IN typename ZFComparer<T_Element, T_Another>::Comparer comparer)
    {
        for(T_Element *p = d->buf, *pEnd = d->buf + d->count; p < pEnd; ++p)
        {
            if(comparer(*p, e) == ZFCompareTheSame)
            {
                this->remove(p - d->buf);
                return zftrue;
            }
        }
        return zffalse;
    }
    /**
     * @brief remove last matched element, return whether the element removed
     */
    zfbool removeElementReversely(ZF_IN T_Element const &e,
                                  ZF_IN_OPT typename ZFComparer<T_Element>::Comparer comparer = ZFComparerCheckEqual)
    {
        if(d->buf)
        {
            for(T_Element *p = d->buf + d->count - 1; p >= d->buf; --p)
            {
                if(comparer(*p, e) == ZFCompareTheSame)
                {
                    this->remove(p - d->buf);
                    return zftrue;
                }
            }
        }
        return zffalse;
    }
    /**
     * @brief remove last matched element, return whether the element removed
     */
    template<typename T_Another>
    zfbool removeElementReversely(ZF_IN T_Another const &e,
                                  ZF_IN typename ZFComparer<T_Element, T_Another>::Comparer comparer)
    {
        if(d->buf)
        {
            for(T_Element *p = d->buf + d->count - 1; p >= d->buf; --p)
            {
                if(comparer(*p, e) == ZFCompareTheSame)
                {
                    this->remove(p - d->buf);
                    return zftrue;
                }
            }
        }
        return zffalse;
    }
    /**
     * @brief remove all matched element, return number of removed element
     */
    zfindex removeElementAll(ZF_IN T_Element const &e,
                             ZF_IN_OPT typename ZFComparer<T_Element>::Comparer comparer = ZFComparerCheckEqual)
    {
        zfindex removedCount = 0;
        for(T_Element *p = d->buf, *pEnd = d->buf + d->count; p < pEnd; ++p)
        {
            if(comparer(*p, e) == ZFCompareTheSame)
            {
                ++removedCount;
                this->remove(p - d->buf);
                --p;
            }
        }
        return removedCount;
    }
    /**
     * @brief remove all matched element, return number of removed element
     */
    template<typename T_Another>
    zfindex removeElementAll(ZF_IN T_Another const &e,
                             ZF_IN typename ZFComparer<T_Element, T_Another>::Comparer comparer)
    {
        zfindex removedCount = 0;
        for(T_Element *p = d->buf, *pEnd = d->buf + d->count; p < pEnd; ++p)
        {
            if(comparer(*p, e) == ZFCompareTheSame)
            {
                ++removedCount;
                this->remove(p - d->buf);
                --p;
            }
        }
        return removedCount;
    }

    /**
     * @brief remove element at index with count, assert fail if out of range
     */
    void remove(ZF_IN zfindex index)
    {
        if(index >= this->count())
        {
            zfCoreCriticalIndexOutOfRange(index, this->count());
            return ;
        }
        _ZFP_ZFCoreArray_objMove(d->buf + index, d->buf + index + 1, this->count() - index - 1, d->PODType);
        _ZFP_ZFCoreArray_objDestroy(d->buf + d->count - 1, d->buf + d->count, d->PODType);
        --(d->count);
        this->capacityTrim();
    }
    /**
     * @brief remove element at index with count, assert fail if out of range
     */
    void remove(ZF_IN zfindex index,
                ZF_IN zfindex count)
    {
        if(index >= this->count())
        {
            zfCoreCriticalIndexOutOfRange(index, this->count());
            return ;
        }
        if(count > this->count() - index)
        {
            count = this->count() - index;
        }
        _ZFP_ZFCoreArray_objMove(d->buf + index, d->buf + index + count, this->count() - (index + count), d->PODType);
        _ZFP_ZFCoreArray_objDestroy(d->buf + d->count - count, d->buf + d->count, d->PODType);
        d->count -= count;
        this->capacityTrim();
    }
    /**
     * @brief remove and return the removed value
     */
    T_Element removeAndGet(ZF_IN zfindex index)
    {
        if(index >= this->count())
        {
            zfCoreCriticalIndexOutOfRange(index, this->count());
        }
        T_Element t = *(d->buf + index);
        this->remove(index);
        return t;
    }
    /**
     * @brief remove first or do nothing if empty
     */
    void removeFirst(void)
    {
        if(!this->isEmpty())
        {
            this->remove(0);
        }
    }
    /**
     * @brief remove first and return the removed value,
     *   or assert fail if empty
     */
    T_Element removeFirstAndGet(void)
    {
        zfCoreAssertWithMessage(!this->isEmpty(), "removeFirstAndGet an empty array");
        T_Element t = *(d->buf);
        this->remove(0);
        return t;
    }
    /**
     * @brief remove last or do nothing if empty
     */
    void removeLast(void)
    {
        if(!this->isEmpty())
        {
            this->remove(this->count() - 1);
        }
    }
    /**
     * @brief remove last and return the removed value,
     *   or assert fail if empty
     */
    T_Element removeLastAndGet(void)
    {
        zfCoreAssertWithMessage(!this->isEmpty(), "removeLastAndGet an empty array");
        T_Element t = *(d->buf + d->count - 1);
        this->remove(this->count() - 1);
        return t;
    }
    /**
     * @brief remove all content
     */
    void removeAll(void)
    {
        _ZFP_ZFCoreArray_objDestroy(d->buf, d->buf + d->count, d->PODType);
        d->count = 0;
    }

    /**
     * @brief move element
     */
    void move(ZF_IN zfindex fromIndex, ZF_IN zfindex toIndexOrIndexMax)
    {
        if(fromIndex >= this->count())
        {
            zfCoreCriticalIndexOutOfRange(fromIndex, this->count());
            return ;
        }
        if(toIndexOrIndexMax == zfindexMax())
        {
            toIndexOrIndexMax = this->count() - 1;
        }
        if(toIndexOrIndexMax >= this->count())
        {
            zfCoreCriticalIndexOutOfRange(toIndexOrIndexMax, this->count());
            return ;
        }
        if(fromIndex == toIndexOrIndexMax)
        {
            return ;
        }
        T_Element t = d->buf[fromIndex];
        if(fromIndex < toIndexOrIndexMax)
        {
            _ZFP_ZFCoreArray_objMove(d->buf + fromIndex, d->buf + fromIndex + 1, toIndexOrIndexMax - fromIndex, d->PODType);
        }
        else
        {
            _ZFP_ZFCoreArray_objMove(d->buf + toIndexOrIndexMax + 1, d->buf + toIndexOrIndexMax, fromIndex - toIndexOrIndexMax, d->PODType);
        }
        d->buf[toIndexOrIndexMax] = t;
    }

public:
    /**
     * @brief set element at index, or assert fail if index out of range
     */
    void set(ZF_IN zfindex index, ZF_IN T_Element const &e)
    {
        if(index >= this->count())
        {
            zfCoreCriticalIndexOutOfRange(index, this->count());
        }
        d->buf[index] = e;
    }

public:
    /**
     * @brief get element's reference at index
     */
    T_Element &get(ZF_IN zfindex index)
    {
        if(index >= this->count())
        {
            zfCoreCriticalIndexOutOfRange(index, this->count());
        }
        return d->buf[index];
    }
    /**
     * @brief get element's const reference at index
     */
    T_Element const &get(ZF_IN zfindex index) const
    {
        if(index >= this->count())
        {
            zfCoreCriticalIndexOutOfRange(index, this->count());
        }
        return d->buf[index];
    }
    /**
     * @brief get element's reference at index
     */
    T_Element &operator [] (ZF_IN zfindex index)
    {
        if(index >= this->count())
        {
            zfCoreCriticalIndexOutOfRange(index, this->count());
        }
        return d->buf[index];
    }
    /**
     * @brief get element's const reference at index
     */
    T_Element const &operator [] (ZF_IN zfindex index) const
    {
        if(index >= this->count())
        {
            zfCoreCriticalIndexOutOfRange(index, this->count());
        }
        return d->buf[index];
    }
    /**
     * @brief try to get first element, assert fail if empty
     */
    T_Element const &getFirst(void) const
    {
        if(this->count() == 0)
        {
            zfCoreCriticalIndexOutOfRange(0, this->count());
        }
        return *(d->buf);
    }
    /**
     * @brief try to get first element, assert fail if empty
     */
    T_Element const &getLast(void) const
    {
        if(this->count() == 0)
        {
            zfCoreCriticalIndexOutOfRange(0, this->count());
        }
        return *(d->buf + d->count - 1);
    }

    /**
     * @brief directly access the array
     *
     * @warning the returned pointer may changed after modify the array
     * @warning return null if current capacity is 0
     */
    T_Element *arrayBuf(void) {return d->buf;}
    /**
     * @brief see #arrayBuf
     */
    const T_Element *arrayBuf(void) const {return d->buf;}

    /**
     * @brief element count of this array
     */
    zfindex count(void) const {return (zfindex)d->count;}
    /**
     * @brief true if empty
     */
    zfbool isEmpty(void) const {return (d->count == 0);}

public:
    /**
     * @brief sort element
     */
    void sort(ZF_IN typename ZFComparer<T_Element>::Comparer elementComparer,
              ZF_IN_OPT zfbool ascending = zftrue,
              ZF_IN_OPT zfindex start = 0,
              ZF_IN_OPT zfindex count = zfindexMax())
    {
        if(!this->isEmpty() && start + 1 < this->count() && count > 1)
        {
            zfmSort<T_Element>(
                d->buf,
                elementComparer,
                start,
                (count > this->count() - start) ? (this->count() - 1) : (start + count - 1),
                ascending);
        }
    }

protected:
    void _ZFP_PODType(void) {d->PODType = zftrue;}
private:
    _ZFP_ZFCoreArrayPrivate<T_Element> *d;
    void capacityDoChange(ZF_IN zfindex newCapacity)
    {
        if(newCapacity != this->capacity())
        {
            if(newCapacity == 0)
            {
                _ZFP_ZFCoreArray_objDestroy(d->buf, d->buf + d->count, d->PODType);
                zffree(d->buf);
                d->buf = zfnull;
                d->capacity = 0;
                d->count = 0;
            }
            else
            {
                T_Element *oldBuf = d->buf;
                zfuint oldCount = d->count;

                T_Element *newBuf = (T_Element *)zfmalloc(newCapacity * sizeof(T_Element));
                _ZFP_ZFCoreArray_objCreate(newBuf, newBuf + oldCount, oldBuf, d->PODType);

                d->buf = newBuf;
                d->capacity = (zfuint)newCapacity;
                d->count = oldCount;

                _ZFP_ZFCoreArray_objDestroy(oldBuf, oldBuf + oldCount, d->PODType);
                zffree(oldBuf);
            }
        }
    }
};

// ============================================================
/**
 * @brief POD version of #ZFCoreArray
 *
 * typically has higher performance than non-POD version
 * @warning you should ensure the content type is POD type
 */
template<typename T_Element>
zffinal zfclassLikePOD ZF_ENV_EXPORT ZFCoreArrayPOD : zfextendsLikePOD ZFCoreArray<T_Element>
{
public:
    /**
     * @brief see #ZFCoreArray
     */
    ZFCoreArrayPOD(void)
    : ZFCoreArray<T_Element>()
    {
        this->_ZFP_PODType();
    }
    /**
     * @brief see #ZFCoreArray
     */
    ZFCoreArrayPOD(ZF_IN zfindex capacity)
    : ZFCoreArray<T_Element>(capacity)
    {
        this->_ZFP_PODType();
    }
    /**
     * @brief see #ZFCoreArray
     */
    ZFCoreArrayPOD(ZF_IN const ZFCoreArray<T_Element> &ref)
    : ZFCoreArray<T_Element>(ref)
    {
    }
    /**
     * @brief see #ZFCoreArray
     */
    ZFCoreArrayPOD<T_Element> &operator = (ZF_IN const ZFCoreArray<T_Element> &ref)
    {
        ZFCoreArray<T_Element>::operator = (ref);
        return *this;
    }
    /**
     * @brief see #ZFCoreArray
     */
    ZFCoreArrayPOD<T_Element> &operator = (ZF_IN const ZFCoreArrayPOD<T_Element> &ref)
    {
        ZFCoreArray<T_Element>::operator = (ref);
        return *this;
    }
};

template<typename T_Element>
zfclassLikePOD ZF_ENV_EXPORT _ZFP_ZFCoreArrayCreate
{
public:
    inline _ZFP_ZFCoreArrayCreate<T_Element> &add(ZF_IN T_Element const &v)
    {
        this->v.add(v);
        return *this;
    }

public:
    ZFCoreArray<T_Element> v;
};
template<typename T_Element>
zfclassLikePOD ZF_ENV_EXPORT _ZFP_ZFCoreArrayPODCreate
{
public:
    inline _ZFP_ZFCoreArrayPODCreate<T_Element> &add(ZF_IN T_Element const &v)
    {
        this->v.add(v);
        return *this;
    }

public:
    ZFCoreArrayPOD<T_Element> v;
};
#define _ZFP_ZFCoreArrayCreate_action_expand(value) .add(value)
#define _ZFP_ZFCoreArrayCreate_action(CreatorType, values, ...) \
    CreatorType() ZFM_FIX_PARAM(_ZFP_ZFCoreArrayCreate_action_expand, ZFM_EMPTY, values, ##__VA_ARGS__) .v
/**
 * @brief util macro to create a temp array for short
 *
 * usage:
 * @code
 *   ZFCoreArray<YourType> array = ZFCoreArrayCreate(YourType, v0, v1, ...); // any element count
 * @endcode
 */
#define ZFCoreArrayCreate(ElementType, values, ...) _ZFP_ZFCoreArrayCreate_action(_ZFP_ZFCoreArrayCreate<ElementType>, values, ##__VA_ARGS__)
/**
 * @brief create POD version, see #ZFCoreArrayCreate
 */
#define ZFCoreArrayPODCreate(ElementType, values, ...) _ZFP_ZFCoreArrayCreate_action(_ZFP_ZFCoreArrayPODCreate<ElementType>, values, ##__VA_ARGS__)

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreArray_h_

