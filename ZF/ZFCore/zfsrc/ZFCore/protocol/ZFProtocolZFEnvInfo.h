/**
 * @file ZFProtocolZFEnvInfo.h
 * @brief protocol for ZFEnvInfo
 */

#ifndef _ZFI_ZFProtocolZFEnvInfo_h_
#define _ZFI_ZFProtocolZFEnvInfo_h_

#include "ZFCore/ZFProtocol.h"
#include "ZFCore/ZFEnvInfo.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief protocol for ZFEnvInfo
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFEnvInfo_systemInfo)
ZFPROTOCOL_INTERFACE_OPTIONAL
public:
    /**
     * @brief name of native system, e.g. "iOS", empty if not available
     */
    virtual void systemName(ZF_IN_OUT zfstring &ret) {}
    /**
     * @brief version of native system, e.g. "9.0.1", empty if not available
     */
    virtual void systemVersion(ZF_IN_OUT zfstring &ret) {}
ZFPROTOCOL_INTERFACE_END(ZFEnvInfo_systemInfo)

// ============================================================
/**
 * @brief protocol for ZFEnvInfo
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFEnvInfo_frameworkInfo)
ZFPROTOCOL_INTERFACE_OPTIONAL
public:
    /**
     * @brief name of native framework, e.g. "Qt", empty if not available
     */
    virtual void frameworkName(ZF_IN_OUT zfstring &ret) {}
    /**
     * @brief version of native framework, e.g. "9.0.1", empty if not available
     */
    virtual void frameworkVersion(ZF_IN_OUT zfstring &ret) {}
ZFPROTOCOL_INTERFACE_END(ZFEnvInfo_frameworkInfo)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFEnvInfo_h_

