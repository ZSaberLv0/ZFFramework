#include "ZFImpl_sys_Qt_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIViewTransform.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIView.h"

#if ZF_ENV_sys_Qt

#include <QGraphicsWidget>
#include <QTransform>

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIViewTransformImpl_sys_Qt, ZFUIViewTransform, ZFProtocolLevel::e_SystemHigh)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Qt:QGraphicsWidget")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIView, "Qt:QGraphicsWidget")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()
public:
    virtual void viewTransform(ZF_IN ZFUIView *view)
    {
        QGraphicsWidget *nativeView = (QGraphicsWidget *)view->nativeView();
        QTransform t;
        t.translate(nativeView->geometry().width() / 2, nativeView->geometry().height() / 2);
        if(view->viewRotation() != 0)
        {
            t.rotate(view->viewRotation());
        }
        if(view->viewScaleX() != 1 || view->viewScaleY() != 1)
        {
            t.scale(view->viewScaleX(), view->viewScaleY());
        }
        t.translate(-nativeView->geometry().width() / 2, -nativeView->geometry().height() / 2);
        nativeView->setTransform(t);
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIViewTransformImpl_sys_Qt)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUIViewTransformImpl_sys_Qt)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_Qt

