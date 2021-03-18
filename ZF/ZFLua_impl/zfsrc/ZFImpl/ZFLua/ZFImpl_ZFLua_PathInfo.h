/**
 * @file ZFImpl_ZFLua_PathInfo.h
 * @brief path info impl
 */

#ifndef _ZFI_ZFImpl_ZFLua_PathInfo_h_
#define _ZFI_ZFImpl_ZFLua_PathInfo_h_

#include "ZFImpl_ZFLua.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief setup path info for lua execute
 *
 * how it works:\n
 * lua does not have any way to append additional user data to chunk,
 * so we hack it by appending extra code before each #ZFLuaExecute,
 * to setup some path info related datas, mainly `zfl_L()` to return a #ZFPathInfo\n
 * \n
 * impl may use #ZFImpl_ZFLua_implPathInfo_DEFINE to append extra contents,
 * the final extra code may be something look like this:
 * @code
 *   -- in global scope
 *   function _ZFP_l(zfl_l)
 *       return
 *           function (...) -- appended by module xxx
 *               return xxxAction(zfl_l)
 *           end,
 *           function (...) -- appended by module yyy
 *               return yyyAction(zfl_l)
 *           end,
 *           nil
 *   end
 *
 *   -- in local scope
 *   local xxx,yyy = _ZFP_l(ZFPathInfo(pathType, pathData));
 *
 *   -- after the above code,
 *   -- the xxx() and yyy() call should call the local version with local path info
 *   xxx()
 *   yyy()
 * @endcode
 * @code
 *   // in cpp code
 *   ZFImpl_ZFLua_implPathInfo_DEFINE(xxx,
 *           "function(...) return xxxAction(zfl_l) end"
 *       )
 * @endcode
 */
extern ZF_ENV_EXPORT void ZFImpl_ZFLua_implPathInfoSetup(ZF_IN lua_State *L,
                                                         ZF_OUT zfstring &ret,
                                                         ZF_IN const ZFPathInfo *pathInfo);

/** @see #ZFImpl_ZFLua_implPathInfoSetup */
#define ZFImpl_ZFLua_implPathInfo_DEFINE(luaFunc, luaFuncBody) \
    ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFImpl_ZFLua_implPathInfo_##luaFunc, ZFLevelZFFrameworkNormal) \
    { \
        _ZFP_ZFImpl_ZFLua_implPathInfoRegister(ZFM_TOSTRING(luaFunc), (luaFuncBody)); \
    } \
    ZF_GLOBAL_INITIALIZER_DESTROY(ZFImpl_ZFLua_implPathInfo_##luaFunc) \
    { \
        _ZFP_ZFImpl_ZFLua_implPathInfoUnregister(ZFM_TOSTRING(luaFunc)); \
    } \
    ZF_GLOBAL_INITIALIZER_END(ZFImpl_ZFLua_implPathInfo_##luaFunc)

extern ZF_ENV_EXPORT void _ZFP_ZFImpl_ZFLua_implPathInfoRegister(ZF_IN const zfchar *luaFuncName,
                                                                 ZF_IN const zfchar *luaFuncBody);
extern ZF_ENV_EXPORT void _ZFP_ZFImpl_ZFLua_implPathInfoUnregister(ZF_IN const zfchar *luaFuncName);

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFImpl_ZFLua_PathInfo_h_

