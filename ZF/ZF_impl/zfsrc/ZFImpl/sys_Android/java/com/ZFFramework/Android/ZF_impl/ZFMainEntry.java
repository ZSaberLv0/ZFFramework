package com.ZFFramework.Android.ZF_impl;

import android.Manifest;
import android.app.Activity;
import android.app.Application;
import android.content.Context;
import android.content.pm.PackageManager;
import android.content.res.AssetManager;
import android.os.Build;
import android.os.Bundle;
import android.view.Window;

import java.lang.ref.WeakReference;
import java.util.ArrayList;
import java.util.List;

/**
 * @brief main entry as a Activity in Android
 *
 * under Android, start ZFMainEntry as a Activity
 * to startup main entry of ZFFramework,
 * ZFMainEntry must be single task and only one instance is allowed\n
 * before use, you must declare in your AndroidManifest.xml like this:
 * @code
 *   <activity
 *       android:name="com.ZFFramework.Android.ZF_impl.ZFMainEntry"
 *       android:label="ZFFramework"
 *       android:launchMode="singleTask" >
 *   </activity>
 * @endcode
 * \n
 * typical start steps:
 * -  start your launcher activity
 * -  load any so files of ZFFramework and your app's cpp code
 * -  start ZFMainEntry as single task activity
 * -  finish your launcher activity
 *
 * if you do not want the extra main entry activity,
 * you may use ZFMainEntry.mainEntryAttach/ZFMainEntry.mainEntryDetach
 * to attach to your own existing activity\n
 * \n
 * note, typically, ZFMainEntry only used as entry point and state storage,
 * a ZFUISysWindow would be created to hold actual UI,
 * see #ZFUISysWindow::nativeWindowEmbed
 */
public final class ZFMainEntry extends Activity {
    // ============================================================
    // main entry setup
    public static boolean mainEntryAttached() {
        return (_mainEntryActivity != null && _mainEntryActivity.get() != null);
    }
    public static void mainEntryAttach(Activity activity) {
        _mainEntryActivity = new WeakReference<Activity>(activity);
        ZFFrameworkInit();
        ZFMainExecute(null);
    }
    public static void mainEntryAttach(Activity activity, String[] params) {
        _mainEntryActivity = new WeakReference<Activity>(activity);
        ZFFrameworkInit();
        ZFMainExecute(params);
    }
    public static void mainEntryDetach() {
        ZFFrameworkCleanup();
        _mainEntryActivity = null;
    }

    // ============================================================
    // debug mode
    private static boolean _debugMode = false;
    public static void debugMode(boolean value) {
        _debugMode = value;
        ZFMainEntry.native_debugMode(value);
    }
    public static boolean debugMode() {
        return _debugMode;
    }
    private native static void native_debugMode(boolean value);

    // ============================================================
    // whether we need sdcard permission
    private static boolean _sdcardRW = true;
    public static void sdcardRW(boolean value) {
        _sdcardRW = value;
    }
    public static boolean sdcardRW() {
        return _sdcardRW;
    }

    // ============================================================
    // global state
    private static WeakReference<Application> _app = null;
    public static Application app() {
        if(_app == null || _app.get() == null) {
            _app = new WeakReference<Application>(mainEntryActivity().getApplication());
        }
        return _app.get();
    }
    public static Context appContext() {
        return _mainEntryActivity.get().getApplicationContext();
    }

    public static AssetManager assetManager() {
        return ZFMainEntry.appContext().getAssets();
    }

    private static WeakReference<Activity> _mainEntryActivity = null;
    public static Activity mainEntryActivity() {
        return _mainEntryActivity.get();
    }

    // ============================================================
    // activity entry
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        if(this._requestPermission()) {
            if(!ZFMainEntry.mainEntryAttached()) {
                ZFMainEntry.mainEntryAttach(this);
            }
        }
    }
    @Override
    protected void onDestroy() {
        super.onDestroy();
        if(ZFMainEntry.mainEntryAttached() && ZFMainEntry.mainEntryActivity() == this) {
            ZFMainEntry.mainEntryDetach();
        }
    }

    // ============================================================
    // private
    private boolean _requestPermission() {
        if(Build.VERSION.SDK_INT < Build.VERSION_CODES.M) {
            return true;
        }
        List<String> permissions = new ArrayList<String>();

        if(ZFMainEntry.sdcardRW()) {
            if (this.checkSelfPermission(Manifest.permission.WRITE_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED) {
                permissions.add(Manifest.permission.READ_EXTERNAL_STORAGE);
                permissions.add(Manifest.permission.WRITE_EXTERNAL_STORAGE);
            }
        }

        if(permissions.isEmpty()) {
            return true;
        }
        String[] tmp = new String[permissions.size()];
        permissions.toArray(tmp);
        this.requestPermissions(tmp, 1);
        return false;
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if(!ZFMainEntry.mainEntryAttached()) {
            ZFMainEntry.mainEntryAttach(this);
        }
    }

    // ============================================================
    // native
    private static boolean _mainEntryIsStarted = false;
    private static void ZFFrameworkInit() {
        if(!_mainEntryIsStarted) {
            _mainEntryIsStarted = true;
            native_ZFFrameworkInit();
        }
    }
    private static void ZFFrameworkCleanup() {
        if(_mainEntryIsStarted) {
            _mainEntryIsStarted = false;
            native_ZFFrameworkCleanup();
        }
    }
    private static int ZFMainExecute(String[] params) {
        return native_ZFMainExecute(params);
    }

    // ============================================================
    // native communication
    protected static Object native_appContext() {
        return ZFMainEntry.appContext();
    }
    protected static Object native_assetManager() {
        return ZFMainEntry.assetManager();
    }
    protected static Object native_mainEntryActivity() {
        return ZFMainEntry.mainEntryActivity();
    }
    private native static void native_ZFFrameworkInit();
    private native static void native_ZFFrameworkCleanup();
    private native static int native_ZFMainExecute(String[] params);
}
