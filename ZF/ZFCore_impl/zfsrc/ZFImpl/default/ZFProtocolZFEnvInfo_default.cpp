#include "ZFImpl_default_ZFCore_impl.h"
#include "ZFCore/protocol/ZFProtocolZFEnvInfo.h"

#if ZF_ENV_sys_Qt
    #include <QSysInfo>
    #include <QString>
#endif

#if ZF_ENV_sys_Windows
    #include <Windows.h>
#endif

#if ZF_ENV_sys_Posix
    #include <sys/utsname.h>
#endif

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFEnvInfo_systemInfoImpl_default, ZFEnvInfo_systemInfo, ZFProtocolLevel::e_Default)
public:
    virtual void systemName(ZF_IN_OUT zfstring &ret)
    {
        #if ZF_ENV_sys_Qt
        {
            ret += QSysInfo::productType().toStdString().c_str();
            return ;
        }
        #endif

        #if ZF_ENV_sys_Windows
        {
            ret += "Windows";
            return ;
        }
        #endif

        #if ZF_ENV_sys_Posix
        {
            struct utsname buf;
            zfmemset(&buf, 0, sizeof(struct utsname));
            uname(&buf);
            ret += buf.sysname;
            return ;
        }
        #endif
    }
    virtual void systemVersion(ZF_IN_OUT zfstring &ret)
    {
        #if ZF_ENV_sys_Qt
        {
            ret += QSysInfo::productVersion().toStdString().c_str();
            return ;
        }
        #endif
        #if ZF_ENV_sys_Windows
        {
            OSVERSIONINFO osvi;
            zfmemset(&osvi, 0, sizeof(OSVERSIONINFO));
            osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
            GetVersionEx(&osvi);

            zfstring tmp;
            zfVersionSetInt(tmp, ZFVERSION_MAIN(), osvi.dwMajorVersion);
            zfVersionSetInt(tmp, ZFVERSION_MAIN(), osvi.dwMinorVersion);
            zfVersionSetInt(tmp, ZFVERSION_MAIN(), osvi.dwBuildNumber);
            ret += tmp;
            return ;
        }
        #endif

        #if ZF_ENV_sys_Posix
        {
            struct utsname buf;
            zfmemset(&buf, 0, sizeof(struct utsname));
            uname(&buf);
            ret += buf.version;
            return ;
        }
        #endif
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFEnvInfo_systemInfoImpl_default)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFEnvInfo_systemInfoImpl_default)

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFEnvInfo_frameworkInfoImpl_default, ZFEnvInfo_frameworkInfo, ZFProtocolLevel::e_Default)
public:
    virtual void frameworkName(ZF_IN_OUT zfstring &ret)
    {
        #if ZF_ENV_sys_Qt
        {
            ret += "Qt";
            return ;
        }
        #endif
    }
    virtual void frameworkVersion(ZF_IN_OUT zfstring &ret)
    {
        #if ZF_ENV_sys_Qt
        {
            ret += QT_VERSION_STR;
            return ;
        }
        #endif
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFEnvInfo_frameworkInfoImpl_default)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFEnvInfo_frameworkInfoImpl_default)

ZF_NAMESPACE_GLOBAL_END

