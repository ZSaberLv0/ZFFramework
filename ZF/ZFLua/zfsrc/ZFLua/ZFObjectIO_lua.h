/**
 * @file ZFObjectIO_lua.h
 * @brief IO type for #ZFObjectIOLoad
 */

#ifndef _ZFI_ZFObjectIO_lua_h_
#define _ZFI_ZFObjectIO_lua_h_

#include "ZFLuaExecute.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief see #ZFObjectIOLoad
 *
 * for "*.lua" file, which should be executed by #ZFLuaExecute
 * and return an object
 */
#define ZFObjectIO_lua "lua"

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFObjectIO_lua_h_

