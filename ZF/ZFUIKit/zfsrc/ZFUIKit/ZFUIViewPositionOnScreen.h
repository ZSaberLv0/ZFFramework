/**
 * @file ZFUIViewPositionOnScreen.h
 * @brief get view's position on screen
 */

#ifndef _ZFI_ZFUIViewPositionOnScreen_h_
#define _ZFI_ZFUIViewPositionOnScreen_h_

#include "ZFUIView.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief get view's position on screen
 *
 * @note (ZFTAG_LIMITATION) result value would be invalid until whole layout step finished,
 *   due to impl's limitation,
 *   we are unable to be notified which time the layout step would finish,
 *   so the best solution to check valid position is using delay,
 *   use #ZFThreadTaskRequest is recommended
 */
ZFMETHOD_FUNC_DECLARE_2(void, ZFUIViewPositionOnScreen,
                        ZFMP_OUT(ZFUIRect &, rect),
                        ZFMP_IN(ZFUIView *, view))
/**
 * @brief get view's position on screen
 */
ZFMETHOD_FUNC_INLINE_DECLARE_1(ZFUIRect, ZFUIViewPositionOnScreen,
                               ZFMP_IN(ZFUIView *, view))
{
    ZFUIRect rect = ZFUIRectZero();
    ZFUIViewPositionOnScreen(rect, view);
    return rect;
}

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIViewPositionOnScreen_h_

