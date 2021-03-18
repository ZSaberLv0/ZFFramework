package com.ZFFramework.Android.NativeUtil;

import android.annotation.SuppressLint;

public final class ZFAndroidPoint {
    public static final ZFAndroidPoint Zero = new ZFAndroidPoint();

    public int x = 0;
    public int y = 0;

    @SuppressLint("DefaultLocale")
    @Override
    public String toString() {
        return String.format("(%d, %d)", this.x, this.y);
    }

    public ZFAndroidPoint() {
    }
    public ZFAndroidPoint(int x, int y) {
        this.x = x;
        this.y = y;
    }
    public ZFAndroidPoint(ZFAndroidPoint point) {
        if(point != null) {
            this.x = point.x;
            this.y = point.y;
        }
    }

    public void set(int x, int y) {
        this.x = x;
        this.y = y;
    }
    public void set(ZFAndroidPoint point) {
        if(point == null) {
            this.x = 0;
            this.y = 0;
        }
        else {
            this.x = point.x;
            this.y = point.y;
        }
    }
}
