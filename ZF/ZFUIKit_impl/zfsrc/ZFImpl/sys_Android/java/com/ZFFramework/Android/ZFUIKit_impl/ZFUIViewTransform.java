package com.ZFFramework.Android.ZFUIKit_impl;

import android.view.View;

public class ZFUIViewTransform {
    public static void native_viewTransform(Object nativeView,
                                            float viewTranslateX,
                                            float viewTranslateY,
                                            float viewScaleX,
                                            float viewScaleY,
                                            float viewRotate) {
        View nativeViewTmp = (View) nativeView;
        nativeViewTmp.setTranslationX(viewTranslateX);
        nativeViewTmp.setTranslationY(viewTranslateY);
        nativeViewTmp.setScaleX(viewScaleX);
        nativeViewTmp.setScaleY(viewScaleY);
        nativeViewTmp.setRotation(viewRotate);
    }
}
