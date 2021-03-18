#include "ZFGlobalEventCenter.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFGlobalEventCenter)
ZFOBJECT_SINGLETON_DEFINE_DETAIL(ZFGlobalEventCenter, ZFGlobalEventCenter, ZFGlobalEventCenter, instance, ZFLevelZFFrameworkEssential)

ZF_NAMESPACE_GLOBAL_END

