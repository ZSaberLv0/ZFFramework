/**
 * @file ZFUIScrollThumb.h
 * @brief abstract scroll thumb for #ZFUIScrollView
 */

#ifndef _ZFI_ZFUIScrollThumb_h_
#define _ZFI_ZFUIScrollThumb_h_

#include "ZFUIScrollView.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUIScrollThumb
/**
 * @brief abstract scroll thumb for #ZFUIScrollView
 */
zfinterface ZF_ENV_EXPORT ZFUIScrollThumb : zfextends ZFInterface
{
    ZFINTERFACE_DECLARE(ZFUIScrollThumb, ZFInterface)

public:
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when thumb updated
     */
    ZFOBSERVER_EVENT(ScrollThumbOnUpdate)

public:
    /**
     * @brief util method to calculate scroll thumb's position according to view's position
     *
     * 0 result size means no need to show the scroll thumb
     */
    static void scrollThumbPosFromViewPos(ZF_OUT zfint &scrollThumbResultPos,
                                          ZF_OUT zfint &scrollThumbResultSize,
                                          ZF_IN zfint scrollThumbRange,
                                          ZF_IN zfint scrollViewSize,
                                          ZF_IN zfint scrollContentOffset,
                                          ZF_IN zfint scrollContentSize,
                                          ZF_IN zfint scrollThumbMinSize,
                                          ZF_IN_OPT zfint headMargin = 0,
                                          ZF_IN_OPT zfint tailMargin = 0);
    /**
     * @brief util method to calculate view's position according to scroll thumb's position
     */
    static void scrollThumbPosToViewPos(ZF_OUT zfint &scrollContentOffset,
                                        ZF_IN zfint scrollViewSize,
                                        ZF_IN zfint scrollContentSize,
                                        ZF_IN zfint scrollThumbPos,
                                        ZF_IN zfint scrollThumbSize,
                                        ZF_IN zfint scrollThumbRange,
                                        ZF_IN_OPT zfint headMargin = 0,
                                        ZF_IN_OPT zfint tailMargin = 0);

public:
    /**
     * @brief owner scroll view
     */
    zffinal inline ZFUIScrollView *scrollView(void)
    {
        return this->_scrollView;
    }
    /**
     * @brief this thumb is horizontal or not
     */
    zffinal inline zfbool scrollThumbHorizontal(void)
    {
        return this->_horizontal;
    }

public:
    /**
     * @brief init the scroll thumb
     */
    virtual void scrollThumbInit(void) zfpurevirtual;
    /**
     * @brief destroy the scroll thumb
     */
    virtual void scrollThumbDealloc(void) zfpurevirtual;

    /**
     * @brief update the scroll thumb, this method may be called repeatly
     *
     * you may update thumb depending on #ZFUIScrollView::scrollViewState
     */
    virtual void scrollThumbUpdate(void) zfpurevirtual;

private:
    ZFUIScrollView *_scrollView;
    zfbool _horizontal;
    friend zfclassFwd ZFUIScrollView;
};

// ============================================================
/**
 * @brief change default scroll thumb class
 *
 * set to null to use default
 */
ZFMETHOD_FUNC_DECLARE_1(void, ZFUIScrollThumbHorizontalClass,
                        ZFMP_IN(const ZFClass *, cls))
/**
 * @brief get the class of default scroll thumb
 */
ZFMETHOD_FUNC_DECLARE_0(const ZFClass *, ZFUIScrollThumbHorizontalClass)
/**
 * @brief change default scroll thumb class
 *
 * set to null to use default
 */
ZFMETHOD_FUNC_DECLARE_1(void, ZFUIScrollThumbVerticalClass,
                        ZFMP_IN(const ZFClass *, cls))
/**
 * @brief get the class of default scroll thumb
 */
ZFMETHOD_FUNC_DECLARE_0(const ZFClass *, ZFUIScrollThumbVerticalClass)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIScrollThumb_h_

