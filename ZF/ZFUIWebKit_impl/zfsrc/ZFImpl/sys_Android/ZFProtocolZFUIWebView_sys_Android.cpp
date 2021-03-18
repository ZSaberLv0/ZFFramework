#include "ZFImpl_sys_Android_ZFUIWebKit_impl.h"
#include "ZFUIWebKit/protocol/ZFProtocolZFUIWebView.h"

#if ZF_ENV_sys_Android

#define ZFImpl_sys_Android_JNI_ID_ZFUIWebView ZFImpl_sys_Android_JNI_ID(ZFUIWebKit_1impl_ZFUIWebView)
#define ZFImpl_sys_Android_JNI_NAME_ZFUIWebView ZFImpl_sys_Android_JNI_NAME(ZFUIWebKit_impl.ZFUIWebView)

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIWebViewImpl_sys_Android, ZFUIWebView, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Android:WebView")

public:
    zfoverride
    virtual void protocolOnInit(void)
    {
        zfsuper::protocolOnInit();
        JNIEnv *jniEnv = JNIGetJNIEnv();
        jobject tmp = zfnull;

        tmp = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_ZFUIWebView).c_str());
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
    virtual void *nativeWebViewCreate(ZF_IN ZFUIWebView *webView)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_nativeWebViewCreate",
            JNIGetMethodSig(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object), JNIParamTypeContainer()
                .add(JNIPointerJNIType)
            ).c_str());
        jobject tmp = JNIUtilCallStaticObjectMethod(jniEnv, this->jclsOwner, jmId,
            JNIConvertZFObjectToJNIType(jniEnv, webView->toObject()));
        jobject ret = JNIUtilNewGlobalRef(jniEnv, tmp);
        JNIUtilDeleteLocalRef(jniEnv, tmp);
        return ZFCastStatic(void *, ret);
    }
    virtual void nativeWebViewDestroy(ZF_IN ZFUIWebView *webView,
                                      ZF_IN void *nativeWebView)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_nativeWebViewDestroy",
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        jobject nativeWebViewTmp = ZFCastStatic(jobject, nativeWebView);
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId, nativeWebViewTmp);
        JNIUtilDeleteGlobalRef(jniEnv, nativeWebViewTmp);
    }

    virtual void webLoadUrl(ZF_IN ZFUIWebView *webView,
                            ZF_IN const zfchar *url)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_webLoadUrl",
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        jobject urlTmp = JNIUtilNewStringUTF(jniEnv, url);
        JNIBlockedDeleteLocalRef(urlTmp);
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, webView->nativeImplView()),
            urlTmp);
    }
    virtual void webLoadHtml(ZF_IN ZFUIWebView *webView,
                             ZF_IN const zfchar *html,
                             ZF_IN_OPT const zfchar *baseUrl = zfnull)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_webLoadHtml",
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        jobject htmlTmp = JNIUtilNewStringUTF(jniEnv, html);
        JNIBlockedDeleteLocalRef(htmlTmp);
        jobject baseUrlTmp = ((baseUrl == zfnull) ? NULL : JNIUtilNewStringUTF(jniEnv, baseUrl));
        JNIBlockedDeleteLocalRef(baseUrlTmp);
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, webView->nativeImplView()),
            htmlTmp,
            baseUrlTmp);
    }
    virtual void webReload(ZF_IN ZFUIWebView *webView)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_webReload",
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, webView->nativeImplView()));
    }
    virtual void webLoadStop(ZF_IN ZFUIWebView *webView)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_webLoadStop",
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, webView->nativeImplView()));
    }

    virtual void webGoBack(ZF_IN ZFUIWebView *webView)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_webGoBack",
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, webView->nativeImplView()));
    }
    virtual void webGoForward(ZF_IN ZFUIWebView *webView)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_webGoForward",
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, webView->nativeImplView()));
    }

    virtual zfbool webLoading(ZF_IN ZFUIWebView *webView)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_webLoading",
            JNIGetMethodSig(JNIType::S_boolean, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        return JNIUtilCallStaticBooleanMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, webView->nativeImplView()));
    }
    virtual zfbool webGoBackAvailable(ZF_IN ZFUIWebView *webView)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_webGoBackAvailable",
            JNIGetMethodSig(JNIType::S_boolean, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        return JNIUtilCallStaticBooleanMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, webView->nativeImplView()));
    }
    virtual zfbool webGoForwardAvailable(ZF_IN ZFUIWebView *webView)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_webGoForwardAvailable",
            JNIGetMethodSig(JNIType::S_boolean, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        return JNIUtilCallStaticBooleanMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, webView->nativeImplView()));
    }

private:
    jclass jclsOwner;
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIWebViewImpl_sys_Android)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUIWebViewImpl_sys_Android)

ZF_NAMESPACE_GLOBAL_END

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFUIWebView,
                         void, native_1ZFUIWebView_1notifyWebLoadStateChanged,
                         JNIPointer zfjniPointerOwnerZFUIWebView)
{
    ZFPROTOCOL_ACCESS(ZFUIWebView)->notifyWebLoadStateChanged(
        ZFCastZFObject(ZFUIWebView *, JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUIWebView)));
}
JNI_METHOD_DECLARE_END()

#endif // #if ZF_ENV_sys_Android

