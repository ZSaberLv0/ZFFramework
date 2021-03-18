#include "ZFImpl_sys_Qt_ZFUIWebKit_impl.h"
#include "ZFUIWebKit/protocol/ZFProtocolZFUIWebView.h"

#if ZF_ENV_sys_Qt && (defined(QT_WEBKITWIDGETS_LIB) || defined(QT_WEBENGINEWIDGETS_LIB))

#ifndef QT_WEBKITWIDGETS_LIB
    #include <QWebEngineView>
    #include <QWebEngineHistory>
    #define _ZFP_ZFUIWebViewImpl_sys_Qt_QWebViewBase QWebEngineView
#else
    #include <QWebView>
    #include <QWebHistory>
    #define _ZFP_ZFUIWebViewImpl_sys_Qt_QWebViewBase QWebView
#endif

class _ZFP_ZFUIWebViewImpl_sys_Qt_View : public _ZFP_ZFUIWebViewImpl_sys_Qt_QWebViewBase
{
    Q_OBJECT

public:
    ZFUIWebView *_ZFP_ownerZFUIWebView;
    ZFPROTOCOL_INTERFACE_CLASS(ZFUIWebView) *_ZFP_impl;
    zfbool _ZFP_loading;

public:
    _ZFP_ZFUIWebViewImpl_sys_Qt_View(ZF_IN ZFUIWebView *ownerZFUIWebView,
                                    ZFPROTOCOL_INTERFACE_CLASS(ZFUIWebView) *impl)
    : _ZFP_ownerZFUIWebView(ownerZFUIWebView)
    , _ZFP_impl(impl)
    , _ZFP_loading(zffalse)
    {
        this->connect(this, SIGNAL(loadStarted()), this, SLOT(_ZFP_webLoadOnStart()));
        this->connect(this, SIGNAL(loadFinished(bool)), this, SLOT(_ZFP_webLoadOnStop(bool)));
    }
    ~_ZFP_ZFUIWebViewImpl_sys_Qt_View(void)
    {
        this->disconnect();
    }

public slots:
    void _ZFP_webLoadOnStart(void)
    {
        _ZFP_loading = zftrue;
        _ZFP_impl->notifyWebLoadStateChanged(_ZFP_ownerZFUIWebView);
    }
    void _ZFP_webLoadOnStop(bool success)
    {
        _ZFP_loading = zffalse;
        _ZFP_impl->notifyWebLoadStateChanged(_ZFP_ownerZFUIWebView);
    }
};

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIWebViewImpl_sys_Qt, ZFUIWebView, ZFProtocolLevel::e_SystemHigh)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Qt:UIWebView")
public:
    virtual void *nativeWebViewCreate(ZF_IN ZFUIWebView *webView)
    {
        return new _ZFP_ZFUIWebViewImpl_sys_Qt_View(webView, this);
    }
    virtual void nativeWebViewDestroy(ZF_IN ZFUIWebView *webView,
                                      ZF_IN void *nativeWebView)
    {
        _ZFP_ZFUIWebViewImpl_sys_Qt_View *nativeWebViewTmp = ZFCastStatic(_ZFP_ZFUIWebViewImpl_sys_Qt_View *, nativeWebView);
        nativeWebViewTmp->deleteLater();
    }

    virtual void webLoadUrl(ZF_IN ZFUIWebView *webView,
                            ZF_IN const zfchar *url)
    {
        _ZFP_ZFUIWebViewImpl_sys_Qt_View *nativeWebView = ZFCastStatic(_ZFP_ZFUIWebViewImpl_sys_Qt_View *, webView->nativeImplView());
        nativeWebView->load(QUrl(QString::fromUtf8(url)));
        nativeWebView->show();
    }
    virtual void webLoadHtml(ZF_IN ZFUIWebView *webView,
                             ZF_IN const zfchar *html,
                             ZF_IN_OPT const zfchar *baseUrl = zfnull)
    {
        _ZFP_ZFUIWebViewImpl_sys_Qt_View *nativeWebView = ZFCastStatic(_ZFP_ZFUIWebViewImpl_sys_Qt_View *, webView->nativeImplView());
        nativeWebView->setHtml(QString::fromUtf8(html),
                               (baseUrl == zfnull)
                               ? QUrl()
                               : QUrl(QString::fromUtf8(baseUrl))
                               );
        nativeWebView->show();
    }
    virtual void webReload(ZF_IN ZFUIWebView *webView)
    {
        _ZFP_ZFUIWebViewImpl_sys_Qt_View *nativeWebView = ZFCastStatic(_ZFP_ZFUIWebViewImpl_sys_Qt_View *, webView->nativeImplView());
        nativeWebView->reload();
    }
    virtual void webLoadStop(ZF_IN ZFUIWebView *webView)
    {
        _ZFP_ZFUIWebViewImpl_sys_Qt_View *nativeWebView = ZFCastStatic(_ZFP_ZFUIWebViewImpl_sys_Qt_View *, webView->nativeImplView());
        nativeWebView->stop();
    }

    virtual void webGoBack(ZF_IN ZFUIWebView *webView)
    {
        _ZFP_ZFUIWebViewImpl_sys_Qt_View *nativeWebView = ZFCastStatic(_ZFP_ZFUIWebViewImpl_sys_Qt_View *, webView->nativeImplView());
        nativeWebView->back();
    }
    virtual void webGoForward(ZF_IN ZFUIWebView *webView)
    {
        _ZFP_ZFUIWebViewImpl_sys_Qt_View *nativeWebView = ZFCastStatic(_ZFP_ZFUIWebViewImpl_sys_Qt_View *, webView->nativeImplView());
        nativeWebView->forward();
    }

    virtual zfbool webLoading(ZF_IN ZFUIWebView *webView)
    {
        _ZFP_ZFUIWebViewImpl_sys_Qt_View *nativeWebView = ZFCastStatic(_ZFP_ZFUIWebViewImpl_sys_Qt_View *, webView->nativeImplView());
        return nativeWebView->_ZFP_loading;
    }
    virtual zfbool webGoBackAvailable(ZF_IN ZFUIWebView *webView)
    {
        _ZFP_ZFUIWebViewImpl_sys_Qt_View *nativeWebView = ZFCastStatic(_ZFP_ZFUIWebViewImpl_sys_Qt_View *, webView->nativeImplView());
        return nativeWebView->history()->canGoBack();
    }
    virtual zfbool webGoForwardAvailable(ZF_IN ZFUIWebView *webView)
    {
        _ZFP_ZFUIWebViewImpl_sys_Qt_View *nativeWebView = ZFCastStatic(_ZFP_ZFUIWebViewImpl_sys_Qt_View *, webView->nativeImplView());
        return nativeWebView->history()->canGoForward();
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIWebViewImpl_sys_Qt)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUIWebViewImpl_sys_Qt)

ZF_NAMESPACE_GLOBAL_END

#include "ZFProtocolZFUIWebView_sys_Qt.moc"
#endif // #if ZF_ENV_sys_Qt

