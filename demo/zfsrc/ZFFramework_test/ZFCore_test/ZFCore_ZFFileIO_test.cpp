#include "ZFCore_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFCore_ZFFileIO_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFCore_ZFFileIO_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();

        zfLogTrimT() << "============================================================";
        zfLogTrimT() << "res tree:";
        ZFFilePathInfoTreePrint(ZFPathInfo(ZFPathType_res(), "test_ZFFileIO"), ZFOutputDefault(), "  ");

        zfLogTrimT() << "try read content:";
        ZFInputReadToOutput(ZFOutputDefault(),
            ZFInputForResFile("test_ZFFileIO/fileExist"));
        ZFInputReadToOutput(ZFOutputDefault(),
            ZFInputForResFile("test_ZFFileIO/dirExist/fileExist2"));

        zfLogTrimT() << "============================================================";
        zfLogTrimT() << "copy to cache dir, tree:";
        ZFFileResCopy("test_ZFFileIO", zfstringWithFormat("%s/test_ZFFileIO", ZFFilePathForCache()));
        ZFFilePathInfoTreePrint(ZFPathInfo(ZFPathType_cachePath(), "test_ZFFileIO"), ZFOutputDefault(), "  ");

        zfLogTrimT() << "try read content:";
        ZFInputReadToOutput(ZFOutputDefault(),
            ZFInputForPathInfo(ZFPathInfo(ZFPathType_cachePath(), "test_ZFFileIO/fileExist")));
        ZFInputReadToOutput(ZFOutputDefault(),
            ZFInputForPathInfo(ZFPathInfo(ZFPathType_cachePath(), "test_ZFFileIO/dirExist/fileExist2")));

        zfLogTrimT() << "============================================================";
        zfLogTrimT() << "file type check:";

        zfLogTrimT() << "  res isExist:";
        zfLogTrimT() << "    " << ZFFileResIsExist("test_ZFFileIO/fileExist");
        zfLogTrimT() << "    " << ZFFileResIsExist("test_ZFFileIO/dirExist");
        zfLogTrimT() << "    " << ZFFileResIsExist("test_ZFFileIO/dirExist/fileExist2");
        zfLogTrimT() << "    " << !ZFFileResIsExist("test_ZFFileIO/fileNotExist");
        zfLogTrimT() << "    " << !ZFFileResIsExist("test_ZFFileIO/dirNotExist");
        zfLogTrimT() << "    " << !ZFFileResIsExist("test_ZFFileIO/dirExist/fileNotExist");

        zfLogTrimT() << "  res isDir:";
        zfLogTrimT() << "    " << !ZFFileResIsDir("test_ZFFileIO/fileExist");
        zfLogTrimT() << "    " << ZFFileResIsDir("test_ZFFileIO/dirExist");
        zfLogTrimT() << "    " << !ZFFileResIsDir("test_ZFFileIO/dirExist/fileExist2");
        zfLogTrimT() << "    " << !ZFFileResIsDir("test_ZFFileIO/fileNotExist");
        zfLogTrimT() << "    " << !ZFFileResIsDir("test_ZFFileIO/dirNotExist");
        zfLogTrimT() << "    " << !ZFFileResIsDir("test_ZFFileIO/dirExist/fileNotExist");

        zfLogTrimT() << "  file isExist:";
        zfLogTrimT() << "    " << ZFFilePathInfoIsExist(ZFPathInfo(ZFPathType_cachePath(), "test_ZFFileIO/fileExist"));
        zfLogTrimT() << "    " << ZFFilePathInfoIsExist(ZFPathInfo(ZFPathType_cachePath(), "test_ZFFileIO/dirExist"));
        zfLogTrimT() << "    " << ZFFilePathInfoIsExist(ZFPathInfo(ZFPathType_cachePath(), "test_ZFFileIO/dirExist/fileExist2"));
        zfLogTrimT() << "    " << !ZFFilePathInfoIsExist(ZFPathInfo(ZFPathType_cachePath(), "test_ZFFileIO/fileNotExist"));
        zfLogTrimT() << "    " << !ZFFilePathInfoIsExist(ZFPathInfo(ZFPathType_cachePath(), "test_ZFFileIO/dirNotExist"));
        zfLogTrimT() << "    " << !ZFFilePathInfoIsExist(ZFPathInfo(ZFPathType_cachePath(), "test_ZFFileIO/dirExist/fileNotExist"));

        zfLogTrimT() << "  file isDir:";
        zfLogTrimT() << "    " << !ZFFilePathInfoIsDir(ZFPathInfo(ZFPathType_cachePath(), "test_ZFFileIO/fileExist"));
        zfLogTrimT() << "    " << ZFFilePathInfoIsDir(ZFPathInfo(ZFPathType_cachePath(), "test_ZFFileIO/dirExist"));
        zfLogTrimT() << "    " << !ZFFilePathInfoIsDir(ZFPathInfo(ZFPathType_cachePath(), "test_ZFFileIO/dirExist/fileExist2"));
        zfLogTrimT() << "    " << !ZFFilePathInfoIsDir(ZFPathInfo(ZFPathType_cachePath(), "test_ZFFileIO/fileNotExist"));
        zfLogTrimT() << "    " << !ZFFilePathInfoIsDir(ZFPathInfo(ZFPathType_cachePath(), "test_ZFFileIO/dirNotExist"));
        zfLogTrimT() << "    " << !ZFFilePathInfoIsDir(ZFPathInfo(ZFPathType_cachePath(), "test_ZFFileIO/dirExist/fileNotExist"));

        this->testCaseStop();
    }
};
ZFOBJECT_REGISTER(ZFCore_ZFFileIO_test)

ZF_NAMESPACE_GLOBAL_END

