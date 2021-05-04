#include "ZFImpl_sys_Qt_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIView.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIViewFocus.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIKeyboardState.h"

#if ZF_ENV_sys_Qt

#include "ZFProtocolZFUIView_sys_Qt.h"
#include <QGraphicsOpacityEffect>
#include <QGraphicsProxyWidget>
#include <QKeyEvent>

// ============================================================
extern ZFUIKeyCodeEnum ZFUIViewImpl_sys_Qt_ZFUIKeyCodeFromQKeyCode(ZF_IN int qKeyCode);
zfbool (*ZFUIViewImpl_sys_Qt_isMouseCancel)(ZF_IN QGraphicsSceneMouseEvent *event) = zfnull;

QGraphicsSceneMouseEvent *ZFUIViewImpl_sys_Qt_mouseEventClone(ZF_IN QGraphicsSceneMouseEvent *event,
                                                              ZF_IN QEvent::Type type,
                                                              ZF_IN QPointF const &localPos)
{
    QGraphicsSceneMouseEvent *ret = new QGraphicsSceneMouseEvent(type);

    ret->setWidget(event->widget());

    ret->setPos(localPos);
    ret->setScenePos(event->scenePos());
    ret->setScreenPos(event->screenPos());
    ret->setButtonDownPos(Qt::LeftButton, event->buttonDownPos(Qt::LeftButton));
    ret->setButtonDownPos(Qt::RightButton, event->buttonDownPos(Qt::RightButton));
    ret->setButtonDownPos(Qt::MiddleButton, event->buttonDownPos(Qt::MiddleButton));
    ret->setButtonDownScenePos(Qt::LeftButton, event->buttonDownScenePos(Qt::LeftButton));
    ret->setButtonDownScenePos(Qt::RightButton, event->buttonDownScenePos(Qt::RightButton));
    ret->setButtonDownScenePos(Qt::MiddleButton, event->buttonDownScenePos(Qt::MiddleButton));
    ret->setButtonDownScreenPos(Qt::LeftButton, event->buttonDownScreenPos(Qt::LeftButton));
    ret->setButtonDownScreenPos(Qt::RightButton, event->buttonDownScreenPos(Qt::RightButton));
    ret->setButtonDownScreenPos(Qt::MiddleButton, event->buttonDownScreenPos(Qt::MiddleButton));
    ret->setLastPos(ret->pos());
    ret->setLastScenePos(ret->scenePos());
    ret->setLastScreenPos(ret->screenPos());
    ret->setButtons(event->buttons());
    ret->setButton(event->button());
    ret->setModifiers(event->modifiers());
    ret->setSource(event->source());
    ret->setFlags(event->flags());

    return ret;
}

class _ZFP_ZFUIViewImpl_sys_Qt_Layout : public ZFImpl_sys_Qt_Layout
{
public:
    ZFUIView *_ZFP_ownerZFUIView;
    QSizeF _ZFP_layoutedSize;
public:
    _ZFP_ZFUIViewImpl_sys_Qt_Layout(void)
    : ZFImpl_sys_Qt_Layout()
    , _ZFP_ownerZFUIView(zfnull)
    , _ZFP_layoutedSize(QSizeF(0, 0))
    {
    }
protected:
    virtual void onLayout(const QRectF &rect)
    {
        if(_ZFP_ownerZFUIView != zfnull && _ZFP_layoutedSize != rect.size()
            && (_ZFP_ownerZFUIView->viewParent() == zfnull || !_ZFP_ownerZFUIView->viewParent()->layoutRequested()))
        {
            _ZFP_layoutedSize = rect.size();
            ZFPROTOCOL_ACCESS(ZFUIView)->notifyLayoutView(
                _ZFP_ownerZFUIView,
                ZFImpl_sys_Qt_ZFUIRectFromQRectF(rect));
        }
    }
};

// ============================================================
// focus
extern void *_ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_attach(ZF_IN ZFUIView *ownerZFUIView,
                                                        ZF_IN QGraphicsWidget *nativeOwner,
                                                        ZF_IN QGraphicsWidget *nativeImplViewOrNull,
                                                        ZF_IN_OPT void *tokenOld = zfnull);
extern void _ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_detach(ZF_IN void *token);
extern void _ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_cleanup(ZF_IN void *token);
extern void _ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_viewFocusable(ZF_IN void *token, ZF_IN zfbool v);
extern void _ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_viewFocusRequest(ZF_IN void *token, ZF_IN zfbool v);
extern zfbool _ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_viewFocused(ZF_IN void *token);

// ============================================================
// native view
class _ZFP_ZFUIViewImpl_sys_Qt_View : public ZFImpl_sys_Qt_View
{
    Q_OBJECT

public:
    _ZFP_ZFUIViewImpl_sys_Qt_Layout *_ZFP_layoutProxy;
    void *_ZFP_focusProxyToken;
    QGraphicsWidget *_ZFP_nativeImplView;
    zfbool _ZFP_viewUIEnable;
    zfbool _ZFP_viewUIEnableTree;
    zfbool _ZFP_mousePressed;
    ZFUIPoint _ZFP_mouseMoveLastPoint;

public:
    _ZFP_ZFUIViewImpl_sys_Qt_View(void)
    : ZFImpl_sys_Qt_View()
    , _ZFP_layoutProxy(new _ZFP_ZFUIViewImpl_sys_Qt_Layout())
    , _ZFP_focusProxyToken(zfnull)
    , _ZFP_nativeImplView(zfnull)
    , _ZFP_viewUIEnable(zftrue)
    , _ZFP_viewUIEnableTree(zftrue)
    , _ZFP_mousePressed(zffalse)
    , _ZFP_mouseMoveLastPoint(ZFUIPointZero())
    {
        this->setLayout(_ZFP_layoutProxy);

        QPalette palette;
        palette.setColor(QPalette::Window, QColor(0, 0, 0, 0));
        this->setPalette(palette);
        this->setAutoFillBackground(true);
        this->setAcceptTouchEvents(true);
    }
    ~_ZFP_ZFUIViewImpl_sys_Qt_View(void)
    {
        zfCoreAssert(this->_ZFP_nativeImplView == zfnull);
    }

public:
    bool event(QEvent *event)
    {
        if(_ZFP_layoutProxy->_ZFP_ownerZFUIView == zfnull)
        {
            return QGraphicsWidget::event(event);
        }
        switch(event->type())
        {
            // mouse
            case QEvent::GraphicsSceneMousePress:
            case QEvent::GraphicsSceneMouseDoubleClick:
            case QEvent::GraphicsSceneMouseMove:
            case QEvent::GraphicsSceneMouseRelease:
            // hover
            case QEvent::GraphicsSceneHoverEnter:
            case QEvent::GraphicsSceneHoverMove:
            case QEvent::GraphicsSceneHoverLeave:
            // key
            case QEvent::KeyPress:
            case QEvent::KeyRelease:
            // wheel
            case QEvent::GraphicsSceneWheel:
                if(!this->_ZFP_viewUIEnableTree || !this->_ZFP_viewUIEnable)
                {
                    return false;
                }
                return QGraphicsWidget::event(event);

            // default
            default:
                return QGraphicsWidget::event(event);
        }
    }

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event)
    {
        this->_ZFP_mousePressed = zftrue;
        if(_ZFP_layoutProxy->_ZFP_ownerZFUIView == zfnull
            || !this->_ZFP_viewUIEnableTree || !this->_ZFP_viewUIEnable)
        {
            QGraphicsWidget::mousePressEvent(event);
            return ;
        }
        this->mouseEventResolve(event, ZFUIMouseAction::e_MouseDown);
    }
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event)
    {
        this->_ZFP_mouseMoveLastPoint = ZFImpl_sys_Qt_ZFUIPointFromQPointF(event->pos());
        if(_ZFP_layoutProxy->_ZFP_ownerZFUIView == zfnull
            || !this->_ZFP_viewUIEnableTree || !this->_ZFP_viewUIEnable)
        {
            QGraphicsWidget::mouseMoveEvent(event);
            return ;
        }
        if(this->_ZFP_mousePressed)
        {
            this->mouseEventResolve(event, ZFUIMouseAction::e_MouseMove);
        }
    }
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
    {
        this->_ZFP_mousePressed = zffalse;
        if(_ZFP_layoutProxy->_ZFP_ownerZFUIView == zfnull
            || !this->_ZFP_viewUIEnableTree || !this->_ZFP_viewUIEnable)
        {
            QGraphicsWidget::mouseReleaseEvent(event);
            return ;
        }
        if(ZFUIViewImpl_sys_Qt_isMouseCancel != zfnull && ZFUIViewImpl_sys_Qt_isMouseCancel(event))
        {
            this->mouseEventResolve(event, ZFUIMouseAction::e_MouseCancel);
        }
        else
        {
            this->mouseEventResolve(event, ZFUIMouseAction::e_MouseUp);
        }
    }
private:
    void mouseEventResolve(QGraphicsSceneMouseEvent *event, ZFUIMouseActionEnum mouseAction)
    {
        zfblockedAllocWithCache(ZFUIMouseEvent, ev);
        ev->eventResolved(zffalse);
        ev->mouseId = (zfidentity)event->button();
        ev->mouseAction = mouseAction;
        ev->mousePoint = ZFImpl_sys_Qt_ZFUIPointFromQPointF(event->pos());
        ev->mouseButton = ZFUIMouseButton::e_MouseButtonLeft;
        switch(event->button())
        {
            case Qt::RightButton:
                ev->mouseButton = ZFUIMouseButton::e_MouseButtonRight;
                break;
            case Qt::MiddleButton:
                ev->mouseButton = ZFUIMouseButton::e_MouseButtonCenter;
                break;
            case Qt::LeftButton:
            default:
                ev->mouseButton = ZFUIMouseButton::e_MouseButtonLeft;
                break;
        }

        ZFPROTOCOL_ACCESS(ZFUIView)->notifyUIEvent(_ZFP_layoutProxy->_ZFP_ownerZFUIView, ev);
        event->setAccepted(ev->eventResolved());
    }

protected:
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event)
    {
        if(_ZFP_layoutProxy->_ZFP_ownerZFUIView == zfnull
            || !this->_ZFP_viewUIEnableTree || !this->_ZFP_viewUIEnable)
        {
            QGraphicsWidget::hoverEnterEvent(event);
            return ;
        }
        this->mouseHoverEventResolve(event, ZFUIMouseAction::e_MouseHoverEnter);
    }
    virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event)
    {
        if(_ZFP_layoutProxy->_ZFP_ownerZFUIView == zfnull
            || !this->_ZFP_viewUIEnableTree || !this->_ZFP_viewUIEnable)
        {
            QGraphicsWidget::hoverMoveEvent(event);
            return ;
        }
        this->mouseHoverEventResolve(event, ZFUIMouseAction::e_MouseHover);
    }
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
    {
        if(_ZFP_layoutProxy->_ZFP_ownerZFUIView == zfnull
            || !this->_ZFP_viewUIEnableTree || !this->_ZFP_viewUIEnable)
        {
            QGraphicsWidget::hoverLeaveEvent(event);
            return ;
        }
        this->mouseHoverEventResolve(event, ZFUIMouseAction::e_MouseHoverExit);
    }
private:
    void mouseHoverEventResolve(QGraphicsSceneHoverEvent *event, ZFUIMouseActionEnum mouseAction)
    {
        zfblockedAllocWithCache(ZFUIMouseEvent, ev);
        ev->eventResolved(zffalse);
        ev->mouseId = 0;
        ev->mouseAction = mouseAction;
        ev->mousePoint = ZFImpl_sys_Qt_ZFUIPointFromQPointF(event->pos());
        ev->mouseButton = ZFUIMouseButton::e_MouseButtonLeft;
        ZFPROTOCOL_ACCESS(ZFUIView)->notifyUIEvent(_ZFP_layoutProxy->_ZFP_ownerZFUIView, ev);
        if(event != NULL)
        {
            event->setAccepted(ev->eventResolved());
        }
    }

protected:
    virtual void keyPressEvent(QKeyEvent *event)
    {
        if(_ZFP_layoutProxy->_ZFP_ownerZFUIView == zfnull
            || !this->_ZFP_viewUIEnableTree || !this->_ZFP_viewUIEnable)
        {
            QGraphicsWidget::keyPressEvent(event);
            return ;
        }
        this->keyEventResolve(event, event->isAutoRepeat() ? ZFUIKeyAction::e_KeyRepeat : ZFUIKeyAction::e_KeyDown);
    }
    virtual void keyReleaseEvent(QKeyEvent *event)
    {
        if(_ZFP_layoutProxy->_ZFP_ownerZFUIView == zfnull
            || !this->_ZFP_viewUIEnableTree || !this->_ZFP_viewUIEnable)
        {
            QGraphicsWidget::keyReleaseEvent(event);
            return ;
        }
        this->keyEventResolve(event, event->isAutoRepeat() ? ZFUIKeyAction::e_KeyRepeat : ZFUIKeyAction::e_KeyUp);
    }
private:
    void keyEventResolve(QKeyEvent *event, ZFUIKeyActionEnum keyAction)
    {
        if(this->_ZFP_viewUIEnableTree && this->_ZFP_viewUIEnable)
        {
            zfblockedAllocWithCache(ZFUIKeyEvent, ev);
            ev->eventResolved(zffalse);
            ev->keyId = (zfidentity)event->key();
            ev->keyAction = keyAction;
            ev->keyCode = ZFUIViewImpl_sys_Qt_ZFUIKeyCodeFromQKeyCode(event->key());
            ev->keyCodeRaw = (zfflags)event->key();
            ZFPROTOCOL_ACCESS(ZFUIView)->notifyUIEvent(_ZFP_layoutProxy->_ZFP_ownerZFUIView, ev);
            event->setAccepted(ev->eventResolved());
        }
    }

protected:
    virtual void wheelEvent(QGraphicsSceneWheelEvent *event)
    {
        if(_ZFP_layoutProxy->_ZFP_ownerZFUIView == zfnull
            || !this->_ZFP_viewUIEnableTree || !this->_ZFP_viewUIEnable)
        {
            QGraphicsWidget::wheelEvent(event);
            return ;
        }
        this->wheelEventResolve(event);
    }
private:
    void wheelEventResolve(QGraphicsSceneWheelEvent *event)
    {
        zfint eventSteps = event->delta() / 8 / 15;

        zfblockedAllocWithCache(ZFUIWheelEvent, wheelEvent);
        wheelEvent->eventResolved(zffalse);
        if(event->orientation() == Qt::Horizontal)
        {
            wheelEvent->wheelX = -eventSteps;
            wheelEvent->wheelY = 0;
        }
        else
        {
            wheelEvent->wheelX = 0;
            wheelEvent->wheelY = -eventSteps;
        }
        if(wheelEvent->wheelX != 0 || wheelEvent->wheelY != 0)
        {
            ZFPROTOCOL_ACCESS(ZFUIView)->notifyUIEvent(_ZFP_layoutProxy->_ZFP_ownerZFUIView, wheelEvent);
        }
    }
};

// ============================================================
// impl
ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIViewImpl_sys_Qt, ZFUIView, ZFProtocolLevel::e_SystemHigh)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Qt:QGraphicsWidget")

public:
    zfoverride
    virtual void protocolOnInitFinish(void)
    {
        zfsuper::protocolOnInitFinish();
        ZFUIKeyboardStateBuiltinImplRegister();
    }
    zfoverride
    virtual void protocolOnDeallocPrepare(void)
    {
        ZFUIKeyboardStateBuiltinImplUnregister();
        zfsuper::protocolOnDeallocPrepare();
    }

public:
    virtual void *nativeViewCreate(ZF_IN ZFUIView *view)
    {
        _ZFP_ZFUIViewImpl_sys_Qt_View *nativeView = new _ZFP_ZFUIViewImpl_sys_Qt_View();
        nativeView->_ZFP_layoutProxy->_ZFP_ownerZFUIView = view;
        nativeView->_ZFP_focusProxyToken = _ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_attach(
            nativeView->_ZFP_layoutProxy->_ZFP_ownerZFUIView, nativeView, zfnull);
        _ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_viewFocusable(nativeView->_ZFP_focusProxyToken, zffalse);
        return nativeView;
    }
    virtual void nativeViewDestroy(ZF_IN void *nativeView)
    {
        _ZFP_ZFUIViewImpl_sys_Qt_View *nativeViewTmp = ZFCastStatic(_ZFP_ZFUIViewImpl_sys_Qt_View *, nativeView);
        _ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_cleanup(nativeViewTmp->_ZFP_focusProxyToken);
        nativeViewTmp->_ZFP_layoutProxy->_ZFP_ownerZFUIView = zfnull;
        delete nativeViewTmp;
    }

    virtual void nativeImplView(ZF_IN ZFUIView *view,
                                ZF_IN void *nativeImplViewOld,
                                ZF_IN void *nativeImplView,
                                ZF_IN zfindex virtualIndex)
    {
        _ZFP_ZFUIViewImpl_sys_Qt_View *nativeView = ZFCastStatic(_ZFP_ZFUIViewImpl_sys_Qt_View *, view->nativeView());
        QGraphicsWidget *v = ZFCastStatic(QGraphicsWidget *, nativeImplView);

        if(nativeView->_ZFP_nativeImplView != zfnull)
        {
            nativeView->_ZFP_layoutProxy->childRemoveAtIndex(virtualIndex);
        }
        nativeView->_ZFP_nativeImplView = v;
        if(nativeView->_ZFP_nativeImplView != zfnull)
        {
            nativeView->_ZFP_layoutProxy->childAdd(nativeView->_ZFP_nativeImplView, virtualIndex);
        }

        nativeView->_ZFP_focusProxyToken = _ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_attach(
            view, nativeView, nativeView->_ZFP_nativeImplView, nativeView->_ZFP_focusProxyToken);
        _ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_viewFocusable(nativeView->_ZFP_focusProxyToken, view->viewFocusable());

        this->_updateNativeImplViewMouseSetting(view);
    }
    virtual void nativeImplViewFrame(ZF_IN ZFUIView *view,
                                     ZF_IN const ZFUIRect &rect)
    {
        _ZFP_ZFUIViewImpl_sys_Qt_View *nativeView = ZFCastStatic(_ZFP_ZFUIViewImpl_sys_Qt_View *, view->nativeView());
        QRectF frame = ZFImpl_sys_Qt_ZFUIRectToQRectF(rect);
        ZFImpl_sys_Qt_BaseView::ForceGeometry(nativeView->_ZFP_nativeImplView, frame);
    }
    virtual zffloat nativeViewScaleForImpl(ZF_IN void *nativeView)
    {
        return 1;
    }
    virtual zffloat nativeViewScaleForPhysicalPixel(ZF_IN void *nativeView)
    {
        return 1;
    }

    // ============================================================
    // properties
public:
    virtual void viewVisible(ZF_IN ZFUIView *view,
                             ZF_IN zfbool viewVisible)
    {
        _ZFP_ZFUIViewImpl_sys_Qt_View  *nativeView = ZFCastStatic(_ZFP_ZFUIViewImpl_sys_Qt_View *, view->nativeView());
        nativeView->setVisible(viewVisible);
    }
    virtual void viewAlpha(ZF_IN ZFUIView *view,
                           ZF_IN zffloat viewAlpha)
    {
        _ZFP_ZFUIViewImpl_sys_Qt_View *nativeViewTmp = ZFCastStatic(_ZFP_ZFUIViewImpl_sys_Qt_View *, view->nativeView());
        if(viewAlpha == 1)
        {
            nativeViewTmp->setGraphicsEffect(zfnull);
        }
        else
        {
            QGraphicsOpacityEffect *effect = qobject_cast<QGraphicsOpacityEffect *>(nativeViewTmp->graphicsEffect());
            if(effect == zfnull)
            {
                effect = new QGraphicsOpacityEffect(nativeViewTmp);
                nativeViewTmp->setGraphicsEffect(effect);
            }
            effect->setOpacity(viewAlpha);
        }
    }
    virtual void viewUIEnable(ZF_IN ZFUIView *view,
                              ZF_IN zfbool viewUIEnable)
    {
        _ZFP_ZFUIViewImpl_sys_Qt_View *nativeViewTmp = ZFCastStatic(_ZFP_ZFUIViewImpl_sys_Qt_View *, view->nativeView());
        nativeViewTmp->_ZFP_viewUIEnable = viewUIEnable;
    }
    virtual void viewUIEnableTree(ZF_IN ZFUIView *view,
                                  ZF_IN zfbool viewUIEnableTree)
    {
        _ZFP_ZFUIViewImpl_sys_Qt_View *nativeViewTmp = ZFCastStatic(_ZFP_ZFUIViewImpl_sys_Qt_View *, view->nativeView());
        nativeViewTmp->_ZFP_viewUIEnableTree = viewUIEnableTree;
        nativeViewTmp->setAcceptTouchEvents(viewUIEnableTree);
        nativeViewTmp->setAcceptedMouseButtons(viewUIEnableTree ? Qt::AllButtons : Qt::NoButton);
        this->_updateNativeImplViewMouseSetting(view);
    }
    virtual void viewMouseHoverEventEnable(ZF_IN ZFUIView *view,
                                           ZF_IN zfbool viewMouseHoverEventEnable)
    {
        _ZFP_ZFUIViewImpl_sys_Qt_View *nativeViewTmp = ZFCastStatic(_ZFP_ZFUIViewImpl_sys_Qt_View *, view->nativeView());
        nativeViewTmp->setAcceptHoverEvents(viewMouseHoverEventEnable);
    }
    virtual void viewBackgroundColor(ZF_IN ZFUIView *view,
                                     ZF_IN const ZFUIColor &viewBackgroundColor)
    {
        _ZFP_ZFUIViewImpl_sys_Qt_View *nativeViewTmp = ZFCastStatic(_ZFP_ZFUIViewImpl_sys_Qt_View *, view->nativeView());
        QPalette palette = nativeViewTmp->palette();
        palette.setColor(QPalette::Window, ZFImpl_sys_Qt_ZFUIColorToQColor(viewBackgroundColor));
        nativeViewTmp->setPalette(palette);
    }

public:
    virtual void childAdd(ZF_IN ZFUIView *parent,
                          ZF_IN ZFUIView *child,
                          ZF_IN zfindex virtualIndex,
                          ZF_IN ZFUIViewChildLayerEnum childLayer,
                          ZF_IN zfindex childLayerIndex)
    {
        _ZFP_ZFUIViewImpl_sys_Qt_View *nativeView = ZFCastStatic(_ZFP_ZFUIViewImpl_sys_Qt_View *, parent->nativeView());
        QGraphicsWidget *nativeChildView = ZFCastStatic(QGraphicsWidget *, child->nativeView());
        nativeView->_ZFP_layoutProxy->childAdd(nativeChildView, virtualIndex);
    }
    virtual void childRemove(ZF_IN ZFUIView *parent,
                             ZF_IN ZFUIView *child,
                             ZF_IN zfindex virtualIndex,
                             ZF_IN ZFUIViewChildLayerEnum childLayer,
                             ZF_IN zfindex childLayerIndex)
    {
        _ZFP_ZFUIViewImpl_sys_Qt_View *nativeView = ZFCastStatic(_ZFP_ZFUIViewImpl_sys_Qt_View *, parent->nativeView());
        nativeView->_ZFP_layoutProxy->childRemoveAtIndex(virtualIndex);
    }
    virtual void childRemoveAllForDealloc(ZF_IN ZFUIView *parent)
    {
        _ZFP_ZFUIViewImpl_sys_Qt_View *nativeView = ZFCastStatic(_ZFP_ZFUIViewImpl_sys_Qt_View *, parent->nativeView());
        nativeView->_ZFP_layoutProxy->childRemoveAll();
        if(nativeView->_ZFP_nativeImplView != NULL)
        {
            nativeView->_ZFP_layoutProxy->childAdd(nativeView->_ZFP_nativeImplView, 0);
        }
    }

public:
    virtual void viewFrame(ZF_IN ZFUIView *view,
                           ZF_IN const ZFUIRect &rect)
    {
        _ZFP_ZFUIViewImpl_sys_Qt_View *nativeViewTmp = ZFCastStatic(_ZFP_ZFUIViewImpl_sys_Qt_View *, view->nativeView());
        QRectF frame = ZFImpl_sys_Qt_ZFUIRectToQRectF(rect);
        nativeViewTmp->_ZFP_layoutProxy->_ZFP_layoutedSize = frame.size();
        ZFImpl_sys_Qt_BaseView::ForceGeometry(nativeViewTmp, frame);
    }

    virtual void layoutRequest(ZF_IN ZFUIView *view)
    {
        _ZFP_ZFUIViewImpl_sys_Qt_View *nativeViewTmp = ZFCastStatic(_ZFP_ZFUIViewImpl_sys_Qt_View *, view->nativeView());
        nativeViewTmp->_ZFP_layoutProxy->_ZFP_layoutedSize = QSize(-1, -1);
        nativeViewTmp->_ZFP_layoutProxy->invalidate();
    }

    virtual void measureNativeView(ZF_OUT ZFUISize &ret,
                                   ZF_IN void *nativeView,
                                   ZF_IN const ZFUISize &sizeHint)
    {
        QGraphicsWidget *nativeViewTmp = ZFCastStatic(QGraphicsWidget *, nativeView);
        QSizeF minSizeSaved = nativeViewTmp->minimumSize();
        QSizeF maxSizeSaved = nativeViewTmp->maximumSize();
        nativeViewTmp->setMinimumSize(0, 0);
        nativeViewTmp->setMaximumSize(
                sizeHint.width >= 0 ? sizeHint.width : QWIDGETSIZE_MAX,
                sizeHint.height >= 0 ? sizeHint.height : QWIDGETSIZE_MAX
            );
        QSizeF t = nativeViewTmp->effectiveSizeHint(Qt::MinimumDescent);
        nativeViewTmp->setMinimumSize(minSizeSaved);
        nativeViewTmp->setMaximumSize(maxSizeSaved);
        ZFImpl_sys_Qt_ZFUISizeFromQSizeFT(ret, t);
    }
private:
    void _updateNativeImplViewMouseSetting(ZF_IN ZFUIView *view)
    {
        _ZFP_ZFUIViewImpl_sys_Qt_View *nativeViewTmp = ZFCastStatic(_ZFP_ZFUIViewImpl_sys_Qt_View *, view->nativeView());
        if(nativeViewTmp->_ZFP_nativeImplView != NULL)
        {
            nativeViewTmp->_ZFP_nativeImplView->setAcceptTouchEvents(view->viewUIEnableTree());
            nativeViewTmp->_ZFP_nativeImplView->setAcceptedMouseButtons(view->viewUIEnableTree() ? Qt::AllButtons : Qt::NoButton);
        }
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIViewImpl_sys_Qt)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUIViewImpl_sys_Qt)

// ============================================================
// ZFUIViewFocus
ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIViewFocusImpl_sys_Qt, ZFUIViewFocus, ZFProtocolLevel::e_SystemHigh)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Qt:QGraphicsWidget")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIView, "Qt:QGraphicsWidget")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()
public:
    virtual void viewFocusable(ZF_IN ZFUIView *view,
                               ZF_IN zfbool viewFocusable)
    {
        _ZFP_ZFUIViewImpl_sys_Qt_View *nativeView = ZFCastStatic(_ZFP_ZFUIViewImpl_sys_Qt_View *, view->nativeView());
        _ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_viewFocusable(nativeView->_ZFP_focusProxyToken, viewFocusable);
    }
    virtual zfbool viewFocused(ZF_IN ZFUIView *view)
    {
        _ZFP_ZFUIViewImpl_sys_Qt_View *nativeView = ZFCastStatic(_ZFP_ZFUIViewImpl_sys_Qt_View *, view->nativeView());
        return _ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_viewFocused(nativeView->_ZFP_focusProxyToken);
    }
    virtual void viewFocusRequest(ZF_IN ZFUIView *view,
                                  ZF_IN zfbool viewFocus)
    {
        _ZFP_ZFUIViewImpl_sys_Qt_View *nativeView = ZFCastStatic(_ZFP_ZFUIViewImpl_sys_Qt_View *, view->nativeView());
        _ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_viewFocusRequest(nativeView->_ZFP_focusProxyToken, viewFocus);
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIViewFocusImpl_sys_Qt)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUIViewFocusImpl_sys_Qt)

ZF_NAMESPACE_GLOBAL_END

#include "ZFProtocolZFUIView_sys_Qt.moc"
#endif // #if ZF_ENV_sys_Qt

