package com.ZFFramework.Android.ZFUIKit_impl;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.PaintFlagsDrawFilter;
import android.graphics.PorterDuff;
import android.graphics.PorterDuffXfermode;
import android.graphics.Rect;
import android.graphics.drawable.BitmapDrawable;

import com.ZFFramework.Android.ZF_impl.ZFMainEntry;

public final class ZFUIDraw {

    public static class NativeToken {
        public Object canvas = null;
        public Object image = null;
    }

    // ============================================================
    // ZFUIDrawForImage
    public static NativeToken native_beginForImage(int imageSizePixel_width, int imageSizePixel_height) {
        NativeToken nativeToken = new NativeToken();
        Bitmap bitmap = Bitmap.createBitmap(imageSizePixel_width, imageSizePixel_height, Bitmap.Config.ARGB_8888);
        nativeToken.canvas = new Canvas(bitmap);
        nativeToken.image = new BitmapDrawable(ZFMainEntry.appContext().getResources(), bitmap);
        return nativeToken;
    }

    // ============================================================
    // ZFUIDraw
    public static void native_antialiasing(Object canvas, boolean antialiasing) {
        Canvas canvasTmp = (Canvas) canvas;
        canvasTmp.setDrawFilter(antialiasing
                ? new PaintFlagsDrawFilter(0, Paint.ANTI_ALIAS_FLAG | Paint.FILTER_BITMAP_FLAG)
                : null
        );
    }

    private static Paint _drawClear_paintCache_prepare() {
        Paint ret = new Paint();
        ret.setXfermode(new PorterDuffXfermode(PorterDuff.Mode.CLEAR));
        return ret;
    }

    private static Paint _drawClear_paintCache = _drawClear_paintCache_prepare();

    public static void native_drawClear(Object canvas
            , int targetFramePixel_x
            , int targetFramePixel_y
            , int targetFramePixel_width
            , int targetFramePixel_height
    ) {
        ((Canvas) canvas).drawRect(
                targetFramePixel_x, targetFramePixel_y, targetFramePixel_width, targetFramePixel_height,
                _drawClear_paintCache);
    }

    private static Paint _drawColor_paintCache_prepare() {
        Paint ret = new Paint();
        return ret;
    }

    private static Paint _drawColor_paintCache = _drawColor_paintCache_prepare();

    public static void native_drawColor(Object canvas
            , int color
            , int targetFramePixel_x
            , int targetFramePixel_y
            , int targetFramePixel_width
            , int targetFramePixel_height
    ) {
        _drawColor_paintCache.setColor(color);
        ((Canvas) canvas).drawRect(
                targetFramePixel_x, targetFramePixel_y, targetFramePixel_width, targetFramePixel_height,
                _drawColor_paintCache);
    }

    private static Paint _drawImage_paintCache_prepare() {
        Paint ret = new Paint();
        return ret;
    }

    private static Paint _drawImage_paintCache = _drawImage_paintCache_prepare();
    private static Rect _drawImage_rectCache_src = new Rect();
    private static Rect _drawImage_rectCache_dst = new Rect();

    public static void native_drawImage(Object canvas
            , Object nativeImage
            , int imageFramePixel_x
            , int imageFramePixel_y
            , int imageFramePixel_width
            , int imageFramePixel_height
            , int targetFramePixel_x
            , int targetFramePixel_y
            , int targetFramePixel_width
            , int targetFramePixel_height
    ) {
        _drawImage_rectCache_src.set(
                imageFramePixel_x,
                imageFramePixel_y,
                imageFramePixel_x + imageFramePixel_width,
                imageFramePixel_y + imageFramePixel_height);
        _drawImage_rectCache_dst.set(
                targetFramePixel_x,
                targetFramePixel_y,
                targetFramePixel_x + targetFramePixel_width,
                targetFramePixel_y + targetFramePixel_height);
        ((Canvas) canvas).drawBitmap(
                ((BitmapDrawable) nativeImage).getBitmap(),
                _drawImage_rectCache_src,
                _drawImage_rectCache_dst,
                _drawImage_paintCache);
    }

}
