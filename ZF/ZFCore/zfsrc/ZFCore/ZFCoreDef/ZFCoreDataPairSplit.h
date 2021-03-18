/**
 * @file ZFCoreDataPairSplit.h
 * @brief util to split data pairs from a string
 */

#ifndef _ZFI_ZFCoreDataPairSplit_h_
#define _ZFI_ZFCoreDataPairSplit_h_

#include "ZFCoreArray.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief split data pair such as "(a, b, c)" from a string
 *
 * return splited sub-string's index range\n
 * spaces and tabs would be ignored between elements and tokens,
 * spaces and tabs in elements would be reserved\n
 * only all format matched would result success,
 * missing or extra tokens would result fail\n
 * embeded data pair is allowed,
 * such as "(a, (b, c), d)" can be splited to "a", "(b, c)", "d"\n
 * \n
 * leftToken or rightToken can be disabled by setting to zfnull\n
 * if leftToken has been set, then rightToken must also be set with same order\n
 * if rightToken not set, src can only be stopped by reach end
 */
extern ZF_ENV_EXPORT zfbool zfCoreDataPairSplitString(ZF_IN_OUT ZFCoreArray<ZFIndexRange> &outData,
                                                      ZF_IN zfindex desiredCountOrIndexMax,
                                                      ZF_IN const zfchar *src,
                                                      ZF_IN_OPT zfindex srcLen = zfindexMax(),
                                                      ZF_IN_OPT const zfchar *separatorTokens = ",",
                                                      ZF_IN_OPT const zfchar *leftTokens = "(",
                                                      ZF_IN_OPT const zfchar *rightTokens = ")",
                                                      ZF_IN_OPT zfbool allowEmptyItem = zffalse,
                                                      ZF_OUT_OPT const zfchar **outErrorPos = zfnull);

/**
 * @brief split data to int array format, see #zfCoreDataPairSplitString
 */
template<typename T_int>
zfbool zfCoreDataPairSplitInt(ZF_IN_OUT ZFCoreArray<T_int> &outData,
                              ZF_IN zfindex desiredCountOrIndexMax,
                              ZF_IN const zfchar *src,
                              ZF_IN_OPT zfindex srcLen = zfindexMax(),
                              ZF_IN_OPT const zfchar *separatorTokens = ",",
                              ZF_IN_OPT const zfchar *leftTokens = "(",
                              ZF_IN_OPT const zfchar *rightTokens = ")",
                              ZF_IN_OPT zfbool allowEmptyItem = zffalse,
                              ZF_OUT_OPT const zfchar **outErrorPos = zfnull)
{
    ZFCoreArrayPOD<ZFIndexRange> splited;
    if(!zfCoreDataPairSplitString(
        splited,
        desiredCountOrIndexMax,
        src, srcLen,
        separatorTokens, leftTokens, rightTokens,
        allowEmptyItem,
        outErrorPos))
    {
        return zffalse;
    }
    for(zfindex i = 0; i < splited.count(); ++i)
    {
        T_int tmp;
        tmp = 0;
        if(!zfsToIntT(tmp, src + splited[i].start, splited[i].count, 10, zftrue, outErrorPos))
        {
            return zffalse;
        }
        outData.add(tmp);
    }
    return zftrue;
}

/**
 * @brief split data to float array format, see #zfCoreDataPairSplitString
 */
template<typename T_float>
zfbool zfCoreDataPairSplitFloat(ZF_IN_OUT ZFCoreArray<T_float> &outData,
                                ZF_IN zfindex desiredCountOrIndexMax,
                                ZF_IN const zfchar *src,
                                ZF_IN_OPT zfindex srcLen = zfindexMax(),
                                ZF_IN_OPT const zfchar *separatorTokens = ",",
                                ZF_IN_OPT const zfchar *leftTokens = "(",
                                ZF_IN_OPT const zfchar *rightTokens = ")",
                                ZF_IN_OPT zfbool allowEmptyItem = zffalse,
                                ZF_OUT_OPT const zfchar **outErrorPos = zfnull)
{
    ZFCoreArrayPOD<ZFIndexRange> splited;
    if(!zfCoreDataPairSplitString(
        splited,
        desiredCountOrIndexMax,
        src, srcLen,
        separatorTokens, leftTokens, rightTokens,
        allowEmptyItem,
        outErrorPos))
    {
        return zffalse;
    }
    for(zfindex i = 0; i < splited.count(); ++i)
    {
        T_float tmp = 0;
        if(!zfsToFloatT(tmp, src + splited[i].start, splited[i].count, outErrorPos))
        {
            return zffalse;
        }
        outData.add(tmp);
    }
    return zftrue;
}

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreDataPairSplit_h_

