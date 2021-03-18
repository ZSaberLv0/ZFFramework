package com.ZFFramework.Android.NativeUtil;

import android.annotation.SuppressLint;

public final class ZFAndroidRect {
    public static final ZFAndroidRect Zero = new ZFAndroidRect();

    public int x = 0;
    public int y = 0;
    public int width = 0;
    public int height = 0;

    @SuppressLint("DefaultLocale")
    @Override
    public String toString() {
        return String.format("(%d, %d, %d, %d)", this.x, this.y, this.width, this.height);
    }

    public ZFAndroidRect() {
    }
    public ZFAndroidRect(ZFAndroidRect rect) {
        if(rect != null) {
            this.x = rect.x;
            this.y = rect.y;
            this.width = rect.width;
            this.height = rect.height;
        }
    }
    public ZFAndroidRect(int x, int y, int width, int height) {
        this.x = x;
        this.y = y;
        this.width = width;
        this.height = height;
    }

    public void set(ZFAndroidRect rect) {
        if(rect != null) {
            this.x = rect.x;
            this.y = rect.y;
            this.width = rect.width;
            this.height = rect.height;
        }
    }
    public void set(int x, int y, int width, int height) {
        this.x = x;
        this.y = y;
        this.width = width;
        this.height = height;
    }
}
