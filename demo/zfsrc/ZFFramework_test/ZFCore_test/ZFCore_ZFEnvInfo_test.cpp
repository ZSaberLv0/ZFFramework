#include "ZFCore_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclass ZFCore_ZFEnvInfo_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFCore_ZFEnvInfo_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();

        this->testCaseOutput("current env info:");
        this->testCaseOutput(ZFEnvInfo::envSummary().cString());
        this->testCaseStop();
    }
};
ZFOBJECT_REGISTER(ZFCore_ZFEnvInfo_test)

ZF_NAMESPACE_GLOBAL_END

