#include "ZFUIListCellViewBasic.h"
#include "ZFUIViewLayout.h"
#include "ZFUILinearLayout.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassNotPOD _ZFP_ZFUIListCellViewBasicPrivate
{
public:
    ZFUIViewLayout *cellIconContainer;
    ZFUILinearLayout *cellCenterContainer;
    ZFUIViewLayout *cellLabelMainContainer;
    ZFUIViewLayout *cellLabelSubContainer;
    ZFUIViewLayout *cellAccessoryContainer;
};

// ============================================================
ZFOBJECT_REGISTER(ZFUIListCellViewBasic)
ZFSTYLE_DEFAULT_DEFINE(ZFUIListCellViewBasic)

ZFPROPERTY_ON_INIT_DEFINE(ZFUIListCellViewBasic, ZFUIImageView *, cellIcon)
{
    zfblockedAlloc(ZFUIImageView, cellIcon);
    propertyValue = cellIcon;
    cellIcon->viewSizeMax(ZFUISizeMake(ZFUIGlobalStyle::DefaultStyle()->itemSizeIcon()));
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIListCellViewBasic, ZFUIImageView *, cellIcon)
{
    this->cellIconContainer()->childAdd(this->cellIcon());
    this->cellIcon()->layoutParam()->layoutAlign(ZFUIAlign::e_Center);
}
ZFPROPERTY_ON_DETACH_DEFINE(ZFUIListCellViewBasic, ZFUIImageView *, cellIcon)
{
    this->cellIcon()->viewRemoveFromParent();
}

ZFPROPERTY_ON_INIT_DEFINE(ZFUIListCellViewBasic, ZFUITextView *, cellLabelMain)
{
    zfblockedAlloc(ZFUITextView, cellLabelMain);
    propertyValue = cellLabelMain;
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIListCellViewBasic, ZFUITextView *, cellLabelMain)
{
    this->cellLabelMainContainer()->childAdd(this->cellLabelMain());
    this->cellLabelMain()->layoutParam()->layoutAlign(ZFUIAlign::e_LeftInner);
}
ZFPROPERTY_ON_DETACH_DEFINE(ZFUIListCellViewBasic, ZFUITextView *, cellLabelMain)
{
    this->cellLabelMain()->viewRemoveFromParent();
}

ZFPROPERTY_ON_INIT_DEFINE(ZFUIListCellViewBasic, ZFUITextView *, cellLabelSub)
{
    zfblockedAlloc(ZFUITextView, cellLabelSub);
    propertyValue = cellLabelSub;
    cellLabelSub->textColor(ZFUIGlobalStyle::DefaultStyle()->textColorSecondary());
    cellLabelSub->textSize(ZFUIGlobalStyle::DefaultStyle()->textSizeSmall());
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIListCellViewBasic, ZFUITextView *, cellLabelSub)
{
    this->cellLabelSubContainer()->childAdd(this->cellLabelSub());
    this->cellLabelSub()->layoutParam()->layoutAlign(ZFUIAlign::e_LeftInner);
}
ZFPROPERTY_ON_DETACH_DEFINE(ZFUIListCellViewBasic, ZFUITextView *, cellLabelSub)
{
    this->cellLabelSub()->viewRemoveFromParent();
}

ZFMETHOD_DEFINE_0(ZFUIListCellViewBasic, ZFUIView *, cellIconContainer)
{
    return d->cellIconContainer;
}
ZFMETHOD_DEFINE_0(ZFUIListCellViewBasic, ZFUIView *, cellCenterContainer)
{
    return d->cellCenterContainer;
}
ZFMETHOD_DEFINE_0(ZFUIListCellViewBasic, ZFUIView *, cellLabelMainContainer)
{
    return d->cellLabelMainContainer;
}
ZFMETHOD_DEFINE_0(ZFUIListCellViewBasic, ZFUIView *, cellLabelSubContainer)
{
    return d->cellLabelSubContainer;
}
ZFMETHOD_DEFINE_0(ZFUIListCellViewBasic, ZFUIView *, cellAccessoryContainer)
{
    return d->cellAccessoryContainer;
}

void ZFUIListCellViewBasic::objectOnInit(void)
{
    zfsuper::objectOnInit();

    d = zfpoolNew(_ZFP_ZFUIListCellViewBasicPrivate);

    d->cellIconContainer = zfAlloc(ZFUIViewLayout);

    {
        d->cellCenterContainer = zfAlloc(ZFUILinearLayout);
        d->cellCenterContainer->layoutOrientation(ZFUIOrientation::e_Top);

        d->cellLabelMainContainer = zfAlloc(ZFUIViewLayout);
        d->cellCenterContainer->childAdd(d->cellLabelMainContainer);
        d->cellLabelMainContainer->layoutParam()->layoutAlign(ZFUIAlign::e_LeftInner);

        d->cellLabelSubContainer = zfAlloc(ZFUIViewLayout);
        d->cellCenterContainer->childAdd(d->cellLabelSubContainer);
        d->cellLabelSubContainer->layoutParam()->layoutAlign(ZFUIAlign::e_LeftInner);
    }

    d->cellAccessoryContainer = zfAlloc(ZFUIViewLayout);
}
void ZFUIListCellViewBasic::objectOnDealloc(void)
{
    zfRelease(d->cellIconContainer);
    zfRelease(d->cellCenterContainer);
    zfRelease(d->cellLabelMainContainer);
    zfRelease(d->cellLabelSubContainer);
    zfRelease(d->cellAccessoryContainer);

    zfpoolDelete(d);
    zfsuper::objectOnDealloc();
}

ZF_NAMESPACE_GLOBAL_END

