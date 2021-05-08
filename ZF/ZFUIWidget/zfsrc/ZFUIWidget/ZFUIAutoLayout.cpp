#include "ZFUIAutoLayout.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// misc
ZFENUM_DEFINE(ZFUIAutoLayoutPos)
ZFTYPEID_ACCESS_ONLY_DEFINE_UNCOMPARABLE(ZFUIAutoLayoutRule, ZFUIAutoLayoutRule)

// ============================================================
// ZFUIAutoLayoutParam
ZFOBJECT_REGISTER(ZFUIAutoLayoutParam)

ZFCompareResult ZFUIAutoLayoutParam::objectCompare(ZF_IN ZFObject *anotherObj)
{
    ZFUIAutoLayoutParam *ref = ZFCastZFObject(ZFUIAutoLayoutParam *, anotherObj);
    if(anotherObj == zfnull || zfsuper::objectCompare(anotherObj) != ZFCompareTheSame)
    {
        return ZFCompareUncomparable;
    }
    for(zfindex i = ZFUIAutoLayoutParam::_ZFP_PosBegin(); i < ZFUIAutoLayoutPos::ZFEnumCount; ++i)
    {
        if(_ZFP_al_d.ruleList[i] != ref->_ZFP_al_d.ruleList[i])
        {
            return ZFCompareUncomparable;
        }
    }
    return ZFCompareTheSame;
}

zfbool ZFUIAutoLayoutParam::serializableOnSerializeFromData(ZF_IN const ZFSerializableData &serializableData,
                                                            ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */,
                                                            ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */)
{
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeFromData(serializableData, outErrorHint, outErrorPos)) {return zffalse;}

    // remove all rule
    for(zfindex i = ZFUIAutoLayoutParam::_ZFP_PosBegin(); i < ZFUIAutoLayoutPos::ZFEnumCount; ++i)
    {
        _ZFP_al_d.ruleList[i].pos(ZFUIAutoLayoutPos::e_None);
    }

    for(zfindex i = 0; i < serializableData.elementCount(); ++i)
    {
        const ZFSerializableData &element = serializableData.elementAtIndex(i);
        if(element.resolved()
            || ZFSerializableUtil::checkItemClass(element, ZFSerializableKeyword_ZFUIAutoLayoutParam_rule) == zfnull)
        {
            continue;
        }

        ZFUIAutoLayoutPosEnum pos = ZFUIAutoLayoutPos::e_None;

        // pos
        ZFSerializableUtilSerializeAttributeFromData(element, outErrorHint, outErrorPos,
            require, ZFSerializableKeyword_ZFUIAutoLayoutParam_pos, ZFUIAutoLayoutPosEnum, pos);
        if(pos == ZFUIAutoLayoutPos::e_None)
        {
            ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, element,
                "\"%s\" does not support \"%s\"",
                ZFSerializableKeyword_ZFUIAutoLayoutParam_pos,
                ZFUIAutoLayoutPosEnumToString(pos).cString());
            return zffalse;
        }
        if(_ZFP_al_d.ruleList[pos].pos() != ZFUIAutoLayoutPos::e_None)
        {
            ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, element,
                "\"%s\" rule already has been set",
                ZFUIAutoLayoutPosEnumToString(pos).cString());
            return zffalse;
        }

        ZFUIAutoLayoutRule &rule = _ZFP_al_d.ruleList[pos];

        // target/targetPos
        {
            const zfchar *value = ZFSerializableUtil::requireAttribute(
                element, ZFSerializableKeyword_ZFUIAutoLayoutParam_target, outErrorHint, outErrorPos);
            if(value == zfnull)
            {
                return zffalse;
            }
            const zfchar *valueTmp = value;
            while(*valueTmp != '\0' && *valueTmp != ZFSerializableKeyword_ZFUIAutoLayoutParam_target_token) {++valueTmp;}
            if(*valueTmp != ZFSerializableKeyword_ZFUIAutoLayoutParam_target_token
                || !ZFUIAutoLayoutPosEnumFromString(rule.targetPos(), value, valueTmp - value))
            {
                ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, element,
                    "invalid target value: %s",
                    value);
                return zffalse;
            }
            rule.targetId() = valueTmp + 1;
        }

        // scale
        ZFSerializableUtilSerializeAttributeFromData(element, outErrorHint, outErrorPos,
            check, ZFSerializableKeyword_ZFUIAutoLayoutParam_scale, zffloat, rule.scale());

        // offset
        ZFSerializableUtilSerializeAttributeFromData(element, outErrorHint, outErrorPos,
            check, ZFSerializableKeyword_ZFUIAutoLayoutParam_offset, zffloat, rule.offset());

        // finish
        element.resolveMark();
        rule.pos(pos);
    }
    return zftrue;
}
zfbool ZFUIAutoLayoutParam::serializableOnSerializeToData(ZF_IN_OUT ZFSerializableData &serializableData,
                                                          ZF_IN ZFSerializable *referencedOwnerOrNull,
                                                          ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */)
{
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeToData(serializableData, referencedOwnerOrNull, outErrorHint)) {return zffalse;}
    zfself *ref = ZFCastZFObject(zfself *, referencedOwnerOrNull);

    for(zfindex i = ZFUIAutoLayoutParam::_ZFP_PosBegin(); i < ZFUIAutoLayoutPos::ZFEnumCount; ++i)
    {
        const ZFUIAutoLayoutRule &rule = _ZFP_al_d.ruleList[i];
        if(ref != zfnull && ref->_ZFP_al_d.ruleList[i] == rule)
        {
            continue;
        }
        if(rule.pos() == ZFUIAutoLayoutPos::e_None)
        {
            continue;
        }
        ZFSerializableData element;
        zfstring value;

        // pos
        value.removeAll();
        ZFUIAutoLayoutPosEnumToString(value, rule.pos());
        element.attributeForName(ZFSerializableKeyword_ZFUIAutoLayoutParam_pos, value);

        // target
        {
            zfstring target;
            ZFUIAutoLayoutPosEnumToString(target, rule.targetPos());
            target += ZFSerializableKeyword_ZFUIAutoLayoutParam_target_token;
            target += rule.targetId();
            element.attributeForName(ZFSerializableKeyword_ZFUIAutoLayoutParam_target, target);
        }

        // scale
        ZFSerializableUtilSerializeAttributeToData(element, outErrorHint, ref,
            ZFSerializableKeyword_ZFUIAutoLayoutParam_scale, zffloat, rule.scale(), 0, 0);

        // offset
        ZFSerializableUtilSerializeAttributeToData(element, outErrorHint, ref,
            ZFSerializableKeyword_ZFUIAutoLayoutParam_offset, zffloat, rule.offset(), 0, 0);

        element.itemClass(ZFSerializableKeyword_ZFUIAutoLayoutParam_rule);
        serializableData.elementAdd(element);
    }

    return zftrue;
}

// ============================================================
// ZFUIAutoLayout
ZFSTYLE_DEFAULT_DEFINE(ZFUIAutoLayout)
ZFOBJECT_REGISTER(ZFUIAutoLayout)

ZFMETHOD_DEFINE_3(ZFUIAutoLayout, void, rule,
                  ZFMP_IN(ZFUIView *, child),
                  ZFMP_IN(ZFUIAutoLayoutPosEnum, pos),
                  ZFMP_IN(const ZFUIAutoLayoutRule &, rule))
{
    zfCoreAssert(pos != ZFUIAutoLayoutPos::e_None);
    zfCoreAssert(rule.targetPos() != ZFUIAutoLayoutPos::e_None);

    ZFUIAutoLayoutParam *layoutParam = child->layoutParam<ZFUIAutoLayoutParam *>();
    ZFUIAutoLayoutRule &ruleRef = layoutParam->implRule(pos);
    ruleRef = rule;
    ruleRef.pos(pos);
    if(ruleRef.target() == zfnull)
    {
        ruleRef.target() = this;
    }
    this->_ZFP_updateTargetId(child, ruleRef);
    this->layoutRequest();

    if(ruleRef.target() != this)
    {
        ruleRef.target()->layoutParam<ZFUIAutoLayoutParam *>()->_ZFP_al_d.refByRule.add(&ruleRef);
    }
}
ZFMETHOD_DEFINE_2(ZFUIAutoLayout, zfbool, ruleExist,
                  ZFMP_IN(ZFUIView *, child),
                  ZFMP_IN(ZFUIAutoLayoutPosEnum, pos))
{
    return (child->layoutParam<ZFUIAutoLayoutParam *>()->implRule(pos).pos() != ZFUIAutoLayoutPos::e_None);
}
ZFMETHOD_DEFINE_2(ZFUIAutoLayout, void, ruleRemove,
                  ZFMP_IN(ZFUIView *, child),
                  ZFMP_IN(ZFUIAutoLayoutPosEnum, pos))
{
    if(this->ruleExist(child, pos))
    {
        ZFUIAutoLayoutRule &ruleRef = child->layoutParam<ZFUIAutoLayoutParam *>()->implRule(pos);
        ruleRef.pos(ZFUIAutoLayoutPos::e_None);
        this->layoutRequest();

        if(ruleRef.target() != this)
        {
            ruleRef.target()->layoutParam<ZFUIAutoLayoutParam *>()->_ZFP_al_d.refByRule.removeElement(&ruleRef);
        }
    }
}
ZFMETHOD_DEFINE_1(ZFUIAutoLayout, void, ruleRemoveAll,
                  ZFMP_IN(ZFUIView *, child))
{
    ZFUIAutoLayoutRule *ruleList = child->layoutParam<ZFUIAutoLayoutParam *>()->_ZFP_al_d.ruleList;
    for(zfindex i = ZFUIAutoLayoutParam::_ZFP_PosBegin(); i < ZFUIAutoLayoutPos::ZFEnumCount; ++i)
    {
        if(ruleList[i].pos() != ZFUIAutoLayoutPos::e_None)
        {
            ruleList[i].pos(ZFUIAutoLayoutPos::e_None);
            this->layoutRequest();

            if(ruleList[i].target() != this)
            {
                ruleList[i].target()->layoutParam<ZFUIAutoLayoutParam *>()->_ZFP_al_d.refByRule.removeElement(&(ruleList[i]));
            }
        }
    }
}

// ============================================================
// override ZFUIView
zfbool ZFUIAutoLayout::serializableOnSerializeFromData(ZF_IN const ZFSerializableData &serializableData,
                                                       ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */,
                                                       ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */)
{
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeFromData(serializableData, outErrorHint, outErrorPos)) {return zffalse;}
    for(zfindex childIndex = 0; childIndex < this->childCount(); ++childIndex)
    {
        ZFUIView *child = this->childAtIndex(childIndex);
        ZFUIAutoLayoutParam *layoutParam = child->layoutParam<ZFUIAutoLayoutParam *>();
        for(zfindex ruleIndex = ZFUIAutoLayoutParam::_ZFP_PosBegin(); ruleIndex < ZFUIAutoLayoutPos::ZFEnumCount; ++ruleIndex)
        {
            if(!this->_ZFP_updateTarget(child, layoutParam->_ZFP_al_d.ruleList[ruleIndex]))
            {
                ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
                    "%s invalid target \"%s\" for %s",
                    this->objectInfo().cString(),
                    layoutParam->_ZFP_al_d.ruleList[ruleIndex].targetId().cString(),
                    this->childAtIndex(childIndex)->objectInfo().cString());
                return zffalse;
            }
        }
    }
    return zftrue;
}

zfbool ZFUIAutoLayout::_ZFP_updateTarget(ZF_IN ZFUIView *child, ZF_IN_OUT ZFUIAutoLayoutRule &rule)
{
    if(rule.pos() == ZFUIAutoLayoutPos::e_None)
    {
        return zftrue;
    }
    rule.target() = zfnull;
    if(!rule.targetId().isEmpty())
    {
        if(rule.targetId()[0] == '@')
        {
            rule.target() = this->childFindById(rule.targetId().cString() + 1, zfHint("findRecursively")zffalse);
        }
        else if(zfscmpTheSame(rule.targetId().cString(), ZFSerializableKeyword_ZFUIAutoLayoutParam_target_parent))
        {
            rule.target() = this;
        }
        else if(zfscmpTheSame(rule.targetId().cString(), ZFSerializableKeyword_ZFUIAutoLayoutParam_target_self))
        {
            rule.target() = child;
        }
        else
        {
            zfindex childIndexRef = zfindexMax();
            if(zfindexFromString(childIndexRef, rule.targetId()) && childIndexRef < this->childCount())
            {
                rule.target() = this->childAtIndex(childIndexRef);
            }
        }
    }
    return (rule.target() != zfnull);
}
zfbool ZFUIAutoLayout::_ZFP_updateTargetId(ZF_IN ZFUIView *child, ZF_IN_OUT ZFUIAutoLayoutRule &rule)
{
    if(rule.target() == zfnull || (rule.target() != this && rule.target()->viewParent() != this))
    {
        return zffalse;
    }
    if(rule.target() == this)
    {
        rule.targetId() = ZFSerializableKeyword_ZFUIAutoLayoutParam_target_parent;
    }
    else if(!rule.target()->viewId().isEmpty())
    {
        rule.targetId() = rule.target()->viewId();
    }
    else if(rule.target() == child)
    {
        rule.targetId() = ZFSerializableKeyword_ZFUIAutoLayoutParam_target_self;
    }
    else
    {
        zfindex childIndexRef = this->childFind(rule.target());
        if(childIndexRef == zfindexMax())
        {
            return zffalse;
        }
        else
        {
            rule.targetId().removeAll();
            zfindexToString(rule.targetId(), childIndexRef);
        }
    }
    return zftrue;
}

ZF_NAMESPACE_GLOBAL_END

