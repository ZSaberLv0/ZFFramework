#include "ZFImpl_sys_Qt_ZFCore_impl.h"
#include "ZFCore/protocol/ZFProtocolZFFilePath.h"
#include "ZFCore/ZFString.h"

#if ZF_ENV_sys_Qt
#include <QCoreApplication>
#include <QStandardPaths>

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFFilePathImpl_sys_Qt, ZFFilePath, ZFProtocolLevel::e_SystemHigh)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Qt::applicationDirPath")
public:
    virtual const zfchar *pathForModule(void)
    {
        if(this->_pathForModule.isEmpty())
        {
            this->_pathForModule = QCoreApplication::applicationDirPath().toStdString().c_str();
            if(this->_pathForModule.isEmpty())
            {
                this->_pathForModule = "./";
            }
        }
        return this->_pathForModule;
    }
    virtual const zfchar *pathForModuleFile(void)
    {
        if(this->_pathForModuleFile.isEmpty())
        {
            this->_pathForModuleFile = QCoreApplication::applicationFilePath().toStdString().c_str();
            if(this->_pathForModuleFile.isEmpty())
            {
                this->_pathForModuleFile = "./unknown";
            }
        }
        return this->_pathForModuleFile;
    }

    virtual const zfchar *pathForSetting(void)
    {
        if(this->_pathForSetting.isEmpty())
        {
            ZFFilePathFormat(this->_pathForSetting, QStandardPaths::writableLocation(QStandardPaths::ConfigLocation).toStdString().c_str());
            if(this->_pathForSetting.isEmpty())
            {
                this->_pathForSetting = this->pathForModule();
            }
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
            ZFFilePathFormat(this->_pathForStorage, QStandardPaths::writableLocation(QStandardPaths::AppDataLocation).toStdString().c_str());
            if(this->_pathForStorage.isEmpty())
            {
                this->_pathForStorage = this->pathForModule();
            }
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
            ZFFilePathFormat(this->_pathForStorageShared, QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation).toStdString().c_str());
            if(this->_pathForStorageShared.isEmpty())
            {
                this->_pathForStorageShared = this->pathForModule();
                this->_pathForStorageShared += ZFFileSeparator();
                this->_pathForStorageShared += "zfstorage";
            }
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
            ZFFilePathFormat(this->_pathForCache, QStandardPaths::writableLocation(QStandardPaths::CacheLocation).toStdString().c_str());
            if(this->_pathForCache.isEmpty())
            {
                this->_pathForCache = this->pathForModule();
            }
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
ZFPROTOCOL_IMPLEMENTATION_END(ZFFilePathImpl_sys_Qt)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFFilePathImpl_sys_Qt)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_Qt

