/**
 * @file ZFUIViewLayout.h
 * @brief simple container view that measure according children
 */

#ifndef _ZFI_ZFUIViewLayout_h_
#define _ZFI_ZFUIViewLayout_h_

#include "ZFUIWidgetDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUIViewLayout
/**
 * @brief simple container view that measure according children
 */
zfclass ZF_ENV_EXPORT ZFUIViewLayout : zfextends ZFUIView
{
    ZFOBJECT_DECLARE(ZFUIViewLayout, ZFUIView)

    // ============================================================
    // override ZFUIView
protected:
    /**
     * @brief measure according all of its children,
     *   return the largest area of them
     */
    zfoverride
    virtual void layoutOnMeasure(ZF_OUT ZFUISize &ret,
                                 ZF_IN const ZFUISize &sizeHint,
                                 ZF_IN const ZFUISizeParam &sizeParam);
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIViewLayout_h_

