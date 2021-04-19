#include "ZFImpl_sys_Qt_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIScrollView.h"

#include "ZFImpl_sys_Qt_QLayout.h"

#if ZF_ENV_sys_Qt

#include <QTime>
#include <QWidget>
#include <QLayout>
#include <QMouseEvent>
#include <QTimer>
#include <QCoreApplication>
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
    QSet<QWidget *> forwardedFlag;
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
    zfbool preventFutureForward(ZF_IN QWidget *v)
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

static QMouseEvent *_ZFP_ZFUIScrollViewImpl_sys_Qt_MouseEventClone(ZF_IN QMouseEvent *event,
                                                                   ZF_IN QEvent::Type type,
                                                                   ZF_IN QPointF const &localPos)
{
    QMouseEvent *ret = new QMouseEvent(
            type
            , localPos
            , event->windowPos()
            , event->screenPos()
            , event->button()
            , event->buttons()
            , event->modifiers()
        );
    ret->setTimestamp(event->timestamp());
    _ZFP_ZFUIScrollViewImpl_sys_Qt_MouseEventTagData *tag = _ZFP_ZFUIScrollViewImpl_sys_Qt_MouseEventTagCheck(event);
    if(tag != zfnull)
    {
        _ZFP_ZFUIScrollViewImpl_sys_Qt_MouseEventTagAccess(ret)->copyFrom(*tag);
    }
    return ret;
}
static zfbool _ZFP_ZFUIScrollViewImpl_sys_Qt_ChildOffset(ZF_OUT zfint &xOffset,
                                                         ZF_OUT zfint &yOffset,
                                                         ZF_IN QWidget *parent,
                                                         ZF_IN QWidget *child)
{
    while(child != zfnull && child != parent)
    {
        xOffset += child->geometry().x();
        yOffset += child->geometry().y();
        child = child->parentWidget();
    }
    return (child == parent);
}

zfbool ZFUIViewImpl_sys_Qt_isMouseCancel(ZF_IN QMouseEvent *event)
{
    _ZFP_ZFUIScrollViewImpl_sys_Qt_MouseEventTagData *tag = _ZFP_ZFUIScrollViewImpl_sys_Qt_MouseEventTagCheck(event);
    return (tag != zfnull && tag->cancelFlag);
}

// ============================================================
// QCoreApplication::sendEvent would cause recursive event dispatch,
// we only need exactly the one we send
static zfindex _ZFP_ZFUIScrollViewImpl_sys_Qt_sendEvent_flag = 0;
static QMap<QEvent *, QObject *> _ZFP_ZFUIScrollViewImpl_sys_Qt_sendEvent_sendingEvent;

// ============================================================
// native scroll view
class _ZFP_ZFUIScrollViewImpl_sys_Qt_ScrollView : public QWidget, public ZFUIScrollViewImplHelperProtocol
{
    Q_OBJECT

public:
    ZFPROTOCOL_INTERFACE_CLASS(ZFUIScrollView) *_ZFP_impl;
    ZFUIScrollView *_ZFP_ownerZFUIScrollView;
    ZFImpl_sys_Qt_QLayout *_ZFP_layoutProxy;
    ZFUIScrollViewImplHelper _ZFP_scrollViewImplHelper;
    QTimer _ZFP_scrollAnimationTimer;
    zfindex _ZFP_scrollViewBgViewCount;
    QWidget *_ZFP_scrollViewContentView;
    ZFImpl_sys_Qt_QLayout *_ZFP_scrollViewContentViewLayoutProxy;

public:
    _ZFP_ZFUIScrollViewImpl_sys_Qt_ScrollView(void)
    : QWidget()
    , _ZFP_impl(zfnull)
    , _ZFP_ownerZFUIScrollView(zfnull)
    , _ZFP_layoutProxy(new ZFImpl_sys_Qt_QLayout())
    , _ZFP_scrollViewImplHelper()
    , _ZFP_scrollAnimationTimer()
    , _ZFP_scrollViewBgViewCount(0)
    , _ZFP_scrollViewContentView(zfnull)
    , _ZFP_scrollViewContentViewLayoutProxy(new ZFImpl_sys_Qt_QLayout())
    {
        this->setLayout(_ZFP_layoutProxy);

        _ZFP_scrollViewContentView = new QWidget();
        _ZFP_layoutProxy->childAdd(zfnull, _ZFP_scrollViewContentView, 0);
        _ZFP_scrollViewContentView->setLayout(_ZFP_scrollViewContentViewLayoutProxy);

        QCoreApplication::instance()->installEventFilter(this);
    }
    ~_ZFP_ZFUIScrollViewImpl_sys_Qt_ScrollView(void)
    {
        QCoreApplication::instance()->removeEventFilter(this);

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
        QWidget *child = qobject_cast<QWidget *>(obj);
        if(child == zfnull || child->window() != this->window())
        {
            return false;
        }
        ZFUIMouseActionEnum mouseAction = ZFUIMouseAction::e_MouseCancel;
        switch(event->type())
        {
            case QEvent::MouseButtonPress:
            case QEvent::MouseButtonDblClick:
                ZFImpl_sys_Qt_QObjectTag(obj, "_ZFP_ZFImpl_ZFUIScrollView_sys_Qt_mouseDownTag", QVariant::fromValue(zftrue));
                mouseAction = ZFUIMouseAction::e_MouseDown;
                break;
            case QEvent::MouseMove:
                if(!ZFImpl_sys_Qt_QObjectTag(obj, "_ZFP_ZFImpl_ZFUIScrollView_sys_Qt_mouseDownTag").isValid())
                {
                    return false;
                }
                mouseAction = ZFUIMouseAction::e_MouseMove;
                break;
            case QEvent::MouseButtonRelease:
                ZFImpl_sys_Qt_QObjectTag(obj, "_ZFP_ZFImpl_ZFUIScrollView_sys_Qt_mouseDownTag", QVariant());
                mouseAction = ZFUIMouseAction::e_MouseUp;
                break;
            default:
                return false;
        }

        if(_ZFP_ZFUIScrollViewImpl_sys_Qt_sendEvent_flag
            && _ZFP_ZFUIScrollViewImpl_sys_Qt_sendEvent_sendingEvent.value(event) != obj)
        {
            return true;
        }

        QWidget *parentScrollView = this->parentWidget();
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
        zfint xOffset = 0;
        zfint yOffset = 0;
        if(!_ZFP_ZFUIScrollViewImpl_sys_Qt_ChildOffset(xOffset, yOffset, this, child))
        {
            return false;
        }

        QMouseEvent *e = (QMouseEvent *)event;

        QWidget *touchedFgView = this->_ZFP_findFgView(e->localPos().x() + xOffset, e->localPos().y() + yOffset);
        if(touchedFgView != zfnull)
        {
            // cloned even if no extra processing
            // to ensure tag map would be cleaned to the event
            QMouseEvent *t = _ZFP_ZFUIScrollViewImpl_sys_Qt_MouseEventClone(e, e->type(), QPointF(e->localPos().x() + xOffset, e->localPos().y() + yOffset));

            QEvent *eTmp = (QEvent *)this->translateFromParentToChild(touchedFgView, t, t->localPos().x() + xOffset, t->localPos().y() + yOffset);
            _ZFP_ZFUIScrollViewImpl_sys_Qt_MouseEventTagAccess(eTmp)->forwardedFlag.insert(touchedFgView);

            ++_ZFP_ZFUIScrollViewImpl_sys_Qt_sendEvent_flag;
            _ZFP_ZFUIScrollViewImpl_sys_Qt_sendEvent_sendingEvent.insert(eTmp, touchedFgView);
            QCoreApplication::instance()->sendEvent(touchedFgView, eTmp);
            _ZFP_ZFUIScrollViewImpl_sys_Qt_sendEvent_sendingEvent.remove(eTmp);
            --_ZFP_ZFUIScrollViewImpl_sys_Qt_sendEvent_flag;

            this->mouseEventCleanup(eTmp);
            return true;
        }

        zfRetain(this->_ZFP_ownerZFUIScrollView);
        zfblockedRelease(this->_ZFP_ownerZFUIScrollView);

        // cloned even if no extra processing
        // to ensure tag map would be cleaned to the event
        QMouseEvent *t = _ZFP_ZFUIScrollViewImpl_sys_Qt_MouseEventClone(e, e->type(), QPointF(e->localPos().x() + xOffset, e->localPos().y() + yOffset));
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
        QMouseEvent *e = ZFCastStatic(QMouseEvent *, nativeMouseEvent);
        ret.x = (zfint)e->x();
        ret.y = (zfint)e->y();
    }
    virtual void *mouseEventClone(ZF_IN void *nativeMouseEvent,
                                  ZF_IN_OPT zfbool changeMouseAction = zffalse,
                                  ZF_IN_OPT ZFUIMouseActionEnum mouseAction = ZFUIMouseAction::e_MouseCancel)
    {
        QMouseEvent *e = ZFCastStatic(QMouseEvent *, nativeMouseEvent);
        QEvent::Type type = e->type();
        if(changeMouseAction)
        {
            switch(mouseAction)
            {
                case ZFUIMouseAction::e_MouseDown:
                    type = QEvent::MouseButtonPress;
                    break;
                case ZFUIMouseAction::e_MouseMove:
                    type = QEvent::MouseMove;
                    break;
                case ZFUIMouseAction::e_MouseUp:
                    type = QEvent::MouseButtonRelease;
                    break;
                case ZFUIMouseAction::e_MouseCancel:
                    type = QEvent::MouseButtonRelease;
                    break;
                default:
                    zfCoreCriticalShouldNotGoHere();
                    return zfnull;
            }
        }
        QMouseEvent *ret = _ZFP_ZFUIScrollViewImpl_sys_Qt_MouseEventClone(e, type, e->localPos());
        if(changeMouseAction && mouseAction == ZFUIMouseAction::e_MouseCancel)
        {
            _ZFP_ZFUIScrollViewImpl_sys_Qt_MouseEventTagAccess(ret)->cancelFlag = zftrue;
        }
        return ret;
    }
    virtual void mouseEventCleanup(ZF_IN void *nativeMouseEvent)
    {
        QMouseEvent *e = ZFCastStatic(QMouseEvent *, nativeMouseEvent);
        _ZFP_ZFUIScrollViewImpl_sys_Qt_MouseEventTagMap().remove(e);
        delete e;
    }
    virtual void mouseEventForward(ZF_IN void *nativeChild,
                                   ZF_IN void *nativeMouseEvent)
    {
        QWidget *v = ZFCastStatic(QWidget *, nativeChild);
        QMouseEvent *e = ZFCastStatic(QMouseEvent *, nativeMouseEvent);
        _ZFP_ZFUIScrollViewImpl_sys_Qt_MouseEventTagAccess(e)->forwardedFlag.insert(v);

        ++_ZFP_ZFUIScrollViewImpl_sys_Qt_sendEvent_flag;
        _ZFP_ZFUIScrollViewImpl_sys_Qt_sendEvent_sendingEvent.insert(e, v);
        QCoreApplication::instance()->sendEvent(v, e);
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

    QWidget *_ZFP_findFgView(ZF_IN zfint x, ZF_IN zfint y)
    {
        ZFCoreArrayPOD<ZFUIView *> fgViews = _ZFP_ownerZFUIScrollView->internalFgViewArray();
        for(zfindex i = fgViews.count() - 1; i != zfindexMax(); --i)
        {
            QWidget *t = ZFCastStatic(QWidget *, fgViews[i]->nativeView());
            if(t->isEnabled() && t->geometry().contains(x, y))
            {
                return t;
            }
        }
        return zfnull;
    }
    QWidget *_ZFP_findChildRecursive(ZF_IN QWidget *parent, ZF_IN zfint x, ZF_IN zfint y, ZF_IN zfbool findScrollView)
    {
        if(findScrollView && qobject_cast<_ZFP_ZFUIScrollViewImpl_sys_Qt_ScrollView *>(parent) != zfnull)
        {
            return parent;
        }
        const QObjectList &children = parent->children();
        for(int i = children.size() - 1; i != -1; --i)
        {
            QWidget *t = qobject_cast<QWidget *>(children.at(i));
            if(t != zfnull && t->isEnabled() && t->geometry().contains(x, y))
            {
                if(qobject_cast<_ZFP_ZFUIScrollViewImpl_sys_Qt_ScrollView *>(t) != zfnull)
                {
                    return t;
                }
                QWidget *inner = _ZFP_findChildRecursive(t, x - t->geometry().x(), y - t->geometry().y(), findScrollView);
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
    QWidget *_ZFP_findChild(ZF_IN_OUT zfindex &beforeIndex, ZF_IN zfint x, ZF_IN zfint y)
    {
        --beforeIndex;
        x -= _ZFP_scrollViewContentView->geometry().x();
        y -= _ZFP_scrollViewContentView->geometry().y();
        for( ; beforeIndex != zfindexMax(); --beforeIndex)
        {
            QWidget *t = _ZFP_scrollViewContentViewLayoutProxy->itemAt(beforeIndex)->widget();
            if(t->isEnabled() && t->geometry().contains(x, y))
            {
                return t;
            }
        }
        return zfnull;
    }
    virtual ZFUIScrollViewImplHelper *findTouchedChildScrollView(ZF_IN zfint x, ZF_IN zfint y)
    {
        zfindex beforeIndex = _ZFP_scrollViewContentViewLayoutProxy->count();
        do
        {
            QWidget *t = _ZFP_findChild(beforeIndex, x, y);
            if(t != zfnull)
            {
                QWidget *innerChild = _ZFP_findChildRecursive(
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
    virtual void *findTouchedChild(ZF_IN zfint x, ZF_IN zfint y)
    {
        zfindex beforeIndex = _ZFP_scrollViewContentViewLayoutProxy->count();
        do
        {
            QWidget *t = _ZFP_findChild(beforeIndex, x, y);
            if(t != zfnull)
            {
                if(qobject_cast<_ZFP_ZFUIScrollViewImpl_sys_Qt_ScrollView *>(t) != zfnull)
                {
                    return t;
                }
                QWidget *innerChild = _ZFP_findChildRecursive(
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
                                             ZF_IN zfint const &xInParent, ZF_IN zfint const &yInParent)
    {
        QMouseEvent *e = ZFCastStatic(QMouseEvent *, nativeMouseEvent);
        zfint xOffset = 0;
        zfint yOffset = 0;
        _ZFP_ZFUIScrollViewImpl_sys_Qt_ChildOffset(xOffset, yOffset, this, ZFCastStatic(QWidget *, nativeChild));
        QMouseEvent *ret = _ZFP_ZFUIScrollViewImpl_sys_Qt_MouseEventClone(e, e->type(), QPointF(xInParent - xOffset, yInParent - yOffset));
        this->mouseEventCleanup(e);
        return ret;
    }
    virtual void *translateFromChildToParent(ZF_IN void *nativeChild,
                                             ZF_IN_OUT void *nativeMouseEvent,
                                             ZF_IN zfint const &xInChild, ZF_IN zfint const &yInChild)
    {
        QMouseEvent *e = ZFCastStatic(QMouseEvent *, nativeMouseEvent);
        zfint xOffset = 0;
        zfint yOffset = 0;
        _ZFP_ZFUIScrollViewImpl_sys_Qt_ChildOffset(xOffset, yOffset, this, ZFCastStatic(QWidget *, nativeChild));
        QMouseEvent *ret = _ZFP_ZFUIScrollViewImpl_sys_Qt_MouseEventClone(e, e->type(), QPointF(xInChild + xOffset, yInChild + yOffset));
        this->mouseEventCleanup(e);
        return ret;
    }
};

// ============================================================
// impl
ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIScrollViewImpl_sys_Qt, ZFUIScrollView, ZFProtocolLevel::e_SystemHigh)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Qt:QWidget")

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
        nativeScrollView->_ZFP_scrollViewContentView->setGeometry(nativeFrame);
    }
    virtual zftimet scrollAnimationStart(ZF_IN ZFUIScrollView *scrollView,
                                         ZF_IN zftimet recommendTimerInterval)
    {
        _ZFP_ZFUIScrollViewImpl_sys_Qt_ScrollView *nativeScrollView = ZFCastStatic(_ZFP_ZFUIScrollViewImpl_sys_Qt_ScrollView *, scrollView->nativeImplView());
        nativeScrollView->_ZFP_scrollAnimationTimer.connect(
            &(nativeScrollView->_ZFP_scrollAnimationTimer), SIGNAL(timeout()),
            nativeScrollView, SLOT(_ZFP_scrollAnimationTimerOnActivate()));
        nativeScrollView->_ZFP_scrollAnimationTimer.moveToThread(QCoreApplication::instance()->thread());
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
        nativeScrollView->_ZFP_scrollViewContentViewLayoutProxy->childAdd(child, ZFCastStatic(QWidget *, child->nativeView()), atIndex);
    }
    virtual void scrollChildRemove(ZF_IN ZFUIScrollView *parent,
                                   ZF_IN ZFUIView *child,
                                   ZF_IN zfindex atIndex)
    {
        _ZFP_ZFUIScrollViewImpl_sys_Qt_ScrollView *nativeScrollView = ZFCastStatic(_ZFP_ZFUIScrollViewImpl_sys_Qt_ScrollView *, parent->nativeImplView());
        nativeScrollView->_ZFP_scrollViewContentViewLayoutProxy->childRemove(atIndex);
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

