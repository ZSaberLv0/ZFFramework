#include "ZFCoreMutex.h"

ZF_NAMESPACE_GLOBAL_BEGIN

void *_ZFP_ZFCoreMutexImplObject = zfnull;
ZFCoreMutexImplCallbackLock _ZFP_ZFCoreMutexImplLock = zfnull;
ZFCoreMutexImplCallbackUnlock _ZFP_ZFCoreMutexImplUnlock = zfnull;

ZF_NAMESPACE_GLOBAL_END

