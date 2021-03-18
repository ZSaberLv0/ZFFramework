/**
 * @file ZFUIHint_Basic.h
 * @brief common types of #ZFUIHint
 */

#ifndef _ZFI_ZFUIHint_Basic_h_
#define _ZFI_ZFUIHint_Basic_h_

#include "ZFUIHint.h"
#include "ZFUIButtonBasic.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// hint with simple text and icon
/**
 * @brief #ZFUIHint's content with simple text and background
 */
zfclass ZF_ENV_EXPORT ZFUIHintContentBasic : zfextends ZFUIButtonBasic
{
    ZFOBJECT_DECLARE(ZFUIHintContentBasic, ZFUIButtonBasic)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIHintContentBasic)

public:
    ZFPROPERTY_OVERRIDE_ON_INIT_INLINE(zfbool, viewUIEnableTree)
    {
        propertyValue = zffalse;
    }
    ZFPROPERTY_OVERRIDE_ON_INIT_INLINE(ZFUISize, viewSizeMin)
    {
        propertyValue = ZFUISizeMake(
            ZFUIGlobalStyle::DefaultStyle()->itemSizeButtonWidth(),
            ZFUIGlobalStyle::DefaultStyle()->itemSizeButton());
    }

    ZFPROPERTY_OVERRIDE_ON_INIT_INLINE(ZFUITextView *, buttonLabelStyleNormal)
    {
        ZFUITextView *value = propertyValue.to<ZFUITextView *>();
        value->textColor(ZFUIColorWhite());
        value->textSingleLine(zffalse);
        value->textSizeAutoChangeMinSize(0);
    }
    ZFPROPERTY_OVERRIDE_ON_INIT_INLINE(ZFUIImageView *, buttonBackgroundStyleNormal)
    {
        ZFUIImageView *value = propertyValue;
        value->image(zfRes("ZFUIWidget/ZFUIHintContentBasic_background.xml"));
    }
};

/**
 * @brief make a hint with text
 *
 * the hint would:
 * -  #ZFUIHintContentBasic as the #ZFUIHint::hintContent
 * -  have an alpha change animation as the hint's show and hide animation
 */
ZFMETHOD_FUNC_DECLARE_2(zfautoObject, ZFUIHintMake,
                        ZFMP_IN(const zfchar *, text),
                        ZFMP_IN_OPT(ZFUIImage *, icon, zfnull))
/**
 * @brief #ZFUIHintMake and show the hint
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(zfautoObject, ZFUIHintShow,
                               ZFMP_IN(const zfchar *, text),
                               ZFMP_IN_OPT(ZFUIImage *, icon, zfnull))
{
    zfautoObject hint = ZFUIHintMake(text, icon);
    hint.to<ZFUIHint *>()->hintShow();
    return hint;
}

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIHint_Basic_h_

