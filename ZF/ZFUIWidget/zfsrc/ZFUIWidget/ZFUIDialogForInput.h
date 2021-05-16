/**
 * @file ZFUIDialogForInput.h
 * @brief basic dialog for user to input a short text
 */

#ifndef _ZFI_ZFUIDialogForInput_h_
#define _ZFI_ZFUIDialogForInput_h_

#include "ZFUIDialogBasic.h"
#include "ZFUITextEditWidget.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUIDialogForInput
/**
 * @brief basic dialog for input
 */
zfclass ZF_ENV_EXPORT ZFUIDialogForInput : zfextends ZFUIDialogBasic
{
    ZFOBJECT_DECLARE(ZFUIDialogForInput, ZFUIDialogBasic)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIDialogForInput)

public:
    /**
     * @brief the input text edit view
     */
    ZFPROPERTY_RETAIN_READONLY(ZFUITextEditWidget *, inputView, ZFPropertyNoInitValue)
    ZFPROPERTY_ON_INIT_DECLARE(ZFUITextEditWidget *, inputView)

    /**
     * @brief util method to set input hint text
     */
    ZFMETHOD_DECLARE_1(void, inputHintText,
                       ZFMP_IN(const zfchar *, text))
    /**
     * @brief util method to get input hint text
     */
    ZFMETHOD_DECLARE_0(const zfchar *, inputHintText)

    /**
     * @brief util method to set input text
     */
    ZFMETHOD_DECLARE_1(void, inputText,
                       ZFMP_IN(const zfchar *, text))
    /**
     * @brief util method to get input text
     */
    ZFMETHOD_DECLARE_0(const zfchar *, inputText)

protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);

    zfoverride
    virtual ZFUIView *dialogFocusOnUpdate(void)
    {
        return this->inputView();
    }
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIDialogForInput_h_

