package com.ZFFramework.Android.ZFUIKit_impl;

import java.util.ArrayList;
import java.util.List;
import com.ZFFramework.Android.NativeUtil.ZFAndroidSize;
import com.ZFFramework.Android.NativeUtil.ZFAndroidUI;
import com.ZFFramework.Android.ZF_impl.ZFMainEntry;
import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.Color;
import android.graphics.Rect;
import android.util.AttributeSet;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;

public class ZFUIView extends ViewGroup {

    // ============================================================
    public long zfjniPointerOwnerZFUIView = 0;
    public View nativeImplView = null;
    public boolean viewUIEnable = true;
    public boolean viewUIEnableTree = true;
    public int layoutedWidth = -1;
    public int layoutedHeight = -1;
    public int viewFrame_x = 0;
    public int viewFrame_y = 0;
    public int viewFrame_width = 0;
    public int viewFrame_height = 0;
    public int nativeImplViewFrame_x = 0;
    public int nativeImplViewFrame_y = 0;
    public int nativeImplViewFrame_width = 0;
    public int nativeImplViewFrame_height = 0;

    // ============================================================
    private Rect _rectNativeCache = new Rect();

    // ============================================================
    public static void native_nativeViewCacheOnSave(Object nativeView) {
        ((ZFUIView)nativeView).zfjniPointerOwnerZFUIView = 0;
    }
    public static void native_nativeViewCacheOnRestore(Object nativeView, long zfjniPointerOwnerZFUIView) {
        ZFUIView nativeViewTmp = (ZFUIView)nativeView;
        nativeViewTmp.zfjniPointerOwnerZFUIView = zfjniPointerOwnerZFUIView;

        nativeViewTmp.setVisibility(View.VISIBLE);
        nativeViewTmp.setAlpha(1);
        nativeViewTmp.viewUIEnable = true;
        nativeViewTmp.viewUIEnableTree = true;
        nativeViewTmp.setClickable(true);
        nativeViewTmp.setBackgroundColor(Color.TRANSPARENT);

        ZFUIViewFocus.native_viewFocusable(nativeView, false);
    }
    public static Object native_nativeViewCreate(long zfjniPointerOwnerZFUIView) {
        ZFUIView ret = new ZFUIView(ZFMainEntry.mainEntryActivity());
        ret.zfjniPointerOwnerZFUIView = zfjniPointerOwnerZFUIView;
        return ret;
    }
    public static void native_nativeImplView(Object nativeView,
                                                Object nativeImplView,
                                                int virtualIndex) {
        ZFUIView nativeViewTmp = (ZFUIView)nativeView;
        View nativeImplViewOld = nativeViewTmp.nativeImplView;
        View nativeImplViewNew = (View)nativeImplView;
        if(nativeViewTmp.nativeImplView != null) {
            nativeViewTmp.removeView(nativeViewTmp.nativeImplView);
        }
        nativeViewTmp.nativeImplView = nativeImplViewNew;
        if(nativeViewTmp.nativeImplView != null) {
            nativeViewTmp.addView(nativeViewTmp.nativeImplView, virtualIndex);
        }

        ZFUIViewFocus.ZFUIViewImplChanged(nativeViewTmp, nativeImplViewOld, nativeImplViewNew);
    }
    public static void native_nativeImplViewFrame(Object nativeView,
                                                  int nativeImplViewFrame_x,
                                                  int nativeImplViewFrame_y,
                                                  int nativeImplViewFrame_width,
                                                  int nativeImplViewFrame_height) {
        ZFUIView nativeViewTmp = (ZFUIView)nativeView;
        nativeViewTmp.nativeImplViewFrame_x = nativeImplViewFrame_x;
        nativeViewTmp.nativeImplViewFrame_y = nativeImplViewFrame_y;
        nativeViewTmp.nativeImplViewFrame_width = nativeImplViewFrame_width;
        nativeViewTmp.nativeImplViewFrame_height = nativeImplViewFrame_height;
    }
    public static float native_nativeViewScaleForImpl(Object nativeView) {
        return ZFUIView.native_nativeViewScaleForPhysicalPixel(nativeView);
    }
    public static float native_nativeViewScaleForPhysicalPixel(Object nativeView) {
        return ZFAndroidUI.screenDensity(((View)nativeView).getContext());
    }
    public static void native_viewVisible(Object nativeView,
                                             boolean viewVisible) {
        ((ZFUIView)nativeView).setVisibility(viewVisible ? View.VISIBLE : View.GONE);
    }
    public static void native_viewAlpha(Object nativeView,
                                           float viewAlpha) {
        ((ZFUIView)nativeView).setAlpha(viewAlpha);
    }
    public static void native_viewUIEnable(Object nativeView,
                                              boolean viewUIEnable) {
        ((ZFUIView)nativeView).viewUIEnable = viewUIEnable;
    }
    public static void native_viewUIEnableTree(Object nativeView,
                                                  boolean viewUIEnableTree) {
        ZFUIView nativeViewTmp = (ZFUIView)nativeView;
        nativeViewTmp.viewUIEnableTree = viewUIEnableTree;
        nativeViewTmp.setClickable(viewUIEnableTree);
    }
    public static void native_viewBackgroundColor(Object nativeView,
                                                     int viewBackgroundColor) {
        ((ZFUIView)nativeView).setBackgroundColor(viewBackgroundColor);
    }
    public static void native_childAdd(Object nativeView,
                                       Object nativeChild,
                                       int virtualIndex,
                                       int childLayer,
                                       int childLayerIndex) {
        ((ZFUIView)nativeView).addView((View)nativeChild, virtualIndex);
    }
    public static void native_childRemove(Object nativeView,
                                          int virtualIndex,
                                          int childLayer,
                                          int childLayerIndex) {
        ((ZFUIView)nativeView).removeViewAt(virtualIndex);
    }
    public static void native_childRemoveAllForDealloc(Object nativeView) {
        ZFUIView nativeViewTmp = (ZFUIView)nativeView;
        if(!(nativeViewTmp.getChildCount() == 0
            || (nativeViewTmp.getChildCount() == 1 && nativeViewTmp.nativeImplView != null)))
        {
            nativeViewTmp.removeAllViews();
            if(nativeViewTmp.nativeImplView != null) {
                nativeViewTmp.addView(nativeViewTmp.nativeImplView);
            }
        }
    }
    public static void native_viewFrame(Object nativeView,
                                           int viewFrame_x,
                                           int viewFrame_y,
                                           int viewFrame_width,
                                           int viewFrame_height) {
        ZFUIView nativeViewTmp = (ZFUIView)nativeView;
        nativeViewTmp.viewFrame_x = viewFrame_x;
        nativeViewTmp.viewFrame_y = viewFrame_y;
        nativeViewTmp.viewFrame_width = viewFrame_width;
        nativeViewTmp.viewFrame_height = viewFrame_height;
    }
    public static void native_layoutRequest(Object nativeView) {
        ZFUIView nativeViewTmp = (ZFUIView)nativeView;
        nativeViewTmp.layoutedWidth = -1;
        nativeViewTmp.layoutedHeight = -1;
        nativeViewTmp.requestLayout();
    }
    public static Object native_measureNativeView(Object nativeView,
                                                  int maxWidthOrNegative,
                                                  int maxHeightOrNegative) {
        View nativeViewTmp = (View)nativeView;
        nativeViewTmp.measure(
            (maxWidthOrNegative >= 0)
                ? MeasureSpec.makeMeasureSpec(maxWidthOrNegative, MeasureSpec.AT_MOST)
                : MeasureSpec.makeMeasureSpec(0, MeasureSpec.UNSPECIFIED),
            (maxHeightOrNegative >= 0)
                ? MeasureSpec.makeMeasureSpec(maxHeightOrNegative, MeasureSpec.AT_MOST)
                : MeasureSpec.makeMeasureSpec(0, MeasureSpec.UNSPECIFIED));
        return new ZFAndroidSize(nativeViewTmp.getMeasuredWidth(), nativeViewTmp.getMeasuredHeight());
    }

    // ============================================================
    public static native void native_notifyLayoutView(long zfjniPointerOwnerZFUIView,
                                                      int rect_x, int rect_y, int rect_width, int rect_height);
    public static native void native_notifyUIEvent_mouse(long zfjniPointerOwnerZFUIView,
                                                         int mouseId,
                                                         int mouseAction,
                                                         int mousePointX,
                                                         int mousePointY);
    public static native boolean native_notifyUIEvent_key(long zfjniPointerOwnerZFUIView,
                                                          int keyId,
                                                          int keyAction,
                                                          int keyCode,
                                                          int keyCodeRaw);

    // ============================================================
    protected ZFUIView(Context context) {
        super(context);
        this.init(context);
    }
    protected ZFUIView(Context context, AttributeSet attrs) {
        super(context, attrs);
        this.init(context);
    }
    protected ZFUIView(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
        this.init(context);
    }
    private void init(Context context) {
        this.setClipToPadding(false);
        this.setFocusable(false);
        this.setFocusableInTouchMode(false);
    }

    private static int _keyCodeFromKeyCodeRaw(int keyCodeRaw) {
        switch(keyCodeRaw) {
            case KeyEvent.KEYCODE_UNKNOWN:
                return ZFUIKeyCode.e_kUnknown;

            case KeyEvent.KEYCODE_ESCAPE:
                return ZFUIKeyCode.e_kEsc;
            case KeyEvent.KEYCODE_TAB:
                return ZFUIKeyCode.e_kTab;
            case KeyEvent.KEYCODE_FORWARD_DEL:
                return ZFUIKeyCode.e_kBackspace;
            case KeyEvent.KEYCODE_ENTER:
                return ZFUIKeyCode.e_kEnter;
            case KeyEvent.KEYCODE_SPACE:
                return ZFUIKeyCode.e_kSpace;
            case KeyEvent.KEYCODE_CAPS_LOCK:
                return ZFUIKeyCode.e_kCapsLock;
            case KeyEvent.KEYCODE_SHIFT_LEFT:
            case KeyEvent.KEYCODE_SHIFT_RIGHT:
                return ZFUIKeyCode.e_kShift;
            case KeyEvent.KEYCODE_CTRL_LEFT:
            case KeyEvent.KEYCODE_CTRL_RIGHT:
                return ZFUIKeyCode.e_kCtrl;
            case KeyEvent.KEYCODE_ALT_LEFT:
            case KeyEvent.KEYCODE_ALT_RIGHT:
                return ZFUIKeyCode.e_kAlt;
            case KeyEvent.KEYCODE_META_LEFT:
            case KeyEvent.KEYCODE_META_RIGHT:
                return ZFUIKeyCode.e_kCommand;
            /* case KeyEvent.KEYCODE_MENU:
                return ZFUIKeyCode.e_kContextMenu; */ // mapped to kPhoneMenu

            case KeyEvent.KEYCODE_F1:
                return ZFUIKeyCode.e_kF1;
            case KeyEvent.KEYCODE_F2:
                return ZFUIKeyCode.e_kF2;
            case KeyEvent.KEYCODE_F3:
                return ZFUIKeyCode.e_kF3;
            case KeyEvent.KEYCODE_F4:
                return ZFUIKeyCode.e_kF4;
            case KeyEvent.KEYCODE_F5:
                return ZFUIKeyCode.e_kF5;
            case KeyEvent.KEYCODE_F6:
                return ZFUIKeyCode.e_kF6;
            case KeyEvent.KEYCODE_F7:
                return ZFUIKeyCode.e_kF7;
            case KeyEvent.KEYCODE_F8:
                return ZFUIKeyCode.e_kF8;
            case KeyEvent.KEYCODE_F9:
                return ZFUIKeyCode.e_kF9;
            case KeyEvent.KEYCODE_F10:
                return ZFUIKeyCode.e_kF10;
            case KeyEvent.KEYCODE_F11:
                return ZFUIKeyCode.e_kF11;
            case KeyEvent.KEYCODE_F12:
                return ZFUIKeyCode.e_kF12;

            case KeyEvent.KEYCODE_0:
                return ZFUIKeyCode.e_k0;
            case KeyEvent.KEYCODE_1:
                return ZFUIKeyCode.e_k1;
            case KeyEvent.KEYCODE_2:
                return ZFUIKeyCode.e_k2;
            case KeyEvent.KEYCODE_3:
                return ZFUIKeyCode.e_k3;
            case KeyEvent.KEYCODE_4:
                return ZFUIKeyCode.e_k4;
            case KeyEvent.KEYCODE_5:
                return ZFUIKeyCode.e_k5;
            case KeyEvent.KEYCODE_6:
                return ZFUIKeyCode.e_k6;
            case KeyEvent.KEYCODE_7:
                return ZFUIKeyCode.e_k7;
            case KeyEvent.KEYCODE_8:
                return ZFUIKeyCode.e_k8;
            case KeyEvent.KEYCODE_9:
                return ZFUIKeyCode.e_k9;

            case KeyEvent.KEYCODE_A:
                return ZFUIKeyCode.e_kA;
            case KeyEvent.KEYCODE_B:
                return ZFUIKeyCode.e_kB;
            case KeyEvent.KEYCODE_C:
                return ZFUIKeyCode.e_kC;
            case KeyEvent.KEYCODE_D:
                return ZFUIKeyCode.e_kD;
            case KeyEvent.KEYCODE_E:
                return ZFUIKeyCode.e_kE;
            case KeyEvent.KEYCODE_F:
                return ZFUIKeyCode.e_kF;
            case KeyEvent.KEYCODE_G:
                return ZFUIKeyCode.e_kG;
            case KeyEvent.KEYCODE_H:
                return ZFUIKeyCode.e_kH;
            case KeyEvent.KEYCODE_I:
                return ZFUIKeyCode.e_kI;
            case KeyEvent.KEYCODE_J:
                return ZFUIKeyCode.e_kJ;
            case KeyEvent.KEYCODE_K:
                return ZFUIKeyCode.e_kK;
            case KeyEvent.KEYCODE_L:
                return ZFUIKeyCode.e_kL;
            case KeyEvent.KEYCODE_M:
                return ZFUIKeyCode.e_kM;
            case KeyEvent.KEYCODE_N:
                return ZFUIKeyCode.e_kN;
            case KeyEvent.KEYCODE_O:
                return ZFUIKeyCode.e_kO;
            case KeyEvent.KEYCODE_P:
                return ZFUIKeyCode.e_kP;
            case KeyEvent.KEYCODE_Q:
                return ZFUIKeyCode.e_kQ;
            case KeyEvent.KEYCODE_R:
                return ZFUIKeyCode.e_kR;
            case KeyEvent.KEYCODE_S:
                return ZFUIKeyCode.e_kS;
            case KeyEvent.KEYCODE_T:
                return ZFUIKeyCode.e_kT;
            case KeyEvent.KEYCODE_U:
                return ZFUIKeyCode.e_kU;
            case KeyEvent.KEYCODE_V:
                return ZFUIKeyCode.e_kV;
            case KeyEvent.KEYCODE_W:
                return ZFUIKeyCode.e_kW;
            case KeyEvent.KEYCODE_X:
                return ZFUIKeyCode.e_kX;
            case KeyEvent.KEYCODE_Y:
                return ZFUIKeyCode.e_kY;
            case KeyEvent.KEYCODE_Z:
                return ZFUIKeyCode.e_kZ;

            case KeyEvent.KEYCODE_GRAVE:
                return ZFUIKeyCode.e_kGrave;
            case KeyEvent.KEYCODE_MINUS:
                return ZFUIKeyCode.e_kMinus;
            case KeyEvent.KEYCODE_EQUALS:
                return ZFUIKeyCode.e_kEquals;
            case KeyEvent.KEYCODE_LEFT_BRACKET:
                return ZFUIKeyCode.e_kBracketLeft;
            case KeyEvent.KEYCODE_RIGHT_BRACKET:
                return ZFUIKeyCode.e_kBracketRight;
            case KeyEvent.KEYCODE_BACKSLASH:
                return ZFUIKeyCode.e_kBackSlash;
            case KeyEvent.KEYCODE_SEMICOLON:
                return ZFUIKeyCode.e_kSemiColon;
            case KeyEvent.KEYCODE_APOSTROPHE:
                return ZFUIKeyCode.e_kApostrophe;
            case KeyEvent.KEYCODE_COMMA:
                return ZFUIKeyCode.e_kComma;
            case KeyEvent.KEYCODE_PERIOD:
                return ZFUIKeyCode.e_kPeriod;
            case KeyEvent.KEYCODE_SLASH:
                return ZFUIKeyCode.e_kSlash;

            case KeyEvent.KEYCODE_DPAD_LEFT:
                return ZFUIKeyCode.e_kLeft;
            case KeyEvent.KEYCODE_DPAD_UP:
                return ZFUIKeyCode.e_kUp;
            case KeyEvent.KEYCODE_DPAD_RIGHT:
                return ZFUIKeyCode.e_kRight;
            case KeyEvent.KEYCODE_DPAD_DOWN:
                return ZFUIKeyCode.e_kDown;
            case KeyEvent.KEYCODE_MOVE_HOME:
                return ZFUIKeyCode.e_kHome;
            case KeyEvent.KEYCODE_MOVE_END:
                return ZFUIKeyCode.e_kEnd;
            case KeyEvent.KEYCODE_PAGE_UP:
                return ZFUIKeyCode.e_kPageUp;
            case KeyEvent.KEYCODE_PAGE_DOWN:
                return ZFUIKeyCode.e_kPageDown;
            case KeyEvent.KEYCODE_INSERT:
                return ZFUIKeyCode.e_kInsert;
            case KeyEvent.KEYCODE_DEL:
                return ZFUIKeyCode.e_kDelete;
            case KeyEvent.KEYCODE_SYSRQ:
                return ZFUIKeyCode.e_kPrintScreen;
            case KeyEvent.KEYCODE_SCROLL_LOCK:
                return ZFUIKeyCode.e_kScrollLock;
            case KeyEvent.KEYCODE_BREAK:
                return ZFUIKeyCode.e_kPauseBreak;

            case KeyEvent.KEYCODE_NUMPAD_0:
                return ZFUIKeyCode.e_kNum0;
            case KeyEvent.KEYCODE_NUMPAD_1:
                return ZFUIKeyCode.e_kNum1;
            case KeyEvent.KEYCODE_NUMPAD_2:
                return ZFUIKeyCode.e_kNum2;
            case KeyEvent.KEYCODE_NUMPAD_3:
                return ZFUIKeyCode.e_kNum3;
            case KeyEvent.KEYCODE_NUMPAD_4:
                return ZFUIKeyCode.e_kNum4;
            case KeyEvent.KEYCODE_NUMPAD_5:
                return ZFUIKeyCode.e_kNum5;
            case KeyEvent.KEYCODE_NUMPAD_6:
                return ZFUIKeyCode.e_kNum6;
            case KeyEvent.KEYCODE_NUMPAD_7:
                return ZFUIKeyCode.e_kNum7;
            case KeyEvent.KEYCODE_NUMPAD_8:
                return ZFUIKeyCode.e_kNum8;
            case KeyEvent.KEYCODE_NUMPAD_9:
                return ZFUIKeyCode.e_kNum9;
            case KeyEvent.KEYCODE_NUM_LOCK:
                return ZFUIKeyCode.e_kNumLock;
            case KeyEvent.KEYCODE_NUMPAD_ADD:
                return ZFUIKeyCode.e_kNumAdd;
            case KeyEvent.KEYCODE_NUMPAD_SUBTRACT:
                return ZFUIKeyCode.e_kNumMinus;
            case KeyEvent.KEYCODE_NUMPAD_MULTIPLY:
                return ZFUIKeyCode.e_kNumMultiply;
            case KeyEvent.KEYCODE_NUMPAD_DIVIDE:
                return ZFUIKeyCode.e_kNumDivide;
            case KeyEvent.KEYCODE_NUMPAD_ENTER:
                return ZFUIKeyCode.e_kNumEnter;
            case KeyEvent.KEYCODE_NUMPAD_DOT:
                return ZFUIKeyCode.e_kNumDot;

            case KeyEvent.KEYCODE_MEDIA_PLAY:
                return ZFUIKeyCode.e_kMediaPlay;
            case KeyEvent.KEYCODE_MEDIA_PAUSE:
                return ZFUIKeyCode.e_kMediaPause;
            case KeyEvent.KEYCODE_MEDIA_PLAY_PAUSE:
                return ZFUIKeyCode.e_kMediaPlayPause;
            case KeyEvent.KEYCODE_MEDIA_STOP:
                return ZFUIKeyCode.e_kMediaStop;
            case KeyEvent.KEYCODE_MEDIA_RECORD:
                return ZFUIKeyCode.e_kMediaRecord;
            case KeyEvent.KEYCODE_MEDIA_PREVIOUS:
                return ZFUIKeyCode.e_kMediaPrev;
            case KeyEvent.KEYCODE_MEDIA_NEXT:
                return ZFUIKeyCode.e_kMediaNext;
            case KeyEvent.KEYCODE_MEDIA_FAST_FORWARD:
                return ZFUIKeyCode.e_kMediaFastForward;
            case KeyEvent.KEYCODE_MEDIA_REWIND:
                return ZFUIKeyCode.e_kMediaFastRewind;

            case KeyEvent.KEYCODE_VOLUME_MUTE:
                return ZFUIKeyCode.e_kVolumeMute;
            case KeyEvent.KEYCODE_VOLUME_UP:
                return ZFUIKeyCode.e_kVolumeUp;
            case KeyEvent.KEYCODE_VOLUME_DOWN:
                return ZFUIKeyCode.e_kVolumeDown;

            case KeyEvent.KEYCODE_CALL:
                return ZFUIKeyCode.e_kPhoneCall;
            case KeyEvent.KEYCODE_ENDCALL:
                return ZFUIKeyCode.e_kPhoneCallEnd;
            case KeyEvent.KEYCODE_HOME:
                return ZFUIKeyCode.e_kPhoneHome;
            case KeyEvent.KEYCODE_MENU:
                return ZFUIKeyCode.e_kPhoneMenu;
            case KeyEvent.KEYCODE_BACK:
                return ZFUIKeyCode.e_kPhoneBack;
            case KeyEvent.KEYCODE_SEARCH:
                return ZFUIKeyCode.e_kPhoneSearch;
            case KeyEvent.KEYCODE_CAMERA:
                return ZFUIKeyCode.e_kPhoneCamera;
            default:
                return ZFUIKeyCode.e_kUnknown;
        }
    }
    private static int _keyActionFromKeyActionRaw(int keyActionRaw) {
        switch(keyActionRaw) {
            case KeyEvent.ACTION_DOWN:
                return ZFUIKeyAction.e_KeyDown;
            case KeyEvent.ACTION_MULTIPLE:
                return ZFUIKeyAction.e_KeyRepeat;
            case KeyEvent.ACTION_UP:
                return ZFUIKeyAction.e_KeyUp;
            default:
                return ZFUIKeyAction.e_KeyCancel;
        }
    }
    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        _pressedKeyAdd((int)event.getDownTime(), event.getKeyCode());
        if(this.zfjniPointerOwnerZFUIView != 0
            && ZFUIView.native_notifyUIEvent_key(
                this.zfjniPointerOwnerZFUIView,
                (int)event.getDownTime(),
                _keyActionFromKeyActionRaw(event.getAction()),
                _keyCodeFromKeyCodeRaw(keyCode),
                keyCode)) {
            return true;
        }
        return super.onKeyDown(keyCode, event);
    }
    @Override
    public boolean onKeyMultiple(int keyCode, int repeatCount, KeyEvent event) {
        if(this.zfjniPointerOwnerZFUIView != 0
            && ZFUIView.native_notifyUIEvent_key(
                this.zfjniPointerOwnerZFUIView,
                (int)event.getDownTime(),
                _keyActionFromKeyActionRaw(event.getAction()),
                _keyCodeFromKeyCodeRaw(keyCode),
                keyCode)) {
            return true;
        }
        return super.onKeyMultiple(keyCode, repeatCount, event);
    }
    @Override
    public boolean onKeyUp(int keyCode, KeyEvent event) {
        _pressedKeyRemove((int)event.getDownTime(), event.getKeyCode());
        if(this.zfjniPointerOwnerZFUIView != 0
            && ZFUIView.native_notifyUIEvent_key(
                this.zfjniPointerOwnerZFUIView,
                (int)event.getDownTime(),
                _keyActionFromKeyActionRaw(event.getAction()),
                _keyCodeFromKeyCodeRaw(keyCode),
                keyCode)) {
            return true;
        }
        return super.onKeyUp(keyCode, event);
    }
    private final List<Integer> _pressedKeyId = new ArrayList<Integer>();
    private final List<Integer> _pressedKeyCode = new ArrayList<Integer>();
    private void _pressedKeyAdd(int keyId, int keyCode) {
        _pressedKeyId.add(keyId);
        _pressedKeyCode.add(keyCode);
    }
    private void _pressedKeyRemove(int keyId, int keyCode) {
        int index = _pressedKeyId.indexOf(keyId);
        if(index >= 0 && _pressedKeyCode.get(index) == keyCode) {
            _pressedKeyId.remove(index);
            _pressedKeyCode.remove(index);
        }
    }
    private void _pressedKeyCancel() {
        for(int i = 0; i < _pressedKeyId.size(); ++i) {
            ZFUIView.native_notifyUIEvent_key(
                this.zfjniPointerOwnerZFUIView,
                _pressedKeyId.get(i),
                ZFUIKeyAction.e_KeyCancel,
                _keyCodeFromKeyCodeRaw(_pressedKeyCode.get(i)),
                _pressedKeyCode.get(i));
        }
    }
    @Override
    protected void onDetachedFromWindow() {
        super.onDetachedFromWindow();
        _pressedKeyCancel();
    }
    @Override
    protected void onFocusChanged(boolean gainFocus,
                                  int direction,
                                  Rect previouslyFocusedRect) {
        super.onFocusChanged(gainFocus, direction, previouslyFocusedRect);
        _pressedKeyCancel();
    }

    @Override
    public boolean dispatchTouchEvent(MotionEvent ev) {
        if(!this.viewUIEnableTree) {
            return false;
        }
        if(this.viewUIEnable) {
            return super.dispatchTouchEvent(ev);
        }

        int x = (int)ev.getX() + this.getScrollX();
        int y = (int)ev.getY() + this.getScrollY();
        for(int i = 0; i < this.getChildCount(); ++i) {
            View child = this.getChildAt(i);
            if(child != this.nativeImplView
                && (child.getVisibility() == View.VISIBLE || child.getAnimation() != null)) {
                child.getHitRect(this._rectNativeCache);
                if(this._rectNativeCache.contains(x, y)) {
                    return super.dispatchTouchEvent(ev);
                }
            }
        }
        return false;
    }
    @SuppressLint("ClickableViewAccessibility")
    @Override
    public boolean onTouchEvent(MotionEvent event) {
        int action = ZFUIMouseAction.e_MouseDown;
        switch(event.getAction()) {
            case MotionEvent.ACTION_DOWN:
            case MotionEvent.ACTION_POINTER_DOWN:
                action = ZFUIMouseAction.e_MouseDown;
                break;
            case MotionEvent.ACTION_MOVE:
                action = ZFUIMouseAction.e_MouseMove;
                break;
            case MotionEvent.ACTION_UP:
            case MotionEvent.ACTION_POINTER_UP:
                action = ZFUIMouseAction.e_MouseUp;
                break;
            case MotionEvent.ACTION_CANCEL:
                action = ZFUIMouseAction.e_MouseCancel;
                break;
            default:
                return true;
        }
        for(int i = 0; i < event.getPointerCount(); ++i) {
            if(this.zfjniPointerOwnerZFUIView != 0) {
                ZFUIView.native_notifyUIEvent_mouse(
                    this.zfjniPointerOwnerZFUIView,
                    event.getPointerId(i),
                    action,
                    (int)event.getX(i),
                    (int)event.getY(i));
            }
        }
        return true;
    }

    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        this.setMeasuredDimension(this.viewFrame_width, this.viewFrame_height);
    }
    @Override
    protected void onLayout(boolean changed, int l, int t, int r, int b) {
        if(this.zfjniPointerOwnerZFUIView != 0) {
            if(this.layoutedWidth != (r - l) || this.layoutedHeight != (b - t)) {
                this.layoutedWidth = r - l;
                this.layoutedHeight = b - t;
                ZFUIView.native_notifyLayoutView(
                    this.zfjniPointerOwnerZFUIView,
                    l,
                    t,
                    this.layoutedWidth,
                    this.layoutedHeight);
            }
        }
        for(int i = 0; i < this.getChildCount(); ++i) {
            View child = this.getChildAt(i);
            if(child == this.nativeImplView) {
                child.measure(
                    MeasureSpec.makeMeasureSpec(this.nativeImplViewFrame_width, MeasureSpec.EXACTLY),
                    MeasureSpec.makeMeasureSpec(this.nativeImplViewFrame_height, MeasureSpec.EXACTLY));
                child.layout(
                    this.nativeImplViewFrame_x,
                    this.nativeImplViewFrame_y,
                    this.nativeImplViewFrame_x + this.nativeImplViewFrame_width,
                    this.nativeImplViewFrame_y + this.nativeImplViewFrame_height);
            }
            else if (child instanceof ZFUIView) {
                ZFUIView childTmp = (ZFUIView)child;
                child.measure(
                    MeasureSpec.makeMeasureSpec(childTmp.viewFrame_width, MeasureSpec.EXACTLY),
                    MeasureSpec.makeMeasureSpec(childTmp.viewFrame_height, MeasureSpec.EXACTLY));
                childTmp.layout(
                    childTmp.viewFrame_x,
                    childTmp.viewFrame_y,
                    childTmp.viewFrame_x + childTmp.viewFrame_width,
                    childTmp.viewFrame_y + childTmp.viewFrame_height);
            }
            else {
                // usually should not go here
                child.measure(
                    MeasureSpec.makeMeasureSpec(r - l, MeasureSpec.EXACTLY),
                    MeasureSpec.makeMeasureSpec(b - t, MeasureSpec.EXACTLY));
                child.layout(0, 0, r - l, b - t);
            }
        }
    }
}
