/**
 * @file ZFProtocolZFEnvInfo_ZFUIKit.h
 * @brief protocol for ZFEnvInfo
 */

#ifndef _ZFI_ZFProtocolZFEnvInfo_ZFUIKit_h_
#define _ZFI_ZFProtocolZFEnvInfo_ZFUIKit_h_

#include "ZFCore/ZFProtocol.h"
#include "ZFUIKit/ZFEnvInfo_ZFUIKit.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief protocol for ZFEnvInfo
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFEnvInfo_deviceUIInfo)
ZFPROTOCOL_INTERFACE_OPTIONAL
public:
    /**
     * @brief device type
     */
    virtual ZFEnvDeviceUITypeEnum deviceUIType(ZF_IN_OPT ZFEnvDeviceUITypeEnum defaultValue = ZFEnvDeviceUIType::e_Desktop)
    {
        return defaultValue;
    }
ZFPROTOCOL_INTERFACE_END(ZFEnvInfo_deviceUIInfo)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFEnvInfo_ZFUIKit_h_

