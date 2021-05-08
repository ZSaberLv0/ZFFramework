#include "ZFUIScrollThumb.h"
#include "ZFUIScrollThumbDefault.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFUIScrollThumb)

ZFOBSERVER_EVENT_REGISTER(ZFUIScrollThumb, ScrollThumbOnUpdate)

void ZFUIScrollThumb::scrollThumbPosFromViewPos(ZF_OUT zffloat &scrollThumbResultPos,
                                                ZF_OUT zffloat &scrollThumbResultSize,
                                                ZF_IN zffloat scrollThumbRange,
                                                ZF_IN zffloat scrollViewSize,
                                                ZF_IN zffloat scrollContentOffset,
                                                ZF_IN zffloat scrollContentSize,
                                                ZF_IN zffloat scrollThumbMinSize,
                                                ZF_IN_OPT zffloat headMargin /* = 0 */,
                                                ZF_IN_OPT zffloat tailMargin /* = 0 */)
{
    if(scrollThumbRange <= 0
        || scrollViewSize <= 0
        || scrollContentSize <= 0)
    {
        scrollThumbResultSize = 0;
        return ;
    }

    zffloat pos = 0;
    zffloat size = 0;
    zffloat fixedContentSize = zfmMax(scrollViewSize, scrollContentSize);
    zffloat virtualContentSize = scrollThumbRange - headMargin - tailMargin;

    if(scrollContentOffset > 0)
    { // bounce at head
        pos = 0;
        size = scrollViewSize - scrollContentOffset;
    }
    else if(scrollViewSize >= scrollContentSize && scrollContentOffset < 0)
    { // bounce at tail
        pos = -scrollContentOffset;
        size = scrollViewSize - pos;
    }
    else if(scrollContentOffset + fixedContentSize < scrollViewSize)
    { // bounce at tail
        pos = -scrollContentOffset;
        size = scrollContentOffset + fixedContentSize;
    }
    else
    { // no bounce
        pos = -scrollContentOffset;
        size = scrollViewSize;
    }

    scrollThumbResultPos = headMargin + pos * virtualContentSize / fixedContentSize;
    scrollThumbResultSize = size * virtualContentSize / fixedContentSize;

    if(scrollThumbResultSize < scrollThumbMinSize)
    {
        scrollThumbResultPos -= (scrollThumbMinSize - scrollThumbResultSize) / 2;
        scrollThumbResultSize = scrollThumbMinSize;
    }

    if(scrollThumbResultSize >= virtualContentSize - 2)
    {
        scrollThumbResultSize = virtualContentSize;
    }

    if(scrollThumbResultPos + scrollThumbResultSize + tailMargin > scrollThumbRange)
    {
        scrollThumbResultPos = scrollThumbRange - scrollThumbResultSize - tailMargin;
    }
    if(scrollThumbResultPos < headMargin)
    {
        scrollThumbResultPos = headMargin;
    }
}
void ZFUIScrollThumb::scrollThumbPosToViewPos(ZF_OUT zffloat &scrollContentOffset,
                                              ZF_IN zffloat scrollViewSize,
                                              ZF_IN zffloat scrollContentSize,
                                              ZF_IN zffloat scrollThumbPos,
                                              ZF_IN zffloat scrollThumbSize,
                                              ZF_IN zffloat scrollThumbRange,
                                              ZF_IN_OPT zffloat headMargin /* = 0 */,
                                              ZF_IN_OPT zffloat tailMargin /* = 0 */)
{
    if(scrollThumbRange <= 0
        || scrollContentSize <= scrollViewSize
        || scrollThumbPos <= headMargin
        || scrollThumbRange - headMargin - tailMargin - scrollThumbSize <= 0)
    {
        scrollContentOffset = 0;
    }
    else if(scrollThumbPos + scrollThumbSize >= scrollThumbRange - tailMargin)
    {
        scrollContentOffset = scrollViewSize - scrollContentSize;
    }
    else
    {
        scrollContentOffset =
            (scrollThumbPos - headMargin)
            * (scrollViewSize - scrollContentSize)
            / (scrollThumbRange - headMargin - tailMargin - scrollThumbSize);
    }
}

static const ZFClass *_ZFP_ZFUIScrollThumbHorizontal_cls = zfnull;
static const ZFClass *_ZFP_ZFUIScrollThumbVertical_cls = zfnull;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIScrollThumbRegister, ZFLevelZFFrameworkHigh)
{
    _ZFP_ZFUIScrollThumbHorizontal_cls = ZFUIScrollThumbDefault::ClassData();
    _ZFP_ZFUIScrollThumbVertical_cls = ZFUIScrollThumbDefault::ClassData();
}
ZF_GLOBAL_INITIALIZER_END(ZFUIScrollThumbRegister)

ZFMETHOD_FUNC_DEFINE_1(void, ZFUIScrollThumbHorizontalClass,
                       ZFMP_IN(const ZFClass *, cls))
{
    if(cls != zfnull && !cls->classIsTypeOf(ZFUIScrollThumb::ClassData()))
    {
        zfCoreLog("class %s not type of %s",
            cls->classNameFull(),
            ZFUIScrollThumb::ClassData()->classNameFull());
        return ;
    }

    if(cls == zfnull)
    {
        cls = ZFUIScrollThumbDefault::ClassData();
    }
    _ZFP_ZFUIScrollThumbHorizontal_cls = cls;
}
ZFMETHOD_FUNC_DEFINE_0(const ZFClass *, ZFUIScrollThumbHorizontalClass)
{
    return _ZFP_ZFUIScrollThumbHorizontal_cls;
}

ZFMETHOD_FUNC_DEFINE_1(void, ZFUIScrollThumbVerticalClass,
                       ZFMP_IN(const ZFClass *, cls))
{
    if(cls != zfnull && !cls->classIsTypeOf(ZFUIScrollThumb::ClassData()))
    {
        zfCoreLog("class %s not type of %s",
            cls->classNameFull(),
            ZFUIScrollThumb::ClassData()->classNameFull());
        return ;
    }

    if(cls == zfnull)
    {
        cls = ZFUIScrollThumbDefault::ClassData();
    }
    _ZFP_ZFUIScrollThumbVertical_cls = cls;
}
ZFMETHOD_FUNC_DEFINE_0(const ZFClass *, ZFUIScrollThumbVerticalClass)
{
    return _ZFP_ZFUIScrollThumbVertical_cls;
}

ZF_NAMESPACE_GLOBAL_END

