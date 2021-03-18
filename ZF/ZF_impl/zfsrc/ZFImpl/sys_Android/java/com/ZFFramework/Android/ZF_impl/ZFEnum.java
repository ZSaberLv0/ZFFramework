package com.ZFFramework.Android.ZF_impl;

public class ZFEnum {
    public static int raw(String rawEnumNamespace, String rawEnumValueName) {
        return native_rawEnumValue(rawEnumNamespace, rawEnumValueName);
    }
    public static int e(String enumClassName, String enumValueName) {
        return native_enumValue(enumClassName, enumValueName);
    }
    public static int eDefault(String enumClassName) {
        return native_enumDefault(enumClassName);
    }
    public static String eName(String enumClassName, int enumValue) {
        return native_enumName(enumClassName, enumValue);
    }

    public static int rawAssert(String rawEnumNamespace, String rawEnumValueName) {
        int ret = native_rawEnumValue(rawEnumNamespace, rawEnumValueName);
        if(ret == -1) {
            native_enumInvalid(rawEnumNamespace, rawEnumValueName);
        }
        return ret;
    }
    public static int eAssert(String enumClassName, String enumValueName) {
        int ret = native_enumValue(enumClassName, enumValueName);
        if(ret == -1) {
            native_enumInvalid(enumClassName, enumValueName);
        }
        return ret;
    }

    private native static void native_enumInvalid(String rawEnumNamespace, String rawEnumValueName);
    private native static int native_rawEnumValue(String rawEnumNamespace, String rawEnumValueName);
    private native static int native_enumValue(String enumClassName, String enumValueName);
    private native static int native_enumDefault(String enumClassName);
    private native static String native_enumName(String enumClassName, int enumValue);
}
