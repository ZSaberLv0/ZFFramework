#include "ZFAlgorithm_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFAlgorithm_ZFCrc32_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFAlgorithm_ZFCrc32_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();

        const zfchar *testString = "123abc";
        zfflags testValue = (zfflags)0x8DFE51D2; // testString's CRC32 to verify
        zfflags value = ZFCrc32ValueZero();

        value = zfCrc32Calc((const zfbyte *)testString, zfslen(testString) * sizeof(zfchar));
        this->testCaseOutput("CRC32 of array \"%s\": %x", testString, (zfuint)value);
        ZFTestCaseAssert(value == testValue);

        zfstring tmpFilePath = this->testCaseUseTmpFile("ZFCrc32_Crc32.txt");
        ZFToken fp = ZFFileFileOpen(tmpFilePath, ZFFileOpenOption::e_Write);
        if(fp != ZFTokenInvalid())
        {
            ZFFileFileWrite(fp, testString);
            ZFFileFileClose(fp);
            fp = ZFTokenInvalid();
        }
        this->testCaseOutput("write it to file %s, file's CRC32: %x",
                tmpFilePath.cString(),
                (zfuint)zfCrc32Calc(ZFInputForFile(tmpFilePath)));

        this->testCaseOutputSeparator();
        tmpFilePath = this->testCaseUseTmpFile("ZFCrc32_Crc32_big.txt");
        fp = ZFFileFileOpen(tmpFilePath, ZFFileOpenOption::e_Write);
        zfindex fileSize = 0;
        if(fp != zfnull)
        {
            for(zfindex i = 0; i < 1000; i++)
            {
                for(zfindex j = 0; j < 1000; j++)
                {
                    ZFFileFileWrite(fp, testString);
                }
            }
            fileSize = ZFFileFileTell(fp);
            ZFFileFileClose(fp);
            fp = zfnull;
        }
        ZFTimeValue tv1 = ZFTime::currentTimeValue();
        zfflags CRC32BigFile = zfCrc32Calc(ZFInputForFile(tmpFilePath.cString()));
        ZFTimeValue tv2 = ZFTimeValueDec(ZFTime::currentTimeValue(), tv1);
        this->testCaseOutput("write it 1000*1000 times to file %s, file's size: %zi, CRC32: %X, time: %s.%03s %03s",
            tmpFilePath.cString(),
            fileSize,
            (zfuint)CRC32BigFile,
            zfsFromInt(tv2.sec).cString(),
            zfsFromInt(tv2.usec / 1000).cString(),
            zfsFromInt(tv2.usec % 1000).cString());

        this->testCaseStop();
    }
};
ZFOBJECT_REGISTER(ZFAlgorithm_ZFCrc32_test)

ZF_NAMESPACE_GLOBAL_END

