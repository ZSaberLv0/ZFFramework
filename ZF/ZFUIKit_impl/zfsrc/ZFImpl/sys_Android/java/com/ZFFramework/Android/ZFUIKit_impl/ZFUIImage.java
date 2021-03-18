package com.ZFFramework.Android.ZFUIKit_impl;

import com.ZFFramework.Android.NativeUtil.ZFAndroidBuffer;
import android.graphics.Bitmap;
import android.graphics.Bitmap.CompressFormat;
import android.graphics.BitmapFactory;
import android.graphics.drawable.BitmapDrawable;


public final class ZFUIImage {
    @SuppressWarnings("deprecation")
    public static Object native_nativeImageFromInput(Object buf) {
        Bitmap bitmap = BitmapFactory.decodeStream(((ZFAndroidBuffer)buf).toInputStream());
        if(bitmap == null) {
            return null;
        }
        else {
            return new BitmapDrawable(bitmap);
        }
    }
    public static Object native_nativeImageToOutput(Object nativeImage) {
        BitmapDrawable src = (BitmapDrawable)nativeImage;
        ZFAndroidBuffer buffer = new ZFAndroidBuffer(1024);
        src.getBitmap().compress(CompressFormat.PNG, 1, buffer.toOutputStream());
        buffer.bufferReadyToRead();
        return buffer;
    }
    @SuppressWarnings("deprecation")
    public static Object native_nativeImageCopy(Object nativeImage) {
        BitmapDrawable src = (BitmapDrawable)nativeImage;
        Bitmap dst = src.getBitmap().copy(Bitmap.Config.ARGB_8888, true);
        return new BitmapDrawable(dst);
    }
    public static int[] native_nativeImageSize(Object nativeImage) {
        BitmapDrawable src = (BitmapDrawable)nativeImage;
        return new int[] {src.getBitmap().getWidth(), src.getBitmap().getHeight()};
    }
}
