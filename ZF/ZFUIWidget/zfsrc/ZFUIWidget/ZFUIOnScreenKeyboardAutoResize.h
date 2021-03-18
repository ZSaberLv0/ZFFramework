/**
 * @file ZFUIOnScreenKeyboardAutoResize.h
 * @brief auto fit #ZFUIWindow according to #ZFUIOnScreenKeyboardState
 */

#ifndef _ZFI_ZFUIOnScreenKeyboardAutoResize_h_
#define _ZFI_ZFUIOnScreenKeyboardAutoResize_h_

#include "ZFUIWidgetDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief auto resize #ZFUIWindow according to #ZFUIOnScreenKeyboardState
 *
 * resize logic are done by changing window's #ZFUIViewLayoutParam::layoutMargin\n
 * \n
 * #ZFUIOnScreenKeyboardAutoResizeStop would be applied automatically when owner window destroyed
 */
ZFMETHOD_FUNC_DECLARE_1(void, ZFUIOnScreenKeyboardAutoResizeStart,
                        ZFMP_IN(ZFUIWindow *, window))
/** @brief see #ZFUIOnScreenKeyboardAutoResizeStart */
ZFMETHOD_FUNC_DECLARE_1(void, ZFUIOnScreenKeyboardAutoResizeStop,
                        ZFMP_IN(ZFUIWindow *, window))

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIOnScreenKeyboardAutoResize_h_

