package com.ZFFramework.Android.NativeUtil;

import com.ZFFramework.Android.ZF_impl.ZFMainEntry;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Rect;

public class ZFAndroidNinePatch {
    private static final Rect _srcRectCache = new Rect();
    private static final Rect _dstRectCache = new Rect();
    public static void drawNinePatch(Bitmap src,
                                     float imageScale,
                                     int srcNinePatchLeft,
                                     int srcNinePatchTop,
                                     int srcNinePatchRight,
                                     int srcNinePatchBottom,
                                     Canvas dst,
                                     int dstWidth,
                                     int dstHeight) {
        float uiScale = ZFAndroidUI.screenDensity(ZFMainEntry.appContext());
        int srcLeft = (int)(srcNinePatchLeft / imageScale);
        int srcTop = (int)(srcNinePatchTop / imageScale);
        int srcRight = (int)(srcNinePatchRight / imageScale);
        int srcBottom = (int)(srcNinePatchBottom / imageScale);
        int srcWidth = (int)(src.getWidth() / imageScale);
        int srcHeight = (int)(src.getHeight() / imageScale);
        if(srcWidth <= 0 || srcHeight <= 0 || dstWidth <= 0 || dstHeight <= 0) {
            return ;
        }
        if(srcLeft + srcRight >= dstWidth) {
            srcLeft = 0;
            srcRight = 0;
        }
        if(srcTop + srcBottom >= dstHeight) {
            srcTop = 0;
            srcBottom = 0;
        }
        if(srcLeft == 0 && srcRight == 0 && srcTop == 0 && srcBottom == 0) {
            _srcRectCache.set(0, 0, src.getWidth(), src.getHeight());
            _dstRectCache.set(0, 0, dstWidth, dstHeight);
            dst.drawBitmap(src, _srcRectCache, _dstRectCache, null);
            return ;
        }

        int leftDst = (int)(srcLeft * uiScale);
        int topDst = (int)(srcTop * uiScale);
        int rightDst = (int)(srcRight * uiScale);
        int bottomDst = (int)(srcBottom * uiScale);
        if(leftDst + rightDst > dstWidth) {
            leftDst = dstWidth * leftDst / srcWidth;
            rightDst = dstWidth * rightDst / srcWidth;
        }
        if(topDst + bottomDst > dstHeight) {
            topDst = dstHeight * topDst / srcHeight;
            bottomDst = dstHeight * bottomDst / srcHeight;
        }

        // top
        if(srcTop > 0) {
            // left
            if(srcLeft > 0) {
                _srcRectCache.set(0, 0, srcNinePatchLeft, srcNinePatchTop);
                _dstRectCache.set(0, 0, leftDst, topDst);
                dst.drawBitmap(src, _srcRectCache, _dstRectCache, null);
            }
            // center
            _srcRectCache.set(srcNinePatchLeft, 0, src.getWidth() - srcNinePatchRight, srcNinePatchTop);
            _dstRectCache.set(leftDst, 0, dstWidth - rightDst, topDst);
            dst.drawBitmap(src, _srcRectCache, _dstRectCache, null);
            // right
            if(srcRight > 0) {
                _srcRectCache.set(src.getWidth() - srcNinePatchRight, 0, src.getWidth(), srcNinePatchTop);
                _dstRectCache.set(dstWidth - rightDst, 0, dstWidth, topDst);
                dst.drawBitmap(src, _srcRectCache, _dstRectCache, null);
            }
        }
        // center
        {
            // left
            if(srcLeft > 0) {
                _srcRectCache.set(0, srcNinePatchTop, srcNinePatchLeft, src.getHeight() - srcNinePatchBottom);
                _dstRectCache.set(0, topDst, leftDst, dstHeight - bottomDst);
                dst.drawBitmap(src, _srcRectCache, _dstRectCache, null);
            }
            // center
            _srcRectCache.set(srcNinePatchLeft, srcNinePatchTop, src.getWidth() - srcNinePatchRight, src.getHeight() - srcNinePatchBottom);
            _dstRectCache.set(leftDst, topDst, dstWidth - rightDst, dstHeight - bottomDst);
            dst.drawBitmap(src, _srcRectCache, _dstRectCache, null);
            // right
            if(srcRight > 0) {
                _srcRectCache.set(src.getWidth() - srcNinePatchRight, srcNinePatchTop, src.getWidth(), src.getHeight() - srcNinePatchBottom);
                _dstRectCache.set(dstWidth - rightDst, topDst, dstWidth, dstHeight - bottomDst);
                dst.drawBitmap(src, _srcRectCache, _dstRectCache, null);
            }
        }
        // bottom
        if(srcBottom > 0) {
            // left
            if(srcLeft > 0) {
                _srcRectCache.set(0, src.getHeight() - srcNinePatchBottom, srcNinePatchLeft, src.getHeight());
                _dstRectCache.set(0, dstHeight - bottomDst, leftDst, dstHeight);
                dst.drawBitmap(src, _srcRectCache, _dstRectCache, null);
            }
            // center
            _srcRectCache.set(srcNinePatchLeft, src.getHeight() - srcNinePatchBottom, src.getWidth() - srcNinePatchRight, src.getHeight());
            _dstRectCache.set(leftDst, dstHeight - bottomDst, dstWidth - rightDst, dstHeight);
            dst.drawBitmap(src, _srcRectCache, _dstRectCache, null);
            // right
            if(srcRight > 0) {
                _srcRectCache.set(src.getWidth() - srcNinePatchRight, src.getHeight() - srcNinePatchBottom, src.getWidth(), src.getHeight());
                _dstRectCache.set(dstWidth - rightDst, dstHeight - bottomDst, dstWidth, dstHeight);
                dst.drawBitmap(src, _srcRectCache, _dstRectCache, null);
            }
        }
    }
}
