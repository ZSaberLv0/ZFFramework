/**
 * @file ZFUIEvent.h
 * @brief base class of all ui event
 */

#ifndef _ZFI_ZFUIEvent_h_
#define _ZFI_ZFUIEvent_h_

#include "ZFUITypeDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief base class of all ui event
 */
zfclass ZF_ENV_EXPORT ZFUIEvent : zfextends ZFObject
{
    ZFOBJECT_DECLARE_WITH_CUSTOM_CTOR(ZFUIEvent, ZFObject)

    ZFALLOC_CACHE_RELEASE({
        cache->eventResolved(zffalse);
    })

public:
    /**
     * @brief whether the event has been resolved
     */
    ZFMETHOD_DECLARE_1(void, eventResolved,
                       ZFMP_IN(zfbool const &, value))
    /**
     * @brief see #eventResolved
     */
    ZFMETHOD_DECLARE_0(zfbool, eventResolved)

protected:
    /**
     * @brief see #ZFUIView::UIScaleOnChange, usually internal use only
     */
    virtual void eventOnApplyScale(ZF_IN zffloat scale)
    {
    }
    /**
     * @brief see #eventOnApplyScale
     */
    virtual void eventOnApplyScaleReversely(ZF_IN zffloat scale)
    {
    }

protected:
    zfoverride
    virtual void objectInfoOnAppend(ZF_IN_OUT zfstring &ret);

private:
    zfbool _ZFP_ZFUIEvent_eventResolved;
protected:
    /** @cond ZFPrivateDoc */
    ZFUIEvent(void) : _ZFP_ZFUIEvent_eventResolved(zffalse) {}
    /** @endcond */
public:
    inline void _ZFP_ZFUIEvent_eventOnApplyScale(ZF_IN zffloat scale) {this->eventOnApplyScale(scale);}
    inline void _ZFP_ZFUIEvent_eventOnApplyScaleReversely(ZF_IN zffloat scale) {this->eventOnApplyScaleReversely(scale);}
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIEvent_h_

