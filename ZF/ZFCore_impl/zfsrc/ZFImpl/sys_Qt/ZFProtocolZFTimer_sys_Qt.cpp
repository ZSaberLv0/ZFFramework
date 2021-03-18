#include "ZFImpl_sys_Qt_ZFCore_impl.h"
#include "ZFCore/protocol/ZFProtocolZFTimer.h"

#if ZF_ENV_sys_Qt
#include <QTimer>
#include <QThread>
#include <QCoreApplication>

class _ZFP_ZFTimerImpl_sys_Qt_Timer : public QTimer
{
    Q_OBJECT
public:
    _ZFP_ZFTimerImpl_sys_Qt_Timer(void)
    : QTimer()
    , impl(zfnull)
    , ownerZFTimer(zfnull)
    , delaying(zffalse)
    , firstTime(zftrue)
    , timerThread(zfnull)
    {
    }
public:
    ZFPROTOCOL_INTERFACE_CLASS(ZFTimer) *impl;
    ZFTimer *ownerZFTimer;
    zfbool delaying;
    zfbool firstTime;
    QThread *timerThread;

public:
    void timerStart(void)
    {
        if(this->ownerZFTimer->timerDelay() > 0)
        {
            this->delaying = zftrue;
            this->setSingleShot(true);
            this->start(this->ownerZFTimer->timerDelay());
            return ;
        }
        this->delaying = zffalse;
        this->timerDoStart();
    }
    void timerStop(void)
    {
        if(this->timerThread != zfnull)
        {
            this->timerThread->disconnect();
            this->timerThread->connect(this->timerThread, SIGNAL(finished()), this->timerThread, SLOT(deleteLater()));
            this->timerThread->quit();
            this->timerThread = zfnull;
        }
        this->disconnect();
        this->stop();
        this->impl->notifyTimerStop(this->ownerZFTimer);
    }
private:
    void timerDoStart(void)
    {
        this->delaying = zffalse;
        this->setSingleShot(false);
        this->firstTime = zftrue;
        if(this->ownerZFTimer->timerActivateInMainThread())
        {
            connect(this, SIGNAL(timeout()), this, SLOT(timerOnActivate()));
            this->moveToThread(QCoreApplication::instance()->thread());
            this->setInterval(this->ownerZFTimer->timerInterval());
            this->start(this->ownerZFTimer->timerInterval());
        }
        else
        {
            this->timerThread = zfnew(QThread);
            this->timerThread->start();
            this->moveToThread(this->timerThread);
            this->setInterval(this->ownerZFTimer->timerInterval());
            connect(this, SIGNAL(timeout()), this, SLOT(timerOnActivate()), Qt::DirectConnection);
            connect(this->timerThread, SIGNAL(started()), this, SLOT(start()));
        }
    }

public slots:
    void timerOnActivate(void)
    {
        if(this->delaying)
        {
            this->timerDoStart();
            return ;
        }
        if(this->firstTime)
        {
            this->firstTime = zffalse;
            this->impl->notifyTimerStart(this->ownerZFTimer);
        }
        this->impl->notifyTimerActivate(this->ownerZFTimer);
    }
};

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFTimerImpl_sys_Qt, ZFTimer, ZFProtocolLevel::e_SystemHigh)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Qt:QTimer")
public:
    virtual void *nativeTimerCreate(ZF_IN ZFTimer *timer)
    {
        _ZFP_ZFTimerImpl_sys_Qt_Timer *nativeTimer = zfnew(_ZFP_ZFTimerImpl_sys_Qt_Timer);
        nativeTimer->impl = this;
        nativeTimer->ownerZFTimer = timer;
        return nativeTimer;
    }
    virtual void nativeTimerDestroy(ZF_IN ZFTimer *timer,
                                    ZF_IN void *nativeTimer)
    {
        _ZFP_ZFTimerImpl_sys_Qt_Timer *nativeTimerTmp = ZFCastStatic(_ZFP_ZFTimerImpl_sys_Qt_Timer *, nativeTimer);
        zfdelete(nativeTimerTmp);
    }
    virtual void timerStart(ZF_IN ZFTimer *timer)
    {
        _ZFP_ZFTimerImpl_sys_Qt_Timer *nativeTimer = ZFCastStatic(_ZFP_ZFTimerImpl_sys_Qt_Timer *, timer->nativeTimer());
        nativeTimer->timerStart();
    }
    virtual void timerStop(ZF_IN ZFTimer *timer)
    {
        _ZFP_ZFTimerImpl_sys_Qt_Timer *nativeTimer = ZFCastStatic(_ZFP_ZFTimerImpl_sys_Qt_Timer *, timer->nativeTimer());
        nativeTimer->timerStop();
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFTimerImpl_sys_Qt)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFTimerImpl_sys_Qt)

ZF_NAMESPACE_GLOBAL_END

#include "ZFProtocolZFTimer_sys_Qt.moc"
#endif // #if ZF_ENV_sys_Qt

