#include "ZFSemaphore.h"
#include "protocol/ZFProtocolZFSemaphore.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassNotPOD _ZFP_ZFSemaphorePrivate
{
public:
    void *nativeSemaphore;
    ZFPROTOCOL_INTERFACE_CLASS(ZFSemaphore) *impl;
};

// ============================================================
// ZFSemaphore
ZFOBJECT_REGISTER(ZFSemaphore)
void ZFSemaphore::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFSemaphorePrivate);
    d->impl = ZFPROTOCOL_ACCESS(ZFSemaphore);
    d->nativeSemaphore = d->impl->nativeSemaphoreCreate(this);
}
void ZFSemaphore::objectOnDealloc(void)
{
    d->impl->nativeSemaphoreDestroy(this, d->nativeSemaphore);
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}

void *ZFSemaphore::nativeSemaphore(void)
{
    return d->nativeSemaphore;
}

void ZFSemaphore::semaphoreLock(void)
{
    zfRetain(this);
    d->impl->semaphoreLock(this);
}
void ZFSemaphore::semaphoreUnlock(void)
{
    d->impl->semaphoreUnlock(this);
    zfRelease(this);
}

void ZFSemaphore::semaphoreSignal(void)
{
    d->impl->semaphoreSignal(this);
}
void ZFSemaphore::semaphoreSignalLocked(void)
{
    this->semaphoreLock();
    this->semaphoreSignal();
    this->semaphoreUnlock();
}

void ZFSemaphore::semaphoreBroadcast(void)
{
    d->impl->semaphoreBroadcast(this);
}
void ZFSemaphore::semaphoreBroadcastLocked(void)
{
    this->semaphoreLock();
    this->semaphoreBroadcast();
    this->semaphoreUnlock();
}

void ZFSemaphore::semaphoreWait(void)
{
    d->impl->semaphoreWait(this);
}
void ZFSemaphore::semaphoreWaitLocked(void)
{
    this->semaphoreLock();
    this->semaphoreWait();
    this->semaphoreUnlock();
}

zfbool ZFSemaphore::semaphoreWait(ZF_IN zftimet miliSecs)
{
    return d->impl->semaphoreWait(this, miliSecs);
}
zfbool ZFSemaphore::semaphoreWaitLocked(ZF_IN zftimet miliSecs)
{
    this->semaphoreLock();
    zfbool ret = this->semaphoreWait(miliSecs);
    this->semaphoreUnlock();
    return ret;
}

ZF_NAMESPACE_GLOBAL_END

