#include "ZFImpl_sys_iOS_ZFCore_impl.h"
#include "ZFCore/protocol/ZFProtocolZFThreadTaskRequest.h"

#if ZF_ENV_sys_iOS

// ============================================================
// global type
@interface _ZFP_ZFThreadTaskRequestImpl_sys_iOS_TaskHolder : NSObject
@property (nonatomic, assign) ZFListener task;
@property (nonatomic, assign) ZFObject *param0;
@property (nonatomic, assign) ZFObject *param1;
- (void)taskCallback;
@end
@implementation _ZFP_ZFThreadTaskRequestImpl_sys_iOS_TaskHolder
- (void)setParam0:(ZFObject *)newParam0 ZFImpl_sys_iOS_overrideProperty
{
    zfRetainChange(self->_param0, newParam0);
}
- (void)setParam1:(ZFObject *)newParam1 ZFImpl_sys_iOS_overrideProperty
{
    zfRetainChange(self->_param1, newParam1);
}
- (void)taskCallback
{
    if(self.task.callbackIsValid())
    {
        self.task.execute(ZFListenerData().param0(self.param0).param1(self.param1));
    }

    self.param0 = zfnull;
    self.param1 = zfnull;
}
@end

ZF_NAMESPACE_GLOBAL_BEGIN

static NSMutableArray *_ZFP_ZFThreadTaskRequestImpl_sys_iOS_taskHolders = nil;
// ============================================================
ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFThreadTaskRequestImpl_sys_iOS, ZFThreadTaskRequest, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("iOS:GrandCentralDispatch")

public:
    zfoverride
    virtual void protocolOnInit(void)
    {
        zfsuper::protocolOnInit();
        _ZFP_ZFThreadTaskRequestImpl_sys_iOS_taskHolders = [NSMutableArray new];
    }
    zfoverride
    virtual void protocolOnDealloc(void)
    {
        for(NSUInteger i = 0; i < [_ZFP_ZFThreadTaskRequestImpl_sys_iOS_taskHolders count]; ++i)
        {
            _ZFP_ZFThreadTaskRequestImpl_sys_iOS_TaskHolder *taskHolder = [_ZFP_ZFThreadTaskRequestImpl_sys_iOS_taskHolders objectAtIndex:i];
            taskHolder.task = ZFCallbackNull();
            taskHolder.param0 = zfnull;
            taskHolder.param1 = zfnull;
        }
        _ZFP_ZFThreadTaskRequestImpl_sys_iOS_taskHolders = nil;
        zfsuper::protocolOnDealloc();
    }
public:
    virtual void taskRequest(ZF_IN const ZFListener &task,
                             ZF_IN ZFObject *param0,
                             ZF_IN ZFObject *param1)
    {
        _ZFP_ZFThreadTaskRequestImpl_sys_iOS_TaskHolder *taskHolder = [_ZFP_ZFThreadTaskRequestImpl_sys_iOS_TaskHolder new];
        taskHolder.task = task;
        taskHolder.param0 = param0;
        taskHolder.param1 = param1;
        [_ZFP_ZFThreadTaskRequestImpl_sys_iOS_taskHolders addObject:taskHolder];

        dispatch_async(dispatch_get_main_queue(), ^{
            if([_ZFP_ZFThreadTaskRequestImpl_sys_iOS_taskHolders containsObject:taskHolder])
            {
                [_ZFP_ZFThreadTaskRequestImpl_sys_iOS_taskHolders removeObject:taskHolder];
                [taskHolder taskCallback];
            }
        });
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFThreadTaskRequestImpl_sys_iOS)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFThreadTaskRequestImpl_sys_iOS)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_iOS

