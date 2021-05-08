/**
 * @file ZFUIViewType.h
 * @brief additional class for ZFUIView
 */

#ifndef _ZFI_ZFUIViewType_h_
#define _ZFI_ZFUIViewType_h_

#include "ZFUIGlobalStyle.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd ZFUIView;

// ============================================================
// ZFUISizeType
/**
 * @brief size params for parent to define how to measure children,
 *   see #ZFUILayoutParam for more info
 */
ZFENUM_BEGIN(ZFUISizeType)
    /**
     * @brief wrap child to minimal
     */
    ZFENUM_VALUE(Wrap)
    /**
     * @brief fill child to parent
     */
    ZFENUM_VALUE(Fill)
ZFENUM_SEPARATOR(ZFUISizeType)
    ZFENUM_VALUE_REGISTER(Wrap)
    ZFENUM_VALUE_REGISTER(Fill)
ZFENUM_END(ZFUISizeType)

// ============================================================
// ZFUISizeParam
/**
 * @brief 2D size
 */
zfclassPOD ZF_ENV_EXPORT ZFUISizeParam
{
public:
    ZFUISizeTypeEnum width;   /**< @brief width */
    ZFUISizeTypeEnum height;  /**< @brief height */
};

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFUISizeParam
 *       value="(Wrap, Wrap)" // optional, #ZFUISizeType::e_Wrap by default
 *   />
 * @endcode
 */
ZFTYPEID_DECLARE(ZFUISizeParam, ZFUISizeParam)

ZFOUTPUT_TYPE(ZFUISizeParam, {output << ZFUISizeParamToString(v);})

ZFCORE_POD_COMPARER_DECLARE(ZFUISizeParam)

/**
 * @brief make a ZFUISizeParam
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFUISizeParam, ZFUISizeParamMake,
                               ZFMP_IN(ZFUISizeTypeEnum const &, width),
                               ZFMP_IN(ZFUISizeTypeEnum const &, height))
{
    ZFUISizeParam ret = {width, height};
    return ret;
}
/**
 * @brief make a ZFUISizeParam
 */
ZFMETHOD_FUNC_INLINE_DECLARE_1(ZFUISizeParam, ZFUISizeParamMake,
                               ZFMP_IN(ZFUISizeTypeEnum const &, v))
{
    ZFUISizeParam ret = {v, v};
    return ret;
}

/**
 * @brief #ZFUISizeParamMake(#ZFUISizeType::e_Wrap, #ZFUISizeType::e_Wrap)
 */
ZFEXPORT_VAR_READONLY_DECLARE(ZFUISizeParam, ZFUISizeParamZero)
/**
 * @brief #ZFUISizeParamMake(#ZFUISizeType::e_Wrap, #ZFUISizeType::e_Wrap)
 */
ZFEXPORT_VAR_READONLY_DECLARE(ZFUISizeParam, ZFUISizeParamWrapWrap)
/**
 * @brief #ZFUISizeParamMake(#ZFUISizeType::e_Wrap, #ZFUISizeType::e_Fill)
 */
ZFEXPORT_VAR_READONLY_DECLARE(ZFUISizeParam, ZFUISizeParamWrapFill)
/**
 * @brief #ZFUISizeParamMake(#ZFUISizeType::e_Fill, #ZFUISizeType::e_Wrap)
 */
ZFEXPORT_VAR_READONLY_DECLARE(ZFUISizeParam, ZFUISizeParamFillWrap)
/**
 * @brief #ZFUISizeParamMake(#ZFUISizeType::e_Fill, #ZFUISizeType::e_Fill)
 */
ZFEXPORT_VAR_READONLY_DECLARE(ZFUISizeParam, ZFUISizeParamFillFill)

// ============================================================
// ZFUILayoutParam
/**
 * @brief base class of all layout param in ZFUIView
 *
 * ZFUIView supply basic but useful dynamic layout mechanism,
 * which use align, margin and measure logic to supply dynamic layout\n
 * while layout a child, ZFUIView would do those things:
 * -  measure child to see it's size depends on #sizeParam
 * -  layout child depends on #ZFUIAlignApply
 *
 * you should always use #ZFUIView::layoutParamCreate to create a layout param\n
 * subclass may override or define new params to supply more complex layout mechanism\n
 * \n
 * dynamic layout logic depends on #ZFUILayoutParam::sizeHint and #ZFUILayoutParam::sizeParam
 *
 * @note all layout param must be declared as #ZFProperty,
 *   and must be serializable
 * @note by default, a layout param would be compared by comparing all property
 */
zfclass ZF_ENV_EXPORT ZFUILayoutParam : zfextends ZFStyleableObject
{
    ZFOBJECT_DECLARE(ZFUILayoutParam, ZFStyleableObject)

public:
    // ============================================================
    // events
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when layout param changed,
     * this event would be automatically fired if any of property value changed
     * (#ZFObject::EventObjectPropertyValueOnUpdate),
     * you may also notify it manually to show the layout param changed
     */
    ZFOBSERVER_EVENT(LayoutParamOnChange)


public:
    // ============================================================
    /**
     * @brief see #ZFUILayoutParam, default is #ZFUISizeInvalid
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUISize, sizeHint,
                                ZFUISizeInvalid())
    /**
     * @brief see #ZFUILayoutParam, default is #ZFUISizeParamWrapWrap
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUISizeParam, sizeParam,
                                ZFUISizeParamWrapWrap())
    /**
     * @brief see #ZFUILayoutParam, default is (ZFUIAlign::e_LeftInner | ZFUIAlign::e_TopInner)
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIAlignFlags, layoutAlign,
                                ZFUIAlign::e_LeftInner | ZFUIAlign::e_TopInner)
    /**
     * @brief see #ZFUILayoutParam, default is (0, 0, 0, 0)
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIMargin, layoutMargin,
                                ZFUIMarginZero())

public:
    // ============================================================
    /** @brief see #layoutParamApply */
    ZFMETHOD_DECLARE_STATIC_4(void, layoutParamApply,
                              ZFMP_OUT(ZFUIRect &, ret),
                              ZFMP_IN(const ZFUIRect &, rect),
                              ZFMP_IN(ZFUIView *, child),
                              ZFMP_IN(ZFUILayoutParam *, lp))
    /**
     * @brief calculate single child's frame using logic of #ZFUILayoutParam
     *
     * this is not necessary for subclass to declare its own one,
     * it's declared for convenient for subclass to layout child
     * using parent's layout logic
     */
    ZFMETHOD_DECLARE_STATIC_3(ZFUIRect, layoutParamApply,
                              ZFMP_IN(const ZFUIRect &, rect),
                              ZFMP_IN(ZFUIView *, child),
                              ZFMP_IN(ZFUILayoutParam *, lp))

    /**
     * @brief util method to apply sizeHint according sizeParam
     */
    ZFMETHOD_DECLARE_STATIC_4(void, sizeHintApply,
                              ZFMP_OUT(zffloat &, ret),
                              ZFMP_IN(zffloat, size),
                              ZFMP_IN(zffloat, sizeHint),
                              ZFMP_IN(ZFUISizeTypeEnum, sizeParam))
    /** @brief see #sizeHintApply */
    ZFMETHOD_DECLARE_STATIC_3(zffloat, sizeHintApply,
                              ZFMP_IN(zffloat, size),
                              ZFMP_IN(zffloat, sizeHint),
                              ZFMP_IN(ZFUISizeTypeEnum, sizeParam))

    /**
     * @brief util method to apply sizeHint according sizeParam
     */
    ZFMETHOD_DECLARE_STATIC_4(void, sizeHintApply,
                              ZFMP_OUT(ZFUISize &, ret),
                              ZFMP_IN(const ZFUISize &, size),
                              ZFMP_IN(const ZFUISize &, sizeHint),
                              ZFMP_IN(const ZFUISizeParam &, sizeParam))
    /** @brief see #sizeHintApply */
    ZFMETHOD_DECLARE_STATIC_3(ZFUISize, sizeHintApply,
                              ZFMP_IN(const ZFUISize &, size),
                              ZFMP_IN(const ZFUISize &, sizeHint),
                              ZFMP_IN(const ZFUISizeParam &, sizeParam))

    /**
     * @brief merge two size hint
     */
    ZFMETHOD_DECLARE_STATIC_3(void, sizeHintMerge,
                              ZFMP_OUT(zffloat &, ret),
                              ZFMP_IN(zffloat, sizeHint0),
                              ZFMP_IN(zffloat, sizeHint1))
    /** @brief see #sizeHintMerge */
    ZFMETHOD_DECLARE_STATIC_2(zffloat, sizeHintMerge,
                              ZFMP_IN(zffloat, sizeHint0),
                              ZFMP_IN(zffloat, sizeHint1))

    /**
     * @brief merge two size hint
     */
    ZFMETHOD_DECLARE_STATIC_3(void, sizeHintMerge,
                              ZFMP_OUT(ZFUISize &, ret),
                              ZFMP_IN(const ZFUISize &, sizeHint0),
                              ZFMP_IN(const ZFUISize &, sizeHint1))
    /** @brief see #sizeHintMerge */
    ZFMETHOD_DECLARE_STATIC_2(ZFUISize, sizeHintMerge,
                              ZFMP_IN(const ZFUISize &, sizeHint0),
                              ZFMP_IN(const ZFUISize &, sizeHint1))

    /**
     * @brief safely increase or decrease size hint, do nothing if old one is no limit
     */
    ZFMETHOD_DECLARE_STATIC_3(void, sizeHintOffset,
                              ZFMP_OUT(zffloat &, ret),
                              ZFMP_IN(zffloat, sizeHint),
                              ZFMP_IN(zffloat, offset))
    /** @brief see #sizeHintOffset */
    ZFMETHOD_DECLARE_STATIC_2(zffloat, sizeHintOffset,
                              ZFMP_IN(zffloat, sizeHint),
                              ZFMP_IN(zffloat, offset))

    /**
     * @brief safely increase or decrease size hint, do nothing if old one is no limit
     */
    ZFMETHOD_DECLARE_STATIC_3(void, sizeHintOffset,
                              ZFMP_OUT(ZFUISize &, ret),
                              ZFMP_IN(const ZFUISize &, sizeHint),
                              ZFMP_IN(const ZFUISize &, offset))
    /** @brief see #sizeHintOffset */
    ZFMETHOD_DECLARE_STATIC_2(ZFUISize, sizeHintOffset,
                              ZFMP_IN(const ZFUISize &, sizeHint),
                              ZFMP_IN(const ZFUISize &, offset))

    /**
     * @brief safely increase or decrease size hint, do nothing if old one is no limit
     */
    ZFMETHOD_DECLARE_STATIC_3(void, sizeHintOffset,
                              ZFMP_OUT(ZFUISize &, ret),
                              ZFMP_IN(const ZFUISize &, sizeHint),
                              ZFMP_IN(zffloat, offset))
    /** @brief see #sizeHintOffset */
    ZFMETHOD_DECLARE_STATIC_2(ZFUISize, sizeHintOffset,
                              ZFMP_IN(const ZFUISize &, sizeHint),
                              ZFMP_IN(zffloat, offset))

public:
    zfoverride
    virtual zfidentity objectHash(void)
    {
        return zfidentityCalcString(this->classData()->classNameFull());
    }
    zfoverride
    virtual ZFCompareResult objectCompare(ZF_IN ZFObject *anotherObj)
    {
        if(anotherObj != zfnull && anotherObj->classData()->classIsTypeOf(zfself::ClassData())
            && ZFClassUtil::allPropertyIsEqual(this, anotherObj))
        {
            return ZFCompareTheSame;
        }
        return ZFCompareUncomparable;
    }
    zfoverride
    virtual inline void objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
    {
        zfsuper::objectInfoOnAppend(ret);
        ZFClassUtil::objectPropertyInfo(ret, this);
    }

protected:
    zfoverride
    virtual void objectPropertyValueOnUpdate(ZF_IN const ZFProperty *property, ZF_IN const void *oldValue)
    {
        zfsuper::objectPropertyValueOnUpdate(property, oldValue);
        if(oldValue != zfnull)
        {
            this->layoutParamOnChange();
        }
    }
protected:
    /** @brief see #EventLayoutParamOnChange */
    virtual inline void layoutParamOnChange(void)
    {
        this->observerNotify(ZFUILayoutParam::EventLayoutParamOnChange());
    }
};

// ============================================================
/**
 * @brief three layer for ZFUIView
 */
ZFENUM_BEGIN(ZFUIViewChildLayer)
    ZFENUM_VALUE(Normal) /**< @brief added by #ZFUIView::childAdd */
    ZFENUM_VALUE(InternalImpl) /**< @brief added by #ZFUIView::internalImplViewAdd */
    ZFENUM_VALUE(InternalBg) /**< @brief added by #ZFUIView::internalBgViewAdd */
    ZFENUM_VALUE(InternalFg) /**< @brief added by #ZFUIView::internalFgViewAdd */
ZFENUM_SEPARATOR(ZFUIViewChildLayer)
    ZFENUM_VALUE_REGISTER(Normal)
    ZFENUM_VALUE_REGISTER(InternalImpl)
    ZFENUM_VALUE_REGISTER(InternalBg)
    ZFENUM_VALUE_REGISTER(InternalFg)
ZFENUM_END(ZFUIViewChildLayer)

// ============================================================
// ZFUIViewMeasureResult
/**
 * @brief data used by #ZFUIView::EventViewLayoutOnMeasureFinish,
 *   you may modify the #ZFUIViewMeasureResult::measuredSize
 *   to override the measured result
 */
zfclass ZF_ENV_EXPORT ZFUIViewMeasureResult : zfextends ZFObject
{
    ZFOBJECT_DECLARE(ZFUIViewMeasureResult, ZFObject)

public:
    /** @brief see #ZFUIViewMeasureResult */
    ZFUISize sizeHint;
    /** @brief see #ZFUIViewMeasureResult */
    ZFUISizeParam sizeParam;
    /** @brief see #ZFUIViewMeasureResult */
    ZFUISize measuredSize;

    ZFALLOC_CACHE_RELEASE({
        cache->sizeHint = ZFUISizeZero();
        cache->sizeParam = ZFUISizeParamZero();
        cache->measuredSize = ZFUISizeZero();
    })

protected:
    zfoverride
    virtual inline void objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
    {
        zfsuper::objectInfoOnAppend(ret);
        ZFClassUtil::objectPropertyInfo(ret, this);
    }
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIViewType_h_

