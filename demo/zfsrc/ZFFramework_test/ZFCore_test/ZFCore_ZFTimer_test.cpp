#include "ZFCore_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

static void _ZFP_ZFCore_ZFTimer_test_timerEvent(ZF_IN const ZFListenerData &listenerData, ZF_IN ZFObject *userData)
{
    ZFTimer *timer = listenerData.sender<ZFTimer *>();
    zfLogTrim("timer event, current thread: %s", ZFThread::currentThread()->objectInfo().cString());
    if(timer->timerActivatedCount() >= 3)
    {
        timer->timerStop();

        ZFLISTENER_LOCAL(action, {
            userData->to<ZFTestCase *>()->testCaseStop();
        })
        ZFThreadExecuteInMainThread(action, userData);
    }
}
zfclass ZFCore_ZFTimer_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFCore_ZFTimer_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFTimer);

        this->testCaseOutputSeparator();
        this->testCaseOutput("ZFTimer");
        this->testCaseOutput("current thread: %s", ZFThread::currentThread()->objectInfo().cString());

        zfblockedAlloc(ZFTimer, timer);
#if 0
        timer->timerActivateInMainThread(zftrue);
#endif
        timer->observerAdd(ZFTimer::EventTimerOnActivate(), ZFCallbackForFunc(_ZFP_ZFCore_ZFTimer_test_timerEvent), this);
        timer->timerInterval((zftimet)1000);
        timer->timerStart();
    }
};
ZFOBJECT_REGISTER(ZFCore_ZFTimer_test)

ZF_NAMESPACE_GLOBAL_END

