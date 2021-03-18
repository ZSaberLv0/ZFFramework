/**
 * @file ZFUIViewUtil.h
 * @brief utils for ZFUIView
 */

#ifndef _ZFI_ZFUIViewUtil_h_
#define _ZFI_ZFUIViewUtil_h_

#include "ZFUIView.h"

ZF_NAMESPACE_GLOBAL_BEGIN
ZF_NAMESPACE_BEGIN(ZFUIViewUtil)

/**
 * @brief recursive find the root ZFUIView parent of this view,
 *   may return self if no parent
 */
ZFMETHOD_FUNC_DECLARE_1(ZFUIView *, viewRoot,
                        ZFMP_IN(ZFUIView *, view))

/**
 * @brief check whether this view is child of specified view (recursively)
 *
 * true if view equals to this view
 */
ZFMETHOD_FUNC_DECLARE_2(zfbool, viewIsChildOf,
                        ZFMP_IN(ZFUIView *, view),
                        ZFMP_IN(ZFUIView *, parentToCheck))

/**
 * @brief recursively check child at position (relative to the view)
 *
 * may be invalid if layout step not finished
 */
ZFMETHOD_FUNC_DECLARE_5(ZFUIView *, viewChildAt,
                        ZFMP_IN(ZFUIView *, view),
                        ZFMP_IN(const ZFUIPoint &, pos),
                        ZFMP_IN_OPT(zfbool, filterDisabledView, zffalse),
                        ZFMP_IN_OPT(zfbool, filterInternalView, zftrue),
                        ZFMP_IN_OPT(const ZFFilterForZFObject *, filter, zfnull))

/**
 * @brief check rect of view relative to parent,
 *   return #ZFUIRectZero if not child of parent or invalid
 *
 * result value would be invalid until whole layout step finished\n
 * unlike #ZFUIViewPositionOnScreen,
 * this method does not depends on impl,
 * however, result may be invalid if view's parent contains #ZFUINativeViewWrapper\n
 * this method would calculate rect depends on #ZFUIView::layoutChildOffset recursively
 */
ZFMETHOD_FUNC_DECLARE_3(void, viewRectToParent,
                        ZFMP_OUT(ZFUIRect &, rect),
                        ZFMP_IN(ZFUIView *, view),
                        ZFMP_IN(ZFUIView *, parent))
/** @brief see #viewRectToParent */
ZFMETHOD_FUNC_DECLARE_2(ZFUIRect, viewRectToParent,
                        ZFMP_IN(ZFUIView *, view),
                        ZFMP_IN(ZFUIView *, parent))

ZF_NAMESPACE_END(ZFUIViewUtil)
ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIViewUtil_h_

