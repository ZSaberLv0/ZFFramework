/**
 * @file ZFUIAutoLayout.h
 * @brief auto layout container with constraint
 */

#ifndef _ZFI_ZFUIAutoLayout_h_
#define _ZFI_ZFUIAutoLayout_h_

#include "ZFUIWidgetDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/** @brief see #ZFUIAutoLayout */
ZFENUM_BEGIN(ZFUIAutoLayoutPos)
    ZFENUM_VALUE(None)
    ZFENUM_VALUE(Width)
    ZFENUM_VALUE(Height)
    ZFENUM_VALUE(Left)
    ZFENUM_VALUE(Top)
    ZFENUM_VALUE(Right)
    ZFENUM_VALUE(Bottom)
ZFENUM_SEPARATOR(ZFUIAutoLayoutPos)
    ZFENUM_VALUE_REGISTER(None)
    ZFENUM_VALUE_REGISTER(Width)
    ZFENUM_VALUE_REGISTER(Height)
    ZFENUM_VALUE_REGISTER(Left)
    ZFENUM_VALUE_REGISTER(Top)
    ZFENUM_VALUE_REGISTER(Right)
    ZFENUM_VALUE_REGISTER(Bottom)
ZFENUM_END(ZFUIAutoLayoutPos)

/** @brief see #ZFUIAutoLayout */
zfclassLikePOD ZF_ENV_EXPORT ZFUIAutoLayoutRule
{
    ZFCORE_PARAM_DECLARE_SELF(ZFUIAutoLayoutRule)

public:
    /** @brief see #ZFUIAutoLayout */
    ZFCORE_PARAM_WITH_INIT(ZFUIAutoLayoutPosEnum, pos, ZFUIAutoLayoutPos::e_None)
    /** @brief see #ZFUIAutoLayout */
    ZFCORE_PARAM(zfstring, targetId)
    /** @brief see #ZFUIAutoLayout */
    ZFCORE_PARAM(ZFUIView *, target)
    /** @brief see #ZFUIAutoLayout */
    ZFCORE_PARAM_WITH_INIT(ZFUIAutoLayoutPosEnum, targetPos, ZFUIAutoLayoutPos::e_None)
    /** @brief see #ZFUIAutoLayout */
    ZFCORE_PARAM_WITH_INIT(zffloat, scale, 0)
    /** @brief see #ZFUIAutoLayout */
    ZFCORE_PARAM(zfint, offset)

public:
    /** @brief remove all contents */
    void removeAll(void)
    {
        this->pos() = ZFUIAutoLayoutPos::e_None;
        this->targetId().removeAll();
        this->target() = zfnull;
        this->targetPos() = ZFUIAutoLayoutPos::e_None;
        this->scale() = 0;
        this->offset() = 0;
    }

public:
    /** @cond ZFPrivateDoc */
    zfbool operator == (ZF_IN const ZFUIAutoLayoutRule &ref) const
    {
        return ((this->pos() == ZFUIAutoLayoutPos::e_None && ref.pos() == ZFUIAutoLayoutPos::e_None) || (zftrue
                && this->pos() == ref.pos()
                && this->target() == ref.target()
                && this->targetPos() == ref.targetPos()
                && this->scale() == ref.scale()
                && this->offset() == ref.offset()
            ));
    }
    inline zfbool operator != (ZF_IN const ZFUIAutoLayoutRule &ref) const
    {
        return !this->operator == (ref);
    }
    /** @endcond */
};
ZFTYPEID_ACCESS_ONLY_DECLARE(ZFUIAutoLayoutRule, ZFUIAutoLayoutRule)

// ============================================================
// ZFUIAutoLayoutParam
zfclassFwd ZFUIAutoLayout;
/** @brief see #ZFUIAutoLayout */
zfclass ZF_ENV_EXPORT ZFUIAutoLayoutParam : zfextends ZFUIViewLayoutParam
{
    ZFOBJECT_DECLARE(ZFUIAutoLayoutParam, ZFUIViewLayoutParam)

    /** @brief see #ZFUIAutoLayout */
    ZFPROPERTY_ASSIGN_WITH_INIT(zffloat, biasX, 0.5f)
    /** @brief see #ZFUIAutoLayout */
    ZFPROPERTY_ASSIGN_WITH_INIT(zffloat, biasY, 0.5f)

public:
    /**
     * @brief for impl only, to access the low level rule
     */
    inline ZFUIAutoLayoutRule &implRule(ZF_IN ZFUIAutoLayoutPosEnum pos)
    {
        return _ZFP_al_d.ruleList[pos];
    }

public:
    zfoverride
    virtual ZFCompareResult objectCompare(ZF_IN ZFObject *anotherObj);

protected:
    zfoverride
    virtual zfbool serializableOnSerializeFromData(ZF_IN const ZFSerializableData &serializableData,
                                                   ZF_OUT_OPT zfstring *outErrorHint = zfnull,
                                                   ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull);
    zfoverride
    virtual zfbool serializableOnSerializeToData(ZF_IN_OUT ZFSerializableData &serializableData,
                                                 ZF_IN ZFSerializable *referencedOwnerOrNull,
                                                 ZF_OUT_OPT zfstring *outErrorHint = zfnull);

public:
    static inline zfindex _ZFP_PosBegin(void) {return 1;}
public:
    zfclassLikePOD _ZFP_Data
    {
    public:
        ZFUIAutoLayout *owner;
        ZFCoreArrayPOD<ZFUIAutoLayoutRule *> refByRule;
        ZFUIAutoLayoutRule ruleList[ZFUIAutoLayoutPos::ZFEnumCount];
    public:
        _ZFP_Data(void) : owner(zfnull), refByRule(), ruleList() {}
    };
public:
    _ZFP_Data _ZFP_al_d;
};

/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUIAutoLayoutParam_rule "rule"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUIAutoLayoutParam_pos "pos"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUIAutoLayoutParam_target "target"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUIAutoLayoutParam_target_token ':'
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUIAutoLayoutParam_target_parent "parent"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUIAutoLayoutParam_target_self "self"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUIAutoLayoutParam_target_prev "prev"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUIAutoLayoutParam_scale "scale"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUIAutoLayoutParam_offset "offset"

// ============================================================
// ZFUIAutoLayout
zfclassFwd _ZFP_ZFUIAutoLayoutPrivate;
/**
 * @brief auto layout with contraint rule
 *
 * recommended to use #ZFUIAutoLayoutMaker to create rules\n
 * \n
 * for #ZFUIAutoLayout,
 * base class #ZFUIViewLayoutParam's params would take no effect\n
 * \n
 * serializable data:
 * @code
 *   <ZFUIAutoLayoutParam>
 *       <zffloat name="biasX" />   // optional, 0.5 by default
 *       <zffloat name="biasY" />   // optional, 0.5 by default
 *       <rule
 *           pos=""                 // required, #ZFUIAutoLayoutPos
 *           target=""              // required, rule target, see below
 *           scale=""               // optional, 0 by default
 *           offset=""              // optional, 0 by default
 *           />
 *       ... // other rules
 *   </ZFUIAutoLayoutParam>
 * @endcode
 *
 * rules:
 * -  `pos` : #ZFUIAutoLayoutPos shows the rule affect which position
 * -  `target` : target of the rule, it splits by two part, `Pos:Target`,
 *   while `Pos` is the value of #ZFUIAutoLayoutPos,
 *   and `Target` can be:
 *   -  `@viewId` while `viewId` is #ZFUIView::viewId,
 *     ref to the sibling with the view id
 *   -  `parent`, ref to parent
 *   -  `self`, ref to self
 *   -  `N` while `N` is a #zfindex value :
 *     ref to the sibling at the index
 * -  `scale` : #zffloat value, `0` by default
 *   -  for #ZFUIAutoLayoutPos::e_Width rule,
 *     `0` or negative value to show the width should be flexible,
 *     otherwise it's the scale value ref to the `target`
 *   -  for #ZFUIAutoLayoutPos::e_Left rule,
 *     `0` or negative value to show the rule should be flexible,
 *     otherwise shows the rule are fixed value\n
 *     note, if left or right rule not set,
 *     it's implicitly set to align to parent with flexible rule\n
 *     note, if both left/right and width have flexible rule,
 *     the left/right rule would have higher priority
 * -  `offset` : #zfint shows offset to `target`
 * -  `biasX` : if both left and right has flexible rule,
 *   align to which side, `0` means totally align to left,
 *   while `1` means align to right
 *
 *
 * during layout, a child has a list of positions (see #ZFUIAutoLayoutPos)
 * to apply rules to finally confirm the child's frame,
 * each position can have 0 or 1 rule
 */
zfclass ZF_ENV_EXPORT ZFUIAutoLayout : zfextends ZFUIView
{
    ZFOBJECT_DECLARE(ZFUIAutoLayout, ZFUIView)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIAutoLayout)

public:
    /**
     * @brief set rule
     */
    ZFMETHOD_DECLARE_3(void, rule,
                       ZFMP_IN(ZFUIView *, child),
                       ZFMP_IN(ZFUIAutoLayoutPosEnum, pos),
                       ZFMP_IN(const ZFUIAutoLayoutRule &, rule))
    /**
     * @brief whether the rule has set
     */
    ZFMETHOD_DECLARE_2(zfbool, ruleExist,
                       ZFMP_IN(ZFUIView *, child),
                       ZFMP_IN(ZFUIAutoLayoutPosEnum, pos))
    /**
     * @brief remove rule
     */
    ZFMETHOD_DECLARE_2(void, ruleRemove,
                       ZFMP_IN(ZFUIView *, child),
                       ZFMP_IN(ZFUIAutoLayoutPosEnum, pos))
    /**
     * @brief remove rule
     */
    ZFMETHOD_DECLARE_1(void, ruleRemoveAll,
                       ZFMP_IN(ZFUIView *, child))

    // ============================================================
    // override ZFUIView
protected:
    zfoverride
    virtual void viewChildOnAdd(ZF_IN ZFUIView *child,
                                ZF_IN ZFUIViewChildLayerEnum layer);
    zfoverride
    virtual void viewChildOnRemove(ZF_IN ZFUIView *child,
                                   ZF_IN ZFUIViewChildLayerEnum layer);

    zfoverride
    virtual const ZFClass *layoutParamClass(void)
    {
        return ZFUIAutoLayoutParam::ClassData();
    }

    zfoverride
    virtual void layoutOnMeasure(ZF_OUT ZFUISize &ret,
                                 ZF_IN const ZFUISize &sizeHint,
                                 ZF_IN const ZFUISizeParam &sizeParam);
    zfoverride
    virtual void layoutOnLayout(ZF_IN const ZFUIRect &bounds);

protected:
    zfoverride
    virtual zfbool serializableOnSerializeFromData(ZF_IN const ZFSerializableData &serializableData,
                                                   ZF_OUT_OPT zfstring *outErrorHint = zfnull,
                                                   ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull);

protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);

private:
    zfbool _ZFP_updateTarget(ZF_IN ZFUIView *child, ZF_IN_OUT ZFUIAutoLayoutRule &rule);
    zfbool _ZFP_updateTargetId(ZF_IN ZFUIView *child, ZF_IN_OUT ZFUIAutoLayoutRule &rule);
private:
    _ZFP_ZFUIAutoLayoutPrivate *d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIAutoLayout_h_

