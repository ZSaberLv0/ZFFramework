#include "ZFUIAutoLayoutMaker.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassNotPOD _ZFP_ZFUIAutoLayoutMakerPrivate
{
public:
    zfuint refCount;
    ZFUIView *child;
    ZFUIAutoLayoutRule ruleList[ZFUIAutoLayoutPos::ZFEnumCount];
    ZFCoreArrayPOD<ZFUIAutoLayoutRule *> ruleAttached;
public:
    _ZFP_ZFUIAutoLayoutMakerPrivate(void)
    : refCount(1)
    , child(zfnull)
    , ruleList()
    , ruleAttached()
    {
    }
    ~_ZFP_ZFUIAutoLayoutMakerPrivate(void)
    {
        this->done();
    }
public:
    void zfal_maker(ZF_IN ZFUIView *child, ZF_IN_OPT ZFUIAutoLayout *parent = zfnull)
    {
        if(child->viewParent() == zfnull && parent != zfnull)
        {
            parent->childAdd(child);
        }

        zfCoreAssertWithMessageTrim(child != zfnull,
            "[ZFUIAutoLayoutMaker] null child");
        zfCoreAssertWithMessageTrim(child->viewParent() != zfnull,
            "[ZFUIAutoLayoutMaker] child must be added to %s",
            ZFUIAutoLayout::ClassData()->className());
        zfCoreAssertWithMessageTrim(child->viewParent() != zfnull,
            "[ZFUIAutoLayoutMaker] child's parent must be %s, got: %s",
            ZFUIAutoLayout::ClassData()->className(),
            child->viewParent()->objectInfoOfInstance().cString());
        this->done();
        this->child = child;
    }
    void done(void)
    {
        if(this->child == zfnull || this->ruleAttached.isEmpty())
        {
            return ;
        }
        zfCoreAssertWithMessageTrim(
            this->ruleAttached[0]->target() != zfnull,
            "[ZFUIAutoLayoutMaker] target rule not set (toLeft/toWidth etc)");
        ZFUIAutoLayout *parent = ZFCastZFObject(ZFUIAutoLayout *, this->child->viewParent());
        for(zfindex i = 0; i < this->ruleAttached.count(); ++i)
        {
            parent->rule(this->child, this->ruleAttached[i]->pos(), *(this->ruleAttached[i]));
            this->ruleAttached[i]->pos() = ZFUIAutoLayoutPos::e_None;
        }
        this->ruleAttached.removeAll();
    }

    void requireChild(void)
    {
        zfCoreAssertWithMessageTrim(
            this->child != zfnull,
            "[ZFUIAutoLayoutMaker] zfal_maker not called");
    }
    void requirePos(void)
    {
        zfCoreAssertWithMessageTrim(
            this->child != zfnull,
            "[ZFUIAutoLayoutMaker] zfal_maker not called");
        zfCoreAssertWithMessageTrim(
            !this->ruleAttached.isEmpty(),
            "[ZFUIAutoLayoutMaker] pos rule not set (left/width etc)");
    }
    void requireTarget(void)
    {
        zfCoreAssertWithMessageTrim(
            this->child != zfnull,
            "[ZFUIAutoLayoutMaker] zfal_maker not called");
        zfCoreAssertWithMessageTrim(
            !this->ruleAttached.isEmpty(),
            "[ZFUIAutoLayoutMaker] pos rule not set (left/width etc)");
        zfCoreAssertWithMessageTrim(
            this->ruleAttached[0]->target() != zfnull,
            "[ZFUIAutoLayoutMaker] target rule not set (toLeft/toWidth etc)");
    }

    void posAttach(ZF_IN ZFUIAutoLayoutPosEnum pos)
    {
        this->requireChild();
        if(!this->ruleAttached.isEmpty() && this->ruleAttached[0]->target() != zfnull)
        {
            this->done();
        }
        if(this->ruleList[pos].pos() == ZFUIAutoLayoutPos::e_None)
        {
            this->ruleList[pos].pos() = pos;
            this->ruleAttached.add(&(this->ruleList[pos]));
        }
        else
        {
            zfCoreCriticalMessageTrim(
                "pos rule specified twice: %s",
                ZFUIAutoLayoutPosEnumToString(pos).cString());
        }
    }
    void targetAttach(ZF_IN ZFUIView *target, ZFUIAutoLayoutPosEnum targetPos)
    {
        this->requirePos();
        if(target == zfnull)
        {
            target = this->child->viewParent();
        }
        for(zfindex i = this->ruleAttached.count() - 1; i != zfindexMax(); --i)
        {
            this->ruleAttached[i]->target() = target;
            this->ruleAttached[i]->targetPos() = targetPos;
        }
    }
    void targetAttach(ZF_IN ZFUIView *target)
    {
        this->requirePos();
        if(target == zfnull)
        {
            target = this->child->viewParent();
        }
        for(zfindex i = this->ruleAttached.count() - 1; i != zfindexMax(); --i)
        {
            this->ruleAttached[i]->target() = target;
            this->ruleAttached[i]->targetPos() = this->ruleAttached[i]->pos();
        }
    }
};

// ============================================================
ZFUIAutoLayoutMaker::ZFUIAutoLayoutMaker(void)
: d(zfnew(_ZFP_ZFUIAutoLayoutMakerPrivate))
{
}
ZFUIAutoLayoutMaker::ZFUIAutoLayoutMaker(ZF_IN ZFUIView *child)
: d(zfnew(_ZFP_ZFUIAutoLayoutMakerPrivate))
{
    d->zfal_maker(child);
}
ZFUIAutoLayoutMaker::ZFUIAutoLayoutMaker(ZF_IN ZFUIView *child,
                                         ZF_IN ZFUIAutoLayout *parent)
: d(zfnew(_ZFP_ZFUIAutoLayoutMakerPrivate))
{
    d->zfal_maker(child, parent);
}
ZFUIAutoLayoutMaker::ZFUIAutoLayoutMaker(ZF_IN const ZFUIAutoLayoutMaker &ref)
: d(ref.d)
{
    ++(d->refCount);
}
ZFUIAutoLayoutMaker::~ZFUIAutoLayoutMaker(void)
{
    --(d->refCount);
    if(d->refCount == 0)
    {
        zfdelete(d);
    }
}
ZFUIAutoLayoutMaker &ZFUIAutoLayoutMaker::operator = (ZF_IN const ZFUIAutoLayoutMaker &ref)
{
    ++(ref.d->refCount);
    --(d->refCount);
    if(d->refCount == 0)
    {
        zfdelete(d);
    }
    d = ref.d;
    return *this;
}

ZFUIAutoLayoutMaker &ZFUIAutoLayoutMaker::zfal_maker(ZF_IN ZFUIView *child)
{
    d->zfal_maker(child);
    return *this;
}
ZFUIAutoLayoutMaker &ZFUIAutoLayoutMaker::zfal_maker(ZF_IN ZFUIView *child, ZF_IN ZFUIAutoLayout *parent)
{
    d->zfal_maker(child, parent);
    return *this;
}

ZFUIAutoLayoutMaker &ZFUIAutoLayoutMaker::width(ZF_IN zfint size)
{
    d->requireChild();
    ZFUILayoutParam *layoutParam = d->child->layoutParam();
    if(size < 0)
    {
        layoutParam->sizeHint(ZFUISizeMake(-1, layoutParam->sizeHint().height));
        layoutParam->sizeParam(ZFUISizeParamMake(ZFUISizeType::e_Wrap, layoutParam->sizeParam().height));
    }
    else
    {
        layoutParam->sizeHint(ZFUISizeMake(size, layoutParam->sizeHint().height));
        layoutParam->sizeParam(ZFUISizeParamMake(ZFUISizeType::e_Fill, layoutParam->sizeParam().height));
    }
    return *this;
}
ZFUIAutoLayoutMaker &ZFUIAutoLayoutMaker::height(ZF_IN zfint size)
{
    d->requireChild();
    ZFUILayoutParam *layoutParam = d->child->layoutParam();
    if(size < 0)
    {
        layoutParam->sizeHint(ZFUISizeMake(layoutParam->sizeHint().width, -1));
        layoutParam->sizeParam(ZFUISizeParamMake(layoutParam->sizeParam().height, ZFUISizeType::e_Wrap));
    }
    else
    {
        layoutParam->sizeHint(ZFUISizeMake(layoutParam->sizeHint().height, size));
        layoutParam->sizeParam(ZFUISizeParamMake(layoutParam->sizeParam().height, ZFUISizeType::e_Fill));
    }
    return *this;
}
ZFUIAutoLayoutMaker &ZFUIAutoLayoutMaker::size(ZF_IN zfint size)
{
    return this->size(size, size);
}
ZFUIAutoLayoutMaker &ZFUIAutoLayoutMaker::size(ZF_IN zfint width, ZF_IN zfint height)
{
    d->requireChild();
    ZFUILayoutParam *layoutParam = d->child->layoutParam();
    if(width < 0)
    {
        layoutParam->sizeHint(ZFUISizeMake(-1, layoutParam->sizeHint().height));
        layoutParam->sizeParam(ZFUISizeParamMake(ZFUISizeType::e_Wrap, layoutParam->sizeParam().height));
    }
    else
    {
        layoutParam->sizeHint(ZFUISizeMake(width, layoutParam->sizeHint().height));
        layoutParam->sizeParam(ZFUISizeParamMake(ZFUISizeType::e_Fill, layoutParam->sizeParam().height));
    }
    if(height < 0)
    {
        layoutParam->sizeHint(ZFUISizeMake(layoutParam->sizeHint().width, -1));
        layoutParam->sizeParam(ZFUISizeParamMake(layoutParam->sizeParam().height, ZFUISizeType::e_Wrap));
    }
    else
    {
        layoutParam->sizeHint(ZFUISizeMake(layoutParam->sizeHint().height, height));
        layoutParam->sizeParam(ZFUISizeParamMake(layoutParam->sizeParam().height, ZFUISizeType::e_Fill));
    }
    return *this;
}
ZFUIAutoLayoutMaker &ZFUIAutoLayoutMaker::size(ZF_IN const ZFUISize &size)
{
    return this->size(size.width, size.height);
}

ZFUIAutoLayoutMaker &ZFUIAutoLayoutMaker::width(void)
{
    d->posAttach(ZFUIAutoLayoutPos::e_Width);
    return *this;
}
ZFUIAutoLayoutMaker &ZFUIAutoLayoutMaker::height(void)
{
    d->posAttach(ZFUIAutoLayoutPos::e_Height);
    return *this;
}
ZFUIAutoLayoutMaker &ZFUIAutoLayoutMaker::left(void)
{
    d->posAttach(ZFUIAutoLayoutPos::e_Left);
    return *this;
}
ZFUIAutoLayoutMaker &ZFUIAutoLayoutMaker::top(void)
{
    d->posAttach(ZFUIAutoLayoutPos::e_Top);
    return *this;
}
ZFUIAutoLayoutMaker &ZFUIAutoLayoutMaker::right(void)
{
    d->posAttach(ZFUIAutoLayoutPos::e_Right);
    return *this;
}
ZFUIAutoLayoutMaker &ZFUIAutoLayoutMaker::bottom(void)
{
    d->posAttach(ZFUIAutoLayoutPos::e_Bottom);
    return *this;
}

ZFUIAutoLayoutMaker &ZFUIAutoLayoutMaker::toWidth(ZF_IN ZFUIView *target)
{
    d->targetAttach(target, ZFUIAutoLayoutPos::e_Width);
    return *this;
}
ZFUIAutoLayoutMaker &ZFUIAutoLayoutMaker::toHeight(ZF_IN ZFUIView *target)
{
    d->targetAttach(target, ZFUIAutoLayoutPos::e_Height);
    return *this;
}
ZFUIAutoLayoutMaker &ZFUIAutoLayoutMaker::toLeft(ZF_IN ZFUIView *target)
{
    d->targetAttach(target, ZFUIAutoLayoutPos::e_Left);
    return *this;
}
ZFUIAutoLayoutMaker &ZFUIAutoLayoutMaker::toTop(ZF_IN ZFUIView *target)
{
    d->targetAttach(target, ZFUIAutoLayoutPos::e_Top);
    return *this;
}
ZFUIAutoLayoutMaker &ZFUIAutoLayoutMaker::toRight(ZF_IN ZFUIView *target)
{
    d->targetAttach(target, ZFUIAutoLayoutPos::e_Right);
    return *this;
}
ZFUIAutoLayoutMaker &ZFUIAutoLayoutMaker::toBottom(ZF_IN ZFUIView *target)
{
    d->targetAttach(target, ZFUIAutoLayoutPos::e_Bottom);
    return *this;
}

ZFUIAutoLayoutMaker &ZFUIAutoLayoutMaker::toParentWidth(void)
{
    d->targetAttach(zfnull, ZFUIAutoLayoutPos::e_Width);
    return *this;
}
ZFUIAutoLayoutMaker &ZFUIAutoLayoutMaker::toParentHeight(void)
{
    d->targetAttach(zfnull, ZFUIAutoLayoutPos::e_Height);
    return *this;
}
ZFUIAutoLayoutMaker &ZFUIAutoLayoutMaker::toParentLeft(void)
{
    d->targetAttach(zfnull, ZFUIAutoLayoutPos::e_Left);
    return *this;
}
ZFUIAutoLayoutMaker &ZFUIAutoLayoutMaker::toParentTop(void)
{
    d->targetAttach(zfnull, ZFUIAutoLayoutPos::e_Top);
    return *this;
}
ZFUIAutoLayoutMaker &ZFUIAutoLayoutMaker::toParentRight(void)
{
    d->targetAttach(zfnull, ZFUIAutoLayoutPos::e_Right);
    return *this;
}
ZFUIAutoLayoutMaker &ZFUIAutoLayoutMaker::toParentBottom(void)
{
    d->targetAttach(zfnull, ZFUIAutoLayoutPos::e_Bottom);
    return *this;
}

ZFUIAutoLayoutMaker &ZFUIAutoLayoutMaker::to(ZF_IN ZFUIView *target)
{
    d->targetAttach(target);
    return *this;
}
ZFUIAutoLayoutMaker &ZFUIAutoLayoutMaker::toParent(void)
{
    d->targetAttach(zfnull);
    return *this;
}

ZFUIAutoLayoutMaker &ZFUIAutoLayoutMaker::scale(ZF_IN zffloat scale)
{
    d->requireTarget();
    for(zfindex i = d->ruleAttached.count() - 1; i != zfindexMax(); --i)
    {
        d->ruleAttached[i]->scale() = scale;
    }
    return *this;
}
ZFUIAutoLayoutMaker &ZFUIAutoLayoutMaker::offset(ZF_IN zfint offset)
{
    d->requireTarget();
    for(zfindex i = d->ruleAttached.count() - 1; i != zfindexMax(); --i)
    {
        d->ruleAttached[i]->offset() = offset;
    }
    return *this;
}

ZFUIAutoLayoutMaker &ZFUIAutoLayoutMaker::biasX(ZF_IN zffloat biasX)
{
    d->requireChild();
    d->child->layoutParam<ZFUIAutoLayoutParam *>()->biasX(biasX);
    return *this;
}
ZFUIAutoLayoutMaker &ZFUIAutoLayoutMaker::biasY(ZF_IN zffloat biasY)
{
    d->requireChild();
    d->child->layoutParam<ZFUIAutoLayoutParam *>()->biasY(biasY);
    return *this;
}

ZFUIAutoLayoutMaker &ZFUIAutoLayoutMaker::done(void)
{
    d->done();
    return *this;
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_1(ZFUIAutoLayoutMaker, zfal_maker, ZFMP_IN(ZFUIView *, child))
{
    return ZFUIAutoLayoutMaker(child);
}
ZFMETHOD_FUNC_DEFINE_2(ZFUIAutoLayoutMaker, zfal_maker, ZFMP_IN(ZFUIView *, child), ZFMP_IN(ZFUIAutoLayout *, parent))
{
    return ZFUIAutoLayoutMaker(child, parent);
}

// ============================================================
ZFTYPEID_ACCESS_ONLY_DEFINE(ZFUIAutoLayoutMaker, ZFUIAutoLayoutMaker)

ZFOBJECT_ON_INIT_USER_REGISTER_1({
        invokerObject->to<v_ZFUIAutoLayoutMaker *>()->zfv.zfal_maker(child);
    }, v_ZFUIAutoLayoutMaker
    , ZFMP_IN(ZFUIView *, child)
    )
ZFOBJECT_ON_INIT_USER_REGISTER_2({
        invokerObject->to<v_ZFUIAutoLayoutMaker *>()->zfv.zfal_maker(child, parent);
    }, v_ZFUIAutoLayoutMaker
    , ZFMP_IN(ZFUIView *, child)
    , ZFMP_IN(ZFUIAutoLayout *, parent)
    )

ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFUIAutoLayoutMaker, ZFUIAutoLayoutMaker &, zfal_maker, ZFMP_IN(ZFUIView *, child))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFUIAutoLayoutMaker, ZFUIAutoLayoutMaker &, zfal_maker, ZFMP_IN(ZFUIView *, child), ZFMP_IN(ZFUIAutoLayout *, parent))

ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFUIAutoLayoutMaker, ZFUIAutoLayoutMaker &, width, ZFMP_IN(zfint, size))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFUIAutoLayoutMaker, ZFUIAutoLayoutMaker &, height, ZFMP_IN(zfint, size))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFUIAutoLayoutMaker, ZFUIAutoLayoutMaker &, size, ZFMP_IN(zfint, size))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFUIAutoLayoutMaker, ZFUIAutoLayoutMaker &, size, ZFMP_IN(zfint, width), ZFMP_IN(zfint, height))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFUIAutoLayoutMaker, ZFUIAutoLayoutMaker &, size, ZFMP_IN(const ZFUISize &, size))

ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFUIAutoLayoutMaker, ZFUIAutoLayoutMaker &, width)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFUIAutoLayoutMaker, ZFUIAutoLayoutMaker &, height)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFUIAutoLayoutMaker, ZFUIAutoLayoutMaker &, left)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFUIAutoLayoutMaker, ZFUIAutoLayoutMaker &, top)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFUIAutoLayoutMaker, ZFUIAutoLayoutMaker &, right)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFUIAutoLayoutMaker, ZFUIAutoLayoutMaker &, bottom)

ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFUIAutoLayoutMaker, ZFUIAutoLayoutMaker &, toWidth, ZFMP_IN(ZFUIView *, target))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFUIAutoLayoutMaker, ZFUIAutoLayoutMaker &, toHeight, ZFMP_IN(ZFUIView *, target))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFUIAutoLayoutMaker, ZFUIAutoLayoutMaker &, toLeft, ZFMP_IN(ZFUIView *, target))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFUIAutoLayoutMaker, ZFUIAutoLayoutMaker &, toTop, ZFMP_IN(ZFUIView *, target))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFUIAutoLayoutMaker, ZFUIAutoLayoutMaker &, toRight, ZFMP_IN(ZFUIView *, target))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFUIAutoLayoutMaker, ZFUIAutoLayoutMaker &, toBottom, ZFMP_IN(ZFUIView *, target))

ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFUIAutoLayoutMaker, ZFUIAutoLayoutMaker &, toParentWidth)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFUIAutoLayoutMaker, ZFUIAutoLayoutMaker &, toParentHeight)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFUIAutoLayoutMaker, ZFUIAutoLayoutMaker &, toParentLeft)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFUIAutoLayoutMaker, ZFUIAutoLayoutMaker &, toParentTop)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFUIAutoLayoutMaker, ZFUIAutoLayoutMaker &, toParentRight)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFUIAutoLayoutMaker, ZFUIAutoLayoutMaker &, toParentBottom)

ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFUIAutoLayoutMaker, ZFUIAutoLayoutMaker &, to, ZFMP_IN(ZFUIView *, target))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFUIAutoLayoutMaker, ZFUIAutoLayoutMaker &, toParent)

ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFUIAutoLayoutMaker, ZFUIAutoLayoutMaker &, scale, ZFMP_IN(zffloat, scale))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFUIAutoLayoutMaker, ZFUIAutoLayoutMaker &, offset, ZFMP_IN(zfint, offset))

ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFUIAutoLayoutMaker, ZFUIAutoLayoutMaker &, biasX, ZFMP_IN(zffloat, biasX))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFUIAutoLayoutMaker, ZFUIAutoLayoutMaker &, biasY, ZFMP_IN(zffloat, biasY))

ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFUIAutoLayoutMaker, ZFUIAutoLayoutMaker &, done)

ZF_NAMESPACE_GLOBAL_END

