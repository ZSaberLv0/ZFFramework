/**
 * @file ZFProtocolZFImplOutput.h
 * @brief protocol for output logs,
 *   used by zfCoreLog, ZFLog, etc,
 *   as default output callback
 */

#ifndef _ZFI_ZFProtocolZFImplOutput_h_
#define _ZFI_ZFProtocolZFImplOutput_h_

#include "ZFCore/ZFProtocol.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for output logs
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFImplOutput)
ZFPROTOCOL_INTERFACE_OPTIONAL
public:
    /**
     * @brief output callback for zfCoreLog,
     *   usually as critical error messages
     *
     * within this method,
     * you must not access any other ZFFramework's method
     */
    virtual void outputCoreLog(ZF_IN const zfchar *s) zfpurevirtual;
    /**
     * @brief output callback for ZFLog
     */
    virtual void outputLog(ZF_IN const zfchar *s, ZF_IN_OPT zfindex count = zfindexMax()) zfpurevirtual;
ZFPROTOCOL_INTERFACE_END(ZFImplOutput)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFImplOutput_h_

