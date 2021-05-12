#include "ZFMethodGenericInvoker.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN
/* ZFMETHOD_MAX_PARAM */

// ============================================================
zfclass _ZFP_I_ZFMethodGenericInvokerDefaultParamType : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_I_ZFMethodGenericInvokerDefaultParamType, ZFObject)
};
ZFObject *_ZFP_ZFMethodGenericInvokerDefaultParamRef = zfnull;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFMethodGenericInvokerDefaultParamInit, ZFLevelZFFrameworkEssential)
{
    _ZFP_ZFMethodGenericInvokerDefaultParamRef = zfAlloc(_ZFP_I_ZFMethodGenericInvokerDefaultParamType);
    _ZFP_ZFMethodGenericInvokerDefaultParamHolderRef = _ZFP_ZFMethodGenericInvokerDefaultParamRef;
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFMethodGenericInvokerDefaultParamInit)
{
    zfRelease(_ZFP_ZFMethodGenericInvokerDefaultParamRef);
    _ZFP_ZFMethodGenericInvokerDefaultParamRef = zfnull;

    _ZFP_ZFMethodGenericInvokerDefaultParamHolderRef = zfnull;
}
ZF_GLOBAL_INITIALIZER_END(ZFMethodGenericInvokerDefaultParamInit)

zfautoObject _ZFP_ZFMethodGenericInvokerDefaultParamHolderRef;

zfbool _ZFP_MtdGIParamCheck(ZF_OUT_OPT zfstring *errorHint,
                            ZF_IN zfbool zfvAccessAvailable,
                            ZF_IN const ZFMethod *invokerMethod,
                            ZF_IN zfindex paramIndex,
                            ZF_IN const zfchar *paramType,
                            ZF_IN ZFObject *param)
{
    if((param != ZFMethodGenericInvokerDefaultParam() && !zfvAccessAvailable)
        || (param == ZFMethodGenericInvokerDefaultParam() && paramIndex < invokerMethod->methodParamDefaultBeginIndex()))
    {
        zfstringAppend(errorHint,
                "unable to access param%zi as type (%s): (%s)%s",
                paramIndex,
                paramType,
                param != zfnull ? param->classData()->className() : ZFTOKEN_zfnull,
                ZFObjectInfo(param).cString()
            );
        return zffalse;
    }
    else
    {
        return zftrue;
    }
}
void _ZFP_MtdGIRetError(ZF_OUT_OPT zfstring *errorHint,
                        ZF_IN const zfchar *returnTypeId,
                        ZF_IN const zfchar *returnValueInfo)
{
    zfstringAppend(errorHint,
        "unable to convert return value as type %s: %s",
        returnTypeId,
        returnValueInfo);
}

// ============================================================
zfbool _ZFP_ZFMethodGenericInvoke(ZF_IN const ZFMethod *invokerMethod
                                  , ZF_IN ZFObject *invokerObject
                                  , ZF_OUT_OPT zfstring *errorHint
                                  , ZF_OUT zfautoObject &ret
                                  , ZF_IN_OUT zfautoObject (&paramList)[ZFMETHOD_MAX_PARAM]
                                  )
{
    return invokerMethod->methodGenericInvoker()(
            invokerMethod
            , invokerObject
            , errorHint
            , ret
            , paramList
        );
}
void _ZFP_ZFMethodGenericInvokeError(ZF_IN const ZFMethod *method,
                                     ZF_IN ZFObject *obj,
                                     ZF_IN zfint pos,
                                     ZF_IN_OPT const zfchar *errorHint /* = zfnull */)
{
    if(pos >= 0)
    {
        zfCoreCriticalMessageTrim(
            "[ZFMethodDynamicRegister] method %s unable to convert param %d",
            method->objectInfo().cString(),
            pos);
    }
    else if(pos == -1)
    {
        zfCoreCriticalMessageTrim(
            "[ZFMethodDynamicRegister] method %s unable to perform generic invoker, reason: %s",
            method->objectInfo().cString(),
            errorHint);
    }
    else
    {
        zfCoreCriticalMessageTrim(
            "[ZFMethodDynamicRegister] method %s unable to convert return value",
            method->objectInfo().cString());
    }
}

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_USER_REGISTER_0({return ZFMethodGenericInvokerDefaultParam();}, ZFObject *, ZFMethodGenericInvokerDefaultParam)

ZF_NAMESPACE_GLOBAL_END
#endif

