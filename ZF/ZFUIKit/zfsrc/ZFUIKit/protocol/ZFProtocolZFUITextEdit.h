/**
 * @file ZFProtocolZFUITextEdit.h
 * @brief protocol for #ZFUITextEdit
 */

#ifndef _ZFI_ZFProtocolZFUITextEdit_h_
#define _ZFI_ZFProtocolZFUITextEdit_h_

#include "ZFCore/ZFProtocol.h"
#include "ZFUIKit/ZFUITextEdit.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for #ZFUITextEdit
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFUITextEdit)
public:
    /**
     * @brief create native text edit view
     */
    virtual void *nativeTextEditCreate(ZF_IN ZFUITextEdit *textEdit) zfpurevirtual;
    /**
     * @brief destroy native text edit view
     */
    virtual void nativeTextEditDestroy(ZF_IN ZFUITextEdit *textEdit,
                                       ZF_IN void *nativeTextEdit) zfpurevirtual;

    // ============================================================
    // properties
public:
    /** @brief see #ZFUITextEdit */
    virtual void textEditEnable(ZF_IN ZFUITextEdit *textEdit,
                                ZF_IN zfbool textEditEnable) zfpurevirtual;
    /** @brief see #ZFUITextEdit */
    virtual void textEditSecure(ZF_IN ZFUITextEdit *textEdit,
                                ZF_IN zfbool textEditSecured) zfpurevirtual;
    /** @brief see #ZFUITextEdit */
    virtual void textEditKeyboardType(ZF_IN ZFUITextEdit *textEdit,
                                      ZF_IN ZFUITextEditKeyboardTypeEnum textEditKeyboardType) zfpurevirtual;
    /** @brief see #ZFUITextEdit */
    virtual void textEditKeyboardReturnType(ZF_IN ZFUITextEdit *textEdit,
                                            ZF_IN ZFUITextEditKeyboardReturnTypeEnum textEditKeyboardReturnType) zfpurevirtual;

    /** @brief see #ZFUITextEdit */
    virtual void textSelectRange(ZF_IN ZFUITextEdit *textEdit, ZF_OUT ZFIndexRange &textSelectRange) zfpurevirtual;
    /** @brief see #ZFUITextEdit */
    virtual void textSelectRange(ZF_IN ZFUITextEdit *textEdit, ZF_IN const ZFIndexRange &textSelectRange) zfpurevirtual;

public:
    /** @brief see #ZFUITextEdit */
    virtual void text(ZF_IN ZFUITextEdit *textEdit,
                      ZF_IN const zfchar *text) zfpurevirtual;
    /** @brief see #ZFUITextEdit */
    virtual void textAppearance(ZF_IN ZFUITextEdit *textEdit,
                                ZF_IN ZFUITextAppearanceEnum const &textAppearance) zfpurevirtual;
    /** @brief see #ZFUITextEdit */
    virtual void textAlign(ZF_IN ZFUITextEdit *textEdit,
                           ZF_IN ZFUIAlignFlags const &textAlign) zfpurevirtual;
    /** @brief see #ZFUITextEdit */
    virtual void textColor(ZF_IN ZFUITextEdit *textEdit,
                           ZF_IN ZFUIColor const &textColor) zfpurevirtual;
    /** @brief see #ZFUITextEdit */
    virtual void textShadowColor(ZF_IN ZFUITextEdit *textEdit,
                                 ZF_IN ZFUIColor const &textShadowColor) zfpurevirtual;
    /** @brief see #ZFUITextEdit */
    virtual void textShadowOffset(ZF_IN ZFUITextEdit *textEdit,
                                  ZF_IN ZFUISize const &textShadowOffset) zfpurevirtual;
    /** @brief see #ZFUITextEdit */
    virtual void textSize(ZF_IN ZFUITextEdit *textEdit,
                          ZF_IN zfint textSize) zfpurevirtual;

    // ============================================================
    // layout
public:
    /**
     * @brief measure text edit view using size hint
     *
     * implementation should truncate or wrap new line according to text style previously set by setTextStyle\n
     * sizeHint shows max size the text edit view may have, or 0 if no limit
     */
    virtual ZFUISize measureNativeTextEdit(ZF_IN ZFUITextEdit *textEdit,
                                           ZF_IN const ZFUISize &sizeHint,
                                           ZF_IN zfint textSize) zfpurevirtual;

    // ============================================================
    // edit
public:
    /** @brief see #ZFUITextEdit::textEditBegin */
    virtual void textEditBegin(ZF_IN ZFUITextEdit *textEdit) zfpurevirtual;
    /** @brief see #ZFUITextEdit::textEditEnd */
    virtual void textEditEnd(ZF_IN ZFUITextEdit *textEdit) zfpurevirtual;

    // ============================================================
    // callbacks that implementations must notify
public:
    /**
     * @brief implementation must notify to check whether text should change
     */
    zffinal zfbool notifyCheckTextShouldChange(ZF_IN ZFUITextEdit *textEdit,
                                               ZF_IN const zfchar *newValue)
    {
        return textEdit->textShouldChange(newValue);
    }
    /**
     * @brief implementation must notify when text changed
     */
    zffinal void notifyTextChange(ZF_IN ZFUITextEdit *textEdit,
                                  ZF_IN const zfchar *newValue)
    {
        textEdit->_ZFP_ZFUITextEdit_textNotifyChange(newValue);
    }
    /**
     * @brief implementation must notify when text select range changed
     */
    zffinal void notifyTextSelectRangeChange(ZF_IN ZFUITextEdit *textEdit)
    {
        textEdit->_ZFP_ZFUITextEdit_textSelectRangeNotifyChange();
    }
    /**
     * @brief implementation must notify when text changed
     */
    zffinal void notifyTextReturnClicked(ZF_IN ZFUITextEdit *textEdit)
    {
        textEdit->_ZFP_ZFUITextEdit_textNotifyReturnClicked();
    }
    /**
     * @brief implementation must notify when text begin edit
     */
    zffinal void notifyTextEditBegin(ZF_IN ZFUITextEdit *textEdit)
    {
        textEdit->_ZFP_ZFUITextEdit_textNotifyBeginEdit();
    }
    /**
     * @brief implementation must notify when text begin edit
     */
    zffinal void notifyTextEditEnd(ZF_IN ZFUITextEdit *textEdit)
    {
        textEdit->_ZFP_ZFUITextEdit_textNotifyEndEdit();
    }
ZFPROTOCOL_INTERFACE_END(ZFUITextEdit)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFUITextEdit_h_

