#include "ZFImpl_sys_Qt_ZFUIKit_impl.h"

#include "ZFUIKit.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIViewFocus.h"

#if ZF_ENV_sys_Qt

#include <QGraphicsWidget>
#include <QEvent>
#include <QCoreApplication>

class _ZFP_ZFUIViewImpl_sys_Qt_FocusProxyToken : public QObject
{
    Q_OBJECT

public:
    ZFUIView *ownerZFUIView;
    QGraphicsWidget *nativeOwner;
    QGraphicsWidget *nativeImplView;

public:
    void attach(ZF_IN ZFUIView *ownerZFUIView,
                ZF_IN QGraphicsWidget *nativeOwner,
                ZF_IN QGraphicsWidget *nativeImplView)
    {
        this->ownerZFUIView = ownerZFUIView;
        this->nativeOwner = nativeOwner;
        this->nativeImplView = nativeImplView;

        this->nativeOwner->installEventFilter(this);
        if(this->nativeImplView != zfnull)
        {
            this->nativeImplView->installEventFilter(this);
        }
    }
    void detach(void)
    {
        this->nativeOwner->removeEventFilter(this);
        if(this->nativeImplView != zfnull)
        {
            this->nativeImplView->removeEventFilter(this);
        }
    }

public:
    ~_ZFP_ZFUIViewImpl_sys_Qt_FocusProxyToken(void)
    {
        this->detach();
    }

protected:
    bool eventFilter(QObject *obj, QEvent *event)
    {
        static ZFUIView *_lastFocused = zfnull;
        switch(event->type())
        {
            case QEvent::FocusIn:
                if(this->ownerZFUIView != _lastFocused)
                {
                    _lastFocused = this->ownerZFUIView;
                    ZFPROTOCOL_ACCESS(ZFUIViewFocus)->notifyViewFocusChanged(this->ownerZFUIView);
                }
                break;
            case QEvent::FocusOut:
                if(this->ownerZFUIView == _lastFocused)
                {
                    _lastFocused = zfnull;
                    ZFPROTOCOL_ACCESS(ZFUIViewFocus)->notifyViewFocusChanged(this->ownerZFUIView);
                }
                break;
            default:
                break;
        }
        return false;
    }
};

void *_ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_attach(ZF_IN ZFUIView *ownerZFUIView,
                                                 ZF_IN QGraphicsWidget *nativeOwner,
                                                 ZF_IN QGraphicsWidget *nativeImplViewOrNull,
                                                 ZF_IN_OPT void *tokenOld /* = zfnull */)
{
    if(tokenOld == zfnull)
    {
        _ZFP_ZFUIViewImpl_sys_Qt_FocusProxyToken *token = new _ZFP_ZFUIViewImpl_sys_Qt_FocusProxyToken();
        token->attach(ownerZFUIView, nativeOwner, nativeImplViewOrNull);
        return token;
    }
    else
    {
        _ZFP_ZFUIViewImpl_sys_Qt_FocusProxyToken *token = (_ZFP_ZFUIViewImpl_sys_Qt_FocusProxyToken *)tokenOld;
        token->detach();
        token->attach(ownerZFUIView, nativeOwner, nativeImplViewOrNull);
        return token;
    }
}
void _ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_detach(ZF_IN void *token)
{
    _ZFP_ZFUIViewImpl_sys_Qt_FocusProxyToken *tokenTmp = (_ZFP_ZFUIViewImpl_sys_Qt_FocusProxyToken *)token;
    tokenTmp->detach();
}
void _ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_cleanup(ZF_IN void *token)
{
    delete ZFCastStatic(_ZFP_ZFUIViewImpl_sys_Qt_FocusProxyToken *, token);
}
void _ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_viewFocusable(ZF_IN void *token, ZF_IN zfbool v)
{
    _ZFP_ZFUIViewImpl_sys_Qt_FocusProxyToken *t = ZFCastStatic(_ZFP_ZFUIViewImpl_sys_Qt_FocusProxyToken *, token);
    if(v)
    {
        t->nativeOwner->setFocusPolicy(Qt::ClickFocus);
        if(t->nativeImplView != zfnull)
        {
            t->nativeImplView->setFocusPolicy(Qt::ClickFocus);
            t->nativeOwner->setFocusProxy(t->nativeImplView);
        }
    }
    else
    {
        t->nativeOwner->setFocusPolicy(Qt::NoFocus);
        t->nativeOwner->setFocusProxy(NULL);
        if(t->nativeImplView != zfnull)
        {
            t->nativeImplView->setFocusPolicy(Qt::NoFocus);
        }
    }
}
void _ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_viewFocusRequest(ZF_IN void *token, ZF_IN zfbool v)
{
    _ZFP_ZFUIViewImpl_sys_Qt_FocusProxyToken *t = ZFCastStatic(_ZFP_ZFUIViewImpl_sys_Qt_FocusProxyToken *, token);
    if(v)
    {
        t->nativeOwner->setFocus();
    }
    else
    {
        t->nativeOwner->clearFocus();
    }
}
zfbool _ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_viewFocused(ZF_IN void *token)
{
    _ZFP_ZFUIViewImpl_sys_Qt_FocusProxyToken *t = ZFCastStatic(_ZFP_ZFUIViewImpl_sys_Qt_FocusProxyToken *, token);
    return t->nativeOwner->hasFocus();
}

#include "ZFProtocolZFUIView_sys_Qt_FocusProxy.moc"

#endif // #if ZF_ENV_sys_Qt

