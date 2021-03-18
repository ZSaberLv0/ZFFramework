#include "ZFImpl_sys_Android_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIImageIO.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

#define ZFImpl_sys_Android_JNI_ID_ZFUIImageIO ZFImpl_sys_Android_JNI_ID(ZFUIKit_1impl_ZFUIImageIO)
#define ZFImpl_sys_Android_JNI_NAME_ZFUIImageIO ZFImpl_sys_Android_JNI_NAME(ZFUIKit_impl.ZFUIImageIO)

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIImageIOImpl_sys_Android, ZFUIImageIO, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Android:BitmapDrawable")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIImage, "Android:BitmapDrawable")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()
public:
    zfoverride
    virtual void protocolOnInit(void)
    {
        zfsuper::protocolOnInit();
        JNIEnv *jniEnv = JNIGetJNIEnv();
        jobject tmp = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_ZFUIImageIO).c_str());
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
    virtual void *imageApplyScale(ZF_IN zffloat imageScale,
                                  ZF_IN void *nativeImage,
                                  ZF_IN const ZFUISize &newSize,
                                  ZF_IN const ZFUIMargin &ninePatch)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_imageApplyScale",
            JNIGetMethodSig(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object), JNIParamTypeContainer()
                .add(JNIType::S_float)
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_int).add(JNIType::S_int)
                .add(JNIType::S_int).add(JNIType::S_int).add(JNIType::S_int).add(JNIType::S_int)
            ).c_str());
        jobject tmp = JNIUtilCallStaticObjectMethod(jniEnv, this->jclsOwner, jmId,
            imageScale,
            ZFCastStatic(jobject, nativeImage),
            (jint)newSize.width, (jint)newSize.height,
            (jint)ninePatch.left, (jint)ninePatch.top, (jint)ninePatch.right, (jint)ninePatch.bottom);
        JNIBlockedDeleteLocalRefWithEnv(tmp, jniEnv);
        return JNIUtilNewGlobalRef(jniEnv, tmp);
    }
    virtual void *imageLoadInFrame(ZF_IN zffloat imageScale,
                                   ZF_IN void *nativeImage,
                                   ZF_IN const ZFUIRect &frameInImage)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_imageLoadInFrame",
            JNIGetMethodSig(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object), JNIParamTypeContainer()
                .add(JNIType::S_float)
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_int).add(JNIType::S_int).add(JNIType::S_int).add(JNIType::S_int)
            ).c_str());
        jobject tmp = JNIUtilCallStaticObjectMethod(jniEnv, this->jclsOwner, jmId,
            imageScale,
            ZFCastStatic(jobject, nativeImage),
            (jint)frameInImage.x, (jint)frameInImage.y, (jint)frameInImage.width, (jint)frameInImage.height);
        JNIBlockedDeleteLocalRefWithEnv(tmp, jniEnv);
        return JNIUtilNewGlobalRef(jniEnv, tmp);
    }
    virtual void *imageLoadFromColor(ZF_IN zffloat imageScale,
                                     ZF_IN const ZFUIColor &color,
                                     ZF_IN const ZFUISize &size)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_imageLoadFromColor",
            JNIGetMethodSig(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object), JNIParamTypeContainer()
                .add(JNIType::S_int)
                .add(JNIType::S_int).add(JNIType::S_int)
            ).c_str());
        jobject tmp = JNIUtilCallStaticObjectMethod(jniEnv, this->jclsOwner, jmId,
            ZFImpl_sys_Android_ZFUIKit_impl_ZFUIColorToColor(color),
            (jint)size.width, (jint)size.height);
        JNIBlockedDeleteLocalRefWithEnv(tmp, jniEnv);
        return JNIUtilNewGlobalRef(jniEnv, tmp);
    }
private:
    jclass jclsOwner;
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIImageIOImpl_sys_Android)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUIImageIOImpl_sys_Android)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_Android

