#include "ZFImpl_sys_Android_ZFUIKit_impl.h"

#if ZF_ENV_sys_Android
ZF_NAMESPACE_GLOBAL_BEGIN

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFImpl_sys_Android_ZFUIKit_impl_jclsHolder, ZFLevelZFFrameworkEssential)
{
    jobject tmp = zfnull;
    JNIEnv *jniEnv = JNIGetJNIEnv();

    tmp = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_ZFAndroidPoint).c_str());
    this->jclsPoint = (jclass)JNIUtilNewGlobalRef(jniEnv, tmp);
    JNIUtilDeleteLocalRef(jniEnv, tmp);

    tmp = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_ZFAndroidSize).c_str());
    this->jclsSize = (jclass)JNIUtilNewGlobalRef(jniEnv, tmp);
    JNIUtilDeleteLocalRef(jniEnv, tmp);

    tmp = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_ZFAndroidMargin).c_str());
    this->jclsMargin = (jclass)JNIUtilNewGlobalRef(jniEnv, tmp);
    JNIUtilDeleteLocalRef(jniEnv, tmp);

    tmp = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_ZFAndroidRect).c_str());
    this->jclsRect = (jclass)JNIUtilNewGlobalRef(jniEnv, tmp);
    JNIUtilDeleteLocalRef(jniEnv, tmp);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFImpl_sys_Android_ZFUIKit_impl_jclsHolder)
{
    JNIEnv *jniEnv = JNIGetJNIEnv();
    JNIUtilDeleteGlobalRef(jniEnv, this->jclsPoint);
    JNIUtilDeleteGlobalRef(jniEnv, this->jclsSize);
    JNIUtilDeleteGlobalRef(jniEnv, this->jclsMargin);
    JNIUtilDeleteGlobalRef(jniEnv, this->jclsRect);
}
public:
    jclass jclsPoint;
    jclass jclsSize;
    jclass jclsMargin;
    jclass jclsRect;
ZF_GLOBAL_INITIALIZER_END(ZFImpl_sys_Android_ZFUIKit_impl_jclsHolder)

// ============================================================
// Point
jclass ZFImpl_sys_Android_jclassZFAndroidPoint(void)
{
    return ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_sys_Android_ZFUIKit_impl_jclsHolder)->jclsPoint;
}
jobject ZFImpl_sys_Android_ZFUIPointToZFAndroidPoint(ZF_IN const ZFUIPoint &point, ZF_IN_OUT jobject jobjPoint)
{
    JNIEnv *jniEnv = JNIGetJNIEnv();
    jclass jclsPoint = ZFImpl_sys_Android_jclassZFAndroidPoint();
    static jfieldID jfIdX = JNIUtilGetFieldID(jniEnv, jclsPoint, "x", JNIType::S_int.getId());
    static jfieldID jfIdY = JNIUtilGetFieldID(jniEnv, jclsPoint, "y", JNIType::S_int.getId());
    JNIUtilSetIntField(jniEnv, jobjPoint, jfIdX, (jint)point.x);
    JNIUtilSetIntField(jniEnv, jobjPoint, jfIdY, (jint)point.y);
    return jobjPoint;
}
jobject ZFImpl_sys_Android_ZFUIPointToZFAndroidPoint(ZF_IN const ZFUIPoint &point)
{
    JNIEnv *jniEnv = JNIGetJNIEnv();
    jclass jclsPoint = ZFImpl_sys_Android_jclassZFAndroidPoint();
    static jmethodID jmId = JNIUtilGetMethodID(jniEnv, jclsPoint, JNIConstructorName,
        JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
        ).c_str());
    jobject jobjPoint = JNIUtilNewObject(jniEnv, jclsPoint, jmId);
    return ZFImpl_sys_Android_ZFUIPointToZFAndroidPoint(point, jobjPoint);
}
void ZFImpl_sys_Android_ZFUIPointFromZFAndroidPointT(ZF_OUT ZFUIPoint &ret, ZF_IN jobject jobjPoint)
{
    JNIEnv *jniEnv = JNIGetJNIEnv();
    jclass jclsPoint = ZFImpl_sys_Android_jclassZFAndroidPoint();
    static jfieldID jfIdX = JNIUtilGetFieldID(jniEnv, jclsPoint, "x", JNIType::S_int.getId());
    static jfieldID jfIdY = JNIUtilGetFieldID(jniEnv, jclsPoint, "y", JNIType::S_int.getId());
    ret.x = JNIUtilGetIntField(jniEnv, jobjPoint, jfIdX);
    ret.y = JNIUtilGetIntField(jniEnv, jobjPoint, jfIdY);
}

// ============================================================
// Size
jclass ZFImpl_sys_Android_jclassZFAndroidSize(void)
{
    return ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_sys_Android_ZFUIKit_impl_jclsHolder)->jclsSize;
}
jobject ZFImpl_sys_Android_ZFUISizeToZFAndroidSize(ZF_IN const ZFUISize &size, ZF_IN_OUT jobject jobjSize)
{
    JNIEnv *jniEnv = JNIGetJNIEnv();
    jclass jclsSize = ZFImpl_sys_Android_jclassZFAndroidSize();
    static jfieldID jfIdWidth = JNIUtilGetFieldID(jniEnv, jclsSize, "width", JNIType::S_int.getId());
    static jfieldID jfIdHeight = JNIUtilGetFieldID(jniEnv, jclsSize, "height", JNIType::S_int.getId());
    JNIUtilSetIntField(jniEnv, jobjSize, jfIdWidth, (jint)size.width);
    JNIUtilSetIntField(jniEnv, jobjSize, jfIdHeight, (jint)size.height);
    return jobjSize;
}
jobject ZFImpl_sys_Android_ZFUISizeToZFAndroidSize(ZF_IN const ZFUISize &size)
{
    JNIEnv *jniEnv = JNIGetJNIEnv();
    jclass jclsSize = ZFImpl_sys_Android_jclassZFAndroidSize();
    static jmethodID jmId = JNIUtilGetMethodID(jniEnv, jclsSize, JNIConstructorName,
        JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
        ).c_str());
    jobject jobjSize = JNIUtilNewObject(jniEnv, jclsSize, jmId);
    return ZFImpl_sys_Android_ZFUISizeToZFAndroidSize(size, jobjSize);
}
void ZFImpl_sys_Android_ZFUISizeFromZFAndroidSizeT(ZF_OUT ZFUISize &ret, ZF_IN jobject jobjSize)
{
    JNIEnv *jniEnv = JNIGetJNIEnv();
    jclass jclsSize = ZFImpl_sys_Android_jclassZFAndroidSize();
    static jfieldID jfIdWidth = JNIUtilGetFieldID(jniEnv, jclsSize, "width", JNIType::S_int.getId());
    static jfieldID jfIdHeight = JNIUtilGetFieldID(jniEnv, jclsSize, "height", JNIType::S_int.getId());
    ret.width = JNIUtilGetIntField(jniEnv, jobjSize, jfIdWidth);
    ret.height = JNIUtilGetIntField(jniEnv, jobjSize, jfIdHeight);
}

// ============================================================
// Margin
jclass ZFImpl_sys_Android_jclassZFAndroidMargin(void)
{
    return ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_sys_Android_ZFUIKit_impl_jclsHolder)->jclsMargin;
}
jobject ZFImpl_sys_Android_ZFUIMarginToZFAndroidMargin(ZF_IN const ZFUIMargin &margin, ZF_IN_OUT jobject jobjMargin)
{
    JNIEnv *jniEnv = JNIGetJNIEnv();
    jclass jclsMargin = ZFImpl_sys_Android_jclassZFAndroidMargin();
    static jfieldID jfIdLeft = JNIUtilGetFieldID(jniEnv, jclsMargin, "left", JNIType::S_int.getId());
    static jfieldID jfIdTop = JNIUtilGetFieldID(jniEnv, jclsMargin, "top", JNIType::S_int.getId());
    static jfieldID jfIdRight = JNIUtilGetFieldID(jniEnv, jclsMargin, "right", JNIType::S_int.getId());
    static jfieldID jfIdBottom = JNIUtilGetFieldID(jniEnv, jclsMargin, "bottom", JNIType::S_int.getId());
    JNIUtilSetIntField(jniEnv, jobjMargin, jfIdLeft, (jint)margin.left);
    JNIUtilSetIntField(jniEnv, jobjMargin, jfIdTop, (jint)margin.top);
    JNIUtilSetIntField(jniEnv, jobjMargin, jfIdRight, (jint)margin.right);
    JNIUtilSetIntField(jniEnv, jobjMargin, jfIdBottom, (jint)margin.bottom);
    return jobjMargin;
}
jobject ZFImpl_sys_Android_ZFUIMarginToZFAndroidMargin(ZF_IN const ZFUIMargin &margin)
{
    JNIEnv *jniEnv = JNIGetJNIEnv();
    jclass jclsMargin = ZFImpl_sys_Android_jclassZFAndroidMargin();
    static jmethodID jmId = JNIUtilGetMethodID(jniEnv, jclsMargin, JNIConstructorName,
        JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
        ).c_str());
    jobject jobjMargin = JNIUtilNewObject(jniEnv, jclsMargin, jmId);
    return ZFImpl_sys_Android_ZFUIMarginToZFAndroidMargin(margin, jobjMargin);
}
void ZFImpl_sys_Android_ZFUIMarginFromZFAndroidMarginT(ZF_OUT ZFUIMargin &ret, ZF_IN jobject jobjMargin)
{
    JNIEnv *jniEnv = JNIGetJNIEnv();
    jclass jclsMargin = ZFImpl_sys_Android_jclassZFAndroidMargin();
    static jfieldID jfIdLeft = JNIUtilGetFieldID(jniEnv, jclsMargin, "left", JNIType::S_int.getId());
    static jfieldID jfIdTop = JNIUtilGetFieldID(jniEnv, jclsMargin, "top", JNIType::S_int.getId());
    static jfieldID jfIdRight = JNIUtilGetFieldID(jniEnv, jclsMargin, "right", JNIType::S_int.getId());
    static jfieldID jfIdBottom = JNIUtilGetFieldID(jniEnv, jclsMargin, "bottom", JNIType::S_int.getId());
    ret.left = JNIUtilGetIntField(jniEnv, jobjMargin, jfIdLeft);
    ret.top = JNIUtilGetIntField(jniEnv, jobjMargin, jfIdTop);
    ret.right = JNIUtilGetIntField(jniEnv, jobjMargin, jfIdRight);
    ret.bottom = JNIUtilGetIntField(jniEnv, jobjMargin, jfIdBottom);
}

// ============================================================
// Rect
jclass ZFImpl_sys_Android_jclassZFAndroidRect(void)
{
    return ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_sys_Android_ZFUIKit_impl_jclsHolder)->jclsRect;
}
jobject ZFImpl_sys_Android_ZFUIRectToZFAndroidRect(ZF_IN const ZFUIRect &rect, ZF_IN_OUT jobject jobjRect)
{
    JNIEnv *jniEnv = JNIGetJNIEnv();
    jclass jclsRect = ZFImpl_sys_Android_jclassZFAndroidRect();
    static jfieldID jfIdX = JNIUtilGetFieldID(jniEnv, jclsRect, "x", JNIType::S_int.getId());
    static jfieldID jfIdY = JNIUtilGetFieldID(jniEnv, jclsRect, "y", JNIType::S_int.getId());
    static jfieldID jfIdWidth = JNIUtilGetFieldID(jniEnv, jclsRect, "width", JNIType::S_int.getId());
    static jfieldID jfIdHeight = JNIUtilGetFieldID(jniEnv, jclsRect, "height", JNIType::S_int.getId());
    JNIUtilSetIntField(jniEnv, jobjRect, jfIdX, (jint)rect.x);
    JNIUtilSetIntField(jniEnv, jobjRect, jfIdY, (jint)rect.y);
    JNIUtilSetIntField(jniEnv, jobjRect, jfIdWidth, (jint)rect.width);
    JNIUtilSetIntField(jniEnv, jobjRect, jfIdHeight, (jint)rect.height);
    return jobjRect;
}
jobject ZFImpl_sys_Android_ZFUIRectToZFAndroidRect(ZF_IN const ZFUIRect &rect)
{
    JNIEnv *jniEnv = JNIGetJNIEnv();
    jclass jclsRect = ZFImpl_sys_Android_jclassZFAndroidRect();
    static jmethodID jmId = JNIUtilGetMethodID(jniEnv, jclsRect, JNIConstructorName,
        JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
        ).c_str());
    jobject jobjRect = JNIUtilNewObject(jniEnv, jclsRect, jmId);
    return ZFImpl_sys_Android_ZFUIRectToZFAndroidRect(rect, jobjRect);
}
void ZFImpl_sys_Android_ZFUIRectFromZFAndroidRectT(ZF_OUT ZFUIRect &ret, ZF_IN jobject jobjRect)
{
    JNIEnv *jniEnv = JNIGetJNIEnv();
    jclass jclsRect = ZFImpl_sys_Android_jclassZFAndroidRect();
    static jfieldID jfIdX = JNIUtilGetFieldID(jniEnv, jclsRect, "x", JNIType::S_int.getId());
    static jfieldID jfIdY = JNIUtilGetFieldID(jniEnv, jclsRect, "y", JNIType::S_int.getId());
    static jfieldID jfIdWidth = JNIUtilGetFieldID(jniEnv, jclsRect, "width", JNIType::S_int.getId());
    static jfieldID jfIdHeight = JNIUtilGetFieldID(jniEnv, jclsRect, "height", JNIType::S_int.getId());
    ret.x = JNIUtilGetIntField(jniEnv, jobjRect, jfIdX);
    ret.y = JNIUtilGetIntField(jniEnv, jobjRect, jfIdY);
    ret.width = JNIUtilGetIntField(jniEnv, jobjRect, jfIdWidth);
    ret.height = JNIUtilGetIntField(jniEnv, jobjRect, jfIdHeight);
}

jint ZFImpl_sys_Android_ZFUIColorToColor(ZF_IN const ZFUIColor &color)
{
    return (jint)(0
            | ((zfuint)(ZFUIColorGetA(color) * 0xFF) << 24)
            | ((zfuint)(ZFUIColorGetR(color) * 0xFF) << 16)
            | ((zfuint)(ZFUIColorGetG(color) * 0xFF) << 8)
            | ((zfuint)(ZFUIColorGetB(color) * 0xFF) << 0)
        );
}
void ZFImpl_sys_Android_ZFUIColorFromColorT(ZF_OUT ZFUIColor &ret, ZF_IN jint jColor)
{
    ret = ZFUIColorMake(
            (zfuint)((jColor >> 16) & 0xFF) / 255.0f,
            (zfuint)((jColor >> 8) & 0xFF) / 255.0f,
            (zfuint)((jColor >> 0) & 0xFF) / 255.0f,
            (zfuint)((jColor >> 24) & 0xFF) / 255.0f
        );
}

// ============================================================
void ZFImpl_sys_Android_viewTreePrintT(ZF_OUT zfstring &ret, ZF_IN jobject nativeView)
{
    JNIEnv *jniEnv = JNIGetJNIEnv();
    jclass jcls = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_ZFAndroidUI).c_str());
    static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, jcls, "native_viewTreePrint",
        JNIGetMethodSig(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_String), JNIParamTypeContainer()
            .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
        ).c_str());
    jobject tmp = JNIUtilCallStaticObjectMethod(jniEnv, jcls, jmId, nativeView);
    const char *utf = JNIUtilGetStringUTFChars(jniEnv, (jstring)tmp, zfnull);
    ret += utf;
    JNIUtilReleaseStringUTFChars(jniEnv, (jstring)tmp, utf);
}

ZF_NAMESPACE_GLOBAL_END

#if 1 // zfzfzf
    #include "ZFUIKit/ZFUISysWindow.h"
    ZF_GLOBAL_INITIALIZER_INIT(ZFImpl_sys_Android_autoPrintViewTree)
    {
        if(!ZFProtocolIsAvailable("ZFUIView"))
        {
            return ;
        }
        ZFLISTENER_LOCAL(windowOnPause, {
            ZFUISysWindow *sysWindow = listenerData.sender<ZFUISysWindow *>();
            zfstring s;
            ZFImpl_sys_Android_viewTreePrintT(s, (jobject)sysWindow->rootView()->nativeView());
            zfLogTrimT() << s;
        })
        this->windowOnPauseListener = windowOnPause;
        ZFGlobalObserver().observerAdd(
            ZFUISysWindow::EventSysWindowOnPause(), this->windowOnPauseListener);
    }
    ZF_GLOBAL_INITIALIZER_DESTROY(ZFImpl_sys_Android_autoPrintViewTree)
    {
        ZFGlobalObserver().observerRemove(
            ZFUISysWindow::EventSysWindowOnPause(), this->windowOnPauseListener);
    }
    private:
        ZFListener windowOnPauseListener;
    ZF_GLOBAL_INITIALIZER_END(ZFImpl_sys_Android_autoPrintViewTree)
#endif

#endif // #if ZF_ENV_sys_Android

