#include "ZFObjectMutex.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFObjectMutexImplCallbackInit _ZFP_ZFObjectMutexImplInit = zfnull;
ZFObjectMutexImplCallbackDealloc _ZFP_ZFObjectMutexImplDealloc = zfnull;
ZFObjectMutexImplCallbackLock _ZFP_ZFObjectMutexImplLock = zfnull;
ZFObjectMutexImplCallbackUnlock _ZFP_ZFObjectMutexImplUnlock = zfnull;
ZFObjectMutexImplCallbackTryLock _ZFP_ZFObjectMutexImplTryLock = zfnull;

// ============================================================
void ZFObjectMutexImplSet(ZF_IN_OPT ZFObjectMutexImplCallbackInit implInit /* = zfnull */,
                          ZF_IN_OPT ZFObjectMutexImplCallbackDealloc implDealloc /* = zfnull */,
                          ZF_IN_OPT ZFObjectMutexImplCallbackLock implLock /* = zfnull */,
                          ZF_IN_OPT ZFObjectMutexImplCallbackUnlock implUnlock /* = zfnull */,
                          ZF_IN_OPT ZFObjectMutexImplCallbackTryLock implTryLock /* = zfnull */)
{
    if(implInit == zfnull && _ZFP_ZFObjectMutexImplInit != zfnull)
    {
        _ZFP_ZFObjectMutexImplDealloc(ZFCoreMutexImplGetObject());
        ZFCoreMutexImplSet(zfnull, zfnull, zfnull);
     }

    _ZFP_ZFObjectMutexImplInit = implInit;
    _ZFP_ZFObjectMutexImplDealloc = implDealloc;
    _ZFP_ZFObjectMutexImplLock = implLock;
    _ZFP_ZFObjectMutexImplUnlock = implUnlock;
    _ZFP_ZFObjectMutexImplTryLock = implTryLock;
}

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(_ZFP_ZFObjectMutexImplSetup, ZFLevelZFFrameworkLow)
{
    if(_ZFP_ZFObjectMutexImplInit != zfnull)
    {
        void *implObject = _ZFP_ZFObjectMutexImplInit();
        ZFCoreMutexImplSet(implObject, _ZFP_ZFObjectMutexImplLock, _ZFP_ZFObjectMutexImplUnlock);
    }
}
ZF_GLOBAL_INITIALIZER_END(_ZFP_ZFObjectMutexImplSetup)

ZF_NAMESPACE_GLOBAL_END

