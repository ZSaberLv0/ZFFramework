/**
 * @file ZFUIWheelEvent.h
 * @brief wheel event for ui elements
 */

#ifndef _ZFI_ZFUIWheelEvent_h_
#define _ZFI_ZFUIWheelEvent_h_

#include "ZFUIEvent.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief wheel event for ui elements
 */
zffinal zfclass ZF_ENV_EXPORT ZFUIWheelEvent : zfextends ZFUIEvent
{
    ZFOBJECT_DECLARE(ZFUIWheelEvent, ZFUIEvent)

public:
    /**
     * @brief wheel rotate count in x direction
     *
     * positive value means wheel from left to right
     */
    zfint wheelX;
    /**
     * @brief see #wheelX
     */
    zfint wheelY;

protected:
    zfoverride
    virtual void objectInfoOnAppend(ZF_IN_OUT zfstring &ret);
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIWheelEvent_h_

