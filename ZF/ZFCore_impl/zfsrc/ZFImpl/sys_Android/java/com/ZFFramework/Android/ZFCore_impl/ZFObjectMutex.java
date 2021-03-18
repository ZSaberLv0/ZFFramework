package com.ZFFramework.Android.ZFCore_impl;

import java.util.concurrent.locks.ReentrantLock;

public final class ZFObjectMutex {
    public static Object native_nativeMutexCreate() {
        return new ReentrantLock();
    }
    public static void native_mutexLock(Object nativeMutex) {
        ((ReentrantLock)nativeMutex).lock();
    }
    public static boolean native_mutexTryLock(Object nativeMutex) {
        return ((ReentrantLock)nativeMutex).tryLock();
    }
    public static void native_mutexUnlock(Object nativeMutex) {
        ((ReentrantLock)nativeMutex).unlock();
    }
}
