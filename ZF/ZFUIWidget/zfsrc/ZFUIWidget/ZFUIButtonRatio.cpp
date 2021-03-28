#include "ZFUIButtonRatio.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFUIButtonRatio)
ZFSTYLE_DEFAULT_DEFINE(ZFUIButtonRatio)

ZFPROPERTY_OVERRIDE_ON_INIT_DEFINE(ZFUIButtonRatio, zfbool, buttonCheckable)
{
    propertyValue = zftrue;
}

ZFPROPERTY_OVERRIDE_ON_INIT_DEFINE(ZFUIButtonRatio, ZFUIAlignFlags, buttonContentAlign)
{
    propertyValue = ZFUIAlign::e_LeftInner;
}

ZFPROPERTY_OVERRIDE_ON_INIT_DEFINE(ZFUIButtonRatio, ZFUIImageView *, buttonIconStyleNormal)
{
    propertyValue.to<ZFUIImageView *>()->image(zfRes("ZFUIWidget/ZFUIButtonRatio_IconNormal.png"));
}
ZFPROPERTY_OVERRIDE_ON_INIT_DEFINE(ZFUIButtonRatio, ZFUIImageView *, buttonIconStyleHighlighted)
{
    propertyValue.to<ZFUIImageView *>()->image(zfRes("ZFUIWidget/ZFUIButtonRatio_IconHighlighted.png"));
}
ZFPROPERTY_OVERRIDE_ON_INIT_DEFINE(ZFUIButtonRatio, ZFUIImageView *, buttonIconStyleChecked)
{
    propertyValue.to<ZFUIImageView *>()->image(zfRes("ZFUIWidget/ZFUIButtonRatio_IconChecked.png"));
}
ZFPROPERTY_OVERRIDE_ON_INIT_DEFINE(ZFUIButtonRatio, ZFUIImageView *, buttonIconStyleCheckedHighlighted)
{
    propertyValue.to<ZFUIImageView *>()->image(zfRes("ZFUIWidget/ZFUIButtonRatio_IconCheckedHighlighted.png"));
}

ZF_NAMESPACE_GLOBAL_END

