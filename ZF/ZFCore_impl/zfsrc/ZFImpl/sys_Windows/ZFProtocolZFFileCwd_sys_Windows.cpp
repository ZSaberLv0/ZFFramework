#include "ZFImpl_sys_Windows_ZFCore_impl.h"
#include "ZFCore/protocol/ZFProtocolZFFileCwd.h"
#include "ZFCore/ZFString.h"

#if ZF_ENV_sys_Windows
#include <Windows.h>

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFFileCwdImpl_sys_Windows, ZFFileCwd, ZFProtocolLevel::e_SystemLow)
public:
    virtual const zfchar *pathForCwd(void)
    {
        static zfstring _pathForCwd;
        wchar_t *buf = _wgetcwd(NULL, 0);
        if(buf)
        {
            zfstringToUTF8(_pathForCwd, buf, ZFStringEncoding::e_UTF16);
            free(buf);
            return _pathForCwd;
        }
        else
        {
            return zfnull;
        }
    }
    virtual zfbool pathForCwdChange(ZF_IN const zfchar *pathForCwd)
    {
        return (_wchdir(zfstringToUTF16(pathForCwd, ZFStringEncoding::e_UTF8).cString()) == 0);
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFFileCwdImpl_sys_Windows)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFFileCwdImpl_sys_Windows)

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_Windows

