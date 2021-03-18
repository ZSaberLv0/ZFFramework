/**
 * @file ZFGlobalEventCenter.h
 * @brief event center to broadcast and listen notifications
 */

#ifndef _ZFI_ZFGlobalEventCenter_h_
#define _ZFI_ZFGlobalEventCenter_h_

#include "ZFObjectGlobalInstance.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief event center to notify events which not belong to any object, see #ZFObject::observerNotify
 *
 * this is actually a dummy object,
 * to hold app-wide observers\n
 * to use it:
 * -  declare your own event defines (by #ZFOBSERVER_EVENT or #ZFOBSERVER_EVENT_GLOBAL)
 * -  register observer by #ZFObject::observerAdd
 * -  notify observer by #ZFObject::observerNotifyWithCustomSender
 * -  remember to use #ZFObject::observerRemove to unregister if necessary
 */
zfclass ZF_ENV_EXPORT ZFGlobalEventCenter : zfextends ZFObject
{
    ZFOBJECT_DECLARE(ZFGlobalEventCenter, ZFObject)
    ZFOBJECT_SINGLETON_DECLARE(ZFGlobalEventCenter, instance)

public:
    /**
     * @brief for global event center, sender would always be null if not specified
     */
    zfoverride
    virtual inline void observerNotify(ZF_IN zfidentity eventId,
                                       ZF_IN_OPT ZFObject *param0 = zfnull,
                                       ZF_IN_OPT ZFObject *param1 = zfnull)
    {
        this->observerNotifyWithCustomSender(zfnull, eventId, param0, param1);
    }
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFGlobalEventCenter_h_

