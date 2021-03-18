
package com.ZFFramework.Android.ZFUIKit_impl;

import com.ZFFramework.Android.NativeUtil.ZFAndroidLog;
import android.graphics.Camera;
import android.graphics.Matrix;
import android.os.Handler;
import android.os.Message;
import android.view.View;
import android.view.animation.Animation;
import android.view.animation.Animation.AnimationListener;
import android.view.animation.AnimationSet;
import android.view.animation.Interpolator;
import android.view.animation.Transformation;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;


public class ZFAnimationNativeView {
    protected static class NativeAnimation extends Animation {
        public NativeAnimation(long zfjniPointerOwnerZFAnimationNativeView) {
            this.setFillEnabled(true);
            this.setFillBefore(true);
            this.setFillAfter(true);
            this.zfjniPointerOwnerZFAnimationNativeView = zfjniPointerOwnerZFAnimationNativeView;
        }

        public long zfjniPointerOwnerZFAnimationNativeView = 0;

        private NativeAnimationListener _animationListener = null;
        public void nativeAnimationListener(NativeAnimationListener listener) {
            if(this._animationListener != null) {
                this._animationListener.nativeAniDetach();
            }
            this._animationListener = listener;
            super.setAnimationListener(listener);
        }
        public NativeAnimationListener nativeAnimationListener() {
            return _animationListener;
        }
        @Deprecated
        @Override
        public void setAnimationListener(AnimationListener listener) {
            super.setAnimationListener(listener);
        }

        // alpha
        public float aniAlphaFrom = 1;
        public float aniAlphaTo = 1;
        // scale
        public float aniScaleXFrom = 1;
        public float aniScaleXTo = 1;
        public float aniScaleYFrom = 1;
        public float aniScaleYTo = 1;
        public float aniScaleZFrom = 1;
        public float aniScaleZTo = 1;
        // translate by view size's percent
        public float aniTranslateXFrom = 0;
        public float aniTranslateXTo = 0;
        public float aniTranslateYFrom = 0;
        public float aniTranslateYTo = 0;
        public float aniTranslateZFrom = 0;
        public float aniTranslateZTo = 0;
        // translate by pixel
        public int aniTranslatePixelXFrom = 0;
        public int aniTranslatePixelXTo = 0;
        public int aniTranslatePixelYFrom = 0;
        public int aniTranslatePixelYTo = 0;
        public int aniTranslatePixelZFrom = 0;
        public int aniTranslatePixelZTo = 0;
        // rotate
        public float aniRotateXFrom = 0;
        public float aniRotateXTo = 0;
        public float aniRotateYFrom = 0;
        public float aniRotateYTo = 0;
        public float aniRotateZFrom = 0;
        public float aniRotateZTo = 0;

        public boolean hasRotate() {
            return (this.aniRotateXFrom != 0 || this.aniRotateXTo != 0
                || this.aniRotateYFrom != 0 || this.aniRotateYTo != 0
                || this.aniRotateZFrom != 0 || this.aniRotateZTo != 0);
        }

        public void nativeAnimationReset() {
            // alpha
            this.aniAlphaFrom = 1;
            this.aniAlphaTo = 1;
            // scale
            this.aniScaleXFrom = 1;
            this.aniScaleXTo = 1;
            this.aniScaleYFrom = 1;
            this.aniScaleYTo = 1;
            // translate by view size's percent
            this.aniTranslateXFrom = 0;
            this.aniTranslateXTo = 0;
            this.aniTranslateYFrom = 0;
            this.aniTranslateYTo = 0;
            // translate by pixel
            this.aniTranslatePixelXFrom = 0;
            this.aniTranslatePixelXTo = 0;
            this.aniTranslatePixelYFrom = 0;
            this.aniTranslatePixelYTo = 0;
            // rotate
            this.aniRotateXFrom = 0;
            this.aniRotateXTo = 0;
            this.aniRotateYFrom = 0;
            this.aniRotateYTo = 0;
            this.aniRotateZFrom = 0;
            this.aniRotateZTo = 0;
        }

        @Override
        public void initialize(int width,
                               int height,
                               int parentWidth,
                               int parentHeight) {
            super.initialize(width, height, parentWidth, parentHeight);
            m_width = width;
            m_height = height;
            m_centerX = width / 2;
            m_centerY = height / 2;

            _hasScale = (this.aniScaleXFrom != 1 || this.aniScaleXTo != 1
                || this.aniScaleYFrom != 1 || this.aniScaleYTo != 1
                || this.aniScaleZFrom != 1 || this.aniScaleZTo != 1);
            _hasTranslate = (this.aniTranslateXFrom != 0 || this.aniTranslateXTo != 0
                || this.aniTranslateYFrom != 0 || this.aniTranslateYTo != 0
                || this.aniTranslateZFrom != 0 || this.aniTranslateZTo != 0);
            _hasTranslatePixel = (this.aniTranslatePixelXFrom != 0 || this.aniTranslatePixelXTo != 0
                || this.aniTranslatePixelYFrom != 0 || this.aniTranslatePixelYTo != 0
                || this.aniTranslatePixelZFrom != 0 || this.aniTranslatePixelZTo != 0);
            _hasRotate = (this.aniRotateXFrom != 0 || this.aniRotateXTo != 0
                || this.aniRotateYFrom != 0 || this.aniRotateYTo != 0
                || this.aniRotateZFrom != 0 || this.aniRotateZTo != 0);
        }

        @Override
        protected void applyTransformation(float interpolatedTime, Transformation t) {
            if(this._animationListener == null) {
                return ;
            }

            Matrix matrix = t.getMatrix();

            // fix coordinate axis
            matrix.postTranslate(-m_centerX, -m_centerY);

            // scale
            if(_hasScale) {
                matrix.postScale(
                    this.aniScaleXFrom + (this.aniScaleXTo - this.aniScaleXFrom) * interpolatedTime,
                    this.aniScaleYFrom + (this.aniScaleYTo - this.aniScaleYFrom) * interpolatedTime);
            }

            // rotate
            if(_hasRotate) {
                m_camera.save();
                if(this.aniRotateXFrom != 0 || this.aniRotateXTo != 0) {
                    m_camera.rotateX(this.aniRotateXFrom + (this.aniRotateXTo - this.aniRotateXFrom) * interpolatedTime);
                }
                if(this.aniRotateYFrom != 0 || this.aniRotateYTo != 0) {
                    m_camera.rotateY(-(this.aniRotateYFrom + (this.aniRotateYTo - this.aniRotateYFrom) * interpolatedTime));
                }
                if(this.aniRotateZFrom != 0 || this.aniRotateZTo != 0) {
                    m_camera.rotateZ(-(this.aniRotateZFrom + (this.aniRotateZTo - this.aniRotateZFrom) * interpolatedTime));
                }
                m_camera.getMatrix(m_matrix);
                m_camera.restore();
                matrix.postConcat(m_matrix);
            }

            // translate
            if(_hasTranslate) {
                matrix.postTranslate(
                    m_width * (this.aniTranslateXFrom + (this.aniTranslateXTo - this.aniTranslateXFrom) * interpolatedTime),
                    m_height * (this.aniTranslateYFrom + (this.aniTranslateYTo - this.aniTranslateYFrom) * interpolatedTime));
            }
            if(_hasTranslatePixel) {
                matrix.postTranslate(
                    (this.aniTranslatePixelXFrom + (this.aniTranslatePixelXTo - this.aniTranslatePixelXFrom) * interpolatedTime),
                    (this.aniTranslatePixelYFrom + (this.aniTranslatePixelYTo - this.aniTranslatePixelYFrom) * interpolatedTime));
            }

            // alpha
            if(this.aniAlphaFrom != this.aniAlphaTo) {
                t.setAlpha(t.getAlpha() * (this.aniAlphaFrom + (this.aniAlphaTo - this.aniAlphaFrom) * interpolatedTime));
            }

            // restore coordinate axis
            matrix.postTranslate(m_centerX, m_centerY);
        }

        private int m_width = 0;
        private int m_height = 0;
        private int m_centerX = 0;
        private int m_centerY = 0;
        private boolean _hasScale = false;
        private boolean _hasTranslate = false;
        private boolean _hasTranslatePixel = false;
        private boolean _hasRotate = false;
        private static Matrix m_matrix = new Matrix();
        private static Camera m_camera = new Camera();
    }

    protected static class NativeAnimationListener implements AnimationListener, View.OnAttachStateChangeListener {
        private NativeAnimation _owner = null;
        private View _ownerView = null;

        public NativeAnimationListener(NativeAnimation owner, View ownerView) {
            this._owner = owner;
            this._ownerView = ownerView;
            this._ownerView.addOnAttachStateChangeListener(this);
        }

        public void nativeAniDetach() {
            if(this._owner != null) {
                View ownerViewTmp = this._ownerView;
                this._owner = null;
                this._ownerView = null;
                ownerViewTmp.clearAnimation();
                ownerViewTmp.removeOnAttachStateChangeListener(this);
            }
        }

        @Override
        public void onAnimationStart(Animation animation) {
        }
        private static Handler _stopDelay = new Handler() {
            @Override
            public void handleMessage(Message msg) {
                super.handleMessage(msg);
                ((NativeAnimationListener)msg.obj).doStop();
            }
        };
        @Override
        public void onAnimationEnd(Animation animation) {
            _stopDelay.sendMessageDelayed(Message.obtain(_stopDelay, 0, this), 0);
        }
        @Override
        public void onAnimationRepeat(Animation animation) {
        }

        @Override
        public void onViewAttachedToWindow(View v) {
        }
        @Override
        public void onViewDetachedFromWindow(View v) {
            this.doStop();
        }

        private void doStop() {
            if(this._owner != null) {
                long ownerTmp = this._owner.zfjniPointerOwnerZFAnimationNativeView;
                this.nativeAniDetach();
                ZFAnimationNativeView.native_notifyAniStop(ownerTmp);
            }
        }
    }

    private static Map<View, List<NativeAnimation> > _anis = new HashMap<View, List<NativeAnimation> >();

    public static Object native_nativeAniCreate(long zfjniPointerOwnerZFAnimationNativeView) {
        return new NativeAnimation(zfjniPointerOwnerZFAnimationNativeView);
    }
    public static void native_nativeAniDestroy(Object nativeAnimation) {
    }
    public static void native_nativeAniStart(Object nativeAnimation,
                                             Object nativeView) {
        NativeAnimation nativeAnimationTmp = (NativeAnimation)nativeAnimation;
        View nativeViewTmp = (View)nativeView;
        nativeAnimationTmp.nativeAnimationListener(new NativeAnimationListener(nativeAnimationTmp, nativeViewTmp));

        AnimationSet as = null;
        boolean asNeedStart = false;
        if(nativeViewTmp.getAnimation() != null && nativeViewTmp.getAnimation() instanceof AnimationSet) {
            as = (AnimationSet)nativeViewTmp.getAnimation();
        }
        else {
            as = new AnimationSet(false);
            asNeedStart = true;
        }

        List<NativeAnimation> attached = _anis.get(nativeViewTmp);
        if(attached == null) {
            attached = new ArrayList<NativeAnimation>();
            _anis.put(nativeViewTmp, attached);
        }
        attached.add(nativeAnimationTmp);
        as.addAnimation(nativeAnimationTmp);

        if(asNeedStart) {
            nativeViewTmp.startAnimation(as);
        }

        // rotate must be placed at head
        // otherwise, rotation after scale/translate would have wrong center axis
        ArrayList<Animation> al = (ArrayList<Animation>)as.getAnimations();
        if(al.size() > 1) {
            for(int i = 0; i < al.size(); ++i) {
                if(al.get(i) instanceof  NativeAnimation
                    && ((NativeAnimation)al.get(i)).hasRotate()) {
                    _anisCache.add(al.remove(i));
                    --i;
                }
            }
            al.addAll(0, _anisCache);
            _anisCache.clear();
        }
    }
    private static ArrayList<Animation> _anisCache = new ArrayList<Animation>();
    public static void native_nativeAniStop(Object nativeAnimation,
                                            Object nativeView) {
        NativeAnimation nativeAnimationTmp = (NativeAnimation)nativeAnimation;
        nativeAnimationTmp.nativeAnimationListener(null);
        View nativeViewTmp = (View)nativeView;

        List<NativeAnimation> attached = _anis.get(nativeViewTmp);
        if(attached == null) {
            nativeViewTmp.setAnimation(null);
            return ;
        }
        attached.remove(nativeAnimationTmp);
        if(attached.isEmpty()) {
            _anis.remove(nativeViewTmp);
            nativeViewTmp.setAnimation(null);
        }
    }
    private static native void native_notifyAniStop(long zfjniPointerOwnerZFAnimationNativeView);

    private static _CurveLinear _curveLinear = new _CurveLinear();
    private static _CurveEaseIn _curveEaseIn = new _CurveEaseIn();
    private static _CurveEaseOut _curveEaseOut = new _CurveEaseOut();
    private static _CurveEaseInOut _curveEaseInOut = new _CurveEaseInOut();
    public static void native_setup(Object nativeAnimation
                                    , int aniCurve
                                    , int aniDuration
                                    ) {
        NativeAnimation nativeAnimationTmp = (NativeAnimation)nativeAnimation;
        nativeAnimationTmp.nativeAnimationReset();

        if(aniCurve == ZFAnimationNativeViewCurve.e_Linear) {
            nativeAnimationTmp.setInterpolator(_curveLinear);
        }
        else if(aniCurve == ZFAnimationNativeViewCurve.e_EaseInOut) {
            nativeAnimationTmp.setInterpolator(_curveEaseInOut);
        }
        else if(aniCurve == ZFAnimationNativeViewCurve.e_EaseIn) {
            nativeAnimationTmp.setInterpolator(_curveEaseIn);
        }
        else if(aniCurve == ZFAnimationNativeViewCurve.e_EaseOut) {
            nativeAnimationTmp.setInterpolator(_curveEaseOut);
        }
        else {
            ZFAndroidLog.shouldNotGoHere();
        }

        nativeAnimationTmp.setDuration(aniDuration);
    }

    // ============================================================
    private static float _pow(float x, int n) {
        /*
            float ret = x;
            for(int i = 1; i < n; ++i) {
                ret *= x;
            }
            return ret;
        */
        return (float)Math.pow(x, n);
    }
    private static float _bezier(float x,
                                 float y0,
                                 float y1,
                                 float y2,
                                 float y3,
                                 float y4,
                                 float y5,
                                 float y6)
    {
        return (float)(0
            +      y0              * _pow(1-x, 6)
            + 6  * y1 * x          * _pow(1-x, 5)
            + 15 * y2 * _pow(x, 2) * _pow(1-x, 4)
            + 20 * y3 * _pow(x, 3) * _pow(1-x, 3)
            + 15 * y4 * _pow(x, 4) * _pow(1-x, 2)
            + 6  * y5 * _pow(x, 5) * (1-x)
            +      y6 * _pow(x, 6)
            );
    }
    private static float _bezier(float x,
                                 float y0,
                                 float y1,
                                 float y2,
                                 float y3,
                                 float y4,
                                 float y5,
                                 float y6,
                                 float y7,
                                 float y8,
                                 float y9,
                                 float y10,
                                 float y11,
                                 float y12,
                                 float y13,
                                 float y14,
                                 float y15)
    {
        return (float)(0
            +        y0                * _pow(1-x, 15)
            + 15   * y1  * x           * _pow(1-x, 14)
            + 105  * y2  * _pow(x, 2)  * _pow(1-x, 13)
            + 455  * y3  * _pow(x, 3)  * _pow(1-x, 12)
            + 1365 * y4  * _pow(x, 4)  * _pow(1-x, 11)
            + 3003 * y5  * _pow(x, 5)  * _pow(1-x, 10)
            + 5005 * y6  * _pow(x, 6)  * _pow(1-x, 9)
            + 6435 * y7  * _pow(x, 7)  * _pow(1-x, 8)
            + 6435 * y8  * _pow(x, 8)  * _pow(1-x, 7)
            + 5005 * y9  * _pow(x, 9)  * _pow(1-x, 6)
            + 3003 * y10 * _pow(x, 10) * _pow(1-x, 5)
            + 1365 * y11 * _pow(x, 11) * _pow(1-x, 4)
            + 455  * y12 * _pow(x, 12) * _pow(1-x, 3)
            + 105  * y13 * _pow(x, 13) * _pow(1-x, 2)
            + 15   * y14 * _pow(x, 14) * (1-x)
            +        y15 * _pow(x, 15)
            );
    }

    // ============================================================
    private static class _CurveLinear implements Interpolator {
        @Override
        public float getInterpolation(float time) {
            return time;
        }
    }

    // ============================================================
    private static class _CurveEaseIn implements Interpolator {
        private static final float y0 = 0.000f;
        private static final float y1 = 0.005f;
        private static final float y2 = 0.010f;
        private static final float y3 = 0.015f;
        private static final float y4 = 0.020f;
        private static final float y5 = 0.025f;
        private static final float y6 = 1.000f;
        @Override
        public float getInterpolation(float time) {
            return (float)_bezier((float)time, y0, y1, y2, y3, y4, y5, y6);
        }
    }

    // ============================================================
    private static class _CurveEaseOut implements Interpolator {
        private static final float y0 = 0.000f;
        private static final float y1 = 0.975f;
        private static final float y2 = 0.980f;
        private static final float y3 = 0.985f;
        private static final float y4 = 0.990f;
        private static final float y5 = 0.995f;
        private static final float y6 = 1.000f;
        @Override
        public float getInterpolation(float time) {
            return (float)_bezier((float)time, y0, y1, y2, y3, y4, y5, y6);
        }
    }

    // ============================================================
    private static class _CurveEaseInOut implements Interpolator {
        private static final float y0 = 0.000f;
        private static final float y1 = 0.005f;
        private static final float y2 = 0.010f;
        private static final float y3 = 0.015f;
        private static final float y4 = 0.020f;
        private static final float y5 = 0.025f;
        private static final float y6 = 0.030f;
        private static final float y7 = 0.035f;
        private static final float y8 = 0.965f;
        private static final float y9 = 0.970f;
        private static final float y10 = 0.975f;
        private static final float y11 = 0.980f;
        private static final float y12 = 0.985f;
        private static final float y13 = 0.990f;
        private static final float y14 = 0.995f;
        private static final float y15 = 1.000f;
        @Override
        public float getInterpolation(float time) {
            return (float)_bezier((float)time, y0, y1, y2, y3, y4, y5, y6, y7, y8, y9, y10, y11, y12, y13, y14, y15);
        }
    }
}
