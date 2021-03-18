#include "ZFImpl_sys_Android_ZFCore_impl.h"
#include "ZFCore/protocol/ZFProtocolZFFilePath.h"

#if ZF_ENV_sys_Android
ZF_NAMESPACE_GLOBAL_BEGIN
#define ZFImpl_sys_Android_JNI_ID_ZFFilePath ZFImpl_sys_Android_JNI_ID(ZFCore_1impl_ZFFilePath)
#define ZFImpl_sys_Android_JNI_NAME_ZFFilePath ZFImpl_sys_Android_JNI_NAME(ZFCore_impl.ZFFilePath)

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFFilePathImpl_sys_Android, ZFFilePath, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Android:FileSystem")
public:
    zfoverride
    virtual void protocolOnInit(void)
    {
        zfsuper::protocolOnInit();
        JNIEnv *jniEnv = JNIGetJNIEnv();
        jobject tmp = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_ZFFilePath).c_str());
        this->jclsOwner = (jclass)JNIUtilNewGlobalRef(jniEnv, tmp);
        JNIUtilDeleteLocalRef(jniEnv, tmp);
    }
    zfoverride
    virtual void protocolOnDealloc(void)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        JNIUtilDeleteGlobalRef(jniEnv, this->jclsOwner);
        zfsuper::protocolOnDealloc();
    }
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
            JNIEnv *jniEnv = JNIGetJNIEnv();
            static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_pathForModuleFile",
                JNIGetMethodSig(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object), JNIParamTypeContainer()
                ).c_str());
            jobject path = JNIUtilCallStaticObjectMethod(jniEnv, this->jclsOwner, jmId);
            ZFImpl_sys_Android_zfstringFromString(this->_pathForModuleFile, path);
            zfCoreAssert(!this->_pathForModuleFile.isEmpty());
        }
        return this->_pathForModuleFile;
    }

    virtual const zfchar *pathForSetting(void)
    {
        if(this->_pathForSetting.isEmpty())
        {
            JNIEnv *jniEnv = JNIGetJNIEnv();
            static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_pathForSetting",
                JNIGetMethodSig(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object), JNIParamTypeContainer()
                ).c_str());
            jobject path = JNIUtilCallStaticObjectMethod(jniEnv, this->jclsOwner, jmId);
            ZFImpl_sys_Android_zfstringFromString(this->_pathForSetting, path);
            zfCoreAssert(!this->_pathForSetting.isEmpty());
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
            JNIEnv *jniEnv = JNIGetJNIEnv();
            static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_pathForStorage",
                JNIGetMethodSig(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object), JNIParamTypeContainer()
                ).c_str());
            jobject path = JNIUtilCallStaticObjectMethod(jniEnv, this->jclsOwner, jmId);
            ZFImpl_sys_Android_zfstringFromString(this->_pathForStorage, path);
            zfCoreAssert(!this->_pathForStorage.isEmpty());
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
            JNIEnv *jniEnv = JNIGetJNIEnv();
            static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_pathForStorageShared",
                JNIGetMethodSig(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object), JNIParamTypeContainer()
                ).c_str());
            jobject path = JNIUtilCallStaticObjectMethod(jniEnv, this->jclsOwner, jmId);
            ZFImpl_sys_Android_zfstringFromString(this->_pathForStorageShared, path);
            zfCoreAssert(!this->_pathForStorageShared.isEmpty());
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
            JNIEnv *jniEnv = JNIGetJNIEnv();
            static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_pathForCache",
                JNIGetMethodSig(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object), JNIParamTypeContainer()
                ).c_str());
            jobject path = JNIUtilCallStaticObjectMethod(jniEnv, this->jclsOwner, jmId);
            ZFImpl_sys_Android_zfstringFromString(this->_pathForCache, path);
            zfCoreAssert(!this->_pathForCache.isEmpty());
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
    jclass jclsOwner;
private:
    zfstring _pathForModule;
    zfstring _pathForModuleFile;
    zfstring _pathForSetting;
    zfstring _pathForStorage;
    zfstring _pathForStorageShared;
    zfstring _pathForCache;
ZFPROTOCOL_IMPLEMENTATION_END(ZFFilePathImpl_sys_Android)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFFilePathImpl_sys_Android)

ZF_NAMESPACE_GLOBAL_END

#endif

