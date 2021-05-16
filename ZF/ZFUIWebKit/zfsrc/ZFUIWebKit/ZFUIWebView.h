/**
 * @file ZFUIWebView.h
 * @brief web view
 */

#ifndef _ZFI_ZFUIWebView_h_
#define _ZFI_ZFUIWebView_h_

#include "ZFUIWebKitDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUIWebView
/**
 * @brief web view
 */
zfclass ZF_ENV_EXPORT ZFUIWebView : zfextends ZFUIView
{
    ZFOBJECT_DECLARE(ZFUIWebView, ZFUIView)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIWebView)

public:
    // ============================================================
    // event
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when load start or stop,
     * new state can be checked by #webLoading
     */
    ZFOBSERVER_EVENT(WebLoadStateOnChange)

public:
    // ============================================================
    // properties
    ZFPROPERTY_ON_INIT_INLINE(zfbool, viewFocusable)
    {
        propertyValue = zftrue;
    }

public:
    /**
     * @brief load from url
     */
    ZFMETHOD_DECLARE_1(void, webLoadUrl,
                       ZFMP_IN(const zfchar *, url))
    /**
     * @brief load from html string
     */
    ZFMETHOD_DECLARE_2(void, webLoadHtml,
                       ZFMP_IN(const zfchar *, html),
                       ZFMP_IN_OPT(const zfchar *, baseUrl, zfnull))
    /**
     * @brief reload current web data
     */
    ZFMETHOD_DECLARE_0(void, webReload)
    /**
     * @brief stop loading
     */
    ZFMETHOD_DECLARE_0(void, webLoadStop)

    /**
     * @brief go back, see #webGoBackAvailable
     */
    ZFMETHOD_DECLARE_0(void, webGoBack)
    /**
     * @brief go forward, see #webGoForwardAvailable
     */
    ZFMETHOD_DECLARE_0(void, webGoForward)

public:
    /**
     * @brief true if web is loading
     */
    ZFMETHOD_DECLARE_0(zfbool, webLoading)

    /**
     * @brief whether the web can go back, see #webGoBack
     */
    ZFMETHOD_DECLARE_0(zfbool, webGoBackAvailable)
    /**
     * @brief whether the web can go forward, see #webGoForward
     */
    ZFMETHOD_DECLARE_0(zfbool, webGoForwardAvailable)

public:
    zffinal void _ZFP_ZFUIWebView_notifyWebLoadStateChanged(void)
    {
        this->webLoadStateOnChange();
    }

protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);
    zfoverride
    virtual void objectOnDeallocPrepare(void);

protected:
    /** @brief see #EventWebLoadStateOnChange */
    virtual inline void webLoadStateOnChange(void)
    {
        this->observerNotify(ZFUIWebView::EventWebLoadStateOnChange());
    }
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIWebView_h_

