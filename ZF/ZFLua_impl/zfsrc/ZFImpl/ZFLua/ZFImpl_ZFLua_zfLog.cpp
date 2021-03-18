#include "ZFImpl_ZFLua.h"
#include "ZFImpl_ZFLua_PathInfo.h"

ZF_NAMESPACE_GLOBAL_BEGIN

#define _ZFP_ZFImpl_ZFLua_zfLog_DEBUG_ENABLE ZF_ENV_DEBUG

static int _ZFP_ZFImpl_ZFLua_zfLog(ZF_IN lua_State *L)
{
    zfstring s;
    if(ZFImpl_ZFLua_zfstringAppend(L, s))
    {
        zfLogTrimT() << zfLogCurTimeString() << s;
    }
    return 0;
}

static int _ZFP_ZFImpl_ZFLua_zfLogTrim(ZF_IN lua_State *L)
{
    zfstring s;
    if(ZFImpl_ZFLua_zfstringAppend(L, s))
    {
        zfLogTrimT() << s;
    }
    return 0;
}

static int _ZFP_ZFImpl_ZFLua_zfLogT(ZF_IN lua_State *L)
{
    int count = (int)lua_gettop(L);
    if(count != 0)
    {
        ZFLuaErrorOccurredTrim(
            "[zfLogT] takes no param, got %zi param",
            (zfindex)count);
        return ZFImpl_ZFLua_luaError(L);
    }

    zfblockedAlloc(v_ZFCallback, ret);
    ret->zfv = (zfLogTrimT() << zfLogCurTimeString());
    ZFImpl_ZFLua_luaPush(L, ret);
    return 1;
}

static int _ZFP_ZFImpl_ZFLua_zfLogTrimT(ZF_IN lua_State *L)
{
    int count = (int)lua_gettop(L);
    if(count != 0)
    {
        ZFLuaErrorOccurredTrim(
            "[zfLogTrimT] takes no param, got %zi param",
            (zfindex)count);
        return ZFImpl_ZFLua_luaError(L);
    }

    zfblockedAlloc(v_ZFCallback, ret);
    ret->zfv = zfLogTrimT();
    ZFImpl_ZFLua_luaPush(L, ret);
    return 1;
}

// ============================================================
ZFImpl_ZFLua_implSetupCallback_DEFINE(zfLog, {
        ZFImpl_ZFLua_luaCFunctionRegister(L, "zfLog", _ZFP_ZFImpl_ZFLua_zfLog);
        ZFImpl_ZFLua_luaCFunctionRegister(L, "zfLogTrim", _ZFP_ZFImpl_ZFLua_zfLogTrim);
        ZFImpl_ZFLua_luaCFunctionRegister(L, "zfLogT", _ZFP_ZFImpl_ZFLua_zfLogT);
        ZFImpl_ZFLua_luaCFunctionRegister(L, "zfLogTrimT", _ZFP_ZFImpl_ZFLua_zfLogTrimT);
    }, {
    })

#if _ZFP_ZFImpl_ZFLua_zfLog_DEBUG_ENABLE
    ZFImpl_ZFLua_implPathInfo_DEFINE(zfLog,
            "function (f, ...)"
            "    return _G['zfLog']('[' .. tostring(zfl_l) .. ' (' .. debug.getinfo(2).currentline .. ')] ' .. (f or ''), ...);"
            "end"
        )
    ZFImpl_ZFLua_implPathInfo_DEFINE(zfLogT,
            "function ()"
            "    return _G['zfLogT']():log('[%s (%s)]', zfl_l, zfint(debug.getinfo(2).currentline));"
            "end"
        )
#else
    ZFImpl_ZFLua_implPathInfo_DEFINE(zfLog,
            "function (f, ...)"
            "    return _G['zfLog']('[' .. tostring(zfl_l) .. '] ' .. (f or ''), ...);"
            "end"
        )
    ZFImpl_ZFLua_implPathInfo_DEFINE(zfLogT,
            "function ()"
            "    return _G['zfLogT']():log('[%s]', zfl_l);"
            "end"
        )
#endif

ZF_NAMESPACE_GLOBAL_END

