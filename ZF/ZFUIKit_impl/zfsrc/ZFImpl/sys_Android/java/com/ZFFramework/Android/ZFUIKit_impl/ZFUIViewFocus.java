package com.ZFFramework.Android.ZFUIKit_impl;

import java.lang.ref.WeakReference;
import android.content.Context;
import android.view.View;
import android.view.View.OnFocusChangeListener;
import android.view.ViewParent;
import android.view.inputmethod.InputMethodManager;
import android.widget.EditText;

public class ZFUIViewFocus {
    private static WeakReference<ZFUIView> _focusedView = null;
    public static ZFUIView focusedView() {
        return ((_focusedView != null) ? _focusedView.get() : null);
    }

    private static OnFocusChangeListener _onFocusChangeListener = new OnFocusChangeListener() {
        @Override
        public void onFocusChange(View v, boolean hasFocus) {
            if(hasFocus && (v instanceof EditText)) {
                InputMethodManager imm = (InputMethodManager)v.getContext().getSystemService(Context.INPUT_METHOD_SERVICE);
                imm.showSoftInput(v, InputMethodManager.SHOW_IMPLICIT);
            }
            ZFUIView tmp = null;
            if(v instanceof ZFUIView) {
                tmp = (ZFUIView)v;
            }
            else {
                ViewParent parent = v.getParent();
                if(parent instanceof ZFUIView) {
                    tmp = (ZFUIView)parent;
                }
            }
            if(tmp != null && tmp.zfjniPointerOwnerZFUIView != 0) {
                _focusedView = new WeakReference<ZFUIView>(tmp);
                ZFUIViewFocus.native_notifyViewFocusChanged(tmp.zfjniPointerOwnerZFUIView);
            }
        }
    };
    public static void ZFUIViewImplChanged(ZFUIView view, View implViewOld, View implViewNew) {
        if(implViewOld != null) {
            implViewOld.setOnFocusChangeListener(null);
        }
        if(implViewNew != null) {
            implViewNew.setFocusable(view.isFocusable());
            implViewNew.setFocusableInTouchMode(view.isFocusableInTouchMode());
            if(view.isFocusable()) {
                implViewNew.setOnFocusChangeListener(_onFocusChangeListener);
            }
        }
    }
    public static void native_viewFocusable(Object nativeView, boolean viewFocusable) {
        ZFUIView nativeViewTmp = (ZFUIView)nativeView;
        View target = nativeViewTmp;
        if(nativeViewTmp.nativeImplView != null) {
            target = nativeViewTmp.nativeImplView;
        }

        nativeViewTmp.setFocusable(viewFocusable);
        nativeViewTmp.setFocusableInTouchMode(viewFocusable);
        target.setFocusable(viewFocusable);
        target.setFocusableInTouchMode(viewFocusable);
        if(viewFocusable) {
            nativeViewTmp.setOnFocusChangeListener(_onFocusChangeListener);
            target.setOnFocusChangeListener(_onFocusChangeListener);
        }
        else {
            nativeViewTmp.setOnFocusChangeListener(null);
            target.setOnFocusChangeListener(null);
        }
    }
    public static boolean native_viewFocused(Object nativeView) {
        ZFUIView nativeViewTmp = (ZFUIView)nativeView;
        return (nativeViewTmp.isFocused()
            || (nativeViewTmp.nativeImplView != null && nativeViewTmp.nativeImplView.isFocused()));
    }
    public static long native_viewFocusFind(Object nativeView) {
        ZFUIView nativeViewTmp = (ZFUIView)nativeView;
        View child = nativeViewTmp.findFocus();
        if(child == null) {
            return 0;
        }
        while(child != null) {
            if(child instanceof ZFUIView) {
                return ((ZFUIView)child).zfjniPointerOwnerZFUIView;
            }
            if(child.getParent() instanceof View) {
                child = (View)child.getParent();
            }
            else {
                break;
            }
        }
        return 0;
    }
    public static void native_viewFocusRequest(Object nativeView, boolean viewFocus) {
        ZFUIView nativeViewTmp = (ZFUIView)nativeView;
        View target = nativeViewTmp;
        if(nativeViewTmp.nativeImplView != null && nativeViewTmp.nativeImplView.isFocusable()) {
            target = nativeViewTmp.nativeImplView;
        }

        if(viewFocus) {
            target.requestFocus();
        }
        else {
            target.clearFocus();
        }
    }

    public static native void native_notifyViewFocusChanged(long zfjniPointerOwnerZFUIView);
}
