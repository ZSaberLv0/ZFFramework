#include "ZFMethodDynamicRegister.h"
#include "ZFMethodDynamicRegisterExtra.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFOBJECT_REGISTER(ZFMethodInvokeData)

ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFMethodInvokeData, zfbool, invokeSuccess)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFMethodInvokeData, const ZFMethod *, invokerMethod)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFMethodInvokeData, ZFObject *, invokerObject)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFMethodInvokeData, zfstring, errorHint)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFMethodInvokeData, zfautoObject, ret)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFMethodInvokeData, zfautoObject, param0)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFMethodInvokeData, zfautoObject, param1)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFMethodInvokeData, zfautoObject, param2)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFMethodInvokeData, zfautoObject, param3)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFMethodInvokeData, zfautoObject, param4)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFMethodInvokeData, zfautoObject, param5)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFMethodInvokeData, zfautoObject, param6)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFMethodInvokeData, zfautoObject, param7)

ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFMethodInvokeData, void, callSuper)

void ZFMethodInvokeData::CallSuper(ZFMETHOD_GENERIC_INVOKER_PARAMS)
{
    zfCoreAssertWithMessage(invokerMethod->methodIsDynamicRegister(),
        "ZFMethodInvokeData::callSuper() only works for dynamic registered method");
    zfCoreAssertWithMessage(invokerMethod->methodOwnerClass() != zfnull && invokerMethod->methodType() == ZFMethodTypeVirtual,
        "ZFMethodInvokeData::callSuper() only works for class virtual method");
    ZFCoreArrayPOD<const ZFMethod *> allMethods;
    invokerMethod->methodOwnerClass()->methodForNameGetAllT(allMethods, invokerMethod->methodName());

    zfautoObject dummyRet;
    zfbool hasCalledNonDynamicMethod = zffalse;
    for(zfindex i = 1; i < allMethods.count(); ++i)
    {
        const ZFMethod *m = allMethods[i];
        if(m->methodParamTypeIdIsMatch(invokerMethod))
        {
            if(!m->methodIsDynamicRegister())
            {
                if(hasCalledNonDynamicMethod)
                {
                    continue;
                }
                else
                {
                    hasCalledNonDynamicMethod = zftrue;
                }
            }
            m->methodGenericInvoker()(m, invokerObject, zfnull, dummyRet, paramList);
        }
    }
}
void ZFMethodInvokeData::callSuper(void)
{
    zfCoreAssertWithMessage(invokerMethod->methodIsDynamicRegister(),
        "ZFMethodInvokeData::callSuper() only works for dynamic registered method");
    zfCoreAssertWithMessage(invokerMethod->methodOwnerClass() != zfnull && invokerMethod->methodType() == ZFMethodTypeVirtual,
        "ZFMethodInvokeData::callSuper() only works for class virtual method");
    ZFCoreArrayPOD<const ZFMethod *> allMethods;
    invokerMethod->methodOwnerClass()->methodForNameGetAllT(allMethods, invokerMethod->methodName());

    zfbool hasCalledNonDynamicMethod = zffalse;
    for(zfindex i = 1; i < allMethods.count(); ++i)
    {
        const ZFMethod *m = allMethods[i];
        if(m->methodParamTypeIdIsMatch(invokerMethod))
        {
            if(!m->methodIsDynamicRegister())
            {
                if(hasCalledNonDynamicMethod)
                {
                    continue;
                }
                else
                {
                    hasCalledNonDynamicMethod = zftrue;
                }
            }
            m->methodGenericInvoke(invokerObject
                    , param0
                    , param1
                    , param2
                    , param3
                    , param4
                    , param5
                    , param6
                    , param7
                );
        }
    }
}

// ============================================================
zfclassNotPOD _ZFP_ZFMethodDynamicRegisterParamPrivate
{
public:
    zfuint refCount;
    zfautoObject methodDynamicRegisterUserData;
    const ZFClass *methodOwnerClass;
    zfstring methodNamespace;
    ZFMethodGenericInvoker methodGenericInvoker;
    ZFListener methodImpl;
    zfautoObject methodImplUserData;
    ZFMethodType methodType;
    ZFMethodPrivilegeType methodPrivilegeType;
    zfstring methodName;
    zfstring methodReturnTypeId;
    zfstring methodReturnTypeName;
    zfindex methodParamCount;
    zfstring methodParamTypeId[ZFMETHOD_MAX_PARAM];
    zfstring methodParamTypeName[ZFMETHOD_MAX_PARAM];
    zfstring methodParamName[ZFMETHOD_MAX_PARAM];
    ZFMethodParamDefaultValueCallback methodParamDefaultValueCallback[ZFMETHOD_MAX_PARAM];
    zfautoObject methodParamDefaultValue[ZFMETHOD_MAX_PARAM];

public:
    _ZFP_ZFMethodDynamicRegisterParamPrivate(void)
    : refCount(1)
    , methodDynamicRegisterUserData()
    , methodOwnerClass(zfnull)
    , methodNamespace()
    , methodGenericInvoker(zfnull)
    , methodImpl()
    , methodImplUserData()
    , methodType(ZFMethodTypeStatic)
    , methodPrivilegeType(ZFMethodPrivilegeTypePublic)
    , methodName()
    , methodReturnTypeId(ZFTypeId_void())
    , methodReturnTypeName()
    , methodParamCount(0)
    , methodParamTypeId()
    , methodParamTypeName()
    , methodParamName()
    , methodParamDefaultValueCallback()
    , methodParamDefaultValue()
    {
    }
};

// ============================================================
ZFMethodDynamicRegisterParam &ZFMethodDynamicRegisterParam::methodDynamicRegisterUserData(ZF_IN ZFObject *methodDynamicRegisterUserData)
{
    d->methodDynamicRegisterUserData = methodDynamicRegisterUserData;
    return *this;
}
ZFObject *ZFMethodDynamicRegisterParam::methodDynamicRegisterUserData(void) const
{
    return d->methodDynamicRegisterUserData;
}

ZFMethodDynamicRegisterParam &ZFMethodDynamicRegisterParam::methodOwnerClass(ZF_IN const ZFClass *methodOwnerClass)
{
    d->methodOwnerClass = methodOwnerClass;
    return *this;
}
const ZFClass *ZFMethodDynamicRegisterParam::methodOwnerClass(void) const
{
    return d->methodOwnerClass;
}

ZFMethodDynamicRegisterParam &ZFMethodDynamicRegisterParam::methodNamespace(ZF_IN const zfchar *methodNamespace)
{
    d->methodNamespace = methodNamespace;
    return *this;
}
const zfchar *ZFMethodDynamicRegisterParam::methodNamespace(void) const
{
    return (d->methodNamespace.isEmpty() ? zfnull : d->methodNamespace.cString());
}

ZFMethodDynamicRegisterParam &ZFMethodDynamicRegisterParam::methodGenericInvoker(ZF_IN ZFMethodGenericInvoker methodGenericInvoker)
{
    d->methodGenericInvoker = methodGenericInvoker;
    return *this;
}
ZFMethodGenericInvoker ZFMethodDynamicRegisterParam::methodGenericInvoker(void) const
{
    return d->methodGenericInvoker;
}

ZFMethodDynamicRegisterParam &ZFMethodDynamicRegisterParam::methodImpl(ZF_IN const ZFListener &methodImpl)
{
    d->methodImpl = methodImpl;
    return *this;
}
const ZFListener &ZFMethodDynamicRegisterParam::methodImpl(void) const
{
    return d->methodImpl;
}

ZFMethodDynamicRegisterParam &ZFMethodDynamicRegisterParam::methodImplUserData(ZF_IN ZFObject *methodImplUserData)
{
    d->methodImplUserData = methodImplUserData;
    return *this;
}
ZFObject *ZFMethodDynamicRegisterParam::methodImplUserData(void) const
{
    return d->methodImplUserData;
}

ZFMethodDynamicRegisterParam &ZFMethodDynamicRegisterParam::methodType(ZF_IN ZFMethodType methodType)
{
    d->methodType = methodType;
    return *this;
}
ZFMethodType ZFMethodDynamicRegisterParam::methodType(void) const
{
    return d->methodType;
}

ZFMethodDynamicRegisterParam &ZFMethodDynamicRegisterParam::methodPrivilegeType(ZF_IN ZFMethodPrivilegeType methodPrivilegeType)
{
    d->methodPrivilegeType = methodPrivilegeType;
    return *this;
}
ZFMethodPrivilegeType ZFMethodDynamicRegisterParam::methodPrivilegeType(void) const
{
    return d->methodPrivilegeType;
}

ZFMethodDynamicRegisterParam &ZFMethodDynamicRegisterParam::methodName(ZF_IN const zfchar *methodName)
{
    d->methodName = methodName;
    return *this;
}
const zfchar *ZFMethodDynamicRegisterParam::methodName(void) const
{
    return (d->methodName.isEmpty() ? zfnull : d->methodName.cString());
}

ZFMethodDynamicRegisterParam &ZFMethodDynamicRegisterParam::methodReturnTypeId(ZF_IN const zfchar *methodReturnTypeId)
{
    d->methodReturnTypeId = methodReturnTypeId;
    return *this;
}
const zfchar *ZFMethodDynamicRegisterParam::methodReturnTypeId(void) const
{
    return (d->methodReturnTypeId.isEmpty() ? zfnull : d->methodReturnTypeId.cString());
}

ZFMethodDynamicRegisterParam &ZFMethodDynamicRegisterParam::methodReturnTypeName(ZF_IN const zfchar *methodReturnTypeName)
{
    d->methodReturnTypeName = methodReturnTypeName;
    return *this;
}
const zfchar *ZFMethodDynamicRegisterParam::methodReturnTypeName(void) const
{
    return (d->methodReturnTypeName.isEmpty() ? d->methodReturnTypeId.cString() : d->methodReturnTypeName.cString());
}

ZFMethodDynamicRegisterParam &ZFMethodDynamicRegisterParam::methodParamAdd(ZF_IN const zfchar *methodParamTypeId,
                                                                           ZF_IN_OPT const zfchar *methodParamTypeName /* = zfnull */,
                                                                           ZF_IN_OPT const zfchar *methodParamName /* = zfnull */,
                                                                           ZF_IN_OPT ZFMethodParamDefaultValueCallback methodParamDefaultValueCallback /* = zfnull */)
{
    zfCoreAssert(d->methodParamCount < ZFMETHOD_MAX_PARAM);
    if(methodParamTypeId != zfnull)
    {
        d->methodParamTypeId[d->methodParamCount] = methodParamTypeId;
        d->methodParamTypeName[d->methodParamCount] = methodParamTypeName;
        if(!zfsIsEmpty(methodParamName))
        {
            d->methodParamName[d->methodParamCount] = methodParamName;
        }
        else
        {
            d->methodParamName[d->methodParamCount] = zfstringWithFormat("p%zi", d->methodParamCount);
        }
        d->methodParamDefaultValueCallback[d->methodParamCount] = methodParamDefaultValueCallback;
        ++(d->methodParamCount);
    }
    return *this;
}
static zfautoObject _ZFP_ZFMethodDynamicRegisterParamDefaultGetter(ZF_IN const ZFMethod *invokerMethod, ZF_IN zfindex paramIndex)
{
    return invokerMethod->_ZFP_ZFMethod_paramDefaultValueList[paramIndex];
}
ZFMethodDynamicRegisterParam &ZFMethodDynamicRegisterParam::methodParamAddWithDefault(ZF_IN const zfchar *methodParamTypeId,
                                                                                      ZF_IN_OPT const zfchar *methodParamTypeName /* = zfnull */,
                                                                                      ZF_IN_OPT const zfchar *methodParamName /* = zfnull */,
                                                                                      ZF_IN_OPT ZFObject *methodParamDefaultValue /* = zfnull */)
{
    d->methodParamDefaultValue[d->methodParamCount] = methodParamDefaultValue;
    this->methodParamAdd(methodParamTypeId, methodParamTypeName, methodParamName, _ZFP_ZFMethodDynamicRegisterParamDefaultGetter);
    return *this;
}
zfindex ZFMethodDynamicRegisterParam::methodParamCount(void) const
{
    return d->methodParamCount;
}
const zfchar *ZFMethodDynamicRegisterParam::methodParamTypeIdAtIndex(ZF_IN zfindex index) const
{
    return (index < d->methodParamCount ? d->methodParamTypeId[index].cString() : zfnull);
}
const zfchar *ZFMethodDynamicRegisterParam::methodParamTypeNameAtIndex(ZF_IN zfindex index) const
{
    if(index < d->methodParamCount)
    {
        if(d->methodParamTypeName[index].isEmpty())
        {
            return d->methodParamTypeId[index].cString();
        }
        else
        {
            return d->methodParamTypeName[index].cString();
        }
    }
    else
    {
        return zfnull;
    }
}
const zfchar *ZFMethodDynamicRegisterParam::methodParamNameAtIndex(ZF_IN zfindex index) const
{
    return (index < d->methodParamCount ? d->methodParamName[index].cString() : zfnull);
}
ZFMethodParamDefaultValueCallback ZFMethodDynamicRegisterParam::methodParamDefaultValueCallbackAtIndex(ZF_IN zfindex index) const
{
    return (index < d->methodParamCount ? d->methodParamDefaultValueCallback[index] : zfnull);
}
ZFObject *ZFMethodDynamicRegisterParam::methodParamDefaultValueAtIndex(ZF_IN zfindex index) const
{
    return (index < d->methodParamCount ? d->methodParamDefaultValue[index].toObject() : zfnull);
}

ZFMethodDynamicRegisterParam::ZFMethodDynamicRegisterParam(void)
: d(zfnew(_ZFP_ZFMethodDynamicRegisterParamPrivate))
{
}
ZFMethodDynamicRegisterParam::ZFMethodDynamicRegisterParam(ZF_IN ZFMethodDynamicRegisterParam const &ref)
: d(ref.d)
{
    ++(d->refCount);
}
ZFMethodDynamicRegisterParam::~ZFMethodDynamicRegisterParam(void)
{
    --(d->refCount);
    if(d->refCount == 0)
    {
        zfdelete(d);
    }
}
ZFMethodDynamicRegisterParam &ZFMethodDynamicRegisterParam::operator = (ZF_IN ZFMethodDynamicRegisterParam const &ref)
{
    ++(ref.d->refCount);
    --(d->refCount);
    if(d->refCount == 0)
    {
        zfdelete(d);
    }
    d = ref.d;
    return *this;
}
zfbool ZFMethodDynamicRegisterParam::operator == (ZF_IN ZFMethodDynamicRegisterParam const &ref) const
{
    return (d == ref.d);
}

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(const ZFMethod *, ZFMethodDynamicRegister, ZFMP_IN(const ZFMethodDynamicRegisterParam &, param), ZFMP_OUT_OPT(zfstring *, errorHint, zfnull))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(void, ZFMethodDynamicUnregister, ZFMP_IN(const ZFMethod *, method))

ZF_NAMESPACE_GLOBAL_END
#endif

