#include "ZFUIWebView.h"
#include "protocol/ZFProtocolZFUIWebView.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFSTYLE_DEFAULT_DEFINE(ZFUIWebView)

// ============================================================
// ZFUIWebView
ZFOBJECT_REGISTER(ZFUIWebView)

ZFOBSERVER_EVENT_REGISTER(ZFUIWebView, WebLoadStateOnChange)

ZFMETHOD_DEFINE_1(ZFUIWebView, void, webLoadUrl,
                  ZFMP_IN(const zfchar *, url))
{
    ZFPROTOCOL_ACCESS(ZFUIWebView)->webLoadUrl(this, url);
}
ZFMETHOD_DEFINE_2(ZFUIWebView, void, webLoadHtml,
                  ZFMP_IN(const zfchar *, html),
                  ZFMP_IN_OPT(const zfchar *, baseUrl, zfnull))
{
    ZFPROTOCOL_ACCESS(ZFUIWebView)->webLoadHtml(this, html, baseUrl);
}
ZFMETHOD_DEFINE_0(ZFUIWebView, void, webReload)
{
    ZFPROTOCOL_ACCESS(ZFUIWebView)->webReload(this);
}
ZFMETHOD_DEFINE_0(ZFUIWebView, void, webLoadStop)
{
    ZFPROTOCOL_ACCESS(ZFUIWebView)->webLoadStop(this);
}
ZFMETHOD_DEFINE_0(ZFUIWebView, void, webGoBack)
{
    ZFPROTOCOL_ACCESS(ZFUIWebView)->webGoBack(this);
}
ZFMETHOD_DEFINE_0(ZFUIWebView, void, webGoForward)
{
    ZFPROTOCOL_ACCESS(ZFUIWebView)->webGoForward(this);
}

ZFMETHOD_DEFINE_0(ZFUIWebView, zfbool, webLoading)
{
    return ZFPROTOCOL_ACCESS(ZFUIWebView)->webLoading(this);
}
ZFMETHOD_DEFINE_0(ZFUIWebView, zfbool, webGoBackAvailable)
{
    return ZFPROTOCOL_ACCESS(ZFUIWebView)->webGoBackAvailable(this);
}
ZFMETHOD_DEFINE_0(ZFUIWebView, zfbool, webGoForwardAvailable)
{
    return ZFPROTOCOL_ACCESS(ZFUIWebView)->webGoForwardAvailable(this);
}

void ZFUIWebView::objectOnInit(void)
{
    zfsuper::objectOnInit();

    zfCoreAssert(this->nativeImplView() == zfnull);
    zfclassNotPOD _ZFP_ZFUIWebView_nativeImplViewDestroy
    {
    public:
        static void action(ZF_IN ZFUIView *view,
                           ZF_IN void *nativeImplView)
        {
            ZFPROTOCOL_ACCESS(ZFUIWebView)->nativeWebViewDestroy(view->to<ZFUIWebView *>(), nativeImplView);
        }
    };
    this->nativeImplView(ZFPROTOCOL_ACCESS(ZFUIWebView)->nativeWebViewCreate(this),
                            _ZFP_ZFUIWebView_nativeImplViewDestroy::action);
}
void ZFUIWebView::objectOnDealloc(void)
{
    zfsuper::objectOnDealloc();
}
void ZFUIWebView::objectOnDeallocPrepare(void)
{
    this->webLoadStop();
    zfsuper::objectOnDeallocPrepare();
}

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIWebView_ignoreFrom_ZFUIViewBlinkWhenFoocus, ZFLevelZFFrameworkNormal)
{
    ZFUIViewBlinkWhenFocusFilter().classFilter.filterAdd(ZFUIWebView::ClassData());
}
ZF_GLOBAL_INITIALIZER_END(ZFUIWebView_ignoreFrom_ZFUIViewBlinkWhenFoocus)

ZF_NAMESPACE_GLOBAL_END

