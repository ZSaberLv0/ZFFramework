package com.ZFFramework.Android.ZFUIKit_impl;

import android.view.View;

public class ZFUIViewPositionOnScreen {
    private static int[] _viewPositionOnScreenCache = new int[4];
    public static int[] native_viewPositionOnScreen(Object nativeView) {
        View nativeViewTmp = (View)nativeView;
        nativeViewTmp.getLocationOnScreen(_viewPositionOnScreenCache);
        _viewPositionOnScreenCache[2] = nativeViewTmp.getWidth();
        _viewPositionOnScreenCache[3] = nativeViewTmp.getHeight();
        return _viewPositionOnScreenCache;
    }
}
