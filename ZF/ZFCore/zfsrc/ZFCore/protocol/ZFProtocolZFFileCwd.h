/**
 * @file ZFProtocolZFFileCwd.h
 * @brief protocol for ZFFile
 */

#ifndef _ZFI_ZFProtocolZFFileCwd_h_
#define _ZFI_ZFProtocolZFFileCwd_h_

#include "ZFCore/ZFProtocol.h"
#include "ZFCore/ZFFile.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for ZFFile
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFFileCwd)
ZFPROTOCOL_INTERFACE_OPTIONAL
public:
    /**
     * @brief see #ZFFilePathForCwd
     */
    virtual const zfchar *pathForCwd(void) zfpurevirtual;
    /**
     * @brief see #ZFFilePathForCwdChange
     */
    virtual zfbool pathForCwdChange(ZF_IN const zfchar *pathForCwd) zfpurevirtual;
ZFPROTOCOL_INTERFACE_END(ZFFileCwd)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFFileCwd_h_

