#include "ZFImpl_sys_Qt_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIScrollView.h"

#if ZF_ENV_sys_Qt

#include "ZFProtocolZFUIView_sys_Qt.h"
#include <QTime>
#include <QGraphicsWidget>
#include <QGraphicsProxyWidget>
#include <QGraphicsSceneMouseEvent>
#include <QLayout>
#include <QTimer>
#include <QApplication>
#include <QSet>
#include <QMap>

// ============================================================
static zftimet _ZFP_ZFUIScrollViewImpl_sys_Qt_timestamp(void)
{
    return (zftimet)QDateTime::currentMSecsSinceEpoch();
}

class _ZFP_ZFUIScrollViewImpl_sys_Qt_MouseEventTagData
{
public:
    zfbool cancelFlag;
    QSet<QGraphicsWidget *> forwardedFlag;
public:
    _ZFP_ZFUIScrollViewImpl_sys_Qt_MouseEventTagData(void)
    : cancelFlag(zffalse)
    , forwardedFlag()
    {
    }
public:
    void copyFrom(ZF_IN _ZFP_ZFUIScrollViewImpl_sys_Qt_MouseEventTagData const &ref)
    {
        this->cancelFlag = ref.cancelFlag;
        // do not copy forwardedFlag
    }
    zfbool preventFutureForward(ZF_IN QGraphicsWidget *v)
    {
        if(this->forwardedFlag.isEmpty())
        {
            return zffalse;
        }
        while(v != NULL)
        {
            if(this->forwardedFlag.contains(v))
            {
                return zffalse;
            }
            v = v->parentWidget();
        }
        return zftrue;
    }
};

static QMap<QEvent *, _ZFP_ZFUIScrollViewImpl_sys_Qt_MouseEventTagData> &_ZFP_ZFUIScrollViewImpl_sys_Qt_MouseEventTagMap(void)
{
    static QMap<QEvent *, _ZFP_ZFUIScrollViewImpl_sys_Qt_MouseEventTagData> m;
    return m;
}
static _ZFP_ZFUIScrollViewImpl_sys_Qt_MouseEventTagData *_ZFP_ZFUIScrollViewImpl_sys_Qt_MouseEventTagCheck(ZF_IN QEvent *event)
{
    QMap<QEvent *, _ZFP_ZFUIScrollViewImpl_sys_Qt_MouseEventTagData> &m = _ZFP_ZFUIScrollViewImpl_sys_Qt_MouseEventTagMap();
    QMap<QEvent *, _ZFP_ZFUIScrollViewImpl_sys_Qt_MouseEventTagData>::iterator it = m.find(event);
    if(it != m.end())
    {
        return &(it.value());
    }
    return zfnull;
}
static _ZFP_ZFUIScrollViewImpl_sys_Qt_MouseEventTagData *_ZFP_ZFUIScrollViewImpl_sys_Qt_MouseEventTagAccess(ZF_IN QEvent *event)
{
    QMap<QEvent *, _ZFP_ZFUIScrollViewImpl_sys_Qt_MouseEventTagData> &m = _ZFP_ZFUIScrollViewImpl_sys_Qt_MouseEventTagMap();
    QMap<QEvent *, _ZFP_ZFUIScrollViewImpl_sys_Qt_MouseEventTagData>::iterator it = m.find(event);
    if(it != m.end())
    {
        return &(it.value());
    }
    m.insert(event, _ZFP_ZFUIScrollViewImpl_sys_Qt_MouseEventTagData());
    return &(m.find(event).value());
}

static QGraphicsSceneMouseEvent *_ZFP_ZFUIScrollViewImpl_sys_Qt_MouseEventClone(ZF_IN QGraphicsSceneMouseEvent *event,
                                                                                ZF_IN QEvent::Type type,
                                                                                ZF_IN QPointF const &localPos)
{
    QGraphicsSceneMouseEvent *ret = ZFUIViewImpl_sys_Qt_mouseEventClone(event, type, localPos);
    _ZFP_ZFUIScrollViewImpl_sys_Qt_MouseEventTagData *tag = _ZFP_ZFUIScrollViewImpl_sys_Qt_MouseEventTagCheck(event);
    if(tag != zfnull)
    {
        _ZFP_ZFUIScrollViewImpl_sys_Qt_MouseEventTagAccess(ret)->copyFrom(*tag);
    }
    return ret;
}
static zfbool _ZFP_ZFUIScrollViewImpl_sys_Qt_ChildOffset(ZF_OUT zffloat &xOffset,
                                                         ZF_OUT zffloat &yOffset,
                                                         ZF_IN QGraphicsWidget *parent,
                                                         ZF_IN QGraphicsWidget *child)
{
    while(child != zfnull && child != parent)
    {
        xOffset += child->geometry().x();
        yOffset += child->geometry().y();
        child = child->parentWidget();
    }
    return (child == parent);
}

static zfbool _ZFP_ZFUIViewImpl_sys_Qt_isMouseCancel(ZF_IN QGraphicsSceneMouseEvent *event)
{
    _ZFP_ZFUIScrollViewImpl_sys_Qt_MouseEventTagData *tag = _ZFP_ZFUIScrollViewImpl_sys_Qt_MouseEventTagCheck(event);
    return (tag != zfnull && tag->cancelFlag);
}
ZF_STATIC_INITIALIZER_INIT(ZFUIViewImpl_sys_Qt_isMouseCancel)
{
    ZFUIViewImpl_sys_Qt_isMouseCancel = _ZFP_ZFUIViewImpl_sys_Qt_isMouseCancel;
}
ZF_STATIC_INITIALIZER_END(ZFUIViewImpl_sys_Qt_isMouseCancel)

// ============================================================
// QApplication::sendEvent would cause recursive event dispatch,
// we only need exactly the one we send
static zfindex _ZFP_ZFUIScrollViewImpl_sys_Qt_sendEvent_flag = 0;
static QMap<QEvent *, QObject *> _ZFP_ZFUIScrollViewImpl_sys_Qt_sendEvent_sendingEvent;

// ============================================================
// native scroll view
class _ZFP_ZFUIScrollViewImpl_sys_Qt_ScrollContainer : public ZFImpl_sys_Qt_BaseView
{
    Q_OBJECT
};
class _ZFP_ZFUIScrollViewImpl_sys_Qt_ScrollView : public QGraphicsWidget, public ZFUIScrollViewImplHelperProtocol
{
    Q_OBJECT

public:
    ZFPROTOCOL_INTERFACE_CLASS(ZFUIScrollView) *_ZFP_impl;
    ZFUIScrollView *_ZFP_ownerZFUIScrollView;
    ZFImpl_sys_Qt_Layout *_ZFP_layoutProxy;
    ZFUIScrollViewImplHelper _ZFP_scrollViewImplHelper;
    QTimer _ZFP_scrollAnimationTimer;
    zfindex _ZFP_scrollViewBgViewCount;
    _ZFP_ZFUIScrollViewImpl_sys_Qt_ScrollContainer *_ZFP_scrollViewContentView;
    ZFImpl_sys_Qt_Layout *_ZFP_scrollViewContentViewLayoutProxy;

public:
    _ZFP_ZFUIScrollViewImpl_sys_Qt_ScrollView(void)
    : QGraphicsWidget()
    , _ZFP_impl(zfnull)
    , _ZFP_ownerZFUIScrollView(zfnull)
    , _ZFP_layoutProxy(new ZFImpl_sys_Qt_Layout())
    , _ZFP_scrollViewImplHelper()
    , _ZFP_scrollAnimationTimer()
    , _ZFP_scrollViewBgViewCount(0)
    , _ZFP_scrollViewContentView(zfnull)
    , _ZFP_scrollViewContentViewLayoutProxy(new ZFImpl_sys_Qt_Layout())
    {
        this->setLayout(_ZFP_layoutProxy);
        this->setAcceptTouchEvents(true);

        _ZFP_scrollViewContentView = new _ZFP_ZFUIScrollViewImpl_sys_Qt_ScrollContainer();
        _ZFP_layoutProxy->childAdd(_ZFP_scrollViewContentView, 0);
        _ZFP_scrollViewContentView->setLayout(_ZFP_scrollViewContentViewLayoutProxy);

        QApplication::instance()->installEventFilter(this);
    }
    ~_ZFP_ZFUIScrollViewImpl_sys_Qt_ScrollView(void)
    {
        QApplication::instance()->removeEventFilter(this);

        zfCoreAssert(_ZFP_scrollViewBgViewCount == 0);
        _ZFP_layoutProxy->childRemove(0);
        delete _ZFP_scrollViewContentView;
    }

public slots:
    void _ZFP_scrollAnimationTimerOnActivate(void)
    {
        this->_ZFP_impl->notifyScrollViewScrollAnimation(this->_ZFP_ownerZFUIScrollView, _ZFP_ZFUIScrollViewImpl_sys_Qt_timestamp());
    }

    // ============================================================
    // scroll drag impl
protected:
    bool eventFilter(QObject *obj, QEvent *event)
    {
        QGraphicsWidget *child = qobject_cast<QGraphicsWidget *>(obj);
        if(child == zfnull || child->window() != this->window())
        {
            return false;
        }
        ZFUIMouseActionEnum mouseAction = ZFUIMouseAction::e_MouseCancel;
        switch(event->type())
        {
            case QEvent::GraphicsSceneMousePress:
            case QEvent::GraphicsSceneMouseDoubleClick:
                ZFImpl_sys_Qt_QObjectTag(obj, "_ZFP_ZFImpl_ZFUIScrollView_sys_Qt_mouseDownTag", QVariant::fromValue(zftrue));
                mouseAction = ZFUIMouseAction::e_MouseDown;
                break;
            case QEvent::GraphicsSceneMouseMove:
                if(!ZFImpl_sys_Qt_QObjectTag(obj, "_ZFP_ZFImpl_ZFUIScrollView_sys_Qt_mouseDownTag").isValid())
                {
                    return false;
                }
                mouseAction = ZFUIMouseAction::e_MouseMove;
                break;
            case QEvent::GraphicsSceneMouseRelease:
                ZFImpl_sys_Qt_QObjectTag(obj, "_ZFP_ZFImpl_ZFUIScrollView_sys_Qt_mouseDownTag", QVariant());
                mouseAction = ZFUIMouseAction::e_MouseUp;
                break;
            default:
                return false;
        }

        if(_ZFP_ZFUIScrollViewImpl_sys_Qt_sendEvent_flag
            && _ZFP_ZFUIScrollViewImpl_sys_Qt_sendEvent_sendingEvent.value(event) != obj)
        {
            return false;
        }

        QGraphicsWidget *parentScrollView = this->parentWidget();
        while(parentScrollView != NULL)
        {
            if(qobject_cast<_ZFP_ZFUIScrollViewImpl_sys_Qt_ScrollView *>(parentScrollView) != NULL)
            {
                break;
            }
            parentScrollView = parentScrollView->parentWidget();
        }

        _ZFP_ZFUIScrollViewImpl_sys_Qt_MouseEventTagData *tag = _ZFP_ZFUIScrollViewImpl_sys_Qt_MouseEventTagCheck(event);
        if(parentScrollView != NULL && !(tag != zfnull && tag->preventFutureForward(parentScrollView)))
        {
            // let parent to resolve
            return false;
        }
        if(tag != zfnull && tag->preventFutureForward(this))
        {
            // this is forwarded event, ignore
            return false;
        }
        zffloat xOffset = 0;
        zffloat yOffset = 0;
        if(!_ZFP_ZFUIScrollViewImpl_sys_Qt_ChildOffset(xOffset, yOffset, this, child))
        {
            return false;
        }

        QGraphicsSceneMouseEvent *e = (QGraphicsSceneMouseEvent *)event;

        QGraphicsWidget *touchedFgView = this->_ZFP_findFgView(e->pos().x() + xOffset, e->pos().y() + yOffset);
        if(touchedFgView != zfnull)
        {
            // cloned even if no extra processing
            // to ensure tag map would be cleaned to the event
            QGraphicsSceneMouseEvent *t = _ZFP_ZFUIScrollViewImpl_sys_Qt_MouseEventClone(e, e->type(), QPointF(e->pos().x() + xOffset, e->pos().y() + yOffset));

            QEvent *eTmp = (QEvent *)this->translateFromParentToChild(touchedFgView, t, t->pos().x() + xOffset, t->pos().y() + yOffset);
            _ZFP_ZFUIScrollViewImpl_sys_Qt_MouseEventTagAccess(eTmp)->forwardedFlag.insert(touchedFgView);

            ++_ZFP_ZFUIScrollViewImpl_sys_Qt_sendEvent_flag;
            _ZFP_ZFUIScrollViewImpl_sys_Qt_sendEvent_sendingEvent.insert(eTmp, touchedFgView);
            QApplication::sendEvent(touchedFgView->scene(), eTmp);
            _ZFP_ZFUIScrollViewImpl_sys_Qt_sendEvent_sendingEvent.remove(eTmp);
            --_ZFP_ZFUIScrollViewImpl_sys_Qt_sendEvent_flag;

            this->mouseEventCleanup(eTmp);
            return true;
        }

        zfRetain(this->_ZFP_ownerZFUIScrollView);
        zfblockedRelease(this->_ZFP_ownerZFUIScrollView);

        // cloned even if no extra processing
        // to ensure tag map would be cleaned to the event
        QGraphicsSceneMouseEvent *t = _ZFP_ZFUIScrollViewImpl_sys_Qt_MouseEventClone(e, e->type(), QPointF(e->pos().x() + xOffset, e->pos().y() + yOffset));
        _ZFP_scrollViewImplHelper.interceptMouse(t, mouseAction);
        this->mouseEventCleanup(t);
        return true;
    }
public:
    virtual zftimet nativeTime(void)
    {
        return _ZFP_ZFUIScrollViewImpl_sys_Qt_timestamp();
    }
    virtual void mouseEventPos(ZF_OUT ZFUIPoint &ret, ZF_IN void *nativeMouseEvent)
    {
        QGraphicsSceneMouseEvent *e = ZFCastStatic(QGraphicsSceneMouseEvent *, nativeMouseEvent);
        ret.x = (zffloat)e->pos().x();
        ret.y = (zffloat)e->pos().y();
    }
    virtual void *mouseEventClone(ZF_IN void *nativeMouseEvent,
                                  ZF_IN_OPT zfbool changeMouseAction = zffalse,
                                  ZF_IN_OPT ZFUIMouseActionEnum mouseAction = ZFUIMouseAction::e_MouseCancel)
    {
        QGraphicsSceneMouseEvent *e = ZFCastStatic(QGraphicsSceneMouseEvent *, nativeMouseEvent);
        QEvent::Type type = e->type();
        if(changeMouseAction)
        {
            switch(mouseAction)
            {
                case ZFUIMouseAction::e_MouseDown:
                    type = QEvent::GraphicsSceneMousePress;
                    break;
                case ZFUIMouseAction::e_MouseMove:
                    type = QEvent::GraphicsSceneMouseMove;
                    break;
                case ZFUIMouseAction::e_MouseUp:
                    type = QEvent::GraphicsSceneMouseRelease;
                    break;
                case ZFUIMouseAction::e_MouseCancel:
                    type = QEvent::GraphicsSceneMouseRelease;
                    break;
                default:
                    zfCoreCriticalShouldNotGoHere();
                    return zfnull;
            }
        }
        QGraphicsSceneMouseEvent *ret = _ZFP_ZFUIScrollViewImpl_sys_Qt_MouseEventClone(e, type, e->pos());
        if(changeMouseAction && mouseAction == ZFUIMouseAction::e_MouseCancel)
        {
            _ZFP_ZFUIScrollViewImpl_sys_Qt_MouseEventTagAccess(ret)->cancelFlag = zftrue;
        }
        return ret;
    }
    virtual void mouseEventCleanup(ZF_IN void *nativeMouseEvent)
    {
        QGraphicsSceneMouseEvent *e = ZFCastStatic(QGraphicsSceneMouseEvent *, nativeMouseEvent);
        _ZFP_ZFUIScrollViewImpl_sys_Qt_MouseEventTagMap().remove(e);
        delete e;
    }
    virtual void mouseEventForward(ZF_IN void *nativeChild,
                                   ZF_IN void *nativeMouseEvent)
    {
        QGraphicsWidget *v = ZFCastStatic(QGraphicsWidget *, nativeChild);
        QGraphicsSceneMouseEvent *e = ZFCastStatic(QGraphicsSceneMouseEvent *, nativeMouseEvent);
        _ZFP_ZFUIScrollViewImpl_sys_Qt_MouseEventTagAccess(e)->forwardedFlag.insert(v);

        // forwarded mouse event is not QEvent::spontaneous(),
        // QApplication won't dispatch focusInEvent,
        // try to manually focus
        if(v->focusPolicy() != Qt::NoFocus)
        {
            v->scene()->setFocusItem(v, Qt::MouseFocusReason);
            QGraphicsProxyWidget *proxy = NULL;
            if(v->layout() != NULL)
            {
                for(int i = 0; i < v->layout()->count(); ++i)
                {
                    QGraphicsItem *item = v->layout()->itemAt(i)->graphicsItem();
                    if(item == NULL || !item->isWidget())
                    {
                        continue;
                    }
                    proxy = qobject_cast<QGraphicsProxyWidget *>((QGraphicsWidget *)item);
                    if(proxy != NULL)
                    {
                        break;
                    }
                }
            }
            if(proxy != NULL && proxy->widget() != NULL && proxy->focusPolicy() != Qt::NoFocus)
            {
                proxy->widget()->setFocus(Qt::MouseFocusReason);
            }
        }

        ++_ZFP_ZFUIScrollViewImpl_sys_Qt_sendEvent_flag;
        _ZFP_ZFUIScrollViewImpl_sys_Qt_sendEvent_sendingEvent.insert(e, v);
        QApplication::sendEvent(v->scene(), e);
        _ZFP_ZFUIScrollViewImpl_sys_Qt_sendEvent_sendingEvent.remove(e);
        --_ZFP_ZFUIScrollViewImpl_sys_Qt_sendEvent_flag;
    }
    zfbool _ZFP_isSubclass(ZF_IN const QMetaObject *cls0, ZF_IN const QMetaObject *cls1)
    {
        while(cls0 != NULL && cls0 != cls1)
        {
            cls0 = cls0->superClass();
        }
        return (cls0 == cls1);
    }

    QGraphicsWidget *_ZFP_findFgView(ZF_IN zffloat x, ZF_IN zffloat y)
    {
        ZFCoreArrayPOD<ZFUIView *> fgViews = _ZFP_ownerZFUIScrollView->internalFgViewArray();
        for(zfindex i = fgViews.count() - 1; i != zfindexMax(); --i)
        {
            QGraphicsWidget *t = ZFCastStatic(QGraphicsWidget *, fgViews[i]->nativeView());
            if(t->isVisible() && t->acceptTouchEvents() && t->geometry().contains(x, y))
            {
                return t;
            }
        }
        return zfnull;
    }
    QGraphicsWidget *_ZFP_findChildRecursive(ZF_IN QGraphicsWidget *parent, ZF_IN zffloat x, ZF_IN zffloat y, ZF_IN zfbool findScrollView)
    {
        if(findScrollView && qobject_cast<_ZFP_ZFUIScrollViewImpl_sys_Qt_ScrollView *>(parent) != zfnull)
        {
            return parent;
        }
        const QObjectList &children = parent->children();
        for(int i = children.size() - 1; i != -1; --i)
        {
            QGraphicsWidget *t = qobject_cast<QGraphicsWidget *>(children.at(i));
            if(t != zfnull && t->isEnabled() && t->geometry().contains(x, y))
            {
                if(qobject_cast<_ZFP_ZFUIScrollViewImpl_sys_Qt_ScrollView *>(t) != zfnull)
                {
                    return t;
                }
                QGraphicsWidget *inner = _ZFP_findChildRecursive(t, x - t->geometry().x(), y - t->geometry().y(), findScrollView);
                if(inner != zfnull)
                {
                    return inner;
                }
                if(!findScrollView)
                {
                    return t;
                }
            }
        }
        return zfnull;
    }
    QGraphicsWidget *_ZFP_findChild(ZF_IN_OUT zfindex &beforeIndex, ZF_IN zffloat x, ZF_IN zffloat y)
    {
        --beforeIndex;
        x -= _ZFP_scrollViewContentView->geometry().x();
        y -= _ZFP_scrollViewContentView->geometry().y();
        for( ; beforeIndex != zfindexMax(); --beforeIndex)
        {
            QGraphicsWidget *t = _ZFP_scrollViewContentViewLayoutProxy->childAtIndex(beforeIndex);
            if(t->isEnabled() && t->geometry().contains(x, y))
            {
                return t;
            }
        }
        return zfnull;
    }
    virtual ZFUIScrollViewImplHelper *findTouchedChildScrollView(ZF_IN zffloat x, ZF_IN zffloat y)
    {
        zfindex beforeIndex = _ZFP_scrollViewContentViewLayoutProxy->count();
        do
        {
            QGraphicsWidget *t = _ZFP_findChild(beforeIndex, x, y);
            if(t != zfnull)
            {
                QGraphicsWidget *innerChild = _ZFP_findChildRecursive(
                    t,
                    x - _ZFP_scrollViewContentView->geometry().x() - t->geometry().x(),
                    y - _ZFP_scrollViewContentView->geometry().y() - t->geometry().y(),
                    zftrue);
                if(innerChild != zfnull)
                {
                    return &(qobject_cast<_ZFP_ZFUIScrollViewImpl_sys_Qt_ScrollView *>(innerChild)->_ZFP_scrollViewImplHelper);
                }
            }
        } while(beforeIndex != zfindexMax());
        return zfnull;
    }
    virtual void findTouchedChildScrollViewCleanup(ZF_IN void *nativeChild)
    {
        // nothing to do
    }
    virtual void *findTouchedChild(ZF_IN zffloat x, ZF_IN zffloat y)
    {
        zfindex beforeIndex = _ZFP_scrollViewContentViewLayoutProxy->count();
        do
        {
            QGraphicsWidget *t = _ZFP_findChild(beforeIndex, x, y);
            if(t != zfnull)
            {
                if(qobject_cast<_ZFP_ZFUIScrollViewImpl_sys_Qt_ScrollView *>(t) != zfnull)
                {
                    return t;
                }
                QGraphicsWidget *innerChild = _ZFP_findChildRecursive(
                    t,
                    x - _ZFP_scrollViewContentView->geometry().x() - t->geometry().x(),
                    y - _ZFP_scrollViewContentView->geometry().y() - t->geometry().y(),
                    zffalse);
                if(innerChild != zfnull)
                {
                    return innerChild;
                }
                return t;
            }
        } while(beforeIndex != zfindexMax());
        return zfnull;
    }
    virtual void findTouchedChildCleanup(ZF_IN void *nativeChild)
    {
        // nothing to do
    }

    virtual void *translateFromParentToChild(ZF_IN void *nativeChild,
                                             ZF_IN_OUT void *nativeMouseEvent,
                                             ZF_IN zffloat const &xInParent, ZF_IN zffloat const &yInParent)
    {
        QGraphicsSceneMouseEvent *e = ZFCastStatic(QGraphicsSceneMouseEvent *, nativeMouseEvent);
        zffloat xOffset = 0;
        zffloat yOffset = 0;
        _ZFP_ZFUIScrollViewImpl_sys_Qt_ChildOffset(xOffset, yOffset, this, ZFCastStatic(QGraphicsWidget *, nativeChild));
        QGraphicsSceneMouseEvent *ret = _ZFP_ZFUIScrollViewImpl_sys_Qt_MouseEventClone(e, e->type(), QPointF(xInParent - xOffset, yInParent - yOffset));
        this->mouseEventCleanup(e);
        return ret;
    }
    virtual void *translateFromChildToParent(ZF_IN void *nativeChild,
                                             ZF_IN_OUT void *nativeMouseEvent,
                                             ZF_IN zffloat const &xInChild, ZF_IN zffloat const &yInChild)
    {
        QGraphicsSceneMouseEvent *e = ZFCastStatic(QGraphicsSceneMouseEvent *, nativeMouseEvent);
        zffloat xOffset = 0;
        zffloat yOffset = 0;
        _ZFP_ZFUIScrollViewImpl_sys_Qt_ChildOffset(xOffset, yOffset, this, ZFCastStatic(QGraphicsWidget *, nativeChild));
        QGraphicsSceneMouseEvent *ret = _ZFP_ZFUIScrollViewImpl_sys_Qt_MouseEventClone(e, e->type(), QPointF(xInChild + xOffset, yInChild + yOffset));
        this->mouseEventCleanup(e);
        return ret;
    }
};

// ============================================================
// impl
ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIScrollViewImpl_sys_Qt, ZFUIScrollView, ZFProtocolLevel::e_SystemHigh)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Qt:QGraphicsWidget")

public:
    virtual void *nativeScrollViewCreate(ZF_IN ZFUIScrollView *scrollView)
    {
        _ZFP_ZFUIScrollViewImpl_sys_Qt_ScrollView *nativeScrollView = new _ZFP_ZFUIScrollViewImpl_sys_Qt_ScrollView();
        nativeScrollView->_ZFP_impl = this;
        nativeScrollView->_ZFP_ownerZFUIScrollView = scrollView;
        nativeScrollView->_ZFP_scrollViewImplHelper.implProtocol = nativeScrollView;
        nativeScrollView->_ZFP_scrollViewImplHelper.scrollView = scrollView;
        return nativeScrollView;
    }
    virtual void nativeScrollViewDestroy(ZF_IN ZFUIScrollView *scrollView,
                                         ZF_IN void *nativeScrollView)
    {
        _ZFP_ZFUIScrollViewImpl_sys_Qt_ScrollView *nativeScrollViewTmp = ZFCastStatic(_ZFP_ZFUIScrollViewImpl_sys_Qt_ScrollView *, nativeScrollView);
        delete nativeScrollViewTmp;
    }

public:
    virtual void scrollEnable(ZF_IN ZFUIScrollView *scrollView,
                              ZF_IN zfbool scrollEnable)
    {
        // nothing to do, scroll impl helper would solve this
    }
    virtual void scrollBounce(ZF_IN ZFUIScrollView *scrollView,
                              ZF_IN zfbool scrollBounceHorizontal,
                              ZF_IN zfbool scrollBounceVertical,
                              ZF_IN zfbool scrollBounceHorizontalAlways,
                              ZF_IN zfbool scrollBounceVerticalAlways)
{
        // nothing to do, scroll impl helper would solve this
    }
    virtual void scrollContentFrame(ZF_IN ZFUIScrollView *scrollView,
                                    ZF_IN const ZFUIRect &frame)
    {
        _ZFP_ZFUIScrollViewImpl_sys_Qt_ScrollView *nativeScrollView = ZFCastStatic(_ZFP_ZFUIScrollViewImpl_sys_Qt_ScrollView *, scrollView->nativeImplView());
        QRect nativeFrame = ZFImpl_sys_Qt_ZFUIRectToQRect(frame);
        nativeScrollView->_ZFP_scrollViewContentView->forceGeometry(nativeFrame);
    }
    virtual zftimet scrollAnimationStart(ZF_IN ZFUIScrollView *scrollView,
                                         ZF_IN zftimet recommendTimerInterval)
    {
        _ZFP_ZFUIScrollViewImpl_sys_Qt_ScrollView *nativeScrollView = ZFCastStatic(_ZFP_ZFUIScrollViewImpl_sys_Qt_ScrollView *, scrollView->nativeImplView());
        nativeScrollView->_ZFP_scrollAnimationTimer.connect(
            &(nativeScrollView->_ZFP_scrollAnimationTimer), SIGNAL(timeout()),
            nativeScrollView, SLOT(_ZFP_scrollAnimationTimerOnActivate()));
        nativeScrollView->_ZFP_scrollAnimationTimer.moveToThread(QApplication::instance()->thread());
        nativeScrollView->_ZFP_scrollAnimationTimer.start((zfuint)recommendTimerInterval);
        return _ZFP_ZFUIScrollViewImpl_sys_Qt_timestamp();
    }
    virtual void scrollAnimationStop(ZF_IN ZFUIScrollView *scrollView)
    {
        _ZFP_ZFUIScrollViewImpl_sys_Qt_ScrollView *nativeScrollView = ZFCastStatic(_ZFP_ZFUIScrollViewImpl_sys_Qt_ScrollView *, scrollView->nativeImplView());
        nativeScrollView->_ZFP_scrollAnimationTimer.disconnect();
        nativeScrollView->_ZFP_scrollAnimationTimer.stop();
    }

public:
    virtual void scrollChildAdd(ZF_IN ZFUIScrollView *parent,
                                ZF_IN ZFUIView *child,
                                ZF_IN zfindex atIndex)
    {
        _ZFP_ZFUIScrollViewImpl_sys_Qt_ScrollView *nativeScrollView = ZFCastStatic(_ZFP_ZFUIScrollViewImpl_sys_Qt_ScrollView *, parent->nativeImplView());
        nativeScrollView->_ZFP_scrollViewContentViewLayoutProxy->childAdd(ZFCastStatic(QGraphicsWidget *, child->nativeView()), atIndex);
    }
    virtual void scrollChildRemove(ZF_IN ZFUIScrollView *parent,
                                   ZF_IN ZFUIView *child,
                                   ZF_IN zfindex atIndex)
    {
        _ZFP_ZFUIScrollViewImpl_sys_Qt_ScrollView *nativeScrollView = ZFCastStatic(_ZFP_ZFUIScrollViewImpl_sys_Qt_ScrollView *, parent->nativeImplView());
        nativeScrollView->_ZFP_scrollViewContentViewLayoutProxy->childRemoveAtIndex(atIndex);
    }
    virtual void scrollChildRemoveAllForDealloc(ZF_IN ZFUIScrollView *parent)
    {
        _ZFP_ZFUIScrollViewImpl_sys_Qt_ScrollView *nativeScrollView = ZFCastStatic(_ZFP_ZFUIScrollViewImpl_sys_Qt_ScrollView *, parent->nativeImplView());
        nativeScrollView->_ZFP_scrollViewContentViewLayoutProxy->childRemoveAll();
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIScrollViewImpl_sys_Qt)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUIScrollViewImpl_sys_Qt)

ZF_NAMESPACE_GLOBAL_END

#include "ZFProtocolZFUIScrollView_sys_Qt.moc"
#endif // #if ZF_ENV_sys_Qt

