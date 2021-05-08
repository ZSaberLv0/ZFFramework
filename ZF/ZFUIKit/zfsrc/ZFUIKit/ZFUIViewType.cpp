#include "ZFUIViewType.h"
#include "ZFUIView.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFENUM_DEFINE(ZFUISizeType)

// ============================================================
// ZFUISizeParam
ZFTYPEID_DEFINE_BY_STRING_CONVERTER(ZFUISizeParam, ZFUISizeParam, {
        ZFCoreArrayPOD<ZFIndexRange> pos;
        if(!zfCoreDataPairSplitString(pos, 2, src, srcLen))
        {
            return zffalse;
        }

        if(!ZFUISizeTypeEnumFromString(v.width, src + pos[0].start, pos[0].count))
        {
            return zffalse;
        }

        if(!ZFUISizeTypeEnumFromString(v.height, src + pos[1].start, pos[1].count))
        {
            return zffalse;
        }

        return zftrue;
    }, {
        s += "(";
        ZFUISizeTypeEnumToString(s, v.width);
        s += ", ";
        ZFUISizeTypeEnumToString(s, v.height);
        s += ")";
        return zftrue;
    })
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFUISizeParam, ZFUISizeTypeEnum, width)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFUISizeParam, ZFUISizeTypeEnum, height)

ZFMETHOD_FUNC_INLINE_DEFINE_2(ZFUISizeParam, ZFUISizeParamMake,
                              ZFMP_IN(ZFUISizeTypeEnum const &, width),
                              ZFMP_IN(ZFUISizeTypeEnum const &, height))
ZFMETHOD_FUNC_INLINE_DEFINE_1(ZFUISizeParam, ZFUISizeParamMake,
                              ZFMP_IN(ZFUISizeTypeEnum const &, v))

ZFEXPORT_VAR_READONLY_DEFINE(ZFUISizeParam, ZFUISizeParamZero, ZFUISizeParamMake(ZFUISizeType::e_Wrap, ZFUISizeType::e_Wrap))
ZFEXPORT_VAR_READONLY_DEFINE(ZFUISizeParam, ZFUISizeParamWrapWrap, ZFUISizeParamMake(ZFUISizeType::e_Wrap, ZFUISizeType::e_Wrap))
ZFEXPORT_VAR_READONLY_DEFINE(ZFUISizeParam, ZFUISizeParamWrapFill, ZFUISizeParamMake(ZFUISizeType::e_Wrap, ZFUISizeType::e_Fill))
ZFEXPORT_VAR_READONLY_DEFINE(ZFUISizeParam, ZFUISizeParamFillWrap, ZFUISizeParamMake(ZFUISizeType::e_Fill, ZFUISizeType::e_Wrap))
ZFEXPORT_VAR_READONLY_DEFINE(ZFUISizeParam, ZFUISizeParamFillFill, ZFUISizeParamMake(ZFUISizeType::e_Fill, ZFUISizeType::e_Fill))

// ============================================================
ZFOBJECT_REGISTER(ZFUILayoutParam)
ZFOBSERVER_EVENT_REGISTER(ZFUILayoutParam, LayoutParamOnChange)

ZFMETHOD_DEFINE_4(ZFUILayoutParam, void, layoutParamApply,
                  ZFMP_OUT(ZFUIRect &, ret),
                  ZFMP_IN(const ZFUIRect &, rect),
                  ZFMP_IN(ZFUIView *, child),
                  ZFMP_IN(ZFUILayoutParam *, lp))
{
    ZFUISize refSizeTmp = ZFUIRectGetSize(ZFUIRectApplyMargin(rect, lp->layoutMargin()));
    if(refSizeTmp.width < 0)
    {
        refSizeTmp.width = 0;
    }
    if(refSizeTmp.height < 0)
    {
        refSizeTmp.height = 0;
    }
    if(lp->sizeHint().width >= 0)
    {
        refSizeTmp.width = zfmMin(lp->sizeHint().width, refSizeTmp.width);
    }
    if(lp->sizeHint().height >= 0)
    {
        refSizeTmp.height = zfmMin(lp->sizeHint().height, refSizeTmp.height);
    }

    child->layoutMeasure(refSizeTmp, lp->sizeParam());
    ZFUIAlignApply(ret, lp->layoutAlign(), rect, child->layoutMeasuredSize(), lp->layoutMargin());
}
ZFMETHOD_DEFINE_3(ZFUILayoutParam, ZFUIRect, layoutParamApply,
                  ZFMP_IN(const ZFUIRect &, rect),
                  ZFMP_IN(ZFUIView *, child),
                  ZFMP_IN(ZFUILayoutParam *, lp))
{
    ZFUIRect ret = ZFUIRectZero();
    ZFUILayoutParam::layoutParamApply(ret, rect, child, lp);
    return ret;
}

ZFMETHOD_DEFINE_4(ZFUILayoutParam, void, sizeHintApply,
                  ZFMP_OUT(zffloat &, ret),
                  ZFMP_IN(zffloat, size),
                  ZFMP_IN(zffloat, sizeHint),
                  ZFMP_IN(ZFUISizeTypeEnum, sizeParam))
{
    ret = size;
    switch(sizeParam)
    {
        case ZFUISizeType::e_Wrap:
            if(sizeHint >= 0 && size > sizeHint)
            {
                ret = sizeHint;
            }
            break;
        case ZFUISizeType::e_Fill:
            if(sizeHint >= 0)
            {
                ret = sizeHint;
            }
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            break;
    }
}
ZFMETHOD_DEFINE_3(ZFUILayoutParam, zffloat, sizeHintApply,
                  ZFMP_IN(zffloat, size),
                  ZFMP_IN(zffloat, sizeHint),
                  ZFMP_IN(ZFUISizeTypeEnum, sizeParam))
{
    zffloat ret = size;
    ZFUILayoutParam::sizeHintApply(ret, size, sizeHint, sizeParam);
    return ret;
}

ZFMETHOD_DEFINE_4(ZFUILayoutParam, void, sizeHintApply,
                  ZFMP_OUT(ZFUISize &, ret),
                  ZFMP_IN(const ZFUISize &, size),
                  ZFMP_IN(const ZFUISize &, sizeHint),
                  ZFMP_IN(const ZFUISizeParam &, sizeParam))
{
    ret.width = ZFUILayoutParam::sizeHintApply(size.width, sizeHint.width, sizeParam.width);
    ret.height = ZFUILayoutParam::sizeHintApply(size.height, sizeHint.height, sizeParam.height);
}
ZFMETHOD_DEFINE_3(ZFUILayoutParam, ZFUISize, sizeHintApply,
                  ZFMP_IN(const ZFUISize &, size),
                  ZFMP_IN(const ZFUISize &, sizeHint),
                  ZFMP_IN(const ZFUISizeParam &, sizeParam))
{
    ZFUISize ret = ZFUISizeZero();
    ZFUILayoutParam::sizeHintApply(ret, size, sizeHint, sizeParam);
    return ret;
}

ZFMETHOD_DEFINE_3(ZFUILayoutParam, void, sizeHintMerge,
                  ZFMP_OUT(zffloat &, ret),
                  ZFMP_IN(zffloat, sizeHint0),
                  ZFMP_IN(zffloat, sizeHint1))
{
    if(sizeHint0 < 0 && sizeHint1 < 0)
    {
        ret = -1;
    }
    else if(sizeHint0 >= 0 && sizeHint1 >= 0)
    {
        ret = zfmMin(sizeHint0, sizeHint1);
    }
    else
    {
        ret = zfmMax(sizeHint0, sizeHint1);
    }
}
ZFMETHOD_DEFINE_2(ZFUILayoutParam, zffloat, sizeHintMerge,
                  ZFMP_IN(zffloat, sizeHint0),
                  ZFMP_IN(zffloat, sizeHint1))
{
    zffloat ret = 0;
    ZFUILayoutParam::sizeHintMerge(ret, sizeHint0, sizeHint1);
    return ret;
}

ZFMETHOD_DEFINE_3(ZFUILayoutParam, void, sizeHintMerge,
                  ZFMP_OUT(ZFUISize &, ret),
                  ZFMP_IN(const ZFUISize &, sizeHint0),
                  ZFMP_IN(const ZFUISize &, sizeHint1))
{
    ret.width = ZFUILayoutParam::sizeHintMerge(sizeHint0.width, sizeHint1.width);
    ret.height = ZFUILayoutParam::sizeHintMerge(sizeHint0.height, sizeHint1.height);
}
ZFMETHOD_DEFINE_2(ZFUILayoutParam, ZFUISize, sizeHintMerge,
                  ZFMP_IN(const ZFUISize &, sizeHint0),
                  ZFMP_IN(const ZFUISize &, sizeHint1))
{
    ZFUISize ret = ZFUISizeZero();
    ZFUILayoutParam::sizeHintMerge(ret, sizeHint0, sizeHint1);
    return ret;
}

ZFMETHOD_DEFINE_3(ZFUILayoutParam, void, sizeHintOffset,
                  ZFMP_OUT(zffloat &, ret),
                  ZFMP_IN(zffloat, sizeHint),
                  ZFMP_IN(zffloat, offset))
{
    if(offset >= 0)
    {
        ret = ((sizeHint >= 0) ? sizeHint + offset : -1);
    }
    else
    {
        ret = ((sizeHint >= 0) ? zfmMax((zffloat)0, sizeHint + offset) : -1);
    }
}
ZFMETHOD_DEFINE_2(ZFUILayoutParam, zffloat, sizeHintOffset,
                  ZFMP_IN(zffloat, sizeHint),
                  ZFMP_IN(zffloat, offset))
{
    zffloat ret = 0;
    ZFUILayoutParam::sizeHintOffset(ret, sizeHint, offset);
    return ret;
}

ZFMETHOD_DEFINE_3(ZFUILayoutParam, void, sizeHintOffset,
                  ZFMP_OUT(ZFUISize &, ret),
                  ZFMP_IN(const ZFUISize &, sizeHint),
                  ZFMP_IN(const ZFUISize &, offset))
{
    ret.width = ZFUILayoutParam::sizeHintOffset(sizeHint.width, offset.width);
    ret.height = ZFUILayoutParam::sizeHintOffset(sizeHint.height, offset.height);
}
ZFMETHOD_DEFINE_2(ZFUILayoutParam, ZFUISize, sizeHintOffset,
                  ZFMP_IN(const ZFUISize &, sizeHint),
                  ZFMP_IN(const ZFUISize &, offset))
{
    ZFUISize ret = ZFUISizeZero();
    ZFUILayoutParam::sizeHintOffset(ret, sizeHint, offset);
    return ret;
}

ZFMETHOD_DEFINE_3(ZFUILayoutParam, void, sizeHintOffset,
                  ZFMP_OUT(ZFUISize &, ret),
                  ZFMP_IN(const ZFUISize &, sizeHint),
                  ZFMP_IN(zffloat, offset))
{
    ret.width = ZFUILayoutParam::sizeHintOffset(sizeHint.width, offset);
    ret.height = ZFUILayoutParam::sizeHintOffset(sizeHint.height, offset);
}
ZFMETHOD_DEFINE_2(ZFUILayoutParam, ZFUISize, sizeHintOffset,
                  ZFMP_IN(const ZFUISize &, sizeHint),
                  ZFMP_IN(zffloat, offset))
{
    ZFUISize ret = ZFUISizeZero();
    ZFUILayoutParam::sizeHintOffset(ret, sizeHint, offset);
    return ret;
}

// ============================================================
ZFENUM_DEFINE(ZFUIViewChildLayer)

ZFOBJECT_REGISTER(ZFUIViewMeasureResult)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFUIViewMeasureResult, ZFUISize, sizeHint)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFUIViewMeasureResult, ZFUISizeParam, sizeParam)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFUIViewMeasureResult, ZFUISize, measuredSize)

ZF_NAMESPACE_GLOBAL_END

