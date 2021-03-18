package com.ZFFramework.Android.ZFUIKit_impl;

import java.lang.ref.WeakReference;
import com.ZFFramework.Android.NativeUtil.ZFAndroidUI;
import com.ZFFramework.Android.ZF_impl.ZFMainEntry;
import android.content.Context;
import android.os.Handler;
import android.os.Message;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewParent;

public class ZFUIScrollView extends ZFUIView {
    // ============================================================
    public long zfjniPointerOwnerZFUIScrollView = 0;
    public boolean scrollEnable = true;
    public boolean scrollBounceHorizontal = true;
    public boolean scrollBounceVertical = true;
    public boolean scrollBounceHorizontalAlways = false;
    public boolean scrollBounceVerticalAlways = false;

    // ============================================================
    public static Object native_nativeScrollViewCreate(long zfjniPointerOwnerZFUIScrollView) {
        ZFUIScrollView ret = new ZFUIScrollView(ZFMainEntry.mainEntryActivity());
        ret.zfjniPointerOwnerZFUIScrollView = zfjniPointerOwnerZFUIScrollView;
        return ret;
    }

    public static void native_scrollEnable(Object nativeView,
                                                        boolean scrollEnable) {
        ZFUIScrollView nativeViewTmp = (ZFUIScrollView)nativeView;
        nativeViewTmp.scrollEnable = scrollEnable;
    }
    public static void native_scrollBounce(Object nativeView,
                                                        boolean scrollBounceHorizontal,
                                                        boolean scrollBounceVertical,
                                                        boolean scrollBounceHorizontalAlways,
                                                        boolean scrollBounceVerticalAlways) {
        ZFUIScrollView nativeViewTmp = (ZFUIScrollView)nativeView;
        nativeViewTmp.scrollBounceHorizontal = scrollBounceHorizontal;
        nativeViewTmp.scrollBounceVertical = scrollBounceVertical;
        nativeViewTmp.scrollBounceHorizontalAlways = scrollBounceHorizontalAlways;
        nativeViewTmp.scrollBounceVerticalAlways = scrollBounceVerticalAlways;
    }
    public static void native_scrollContentFrame(Object nativeView,
                                                              int contentFrame_x,
                                                              int contentFrame_y,
                                                              int contentFrame_width,
                                                              int contentFrame_height)
    {
        ZFUIScrollView nativeViewTmp = (ZFUIScrollView)nativeView;
        ZFUIView.native_viewFrame(nativeViewTmp._contentLayout,
            contentFrame_x, contentFrame_y, contentFrame_width, contentFrame_height);
        nativeViewTmp.requestLayout();
    }

    private boolean _scrollAnimating = false;
    private int _scrollAniTaskId = 0;
    private int _recommendTimerInterval = 15;
    private static Handler _scrollAniTimerHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            ZFUIScrollView scrollView = (ZFUIScrollView)msg.obj;
            if(scrollView._scrollAnimating && scrollView._scrollAniTaskId == msg.what) {
                ZFUIScrollView.native_notifyScrollViewScrollAnimation(
                    scrollView.zfjniPointerOwnerZFUIScrollView, System.currentTimeMillis());
                _scrollAniTimerHandler.sendMessageDelayed(Message.obtain(
                    _scrollAniTimerHandler, scrollView._scrollAniTaskId, scrollView), scrollView._recommendTimerInterval);
            }
        }
    };
    public static long native_scrollAnimationStart(Object nativeView, int recommendTimerInterval) {
        ZFUIScrollView nativeViewTmp = (ZFUIScrollView)nativeView;
        ++nativeViewTmp._scrollAniTaskId;
        nativeViewTmp._scrollAnimating = true;
        nativeViewTmp._recommendTimerInterval = recommendTimerInterval;
        _scrollAniTimerHandler.sendMessage(Message.obtain(_scrollAniTimerHandler, nativeViewTmp._scrollAniTaskId, nativeViewTmp));
        return System.currentTimeMillis();
    }
    public static void native_scrollAnimationStop(Object nativeView) {
        ZFUIScrollView nativeViewTmp = (ZFUIScrollView)nativeView;
        if(nativeViewTmp._scrollAnimating) {
            _scrollAniTimerHandler.removeMessages(nativeViewTmp._scrollAniTaskId, nativeViewTmp);
            ++nativeViewTmp._scrollAniTaskId;
            nativeViewTmp._scrollAnimating = false;
        }
    }

    public static void native_scrollChildAdd(Object nativeView,
                                             Object nativeChild,
                                             int atIndex) {
        ZFUIScrollView nativeViewTmp = (ZFUIScrollView)nativeView;
        nativeViewTmp._contentLayout.addView((View)nativeChild, atIndex);
    }
    public static void native_scrollChildRemove(Object nativeView,
                                                int atIndex) {
        ZFUIScrollView nativeViewTmp = (ZFUIScrollView)nativeView;
        nativeViewTmp._contentLayout.removeViewAt(atIndex);
    }
    public static void native_scrollChildRemoveAllForDealloc(Object nativeView) {
         ZFUIScrollView nativeViewTmp = (ZFUIScrollView)nativeView;
         nativeViewTmp._contentLayout.removeAllViews();
    }

    private static class ScrollContentLayout extends ZFUIView {
        protected ScrollContentLayout(Context context) {
            super(context);
        }
    }

    // ============================================================
    private static native void native_notifyScrollViewDragBegin(long zfjniPointerOwnerZFUIScrollView,
                                                                int mousePosX,
                                                                int mousePosY,
                                                                long mouseTime);
    private static native void native_notifyScrollViewDrag(long zfjniPointerOwnerZFUIScrollView,
                                                           int mousePosX,
                                                           int mousePosY,
                                                           long mouseTime);
    private static native void native_notifyScrollViewDragEnd(long zfjniPointerOwnerZFUIScrollView,
                                                              long mouseTime,
                                                              boolean needScrollAni);
    private static native void native_notifyScrollViewScrollAnimation(long zfjniPointerOwnerZFUIScrollView,
                                                                      long relativeTimeInMiliseconds);

    // ============================================================
    private ScrollContentLayout _contentLayout = null;
    private int _scrollViewBgViewCount = 0;

    // ============================================================
    protected ZFUIScrollView(Context context) {
        super(context);
        this.init(context);
    }
    protected ZFUIScrollView(Context context, AttributeSet attrs) {
        super(context, attrs);
        this.init(context);
    }
    protected ZFUIScrollView(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
        this.init(context);
    }
    private void init(Context context) {
        _contentLayout = new ScrollContentLayout(context);
        this.addView(_contentLayout);
    }

    // ============================================================
    private static interface Impl {
        public void dispatchTouchEvent(MotionEvent ev);
    }
    private final Impl impl = new JavaImpl(this);
    @Override
    public boolean dispatchTouchEvent(MotionEvent ev) {
        if(!this.viewUIEnableTree) {
            return false;
        }
        if(!this.viewUIEnable) {
            return super.dispatchTouchEvent(ev);
        }
        impl.dispatchTouchEvent(ev);
        return true;
    }

    // ============================================================
    // drag logic
    // ============================================================
    @SuppressWarnings("unused")
    private static class JavaImpl implements Impl {
        private WeakReference<ZFUIScrollView> owner = null;
        public JavaImpl(ZFUIScrollView owner) {
            this.owner = new WeakReference<ZFUIScrollView>(owner);
        }

        // ============================================================
        // global state
        private static final int _dragTolerance = (int)(4 * ZFAndroidUI.screenDensity(ZFMainEntry.appContext()));
        private static enum _DragState {
            /**
             * touched down but not dragging nor touch timeout
             */
            Tracking,
            /**
             * dragging, this scroll view would intercept all touch events
             */
            Dragging,
            /**
             * ignored, maybe timeout when touch down without dragging,
             * maybe self can't drag but parent can drag
             */
            Ignored,
        }
        private _DragState _dragState = _DragState.Tracking;
        private MotionEvent _dragMouseDownEventSaved = null;

        // ============================================================
        // util methods
        private static boolean _dragFindChild_touched(View child, int x, int y) {
            return (child.getVisibility() == View.VISIBLE
                && (x >= child.getLeft() && x <= child.getRight() && y >= child.getTop() && y <= child.getBottom()));
        }
        private static View _dragFindChild(ZFUIScrollView parent, int x, int y) {
            // verbosely for performance
            int iDirectChild = parent.getChildCount() - 1;
            for( ; iDirectChild >= 0; --iDirectChild) {
                View child = parent.getChildAt(iDirectChild);
                if(child == parent._contentLayout) {
                    --iDirectChild;
                    break;
                }
                if(_dragFindChild_touched(child, x, y)) {
                    return child;
                }
            }
            int xTmp = x - parent._contentLayout.getLeft();
            int yTmp = y - parent._contentLayout.getTop();
            for(int i = parent._contentLayout.getChildCount() - 1; i >= 0; --i) {
                View child = parent._contentLayout.getChildAt(i);
                if(_dragFindChild_touched(child, xTmp, yTmp)) {
                    return child;
                }
            }
            for( ; iDirectChild >= 0; --iDirectChild) {
                View child = parent.getChildAt(iDirectChild);
                if(_dragFindChild_touched(child, x, y)) {
                    return child;
                }
            }
            return null;
        }
        private static ZFUIScrollView.JavaImpl _dragFindChildScrollView_recursive(View view, int x, int y) {
            if(view instanceof ZFUIScrollView) {
                return (JavaImpl)((ZFUIScrollView)view).impl;
            }
            else if(view instanceof ViewGroup) {
                return _dragFindChildScrollView((ViewGroup)view, x - view.getLeft(), y - view.getTop());
            }
            else {
                return null;
            }
        }
        private static ZFUIScrollView.JavaImpl _dragFindChildScrollView(ViewGroup parent, int x, int y) {
            // verbosely for performance
            if(parent instanceof ZFUIScrollView) {
                ZFUIScrollView parentTmp = (ZFUIScrollView)parent;
                int xTmp = x - parentTmp._contentLayout.getLeft();
                int yTmp = y - parentTmp._contentLayout.getTop();
                int iDirectChild = parentTmp.getChildCount() - 1;
                for( ; iDirectChild >= 0; --iDirectChild) {
                    View child = parentTmp.getChildAt(iDirectChild);
                    if(child == parentTmp._contentLayout) {
                        --iDirectChild;
                        break;
                    }
                    if(!_dragFindChild_touched(child, x, y)) {
                        continue;
                    }
                    return _dragFindChildScrollView_recursive(child, x, y);
                }
                for(int i = parentTmp._contentLayout.getChildCount() - 1; i >= 0; --i) {
                    View child = parentTmp._contentLayout.getChildAt(i);
                    if(!_dragFindChild_touched(child, xTmp, yTmp)) {
                        continue;
                    }
                    return _dragFindChildScrollView_recursive(child, xTmp, yTmp);
                }
                for( ; iDirectChild >= 0; --iDirectChild) {
                    View child = parentTmp.getChildAt(iDirectChild);
                    if(!_dragFindChild_touched(child, x, y)) {
                        continue;
                    }
                    return _dragFindChildScrollView_recursive(child, x, y);
                }
                return null;
            }
            else {
                for(int i = parent.getChildCount() - 1; i >= 0; --i) {
                    View child = parent.getChildAt(i);
                    if(!_dragFindChild_touched(child, x, y)) {
                        continue;
                    }
                    return _dragFindChildScrollView_recursive(child, x, y);
                }
                return null;
            }
        }

        private boolean _dragScrollable() {
            ZFUIScrollView t = this.owner.get();
            return (t.viewUIEnable && t.viewUIEnableTree && t.scrollEnable && (t._contentLayout.viewFrame_width > t.getWidth() || t._contentLayout.viewFrame_height > t.getHeight()));
        }
        private boolean _dragScrollableX() {
            ZFUIScrollView t = this.owner.get();
            return (t.viewUIEnable && t.viewUIEnableTree && t.scrollEnable && (t._contentLayout.viewFrame_width > t.getWidth()));
        }
        private boolean _dragScrollableY() {
            ZFUIScrollView t = this.owner.get();
            return (t.viewUIEnable && t.viewUIEnableTree && t.scrollEnable && (t._contentLayout.viewFrame_height > t.getHeight()));
        }
        private boolean _dragScrollableLeft() {
            ZFUIScrollView t = this.owner.get();
            return (t.viewUIEnable && t.viewUIEnableTree && t.scrollEnable && (t._contentLayout.viewFrame_x < 0));
        }
        private boolean _dragScrollableTop() {
            ZFUIScrollView t = this.owner.get();
            return (t.viewUIEnable && t.viewUIEnableTree && t.scrollEnable && (t._contentLayout.viewFrame_y < 0));
        }
        private boolean _dragScrollableRight() {
            ZFUIScrollView t = this.owner.get();
            return (t.viewUIEnable && t.viewUIEnableTree && t.scrollEnable && (t._contentLayout.viewFrame_x + t._contentLayout.viewFrame_width > t.getWidth()));
        }
        private boolean _dragScrollableBottom() {
            ZFUIScrollView t = this.owner.get();
            return (t.viewUIEnable && t.viewUIEnableTree && t.scrollEnable && (t._contentLayout.viewFrame_y + t._contentLayout.viewFrame_height > t.getHeight()));
        }

        private boolean _dragScrollableOrBounceable() {
            ZFUIScrollView t = this.owner.get();
            return (t.viewUIEnable && t.viewUIEnableTree && t.scrollEnable && (t.scrollBounceHorizontalAlways || t.scrollBounceVerticalAlways || t._contentLayout.viewFrame_width > t.getWidth() || t._contentLayout.viewFrame_height > t.getHeight()));
        }
        private boolean _dragScrollableOrBounceableX() {
            ZFUIScrollView t = this.owner.get();
            return (t.viewUIEnable && t.viewUIEnableTree && t.scrollEnable && (t.scrollBounceHorizontalAlways || t._contentLayout.viewFrame_width > t.getWidth()));
        }
        private boolean _dragScrollableOrBounceableY() {
            ZFUIScrollView t = this.owner.get();
            return (t.viewUIEnable && t.viewUIEnableTree && t.scrollEnable && (t.scrollBounceVerticalAlways || t._contentLayout.viewFrame_height > t.getHeight()));
        }
        private boolean _dragScrollableOrBounceableLeft() {
            ZFUIScrollView t = this.owner.get();
            return (t.viewUIEnable && t.viewUIEnableTree && t.scrollEnable && (
                t.scrollBounceHorizontalAlways
                || (t.scrollBounceHorizontal && t._contentLayout.viewFrame_width > t.getWidth())
                || (t._contentLayout.viewFrame_x < 0)
                ));
        }
        private boolean _dragScrollableOrBounceableTop() {
            ZFUIScrollView t = this.owner.get();
            return (t.viewUIEnable && t.viewUIEnableTree && t.scrollEnable && (
                t.scrollBounceVerticalAlways
                || (t.scrollBounceVertical && t._contentLayout.viewFrame_height > t.getHeight())
                || (t._contentLayout.viewFrame_y < 0)
                ));
        }
        private boolean _dragScrollableOrBounceableRight() {
            ZFUIScrollView t = this.owner.get();
            return (t.viewUIEnable && t.viewUIEnableTree && t.scrollEnable && (
                t.scrollBounceHorizontalAlways
                || (t.scrollBounceHorizontal && t._contentLayout.viewFrame_width > t.getWidth())
                || (t._contentLayout.viewFrame_x + t._contentLayout.viewFrame_width > t.getWidth())
                ));
        }
        private boolean _dragScrollableOrBounceableBottom() {
            ZFUIScrollView t = this.owner.get();
            return (t.viewUIEnable && t.viewUIEnableTree && t.scrollEnable && (
                t.scrollBounceVerticalAlways
                || (t.scrollBounceVertical && t._contentLayout.viewFrame_height > t.getHeight())
                || (t._contentLayout.viewFrame_y + t._contentLayout.viewFrame_height > t.getHeight())
                ));
        }

        private static void _dragTranslateEventToChild(MotionEvent ev,
                                                       int xInSelf,
                                                       int yInSelf,
                                                       View parentScrollView,
                                                       View child) {
            int xOffset = child.getLeft();
            int yOffset = child.getTop();
            ViewParent parent = child.getParent();
            while(parent != null && (parent instanceof View)) {
                if(parent == parentScrollView) {
                    break;
                }
                xOffset += ((View)parent).getLeft();
                yOffset += ((View)parent).getTop();
                parent = parent.getParent();
            }

            ev.setLocation(xInSelf - xOffset, yInSelf - yOffset);
        }
        private static void _dragTranslateEventFromChild(MotionEvent ev,
                                                         int xInChild,
                                                         int yInChild,
                                                         View parentScrollView,
                                                         View child) {
            int xOffset = child.getLeft();
            int yOffset = child.getTop();
            ViewParent parent = child.getParent();
            while(parent != null && (parent instanceof View)) {
                if(parent == parentScrollView) {
                    break;
                }
                xOffset += ((View)parent).getLeft();
                yOffset += ((View)parent).getTop();
                parent = parent.getParent();
            }

            ev.setLocation(xInChild + xOffset, yInChild + yOffset);
        }

        private void _dragNotifyBegin(MotionEvent ev) {
            ZFUIScrollView.native_notifyScrollViewDragBegin(this.owner.get().zfjniPointerOwnerZFUIScrollView, (int)ev.getX(), (int)ev.getY(), System.currentTimeMillis());
        }
        private void _dragNotifyMove(MotionEvent ev) {
            ZFUIScrollView.native_notifyScrollViewDrag(this.owner.get().zfjniPointerOwnerZFUIScrollView, (int)ev.getX(), (int)ev.getY(), System.currentTimeMillis());
        }
        private void _dragNotifyEnd(MotionEvent ev) {
            ZFUIScrollView.native_notifyScrollViewDragEnd(this.owner.get().zfjniPointerOwnerZFUIScrollView, System.currentTimeMillis(), true);
        }
        private void _dragNotifyCancel(MotionEvent ev) {
            ZFUIScrollView.native_notifyScrollViewDragEnd(this.owner.get().zfjniPointerOwnerZFUIScrollView, System.currentTimeMillis(), false);
        }

        // ============================================================
        // tracking logic
        /*
         * here's a memo for the tracking logic:
         *
         * scrollView can be embeded with one or more scrollView
         *
         * parent would receive dispatchTouchEvent and always return true,
         * and it will find the proper child scrollView during touch down,
         * if any, that child would start tracking and intercept all touch events from parent
         *
         * if that child reaches scroll end and continue dragging,
         * it would then notify parent that "we need no tracking anymore",
         * so that parent may start dragging when child reaches end
         *
         * touch events would always dispatched by parent scroll view
         * instead of original dispatch logic of Android
         */
        private ZFUIScrollView.JavaImpl _dragTrackingParent = null;
        private ZFUIScrollView.JavaImpl _dragTrackingChild = null;

        public void dispatchTouchEvent(MotionEvent ev) {
            switch(ev.getAction()) {
                case MotionEvent.ACTION_DOWN:
                    _dragTrackingProcessMouseDown(ev);
                    break;
                case MotionEvent.ACTION_MOVE:
                    _dragTrackingProcessMouseMove(ev);
                    break;
                case MotionEvent.ACTION_UP:
                    _dragTrackingProcessMouseUp(ev);
                    break;
                case MotionEvent.ACTION_CANCEL:
                    _dragTrackingProcessMouseCancel(ev);
                    break;
                default:
                    break;
            }
        }
        private void _dragTrackingProcessMouseDown(MotionEvent ev) {
            _dragState = _DragState.Tracking;
            _dragTrackingChild = _dragFindChildScrollView(this.owner.get(), (int)ev.getX(), (int)ev.getY());
            if(_dragTrackingChild != null && _dragTrackingChild._dragScrollableOrBounceable()) {
                _dragTrackingChild._dragTrackingParent = this;

                MotionEvent evTmp = MotionEvent.obtain(ev);
                _dragTranslateEventToChild(evTmp, (int)evTmp.getX(), (int)evTmp.getY(), this.owner.get(), _dragTrackingChild.owner.get());
                _dragTrackingChild._dragTrackingProcessMouseDown(evTmp);
                evTmp.recycle();
                return ;
            }

            _dragTrackingChild = null;
            _dragProcessMouseDown(ev);
        }
        private void _dragTrackingProcessMouseMove(MotionEvent ev) {
            if(_dragTrackingChild != null) {
                MotionEvent evTmp = MotionEvent.obtain(ev);
                _dragTranslateEventToChild(evTmp, (int)evTmp.getX(), (int)evTmp.getY(), this.owner.get(), _dragTrackingChild.owner.get());
                _dragTrackingChild._dragTrackingProcessMouseMove(evTmp);
                evTmp.recycle();
                return ;
            }

            if(_dragTrackingParent == null || _dragState != _DragState.Tracking) {
                _dragProcessMouseMove(ev);
                return ;
            }

            int xOffset = (int)(ev.getX() - _dragMouseDownEventSaved.getX());
            int yOffset = (int)(ev.getY() - _dragMouseDownEventSaved.getY());
            if(Math.abs(xOffset) <= _dragTolerance && Math.abs(yOffset) <= _dragTolerance) {
                return ;
            }
            boolean stopTracking = false;
            if((Math.abs(yOffset) >= Math.abs(xOffset))) { // drag vertical
                if(yOffset > 0) {
                    if(_dragScrollableTop()) {
                        _dragTrackingParent = null;
                    }
                    else if(_dragTrackingParent._dragScrollableTop()) {
                        stopTracking = true;
                    }
                    else if(_dragScrollableOrBounceableTop()) {
                        _dragTrackingParent = null;
                    }
                    else if(_dragTrackingParent._dragScrollableOrBounceableTop()) {
                        stopTracking = true;
                    }
                }
                else if(yOffset < 0) {
                    if(_dragScrollableBottom()) {
                        _dragTrackingParent = null;
                    }
                    else if(_dragTrackingParent._dragScrollableBottom()) {
                        stopTracking = true;
                    }
                    else if(_dragScrollableOrBounceableBottom()) {
                        _dragTrackingParent = null;
                    }
                    else if(_dragTrackingParent._dragScrollableOrBounceableBottom()) {
                        stopTracking = true;
                    }
                }
            }
            else { // drag horizontal
                if(xOffset > 0) {
                    if(_dragScrollableLeft()) {
                        _dragTrackingParent = null;
                    }
                    else if(_dragTrackingParent._dragScrollableLeft()) {
                        stopTracking = true;
                    }
                    else if(_dragScrollableOrBounceableLeft()) {
                        _dragTrackingParent = null;
                    }
                    else if(_dragTrackingParent._dragScrollableOrBounceableLeft()) {
                        stopTracking = true;
                    }
                }
                else if(xOffset < 0) {
                    if(_dragScrollableRight()) {
                        _dragTrackingParent = null;
                    }
                    else if(_dragTrackingParent._dragScrollableRight()) {
                        stopTracking = true;
                    }
                    else if(_dragScrollableOrBounceableRight()) {
                        _dragTrackingParent = null;
                    }
                    else if(_dragTrackingParent._dragScrollableOrBounceableRight()) {
                        stopTracking = true;
                    }
                }
            }

            if(stopTracking) {
                MotionEvent mouseDownSaved = MotionEvent.obtain(_dragMouseDownEventSaved);

                // stop child's drag and restore parent's touch down
                MotionEvent evTmp = MotionEvent.obtain(ev);
                evTmp.setAction(MotionEvent.ACTION_CANCEL);
                _dragProcessMouseCancel(ev);
                evTmp.recycle();

                _dragMouseDownEventRecycle();
                _dragTranslateEventFromChild(mouseDownSaved, (int)mouseDownSaved.getX(), (int)mouseDownSaved.getY(), _dragTrackingParent.owner.get(), this.owner.get());
                _dragTrackingParent._dragTrackingChild = null;
                _dragTrackingParent._dragProcessMouseDown(mouseDownSaved);
                mouseDownSaved.recycle();
            }
            else {
                _dragProcessMouseMove(ev);
            }
        }
        private void _dragTrackingProcessMouseUp(MotionEvent ev) {
            if(_dragTrackingChild != null) {
                ZFUIScrollView.JavaImpl tmp = _dragTrackingChild;
                _dragTrackingChild._dragTrackingParent = null;
                _dragTrackingChild = null;

                MotionEvent evTmp = MotionEvent.obtain(ev);
                _dragTranslateEventToChild(evTmp, (int)evTmp.getX(), (int)evTmp.getY(), this.owner.get(), tmp.owner.get());
                tmp._dragTrackingProcessMouseUp(evTmp);
                evTmp.recycle();
                return ;
            }
            _dragProcessMouseUp(ev);
        }
        private void _dragTrackingProcessMouseCancel(MotionEvent ev) {
            if(_dragTrackingChild != null) {
                ZFUIScrollView.JavaImpl tmp = _dragTrackingChild;
                _dragTrackingChild._dragTrackingParent = null;
                _dragTrackingChild = null;

                MotionEvent evTmp = MotionEvent.obtain(ev);
                _dragTranslateEventToChild(evTmp, (int)evTmp.getX(), (int)evTmp.getY(), this.owner.get(), tmp.owner.get());
                tmp._dragTrackingProcessMouseCancel(evTmp);
                evTmp.recycle();
                return ;
            }
            _dragProcessMouseCancel(ev);
        }

        // ============================================================
        // touch event process
        private View _dragProcessChildIntercepted = null;
        private void _dragProcessMouseDown(MotionEvent ev) {
            _dragMouseDownEventRecycle();

            if(!_dragScrollableOrBounceable()) {
                _dragState = _DragState.Ignored;
                _dragProcessChildIntercepted = _dragFindChild(this.owner.get(), (int)ev.getX(), (int)ev.getY());

                if(_dragProcessChildIntercepted != null) {
                    MotionEvent evTmp = MotionEvent.obtain(ev);
                    _dragTranslateEventToChild(evTmp, (int)evTmp.getX(), (int)evTmp.getY(), this.owner.get(), _dragProcessChildIntercepted);
                    _dragProcessChildIntercepted.dispatchTouchEvent(evTmp);
                    evTmp.recycle();
                }
                return ;
            }

            _dragMouseDownEventSaved = MotionEvent.obtain(ev);

            if(this.owner.get()._scrollAnimating) {
                _dragProcessChildIntercepted = null;
                _dragState = _DragState.Dragging;
                _dragNotifyBegin(ev);
                return ;
            }

            _dragProcessChildIntercepted = _dragFindChild(this.owner.get(), (int)ev.getX(), (int)ev.getY());
            if(_dragProcessChildIntercepted != null) {
                _dragMouseDownTimeoutStart();
            }
        }
        private void _dragProcessMouseMove(MotionEvent ev) {
            if(_dragState == _DragState.Dragging) {
                _dragNotifyMove(ev);
                return ;
            }
            else if(_dragState == _DragState.Ignored) {
                if(_dragProcessChildIntercepted != null) {
                    MotionEvent evTmp = MotionEvent.obtain(ev);
                    _dragTranslateEventToChild(evTmp, (int)evTmp.getX(), (int)evTmp.getY(), this.owner.get(), _dragProcessChildIntercepted);
                    _dragProcessChildIntercepted.dispatchTouchEvent(evTmp);
                    evTmp.recycle();
                }
                return ;
            }

            int xOffset = (int)(ev.getX() - _dragMouseDownEventSaved.getX());
            int yOffset = (int)(ev.getY() - _dragMouseDownEventSaved.getY());
            int xOffsetAbs = Math.abs(xOffset);
            int yOffsetAbs = Math.abs(yOffset);
            if(xOffsetAbs <= _dragTolerance && yOffsetAbs <= _dragTolerance) {
                return ;
            }

            boolean childIntercepted = false;
            if(_dragProcessChildIntercepted != null) {
                if(yOffsetAbs >= xOffsetAbs * 1.5f) { // drag vertical
                    if(yOffset > 0) {
                        if(!_dragScrollableOrBounceableTop()) {
                            childIntercepted = true;
                        }
                    }
                    else {
                        if(!_dragScrollableOrBounceableBottom()) {
                            childIntercepted = true;
                        }
                    }
                }
                else if(xOffsetAbs >= yOffsetAbs * 1.5f) { // drag horizontal
                    if(xOffset > 0) {
                        if(!_dragScrollableOrBounceableLeft()) {
                            childIntercepted = true;
                        }
                    }
                    else {
                        if(!_dragScrollableOrBounceableRight()) {
                            childIntercepted = true;
                        }
                    }
                }
            }

            if(childIntercepted) {
                _dragState = _DragState.Ignored;
                _dragMouseDownEventResend();
                MotionEvent evTmp = MotionEvent.obtain(ev);
                _dragTranslateEventToChild(evTmp, (int)evTmp.getX(), (int)evTmp.getY(), this.owner.get(), _dragProcessChildIntercepted);
                _dragProcessChildIntercepted.dispatchTouchEvent(evTmp);
                evTmp.recycle();
            }
            else {
                _dragState = _DragState.Dragging;
                _dragMouseDownTimeoutStop();
                MotionEvent evTmp = MotionEvent.obtain(_dragMouseDownEventSaved);
                _dragMouseDownEventRecycle();
                _dragNotifyBegin(evTmp);
                evTmp.recycle();
                _dragNotifyMove(ev);
            }
        }
        private void _dragProcessMouseUp(MotionEvent ev) {
            if(_dragState == _DragState.Dragging) {
                _dragNotifyEnd(ev);
                return ;
            }
            else if(_dragState == _DragState.Ignored) {
                if(_dragProcessChildIntercepted != null) {
                    MotionEvent evTmp = MotionEvent.obtain(ev);
                    _dragTranslateEventToChild(evTmp, (int)evTmp.getX(), (int)evTmp.getY(), this.owner.get(), _dragProcessChildIntercepted);
                    _dragProcessChildIntercepted.dispatchTouchEvent(evTmp);
                    evTmp.recycle();
                }
                return ;
            }

            // mouse down then up quickly
            _dragState = _DragState.Tracking;
            if(_dragProcessChildIntercepted != null) {
                _dragMouseDownEventResend();
                MotionEvent evTmp = MotionEvent.obtain(ev);
                _dragTranslateEventToChild(evTmp, (int)evTmp.getX(), (int)evTmp.getY(), this.owner.get(), _dragProcessChildIntercepted);
                _dragProcessChildIntercepted.dispatchTouchEvent(evTmp);
                evTmp.recycle();
            }
            else {
                _dragMouseDownEventRecycle();
            }
        }
        private void _dragProcessMouseCancel(MotionEvent ev) {
            if(_dragState == _DragState.Dragging) {
                _dragNotifyCancel(ev);
                return ;
            }
            else if(_dragState == _DragState.Ignored) {
                if(_dragProcessChildIntercepted != null) {
                    MotionEvent evTmp = MotionEvent.obtain(ev);
                    _dragTranslateEventToChild(evTmp, (int)evTmp.getX(), (int)evTmp.getY(), this.owner.get(), _dragProcessChildIntercepted);
                    _dragProcessChildIntercepted.dispatchTouchEvent(evTmp);
                    evTmp.recycle();
                }
                return ;
            }

            // mouse down then cancel quickly
            _dragState = _DragState.Tracking;
            _dragMouseDownEventRecycle();
        }

        // ============================================================
        // mouse down timeout
        private int _dragMouseDownTimeoutTaskId = 0;
        private static final int _dragMouseDownTimeoutDelay = 200;
        private static Handler _dragMouseDownTimeoutHandler = new Handler() {
            @Override
            public void handleMessage(Message msg) {
                super.handleMessage(msg);
                ZFUIScrollView.JavaImpl scrollView = (ZFUIScrollView.JavaImpl)msg.obj;
                int taskId = msg.what;
                if(scrollView._dragMouseDownTimeoutTaskId != taskId) {
                    return ;
                }
                scrollView._dragMouseDownTimeoutOnTimeout();
            }
        };
        private void _dragMouseDownTimeoutStart() {
            _dragMouseDownTimeoutStop();
            _dragMouseDownTimeoutHandler.sendMessageDelayed(Message.obtain(
                _dragMouseDownTimeoutHandler, _dragMouseDownTimeoutTaskId, this),
                _dragMouseDownTimeoutDelay);
        }
        private void _dragMouseDownTimeoutStop() {
            _dragMouseDownTimeoutHandler.removeMessages(_dragMouseDownTimeoutTaskId);
            ++_dragMouseDownTimeoutTaskId;
        }
        private void _dragMouseDownTimeoutOnTimeout() {
            // mouse without drag and timeout,
            // restore mouse down event and ignore all following events
            _dragState = _DragState.Ignored;

            // restore mouse down
            _dragMouseDownEventResend();
        }
        private void _dragMouseDownEventResend() {
            if(_dragMouseDownEventSaved != null && _dragProcessChildIntercepted != null) {
                _dragTranslateEventToChild(_dragMouseDownEventSaved,
                    (int)_dragMouseDownEventSaved.getX(),
                    (int)_dragMouseDownEventSaved.getY(),
                    this.owner.get(),
                    _dragProcessChildIntercepted);
                MotionEvent evTmp = MotionEvent.obtain(_dragMouseDownEventSaved);
                _dragMouseDownEventRecycle();
                _dragProcessChildIntercepted.dispatchTouchEvent(evTmp);
                evTmp.recycle();
            }
            else {
                _dragMouseDownEventRecycle();
            }
        }
        private void _dragMouseDownEventRecycle() {
            if(_dragMouseDownEventSaved != null) {
                _dragMouseDownEventSaved.recycle();
                _dragMouseDownEventSaved = null;
            }
        }
    }
}
