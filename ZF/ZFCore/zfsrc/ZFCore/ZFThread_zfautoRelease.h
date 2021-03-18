/**
 * @file ZFThread_zfautoRelease.h
 * @brief thread utility
 */

#ifndef _ZFI_ZFThread_zfautoRelease_h_
#define _ZFI_ZFThread_zfautoRelease_h_

#include "ZFThread.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
extern ZF_ENV_EXPORT void _ZFP_zfautoReleaseAction(ZF_IN ZFObject *obj);
template<typename T_ZFObject>
inline T_ZFObject _ZFP_zfautoRelease(ZF_IN T_ZFObject obj)
{
    _ZFP_zfautoReleaseAction(ZFCastZFObjectUnchecked(ZFObject *, obj));
    return obj;
}
/**
 * @brief make object autorelease, which would be released by the owner thread
 *
 * calling this function will add the object to current thread's autorelease pool\n
 * assert failure if current thread isn't started or registered by ZFThread\n
 * note that which time to release the objects in pool, is depends on implementation
 * @see zfRetain, zfRelease
 *
 * ADVANCED:\n
 * this method depends on ZFThread's implementation\n
 * if no ZFThread's implementation found,
 * we'll try to add to global memory pool (see #ZFAutoReleasePool::instance),
 * and try to schedule a drain task by #ZFThreadTaskRequest\n
 * if even taskRequest not be implemented,
 * then the pool won't be drained automatically,
 * you should do it manually if necessary\n
 * \n
 * so, try not to abuse this method, since:
 * -  the actual behavior depends on ZFThread's implementation
 * -  it have lower performance than other release methods
 *   (see #ZFObject for more info about other release methods)
 */
#define zfautoRelease(obj) _ZFP_zfautoRelease(obj)
/** @see zfautoRelease */
#define zfautoRelease(obj) _ZFP_zfautoRelease(obj)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFThread_zfautoRelease_h_

