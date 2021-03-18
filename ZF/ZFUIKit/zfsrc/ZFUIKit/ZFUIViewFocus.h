/**
 * @file ZFUIViewFocus.h
 * @brief ZFUIView focus utility
 */

#ifndef _ZFI_ZFUIViewFocus_h_
#define _ZFI_ZFUIViewFocus_h_

#include "ZFUIView.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief manually set the next focus target for the from view, set null to remove
 */
ZFMETHOD_FUNC_DECLARE_2(void, ZFUIViewFocusNextSetup,
                        ZFMP_IN(ZFUIView *, from),
                        ZFMP_IN(ZFUIView *, nextFocus))

#define _ZFP_ZFUIViewFocusNextSetupChainEndPtr ((ZFUIView *)-1)
extern ZF_ENV_EXPORT void _ZFP_ZFUIViewFocusNextSetupChain(ZF_IN ZFUIView *view0, ZF_IN ZFUIView *view1, ...);
/**
 * @brief util method to chain all view's next focus target by #ZFUIViewFocusNextSetup
 */
#define ZFUIViewFocusNextSetupChain(view0, view1, ...) \
    _ZFP_ZFUIViewFocusNextSetupChain(view0, view1, ##__VA_ARGS__, _ZFP_ZFUIViewFocusNextSetupChainEndPtr)

// ============================================================
/**
 * @brief filter to exclude certain view from being focused by #ZFUIViewFocusNextFind, empty by default
 */
ZFEXPORT_VAR_DECLARE(ZFFilterForZFObject, ZFUIViewFocusNextFilter)

/**
 * @brief param for #ZFUIViewFocusNextFind and #ZFUIViewFocusNextMove
 */
zfclassLikePOD ZF_ENV_EXPORT ZFUIViewFocusNextParam
{
    ZFCORE_PARAM_DECLARE_SELF(ZFUIViewFocusNextParam)

    /**
     * @brief direction to find, #ZFUIOrientation::e_Right | #ZFUIOrientation::e_Bottom by default
     */
    ZFCORE_PARAM_WITH_INIT(ZFUIOrientationFlags, focusDirection, (ZFUIOrientation::e_Right | ZFUIOrientation::e_Bottom))
    /**
     * @brief whether find in loop mode, false by default
     */
    ZFCORE_PARAM_WITH_INIT(zfbool, focusLoopMode, zffalse)
    /**
     * @brief only find within this view, null to end with #ZFUIWindow, null by default
     */
    ZFCORE_PARAM(ZFUIView *, focusEndParent)
    /**
     * @brief whether to find internal views, true by default
     */
    ZFCORE_PARAM_WITH_INIT(zfbool, focusInternalViews, zftrue)

public:
    /** @cond ZFPrivateDoc */
    zfbool operator == (ZF_IN const ZFUIViewFocusNextParam &ref) const
    {
        return (zftrue
                && this->focusDirection() == ref.focusDirection()
                && this->focusLoopMode() == ref.focusLoopMode()
                && this->focusEndParent() == ref.focusEndParent()
                && this->focusInternalViews() == ref.focusInternalViews()
            );
    }
    zfbool operator != (ZF_IN const ZFUIViewFocusNextParam &ref) const
    {
        return !this->operator == (ref);
    }
    /** @endcond */
};
ZFTYPEID_ACCESS_ONLY_DECLARE(ZFUIViewFocusNextParam, ZFUIViewFocusNextParam)

// ============================================================
/**
 * @brief find next focusable from view (excluding) with direction
 *
 * return first focusable view if found
 */
ZFMETHOD_FUNC_DECLARE_2(ZFUIView *, ZFUIViewFocusNextFind,
                        ZFMP_IN(ZFUIView *, view),
                        ZFMP_IN_OPT(const ZFUIViewFocusNextParam &, param, ZFUIViewFocusNextParam()))

/**
 * @brief move focus to next if available or do nothing otherwise, see #ZFUIViewFocusNextFind
 */
ZFMETHOD_FUNC_DECLARE_2(ZFUIView *, ZFUIViewFocusNextMove,
                        ZFMP_IN(ZFUIView *, view),
                        ZFMP_IN_OPT(const ZFUIViewFocusNextParam &, param, ZFUIViewFocusNextParam()))

// ============================================================
/**
 * @brief util method to resolve key event (such as up/down/left/right keys) and check to move focus, see #ZFUIViewFocusNextFind
 *
 * return true if the event has been resolved,
 * next focused view can be checked by the optional nextFocus param
 */
ZFMETHOD_FUNC_DECLARE_4(zfbool, ZFUIViewFocusResolveKeyEvent,
                        ZFMP_IN(ZFUIView *, view),
                        ZFMP_IN(ZFUIKeyEvent *, keyEvent),
                        ZFMP_OUT_OPT(zfautoObject *, nextFocus, zfnull),
                        ZFMP_IN_OPT(ZFUIView *, endParent, zfnull))

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIViewFocus_h_

