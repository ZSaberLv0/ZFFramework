#include "ZFCore_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFCore_ZFFilePath_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFCore_ZFFilePath_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();

        this->testCaseOutput("ZFFilePathForModule:");
        this->testCaseOutput(ZFFilePathForModule());

        this->testCaseOutput("ZFFilePathForModuleFile:");
        this->testCaseOutput(ZFFilePathForModuleFile());

        this->testCaseOutput("ZFFilePathForSetting:");
        this->testCaseOutput(ZFFilePathForSetting());

        this->testCaseOutput("ZFFilePathForStorage:");
        this->testCaseOutput(ZFFilePathForStorage());

        this->testCaseOutput("ZFFilePathForStorageShared:");
        this->testCaseOutput(ZFFilePathForStorageShared());

        this->testCaseOutput("ZFFilePathForCache:");
        this->testCaseOutput(ZFFilePathForCache());

        this->testCaseStop();
    }
};
ZFOBJECT_REGISTER(ZFCore_ZFFilePath_test)

ZF_NAMESPACE_GLOBAL_END

