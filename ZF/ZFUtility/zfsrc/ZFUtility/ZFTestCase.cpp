#include "ZFTestCase.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFTestCase)

ZFOBSERVER_EVENT_REGISTER(ZFTestCase, TestCaseOnOutput)
ZFOBSERVER_EVENT_REGISTER(ZFTestCase, TestCaseOnStart)
ZFOBSERVER_EVENT_REGISTER(ZFTestCase, TestCaseOnProgress)
ZFOBSERVER_EVENT_REGISTER(ZFTestCase, TestCaseOnStop)

void ZFTestCase::objectOnDeallocPrepare(void)
{
    zfCoreAssert(!this->testCaseIsRunning());
    zfsuper::objectOnDeallocPrepare();
}

ZFMETHOD_DEFINE_0(ZFTestCase, zfbool, testCaseIsRunning)
{
    return this->_testCaseIsRunning;
}
void ZFTestCase::testCaseOutput(ZF_IN const zfchar *info, ...)
{
    va_list vaList;
    va_start(vaList, info);
    zfstring data = zfstringWithFormatV(info, vaList);
    va_end(vaList);

    this->testCaseOnOutput(data.cString());
}
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFTestCase, void, testCaseOutput, ZFMP_IN(const zfchar *, info))
ZFMETHOD_DEFINE_0(ZFTestCase, void, testCaseStart)
{
    if(this->_testCaseIsRunning)
    {
        return ;
    }
    this->_testCaseIsRunning = zftrue;
    zfRetain(this);
    this->testCaseOnStart();
}

ZFMETHOD_DEFINE_1(ZFTestCase, void, testCaseProgress,
                  ZFMP_IN_OPT(ZFObject *, progress, zfnull))
{
    if(!this->_testCaseIsRunning)
    {
        return ;
    }
    this->testCaseOnProgress(progress);

    zfRelease(progress);
}

ZFMETHOD_DEFINE_1(ZFTestCase, void, testCaseStop,
                  ZFMP_IN_OPT(ZFResultTypeEnum, testCaseResult, ZFResultType::e_Success))
{
    if(!this->_testCaseIsRunning)
    {
        return ;
    }
    this->_testCaseIsRunning = zffalse;
    this->testCaseOnStop(testCaseResult);
}

ZF_NAMESPACE_GLOBAL_END

