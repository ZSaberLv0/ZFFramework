#include "ZFImpl_sys_iOS_ZFCore_impl.h"
#include "ZFCore/protocol/ZFProtocolZFFilePath.h"

#if ZF_ENV_sys_iOS
#import <Foundation/Foundation.h>

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFFilePathImpl_sys_iOS, ZFFilePath, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("iOS:SandboxPath")
public:
    virtual const zfchar *pathForModule(void)
    {
        if(this->_pathForModule.isEmpty())
        {
            (void)this->pathForModuleFile();
            zfindex pos = zfstringFindReversely(this->_pathForModuleFile, ZFFileSeparator());
            if(pos != zfindexMax())
            {
                this->_pathForModule.assign(this->_pathForModuleFile, pos);
            }
        }
        return this->_pathForModule;
    }
    virtual const zfchar *pathForModuleFile(void)
    {
        if(this->_pathForModuleFile.isEmpty())
        {
            zfstring tmp;
            ZFImpl_sys_iOS_zfstringFromNSString(tmp, [[NSBundle mainBundle] bundlePath]);
            ZFFilePathFormat(this->_pathForModuleFile, tmp.cString());
        }
        return this->_pathForModuleFile;
    }

    virtual const zfchar *pathForSetting(void)
    {
        if(this->_pathForSetting.isEmpty())
        {
            this->_pathForSetting = this->nativeDocumentPath();
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
            this->_pathForStorage = this->nativeDocumentPath();
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
            this->_pathForStorageShared = this->nativeDocumentPath();
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
            this->_pathForCache = this->nativeCachePath();
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
    zfstring _nativeDocumentPath;
    const zfchar *nativeDocumentPath(void)
    {
        if(_nativeDocumentPath.isEmpty())
        {
            NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
            zfstring tmp;
            ZFImpl_sys_iOS_zfstringFromNSString(tmp, (NSString *)[paths objectAtIndex:0]);
            ZFFilePathFormat(_nativeDocumentPath, tmp.cString());
        }
        return _nativeDocumentPath;
    }
    zfstring _nativeCachePath;
    const zfchar *nativeCachePath(void)
    {
        if(_nativeCachePath.isEmpty())
        {
            NSArray *paths = NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);
            zfstring tmp;
            ZFImpl_sys_iOS_zfstringFromNSString(tmp, (NSString *)[paths objectAtIndex:0]);
            ZFFilePathFormat(_nativeCachePath, tmp.cString());
        }
        return _nativeCachePath;
    }

private:
    zfstring _pathForModule;
    zfstring _pathForModuleFile;
    zfstring _pathForSetting;
    zfstring _pathForStorage;
    zfstring _pathForStorageShared;
    zfstring _pathForCache;
ZFPROTOCOL_IMPLEMENTATION_END(ZFFilePathImpl_sys_iOS)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFFilePathImpl_sys_iOS)

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_iOS

