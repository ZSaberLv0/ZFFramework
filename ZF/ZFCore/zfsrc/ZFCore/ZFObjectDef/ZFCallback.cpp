#include "ZFCallback.h"
#include "ZFObjectImpl.h"

#include "ZFCore/ZFSTLWrapper/zfstl_string.h"
#include "ZFCore/ZFSTLWrapper/zfstl_map.h"
#include "ZFCore/ZFSTLWrapper/zfstl_vector.h"

ZF_NAMESPACE_GLOBAL_BEGIN

void _ZFP_ZFCallback_executeNullCallback(void)
{
    zfCoreCriticalMessageTrim("try execute a null callback");
}

// ============================================================
// _ZFP_ZFCallbackPrivate
typedef zfstlmap<zfstlstringZ, zfautoObject> _ZFP_ZFCallbackTagMap;
zfclassNotPOD ZF_ENV_EXPORT _ZFP_ZFCallbackPrivate
{
public:
    zfuint refCount;
    zfchar *callbackId;
    _ZFP_ZFCallbackTagMap callbackTagMap;

    ZFCallbackType callbackType;
    ZFObject *callbackOwnerObject; // assign only
    zfuint callbackOwnerObjectRetainFlag;
    const ZFMethod *callbackMethod;
    ZFFuncAddrType callbackRawFunction;
    _ZFP_ZFCallbackLambda *callbackLambdaImpl;
    ZFFuncAddrType callbackLambdaInvoker;
    _ZFP_ZFCallbackLambda::DestroyCallback callbackLambdaImplDestroy;

    zfchar *serializableCustomType;
    ZFSerializableData *serializableCustomData;
    ZFPathInfo *pathInfo;

public:
    _ZFP_ZFCallbackPrivate(void)
    : refCount(1)
    , callbackId(zfnull)
    , callbackTagMap()
    , callbackType(ZFCallbackTypeDummy)
    , callbackOwnerObject(zfnull)
    , callbackOwnerObjectRetainFlag(0)
    , callbackMethod(zfnull)
    , callbackRawFunction(zfnull)
    , callbackLambdaImpl(zfnull)
    , callbackLambdaInvoker(zfnull)
    , callbackLambdaImplDestroy(zfnull)
    , serializableCustomType(zfnull)
    , serializableCustomData(zfnull)
    , pathInfo(zfnull)
    {
    }
    ~_ZFP_ZFCallbackPrivate(void)
    {
        zffree(this->callbackId);
        if(this->callbackOwnerObjectRetainFlag != 0)
        {
            zfRelease(this->callbackOwnerObject);
        }
        zffree(this->serializableCustomType);
        zfdelete(this->serializableCustomData);
        zfdelete(this->pathInfo);
        if(this->callbackLambdaImplDestroy)
        {
            this->callbackLambdaImplDestroy(this->callbackLambdaImpl);
        }
    }
};

// ============================================================
// global
static void _ZFP_ZFCallbackPrivateDataChange(_ZFP_ZFCallbackPrivate *&oldData, _ZFP_ZFCallbackPrivate *newData)
{
    _ZFP_ZFCallbackPrivate *dTmp = oldData;
    oldData = newData;
    if(newData != zfnull)
    {
        ++newData->refCount;
    }
    if(dTmp != zfnull)
    {
        --dTmp->refCount;
        if(dTmp->refCount == 0)
        {
            zfdelete(dTmp);
        }
    }
}

// ============================================================
// ZFCallback
ZFCallback::ZFCallback(void)
: d(zfnull)
{
}
ZFCallback::ZFCallback(ZF_IN const ZFCallback &ref)
: d(zfnull)
{
    this->operator = (ref);
}
ZFCallback &ZFCallback::operator = (ZF_IN const ZFCallback &ref)
{
    _ZFP_ZFCallbackPrivateDataChange(d, ref.d);
    return *this;
}
ZFCallback::~ZFCallback(void)
{
    if(d != zfnull)
    {
        --d->refCount;
        if(d->refCount == 0)
        {
            zfdelete(d);
        }
    }
}
ZFCallback ZFCallback::_ZFP_ZFCallbackCreate(ZF_IN ZFCallbackType callbackType,
                                             ZF_IN ZFObject *callbackOwnerObject,
                                             ZF_IN const ZFMethod *callbackMethod,
                                             ZF_IN ZFFuncAddrType callbackRawFunction,
                                             ZF_IN _ZFP_ZFCallbackLambda *callbackLambdaImpl,
                                             ZF_IN _ZFP_ZFCallbackLambda::DestroyCallback callbackLambdaImplDestroy)
{
    ZFCallback callback;
    callback.d = zfnew(_ZFP_ZFCallbackPrivate);
    switch(callbackType)
    {
        case ZFCallbackTypeDummy:
            break;
        case ZFCallbackTypeMethod:
            zfCoreAssertWithMessageTrim(callbackMethod != zfnull, "[ZFCallback] method is null");
            zfCoreAssertWithMessageTrim(callbackMethod->methodType() == ZFMethodTypeStatic,
                "[ZFCallback] method \"%s\" is not class static member type",
                callbackMethod->objectInfo().cString());
            callback.d->callbackType = ZFCallbackTypeMethod;
            callback.d->callbackMethod = callbackMethod;
            break;
        case ZFCallbackTypeMemberMethod:
            zfCoreAssertWithMessageTrim(callbackOwnerObject != zfnull && callbackMethod != zfnull,
                "[ZFCallback] invalid callback, ownerObj: %s, method: %s",
                zfsFromPointer(callbackOwnerObject).cString(),
                zfsFromPointer(callbackMethod).cString());
            zfCoreAssertWithMessageTrim(callbackMethod->methodType() != ZFMethodTypeStatic,
                "[ZFCallback] method \"%s\" is not class member type",
                callbackMethod->objectInfo().cString());
            zfCoreAssertWithMessageTrim(callbackOwnerObject->classData()->classIsTypeOf(callbackMethod->methodOwnerClass()),
                "[ZFCallback] object %s has no such method \"%s\"",
                callbackOwnerObject->objectInfoOfInstance().cString(),
                callbackMethod->objectInfo().cString());
            callback.d->callbackType = ZFCallbackTypeMemberMethod;
            callback.d->callbackOwnerObject = callbackOwnerObject;
            callback.d->callbackMethod = callbackMethod;
            break;
        case ZFCallbackTypeRawFunction:
            zfCoreAssertWithMessageTrim(callbackRawFunction != zfnull, "[ZFCallback] invalid function address");
            callback.d->callbackType = ZFCallbackTypeRawFunction;
            callback.d->callbackRawFunction = callbackRawFunction;
            break;
        case ZFCallbackTypeLambda:
            zfCoreAssertWithMessageTrim(callbackLambdaImpl != zfnull && callbackLambdaImplDestroy != zfnull, "[ZFCallback] invalid lambda impl");
            callback.d->callbackType = ZFCallbackTypeLambda;
            callback.d->callbackLambdaImpl = callbackLambdaImpl;
            callback.d->callbackLambdaInvoker = callbackLambdaImpl->_ZFP_ivk();
            callback.d->callbackLambdaImplDestroy = callbackLambdaImplDestroy;
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            break;
    }
    return callback;
}

zfindex ZFCallback::objectRetainCount(void) const
{
    return (d ? d->refCount : 1);
}

void ZFCallback::objectInfoT(ZF_IN_OUT zfstring &ret) const
{
    switch(this->callbackType())
    {
        case ZFCallbackTypeDummy:
            ret += "ZFCallbackNull";
            break;
        case ZFCallbackTypeMethod:
        case ZFCallbackTypeMemberMethod:
            this->callbackMethod()->objectInfoT(ret);
            break;
        case ZFCallbackTypeRawFunction:
            ret += ZFTOKEN_ZFObjectInfoLeft;
            ret += "ZFCallback func: ";
            zfsFromPointerT(ret, ZFCastReinterpret(const void *, this->callbackRawFunction()));
            ret += ZFTOKEN_ZFObjectInfoRight;
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            break;
    }
    if(this->callbackOwnerObject() != zfnull)
    {
        ret += ", owner: ";
        this->callbackOwnerObject()->objectInfoT(ret);
    }
    if(d != zfnull && !d->callbackTagMap.empty())
    {
        ret += ", tags: ";
        _ZFP_ZFCallbackTagMap &m = d->callbackTagMap;
        for(_ZFP_ZFCallbackTagMap::iterator it = m.begin(); it != m.end(); ++it)
        {
            if(it != m.begin())
            {
                ret += ", ";
            }
            ret += "<";
            ret += it->first.c_str();
            ret += ", ";
            ZFObjectInfoT(ret, it->second);
            ret += ">";
        }
    }
}

ZFCompareResult ZFCallback::objectCompare(ZF_IN const ZFCallback &ref) const
{
    return ((d == ref.d || (
            this->callbackType() == ref.callbackType()
            && this->callbackOwnerObject() == ref.callbackOwnerObject()
            && this->callbackMethod() == ref.callbackMethod()
            && this->callbackRawFunction() == ref.callbackRawFunction()
            ))
        ? ZFCompareTheSame : ZFCompareUncomparable);
}

void ZFCallback::callbackId(ZF_IN const zfchar *callbackId)
{
    if(d == zfnull)
    {
        d = zfnew(_ZFP_ZFCallbackPrivate);
    }
    if(callbackId != zfnull && *callbackId == '\0')
    {
        callbackId = zfnull;
    }
    zfsChange(d->callbackId, callbackId);
}
const zfchar *ZFCallback::callbackId(void) const
{
    return (d ? d->callbackId : zfnull);
}

void ZFCallback::callbackTag(ZF_IN const zfchar *key,
                             ZF_IN ZFObject *tag)
{
    if(key == zfnull)
    {
        return ;
    }
    if(d == zfnull)
    {
        d = zfnew(_ZFP_ZFCallbackPrivate);
    }

    _ZFP_ZFCallbackTagMap &m = d->callbackTagMap;
    _ZFP_ZFCallbackTagMap::iterator it = m.find(key);
    if(it == m.end())
    {
        if(tag != zfnull)
        {
            m[key] = tag;
        }
    }
    else
    {
        if(tag == zfnull)
        {
            zfautoObject holder = it->second;
            ZFUNUSED(holder);
            m.erase(it);
        }
        else
        {
            zfautoObject holder = it->second;
            ZFUNUSED(holder);
            it->second = tag;
        }
    }
}
ZFObject *ZFCallback::callbackTag(ZF_IN const zfchar *key) const
{
    if(d != zfnull && key != zfnull)
    {
        _ZFP_ZFCallbackTagMap &m = d->callbackTagMap;
        _ZFP_ZFCallbackTagMap::iterator it = m.find(key);
        if(it != m.end())
        {
            return it->second.toObject();
        }
    }
    return zfnull;
}
void ZFCallback::callbackTagGetAllKeyValue(ZF_IN_OUT ZFCoreArray<const zfchar *> &allKey,
                                           ZF_IN_OUT ZFCoreArray<ZFObject *> &allValue) const
{
    if(d != zfnull)
    {
        _ZFP_ZFCallbackTagMap &m = d->callbackTagMap;
        allKey.capacity(allKey.count() + m.size());
        allValue.capacity(allValue.count() + m.size());
        for(_ZFP_ZFCallbackTagMap::iterator it = m.begin(); it != m.end(); ++it)
        {
            allKey.add(it->first.c_str());
            allValue.add(it->second.toObject());
        }
    }
}
zfautoObject ZFCallback::callbackTagRemoveAndGet(ZF_IN const zfchar *key)
{
    if(d != zfnull && key != zfnull)
    {
        _ZFP_ZFCallbackTagMap &m = d->callbackTagMap;
        _ZFP_ZFCallbackTagMap::iterator it = m.find(key);
        if(it != m.end())
        {
            zfautoObject ret = it->second;
            m.erase(it);
            return ret;
        }
    }
    return zfnull;
}
void ZFCallback::callbackTagRemoveAll(void)
{
    if(d != zfnull && !d->callbackTagMap.empty())
    {
        _ZFP_ZFCallbackTagMap tmp;
        tmp.swap(d->callbackTagMap);
    }
}

ZFCallbackType ZFCallback::callbackType(void) const
{
    return (d ? d->callbackType : ZFCallbackTypeDummy);
}

ZFObject *ZFCallback::callbackOwnerObject(void) const
{
    return (d ? d->callbackOwnerObject : zfnull);
}

const ZFMethod *ZFCallback::callbackMethod(void) const
{
    return (d ? d->callbackMethod : zfnull);
}

ZFFuncAddrType ZFCallback::callbackRawFunction(void) const
{
    return (d ? d->callbackRawFunction : zfnull);
}

void ZFCallback::callbackClear(void)
{
    _ZFP_ZFCallbackPrivateDataChange(d, zfnull);
}

void ZFCallback::callbackOwnerObjectRetain(void) const
{
    if(d != zfnull && d->callbackOwnerObject != zfnull && !d->callbackOwnerObjectRetainFlag)
    {
        zfRetain(d->callbackOwnerObject);
        ++(d->callbackOwnerObjectRetainFlag);
    }
}
void ZFCallback::callbackOwnerObjectRelease(void) const
{
    if(d != zfnull && d->callbackOwnerObjectRetainFlag > 0)
    {
        --(d->callbackOwnerObjectRetainFlag);
        if(d->callbackOwnerObjectRetainFlag == 0)
        {
            zfRetainChange(d->callbackOwnerObject, zfnull);
        }
    }
}

void ZFCallback::callbackSerializeCustomType(ZF_IN const zfchar *customType)
{
    if(d == zfnull)
    {
        d = zfnew(_ZFP_ZFCallbackPrivate);
    }
    zfsChange(d->serializableCustomType, customType);
}
const zfchar *ZFCallback::callbackSerializeCustomType(void) const
{
    return (d ? d->serializableCustomType : zfnull);
}
void ZFCallback::callbackSerializeCustomData(ZF_IN const ZFSerializableData *customData)
{
    if(d == zfnull)
    {
        d = zfnew(_ZFP_ZFCallbackPrivate);
    }
    if(customData == zfnull)
    {
        if(d->serializableCustomData != zfnull)
        {
            zfdelete(d->serializableCustomData);
            d->serializableCustomData = zfnull;
        }
    }
    else
    {
        if(d->serializableCustomData != zfnull)
        {
            *(d->serializableCustomData) = *customData;
        }
        else
        {
            d->serializableCustomData = zfnew(ZFSerializableData, *customData);
        }
    }
}
const ZFSerializableData *ZFCallback::callbackSerializeCustomData(void) const
{
    return (d ? d->serializableCustomData : zfnull);
}

const ZFPathInfo *ZFCallback::pathInfo(void) const
{
    return (d ? d->pathInfo : zfnull);
}
void ZFCallback::pathInfo(ZF_IN const ZFPathInfo *pathInfo)
{
    if(d == zfnull)
    {
        d = zfnew(_ZFP_ZFCallbackPrivate);
    }
    if(pathInfo != zfnull && !(pathInfo->pathType.isEmpty() && pathInfo->pathData.isEmpty()))
    {
        if(d->pathInfo == zfnull)
        {
            d->pathInfo = zfnew(ZFPathInfo, *pathInfo);
        }
        else
        {
            *(d->pathInfo) = *pathInfo;
        }
    }
    else
    {
        if(d->pathInfo != zfnull)
        {
            zfdelete(d->pathInfo);
            d->pathInfo = zfnull;
        }
    }
}
void ZFCallback::pathInfo(ZF_IN const zfchar *pathType, ZF_IN const zfchar *pathData)
{
    if(d == zfnull)
    {
        d = zfnew(_ZFP_ZFCallbackPrivate);
    }
    if(!zfsIsEmpty(pathType) || !zfsIsEmpty(pathData))
    {
        if(d->pathInfo == zfnull)
        {
            d->pathInfo = zfnew(ZFPathInfo, pathType, pathData);
        }
        else
        {
            d->pathInfo->pathType = pathType;
            d->pathInfo->pathData = pathData;
        }
    }
    else
    {
        if(d->pathInfo != zfnull)
        {
            zfdelete(d->pathInfo);
            d->pathInfo = zfnull;
        }
    }
}

_ZFP_ZFCallbackLambda *ZFCallback::_ZFP_ZFCallback_callbackLambdaImpl(void) const
{
    return d->callbackLambdaImpl;
}
ZFFuncAddrType ZFCallback::_ZFP_ZFCallback_callbackLambdaInvoker(void) const
{
    return d->callbackLambdaInvoker;
}

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_0(ZFCallback, ZFCallbackNull)
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(ZFCallback, ZFCallbackForMethod, ZFMP_IN(const ZFMethod *, zfmethod))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(ZFCallback, ZFCallbackForMemberMethod, ZFMP_IN(ZFObject *, obj), ZFMP_IN(const ZFMethod *, zfmethod))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(ZFCallback, ZFCallbackForFunc, ZFMP_IN(ZFFuncAddrType, callbackRawFunction))

ZF_NAMESPACE_GLOBAL_END
#endif

