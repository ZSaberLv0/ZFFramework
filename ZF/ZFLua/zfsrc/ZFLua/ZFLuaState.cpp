#include "ZFLuaState.h"
#include "protocol/ZFProtocolZFLua.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFMETHOD_FUNC_DEFINE_0(void *, ZFLuaState)
{
    return ZFPROTOCOL_ACCESS(ZFLua)->luaState();
}

ZFMETHOD_FUNC_DEFINE_1(void, ZFLuaStateChange,
                       ZFMP_IN(void *, L))
{
    ZFPROTOCOL_ACCESS(ZFLua)->luaStateChange(L);
}

ZFMETHOD_FUNC_DEFINE_1(void, ZFLuaStateListT,
                       ZFMP_IN_OUT(ZFCoreArray<void *> &, ret))
{
    ZFPROTOCOL_ACCESS(ZFLua)->luaStateList(ret);
}
ZFMETHOD_FUNC_DEFINE_0(ZFCoreArrayPOD<void *>, ZFLuaStateList)
{
    ZFCoreArrayPOD<void *> ret;
    ZFPROTOCOL_ACCESS(ZFLua)->luaStateList(ret);
    return ret;
}

ZFMETHOD_FUNC_DEFINE_0(void *, ZFLuaStateOpen)
{
    return ZFPROTOCOL_ACCESS(ZFLua)->luaStateOpen();
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFLuaStateClose,
                       ZFMP_IN(void *, L))
{
    ZFPROTOCOL_ACCESS(ZFLua)->luaStateClose(L);
}

ZFMETHOD_FUNC_DEFINE_1(void, ZFLuaStateAttach,
                       ZFMP_IN(void *, L))
{
    ZFPROTOCOL_ACCESS(ZFLua)->luaStateAttach(L);
    if(ZFGlobalObserver().observerHasAdd(ZFGlobalEvent::EventLuaStateOnAttach()))
    {
        ZFGlobalObserver().observerNotify(
            ZFGlobalEvent::EventLuaStateOnAttach(),
            zflineAlloc(v_VoidPointer, L));
    }
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFLuaStateDetach,
                       ZFMP_IN(void *, L))
{
    if(ZFGlobalObserver().observerHasAdd(ZFGlobalEvent::EventLuaStateOnDetach()))
    {
        ZFGlobalObserver().observerNotify(
            ZFGlobalEvent::EventLuaStateOnDetach(),
            zflineAlloc(v_VoidPointer, L));
    }
    ZFPROTOCOL_ACCESS(ZFLua)->luaStateDetach(L);
}

// ============================================================
ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
ZFOBSERVER_EVENT_GLOBAL_REGISTER(LuaStateOnAttach)
ZFOBSERVER_EVENT_GLOBAL_REGISTER(LuaStateOnDetach)
ZF_NAMESPACE_END(ZFGlobalEvent)

ZF_NAMESPACE_GLOBAL_END

