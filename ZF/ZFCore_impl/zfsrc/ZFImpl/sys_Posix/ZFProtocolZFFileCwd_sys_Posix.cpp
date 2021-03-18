#include "ZFImpl_sys_Posix_ZFCore_impl.h"
#include "ZFCore/protocol/ZFProtocolZFFileCwd.h"
#include "ZFCore/ZFString.h"

#if ZF_ENV_sys_Posix || ZF_ENV_sys_unknown
#include <unistd.h>
#include <limits.h>

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFFileCwdImpl_sys_Posix, ZFFileCwd, ZFProtocolLevel::e_SystemLow)
public:
    virtual const zfchar *pathForCwd(void)
    {
        static zfchar _pathForCwd[PATH_MAX];
        if(getcwd(_pathForCwd, PATH_MAX) != zfnull)
        {
            return _pathForCwd;
        }
        else
        {
            return zfnull;
        }
    }
    virtual zfbool pathForCwdChange(ZF_IN const zfchar *pathForCwd)
    {
        return (chdir(pathForCwd) == 0);
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFFileCwdImpl_sys_Posix)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFFileCwdImpl_sys_Posix)

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_Posix || ZF_ENV_sys_unknown

