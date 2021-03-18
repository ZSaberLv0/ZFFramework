/**
 * @file ZFUIDialogBasic.h
 * @brief basic dialog
 */

#ifndef _ZFI_ZFUIDialogBasic_h_
#define _ZFI_ZFUIDialogBasic_h_

#include "ZFUIDialog.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUIDialogButtonType
/**
 * @brief button state
 */
ZFENUM_BEGIN(ZFUIDialogButtonType)
    ZFENUM_VALUE(Normal) /**< @brief normal button */
    ZFENUM_VALUE(Yes) /**< @brief yes button */
    ZFENUM_VALUE(No) /**< @brief no button */
    ZFENUM_VALUE(Cancel) /**< @brief cancel button */
    ZFENUM_VALUE(Destructive) /**< @brief for destructive operations */
ZFENUM_SEPARATOR(ZFUIDialogButtonType)
    ZFENUM_VALUE_REGISTER(Normal)
    ZFENUM_VALUE_REGISTER(Yes)
    ZFENUM_VALUE_REGISTER(No)
    ZFENUM_VALUE_REGISTER(Cancel)
    ZFENUM_VALUE_REGISTER(Destructive)
ZFENUM_END(ZFUIDialogButtonType)

// ============================================================
/**
 * @brief abstract dialog content for #ZFUIDialogBasic
 */
zfinterface ZF_ENV_EXPORT ZFUIDialogContent : zfextends ZFInterface
{
    ZFINTERFACE_DECLARE(ZFUIDialogContent, ZFInterface)

public:
    // ============================================================
    // events
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when dialog button added,
     * param0 is the dialog button
     */
    ZFOBSERVER_EVENT(DialogButtonOnAdd)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when dialog button removed,
     * param0 is the dialog button
     */
    ZFOBSERVER_EVENT(DialogButtonOnRemove)

public:
    // ============================================================
    // title
    /**
     * @brief container to hold custom title views
     */
    virtual ZFUIView *dialogTitleContainer(void) zfpurevirtual;
    /**
     * @brief title text view
     */
    virtual ZFUITextView *dialogTitleView(void) zfpurevirtual;
    /**
     * @brief set the dialog's title text
     */
    ZFMETHOD_INLINE_1(void, dialogTitleText,
                      ZFMP_IN(const zfchar *, text))
    {
        this->dialogTitleView()->text(text);
    }
    /**
     * @brief get the dialog's title text
     */
    ZFMETHOD_INLINE_0(const zfchar *, dialogTitleText)
    {
        return this->dialogTitleView()->text();
    }

    // ============================================================
    // content
    /**
     * @brief container to hold custom content views
     */
    virtual ZFUIView *dialogContentContainer(void) zfpurevirtual;
    /**
     * @brief content text view
     */
    virtual ZFUITextView *dialogContentView(void) zfpurevirtual;
    /**
     * @brief set the dialog's content text
     */
    ZFMETHOD_INLINE_1(void, dialogContentText,
                      ZFMP_IN(const zfchar *, text))
    {
        this->dialogContentView()->text(text);
    }
    /**
     * @brief get the dialog's content text
     */
    ZFMETHOD_INLINE_0(const zfchar *, dialogContentText)
    {
        return this->dialogContentView()->text();
    }

    // ============================================================
    // button
    /**
     * @brief container to hold custom button views
     */
    virtual ZFUIView *dialogButtonContainer(void) zfpurevirtual;
    /**
     * @brief access dialog button with specifed type
     *
     * if autoCreateIfNotExist is not true,
     * button won't be created automatically,
     * and may return null if not exist,
     * see #dialogButtonRemove\n
     * access #ZFUIDialogButtonType::e_Normal would always return null
     */
    virtual ZFUIButton *dialogButton(ZF_IN ZFUIDialogButtonTypeEnum dialogButtonType,
                                     ZF_IN_OPT zfbool autoCreateIfNotExist = zftrue) zfpurevirtual;
    /**
     * @brief text of the button
     */
    virtual const zfchar *dialogButtonText(ZF_IN ZFUIDialogButtonTypeEnum dialogButtonType) zfpurevirtual;
    /**
     * @brief see #dialogButtonText
     */
    virtual void dialogButtonText(ZF_IN ZFUIDialogButtonTypeEnum dialogButtonType,
                                  ZF_IN const zfchar *text) zfpurevirtual;
    /**
     * @brief remove specified button type, see #dialogButton
     */
    virtual void dialogButtonRemove(ZF_IN ZFUIDialogButtonTypeEnum dialogButtonType) zfpurevirtual;

    /** @brief util method to access #dialogButton */
    ZFMETHOD_INLINE_1(ZFUIButton *, dialogButtonYes,
                      ZFMP_IN_OPT(zfbool, autoCreateIfNotExist, zftrue))
    {
        return this->dialogButton(ZFUIDialogButtonType::e_Yes, autoCreateIfNotExist);
    }
    /** @brief util method to access #dialogButton */
    ZFMETHOD_INLINE_1(ZFUIButton *, dialogButtonNo,
                      ZFMP_IN_OPT(zfbool, autoCreateIfNotExist, zftrue))
    {
        return this->dialogButton(ZFUIDialogButtonType::e_No, autoCreateIfNotExist);
    }
    /** @brief util method to access #dialogButton */
    ZFMETHOD_INLINE_1(ZFUIButton *, dialogButtonCancel,
                      ZFMP_IN_OPT(zfbool, autoCreateIfNotExist, zftrue))
    {
        return this->dialogButton(ZFUIDialogButtonType::e_Cancel, autoCreateIfNotExist);
    }
    /** @brief util method to access #dialogButton */
    ZFMETHOD_INLINE_1(ZFUIButton *, dialogButtonDestructive,
                      ZFMP_IN_OPT(zfbool, autoCreateIfNotExist, zftrue))
    {
        return this->dialogButton(ZFUIDialogButtonType::e_Destructive, autoCreateIfNotExist);
    }

    /** @brief util method to access #dialogButtonText */
    ZFMETHOD_INLINE_0(const zfchar *, dialogButtonYesText)
    {
        return this->dialogButtonText(ZFUIDialogButtonType::e_Yes);
    }
    /** @brief util method to access #dialogButtonText */
    ZFMETHOD_INLINE_1(void, dialogButtonYesText,
                      ZFMP_IN(const zfchar *, text))
    {
        this->dialogButtonText(ZFUIDialogButtonType::e_Yes, text);
    }
    /** @brief util method to access #dialogButtonText */
    ZFMETHOD_INLINE_0(const zfchar *, dialogButtonNoText)
    {
        return this->dialogButtonText(ZFUIDialogButtonType::e_No);
    }
    /** @brief util method to access #dialogButtonText */
    ZFMETHOD_INLINE_1(void, dialogButtonNoText,
                      ZFMP_IN(const zfchar *, text))
    {
        this->dialogButtonText(ZFUIDialogButtonType::e_No, text);
    }
    /** @brief util method to access #dialogButtonText */
    ZFMETHOD_INLINE_0(const zfchar *, dialogButtonCancelText)
    {
        return this->dialogButtonText(ZFUIDialogButtonType::e_Cancel);
    }
    /** @brief util method to access #dialogButtonText */
    ZFMETHOD_INLINE_1(void, dialogButtonCancelText,
                      ZFMP_IN(const zfchar *, text))
    {
        this->dialogButtonText(ZFUIDialogButtonType::e_Cancel, text);
    }
    /** @brief util method to access #dialogButtonText */
    ZFMETHOD_INLINE_0(const zfchar *, dialogButtonDestructiveText)
    {
        return this->dialogButtonText(ZFUIDialogButtonType::e_Destructive);
    }
    /** @brief util method to access #dialogButtonText */
    ZFMETHOD_INLINE_1(void, dialogButtonDestructiveText,
                      ZFMP_IN(const zfchar *, text))
    {
        this->dialogButtonText(ZFUIDialogButtonType::e_Destructive, text);
    }

    // ============================================================
    // button
    /**
     * @brief button count with #ZFUIDialogButtonType::e_Normal type
     */
    virtual zfindex dialogButtonCount(void) zfpurevirtual;
    /**
     * @brief access button at index, for #ZFUIDialogButtonType::e_Normal type only
     */
    virtual ZFUIButton *dialogButtonAtIndex(ZF_IN zfindex index) zfpurevirtual;
    /**
     * @brief find button's index, for #ZFUIDialogButtonType::e_Normal type only
     */
    virtual zfindex dialogButtonFind(ZF_IN ZFUIButton *dialogButton) zfpurevirtual;
    /**
     * @brief manually add a button with #ZFUIDialogButtonType::e_Normal type
     */
    virtual void dialogButtonAdd(ZF_IN ZFUIButton *button,
                                 ZF_IN_OPT zfindex atIndex = zfindexMax()) zfpurevirtual;
    /**
     * @brief manually remove a specified button, which can be #dialogButton
     */
    virtual void dialogButtonRemove(ZF_IN ZFUIButton *button) zfpurevirtual;
    /**
     * @brief manually remove a specified button at index
     */
    virtual void dialogButtonRemoveAtIndex(ZF_IN zfindex index) zfpurevirtual;
    /**
     * @brief manually remove all button
     */
    virtual void dialogButtonRemoveAll(void) zfpurevirtual;

protected:
    /** @brief see #EventDialogButtonOnAdd */
    virtual inline void dialogButtonOnAdd(ZF_IN ZFUIButton *button)
    {
        this->toObject()->observerNotify(ZFUIDialogContent::EventDialogButtonOnAdd(), button);
    }
    /** @brief see #EventDialogButtonOnRemove */
    virtual inline void dialogButtonOnRemove(ZF_IN ZFUIButton *button)
    {
        this->toObject()->observerNotify(ZFUIDialogContent::EventDialogButtonOnRemove(), button);
    }
};

// ============================================================
/**
 * @brief set the default class of content view for #ZFUIDialogBasic
 *
 * #ZFUIDialogContentBasic by default,
 * set null to reset to default
 */
ZFMETHOD_FUNC_DECLARE_1(void, ZFUIDialogContentClass,
                        ZFMP_IN(const ZFClass *, cls))
/**
 * @brief see #ZFUIDialogContentClass
 */
ZFMETHOD_FUNC_DECLARE_0(const ZFClass *, ZFUIDialogContentClass)

// ============================================================
// ZFUIDialogBasic
/**
 * @brief basic dialog with title, content and buttons
 *
 * actual dialog content is implemented by #ZFUIDialogContent,
 * you may change the default impl by #ZFUIDialogContentClass,
 * or directly change #ZFUIDialogBasic::dialogContent
 */
zfclass ZF_ENV_EXPORT ZFUIDialogBasic : zfextends ZFUIDialog, zfimplements ZFUIDialogContent
{
    ZFOBJECT_DECLARE(ZFUIDialogBasic, ZFUIDialog)
    ZFIMPLEMENTS_DECLARE(ZFUIDialogContent)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIDialogBasic)

public:
    /**
     * @brief dialog content, create instance from #ZFUIDialogContentClass by default
     */
    ZFPROPERTY_RETAIN_WITH_INIT(ZFUIDialogContent *, dialogContent,
                                ZFUIDialogContentClass()->newInstance().to<ZFUIDialogContent *>())
    ZFPROPERTY_OVERRIDE_ON_VERIFY_DECLARE(ZFUIDialogContent *, dialogContent)
    ZFPROPERTY_OVERRIDE_ON_ATTACH_DECLARE(ZFUIDialogContent *, dialogContent)
    ZFPROPERTY_OVERRIDE_ON_DETACH_DECLARE(ZFUIDialogContent *, dialogContent)

public:
    // ============================================================
    // title
    ZFMETHOD_INLINE_0(ZFUIView *, dialogTitleContainer)
    {
        return this->dialogContent()->dialogTitleContainer();
    }
    ZFMETHOD_INLINE_0(ZFUITextView *, dialogTitleView)
    {
        return this->dialogContent()->dialogTitleView();
    }

    // ============================================================
    // content
    ZFMETHOD_INLINE_0(ZFUIView *, dialogContentContainer)
    {
        return this->dialogContent()->dialogContentContainer();
    }
    ZFMETHOD_INLINE_0(ZFUITextView *, dialogContentView)
    {
        return this->dialogContent()->dialogContentView();
    }

    // ============================================================
    // button
    ZFMETHOD_INLINE_0(ZFUIView *, dialogButtonContainer)
    {
        return this->dialogContent()->dialogButtonContainer();
    }
    ZFMETHOD_INLINE_2(ZFUIButton *, dialogButton,
                      ZFMP_IN(ZFUIDialogButtonTypeEnum, dialogButtonType),
                      ZFMP_IN_OPT(zfbool, autoCreateIfNotExist, zftrue))
    {
        return this->dialogContent()->dialogButton(dialogButtonType, autoCreateIfNotExist);
    }
    ZFMETHOD_INLINE_1(const zfchar *, dialogButtonText,
                      ZFMP_IN(ZFUIDialogButtonTypeEnum, dialogButtonType))
    {
        return this->dialogContent()->dialogButtonText(dialogButtonType);
    }
    ZFMETHOD_INLINE_2(void, dialogButtonText,
                      ZFMP_IN(ZFUIDialogButtonTypeEnum, dialogButtonType),
                      ZFMP_IN(const zfchar *, text))
    {
        this->dialogContent()->dialogButtonText(dialogButtonType, text);
    }
    ZFMETHOD_INLINE_1(void, dialogButtonRemove,
                      ZFMP_IN(ZFUIDialogButtonTypeEnum , dialogButtonType))
    {
        this->dialogContent()->dialogButtonRemove(dialogButtonType);
    }

    // ============================================================
    // button
    ZFMETHOD_INLINE_0(zfindex, dialogButtonCount)
    {
        return this->dialogContent()->dialogButtonCount();
    }
    ZFMETHOD_INLINE_1(ZFUIButton *, dialogButtonAtIndex,
                      ZFMP_IN(zfindex, index))
    {
        return this->dialogContent()->dialogButtonAtIndex(index);
    }
    ZFMETHOD_INLINE_1(zfindex, dialogButtonFind,
                      ZFMP_IN(ZFUIButton *, dialogButton))
    {
        return this->dialogContent()->dialogButtonFind(dialogButton);
    }
    ZFMETHOD_INLINE_2(void, dialogButtonAdd,
                      ZFMP_IN(ZFUIButton *, button),
                      ZFMP_IN_OPT(zfindex, atIndex, zfindexMax()))
    {
        this->dialogContent()->dialogButtonAdd(button, atIndex);
    }
    ZFMETHOD_INLINE_1(void, dialogButtonRemove,
                      ZFMP_IN(ZFUIButton *, button))
    {
        this->dialogContent()->dialogButtonRemove(button);
    }
    ZFMETHOD_INLINE_1(void, dialogButtonRemoveAtIndex,
                      ZFMP_IN(zfindex, index))
    {
        this->dialogContent()->dialogButtonRemoveAtIndex(index);
    }
    ZFMETHOD_INLINE_0(void, dialogButtonRemoveAll)
    {
        this->dialogContent()->dialogButtonRemoveAll();
    }

protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDeallocPrepare(void);
    zfoverride
    virtual void objectOnDealloc(void);

public:
    zffinal void _ZFP_ZFUIDialogBasic_dialogButtonOnAdd(ZF_IN ZFUIButton *button)
    {
        this->dialogButtonOnAdd(button);
    }
    zffinal void _ZFP_ZFUIDialogBasic_dialogButtonOnRemove(ZF_IN ZFUIButton *button)
    {
        this->dialogButtonOnRemove(button);
    }
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIDialogBasic_h_

#include "ZFUIDialogContentBasic.h"

