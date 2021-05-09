#include "ZFUIAutoLayout.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassNotPOD _ZFP_ZFUIAutoLayoutPrivate
{
public:
    zffloat _parentWidth;
    zffloat _parentHeight;
    zfindex _cacheSize;
    ZFUIRect *_childFrame;
    zffloat *_childSizeCacheX;
    zffloat *_childSizeCacheY;
    zfbool *_layoutingX;
    zfbool *_layoutingY;
    zfbool *_layoutingWidth;
    zfbool *_layoutingHeight;
    zfbool *_layoutFinishX;
    zfbool *_layoutFinishY;
    zfbool *_layoutFinishWidth;
    zfbool *_layoutFinishHeight;
public:
    _ZFP_ZFUIAutoLayoutPrivate(void)
    : _parentWidth(0)
    , _parentHeight(0)
    , _cacheSize(0)
    , _childFrame(zfnull)
    , _childSizeCacheX(zfnull)
    , _childSizeCacheY(zfnull)
    , _layoutingX(zfnull)
    , _layoutingY(zfnull)
    , _layoutingWidth(zfnull)
    , _layoutingHeight(zfnull)
    , _layoutFinishX(zfnull)
    , _layoutFinishY(zfnull)
    , _layoutFinishWidth(zfnull)
    , _layoutFinishHeight(zfnull)
    {
    }
    ~_ZFP_ZFUIAutoLayoutPrivate(void)
    {
        zffree(_childFrame);
        zffree(_childSizeCacheX);
        zffree(_layoutingX);
    }

public:
    void prepareLayout(ZF_IN ZFUIView *parent,
                       ZF_IN const ZFUISize &sizeHint)
    {
        zfindex childCount = parent->childCount();
        if(_cacheSize != childCount)
        {
            _cacheSize = childCount;
            _childFrame = (ZFUIRect *)zfrealloc(_childFrame, sizeof(ZFUIRect) * childCount);
            _childSizeCacheX = (zffloat *)zfrealloc(_childSizeCacheX, sizeof(zffloat) * childCount * 2);
            _childSizeCacheY = _childSizeCacheX + childCount;
            _layoutingX = (zfbool *)zfrealloc(_layoutingX, sizeof(zfbool) * childCount * 8);
            _layoutingY         = _layoutingX + childCount * 1;
            _layoutingWidth     = _layoutingX + childCount * 2;
            _layoutingHeight    = _layoutingX + childCount * 3;
            _layoutFinishX      = _layoutingX + childCount * 4;
            _layoutFinishY      = _layoutingX + childCount * 5;
            _layoutFinishWidth  = _layoutingX + childCount * 6;
            _layoutFinishHeight = _layoutingX + childCount * 7;
        }
        zfmemset(_layoutingX, 0, sizeof(zfbool) * childCount * 8);

        for(zfindex i = childCount - 1; i != zfindexMax(); --i)
        {
            ZFUIView *child = parent->childAtIndex(i);
            ZFUIAutoLayoutParam *layoutParam = child->layoutParam<ZFUIAutoLayoutParam *>();
            ZFUISize childSize = child->layoutMeasure(
                ZFUILayoutParam::sizeHintMerge(sizeHint, layoutParam->sizeHint()),
                layoutParam->sizeParam());
            _childFrame[i].width = childSize.width;
            _childFrame[i].height = childSize.height;
        }
    }
    zfbool isFixedRule(ZF_IN const ZFUIAutoLayoutRule &rule)
    {
        return (rule.pos() != ZFUIAutoLayoutPos::e_None && rule.scale() != 0);
    }
    void layoutChild(ZF_IN ZFUIView *parent,
                     ZF_IN zfindex childIndex,
                     ZF_IN zfbool xAxis);
private:
    void updateChildSize(ZF_IN ZFUIView *parent,
                         ZF_IN zfindex childIndex,
                         ZF_IN const ZFUIAutoLayoutRule &rule,
                         ZF_IN zfbool xAxis);
    void updateChain(ZF_IN ZFUIView *parent,
                     ZF_IN_OUT ZFCoreArray<zfindex> &chain,
                     ZF_IN zfindex childIndex,
                     ZF_IN const ZFUIAutoLayoutRule *ruleList,
                     ZF_IN ZFUIAutoLayoutPosEnum posHead,
                     ZF_IN ZFUIAutoLayoutPosEnum posTail);
    void calcChildHead(ZF_IN ZFUIView *parent,
                       ZF_IN const ZFUIAutoLayoutRule *ruleList,
                       ZF_IN ZFUIAutoLayoutPosEnum posHead,
                       ZF_OUT zffloat &head,
                       ZF_IN zfbool xAxis);
    void calcChildTail(ZF_IN ZFUIView *parent,
                       ZF_IN const ZFUIAutoLayoutRule *ruleList,
                       ZF_IN ZFUIAutoLayoutPosEnum posTail,
                       ZF_OUT zffloat &tail,
                       ZF_IN zfbool xAxis);
    void layoutChildByChain(ZF_IN ZFUIView *parent,
                            ZF_IN const ZFCoreArray<zfindex> &chain,
                            ZF_IN zfbool xAxis);
    void layoutChildByRange(ZF_IN const ZFUIAutoLayoutRule &ruleHead,
                            ZF_IN const ZFUIAutoLayoutRule &ruleTail,
                            ZF_IN zffloat headPos,
                            ZF_IN zffloat childSizeCache,
                            ZF_IN zffloat bias,
                            ZF_IN_OUT zffloat &childHead,
                            ZF_IN_OUT zffloat &childSize);
};

// ============================================================
void ZFUIAutoLayout::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFUIAutoLayoutPrivate);
}
void ZFUIAutoLayout::objectOnDealloc(void)
{
    zfpoolDelete(d);
    zfsuper::objectOnDealloc();
}

void ZFUIAutoLayout::viewChildOnAdd(ZF_IN ZFUIView *child,
                                    ZF_IN ZFUIViewChildLayerEnum layer)
{
    zfsuper::viewChildOnAdd(child, layer);
    if(layer == ZFUIViewChildLayer::e_Normal)
    {
        d->_cacheSize = 0;
        ZFUIAutoLayoutParam *layoutParam = child->layoutParam<ZFUIAutoLayoutParam *>();
        zfCoreAssertWithMessageTrim(
            layoutParam->_ZFP_al_d.owner == zfnull,
            "[ZFUIAutoLayout] layout param %s already attached to %s",
            child->layoutParam()->objectInfo().cString(),
            layoutParam->_ZFP_al_d.owner->objectInfo().cString());
        layoutParam->_ZFP_al_d.owner = this;
    }
}
void ZFUIAutoLayout::viewChildOnRemove(ZF_IN ZFUIView *child,
                                       ZF_IN ZFUIViewChildLayerEnum layer)
{
    if(layer == ZFUIViewChildLayer::e_Normal)
    {
        ZFUIAutoLayoutParam::_ZFP_Data &t = child->layoutParam<ZFUIAutoLayoutParam *>()->_ZFP_al_d;
        for(zfindex i = t.refByRule.count() - 1; i != zfindexMax(); --i)
        {
            t.refByRule[i]->pos() = ZFUIAutoLayoutPos::e_None;
        }
        t.refByRule.removeAll();
        t.owner = zfnull;
        d->_cacheSize = 0;
    }
    zfsuper::viewChildOnRemove(child, layer);
}

// ============================================================
void ZFUIAutoLayout::layoutOnMeasure(ZF_OUT ZFUISize &ret,
                                     ZF_IN const ZFUISize &sizeHint,
                                     ZF_IN const ZFUISizeParam &sizeParam)
{
    d->prepareLayout(this, sizeHint);
    d->_parentWidth = 0;
    d->_parentHeight = 0;

    // we need to calculate twice
    // because some rule may depends on parent's size
    for(zfindex i = 0; i < 2; ++i)
    {
        for(zfindex i = this->childCount() - 1; i != zfindexMax(); --i)
        {
            d->layoutChild(this, i, zftrue);
            d->layoutChild(this, i, zffalse);
        }

        zffloat parentLeft = 0;
        zffloat parentTop = 0;
        zffloat parentRight = 0;
        zffloat parentBottom = 0;
        for(zfindex i = this->childCount() - 1; i != zfindexMax(); --i)
        {
            const ZFUIRect &childFrame = d->_childFrame[i];
            parentLeft = zfmMin(parentLeft, ZFUIRectGetLeft(childFrame));
            parentTop = zfmMin(parentTop, ZFUIRectGetTop(childFrame));
            parentRight = zfmMax(parentRight, ZFUIRectGetRight(childFrame));
            parentBottom = zfmMax(parentBottom, ZFUIRectGetBottom(childFrame));
        }
        d->_parentWidth = parentRight - parentLeft;
        d->_parentHeight = parentBottom - parentTop;
    }
    ret.width = d->_parentWidth;
    ret.height = d->_parentHeight;
}
void ZFUIAutoLayout::layoutOnLayout(ZF_IN const ZFUIRect &bounds)
{
    d->prepareLayout(this, ZFUIRectGetSize(this->viewFrame()));
    d->_parentWidth = bounds.width;
    d->_parentHeight = bounds.height;
    for(zfindex i = this->childCount() - 1; i != zfindexMax(); --i)
    {
        d->layoutChild(this, i, zftrue);
        d->layoutChild(this, i, zffalse);
    }
    for(zfindex i = this->childCount() - 1; i != zfindexMax(); --i)
    {
        this->childAtIndex(i)->viewFrame(d->_childFrame[i]);
    }
}

// ============================================================
void _ZFP_ZFUIAutoLayoutPrivate::layoutChild(ZF_IN ZFUIView *parent,
                                             ZF_IN zfindex childIndex,
                                             ZF_IN zfbool xAxis)
{
    zfbool *&_layouting = (xAxis ? _layoutingX : _layoutingY);
    zfbool *&_layoutFinish = (xAxis ? _layoutFinishX : _layoutFinishY);
    zfCoreAssertWithMessageTrim(!_layouting[childIndex],
        "[ZFUIAutoLayout] recursive rule detected, first on: %s, at index: %s",
        parent->childAtIndex(childIndex)->objectInfoOfInstance().cString(),
        zfsFromInt(childIndex).cString());
    if(_layoutFinish[childIndex])
    {
        return ;
    }
    _layouting[childIndex] = zftrue;

    ZFCoreArrayPOD<zfindex> chain;
    ZFUIAutoLayoutParam *layoutParam = parent->childAtIndex(childIndex)->layoutParam<ZFUIAutoLayoutParam *>();
    const ZFUIAutoLayoutRule *ruleList = layoutParam->_ZFP_al_d.ruleList;
    ZFUIAutoLayoutPosEnum posHead = xAxis ? ZFUIAutoLayoutPos::e_Left : ZFUIAutoLayoutPos::e_Top;
    ZFUIAutoLayoutPosEnum posTail = xAxis ? ZFUIAutoLayoutPos::e_Right : ZFUIAutoLayoutPos::e_Bottom;

    this->updateChildSize(parent, childIndex, ruleList[xAxis ? ZFUIAutoLayoutPos::e_Width : ZFUIAutoLayoutPos::e_Height], xAxis);

    this->updateChain(parent, chain, childIndex, ruleList, posHead, posTail);
    if(!chain.isEmpty())
    {
        this->layoutChildByChain(parent, chain, xAxis);
    }
    else
    {
        zffloat head = 0;
        zffloat tail = 0;
        this->calcChildHead(parent, ruleList, posHead, head, xAxis);
        this->calcChildTail(parent, ruleList, posTail, tail, xAxis);
        this->layoutChildByRange(
            ruleList[posHead],
            ruleList[posTail],
            head,
            tail - head,
            xAxis ? layoutParam->biasX() : layoutParam->biasY(),
            xAxis ? _childFrame[childIndex].x : _childFrame[childIndex].y,
            xAxis ? _childFrame[childIndex].width : _childFrame[childIndex].height);
    }

    _layouting[childIndex] = zffalse;
    _layoutFinish[childIndex] = zftrue;
}

void _ZFP_ZFUIAutoLayoutPrivate::updateChildSize(ZF_IN ZFUIView *parent,
                                                 ZF_IN zfindex childIndex,
                                                 ZF_IN const ZFUIAutoLayoutRule &rule,
                                                 ZF_IN zfbool xAxis)
{
    if(rule.pos() == ZFUIAutoLayoutPos::e_None)
    {
        return ;
    }

    zfbool *&_layouting = xAxis ? _layoutingWidth : _layoutingHeight;
    zfbool *&_layoutFinish = xAxis ? _layoutFinishWidth : _layoutFinishHeight;
    zfCoreAssertWithMessageTrim(!_layouting[childIndex],
        "[ZFUIAutoLayout] recursive rule detected, first on: %s, at index: %s",
        parent->childAtIndex(childIndex)->objectInfoOfInstance().cString(),
        zfsFromInt(childIndex).cString());
    if(_layoutFinish[childIndex])
    {
        return ;
    }
    _layouting[childIndex] = zftrue;

    zffloat &_childSize = xAxis ? _childFrame[childIndex].width : _childFrame[childIndex].height;
    zffloat scale = (rule.scale() > 0 ? rule.scale() : 1);
    if(rule.targetPos() == ZFUIAutoLayoutPos::e_Width)
    {
        if(rule.target() == parent)
        {
            _childSize = zfmMax(_childSize, (zffloat)(_parentWidth * scale + rule.offset()));
        }
        else
        {
            zfindex targetIndex = parent->childArray().find(rule.target());
            this->updateChildSize(
                parent,
                targetIndex,
                rule.target()->layoutParam<ZFUIAutoLayoutParam *>()->_ZFP_al_d.ruleList[ZFUIAutoLayoutPos::e_Width],
                zftrue);
            _childSize = zfmMax(_childSize, (zffloat)(_childFrame[targetIndex].width * scale + rule.offset()));
        }
    }
    else if(rule.targetPos() == ZFUIAutoLayoutPos::e_Height)
    {
        if(rule.target() == parent)
        {
            _childSize = zfmMax(_childSize, (zffloat)(_parentHeight * scale + rule.offset()));
        }
        else
        {
            zfindex targetIndex = parent->childArray().find(rule.target());
            this->updateChildSize(
                parent,
                targetIndex,
                rule.target()->layoutParam<ZFUIAutoLayoutParam *>()->_ZFP_al_d.ruleList[ZFUIAutoLayoutPos::e_Height],
                zftrue);
            _childSize = zfmMax(_childSize, (zffloat)(_childFrame[targetIndex].height * scale + rule.offset()));
        }
    }

    _layouting[childIndex] = zftrue;
    _layoutFinish[childIndex] = zftrue;
}

void _ZFP_ZFUIAutoLayoutPrivate::updateChain(ZF_IN ZFUIView *parent,
                                             ZF_IN_OUT ZFCoreArray<zfindex> &chain,
                                             ZF_IN zfindex childIndex,
                                             ZF_IN const ZFUIAutoLayoutRule *ruleList,
                                             ZF_IN ZFUIAutoLayoutPosEnum posHead,
                                             ZF_IN ZFUIAutoLayoutPosEnum posTail)
{
    const ZFUIAutoLayoutRule *ruleListCur = ruleList;
    const ZFUIAutoLayoutRule *ruleListNext = ruleList;
    ZFUIView *childPrev = parent->childAtIndex(childIndex);
    zfbool centerChildHasAdd = zffalse;

    // head
    do
    {
        if(ruleListCur[posHead].pos() == ZFUIAutoLayoutPos::e_None || ruleListCur[posHead].target() == parent)
        {
            break;
        }
        ruleListNext = ruleListCur[posHead].target()->layoutParam<ZFUIAutoLayoutParam *>()->_ZFP_al_d.ruleList;
        if(ruleListNext[posTail].pos() != ZFUIAutoLayoutPos::e_None && ruleListNext[posTail].target() == childPrev)
        {
            if(!centerChildHasAdd)
            {
                centerChildHasAdd = zftrue;
                chain.add(0, parent->childArray().find(childPrev));
            }
            childPrev = ruleListCur[posHead].target();
            ruleListCur = ruleListNext;
            chain.add(0, parent->childArray().find(childPrev));
        }
        else
        {
            break;
        }
    } while(zftrue);

    // tail
    ruleListCur = ruleList;
    childPrev = parent->childAtIndex(childIndex);
    do
    {
        if(ruleListCur[posTail].pos() == ZFUIAutoLayoutPos::e_None || ruleListCur[posTail].target() == parent)
        {
            break;
        }
        ruleListNext = ruleListCur[posTail].target()->layoutParam<ZFUIAutoLayoutParam *>()->_ZFP_al_d.ruleList;
        if(ruleListNext[posHead].pos() != ZFUIAutoLayoutPos::e_None && ruleListNext[posHead].target() == childPrev)
        {
            if(!centerChildHasAdd)
            {
                centerChildHasAdd = zftrue;
                chain.add(parent->childArray().find(childPrev));
            }
            childPrev = ruleListCur[posTail].target();
            ruleListCur = ruleListNext;
            chain.add(parent->childArray().find(childPrev));
        }
        else
        {
            break;
        }
    } while(zftrue);

    // check
    if(chain.count() <= 1)
    {
        chain.removeAll();
        return ;
    }
    while(!chain.isEmpty()
        && this->isFixedRule(parent->childAtIndex(chain.getLast())->layoutParam<ZFUIAutoLayoutParam *>()->_ZFP_al_d.ruleList[posTail]))
    {
        chain.removeLast();
    }
    while(!chain.isEmpty()
        && this->isFixedRule(parent->childAtIndex(chain.getFirst())->layoutParam<ZFUIAutoLayoutParam *>()->_ZFP_al_d.ruleList[posHead]))
    {
        chain.removeFirst();
    }
    if(chain.count() == 1)
    {
        chain.removeAll();
    }
}

void _ZFP_ZFUIAutoLayoutPrivate::calcChildHead(ZF_IN ZFUIView *parent,
                                               ZF_IN const ZFUIAutoLayoutRule *ruleList,
                                               ZF_IN ZFUIAutoLayoutPosEnum posHead,
                                               ZF_OUT zffloat &head,
                                               ZF_IN zfbool xAxis)
{
    head = 0;
    if(ruleList[posHead].pos() != ZFUIAutoLayoutPos::e_None)
    {
        if(ruleList[posHead].target() != parent)
        {
            zfindex targetIndex = parent->childArray().find(ruleList[posHead].target());
            this->layoutChild(parent, targetIndex, xAxis);
            if(ruleList[posHead].targetPos() == posHead)
            {
                head = xAxis ? ZFUIRectGetLeft(_childFrame[targetIndex]) : ZFUIRectGetTop(_childFrame[targetIndex]);
            }
            else
            {
                head = xAxis ? ZFUIRectGetRight(_childFrame[targetIndex]) : ZFUIRectGetBottom(_childFrame[targetIndex]);
            }
        }
    }
}
void _ZFP_ZFUIAutoLayoutPrivate::calcChildTail(ZF_IN ZFUIView *parent,
                                               ZF_IN const ZFUIAutoLayoutRule *ruleList,
                                               ZF_IN ZFUIAutoLayoutPosEnum posTail,
                                               ZF_OUT zffloat &tail,
                                               ZF_IN zfbool xAxis)
{
    tail = xAxis ? _parentWidth : _parentHeight;
    if(ruleList[posTail].pos() != ZFUIAutoLayoutPos::e_None)
    {
        if(ruleList[posTail].target() != parent)
        {
            zfindex targetIndex = parent->childArray().find(ruleList[posTail].target());
            this->layoutChild(parent, targetIndex, xAxis);
            if(ruleList[posTail].targetPos() == posTail)
            {
                tail = xAxis ? ZFUIRectGetRight(_childFrame[targetIndex]) : ZFUIRectGetBottom(_childFrame[targetIndex]);
            }
            else
            {
                tail = xAxis ? ZFUIRectGetLeft(_childFrame[targetIndex]) : ZFUIRectGetTop(_childFrame[targetIndex]);
            }
        }
    }
}

void _ZFP_ZFUIAutoLayoutPrivate::layoutChildByChain(ZF_IN ZFUIView *parent,
                                                    ZF_IN const ZFCoreArray<zfindex> &chain,
                                                    ZF_IN zfbool xAxis)
{
    ZFUIAutoLayoutPosEnum posHead = xAxis ? ZFUIAutoLayoutPos::e_Left : ZFUIAutoLayoutPos::e_Top;
    ZFUIAutoLayoutPosEnum posTail = xAxis ? ZFUIAutoLayoutPos::e_Right : ZFUIAutoLayoutPos::e_Bottom;
    ZFUIAutoLayoutPosEnum posSize = xAxis ? ZFUIAutoLayoutPos::e_Width : ZFUIAutoLayoutPos::e_Height;
    zffloat head = 0;
    zffloat tail = 0;
    this->calcChildHead(parent,
        parent->childAtIndex(chain.getFirst())->layoutParam<ZFUIAutoLayoutParam *>()->_ZFP_al_d.ruleList,
        posHead, head, xAxis);
    this->calcChildTail(parent,
        parent->childAtIndex(chain.getLast())->layoutParam<ZFUIAutoLayoutParam *>()->_ZFP_al_d.ruleList,
        posTail, tail, xAxis);

    zffloat fixedSize = 0;
    // note the flexibleChildCount must be signed int type for safe divide with negative value
    zfint flexibleChildCount = 0;
    zffloat *&_childSizeCache = xAxis ? _childSizeCacheX : _childSizeCacheY;
    for(zfindex i = chain.count() - 1; i != zfindexMax(); --i)
    {
        zfindex childIndex = chain[i];
        const ZFUIAutoLayoutRule *ruleList = parent->childAtIndex(childIndex)->layoutParam<ZFUIAutoLayoutParam *>()->_ZFP_al_d.ruleList;
        _childSizeCache[childIndex] = xAxis ? _childFrame[childIndex].width : _childFrame[childIndex].height;
        if(ruleList[posHead].pos() != ZFUIAutoLayoutPos::e_None)
        {
            _childSizeCache[childIndex] += ruleList[posHead].offset();
        }
        if(ruleList[posTail].pos() != ZFUIAutoLayoutPos::e_None)
        {
            _childSizeCache[childIndex] += ruleList[posTail].offset();
        }
        fixedSize += _childSizeCache[childIndex];
        if(!this->isFixedRule(ruleList[posHead])
            || !this->isFixedRule(ruleList[posTail])
            || !this->isFixedRule(ruleList[posSize]))
        {
            ++flexibleChildCount;
        }
    }
    zffloat flexibleUnit = 0;
    if(flexibleChildCount > 0)
    {
        flexibleUnit = (tail - head - fixedSize) / flexibleChildCount;
    }

    zffloat tailTmp = tail;
    for(zfindex i = chain.count() - 1; i != zfindexMax(); --i)
    {
        zfindex childIndex = chain[i];
        ZFUIAutoLayoutParam *layoutParam = parent->childAtIndex(childIndex)->layoutParam<ZFUIAutoLayoutParam *>();
        const ZFUIAutoLayoutRule *ruleList = layoutParam->_ZFP_al_d.ruleList;
        if(!this->isFixedRule(ruleList[posHead])
            || !this->isFixedRule(ruleList[posTail])
            || !this->isFixedRule(ruleList[posSize]))
        {
            _childSizeCache[childIndex] += flexibleUnit;
        }
        tailTmp -= _childSizeCache[childIndex];
        this->layoutChildByRange(
            ruleList[posHead],
            ruleList[posTail],
            tailTmp,
            _childSizeCache[childIndex],
            xAxis ? layoutParam->biasX() : layoutParam->biasY(),
            xAxis ? _childFrame[childIndex].x : _childFrame[childIndex].y,
            xAxis ? _childFrame[childIndex].width : _childFrame[childIndex].height);
    }
}

void _ZFP_ZFUIAutoLayoutPrivate::layoutChildByRange(ZF_IN const ZFUIAutoLayoutRule &ruleHead,
                                                    ZF_IN const ZFUIAutoLayoutRule &ruleTail,
                                                    ZF_IN zffloat headPos,
                                                    ZF_IN zffloat childSizeCache,
                                                    ZF_IN zffloat bias,
                                                    ZF_IN_OUT zffloat &childHead,
                                                    ZF_IN_OUT zffloat &childSize)
{
    typedef enum {
        _RuleTypeCenter,
        _RuleTypeFlexibleSize,
        _RuleTypeHead,
        _RuleTypeTail,
    } _RuleType;
    _RuleType t = _RuleTypeCenter;
    if(ruleHead.pos() == ZFUIAutoLayoutPos::e_None)
    {
        if(ruleTail.pos() == ZFUIAutoLayoutPos::e_None)
        { // <none, none>
            t = _RuleTypeCenter;
        }
        else
        { // <none, flexible> or <none, fixed>
            t = _RuleTypeTail;
        }
    }
    else
    {
        if(ruleHead.scale() == 0)
        {
            if(ruleTail.pos() == ZFUIAutoLayoutPos::e_None)
            { // <flexible, none>
                t = _RuleTypeHead;
            }
            else if(ruleTail.scale() == 0)
            { // <flexible, flexible>
                t = _RuleTypeCenter;
            }
            else
            { // <flexible, fixed>
                t = _RuleTypeTail;
            }
        }
        else
        {
            if(ruleTail.pos() != ZFUIAutoLayoutPos::e_None && ruleTail.scale() != 0)
            { // <fixed, fixed>
                t = _RuleTypeFlexibleSize;
            }
            else
            { // <fixed, none> or <fixed, flexible>
                t = _RuleTypeHead;
            }
        }
    }
    switch(t)
    {
        case _RuleTypeCenter:
            childHead = headPos + ruleHead.offset() + (childSizeCache - ruleHead.offset() - ruleTail.offset() - childSize) * bias;
            break;
        case _RuleTypeFlexibleSize:
        {
            zffloat tmp = childSizeCache - ruleHead.offset() - ruleTail.offset();
            if(tmp > childSize)
            {
                childSize = tmp;
            }
            childHead = headPos + ruleHead.offset() + (tmp - childSize) / 2;
            break;
        }
        case _RuleTypeHead:
            childHead = headPos + ruleHead.offset();
            break;
        case _RuleTypeTail:
            childHead = headPos + childSizeCache - ruleTail.offset() - childSize;
            break;
    }
}

ZF_NAMESPACE_GLOBAL_END

