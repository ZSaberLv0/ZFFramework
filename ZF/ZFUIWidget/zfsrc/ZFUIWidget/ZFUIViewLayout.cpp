#include "ZFUIViewLayout.h"

ZF_NAMESPACE_GLOBAL_BEGIN

void ZFUIViewLayout::layoutOnMeasure(ZF_OUT ZFUISize &ret,
                                     ZF_IN const ZFUISize &sizeHint,
                                     ZF_IN const ZFUISizeParam &sizeParam)
{
    ZFUIViewLayoutParam::sizeHintApply(ret, this->viewSizeMin(), sizeHint, sizeParam);
    for(zfindex i = 0; i < this->childCount(); ++i)
    {
        ZFUIView *child = this->childAtIndex(i);
        ZFUISize sizeHintTmp = sizeHint;
        if(sizeHintTmp.width >= 0)
        {
            sizeHintTmp.width -= ZFUIMarginGetWidth(child->layoutParam()->layoutMargin());
            if(sizeHintTmp.width < 0)
            {
                sizeHintTmp.width = 0;
            }
        }
        if(sizeHintTmp.height >= 0)
        {
            sizeHintTmp.height -= ZFUIMarginGetHeight(child->layoutParam()->layoutMargin());
            if(sizeHintTmp.height < 0)
            {
                sizeHintTmp.height = 0;
            }
        }
        child->layoutMeasure(sizeHintTmp, child->layoutParam()->sizeParam());

        ret.width = zfmMax(ret.width, child->layoutMeasuredSize().width + ZFUIMarginGetWidth(child->layoutParam()->layoutMargin()));
        ret.height = zfmMax(ret.height, child->layoutMeasuredSize().height + ZFUIMarginGetHeight(child->layoutParam()->layoutMargin()));
    }
}

ZF_NAMESPACE_GLOBAL_END

