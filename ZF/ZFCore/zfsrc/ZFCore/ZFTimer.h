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
    ZFOBJECT_ON_INIT_DECLARE_5(ZFMP_IN(zftimet, timerInterval),
                               ZFMP_IN_OPT(ZFObject *, timerParam0, zfnull),
                               ZFMP_IN_OPT(ZFObject *, timerParam1, zfnull),
                               ZFMP_IN_OPT(zftimet, timerDelay, zftimetZero()),
                               ZFMP_IN_OPT(zfbool, timerActivateInMainThread, zftrue))
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
    ZFPROPERTY_OVERRIDE_ON_VERIFY_DECLARE(zftimet, timerInterval)

    /**
     * @brief timer's delay when start a timer in mili seconds, default is 0
     *
     * if delay is less than 10, it's treated as 0\n
     * else, first timer event would be fired after (timerDelay + timerInterval)
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zftimet, timerDelay, 0)
    ZFPROPERTY_OVERRIDE_ON_VERIFY_DECLARE(zftimet, timerDelay)

    /**
     * @brief whether timer should be fired in main thread, true by default
     *
     * fired in main thread for convenience but may cause timer to be more inaccurate,
     * use only if necessary
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfbool, timerActivateInMainThread, zftrue)
    ZFPROPERTY_OVERRIDE_ON_VERIFY_DECLARE(zfbool, timerActivateInMainThread)

    /**
     * @brief timer param, automatically retained
     *
     * the param is alive as long as the timer object is alive,
     * it won't be released after timer stop
     */
    ZFPROPERTY_RETAIN(ZFObject *, timerParam0)
    ZFPROPERTY_OVERRIDE_ON_VERIFY_DECLARE(ZFObject *, timerParam0)

    /**
     * @brief timer param, automatically retained
     *
     * the param is alive as long as the timer object is alive,
     * it won't be released after timer stop
     */
    ZFPROPERTY_RETAIN(ZFObject *, timerParam1)
    ZFPROPERTY_OVERRIDE_ON_VERIFY_DECLARE(ZFObject *, timerParam1)

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
/**
 * @brief global timer util
 *
 * you can attach any callbacks to same global timer instance,
 * the global timer would be running if more than one timer callback attached,
 * and would be stopped when all of them detached\n
 * you can change timer interval by #ZFGlobalTimerInterval even when it's running
 */
ZFMETHOD_FUNC_DECLARE_5(zfidentity, ZFGlobalTimerAttach,
                        ZFMP_IN(const ZFListener &, timerCallback),
                        ZFMP_IN_OPT(ZFObject *, timerCallbackUserData, zfnull),
                        ZFMP_IN_OPT(ZFObject *, owner, zfnull),
                        ZFMP_IN_OPT(zfbool, autoRemoveAfterActivate, zffalse),
                        ZFMP_IN_OPT(ZFLevel, observerLevel, ZFLevelAppNormal))
/** @brief see #ZFGlobalTimerAttach */
ZFMETHOD_FUNC_DECLARE_2(void, ZFGlobalTimerDetach,
                        ZFMP_IN(const ZFListener &, timerCallback),
                        ZFMP_IN_OPT(ZFObject *, timerCallbackUserData, zfnull))
/** @brief see #ZFGlobalTimerAttach */
ZFMETHOD_FUNC_DECLARE_1(void, ZFGlobalTimerDetachByTaskId,
                        ZFMP_IN(zfidentity, taskId))
/** @brief see #ZFGlobalTimerAttach */
ZFMETHOD_FUNC_DECLARE_1(void, ZFGlobalTimerDetachByOwner,
                        ZFMP_IN(ZFObject *, owner))
/** @brief see #ZFGlobalTimerAttach */
ZFMETHOD_FUNC_DECLARE_0(void, ZFGlobalTimerDetachAll)

/** @brief see #ZFGlobalTimerAttach */
ZFMETHOD_FUNC_DECLARE_0(zftimet const &, ZFGlobalTimerIntervalDefault)
/** @brief see #ZFGlobalTimerAttach */
ZFMETHOD_FUNC_DECLARE_1(void, ZFGlobalTimerIntervalDefault,
                        ZFMP_IN(zftimet const &, timerIntervalDefault))
/** @brief see #ZFGlobalTimerAttach */
ZFMETHOD_FUNC_DECLARE_0(zftimet const &, ZFGlobalTimerInterval)
/** @brief see #ZFGlobalTimerAttach */
ZFMETHOD_FUNC_DECLARE_1(void, ZFGlobalTimerInterval,
                        ZFMP_IN(zftimet const &, timerInterval))

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFTimer_h_

