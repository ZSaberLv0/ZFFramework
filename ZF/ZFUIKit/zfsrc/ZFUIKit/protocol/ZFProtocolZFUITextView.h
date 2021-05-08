/**
 * @file ZFProtocolZFUITextView.h
 * @brief protocol for #ZFUITextView
 */

#ifndef _ZFI_ZFProtocolZFUITextView_h_
#define _ZFI_ZFProtocolZFUITextView_h_

#include "ZFCore/ZFProtocol.h"
#include "ZFUIKit/ZFUITextView.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for #ZFUITextView
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFUITextView)
public:
    /**
     * @brief create native text view
     */
    virtual void *nativeTextViewCreate(ZF_IN ZFUITextView *textView) zfpurevirtual;
    /**
     * @brief destroy native text view
     */
    virtual void nativeTextViewDestroy(ZF_IN ZFUITextView *textView,
                                       ZF_IN void *nativeTextView) zfpurevirtual;

    // ============================================================
    // properties
public:
    /** @brief see #ZFUITextView */
    virtual void text(ZF_IN ZFUITextView *textView,
                      ZF_IN const zfchar *text) zfpurevirtual;
    /** @brief see #ZFUITextView */
    virtual void textAppearance(ZF_IN ZFUITextView *textView,
                                ZF_IN ZFUITextAppearanceEnum const &textAppearance) zfpurevirtual;
    /** @brief see #ZFUITextView */
    virtual void textAlign(ZF_IN ZFUITextView *textView,
                           ZF_IN ZFUIAlignFlags const &textAlign) zfpurevirtual;
    /** @brief see #ZFUITextView */
    virtual void textColor(ZF_IN ZFUITextView *textView,
                           ZF_IN ZFUIColor const &textColor) zfpurevirtual;
    /** @brief see #ZFUITextView */
    virtual void textShadowColor(ZF_IN ZFUITextView *textView,
                                 ZF_IN ZFUIColor const &textShadowColor) zfpurevirtual;
    /** @brief see #ZFUITextView */
    virtual void textShadowOffset(ZF_IN ZFUITextView *textView,
                                  ZF_IN ZFUISize const &textShadowOffset) zfpurevirtual;
    /** @brief see #ZFUITextView */
    virtual void textSize(ZF_IN ZFUITextView *textView,
                          ZF_IN zffloat textSize) zfpurevirtual;
    /** @brief see #ZFUITextView */
    virtual void textSizeAutoChangeMinSize(ZF_IN ZFUITextView *textView,
                                           ZF_IN zffloat textSizeAutoChangeMinSize) zfpurevirtual;
    /** @brief see #ZFUITextView */
    virtual void textSizeAutoChangeMaxSize(ZF_IN ZFUITextView *textView,
                                           ZF_IN zffloat textSizeAutoChangeMaxSize) zfpurevirtual;
    /** @brief see #ZFUITextView */
    virtual void textSingleLine(ZF_IN ZFUITextView *textView,
                                ZF_IN zfbool textSingleLine) zfpurevirtual;
    /** @brief see #ZFUITextView */
    virtual void textTruncateMode(ZF_IN ZFUITextView *textView,
                                  ZF_IN ZFUITextTruncateModeEnum const &textTruncateMode) zfpurevirtual;

    // ============================================================
    // layout
public:
    /**
     * @brief measure text view using size hint
     *
     * implementation should truncate or wrap new line according to text style previously set by setTextStyle\n
     * sizeHint shows max size the text view may have, or 0 if no limit
     */
    virtual ZFUISize measureNativeTextView(ZF_IN ZFUITextView *textView,
                                           ZF_IN const ZFUISize &sizeHint,
                                           ZF_IN zffloat textSize) zfpurevirtual;

    /**
     * @brief see #ZFUITextView::textSizeCurrent
     */
    virtual zffloat textSizeCurrent(ZF_IN ZFUITextView *textView) zfpurevirtual;

    /**
     * @brief layout text view after ZFUIView's layout step,
     *   so that the view may shrink text size if necessary
     */
    virtual void layoutNativeTextView(ZF_IN ZFUITextView *textView,
                                      ZF_IN const ZFUISize &viewSize) zfpurevirtual;

    // ============================================================
    // util method
public:
    /**
     * @brief util method to calculate text size according auto change setting for this text view
     *
     * usually used by implementation that doesn't support text auto resizing,
     * loop to measure and calculate proper text size,
     * may have performance issues
     */
    zffloat calcTextSizeAutoChange(ZF_IN ZFUITextView *textView,
                                   ZF_IN const ZFUISize &sizeHint)
    {
        zffloat curTextSize = textView->textSize();
        ZFUISize curSize = this->measureNativeTextView(textView, sizeHint, ZFUISizeApplyScale(curTextSize, textView->scaleFixed()));

        if(textView->textSizeAutoChangeMaxSize() > 0)
        {
            while(curTextSize < textView->textSizeAutoChangeMaxSize()
                && ((sizeHint.width <= 0 || curSize.width < sizeHint.width)
                    && (sizeHint.height <= 0 || curSize.height < sizeHint.height)))
            {
                ++curTextSize;
                curSize = this->measureNativeTextView(textView, sizeHint, ZFUISizeApplyScale(curTextSize, textView->scaleFixed()));
            }
        }

        if(textView->textSizeAutoChangeMinSize() > 0)
        {
            while(curTextSize > textView->textSizeAutoChangeMinSize()
                && ((sizeHint.width > 0 && curSize.width > sizeHint.width)
                    || (sizeHint.height > 0 && curSize.height > sizeHint.height)))
            {
                --curTextSize;
                curSize = this->measureNativeTextView(textView, sizeHint, ZFUISizeApplyScale(curTextSize, textView->scaleFixed()));
            }
        }

        return ZFUISizeApplyScale(curTextSize, textView->scaleFixed());
    }
ZFPROTOCOL_INTERFACE_END(ZFUITextView)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFUITextView_h_

