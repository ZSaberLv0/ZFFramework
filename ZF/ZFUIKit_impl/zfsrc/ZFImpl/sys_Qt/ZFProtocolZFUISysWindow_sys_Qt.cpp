#include "ZFImpl_sys_Qt_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUISysWindow.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIView.h"
#include "ZFImpl/sys_Qt/ZFMainEntry_sys_Qt.h"

#if ZF_ENV_sys_Qt

#include <QWidget>
#include <QLayout>
#include <QEvent>
#include <QGuiApplication>
#include <QScreen>

static void _ZFP_ZFUISysWindowImpl_sys_Qt_updateWindowLayout(ZF_IN ZFUISysWindow *sysWindow, ZF_IN QWidget *nativeWindow)
{
    QRect screenRect = QGuiApplication::screens().at(0)->geometry();
    ZFUIRect frame = ZFPROTOCOL_ACCESS(ZFUISysWindow)->notifyMeasureWindow(
            sysWindow,
            ZFUIRectMake(0, 0, screenRect.width(), screenRect.height()),
            ZFUIMarginZero()
        );
    nativeWindow->setGeometry(ZFImpl_sys_Qt_ZFUIKit_impl_ZFUIRectToQRect(frame));
    if(nativeWindow->layout() != zfnull)
    {
        QRect t(0, 0, frame.width, frame.height);
        for(int i = 0; i < nativeWindow->layout()->count(); ++i)
        {
            nativeWindow->layout()->itemAt(i)->widget()->setGeometry(t);
        }
    }
}
class _ZFP_ZFUISysWindowImpl_sys_Qt_EventWrapper : public QObject
{
    Q_OBJECT

protected:
    virtual bool eventFilter(QObject *obj, QEvent *event)
    {
        ZFUISysWindow *owner = ZFImpl_sys_Qt_QObjectZFObjectTag<ZFObjectHolder *>(obj, "_ZFP_ZFUISysWindowImpl_sys_Qt_ownerZFUISysWindow")->objectHolded();
        if(owner == zfnull)
        {
            return QObject::eventFilter(obj, event);
        }
        switch(event->type())
        {
            case QEvent::WindowActivate:
                ZFPROTOCOL_ACCESS(ZFUISysWindow)->notifyOnResume(owner);
                break;
            case QEvent::WindowDeactivate:
                ZFPROTOCOL_ACCESS(ZFUISysWindow)->notifyOnPause(owner);
                #if ZF_ENV_DEBUG && 0
                    zfLogTrimT() << ZFImpl_sys_Qt_ZFUIKit_impl_viewTreePrint(ZFImpl_sys_Qt_rootWindow());
                #endif
                break;
            default:
                break;
        }
        return QObject::eventFilter(obj, event);
    }
};

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUISysWindowImpl_sys_Qt, ZFUISysWindow, ZFProtocolLevel::e_SystemHigh)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Qt:QWidget")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIView, "Qt:QWidget")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()

public:
    zfoverride
    virtual void protocolOnInit(void)
    {
        zfsuper::protocolOnInit();
        this->_mainWindow = zfnull;
    }
    zfoverride
    virtual void protocolOnDeallocPrepare(void)
    {
        this->mainWindowOnCleanup();
        zfsuper::protocolOnDeallocPrepare();
    }
public:
    virtual ZFUISysWindow *mainWindow(void)
    {
        if(this->_mainWindow == zfnull)
        {
            this->_mainWindow = zfRetain(ZFUISysWindow::ClassData()->newInstance().to<ZFUISysWindow *>());
            QWidget *nativeWindow = ZFImpl_sys_Qt_rootWindow();
            ZFImpl_sys_Qt_QObjectZFObjectTag(nativeWindow, "_ZFP_ZFUISysWindowImpl_sys_Qt_ownerZFUISysWindow", this->_mainWindow->objectHolder());
            nativeWindow->installEventFilter(&_eventWrapper);

            this->notifyOnCreate(this->_mainWindow, nativeWindow);

            _ZFP_ZFUISysWindowImpl_sys_Qt_updateWindowLayout(this->_mainWindow, nativeWindow);
            nativeWindow->show();
        }
        return this->_mainWindow;
    }
    virtual void mainWindowOnCleanup(void)
    {
        if(this->_mainWindow != zfnull)
        {
            if(this->_mainWindow->nativeWindowIsResumed())
            {
                this->notifyOnPause(this->_mainWindow);
            }
            zfblockedRelease(this->_mainWindow);
            QWidget *nativeWindow = ZFImpl_sys_Qt_rootWindow();
            nativeWindow->removeEventFilter(&_eventWrapper);
            ZFImpl_sys_Qt_QObjectZFObjectTag(nativeWindow, "_ZFP_ZFUISysWindowImpl_sys_Qt_ownerZFUISysWindow", zfnull);

            this->notifyOnDestroy(this->_mainWindow);
        }
    }
    virtual void mainWindowOnDestroy(void)
    {
        this->_mainWindow = zfnull;
    }

    // ============================================================
public:
    virtual void nativeWindowOnCleanup(ZF_IN ZFUISysWindow *sysWindow)
    {
    }

    virtual void nativeWindowRootViewOnAdd(ZF_IN ZFUISysWindow *sysWindow,
                                           ZF_OUT void *&nativeParentView)
    {
        QWidget *nativeWindow = ZFCastStatic(QWidget *, sysWindow->nativeWindow());

        QWidget *nativeRootView = ZFCastStatic(QWidget *, sysWindow->rootView()->nativeView());
        zfCoreAssertWithMessageTrim(nativeWindow->layout() != NULL,
            "[ZFUISysWindow] window's QWidget::layout not set");
        nativeWindow->layout()->addWidget(nativeRootView);
        nativeParentView = (void *)nativeWindow;
    }
    virtual void nativeWindowRootViewOnRemove(ZF_IN ZFUISysWindow *sysWindow)
    {
        QWidget *nativeWindow = ZFCastStatic(QWidget *, sysWindow->nativeWindow());
        QWidget *nativeRootView = ZFCastStatic(QWidget *, sysWindow->rootView()->nativeView());
        nativeWindow->layout()->removeWidget(nativeRootView);
    }

    virtual zfautoObject modalWindowShow(ZF_IN ZFUISysWindow *sysWindowOwner)
    {
        zfautoObject modalWindow = zfRetain(ZFUISysWindow::ClassData()->newInstance().to<ZFUISysWindow *>());
        ZFImpl_sys_Qt_Window *nativeModalWindow = new ZFImpl_sys_Qt_Window();
        ZFImpl_sys_Qt_QObjectZFObjectTag(nativeModalWindow, "_ZFP_ZFUISysWindowImpl_sys_Qt_ownerZFUISysWindow", modalWindow->objectHolder());
        nativeModalWindow->installEventFilter(&_eventWrapper);
        this->notifyOnCreate(modalWindow, nativeModalWindow);

        _ZFP_ZFUISysWindowImpl_sys_Qt_updateWindowLayout(modalWindow, nativeModalWindow);
        nativeModalWindow->show();

        return modalWindow;
    }
    virtual void modalWindowFinish(ZF_IN ZFUISysWindow *sysWindowOwner,
                                   ZF_IN ZFUISysWindow *sysWindowToFinish)
    {
        QWidget *nativeModalWindow = ZFCastStatic(QWidget *, sysWindowOwner->nativeWindow());
        nativeModalWindow->hide();
        nativeModalWindow->removeEventFilter(&_eventWrapper);
        this->notifyOnDestroy(sysWindowToFinish);
        ZFImpl_sys_Qt_QObjectZFObjectTag(nativeModalWindow, "_ZFP_ZFUISysWindowImpl_sys_Qt_ownerZFUISysWindow", zfnull);
        delete nativeModalWindow;
    }

    virtual void sysWindowLayoutParamOnInit(ZF_IN ZFUISysWindow *sysWindow)
    {
        // centered by default
        sysWindow->sysWindowLayoutParam()->layoutAlign(ZFUIAlign::e_Center);
        sysWindow->sysWindowLayoutParam()->sizeHint(ZFUISizeMake(480, 640));
    }
    virtual void sysWindowLayoutParamOnChange(ZF_IN ZFUISysWindow *sysWindow)
    {
        _ZFP_ZFUISysWindowImpl_sys_Qt_updateWindowLayout(sysWindow, (QWidget *)sysWindow->nativeWindow());
    }

    virtual ZFUIOrientationEnum sysWindowOrientation(ZF_IN ZFUISysWindow *sysWindow)
    {
        // Qt don't support rotate
        return ZFUIOrientation::e_Top;
    }
    virtual void sysWindowOrientationFlags(ZF_IN ZFUISysWindow *sysWindow,
                                           ZF_IN const ZFUIOrientationFlags &flags)
    {
        // Qt don't support rotate
    }

private:
    ZFUISysWindow *_mainWindow;
    _ZFP_ZFUISysWindowImpl_sys_Qt_EventWrapper _eventWrapper;
ZFPROTOCOL_IMPLEMENTATION_END(ZFUISysWindowImpl_sys_Qt)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUISysWindowImpl_sys_Qt)

ZF_NAMESPACE_GLOBAL_END

#include "ZFProtocolZFUISysWindow_sys_Qt.moc"

#endif // #if ZF_ENV_sys_Qt

