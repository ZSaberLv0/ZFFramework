#include "ZFUIPage.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFENUM_DEFINE(ZFUIPageResumeReason)
ZFENUM_DEFINE(ZFUIPagePauseReason)

ZFOBJECT_REGISTER(ZFUIPage)

ZFOBSERVER_EVENT_REGISTER(ZFUIPage, PageOnCreate)
ZFOBSERVER_EVENT_REGISTER(ZFUIPage, PageOnResume)
ZFOBSERVER_EVENT_REGISTER(ZFUIPage, PageOnPause)
ZFOBSERVER_EVENT_REGISTER(ZFUIPage, PageOnDestroy)

ZFOBSERVER_EVENT_REGISTER(ZFUIPage, PageAniOnPrepare)
ZFOBSERVER_EVENT_REGISTER(ZFUIPage, PageAniOnStart)
ZFOBSERVER_EVENT_REGISTER(ZFUIPage, PageAniOnStop)

ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFUIPage, ZFUIPageManager *, pageManager)
ZFMETHOD_DEFINE_0(ZFUIPage, ZFUIView *, pageView)
{
    return _ZFP_ZFUIPage_pageView;
}

ZFMETHOD_DEFINE_0(ZFUIPage, zfbool, pageCreated)
{
    return this->_ZFP_ZFUIPage_pageCreated;
}
ZFMETHOD_DEFINE_0(ZFUIPage, zfbool, pageResumed)
{
    return _ZFP_ZFUIPage_pageResumed;
}

ZFMETHOD_DEFINE_0(ZFUIPage, void, pageResume)
{
    this->pageManager()->pageResume(this);
}

ZFMETHOD_DEFINE_0(ZFUIPage, void, pageDestroy)
{
    this->pageManager()->pageDestroy(this);
}

ZFMETHOD_DEFINE_1(ZFUIPage, void, pageAni,
                  ZFMP_IN(ZFAnimation *, pageAni))
{
    zfRetainChange(this->_ZFP_ZFUIPage_pageAni, pageAni);
}
ZFMETHOD_DEFINE_0(ZFUIPage, ZFAnimation *, pageAni)
{
    return this->_ZFP_ZFUIPage_pageAni;
}

// ============================================================
ZFOBJECT_REGISTER(ZFUIPageManager)

ZFOBSERVER_EVENT_REGISTER(ZFUIPageManager, ManagerOnCreate)
ZFOBSERVER_EVENT_REGISTER(ZFUIPageManager, ManagerOnResume)
ZFOBSERVER_EVENT_REGISTER(ZFUIPageManager, ManagerOnPause)
ZFOBSERVER_EVENT_REGISTER(ZFUIPageManager, ManagerOnDestroy)
ZFOBSERVER_EVENT_REGISTER(ZFUIPageManager, ManagerUIBlockedOnChange)

// ============================================================
// _ZFP_ZFUIPageManagerPrivate
zfclassNotPOD _ZFP_ZFUIPageAniOverrideData
{
public:
    zfautoObjectT<ZFAnimation *> resumeAni;
    zfautoObjectT<ZFAnimation *> pauseAni;
    zfbool pauseAniHigherPriority;
};
zfclassNotPOD _ZFP_ZFUIPageManagerPrivate
{
public:
    ZFUIWindow *managerOwnerWindow; // no auto retain
    zfbool managerCreated;
    zfbool managerResumed;
    zfint managerUIBlocked;
    ZFUIView *managerContainer;
    ZFUIView *pageContainer;
    ZFCoreArrayPOD<ZFUIPage *> pageList;
    ZFCoreArray<_ZFP_ZFUIPageAniOverrideData> pageAniOverrideList;
    zfautoObjectT<ZFAnimation *> resumeAni;
    zfautoObjectT<ZFAnimation *> pauseAni;
    zfint pageMoveFlag;
    ZFUIPage *pageMoveLastResumePage;
    ZFCoreArray<ZFListener> pageRequestQueue;
    ZFCoreArray<zfautoObject> pageRequestUserDataQueue;
    zfbool pageRequestRunningFlag;
public:
    _ZFP_ZFUIPageManagerPrivate(void)
    : managerOwnerWindow(zfnull)
    , managerCreated(zffalse)
    , managerResumed(zffalse)
    , managerUIBlocked(0)
    , managerContainer(zfnull)
    , pageContainer(zfnull)
    , pageList()
    , pageAniOverrideList()
    , resumeAni()
    , pauseAni()
    , pageMoveFlag(0)
    , pageMoveLastResumePage(zfnull)
    , pageRequestQueue()
    , pageRequestUserDataQueue()
    , pageRequestRunningFlag(zffalse)
    {
    }

public:
    void pageRequestAdd(ZF_IN ZFUIPageManager *owner,
                        ZF_IN const ZFListener &callback,
                        ZF_IN_OPT ZFObject *userData = zfnull)
    {
        if(this->pageRequestRunningFlag)
        {
            this->pageRequestQueue.add(callback);
            this->pageRequestUserDataQueue.add(userData);
            return;
        }
        if(this->pageRequestQueue.isEmpty())
        {
            this->pageRequestRunningFlag = zftrue;
            callback.execute(ZFListenerData(zfidentityInvalid(), owner), userData);
            this->pageRequestRunningFlag = zffalse;
        }
        this->pageRequestResolve(owner);
    }
    void pageRequestResolve(ZF_IN ZFUIPageManager *owner)
    {
        while(!this->pageRequestQueue.isEmpty())
        {
            ZFListener callback = this->pageRequestQueue[0];
            zfautoObject userData = this->pageRequestUserDataQueue[0];
            this->pageRequestQueue.remove(0);
            this->pageRequestUserDataQueue.remove(0);
            this->pageRequestRunningFlag = zftrue;
            callback.execute(ZFListenerData(zfidentityInvalid(), owner), userData);
            this->pageRequestRunningFlag = zffalse;
        }
    }

public:
    static void pageOnCreate(ZF_IN ZFUIPage *page)
    {
        zfRetain(page);
        page->pageOnCreate();
        page->observerNotify(ZFUIPage::EventPageOnCreate());
        page->pageManager()->observerNotifyWithCustomSender(page, ZFUIPage::EventPageOnCreate());
    }
    static void pageOnResume(ZF_IN ZFUIPage *page, ZF_IN ZFUIPageResumeReasonEnum resumeReason, ZF_IN ZFUIPage *siblingPage)
    {
        page->pageOnResume(resumeReason);
        zfblockedAlloc(ZFUIPageResumeReason, resumeReasonHolder, resumeReason);
        page->observerNotify(ZFUIPage::EventPageOnResume(), resumeReasonHolder, siblingPage);
        page->pageManager()->observerNotifyWithCustomSender(page, ZFUIPage::EventPageOnResume(), resumeReasonHolder, siblingPage);
    }
    static void pageOnPause(ZF_IN ZFUIPage *page, ZF_IN ZFUIPagePauseReasonEnum pauseReason, ZF_IN ZFUIPage *siblingPage)
    {
        zfblockedAlloc(ZFUIPagePauseReason, pauseReasonHolder, pauseReason);
        page->pageManager()->observerNotifyWithCustomSender(page, ZFUIPage::EventPageOnPause(), pauseReasonHolder, siblingPage);
        page->observerNotify(ZFUIPage::EventPageOnPause(), pauseReasonHolder, siblingPage);
        page->pageOnPause(pauseReason);
    }
    static void pageOnDestroy(ZF_IN ZFUIPage *page)
    {
        page->pageManager()->observerNotifyWithCustomSender(page, ZFUIPage::EventPageOnDestroy());
        page->observerNotify(ZFUIPage::EventPageOnDestroy());
        page->pageOnDestroy();
        zfRelease(page);
    }

    static void pageAniOnPrepare(ZF_IN ZFUIPage *page, ZF_IN ZFEnum *resumeOrPauseReason, ZF_IN ZFUIPage *siblingPage)
    {
        page->pageAniOnPrepare(resumeOrPauseReason, siblingPage);
        page->observerNotify(ZFUIPage::EventPageAniOnPrepare(), resumeOrPauseReason, siblingPage);
        page->pageManager()->observerNotifyWithCustomSender(page, ZFUIPage::EventPageAniOnPrepare(), resumeOrPauseReason, siblingPage);
    }

public:
    static void pageAniUpdate(ZF_IN ZFUIPage *resumePage, ZF_IN ZFUIPageResumeReasonEnum resumeReason,
                              ZF_IN ZFUIPage *pausePage, ZF_IN ZFUIPagePauseReasonEnum pauseReason)
    {
        ZFUIPageManager *manager = zfnull;
        if(resumePage != zfnull)
        {
            resumePage->pageAni(zfnull);
            manager = resumePage->pageManager();
        }
        if(pausePage != zfnull)
        {
            pausePage->pageAni(zfnull);
            manager = pausePage->pageManager();
        }
        if(resumePage != zfnull && resumePage->pageView()->viewParent() == zfnull)
        {
            resumePage->pageManager()->pageContainer()->childAdd(resumePage->pageView(), ZFUISizeParamFillFill());
        }

        if(manager->d->resumeAni != zfnull)
        {
            manager->d->resumeAni->aniStop();
        }
        if(manager->d->pauseAni != zfnull)
        {
            manager->d->pauseAni->aniStop();
        }

        if(!manager->d->pageAniOverrideList.isEmpty())
        {
            manager->d->resumeAni = manager->d->pageAniOverrideList[0].resumeAni;
            manager->d->pauseAni = manager->d->pageAniOverrideList[0].pauseAni;
            if(manager->d->pageAniOverrideList[0].pauseAniHigherPriority)
            {
                if(pausePage != zfnull)
                {
                    manager->pageContainer()->childMove(pausePage->pageView(), zfindexMax());
                }
            }
            manager->d->pageAniOverrideList.removeFirst();
            if(pausePage == zfnull)
            {
                manager->d->pauseAni = zfnull;
            }
            if(resumePage == zfnull)
            {
                manager->d->resumeAni = zfnull;
            }
        }
        else
        {
            switch(resumeReason)
            {
                case ZFUIPageResumeReason::e_ByManagerResume:
                    break;
                case ZFUIPageResumeReason::e_ByRequest:
                    if(resumePage == zfnull || (pausePage != zfnull && pausePage->pageAniPriority() > resumePage->pageAniPriority()))
                    {
                        pageAniOnPrepare(pausePage, zflineAlloc(ZFUIPagePauseReason, pauseReason), resumePage);
                        manager->pageContainer()->childMove(pausePage->pageView(), zfindexMax());
                    }
                    else
                    {
                        pageAniOnPrepare(resumePage, zflineAlloc(ZFUIPageResumeReason, resumeReason), pausePage);
                    }
                    break;
                case ZFUIPageResumeReason::e_FromBackground:
                    if(pausePage == zfnull || (resumePage != zfnull && resumePage->pageAniPriority() > pausePage->pageAniPriority()))
                    {
                        pageAniOnPrepare(resumePage, zflineAlloc(ZFUIPageResumeReason, resumeReason), pausePage);
                    }
                    else
                    {
                        pageAniOnPrepare(pausePage, zflineAlloc(ZFUIPagePauseReason, pauseReason), resumePage);
                        manager->pageContainer()->childMove(pausePage->pageView(), zfindexMax());
                    }
                    break;
                default:
                    zfCoreCriticalShouldNotGoHere();
                    break;
            }

            manager->d->resumeAni = resumePage ? resumePage->pageAni() : zfnull;
            manager->d->pauseAni = pausePage ? pausePage->pageAni() : zfnull;
        }
        zfCoreAssertWithMessageTrim(!(manager->d->resumeAni != zfnull && manager->d->resumeAni == manager->d->pauseAni),
            "[ZFUIPage] same ani instance is not allowed");

        pageAniOnStart(manager, resumePage, resumeReason, pausePage, pauseReason);
        if(manager->d->pauseAni == zfnull && manager->d->resumeAni == zfnull)
        {
            pageAniOnStop(manager, resumePage, resumeReason, pausePage, pauseReason);
            return;
        }

        if(pausePage != zfnull)
        {
            if(manager->d->pauseAni != zfnull)
            {
                manager->d->pauseAni->aniTarget(pausePage->pageView());
                ZFLISTENER_LAMBDA_4(aniOnStop
                    , ZFUIPage *, pausePage
                    , ZFUIPagePauseReasonEnum, pauseReason
                    , ZFUIPage *, resumePage
                    , ZFUIPageResumeReasonEnum, resumeReason
                , {
                    pausePage->pageManager()->d->pauseAni = zfnull;
                    pageAniOnStop(pausePage->pageManager(), resumePage, resumeReason, pausePage, pauseReason);
                })
                manager->d->pauseAni->observerAddForOnce(
                    ZFAnimation::EventAniOnStopOrInvalid(),
                    aniOnStop);
                manager->d->pauseAni->aniStart();
            }
        }

        if(manager->d->resumeAni != zfnull)
        {
            manager->d->resumeAni->aniTarget(resumePage->pageView());
            ZFLISTENER_LAMBDA_4(aniOnStop
                , ZFUIPage *, resumePage
                , ZFUIPageResumeReasonEnum, resumeReason
                , ZFUIPage *, pausePage
                , ZFUIPagePauseReasonEnum, pauseReason
            , {
                resumePage->pageManager()->d->resumeAni = zfnull;
                pageAniOnStop(resumePage->pageManager(), resumePage, resumeReason, pausePage, pauseReason);
            })
            manager->d->resumeAni->observerAddForOnce(
                ZFAnimation::EventAniOnStopOrInvalid(),
                aniOnStop);
            manager->d->resumeAni->aniStart();
        }
    }
    static void pageAniOnStart(ZF_IN ZFUIPageManager *manager,
                               ZF_IN ZFUIPage *resumePage, ZF_IN ZFUIPageResumeReasonEnum resumeReason,
                               ZF_IN ZFUIPage *pausePage, ZF_IN ZFUIPagePauseReasonEnum pauseReason)
    {
        if(pausePage != zfnull)
        {
            zfblockedAlloc(ZFUIPagePauseReason, pauseReasonHolder, pauseReason);
            pausePage->observerNotify(ZFUIPage::EventPageAniOnStart(), pauseReasonHolder);
            manager->observerNotifyWithCustomSender(pausePage, ZFUIPage::EventPageAniOnStart(), pauseReasonHolder);
        }
        if(resumePage != zfnull)
        {
            zfblockedAlloc(ZFUIPageResumeReason, resumeReasonHolder, resumeReason);
            resumePage->observerNotify(ZFUIPage::EventPageAniOnStart(), resumeReasonHolder);
            manager->observerNotifyWithCustomSender(resumePage, ZFUIPage::EventPageAniOnStart(), resumeReasonHolder);
        }
    }
    static void pageAniOnStop(ZF_IN ZFUIPageManager *manager,
                              ZF_IN ZFUIPage *resumePage, ZF_IN ZFUIPageResumeReasonEnum resumeReason,
                              ZF_IN ZFUIPage *pausePage, ZF_IN ZFUIPagePauseReasonEnum pauseReason)
    {
        if(manager->d->resumeAni != zfnull || manager->d->pauseAni != zfnull)
        {
            return;
        }
        if(pausePage != zfnull)
        {
            manager->pageContainer()->childRemove(pausePage->pageView());

            zfblockedAlloc(ZFUIPagePauseReason, pauseReasonHolder, pauseReason);
            pausePage->observerNotify(ZFUIPage::EventPageAniOnStop(), pauseReasonHolder);
            manager->observerNotifyWithCustomSender(pausePage, ZFUIPage::EventPageAniOnStop(), pauseReasonHolder);
        }
        if(resumePage != zfnull)
        {
            zfblockedAlloc(ZFUIPageResumeReason, resumeReasonHolder, resumeReason);
            resumePage->observerNotify(ZFUIPage::EventPageAniOnStop(), resumeReasonHolder);
            manager->observerNotifyWithCustomSender(resumePage, ZFUIPage::EventPageAniOnStop(), resumeReasonHolder);
        }
        if(pausePage != zfnull)
        {
            if(pauseReason == ZFUIPagePauseReason::e_BeforeDestroy)
            {
                pageOnDestroy(pausePage);
            }
        }
    }
};

void ZFUIPageManager::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFUIPageManagerPrivate);
}
void ZFUIPageManager::objectOnDealloc(void)
{
    zflockfree_zfRetainChange(d->pageContainer, zfnull);
    zflockfree_zfRetainChange(d->managerContainer, zfnull);
    zfpoolDelete(d);
    zfsuper::objectOnDealloc();
}

void ZFUIPageManager::managerOnCreate(void)
{
    zfautoObjectT<ZFUIView *> managerContainer = (this->managerContainerClass() != zfnull ? this->managerContainerClass() : ZFUIView::ClassData())->newInstance();
    zfautoObjectT<ZFUIView *> pageContainer = (this->pageContainerClass() != zfnull ? this->pageContainerClass() : ZFUIView::ClassData())->newInstance();
    d->managerContainer = managerContainer;
    d->pageContainer = pageContainer;
    zfCoreAssertWithMessage(d->managerContainer != zfnull, "managerContainerClass must be type of %s", ZFUIView::ClassData()->className());
    zfCoreAssertWithMessage(d->pageContainer != zfnull, "pageContainerClass must be type of %s", ZFUIView::ClassData()->className());
    zfRetain(d->managerContainer);
    zfRetain(d->pageContainer);

    d->managerContainer->childAdd(d->pageContainer, ZFUISizeParamFillFill());
}
void ZFUIPageManager::managerOnDestroy(void)
{
    zflockfree_zfRetainChange(d->pageContainer, zfnull);
    zflockfree_zfRetainChange(d->managerContainer, zfnull);
}

// ============================================================
// manager creation
ZFMETHOD_DEFINE_0(ZFUIPageManager, void, managerCreate)
{
    zfCoreAssert(!d->managerCreated);
    zfRetain(this);
    this->managerOnCreate();
    d->managerCreated = zftrue;
    this->observerNotify(ZFUIPageManager::EventManagerOnCreate());
}
ZFMETHOD_DEFINE_0(ZFUIPageManager, void, managerResume)
{
    zfCoreAssert(d->managerCreated);
    zfCoreAssert(!d->managerResumed);
    this->managerOnResume();
    d->managerResumed = zftrue;
    this->observerNotify(ZFUIPageManager::EventManagerOnResume());
}
ZFMETHOD_DEFINE_0(ZFUIPageManager, void, managerPause)
{
    zfCoreAssert(d->managerResumed);
    this->managerOnPause();
    d->managerResumed = zffalse;
    this->observerNotify(ZFUIPageManager::EventManagerOnPause());
}
ZFMETHOD_DEFINE_0(ZFUIPageManager, void, managerDestroy)
{
    zfCoreAssert(d->managerCreated);
    if(d->managerResumed)
    {
        this->managerPause();
    }

    d->pageRequestResolve(this);

    if(d->managerOwnerWindow != zfnull)
    {
        ZFUIWindow *managerOwnerWindowTmp = d->managerOwnerWindow;
        d->managerOwnerWindow = zfnull;
        managerOwnerWindowTmp->observerRemoveByOwner(this);
        if(managerOwnerWindowTmp->windowShowing())
        {
            managerOwnerWindowTmp->windowHide();
        }
    }

    this->managerOnDestroy();
    d->managerCreated = zffalse;
    this->observerNotify(ZFUIPageManager::EventManagerOnDestroy());
    zfRelease(this);
}

ZFMETHOD_DEFINE_0(ZFUIPageManager, zfbool, managerCreated)
{
    return d->managerCreated;
}
ZFMETHOD_DEFINE_0(ZFUIPageManager, zfbool, managerResumed)
{
    return d->managerResumed;
}

ZFMETHOD_DEFINE_0(ZFUIPageManager, ZFUIWindow *, managerCreateForWindow)
{
    zfblockedAlloc(ZFUIWindow, window);
    d->managerOwnerWindow = window;

    this->managerCreate();
    window->childAdd(this->managerContainer(), ZFUISizeParamFillFill());

    ZFLISTENER_LOCAL(onShow, {
        ZFUIPageManager *t = userData->objectHolded();
        if(!t->managerResumed())
        {
            t->managerResume();
        }
    })
    window->observerAdd(ZFUIWindow::EventWindowOwnerSysWindowOnResume(), onShow, this->objectHolder(), this);

    ZFLISTENER_LOCAL(onHide, {
        ZFUIPageManager *t = userData->objectHolded();
        if(t->managerResumed())
        {
            t->managerOnPause();
        }
    })
    window->observerAdd(ZFUIWindow::EventWindowOwnerSysWindowOnPause(), onHide, this->objectHolder(), this);

    ZFLISTENER_LOCAL(onDestroy, {
        ZFUIPageManager *t = userData->objectHolded();
        t->managerDestroy();
    })
    window->observerAdd(ZFUIWindow::EventObjectBeforeDealloc(), onDestroy, this->objectHolder(), this);

    window->windowShow();
    if(window->windowOwnerSysWindow()->nativeWindowIsResumed())
    {
        this->managerResume();
    }
    return d->managerOwnerWindow;
}
ZFMETHOD_DEFINE_0(ZFUIPageManager, ZFUIWindow *, managerOwnerWindow)
{
    return d->managerOwnerWindow;
}

// ============================================================
// container
ZFMETHOD_DEFINE_0(ZFUIPageManager, ZFUIView *, managerContainer)
{
    return d->managerContainer;
}
ZFMETHOD_DEFINE_0(ZFUIPageManager, ZFUIView *, pageContainer)
{
    return d->pageContainer;
}

// ============================================================
// manager control
ZFMETHOD_DEFINE_1(ZFUIPageManager, void, managerUIBlocked,
                  ZFMP_IN(zfbool, value))
{
    zfsynchronize(this);
    if(value)
    {
        ++d->managerUIBlocked;
        if(d->managerUIBlocked == 1)
        {
            this->managerUIBlockedOnChange();
            this->observerNotify(ZFUIPageManager::EventManagerUIBlockedOnChange());
        }
    }
    else
    {
        --d->managerUIBlocked;
        if(d->managerUIBlocked == 0)
        {
            this->managerUIBlockedOnChange();
            this->observerNotify(ZFUIPageManager::EventManagerUIBlockedOnChange());
        }
    }
}
ZFMETHOD_DEFINE_0(ZFUIPageManager, zfbool, managerUIBlocked)
{
    return d->managerUIBlocked != 0;
}
ZFMETHOD_DEFINE_0(ZFUIPageManager, zfindex, managerUIBlockedCount)
{
    return (zfindex)d->managerUIBlocked;
}

// ============================================================
// page access
ZFMETHOD_DEFINE_0(ZFUIPageManager, zfindex, pageCount)
{
    return this->pageList().count();
}
ZFMETHOD_DEFINE_1(ZFUIPageManager, ZFUIPage *, pageAtIndex,
                  ZFMP_IN(zfindex, index))
{
    return this->pageList().get(index);
}
ZFMETHOD_DEFINE_0(ZFUIPageManager, ZFUIPage *, pageForeground)
{
    if(!this->pageList().isEmpty())
    {
        return this->pageList().getLast();
    }
    return zfnull;
}
ZFMETHOD_DEFINE_1(ZFUIPageManager, zfindex, pageIndex,
                  ZFMP_IN(ZFUIPage *, page))
{
    return this->pageList().find(page);
}

ZFMETHOD_DEFINE_0(ZFUIPageManager, ZFCoreArrayPOD<ZFUIPage *> &, pageList)
{
    return d->pageList;
}

ZFMETHOD_DEFINE_1(ZFUIPageManager, ZFCoreArrayPOD<ZFUIPage *>, pageListForGroupId,
                  ZFMP_IN(const zfchar *, pageGroupId))
{
    ZFCoreArrayPOD<ZFUIPage *> ret;
    for(zfindex i = 0; i < d->pageList.count(); ++i)
    {
        if(zfscmpTheSame(d->pageList[i]->pageGroupId(), pageGroupId))
        {
            ret.add(d->pageList[i]);
        }
    }
    return ret;
}

// ============================================================
// page management
ZFMETHOD_DEFINE_3(ZFUIPageManager, void, pageAniOverride,
                  ZFMP_IN(ZFAnimation *, resumeAni),
                  ZFMP_IN(ZFAnimation *, pauseAni),
                  ZFMP_IN_OPT(zfbool, pauseAniHigherPriority, zffalse))
{
    _ZFP_ZFUIPageAniOverrideData data;
    data.resumeAni = resumeAni;
    data.pauseAni = pauseAni;
    data.pauseAniHigherPriority = pauseAniHigherPriority;
    d->pageAniOverrideList.add(data);
}

ZFMETHOD_DEFINE_1(ZFUIPageManager, void, pageCreate,
                  ZFMP_IN(ZFUIPage *, page))
{
    if(page == zfnull)
    {
        return;
    }

    ZFUIPageManager *pm = this;
    ZFLISTENER_LAMBDA_2(callback
        , ZFUIPageManager *, pm
        , ZFUIPage *, page
        , {
            zfCoreAssert(page->_ZFP_ZFUIPage_pageManager == zfnull);
            zfCoreAssert(pm->d->pageMoveFlag == 0);

            ZFUIPage *resumePage = page;
            ZFUIPage *pausePage = pm->pageForeground();

            pm->d->pageList.add(resumePage);
            resumePage->_ZFP_ZFUIPage_pageManager = pm;

            if(pausePage != zfnull)
            {
                _ZFP_ZFUIPageManagerPrivate::pageOnPause(pausePage, ZFUIPagePauseReason::e_ToBackground, resumePage);
            }
            _ZFP_ZFUIPageManagerPrivate::pageOnCreate(resumePage);
            _ZFP_ZFUIPageManagerPrivate::pageOnResume(resumePage, ZFUIPageResumeReason::e_ByRequest, pausePage);
            _ZFP_ZFUIPageManagerPrivate::pageAniUpdate(
                resumePage, ZFUIPageResumeReason::e_ByRequest,
                pausePage, ZFUIPagePauseReason::e_ToBackground);
        })
    d->pageRequestAdd(this, callback);
}

ZFMETHOD_DEFINE_1(ZFUIPageManager, void, pageResume,
                  ZFMP_IN(ZFUIPage *, page))
{
    ZFUIPageManager *pm = this;
    ZFLISTENER_LAMBDA_2(callback
        , ZFUIPageManager *, pm
        , ZFUIPage *, page
        , {
            pm->pageResume(pm->pageIndex(page));
        })
    d->pageRequestAdd(this, callback);
}
ZFMETHOD_DEFINE_1(ZFUIPageManager, void, pageResume,
                  ZFMP_IN(zfindex, pageIndex))
{
    ZFUIPageManager *pm = this;
    ZFLISTENER_LAMBDA_2(callback
        , ZFUIPageManager *, pm
        , zfindex, pageIndex
        , {
            if(pageIndex < pm->pageCount())
            {
                pm->pageMoveBegin();
                pm->d->pageList.move(pageIndex, zfindexMax());
                pm->pageMoveEnd();
            }
        })
    d->pageRequestAdd(this, callback);
}
ZFMETHOD_DEFINE_1(ZFUIPageManager, void, pageResumeForGroupId,
                  ZFMP_IN(const zfchar *, pageGroupId))
{
    ZFUIPageManager *pm = this;
    ZFLISTENER_LAMBDA_2(callback
        , ZFUIPageManager *, pm
        , zfstring, pageGroupId
        , {
            pm->pageMoveBegin();
            zfindex moveTo = pm->d->pageList.count() - 1;
            for(zfindex i = pm->d->pageList.count() - 1; i != zfindexMax(); --i)
            {
                if(zfscmpTheSame(pm->d->pageList[i]->pageGroupId(), pageGroupId))
                {
                    pm->d->pageList.move(i, moveTo);
                    --moveTo;
                }
            }
            pm->pageMoveEnd();
        })
    d->pageRequestAdd(this, callback);
}

ZFMETHOD_DEFINE_1(ZFUIPageManager, void, pageDestroy,
                  ZFMP_IN(ZFUIPage *, page))
{
    ZFUIPageManager *pm = this;
    ZFLISTENER_LAMBDA_2(callback
        , ZFUIPageManager *, pm
        , ZFUIPage *, page
        , {
            pm->pageDestroy(pm->pageIndex(page));
        })
    d->pageRequestAdd(this, callback);
}
ZFMETHOD_DEFINE_1(ZFUIPageManager, void, pageDestroy,
                  ZFMP_IN(zfindex, pageIndex))
{
    ZFUIPageManager *pm = this;
    ZFLISTENER_LAMBDA_2(callback
        , ZFUIPageManager *, pm
        , zfindex, pageIndex
        , {
            zfCoreAssert(pm->d->pageMoveFlag == 0);
            if(pageIndex >= pm->d->pageList.count())
            {
                return;
            }
            if(pageIndex < pm->d->pageList.count() - 1)
            {
                ZFUIPage *pausePage = pm->d->pageList.removeAndGet(pageIndex);
                _ZFP_ZFUIPageManagerPrivate::pageOnPause(pausePage, ZFUIPagePauseReason::e_BeforeDestroy, zfnull);
                _ZFP_ZFUIPageManagerPrivate::pageOnDestroy(pausePage);
                if(!pm->d->pageAniOverrideList.isEmpty())
                {
                    pm->d->pageAniOverrideList.removeFirst();
                }
                return;
            }

            ZFUIPage *pausePage = pm->d->pageList.removeAndGet(pageIndex);
            ZFUIPage *resumePage = pm->d->pageList.isEmpty() ? zfnull : pm->d->pageList.getLast();
            _ZFP_ZFUIPageManagerPrivate::pageOnPause(pausePage, ZFUIPagePauseReason::e_BeforeDestroy, resumePage);

            if(resumePage != zfnull)
            {
                _ZFP_ZFUIPageManagerPrivate::pageOnResume(resumePage, ZFUIPageResumeReason::e_FromBackground, pausePage);
            }

            _ZFP_ZFUIPageManagerPrivate::pageAniUpdate(
                resumePage, ZFUIPageResumeReason::e_FromBackground,
                pausePage, ZFUIPagePauseReason::e_BeforeDestroy);
        })
    d->pageRequestAdd(this, callback);
}

ZFMETHOD_DEFINE_0(ZFUIPageManager, void, pageMoveBegin)
{
    ++d->pageMoveFlag;
    if(d->pageMoveFlag == 1)
    {
        d->pageMoveLastResumePage = d->pageList.isEmpty() ? zfnull : d->pageList.getLast();
    }
}
ZFMETHOD_DEFINE_0(ZFUIPageManager, void, pageMoveEnd)
{
    --d->pageMoveFlag;
    if(d->pageMoveFlag != 0 || d->pageMoveLastResumePage == zfnull || d->pageMoveLastResumePage == d->pageList.getLast())
    {
        return ;
    }
    ZFUIPage *resumePage = d->pageList.getLast();
    ZFUIPage *pausePage = d->pageMoveLastResumePage;
    d->pageMoveLastResumePage = zfnull;

    _ZFP_ZFUIPageManagerPrivate::pageOnPause(pausePage, ZFUIPagePauseReason::e_ToBackground, resumePage);
    _ZFP_ZFUIPageManagerPrivate::pageOnResume(resumePage, ZFUIPageResumeReason::e_ByRequest, pausePage);
    _ZFP_ZFUIPageManagerPrivate::pageAniUpdate(
        resumePage, ZFUIPageResumeReason::e_ByRequest,
        pausePage, ZFUIPagePauseReason::e_ToBackground);
}

ZFMETHOD_DEFINE_2(ZFUIPageManager, void, pageRequest,
                  ZFMP_IN(const ZFListener &, callback),
                  ZFMP_IN_OPT(ZFObject *, userData, zfnull))
{
    if(callback.callbackIsValid())
    {
        d->pageRequestAdd(this, callback, userData);
    }
}
ZFMETHOD_DEFINE_2(ZFUIPageManager, void, pageRequestCancel,
                  ZFMP_IN(const ZFListener &, callback),
                  ZFMP_IN_OPT(ZFObject *, userData, zfnull))
{
    for(zfindex i = 0; i < d->pageRequestQueue.count(); ++i)
    {
        if(d->pageRequestQueue[i] == callback && d->pageRequestUserDataQueue[i] == userData)
        {
            d->pageRequestQueue.remove(i);
            d->pageRequestUserDataQueue.remove(i);
            break;
        }
    }
}

ZF_NAMESPACE_GLOBAL_END

