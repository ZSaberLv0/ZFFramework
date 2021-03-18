/**
 * @file ZFResultType.h
 * @brief result type define
 */

#ifndef _ZFI_ZFResultType_h_
#define _ZFI_ZFResultType_h_

#include "ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief result type define
 */
ZFENUM_BEGIN(ZFResultType)
    ZFENUM_VALUE(Success)
    ZFENUM_VALUE(Fail)
    ZFENUM_VALUE(Cancel)
ZFENUM_SEPARATOR(ZFResultType)
    ZFENUM_VALUE_REGISTER(Success)
    ZFENUM_VALUE_REGISTER(Fail)
    ZFENUM_VALUE_REGISTER(Cancel)
ZFENUM_END(ZFResultType)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFResultType_h_

