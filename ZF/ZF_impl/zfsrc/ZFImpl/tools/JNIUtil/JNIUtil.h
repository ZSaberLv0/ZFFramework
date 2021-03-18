/**
 * @file JNIUtil.h
 * @brief JNI utilities
 */

#ifndef _JNIUtil_h_
#define _JNIUtil_h_

// ============================================================
#if 1
#include "ZFCore.h"
/** @cond ZFPrivateDoc */
#define _JNI_EXPORT ZF_ENV_EXPORT
/** @endcond */
#else
#define _JNI_EXPORT
#endif

// ============================================================
#ifndef NEED_JNIUTIL
    #if (defined(ANDROID) || defined(__ANDROID__) || defined(JNI_VERSION_1_1) || defined(JNIEXPORT) || defined(JNICALL))
        /**
         * @brief macro to force enable JNIUtil
         */
        #define NEED_JNIUTIL 1
    #endif
#endif

#if NEED_JNIUTIL
#include <stdlib.h>
#include <jni.h>

#if defined(__cplusplus) || defined(c_plusplus)
    #define _JNI_EXTERN_C extern "C"
#else
    #define _JNI_EXTERN_C
#endif

namespace JNIUtil {

// ============================================================
// wrapper
/**
 * @brief workaround to prevent export stl containers
 */
class _JNI_EXPORT JNIString
{
public:
    JNIString(void);
    JNIString(JNIString const &ref);
    JNIString(const char *s);
    ~JNIString(void);
    JNIString &operator = (JNIString const &ref);
    JNIString &operator = (const char *s);
    const char *c_str(void) const;
    operator const char * (void) const;
    JNIString &operator += (char c);
    JNIString &operator += (const char *s);
    void clear(void);
public:
    void *_d;
};

class JNIType;
/**
 * @brief wrapper for JNIGetMethodSig
 */
class _JNI_EXPORT JNIParamTypeContainer
{
public:
    JNIParamTypeContainer(void);
    ~JNIParamTypeContainer(void);
private:
    JNIParamTypeContainer(const JNIParamTypeContainer &ref);
    JNIParamTypeContainer &operator = (const JNIParamTypeContainer &ref);
public:
    /** @brief see #JNIGetMethodSig */
    JNIParamTypeContainer &add(const JNIType &paramType);
public:
    void *_d;
};

// ============================================================
// common
/**
 * @brief init jni jniEnv, must be called in your JNI_OnLoad
 */
extern _JNI_EXPORT bool JNIInit(JavaVM *javaVM, jint version = JNI_VERSION_1_1);

/**
 * @brief global java vm
 */
extern _JNI_EXPORT JavaVM *JNIGetJavaVM(void);

/**
 * @brief get desired version passed from JNIInit, or -1 if init failed
 */
extern _JNI_EXPORT jint JNIGetDesiredVersion(void);

/**
 * @brief get JNIEnv for current thread
 */
extern _JNI_EXPORT JNIEnv *JNIGetJNIEnv(void);

/**
 * @brief convert class name to JNI class sig
 *
 * for example, convert "com.example.OutterClass$InnerClass"
 * to "Lcom/example/OutterClass$InnerClass;"\n
 * class name can be get by SomeClass.class.getName() in your jave code
 */
extern _JNI_EXPORT void JNIConvertClassNameToClassSig(JNIString &ret, const char *className);
/** @brief see #JNIConvertClassNameToClassSig */
inline JNIString JNIConvertClassNameToClassSig(const char *className)
{
    JNIString ret;
    JNIConvertClassNameToClassSig(ret, className);
    return ret;
}

/**
 * @brief convert JNI class sig to class name
 *
 * for example, convert "Lcom/example/OutterClass$InnerClass;"
 * to "com.example.OutterClass$InnerClass"
 */
extern _JNI_EXPORT void JNIConvertClassNameFromClassSig(JNIString &ret, const char *classSig);
/** @brief see #JNIConvertClassNameFromClassSig */
inline JNIString JNIConvertClassNameFromClassSig(const char *className)
{
    JNIString ret;
    JNIConvertClassNameFromClassSig(ret, className);
    return ret;
}

/**
 * @brief convert class name for FindClass in JNI
 *
 * for example, convert "com.example.OutterClass$InnerClass"
 * to "com/example/OutterClass$InnerClass"\n
 * class name can be get by SomeClass.class.getName() in your jave code
 */
extern _JNI_EXPORT void JNIConvertClassNameForFindClass(JNIString &ret, const char *className);
/** @brief see #JNIConvertClassNameForFindClass */
inline JNIString JNIConvertClassNameForFindClass(const char *className)
{
    JNIString ret;
    JNIConvertClassNameForFindClass(ret, className);
    return ret;
}

/**
 * @brief constructor method name
 */
#define JNIConstructorName "<init>"

// ============================================================
// JNI types
class _JNITypePrivate;
/**
 * @brief JNI type utility
 */
class _JNI_EXPORT JNIType
{
public:
    typedef enum
    {
        T_boolean, /**< jboolean */
        T_byte, /**< jbyte */
        T_char, /**< jchar */
        T_short, /**< jshort */
        T_int, /**< jint */
        T_long, /**< jlong */
        T_float, /**< jfloat */
        T_double, /**< jdouble */
        T_void, /**< void */
        T_object, /**< jobect */
        T_array, /**< jarray */
    } Type;

public:
    /**
     * @brief main constructor with jboolean type
     */
    JNIType(void);
    /**
     * @brief construct from type and classNameOrArrayElementTypeId (if type is object or array)
     *
     * classNameOrArrayElementTypeId is only valid if type is object and array\n
     * if object, it means the class name of the object
     * such as "com.example.OutterClass$InnerClass"\n
     * if array, it means the type id which can be accessed by another #JNIType::getId
     */
    JNIType(JNIType::Type type,
            const char *classNameOrArrayElementTypeId = NULL);
    JNIType(const JNIType &ref);
    JNIType &operator = (const JNIType &ref);

public:
    /**
     * @brief set JNI type
     */
    void setType(JNIType::Type type);
    /**
     * @brief see #setType
     */
    JNIType::Type getType(void) const;

    /**
     * @brief see JNIType's constructor
     */
    void setClassNameOrArrayElementTypeId(const char *s);
    /**
     * @brief see #setClassNameOrArrayElementTypeId
     */
    const char *getClassNameOrArrayElementTypeId(void) const;

public:
    /**
     * @brief get type id (e.g. "Z" for jboolean, "V" for void)
     *
     * note: if you set a wrong info about the JNI type
     * (such as a object type with a wrong class name),
     * the returned id may be wrong without any warnings
     */
    const char *getId(void) const;

public:
    #define _JNIUtil_DECLARE_STATIC_TYPE(Type) \
        /** @brief a static instance for the T_##Type type */ \
        static const JNIType &S_##Type;
    _JNIUtil_DECLARE_STATIC_TYPE(boolean)
    _JNIUtil_DECLARE_STATIC_TYPE(byte)
    _JNIUtil_DECLARE_STATIC_TYPE(char)
    _JNIUtil_DECLARE_STATIC_TYPE(short)
    _JNIUtil_DECLARE_STATIC_TYPE(int)
    _JNIUtil_DECLARE_STATIC_TYPE(long)
    _JNIUtil_DECLARE_STATIC_TYPE(float)
    _JNIUtil_DECLARE_STATIC_TYPE(double)
    _JNIUtil_DECLARE_STATIC_TYPE(void)

public:
    /**
     * @brief util method to create object type
     */
    static inline JNIType S_object(const char *className)
    {
        return JNIType(JNIType::T_object, className);
    }
    /**
     * @brief util method to create array type
     */
    static inline JNIType S_array(const JNIType &type)
    {
        return JNIType(JNIType::T_array, type.getId());
    }

private:
    _JNITypePrivate *d;
};

// ============================================================
// signature utilities
/**
 * @brief get method signature, like "(Z)I" for "int func(bool)"
 *
 * you may use JNIParamTypeContainer to create a paramTypeList easily:
 * @code
 *   JNIGetMethodSig(
 *       JNIType::S_boolean,
 *       JNIParamTypeContainer()
 *           .add(JNIType::S_byte)
 *           .add(JNIType::T_boolean)
 *           .add(JNIType(JNIType::T_object, "java.lang.String"))
 *       );
 * @endcode
 */
extern _JNI_EXPORT JNIString JNIGetMethodSig(const JNIType &returnType,
                                             const JNIParamTypeContainer &paramTypeList);

/** @cond ZFPrivateDoc */
#define _JNI_METHOD_DECLARE_BEGIN(OwnerClassId, ReturnType, MethodName, ...) \
    _JNI_EXTERN_C JNIEXPORT ReturnType JNICALL Java_##OwnerClassId##_##MethodName( \
        JNIEnv *jniEnv, jclass jniCls, ##__VA_ARGS__)
/** @endcond */
/**
 * @brief macro to declare a JNI method
 *
 * usage:
 * @code
 *   JNI_METHOD_DECLARE_BEGIN(OwnerClassId,
 *                            ReturnType, MethodName,
 *                            Param0, param0,
 *                            Param1, param1)
 *   {
 *       // (JNIEnv *)jniEnv and (jclass)jniCls are predefined
 *   }
 *   JNI_METHOD_DECLARE_END()
 * @endcode
 *
 * class name should be like this:\n
 *   com_package_ClassName\n
 * if class name or method name has '_',
 * you should replace it with "_1" instead of '_'\n
 * if class name has inner class,
 * you should separate the inner class by "_00024" instead of '$',
 * such as "com_package_OutterClass_00024InnerClass"\n
 * \n
 * then, the final name would be like this:
 *   com_some_1package_OutterClass_00024InnerClass_method_1name\n
 * which stands for "com.some_package.OutterClass$InnerClass"'s
 * method named "method_name"
 */
#define JNI_METHOD_DECLARE_BEGIN(OwnerClassId, ReturnType, MethodName, ...) \
    _JNI_METHOD_DECLARE_BEGIN(OwnerClassId, ReturnType, MethodName, ##__VA_ARGS__)
/** @brief see #JNI_METHOD_DECLARE_BEGIN */
#define JNI_METHOD_DECLARE_END()

/**
 * @brief macro that declare JNI_OnLoad
 */
#define JNI_ONLOAD_ENTRY(paramJavaVM, paramReserved) \
    _JNI_EXTERN_C JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* paramJavaVM, void* paramReserved)

// ============================================================
// pointer converter
/**
 * @brief define to true to use long as #JNIPointer type
 */
#ifndef JNIPointerUseLong
    #define JNIPointerUseLong 1
#endif
/**
 * @brief see #JNIConvertPointerToJNIType
 */
#ifndef JNIPointer
    #if JNIPointerUseLong
        #define JNIPointer jlong
    #else
        #define JNIPointer jbyteArray
    #endif
#endif
/**
 * @brief see #JNIConvertPointerToJNIType
 */
#ifndef JNIPointerInvalid
    #if JNIPointerUseLong
        #define JNIPointerInvalid 0
    #else
        #define JNIPointerInvalid NULL
    #endif
#endif
/**
 * @brief see #JNIConvertPointerToJNIType
 */
#ifndef JNIPointerJNIType
    #if JNIPointerUseLong
        #define JNIPointerJNIType JNIType::S_long
    #else
        #define JNIPointerJNIType JNIType::S_array(JNIType::S_byte)
    #endif
#endif

jbyteArray _JNIConvertPointerToJNITypeAction(JNIEnv *jniEnv, void *p);
void *_JNIConvertPointerFromJNITypeAction(JNIEnv *jniEnv, jbyteArray d);

#if JNIPointerUseLong
    #define _JNIConvertPointerToJNIType(jniEnv, p) (JNIPointer)(p)
    #define _JNIConvertPointerFromJNIType(jniEnv, p) (void *)(p)
#else
    #define _JNIConvertPointerToJNIType(jniEnv, p) _JNIConvertPointerToJNITypeAction(jniEnv, p)
    #define _JNIConvertPointerFromJNIType(jniEnv, d) _JNIConvertPointerFromJNITypeAction(jniEnv, d)
#endif

/**
 * @brief safely convert a C pointer type to JNI type
 *
 * @see JNIConvertPointerFromJNIType
 * @note this method only ensure safe to convert as raw pointer type,
 *   you should take care when processing pointers with inheritance,
 *   it's recommended to convert to base class' pointer first,
 *   and always use base class's pointer when access
 */
#ifndef JNIConvertPointerToJNIType
    #define JNIConvertPointerToJNIType(jniEnv, p) _JNIConvertPointerToJNIType(jniEnv, p)
#endif
/**
 * @brief see #JNIConvertPointerToJNIType
 */
#ifndef JNIConvertPointerFromJNIType
    #define JNIConvertPointerFromJNIType(jniEnv, d) _JNIConvertPointerFromJNIType(jniEnv, d)
#endif

// ============================================================
// local/global ref cleaner
/** @cond ZFPrivateDoc */
namespace JNIUtilPrivate {
    class _JNI_EXPORT JNIAutoDeleteHolder
    {
    public:
        JNIAutoDeleteHolder(JNIEnv *jniEnv,
                            jobject obj,
                            bool globalRef)
        : jniEnvSaved(jniEnv)
        , objSaved(obj)
        , globalRefSaved(globalRef)
        {
        }
        ~JNIAutoDeleteHolder(void)
        {
            if(this->jniEnvSaved && this->objSaved)
            {
                if(this->globalRefSaved)
                {

                    this->jniEnvSaved->DeleteGlobalRef(this->objSaved);
                }
                else
                {
                    this->jniEnvSaved->DeleteLocalRef(this->objSaved);
                }
            }
        }
    public:
        JNIEnv *jniEnvSaved;
        jobject objSaved;
        bool globalRefSaved;
    };
    #define _JNIUtil_uniqueName(name) _JNIUtil_uniqueName2(name, __LINE__)
    #define _JNIUtil_uniqueName2(name, line) _JNIUtil_uniqueName3(name, line)
    #define _JNIUtil_uniqueName3(name, line) _JNIUtil_##name##_##line
} // namespace JNIUtilPrivate
/** @endcond */

/** @brief util macro to delete object after end line for JNI local object */
#define JNILineDeleteLocalRef(obj_) \
    (JNIUtilPrivate::JNIAutoDeleteHolder(JNIUtil::JNIGetJNIEnv(), obj_, false).objSaved)
/** @brief util macro to delete object after end line for JNI local object */
#define JNILineDeleteLocalRefWithEnv(obj_, jniEnv) \
    (JNIUtilPrivate::JNIAutoDeleteHolder(jniEnv, obj_, false).objSaved)
/** @brief util macro to delete object after end line for JNI global object */
#define JNILineDeleteGlobalRef(obj_) \
    (JNIUtilPrivate::JNIAutoDeleteHolder(JNIUtil::JNIGetJNIEnv(), obj_, true).objSaved)
/** @brief util macro to delete object after end line for JNI global object */
#define JNILineDeleteGlobalRefWithEnv(obj_, jniEnv) \
    (JNIUtilPrivate::JNIAutoDeleteHolder(jniEnv, obj_, true).objSaved)

/** @brief util macro to delete object after code block for JNI local object */
#define JNIBlockedDeleteLocalRef(obj_) \
    JNIUtilPrivate::JNIAutoDeleteHolder _JNIUtil_uniqueName(jniObjCleaner)(JNIUtil::JNIGetJNIEnv(), obj_, false)
/** @brief util macro to delete object after code block for JNI local object */
#define JNIBlockedDeleteLocalRefWithEnv(obj_, jniEnv) \
    JNIUtilPrivate::JNIAutoDeleteHolder _JNIUtil_uniqueName(jniObjCleaner)(jniEnv, obj_, false)
/** @brief util macro to delete object after code block for JNI global object */
#define JNIBlockedDeleteGlobalRef(obj_) \
    JNIUtilPrivate::JNIAutoDeleteHolder _JNIUtil_uniqueName(jniObjCleaner)(JNIUtil::JNIGetJNIEnv(), obj_, true)
/** @brief util macro to delete object after code block for JNI global object */
#define JNIBlockedDeleteGlobalRefWithEnv(obj_, jniEnv) \
    JNIUtilPrivate::JNIAutoDeleteHolder _JNIUtil_uniqueName(jniObjCleaner)(jniEnv, obj_, true)

} // namespace JNIUtil
#endif // #if NEED_JNIUTIL

#endif // #ifndef _JNIUtil_h_

#include "JNIUtil_MethodWrapper.h"
