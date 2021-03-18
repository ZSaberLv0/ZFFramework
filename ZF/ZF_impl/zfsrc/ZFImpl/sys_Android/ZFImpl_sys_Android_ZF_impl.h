#ifndef _ZFI_ZFImpl_sys_Android_ZF_impl_h_
#define _ZFI_ZFImpl_sys_Android_ZF_impl_h_

#include "ZFImpl.h"

#if ZF_ENV_sys_Android

#define _ZFP_ZFImpl_sys_Android_JNI_DEBUG 0
#if _ZFP_ZFImpl_sys_Android_JNI_DEBUG
    #include <jni.h>
    #include <android/log.h>
    #define JNIUtilWrap_Log(fmt, ...) \
        ((void)__android_log_print(ANDROID_LOG_ERROR, "test", fmt, ##__VA_ARGS__))

    #define JNIUtilWrap_Enable 1

    inline const char *_JNIUtilWrap_file(const char *filePath)
    {
        const char *ret = filePath;
        while(*filePath)
        {
            if(*filePath == '/' || *filePath == '\\')
            {
                ret = filePath + 1;
            }
            ++filePath;
        }
        return ret;
    }
    template<typename T_Obj>
    T_Obj _JNIUtilWrap_LogAction(T_Obj obj, const char *callerFile, const char *callerFunction, int callerLine,
                                 const char *actionName, const char *type)
    {
        JNIUtilWrap_Log("[%s %s (%d)] %s %s %p",
            _JNIUtilWrap_file(callerFile), callerFunction, callerLine,
            type, actionName, (const void *)obj);
        return obj;
    }

    #if 1
        #define JNIUtilWrap_Created(obj, callerFile, callerFunction, callerLine, actionName) \
            _JNIUtilWrap_LogAction(obj, callerFile, callerFunction, callerLine, actionName, "Create")
    #endif
    #ifndef JNIUtilWrap_Released
        #define JNIUtilWrap_Released(obj, callerFile, callerFunction, callerLine, actionName) \
            _JNIUtilWrap_LogAction(obj, callerFile, callerFunction, callerLine, actionName, "Release")
    #endif
    #ifndef JNIUtilWrap_FuncCalled
        #if 1
            #include <time.h>
            class _JNIUtilWrap_FuncCalledTimeLog
            {
            public:
                _JNIUtilWrap_FuncCalledTimeLog(const char *callerFunction)
                : _callerFunction(callerFunction)
                , _startTime(clock())
                {
                }
                ~_JNIUtilWrap_FuncCalledTimeLog(void)
                {
                    JNIUtilWrap_Log("[%s] %lf",
                        _callerFunction,
                        (double)((double)(clock() - _startTime) / CLOCKS_PER_SEC));
                }
            private:
                const char *_callerFunction;
                clock_t _startTime;
            };
            #define JNIUtilWrap_FuncCalled(callerFile, callerFunction, callerLine, actionName)  \
                _JNIUtilWrap_FuncCalledTimeLog(callerFunction), \
                _JNIUtilWrap_LogAction(NULL, callerFile, callerFunction, callerLine, actionName, "FuncCall")
        #else
            #define JNIUtilWrap_FuncCalled(callerFile, callerFunction, callerLine, actionName)  \
                _JNIUtilWrap_LogAction(NULL, callerFile, callerFunction, callerLine, actionName, "FuncCall")
        #endif
    #endif
    #ifndef JNIUtilWrap_CallbackCalled
        #define JNIUtilWrap_CallbackCalled(className, actionName) \
            JNIUtilWrap_Log("[%s %s (%d)] Callback %s %s", _JNIUtilWrap_file(__FILE__), __FUNCTION__, __LINE__, className, actionName);
    #endif
#endif // #if _ZFP_ZFImpl_sys_Android_JNI_DEBUG

#include "ZFImpl/tools/JNIUtil/AndroidJNIUtil.h"
using namespace JNIUtil;
using namespace AndroidJNIUtil;

ZF_NAMESPACE_GLOBAL_BEGIN

#define ZFImpl_sys_Android_ROOT_JNI_ID com_ZFFramework_Android
#define ZFImpl_sys_Android_ROOT_JNI_NAME "com.ZFFramework.Android"

// usually used when declare JNI method
#define ZFImpl_sys_Android_JNI_ID(clsId) ZFM_CAT(ZFM_CAT(ZFImpl_sys_Android_ROOT_JNI_ID, _), clsId)
// usually used when find class
#define ZFImpl_sys_Android_JNI_NAME(clsName) ZFImpl_sys_Android_ROOT_JNI_NAME "." ZFM_TOSTRING(clsName)

// ============================================================
// Object
#define ZFImpl_sys_Android_JNI_ID_Object java_lang_Object
#define ZFImpl_sys_Android_JNI_NAME_Object "java.lang.Object"
extern ZF_ENV_EXPORT jclass ZFImpl_sys_Android_jclassObject(void);

// ============================================================
// String
#define ZFImpl_sys_Android_JNI_ID_String java_lang_String
#define ZFImpl_sys_Android_JNI_NAME_String "java.lang.String"
extern ZF_ENV_EXPORT jclass ZFImpl_sys_Android_jclassString(void);

// ============================================================
// ZFAndroidLog
#define ZFImpl_sys_Android_JNI_ID_ZFAndroidLog ZFImpl_sys_Android_JNI_ID(NativeUtil_ZFAndroidLog)
#define ZFImpl_sys_Android_JNI_NAME_ZFAndroidLog ZFImpl_sys_Android_JNI_NAME(NativeUtil.ZFAndroidLog)

// ============================================================
// utils
/**
 * @brief see #JNIConvertPointerToJNIType
 */
#ifndef JNIConvertZFObjectToJNIType
#define JNIConvertZFObjectToJNIType(jniEnv, p) JNIConvertPointerToJNIType(jniEnv, ZFObjectToObject(p))
#endif
/**
 * @brief see #JNIConvertPointerToJNIType
 */
#ifndef JNIConvertZFObjectFromJNIType
#define JNIConvertZFObjectFromJNIType(jniEnv, d) ZFCastStatic(ZFObject *, JNIConvertPointerFromJNIType(jniEnv, d))
#endif

// ============================================================
extern ZF_ENV_EXPORT void ZFImpl_sys_Android_zfstringFromString(ZF_IN_OUT zfstring &s, ZF_IN jobject jstr);
inline zfstring ZFImpl_sys_Android_zfstringFromString(ZF_IN jobject jstr)
{
    zfstring ret;
    ZFImpl_sys_Android_zfstringFromString(ret, jstr);
    return ret;
}
// note returned object must be deleted by DeleteLocalRef
extern ZF_ENV_EXPORT jobject ZFImpl_sys_Android_zfstringToString(ZF_IN const zfstring &s);
// note returned object must be deleted by DeleteLocalRef
extern ZF_ENV_EXPORT jobject ZFImpl_sys_Android_zfstringToString(ZF_IN const zfchar *s);

// ============================================================
extern ZF_ENV_EXPORT zfstring ZFImpl_sys_Android_stackTrace(void);
extern ZF_ENV_EXPORT void ZFImpl_sys_Android_stackTracePrint(void);
extern ZF_ENV_EXPORT void ZFImpl_sys_Android_objectInfoT(ZF_OUT zfstring &ret, ZF_IN jobject nativeObject);
inline zfstring ZFImpl_sys_Android_objectInfo(ZF_IN jobject nativeObject)
{
    zfstring ret;
    ZFImpl_sys_Android_objectInfoT(ret, nativeObject);
    return ret;
}

// ============================================================
// ZFAndroidBuffer
// buffer object used to access raw byte[] data from/to Java side
// see ZFAndroidBuffer.java for more info
zfclassPOD ZF_ENV_EXPORT ZFImpl_sys_Android_Buffer
{
public:
    zfbyte *buffer;
    zfindex bufferSize;
};
extern ZF_ENV_EXPORT const ZFImpl_sys_Android_Buffer ZFImpl_sys_Android_BufferZero;
#define ZFImpl_sys_Android_JNI_ID_ZFAndroidBuffer ZFImpl_sys_Android_JNI_ID(NativeUtil_ZFAndroidBuffer)
#define ZFImpl_sys_Android_JNI_NAME_ZFAndroidBuffer ZFImpl_sys_Android_JNI_NAME(NativeUtil.ZFAndroidBuffer)
extern ZF_ENV_EXPORT jclass ZFImpl_sys_Android_jclassZFAndroidBuffer(void);

extern ZF_ENV_EXPORT void ZFImpl_sys_Android_ZFAndroidBufferFromJava(ZF_OUT ZFImpl_sys_Android_Buffer &ret, ZF_IN jobject jobjBuffer);
inline ZFImpl_sys_Android_Buffer ZFImpl_sys_Android_ZFAndroidBufferFromJava(ZF_IN jobject jobjBuffer)
{
    ZFImpl_sys_Android_Buffer ret = ZFImpl_sys_Android_BufferZero;
    ZFImpl_sys_Android_ZFAndroidBufferFromJava(ret, jobjBuffer);
    return ret;
}
// note returned value must be released by DeleteLocalRef
extern ZF_ENV_EXPORT jobject ZFImpl_sys_Android_ZFAndroidBufferToJava(ZF_IN void *buffer, ZF_IN zfindex bufferSize);

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_Android
#endif // #ifndef _ZFI_ZFImpl_sys_Android_ZF_impl_h_

