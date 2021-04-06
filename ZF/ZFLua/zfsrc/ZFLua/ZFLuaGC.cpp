#include "ZFLuaGC.h"
#include "ZFLuaState.h"
#include "protocol/ZFProtocolZFLua.h"

#include "ZFCore/protocol/ZFProtocolZFThreadTaskRequest.h"
#include "ZFCore/ZFSTLWrapper/zfstl_map.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFMETHOD_FUNC_DEFINE_1(void, ZFLuaGCImmediately,
                       ZFMP_IN_OPT(void *, L, zfnull))
{
    ZFPROTOCOL_ACCESS(ZFLua)->luaGC(L ? L : ZFLuaState());
}

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFLuaGCHolder, ZFLevelZFFrameworkNormal)
{
    this->luaStateOnDetachListener = ZFCallbackForFunc(luaStateOnDetach);
    ZFGlobalObserver().observerAdd(
        ZFGlobalEvent::EventLuaStateOnDetach(),
        this->luaStateOnDetachListener);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFLuaGCHolder)
{
    ZFGlobalObserver().observerRemove(
        ZFGlobalEvent::EventLuaStateOnDetach(),
        this->luaStateOnDetachListener);
}
zfstlmap<void *, zfbool> m;
ZFListener luaStateOnDetachListener;
static void luaStateOnDetach(ZF_IN const ZFListenerData &listenerData, ZF_IN ZFObject *userData)
{
    zfCoreMutexLocker();
    zfstlmap<void *, zfbool> &m = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFLuaGCHolder)->m;
    m.erase(listenerData.param0<v_VoidPointer *>()->zfv);
}
ZF_GLOBAL_INITIALIZER_END(ZFLuaGCHolder)

static void _ZFP_ZFLuaGCResolve(ZF_IN const ZFListenerData &listenerData, ZF_IN ZFObject *userData)
{
    zfstlmap<void *, zfbool> &m = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFLuaGCHolder)->m;
    zfCoreMutexLock();
    while(!m.empty())
    {
        zfstlmap<void *, zfbool>::iterator it = m.begin();
        void *L = it->first;
        m.erase(it);

        zfCoreMutexUnlock();
        ZFLuaGCImmediately(L);
        zfCoreMutexLock();
    }
    zfCoreMutexUnlock();
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFLuaGC,
                       ZFMP_IN_OPT(void *, L, zfnull))
{
    if(!ZFPROTOCOL_IS_AVAILABLE(ZFThreadTaskRequest))
    {
        ZFLuaGCImmediately(L);
        return ;
    }

    zfCoreMutexLocker();
    zfstlmap<void *, zfbool> &m = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFLuaGCHolder)->m;
    if(m.find(L) == m.end())
    {
        m[L] = zftrue;
        if(m.size() == 1)
        {
            ZFThreadTaskRequest(ZFCallbackForFunc(_ZFP_ZFLuaGCResolve));
        }
    }
}

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFLuaGCAutoApply, ZFLevelZFFrameworkNormal)
{
    this->classChangeListener = ZFCallbackForFunc(zfself::classChange);
    ZFClassDataChangeObserver.observerAdd(
        ZFGlobalEvent::EventClassDataChange(),
        this->classChangeListener);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFLuaGCAutoApply)
{
    ZFClassDataChangeObserver.observerRemove(
        ZFGlobalEvent::EventClassDataChange(),
        this->classChangeListener);
}
ZFListener classChangeListener;
static void classChange(ZF_IN const ZFListenerData &listenerData, ZF_IN ZFObject *userData)
{
    const ZFClassDataChangeData *data = listenerData.param0<ZFPointerHolder *>()->holdedDataPointer<const ZFClassDataChangeData *>();
    if(data->changedClass != zfnull && data->changeType == ZFClassDataChangeTypeDetach)
    {
        ZFCoreArrayPOD<void *> L;
        ZFLuaStateListT(L);
        for(zfindex i = 0; i < L.count(); ++i)
        {
            ZFLuaGC(L[i]);
        }
    }
}
ZF_GLOBAL_INITIALIZER_END(ZFLuaGCAutoApply)

ZF_NAMESPACE_GLOBAL_END

