#include "ZFCoreStatistic_ZFTime.h"

ZF_NAMESPACE_GLOBAL_BEGIN
ZF_NAMESPACE_BEGIN(ZFCoreStatistic)

zfclassPOD _ZFP_ZFCoreStatisticInvokeTimeData
{
public:
    zfindex invokeCount;
    ZFTimeValue invokeTotalTime;
    ZFTimeValue invokeStartTime;
    zfindex reentrantCount;
};

static ZFCoreMap &_ZFP_ZFCoreStatisticInvokeTimeDataMap(void)
{
    static ZFCoreMap m; // _ZFP_ZFCoreStatisticInvokeTimeData
    return m;
}

void invokeTimeLogBegin(ZF_IN const zfchar *key)
{
    ZFCoreMap &m = _ZFP_ZFCoreStatisticInvokeTimeDataMap();
    _ZFP_ZFCoreStatisticInvokeTimeData *data = m.get<_ZFP_ZFCoreStatisticInvokeTimeData *>(key);
    if(data == zfnull)
    {
        data = (_ZFP_ZFCoreStatisticInvokeTimeData *)zfmalloc(sizeof(_ZFP_ZFCoreStatisticInvokeTimeData));
        zfmemset(data, 0, sizeof(_ZFP_ZFCoreStatisticInvokeTimeData));
        m.set(key, ZFCorePointerForPOD<_ZFP_ZFCoreStatisticInvokeTimeData *>(data));

        ++(data->invokeCount);
        data->invokeStartTime = ZFTime::currentTimeValue();
    }
    else
    {
        ++(data->invokeCount);

        if(data->invokeStartTime != ZFTimeValueZero())
        {
            ++(data->reentrantCount);
        }
        else
        {
            data->invokeStartTime = ZFTime::currentTimeValue();
        }
    }
}
void invokeTimeLogEnd(ZF_IN const zfchar *key)
{
    ZFCoreMap &m = _ZFP_ZFCoreStatisticInvokeTimeDataMap();
    _ZFP_ZFCoreStatisticInvokeTimeData *data = m.get<_ZFP_ZFCoreStatisticInvokeTimeData *>(key);
    if(data != zfnull)
    {
        if(data->reentrantCount > 0)
        {
            --(data->reentrantCount);
        }
        else
        {
            data->invokeTotalTime += ZFTime::currentTimeValue() - data->invokeStartTime;
            data->invokeStartTime = ZFTimeValueZero();
        }
    }
}
void invokeTimeRemove(ZF_IN const zfchar *key)
{
    ZFCoreMap &m = _ZFP_ZFCoreStatisticInvokeTimeDataMap();
    m.remove(key);
}
void invokeTimeRemoveAll(void)
{
    ZFCoreMap &m = _ZFP_ZFCoreStatisticInvokeTimeDataMap();
    m.removeAll();
}
zfindex invokeTimeGetInvokeCount(ZF_IN const zfchar *key)
{
    ZFCoreMap &m = _ZFP_ZFCoreStatisticInvokeTimeDataMap();
    _ZFP_ZFCoreStatisticInvokeTimeData *data = m.get<_ZFP_ZFCoreStatisticInvokeTimeData *>(key);
    if(data != zfnull)
    {
        return data->invokeCount;
    }
    return 0;
}
ZFTimeValue invokeTimeGetAverageTime(ZF_IN const zfchar *key)
{
    ZFCoreMap &m = _ZFP_ZFCoreStatisticInvokeTimeDataMap();
    _ZFP_ZFCoreStatisticInvokeTimeData *data = m.get<_ZFP_ZFCoreStatisticInvokeTimeData *>(key);
    if(data != zfnull && data->invokeCount > 0)
    {
        return data->invokeTotalTime / data->invokeCount;
    }
    return ZFTimeValueZero();
}
ZFTimeValue invokeTimeGetTotalTime(ZF_IN const zfchar *key)
{
    ZFCoreMap &m = _ZFP_ZFCoreStatisticInvokeTimeDataMap();
    _ZFP_ZFCoreStatisticInvokeTimeData *data = m.get<_ZFP_ZFCoreStatisticInvokeTimeData *>(key);
    if(data != zfnull)
    {
        return data->invokeTotalTime;
    }
    return ZFTimeValueZero();
}
void invokeTimeGetSummary(ZF_OUT zfstring &ret, ZF_IN const zfchar *key)
{
    ZFCoreMap &m = _ZFP_ZFCoreStatisticInvokeTimeDataMap();
    _ZFP_ZFCoreStatisticInvokeTimeData *data = m.get<_ZFP_ZFCoreStatisticInvokeTimeData *>(key);
    zfindex invokeCount = ((data == zfnull) ? 0 : data->invokeCount);
    ZFTimeValue invokeTotalTime = ((data == zfnull) ? ZFTimeValueZero() : data->invokeTotalTime);
    if(invokeCount > 1)
    {
        ZFTimeValue invokeAverageTime = ((invokeCount == 0) ? ZFTimeValueZero() : invokeTotalTime / invokeCount);
        zfstringAppend(ret, "[%s] invoke count: %s, total: %s, average: %s",
            (key == zfnull) ? ZFTOKEN_zfnull : key,
            zfsFromInt(invokeCount).cString(),
            ZFTimeValueToStringFriendly(invokeTotalTime).cString(),
            ZFTimeValueToStringFriendly(invokeAverageTime).cString());
    }
    else
    {
        zfstringAppend(ret, "[%s] invoke time: %s",
            (key == zfnull) ? ZFTOKEN_zfnull : key,
            ZFTimeValueToStringFriendly(invokeTotalTime).cString());
    }
}

ZF_NAMESPACE_END(ZFCoreStatistic)
ZF_NAMESPACE_GLOBAL_END

