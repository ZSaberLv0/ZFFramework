/**
 * @file ZFTestCase.h
 * @brief test case define
 */

#ifndef _ZFI_ZFTestCase_h_
#define _ZFI_ZFTestCase_h_

#include "ZFUtilityDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd _ZFP_ZFTestCasePrivate;
/**
 * @brief abstract test case
 *
 * to use, declare a subclass of #ZFTestCase,
 * and override test methods to supply your own test steps,
 * and finally notify end by #testCaseStop\n
 * all test cases can be started automatically by invoking #ZFTestCaseRunAllStart,
 * or you may manually start one by creating new instance of a test case,
 * and call #testCaseStart on it
 * @note automatically test case run depends on #ZFClass's class map,
 *   see #ZFOBJECT_REGISTER for more info
 */
zfabstract ZF_ENV_EXPORT ZFTestCase : zfextends ZFObject
{
    ZFOBJECT_DECLARE_ABSTRACT_WITH_CUSTOM_CTOR(ZFTestCase, ZFObject)

public:
    /**
     * @brief called when output something
     *
     * param0 is a #v_zfstring holds the output,
     * notified when #testCaseOutput called
     */
    ZFOBSERVER_EVENT(TestCaseOnOutput)
    /**
     * @brief called when test started
     *
     * called when test case started
     */
    ZFOBSERVER_EVENT(TestCaseOnStart)
    /**
     * @brief called when test progress
     *
     * param0 is the param passed from #testCaseProgress
     */
    ZFOBSERVER_EVENT(TestCaseOnProgress)
    /**
     * @brief called when test stopped
     *
     * param0 is #ZFResultType
     */
    ZFOBSERVER_EVENT(TestCaseOnStop)

protected:
    zfoverride
    virtual void objectOnDeallocPrepare(void);

protected:
    /** @brief see #EventTestCaseOnOutput */
    virtual inline void testCaseOnOutput(ZF_IN const zfchar *info)
    {
        if(this->observerHasAdd(ZFTestCase::EventTestCaseOnOutput()))
        {
            this->observerNotify(ZFTestCase::EventTestCaseOnOutput(), zflineAlloc(v_zfstring, info));
        }
    }
    /** @brief see #EventTestCaseOnStart */
    virtual inline void testCaseOnStart(void)
    {
        this->observerNotify(ZFTestCase::EventTestCaseOnStart());
    }
    /** @brief see #EventTestCaseOnProgress */
    virtual inline void testCaseOnProgress(ZF_IN ZFObject *progress)
    {
        this->observerNotify(ZFTestCase::EventTestCaseOnProgress(), progress);
    }
    /** @brief see #EventTestCaseOnStop */
    virtual inline void testCaseOnStop(ZF_IN ZFResultTypeEnum testCaseResult)
    {
        zfblockedAlloc(ZFResultType, testCaseResultTmp, testCaseResult);
        this->observerNotify(ZFTestCase::EventTestCaseOnStop(), testCaseResultTmp);
        zfRelease(this);
    }

public:
    /**
     * @brief whether test is running
     */
    ZFMETHOD_DECLARE_0(zfbool, testCaseIsRunning)

    /**
     * @brief output something for the test case,
     *   used to debug
     */
    zffinal void testCaseOutput(ZF_IN const zfchar *info, ...);
    /**
     * @brief manually start a test case
     *
     * do nothing if already begun,
     * automatically retain this object during test,
     * and release it after #testCaseStop
     */
    ZFMETHOD_DECLARE_0(void, testCaseStart)
    /**
     * @brief called to notify progress,
     *   do nothing if not running
     */
    ZFMETHOD_DECLARE_1(void, testCaseProgress,
                       ZFMP_IN_OPT(ZFObject *, progress, zfnull))
    /**
     * @brief must be called to stop test,
     *   do nothing if not running
     *
     * @warning you must not access this object after calling this method
     */
    ZFMETHOD_DECLARE_1(void, testCaseStop,
                       ZFMP_IN_OPT(ZFResultTypeEnum, testCaseResult, ZFResultType::e_Success))

private:
    zfbool _testCaseIsRunning;
protected:
    ZFTestCase(void) : _testCaseIsRunning(zffalse) {}
};

/**
 * @brief util macro to assert within test case
 *
 * upon assert fail, finish the test case by #ZFTestCase::testCaseStop
 */
#define ZFTestCaseAssert(cond) \
    ZFTestCaseAssertWithMessage(cond, "test failed: %s", ZFM_TOSTRING(cond))
/** @brief see #ZFTestCaseAssert */
#define ZFTestCaseAssertTrim(cond) \
    ZFTestCaseAssertTrimWithMessage(cond, "test failed: %s", ZFM_TOSTRING(cond))
/** @brief see #ZFTestCaseAssert */
#define ZFTestCaseAssertDetail(cond, callerInfo) \
    ZFTestCaseAssertDetailWithMessage(cond, callerInfo, "test failed: %s", ZFM_TOSTRING(cond))

/** @brief see #ZFTestCaseAssert */
#define ZFTestCaseAssertWithMessage(cond, fmt, ...) \
    do \
    { \
        if(!(cond)) \
        { \
            this->testCaseOutput("%s " fmt, ZFCallerInfoMake().callerInfo().cString(), ##__VA_ARGS__); \
            this->testCaseStop(ZFResultType::e_Fail); \
            return ; \
        } \
    } while(zffalse)
/** @brief see #ZFTestCaseAssert */
#define ZFTestCaseAssertWithMessageTrim(cond, fmt, ...) \
    do \
    { \
        if(!(cond)) \
        { \
            this->testCaseOutput(fmt, ##__VA_ARGS__); \
            this->testCaseStop(ZFResultType::e_Fail); \
            return ; \
        } \
    } while(zffalse)
/** @brief see #ZFTestCaseAssert */
#define ZFTestCaseAssertWithMessageDetail(cond, callerInfo, fmt, ...) \
    do \
    { \
        if(!(cond)) \
        { \
            this->testCaseOutput("%s " fmt, callerInfo.callerInfo().cString(), fmt, ##__VA_ARGS__); \
            this->testCaseStop(ZFResultType::e_Fail); \
            return ; \
        } \
    } while(zffalse)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFTestCase_h_

