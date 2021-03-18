#include "ZFOutputForConsole.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFOutputForConsole_ZFOutputDefaultExt, ZFLevelZFFrameworkEssential)
{
    ZFExportVarEnsureInit_ZFOutputForConsole();
    ZFOutputDefaultAdd(ZFOutputForConsole());
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFOutputForConsole_ZFOutputDefaultExt)
{
    ZFOutputDefaultRemove(ZFOutputForConsole());
}
ZF_GLOBAL_INITIALIZER_END(ZFOutputForConsole_ZFOutputDefaultExt)

ZF_NAMESPACE_GLOBAL_END

