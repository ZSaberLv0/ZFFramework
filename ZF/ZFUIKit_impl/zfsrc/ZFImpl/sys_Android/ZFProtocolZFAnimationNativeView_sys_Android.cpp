#include "ZFImpl_sys_Android_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFAnimationNativeView.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

#define ZFImpl_sys_Android_JNI_ID_ZFAnimationNativeView ZFImpl_sys_Android_JNI_ID(ZFUIKit_1impl_ZFAnimationNativeView)
#define ZFImpl_sys_Android_JNI_NAME_ZFAnimationNativeView ZFImpl_sys_Android_JNI_NAME(ZFUIKit_impl.ZFAnimationNativeView)

#define ZFImpl_sys_Android_JNI_ID_ZFAnimationNativeView_NativeAnimation ZFImpl_sys_Android_JNI_ID(ZFUIKit_1impl_ZFAnimationNativeView_00024NativeAnimation)
#define ZFImpl_sys_Android_JNI_NAME_ZFAnimationNativeView_NativeAnimation ZFImpl_sys_Android_JNI_NAME(ZFUIKit_impl.ZFAnimationNativeView$NativeAnimation)

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFAnimationNativeViewImpl_sys_Android, ZFAnimationNativeView, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Android:Animation")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIView, "Android:View")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()
public:
    zfoverride
    virtual void protocolOnInit(void)
    {
        zfsuper::protocolOnInit();
        JNIEnv *jniEnv = JNIGetJNIEnv();
        jobject tmp = zfnull;

        tmp = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_ZFAnimationNativeView).c_str());
        this->jclsOwner = (jclass)JNIUtilNewGlobalRef(jniEnv, tmp);
        JNIUtilDeleteLocalRef(jniEnv, tmp);

        tmp = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_ZFAnimationNativeView_NativeAnimation).c_str());
        this->jclsNativeAnimation = (jclass)JNIUtilNewGlobalRef(jniEnv, tmp);
        JNIUtilDeleteLocalRef(jniEnv, tmp);
    }
    zfoverride
    virtual void protocolOnDealloc(void)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        JNIUtilDeleteGlobalRef(jniEnv, this->jclsOwner);
        JNIUtilDeleteGlobalRef(jniEnv, this->jclsNativeAnimation);
        zfsuper::protocolOnDealloc();
    }
public:
    virtual void *nativeAniCreate(ZF_IN ZFAnimationNativeView *ani)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_nativeAniCreate",
            JNIGetMethodSig(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object), JNIParamTypeContainer()
                .add(JNIPointerJNIType)
            ).c_str());
        jobject tmp = JNIUtilCallStaticObjectMethod(jniEnv, this->jclsOwner, jmId,
            JNIConvertZFObjectToJNIType(jniEnv, ani));
        jobject ret = JNIUtilNewGlobalRef(jniEnv, tmp);
        JNIUtilDeleteLocalRef(jniEnv, tmp);
        return ZFCastStatic(void *, ret);
    }
    virtual void nativeAniDestroy(ZF_IN ZFAnimationNativeView *ani,
                                  ZF_IN void *nativeAni)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_nativeAniDestroy",
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        jobject tmp = ZFCastStatic(jobject, nativeAni);
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId, tmp);
        JNIUtilDeleteGlobalRef(jniEnv, tmp);
    }

    virtual void nativeAniStart(ZF_IN ZFAnimationNativeView *ani,
                                ZF_IN zffloat nativeAniScale)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_nativeAniStart",
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        this->nativeAniSetup(ani, nativeAniScale);
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, ani->nativeAnimation()),
            ZFCastStatic(jobject, ZFCastStatic(ZFUIView *, ani->aniTarget())->nativeView()));
    }
    virtual void nativeAniStop(ZF_IN ZFAnimationNativeView *ani)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_nativeAniStop",
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, ani->nativeAnimation()),
            ZFCastStatic(jobject, ZFCastStatic(ZFUIView *, ani->aniTarget())->nativeView()));
    }

private:
    void nativeAniSetup(ZF_IN ZFAnimationNativeView *ani,
                        ZF_IN zffloat nativeAniScale)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_setup",
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_int)
                .add(JNIType::S_int)
            ).c_str());
        jobject nativeAnimation = ZFCastStatic(jobject, ani->nativeAnimation());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId
            , nativeAnimation
            , (jint)ani->aniCurve()
            , (jint)ani->aniDurationFixed()
            );

        // ============================================================
        // alpha
        if(ani->aniAlphaFrom() != 1)
        {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, this->jclsNativeAnimation, "aniAlphaFrom", JNIType::S_float.getId());
            JNIUtilSetFloatField(jniEnv, nativeAnimation, jfId, ani->aniAlphaFrom());
        }
        if(ani->aniAlphaTo() != 1)
        {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, this->jclsNativeAnimation, "aniAlphaTo", JNIType::S_float.getId());
            JNIUtilSetFloatField(jniEnv, nativeAnimation, jfId, ani->aniAlphaTo());
        }
        // ============================================================
        // scale
        if(ani->aniScaleXFrom() != 1)
        {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, this->jclsNativeAnimation, "aniScaleXFrom", JNIType::S_float.getId());
            JNIUtilSetFloatField(jniEnv, nativeAnimation, jfId, ani->aniScaleXFrom());
        }
        if(ani->aniScaleXTo() != 1)
        {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, this->jclsNativeAnimation, "aniScaleXTo", JNIType::S_float.getId());
            JNIUtilSetFloatField(jniEnv, nativeAnimation, jfId, ani->aniScaleXTo());
        }
        if(ani->aniScaleYFrom() != 1)
        {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, this->jclsNativeAnimation, "aniScaleYFrom", JNIType::S_float.getId());
            JNIUtilSetFloatField(jniEnv, nativeAnimation, jfId, ani->aniScaleYFrom());
        }
        if(ani->aniScaleYTo() != 1)
        {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, this->jclsNativeAnimation, "aniScaleYTo", JNIType::S_float.getId());
            JNIUtilSetFloatField(jniEnv, nativeAnimation, jfId, ani->aniScaleYTo());
        }
        if(ani->aniScaleZFrom() != 1)
        {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, this->jclsNativeAnimation, "aniScaleZFrom", JNIType::S_float.getId());
            JNIUtilSetFloatField(jniEnv, nativeAnimation, jfId, ani->aniScaleZFrom());
        }
        if(ani->aniScaleZTo() != 1)
        {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, this->jclsNativeAnimation, "aniScaleZTo", JNIType::S_float.getId());
            JNIUtilSetFloatField(jniEnv, nativeAnimation, jfId, ani->aniScaleZTo());
        }
        // ============================================================
        // translate by view size's percent
        if(ani->aniTranslateXFrom() != 0)
        {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, this->jclsNativeAnimation, "aniTranslateXFrom", JNIType::S_float.getId());
            JNIUtilSetFloatField(jniEnv, nativeAnimation, jfId, ani->aniTranslateXFrom());
        }
        if(ani->aniTranslateXTo() != 0)
        {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, this->jclsNativeAnimation, "aniTranslateXTo", JNIType::S_float.getId());
            JNIUtilSetFloatField(jniEnv, nativeAnimation, jfId, ani->aniTranslateXTo());
        }
        if(ani->aniTranslateYFrom() != 0)
        {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, this->jclsNativeAnimation, "aniTranslateYFrom", JNIType::S_float.getId());
            JNIUtilSetFloatField(jniEnv, nativeAnimation, jfId, ani->aniTranslateYFrom());
        }
        if(ani->aniTranslateYTo() != 0)
        {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, this->jclsNativeAnimation, "aniTranslateYTo", JNIType::S_float.getId());
            JNIUtilSetFloatField(jniEnv, nativeAnimation, jfId, ani->aniTranslateYTo());
        }
        if(ani->aniTranslateZFrom() != 0)
        {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, this->jclsNativeAnimation, "aniTranslateZFrom", JNIType::S_float.getId());
            JNIUtilSetFloatField(jniEnv, nativeAnimation, jfId, ani->aniTranslateZFrom());
        }
        if(ani->aniTranslateZTo() != 0)
        {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, this->jclsNativeAnimation, "aniTranslateZTo", JNIType::S_float.getId());
            JNIUtilSetFloatField(jniEnv, nativeAnimation, jfId, ani->aniTranslateZTo());
        }
        // ============================================================
        // translate by pixel
        if(ani->aniTranslatePixelXFrom() != 0)
        {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, this->jclsNativeAnimation, "aniTranslatePixelXFrom", JNIType::S_int.getId());
            JNIUtilSetIntField(jniEnv, nativeAnimation, jfId, (jint)(ani->aniTranslatePixelXFrom() * nativeAniScale));
        }
        if(ani->aniTranslatePixelXTo() != 0)
        {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, this->jclsNativeAnimation, "aniTranslatePixelXTo", JNIType::S_int.getId());
            JNIUtilSetIntField(jniEnv, nativeAnimation, jfId, (jint)(ani->aniTranslatePixelXTo() * nativeAniScale));
        }
        if(ani->aniTranslatePixelYFrom() != 0)
        {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, this->jclsNativeAnimation, "aniTranslatePixelYFrom", JNIType::S_int.getId());
            JNIUtilSetIntField(jniEnv, nativeAnimation, jfId, (jint)(ani->aniTranslatePixelYFrom() * nativeAniScale));
        }
        if(ani->aniTranslatePixelYTo() != 0)
        {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, this->jclsNativeAnimation, "aniTranslatePixelYTo", JNIType::S_int.getId());
            JNIUtilSetIntField(jniEnv, nativeAnimation, jfId, (jint)(ani->aniTranslatePixelYTo() * nativeAniScale));
        }
        if(ani->aniTranslatePixelZFrom() != 0)
        {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, this->jclsNativeAnimation, "aniTranslatePixelZFrom", JNIType::S_int.getId());
            JNIUtilSetIntField(jniEnv, nativeAnimation, jfId, (jint)(ani->aniTranslatePixelZFrom() * nativeAniScale));
        }
        if(ani->aniTranslatePixelZTo() != 0)
        {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, this->jclsNativeAnimation, "aniTranslatePixelZTo", JNIType::S_int.getId());
            JNIUtilSetIntField(jniEnv, nativeAnimation, jfId, (jint)(ani->aniTranslatePixelZTo() * nativeAniScale));
        }
        // ============================================================
        // rotate
        if(ani->aniRotateXFrom() != 0)
        {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, this->jclsNativeAnimation, "aniRotateXFrom", JNIType::S_float.getId());
            JNIUtilSetFloatField(jniEnv, nativeAnimation, jfId, ani->aniRotateXFrom());
        }
        if(ani->aniRotateXTo() != 0)
        {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, this->jclsNativeAnimation, "aniRotateXTo", JNIType::S_float.getId());
            JNIUtilSetFloatField(jniEnv, nativeAnimation, jfId, ani->aniRotateXTo());
        }
        if(ani->aniRotateYFrom() != 0)
        {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, this->jclsNativeAnimation, "aniRotateYFrom", JNIType::S_float.getId());
            JNIUtilSetFloatField(jniEnv, nativeAnimation, jfId, ani->aniRotateYFrom());
        }
        if(ani->aniRotateYTo() != 0)
        {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, this->jclsNativeAnimation, "aniRotateYTo", JNIType::S_float.getId());
            JNIUtilSetFloatField(jniEnv, nativeAnimation, jfId, ani->aniRotateYTo());
        }
        if(ani->aniRotateZFrom() != 0)
        {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, this->jclsNativeAnimation, "aniRotateZFrom", JNIType::S_float.getId());
            JNIUtilSetFloatField(jniEnv, nativeAnimation, jfId, ani->aniRotateZFrom());
        }
        if(ani->aniRotateZTo() != 0)
        {
            static jfieldID jfId = JNIUtilGetFieldID(jniEnv, this->jclsNativeAnimation, "aniRotateZTo", JNIType::S_float.getId());
            JNIUtilSetFloatField(jniEnv, nativeAnimation, jfId, ani->aniRotateZTo());
        }
    }

private:
    jclass jclsOwner;
    jclass jclsNativeAnimation;
ZFPROTOCOL_IMPLEMENTATION_END(ZFAnimationNativeViewImpl_sys_Android)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFAnimationNativeViewImpl_sys_Android)

ZF_NAMESPACE_GLOBAL_END

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFAnimationNativeView,
                         void, native_1notifyAniStop,
                         JNIPointer zfjniPointerOwnerZFAnimationNativeView)
{
    ZFPROTOCOL_ACCESS(ZFAnimationNativeView)->notifyAniStop(
        ZFCastZFObject(ZFAnimationNativeView *, JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFAnimationNativeView)));
}
JNI_METHOD_DECLARE_END()

#endif // #if ZF_ENV_sys_Android

