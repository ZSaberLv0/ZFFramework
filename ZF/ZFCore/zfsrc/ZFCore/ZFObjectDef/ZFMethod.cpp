#include "ZFMethod.h"
#include "ZFObjectImpl.h"

#include "ZFMethodDeclare.h"
#include "ZFMethodUserRegister.h"

#include "ZFMethodFuncDeclare.h"
#include "ZFMethodFuncUserRegister.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
void ZFMethod::_ZFP_ZFMethod_init(ZF_IN zfbool methodIsUserRegister,
                                  ZF_IN zfbool methodIsDynamicRegister,
                                  ZF_IN ZFObject *methodDynamicRegisterUserData,
                                  ZF_IN ZFFuncAddrType invoker,
                                  ZF_IN ZFMethodGenericInvoker methodGenericInvoker,
                                  ZF_IN const zfchar *methodType,
                                  ZF_IN const zfchar *methodName,
                                  ZF_IN const zfchar *returnTypeId,
                                  ZF_IN const zfchar *returnTypeName,
                                  ...)
{
    zfCoreAssert(methodGenericInvoker != zfnull);

    this->_ZFP_ZFMethod_methodIsUserRegister = methodIsUserRegister;
    this->_ZFP_ZFMethod_methodIsDynamicRegister = methodIsDynamicRegister;
    this->_ZFP_ZFMethod_methodDynamicRegisterUserData = zfRetain(methodDynamicRegisterUserData);
    this->_ZFP_ZFMethod_invoker = invoker;
    this->_ZFP_ZFMethod_invokerOrg = invoker;
    this->_ZFP_ZFMethod_methodGenericInvoker = methodGenericInvoker;
    this->_ZFP_ZFMethod_methodGenericInvokerOrg = methodGenericInvoker;
    this->_ZFP_ZFMethod_methodName = methodName;
    this->_ZFP_ZFMethod_returnTypeId = returnTypeId;
    this->_ZFP_ZFMethod_returnTypeName = returnTypeName;
    this->_ZFP_ZFMethod_paramCount = 0;

    if(zfstringFind(methodType, _ZFP_ZFMethodTypeText(ZFMethodTypeStatic)) != zfindexMax())
    {
        this->_ZFP_ZFMethod_methodType = ZFMethodTypeStatic;
    }
    else if(zfstringFind(methodType, _ZFP_ZFMethodTypeText(ZFMethodTypeVirtual)) != zfindexMax())
    {
        this->_ZFP_ZFMethod_methodType = ZFMethodTypeVirtual;
    }
    else
    {
        this->_ZFP_ZFMethod_methodType = ZFMethodTypeNormal;
    }

    va_list vaList;
    va_start(vaList, returnTypeName);
    do
    {
        const zfchar *paramTypeId = va_arg(vaList, const zfchar *);
        if(paramTypeId == zfnull || paramTypeId[0] == '\0')
        {
            break;
        }
        this->_ZFP_ZFMethod_paramTypeIdList[this->_ZFP_ZFMethod_paramCount] = paramTypeId;
        this->_ZFP_ZFMethod_paramTypeNameList[this->_ZFP_ZFMethod_paramCount] = va_arg(vaList, const zfchar *);

        ZFMethodParamDefaultValueCallback paramDefaultValueCallback = va_arg(vaList, ZFMethodParamDefaultValueCallback);
        if(paramDefaultValueCallback != zfnull)
        {
            if(this->_ZFP_ZFMethod_paramDefaultBeginIndex == zfindexMax())
            {
                this->_ZFP_ZFMethod_paramDefaultBeginIndex = this->_ZFP_ZFMethod_paramCount;
            }
            this->_ZFP_ZFMethod_paramDefaultValueCallbackList[this->_ZFP_ZFMethod_paramCount] = paramDefaultValueCallback;
        }

        ++(this->_ZFP_ZFMethod_paramCount);
    } while(zftrue);
    va_end(vaList);

    if(_ZFP_ZFMethod_paramDefaultBeginIndex == zfindexMax())
    {
        _ZFP_ZFMethod_paramCountMin = _ZFP_ZFMethod_paramCount;
    }
    else
    {
        _ZFP_ZFMethod_paramCountMin = _ZFP_ZFMethod_paramDefaultBeginIndex;
    }
}
void ZFMethod::_ZFP_ZFMethod_initClassMemberType(ZF_IN const ZFClass *methodOwnerClass,
                                                 ZF_IN ZFMethodPrivilegeType privilegeType)
{
    this->_ZFP_ZFMethod_methodOwnerClass = methodOwnerClass;
    this->_ZFP_ZFMethod_privilegeType = privilegeType;
}
void ZFMethod::_ZFP_ZFMethod_initFuncType(ZF_IN const zfchar *methodNamespace)
{
    if(!zfscmpTheSame(methodNamespace, ZF_NAMESPACE_GLOBAL_NAME)
        && !zfscmpTheSame(methodNamespace, ZF_NAMESPACE_GLOBAL_ABBR_NAME))
    {
        this->_ZFP_ZFMethod_methodNamespace = methodNamespace;
    }

    this->_ZFP_ZFMethod_methodOwnerClass = zfnull;
    this->_ZFP_ZFMethod_privilegeType = ZFMethodPrivilegeTypePublic;
}

ZFMethod::ZFMethod(void)
: _ZFP_ZFMethodNeedInit(zftrue)
, _ZFP_ZFMethod_methodIsUserRegister(zffalse)
, _ZFP_ZFMethod_methodIsDynamicRegister(zffalse)
, _ZFP_ZFMethod_methodDynamicRegisterUserData(zfnull)
, _ZFP_ZFMethod_invoker(zfnull)
, _ZFP_ZFMethod_invokerOrg(zfnull)
, _ZFP_ZFMethod_methodGenericInvoker(zfnull)
, _ZFP_ZFMethod_methodGenericInvokerOrg(zfnull)
, _ZFP_ZFMethod_methodName()
, _ZFP_ZFMethod_returnTypeId()
, _ZFP_ZFMethod_returnTypeName()
, _ZFP_ZFMethod_paramCount(0)
, _ZFP_ZFMethod_paramCountMin(0)
, _ZFP_ZFMethod_paramTypeIdList()
, _ZFP_ZFMethod_paramTypeNameList()
, _ZFP_ZFMethod_paramDefaultValueCallbackList()
, _ZFP_ZFMethod_paramDefaultBeginIndex(zfindexMax())
, _ZFP_ZFMethod_methodOwnerClass(zfnull)
, _ZFP_ZFMethod_methodOwnerProperty(zfnull)
, _ZFP_ZFMethod_privilegeType(ZFMethodPrivilegeTypePublic)
, _ZFP_ZFMethod_methodType(ZFMethodTypeNormal)
, _ZFP_ZFMethod_methodNamespace()
{
}
ZFMethod::~ZFMethod(void)
{
    zfRelease(this->_ZFP_ZFMethod_methodDynamicRegisterUserData);
}

void ZFMethod::objectInfoT(ZF_IN_OUT zfstring &ret) const
{
    switch(this->methodPrivilegeType())
    {
        case ZFMethodPrivilegeTypePublic:
            break;
        case ZFMethodPrivilegeTypeProtected:
            ret += ZFTOKEN_ZFMethodPrivilegeTypeProtected;
            ret += ": ";
            break;
        case ZFMethodPrivilegeTypePrivate:
            ret += ZFTOKEN_ZFMethodPrivilegeTypePrivate;
            ret += ": ";
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            break;
    }

    if(this->methodType() == ZFMethodTypeStatic)
    {
        ret += "static ";
    }
    else if(this->methodType() == ZFMethodTypeVirtual)
    {
        ret += "virtual ";
    }

    ret += this->methodReturnTypeName();
    ret += " ";

    if(this->methodIsFunctionType())
    {
        ret += this->methodNamespace();
        ret += "::";
    }
    else
    {
        ret += this->methodOwnerClass()->classNameFull();
        ret += "::";
    }

    ret += this->methodName();

    if(this->methodParamCount() > 0)
    {
        ret += "(";
        for(zfindex i = 0; i < this->methodParamCount(); ++i)
        {
            if(i != 0)
            {
                ret += ", ";
            }
            ret += this->methodParamTypeNameAtIndex(i);
            zfstringAppend(ret, " p%zi", i);
            if(i >= this->methodParamDefaultBeginIndex())
            {
                ret += " = ";
                zfautoObject v = this->methodParamDefaultValueAtIndex(i);
                if(v == zfnull)
                {
                    ret += ZFTOKEN_zfnull;
                }
                else
                {
                    v.toObject()->objectInfoT(ret);
                }
            }
        }
        ret += ")";
    }
    else
    {
        ret += "(void)";
    }
}

zfbool ZFMethod::methodParamTypeIdIsMatch(
                                            ZF_IN_OPT const zfchar *methodParamTypeId0 /* = zfnull */
                                          , ZF_IN_OPT const zfchar *methodParamTypeId1 /* = zfnull */
                                          , ZF_IN_OPT const zfchar *methodParamTypeId2 /* = zfnull */
                                          , ZF_IN_OPT const zfchar *methodParamTypeId3 /* = zfnull */
                                          , ZF_IN_OPT const zfchar *methodParamTypeId4 /* = zfnull */
                                          , ZF_IN_OPT const zfchar *methodParamTypeId5 /* = zfnull */
                                          , ZF_IN_OPT const zfchar *methodParamTypeId6 /* = zfnull */
                                          , ZF_IN_OPT const zfchar *methodParamTypeId7 /* = zfnull */
                                          ) const
{
    #define _ZFP_ZFMethodParamCheck_paramLoop(N) \
        if(zfsIsEmpty(methodParamTypeId##N) || zfscmpTheSame(methodParamTypeId##N, ZFTypeId_void())) {return (this->methodParamCount() == N);} \
        if(this->methodParamCount() <= N || !zfscmpTheSame(this->methodParamTypeIdAtIndex(N), methodParamTypeId##N)) {return zffalse;}
    _ZFP_ZFMethodParamCheck_paramLoop(0)
    _ZFP_ZFMethodParamCheck_paramLoop(1)
    _ZFP_ZFMethodParamCheck_paramLoop(2)
    _ZFP_ZFMethodParamCheck_paramLoop(3)
    _ZFP_ZFMethodParamCheck_paramLoop(4)
    _ZFP_ZFMethodParamCheck_paramLoop(5)
    _ZFP_ZFMethodParamCheck_paramLoop(6)
    _ZFP_ZFMethodParamCheck_paramLoop(7)
    #undef _ZFP_ZFMethodParamCheck_paramLoop
    return zftrue;
}
zfbool ZFMethod::methodParamTypeIdIsMatch(ZF_IN const ZFMethod *method) const
{
    if(method->methodParamCount() != this->methodParamCount()
        || method->methodParamDefaultBeginIndex() != this->methodParamDefaultBeginIndex())
    {
        return zffalse;
    }
    return this->methodParamTypeIdIsMatch(
            method->_ZFP_ZFMethod_paramTypeIdList[0]
            , method->_ZFP_ZFMethod_paramTypeIdList[1]
            , method->_ZFP_ZFMethod_paramTypeIdList[2]
            , method->_ZFP_ZFMethod_paramTypeIdList[3]
            , method->_ZFP_ZFMethod_paramTypeIdList[4]
            , method->_ZFP_ZFMethod_paramTypeIdList[5]
            , method->_ZFP_ZFMethod_paramTypeIdList[6]
            , method->_ZFP_ZFMethod_paramTypeIdList[7]
        );
}

zfautoObject ZFMethod::methodGenericInvoke(ZF_IN_OPT ZFObject *ownerObjOrNull /* = zfnull */
                                           , ZF_IN_OPT ZFObject *param0 /* = ZFMethodGenericInvokerDefaultParam() */
                                           , ZF_IN_OPT ZFObject *param1 /* = ZFMethodGenericInvokerDefaultParam() */
                                           , ZF_IN_OPT ZFObject *param2 /* = ZFMethodGenericInvokerDefaultParam() */
                                           , ZF_IN_OPT ZFObject *param3 /* = ZFMethodGenericInvokerDefaultParam() */
                                           , ZF_IN_OPT ZFObject *param4 /* = ZFMethodGenericInvokerDefaultParam() */
                                           , ZF_IN_OPT ZFObject *param5 /* = ZFMethodGenericInvokerDefaultParam() */
                                           , ZF_IN_OPT ZFObject *param6 /* = ZFMethodGenericInvokerDefaultParam() */
                                           , ZF_IN_OPT ZFObject *param7 /* = ZFMethodGenericInvokerDefaultParam() */
                                           , ZF_OUT_OPT zfbool *success /* = zfnull */
                                           , ZF_OUT_OPT zfstring *errorHint /* = zfnull */
                                           ) const
{
    zfautoObject ret;
    zfautoObject paramList[ZFMETHOD_MAX_PARAM];
    paramList[0].zflockfree_assign(param0);
    paramList[1].zflockfree_assign(param1);
    paramList[2].zflockfree_assign(param2);
    paramList[3].zflockfree_assign(param3);
    paramList[4].zflockfree_assign(param4);
    paramList[5].zflockfree_assign(param5);
    paramList[6].zflockfree_assign(param6);
    paramList[7].zflockfree_assign(param7);

    zfbool t = this->methodGenericInvoker()(this, ownerObjOrNull, errorHint, ret, paramList);
    if(success != zfnull)
    {
        *success = t;
    }
    return ret;
}

void ZFMethod::methodGenericInvoker(ZF_IN ZFMethodGenericInvoker methodGenericInvoker) const
{
    zfCoreMutexLocker();
    ZFMethod *m = this->_ZFP_ZFMethod_removeConst();
    if(methodGenericInvoker != zfnull)
    {
        m->_ZFP_ZFMethod_methodGenericInvoker = methodGenericInvoker;
    }
    else
    {
        m->_ZFP_ZFMethod_methodGenericInvoker = m->_ZFP_ZFMethod_methodGenericInvokerOrg;
    }
    _ZFP_ZFClassDataChangeNotify(ZFClassDataChangeTypeUpdate, zfnull, zfnull, this);
}

void ZFMethod::methodInvoker(ZF_IN ZFFuncAddrType methodInvoker) const
{
    this->_ZFP_ZFMethod_removeConst()->_ZFP_ZFMethod_invoker =
        ((methodInvoker != zfnull) ? methodInvoker : this->_ZFP_ZFMethod_invokerOrg);
    _ZFP_ZFClassDataChangeNotify(ZFClassDataChangeTypeUpdate, zfnull, zfnull, this);
}

// ============================================================
zfclassLikePOD _ZFP_ZFMethodMapData
{
public:
    zfuint refCount;
    ZFMethod *method;
public:
    explicit _ZFP_ZFMethodMapData(ZF_IN ZFMethod *method)
    : refCount(1)
    , method(method)
    {
    }
    ~_ZFP_ZFMethodMapData(void)
    {
        zfdelete(this->method);
    }
};
ZF_STATIC_INITIALIZER_INIT(ZFMethodDataHolder)
{
}
ZFCoreMap methodMap; // _ZFP_ZFMethodMapData *
ZF_STATIC_INITIALIZER_END(ZFMethodDataHolder)
#define _ZFP_ZFMethodMap (ZF_STATIC_INITIALIZER_INSTANCE(ZFMethodDataHolder)->methodMap)

void _ZFP_ZFMethodDataHolderInit(void)
{
    (void)_ZFP_ZFMethodMap;
}

// ============================================================
static void _ZFP_ZFMethodInstanceSig(ZF_OUT zfstring &ret,
                                     ZF_IN const zfchar *methodScope,
                                     ZF_IN const zfchar *methodName
                                     , ZF_IN_OPT const zfchar *methodParamTypeId0 /* = zfnull */
                                     , ZF_IN_OPT const zfchar *methodParamTypeId1 /* = zfnull */
                                     , ZF_IN_OPT const zfchar *methodParamTypeId2 /* = zfnull */
                                     , ZF_IN_OPT const zfchar *methodParamTypeId3 /* = zfnull */
                                     , ZF_IN_OPT const zfchar *methodParamTypeId4 /* = zfnull */
                                     , ZF_IN_OPT const zfchar *methodParamTypeId5 /* = zfnull */
                                     , ZF_IN_OPT const zfchar *methodParamTypeId6 /* = zfnull */
                                     , ZF_IN_OPT const zfchar *methodParamTypeId7 /* = zfnull */
                                     )
{
    if(!zfscmpTheSame(methodScope, ZF_NAMESPACE_GLOBAL_NAME)
        && !zfscmpTheSame(methodScope, ZF_NAMESPACE_GLOBAL_ABBR_NAME))
    {
        ret += methodScope;
    }
    ret += ':';
    ret += methodName;

    ret += '+'; ret += methodParamTypeId0;
    ret += '+'; ret += methodParamTypeId1;
    ret += '+'; ret += methodParamTypeId2;
    ret += '+'; ret += methodParamTypeId3;
    ret += '+'; ret += methodParamTypeId4;
    ret += '+'; ret += methodParamTypeId5;
    ret += '+'; ret += methodParamTypeId6;
    ret += '+'; ret += methodParamTypeId7;
}

static ZFMethod *_ZFP_ZFMethodInstanceAccess(ZF_IN const zfchar *methodInternalId)
{
    zfCoreMutexLocker();
    _ZFP_ZFMethodMapData *v = _ZFP_ZFMethodMap.get<_ZFP_ZFMethodMapData *>(methodInternalId);
    if(v == zfnull)
    {
        v = zfnew(_ZFP_ZFMethodMapData, zfnew(ZFMethod));
        _ZFP_ZFMethodMap.set(methodInternalId, ZFCorePointerForObject<_ZFP_ZFMethodMapData *>(v));
        v->method->_ZFP_ZFMethod_methodInternalId = methodInternalId;
    }
    else
    {
        ++(v->refCount);
    }
    return v->method;
}

ZFMethod *_ZFP_ZFMethodRegister(ZF_IN zfbool methodIsUserRegister
                                , ZF_IN zfbool methodIsDynamicRegister
                                , ZF_IN ZFObject *methodDynamicRegisterUserData
                                , ZF_IN ZFFuncAddrType methodInvoker
                                , ZF_IN ZFMethodGenericInvoker methodGenericInvoker
                                , ZF_IN const zfchar *methodType
                                , ZF_IN const ZFClass *methodOwnerClass
                                , ZF_IN ZFMethodPrivilegeType methodPrivilegeType
                                , ZF_IN const zfchar *methodNamespace
                                , ZF_IN const zfchar *methodName
                                , ZF_IN const zfchar *returnTypeId
                                , ZF_IN const zfchar *returnTypeName
                                /* ParamTypeIdString, ParamTypeName, DefaultValueAccessCallback, end with zfnull */
                                , ...
                                )
{
    va_list vaList;
    va_start(vaList, returnTypeName);
    ZFMethod *method = _ZFP_ZFMethodRegisterV(methodIsUserRegister
            , methodIsDynamicRegister
            , methodDynamicRegisterUserData
            , methodInvoker
            , methodGenericInvoker
            , methodType
            , methodOwnerClass
            , methodPrivilegeType
            , methodNamespace
            , methodName
            , returnTypeId
            , returnTypeName
            , vaList
        );
    va_end(vaList);
    return method;
}
ZFMethod *_ZFP_ZFMethodRegisterV(ZF_IN zfbool methodIsUserRegister
                                 , ZF_IN zfbool methodIsDynamicRegister
                                 , ZF_IN ZFObject *methodDynamicRegisterUserData
                                 , ZF_IN ZFFuncAddrType methodInvoker
                                 , ZF_IN ZFMethodGenericInvoker methodGenericInvoker
                                 , ZF_IN const zfchar *methodType
                                 , ZF_IN const ZFClass *methodOwnerClass
                                 , ZF_IN ZFMethodPrivilegeType methodPrivilegeType
                                 , ZF_IN const zfchar *methodNamespace
                                 , ZF_IN const zfchar *methodName
                                 , ZF_IN const zfchar *returnTypeId
                                 , ZF_IN const zfchar *returnTypeName
                                 /* ParamTypeIdString, ParamTypeName, DefaultValueAccessCallback, end with zfnull */
                                 , ZF_IN va_list vaList
                                 )
{
    zfCoreMutexLocker();

    zfCoreAssert(methodGenericInvoker != zfnull);
    zfCoreAssert(methodType != zfnull);
    zfCoreAssert(methodOwnerClass == zfnull || methodNamespace == zfnull);
    zfCoreAssert(methodName != zfnull && *methodName != '\0');
    zfCoreAssert(returnTypeId != zfnull && *returnTypeId != '\0');
    zfCoreAssert(returnTypeName != zfnull && *returnTypeName != '\0');

    ZFMethod *method = zfnull;

    zfindex paramCount = 0;
    const zfchar *paramTypeId[ZFMETHOD_MAX_PARAM + 1] = {0};
    const zfchar *paramTypeName[ZFMETHOD_MAX_PARAM + 1] = {0};
    ZFMethodParamDefaultValueCallback paramDefaultValueAccess[ZFMETHOD_MAX_PARAM + 1] = {0};
    {
        paramTypeId[paramCount] = va_arg(vaList, const zfchar *);
        while(paramTypeId[paramCount] != zfnull)
        {
            paramTypeName[paramCount] = va_arg(vaList, const zfchar *);
            paramDefaultValueAccess[paramCount] = va_arg(vaList, ZFMethodParamDefaultValueCallback);
            ++paramCount;
            paramTypeId[paramCount] = va_arg(vaList, const zfchar *);
        }
    }

    zfstring methodInternalId;
    _ZFP_ZFMethodInstanceSig(methodInternalId
            , methodOwnerClass ? methodOwnerClass->classNameFull() : methodNamespace
            , methodName
            , paramTypeId[0]
            , paramTypeId[1]
            , paramTypeId[2]
            , paramTypeId[3]
            , paramTypeId[4]
            , paramTypeId[5]
            , paramTypeId[6]
            , paramTypeId[7]
        );

    _ZFP_ZFMethodMapData *methodData = _ZFP_ZFMethodMap.get<_ZFP_ZFMethodMapData *>(methodInternalId);
    if(methodData != zfnull)
    {
        method = methodData->method;
        if(method->methodIsUserRegister())
        {
            if(methodOwnerClass != zfnull)
            {
                zfCoreCriticalMessageTrim(
                    "[ZFMethodUserRegister] registering a method that already registered, class: %s, methodName: %s, methodInternalId: %s",
                    methodOwnerClass->classNameFull(),
                    methodName,
                    methodInternalId.cString());
            }
            else
            {
                zfCoreCriticalMessageTrim(
                    "[ZFMethodFuncUserRegister] registering a method that already registered, namespace: %s, methodName: %s, methodInternalId: %s",
                    methodNamespace,
                    methodName,
                    methodInternalId.cString());
            }
        }
        else if(method->methodIsDynamicRegister())
        {
            if(methodOwnerClass != zfnull)
            {
                zfCoreCriticalMessageTrim(
                    "[ZFMethodDynamicRegister] registering a method that already registered, class: %s, methodName: %s, methodInternalId: %s",
                    methodOwnerClass->classNameFull(),
                    methodName,
                    methodInternalId.cString());
            }
            else
            {
                zfCoreCriticalMessageTrim(
                    "[ZFMethodDynamicRegister] registering a method that already registered, namespace: %s, methodName: %s, methodInternalId: %s",
                    methodNamespace,
                    methodName,
                    methodInternalId.cString());
            }
        }

        // param type id may be same for different cpp type
        // strict check type name to prevent redefine
        zfbool isRedefine = (paramCount != method->methodParamCount());
        if(!isRedefine)
        {
            for(zfindex i = 0; i < paramCount; ++i)
            {
                if(!zfscmpTheSame(paramTypeName[i], method->methodParamTypeNameAtIndex(i)))
                {
                    isRedefine = zftrue;
                    break;
                }
            }
        }
        zfCoreAssertWithMessageTrim(!isRedefine,
            "[ZFMethod] redefine of method: %s, existing: %s",
            methodInternalId.cString(),
            method->objectInfo().cString());
    }
    else
    {
        method = _ZFP_ZFMethodInstanceAccess(methodInternalId);
    }

    if(method->_ZFP_ZFMethodNeedInit)
    {
        method->_ZFP_ZFMethodNeedInit = zffalse;
        method->_ZFP_ZFMethod_init(methodIsUserRegister
                , methodIsDynamicRegister
                , methodDynamicRegisterUserData
                , methodInvoker
                , methodGenericInvoker
                , methodType
                , methodName
                , returnTypeId
                , returnTypeName
                , paramTypeId[0], paramTypeName[0], paramDefaultValueAccess[0]
                , paramTypeId[1], paramTypeName[1], paramDefaultValueAccess[1]
                , paramTypeId[2], paramTypeName[2], paramDefaultValueAccess[2]
                , paramTypeId[3], paramTypeName[3], paramDefaultValueAccess[3]
                , paramTypeId[4], paramTypeName[4], paramDefaultValueAccess[4]
                , paramTypeId[5], paramTypeName[5], paramDefaultValueAccess[5]
                , paramTypeId[6], paramTypeName[6], paramDefaultValueAccess[6]
                , paramTypeId[7], paramTypeName[7], paramDefaultValueAccess[7]
                , zfnull
            );

        if(methodOwnerClass != zfnull)
        {
            method->_ZFP_ZFMethod_initClassMemberType(methodOwnerClass, methodPrivilegeType);
            methodOwnerClass->_ZFP_ZFClass_methodRegister(method);
            _ZFP_ZFClassDataChangeNotify(ZFClassDataChangeTypeAttach, zfnull, zfnull, method);
        }
        else
        {
            method->_ZFP_ZFMethod_initFuncType(methodNamespace);
            _ZFP_ZFMethodFuncRegister(method);
            _ZFP_ZFClassDataChangeNotify(ZFClassDataChangeTypeAttach, zfnull, zfnull, method);
        }
    }

    return method;
}
void _ZFP_ZFMethodUnregister(ZF_IN const ZFMethod *method)
{
    zfCoreMutexLocker();
    _ZFP_ZFMethodMapData *v = _ZFP_ZFMethodMap.get<_ZFP_ZFMethodMapData *>(method->methodInternalId());
    if(v == zfnull)
    {
        return ;
    }
    --(v->refCount);
    if(v->refCount != 0)
    {
        return ;
    }

    if(method->methodIsFunctionType())
    {
        _ZFP_ZFMethodFuncUnregister(method);
    }
    else
    {
        if(method->methodIsUserRegister() || method->methodIsDynamicRegister())
        {
            method->methodOwnerClass()->_ZFP_ZFClass_removeConst()->_ZFP_ZFClass_methodUnregister(method);
        }
    }
    _ZFP_ZFClassDataChangeNotify(ZFClassDataChangeTypeDetach, zfnull, zfnull, method);

    _ZFP_ZFMethodMap.remove(method->methodInternalId());
}

_ZFP_ZFMethodRegisterHolder::_ZFP_ZFMethodRegisterHolder(ZF_IN zfbool methodIsUserRegister
                                                         , ZF_IN zfbool methodIsDynamicRegister
                                                         , ZF_IN ZFObject *methodDynamicRegisterUserData
                                                         , ZF_IN ZFFuncAddrType methodInvoker
                                                         , ZF_IN ZFMethodGenericInvoker methodGenericInvoker
                                                         , ZF_IN const zfchar *methodType
                                                         , ZF_IN const ZFClass *methodOwnerClass
                                                         , ZF_IN ZFMethodPrivilegeType methodPrivilegeType
                                                         , ZF_IN const zfchar *methodNamespace
                                                         , ZF_IN const zfchar *methodName
                                                         , ZF_IN const zfchar *returnTypeId
                                                         , ZF_IN const zfchar *returnTypeName
                                                         /* ParamTypeIdString, ParamTypeName, DefaultValueAccessCallback, end with zfnull */
                                                         , ...
                                                         )
: method(zfnull)
{
    va_list vaList;
    va_start(vaList, returnTypeName);
    method = _ZFP_ZFMethodRegisterV(methodIsUserRegister
            , methodIsDynamicRegister
            , methodDynamicRegisterUserData
            , methodInvoker
            , methodGenericInvoker
            , methodType
            , methodOwnerClass
            , methodPrivilegeType
            , methodNamespace
            , methodName
            , returnTypeId
            , returnTypeName
            , vaList
        );
    va_end(vaList);
}
_ZFP_ZFMethodRegisterHolder::_ZFP_ZFMethodRegisterHolder(ZF_IN zfbool dummy
                                                         , ZF_IN zfbool methodIsUserRegister
                                                         , ZF_IN zfbool methodIsDynamicRegister
                                                         , ZF_IN ZFObject *methodDynamicRegisterUserData
                                                         , ZF_IN ZFFuncAddrType methodInvoker
                                                         , ZF_IN ZFMethodGenericInvoker methodGenericInvoker
                                                         , ZF_IN const zfchar *methodType
                                                         , ZF_IN const ZFClass *methodOwnerClass
                                                         , ZF_IN ZFMethodPrivilegeType methodPrivilegeType
                                                         , ZF_IN const zfchar *methodNamespace
                                                         , ZF_IN const zfchar *methodName
                                                         , ZF_IN const zfchar *returnTypeId
                                                         , ZF_IN const zfchar *returnTypeName
                                                         /* ParamTypeIdString, ParamTypeName, DefaultValueAccessCallback, end with zfnull */
                                                         , ZF_IN va_list vaList
                                                         )
: method(_ZFP_ZFMethodRegisterV(methodIsUserRegister
        , methodIsDynamicRegister
        , methodDynamicRegisterUserData
        , methodInvoker
        , methodGenericInvoker
        , methodType
        , methodOwnerClass
        , methodPrivilegeType
        , methodNamespace
        , methodName
        , returnTypeId
        , returnTypeName
        , vaList
    ))
{
}
_ZFP_ZFMethodRegisterHolder::~_ZFP_ZFMethodRegisterHolder(void)
{
    _ZFP_ZFMethodUnregister(this->method);
}

// ============================================================
void ZFMethodGetAllT(ZF_IN_OUT ZFCoreArray<const ZFMethod *> &ret,
                     ZF_IN_OPT const ZFFilterForZFMethod *methodFilter /* = zfnull */)
{
    zfCoreMutexLocker();
    const ZFCoreMap &m = _ZFP_ZFMethodMap;
    if(methodFilter != zfnull)
    {
        for(zfiterator it = m.iterator(); m.iteratorIsValid(it); )
        {
            _ZFP_ZFMethodMapData *v = m.iteratorNextValue<_ZFP_ZFMethodMapData *>(it);
            if(methodFilter->filterCheckActive(v->method))
            {
                ret.add(v->method);
            }
        }
    }
    else
    {
        for(zfiterator it = m.iterator(); m.iteratorIsValid(it); )
        {
            _ZFP_ZFMethodMapData *v = m.iteratorNextValue<_ZFP_ZFMethodMapData *>(it);
            ret.add(v->method);
        }
    }
}

// ============================================================
/* ZFMETHOD_MAX_PARAM */
const ZFMethod *ZFMethodForName(ZF_IN const zfchar *classNameOrNamespace,
                                ZF_IN const zfchar *methodName)
{
    const ZFClass *cls = ZFClass::classForName(classNameOrNamespace);
    if(cls != zfnull)
    {
        return cls->methodForName(methodName);
    }
    else
    {
        return _ZFP_ZFMethodFuncForName(classNameOrNamespace, methodName);
    }
}
const ZFMethod *ZFMethodForName(ZF_IN const zfchar *classNameOrNamespace,
                                ZF_IN const zfchar *methodName
                                , ZF_IN_OPT const zfchar *methodParamTypeId0
                                , ZF_IN_OPT const zfchar *methodParamTypeId1 /* = zfnull */
                                , ZF_IN_OPT const zfchar *methodParamTypeId2 /* = zfnull */
                                , ZF_IN_OPT const zfchar *methodParamTypeId3 /* = zfnull */
                                , ZF_IN_OPT const zfchar *methodParamTypeId4 /* = zfnull */
                                , ZF_IN_OPT const zfchar *methodParamTypeId5 /* = zfnull */
                                , ZF_IN_OPT const zfchar *methodParamTypeId6 /* = zfnull */
                                , ZF_IN_OPT const zfchar *methodParamTypeId7 /* = zfnull */
                                )
{
    const ZFClass *cls = ZFClass::classForName(classNameOrNamespace);
    if(cls != zfnull)
    {
        return cls->methodForName(methodName
                , methodParamTypeId0
                , methodParamTypeId1
                , methodParamTypeId2
                , methodParamTypeId3
                , methodParamTypeId4
                , methodParamTypeId5
                , methodParamTypeId6
                , methodParamTypeId7
            );
    }
    else
    {
        return _ZFP_ZFMethodFuncForName(classNameOrNamespace, methodName
                , methodParamTypeId0
                , methodParamTypeId1
                , methodParamTypeId2
                , methodParamTypeId3
                , methodParamTypeId4
                , methodParamTypeId5
                , methodParamTypeId6
                , methodParamTypeId7
            );
    }
}
void ZFMethodForNameGetAllT(ZF_IN_OUT ZFCoreArray<const ZFMethod *> &ret,
                            ZF_IN const zfchar *classNameOrNamespace,
                            ZF_IN const zfchar *methodName)
{
    const ZFClass *cls = ZFClass::classForName(classNameOrNamespace);
    if(cls != zfnull)
    {
        return cls->methodForNameGetAllT(ret, methodName);
    }
    else
    {
        return _ZFP_ZFMethodFuncForNameGetAllT(ret, classNameOrNamespace, methodName);
    }
}

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFEXPORT_ENUM_DEFINE(ZFMethodPrivilegeType, ZFMethodPrivilegeTypePublic, ZFMethodPrivilegeTypeProtected, ZFMethodPrivilegeTypePrivate)
ZFEXPORT_ENUM_DEFINE(ZFMethodType, ZFMethodTypeNormal, ZFMethodTypeStatic, ZFMethodTypeVirtual)

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(void, ZFMethodGetAllT, ZFMP_IN_OUT(ZFCoreArray<const ZFMethod *> &, ret), ZFMP_IN_OPT(const ZFFilterForZFMethod *, methodFilter, zfnull))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(ZFCoreArrayPOD<const ZFMethod *>, ZFMethodGetAll, ZFMP_IN_OPT(const ZFFilterForZFMethod *, methodFilter, zfnull))

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(const ZFMethod *, ZFMethodForName, ZFMP_IN(const zfchar *, classNameOrNamespace), ZFMP_IN(const zfchar *, methodName))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_8(const ZFMethod *, ZFMethodForName, ZFMP_IN(const zfchar *, classNameOrNamespace), ZFMP_IN(const zfchar *, methodName)
    , ZFMP_IN(const zfchar *, methodParamTypeId0)
    , ZFMP_IN_OPT(const zfchar *, methodParamTypeId1, zfnull)
    , ZFMP_IN_OPT(const zfchar *, methodParamTypeId2, zfnull)
    , ZFMP_IN_OPT(const zfchar *, methodParamTypeId3, zfnull)
    , ZFMP_IN_OPT(const zfchar *, methodParamTypeId4, zfnull)
    , ZFMP_IN_OPT(const zfchar *, methodParamTypeId5, zfnull)
    /* ZFMETHOD_MAX_PARAM , ZFMP_IN_OPT(const zfchar *, methodParamTypeId6, zfnull) */
    /* ZFMETHOD_MAX_PARAM , ZFMP_IN_OPT(const zfchar *, methodParamTypeId7, zfnull) */
    )

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(void, ZFMethodForNameGetAllT, ZFMP_IN_OUT(ZFCoreArray<const ZFMethod *> &, ret), ZFMP_IN(const zfchar *, classNameOrNamespace), ZFMP_IN(const zfchar *, methodName))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(ZFCoreArrayPOD<const ZFMethod *>, ZFMethodForNameGetAll, ZFMP_IN(const zfchar *, classNameOrNamespace), ZFMP_IN(const zfchar *, methodName))

ZF_NAMESPACE_GLOBAL_END
#endif

