package com.ZFFramework.Android.ZFUIKit_impl;

import android.content.Context;
import android.graphics.Canvas;
import android.util.AttributeSet;
import android.view.View;

import com.ZFFramework.Android.ZF_impl.ZFMainEntry;

public final class ZFUIDrawableView extends View {

    public long zfjniPointerOwnerZFUIDrawableView = 0;

    // ============================================================
    public static Object native_nativeDrawableViewCreate(long zfjniPointerOwnerZFUIDrawableView) {
        ZFUIDrawableView ret = new ZFUIDrawableView(ZFMainEntry.mainEntryActivity());
        ret.zfjniPointerOwnerZFUIDrawableView = zfjniPointerOwnerZFUIDrawableView;
        return ret;
    }

    public static void native_drawRequest(Object nativeDrawableView) {
        ZFUIDrawableView nativeDrawableViewTmp = (ZFUIDrawableView) nativeDrawableView;
        nativeDrawableViewTmp.invalidate();
    }

    public static Object native_beginForView(Object nativeDrawableView) {
        ZFUIDrawableView nativeDrawableViewTmp = (ZFUIDrawableView) nativeDrawableView;
        return nativeDrawableViewTmp._canvasSaved;
    }

    private Canvas _canvasSaved = null;

    public static native void native_notifyOnDraw(long zfjniPointerOwnerZFUIDrawableView);

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        _canvasSaved = canvas;
        ZFUIDrawableView.native_notifyOnDraw(this.zfjniPointerOwnerZFUIDrawableView);
        _canvasSaved = null;
    }

    // ============================================================
    public ZFUIDrawableView(Context context) {
        super(context);
    }

    public ZFUIDrawableView(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    public ZFUIDrawableView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
    }

    public ZFUIDrawableView(Context context, AttributeSet attrs, int defStyleAttr, int defStyleRes) {
        super(context, attrs, defStyleAttr, defStyleRes);
    }
}
