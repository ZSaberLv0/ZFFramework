package com.ZFFramework.Android.ZFUIKit_impl;

import android.view.View;

public class ZFUIViewTransform {
    public static void native_viewTransform(Object nativeView,
                                            float viewScaleX,
                                            float viewScaleY,
                                            float viewRotation) {
        View nativeViewTmp = (View) nativeView;
        nativeViewTmp.setScaleX(viewScaleX);
        nativeViewTmp.setScaleY(viewScaleY);
        nativeViewTmp.setRotation(viewRotation);
    }
}
