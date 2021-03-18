package com.ZFFramework.Android.NativeUtil;

import java.lang.reflect.Field;
import java.lang.reflect.Modifier;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.List;
import android.annotation.SuppressLint;
import android.view.View;

public class ZFAndroidLog {
    public static void p(String msg) {
        android.util.Log.e(ZFAndroidLog.defaultLogTag, msg);
    }
    public static void p(String tag, String msg) {
        android.util.Log.e(tag, msg);
    }
    public static void p(Class<?> cls, String msg) {
        android.util.Log.e(cls.getSimpleName(), msg);
    }

    public static String logTagForClass(Class<?> cls) {
        return cls.getSimpleName();
    }
    public static String defaultLogTag = "";

    public static void assertMsg(String sMsg) {
        ZFAndroidLog._printAssertMsg(ZFAndroidLog.defaultLogTag, sMsg);
    }
    public static void assertMsg(String sTag, String sMsg) {
        ZFAndroidLog._printAssertMsg(sTag, sMsg);
    }

    public static void shouldNotGoHere() {
        ZFAndroidLog._printAssertMsg(ZFAndroidLog.defaultLogTag, "should not go here");
    }
    public static void shouldNotGoHere(String sTag) {
        ZFAndroidLog._printAssertMsg(sTag, "should not go here");
    }
    private static void _printAssertMsg(String sTag, String sMsg) {
        ZFAndroidLog.p(sTag, "====================  assert message   ====================");
        StackTraceElement ste = Thread.currentThread().getStackTrace()[4];
        ZFAndroidLog.p(sTag, String.format("| called by %s [%s (%d)]", ste.getMethodName(), ste.getFileName(), ste.getLineNumber()));
        ZFAndroidLog.p(sTag, "| " + sMsg);
        ZFAndroidLog.p(sTag, "====================  assert message   ====================");
    }

    // caller info
    public static String callerInfo() {
        return ZFAndroidLog._callerInfo(1);
    }
    public static String callerInfo(int ignoreLevel) {
        return ZFAndroidLog._callerInfo(ignoreLevel);
    }
    @SuppressLint("DefaultLocale")
    private static String _callerInfo(int ignoreLevel) {
        StackTraceElement ste = Thread.currentThread().getStackTrace()[4+ignoreLevel];
        return String.format("[%s %s (%d)]", ste.getMethodName(), ste.getFileName(), ste.getLineNumber());
    }

    // log called function
    public static void logFunc() {
        ZFAndroidLog._logFunc(ZFAndroidLog.defaultLogTag, "");
    }
    public static void logFunc(String msg) {
        ZFAndroidLog._logFunc(ZFAndroidLog.defaultLogTag, msg);
    }
    public static void logFunc(String tag, String msg) {
        ZFAndroidLog._logFunc(tag, msg);
    }
    private static void _logFunc(String tag, String msg) {
        StackTraceElement ste = Thread.currentThread().getStackTrace()[4];
        ZFAndroidLog.p(tag, String.format("%s %s", ste.getMethodName(), msg));
    }

    // stack trace
    public static String stackTrace() {
        return ZFAndroidLog._stackTrace(0, Integer.MAX_VALUE);
    }
    public static String stackTrace(int levelIgnore) {
        return ZFAndroidLog._stackTrace(levelIgnore, Integer.MAX_VALUE);
    }
    public static String stackTrace(int levelIgnore, int maxNum) {
        return ZFAndroidLog._stackTrace(levelIgnore, maxNum);
    }
    private static String _stackTrace(int levelIgnore, int maxNum) {
        StringBuilder sb = new StringBuilder();
        StackTraceElement ste[] = Thread.currentThread().getStackTrace();
        sb.append("==================== stack trace begin ====================");
        sb.append('\n');
        for(int i = 4 + levelIgnore, num = 0; i < ste.length && num < maxNum; ++i, ++num) {
            sb.append(String.format("|  %s [%s (%d)]",
                ste[i].getMethodName(),
                ste[i].getFileName(),
                ste[i].getLineNumber()));
            sb.append('\n');
        }
        sb.append("====================  stack trace end  ====================");
        sb.append('\n');
        return sb.toString();
    }

    public static void stackTracePrint() {
        ZFAndroidLog._stackTracePrint(ZFAndroidLog.defaultLogTag, 0, Integer.MAX_VALUE);
    }
    public static void stackTracePrint(int levelIgnore) {
        ZFAndroidLog._stackTracePrint(ZFAndroidLog.defaultLogTag, levelIgnore, Integer.MAX_VALUE);
    }
    public static void stackTracePrint(int levelIgnore, int maxNum) {
        ZFAndroidLog._stackTracePrint(ZFAndroidLog.defaultLogTag, levelIgnore, maxNum);
    }
    public static void stackTracePrint(String sTag, int levelIgnore, int maxNum) {
        ZFAndroidLog._stackTracePrint(sTag, levelIgnore, maxNum);
    }
    private static void _stackTracePrint(String sTag, int levelIgnore, int maxNum) {
        StackTraceElement ste[] = Thread.currentThread().getStackTrace();
        ZFAndroidLog.p(sTag, "==================== stack trace begin ====================");
        for(int i = 4 + levelIgnore, num = 0; i < ste.length && num < maxNum; ++i, ++num) {
            ZFAndroidLog.p(sTag, String.format("|  %s [%s (%d)]",
                ste[i].getMethodName(),
                ste[i].getFileName(),
                ste[i].getLineNumber()));
        }
        ZFAndroidLog.p(sTag, "====================  stack trace end  ====================");
    }

    /**
     * @brief get info of native ZFObject, param must a (ZFObject *) pointer
     *   converted by #JNIConvertZFObjectFromJNIType
     */
    public native static String native_objectInfo(long zfjniPointerZFObject);

    private static boolean _isSysClass(Class<?> cls) {
        return (false
            || cls == Object.class
            || cls == String.class
            || cls == View.class
            );
    }
    public static String objectInfo(Object obj) {
        return ZFAndroidLog.objectInfo(obj, null);
    }
    public static String objectInfo(Object obj, List<String> ignorePropertyNames) {
        if(obj == null) {
            return "<null>";
        }

        StringBuilder sb = new StringBuilder();
        sb.append(String.format("<%s %08X", obj.getClass().getSimpleName(), obj.hashCode()));

        for(Class<?> cls = obj.getClass();
            cls != null && !ZFAndroidLog._isSysClass(cls);
            cls = cls.getSuperclass()) {
            Field[] allProperty = cls.getDeclaredFields();
            for(int i = 0; i < allProperty.length; ++i) {
                allProperty[i].setAccessible(true);
                if(Modifier.isStatic(allProperty[i].getModifiers())) {
                    continue;
                }

                if(ignorePropertyNames != null) {
                    boolean ignore = false;
                    for(int iIgnore = 0; iIgnore < ignorePropertyNames.size(); ++iIgnore) {
                        if(ignorePropertyNames.get(iIgnore).compareTo(allProperty[i].getName()) == 0) {
                            ignore = true;
                            break;
                        }
                    }
                    if(ignore) {
                        continue;
                    }
                }

                Object value = null;
                try {
                    value = allProperty[i].get(obj);
                }
                catch(Exception e) {
                }
                sb.append(String.format(" \"%s\"=\"%s\"",
                    allProperty[i].getName(),
                    value == null ? "<null>" : value.toString()));
            }
        }

        sb.append(">");
        return sb.toString();

    }

    // log utils
    @SuppressLint("SimpleDateFormat")
    private static SimpleDateFormat _curTimeToStringFmtCache = new SimpleDateFormat("HH:mm:ss.SSS");
    public static String curTimeToString() {
        return _curTimeToStringFmtCache.format(new Date());
    }
}
