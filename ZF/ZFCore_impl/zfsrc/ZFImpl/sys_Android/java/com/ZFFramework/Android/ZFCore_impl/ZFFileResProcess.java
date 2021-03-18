package com.ZFFramework.Android.ZFCore_impl;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import com.ZFFramework.Android.ZF_impl.ZFMainEntry;
import android.content.res.AssetManager;

public final class ZFFileResProcess {
    private static final String _resPostfix = ".mp2";

    public static String[] native_listAssets(String resPath) {
        try {
            return ZFMainEntry.assetManager().list(resPath);
        }
        catch(IOException e) {
            return null;
        }
    }

    public static boolean native_resIsExist(String resPath) {
        try {
            String[] files = ZFMainEntry.assetManager().list(resPath);
            if(files.length > 0) {
                return true;
            }
        }
        catch(Exception e) {
        }

        try {
            InputStream is = ZFMainEntry.assetManager().open(resPath);
            is.close();
            return true;
        }
        catch(Exception e) {
        }

        try {
            InputStream is = ZFMainEntry.assetManager().open(resPath + _resPostfix);
            is.close();
            return true;
        }
        catch(Exception e) {
        }

        return false;
    }

    public static boolean native_resIsDir(String resPath) {
        try {
            String[] files = ZFMainEntry.assetManager().list(resPath);
            if(files.length > 0) {
                return true;
            }
        }
        catch(Exception e) {
        }

        return false;
    }

    public static String native_resCopy(String resPath,
                                        String dstPath,
                                        boolean isRecursive,
                                        boolean isForce) {
        AssetManager assetManager = ZFMainEntry.assetManager();
        boolean isDir = false;
        try {
            String[] files = assetManager.list(resPath);
            if(files.length > 0) {
                isDir = true;
            }
        }
        catch(Exception e) {
        }
        if(isDir) {
            return native_resCopy_dir(assetManager, resPath, dstPath, isRecursive, isForce);
        }
        else {
            if(native_resCopy_file(assetManager, resPath, dstPath, isRecursive, isForce)) {
                return null;
            }
            else {
                return resPath;
            }
        }
    }

    private static String native_resCopy_dir(AssetManager assetManager,
                                             String resPath,
                                             String dstPath,
                                             boolean isRecursive,
                                             boolean isForce) {
        if(!isRecursive) {
            return resPath;
        }
        File dstFd = new File(dstPath);
        if(isForce) {
            try {
                native_resCopy_rm(dstFd);
            }
            catch(Exception e) {
            }
        }
        String[] files = null;
        try {
            files = assetManager.list(resPath);
            dstFd.mkdirs();
        }
        catch(Exception e) {
            return resPath;
        }
        for(String file : files) {
            String fromAbsPath = String.format("%s/%s", resPath, file);
            InputStream is = native_resCopy_tryReadFile(assetManager, fromAbsPath);
            String toAbsPath = String.format("%s/%s", dstPath, file);
            if(is == null) {
                String tmp = native_resCopy_dir(assetManager, fromAbsPath, toAbsPath, isRecursive, isForce);
                if(tmp != null) {
                    return tmp;
                }
            }
            else {
                try {
                    is.close();
                }
                catch(IOException e) {
                }
                if(!native_resCopy_file(assetManager, fromAbsPath, toAbsPath, isRecursive, isForce)) {
                    return fromAbsPath;
                }
            }
        }
        return null;
    }

    private static boolean native_resCopy_file(AssetManager assetManager,
                                               String resPath,
                                               String dstPath,
                                               boolean isRecursive,
                                               boolean isForce) {
        File dstFd = new File(dstPath);
        if(isForce) {
            try {
                native_resCopy_rm(dstFd);
            }
            catch(Exception e) {
            }
        }
        InputStream in = null;
        OutputStream out = null;
        try {
            in = assetManager.open(resPath);
            dstFd.createNewFile();
            out = new FileOutputStream(dstPath);
            byte[] buffer = new byte[1024];
            int read;
            while((read = in.read(buffer)) != -1) {
                out.write(buffer, 0, read);
            }
            in.close();
            in = null;
            out.flush();
            out.close();
            out = null;
            return true;
        }
        catch(Exception e) {
            return false;
        }
    }

    private static void native_resCopy_rm(File fileOrDirectory) {
        if(fileOrDirectory.isDirectory()) {
            for(File child : fileOrDirectory.listFiles()) {
                native_resCopy_rm(child);
            }
        }
        fileOrDirectory.delete();
    }
    private static InputStream native_resCopy_tryReadFile(AssetManager assetManager,
                                                          String resPath) {
        InputStream ret = null;
        try {
            ret = assetManager.open(resPath);
            if(ret == null) {
                ret = assetManager.open(String.format("%s%s", resPath, _resPostfix));
            }
        }
        catch(Exception e) {
        }
        return ret;
    }
}
