/**
 * @file ZFUIOnScreenKeyboardAutoFit.h
 * @brief a scroll container that automatically fits content's position
 *   according #ZFUIOnScreenKeyboardState
 */

#ifndef _ZFI_ZFUIOnScreenKeyboardAutoFit_h_
#define _ZFI_ZFUIOnScreenKeyboardAutoFit_h_

#include "ZFUIWidgetDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUIOnScreenKeyboardAutoFitLayout
zfclassFwd _ZFP_ZFUIOnScreenKeyboardAutoFitLayoutPrivate;
/**
 * @brief a scroll container that automatically fits content's position
 *   according #ZFUIOnScreenKeyboardState
 */
zfclass ZF_ENV_EXPORT ZFUIOnScreenKeyboardAutoFitLayout : zfextends ZFUIScrollView
{
    ZFOBJECT_DECLARE(ZFUIOnScreenKeyboardAutoFitLayout, ZFUIScrollView)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIOnScreenKeyboardAutoFitLayout)

public:
    /**
     * @brief whether enable auto fit, true by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfbool, autoFitEnable,
                                zftrue)
    ZFPROPERTY_ON_ATTACH_DECLARE(zfbool, autoFitEnable)
    /**
     * @brief whether auto move focused view to visible position, true by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfbool, autoFitFocusedViewToVisible,
                                zftrue)
    ZFPROPERTY_ON_ATTACH_DECLARE(zfbool, autoFitFocusedViewToVisible)

    /**
     * @brief whether allow manually scroll when auto fit enabled, true by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfbool, autoFitScrollEnable,
                                zftrue)
    ZFPROPERTY_ON_ATTACH_DECLARE(zfbool, autoFitScrollEnable)

public:
    /**
     * @brief current focused child view inside this layout, null if none
     */
    virtual ZFUIView *autoFitFocusedView(void);

protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);
    zfoverride
    virtual void objectOnInitFinish(void);

protected:
    virtual ZFSerializablePropertyType serializableOnCheckPropertyType(ZF_IN const ZFProperty *property);

protected:
    zfoverride
    virtual void layoutOnMeasure(ZF_OUT ZFUISize &ret,
                                 ZF_IN const ZFUISize &sizeHint,
                                 ZF_IN const ZFUISizeParam &sizeParam);
    zfoverride
    virtual void layoutOnLayoutPrepare(ZF_IN const ZFUIRect &bounds);

protected:
    // disable scroll thumb
    zfoverride
    virtual void scrollThumbHorizontalOnInit(void)
    {
    }
    zfoverride
    virtual void scrollThumbVerticalOnInit(void)
    {
    }

private:
    _ZFP_ZFUIOnScreenKeyboardAutoFitLayoutPrivate *d;
    friend zfclassFwd _ZFP_ZFUIOnScreenKeyboardAutoFitLayoutPrivate;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIOnScreenKeyboardAutoFit_h_

