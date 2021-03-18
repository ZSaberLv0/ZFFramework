/**
 * @file ZFTimer.h
 * @brief timer utility
 */

#ifndef _ZFI_ZFTimer_h_
#define _ZFI_ZFTimer_h_

#include "ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd _ZFP_ZFTimerPrivate;
/**
 * @brief timer utility
 *
 * usage:
 * @code
 *   ZFTimer *timer = zfAlloc(ZFTimer, ...);
 *   timer->start();
 *   zfRelease(timer); // safe to release after start, timer would be auto released after stop
 * @endcode
 */
zfclass ZF_ENV_EXPORT ZFTimer : zfextends ZFObject
{
    ZFOBJECT_DECLARE(ZFTimer, ZFObject)

    // ============================================================
    // observer events
public:
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when timer started, ensured in the same thread of timer event\n
     * param0 is the #ZFTimer::timerParam0
     * and param1 is the #ZFTimer::timerParam1
     */
    ZFOBSERVER_EVENT(TimerOnStart)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when timer activated\n
     * param0 is the #ZFTimer::timerParam0
     * and param1 is the #ZFTimer::timerParam1
     * @note on what thread this event is fired,
     *   depends on #timerActivateInMainThread
     */
    ZFOBSERVER_EVENT(TimerOnActivate)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when timer stopped, ensured in the same thread that stop the timer\n
     * param0 is the #ZFTimer::timerParam0
     * and param1 is the #ZFTimer::timerParam1
     */
    ZFOBSERVER_EVENT(TimerOnStop)

protected:
    /**
     * @brief init with timer settings
     *
     * you may change them after timer created,
     * but you must not if timer is started
     */
    ZFOBJECT_ON_INIT_INLINE_5(ZFMP_IN(zftimet, timerInterval),
                              ZFMP_IN_OPT(ZFObject *, timerParam0, zfnull),
                              ZFMP_IN_OPT(ZFObject *, timerParam1, zfnull),
                              ZFMP_IN_OPT(zftimet, timerDelay, zftimetZero()),
                              ZFMP_IN_OPT(zfbool, timerActivateInMainThread, zftrue))
    {
        this->objectOnInit();
        zfself::timerInterval(timerInterval);
        zfself::timerDelay(timerDelay);
        zfself::timerParam0(timerParam0);
        zfself::timerParam1(timerParam1);
        zfself::timerActivateInMainThread(timerActivateInMainThread);
    }

    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);
    zfoverride
    virtual void objectOnDeallocPrepare(void);

public:
    /** @brief for internal use only */
    ZFMETHOD_DECLARE_0(void *, nativeTimer)

public:
    /**
     * @brief timer's interval when start a timer in mili seconds, default is 1000
     *
     * assert fail if interval is less than 0
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zftimet, timerInterval, 1000)
    ZFPROPERTY_OVERRIDE_ON_VERIFY_INLINE(zftimet, timerInterval)
    {
        zfCoreAssert(!this->timerStarted());
        zfCoreAssert(this->timerInterval() > 0);
    }

    /**
     * @brief timer's delay when start a timer in mili seconds, default is 0
     *
     * if delay is less than 10, it's treated as 0\n
     * else, first timer event would be fired after (timerDelay + timerInterval)
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zftimet, timerDelay, 0)
    ZFPROPERTY_OVERRIDE_ON_VERIFY_INLINE(zftimet, timerDelay)
    {
        zfCoreAssert(!this->timerStarted());
        zfCoreAssert(this->timerDelay() >= 0);
    }

    /**
     * @brief whether timer should be fired in main thread, true by default
     *
     * fired in main thread for convenience but may cause timer to be more inaccurate,
     * use only if necessary
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfbool, timerActivateInMainThread, zftrue)
    ZFPROPERTY_OVERRIDE_ON_VERIFY_INLINE(zfbool, timerActivateInMainThread)
    {
        zfCoreAssert(!this->timerStarted());
    }

    /**
     * @brief timer param, automatically retained
     *
     * the param is alive as long as the timer object is alive,
     * it won't be released after timer stop
     */
    ZFPROPERTY_RETAIN(ZFObject *, timerParam0)
    ZFPROPERTY_OVERRIDE_ON_VERIFY_INLINE(ZFObject *, timerParam0)
    {
        zfCoreAssert(!this->timerStarted());
    }

    /**
     * @brief timer param, automatically retained
     *
     * the param is alive as long as the timer object is alive,
     * it won't be released after timer stop
     */
    ZFPROPERTY_RETAIN(ZFObject *, timerParam1)
    ZFPROPERTY_OVERRIDE_ON_VERIFY_INLINE(ZFObject *, timerParam1)
    {
        zfCoreAssert(!this->timerStarted());
    }

public:
    /**
     * @brief start the timer
     */
    ZFMETHOD_DECLARE_0(void, timerStart)
    /**
     * @brief stop the timer
     */
    ZFMETHOD_DECLARE_0(void, timerStop)
    /**
     * @brief true if started
     */
    ZFMETHOD_DECLARE_0(zfbool, timerStarted)

public:
    /**
     * @brief get timer's current fired count, with 1 as first fired time's value,
     *   keep after timer stop, but reset before timer start
     */
    ZFMETHOD_DECLARE_0(zfindex, timerActivatedCount)

public:
    zffinal void _ZFP_ZFTimer_timerOnStart(void);
    zffinal void _ZFP_ZFTimer_timerOnActivate(void);
    zffinal void _ZFP_ZFTimer_timerOnStop(void);
protected:
    /** @brief see #EventTimerOnStart */
    virtual inline void timerOnStart(void)
    {
        this->observerNotify(ZFTimer::EventTimerOnStart(), this->timerParam0(), this->timerParam1());
    }
    /** @brief see #EventTimerOnActivate */
    virtual inline void timerOnActivate(void)
    {
        this->observerNotify(ZFTimer::EventTimerOnActivate(), this->timerParam0(), this->timerParam1());
    }
    /** @brief see #EventTimerOnStop */
    virtual inline void timerOnStop(void)
    {
        this->observerNotify(ZFTimer::EventTimerOnStop(), this->timerParam0(), this->timerParam1());
    }

private:
    _ZFP_ZFTimerPrivate *d;
};

// ============================================================
// timer util
/**
 * @brief see #ZFTimerExecute
 */
zfclassLikePOD ZF_ENV_EXPORT ZFTimerExecuteParam
{
    ZFCORE_PARAM_DECLARE_SELF(ZFTimerExecuteParam)

public:
    /** @brief see #ZFTimer::timerInterval */
    ZFCORE_PARAM_WITH_INIT(zftimet, timerInterval, 1000)
    /** @brief see #ZFTimer::timerDelay */
    ZFCORE_PARAM_WITH_INIT(zftimet, timerDelay, 0)
    /** @brief see #ZFTimer::timerActivateInMainThread */
    ZFCORE_PARAM_WITH_INIT(zfbool, timerActivateInMainThread, zftrue)
    /** @brief see #ZFTimer::timerParam0 */
    ZFCORE_PARAM_RETAIN_WITH_INIT(ZFObject *, timerParam0, zfnull)
    /** @brief see #ZFTimer::timerParam1 */
    ZFCORE_PARAM_RETAIN_WITH_INIT(ZFObject *, timerParam1, zfnull)
    /** @brief see #ZFTimer */
    ZFCORE_PARAM_RETAIN_WITH_INIT(ZFObject *, userData, zfnull)

    /**
     * @brief the timer callback to run
     */
    ZFCORE_PARAM(ZFListener, timerCallback)
    /**
     * @brief automatically stop timer when reach max count, 0 means no limit, 0 by default
     */
    ZFCORE_PARAM_WITH_INIT(zfindex, timerActivateCountMax, 0)

public:
    /** @cond ZFPrivateDoc */
    zfbool operator == (ZF_IN const ZFTimerExecuteParam &ref) const
    {
        return (zftrue
                && this->timerInterval() == ref.timerInterval()
                && this->timerDelay() == ref.timerDelay()
                && this->timerActivateInMainThread() == ref.timerActivateInMainThread()
                && this->timerParam0() == ref.timerParam0()
                && this->timerParam1() == ref.timerParam1()
                && this->userData() == ref.userData()
                && this->timerCallback() == ref.timerCallback()
                && this->timerActivateCountMax() == ref.timerActivateCountMax()
            );
    }
    zfbool operator != (ZF_IN const ZFTimerExecuteParam &ref) const
    {
        return !this->operator == (ref);
    }
    /** @endcond */
};
ZFTYPEID_ACCESS_ONLY_DECLARE(ZFTimerExecuteParam, ZFTimerExecuteParam)
/**
 * @brief util method to start a timer
 *
 * return the started timer if success or null otherwise,
 * the started timer would be released automatically when stopped
 */
ZFMETHOD_FUNC_DECLARE_1(zfautoObject, ZFTimerExecute,
                        ZFMP_IN(const ZFTimerExecuteParam &, param))

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFTimer_h_

