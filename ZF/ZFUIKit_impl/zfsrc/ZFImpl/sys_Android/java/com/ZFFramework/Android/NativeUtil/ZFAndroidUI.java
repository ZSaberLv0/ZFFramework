package com.ZFFramework.Android.NativeUtil;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.Context;
import android.graphics.Rect;
import android.graphics.RectF;
import android.os.Handler;
import android.os.Message;
import android.util.DisplayMetrics;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.MeasureSpec;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.widget.TextView;

@SuppressLint("DefaultLocale")
public class ZFAndroidUI {
    // screen info
    private static DisplayMetrics _displayMetricsCache = new DisplayMetrics();
    public static ZFAndroidSize screenSize(Context context) {
        ((WindowManager)context.getSystemService(Context.WINDOW_SERVICE)).getDefaultDisplay().getMetrics(_displayMetricsCache);
        return new ZFAndroidSize(_displayMetricsCache.widthPixels, _displayMetricsCache.heightPixels);
    }
    public static float screenDensity(Context context) {
        ((WindowManager)context.getSystemService(Context.WINDOW_SERVICE)).getDefaultDisplay().getMetrics(_displayMetricsCache);
        return _displayMetricsCache.density;
    }

    // activity view info
    public static View activityRootView(Activity activity) {
        return activity.findViewById(android.R.id.content);
    }

    // view tree
    public static interface ViewTreeOutput {
        public void outputLine(String s);
    }
    public static void viewTreePrint(ViewTreeOutput output, View view) {
        output.outputLine("====================  view tree begin  ====================");
        _viewTreePrintRecursion(output, view, 0, 0);
        output.outputLine("====================   view tree end   ====================");
    }
    public static String native_viewTreePrint(Object view) {
        final StringBuilder sb = new StringBuilder();
        viewTreePrint(new ViewTreeOutput() {
            @Override
            public void outputLine(String s) {
                sb.append(s);
                sb.append("\n");
            }
        }, (View)view);
        return sb.toString();
    }
    public static void viewTreePrintToLog(View view) {
        viewTreePrintToLog("", view);
    }
    public static void viewTreePrintToLog(final String sTag, View view) {
        viewTreePrint(new ViewTreeOutput() {
            @Override
            public void outputLine(String s) {
                ZFAndroidLog.p(sTag, s);
            }
        }, view);
    }
    public static void viewTreePrintToLogAfterDelay(long delayInMiliSeconds, String sTag, View view) {
        _ViewTreePrintData data = new _ViewTreePrintData();
        data.tag = sTag;
        data.view = view;
        _viewTreePrintAfterDelayHandler.sendMessageDelayed(Message.obtain(_viewTreePrintAfterDelayHandler, 0, data), delayInMiliSeconds);
    }
    private static class _ViewTreePrintData {
        public String tag = null;
        public View view = null;
    }
    private static Handler _viewTreePrintAfterDelayHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            _ViewTreePrintData data = (_ViewTreePrintData)msg.obj;
            viewTreePrintToLog(data.tag, data.view);
        }
    };
    private static void _viewTreePrintRecursion(ViewTreeOutput output, View view, int depth, int index) {
        if(view == null) {
            output.outputLine("| null");
            return ;
        }
        String sIndent = "";
        for(int i = 0; i < depth; ++i) {
            sIndent += "| ";
        }

        String sDetail = "";
        if(view.getTag() != null && (view.getTag() instanceof String)) {
            sDetail += " (" + ((String)view.getTag()) + ")";
        }
        if(view instanceof TextView) {
            sDetail += " \"" + ((TextView)view).getText().toString() + "\"";
        }
        if(view.isFocusable()) {
            sDetail += " focusable";
        }

        output.outputLine(String.format("|%2d %s[%s %08X (%d, %d, %d, %d)%s]%s%s",
            index,
            sIndent,
            view.getClass().getSimpleName(),
            view.hashCode(),
            view.getLeft(), view.getTop(), view.getWidth(), view.getHeight(),
            sDetail,
            (view.getVisibility() == View.VISIBLE) ? "" : (" " + ZFAndroidUI.visibilityToString(view.getVisibility())),
            view.isEnabled() ? "" : " Disabled"
            ));

        if(view instanceof ViewGroup) {
            ViewGroup viewGroup = (ViewGroup)view;
            int n = viewGroup.getChildCount();
            for(int i = 0; i < n; ++i) {
                _viewTreePrintRecursion(output, viewGroup.getChildAt(i), depth + 1, i);
            }
        }
    }

    // value info
    public static String rectToString(Rect rect) {
        return String.format("(%d, %d, %d, %d)", rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
    }
    public static String rectFToString(RectF rect) {
        return String.format("(%f, %f, %f, %f)", rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
    }

    public static String visibilityToString(int visibility) {
        if(visibility == View.VISIBLE) {
            return "Visible";
        }
        else if(visibility == View.INVISIBLE) {
            return "Invisible";
        }
        else if(visibility == View.GONE) {
            return "Gone";
        }
        else {
            return String.format("<unknown visibility %d>", visibility);
        }
    }

    public static String measureSpecToString(int spec) {
        int mode = MeasureSpec.getMode(spec);
        int size = MeasureSpec.getSize(spec);
        if(mode == MeasureSpec.AT_MOST) {
            return String.format("(at most %d)", size);
        }
        else if(mode == MeasureSpec.EXACTLY) {
            return String.format("(exactly %d)", size);
        }
        else if(mode == MeasureSpec.UNSPECIFIED) {
            return String.format("(unspecified %d)", size);
        }
        else {
            return String.format("<unknown spec: %d %d(0x%08X)>", mode, size, spec);
        }
    }

    public static String mouseActionToString(int action) {
        switch(action) {
            case MotionEvent.ACTION_DOWN:
                return "MouseDown";
            case MotionEvent.ACTION_MOVE:
                return "MouseMove";
            case MotionEvent.ACTION_UP:
                return "MouseUp";
            case MotionEvent.ACTION_CANCEL:
                return "MouseCancel";
            case MotionEvent.ACTION_OUTSIDE:
                return "MouseOutside";
            default:
                return String.format("<MouseUnknown %d>", action);
        }
    }
    public static String mouseEventToString(MotionEvent event) {
        return String.format("%s (%d, %d)", ZFAndroidUI.mouseActionToString(event.getAction()), event.getX(), event.getY());
    }
    public static String keyActionToString(int action) {
        switch(action) {
            case KeyEvent.ACTION_DOWN:
                return "KeyDown";
            case KeyEvent.ACTION_UP:
                return "KeyUp";
            case KeyEvent.ACTION_MULTIPLE:
                return "KeyMultiple";
            default:
                return String.format("<KeyUnknown %d>", action);
        }
    }
    public static String keyCodeToString(int keyCode) {
        return KeyEvent.keyCodeToString(keyCode);
    }
    public static String keyEventToString(KeyEvent event) {
        return String.format("%s %s", ZFAndroidUI.keyActionToString(event.getAction()), ZFAndroidUI.keyCodeToString(event.getKeyCode()));
    }
}
