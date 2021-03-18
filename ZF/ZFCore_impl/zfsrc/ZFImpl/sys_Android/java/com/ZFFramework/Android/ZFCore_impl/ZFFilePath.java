package com.ZFFramework.Android.ZFCore_impl;

import com.ZFFramework.Android.ZF_impl.ZFMainEntry;
import android.os.Environment;

public final class ZFFilePath {
    public static Object native_pathForModuleFile() {
        try {
            return ZFMainEntry.appContext().getPackageManager().getApplicationInfo(
                ZFMainEntry.appContext().getPackageName(),
                0).publicSourceDir;
        }
        catch(Exception e) {
            e.printStackTrace();
        }
        return null;
    }
    public static Object native_pathForSetting() {
        return ZFMainEntry.appContext().getFilesDir().getAbsolutePath() + "/zfsetting";
    }
    public static Object native_pathForStorage() {
        return ZFMainEntry.appContext().getFilesDir().getAbsolutePath() + "/zfstorage";
    }
    public static Object native_pathForStorageShared() {
        String ret = null;
        if(Environment.MEDIA_MOUNTED.equals(Environment.getExternalStorageState())) {
            ret = Environment.getExternalStorageDirectory().getAbsolutePath();
        }
        else {
            ret = ZFMainEntry.appContext().getFilesDir().getAbsolutePath() + "/zfstorageshared";
        }
        return ret;
    }
    public static Object native_pathForCache() {
        return ZFMainEntry.appContext().getCacheDir().getAbsolutePath() + "/zfcache";
    }
}
