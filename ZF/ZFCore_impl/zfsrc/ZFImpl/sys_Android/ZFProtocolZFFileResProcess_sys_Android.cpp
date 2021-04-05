#include "ZFImpl_sys_Android_ZFCore_impl.h"
#include "ZFCore/protocol/ZFProtocolZFFileResProcess.h"
#include "ZFCore/ZFString.h"
#include "ZFImpl/sys_Android/ZFMainEntry_sys_Android.h"

#if ZF_ENV_sys_Android
// need Android 2.3 or above for asset RW for JNI
// need -landroid
// all resource files are located under "project_dir/assets/zfres/"
//
// (ZFTAG_LIMITATION) due to the limitation of Android asset (non-media file type must not exceeds 1M bytes),
// we add ".mp2" as dummy extension,
// which is a media file type's extension
// all res files with this extension would have its file extension to be removed
// (e.g. "file.ext.mp2" would have "file.ext" as the final file name,
// if no such extension found, the original file name would be used)
// so, if you have res file that exceed 1M bytes,
// you may simply append ".mp2" as file extension
// and access as normal
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

ZF_NAMESPACE_GLOBAL_BEGIN
#define ZFImpl_sys_Android_JNI_ID_ZFFileResProcess ZFImpl_sys_Android_JNI_ID(ZFCore_1impl_ZFFileResProcess)
#define ZFImpl_sys_Android_JNI_NAME_ZFFileResProcess ZFImpl_sys_Android_JNI_NAME(ZFCore_impl.ZFFileResProcess)

zfclassLikePOD _ZFP_ZFProtocolZFFileResProcess_sys_Android_FileToken
{
public:
    JNIObjectHolder assetManagerHolder;
    AAssetManager *assetManager;
    AAsset *token;
    zfbool isEof;
    zfbool isError;
public:
    _ZFP_ZFProtocolZFFileResProcess_sys_Android_FileToken(void)
    : assetManagerHolder()
    , assetManager(NULL)
    , token(NULL)
    , isEof(zffalse)
    , isError(zffalse)
    {
    }
};

zfclassNotPOD _ZFP_ZFProtocolZFFileResProcess_sys_Android_FindData
{
public:
    zfstring parentPath;
    jobjectArray files; // jstring[]
    zfindex curFileIndex;
public:
    _ZFP_ZFProtocolZFFileResProcess_sys_Android_FindData(void)
    : parentPath()
    , files(zfnull)
    , curFileIndex(0)
    {
    }
    virtual ~_ZFP_ZFProtocolZFFileResProcess_sys_Android_FindData(void)
    {
    }
};

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFFileResProcessImpl_sys_Android, ZFFileResProcess, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Android:AssetsFile")
private:
    const zfchar *zfresPrefix;
    zfindex zfresPrefixLen;
    const zfchar *zfresPostfix;
    zfindex zfresPostfixLen;

public:
    zfoverride
    virtual void protocolOnInit(void)
    {
        zfsuper::protocolOnInit();

        this->zfresPrefix = "zfres";
        this->zfresPrefixLen = zfslen(this->zfresPrefix);
        this->zfresPostfix = ".mp2";
        this->zfresPostfixLen = zfslen(this->zfresPostfix);

        JNIEnv *jniEnv = JNIGetJNIEnv();
        jobject tmp = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_ZFFileResProcess).c_str());
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
    virtual zfbool resIsExist(ZF_IN const zfchar *resPath)
    {
        zfstring resPathFixed;
        this->resPathFormat(resPathFixed, resPath);

        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_resIsExist",
            JNIGetMethodSig(JNIType::S_boolean, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_String))
            ).c_str());
        jstring param = JNIUtilNewStringUTF(jniEnv, resPathFixed.cString());
        JNIBlockedDeleteLocalRef(param);
        return (zfbool)JNIUtilCallStaticBooleanMethod(jniEnv, this->jclsOwner, jmId, param);
    }
    virtual zfbool resIsDir(ZF_IN const zfchar *resPath)
    {
        zfstring resPathFixed;
        this->resPathFormat(resPathFixed, resPath);

        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_resIsDir",
            JNIGetMethodSig(JNIType::S_boolean, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_String))
            ).c_str());
        jstring param = JNIUtilNewStringUTF(jniEnv, resPathFixed.cString());
        JNIBlockedDeleteLocalRef(param);
        return (zfbool)JNIUtilCallStaticBooleanMethod(jniEnv, this->jclsOwner, jmId, param);
    }

    virtual zfbool resCopy(ZF_IN const zfchar *resPath,
                           ZF_IN const zfchar *dstPath,
                           ZF_IN_OPT zfbool isRecursive = zftrue,
                           ZF_IN_OPT zfbool isForce = zffalse,
                           ZF_IN_OPT zfstring *errPos = zfnull)
    {
        zfstring resPathFixed;
        this->resPathFormat(resPathFixed, resPath);

        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_resCopy",
            JNIGetMethodSig(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_String), JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_String))
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_String))
                .add(JNIType::S_boolean)
                .add(JNIType::S_boolean)
            ).c_str());
        jobject errPosJ = JNIUtilCallStaticObjectMethod(jniEnv, this->jclsOwner, jmId,
            JNILineDeleteLocalRefWithEnv(ZFImpl_sys_Android_zfstringToString(resPathFixed), jniEnv),
            JNILineDeleteLocalRefWithEnv(ZFImpl_sys_Android_zfstringToString(dstPath), jniEnv),
            isRecursive,
            isForce);
        if(errPosJ == NULL)
        {
            return zftrue;
        }
        else
        {
            if(errPos != zfnull)
            {
                ZFImpl_sys_Android_zfstringFromString(*errPos, errPosJ);
            }
            return zffalse;
        }
    }

    // ============================================================
    // res RW
    virtual void *resOpen(ZF_IN const zfchar *resPath)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        _ZFP_ZFProtocolZFFileResProcess_sys_Android_FileToken *d = zfnew(_ZFP_ZFProtocolZFFileResProcess_sys_Android_FileToken);
        d->assetManagerHolder = ZFImpl_sys_Android_assetManager();
        d->assetManager = AAssetManager_fromJava(jniEnv, d->assetManagerHolder);
        zfCoreAssert(d->assetManager != zfnull);

        zfstring absPath;
        this->resPathFormat(absPath, resPath);

        d->token = AAssetManager_open(d->assetManager,
            absPath.cString(),
            AASSET_MODE_RANDOM);
        if(d->token == zfnull)
        {
            absPath += this->zfresPostfix;
            d->token = AAssetManager_open(d->assetManager,
                absPath.cString(),
                AASSET_MODE_RANDOM);
            if(d->token == zfnull)
            {
                zfdelete(d);
                d = zfnull;
            }
        }
        return d;
    }
    virtual zfbool resClose(ZF_IN void *token)
    {
        if(token == zfnull)
        {
            return zffalse;
        }

        _ZFP_ZFProtocolZFFileResProcess_sys_Android_FileToken *d = ZFCastStatic(_ZFP_ZFProtocolZFFileResProcess_sys_Android_FileToken *, token);
        AAsset_close(d->token);
        zfdelete(d);
        return zftrue;
    }
    virtual zfindex resTell(ZF_IN void *token)
    {
        if(token == zfnull)
        {
            return zfindexMax();
        }
        _ZFP_ZFProtocolZFFileResProcess_sys_Android_FileToken *d = ZFCastStatic(_ZFP_ZFProtocolZFFileResProcess_sys_Android_FileToken *, token);
        return (AAsset_getLength(d->token) - AAsset_getRemainingLength(d->token));
    }
    virtual zfbool resSeek(ZF_IN void *token,
                           ZF_IN zfindex byteSize,
                           ZF_IN_OPT ZFSeekPos position = ZFSeekPosBegin)
    {
        if(token == zfnull)
        {
            return zffalse;
        }
        _ZFP_ZFProtocolZFFileResProcess_sys_Android_FileToken *d = ZFCastStatic(_ZFP_ZFProtocolZFFileResProcess_sys_Android_FileToken *, token);
        zfint seekPos = SEEK_SET;
        long seekSize = (long)byteSize;
        switch(position)
        {
            case ZFSeekPosBegin:
                seekPos = SEEK_SET;
                break;
            case ZFSeekPosCur:
                seekPos = SEEK_CUR;
                break;
            case ZFSeekPosCurReversely:
                seekPos = SEEK_CUR;
                seekSize = -seekSize;
                break;
            case ZFSeekPosEnd:
                seekPos = SEEK_END;
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                break;
        }
        return (AAsset_seek(d->token, seekSize, seekPos) != -1);
    }
    virtual zfindex resRead(ZF_IN void *token,
                            ZF_IN void *buf,
                            ZF_IN zfindex maxByteSize)
    {
        if(token == zfnull || maxByteSize == 0)
        {
            return 0;
        }
        _ZFP_ZFProtocolZFFileResProcess_sys_Android_FileToken *d = ZFCastStatic(_ZFP_ZFProtocolZFFileResProcess_sys_Android_FileToken *, token);
        if(d->isEof || d->isError)
        {
            return 0;
        }
        zfint ret = AAsset_read(d->token, buf, maxByteSize);
        if(ret < 0)
        {
            d->isError = zftrue;
            ret = 0;
        }
        else if(ret == 0 || ret < maxByteSize)
        {
            d->isEof = zftrue;
        }
        return ret;
    }
    virtual zfbool resIsEof(ZF_IN void *token)
    {
        if(token == zfnull)
        {
            return zffalse;
        }
        _ZFP_ZFProtocolZFFileResProcess_sys_Android_FileToken *d = ZFCastStatic(_ZFP_ZFProtocolZFFileResProcess_sys_Android_FileToken *, token);
        return d->isEof;
    }
    virtual zfbool resIsError(ZF_IN void *token)
    {
        if(token == zfnull)
        {
            return zffalse;
        }
        _ZFP_ZFProtocolZFFileResProcess_sys_Android_FileToken *d = ZFCastStatic(_ZFP_ZFProtocolZFFileResProcess_sys_Android_FileToken *, token);
        return d->isError;
    }

    // ============================================================
    // res find
    virtual zfbool resFindFirst(ZF_IN_OUT ZFFileFindData::Impl &fd,
                                ZF_IN const zfchar *resPath)
    {
        zfstring absPath;
        this->resPathFormat(absPath, resPath);

        _ZFP_ZFProtocolZFFileResProcess_sys_Android_FindData *d = zfnew(_ZFP_ZFProtocolZFFileResProcess_sys_Android_FindData);
        d->parentPath = absPath.cString() + this->zfresPrefixLen
            + ((absPath.length() > this->zfresPrefixLen) ? 1 : 0);
        fd.nativeFd = d;
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_listAssets",
            JNIGetMethodSig(JNIType::S_array(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_String)), JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_String))
            ).c_str());
        jstring param = JNIUtilNewStringUTF(jniEnv, absPath.cString());
        JNIBlockedDeleteLocalRef(param);
        jobjectArray files = (jobjectArray)JNIUtilCallStaticObjectMethod(jniEnv, this->jclsOwner, jmId, param);
        JNIBlockedDeleteLocalRef(files);
        zfbool success = zffalse;
        if(files != zfnull)
        {
            d->files = (jobjectArray)JNIUtilNewGlobalRef(jniEnv, files);
            success = this->resFindNext(fd);
        }
        if(!success)
        {
            this->resFindClose(fd);
        }
        return success;
    }
    virtual zfbool resFindNext(ZF_IN_OUT ZFFileFindData::Impl &fd)
    {
        if(fd.nativeFd == zfnull)
        {
            return zffalse;
        }
        JNIEnv *jniEnv = JNIGetJNIEnv();
        _ZFP_ZFProtocolZFFileResProcess_sys_Android_FindData *d = ZFCastStatic(_ZFP_ZFProtocolZFFileResProcess_sys_Android_FindData *, fd.nativeFd);
        if(d->curFileIndex >= JNIUtilGetArrayLength(jniEnv, d->files))
        {
            return zffalse;
        }
        jstring jsPath = (jstring)JNIUtilGetObjectArrayElement(jniEnv, d->files, d->curFileIndex);
        ++d->curFileIndex;
        const zfchar *sName = JNIUtilGetStringUTFChars(jniEnv, jsPath, zfnull);
        fd.fileName = sName;
        JNIUtilReleaseStringUTFChars(jniEnv, jsPath, sName);

        zfstring absPath = this->zfresPrefix;
        absPath += ZFFileSeparator();
        if(!d->parentPath.isEmpty())
        {
            absPath += d->parentPath;
            absPath += ZFFileSeparator();
        }
        absPath += fd.fileName.cString();

        AAsset *asset = AAssetManager_open(
            AAssetManager_fromJava(jniEnv, ZFImpl_sys_Android_assetManager()),
            absPath.cString(),
            AASSET_MODE_STREAMING);
        if(asset == zfnull)
        {
            fd.fileIsDir = zftrue;
        }
        else
        {
            AAsset_close(asset);
            fd.fileIsDir = zffalse;
            if(zfstringFindReversely(fd.fileName, this->zfresPostfix) == fd.fileName.length() - this->zfresPostfixLen)
            {
                fd.fileName.remove(fd.fileName.length() - this->zfresPostfixLen);
            }
        }

        return zftrue;
    }
    virtual void resFindClose(ZF_IN_OUT ZFFileFindData::Impl &fd)
    {
        if(fd.nativeFd == zfnull)
        {
            return ;
        }
        JNIEnv *jniEnv = JNIGetJNIEnv();
        _ZFP_ZFProtocolZFFileResProcess_sys_Android_FindData *d = ZFCastStatic(_ZFP_ZFProtocolZFFileResProcess_sys_Android_FindData *, fd.nativeFd);
        if(d->files != zfnull)
        {
            JNIUtilDeleteGlobalRef(jniEnv, d->files);
            d->files = zfnull;
        }
        zfdelete(d);
        fd.nativeFd = zfnull;
    }

private:
    jclass jclsOwner;
    /*
     * AssetManager doesn't support these format:
     * -  "." => ""
     * -  "./" => ""
     * -  "./path" => "path"
     * -  "path/." => "path"
     * -  "abc/./xyz" => "abc/xyz"
     */
    void resPathFormat(ZF_OUT zfstring &ret, ZF_IN const zfchar *s)
    {
        ret = this->zfresPrefix;
        if(zfscmpTheSame(s, "."))
        {
            return ;
        }
        ret += ZFFileSeparator();
        ret += s;
        zfindex tmp;

        // remove "/./"
        while((tmp = zfstringFind(ret, "/./")) != zfindexMax())
        {
            ret.remove(tmp, 2);
        }

        // remove tail "/."
        while(ret.length() >= 2 && zfsncmp(ret.cString(), "/.", 2) == 0)
        {
            ret.remove(ret.length() - 2);
        }

        // remove head "./"
        while(zfsncmp(ret.cString(), "./", 2) == 0)
        {
            ret.remove(0, 2);
        }

        // remove tail "/"
        while(ret.length() > 0 && ret[ret.length() - 1] == '/')
        {
            ret.remove(ret.length() - 1);
        }
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFFileResProcessImpl_sys_Android)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFFileResProcessImpl_sys_Android)

ZF_NAMESPACE_GLOBAL_END

#endif

