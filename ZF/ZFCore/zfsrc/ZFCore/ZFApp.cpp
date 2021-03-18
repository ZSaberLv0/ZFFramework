#include "ZFApp.h"

ZF_NAMESPACE_GLOBAL_BEGIN
ZF_NAMESPACE_BEGIN(ZFApp)

ZFMETHOD_FUNC_DEFINE_0(void, appExit)
{
    ZFFrameworkCleanup();
    exit(0);
}

ZF_NAMESPACE_END_WITH_REGISTER(ZFApp, ZF_NAMESPACE_GLOBAL)
ZF_NAMESPACE_GLOBAL_END

