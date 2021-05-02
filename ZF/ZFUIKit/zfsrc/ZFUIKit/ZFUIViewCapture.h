/**
 * @file ZFUIViewCapture.h
 * @brief capture ZFUIView to ZFUIImage
 */

#ifndef _ZFI_ZFUIViewCapture_h_
#define _ZFI_ZFUIViewCapture_h_

#include "ZFUIView.h"
#include "ZFUIImage.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief capture ZFUIView to ZFUIImage
 *
 * note: you could only capture view when the view fully layouted and displayed,
 * use #ZFThreadTaskRequest if necessary
 */
ZFMETHOD_FUNC_DECLARE_1(zfautoObjectT<ZFUIImage *>, ZFUIViewCapture,
                        ZFMP_IN(ZFUIView *, view))

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIViewCapture_h_

