/**
 * @file ZFProtocolZFObjectMutex.h
 * @brief protocol for ZFThread (used in ZFObject)
 */

#ifndef _ZFI_ZFProtocolZFObjectMutex_h_
#define _ZFI_ZFProtocolZFObjectMutex_h_

#include "ZFCore/ZFProtocol.h"
#include "ZFCore/ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief register impl for #ZFObjectMutexImplSet
 */
#define ZFOBJECT_MUTEX_IMPL_DEFINE(registerSig, protocolLevel, setupAction) \
    ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(_ZFP_OMI_##registerSig, ZFLevelZFFrameworkStatic) \
    { \
        _protocolLevel = protocolLevel; \
        if(_ZFP_ZFObjectMutexImplRegistered == zfnull || _protocolLevel > *_ZFP_ZFObjectMutexImplRegistered) \
        { \
            _ZFP_ZFObjectMutexImplRegistered = &_protocolLevel; \
            { \
                setupAction \
            } \
        } \
    } \
    ZF_GLOBAL_INITIALIZER_DESTROY(_ZFP_OMI_##registerSig) \
    { \
        if(&_protocolLevel == _ZFP_ZFObjectMutexImplRegistered) \
        { \
            ZFObjectMutexImplSet(); \
        } \
    } \
    ZFProtocolLevelEnum _protocolLevel; \
    ZF_GLOBAL_INITIALIZER_END(_ZFP_OMI_##registerSig)

extern ZF_ENV_EXPORT ZFProtocolLevelEnum *_ZFP_ZFObjectMutexImplRegistered;

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFObjectMutex_h_

