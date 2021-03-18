/**
 * @file ZFProtocolZFUIWebView.h
 * @brief protocol for ZFUIWebView
 */

#ifndef _ZFI_ZFProtocolZFUIWebView_h_
#define _ZFI_ZFProtocolZFUIWebView_h_

#include "ZFCore/ZFProtocol.h"
#include "ZFUIWebKit/ZFUIWebView.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for #ZFUIWebView
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFUIWebView)
public:
    /**
     * @brief create native web view
     */
    virtual void *nativeWebViewCreate(ZF_IN ZFUIWebView *webView) zfpurevirtual;
    /**
     * @brief destroy native web view
     */
    virtual void nativeWebViewDestroy(ZF_IN ZFUIWebView *webView,
                                      ZF_IN void *nativeWebView) zfpurevirtual;

    /**
     * @brief see #ZFUIWebView::webLoadUrl
     */
    virtual void webLoadUrl(ZF_IN ZFUIWebView *webView,
                            ZF_IN const zfchar *url) zfpurevirtual;
    /**
     * @brief see #ZFUIWebView::webLoadHtml
     */
    virtual void webLoadHtml(ZF_IN ZFUIWebView *webView,
                             ZF_IN const zfchar *html,
                             ZF_IN_OPT const zfchar *baseUrl = zfnull) zfpurevirtual;
    /**
     * @brief see #ZFUIWebView::webReload
     */
    virtual void webReload(ZF_IN ZFUIWebView *webView) zfpurevirtual;
    /**
     * @brief see #ZFUIWebView::webLoadStop
     */
    virtual void webLoadStop(ZF_IN ZFUIWebView *webView) zfpurevirtual;

    /**
     * @brief see #ZFUIWebView::webGoBack
     */
    virtual void webGoBack(ZF_IN ZFUIWebView *webView) zfpurevirtual;
    /**
     * @brief see #ZFUIWebView::webGoForward
     */
    virtual void webGoForward(ZF_IN ZFUIWebView *webView) zfpurevirtual;

    /**
     * @brief see #ZFUIWebView::webLoading
     */
    virtual zfbool webLoading(ZF_IN ZFUIWebView *webView) zfpurevirtual;
    /**
     * @brief see #ZFUIWebView::webGoBackAvailable
     */
    virtual zfbool webGoBackAvailable(ZF_IN ZFUIWebView *webView) zfpurevirtual;
    /**
     * @brief see #ZFUIWebView::webGoForwardAvailable
     */
    virtual zfbool webGoForwardAvailable(ZF_IN ZFUIWebView *webView) zfpurevirtual;

    // ============================================================
    // callbacks that implementations must notify
public:
    /**
     * @brief implementations must notify when web load start or stop
     *   (both success or fail)
     */
    zffinal void notifyWebLoadStateChanged(ZF_IN ZFUIWebView *webView)
    {
        webView->_ZFP_ZFUIWebView_notifyWebLoadStateChanged();
    }
ZFPROTOCOL_INTERFACE_END(ZFUIWebView)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFUIWebView_h_

