#include "ZFUIHint_Basic.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// hint with simple text and icon
ZFSTYLE_DEFAULT_DEFINE(ZFUIHintContentBasic)

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

