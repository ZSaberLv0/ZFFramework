#include "ZFImpl_default_ZFCore_impl.h"
#include "ZFCore/protocol/ZFProtocolZFFilePath.h"
#include "ZFCore/ZFString.h"

#if ZF_ENV_sys_Windows
    #include <Windows.h>
#elif ZF_ENV_sys_Posix || ZF_ENV_sys_unknown // #if ZF_ENV_sys_Windows
    #include <unistd.h>
#endif // #elif ZF_ENV_sys_Posix || ZF_ENV_sys_unknown

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFFilePathImpl_default, ZFFilePath, ZFProtocolLevel::e_Default)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("ModuleLocalPath")
public:
    virtual const zfchar *pathForModule(void)
    {
        if(this->_pathForModule.isEmpty())
        {
            (void)this->pathForModuleFile();
            zfindex pos = zfstringFindReversely(this->_pathForModuleFile, ZFFileSeparator());
            zfCoreAssert(pos != zfindexMax());
            this->_pathForModule.assign(this->_pathForModuleFile, pos);
        }
        return this->_pathForModule;
    }
    virtual const zfchar *pathForModuleFile(void)
    {
        if(this->_pathForModuleFile.isEmpty())
        {
            #if ZF_ENV_sys_Windows
                zfstring tmp;
                zfcharW buf[1024] = {0};
                GetModuleFileNameW(zfnull, buf, 1024);
                zfstringToUTF8(tmp, buf, ZFStringEncoding::e_UTF16);
            #elif ZF_ENV_sys_Posix || ZF_ENV_sys_unknown // #if ZF_ENV_sys_Windows
                zfchar tmp[1024] = {0};
                zfint len = (zfint)readlink("/proc/self/exe", tmp, 1024);
                if(len > 0)
                {
                    tmp[len] = '\0';
                }
                else
                {
                    zfscpy(tmp, zfstringWithFormat("./unknown").cString());
                }
            #endif // #elif ZF_ENV_sys_Posix || ZF_ENV_sys_unknown
            zfbool result = ZFFilePathFormat(this->_pathForModuleFile, tmp);
            zfCoreAssert(result);
        }
        return this->_pathForModuleFile;
    }

    virtual const zfchar *pathForSetting(void)
    {
        if(this->_pathForSetting.isEmpty())
        {
            this->_pathForSetting = this->pathForModule();
            this->_pathForSetting += ZFFileSeparator();
            this->_pathForSetting += "zfsetting";
        }
        return this->_pathForSetting;
    }
    virtual void pathForSetting(ZF_IN const zfchar *path = zfnull)
    {
        this->_pathForSetting = path;
    }

    virtual const zfchar *pathForStorage(void)
    {
        if(this->_pathForStorage.isEmpty())
        {
            this->_pathForStorage = this->pathForModule();
            this->_pathForStorage += ZFFileSeparator();
            this->_pathForStorage += "zfstorage";
        }
        return this->_pathForStorage;
    }
    virtual void pathForStorage(ZF_IN const zfchar *path = zfnull)
    {
        this->_pathForStorage = path;
    }

    virtual const zfchar *pathForStorageShared(void)
    {
        if(this->_pathForStorageShared.isEmpty())
        {
            this->_pathForStorageShared = this->pathForModule();
            this->_pathForStorageShared += ZFFileSeparator();
            this->_pathForStorageShared += "zfstorageshared";
        }
        return this->_pathForStorageShared;
    }
    virtual void pathForStorageShared(ZF_IN const zfchar *path = zfnull)
    {
        this->_pathForStorageShared = path;
    }

    virtual const zfchar *pathForCache(void)
    {
        if(this->_pathForCache.isEmpty())
        {
            this->_pathForCache = this->pathForModule();
            this->_pathForCache += ZFFileSeparator();
            this->_pathForCache += "zfcache";
        }
        return this->_pathForCache;
    }
    virtual void pathForCache(ZF_IN const zfchar *path = zfnull)
    {
        this->_pathForCache = path;
    }
    virtual void pathForCacheClear(void)
    {
        ZFFileFileRemove(this->_pathForCache, zfHint("isRecursive")zftrue, zfHint("isForce")zftrue);
    }

private:
    zfstring _pathForModule;
    zfstring _pathForModuleFile;
    zfstring _pathForSetting;
    zfstring _pathForStorage;
    zfstring _pathForStorageShared;
    zfstring _pathForCache;
ZFPROTOCOL_IMPLEMENTATION_END(ZFFilePathImpl_default)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFFilePathImpl_default)

ZF_NAMESPACE_GLOBAL_END

