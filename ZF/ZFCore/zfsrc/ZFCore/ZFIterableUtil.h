/**
 * @file ZFIterableUtil.h
 * @brief util for #ZFIterable
 */

#ifndef _ZFI_ZFIterableUtil_h_
#define _ZFI_ZFIterableUtil_h_

#include "ZFIterable.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief util method to add all contents from src to dst
 */
template<typename T_Dst, typename T_Src>
void ZFIterableAddFrom(ZF_IN_OUT T_Dst dst, ZF_IN T_Src src)
{
    for(zfiterator it = src->iterator(); src->iteratorIsValid(it);)
    {
        dst->iteratorAdd(src->iteratorNextValue(it));
    }
}

/**
 * @brief util method to find element
 */
template<typename T_ZFIterable, typename T_Element>
zfiterator ZFIterableFind(ZF_IN T_ZFIterable iterable,
                          ZF_IN T_Element const &e,
                          ZF_IN_OPT typename ZFComparer<T_Element>::Comparer comparer = ZFComparerDefault)
{
    for(zfiterator it = iterable->iterator(); iterable->iteratorIsValid(it); iterable->iteratorNextValue(it))
    {
        if(comparer(iterable->iteratorValue(it), e) == ZFCompareTheSame)
        {
            return it;
        }
    }
    return zfiteratorInvalid();
}

/**
 * @brief util method to find element
 */
template<typename T_ZFIterable, typename T_Element>
zfiterator ZFIterableFind(ZF_IN T_ZFIterable iterable,
                          ZF_IN T_Element const &e,
                          ZF_IN const zfiterator &itBegin,
                          ZF_IN_OPT const zfiterator &itEnd = zfiteratorInvalid(),
                          ZF_IN_OPT typename ZFComparer<T_Element>::Comparer comparer = ZFComparerDefault)
{
    for(zfiterator it = itBegin; !iterable->iteratorIsEqual(it, itEnd); iterable->iteratorNextValue(it))
    {
        if(comparer(iterable->iteratorValue(it), e) == ZFCompareTheSame)
        {
            return it;
        }
    }
    return zfiteratorInvalid();
}

/**
 * @brief util method to remove element
 */
template<typename T_ZFIterable, typename T_Element>
zfiterator ZFIterableRemove(ZF_IN_OUT T_ZFIterable iterable,
                            ZF_IN T_Element const &e,
                            ZF_IN_OPT zfindex maxCountToRemove = 1,
                            ZF_IN_OPT typename ZFComparer<T_Element>::Comparer comparer = ZFComparerDefault)
{
    zfindex removed = 0;
    for(zfiterator it = iterable->iterator(); removed < maxCountToRemove && iterable->iteratorIsValid(it); )
    {
        if(comparer(iterable->iteratorValue(it), e) == ZFCompareTheSame)
        {
            ++removed;
            iterable->iteratorRemove(it);
        }
        else
        {
            iterable->iteratorNextValue(it);
        }
    }
    return zfiteratorInvalid();
}

/**
 * @brief util method to remove element
 */
template<typename T_ZFIterable, typename T_Element>
zfiterator ZFIterableRemove(ZF_IN_OUT T_ZFIterable iterable,
                            ZF_IN T_Element const &e,
                            ZF_IN const zfiterator &itBegin,
                            ZF_IN_OPT const zfiterator &itEnd = zfiteratorInvalid(),
                            ZF_IN_OPT zfindex maxCountToRemove = 1,
                            ZF_IN_OPT typename ZFComparer<T_Element>::Comparer comparer = ZFComparerDefault)
{
    zfindex removed = 0;
    for(zfiterator it = itBegin; removed < maxCountToRemove && !iterable->iteratorIsEqual(it, itEnd); )
    {
        if(comparer(iterable->iteratorValue(it), e) == ZFCompareTheSame)
        {
            ++removed;
            iterable->iteratorRemove(it);
        }
        else
        {
            iterable->iteratorNextValue(it);
        }
    }
    return zfiteratorInvalid();
}

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFIterableUtil_h_

