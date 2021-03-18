/**
 * @file JNIUtil_MethodWrapper.h
 * @brief JNI utilities
 */

#ifndef _JNIUtil_MethodWrapper_h_
#define _JNIUtil_MethodWrapper_h_

#include "JNIUtil.h"

#if NEED_JNIUTIL

namespace JNIUtil {

// ============================================================
// expand actions
#ifndef JNIUtilWrap_Enable
    #define JNIUtilWrap_Enable 0
#endif
#ifndef JNIUtilWrap_Created
    #define JNIUtilWrap_Created(obj, callerFile, callerFunction, callerLine, actionName) obj
#endif
#ifndef JNIUtilWrap_Released
    #define JNIUtilWrap_Released(obj, callerFile, callerFunction, callerLine, actionName) obj
#endif
#ifndef JNIUtilWrap_FuncCalled
    #define JNIUtilWrap_FuncCalled(callerFile, callerFunction, callerLine, actionName) NULL
#endif
#ifndef JNIUtilWrap_CallbackCalled
    #define JNIUtilWrap_CallbackCalled(className, actionName)
#endif

// ============================================================
#if JNIUtilWrap_Enable
#undef JNI_METHOD_DECLARE_BEGIN
#define JNI_METHOD_DECLARE_BEGIN(OwnerClassId, ReturnType, MethodName, ...) \
    _JNI_METHOD_DECLARE_BEGIN(OwnerClassId, ReturnType, MethodName, ##__VA_ARGS__) \
    { \
        JNIUtilWrap_CallbackCalled(_JNIUtilMacro_toString(OwnerClassId), _JNIUtilMacro_toString(MethodName))
#undef JNI_METHOD_DECLARE_END
#define JNI_METHOD_DECLARE_END() \
    }

#undef JNIConvertPointerToJNIType
#define JNIConvertPointerToJNIType(jniEnv, p) JNIUtilWrap_Created(_JNIConvertPointerToJNIType(jniEnv, p), __FILE__, __FUNCTION__, __LINE__, "JNIConvertPointerToJNIType")

/** @cond ZFPrivateDoc */
namespace JNIUtilMethodWrapperPrivate {
    class _JNI_EXPORT JNIAutoDeleteHolder
    {
    public:
        JNIAutoDeleteHolder(JNIEnv *jniEnv,
                            jobject obj,
                            bool globalRef,
                            const char *callerFile,
                            const char *callerFunction,
                            int callerLine)
        : jniEnvSaved(jniEnv)
        , objSaved(obj)
        , globalRefSaved(globalRef)
        , callerFile(callerFile)
        , callerFunction(callerFunction)
        , callerLine(callerLine)
        {
        }
        ~JNIAutoDeleteHolder(void)
        {
            if(this->jniEnvSaved && this->objSaved)
            {
                if(this->globalRefSaved)
                {
                    JNIUtilWrap_Released(this->objSaved, this->callerFile, this->callerFunction, this->callerLine, "DeleteGlobalRef");
                    this->jniEnvSaved->DeleteGlobalRef(this->objSaved);
                }
                else
                {
                    JNIUtilWrap_Released(this->objSaved, this->callerFile, this->callerFunction, this->callerLine, "DeleteLocalRef");
                    this->jniEnvSaved->DeleteLocalRef(this->objSaved);
                }
            }
        }
    public:
        JNIEnv *jniEnvSaved;
        jobject objSaved;
        bool globalRefSaved;
        const char *callerFile;
        const char *callerFunction;
        int callerLine;
    };
} // namespace JNIUtilMethodWrapperPrivate
/** @endcond */

#undef JNILineDeleteLocalRef
#define JNILineDeleteLocalRef(obj_) \
    (JNIUtilMethodWrapperPrivate::JNIAutoDeleteHolder(JNIUtil::JNIGetJNIEnv(), obj_, false, __FILE__, __FUNCTION__, __LINE__).objSaved)
#undef JNILineDeleteLocalRefWithEnv
#define JNILineDeleteLocalRefWithEnv(obj_, jniEnv) \
    (JNIUtilMethodWrapperPrivate::JNIAutoDeleteHolder(jniEnv, obj_, false, __FILE__, __FUNCTION__, __LINE__).objSaved)
#undef JNILineDeleteGlobalRef
#define JNILineDeleteGlobalRef(obj_) \
    (JNIUtilMethodWrapperPrivate::JNIAutoDeleteHolder(JNIUtil::JNIGetJNIEnv(), obj_, true, __FILE__, __FUNCTION__, __LINE__).objSaved)
#undef JNILineDeleteGlobalRefWithEnv
#define JNILineDeleteGlobalRefWithEnv(obj_, jniEnv) \
    (JNIUtilMethodWrapperPrivate::JNIAutoDeleteHolder(jniEnv, obj_, true, __FILE__, __FUNCTION__, __LINE__).objSaved)

#undef JNIBlockedDeleteLocalRef
#define JNIBlockedDeleteLocalRef(obj_) \
    JNIUtilMethodWrapperPrivate::JNIAutoDeleteHolder _JNIUtil_uniqueName(jniObjCleaner)(JNIUtil::JNIGetJNIEnv(), obj_, false, __FILE__, __FUNCTION__, __LINE__)
#undef JNIBlockedDeleteLocalRefWithEnv
#define JNIBlockedDeleteLocalRefWithEnv(obj_, jniEnv) \
    JNIUtilMethodWrapperPrivate::JNIAutoDeleteHolder _JNIUtil_uniqueName(jniObjCleaner)(jniEnv, obj_, false, __FILE__, __FUNCTION__, __LINE__)
#undef JNIBlockedDeleteGlobalRef
#define JNIBlockedDeleteGlobalRef(obj_) \
    JNIUtilMethodWrapperPrivate::JNIAutoDeleteHolder _JNIUtil_uniqueName(jniObjCleaner)(JNIUtil::JNIGetJNIEnv(), obj_, true, __FILE__, __FUNCTION__, __LINE__)
#undef JNIBlockedDeleteGlobalRefWithEnv
#define JNIBlockedDeleteGlobalRefWithEnv(obj_, jniEnv) \
    JNIUtilMethodWrapperPrivate::JNIAutoDeleteHolder _JNIUtil_uniqueName(jniObjCleaner)(jniEnv, obj_, true, __FILE__, __FUNCTION__, __LINE__)
#endif // #if JNIUtilWrap_Enable

// ============================================================
#define _JNIUtilMacro_toString(e) #e
#define _JNIUtilWrapMacro_Created(func, jniEnv, ...) \
    JNIUtilWrap_Created( \
        (JNIUtilWrap_FuncCalled(__FILE__, __FUNCTION__, __LINE__, _JNIUtilMacro_toString(func)), \
            (jniEnv)->func(__VA_ARGS__)), \
        __FILE__, __FUNCTION__, __LINE__, _JNIUtilMacro_toString(func))
#define _JNIUtilWrapMacro_Released_0(func, jniEnv, obj, ...) \
    (JNIUtilWrap_FuncCalled(__FILE__, __FUNCTION__, __LINE__, _JNIUtilMacro_toString(func)), \
        (jniEnv)->func( \
            JNIUtilWrap_Released(obj, __FILE__, __FUNCTION__, __LINE__, _JNIUtilMacro_toString(func)), \
            ##__VA_ARGS__))
#define _JNIUtilWrapMacro_Released_1(func, jniEnv, param0, obj, ...) \
    (JNIUtilWrap_FuncCalled(__FILE__, __FUNCTION__, __LINE__, _JNIUtilMacro_toString(func)), \
        (jniEnv)->func( \
            param0, \
            JNIUtilWrap_Released(obj, __FILE__, __FUNCTION__, __LINE__, _JNIUtilMacro_toString(func)), \
            ##__VA_ARGS__))
#define _JNIUtilWrapMacro_Released_2(func, jniEnv, param0, param1, obj, ...) \
    (JNIUtilWrap_FuncCalled(__FILE__, __FUNCTION__, __LINE__, _JNIUtilMacro_toString(func)), \
        (jniEnv)->func( \
            param0, param1, \
            JNIUtilWrap_Released(obj, __FILE__, __FUNCTION__, __LINE__, _JNIUtilMacro_toString(func)), \
            ##__VA_ARGS__))
#define _JNIUtilWrapMacro_FuncCalled(func, jniEnv, ...) \
    (JNIUtilWrap_FuncCalled(__FILE__, __FUNCTION__, __LINE__, _JNIUtilMacro_toString(func)), \
        (jniEnv)->func(__VA_ARGS__))

// ============================================================
// wrapper methods
#ifndef JNIUtilGetVersion
#define JNIUtilGetVersion(jniEnv) \
    _JNIUtilWrapMacro_FuncCalled(GetVersion, jniEnv)
#endif

#ifndef JNIUtilDefineClass
#define JNIUtilDefineClass(jniEnv, name, loader, buf, bufLen) \
    _JNIUtilWrapMacro_Created(DefineClass, jniEnv, name, loader, buf, bufLen)
#endif

#ifndef JNIUtilFindClass
#define JNIUtilFindClass(jniEnv, name) \
    _JNIUtilWrapMacro_Created(FindClass, jniEnv, name)
#endif

#ifndef JNIUtilFromReflectedMethod
#define JNIUtilFromReflectedMethod(jniEnv, method) \
    _JNIUtilWrapMacro_FuncCalled(FromReflectedMethod, jniEnv, method)
#endif

#ifndef JNIUtilFromReflectedField
#define JNIUtilFromReflectedField(jniEnv, field) \
    _JNIUtilWrapMacro_FuncCalled(FromReflectedMethod, jniEnv, field)
#endif

#ifndef JNIUtilToReflectedMethod
#define JNIUtilToReflectedMethod(jniEnv, cls, methodID, isStatic) \
    _JNIUtilWrapMacro_Created(ToReflectMethod, jniEnv, cls, methodID, isStatic)
#endif

#ifndef JNIUtilGetSuperclass
#define JNIUtilGetSuperclass(jniEnv, clazz) \
    _JNIUtilWrapMacro_Created(GetSuperclass, jniEnv, clazz)
#endif

#ifndef JNIUtilIsAssignableFrom
#define JNIUtilIsAssignableFrom(jniEnv, clazz1, clazz2) \
    _JNIUtilWrapMacro_FuncCalled(IsAssignableFrom, jniEnv, clazz1, clazz2)
#endif

#ifndef JNIUtilToReflectedField
#define JNIUtilToReflectedField(jniEnv, cls, fieldID, isStatic) \
    _JNIUtilWrapMacro_Created(ToReflectedField, jniEnv, cls, fieldID, isStatic)
#endif

#ifndef JNIUtilThrow
#define JNIUtilThrow(jniEnv, obj) \
    _JNIUtilWrapMacro_FuncCalled(Throw, jniEnv, obj)
#endif

#ifndef JNIUtilThrowNew
#define JNIUtilThrowNew(jniEnv, clazz, message) \
    _JNIUtilWrapMacro_FuncCalled(ThrowNew, jniEnv, clazz, message)
#endif

#ifndef JNIUtilExceptionOccurred
#define JNIUtilExceptionOccurred(jniEnv) \
    _JNIUtilWrapMacro_FuncCalled(ExceptionOccurred, jniEnv)
#endif

#ifndef JNIUtilExceptionDescribe
#define JNIUtilExceptionDescribe(jniEnv) \
    _JNIUtilWrapMacro_FuncCalled(ExceptionDescribe, jniEnv)
#endif

#ifndef JNIUtilExceptionClear
#define JNIUtilExceptionClear(jniEnv) \
    _JNIUtilWrapMacro_FuncCalled(ExceptionClear, jniEnv)
#endif

#ifndef JNIUtilFatalError
#define JNIUtilFatalError(jniEnv, msg) \
    _JNIUtilWrapMacro_FuncCalled(FatalError, jniEnv, msg)
#endif

#ifndef JNIUtilPushLocalFrame
#define JNIUtilPushLocalFrame(jniEnv, capacity) \
    _JNIUtilWrapMacro_FuncCalled(PushLocalFrame, jniEnv, capacity)
#endif

#ifndef JNIUtilPopLocalFrame
#define JNIUtilPopLocalFrame(jniEnv, result) \
    _JNIUtilWrapMacro_Created(PopLocalFrame, jniEnv, result)
#endif

#ifndef JNIUtilNewGlobalRef
#define JNIUtilNewGlobalRef(jniEnv, obj) \
    _JNIUtilWrapMacro_Created(NewGlobalRef, jniEnv, obj)
#endif

#ifndef JNIUtilDeleteGlobalRef
#define JNIUtilDeleteGlobalRef(jniEnv, globalRef) \
    _JNIUtilWrapMacro_Released_0(DeleteGlobalRef, jniEnv, globalRef)
#endif

#ifndef JNIUtilDeleteLocalRef
#define JNIUtilDeleteLocalRef(jniEnv, localRef) \
    _JNIUtilWrapMacro_Released_0(DeleteLocalRef, jniEnv, localRef)
#endif

#ifndef JNIUtilIsSameObject
#define JNIUtilIsSameObject(jniEnv, ref1, ref2) \
    _JNIUtilWrapMacro_FuncCalled(IsSameObject, jniEnv, ref1, ref2)
#endif

#ifndef JNIUtilNewLocalRef
#define JNIUtilNewLocalRef(jniEnv, ref) \
    _JNIUtilWrapMacro_Created(NewLocalRef, jniEnv, ref)
#endif

#ifndef JNIUtilEnsureLocalCapacity
#define JNIUtilEnsureLocalCapacity(jniEnv, capacity) \
    _JNIUtilWrapMacro_FuncCalled(EnsureLocalCapacity, jniEnv, capacity)
#endif

#ifndef JNIUtilAllocObject
#define JNIUtilAllocObject(jniEnv, clazz) \
    _JNIUtilWrapMacro_Created(AllocObject, jniEnv, clazz)
#endif

#ifndef JNIUtilNewObject
#define JNIUtilNewObject(jniEnv, clazz, methodID, ...) \
    _JNIUtilWrapMacro_Created(NewObject, jniEnv, clazz, methodID, ##__VA_ARGS__)
#endif

#ifndef JNIUtilNewObjectV
#define JNIUtilNewObjectV(jniEnv, clazz, methodID, args) \
    _JNIUtilWrapMacro_Created(NewObjectV, jniEnv, clazz, methodID, args)
#endif

#ifndef JNIUtilNewObjectA
#define JNIUtilNewObjectA(jniEnv, clazz, methodID, args) \
    _JNIUtilWrapMacro_Created(NewObjectA, jniEnv, clazz, methodID, args)
#endif

#ifndef JNIUtilGetObjectClass
#define JNIUtilGetObjectClass(jniEnv, obj) \
    _JNIUtilWrapMacro_Created(GetObjectClass, jniEnv, obj)
#endif

#ifndef JNIUtilIsInstanceOf
#define JNIUtilIsInstanceOf(jniEnv, obj, clazz) \
    _JNIUtilWrapMacro_FuncCalled(IsInstanceOf, jniEnv, obj, clazz)
#endif

#ifndef JNIUtilGetMethodID
#define JNIUtilGetMethodID(jniEnv, clazz, name, sig) \
    _JNIUtilWrapMacro_FuncCalled(GetMethodID, jniEnv, clazz, name, sig)
#endif

#ifndef JNIUtilCallObjectMethod
#define JNIUtilCallObjectMethod(jniEnv, obj, methodID, ...) \
    _JNIUtilWrapMacro_Created(CallObjectMethod, jniEnv, obj, methodID, ##__VA_ARGS__)
#endif

#ifndef JNIUtilCallObjectMethodV
#define JNIUtilCallObjectMethodV(jniEnv, obj, methodID, args) \
    _JNIUtilWrapMacro_Created(CallObjectMethodV, jniEnv, obj, methodID, args)
#endif

#ifndef JNIUtilCallObjectMethodA
#define JNIUtilCallObjectMethodA(jniEnv, obj, methodID, args) \
    _JNIUtilWrapMacro_Created(CallObjectMethodA, jniEnv, obj, methodID, args)
#endif

#ifndef JNIUtilCallBooleanMethod
#define JNIUtilCallBooleanMethod(jniEnv, obj, methodID, ...) \
    _JNIUtilWrapMacro_FuncCalled(CallBooleanMethod, jniEnv, obj, methodID, ##__VA_ARGS__)
#endif

#ifndef JNIUtilCallBooleanMethodV
#define JNIUtilCallBooleanMethodV(jniEnv, obj, methodID, args) \
    _JNIUtilWrapMacro_FuncCalled(CallBooleanMethodV, jniEnv, obj, methodID, args)
#endif

#ifndef JNIUtilCallBooleanMethodA
#define JNIUtilCallBooleanMethodA(jniEnv, obj, methodID, args) \
    _JNIUtilWrapMacro_FuncCalled(CallBooleanMethodA, jniEnv, obj, methodID,  args)
#endif

#ifndef JNIUtilCallByteMethod
#define JNIUtilCallByteMethod(jniEnv, obj, methodID, ...) \
    _JNIUtilWrapMacro_FuncCalled(CallByteMethod, jniEnv, obj, methodID, ##__VA_ARGS__)
#endif

#ifndef JNIUtilCallByteMethodV
#define JNIUtilCallByteMethodV(jniEnv, obj, methodID, args) \
    _JNIUtilWrapMacro_FuncCalled(CallByteMethodV, jniEnv, obj, methodID, args)
#endif

#ifndef JNIUtilCallByteMethodA
#define JNIUtilCallByteMethodA(jniEnv, obj, methodID, args) \
    _JNIUtilWrapMacro_FuncCalled(CallByteMethodA, jniEnv, obj, methodID, args)
#endif

#ifndef JNIUtilCallCharMethod
#define JNIUtilCallCharMethod(jniEnv, obj, methodID, ...) \
    _JNIUtilWrapMacro_FuncCalled(CallCharMethod, jniEnv, obj, methodID, ##__VA_ARGS__)
#endif

#ifndef JNIUtilCallCharMethodV
#define JNIUtilCallCharMethodV(jniEnv, obj, methodID, args) \
    _JNIUtilWrapMacro_FuncCalled(CallCharMethodV, jniEnv, obj, methodID, args)
#endif

#ifndef JNIUtilCallCharMethodA
#define JNIUtilCallCharMethodA(jniEnv, obj, methodID, args) \
    _JNIUtilWrapMacro_FuncCalled(CallCharMethodA, jniEnv, obj, methodID, args)
#endif

#ifndef JNIUtilCallShortMethod
#define JNIUtilCallShortMethod(jniEnv, obj, methodID, ...) \
    _JNIUtilWrapMacro_FuncCalled(CallShortMethod, jniEnv, obj, methodID, ##__VA_ARGS__)
#endif

#ifndef JNIUtilCallShortMethodV
#define JNIUtilCallShortMethodV(jniEnv, obj, methodID, args) \
    _JNIUtilWrapMacro_FuncCalled(CallShortMethodV, jniEnv, obj, methodID, args)
#endif

#ifndef JNIUtilCallShortMethodA
#define JNIUtilCallShortMethodA(jniEnv, obj, methodID, args) \
    _JNIUtilWrapMacro_FuncCalled(CallShortMethodA, jniEnv, obj, methodID, args)
#endif

#ifndef JNIUtilCallIntMethod
#define JNIUtilCallIntMethod(jniEnv, obj, methodID, ...) \
    _JNIUtilWrapMacro_FuncCalled(CallIntMethod, jniEnv, obj, methodID, ##__VA_ARGS__)
#endif

#ifndef JNIUtilCallIntMethodV
#define JNIUtilCallIntMethodV(jniEnv, obj, methodID, args) \
    _JNIUtilWrapMacro_FuncCalled(CallIntMethodV, jniEnv, obj, methodId, args)
#endif

#ifndef JNIUtilCallIntMethodA
#define JNIUtilCallIntMethodA(jniEnv, obj, methodID, args) \
    _JNIUtilWrapMacro_FuncCalled(CallIntMethodA, jniEnv, methodID, args)
#endif

#ifndef JNIUtilCallLongMethod
#define JNIUtilCallLongMethod(jniEnv, obj, methodID, ...) \
    _JNIUtilWrapMacro_FuncCalled(CallLongMethod, jniEnv, obj, methodID, ##__VA_ARGS__)
#endif

#ifndef JNIUtilCallLongMethodV
#define JNIUtilCallLongMethodV(jniEnv, obj, methodID, args) \
    _JNIUtilWrapMacro_FuncCalled(CallLongMethodV, jniEnv, obj, methodID, args)
#endif

#ifndef JNIUtilCallLongMethodA
#define JNIUtilCallLongMethodA(jniEnv, obj, methodID, args) \
    _JNIUtilWrapMacro_FuncCalled(CallLongMethodVA, jniEnv, obj, methodID, args)
#endif

#ifndef JNIUtilCallFloatMethod
#define JNIUtilCallFloatMethod(jniEnv, obj, methodID, ...) \
    _JNIUtilWrapMacro_FuncCalled(CallFloatMethod, jniEnv, obj, methodID, ##__VA_ARGS__)
#endif

#ifndef JNIUtilCallFloatMethodV
#define JNIUtilCallFloatMethodV(jniEnv, obj, methodID, args) \
    _JNIUtilWrapMacro_FuncCalled(CallFloatMethodV, jniEnv, obj, methodID, args)
#endif

#ifndef JNIUtilCallFloatMethodA
#define JNIUtilCallFloatMethodA(jniEnv, obj, methodID, args) \
    _JNIUtilWrapMacro_FuncCalled(CallFloatMethodA, jniEnv, obj, methodID, args)
#endif

#ifndef JNIUtilCallDoubleMethod
#define JNIUtilCallDoubleMethod(jniEnv, obj, methodID, ...) \
    _JNIUtilWrapMacro_FuncCalled(CallDoubleMethod, jniEnv, obj, methodID, ##__VA_ARGS__)
#endif

#ifndef JNIUtilCallDoubleMethodV
#define JNIUtilCallDoubleMethodV(jniEnv, obj, methodID, args) \
    _JNIUtilWrapMacro_FuncCalled(CallDoubleMethodV, jniEnv, obj, methodID, args)
#endif

#ifndef JNIUtilCallDoubleMethodA
#define JNIUtilCallDoubleMethodA(jniEnv, obj, methodID, args) \
    _JNIUtilWrapMacro_FuncCalled(CallDoubleMethodA, jniEnv, obj, methodID, args)
#endif

#ifndef JNIUtilCallVoidMethod
#define JNIUtilCallVoidMethod(jniEnv, obj, methodID, ...) \
    _JNIUtilWrapMacro_FuncCalled(CallVoidMethod, jniEnv, obj, methodID, ##__VA_ARGS__)
#endif

#ifndef JNIUtilCallVoidMethodV
#define JNIUtilCallVoidMethodV(jniEnv, obj, methodID, args) \
    _JNIUtilWrapMacro_FuncCalled(CallVoidMethodV, jniEnv, obj, methodID, args)
#endif

#ifndef JNIUtilCallVoidMethodA
#define JNIUtilCallVoidMethodA(jniEnv, obj, methodID, args) \
    _JNIUtilWrapMacro_FuncCalled(CallVoidMethodA, jniEnv, obj, methodID, args)
#endif

#ifndef JNIUtilCallNonvirtualObjectMethod
#define JNIUtilCallNonvirtualObjectMethod(jniEnv, obj, methodID, ...) \
    _JNIUtilWrapMacro_Created(CallNonvirtualObjectMethod, jniEnv, obj, methodID, ##__VA_ARGS__)
#endif

#ifndef JNIUtilCallNonvirtualObjectMethodV
#define JNIUtilCallNonvirtualObjectMethodV(jniEnv, obj, methodID, args) \
    _JNIUtilWrapMacro_Created(CallNonvirtualObjectMethodV, jniEnv, obj, methodID, args)
#endif

#ifndef JNIUtilCallNonvirtualObjectMethodA
#define JNIUtilCallNonvirtualObjectMethodA(jniEnv, obj, methodID, args) \
    _JNIUtilWrapMacro_Created(CallNonvirtualObjectMethodA, jniEnv, obj, methodID, args)
#endif

#ifndef JNIUtilCallNonvirtualBooleanMethod
#define JNIUtilCallNonvirtualBooleanMethod(jniEnv, obj, methodID, ...) \
    _JNIUtilWrapMacro_FuncCalled(CallNonvirtualBooleanMethod, jniEnv, obj, methodID, ##__VA_ARGS__)
#endif

#ifndef JNIUtilCallNonvirtualBooleanMethodV
#define JNIUtilCallNonvirtualBooleanMethodV(jniEnv, obj, methodID, args) \
    _JNIUtilWrapMacro_FuncCalled(CallNonvirtualBooleanMethodV, jniEnv, obj, methodID, args)
#endif

#ifndef JNIUtilCallNonvirtualBooleanMethodA
#define JNIUtilCallNonvirtualBooleanMethodA(jniEnv, obj, methodID, args) \
    _JNIUtilWrapMacro_FuncCalled(CallNonvirtualBooleanMethodA, jniEnv, obj, methodID, args)
#endif

#ifndef JNIUtilCallNonvirtualByteMethod
#define JNIUtilCallNonvirtualByteMethod(jniEnv, obj, methodID, ...) \
    _JNIUtilWrapMacro_FuncCalled(CallNonvirtualByteMethod, jniEnv, obj, methodID, ##__VA_ARGS__)
#endif

#ifndef JNIUtilCallNonvirtualByteMethodV
#define JNIUtilCallNonvirtualByteMethodV(jniEnv, obj, methodID, args) \
    _JNIUtilWrapMacro_FuncCalled(CallNonvirtualByteMethodV, jniEnv, obj, methodID, args)
#endif

#ifndef JNIUtilCallNonvirtualByteMethodA
#define JNIUtilCallNonvirtualByteMethodA(jniEnv, obj, methodID, args) \
    _JNIUtilWrapMacro_FuncCalled(CallNonvirtualByteMethodA, jniEnv, obj, methodID, args)
#endif

#ifndef JNIUtilCallNonvirtualCharMethod
#define JNIUtilCallNonvirtualCharMethod(jniEnv, obj, methodID, ...) \
    _JNIUtilWrapMacro_FuncCalled(CallNonvirtualCharMethod, jniEnv, obj, methodID, ##__VA_ARGS__)
#endif

#ifndef JNIUtilCallNonvirtualCharMethodV
#define JNIUtilCallNonvirtualCharMethodV(jniEnv, obj, methodID, args) \
    _JNIUtilWrapMacro_FuncCalled(CallNonvirtualCharMethodV, jniEnv, obj, methodID, args)
#endif

#ifndef JNIUtilCallNonvirtualCharMethodA
#define JNIUtilCallNonvirtualCharMethodA(jniEnv, obj, methodID, args) \
    _JNIUtilWrapMacro_FuncCalled(CallNonvirtualCharMethodA, jniEnv, obj, methodID, args)
#endif

#ifndef JNIUtilCallNonvirtualShortMethod
#define JNIUtilCallNonvirtualShortMethod(jniEnv, obj, methodID, ...) \
    _JNIUtilWrapMacro_FuncCalled(CallNonvirtualShortMethod, jniEnv, obj, methodID, ##__VA_ARGS__)
#endif

#ifndef JNIUtilCallNonvirtualShortMethodV
#define JNIUtilCallNonvirtualShortMethodV(jniEnv, obj, methodID, args) \
    _JNIUtilWrapMacro_FuncCalled(CallNonvirtualShortMethodV, jniEnv, obj, methodID, args)
#endif

#ifndef JNIUtilCallNonvirtualShortMethodA
#define JNIUtilCallNonvirtualShortMethodA(jniEnv, obj, methodID, args) \
    _JNIUtilWrapMacro_FuncCalled(CallNonvirtualShortMethodA, jniEnv, obj, methodID, args)
#endif

#ifndef JNIUtilCallNonvirtualIntMethod
#define JNIUtilCallNonvirtualIntMethod(jniEnv, obj, methodID, ...) \
    _JNIUtilWrapMacro_FuncCalled(CallNonvirtualIntMethod, jniEnv, obj, methodID, ##__VA_ARGS__)
#endif

#ifndef JNIUtilCallNonvirtualIntMethodV
#define JNIUtilCallNonvirtualIntMethodV(jniEnv, obj, methodID, args) \
    _JNIUtilWrapMacro_FuncCalled(CallNonvirtualIntMethodV, jniEnv, obj, methodID, args)
#endif

#ifndef JNIUtilCallNonvirtualIntMethodA
#define JNIUtilCallNonvirtualIntMethodA(jniEnv, obj, methodID, args) \
    _JNIUtilWrapMacro_FuncCalled(CallNonvirtualIntMethodA, jniEnv, obj, methodID, args)
#endif

#ifndef JNIUtilCallNonvirtualLongMethod
#define JNIUtilCallNonvirtualLongMethod(jniEnv, obj, methodID, ...) \
    _JNIUtilWrapMacro_FuncCalled(CallNonvirtualLongMethod, jniEnv, obj, methodID, ##__VA_ARGS__)
#endif

#ifndef JNIUtilCallNonvirtualLongMethodV
#define JNIUtilCallNonvirtualLongMethodV(jniEnv, obj, methodID, args) \
    _JNIUtilWrapMacro_FuncCalled(CallNonvirtualLongMethodV, jniEnv, obj, methodID, args)
#endif

#ifndef JNIUtilCallNonvirtualLongMethodA
#define JNIUtilCallNonvirtualLongMethodA(jniEnv, obj, methodID, args) \
    _JNIUtilWrapMacro_FuncCalled(CallNonvirtualLongMethodA, jniEnv, obj, methodID, args)
#endif

#ifndef JNIUtilCallNonvirtualFloatMethod
#define JNIUtilCallNonvirtualFloatMethod(jniEnv, obj, methodID, ...) \
    _JNIUtilWrapMacro_FuncCalled(CallNonvirtualFloatMethod, jniEnv, obj, methodID, ##__VA_ARGS__)
#endif

#ifndef JNIUtilCallNonvirtualFloatMethodV
#define JNIUtilCallNonvirtualFloatMethodV(jniEnv, obj, methodID, args) \
    _JNIUtilWrapMacro_FuncCalled(CallNonvirtualFloatMethodV, jniEnv, obj, methodID, args)
#endif

#ifndef JNIUtilCallNonvirtualFloatMethodA
#define JNIUtilCallNonvirtualFloatMethodA(jniEnv, obj, methodID, args) \
    _JNIUtilWrapMacro_FuncCalled(CallNonvirtualFloatMethodA, jniEnv, obj, methodID, args)
#endif

#ifndef JNIUtilCallNonvirtualDoubleMethod
#define JNIUtilCallNonvirtualDoubleMethod(jniEnv, obj, methodID, ...) \
    _JNIUtilWrapMacro_FuncCalled(CallNonvirtualDoubleMethod, jniEnv, obj, methodID, ##__VA_ARGS__)
#endif

#ifndef JNIUtilCallNonvirtualDoubleMethodV
#define JNIUtilCallNonvirtualDoubleMethodV(jniEnv, obj, methodID, args) \
    _JNIUtilWrapMacro_FuncCalled(CallNonvirtualDoubleMethodV, jniEnv, obj, methodID, args)
#endif

#ifndef JNIUtilCallNonvirtualDoubleMethodA
#define JNIUtilCallNonvirtualDoubleMethodA(jniEnv, obj, methodID, args) \
    _JNIUtilWrapMacro_FuncCalled(CallNonvirtualDoubleMethodA, jniEnv, obj, methodID, args)
#endif

#ifndef JNIUtilCallNonvirtualVoidMethod
#define JNIUtilCallNonvirtualVoidMethod(jniEnv, obj, methodID, ...) \
    _JNIUtilWrapMacro_FuncCalled(CallNonvirtualVoidMethod, jniEnv, obj, methodID, ##__VA_ARGS__)
#endif

#ifndef JNIUtilCallNonvirtualVoidMethodV
#define JNIUtilCallNonvirtualVoidMethodV(jniEnv, obj, methodID, args) \
    _JNIUtilWrapMacro_FuncCalled(CallNonvirtualVoidMethodV, jniEnv, obj, methodID, args)
#endif

#ifndef JNIUtilCallNonvirtualVoidMethodA
#define JNIUtilCallNonvirtualVoidMethodA(jniEnv, obj, methodID, args) \
    _JNIUtilWrapMacro_FuncCalled(CallNonvirtualVoidMethodA, jniEnv, obj, methodID, args)
#endif

#ifndef JNIUtilGetFieldID
#define JNIUtilGetFieldID(jniEnv, clazz, name, sig) \
    _JNIUtilWrapMacro_FuncCalled(GetFieldID, jniEnv, clazz, name, sig)
#endif

#ifndef JNIUtilGetObjectField
#define JNIUtilGetObjectField(jniEnv, obj, fieldID) \
    _JNIUtilWrapMacro_Created(GetObjectField, jniEnv, obj, fieldID)
#endif

#ifndef JNIUtilGetBooleanField
#define JNIUtilGetBooleanField(jniEnv, obj, fieldID) \
    _JNIUtilWrapMacro_FuncCalled(GetBooleanField, jniEnv, obj, fieldID)
#endif

#ifndef JNIUtilGetByteField
#define JNIUtilGetByteField(jniEnv, obj, fieldID) \
    _JNIUtilWrapMacro_FuncCalled(GetByteField, jniEnv, obj, fieldID)
#endif

#ifndef JNIUtilGetCharField
#define JNIUtilGetCharField(jniEnv, obj, fieldID) \
    _JNIUtilWrapMacro_FuncCalled(GetCharField, jniEnv, obj, fieldID)
#endif

#ifndef JNIUtilGetShortField
#define JNIUtilGetShortField(jniEnv, obj, fieldID) \
    _JNIUtilWrapMacro_FuncCalled(GetShortField, jniEnv, obj, fieldID)
#endif

#ifndef JNIUtilGetIntField
#define JNIUtilGetIntField(jniEnv, obj, fieldID) \
    _JNIUtilWrapMacro_FuncCalled(GetIntField, jniEnv, obj, fieldID)
#endif

#ifndef JNIUtilGetLongField
#define JNIUtilGetLongField(jniEnv, obj, fieldID) \
    _JNIUtilWrapMacro_FuncCalled(GetLongField, jniEnv, obj, fieldID)
#endif

#ifndef JNIUtilGetFloatField
#define JNIUtilGetFloatField(jniEnv, obj, fieldID) \
    _JNIUtilWrapMacro_FuncCalled(GetFloatField, jniEnv, obj, fieldID)
#endif

#ifndef JNIUtilGetDoubleField
#define JNIUtilGetDoubleField(jniEnv, obj, fieldID) \
    _JNIUtilWrapMacro_FuncCalled(GetDoubleField, jniEnv, obj, fieldID)
#endif

#ifndef JNIUtilSetObjectField
#define JNIUtilSetObjectField(jniEnv, obj, fieldID, value) \
    _JNIUtilWrapMacro_FuncCalled(SetObjectField, jniEnv, obj, fieldID, value)
#endif

#ifndef JNIUtilSetBooleanField
#define JNIUtilSetBooleanField(jniEnv, obj, fieldID, value) \
    _JNIUtilWrapMacro_FuncCalled(SetBooleanField, jniEnv, obj, fieldID, value)
#endif

#ifndef JNIUtilSetByteField
#define JNIUtilSetByteField(jniEnv, obj, fieldID, value) \
    _JNIUtilWrapMacro_FuncCalled(SetByteField, jniEnv, obj, fieldID, value)
#endif

#ifndef JNIUtilSetCharField
#define JNIUtilSetCharField(jniEnv, obj, fieldID, value) \
    _JNIUtilWrapMacro_FuncCalled(SetCharField, jniEnv, obj, fieldID, value)
#endif

#ifndef JNIUtilSetShortField
#define JNIUtilSetShortField(jniEnv, obj, fieldID, value) \
    _JNIUtilWrapMacro_FuncCalled(SetShortField, jniEnv, obj, fieldID, value)
#endif

#ifndef JNIUtilSetIntField
#define JNIUtilSetIntField(jniEnv, obj, fieldID, value) \
    _JNIUtilWrapMacro_FuncCalled(SetIntField, jniEnv, obj, fieldID, value)
#endif

#ifndef JNIUtilSetLongField
#define JNIUtilSetLongField(jniEnv, obj, fieldID, value) \
    _JNIUtilWrapMacro_FuncCalled(SetLongField, jniEnv, obj, fieldID, value)
#endif

#ifndef JNIUtilSetFloatField
#define JNIUtilSetFloatField(jniEnv, obj, fieldID, value) \
    _JNIUtilWrapMacro_FuncCalled(SetFloatField, jniEnv, obj, fieldID, value)
#endif

#ifndef JNIUtilSetDoubleField
#define JNIUtilSetDoubleField(jniEnv, obj, fieldID, value) \
    _JNIUtilWrapMacro_FuncCalled(SetDoubleField, jniEnv, obj, fieldID, value)
#endif

#ifndef JNIUtilGetStaticMethodID
#define JNIUtilGetStaticMethodID(jniEnv, clazz, name, sig) \
    _JNIUtilWrapMacro_FuncCalled(GetStaticMethodID, jniEnv, clazz, name, sig)
#endif

#ifndef JNIUtilCallStaticObjectMethod
#define JNIUtilCallStaticObjectMethod(jniEnv, clazz, methodID, ...) \
    _JNIUtilWrapMacro_Created(CallStaticObjectMethod, jniEnv, clazz, methodID, ##__VA_ARGS__)
#endif

#ifndef JNIUtilCallStaticObjectMethodV
#define JNIUtilCallStaticObjectMethodV(jniEnv, clazz, methodID, args) \
    _JNIUtilWrapMacro_Created(CallStaticObjectMethodV, jniEnv, clazz, methodID, args)
#endif

#ifndef JNIUtilCallStaticObjectMethodA
#define JNIUtilCallStaticObjectMethodA(jniEnv, clazz, methodID, args) \
    _JNIUtilWrapMacro_Created(CallStaticObjectMethodA, jniEnv, clazz, methodID, args)
#endif

#ifndef JNIUtilCallStaticBooleanMethod
#define JNIUtilCallStaticBooleanMethod(jniEnv, clazz, methodID, ...) \
    _JNIUtilWrapMacro_FuncCalled(CallStaticBooleanMethod, jniEnv, clazz, methodID, ##__VA_ARGS__)
#endif

#ifndef JNIUtilCallStaticBooleanMethodV
#define JNIUtilCallStaticBooleanMethodV(jniEnv, clazz, methodID, args) \
    _JNIUtilWrapMacro_FuncCalled(CallStaticBooleanMethodV, jniEnv, clazz, methodID, args)
#endif

#ifndef JNIUtilCallStaticBooleanMethodA
#define JNIUtilCallStaticBooleanMethodA(jniEnv, clazz, methodID, args) \
    _JNIUtilWrapMacro_FuncCalled(CallStaticBooleanMethodA, jniEnv, clazz, methodID, args)
#endif

#ifndef JNIUtilCallStaticByteMethod
#define JNIUtilCallStaticByteMethod(jniEnv, clazz, methodID, ...) \
    _JNIUtilWrapMacro_FuncCalled(CallStaticByteMethod, jniEnv, clazz, methodID, ##__VA_ARGS__)
#endif

#ifndef JNIUtilCallStaticByteMethodV
#define JNIUtilCallStaticByteMethodV(jniEnv, clazz, methodID, args) \
    _JNIUtilWrapMacro_FuncCalled(CallStaticByteMethodV, jniEnv, clazz, methodID, args)
#endif

#ifndef JNIUtilCallStaticByteMethodA
#define JNIUtilCallStaticByteMethodA(jniEnv, clazz, methodID, args) \
    _JNIUtilWrapMacro_FuncCalled(CallStaticByteMethodA, jniEnv, clazz, methodID, args)
#endif

#ifndef JNIUtilCallStaticCharMethod
#define JNIUtilCallStaticCharMethod(jniEnv, clazz, methodID, ...) \
    _JNIUtilWrapMacro_FuncCalled(CallStaticCharMethod, jniEnv, clazz, methodID, ##__VA_ARGS__)
#endif

#ifndef JNIUtilCallStaticCharMethodV
#define JNIUtilCallStaticCharMethodV(jniEnv, clazz, methodID, args) \
    _JNIUtilWrapMacro_FuncCalled(CallStaticCharMethodV, jniEnv, clazz, methodID, args)
#endif

#ifndef JNIUtilCallStaticCharMethodA
#define JNIUtilCallStaticCharMethodA(jniEnv, clazz, methodID, args) \
    _JNIUtilWrapMacro_FuncCalled(CallStaticCharMethodA, jniEnv, clazz, methodID, args)
#endif

#ifndef JNIUtilCallStaticShortMethod
#define JNIUtilCallStaticShortMethod(jniEnv, clazz, methodID, ...) \
    _JNIUtilWrapMacro_FuncCalled(CallStaticShortMethod, jniEnv, clazz, methodID, ##__VA_ARGS__)
#endif

#ifndef JNIUtilCallStaticShortMethodV
#define JNIUtilCallStaticShortMethodV(jniEnv, clazz, methodID, args) \
    _JNIUtilWrapMacro_FuncCalled(CallStaticShortMethodV, jniEnv, clazz, methodID, args)
#endif

#ifndef JNIUtilCallStaticShortMethodA
#define JNIUtilCallStaticShortMethodA(jniEnv, clazz, methodID, args) \
    _JNIUtilWrapMacro_FuncCalled(CallStaticShortMethodA, jniEnv, clazz, methodID, args)
#endif

#ifndef JNIUtilCallStaticIntMethod
#define JNIUtilCallStaticIntMethod(jniEnv, clazz, methodID, ...) \
    _JNIUtilWrapMacro_FuncCalled(CallStaticIntMethod, jniEnv, clazz, methodID, ##__VA_ARGS__)
#endif

#ifndef JNIUtilCallStaticIntMethodV
#define JNIUtilCallStaticIntMethodV(jniEnv, clazz, methodID, args) \
    _JNIUtilWrapMacro_FuncCalled(CallStaticIntMethodV, jniEnv, clazz, methodID, args)
#endif

#ifndef JNIUtilCallStaticIntMethodA
#define JNIUtilCallStaticIntMethodA(jniEnv, clazz, methodID, args) \
    _JNIUtilWrapMacro_FuncCalled(CallStaticIntMethodA, jniEnv, clazz, methodID, args)
#endif

#ifndef JNIUtilCallStaticLongMethod
#define JNIUtilCallStaticLongMethod(jniEnv, clazz, methodID, ...) \
    _JNIUtilWrapMacro_FuncCalled(CallStaticLongMethod, jniEnv, clazz, methodID, ##__VA_ARGS__)
#endif

#ifndef JNIUtilCallStaticLongMethodV
#define JNIUtilCallStaticLongMethodV(jniEnv, clazz, methodID, args) \
    _JNIUtilWrapMacro_FuncCalled(CallStaticLongMethodV, jniEnv, clazz, methodID, args)
#endif

#ifndef JNIUtilCallStaticLongMethodA
#define JNIUtilCallStaticLongMethodA(jniEnv, clazz, methodID, args) \
    _JNIUtilWrapMacro_FuncCalled(CallStaticLongMethodA, jniEnv, clazz, methodID, args)
#endif

#ifndef JNIUtilCallStaticFloatMethod
#define JNIUtilCallStaticFloatMethod(jniEnv, clazz, methodID, ...) \
    _JNIUtilWrapMacro_FuncCalled(CallStaticFloatMethod, jniEnv, clazz, methodID, ##__VA_ARGS__)
#endif

#ifndef JNIUtilCallStaticFloatMethodV
#define JNIUtilCallStaticFloatMethodV(jniEnv, clazz, methodID, args) \
    _JNIUtilWrapMacro_FuncCalled(CallStaticFloatMethodV, jniEnv, clazz, methodID, args)
#endif

#ifndef JNIUtilCallStaticFloatMethodA
#define JNIUtilCallStaticFloatMethodA(jniEnv, clazz, methodID, args) \
    _JNIUtilWrapMacro_FuncCalled(CallStaticFloatMethodA, jniEnv, clazz, methodID, args)
#endif

#ifndef JNIUtilCallStaticDoubleMethod
#define JNIUtilCallStaticDoubleMethod(jniEnv, clazz, methodID, ...) \
    _JNIUtilWrapMacro_FuncCalled(CallStaticDoubleMethod, jniEnv, clazz, methodID, ##__VA_ARGS__)
#endif

#ifndef JNIUtilCallStaticDoubleMethodV
#define JNIUtilCallStaticDoubleMethodV(jniEnv, clazz, methodID, args) \
    _JNIUtilWrapMacro_FuncCalled(CallStaticDoubleMethodV, jniEnv, clazz, methodID, args)
#endif

#ifndef JNIUtilCallStaticDoubleMethodA
#define JNIUtilCallStaticDoubleMethodA(jniEnv, clazz, methodID, args) \
    _JNIUtilWrapMacro_FuncCalled(CallStaticDoubleMethodA, jniEnv, clazz, methodID, args)
#endif

#ifndef JNIUtilCallStaticVoidMethod
#define JNIUtilCallStaticVoidMethod(jniEnv, clazz, methodID, ...) \
    _JNIUtilWrapMacro_FuncCalled(CallStaticVoidMethod, jniEnv, clazz, methodID, ##__VA_ARGS__)
#endif

#ifndef JNIUtilCallStaticVoidMethodV
#define JNIUtilCallStaticVoidMethodV(jniEnv, clazz, methodID, args) \
    _JNIUtilWrapMacro_FuncCalled(CallStaticVoidMethodV, jniEnv, clazz, methodID, args)
#endif

#ifndef JNIUtilCallStaticVoidMethodA
#define JNIUtilCallStaticVoidMethodA(jniEnv, clazz, methodID, args) \
    _JNIUtilWrapMacro_FuncCalled(CallStaticVoidMethodA, jniEnv, clazz, methodID, args)
#endif

#ifndef JNIUtilGetStaticFieldID
#define JNIUtilGetStaticFieldID(jniEnv, clazz, name, sig) \
    _JNIUtilWrapMacro_FuncCalled(GetStaticFieldID, jniEnv, clazz, name, sig)
#endif

#ifndef JNIUtilGetStaticObjectField
#define JNIUtilGetStaticObjectField(jniEnv, clazz, fieldID) \
    _JNIUtilWrapMacro_Created(GetStaticObjectField, jniEnv, clazz, fieldID)
#endif

#ifndef JNIUtilGetStaticBooleanField
#define JNIUtilGetStaticBooleanField(jniEnv, clazz, fieldID) \
    _JNIUtilWrapMacro_FuncCalled(GetStaticBooleanField, jniEnv, clazz, fieldID)
#endif

#ifndef JNIUtilGetStaticByteField
#define JNIUtilGetStaticByteField(jniEnv, clazz, fieldID) \
    _JNIUtilWrapMacro_FuncCalled(GetStaticByteField, jniEnv, clazz, fieldID)
#endif

#ifndef JNIUtilGetStaticCharField
#define JNIUtilGetStaticCharField(jniEnv, clazz, fieldID) \
    _JNIUtilWrapMacro_FuncCalled(GetStaticCharField, jniEnv, clazz, fieldID)
#endif

#ifndef JNIUtilGetStaticShortField
#define JNIUtilGetStaticShortField(jniEnv, clazz, fieldID) \
    _JNIUtilWrapMacro_FuncCalled(GetStaticShortField, jniEnv, clazz, fieldID)
#endif

#ifndef JNIUtilGetStaticIntField
#define JNIUtilGetStaticIntField(jniEnv, clazz, fieldID) \
    _JNIUtilWrapMacro_FuncCalled(GetStaticIntField, jniEnv, clazz, fieldID)
#endif

#ifndef JNIUtilGetStaticLongField
#define JNIUtilGetStaticLongField(jniEnv, clazz, fieldID) \
    _JNIUtilWrapMacro_FuncCalled(GetStaticLongField, jniEnv, clazz, fieldID)
#endif

#ifndef JNIUtilGetStaticFloatField
#define JNIUtilGetStaticFloatField(jniEnv, clazz, fieldID) \
    _JNIUtilWrapMacro_FuncCalled(GetStaticFloatField, jniEnv, clazz, fieldID)
#endif

#ifndef JNIUtilGetStaticDoubleField
#define JNIUtilGetStaticDoubleField(jniEnv, clazz, fieldID) \
    _JNIUtilWrapMacro_FuncCalled(GetStaticDoubleField, jniEnv, clazz, fieldID)
#endif

#ifndef JNIUtilSetStaticObjectField
#define JNIUtilSetStaticObjectField(jniEnv, clazz, fieldID, value) \
    _JNIUtilWrapMacro_FuncCalled(SetStaticObjectField, jniEnv, clazz, fieldID, value)
#endif

#ifndef JNIUtilSetStaticBooleanField
#define JNIUtilSetStaticBooleanField(jniEnv, clazz, fieldID, value) \
    _JNIUtilWrapMacro_FuncCalled(SetStaticBooleanField, jniEnv, clazz, fieldID, value)
#endif

#ifndef JNIUtilSetStaticByteField
#define JNIUtilSetStaticByteField(jniEnv, clazz, fieldID, value) \
    _JNIUtilWrapMacro_FuncCalled(SetStaticByteField, jniEnv, clazz, fieldID, value)
#endif

#ifndef JNIUtilSetStaticCharField
#define JNIUtilSetStaticCharField(jniEnv, clazz, fieldID, value) \
    _JNIUtilWrapMacro_FuncCalled(SetStaticCharField, jniEnv, clazz, fieldID, value)
#endif

#ifndef JNIUtilSetStaticShortField
#define JNIUtilSetStaticShortField(jniEnv, clazz, fieldID, value) \
    _JNIUtilWrapMacro_FuncCalled(SetStaticShortField, jniEnv, clazz, fieldID, value)
#endif

#ifndef JNIUtilSetStaticIntField
#define JNIUtilSetStaticIntField(jniEnv, clazz, fieldID, value) \
    _JNIUtilWrapMacro_FuncCalled(SetStaticIntField, jniEnv, clazz, fieldID, value)
#endif

#ifndef JNIUtilSetStaticLongField
#define JNIUtilSetStaticLongField(jniEnv, clazz, fieldID, value) \
    _JNIUtilWrapMacro_FuncCalled(SetStaticLongField, jniEnv, clazz, fieldID, value)
#endif

#ifndef JNIUtilSetStaticFloatField
#define JNIUtilSetStaticFloatField(jniEnv, clazz, fieldID, value) \
    _JNIUtilWrapMacro_FuncCalled(SetStaticFloatField, jniEnv, clazz, fieldID, value)
#endif

#ifndef JNIUtilSetStaticDoubleField
#define JNIUtilSetStaticDoubleField(jniEnv, clazz, fieldID, value) \
    _JNIUtilWrapMacro_FuncCalled(SetStaticDoubleField, jniEnv, clazz, fieldID, value)
#endif

#ifndef JNIUtilNewString
#define JNIUtilNewString(jniEnv, unicodeChars, len) \
    _JNIUtilWrapMacro_Created(NewString, jniEnv, unicodeChars, len)
#endif

#ifndef JNIUtilGetStringLength
#define JNIUtilGetStringLength(jniEnv, string) \
    _JNIUtilWrapMacro_FuncCalled(GetStringLength, jniEnv, string)
#endif

#ifndef JNIUtilGetStringChars
#define JNIUtilGetStringChars(jniEnv, string, isCopy) \
    _JNIUtilWrapMacro_Created(GetStringChars, jniEnv, string, isCopy)
#endif

#ifndef JNIUtilReleaseStringChars
#define JNIUtilReleaseStringChars(jniEnv, string, chars) \
    _JNIUtilWrapMacro_Released_1(ReleaseStringChars, jniEnv, string, chars)
#endif

#ifndef JNIUtilNewStringUTF
#define JNIUtilNewStringUTF(jniEnv, bytes) \
    _JNIUtilWrapMacro_Created(NewStringUTF, jniEnv, bytes)
#endif

#ifndef JNIUtilGetStringUTFLength
#define JNIUtilGetStringUTFLength(jniEnv, string) \
    _JNIUtilWrapMacro_FuncCalled(GetStringUTFLength, jniEnv, bytes)
#endif

#ifndef JNIUtilGetStringUTFChars
#define JNIUtilGetStringUTFChars(jniEnv, string, isCopy) \
    _JNIUtilWrapMacro_Created(GetStringUTFChars, jniEnv, string, isCopy)
#endif

#ifndef JNIUtilReleaseStringUTFChars
#define JNIUtilReleaseStringUTFChars(jniEnv, string, utf) \
    _JNIUtilWrapMacro_Released_1(ReleaseStringUTFChars, jniEnv, string, utf)
#endif

#ifndef JNIUtilGetArrayLength
#define JNIUtilGetArrayLength(jniEnv, array) \
    _JNIUtilWrapMacro_FuncCalled(GetArrayLength, jniEnv, array)
#endif

#ifndef JNIUtilNewObjectArray
#define JNIUtilNewObjectArray(jniEnv, length, elementClass, initialElement) \
    _JNIUtilWrapMacro_Created(NewObjectArray, jniEnv, length, elementClass, initialElement)
#endif

#ifndef JNIUtilGetObjectArrayElement
#define JNIUtilGetObjectArrayElement(jniEnv, array, index) \
    _JNIUtilWrapMacro_Created(GetObjectArrayElement, jniEnv, array, index)
#endif

#ifndef JNIUtilSetObjectArrayElement
#define JNIUtilSetObjectArrayElement(jniEnv, array, index, value) \
    _JNIUtilWrapMacro_FuncCalled(SetObjectArrayElement, jniEnv, array, index, value)
#endif

#ifndef JNIUtilNewBooleanArray
#define JNIUtilNewBooleanArray(jniEnv, length) \
    _JNIUtilWrapMacro_Created(NewBooleanArray, jniEnv, length)
#endif

#ifndef JNIUtilNewByteArray
#define JNIUtilNewByteArray(jniEnv, length) \
    _JNIUtilWrapMacro_Created(NewByteArray, jniEnv, length)
#endif

#ifndef JNIUtilNewCharArray
#define JNIUtilNewCharArray(jniEnv, length) \
    _JNIUtilWrapMacro_Created(NewCharArray, jniEnv, length)
#endif

#ifndef JNIUtilNewShortArray
#define JNIUtilNewShortArray(jniEnv, length) \
    _JNIUtilWrapMacro_Created(NewShortArray, jniEnv, length)
#endif

#ifndef JNIUtilNewIntArray
#define JNIUtilNewIntArray(jniEnv, length) \
    _JNIUtilWrapMacro_Created(NewIntArray, jniEnv, length)
#endif

#ifndef JNIUtilNewLongArray
#define JNIUtilNewLongArray(jniEnv, length) \
    _JNIUtilWrapMacro_Created(NewLongArray, jniEnv, length)
#endif

#ifndef JNIUtilNewFloatArray
#define JNIUtilNewFloatArray(jniEnv, length) \
    _JNIUtilWrapMacro_Created(NewFloatArray, jniEnv, length)
#endif

#ifndef JNIUtilNewDoubleArray
#define JNIUtilNewDoubleArray(jniEnv, length) \
    _JNIUtilWrapMacro_Created(NewDoubleArray, jniEnv, length)
#endif

#ifndef JNIUtilGetBooleanArrayElements
#define JNIUtilGetBooleanArrayElements(jniEnv, array, isCopy) \
    _JNIUtilWrapMacro_Created(GetBooleanArrayElements, jniEnv, array, isCopy)
#endif

#ifndef JNIUtilGetByteArrayElements
#define JNIUtilGetByteArrayElements(jniEnv, array, isCopy) \
    _JNIUtilWrapMacro_Created(GetByteArrayElements, jniEnv, array, isCopy)
#endif

#ifndef JNIUtilGetCharArrayElements
#define JNIUtilGetCharArrayElements(jniEnv, array, isCopy) \
    _JNIUtilWrapMacro_Created(GetCharArrayElements, jniEnv, array, isCopy)
#endif

#ifndef JNIUtilGetShortArrayElements
#define JNIUtilGetShortArrayElements(jniEnv, array, isCopy) \
    _JNIUtilWrapMacro_Created(GetShortArrayElements, jniEnv, array, isCopy)
#endif

#ifndef JNIUtilGetIntArrayElements
#define JNIUtilGetIntArrayElements(jniEnv, array, isCopy) \
    _JNIUtilWrapMacro_Created(GetIntArrayElements, jniEnv, array, isCopy)
#endif

#ifndef JNIUtilGetLongArrayElements
#define JNIUtilGetLongArrayElements(jniEnv, array, isCopy) \
    _JNIUtilWrapMacro_Created(GetLongArrayElements, jniEnv, array, isCopy)
#endif

#ifndef JNIUtilGetFloatArrayElements
#define JNIUtilGetFloatArrayElements(jniEnv, array, isCopy) \
    _JNIUtilWrapMacro_Created(GetFloatArrayElements, jniEnv, array, isCopy)
#endif

#ifndef JNIUtilGetDoubleArrayElements
#define JNIUtilGetDoubleArrayElements(jniEnv, array, isCopy) \
    _JNIUtilWrapMacro_Created(GetDoubleArrayElements, jniEnv, array, isCopy)
#endif

#ifndef JNIUtilReleaseBooleanArrayElements
#define JNIUtilReleaseBooleanArrayElements(jniEnv, array, elems, mode) \
    _JNIUtilWrapMacro_Released_1(ReleaseBooleanArrayElements, jniEnv, array, elems, mode)
#endif

#ifndef JNIUtilReleaseByteArrayElements
#define JNIUtilReleaseByteArrayElements(jniEnv, array, elems, mode) \
    _JNIUtilWrapMacro_Released_1(ReleaseByteArrayElements, jniEnv, array, elems, mode)
#endif

#ifndef JNIUtilReleaseCharArrayElements
#define JNIUtilReleaseCharArrayElements(jniEnv, array, elems, mode) \
    _JNIUtilWrapMacro_Released_1(ReleaseCharArrayElements, jniEnv, array, elems, mode)
#endif

#ifndef JNIUtilReleaseShortArrayElements
#define JNIUtilReleaseShortArrayElements(jniEnv, array, elems, mode) \
    _JNIUtilWrapMacro_Released_1(ReleaseShortArrayElements, jniEnv, array, elems, mode)
#endif

#ifndef JNIUtilReleaseIntArrayElements
#define JNIUtilReleaseIntArrayElements(jniEnv, array, elems, mode) \
    _JNIUtilWrapMacro_Released_1(ReleaseIntArrayElements, jniEnv, array, elems, mode)
#endif

#ifndef JNIUtilReleaseLongArrayElements
#define JNIUtilReleaseLongArrayElements(jniEnv, array, elems, mode) \
    _JNIUtilWrapMacro_Released_1(ReleaseLongArrayElements, jniEnv, array, elems, mode)
#endif

#ifndef JNIUtilReleaseFloatArrayElements
#define JNIUtilReleaseFloatArrayElements(jniEnv, array, elems, mode) \
    _JNIUtilWrapMacro_Released_1(ReleaseFloatArrayElements, jniEnv, array, elems, mode)
#endif

#ifndef JNIUtilReleaseDoubleArrayElements
#define JNIUtilReleaseDoubleArrayElements(jniEnv, array, elems, mode) \
    _JNIUtilWrapMacro_Released_1(ReleaseDoubleArrayElements, jniEnv, array, elems, mode)
#endif

#ifndef JNIUtilGetBooleanArrayRegion
#define JNIUtilGetBooleanArrayRegion(jniEnv, array, start, len, buf) \
    _JNIUtilWrapMacro_FuncCalled(GetBooleanArrayRegion, jniEnv, array, start, len, buf)
#endif

#ifndef JNIUtilGetByteArrayRegion
#define JNIUtilGetByteArrayRegion(jniEnv, array, start, len, buf) \
    _JNIUtilWrapMacro_FuncCalled(GetByteArrayRegion, jniEnv, array, start, len, buf)
#endif

#ifndef JNIUtilGetCharArrayRegion
#define JNIUtilGetCharArrayRegion(jniEnv, array, start, len, buf) \
    _JNIUtilWrapMacro_FuncCalled(GetCharArrayRegion, jniEnv, array, start, len, buf)
#endif

#ifndef JNIUtilGetShortArrayRegion
#define JNIUtilGetShortArrayRegion(jniEnv, array, start, len, buf) \
    _JNIUtilWrapMacro_FuncCalled(GetShortArrayRegion, jniEnv, array, start, len, buf)
#endif

#ifndef JNIUtilGetIntArrayRegion
#define JNIUtilGetIntArrayRegion(jniEnv, array, start, len, buf) \
    _JNIUtilWrapMacro_FuncCalled(GetIntArrayRegion, jniEnv, array, start, len, buf)
#endif

#ifndef JNIUtilGetLongArrayRegion
#define JNIUtilGetLongArrayRegion(jniEnv, array, start, len, buf) \
    _JNIUtilWrapMacro_FuncCalled(GetLongArrayRegion, jniEnv, array, start, len, buf)
#endif

#ifndef JNIUtilGetFloatArrayRegion
#define JNIUtilGetFloatArrayRegion(jniEnv, array, start, len, buf) \
    _JNIUtilWrapMacro_FuncCalled(GetFloatArrayRegion, jniEnv, array, start, len, buf)
#endif

#ifndef JNIUtilGetDoubleArrayRegion
#define JNIUtilGetDoubleArrayRegion(jniEnv, array, start, len, buf) \
    _JNIUtilWrapMacro_FuncCalled(GetDoubleArrayRegion, jniEnv, array, start, len, buf)
#endif

#ifndef JNIUtilSetBooleanArrayRegion
#define JNIUtilSetBooleanArrayRegion(jniEnv, array, start, len, buf) \
    _JNIUtilWrapMacro_FuncCalled(SetBooleanArrayRegion, jniEnv, array, start, len, buf)
#endif

#ifndef JNIUtilSetByteArrayRegion
#define JNIUtilSetByteArrayRegion(jniEnv, array, start, len, buf) \
    _JNIUtilWrapMacro_FuncCalled(SetByteArrayRegion, jniEnv, array, start, len, buf)
#endif

#ifndef JNIUtilSetCharArrayRegion
#define JNIUtilSetCharArrayRegion(jniEnv, array, start, len, buf) \
    _JNIUtilWrapMacro_FuncCalled(SetCharArrayRegion, jniEnv, array, start, len, buf)
#endif

#ifndef JNIUtilSetShortArrayRegion
#define JNIUtilSetShortArrayRegion(jniEnv, array, start, len, buf) \
    _JNIUtilWrapMacro_FuncCalled(SetShortArrayRegion, jniEnv, array, start, len, buf)
#endif

#ifndef JNIUtilSetIntArrayRegion
#define JNIUtilSetIntArrayRegion(jniEnv, array, start, len, buf) \
    _JNIUtilWrapMacro_FuncCalled(SetIntArrayRegion, jniEnv, array, start, len, buf)
#endif

#ifndef JNIUtilSetLongArrayRegion
#define JNIUtilSetLongArrayRegion(jniEnv, array, start, len, buf) \
    _JNIUtilWrapMacro_FuncCalled(SetLongArrayRegion, jniEnv, array, start, len, buf)
#endif

#ifndef JNIUtilSetFloatArrayRegion
#define JNIUtilSetFloatArrayRegion(jniEnv, array, start, len, buf) \
    _JNIUtilWrapMacro_FuncCalled(SetFloatArrayRegion, jniEnv, array, start, len, buf)
#endif

#ifndef JNIUtilSetDoubleArrayRegion
#define JNIUtilSetDoubleArrayRegion(jniEnv, array, start, len, buf) \
    _JNIUtilWrapMacro_FuncCalled(SetDoubleArrayRegion, jniEnv, array, start, len, buf)
#endif

#ifndef JNIUtilRegisterNatives
#define JNIUtilRegisterNatives(jniEnv, clazz, methods, nMethods) \
    _JNIUtilWrapMacro_FuncCalled(RegisterNatives, jniEnv, clazz, methods, nMethods)
#endif

#ifndef JNIUtilUnregisterNatives
#define JNIUtilUnregisterNatives(jniEnv, clazz) \
    _JNIUtilWrapMacro_FuncCalled(UnregisterNatives, jniEnv, clazz)
#endif

#ifndef JNIUtilMonitorEnter
#define JNIUtilMonitorEnter(jniEnv, obj) \
    _JNIUtilWrapMacro_FuncCalled(MonitorEnter, jniEnv, obj)
#endif

#ifndef JNIUtilMonitorExit
#define JNIUtilMonitorExit(jniEnv, obj) \
    _JNIUtilWrapMacro_FuncCalled(MonitorExit, jniEnv, obj)
#endif

#ifndef JNIUtilGetJavaVM
#define JNIUtilGetJavaVM(jniEnv, vm) \
    _JNIUtilWrapMacro_FuncCalled(GetJavaVM, jniEnv, vm)
#endif

#ifndef JNIUtilGetStringRegion
#define JNIUtilGetStringRegion(jniEnv, str, start, len, buf) \
    _JNIUtilWrapMacro_FuncCalled(GetStringRegion, jniEnv, str, start, len, buf)
#endif

#ifndef JNIUtilGetStringUTFRegion
#define JNIUtilGetStringUTFRegion(jniEnv, str, start, len, buf) \
    _JNIUtilWrapMacro_FuncCalled(GetStringUTFRegion, jniEnv, str, start, len, buf)
#endif

#ifndef JNIUtilGetPrimitiveArrayCritical
#define JNIUtilGetPrimitiveArrayCritical(jniEnv, array, isCopy) \
    _JNIUtilWrapMacro_Created(GetPrimitiveArrayCritical, jniEnv, array, isCopy)
#endif

#ifndef JNIUtilReleasePrimitiveArrayCritical
#define JNIUtilReleasePrimitiveArrayCritical(jniEnv, array, carray, mode) \
    _JNIUtilWrapMacro_Released_1(ReleasePrimitiveArrayCritical, jniEnv, array, carray, mode)
#endif

#ifndef JNIUtilGetStringCritical
#define JNIUtilGetStringCritical(jniEnv, string, isCopy) \
    _JNIUtilWrapMacro_Created(GetStringCritical, jniEnv, string, isCopy)
#endif

#ifndef JNIUtilReleaseStringCritical
#define JNIUtilReleaseStringCritical(jniEnv, string, carray) \
    _JNIUtilWrapMacro_Released_1(ReleaseStringCritical, jniEnv, string, carray)
#endif

#ifndef JNIUtilNewWeakGlobalRef
#define JNIUtilNewWeakGlobalRef(jniEnv, obj) \
    _JNIUtilWrapMacro_Created(NewWeakGlobalRef, jniEnv, obj)
#endif

#ifndef JNIUtilDeleteWeakGlobalRef
#define JNIUtilDeleteWeakGlobalRef(jniEnv, obj) \
    _JNIUtilWrapMacro_Released_0(DeleteWeakGlobalRef, jniEnv, obj)
#endif

#ifndef JNIUtilExceptionCheck
#define JNIUtilExceptionCheck(jniEnv) \
    _JNIUtilWrapMacro_FuncCalled(ExceptionCheck, jniEnv)
#endif

#ifndef JNIUtilNewDirectByteBuffer
#define JNIUtilNewDirectByteBuffer(jniEnv, address, capacity) \
    _JNIUtilWrapMacro_Created(NewDirectByteBuffer, jniEnv, address, capacity)
#endif

#ifndef JNIUtilGetDirectBufferAddress
#define JNIUtilGetDirectBufferAddress(jniEnv, buf) \
    _JNIUtilWrapMacro_FuncCalled(GetDirectBufferAddress, jniEnv, buf)
#endif

#ifndef JNIUtilGetDirectBufferCapacity
#define JNIUtilGetDirectBufferCapacity(jniEnv, buf) \
    _JNIUtilWrapMacro_FuncCalled(GetDirectBufferCapacity, jniEnv, buf)
#endif

#ifndef JNIUtilGetObjectRefType
#define JNIUtilGetObjectRefType(jniEnv, obj) \
    _JNIUtilWrapMacro_FuncCalled(GetObjectRefType, jniEnv, obj)
#endif

} // namespace JNIUtil

// ============================================================
// other
namespace JNIUtil
{
/**
 * @brief util object to hold jobject and delete automatically by DeleteGlobalRef
 */
class _JNI_EXPORT JNIObjectHolder
{
public:
    /** @cond ZFPrivateDoc */
    JNIObjectHolder(void)
    : _obj(NULL)
    {
    }
    JNIObjectHolder(jobject obj)
    : _obj(obj == NULL ? NULL : JNIUtilNewGlobalRef(JNIGetJNIEnv(), obj))
    {
    }
    JNIObjectHolder(const JNIObjectHolder &ref)
    : _obj(ref.get() == NULL ? NULL : JNIUtilNewGlobalRef(JNIGetJNIEnv(), ref.get()))
    {
    }
    ~JNIObjectHolder(void)
    {
        if(_obj != NULL)
        {
            JNIUtilDeleteGlobalRef(JNIGetJNIEnv(), _obj);
        }
    }
    operator jobject (void) const
    {
        return _obj;
    }
    JNIObjectHolder &operator = (const JNIObjectHolder &ref)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        jobject tmp = ((ref.get() == NULL) ? NULL : JNIUtilNewGlobalRef(jniEnv, ref.get()));
        if(_obj != NULL)
        {
            JNIUtilDeleteGlobalRef(jniEnv, _obj);
        }
        _obj = tmp;
        return *this;
    }
    JNIObjectHolder &operator = (jobject const &obj)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        jobject tmp = ((obj == NULL) ? NULL : JNIUtilNewGlobalRef(jniEnv, obj));
        if(_obj != NULL)
        {
            JNIUtilDeleteGlobalRef(jniEnv, _obj);
        }
        _obj = tmp;
        return *this;
    }
    /** @endcond */
public:
    /**
     * @brief get the jobject
     */
    jobject get(void) const
    {
        return _obj;
    }
private:
    jobject _obj;
};
} // namespace JNIUtil
#endif // #if NEED_JNIUTIL

#endif // #ifndef _JNIUtil_MethodWrapper_h_

