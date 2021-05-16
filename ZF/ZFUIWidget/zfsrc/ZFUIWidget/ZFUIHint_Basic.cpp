#include "ZFUIHint_Basic.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// hint with simple text and icon
ZFSTYLE_DEFAULT_DEFINE(ZFUIHintContentBasic)

ZFPROPERTY_ON_INIT_DEFINE(ZFUIHintContentBasic, zfbool, viewUIEnableTree)
{
    propertyValue = zffalse;
}
ZFPROPERTY_ON_INIT_DEFINE(ZFUIHintContentBasic, ZFUISize, viewSizeMin)
{
    propertyValue = ZFUISizeMake(
        ZFUIGlobalStyle::DefaultStyle()->itemSizeButtonWidth(),
        ZFUIGlobalStyle::DefaultStyle()->itemSizeButton());
}

ZFPROPERTY_ON_INIT_DEFINE(ZFUIHintContentBasic, ZFUITextView *, buttonLabelStyleNormal)
{
    ZFUITextView *value = propertyValue.to<ZFUITextView *>();
    value->textColor(ZFUIColorWhite());
    value->textSingleLine(zffalse);
    value->textSizeAutoChangeMinSize(0);
}
ZFPROPERTY_ON_INIT_DEFINE(ZFUIHintContentBasic, ZFUIImageView *, buttonBackgroundStyleNormal)
{
    ZFUIImageView *value = propertyValue;
    value->image(zfRes("ZFUIWidget/ZFUIHintContentBasic_background.xml"));
}

ZFMETHOD_FUNC_DEFINE_2(zfautoObjectT<ZFUIHint *>, ZFUIHintMake,
                       ZFMP_IN(const zfchar *, text),
                       ZFMP_IN_OPT(ZFUIImage *, icon, zfnull))
{
    zfblockedAlloc(ZFUIHint, hint);

    zfblockedAlloc(ZFAnimationNativeView, hintAniShow);
    hint->hintAniShow(hintAniShow);
    hintAniShow->aniAlphaFrom(0);
    zfblockedAlloc(ZFAnimationNativeView, hintAniHide);
    hint->hintAniHide(hintAniHide);
    hintAniHide->aniAlphaTo(0);

    zfblockedAlloc(ZFUIHintContentBasic, hintContent);
    hint->hintContent(hintContent);
    hintContent->buttonLabelText(text);
    hintContent->buttonIconImage(icon);

    return hint;
}
ZFMETHOD_FUNC_INLINE_DEFINE_2(zfautoObjectT<ZFUIHint *>, ZFUIHintShow,
                              ZFMP_IN(const zfchar *, text),
                              ZFMP_IN_OPT(ZFUIImage *, icon, zfnull))

ZF_NAMESPACE_GLOBAL_END

