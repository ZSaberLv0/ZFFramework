/**
 * @file ZFUIScrollerDefault.h
 * @brief default scroller of #ZFUIScroller
 */

#ifndef _ZFI_ZFUIScrollerDefault_h_
#define _ZFI_ZFUIScrollerDefault_h_

#include "ZFUIScroller.h"
ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd _ZFP_ZFUIScrollerDefaultPrivate;
/**
 * @brief default scroller of #ZFUIScroller
 */
zfclass ZF_ENV_EXPORT ZFUIScrollerDefault : zfextends ZFObject, zfimplements ZFUIScroller
{
    ZFOBJECT_DECLARE(ZFUIScrollerDefault, ZFObject)
    ZFIMPLEMENTS_DECLARE(ZFUIScroller)

protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);

public:
    zfoverride
    virtual void scrollOwnerSizeChanged(ZF_IN zffloat ownerSize);
    zfoverride
    virtual void scrollBounceChanged(ZF_IN zfbool scrollBounce,
                                     ZF_IN zfbool scrollBounceAlways);
    zfoverride
    virtual void scrollAlignToPageChanged(ZF_IN zfbool scrollAlignToPage);
    zfoverride
    virtual void scrollContentChanged(ZF_IN zffloat contentOffset, ZF_IN zffloat contentSize);

public:
    zfoverride
    virtual void scrollToFitRange(void);
    zfoverride
    virtual void scrollWithoutAnimation(ZF_IN zffloat contentOffset);
    zfoverride
    virtual void scrollByPoint(ZF_IN zffloat point);
    zfoverride
    virtual zffloat scrollByPointEndPoint(void);
    zfoverride
    virtual void scrollBySpeed(ZF_IN zffloat speed);
    zfoverride
    virtual zffloat scrollBySpeedCurrentSpeed(void);
    zfoverride
    virtual zffloat scrollBySpeedEndPointPredicted(void);

    zfoverride
    virtual zffloat scrollEndPointPredicted(void);

    zfoverride
    virtual zfbool scrollRequireFocus(void);

public:
    zfoverride
    virtual zffloat scrollOwnerSize(void);
    zfoverride
    virtual zffloat scrollContentOffset(void);
    zfoverride
    virtual zffloat scrollContentSize(void);

public:
    zfoverride
    virtual void scrollOnDragBegin(ZF_IN zffloat mousePos,
                                   ZF_IN zftimet mouseTime);
    zfoverride
    virtual void scrollOnDrag(ZF_IN zffloat mousePos,
                              ZF_IN zftimet mouseTime);
    zfoverride
    virtual void scrollOnDragEnd(ZF_IN zftimet mouseTime,
                                 ZF_IN zfbool needScrollAni);

public:
    zfoverride
    virtual void scrollAniOnUpdate(ZF_IN zftimet time);

private:
    _ZFP_ZFUIScrollerDefaultPrivate *d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIScrollerDefault_h_

