#include "ZFImpl_sys_Qt_ZFCore_impl.h"
#include "ZFCore/protocol/ZFProtocolZFThread.h"
#include "ZFCore/ZFSTLWrapper/zfstl_map.h"

#if ZF_ENV_sys_Qt && 1
#include <QThread>
#include <QMutex>
#include <QCoreApplication>
#include <QTimer>

class _ZFP_ZFThreadImpl_sys_Qt_MainThreadHolder : public QObject
{
    Q_OBJECT

public:
    _ZFP_ZFThreadImpl_sys_Qt_MainThreadHolder(void)
    : QObject()
    {
        connect(this, SIGNAL(executeInMainThread_signal(void *)), SLOT(executeInMainThread_slot(void *)));
    }

public:
    void executeInMainThread(void *listenerHolder)
    {
        emit executeInMainThread_signal(listenerHolder);
    }

signals:
    void executeInMainThread_signal(void *listenerHolder);

public slots:
    void executeInMainThread_slot(void *listenerHolder)
    {
        ZFListenerHolder *listenerHolderTmp = ZFCastStatic(ZFListenerHolder *, listenerHolder);
        listenerHolderTmp->runnableExecute();
        zfRelease(listenerHolderTmp);
    }
};

class _ZFP_ZFThreadImpl_sys_Qt_NewThreadHolder : public QThread
{
    Q_OBJECT

public:
    _ZFP_ZFThreadImpl_sys_Qt_NewThreadHolder(void)
    : QThread()
    , _ZFP_listenerHolder(zfnull)
    , _ZFP_cancelFlag(zffalse)
    {
    }

public:
    ZFListenerHolder *_ZFP_listenerHolder;
    zfbool _ZFP_cancelFlag;

protected:
    virtual void run()
    {
        if(!this->_ZFP_cancelFlag)
        {
            this->_ZFP_listenerHolder->runnableExecute();
        }
        zfRelease(this->_ZFP_listenerHolder);
        this->deleteLater();
    }
};

class _ZFP_ZFThreadImpl_sys_Qt_MainThreadAfterDelayHolder : public QObject
{
    Q_OBJECT

public:
    QTimer _ZFP_timer;
    ZFListenerHolder *_ZFP_listenerHolder;
    zfbool _ZFP_runFlag;

public:
    _ZFP_ZFThreadImpl_sys_Qt_MainThreadAfterDelayHolder(void)
    : QObject()
    , _ZFP_timer()
    , _ZFP_listenerHolder(zfnull)
    , _ZFP_runFlag(zffalse)
    {
    }
    ~_ZFP_ZFThreadImpl_sys_Qt_MainThreadAfterDelayHolder(void)
    {
        zfRelease(_ZFP_listenerHolder);
    }

public:
    void _ZFP_start(ZF_IN zftimet delay,
                    ZF_IN const ZFListener &runnable,
                    ZF_IN ZFObject *param0,
                    ZF_IN ZFObject *param1)
    {
        _ZFP_listenerHolder = zfAlloc(ZFListenerHolder, runnable,
            ZFListenerData().param0(param0).param1(param1));
        _ZFP_timer.connect(&_ZFP_timer, SIGNAL(timeout()), this, SLOT(_ZFP_run()));
        _ZFP_timer.moveToThread(QCoreApplication::instance()->thread());
        _ZFP_timer.setInterval(delay);
        _ZFP_timer.start();
    }
    void _ZFP_stop(void)
    {
        if(!_ZFP_runFlag)
        {
            _ZFP_runFlag = zftrue;
            _ZFP_timer.stop();
            this->deleteLater();
        }
    }

public slots:
    void _ZFP_run(void)
    {
        if(!_ZFP_runFlag)
        {
            _ZFP_runFlag = zftrue;
            _ZFP_listenerHolder->runnableExecute();
            this->deleteLater();
        }
    }
};

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// global data
typedef QThread * _ZFP_ZFThreadImpl_sys_Qt_NativeThreadIdType;
typedef zfstlmap<_ZFP_ZFThreadImpl_sys_Qt_NativeThreadIdType, ZFThread *> _ZFP_ZFThreadImpl_sys_Qt_ThreadMapType;

static _ZFP_ZFThreadImpl_sys_Qt_NativeThreadIdType _ZFP_ZFThreadImpl_sys_Qt_getNativeThreadId(void)
{
    return QThread::currentThread();
}

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFThreadImpl_sys_Qt_DataHolder, ZFLevelZFFrameworkHigh)
{
    mainThread = zfAlloc(ZFThreadMainThread);
    threadMap[_ZFP_ZFThreadImpl_sys_Qt_getNativeThreadId()] = mainThread;
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFThreadImpl_sys_Qt_DataHolder)
{
    zfRelease(mainThread);
    mainThread = zfnull;
}
public:
    ZFThread *mainThread;
    _ZFP_ZFThreadImpl_sys_Qt_ThreadMapType threadMap;
ZF_GLOBAL_INITIALIZER_END(ZFThreadImpl_sys_Qt_DataHolder)
#define _ZFP_ZFThreadImpl_sys_Qt_mainThreadInstance (ZF_GLOBAL_INITIALIZER_INSTANCE(ZFThreadImpl_sys_Qt_DataHolder)->mainThread)
#define _ZFP_ZFThreadImpl_sys_Qt_threadMap (ZF_GLOBAL_INITIALIZER_INSTANCE(ZFThreadImpl_sys_Qt_DataHolder)->threadMap)

// ============================================================
ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFThreadImpl_sys_Qt, ZFThread, ZFProtocolLevel::e_SystemHigh)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Qt:QThread")
public:
    virtual void *nativeThreadRegister(ZF_IN ZFThread *ownerZFThread)
    {
        _ZFP_ZFThreadImpl_sys_Qt_NativeThreadIdType *token = new _ZFP_ZFThreadImpl_sys_Qt_NativeThreadIdType();
        *token = _ZFP_ZFThreadImpl_sys_Qt_getNativeThreadId();
        zfbool exist = zffalse;
        {
            QMutexLocker _syncObjLocker(&(this->_syncObj));
            exist = (_ZFP_ZFThreadImpl_sys_Qt_threadMap.find(*token) != _ZFP_ZFThreadImpl_sys_Qt_threadMap.end());
        }
        zfCoreAssertWithMessage(!exist, "thread already registered: %s", ownerZFThread->objectInfo().cString());

        {
            QMutexLocker _syncObjLocker(&(this->_syncObj));
            _ZFP_ZFThreadImpl_sys_Qt_threadMap[*token] = ownerZFThread;
        }
        return ZFCastStatic(void *, token);
    }
    virtual void nativeThreadUnregister(ZF_IN void *token)
    {
        {
            QMutexLocker _syncObjLocker(&(this->_syncObj));
            _ZFP_ZFThreadImpl_sys_Qt_threadMap.erase(_ZFP_ZFThreadImpl_sys_Qt_getNativeThreadId());
        }
        delete ZFCastStatic(_ZFP_ZFThreadImpl_sys_Qt_NativeThreadIdType *, token);
    }
    virtual ZFThread *threadForToken(ZF_IN void *token)
    {
        {
            QMutexLocker _syncObjLocker(&(this->_syncObj));
            _ZFP_ZFThreadImpl_sys_Qt_ThreadMapType::iterator it = _ZFP_ZFThreadImpl_sys_Qt_threadMap.find(
                *ZFCastStatic(_ZFP_ZFThreadImpl_sys_Qt_NativeThreadIdType *, token));
            if(it != _ZFP_ZFThreadImpl_sys_Qt_threadMap.end())
            {
                return it->second;
            }
        }
        return zfnull;
    }
    virtual ZFThread *mainThread(void)
    {
        return _ZFP_ZFThreadImpl_sys_Qt_mainThreadInstance;
    }
    virtual ZFThread *currentThread(void)
    {
        {
            QMutexLocker _syncObjLocker(&(this->_syncObj));
            _ZFP_ZFThreadImpl_sys_Qt_ThreadMapType::const_iterator it =
                _ZFP_ZFThreadImpl_sys_Qt_threadMap.find(_ZFP_ZFThreadImpl_sys_Qt_getNativeThreadId());
            if(it == _ZFP_ZFThreadImpl_sys_Qt_threadMap.end())
            {
                return zfnull;
            }
            return it->second;
        }
    }

    virtual void sleep(ZF_IN zftimet miliSecs)
    {
        QThread::msleep(miliSecs);
    }

    virtual void *executeInMainThread(ZF_IN zfidentity taskId,
                                      ZF_IN const ZFListener &runnable,
                                      ZF_IN ZFObject *param0,
                                      ZF_IN ZFObject *param1)
    {
        ZFListenerHolder *listenerHolder = zfAlloc(ZFListenerHolder, runnable,
            ZFListenerData().param0(param0).param1(param1));
        this->_mainThreadHolder.executeInMainThread(listenerHolder);
        return zfnull;
    }
    virtual void executeInMainThreadCancel(ZF_IN zfidentity taskId,
                                           ZF_IN void *nativeToken)
    {
        // nothing to do
    }
    virtual void executeInMainThreadCleanup(ZF_IN zfidentity taskId,
                                            ZF_IN void *nativeToken)
    {
        // nothing to do
    }

    virtual void *executeInNewThread(ZF_IN zfidentity taskId,
                                     ZF_IN ZFThread *ownerZFThread,
                                     ZF_IN const ZFListener &runnable,
                                     ZF_IN ZFObject *param0,
                                     ZF_IN ZFObject *param1)
    {
        _ZFP_ZFThreadImpl_sys_Qt_NewThreadHolder *threadHolder = new _ZFP_ZFThreadImpl_sys_Qt_NewThreadHolder();
        threadHolder->_ZFP_listenerHolder = zfAlloc(ZFListenerHolder, runnable,
            ZFListenerData().param0(param0).param1(param1));
        threadHolder->start();
        return threadHolder;
    }
    virtual void executeInNewThreadCancel(ZF_IN zfidentity taskId,
                                          ZF_IN void *nativeToken)
    {
        _ZFP_ZFThreadImpl_sys_Qt_NewThreadHolder *threadHolder = ZFCastStatic(_ZFP_ZFThreadImpl_sys_Qt_NewThreadHolder *, nativeToken);
        threadHolder->_ZFP_cancelFlag = zftrue;
    }
    virtual void executeInNewThreadCleanup(ZF_IN zfidentity taskId,
                                           ZF_IN void *nativeToken)
    {
        // nothing to do
    }

    virtual void *executeInMainThreadAfterDelay(ZF_IN zfidentity taskId,
                                                ZF_IN zftimet delay,
                                                ZF_IN const ZFListener &runnable,
                                                ZF_IN ZFObject *param0,
                                                ZF_IN ZFObject *param1)
    {
        _ZFP_ZFThreadImpl_sys_Qt_MainThreadAfterDelayHolder *threadHolder = new _ZFP_ZFThreadImpl_sys_Qt_MainThreadAfterDelayHolder();
        threadHolder->_ZFP_start(delay, runnable, param0, param1);
        return threadHolder;
    }
    virtual void executeInMainThreadAfterDelayCancel(ZF_IN zfidentity taskId,
                                                     ZF_IN void *nativeToken)
    {
        _ZFP_ZFThreadImpl_sys_Qt_MainThreadAfterDelayHolder *threadHolder = ZFCastStatic(_ZFP_ZFThreadImpl_sys_Qt_MainThreadAfterDelayHolder *, nativeToken);
        threadHolder->_ZFP_stop();
    }
    virtual void executeInMainThreadAfterDelayCleanup(ZF_IN zfidentity taskId,
                                                      ZF_IN void *nativeToken)
    {
        // nothing to do
    }

private:
    QMutex _syncObj;
    _ZFP_ZFThreadImpl_sys_Qt_MainThreadHolder _mainThreadHolder;
ZFPROTOCOL_IMPLEMENTATION_END(ZFThreadImpl_sys_Qt)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFThreadImpl_sys_Qt)

ZF_NAMESPACE_GLOBAL_END

#include "ZFProtocolZFThread_sys_Qt.moc"
#endif // #if ZF_ENV_sys_Qt

