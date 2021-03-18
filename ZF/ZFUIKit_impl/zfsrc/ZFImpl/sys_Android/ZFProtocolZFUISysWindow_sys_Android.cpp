#include "ZFImpl_sys_Android_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUISysWindow.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIView.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

#define ZFImpl_sys_Android_JNI_ID_ZFUISysWindow ZFImpl_sys_Android_JNI_ID(ZFUIKit_1impl_ZFUISysWindow)
#define ZFImpl_sys_Android_JNI_NAME_ZFUISysWindow ZFImpl_sys_Android_JNI_NAME(ZFUIKit_impl.ZFUISysWindow)

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUISysWindowImpl_sys_Android, ZFUISysWindow, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Android:Activity")
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

        tmp = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_ZFUISysWindow).c_str());
        this->jclsOwner = (jclass)JNIUtilNewGlobalRef(jniEnv, tmp);
        JNIUtilDeleteLocalRef(jniEnv, tmp);

        this->_mainWindow = zfnull;
    }
    zfoverride
    virtual void protocolOnDealloc(void)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        JNIUtilDeleteGlobalRef(jniEnv, this->jclsOwner);
        zfsuper::protocolOnDealloc();
    }
public:
    virtual ZFUISysWindow *mainWindow(void)
    {
        if(this->_mainWindow == zfnull)
        {
            this->_mainWindow = zfRetain(ZFUISysWindow::ClassData()->newInstance().to<ZFUISysWindow *>());

            JNIEnv *jniEnv = JNIGetJNIEnv();
            static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_nativeMainWindowCreate",
                JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                    .add(JNIPointerJNIType)
                ).c_str());
            JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId,
                JNIConvertZFObjectToJNIType(jniEnv, this->_mainWindow));
        }
        return this->_mainWindow;
    }
    virtual void mainWindowOnCleanup(void)
    {
        if(this->_mainWindow != zfnull)
        {
            ZFUISysWindow *mainWindowTmp = this->_mainWindow;
            this->_mainWindow = zfnull;
            zfRelease(mainWindowTmp);
        }
    }
    virtual void mainWindowOnDestroy(void)
    {
        this->_mainWindow = zfnull;
    }

    // ============================================================
public:
    virtual void nativeWindowOnCleanup(ZF_IN ZFUISysWindow *sysWindow)
    {
    }

    virtual void nativeWindowRootViewOnAdd(ZF_IN ZFUISysWindow *sysWindow,
                                           ZF_OUT void *&nativeParentView)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_nativeWindowRootViewOnAdd",
            JNIGetMethodSig(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object), JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        nativeParentView = (void *)JNIUtilCallStaticObjectMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, sysWindow->nativeWindow()),
            ZFCastStatic(jobject, sysWindow->rootView()->nativeView())
            );
    }
    virtual void nativeWindowRootViewOnRemove(ZF_IN ZFUISysWindow *sysWindow)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_nativeWindowRootViewOnRemove",
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, sysWindow->nativeWindow()),
            ZFCastStatic(jobject, sysWindow->rootView()->nativeView())
            );
    }

    virtual zfautoObject modalWindowShow(ZF_IN ZFUISysWindow *sysWindowOwner)
    {
        zfautoObject modalWindow = ZFUISysWindow::ClassData()->newInstance();

        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_modalWindowShow",
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIPointerJNIType)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, sysWindowOwner->nativeWindow()),
            JNIConvertZFObjectToJNIType(jniEnv, modalWindow.toObject()));

        return modalWindow;
    }
    virtual void modalWindowFinish(ZF_IN ZFUISysWindow *sysWindowOwner,
                                   ZF_IN ZFUISysWindow *sysWindowToFinish)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_modalWindowFinish",
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, sysWindowToFinish->nativeWindow()));
    }

    virtual void sysWindowLayoutParamOnInit(ZF_IN ZFUISysWindow *sysWindow)
    {
    }
    virtual void sysWindowLayoutParamOnChange(ZF_IN ZFUISysWindow *sysWindow)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_sysWindowLayoutParamOnChange",
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, sysWindow->nativeWindow()));
    }

    virtual ZFUIOrientationEnum sysWindowOrientation(ZF_IN ZFUISysWindow *sysWindow)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_sysWindowOrientation",
            JNIGetMethodSig(JNIType::S_int, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        jint ret = JNIUtilCallStaticIntMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, sysWindow->nativeWindow()));
        return ZFCastStatic(ZFUIOrientationEnum, ret);
    }
    virtual void sysWindowOrientationFlags(ZF_IN ZFUISysWindow *sysWindow,
                                           ZF_IN const ZFUIOrientationFlags &flags)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_sysWindowOrientationFlags",
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_int)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, sysWindow->nativeWindow()),
            (jint)flags);
    }

private:
    jclass jclsOwner;
    ZFUISysWindow *_mainWindow;
ZFPROTOCOL_IMPLEMENTATION_END(ZFUISysWindowImpl_sys_Android)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUISysWindowImpl_sys_Android)

ZF_NAMESPACE_GLOBAL_END

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFUISysWindow,
                         void, native_1notifyMeasureWindow,
                         JNIPointer zfjniPointerOwnerZFUISysWindow,
                         jint refWidth,
                         jint refHeight,
                         jintArray resultRect)
{
    ZFUIRect result = ZFPROTOCOL_ACCESS(ZFUISysWindow)->notifyMeasureWindow(
        ZFCastZFObject(ZFUISysWindow *, JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUISysWindow)),
        ZFUIRectMake(0, 0, refWidth, refHeight),
        ZFUIMarginZero());
    jint buf[] = {(jint)result.x, (jint)result.y, (jint)result.width, (jint)result.height};
    JNIUtilSetIntArrayRegion(jniEnv, resultRect, 0, 4, buf);
}
JNI_METHOD_DECLARE_END()
JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFUISysWindow,
                         void, native_1notifyOnCreate,
                         JNIPointer zfjniPointerOwnerZFUISysWindow,
                         jobject nativeWindow)
{
    jobject nativeWindowGlobalRef = JNIUtilNewGlobalRef(jniEnv, nativeWindow);
    ZFPROTOCOL_ACCESS(ZFUISysWindow)->notifyOnCreate(
        ZFCastZFObject(ZFUISysWindow *, JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUISysWindow)),
        ZFCastStatic(void *, nativeWindowGlobalRef));
}
JNI_METHOD_DECLARE_END()
JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFUISysWindow,
                         void, native_1notifyOnDestroy,
                         JNIPointer zfjniPointerOwnerZFUISysWindow)
{
    ZFUISysWindow *window = ZFCastZFObject(ZFUISysWindow *, JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUISysWindow));
    jobject nativeWindowGlobalRef = ZFCastStatic(jobject, window->nativeWindow());
    ZFPROTOCOL_ACCESS(ZFUISysWindow)->notifyOnDestroy(window);
    JNIUtilDeleteGlobalRef(jniEnv, nativeWindowGlobalRef);
}
JNI_METHOD_DECLARE_END()
JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFUISysWindow,
                         void, native_1notifyOnResume,
                         JNIPointer zfjniPointerOwnerZFUISysWindow)
{
    ZFPROTOCOL_ACCESS(ZFUISysWindow)->notifyOnResume(
        ZFCastZFObject(ZFUISysWindow *, JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUISysWindow)));
}
JNI_METHOD_DECLARE_END()
JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFUISysWindow,
                         void, native_1notifyOnPause,
                         JNIPointer zfjniPointerOwnerZFUISysWindow)
{
    ZFPROTOCOL_ACCESS(ZFUISysWindow)->notifyOnPause(
        ZFCastZFObject(ZFUISysWindow *, JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUISysWindow)));
}
JNI_METHOD_DECLARE_END()
JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFUISysWindow,
                         void, native_1notifyOnRotate,
                         JNIPointer zfjniPointerOwnerZFUISysWindow)
{
    ZFPROTOCOL_ACCESS(ZFUISysWindow)->notifyOnRotate(
        ZFCastZFObject(ZFUISysWindow *, JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUISysWindow)));
}
JNI_METHOD_DECLARE_END()

#endif // #if ZF_ENV_sys_Android

