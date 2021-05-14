#include "ZFMethodDynamicRegister.h"
#include "ZFMethodDynamicRegisterExtra.h"
#include "ZFObjectImpl.h"
#include "ZFMethodFuncDeclare.h"

#include "../ZFSTLWrapper/zfstl_map.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFMethodDynamicRegisterDataHolder, ZFLevelZFFrameworkStatic)
{
}
zfstlmap<const ZFMethod *, zfbool> m;
ZF_GLOBAL_INITIALIZER_END(ZFMethodDynamicRegisterDataHolder)

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFMethodDynamicRegisterAutoRemove, ZFLevelZFFrameworkHigh)
{
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFMethodDynamicRegisterAutoRemove)
{
    zfstlmap<const ZFMethod *, zfbool> t;
    t.swap(ZF_GLOBAL_INITIALIZER_INSTANCE(ZFMethodDynamicRegisterDataHolder)->m);
    for(zfstlmap<const ZFMethod *, zfbool>::iterator it = t.begin(); it != t.end(); ++it)
    {
        _ZFP_ZFMethodUnregister(it->first);
    }
}
ZF_GLOBAL_INITIALIZER_END(ZFMethodDynamicRegisterAutoRemove)

// ============================================================
zfclass _ZFP_I_ZFMethodDynamicRegisterGITask : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_I_ZFMethodDynamicRegisterGITask, ZFObject)
public:
    ZFListener methodImpl;
    zfautoObject methodImplUserData;
public:
    static zfautoObject create(ZF_IN const ZFListener &methodImpl, ZF_IN ZFObject *methodImplUserData)
    {
        zfblockedAlloc(_ZFP_I_ZFMethodDynamicRegisterGITask, task);
        task->methodImpl = methodImpl;
        task->methodImplUserData = methodImplUserData;
        return task;
    }
};
static zfbool _ZFP_I_ZFMethodDynamicRegisterGI(ZFMETHOD_GENERIC_INVOKER_PARAMS)
{
    zfblockedAlloc(ZFMethodInvokeData, d);
    d->invokerMethod = invokerMethod;
    d->invokerObject = invokerObject;
    d->param0 = paramList[0];
    d->param1 = paramList[1];
    d->param2 = paramList[2];
    d->param3 = paramList[3];
    d->param4 = paramList[4];
    d->param5 = paramList[5];
    d->param6 = paramList[6];
    d->param7 = paramList[7];

    _ZFP_I_ZFMethodDynamicRegisterGITask *task = ZFCastZFObjectUnchecked(_ZFP_I_ZFMethodDynamicRegisterGITask *, invokerMethod->methodDynamicRegisterUserData());
    task->methodImpl.execute(ZFListenerData().sender(invokerObject).param0(d), task->methodImplUserData);
    ret = d->ret;
    if(errorHint != zfnull)
    {
        *errorHint += d->errorHint;
    }
    if(d->invokeSuccess)
    {
        paramList[0] = d->param0;
        paramList[1] = d->param1;
        paramList[2] = d->param2;
        paramList[3] = d->param3;
        paramList[4] = d->param4;
        paramList[5] = d->param5;
        paramList[6] = d->param6;
        paramList[7] = d->param7;
    }
    return d->invokeSuccess;
}

// ============================================================
const ZFMethod *ZFMethodDynamicRegister(ZF_IN const ZFMethodDynamicRegisterParam &param,
                                        ZF_OUT_OPT zfstring *errorHint /* = zfnull */)
{
    if(param.methodOwnerClass() != zfnull && param.methodNamespace() != zfnull)
    {
        zfstringAppend(errorHint,
            "methodOwnerClass(%s) and methodNamespace(%s) can not both set",
            param.methodOwnerClass()->objectInfo().cString(),
            param.methodNamespace());
        return zfnull;
    }
    zfbool methodImplValid = param.methodImpl().callbackIsValid();
    if(param.methodGenericInvoker() == zfnull && !methodImplValid)
    {
        zfstringAppend(errorHint, "methodGenericInvoker / methodImpl not set");
        return zfnull;
    }
    if(param.methodGenericInvoker() != zfnull && methodImplValid)
    {
        zfstringAppend(errorHint,
            "methodGenericInvoker(%p) and methodImpl(%s) can not both set",
            param.methodGenericInvoker(),
            param.methodImpl().objectInfo().cString());
        return zfnull;
    }
    if(methodImplValid && param.methodDynamicRegisterUserData() != zfnull)
    {
        zfstringAppend(errorHint,
            "when methodImpl specified, methodDynamicRegisterUserData must not set, use methodImplUserData instead");
        return zfnull;
    }
    if(param.methodOwnerClass() == zfnull
        && (param.methodType() != ZFMethodTypeStatic || param.methodPrivilegeType() != ZFMethodPrivilegeTypePublic))
    {
        zfstringAppend(errorHint,
            "function type method must specified as public static, but got %s %s",
            ZFMethodTypeToString(param.methodType()).cString(),
            ZFMethodPrivilegeTypeToString(param.methodPrivilegeType()).cString());
        return zfnull;
    }
    if(param.methodName() == zfnull)
    {
        zfstringAppend(errorHint, "methodName not set");
        return zfnull;
    }
    if(param.methodReturnTypeId() == zfnull)
    {
        zfstringAppend(errorHint, "methodReturnTypeId not set");
        return zfnull;
    }
    for(zfindex i = 0; i < param.methodParamCount(); ++i)
    {
        if(param.methodParamDefaultValueCallbackAtIndex(i) != zfnull)
        {
            for( ; i < param.methodParamCount(); ++i)
            {
                if(param.methodParamDefaultValueCallbackAtIndex(i) == zfnull)
                {
                    zfstringAppend(errorHint,
                        "param %zi has no default value by previous param has",
                        i);
                }
            }
            break;
        }
    }
    const ZFMethod *existMethod = zfnull;
    if(param.methodOwnerClass() != zfnull)
    {
        existMethod = param.methodOwnerClass()->methodForNameIgnoreParent(
                param.methodName()
                , param.methodParamTypeIdAtIndex(0)
                , param.methodParamTypeIdAtIndex(1)
                , param.methodParamTypeIdAtIndex(2)
                , param.methodParamTypeIdAtIndex(3)
                , param.methodParamTypeIdAtIndex(4)
                , param.methodParamTypeIdAtIndex(5)
                , param.methodParamTypeIdAtIndex(6)
                , param.methodParamTypeIdAtIndex(7)
            );
    }
    else
    {
        existMethod = ZFMethodForName(
                param.methodNamespace()
                , param.methodName()
                , param.methodParamTypeIdAtIndex(0)
                , param.methodParamTypeIdAtIndex(1)
                , param.methodParamTypeIdAtIndex(2)
                , param.methodParamTypeIdAtIndex(3)
                , param.methodParamTypeIdAtIndex(4)
                , param.methodParamTypeIdAtIndex(5)
                , param.methodParamTypeIdAtIndex(6)
                , param.methodParamTypeIdAtIndex(7)
            );
    }
    if(existMethod != zfnull)
    {
        zfstringAppend(errorHint,
            "method with same sig already exists: %s",
            existMethod->objectInfo().cString());
        return zfnull;
    }

    const zfchar *methodType = "";
    switch(param.methodType())
    {
        case ZFMethodTypeNormal:
            methodType = _ZFP_ZFMethodTypeText(ZFMethodTypeNormal);
            break;
        case ZFMethodTypeStatic:
            methodType = _ZFP_ZFMethodTypeText(ZFMethodTypeStatic);
            break;
        case ZFMethodTypeVirtual:
            methodType = _ZFP_ZFMethodTypeText(ZFMethodTypeVirtual);
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            break;
    }

    zfCoreMutexLocker();
    const ZFMethod *method = _ZFP_ZFMethodRegister(zffalse
            , zftrue
            , methodImplValid
                ? _ZFP_I_ZFMethodDynamicRegisterGITask::create(param.methodImpl(), param.methodImplUserData()).toObject()
                : param.methodDynamicRegisterUserData()
            , zfnull
            , methodImplValid ? _ZFP_I_ZFMethodDynamicRegisterGI : param.methodGenericInvoker()
            , methodType
            , param.methodOwnerClass()
            , param.methodPrivilegeType()
            , param.methodNamespace()
            , param.methodName()
            , param.methodReturnTypeId()
            , param.methodReturnTypeName()
            , param.methodParamTypeIdAtIndex(0), param.methodParamTypeNameAtIndex(0), param.methodParamNameAtIndex(0), param.methodParamDefaultValueCallbackAtIndex(0)
            , param.methodParamTypeIdAtIndex(1), param.methodParamTypeNameAtIndex(1), param.methodParamNameAtIndex(1), param.methodParamDefaultValueCallbackAtIndex(1)
            , param.methodParamTypeIdAtIndex(2), param.methodParamTypeNameAtIndex(2), param.methodParamNameAtIndex(2), param.methodParamDefaultValueCallbackAtIndex(2)
            , param.methodParamTypeIdAtIndex(3), param.methodParamTypeNameAtIndex(3), param.methodParamNameAtIndex(3), param.methodParamDefaultValueCallbackAtIndex(3)
            , param.methodParamTypeIdAtIndex(4), param.methodParamTypeNameAtIndex(4), param.methodParamNameAtIndex(4), param.methodParamDefaultValueCallbackAtIndex(4)
            , param.methodParamTypeIdAtIndex(5), param.methodParamTypeNameAtIndex(5), param.methodParamNameAtIndex(5), param.methodParamDefaultValueCallbackAtIndex(5)
            , param.methodParamTypeIdAtIndex(6), param.methodParamTypeNameAtIndex(6), param.methodParamNameAtIndex(6), param.methodParamDefaultValueCallbackAtIndex(6)
            , param.methodParamTypeIdAtIndex(7), param.methodParamTypeNameAtIndex(7), param.methodParamNameAtIndex(7), param.methodParamDefaultValueCallbackAtIndex(7)
            , zfnull
        );
    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFMethodDynamicRegisterDataHolder)->m[method] = zftrue;
    return method;
}
void ZFMethodDynamicUnregister(ZF_IN const ZFMethod *method)
{
    if(method != zfnull)
    {
        zfCoreAssert(method->methodIsDynamicRegister());
        zfCoreMutexLocker();
        ZF_GLOBAL_INITIALIZER_INSTANCE(ZFMethodDynamicRegisterDataHolder)->m.erase(method);
        _ZFP_ZFMethodUnregister(method);
    }
}

ZF_NAMESPACE_GLOBAL_END

