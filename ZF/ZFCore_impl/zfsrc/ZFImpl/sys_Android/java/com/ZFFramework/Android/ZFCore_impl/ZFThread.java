package com.ZFFramework.Android.ZFCore_impl;

import android.os.Handler;
import android.os.Message;

public final class ZFThread {
    // ============================================================
    // common
    private static long currentThreadId() {
        return Thread.currentThread().getId();
    }

    // ============================================================
    // executeInMainThread
    private static Handler _mainThreadHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            native_doExecuteInMainThread(msg.what, ZFThread.currentThreadId());
        }
    };
    protected static void native_executeInMainThread(int executeDataId) {
        _mainThreadHandler.sendEmptyMessage(executeDataId);
    }
    private native static void native_doExecuteInMainThread(int executeDataId, long sysThread);

    // ============================================================
    // executeInNewThread
    private static class _NewThread extends Thread {
        public int executeDataId = 0;

        public _NewThread(int executeDataId) {
            this.executeDataId = executeDataId;
        }

        @Override
        public void run() {
            super.run();
            native_doExecuteInNewThread(this.executeDataId, ZFThread.currentThreadId());
        }
    }
    protected static void native_executeInNewThread(int executeDataId) {
        _NewThread thread = new _NewThread(executeDataId);
        thread.start();
    }
    private native static void native_doExecuteInNewThread(int executeDataId, long sysThread);

    // ============================================================
    // executeInMainThreadAfterDelay
    private static class _ExecuteInMainThreadAfterDelayData {
        public boolean running = true;
        public long zfjniPointerNativeData = 0;
    }
    private static Handler _executeInMainThreadAfterDelayHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            _ExecuteInMainThreadAfterDelayData nativeToken = (_ExecuteInMainThreadAfterDelayData)msg.obj;
            if(nativeToken.running) {
                native_doExecuteInMainThreadAfterDelay(nativeToken.zfjniPointerNativeData);
                native_executeInMainThreadAfterDelayCleanup(nativeToken.zfjniPointerNativeData);
                nativeToken.zfjniPointerNativeData = 0;
            }
        }
    };
    protected static Object native_executeInMainThreadAfterDelay(long delay, long zfjniPointerNativeData) {
        _ExecuteInMainThreadAfterDelayData nativeToken = new _ExecuteInMainThreadAfterDelayData();
        nativeToken.zfjniPointerNativeData = zfjniPointerNativeData;
        _executeInMainThreadAfterDelayHandler.sendMessageDelayed(
            Message.obtain(_executeInMainThreadAfterDelayHandler, 0, nativeToken),
            delay);
        return nativeToken;
    }
    protected static void native_executeInMainThreadAfterDelayCancel(Object nativeToken) {
        _ExecuteInMainThreadAfterDelayData nativeTokenTmp = (_ExecuteInMainThreadAfterDelayData)nativeToken;
        if(nativeTokenTmp.running) {
            nativeTokenTmp.running = false;
            _executeInMainThreadAfterDelayHandler.removeMessages(0, nativeTokenTmp);
            native_executeInMainThreadAfterDelayCleanup(nativeTokenTmp.zfjniPointerNativeData);
            nativeTokenTmp.zfjniPointerNativeData = 0;
        }
    }
    private native static void native_doExecuteInMainThreadAfterDelay(long zfjniPointerNativeData);
    private native static void native_executeInMainThreadAfterDelayCleanup(long zfjniPointerNativeData);

    // ============================================================
    // other native communication
    protected static long native_currentThread() {
        return ZFThread.currentThreadId();
    }
    protected static void native_sleep(long miliSecs) {
        try {
            Thread.sleep(miliSecs);
        }
        catch(InterruptedException e) {
            e.printStackTrace();
        }
    }
}
