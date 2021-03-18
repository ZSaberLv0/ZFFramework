/**
 * @file ZFProtocolZFUIViewFocus.h
 * @brief protocol for #ZFUIView
 */

#ifndef _ZFI_ZFProtocolZFUIViewFocus_h_
#define _ZFI_ZFProtocolZFUIViewFocus_h_

#include "ZFCore/ZFProtocol.h"
#include "ZFUIKit/ZFUIView.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for ZFUIView
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFUIViewFocus)
ZFPROTOCOL_INTERFACE_OPTIONAL
public:
    /**
     * @brief change view's focusable setting
     */
    virtual void viewFocusable(ZF_IN ZFUIView *view,
                               ZF_IN zfbool viewFocusable) zfpurevirtual;
    /**
     * @brief see #ZFUIView::viewFocused
     */
    virtual zfbool viewFocused(ZF_IN ZFUIView *view) zfpurevirtual;
    /**
     * @brief see #ZFUIView::viewFocusFind
     */
    virtual ZFUIView *viewFocusFind(ZF_IN ZFUIView *view)
    {
        if(view->viewFocused())
        {
            return view;
        }
        ZFUIView *ret = zfnull;
        ZFCoreArrayPOD<ZFUIView *> children = view->childArray();
        for(zfindex i = 0; i < children.count(); ++i)
        {
            ret = this->viewFocusFind(children[i]);
            if(ret != zfnull)
            {
                return ret;
            }
        }
        children = view->internalFgViewArray();
        for(zfindex i = 0; i < children.count(); ++i)
        {
            ret = this->viewFocusFind(children[i]);
            if(ret != zfnull)
            {
                return ret;
            }
        }
        children = view->internalBgViewArray();
        for(zfindex i = 0; i < children.count(); ++i)
        {
            ret = this->viewFocusFind(children[i]);
            if(ret != zfnull)
            {
                return ret;
            }
        }
        children = view->internalImplViewArray();
        for(zfindex i = 0; i < children.count(); ++i)
        {
            ret = this->viewFocusFind(children[i]);
            if(ret != zfnull)
            {
                return ret;
            }
        }
        return zfnull;
    }
    /**
     * @brief see #ZFUIView::viewFocusRequest
     */
    virtual void viewFocusRequest(ZF_IN ZFUIView *view,
                                  ZF_IN zfbool viewFocus) zfpurevirtual;

    // ============================================================
    // callbacks that implementations must notify
public:
    /**
     * @brief implementations must notify when view's focus state changed (both obtain or resign)
     */
    zffinal void notifyViewFocusChanged(ZF_IN ZFUIView *view)
    {
        view->_ZFP_ZFUIView_viewFocusOnChange();
    }
ZFPROTOCOL_INTERFACE_END(ZFUIViewFocus)

/**
 * @brief convenient method to call notifyViewFocusChanged of optional protocol ZFUIViewFocus
 */
inline void ZFProtocolZFUIViewFocus_notifyViewFocusChanged(ZF_IN ZFUIView *view)
{
    ZFPROTOCOL_INTERFACE_CLASS(ZFUIViewFocus) *impl = ZFPROTOCOL_TRY_ACCESS(ZFUIViewFocus);
    if(impl != zfnull)
    {
        impl->notifyViewFocusChanged(view);
    }
}

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFUIViewFocus_h_

