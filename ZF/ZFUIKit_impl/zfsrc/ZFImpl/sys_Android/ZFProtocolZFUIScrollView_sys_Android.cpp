#include "ZFImpl_sys_Android_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIScrollView.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

#define ZFImpl_sys_Android_JNI_ID_ZFUIScrollView ZFImpl_sys_Android_JNI_ID(ZFUIKit_1impl_ZFUIScrollView)
#define ZFImpl_sys_Android_JNI_NAME_ZFUIScrollView ZFImpl_sys_Android_JNI_NAME(ZFUIKit_impl.ZFUIScrollView)

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIScrollViewImpl_sys_Android, ZFUIScrollView, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Android:View")

public:
    zfoverride
    virtual void protocolOnInit(void)
    {
        zfsuper::protocolOnInit();
        JNIEnv *jniEnv = JNIGetJNIEnv();
        jobject tmp = zfnull;

        tmp = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_ZFUIScrollView).c_str());
        this->jclsZFUIScrollView = (jclass)JNIUtilNewGlobalRef(jniEnv, tmp);
        JNIUtilDeleteLocalRef(jniEnv, tmp);
    }
    zfoverride
    virtual void protocolOnDealloc(void)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        JNIUtilDeleteGlobalRef(jniEnv, this->jclsZFUIScrollView);
        zfsuper::protocolOnDealloc();
    }

public:
    virtual void *nativeScrollViewCreate(ZF_IN ZFUIScrollView *view)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsZFUIScrollView, "native_nativeScrollViewCreate",
            JNIGetMethodSig(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object), JNIParamTypeContainer()
                .add(JNIPointerJNIType)
            ).c_str());
        jobject tmp = JNIUtilCallStaticObjectMethod(jniEnv, this->jclsZFUIScrollView, jmId,
            JNIConvertZFObjectToJNIType(jniEnv, view));
        jobject ret = JNIUtilNewGlobalRef(jniEnv, tmp);
        JNIUtilDeleteLocalRef(jniEnv, tmp);
        return ret;
    }
    virtual void nativeScrollViewDestroy(ZF_IN ZFUIScrollView *view,
                                         ZF_IN void *nativeView)
    {
        // for performance, we won't have JNI call to destroy, simply delete the global ref

        JNIEnv *jniEnv = JNIGetJNIEnv();
        jobject nativeViewTmp = ZFCastStatic(jobject, nativeView);
        JNIUtilDeleteGlobalRef(jniEnv, nativeViewTmp);
    }

public:
    virtual void scrollEnable(ZF_IN ZFUIScrollView *scrollView,
                              ZF_IN zfbool scrollEnable)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsZFUIScrollView, "native_scrollEnable",
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_boolean)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsZFUIScrollView, jmId,
            ZFCastStatic(jobject, scrollView->nativeImplView()),
            (jboolean)scrollEnable);
    }
    virtual void scrollBounce(ZF_IN ZFUIScrollView *scrollView,
                              ZF_IN zfbool scrollBounceHorizontal,
                              ZF_IN zfbool scrollBounceVertical,
                              ZF_IN zfbool scrollBounceHorizontalAlways,
                              ZF_IN zfbool scrollBounceVerticalAlways)
{
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsZFUIScrollView, "native_scrollBounce",
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_boolean)
                .add(JNIType::S_boolean)
                .add(JNIType::S_boolean)
                .add(JNIType::S_boolean)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsZFUIScrollView, jmId,
            ZFCastStatic(jobject, scrollView->nativeImplView()),
            (jboolean)scrollBounceHorizontal,
            (jboolean)scrollBounceVertical,
            (jboolean)scrollBounceHorizontalAlways,
            (jboolean)scrollBounceVerticalAlways);
    }
    virtual void scrollContentFrame(ZF_IN ZFUIScrollView *scrollView,
                                    ZF_IN const ZFUIRect &frame)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsZFUIScrollView, "native_scrollContentFrame",
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_int)
                .add(JNIType::S_int)
                .add(JNIType::S_int)
                .add(JNIType::S_int)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsZFUIScrollView, jmId,
            ZFCastStatic(jobject, scrollView->nativeImplView()),
            (jint)frame.x,
            (jint)frame.y,
            (jint)frame.width,
            (jint)frame.height);
    }
    virtual zftimet scrollAnimationStart(ZF_IN ZFUIScrollView *scrollView,
                                         ZF_IN zftimet recommendTimerInterval)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsZFUIScrollView, "native_scrollAnimationStart",
            JNIGetMethodSig(JNIType::S_long, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_int)
            ).c_str());
        return (zftimet)JNIUtilCallStaticLongMethod(jniEnv, this->jclsZFUIScrollView, jmId,
            ZFCastStatic(jobject, scrollView->nativeImplView()),
            (jint)(zfuint)recommendTimerInterval);
    }
    virtual void scrollAnimationStop(ZF_IN ZFUIScrollView *scrollView)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsZFUIScrollView, "native_scrollAnimationStop",
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsZFUIScrollView, jmId,
            ZFCastStatic(jobject, scrollView->nativeImplView()));
    }

public:
    virtual void scrollChildAdd(ZF_IN ZFUIScrollView *parent,
                                ZF_IN ZFUIView *child,
                                ZF_IN zfindex atIndex)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsZFUIScrollView, "native_scrollChildAdd",
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_int)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsZFUIScrollView, jmId,
            ZFCastStatic(jobject, parent->nativeImplView()),
            ZFCastStatic(jobject, child->nativeView()),
            (jint)atIndex);
    }
    virtual void scrollChildRemove(ZF_IN ZFUIScrollView *parent,
                                   ZF_IN ZFUIView *child,
                                   ZF_IN zfindex atIndex)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsZFUIScrollView, "native_scrollChildRemove",
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_int)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsZFUIScrollView, jmId,
            ZFCastStatic(jobject, parent->nativeImplView()),
            (jint)atIndex);
    }
    virtual void scrollChildRemoveAllForDealloc(ZF_IN ZFUIScrollView *parent)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsZFUIScrollView, "native_scrollChildRemoveAllForDealloc",
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsZFUIScrollView, jmId,
            ZFCastStatic(jobject, parent->nativeImplView()));
    }

private:
    jclass jclsZFUIScrollView;
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIScrollViewImpl_sys_Android)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUIScrollViewImpl_sys_Android)

ZF_NAMESPACE_GLOBAL_END

// ============================================================
// native methods for ZFUIScrollView
JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFUIScrollView,
                         void, native_1notifyScrollViewDragBegin,
                         JNIPointer zfjniPointerOwnerZFUIScrollView,
                         jint mousePosX,
                         jint mousePosY,
                         jlong mouseTime)
{
    ZFPROTOCOL_ACCESS(ZFUIScrollView)->notifyScrollViewDragBegin(
        ZFCastZFObject(ZFUIScrollView *, JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUIScrollView)),
        ZFUIPointMake(mousePosX, mousePosY), (zftimet)mouseTime);
}
JNI_METHOD_DECLARE_END()
JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFUIScrollView,
                         void, native_1notifyScrollViewDrag,
                         JNIPointer zfjniPointerOwnerZFUIScrollView,
                         jint mousePosX,
                         jint mousePosY,
                         jlong mouseTime)
{
    ZFPROTOCOL_ACCESS(ZFUIScrollView)->notifyScrollViewDrag(
        ZFCastZFObject(ZFUIScrollView *, JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUIScrollView)),
        ZFUIPointMake(mousePosX, mousePosY), (zftimet)mouseTime);
}
JNI_METHOD_DECLARE_END()
JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFUIScrollView,
                         void, native_1notifyScrollViewDragEnd,
                         JNIPointer zfjniPointerOwnerZFUIScrollView,
                         jlong mouseTime,
                         jboolean needScrollAni)
{
    ZFPROTOCOL_ACCESS(ZFUIScrollView)->notifyScrollViewDragEnd(
        ZFCastZFObject(ZFUIScrollView *, JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUIScrollView)),
        (zftimet)mouseTime,
        (zfbool)needScrollAni);
}
JNI_METHOD_DECLARE_END()
JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFUIScrollView,
                         void, native_1notifyScrollViewScrollAnimation,
                         JNIPointer zfjniPointerOwnerZFUIScrollView,
                         jlong relativeTimeInMiliseconds)
{
    ZFPROTOCOL_ACCESS(ZFUIScrollView)->notifyScrollViewScrollAnimation(
        ZFCastZFObject(ZFUIScrollView *, JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUIScrollView)),
        (zftimet)relativeTimeInMiliseconds);
}
JNI_METHOD_DECLARE_END()

#endif // #if ZF_ENV_sys_Android

