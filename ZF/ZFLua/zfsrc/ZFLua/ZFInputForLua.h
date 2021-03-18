/**
 * @file ZFInputForLua.h
 * @brief util to create #ZFInput for lua
 */

#ifndef _ZFI_ZFInputForLua_h_
#define _ZFI_ZFInputForLua_h_

#include "ZFLuaDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief util to create #ZFInput for lua
 *
 * luaCallback's param0 would be a #v_zfstring let lua code to output,
 * and param1 holds a #v_zfindex that indicates the #ZFInput's input length
 * or #zfindexMax to calculate the total input length (see #ZFInput),
 * upon input failure,
 * the lua code should change the param1's value
 * @code
 *   ZFInputForLua(function(listenerData, userData)
 *       if listenerData:param1() == zfindexMax() then
 *           // return total input size
 *           listenerData:param1():zfv(yourTotalLength)
 *       end
 *
 *       local buf = listenerData:param0()
 *       local count = listenerData:param1()
 *       // write to the buf
 *       buf:zfv(yourData)
 *       // save written length
 *       listenerData:param1():zfv(yourWrittenLength)
 *   end)
 * @endcode
 */
ZFMETHOD_FUNC_DECLARE_2(ZFInput, ZFInputForLua,
                        ZFMP_IN(const ZFListener &, luaCallback),
                        ZFMP_IN_OPT(ZFObject *, userData, zfnull))

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFInputForLua_h_

