#include "ZFImpl_sys_Android_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIView.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIKeyboardState.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

#define ZFImpl_sys_Android_JNI_ID_ZFUIView ZFImpl_sys_Android_JNI_ID(ZFUIKit_1impl_ZFUIView)
#define ZFImpl_sys_Android_JNI_NAME_ZFUIView ZFImpl_sys_Android_JNI_NAME(ZFUIKit_impl.ZFUIView)

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIViewImpl_sys_Android, ZFUIView, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Android:View")

public:
    zfoverride
    virtual void protocolOnInit(void)
    {
        zfsuper::protocolOnInit();
        JNIEnv *jniEnv = JNIGetJNIEnv();
        jobject tmp = zfnull;

        tmp = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_ZFUIView).c_str());
        this->jclsZFUIView = (jclass)JNIUtilNewGlobalRef(jniEnv, tmp);
        JNIUtilDeleteLocalRef(jniEnv, tmp);
    }
    zfoverride
    virtual void protocolOnDealloc(void)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        JNIUtilDeleteGlobalRef(jniEnv, this->jclsZFUIView);
        zfsuper::protocolOnDealloc();
    }

    zfoverride
    virtual void protocolOnInitFinish(void)
    {
        zfsuper::protocolOnInitFinish();
        ZFUIKeyboardStateBuiltinImplRegister();
    }
    zfoverride
    virtual void protocolOnDeallocPrepare(void)
    {
        ZFUIKeyboardStateBuiltinImplUnregister();
        zfsuper::protocolOnDeallocPrepare();
    }

public:
    virtual zfbool nativeViewCacheOnSave(ZF_IN void *nativeView)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsZFUIView, "native_nativeViewCacheOnSave",
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsZFUIView, jmId,
            ZFCastStatic(jobject, nativeView));
        return zftrue;
    }
    virtual void nativeViewCacheOnRestore(ZF_IN ZFUIView *view,
                                          ZF_IN void *nativeView)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsZFUIView, "native_nativeViewCacheOnRestore",
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIPointerJNIType)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsZFUIView, jmId,
            ZFCastStatic(jobject, nativeView),
            JNIConvertZFObjectToJNIType(jniEnv, view));
    }
    virtual void *nativeViewCreate(ZF_IN ZFUIView *view)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsZFUIView, "native_nativeViewCreate",
            JNIGetMethodSig(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object), JNIParamTypeContainer()
                .add(JNIPointerJNIType)
            ).c_str());
        jobject tmp = JNIUtilCallStaticObjectMethod(jniEnv, this->jclsZFUIView, jmId,
            JNIConvertZFObjectToJNIType(jniEnv, view));
        jobject ret = JNIUtilNewGlobalRef(jniEnv, tmp);
        JNIUtilDeleteLocalRef(jniEnv, tmp);
        return ret;
    }
    virtual void nativeViewDestroy(ZF_IN void *nativeView)
    {
        // for performance, we won't have JNI call to destroy, simply delete the global ref

        JNIEnv *jniEnv = JNIGetJNIEnv();
        jobject nativeViewTmp = ZFCastStatic(jobject, nativeView);
        JNIUtilDeleteGlobalRef(jniEnv, nativeViewTmp);
    }

    virtual void nativeImplView(ZF_IN ZFUIView *view,
                                ZF_IN void *nativeImplViewOld,
                                ZF_IN void *nativeImplView,
                                ZF_IN zfindex virtualIndex)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsZFUIView, "native_nativeImplView",
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_int)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsZFUIView, jmId,
            ZFCastStatic(jobject, view->nativeView()),
            ZFCastStatic(jobject, nativeImplView),
            (jint)virtualIndex);
    }
    virtual void nativeImplViewFrame(ZF_IN ZFUIView *view,
                                     ZF_IN const ZFUIRect &rect)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsZFUIView, "native_nativeImplViewFrame",
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_int)
                .add(JNIType::S_int)
                .add(JNIType::S_int)
                .add(JNIType::S_int)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsZFUIView, jmId,
            ZFCastStatic(jobject, view->nativeView()),
            rect.x,
            rect.y,
            rect.width,
            rect.height);
    }
    virtual zffloat nativeViewScaleForImpl(ZF_IN void *nativeView)
    {
        static zffloat t = -1;
        if(t <= 0)
        {
            JNIEnv *jniEnv = JNIGetJNIEnv();
            static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsZFUIView, "native_nativeViewScaleForImpl",
                JNIGetMethodSig(JNIType::S_float, JNIParamTypeContainer()
                    .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                ).c_str());
            t = (zffloat)JNIUtilCallStaticFloatMethod(jniEnv, this->jclsZFUIView, jmId,
                ZFCastStatic(jobject, nativeView));
        }
        return t;
    }
    virtual zffloat nativeViewScaleForPhysicalPixel(ZF_IN void *nativeView)
    {
        static zffloat t = -1;
        if(t <= 0)
        {
            JNIEnv *jniEnv = JNIGetJNIEnv();
            static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsZFUIView, "native_nativeViewScaleForPhysicalPixel",
                JNIGetMethodSig(JNIType::S_float, JNIParamTypeContainer()
                    .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                ).c_str());
            t = (zffloat)JNIUtilCallStaticFloatMethod(jniEnv, this->jclsZFUIView, jmId,
                ZFCastStatic(jobject, nativeView));
        }
        return t;
    }

    // ============================================================
    // properties
public:
    virtual void viewVisible(ZF_IN ZFUIView *view,
                             ZF_IN zfbool viewVisible)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsZFUIView, "native_viewVisible",
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_boolean)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsZFUIView, jmId,
            ZFCastStatic(jobject, view->nativeView()),
            (jboolean)viewVisible);
    }
    virtual void viewAlpha(ZF_IN ZFUIView *view,
                           ZF_IN zffloat viewAlpha)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsZFUIView, "native_viewAlpha",
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_float)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsZFUIView, jmId,
            ZFCastStatic(jobject, view->nativeView()),
            (jfloat)viewAlpha);
    }
    virtual void viewUIEnable(ZF_IN ZFUIView *view,
                              ZF_IN zfbool viewUIEnable)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsZFUIView, "native_viewUIEnable",
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_boolean)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsZFUIView, jmId,
            ZFCastStatic(jobject, view->nativeView()),
            (jboolean)viewUIEnable);
    }
    virtual void viewUIEnableTree(ZF_IN ZFUIView *view,
                                  ZF_IN zfbool viewUIEnableTree)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsZFUIView, "native_viewUIEnableTree",
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_boolean)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsZFUIView, jmId,
            ZFCastStatic(jobject, view->nativeView()),
            (jboolean)viewUIEnableTree);
    }
    virtual void viewBackgroundColor(ZF_IN ZFUIView *view,
                                     ZF_IN const ZFUIColor &viewBackgroundColor)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsZFUIView, "native_viewBackgroundColor",
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_int)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsZFUIView, jmId,
            ZFCastStatic(jobject, view->nativeView()),
            ZFImpl_sys_Android_ZFUIKit_impl_ZFUIColorToColor(viewBackgroundColor));
    }

public:
    virtual void childAdd(ZF_IN ZFUIView *parent,
                          ZF_IN ZFUIView *child,
                          ZF_IN zfindex virtualIndex,
                          ZF_IN ZFUIViewChildLayerEnum childLayer,
                          ZF_IN zfindex childLayerIndex)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsZFUIView, "native_childAdd",
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_int)
                .add(JNIType::S_int)
                .add(JNIType::S_int)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsZFUIView, jmId,
            ZFCastStatic(jobject, parent->nativeView()),
            ZFCastStatic(jobject, child->nativeView()),
            (jint)virtualIndex,
            (jint)childLayer,
            (jint)childLayerIndex);
    }
    virtual void childRemove(ZF_IN ZFUIView *parent,
                             ZF_IN ZFUIView *child,
                             ZF_IN zfindex virtualIndex,
                             ZF_IN ZFUIViewChildLayerEnum childLayer,
                             ZF_IN zfindex childLayerIndex)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsZFUIView, "native_childRemove",
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_int)
                .add(JNIType::S_int)
                .add(JNIType::S_int)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsZFUIView, jmId,
            ZFCastStatic(jobject, parent->nativeView()),
            (jint)virtualIndex,
            (jint)childLayer,
            (jint)childLayerIndex);
    }
    virtual void childRemoveAllForDealloc(ZF_IN ZFUIView *parent)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsZFUIView, "native_childRemoveAllForDealloc",
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsZFUIView, jmId,
            ZFCastStatic(jobject, parent->nativeView()));
    }

public:
    virtual void viewFrame(ZF_IN ZFUIView *view,
                           ZF_IN const ZFUIRect &rect)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsZFUIView, "native_viewFrame",
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_int)
                .add(JNIType::S_int)
                .add(JNIType::S_int)
                .add(JNIType::S_int)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsZFUIView, jmId,
            ZFCastStatic(jobject, view->nativeView()),
            rect.x,
            rect.y,
            rect.width,
            rect.height);
    }

    virtual void layoutRequest(ZF_IN ZFUIView *view)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsZFUIView, "native_layoutRequest",
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsZFUIView, jmId,
            ZFCastStatic(jobject, view->nativeView()));
    }

    virtual void measureNativeView(ZF_OUT ZFUISize &ret,
                                   ZF_IN void *nativeView,
                                   ZF_IN const ZFUISize &sizeHint)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsZFUIView, "native_measureNativeView",
            JNIGetMethodSig(JNIType::S_array(JNIType::S_int), JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_int)
                .add(JNIType::S_int)
            ).c_str());
        jintArray jobjSize = (jintArray)JNIUtilCallStaticObjectMethod(jniEnv, this->jclsZFUIView, jmId,
            ZFCastStatic(jobject, nativeView),
            (jint)sizeHint.width,
            (jint)sizeHint.height);
        jint *jarrSize = JNIUtilGetIntArrayElements(jniEnv, jobjSize, NULL);
        ret.width = (zfint)jarrSize[0];
        ret.height = (zfint)jarrSize[1];
        JNIUtilReleaseIntArrayElements(jniEnv, jobjSize, jarrSize, JNI_ABORT);
        JNIUtilDeleteLocalRef(jniEnv, jobjSize);
    }

private:
    jclass jclsZFUIView;
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIViewImpl_sys_Android)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUIViewImpl_sys_Android)

ZF_NAMESPACE_GLOBAL_END

// ============================================================
// native methods for ZFUIView
JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFUIView,
                         void, native_1notifyLayoutView,
                         JNIPointer zfjniPointerOwnerZFUIView,
                         jint rect_x, jint rect_y, jint rect_width, jint rect_height)
{
    ZFPROTOCOL_ACCESS(ZFUIView)->notifyLayoutView(
        ZFCastZFObject(ZFUIView *, JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUIView)),
        ZFUIRectMake((zfint)rect_x, (zfint)rect_y, (zfint)rect_width, (zfint)rect_height));
}
JNI_METHOD_DECLARE_END()
JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFUIView,
                         void, native_1notifyUIEvent_1mouse,
                         JNIPointer zfjniPointerOwnerZFUIView,
                         jint mouseId,
                         jint mouseAction,
                         jint mousePointX,
                         jint mousePointY)
{
    zfblockedAllocWithCache(ZFUIMouseEvent, event);
    event->eventResolved(zffalse);
    event->mouseId = mouseId;
    event->mouseAction = (ZFUIMouseActionEnum)mouseAction;
    event->mousePoint = ZFUIPointMake(mousePointX, mousePointY);
    event->mouseButton = ZFUIMouseButton::e_MouseButtonLeft;
    ZFPROTOCOL_ACCESS(ZFUIView)->notifyUIEvent(
        ZFCastZFObject(ZFUIView *, JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUIView)),
        event);
}
JNI_METHOD_DECLARE_END()

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFUIView,
                         jboolean, native_1notifyUIEvent_1key,
                         JNIPointer zfjniPointerOwnerZFUIView,
                         jint keyId,
                         jint keyAction,
                         jint keyCode,
                         jint keyCodeRaw)
{
    zfblockedAllocWithCache(ZFUIKeyEvent, event);
    event->eventResolved(zffalse);
    event->keyId = keyId;
    event->keyAction = (ZFUIKeyActionEnum)keyAction;
    event->keyCode = (ZFUIKeyCodeEnum)keyCode;
    event->keyCodeRaw = keyCodeRaw;
    ZFPROTOCOL_ACCESS(ZFUIView)->notifyUIEvent(
        ZFCastZFObject(ZFUIView *, JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUIView)),
        event);
    return event->eventResolved();
}
JNI_METHOD_DECLARE_END()

#endif // #if ZF_ENV_sys_Android

