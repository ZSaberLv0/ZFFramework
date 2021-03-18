/**
 * @file ZFUIPageAni.h
 * @brief common page ani
 */

#ifndef _ZFI_ZFUIPageAni_h_
#define _ZFI_ZFUIPageAni_h_

#include "ZFUIPage.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief see #ZFUIPage::pageAniOnPrepare
 *
 * prepare alpha animation, typically used for tab change
 */
ZFMETHOD_FUNC_DECLARE_3(zfbool, ZFUIPageAniPrepareForAlpha,
                        ZFMP_IN(ZFUIPage *, page),
                        ZFMP_IN(ZFEnum *, resumeOrPauseReason),
                        ZFMP_IN(ZFUIPage *, siblingPage))

/**
 * @brief see #ZFUIPage::pageAniOnPrepare
 *
 * prepare slide animation, typically used for new child page
 */
ZFMETHOD_FUNC_DECLARE_3(zfbool, ZFUIPageAniPrepareForSlide,
                        ZFMP_IN(ZFUIPage *, page),
                        ZFMP_IN(ZFEnum *, resumeOrPauseReason),
                        ZFMP_IN(ZFUIPage *, siblingPage))

/**
 * @brief see #ZFUIPage::pageAniOnPrepare
 *
 * prepare popup animation, typically used for temporary page or login page
 */
ZFMETHOD_FUNC_DECLARE_3(zfbool, ZFUIPageAniPrepareForPopup,
                        ZFMP_IN(ZFUIPage *, page),
                        ZFMP_IN(ZFEnum *, resumeOrPauseReason),
                        ZFMP_IN(ZFUIPage *, siblingPage))

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIPageAni_h_

