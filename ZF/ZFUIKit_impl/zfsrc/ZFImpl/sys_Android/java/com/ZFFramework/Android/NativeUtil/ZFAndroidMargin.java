package com.ZFFramework.Android.NativeUtil;

import android.annotation.SuppressLint;

public final class ZFAndroidMargin {
    public static final ZFAndroidMargin Zero = new ZFAndroidMargin();

    public int left = 0;
    public int top = 0;
    public int right = 0;
    public int bottom = 0;

    @SuppressLint("DefaultLocale")
    @Override
    public String toString() {
        return String.format("(%d, %d, %d, %d)", this.left, this.top, this.right, this.bottom);
    }

    public ZFAndroidMargin() {
    }
    public ZFAndroidMargin(ZFAndroidMargin margin) {
        if(margin != null) {
            this.left = margin.left;
            this.top = margin.top;
            this.right = margin.right;
            this.bottom = margin.bottom;
        }
    }
    public ZFAndroidMargin(int margin) {
        this.left = margin;
        this.top = margin;
        this.right = margin;
        this.bottom = margin;
    }
    public ZFAndroidMargin(int x, int y) {
        this.left = x;
        this.top = y;
        this.right = x;
        this.bottom = y;
    }
    public ZFAndroidMargin(int left, int top, int right, int bottom) {
        this.left = left;
        this.top = top;
        this.right = right;
        this.bottom = bottom;
    }

    public void set(ZFAndroidMargin margin) {
        if(margin != null) {
            this.left = margin.left;
            this.top = margin.top;
            this.right = margin.right;
            this.bottom = margin.bottom;
        }
    }
    public void set(int margin) {
        this.left = margin;
        this.top = margin;
        this.right = margin;
        this.bottom = margin;
    }
    public void set(int x, int y) {
        this.left = x;
        this.top = y;
        this.right = x;
        this.bottom = y;
    }
    public void set(int left, int top, int right, int bottom) {
        this.left = left;
        this.top = top;
        this.right = right;
        this.bottom = bottom;
    }
}
