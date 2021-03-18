#include "ZFMutex.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFMutex
ZFOBJECT_REGISTER(ZFMutex)

ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFMutex, void, mutexLock)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFMutex, zfbool, mutexTryLock)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFMutex, void, mutexUnlock)

ZF_NAMESPACE_GLOBAL_END

