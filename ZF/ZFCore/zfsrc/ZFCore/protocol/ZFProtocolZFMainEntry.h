/**
 * @file ZFProtocolZFMainEntry.h
 * @brief protocol for #ZFMAIN_ENTRY
 */

#ifndef _ZFI_ZFProtocolZFMainEntry_h_
#define _ZFI_ZFProtocolZFMainEntry_h_

#include "ZFCore/ZFProtocol.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief used by implementation only
 *
 * impl must:
 * -# call #ZFFrameworkInit at proper time
 * -# call #ZFMainExecute at proper time
 * -# call #ZFFrameworkCleanup at proper time
 */
extern ZF_ENV_EXPORT zfint ZFMainExecute(ZF_IN const ZFCoreArray<zfstring> &appParams = ZFCoreArray<zfstring>());

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFMainEntry_h_

