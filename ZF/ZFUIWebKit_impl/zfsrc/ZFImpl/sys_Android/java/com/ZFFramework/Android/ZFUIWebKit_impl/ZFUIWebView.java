package com.ZFFramework.Android.ZFUIWebKit_impl;

import java.lang.ref.WeakReference;
import com.ZFFramework.Android.ZF_impl.ZFMainEntry;
import android.content.Context;
import android.graphics.Bitmap;
import android.webkit.WebResourceRequest;
import android.webkit.WebView;
import android.webkit.WebViewClient;

public final class ZFUIWebView extends WebView {
    public static class ZFUIWebViewWebClient extends WebViewClient {
        private WeakReference<ZFUIWebView> _owner = null;
        public ZFUIWebViewWebClient(ZFUIWebView owner) {
            _owner = new WeakReference<ZFUIWebView>(owner);
        }

        @Override
        public boolean shouldOverrideUrlLoading(WebView view, WebResourceRequest request) {
            if(_owner.get()._ZFP_webLoading) {
                _owner.get()._ZFP_webRedirect = true;
            }
            _owner.get()._ZFP_webLoading = true;
            return super.shouldOverrideUrlLoading(view, request);
        }

        @Override
        public void onPageStarted(WebView view, String url, Bitmap facIcon) {
            _owner.get()._ZFP_webLoading = true;
            if(_owner.get()._ZFP_zfjniPointerOwnerZFUIWebView != 0) {
                ZFUIWebView.native_ZFUIWebView_notifyWebLoadStateChanged(_owner.get()._ZFP_zfjniPointerOwnerZFUIWebView);
            }
        }

        @Override
        public void onPageFinished(WebView view, String url) {
            if(!_owner.get()._ZFP_webRedirect) {
                _owner.get()._ZFP_webLoading = false;
            }

            if(!_owner.get()._ZFP_webLoading && !_owner.get()._ZFP_webRedirect) {
                if(_owner.get()._ZFP_zfjniPointerOwnerZFUIWebView != 0) {
                    ZFUIWebView.native_ZFUIWebView_notifyWebLoadStateChanged(_owner.get()._ZFP_zfjniPointerOwnerZFUIWebView);
                }
            }
            else {
                _owner.get()._ZFP_webRedirect = false;
            }
        }
    }

    public static Object native_nativeWebViewCreate(long zfjniPointerOwnerZFUIWebView) {
        ZFUIWebView nativeWebView = new ZFUIWebView(ZFMainEntry.mainEntryActivity());
        nativeWebView._ZFP_zfjniPointerOwnerZFUIWebView = zfjniPointerOwnerZFUIWebView;
        return nativeWebView;
    }
    public static void native_nativeWebViewDestroy(Object nativeWebView) {
        ZFUIWebView nativeWebViewTmp = (ZFUIWebView)nativeWebView;
        nativeWebViewTmp.setWebViewClient(null);
        nativeWebViewTmp._ZFP_zfjniPointerOwnerZFUIWebView = 0;
    }

    public static void native_webLoadUrl(Object nativeWebView, Object url) {
        ZFUIWebView nativeWebViewTmp = (ZFUIWebView)nativeWebView;
        nativeWebViewTmp.loadUrl((String)url);
    }
    public static void native_webLoadHtml(Object nativeWebView, Object html, Object baseUrl) {
        ZFUIWebView nativeWebViewTmp = (ZFUIWebView)nativeWebView;
        nativeWebViewTmp.loadDataWithBaseURL((String)baseUrl, (String)html, null, "utf-8", null);
    }
    public static void native_webReload(Object nativeWebView) {
        ZFUIWebView nativeWebViewTmp = (ZFUIWebView)nativeWebView;
        nativeWebViewTmp.reload();
    }
    public static void native_webLoadStop(Object nativeWebView) {
        ZFUIWebView nativeWebViewTmp = (ZFUIWebView)nativeWebView;
        nativeWebViewTmp.stopLoading();
    }

    public static void native_webGoBack(Object nativeWebView) {
        ZFUIWebView nativeWebViewTmp = (ZFUIWebView)nativeWebView;
        nativeWebViewTmp.goBack();
    }
    public static void native_webGoForward(Object nativeWebView) {
        ZFUIWebView nativeWebViewTmp = (ZFUIWebView)nativeWebView;
        nativeWebViewTmp.goForward();
    }

    public static boolean native_webLoading(Object nativeWebView) {
        ZFUIWebView nativeWebViewTmp = (ZFUIWebView)nativeWebView;
        return nativeWebViewTmp._ZFP_webLoading;
    }

    // ============================================================
    public static native void native_ZFUIWebView_notifyWebLoadStateChanged(long zfjniPointerOwnerZFUIWebView);

    // ============================================================
    public long _ZFP_zfjniPointerOwnerZFUIWebView = 0;
    public boolean _ZFP_webLoading = false;
    public boolean _ZFP_webRedirect = false;

    public ZFUIWebView(Context context) {
        super(context);
        this.setWebViewClient(new ZFUIWebViewWebClient(this));
    }
}
