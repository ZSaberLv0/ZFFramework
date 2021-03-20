/**
 * @file ZFAnimationTimeLine.h
 * @brief abstract animation based on time line
 */

#ifndef _ZFI_ZFAnimationTimeLine_h_
#define _ZFI_ZFAnimationTimeLine_h_

#include "ZFAnimation.h"
#include "ZFTimeLineCurve.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassFwd _ZFP_ZFAnimationTimeLinePrivate;
/**
 * @brief animation based on time line
 *
 * this is a dummy animation holder which do nothing by default,
 * you should either:
 * -  supply subclass and implement actual animation by overriding
 *   #aniTimeLineOnUpdate
 * -  attach observer to #EventAniTimeLineOnUpdate
 *
 * serializable data:
 * @code
 *   <ZFAnimationTimeLine />
 * @endcode
 */
zfclass ZF_ENV_EXPORT ZFAnimationTimeLine : zfextends ZFAnimation
{
    ZFOBJECT_DECLARE(ZFAnimationTimeLine, ZFAnimation)

public:
    // ============================================================
    // event
    /**
     * @brief see #ZFObject::observerNotify
     *
     * param0 is a float #v_zffloat containing current time line progress
     */
    ZFOBSERVER_EVENT(AniTimeLineOnUpdate)

    // ============================================================
    // object
protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);

public:
    zfoverride
    virtual zfidentity objectHash(void);
    zfoverride
    virtual ZFCompareResult objectCompare(ZF_IN ZFObject *anotherObj);

    // ============================================================
    // property
public:
    /**
     * @brief curve for time line, null to use linear time line, null by default
     */
    ZFPROPERTY_RETAIN(ZFTimeLineCurve *, aniCurve)
    /**
     * @brief interval to update time line, in miliseconds, 0 by default
     *
     * when this value is 0, we would use special logic to achieve global time line control:
     * -# calculate frame count by #aniDurationFixed/#ZFGlobalTimerIntervalDefault
     * -# step each frame by #ZFGlobalTimerInterval, until reach the frame count
     *
     * for example, if you increace #ZFGlobalTimerInterval,
     * the animation would looks slower\n
     * this is useful to achieve accurate time line control
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zftimet, aniTimeLineInterval, 0)

    // ============================================================
    // start stop
protected:
    zfoverride
    virtual void aniImplStart(void);
    zfoverride
    virtual void aniImplStop(void);

protected:
    /**
     * @brief called to do the actual update
     *
     * note, progress is based on #aniCurve,
     * which typically has value in range [0, 1] as base value,
     * but may exceeds the range for bounce curve
     */
    virtual void aniTimeLineOnUpdate(ZF_IN zffloat progress);

protected:
    zfoverride
    virtual zfbool serializableOnCheck(void)
    {
        return this->classData() != ZFAnimationTimeLine::ClassData();
    }

private:
    _ZFP_ZFAnimationTimeLinePrivate *d;
    friend zfclassFwd _ZFP_ZFAnimationTimeLinePrivate;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFAnimationTimeLine_h_

