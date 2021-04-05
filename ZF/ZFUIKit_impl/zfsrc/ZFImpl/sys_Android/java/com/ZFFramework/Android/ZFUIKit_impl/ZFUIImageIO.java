package com.ZFFramework.Android.ZFUIKit_impl;

import android.graphics.Bitmap;
import android.graphics.Bitmap.Config;
import android.graphics.Canvas;
import android.graphics.drawable.BitmapDrawable;

import com.ZFFramework.Android.NativeUtil.ZFAndroidNinePatch;
import com.ZFFramework.Android.ZF_impl.ZFMainEntry;

public final class ZFUIImageIO {
    public static Object native_imageApplyScale(float imageScale,
                                                Object nativeImage,
                                                int newSize_width, int newSize_height,
                                                int ninePatch_left, int ninePatch_top, int ninePatch_right, int ninePatch_bottom) {
        Bitmap bmp = Bitmap.createBitmap(newSize_width, newSize_height, Config.ARGB_8888);
        Canvas canvas = new Canvas(bmp);
        ZFAndroidNinePatch.drawNinePatch(
                ((BitmapDrawable) nativeImage).getBitmap(),
                1, // src and dst always have same scale
                ninePatch_left, ninePatch_top, ninePatch_right, ninePatch_bottom,
                canvas,
                newSize_width, newSize_height);
        return new BitmapDrawable(ZFMainEntry.appContext().getResources(), bmp);
    }

    public static Object native_imageLoadInFrame(float imageScale,
                                                 Object nativeImage,
                                                 int frame_x, int frame_y, int frame_width, int frame_height) {
        Bitmap bmp = Bitmap.createBitmap(((BitmapDrawable) nativeImage).getBitmap(), frame_x, frame_y, frame_width, frame_height);
        return new BitmapDrawable(ZFMainEntry.appContext().getResources(), bmp);
    }

    public static Object native_imageLoadFromColor(int color,
                                                   int size_width, int size_height) {
        Bitmap bmp = Bitmap.createBitmap(size_width, size_height, Config.ARGB_8888);
        Canvas canvas = new Canvas(bmp);
        canvas.drawColor(color);
        return new BitmapDrawable(ZFMainEntry.appContext().getResources(), bmp);
    }
}
