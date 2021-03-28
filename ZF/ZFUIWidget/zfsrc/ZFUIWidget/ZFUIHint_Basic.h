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
    ZFPROPERTY_OVERRIDE_ON_INIT_DECLARE(zfbool, viewUIEnableTree)
    ZFPROPERTY_OVERRIDE_ON_INIT_DECLARE(ZFUISize, viewSizeMin)
    ZFPROPERTY_OVERRIDE_ON_INIT_DECLARE(ZFUITextView *, buttonLabelStyleNormal)
    ZFPROPERTY_OVERRIDE_ON_INIT_DECLARE(ZFUIImageView *, buttonBackgroundStyleNormal)
};

/**
 * @brief make a hint with text
 *
 * the hint would:
 * -  #ZFUIHintContentBasic as the #ZFUIHint::hintContent
 * -  have an alpha change animation as the hint's show and hide animation
 */
ZFMETHOD_FUNC_DECLARE_2(zfautoObjectT<ZFUIHint *>, ZFUIHintMake,
                        ZFMP_IN(const zfchar *, text),
                        ZFMP_IN_OPT(ZFUIImage *, icon, zfnull))
/**
 * @brief #ZFUIHintMake and show the hint
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(zfautoObjectT<ZFUIHint *>, ZFUIHintShow,
                               ZFMP_IN(const zfchar *, text),
                               ZFMP_IN_OPT(ZFUIImage *, icon, zfnull))
{
    zfautoObjectT<ZFUIHint *> hint = ZFUIHintMake(text, icon);
    hint.to<ZFUIHint *>()->hintShow();
    return hint;
}

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIHint_Basic_h_

