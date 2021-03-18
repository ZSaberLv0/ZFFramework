#include "ZFCore_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclass ZFCore_ZFLogStackTrace_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFCore_ZFLogStackTrace_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();

        this->testCaseOutput("stack trace:\n%s", zfLogStackTrace().cString());

        this->testCaseOutput("caller info: %s", zfLogCallerInfo().cString());

        this->testCaseStop();
    }
};
ZFOBJECT_REGISTER(ZFCore_ZFLogStackTrace_test)

ZF_NAMESPACE_GLOBAL_END

