package com.ZFFramework.Android.NativeUtil;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.ByteBuffer;

/*
 * this class is designed to pass raw memory block from/to JNI for performance
 * the passed buffer must be used directly and can not be stored for future use
 *
 * pass from Java to JNI:
 * 1. new ZFAndroidBuffer()
 * 2. buffer.toOutputStream() and write to it
 * 3. buffer.bufferReadyToRead()
 * 4. pass buffer to JNI, and access raw pointer and buffer size by
 *    ZFImpl_sys_Android_ZFAndroidBufferFromJava
 *
 * pass from JNI to Java:
 * 1. ZFImpl_sys_Android_ZFAndroidBufferToJava
 * 2. pass buffer to Java, and access buffer.toInputStream() to read from it
 */
public class ZFAndroidBuffer {
    public ZFAndroidBuffer() {
    }
    public ZFAndroidBuffer(int bufferCapacity) {
        this.bufferRealloc(bufferCapacity);
    }

    public void bufferRealloc(int bufferSize) {
        if(this.byteBuffer == null) {
            this.byteBuffer = ByteBuffer.allocateDirect(Math.max(32, bufferSize));
            return ;
        }
        if(bufferSize <= this.byteBuffer.capacity()) {
            return ;
        }
        bufferSize = Math.max(this.byteBuffer.capacity() * 2, bufferSize);
        ByteBuffer bufferTmp = ByteBuffer.allocateDirect(bufferSize);
        this.byteBuffer.flip();
        bufferTmp.put(this.byteBuffer);
        this.byteBuffer = bufferTmp;
    }
    public void bufferCheckIncrease(int incSize) {
        if(this.byteBuffer != null) {
            incSize += this.byteBuffer.position();
        }
        this.bufferRealloc(incSize);
    }

    public void bufferReadyToRead() {
        this.byteBuffer.flip();
    }

    public int bufferSize() {
        return this.byteBuffer.remaining();
    }

    public InputStream toInputStream() {
        return new _BufferInputStream(this);
    }

    public OutputStream toOutputStream() {
        return new _BufferOutputStream(this);
    }

    @Override
    public String toString() {
        return "" + this.byteBuffer;
    }

    // ============================================================
    protected ByteBuffer byteBuffer = null;
    protected ZFAndroidBuffer(ByteBuffer byteBuffer) {
        this.byteBuffer = byteBuffer;
    }
    protected static Object native_bufferForByteBuffer(Object byteBuffer) {
        return new ZFAndroidBuffer((ByteBuffer)byteBuffer);
    }
    protected static Object native_buffer(Object buffer) {
        return ((ZFAndroidBuffer)buffer).byteBuffer;
    }
    protected static int native_bufferSize(Object buffer) {
        return ((ZFAndroidBuffer)buffer).byteBuffer.remaining();
    }

    protected static class _BufferInputStream extends InputStream {
        private ZFAndroidBuffer _owner;
        _BufferInputStream(ZFAndroidBuffer buf) {
            this._owner = buf;
        }
        @Override
        public synchronized int read() throws IOException {
            if(!_owner.byteBuffer.hasRemaining()) {
                return -1;
            }
            return _owner.byteBuffer.get();
        }
        @Override
        public synchronized int read(byte[] bytes, int off, int len) throws IOException {
            if(_owner.byteBuffer.remaining() <= 0)
            {
                return -1;
            }
            len = Math.min(len, _owner.byteBuffer.remaining());
            _owner.byteBuffer.get(bytes, off, len);
            return len;
        }
    }
    protected static class _BufferOutputStream extends OutputStream {
        private ZFAndroidBuffer _owner;
        _BufferOutputStream(ZFAndroidBuffer buf) {
            this._owner = buf;
        }
        public synchronized void write(int b) throws IOException {
            _owner.bufferCheckIncrease(1);
            _owner.byteBuffer.put((byte)b);
        }
        public synchronized void write(byte[] bytes, int offset, int len) throws IOException {
            _owner.bufferCheckIncrease(len);
            _owner.byteBuffer.put(bytes, offset, len);
        }
    }
}
