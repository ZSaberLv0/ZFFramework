/**
 * @file ZFUILayoutParam.h
 * @brief layout param types for UI
 */

#ifndef _ZFI_ZFUILayoutParam_h_
#define _ZFI_ZFUILayoutParam_h_

#include "ZFUIKitDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUISizeType
/**
 * @brief size params for parent to define how to measure children,
 *   see #ZFUIViewLayoutParam for more info
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
 * @brief base class of all layout param in ZFFramework
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
    zfoverride
    virtual zfidentity objectHash(void)
    {
        return zfidentityCalcString(this->classData()->classNameFull());
    }
    zfoverride
    virtual ZFCompareResult objectCompare(ZF_IN ZFObject *anotherObj)
    {
        if(anotherObj != zfnull && anotherObj->classData()->classIsTypeOf(ZFUILayoutParam::ClassData())
            && ZFClassUtil::allPropertyIsEqual(this, anotherObj))
        {
            return ZFCompareTheSame;
        }
        return ZFCompareUncomparable;
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

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUILayoutParam_h_

