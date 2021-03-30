/**
 * @file ZFGlobalTimer.h
 * @brief timer utility
 */

#ifndef _ZFI_ZFGlobalTimer_h_
#define _ZFI_ZFGlobalTimer_h_

#include "ZFTimer.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief global timer util
 *
 * you can attach any callbacks to same global timer instance,
 * the global timer would be running if more than one timer callback attached,
 * and would be stopped when all of them detached\n
 * you can change timer interval by #ZFGlobalTimerInterval even when it's running\n
 * \n
 * a typical advanced usage for global timer:
 * -# change time based duration to frame based timer by (duration / ZFGlobalTimerIntervalDefault())
 * -# step each frame by attaching your update logic to global timer
 * -# change ZFGlobalTimerInterval to a greater value,
 *   and your logci would be "slowed down"
 *
 * you may also use #ZFGlobalTimerManualStep to manually control the timer steps
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

// ============================================================
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

// ============================================================
/**
 * @brief manual control global timer step
 *
 * once called, all task attached to #ZFGlobalTimerAttach
 * would not be notified unless called this method again,
 * you may cancel the manual step and restore to original timer logic
 * by #ZFGlobalTimerManualStep
 */
ZFMETHOD_FUNC_DECLARE_0(void, ZFGlobalTimerManualStep)
/** @brief see #ZFGlobalTimerManualStep */
ZFMETHOD_FUNC_DECLARE_0(void, ZFGlobalTimerManualStepCancel)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFGlobalTimer_h_

