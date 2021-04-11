#include "ZFAlgorithm_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFAlgorithm_ZFCompress_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFAlgorithm_ZFCompress_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();

        this->testCaseOutputSeparator();
        this->testCaseOutput("compress buffer");
        {
            zfblockedAlloc(ZFIOBufferByMemory, io);
            ZFCompress(io->output(), ZFInputForBufferUnsafe("uncompressed text"));
            ZFDecompress(ZFOutputDefault(), io->input());
        }

        this->testCaseOutputSeparator();
        this->testCaseOutput("compress tree");
        {
            ZFPathInfo pathInfoSrc(ZFPathType_res(), ".");
            ZFPathInfo pathInfoDst(ZFPathType_cachePath(), "ZFCompress_test");

            this->testCaseOutput("original src tree:");
            ZFFilePathInfoTreePrint(pathInfoSrc, ZFOutputDefault(), "    ");
            this->testCaseOutput("original dst tree:");
            ZFFilePathInfoTreePrint(pathInfoDst, ZFOutputDefault(), "    ");

            zfblockedAlloc(ZFIOBufferByCacheFile, io);
            ZFCompressDir(io->output(), pathInfoSrc);

            ZFDecompressDir(pathInfoDst, io->input());
            this->testCaseOutput("decompressed dst tree:");
            ZFFilePathInfoTreePrint(pathInfoDst, ZFOutputDefault(), "    ");
        }

        this->testCaseStop();
    }
};
ZFOBJECT_REGISTER(ZFAlgorithm_ZFCompress_test)

ZF_NAMESPACE_GLOBAL_END

