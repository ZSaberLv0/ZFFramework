/**
 * @file ZFGlobalEventCenter_common.h
 * @brief common event defines
 */

#ifndef _ZFI_ZFGlobalEventCenter_common_h_
#define _ZFI_ZFGlobalEventCenter_common_h_

#include "ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN
ZF_NAMESPACE_BEGIN(ZFGlobalEvent)

/**
 * @brief see #ZFObject::observerNotify
 *
 * notified when #ZFFrameworkInit finished,
 * params and result has no use and is always null
 */
ZFOBSERVER_EVENT_GLOBAL(AppOnCreate)
/**
 * @brief see #ZFObject::observerNotify
 *
 * notified before #ZFFrameworkCleanup,
 * params and result has no use and is always null
 */
ZFOBSERVER_EVENT_GLOBAL(AppOnDestroy)
/**
 * @brief see #ZFObject::observerNotify
 *
 * notified when received memory warning (depend on implementation),
 * params and result has no use and is always null
 */
ZFOBSERVER_EVENT_GLOBAL(AppOnMemoryLow)

ZF_NAMESPACE_END(ZFGlobalEvent)
ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFGlobalEventCenter_common_h_

