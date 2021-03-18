/**
 * @file ZFAnimation.h
 * @brief base class of all animation
 */

#ifndef _ZFI_ZFAnimation_h_
#define _ZFI_ZFAnimation_h_

#include "ZFUtilityDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief global default animation duration, 250 by default
 */
ZFEXPORT_VAR_DECLARE(zftimet, ZFAnimationDurationDefault)

// ============================================================
zfclassFwd _ZFP_ZFAnimationPrivate;
/**
 * @brief base class of all animation
 */
zfclass ZF_ENV_EXPORT ZFAnimation : zfextends ZFStyleableObject
{
    ZFOBJECT_DECLARE(ZFAnimation, ZFStyleableObject)

public:
    // ============================================================
    // event
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when start an invalid animation
     */
    ZFOBSERVER_EVENT(AniOnInvalid)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when animation delay finished
     */
    ZFOBSERVER_EVENT(AniOnDelayFinish)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when animation started
     * @note for delayed animation,
     *   this event would be fired before delay,
     *   use #EventAniOnDelayFinish if necessary
     */
    ZFOBSERVER_EVENT(AniOnStart)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when animation stopped
     */
    ZFOBSERVER_EVENT(AniOnStop)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when animation stopped or invalid,
     * designed for convenient,
     * param0 is a #v_zfbool to show whether the animation is valid
     */
    ZFOBSERVER_EVENT(AniOnStopOrInvalid)

protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);
    zfoverride
    virtual void objectOnDeallocPrepare(void);

    // ============================================================
    // property
public:
    /**
     * @brief animation's delay in miliseconds, 0 by default
     */
    ZFPROPERTY_ASSIGN(zftimet, aniDelay)
    /**
     * @brief animation's duration in miliseconds, 0 to use #ZFAnimationDurationDefault, 0 by default
     */
    ZFPROPERTY_ASSIGN(zftimet, aniDuration)
    /**
     * @brief util method to #aniDuration
     */
    ZFMETHOD_INLINE_0(zftimet, aniDurationFixed)
    {
        return (this->aniDuration() > 0 ? this->aniDuration() : ZFAnimationDurationDefault());
    }
    /**
     * @brief whether automatically stop previous animation attached to #aniTarget,
     *   false by default
     */
    ZFPROPERTY_ASSIGN(zfbool, aniAutoStopPrev)

public:
    /**
     * @brief animation's target, assign only,
     *   but would be retain automatically while animation is running
     *
     * this is not necessary to be set during animation's running
     * (although most animation subclass need a target)
     */
    ZFMETHOD_DECLARE_1(void, aniTarget,
                       ZFMP_IN(ZFObject *, aniTarget))
    /**
     * @brief animation's target
     */
    ZFMETHOD_DECLARE_0(ZFObject *, aniTarget)

    // ============================================================
    // start stop
public:
    /**
     * @brief start the animation
     * @note an animation would be retained automatically when it's running,
     *   and released after stopped
     */
    ZFMETHOD_DECLARE_0(void, aniStart)
    /**
     * @brief true if the animation is running (or delaying)
     */
    ZFMETHOD_DECLARE_0(zfbool, aniRunning)
    /**
     * @brief true if the animation is delaying
     */
    ZFMETHOD_DECLARE_0(zfbool, aniDelaying)
    /**
     * @brief stop the animation or do nothing if not started
     */
    ZFMETHOD_DECLARE_0(void, aniStop)
    /**
     * @brief true if the animation is stopped by calling #aniStop
     *
     * this state would be kept until next #aniStart called
     */
    ZFMETHOD_DECLARE_0(zfbool, aniStoppedByUser)

public:
    /**
     * @brief get current animation's id
     *
     * animation id would be updated automatically
     * each time animation is started and stopped
     */
    ZFMETHOD_DECLARE_0(zfidentity, aniId)

public:
    /**
     * @brief check whether animation is valid, see #aniImplCheckValid
     */
    ZFMETHOD_DECLARE_0(zfbool, aniValid)

protected:
    /** @brief called when #aniTarget changed */
    virtual void aniImplTargetOnChange(ZF_IN ZFObject *aniTargetOld) {}
    /**
     * @brief called to check whether the animation is currently valid,
     *   an invalid animation is ensured can't be started
     *
     * by default, this method would only check animation's duration,
     * for common case, you should check whether the target is valid
     */
    virtual zfbool aniImplCheckValid(void)
    {
        return (this->aniDurationFixed() > 0);
    }

public:
    zffinal void _ZFP_ZFAnimation_aniImplDelayNotifyFinish(ZF_IN zfidentity taskId);
    zffinal void _ZFP_ZFAnimation_aniReadyStart(void);
    zffinal void _ZFP_ZFAnimation_aniReadyStop(void);
    zffinal void _ZFP_ZFAnimation_aniDummyNotifyStop(void);
protected:
    /**
     * @brief for subclass to achieve delay logic
     *
     * by default, this method would implement delay by #ZFThreadExecuteInMainThreadAfterDelay,
     * you may override and supply your own implementation without call super,
     * if you do, you must override or call all of these methods:
     * -  aniImplDelay: to achieve delay
     * -  aniImplDelayCancel: to achieve delay cancel
     * -  aniImplDelayNotifyFinish: to notify delay finished
     */
    virtual void aniImplDelay(void);
    /**
     * @brief see #aniImplDelay
     */
    virtual void aniImplDelayCancel(void);
    /**
     * @brief see #aniImplDelay
     */
    zffinal void aniImplDelayNotifyFinish(void);
    /**
     * @brief for subclass to achieve actual animation
     */
    virtual void aniImplStart(void);
    /**
     * @brief for subclass to stop actual animation
     */
    virtual void aniImplStop(void);
    /** @brief see #EventAniOnInvalid */
    virtual inline void aniOnInvalid(void)
    {
        this->observerNotify(ZFAnimation::EventAniOnInvalid());
    }
    /** @brief see #EventAniOnDelayFinish */
    virtual inline void aniOnDelayFinish(void)
    {
        this->observerNotify(ZFAnimation::EventAniOnDelayFinish());
    }
    /** @brief see #EventAniOnStart */
    virtual inline void aniOnStart(void)
    {
        this->observerNotify(ZFAnimation::EventAniOnStart());
    }
    /** @brief see #EventAniOnStop */
    virtual inline void aniOnStop(void)
    {
        this->observerNotify(ZFAnimation::EventAniOnStop());
    }
    /** @brief see #EventAniOnStopOrInvalid */
    virtual inline void aniOnStopOrInvalid(ZF_IN zfbool aniValid)
    {
        this->observerNotify(ZFAnimation::EventAniOnStopOrInvalid(), zflineAlloc(v_zfbool, aniValid));
    }
    /**
     * @brief subclass must notify after the animation stop
     */
    zffinal void aniImplNotifyStop(void);

private:
    _ZFP_ZFAnimationPrivate *d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFAnimation_h_

