#include "ZFUtility_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

#if 1
ZF_GLOBAL_INITIALIZER_INIT(ZFUtility_ZFTestCase_debug_LogEvent)
{
    ZFLISTENER_LOCAL(testCaseOnOutput, {
        zfLogTrimT()
            << zfLogCurTimeString()
            << zfstringWithFormat("[%s]", listenerData.sender()->classData()->classNameFull())
            << listenerData.param0<v_zfstring *>()->zfv;
    })
    this->testCaseOnOutputListener = testCaseOnOutput;
    ZFObjectGlobalEventObserver().observerAdd(ZFTestCase::EventTestCaseOnOutput(), this->testCaseOnOutputListener);

    ZFLISTENER_LOCAL(testCaseOnStart, {
        zfLogTrimT()
            << zfLogCurTimeString()
            << zfstringWithFormat("[%s]", listenerData.sender()->classData()->classNameFull())
            << "========================== start ===========================";
    })
    this->testCaseOnStartListener = testCaseOnStart;
    ZFObjectGlobalEventObserver().observerAdd(ZFTestCase::EventTestCaseOnStart(), this->testCaseOnStartListener);

    ZFLISTENER_LOCAL(testCaseOnProgress, {
        zfLogTrimT()
            << zfLogCurTimeString()
            << zfstringWithFormat("[%s]", listenerData.sender()->classData()->classNameFull())
            << "progress updated";
    })
    this->testCaseOnProgressListener = testCaseOnProgress;
    ZFObjectGlobalEventObserver().observerAdd(ZFTestCase::EventTestCaseOnProgress(), this->testCaseOnProgressListener);

    ZFLISTENER_LOCAL(testCaseOnStop, {
        zfLogTrimT()
            << zfLogCurTimeString()
            << zfstringWithFormat("[%s]", listenerData.sender()->classData()->classNameFull())
            << "-------------------------- stop ----------------------------";
    })
    this->testCaseOnStopListener = testCaseOnStop;
    ZFObjectGlobalEventObserver().observerAdd(ZFTestCase::EventTestCaseOnStop(), this->testCaseOnStopListener);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUtility_ZFTestCase_debug_LogEvent)
{
    ZFObjectGlobalEventObserver().observerRemove(ZFTestCase::EventTestCaseOnOutput(), this->testCaseOnOutputListener);
    ZFObjectGlobalEventObserver().observerRemove(ZFTestCase::EventTestCaseOnStart(), this->testCaseOnStartListener);
    ZFObjectGlobalEventObserver().observerRemove(ZFTestCase::EventTestCaseOnProgress(), this->testCaseOnProgressListener);
    ZFObjectGlobalEventObserver().observerRemove(ZFTestCase::EventTestCaseOnStop(), this->testCaseOnStopListener);
}
private:
    ZFListener testCaseOnOutputListener;
    ZFListener testCaseOnStartListener;
    ZFListener testCaseOnProgressListener;
    ZFListener testCaseOnStopListener;
ZF_GLOBAL_INITIALIZER_END(ZFUtility_ZFTestCase_debug_LogEvent)
#endif

ZF_NAMESPACE_GLOBAL_END

