/**
 * @file ZFOutputForLua.h
 * @brief util to create #ZFOutput for lua
 */

#ifndef _ZFI_ZFOutputForLua_h_
#define _ZFI_ZFOutputForLua_h_

#include "ZFLuaDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief util to create #ZFOutput for lua
 *
 * luaCallback's param0 would be a #v_zfstring,
 * and param1 holds a #v_zfindex that indicates the #ZFOutput's return value
 * @code
 *   ZFOutputForLua(function(listenerData, userData)
 *       local text = listenerData:param0()
 *       local result = yourOutput(text)
 *       listenerData:param1():zfv(result)
 *   end)
 * @endcode
 */
ZFMETHOD_FUNC_DECLARE_2(ZFOutput, ZFOutputForLua,
                        ZFMP_IN(const ZFListener &, luaCallback),
                        ZFMP_IN_OPT(ZFObject *, userData, zfnull))

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFOutputForLua_h_

