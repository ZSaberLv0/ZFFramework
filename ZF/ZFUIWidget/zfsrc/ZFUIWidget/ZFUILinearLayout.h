/**
 * @file ZFUILinearLayout.h
 * @brief linear container view
 */

#ifndef _ZFI_ZFUILinearLayout_h_
#define _ZFI_ZFUILinearLayout_h_

#include "ZFUIWidgetDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUILinearLayoutParam
/**
 * @brief layout param for #ZFUILinearLayout
 *
 * for #ZFUILinearLayout,
 * #ZFUILayoutParam::layoutAlign and #ZFUILayoutParam::layoutMargin
 * would be relative to sibling child,
 * instead of the container view
 */
zfclass ZF_ENV_EXPORT ZFUILinearLayoutParam : zfextends ZFUILayoutParam
{
    ZFOBJECT_DECLARE(ZFUILinearLayoutParam, ZFUILayoutParam)

    /**
     * @brief weight for the view, 0 by default
     *
     * if greater than 0, all children with weight would be layouted by its weight percent,
     * ignoring sizeParam\n
     * for example, assume we have 3 children with these layout weight:
     * -  child0: 2
     * -  child1: 4
     * -  child2: 3
     *
     * then the total weight of children would be (2+4+3)=9,
     * and the children's final size would be:
     * -  child0 = parentSize * 2 / 9
     * -  child1 = parentSize * 4 / 9
     * -  child2 = parentSize * 3 / 9
     */
    ZFPROPERTY_ASSIGN(zfint, layoutWeight)

    /**
     * @brief whether reserve space if child is not visible, false by default
     */
    ZFPROPERTY_ASSIGN(zfbool, layoutReserveSpaceWhenNotVisible)

    ZFPROPERTY_OVERRIDE_ON_INIT_INLINE(ZFUIAlignFlags, layoutAlign)
    {
        propertyValue = ZFUIAlign::e_Center;
    }
};

// ============================================================
// ZFUILinearLayout
/**
 * @brief linear container view that layout children in linear by specified orientation
 */
zfclass ZF_ENV_EXPORT ZFUILinearLayout : zfextends ZFUIView
{
    ZFOBJECT_DECLARE(ZFUILinearLayout, ZFUIView)
    ZFSTYLE_DEFAULT_DECLARE(ZFUILinearLayout)

public:
    // ============================================================
    // properties
    /**
     * @brief direction to layout children, #ZFUIOrientation::e_Left by default
     *
     * #ZFUIOrientation::e_Left means layout children from left to right,
     * while #ZFUIOrientation::e_Bottom means layout children from bottom to top
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIOrientationEnum, layoutOrientation,
                                ZFUIOrientation::e_Left)
    ZFPROPERTY_OVERRIDE_ON_ATTACH_DECLARE(ZFUIOrientationEnum, layoutOrientation)

    /**
     * @brief extra margin independent from children's layout param's margin, #ZFUIMarginZero by default
     */
    ZFPROPERTY_ASSIGN(ZFUIMargin, layoutChildMargin)
    ZFPROPERTY_OVERRIDE_ON_ATTACH_DECLARE(ZFUIMargin, layoutChildMargin)

    /**
     * @brief extra space between each child independent from children's layout param, 0 by default
     */
    ZFPROPERTY_ASSIGN(zfint, layoutChildSpace)
    ZFPROPERTY_OVERRIDE_ON_ATTACH_DECLARE(zfint, layoutChildSpace)

    // ============================================================
    // override ZFUIView
protected:
    zfoverride
    virtual const ZFClass *layoutParamClass(void)
    {
        return ZFUILinearLayoutParam::ClassData();
    }

    zfoverride
    virtual void layoutOnMeasure(ZF_OUT ZFUISize &ret,
                                 ZF_IN const ZFUISize &sizeHint,
                                 ZF_IN const ZFUISizeParam &sizeParam);
    /**
     * @brief override ZFUIView to layout scroll view's internal view and content view
     *
     * usually you should not override this method,
     * if necessary, you must call super
     */
    zfoverride
    virtual void layoutOnLayout(ZF_IN const ZFUIRect &bounds);
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUILinearLayout_h_

