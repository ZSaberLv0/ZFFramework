/**
 * @file ZFUIPage.h
 * @brief abstract page manage logic
 */

#ifndef _ZFI_ZFUIPage_h_
#define _ZFI_ZFUIPage_h_

#include "ZFUIWidgetDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief page resume reason
 */
ZFENUM_BEGIN(ZFUIPageResumeReason)
    ZFENUM_VALUE(ByManagerResume)
    ZFENUM_VALUE(ByRequest)
    ZFENUM_VALUE(FromBackground)
ZFENUM_SEPARATOR(ZFUIPageResumeReason)
    ZFENUM_VALUE_REGISTER(ByManagerResume)
    ZFENUM_VALUE_REGISTER(ByRequest)
    ZFENUM_VALUE_REGISTER(FromBackground)
ZFENUM_END(ZFUIPageResumeReason)

/**
 * @brief page pause reason
 */
ZFENUM_BEGIN(ZFUIPagePauseReason)
    ZFENUM_VALUE(ByManagerPause)
    ZFENUM_VALUE(ToBackground)
    ZFENUM_VALUE(BeforeDestroy)
ZFENUM_SEPARATOR(ZFUIPagePauseReason)
    ZFENUM_VALUE_REGISTER(ByManagerPause)
    ZFENUM_VALUE_REGISTER(ToBackground)
    ZFENUM_VALUE_REGISTER(BeforeDestroy)
ZFENUM_END(ZFUIPagePauseReason)

// ============================================================
zfclassFwd ZFUIPageManager;
/**
 * @brief abstract page similar to Android's Activity
 *
 * a page is an abstract object with those life cycle:
 * -  #ZFUIPage::pageOnCreate, create necessary contents
 * -  #ZFUIPage::pageOnResume, before making the page active
 * -  #ZFUIPage::pageOnPause, before making the page deactive
 * -  #ZFUIPage::pageOnDestroy, to destroy the page
 *
 * and a #ZFUIPageManager is the holder to manage multiple pages\n
 * typical usage:
 * @code
 *   zfblockedAlloc(ZFUIPageManager, pm);
 *   pm->observerAdd(ZFUIPageManager::EventManagerOnCreate, myPMCreateCallback);
 *   pm->managerCreateForWindow();
 *   ZFUIPageManager::instance(pm);
 *
 *   zfblockedAlloc(MyCustomPage, page1);
 *   ZFUIPageManager::instance()->pageCreate(page1);
 *   ...
 *   zfblockedAlloc(ZFUIPage, page2);
 *   page2->observerAdd(ZFUIPage::EventPageOnCreate(), myCreateCallback);
 *   ZFUIPageManager::instance()->pageCreate(page2);
 *   ...
 *   page1->pageDestroy();
 * @endcode
 * at most one page would being resumed,
 * when new page created,
 * the old one would be paused to background,
 * with a page switch animation implemented by #pageAniOnPrepare\n
 * \n
 * to implement your own page, either:
 * -  inherit #ZFUIPage and override #pageOnCreate series,
 *   construct your page view and add to #pageView
 * -  alloc a ZFUIPage, #ZFObject::observerAdd to #EventPageOnCreate,
 *   and perform additional page create steps
 */
zfclass ZF_ENV_EXPORT ZFUIPage : zfextends ZFObject
{
    ZFOBJECT_DECLARE_WITH_CUSTOM_CTOR(ZFUIPage, ZFObject)

    // ============================================================
    // observers
public:
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when page created\n
     * this event would also be fired to it's owner #pageManager
     */
    ZFOBSERVER_EVENT(PageOnCreate)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when page resumed,
     * param0 is #ZFUIPageResumeReason,
     * param1 is the sibling pause page or null\n
     * this event would also be fired to it's owner #pageManager
     */
    ZFOBSERVER_EVENT(PageOnResume)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when page resumed,
     * param0 is #ZFUIPagePauseReason,
     * param1 is the sibling resume page or null\n
     * this event would also be fired to it's owner #pageManager
     */
    ZFOBSERVER_EVENT(PageOnPause)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when page destroyed\n
     * this event would also be fired to it's owner #pageManager
     */
    ZFOBSERVER_EVENT(PageOnDestroy)

    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when this page resume or pause (only called to page who has higher #pageAniPriority),
     * to prepare #pageAni for this page and sibling page,
     * param0 is the #ZFUIPageResumeReason or #ZFUIPagePauseReason,
     * param1 is the sibling page\n
     * see #pageAniOnPrepare for more info\n
     * this event would also be fired to it's owner #pageManager
     */
    ZFOBSERVER_EVENT(PageAniOnPrepare)

    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when page ani start,
     * called once each for pause and resume page,
     * even if no actual animation to start,
     * param0 is the #ZFUIPagePauseReason/#ZFUIPageResumeReason\n
     * this event would also be fired to it's owner #pageManager
     */
    ZFOBSERVER_EVENT(PageAniOnStart)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when page ani start,
     * called once each for pause and resume page,
     * even if no actual animation to start,
     * param0 is the #ZFUIPagePauseReason/#ZFUIPageResumeReason\n
     * this event would also be fired to it's owner #pageManager
     */
    ZFOBSERVER_EVENT(PageAniOnStop)

    // ============================================================
    // page manager
public:
    /** @brief owner page manager */
    virtual ZFUIPageManager *pageManager(void)
    {
        return this->_ZFP_ZFUIPage_pageManager;
    }
    /** @brief owner page manager */
    template<typename T_ZFUIPageManager>
    T_ZFUIPageManager pageManager(void)
    {
        return ZFCastZFObjectUnchecked(T_ZFUIPageManager, this->pageManager());
    }

    /** @brief the page's container view, ensured not null */
    ZFMETHOD_INLINE_0(ZFUIView *, pageView)
    {
        return _ZFP_ZFUIPage_pageView;
    }
    /**
     * @brief #pageView's class, must set before base page's #pageOnCreate
     */
    ZFPROPERTY_ASSIGN(const ZFClass *, pageViewClass)

    // ============================================================
    // page property
public:
    /**
     * @brief page group id, see #ZFUIPageManager::pageResumeForGroupId for more info
     */
    ZFPROPERTY_ASSIGN(zfstring, pageGroupId)

    // ============================================================
    // page state
public:
    /**
     * @brief true if page created (true after #pageOnCreate called)
     */
    ZFMETHOD_INLINE_0(zfbool, pageCreated)
    {
        return this->_ZFP_ZFUIPage_pageCreated;
    }
    /**
     * @brief true if page resumed (true after #pageOnResume called)
     */
    ZFMETHOD_INLINE_0(zfbool, pageResumed)
    {
        return _ZFP_ZFUIPage_pageResumed;
    }

public:
    /** @brief util to #ZFUIPageManager::pageResume */
    ZFMETHOD_DECLARE_0(void, pageResume)
    /** @brief util to #ZFUIPageManager::pageDestroy */
    ZFMETHOD_DECLARE_0(void, pageDestroy)

    // ============================================================
    // page life cycle
protected:
    /** @brief see #ZFUIPage */
    virtual void pageOnCreate(void)
    {
        this->_ZFP_ZFUIPage_pageCreated = zftrue;
        zfautoObject pageView = (this->pageViewClass() != zfnull ? this->pageViewClass() : ZFUIView::ClassData())->newInstance();
        this->_ZFP_ZFUIPage_pageView = pageView;
        zfCoreAssertWithMessage(this->_ZFP_ZFUIPage_pageView != zfnull, "pageViewClass must be type of %s", ZFUIView::ClassData()->className());
        zfRetain(this->_ZFP_ZFUIPage_pageView);
    }
    /** @brief see #ZFUIPage */
    virtual inline void pageOnResume(ZF_IN ZFUIPageResumeReasonEnum reason)
    {
        this->_ZFP_ZFUIPage_pageResumed = zftrue;
    }
    /** @brief see #ZFUIPage */
    virtual inline void pageOnPause(ZF_IN ZFUIPagePauseReasonEnum reason)
    {
        this->_ZFP_ZFUIPage_pageResumed = zffalse;
    }
    /** @brief see #ZFUIPage */
    virtual inline void pageOnDestroy(void)
    {
        zfRetainChange(this->_ZFP_ZFUIPage_pageView, zfnull);
        zfRetainChange(this->_ZFP_ZFUIPage_pageAni, zfnull);
        this->_ZFP_ZFUIPage_pageManager = zfnull;
        this->_ZFP_ZFUIPage_pageCreated = zffalse;
    }

    // ============================================================
    // page ani
public:
    /**
     * @brief page priority to detect for #pageAni, see #pageAniOnPrepare for more info
     */
    ZFPROPERTY_ASSIGN(zfint, pageAniPriority)

    /** @brief see #pageAniOnPrepare */
    ZFMETHOD_INLINE_1(void, pageAni,
                      ZFMP_IN(ZFAnimation *, pageAni))
    {
        zfRetainChange(this->_ZFP_ZFUIPage_pageAni, pageAni);
    }
    /** @brief see #pageAniOnPrepare */
    ZFMETHOD_INLINE_0(ZFAnimation *, pageAni)
    {
        return this->_ZFP_ZFUIPage_pageAni;
    }

protected:
    /**
     * @brief called to prepare page animation
     *
     * when switch pages, we would perform animations automatically:
     * -# detect page priority by #pageAniPriority
     * -# pageWithHighPriority->pageAniOnPrepare(xxx);
     *
     * page with high priority has the chance to setup animations
     * for both itself and its sibling page,
     * and store the result animation to #pageAni and siblingPage->pageAni()
     */
    virtual void pageAniOnPrepare(ZF_IN ZFEnum *resumeOrPauseReason, ZF_IN ZFUIPage *siblingPage) {}

    // ============================================================
private:
    ZFUIPageManager *_ZFP_ZFUIPage_pageManager;
    ZFUIView *_ZFP_ZFUIPage_pageView;
    zfbool _ZFP_ZFUIPage_pageCreated;
    zfbool _ZFP_ZFUIPage_pageResumed;
    ZFAnimation *_ZFP_ZFUIPage_pageAni;
protected:
    /** @cond ZFPrivateDoc */
    ZFUIPage(void)
    : _ZFP_ZFUIPage_pageManager(zfnull)
    , _ZFP_ZFUIPage_pageView(zfnull)
    , _ZFP_ZFUIPage_pageCreated(zffalse)
    , _ZFP_ZFUIPage_pageResumed(zffalse)
    , _ZFP_ZFUIPage_pageAni(zfnull)
    {
    }
    /** @endcond */
    zfoverride
    virtual void objectOnDealloc(void)
    {
        zflockfree_zfRetainChange(this->_ZFP_ZFUIPage_pageView, zfnull);
        zflockfree_zfRetainChange(this->_ZFP_ZFUIPage_pageAni, zfnull);
        zfsuper::objectOnDealloc();
    }
private:
    friend zfclassFwd ZFUIPageManager;
    friend zfclassFwd _ZFP_ZFUIPageManagerPrivate;
};

// ============================================================
zfclassFwd _ZFP_ZFUIPageManagerPrivate;
/** @brief see #ZFUIPage */
zfclass ZFUIPageManager : zfextends ZFObject
{
    ZFOBJECT_DECLARE(ZFUIPageManager, ZFObject)

    // ============================================================
    // observers
public:
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when page manager created
     */
    ZFOBSERVER_EVENT(ManagerOnCreate)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when page manager resumed
     */
    ZFOBSERVER_EVENT(ManagerOnResume)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when page manager resumed
     */
    ZFOBSERVER_EVENT(ManagerOnPause)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when page manager destroyed
     */
    ZFOBSERVER_EVENT(ManagerOnDestroy)

    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when #managerUIBlockedOnChange,
     * see #managerUIBlocked
     */
    ZFOBSERVER_EVENT(ManagerUIBlockedOnChange)

    // ============================================================
    // manager creation
public:
    /**
     * @brief manually create the manager
     *
     * for most case, it's recommended to use #managerCreateForWindow
     * to create with a #ZFUIWindow for short\n
     * if you want to embed page manager logic to other container,
     * you may manually use these methods to manage the life cycle:
     * -  #managerCreate
     * -  #managerResume
     * -  #managerPause
     * -  #managerDestroy
     */
    ZFMETHOD_DECLARE_0(void, managerCreate)
    /** @brief see #managerCreate */
    ZFMETHOD_DECLARE_0(void, managerResume)
    /** @brief see #managerCreate */
    ZFMETHOD_DECLARE_0(void, managerPause)
    /** @brief see #managerCreate */
    ZFMETHOD_DECLARE_0(void, managerDestroy)

protected:
    /** @brief for subclass to do actual works */
    virtual void managerOnCreate(void);
    /** @brief for subclass to do actual works */
    virtual void managerOnResume(void) {}
    /** @brief for subclass to do actual works */
    virtual void managerOnPause(void) {}
    /** @brief for subclass to do actual works */
    virtual void managerOnDestroy(void);

public:
    /** @brief true if #managerCreate called */
    ZFMETHOD_DECLARE_0(zfbool, managerCreated)
    /** @brief true if #managerResume called */
    ZFMETHOD_DECLARE_0(zfbool, managerResumed)

public:
    /**
     * @brief util method to create manager and a #ZFUIWindow
     *   to manage life cycle for most case
     *
     * this method would:
     * -# create a #ZFUIWindow
     * -# attach #managerContainer to the window
     * -# automatically call #managerCreate series according to window's life cycle
     * -# when you want to destroy the manager,
     *   simply call #managerDestroy
     */
    ZFMETHOD_DECLARE_0(ZFUIWindow *, managerCreateForWindow)
    /** @brief valid only for #managerCreateForWindow */
    ZFMETHOD_DECLARE_0(ZFUIWindow *, managerOwnerWindow)

    // ============================================================
    // container
public:
    /**
     * @brief container view of this page manager
     *
     * view tree:
     * @code
     *   managerContainer
     *     yourExtraView (title view for example)
     *     pageContainer
     *       page1.pageView
     *     yourExtraView (bottom tab for example)
     * @endcode
     *
     * the class of the container view can be changed by
     * #managerContainerClass/#pageContainerClass,
     * but must be set before base manager's #managerOnCreate
     */
    ZFMETHOD_DECLARE_0(ZFUIView *, managerContainer)
    /** @brief see #managerContainer */
    ZFMETHOD_DECLARE_0(ZFUIView *, pageContainer)

    /** @brief see #managerContainer */
    ZFPROPERTY_ASSIGN(const ZFClass *, managerContainerClass)
    /** @brief see #managerContainer */
    ZFPROPERTY_ASSIGN(const ZFClass *, pageContainerClass)

    // ============================================================
    // manager control
public:
    /**
     * @brief enable or disable manager's user interaction
     *
     * it's useful to disable user interaction when page is doing some heavy works,
     * such as animating switching pages\n
     * this method can be embeded but must be paired,
     * last time you call this method to restore enable state would finally restore enable state
     */
    ZFMETHOD_DECLARE_1(void, managerUIBlocked,
                       ZFMP_IN(zfbool, value))
    /** @brief see #managerUIBlocked */
    ZFMETHOD_DECLARE_0(zfbool, managerUIBlocked)
    /** @brief see #managerUIBlocked */
    ZFMETHOD_DECLARE_0(zfindex, managerUIBlockedCount)
protected:
    /** @brief see #EventManagerUIBlockedOnChange */
    virtual inline void managerUIBlockedOnChange(void) {}

    // ============================================================
    // page access
public:
    /**
     * @brief page count of this page manager
     */
    ZFMETHOD_INLINE_0(zfindex, pageCount)
    {
        return this->pageList().count();
    }
    /**
     * @brief directly access the page list
     *
     * you must not store the list for future use,
     * since its content may change when resolving page request\n
     * you must not modify the list\n
     * you must not reorder the list,
     * except when using with #pageMoveBegin
     */
    ZFMETHOD_DECLARE_0(ZFCoreArrayPOD<ZFUIPage *> &, pageList)
    /**
     * @brief page at index
     */
    ZFMETHOD_INLINE_1(ZFUIPage *, pageAtIndex,
                      ZFMP_IN(zfindex, index))
    {
        return this->pageList().get(index);
    }
    /**
     * @brief page at index
     */
    template<typename T_ZFUIPage>
    inline T_ZFUIPage pageAtIndex(ZF_IN zfindex index)
    {
        return ZFCastZFObjectUnchecked(T_ZFUIPage, this->pageAtIndex(index));
    }
    /**
     * @brief return foreground page or null if no page
     */
    ZFMETHOD_INLINE_0(ZFUIPage *, pageForeground)
    {
        if(!this->pageList().isEmpty())
        {
            return this->pageList().getLast();
        }
        return zfnull;
    }
    /**
     * @brief return foreground page or null if no page
     */
    template<typename T_ZFUIPage>
    inline T_ZFUIPage pageForeground(void)
    {
        return ZFCastZFObjectUnchecked(T_ZFUIPage, this->pageForeground());
    }
    /**
     * @brief find page index
     */
    ZFMETHOD_INLINE_1(zfindex, pageIndex,
                      ZFMP_IN(ZFUIPage *, page))
    {
        return this->pageList().find(page);
    }

    /**
     * @brief find page by #pageGroupId
     */
    ZFMETHOD_DECLARE_1(ZFCoreArrayPOD<ZFUIPage *>, pageListForGroupId,
                       ZFMP_IN(const zfchar *, pageGroupId))

    // ============================================================
    // page management
public:
    /**
     * @brief override animation for next page management,
     *   can be called more than once to override sequently,
     *   can not undo
     */
    ZFMETHOD_DECLARE_3(void, pageAniOverride,
                       ZFMP_IN(ZFAnimation *, resumeAni),
                       ZFMP_IN(ZFAnimation *, pauseAni),
                       ZFMP_IN_OPT(zfbool, pauseAniHigherPriority, zffalse))

    /** @brief create and resume page */
    ZFMETHOD_DECLARE_1(void, pageCreate,
                       ZFMP_IN(ZFUIPage *, page))

    /** @brief resume page */
    ZFMETHOD_DECLARE_1(void, pageResume,
                       ZFMP_IN(ZFUIPage *, page))
    /** @brief resume page */
    ZFMETHOD_DECLARE_1(void, pageResume,
                       ZFMP_IN(zfindex, pageIndex))
    /** @brief resume multiple page for group id */
    ZFMETHOD_DECLARE_1(void, pageResumeForGroupId,
                       ZFMP_IN(const zfchar *, pageGroupId))

    /** @brief destroy page */
    ZFMETHOD_DECLARE_1(void, pageDestroy,
                       ZFMP_IN(ZFUIPage *, page))
    /** @brief destroy page */
    ZFMETHOD_DECLARE_1(void, pageDestroy,
                       ZFMP_IN(zfindex, pageIndex))

    /**
     * @brief advanced page stack management
     *
     * usage:
     * @code
     *   pm->pageMoveBegin();
     *
     *   pageList = pm->pageList();
     *   ... // change page stack order by modifying the pageList directly
     *
     *   pm->pageMoveEnd();
     * @endcode
     */
    ZFMETHOD_DECLARE_0(void, pageMoveBegin)
    /** @brief see #pageMoveBegin */
    ZFMETHOD_DECLARE_0(void, pageMoveEnd)

protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);
private:
    _ZFP_ZFUIPageManagerPrivate *d;
    friend zfclassFwd _ZFP_ZFUIPageManagerPrivate;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIPage_h_

