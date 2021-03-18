#include "ZFCore_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
ZFOBSERVER_EVENT_GLOBAL(ZFCore_EventFilter_test)
ZFOBSERVER_EVENT_GLOBAL_REGISTER(ZFCore_EventFilter_test)
ZF_NAMESPACE_END(ZFGlobalEvent)

zfclass ZFCore_EventFilter_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFCore_EventFilter_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();

        ZFLISTENER_LOCAL(normalObserver, {
            zfLogT() << "normal observer";
        })
        ZFGlobalEventCenter::instance()->observerAdd(
            ZFGlobalEvent::EventZFCore_EventFilter_test(),
            normalObserver);

        ZFLISTENER_LOCAL(eventFilter, {
            zfLogT() << "event filter";
            listenerData.eventFiltered(zftrue);
        })
        ZFGlobalEventCenter::instance()->observerAdd(ZFObserverAddParam()
            .eventId(ZFGlobalEvent::EventZFCore_EventFilter_test())
            .observer(eventFilter)
            .observerLevel(ZFLevelAppHigh));

        this->testCaseOutputSeparator();
        this->testCaseOutput("notify with event filter, only filter would be called");
        ZFGlobalEventCenter::instance()->observerNotify(ZFGlobalEvent::EventZFCore_EventFilter_test());

        this->testCaseOutputSeparator();
        this->testCaseOutput("notify without event filter");
        ZFGlobalEventCenter::instance()->observerRemove(
            ZFGlobalEvent::EventZFCore_EventFilter_test(),
            eventFilter);
        ZFGlobalEventCenter::instance()->observerNotify(ZFGlobalEvent::EventZFCore_EventFilter_test());

        ZFGlobalEventCenter::instance()->observerRemoveAll(ZFGlobalEvent::EventZFCore_EventFilter_test());
        this->testCaseStop();
    }
};
ZFOBJECT_REGISTER(ZFCore_EventFilter_test)

ZF_NAMESPACE_GLOBAL_END

