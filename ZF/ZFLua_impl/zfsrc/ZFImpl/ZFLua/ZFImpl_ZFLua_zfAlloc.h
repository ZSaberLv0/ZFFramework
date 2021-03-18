/**
 * @file ZFImpl_ZFLua_zfAlloc.h
 * @brief lua impl
 */

#ifndef _ZFI_ZFImpl_ZFLua_zfAlloc_h_
#define _ZFI_ZFImpl_ZFLua_zfAlloc_h_

#include "ZFImpl_ZFLua.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief for impl to alloc object
 */
extern ZF_ENV_EXPORT zfbool ZFImpl_ZFLua_zfAlloc(ZF_OUT zfautoObject &ret,
                                                 ZF_IN lua_State *L,
                                                 ZF_IN const ZFClass *cls,
                                                 ZF_IN int paramCount,
                                                 ZF_IN int luaParamOffset);

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFImpl_ZFLua_zfAlloc_h_

