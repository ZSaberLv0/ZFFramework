/**
 * @file ZFObjectMutex.h
 * @brief mutex impl for ZFObject
 */

#ifndef _ZFI_ZFObjectMutex_h_
#define _ZFI_ZFObjectMutex_h_

#include "ZFObjectClassTypeFwd.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/** @brief see #ZFObjectMutexImplSet */
typedef void *(*ZFObjectMutexImplCallbackInit)(void);
/** @brief see #ZFObjectMutexImplSet */
typedef void (*ZFObjectMutexImplCallbackDealloc)(ZF_IN void *implObject);
/** @brief see #ZFObjectMutexImplSet */
typedef ZFCoreMutexImplCallbackLock ZFObjectMutexImplCallbackLock;
/** @brief see #ZFObjectMutexImplSet */
typedef ZFCoreMutexImplCallbackUnlock ZFObjectMutexImplCallbackUnlock;
/** @brief see #ZFObjectMutexImplSet */
typedef zfbool (*ZFObjectMutexImplCallbackTryLock)(ZF_IN void *implObject);

extern ZF_ENV_EXPORT ZFObjectMutexImplCallbackInit _ZFP_ZFObjectMutexImplInit;
extern ZF_ENV_EXPORT ZFObjectMutexImplCallbackDealloc _ZFP_ZFObjectMutexImplDealloc;
extern ZF_ENV_EXPORT ZFObjectMutexImplCallbackLock _ZFP_ZFObjectMutexImplLock;
extern ZF_ENV_EXPORT ZFObjectMutexImplCallbackUnlock _ZFP_ZFObjectMutexImplUnlock;
extern ZF_ENV_EXPORT ZFObjectMutexImplCallbackTryLock _ZFP_ZFObjectMutexImplTryLock;

// ============================================================
/**
 * @brief change the implementation for mutex
 *
 * mutex is an optional module for ZFFramework,
 * however you should only setup the impl once during init setup,
 * and should not change it anymore
 * (it's safe to change impl during runtime if no object holds mutex,
 * but that's fairly impossible for most case)\n
 * the impl can be attached at any time,
 * but must not be detached until all mutex has been unlocked,
 * typically you should register by #ZFOBJECT_MUTEX_IMPL_DEFINE\n
 * \n
 * note, changing this impl would also change #ZFCoreMutexImplSet
 */
extern ZF_ENV_EXPORT void ZFObjectMutexImplSet(ZF_IN_OPT ZFObjectMutexImplCallbackInit implInit = zfnull,
                                               ZF_IN_OPT ZFObjectMutexImplCallbackDealloc implDealloc = zfnull,
                                               ZF_IN_OPT ZFObjectMutexImplCallbackLock implLock = zfnull,
                                               ZF_IN_OPT ZFObjectMutexImplCallbackUnlock implUnlock = zfnull,
                                               ZF_IN_OPT ZFObjectMutexImplCallbackTryLock implTryLock = zfnull);

/** @brief see #ZFObjectMutexImplSet */
inline ZFObjectMutexImplCallbackInit ZFObjectMutexImplGetInit(void) {return _ZFP_ZFObjectMutexImplInit;}
/** @brief see #ZFObjectMutexImplSet */
inline ZFObjectMutexImplCallbackDealloc ZFObjectMutexImplGetDealloc(void) {return _ZFP_ZFObjectMutexImplDealloc;}
/** @brief see #ZFObjectMutexImplSet */
inline ZFObjectMutexImplCallbackLock ZFObjectMutexImplGetLock(void) {return _ZFP_ZFObjectMutexImplLock;}
/** @brief see #ZFObjectMutexImplSet */
inline ZFObjectMutexImplCallbackUnlock ZFObjectMutexImplGetUnlock(void) {return _ZFP_ZFObjectMutexImplUnlock;}
/** @brief see #ZFObjectMutexImplSet */
inline ZFObjectMutexImplCallbackTryLock ZFObjectMutexImplGetTryLock(void) {return _ZFP_ZFObjectMutexImplTryLock;}

/** @brief see #ZFObjectMutexImplSet */
inline zfbool ZFObjectMutexImplAvailable(void) {return (_ZFP_ZFObjectMutexImplInit != zfnull);}

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFObjectMutex_h_

