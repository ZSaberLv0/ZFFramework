#include "ZFCore_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclass ZFCore_ZFThread_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFCore_ZFThread_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFThread);

        zfLogTrim("============================================================");
        zfLogTrim("ZFThread");
        zfidentity taskId = zfidentityInvalid();

#if 1
        zfLogTrim("execute main and thread without lock (async):");
        ZFLISTENER_LOCAL(asyncFunc, {
            zfLogTrim("async thread begin");
            for(zfindex i = 0; i < 5; ++i)
            {
                for(zfindex j = 0; j < 10; ++j)
                {
                    zfLogTrim("  async thread: %zi %zi", i, j);
                    ZFThread::sleep((zftimet)20);
                }
                ZFThread::sleep((zftimet)300);
            }
            zfLogTrim("async thread end");
        })
        taskId = ZFExecuteInNewThread(asyncFunc);
        for(zfindex i = 0; i < 5; ++i)
        {
            for(zfindex j = 0; j < 10; ++j)
            {
                zfLogTrim("  main: %zi %zi", i, j);
                ZFThread::sleep((zftimet)20);
            }
            ZFThread::sleep((zftimet)190);
        }
        zfLogTrim("main thread wait async thread begin");
        ZFExecuteWait(taskId);
        zfLogTrim("main thread wait async thread complete");
#endif

#if 1
        zfLogTrim("============================================================");
        zfLogTrim("execute main and thread with lock (sync):");
        ZFLISTENER_LOCAL(syncFunc, {
            zfLogTrim("sync thread begin");
            for(zfindex i = 0; i < 5; ++i)
            {
                zfCoreMutexLock();
                for(zfindex j = 0; j < 10; ++j)
                {
                    zfLogTrim("  sync thread: %zi %zi", i, j);
                    ZFThread::sleep((zftimet)20);
                }
                zfCoreMutexUnlock();
                ZFThread::sleep((zftimet)200);
            }
            zfLogTrim("sync thread end");
        })
        taskId = ZFExecuteInNewThread(syncFunc);
        for(zfindex i = 0; i < 5; ++i)
        {
            zfCoreMutexLock();
            for(zfindex j = 0; j < 10; ++j)
            {
                zfLogTrim("  main:   %zi %zi", i, j);
                ZFThread::sleep((zftimet)20);
            }
            zfCoreMutexUnlock();
            ZFThread::sleep((zftimet)190);
        }
        zfLogTrim("main thread wait sync thread begin");
        ZFExecuteWait(taskId);
        zfLogTrim("main thread wait sync thread complete");
#endif

        this->testCaseStop();
    }
};
ZFOBJECT_REGISTER(ZFCore_ZFThread_test)

ZF_NAMESPACE_GLOBAL_END

