#include "ZFImpl_sys_Android_ZF_impl.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

#define ZFImpl_sys_Android_JNI_ID_ZFEnum ZFImpl_sys_Android_JNI_ID(ZF_1impl_ZFEnum)
#define ZFImpl_sys_Android_JNI_NAME_ZFEnum ZFImpl_sys_Android_JNI_NAME(ZF_impl.ZFEnum)

ZF_NAMESPACE_GLOBAL_END

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFEnum,
                         void, native_1enumInvalid,
                         jstring rawEnumNamespace,
                         jstring rawEnumValueName)
{
    const zfchar *rawEnumNamespaceT = JNIUtilGetStringUTFChars(jniEnv, rawEnumNamespace, zfnull);
    const zfchar *rawEnumValueNameT = JNIUtilGetStringUTFChars(jniEnv, rawEnumValueName, zfnull);
    zfCoreCriticalMessageTrim("[ZFEnum] invalid enum: %s::%s",
        rawEnumNamespaceT,
        rawEnumValueNameT);
    JNIUtilReleaseStringUTFChars(jniEnv, rawEnumNamespace, rawEnumNamespaceT);
    JNIUtilReleaseStringUTFChars(jniEnv, rawEnumValueName, rawEnumValueNameT);
}
JNI_METHOD_DECLARE_END()

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFEnum,
                         jint, native_1rawEnumValue,
                         jstring rawEnumNamespace,
                         jstring rawEnumValueName)
{
    const zfchar *rawEnumNamespaceT = JNIUtilGetStringUTFChars(jniEnv, rawEnumNamespace, zfnull);
    const zfchar *rawEnumValueNameT = JNIUtilGetStringUTFChars(jniEnv, rawEnumValueName, zfnull);
    zfuint ret = ZFEnumInvalid();
    do
    {
        const ZFMethod *method = ZFMethodForName(rawEnumNamespaceT, rawEnumValueNameT);
        if(method == zfnull) {break;}
        ret = method->execute<zfuint>(zfnull);
    } while(zffalse);
    JNIUtilReleaseStringUTFChars(jniEnv, rawEnumNamespace, rawEnumNamespaceT);
    JNIUtilReleaseStringUTFChars(jniEnv, rawEnumValueName, rawEnumValueNameT);
    return (jint)ret;
}
JNI_METHOD_DECLARE_END()
JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFEnum,
                         jint, native_1enumDefault,
                         jstring enumClassName)
{
    const zfchar *enumClassNameT = JNIUtilGetStringUTFChars(jniEnv, enumClassName, zfnull);
    zfuint ret = ZFEnumInvalid();
    do
    {
        const ZFClass *cls = ZFClass::classForName(enumClassNameT);
        if(cls == zfnull || !cls->classIsTypeOf(ZFEnum::ClassData())) {break;}
        const ZFMethod *method = cls->methodForName("EnumDefault");
        if(method == zfnull) {break;}
        ret = method->execute<zfuint>(zfnull);
    } while(zffalse);
    JNIUtilReleaseStringUTFChars(jniEnv, enumClassName, enumClassNameT);
    return (jint)ret;
}
JNI_METHOD_DECLARE_END()
JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFEnum,
                         jint, native_1enumValue,
                         jstring enumClassName,
                         jstring enumValueName)
{
    const zfchar *enumClassNameT = JNIUtilGetStringUTFChars(jniEnv, enumClassName, zfnull);
    const zfchar *enumValueNameT = JNIUtilGetStringUTFChars(jniEnv, enumValueName, zfnull);
    zfuint ret = ZFEnumInvalid();
    do
    {
        const ZFClass *cls = ZFClass::classForName(enumClassNameT);
        if(cls == zfnull || !cls->classIsTypeOf(ZFEnum::ClassData())) {break;}
        const ZFMethod *method = cls->methodForName("EnumValueForName");
        if(method == zfnull) {break;}
        ret = method->execute<zfuint, const zfchar *>(zfnull, enumValueNameT);
    } while(zffalse);
    JNIUtilReleaseStringUTFChars(jniEnv, enumClassName, enumClassNameT);
    JNIUtilReleaseStringUTFChars(jniEnv, enumValueName, enumValueNameT);
    return (jint)ret;
}
JNI_METHOD_DECLARE_END()
JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFEnum,
                         jstring, native_1enumName,
                         jstring enumClassName,
                         jint enumValue)
{
    const zfchar *enumClassNameT = JNIUtilGetStringUTFChars(jniEnv, enumClassName, zfnull);
    const zfchar *ret = ZFEnumNameInvalid();
    do
    {
        const ZFClass *cls = ZFClass::classForName(enumClassNameT);
        if(cls == zfnull || !cls->classIsTypeOf(ZFEnum::ClassData())) {break;}
        const ZFMethod *method = cls->methodForName("EnumNameForValue");
        if(method == zfnull) {break;}
        ret = method->execute<const zfchar *, zfuint>(zfnull, (zfuint)enumValue);
    } while(zffalse);
    JNIUtilReleaseStringUTFChars(jniEnv, enumClassName, enumClassNameT);
    return JNIUtilNewStringUTF(jniEnv, ret);
}
JNI_METHOD_DECLARE_END()

#endif // #if ZF_ENV_sys_Android

