#include "ZFTestCaseUtil.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
ZFOBSERVER_EVENT_GLOBAL_REGISTER(TestCaseRunAllOnStart)
ZFOBSERVER_EVENT_GLOBAL_REGISTER(TestCaseRunAllOnStop)
ZF_NAMESPACE_END(ZFGlobalEvent)

// ============================================================
zfbool ZFTestCaseRun(ZF_IN const ZFClass *cls,
                     ZF_OUT_OPT ZFTestCase **testCaseHaveRun /* = zfnull */)
{
    if(testCaseHaveRun != zfnull)
    {
        *testCaseHaveRun = zfnull;
    }
    if(cls == zfnull || !cls->classIsTypeOf(ZFTestCase::ClassData()))
    {
        return zffalse;
    }
    zfautoObject testCaseTmp = cls->newInstance();
    if(testCaseTmp == zfnull || !testCaseTmp.toObject()->classData()->classIsTypeOf(ZFTestCase::ClassData()))
    {
        return zffalse;
    }
    ZFTestCase *testCase = testCaseTmp.to<ZFTestCase *>();
    testCase->testCaseStart();
    if(testCase->testCaseIsRunning())
    {
        if(testCaseHaveRun != zfnull)
        {
            *testCaseHaveRun = testCase;
        }
    }
    return zftrue;
}
zfbool ZFTestCaseRun(ZF_IN const zfchar *classNameFull,
                     ZF_OUT_OPT ZFTestCase **testCase /* = zfnull */)
{
    return ZFTestCaseRun(ZFClass::classForName(classNameFull), testCase);
}

// ============================================================
static void _ZFP_ZFTestCaseRunAllHolder_testCaseOnFinish(ZF_IN const ZFListenerData &listenerData, ZF_IN ZFObject *userData);
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFTestCaseRunAllHolder, ZFLevelZFFrameworkEssential)
{
    this->running = zffalse;
    this->testCaseRunning = zfnull;
    this->testCaseFinishListener = ZFCallbackForFunc(_ZFP_ZFTestCaseRunAllHolder_testCaseOnFinish);
}
public:
    void testCaseList(ZF_IN_OUT ZFCoreArray<const ZFClass *> &ret)
    {
        this->testCaseListPrepare();
        ret.addFrom(this->testCases);
    }
    void testCaseStart(ZF_IN const ZFCallbackT<zfbool, ZFTestCase *> *shouldStartChecker)
    {
        if(this->running)
        {
            return ;
        }
        this->running = zftrue;

        this->testCaseListPrepare();

        ZFGlobalObserver().observerNotify(ZFGlobalEvent::EventTestCaseRunAllOnStart());

        this->testCaseRunNext();
    }
    void testCaseStop(void)
    {
        if(!this->running)
        {
            return ;
        }
        ZFTestCase *toStop = this->testCaseRunning;
        this->testCases.removeAll();
        this->testCaseRunning = zfnull;
        this->running = zffalse;

        if(toStop != zfnull)
        {
            toStop->observerRemove(ZFTestCase::EventTestCaseOnStop(), this->testCaseFinishListener);
            toStop->testCaseStop(ZFResultType::e_Cancel);
        }

        ZFGlobalObserver().observerNotify(ZFGlobalEvent::EventTestCaseRunAllOnStop());
    }

    void testCaseRunNext(void)
    {
        if(!this->running)
        {
            return ;
        }

        if(this->testCaseRunning != zfnull)
        {
            this->testCaseRunning->observerRemove(ZFTestCase::EventTestCaseOnStop(), this->testCaseFinishListener);
        }

        if(this->testCases.isEmpty())
        {
            this->testCaseStop();
            return ;
        }

        const ZFClass *cls = this->testCases.getLast();
        this->testCases.removeLast();
        if(!ZFTestCaseRun(cls, &(this->testCaseRunning)) || this->testCaseRunning == zfnull)
        {
            this->testCaseRunNext();
            return ;
        }
        this->testCaseRunning->observerAdd(ZFTestCase::EventTestCaseOnStop(), this->testCaseFinishListener);
    }
private:
    zfbool running;
    ZFCoreArrayPOD<const ZFClass *> testCases;
    ZFTestCase *testCaseRunning;
    ZFListener testCaseFinishListener;
private:
    void testCaseListPrepare(void)
    {
        this->testCases.removeAll();

        ZFCoreArrayPOD<const ZFClass *> allClass = ZFClassGetAll();
        const ZFClass *desired = ZFTestCase::ClassData();
        for(zfindex i = 0; i < allClass.count(); ++i)
        {
            const ZFClass *cls = allClass[i];
            if(cls->classIsTypeOf(desired) && !cls->classIsAbstract())
            {
                this->testCases.add(cls);
            }
        }
    }
ZF_GLOBAL_INITIALIZER_END(ZFTestCaseRunAllHolder)

static void _ZFP_ZFTestCaseRunAllHolder_testCaseOnFinish(ZF_IN const ZFListenerData &listenerData, ZF_IN ZFObject *userData)
{
    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFTestCaseRunAllHolder)->testCaseRunNext();
}

void ZFTestCaseRunAllStart(ZF_IN_OPT const ZFCallbackT<zfbool, ZFTestCase *> *shouldStartChecker /* = zfnull */)
{
    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFTestCaseRunAllHolder)->testCaseStart(shouldStartChecker);
}
void ZFTestCaseRunAllStop(void)
{
    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFTestCaseRunAllHolder)->testCaseStop();
}

void ZFTestCaseGetAllT(ZF_IN_OUT ZFCoreArray<const ZFClass *> &ret)
{
    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFTestCaseRunAllHolder)->testCaseList(ret);
}

ZF_NAMESPACE_GLOBAL_END

