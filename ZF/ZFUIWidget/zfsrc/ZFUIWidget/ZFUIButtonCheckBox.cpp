#include "ZFUIButtonCheckBox.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFUIButtonCheckBox)
ZFSTYLE_DEFAULT_DEFINE(ZFUIButtonCheckBox)

ZFPROPERTY_OVERRIDE_ON_INIT_DEFINE(ZFUIButtonCheckBox, zfbool, buttonCheckable)
{
    propertyValue = zftrue;
}

ZFPROPERTY_OVERRIDE_ON_INIT_DEFINE(ZFUIButtonCheckBox, ZFUIAlignFlags, buttonContentAlign)
{
    propertyValue = ZFUIAlign::e_LeftInner;
}

ZFPROPERTY_OVERRIDE_ON_INIT_DEFINE(ZFUIButtonCheckBox, ZFUIImageView *, buttonIconStyleNormal)
{
    propertyValue.to<ZFUIImageView *>()->image(zfRes("ZFUIWidget/ZFUIButtonCheckBox_IconNormal.png"));
}
ZFPROPERTY_OVERRIDE_ON_INIT_DEFINE(ZFUIButtonCheckBox, ZFUIImageView *, buttonIconStyleHighlighted)
{
    propertyValue.to<ZFUIImageView *>()->image(zfRes("ZFUIWidget/ZFUIButtonCheckBox_IconHighlighted.png"));
}
ZFPROPERTY_OVERRIDE_ON_INIT_DEFINE(ZFUIButtonCheckBox, ZFUIImageView *, buttonIconStyleChecked)
{
    propertyValue.to<ZFUIImageView *>()->image(zfRes("ZFUIWidget/ZFUIButtonCheckBox_IconChecked.png"));
}
ZFPROPERTY_OVERRIDE_ON_INIT_DEFINE(ZFUIButtonCheckBox, ZFUIImageView *, buttonIconStyleCheckedHighlighted)
{
    propertyValue.to<ZFUIImageView *>()->image(zfRes("ZFUIWidget/ZFUIButtonCheckBox_IconCheckedHighlighted.png"));
}

ZF_NAMESPACE_GLOBAL_END

