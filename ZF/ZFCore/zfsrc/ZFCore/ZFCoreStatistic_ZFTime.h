/**
 * @file ZFCoreStatistic_ZFTime.h
 * @brief util to log function's invoke time
 */

#ifndef _ZFI_ZFCoreStatistic_ZFTime_h_
#define _ZFI_ZFCoreStatistic_ZFTime_h_

#include "ZFTime.h"
ZF_NAMESPACE_GLOBAL_BEGIN
ZF_NAMESPACE_BEGIN(ZFCoreStatistic)

// ============================================================
/**
 * @brief util to log function's invoke time
 *
 * usage:
 * @code
 *   ZFCoreStatistic::invokeTimeLogBegin(key);
 *   yourHeavyFunc();
 *   ZFCoreStatistic::invokeTimeLogEnd(key);
 *   // ...
 *   // you may call any times
 *
 *   // finally, print result, and remove
 *   zfLogTrimT() << ZFCoreStatistic::invokeTimeGetSummary(key);
 *   ZFCoreStatistic::invokeTimeRemove(key);
 *
 *   // or, you may use the convenient macro to log and output for one time
 *   {
 *       ZFCoreStatisticInvokeTimeLoggerOneTime(key, optional_outputCallback);
 *       yourHeavyFunc();
 *   }
 * @endcode
 */
extern ZF_ENV_EXPORT void invokeTimeLogBegin(ZF_IN const zfchar *key);
/** @brief see #ZFCoreStatistic::invokeTimeLogBegin */
extern ZF_ENV_EXPORT void invokeTimeLogEnd(ZF_IN const zfchar *key);
/** @brief see #ZFCoreStatistic::invokeTimeLogBegin */
extern ZF_ENV_EXPORT void invokeTimeRemove(ZF_IN const zfchar *key);
/** @brief see #ZFCoreStatistic::invokeTimeLogBegin */
extern ZF_ENV_EXPORT void invokeTimeRemoveAll(void);
/** @brief see #ZFCoreStatistic::invokeTimeLogBegin */
extern ZF_ENV_EXPORT zfindex invokeTimeGetInvokeCount(ZF_IN const zfchar *key);
/** @brief see #ZFCoreStatistic::invokeTimeLogBegin */
extern ZF_ENV_EXPORT ZFTimeValue invokeTimeGetAverageTime(ZF_IN const zfchar *key);
/** @brief see #ZFCoreStatistic::invokeTimeLogBegin */
extern ZF_ENV_EXPORT ZFTimeValue invokeTimeGetTotalTime(ZF_IN const zfchar *key);
/** @brief see #ZFCoreStatistic::invokeTimeLogBegin */
extern ZF_ENV_EXPORT void invokeTimeGetSummary(ZF_OUT zfstring &ret, ZF_IN const zfchar *key);
/** @brief see #ZFCoreStatistic::invokeTimeLogBegin */
inline zfstring invokeTimeGetSummary(ZF_IN const zfchar *key)
{
    zfstring ret;
    ZFCoreStatistic::invokeTimeGetSummary(ret, key);
    return ret;
}

// ============================================================
zfclassLikePOD ZF_ENV_EXPORT _ZFP_ZFCoreStatisticInvokeTimeLogger
{
public:
    _ZFP_ZFCoreStatisticInvokeTimeLogger(ZF_IN const zfchar *key)
    : key(key)
    {
        ZFCoreStatistic::invokeTimeLogBegin(key);
    }
    ~_ZFP_ZFCoreStatisticInvokeTimeLogger(void)
    {
        ZFCoreStatistic::invokeTimeLogEnd(key);
    }
private:
    zfstring key;
};
/** @brief see #ZFCoreStatistic::invokeTimeLogBegin */
#define ZFCoreStatisticInvokeTimeLogger(key) \
    ZFCoreStatistic::_ZFP_ZFCoreStatisticInvokeTimeLogger ZFUniqueName(ZFCoreStatisticInvokeTimeLogger_v)(key)

// ============================================================
zfclassLikePOD ZF_ENV_EXPORT _ZFP_ZFCoreStatisticInvokeTimeLoggerOneTime
{
public:
    _ZFP_ZFCoreStatisticInvokeTimeLoggerOneTime(ZF_IN const zfchar *key,
                                                ZF_IN const ZFOutput &output = ZFOutputDefault())
    : key(key)
    , output(output)
    {
        ZFCoreStatistic::invokeTimeLogBegin(key);
    }
    ~_ZFP_ZFCoreStatisticInvokeTimeLoggerOneTime(void)
    {
        ZFCoreStatistic::invokeTimeLogEnd(key);
        output << ZFCoreStatistic::invokeTimeGetSummary(key) << "\n";
        ZFCoreStatistic::invokeTimeRemove(key);
    }
private:
    zfstring key;
    ZFOutput output;
};
/** @brief see #ZFCoreStatistic::invokeTimeLogBegin */
#define ZFCoreStatisticInvokeTimeLoggerOneTime(key, ...) \
    ZFCoreStatistic::_ZFP_ZFCoreStatisticInvokeTimeLoggerOneTime \
        ZFUniqueName(ZFCoreStatisticInvokeTimeLoggerOneTime_v)(key, ##__VA_ARGS__)

ZF_NAMESPACE_END(ZFCoreStatistic)
ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFCoreStatistic_ZFTime_h_

