#include "ZFImpl_default_ZFCore_impl.h"
#include "ZFCore/protocol/ZFProtocolZFThreadTaskRequest.h"
#include "ZFCore/ZFThread.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFThreadTaskRequestImpl_default, ZFThreadTaskRequest, ZFProtocolLevel::e_Default)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("ZFFramework:ZFThread")

public:
    virtual void taskRequest(ZF_IN const ZFListener &task,
                             ZF_IN ZFObject *param0,
                             ZF_IN ZFObject *param1)
    {
        ZFLISTENER_LOCAL(threadCallback, {
            ZFLISTENER_LOCAL(mainThreadCallback, {
                ZFListenerHolder *listenerHolder = ZFCastZFObjectUnchecked(ZFListenerHolder *, userData);
                listenerHolder->runnableExecute();
            })
            ZFExecuteInMainThread(mainThreadCallback, userData);
        })
        ZFExecuteInNewThread(threadCallback,
            zflineAlloc(ZFListenerHolder, task, ZFListenerData().param0(param0).param1(param1)));
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFThreadTaskRequestImpl_default)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFThreadTaskRequestImpl_default)

ZF_NAMESPACE_GLOBAL_END

