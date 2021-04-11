/**
 * @file ZFIOCallback_util.h
 * @brief common callback defines
 */

#ifndef _ZFI_ZFIOCallback_util_h_
#define _ZFI_ZFIOCallback_util_h_

#include "ZFIOCallback_input.h"
#include "ZFIOCallback_output.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief util method to read all contents of input to output
 *
 * return size already written to output even if error occurred
 */
extern ZF_ENV_EXPORT zfindex ZFInputReadToOutput(ZF_IN_OUT const ZFOutput &output,
                                                 ZF_IN_OUT const ZFInput &input);

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFIOCallback_util_h_

