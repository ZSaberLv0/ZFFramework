#include "ZFUIViewPositionOnScreen.h"
#include "protocol/ZFProtocolZFUIViewPositionOnScreen.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_DEFINE_2(void, ZFUIViewPositionOnScreen,
                       ZFMP_OUT(ZFUIRect &, rect),
                       ZFMP_IN(ZFUIView *, view))
{
    ZFPROTOCOL_ACCESS(ZFUIViewPositionOnScreen)->viewPositionOnScreen(view, rect);
    rect = ZFUIRectApplyScaleReversely(rect, view->scaleFixed());
}
ZFMETHOD_FUNC_INLINE_DEFINE_1(ZFUIRect, ZFUIViewPositionOnScreen,
                              ZFMP_IN(ZFUIView *, view))

ZF_NAMESPACE_GLOBAL_END

