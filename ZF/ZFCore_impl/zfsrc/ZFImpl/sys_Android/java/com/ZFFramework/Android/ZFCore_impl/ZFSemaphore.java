package com.ZFFramework.Android.ZFCore_impl;

import java.util.concurrent.Semaphore;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public final class ZFSemaphore {
    private int _waiterCount = 0;
    private Semaphore _sema = new Semaphore(0);
    private Lock _paramLocker = new ReentrantLock();

    public Object zfnativeImpl() {
        return _sema;
    }

    protected void native_semaphoreLock() {
        _paramLocker.lock();
    }
    protected void native_semaphoreUnlock() {
        _paramLocker.unlock();
    }

    protected void native_semaphoreSignal() {
        if(_waiterCount > 0) {
            --_waiterCount;
            _paramLocker.unlock();
            _sema.release();
            _paramLocker.lock();
        }
    }
    protected void native_semaphoreBroadcast() {
        if(_waiterCount > 0) {
            int tmp = _waiterCount;
            _waiterCount = 0;
            _paramLocker.unlock();
            _sema.release(tmp);
        }
        _paramLocker.lock();
    }
    protected void native_semaphoreWait() {
        ++_waiterCount;
        _paramLocker.unlock();
        try {
            _sema.acquire();
        }
        catch(InterruptedException e) {
            e.printStackTrace();
            --_waiterCount;
        }
        _paramLocker.lock();
    }
    protected boolean native_semaphoreWait(long miliSecs) {
        ++_waiterCount;
        _paramLocker.unlock();
        boolean result = false;
        try {
            result = _sema.tryAcquire(miliSecs, TimeUnit.MILLISECONDS);
        }
        catch(InterruptedException e) {
            e.printStackTrace();
        }
        _paramLocker.lock();
        if(!result) {
            --_waiterCount;
        }
        return result;
    }
}
