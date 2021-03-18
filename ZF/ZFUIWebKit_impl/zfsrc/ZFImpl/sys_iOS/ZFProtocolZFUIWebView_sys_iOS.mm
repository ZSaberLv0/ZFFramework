#include "ZFImpl_sys_iOS_ZFUIWebKit_impl.h"
#include "ZFUIWebKit/protocol/ZFProtocolZFUIWebView.h"

#if ZF_ENV_sys_iOS

#import <WebKit/WebKit.h>

@interface _ZFP_ZFUIWebViewImpl_sys_iOS_View : WKWebView<WKNavigationDelegate, WKUIDelegate>
@property (nonatomic, assign) BOOL _ZFP_webLoadingSaved;
@property (nonatomic, assign) ZFUIWebView *_ZFP_ownerZFUIWebView;
@property (nonatomic, assign) ZFPROTOCOL_INTERFACE_CLASS(ZFUIWebView) *_ZFP_impl;
@end
@implementation _ZFP_ZFUIWebViewImpl_sys_iOS_View
- (void)_notifyStateChange:(WKWebView *)webView
{
    if(self._ZFP_webLoadingSaved != webView.loading)
    {
        self._ZFP_webLoadingSaved = webView.loading;
        self._ZFP_impl->notifyWebLoadStateChanged(self._ZFP_ownerZFUIWebView);
    }
}
- (void)webView:(WKWebView *)webView decidePolicyForNavigationAction:(WKNavigationAction *)navigationAction decisionHandler:(void (^)(WKNavigationActionPolicy))decisionHandler
{
    decisionHandler(WKNavigationActionPolicyAllow);
}
- (void)webView:(WKWebView *)webView didStartProvisionalNavigation:(WKNavigation *)navigation
{
    [self _notifyStateChange:webView];
}
- (void)webView:(WKWebView *)webView didReceiveServerRedirectForProvisionalNavigation:(WKNavigation *)navigation
{
    [self _notifyStateChange:webView];
}
- (void)webView:(WKWebView *)webView didFailProvisionalNavigation:(WKNavigation *)navigation withError:(NSError *)error
{
    [self _notifyStateChange:webView];
}
- (void)webView:(WKWebView *)webView didCommitNavigation:(WKNavigation *)navigation
{
    [self _notifyStateChange:webView];
}
- (void)webView:(WKWebView *)webView didFinishNavigation:(WKNavigation *)navigation
{
    [self _notifyStateChange:webView];
}
- (void)webView:(WKWebView *)webView didFailNavigation:(WKNavigation *)navigation withError:(NSError *)error
{
    [self _notifyStateChange:webView];
}
- (void)webViewWebContentProcessDidTerminate:(WKWebView *)webView
{
    [self _notifyStateChange:webView];
}

- (void)webView:(WKWebView *)webView runJavaScriptAlertPanelWithMessage:(NSString *)message initiatedByFrame:(WKFrameInfo *)frame completionHandler:(void (^)())completionHandler
{
    UIAlertController *alertController = [UIAlertController alertControllerWithTitle:@"Message" message:message preferredStyle:UIAlertControllerStyleAlert];
    [alertController addAction:[UIAlertAction actionWithTitle:@"OK" style:UIAlertActionStyleDefault handler:^(UIAlertAction *action) {
        completionHandler();
    }]];
    [[UIApplication sharedApplication].keyWindow.rootViewController presentViewController:alertController animated:YES completion:nil];
}
- (void)webView:(WKWebView *)webView runJavaScriptConfirmPanelWithMessage:(NSString *)message initiatedByFrame:(WKFrameInfo *)frame completionHandler:(void (^)(BOOL))completionHandler
{
    UIAlertController *alertController = [UIAlertController alertControllerWithTitle:@"Message" message:message preferredStyle:UIAlertControllerStyleAlert];
    [alertController addAction:([UIAlertAction actionWithTitle:@"Cancel" style:UIAlertActionStyleCancel handler:^(UIAlertAction *action) {
        completionHandler(NO);
    }])];
    [alertController addAction:([UIAlertAction actionWithTitle:@"OK" style:UIAlertActionStyleDefault handler:^(UIAlertAction *action) {
        completionHandler(YES);
    }])];
    [[UIApplication sharedApplication].keyWindow.rootViewController presentViewController:alertController animated:YES completion:nil];
}
- (void)webView:(WKWebView *)webView runJavaScriptTextInputPanelWithPrompt:(NSString *)prompt defaultText:(NSString *)defaultText initiatedByFrame:(WKFrameInfo *)frame completionHandler:(void (^)(NSString *))completionHandler
{
    UIAlertController *alertController = [UIAlertController alertControllerWithTitle:prompt message:@"" preferredStyle:UIAlertControllerStyleAlert];
    [alertController addTextFieldWithConfigurationHandler:^(UITextField *textField) {
        textField.text = defaultText;
    }];
    [alertController addAction:([UIAlertAction actionWithTitle:@"OK" style:UIAlertActionStyleDefault handler:^(UIAlertAction *action) {
        completionHandler(alertController.textFields[0].text != nil ? alertController.textFields[0].text : @"");
    }])];

    [[UIApplication sharedApplication].keyWindow.rootViewController presentViewController:alertController animated:YES completion:nil];
}
@end

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIWebViewImpl_sys_iOS, ZFUIWebView, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("iOS:WKWebView")
public:
    virtual void *nativeWebViewCreate(ZF_IN ZFUIWebView *webView)
    {
        _ZFP_ZFUIWebViewImpl_sys_iOS_View *nativeWebView = [_ZFP_ZFUIWebViewImpl_sys_iOS_View new];
        nativeWebView.navigationDelegate = nativeWebView;
        nativeWebView.UIDelegate = nativeWebView;
        nativeWebView._ZFP_ownerZFUIWebView = webView;
        nativeWebView._ZFP_impl = this;
        return (__bridge_retained void *)nativeWebView;
    }
    virtual void nativeWebViewDestroy(ZF_IN ZFUIWebView *webView,
                                      ZF_IN void *nativeWebView)
    {
        _ZFP_ZFUIWebViewImpl_sys_iOS_View *nativeWebViewTmp = (__bridge_transfer _ZFP_ZFUIWebViewImpl_sys_iOS_View *)nativeWebView;
        // load empty data to reduce memory usage
        [nativeWebViewTmp loadHTMLString:@"" baseURL:nil];
        nativeWebViewTmp = nil;
    }

    virtual void webLoadUrl(ZF_IN ZFUIWebView *webView,
                            ZF_IN const zfchar *url)
    {
        _ZFP_ZFUIWebViewImpl_sys_iOS_View *nativeWebView = (__bridge _ZFP_ZFUIWebViewImpl_sys_iOS_View *)webView->nativeImplView();
        [nativeWebView loadRequest:[NSURLRequest requestWithURL:[NSURL URLWithString:[NSString stringWithCString:url encoding:NSUTF8StringEncoding]]]];
    }
    virtual void webLoadHtml(ZF_IN ZFUIWebView *webView,
                             ZF_IN const zfchar *html,
                             ZF_IN_OPT const zfchar *baseUrl = zfnull)
    {
        _ZFP_ZFUIWebViewImpl_sys_iOS_View *nativeWebView = (__bridge _ZFP_ZFUIWebViewImpl_sys_iOS_View *)webView->nativeImplView();
        NSURL *baseUrlTmp = nil;
        if(baseUrl != zfnull)
        {
            baseUrlTmp = [NSURL URLWithString:[NSString stringWithCString:baseUrl encoding:NSUTF8StringEncoding]];
        }
        [nativeWebView loadHTMLString:[NSString stringWithCString:html encoding:NSUTF8StringEncoding] baseURL:baseUrlTmp];
    }
    virtual void webReload(ZF_IN ZFUIWebView *webView)
    {
        _ZFP_ZFUIWebViewImpl_sys_iOS_View *nativeWebView = (__bridge _ZFP_ZFUIWebViewImpl_sys_iOS_View *)webView->nativeImplView();
        [nativeWebView reload];
    }
    virtual void webLoadStop(ZF_IN ZFUIWebView *webView)
    {
        _ZFP_ZFUIWebViewImpl_sys_iOS_View *nativeWebView = (__bridge _ZFP_ZFUIWebViewImpl_sys_iOS_View *)webView->nativeImplView();
        [nativeWebView stopLoading];
    }

    virtual void webGoBack(ZF_IN ZFUIWebView *webView)
    {
        _ZFP_ZFUIWebViewImpl_sys_iOS_View *nativeWebView = (__bridge _ZFP_ZFUIWebViewImpl_sys_iOS_View *)webView->nativeImplView();
        [nativeWebView goBack];
    }
    virtual void webGoForward(ZF_IN ZFUIWebView *webView)
    {
        _ZFP_ZFUIWebViewImpl_sys_iOS_View *nativeWebView = (__bridge _ZFP_ZFUIWebViewImpl_sys_iOS_View *)webView->nativeImplView();
        [nativeWebView goForward];
    }

    virtual zfbool webLoading(ZF_IN ZFUIWebView *webView)
    {
        _ZFP_ZFUIWebViewImpl_sys_iOS_View *nativeWebView = (__bridge _ZFP_ZFUIWebViewImpl_sys_iOS_View *)webView->nativeImplView();
        return nativeWebView.loading;
    }
    virtual zfbool webGoBackAvailable(ZF_IN ZFUIWebView *webView)
    {
        _ZFP_ZFUIWebViewImpl_sys_iOS_View *nativeWebView = (__bridge _ZFP_ZFUIWebViewImpl_sys_iOS_View *)webView->nativeImplView();
        return [nativeWebView canGoBack];
    }
    virtual zfbool webGoForwardAvailable(ZF_IN ZFUIWebView *webView)
    {
        _ZFP_ZFUIWebViewImpl_sys_iOS_View *nativeWebView = (__bridge _ZFP_ZFUIWebViewImpl_sys_iOS_View *)webView->nativeImplView();
        return [nativeWebView canGoForward];
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIWebViewImpl_sys_iOS)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUIWebViewImpl_sys_iOS)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_iOS

