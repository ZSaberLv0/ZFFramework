#include "ZFImpl_sys_Android_ZF_impl.h"
#include "ZFCore/ZFObject.h"
#include "ZFCore/ZFString.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFImpl_sys_Android_jclsHolder, ZFLevelZFFrameworkStatic)
{
    jobject tmp = zfnull;
    JNIEnv *jniEnv = JNIGetJNIEnv();

    tmp = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_Object).c_str());
    this->jclsObject = (jclass)JNIUtilNewGlobalRef(jniEnv, tmp);
    JNIUtilDeleteLocalRef(jniEnv, tmp);

    tmp = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_String).c_str());
    this->jclsString = (jclass)JNIUtilNewGlobalRef(jniEnv, tmp);
    JNIUtilDeleteLocalRef(jniEnv, tmp);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFImpl_sys_Android_jclsHolder)
{
    JNIEnv *jniEnv = JNIGetJNIEnv();
    JNIUtilDeleteGlobalRef(jniEnv, this->jclsObject);
    JNIUtilDeleteGlobalRef(jniEnv, this->jclsString);
}
public:
    jclass jclsObject;
    jclass jclsString;
ZF_GLOBAL_INITIALIZER_END(ZFImpl_sys_Android_jclsHolder)

jclass ZFImpl_sys_Android_jclassObject(void)
{
    return ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_sys_Android_jclsHolder)->jclsObject;
}

void ZFImpl_sys_Android_zfstringFromString(ZF_IN_OUT zfstring &s, ZF_IN jobject jstr)
{
    if(jstr == NULL)
    {
        return ;
    }
    JNIEnv *jniEnv = JNIGetJNIEnv();
    jstring nativeString = ZFCastStatic(jstring, jstr);
    const zfchar *utf8 = JNIUtilGetStringUTFChars(jniEnv, nativeString, zfnull);
    if(utf8 != zfnull)
    {
        s += utf8;
    }
    JNIUtilReleaseStringUTFChars(jniEnv, nativeString, utf8);
}
jobject ZFImpl_sys_Android_zfstringToString(ZF_IN const zfstring &s)
{
    JNIEnv *jniEnv = JNIGetJNIEnv();
    return JNIUtilNewStringUTF(jniEnv, s.cString());
}
jobject ZFImpl_sys_Android_zfstringToString(ZF_IN const zfchar *s)
{
    JNIEnv *jniEnv = JNIGetJNIEnv();
    if(s == zfnull)
    {
        s = "";
    }
    return JNIUtilNewStringUTF(jniEnv, s);
}

zfstring ZFImpl_sys_Android_stackTrace(void)
{
    JNIEnv *jniEnv = JNIGetJNIEnv();
    jclass jcls = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_ZFAndroidLog).c_str());
    static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, jcls, "stackTrace",
        JNIGetMethodSig(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_String), JNIParamTypeContainer()
        ).c_str());
    jstring jobjString = ZFCastStatic(jstring, JNIUtilCallStaticObjectMethod(jniEnv, jcls, jmId));
    const char *utf = JNIUtilGetStringUTFChars(jniEnv, jobjString, zfnull);
    zfstring ret;
    if(utf != zfnull)
    {
        ret = utf;
    }
    JNIUtilReleaseStringUTFChars(jniEnv, jobjString, utf);
    return ret;
}
void ZFImpl_sys_Android_stackTracePrint(void)
{
    JNIEnv *jniEnv = JNIGetJNIEnv();
    jclass jcls = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_ZFAndroidLog).c_str());
    static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, jcls, "stackTracePrint",
        JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
        ).c_str());
    JNIUtilCallStaticVoidMethod(jniEnv, jcls, jmId);
}

void ZFImpl_sys_Android_objectInfoT(ZF_OUT zfstring &ret, ZF_IN jobject nativeObject)
{
    JNIEnv *jniEnv = JNIGetJNIEnv();
    jclass jcls = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_ZFAndroidLog).c_str());
    static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, jcls, "objectInfo",
        JNIGetMethodSig(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_String), JNIParamTypeContainer()
            .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
        ).c_str());
    jstring tmp = ZFCastStatic(jstring, JNIUtilCallStaticObjectMethod(jniEnv, jcls, jmId, nativeObject));
    const char *utf = JNIUtilGetStringUTFChars(jniEnv, tmp, zfnull);
    ret += utf;
    JNIUtilReleaseStringUTFChars(jniEnv, tmp, utf);
}

// ============================================================
// ZFAndroidBuffer
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFImpl_sys_Android_ZFAndroidBuffer_jclsHolder, ZFLevelZFFrameworkStatic)
{
    jobject tmp = zfnull;
    JNIEnv *jniEnv = JNIGetJNIEnv();

    tmp = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_ZFAndroidBuffer).c_str());
    this->jclsBuffer = (jclass)JNIUtilNewGlobalRef(jniEnv, tmp);
    JNIUtilDeleteLocalRef(jniEnv, tmp);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFImpl_sys_Android_ZFAndroidBuffer_jclsHolder)
{
    JNIEnv *jniEnv = JNIGetJNIEnv();
    JNIUtilDeleteGlobalRef(jniEnv, this->jclsBuffer);
}
public:
    jclass jclsBuffer;
ZF_GLOBAL_INITIALIZER_END(ZFImpl_sys_Android_ZFAndroidBuffer_jclsHolder)

const ZFImpl_sys_Android_Buffer ZFImpl_sys_Android_BufferZero = {0};
jclass ZFImpl_sys_Android_jclassZFAndroidBuffer(void)
{
    return ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_sys_Android_ZFAndroidBuffer_jclsHolder)->jclsBuffer;
}
void ZFImpl_sys_Android_ZFAndroidBufferFromJava(ZF_OUT ZFImpl_sys_Android_Buffer &ret, ZF_IN jobject jobjBuffer)
{
    JNIEnv *jniEnv = JNIGetJNIEnv();
    jclass jclsBuffer = ZFImpl_sys_Android_jclassZFAndroidBuffer();
    static jmethodID jmId_buffer = JNIUtilGetStaticMethodID(jniEnv, jclsBuffer, "native_buffer",
        JNIGetMethodSig(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object), JNIParamTypeContainer()
            .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
        ).c_str());
    static jmethodID jmId_bufferSize = JNIUtilGetStaticMethodID(jniEnv, jclsBuffer, "native_bufferSize",
        JNIGetMethodSig(JNIType::S_int, JNIParamTypeContainer()
            .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
        ).c_str());
    jobject jobjByteBuffer = JNIUtilCallStaticObjectMethod(jniEnv, jclsBuffer, jmId_buffer, jobjBuffer);
    jint jByteBufferSize = JNIUtilCallStaticIntMethod(jniEnv, jclsBuffer, jmId_bufferSize, jobjBuffer);
    ret.buffer = (zfbyte *)JNIUtilGetDirectBufferAddress(jniEnv, jobjByteBuffer);
    ret.bufferSize = (zfindex)jByteBufferSize;
    JNIUtilDeleteLocalRef(jniEnv, jobjByteBuffer);
}
jobject ZFImpl_sys_Android_ZFAndroidBufferToJava(ZF_IN void *buffer, ZF_IN zfindex bufferSize)
{
    JNIEnv *jniEnv = JNIGetJNIEnv();
    jclass jclsBuffer = ZFImpl_sys_Android_jclassZFAndroidBuffer();
    static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, jclsBuffer, "native_bufferForByteBuffer",
        JNIGetMethodSig(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object), JNIParamTypeContainer()
            .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
        ).c_str());
    jobject jobjByteBuffer = JNIUtilNewDirectByteBuffer(jniEnv, buffer, bufferSize);
    if(jobjByteBuffer == NULL)
    {
        return NULL;
    }
    jobject ret = JNIUtilCallStaticObjectMethod(jniEnv, jclsBuffer, jmId, jobjByteBuffer);
    JNIUtilDeleteLocalRef(jniEnv, jobjByteBuffer);
    return ret;
}

ZF_NAMESPACE_GLOBAL_END

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFAndroidLog,
                         jstring, native_1objectInfo,
                         JNIPointer zfjnipointerZFObject)
{
    ZFObject *obj = ZFCastZFObject(ZFObject *, JNIConvertZFObjectFromJNIType(jniEnv, zfjnipointerZFObject));
    return jniEnv->NewStringUTF(ZFObjectInfo(obj).cString());
}
JNI_METHOD_DECLARE_END()

#endif

