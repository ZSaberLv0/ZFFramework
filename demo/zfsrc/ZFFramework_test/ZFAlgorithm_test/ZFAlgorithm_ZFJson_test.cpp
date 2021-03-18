#include "ZFAlgorithm_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFAlgorithm_ZFJson_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFAlgorithm_ZFJson_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();

        this->testCaseOutputSeparator();
        this->testCaseOutput("ZFJson");

        const zfchar *src =
            "{"
            "    \"k0\" : \"v0\","
            ""
            "    \"object0\" : {},"
            "    \"object1\" : {"
            "        \"k0\" : \"v0\","
            "        \"k1\" : \"v1\""
            "    },"
            "    \"object2\" : {"
            "        \"k0\" : {\"k0\" : \"v0\"}"
            "    },"
            ""
            "    \"array0\" : [],"
            "    \"array1\" : ["
            "        {\"k0\" : \"v0\"},"
            "        {\"k0\" : \"v0\"}"
            "    ],"
            ""
            "    \"k1\" : \"special chars: \\\\ \\\"\""
            "}"
            ;

        this->testCaseOutputSeparator();
        this->testCaseOutput("ZFJson parse from string:");
        ZFJsonItem json = ZFJsonItemFromString(src);

        this->testCaseOutput("ZFJson parse from string, result:");
        zfLogTrimT() << ZFJsonItemToString(json);

        this->testCaseOutputSeparator();
        this->testCaseOutput("ZFJson add element");
        json.jsonItemValue("testJsonKey", "testJsonValue");
        this->testCaseOutput("ZFJson add element, result:");
        zfLogTrimT() << ZFJsonItemToString(json);

        this->performanceTest(json);

        this->testCaseStop();
    }
private:
    void performanceTest(ZF_IN const ZFJsonItem &jsonItem)
    {
        zfindex toDataTimes = 10000;
        ZFCoreStatistic::invokeTimeLogBegin("ZFJsonPerformance_test_toData");
        for(zfindex i = 0; i < toDataTimes; ++i)
        {
            zfstring tmp;
            ZFJsonItemToOutput(ZFOutputForString(tmp), jsonItem);
        }
        ZFCoreStatistic::invokeTimeLogEnd("ZFJsonPerformance_test_toData");

        zfindex fromDataTimes = toDataTimes;
        zfstring jsonString;
        ZFJsonItemToOutput(ZFOutputForString(jsonString), jsonItem);
        ZFCoreStatistic::invokeTimeLogBegin("ZFJsonPerformance_test_fromData");
        for(zfindex i = 0; i < fromDataTimes; ++i)
        {
            ZFJsonItemFromInput(ZFInputForBufferUnsafe(jsonString));
        }
        ZFCoreStatistic::invokeTimeLogEnd("ZFJsonPerformance_test_fromData");
        ZFJsonItem jsonItemNew = ZFJsonItemFromInput(ZFInputForBufferUnsafe(jsonString));

        this->testCaseOutputSeparator();

        ZFTimeValue toDataUsedTime = ZFCoreStatistic::invokeTimeGetTotalTime("ZFJsonPerformance_test_toData");
        this->testCaseOutput("serialize to data %zi times cost %s seconds",
            toDataTimes,
            ZFTimeValueToStringFriendly(toDataUsedTime).cString());

        ZFTimeValue fromDataUsedTime = ZFCoreStatistic::invokeTimeGetTotalTime("ZFJsonPerformance_test_fromData");
        this->testCaseOutput("serialize from data %zi times cost %s seconds",
            fromDataTimes,
            ZFTimeValueToStringFriendly(fromDataUsedTime).cString());

        #if 0
            this->testCaseOutputSeparator();
            this->testCaseOutput("content: %s", jsonString.cString());
            this->testCaseOutput("newly: %s", ZFJsonItemToString(jsonItemNew).cString());
        #endif

        ZFCoreStatistic::invokeTimeRemove("ZFJsonPerformance_test_toData");
        ZFCoreStatistic::invokeTimeRemove("ZFJsonPerformance_test_fromData");
    }
};
ZFOBJECT_REGISTER(ZFAlgorithm_ZFJson_test)

ZF_NAMESPACE_GLOBAL_END

