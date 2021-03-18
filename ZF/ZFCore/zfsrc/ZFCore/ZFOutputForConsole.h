/**
 * @file ZFOutputForConsole.h
 * @brief common callback defines
 */

#ifndef _ZFI_ZFOutputForConsole_h_
#define _ZFI_ZFOutputForConsole_h_

#include "ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief see #ZFCALLBACK_SERIALIZE_CUSTOM_TYPE_DEFINE
 *
 * serializable data:
 * @code
 *   <node>
 *   </node>
 * @endcode
 */
#define ZFCallbackSerializeCustomType_ZFOutputForConsole "ZFOutputForConsole"

/**
 * @brief default output callback to output to console, source must be zfchar *string
 */
ZFEXPORT_VAR_READONLY_DECLARE(ZFOutput, ZFOutputForConsole)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFOutputForConsole_h_

