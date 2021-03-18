#include "ZFCallbackSerializable.h"
#include "ZFObjectImpl.h"

#include "../ZFSTLWrapper/zfstl_string.h"
#include "../ZFSTLWrapper/zfstl_map.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// serialize routine
ZFTYPEID_DEFINE_BY_SERIALIZABLE_CONVERTER(ZFCallback, ZFCallback, {
        { // custom serialize logic
            const zfchar *customType = ZFSerializableUtil::checkAttribute(serializableData, ZFSerializableKeyword_ZFCallback_callbackType);
            if(customType != zfnull)
            {
                const ZFSerializableData *customData = ZFSerializableUtil::checkElementByCategory(serializableData, ZFSerializableKeyword_ZFCallback_callbackData);
                if(customData == zfnull)
                {
                    ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
                        "missing %s", ZFSerializableKeyword_ZFCallback_callbackData);
                    return zffalse;
                }

                _ZFP_ZFCallbackSerializeCustomCallback serializeCallback = _ZFP_ZFCallbackSerializeCustomTypeForName(customType);
                if(serializeCallback == zfnull)
                {
                    ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
                        "no such callback custom serialize type: %s", customType);
                    return zffalse;
                }
                if(!serializeCallback(v, *customData, outErrorHint, outErrorPos))
                {
                    return zffalse;
                }
                v.callbackSerializeCustomType(customType);
                v.callbackSerializeCustomData(customData);

                serializableData.resolveMark();
                return zftrue;
            }
        }

        if(zfscmpTheSame(serializableData.itemClass(), ZFSerializableKeyword_null))
        {
            v = ZFCallbackNull();
            serializableData.resolveMark();
            return zftrue;
        }
        if(ZFSerializableUtil::requireItemClass(serializableData, ZFTypeId_ZFCallback(), outErrorHint, outErrorPos) == zfnull)
        {
            return zffalse;
        }

        const ZFSerializableData *methodData = ZFSerializableUtil::requireElementByCategory(serializableData, ZFSerializableKeyword_ZFCallback_method, outErrorHint, outErrorPos);
        if(methodData == zfnull)
        {
            return zffalse;
        }
        const ZFMethod *method = zfnull;
        if(!ZFMethodFromData(method, *methodData, outErrorHint, outErrorPos))
        {
            return zffalse;
        }

        if(method->methodOwnerClass() == zfnull)
        {
            v = ZFCallbackForMethod(method);
        }
        else if(method->methodType() == ZFMethodTypeStatic)
        {
            v = ZFCallbackForMethod(method);
        }
        else
        {
            ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
                "member method callback is not supported");
            return zffalse;
        }

        serializableData.resolveMark();
        return zftrue;
    }, {
        if(v.callbackSerializeCustomType() != zfnull)
        {
            if(v.callbackSerializeCustomDisabled())
            {
                ZFSerializableUtil::errorOccurred(outErrorHint, "callback was marked as not serializable");
                return zffalse;
            }

            if(v.callbackSerializeCustomData() == zfnull)
            {
                ZFSerializableUtil::errorOccurred(outErrorHint, "missing callback serialize custom data");
                return zffalse;
            }
            serializableData.itemClass(ZFTypeId_ZFCallback());

            serializableData.attributeForName(ZFSerializableKeyword_ZFCallback_callbackType, v.callbackSerializeCustomType());

            ZFSerializableData customData = v.callbackSerializeCustomData()->copy();
            customData.category(ZFSerializableKeyword_ZFCallback_callbackData);
            serializableData.elementAdd(customData);

            return zftrue;
        }

        switch(v.callbackType())
        {
            case ZFCallbackTypeDummy:
                serializableData.itemClass(ZFSerializableKeyword_null);
                break;
            case ZFCallbackTypeMethod:
            {
                serializableData.itemClass(ZFTypeId_ZFCallback());
                ZFSerializableData methodData;
                if(!ZFMethodToData(methodData, v.callbackMethod(), outErrorHint))
                {
                    return zffalse;
                }
                methodData.category(ZFSerializableKeyword_ZFCallback_method);
                serializableData.elementAdd(methodData);
            }
                break;
            case ZFCallbackTypeMemberMethod:
            {
                ZFSerializableUtil::errorOccurred(outErrorHint,
                    "member method callback is not supported");
                return zffalse;
            }
                break;
            case ZFCallbackTypeRawFunction:
                serializableData.itemClass(ZFTypeId_ZFCallback());
                ZFSerializableUtil::errorOccurred(outErrorHint,
                    "raw function is not supported");
                return zffalse;
            default:
                zfCoreCriticalShouldNotGoHere();
                return zffalse;
        }

        return zftrue;
    })
ZFMETHOD_USER_REGISTER_0({
        ZFListenerData listenerData;
        return ZFListener(invokerObject->to<v_ZFCallback *>()->zfv).execute(listenerData, zfnull);
    }, v_ZFCallback,
    void, execute)
ZFMETHOD_USER_REGISTER_2({
        return ZFListener(invokerObject->to<v_ZFCallback *>()->zfv).execute(listenerData, userData);
    }, v_ZFCallback,
    void, execute
    , ZFMP_IN(const ZFListenerData &, listenerData)
    , ZFMP_IN_OPT(ZFObject *, userData, zfnull)
    )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFCallback, void, callbackId, ZFMP_IN(const zfchar *, callbackId))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, const zfchar *, callbackId)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFCallback, void, callbackTag, ZFMP_IN(const zfchar *, key), ZFMP_IN(ZFObject *, tag))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFCallback, ZFObject *, callbackTag, ZFMP_IN(const zfchar *, key))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFCallback, void, callbackTagGetAllKeyValue, ZFMP_IN_OUT(ZFCoreArray<const zfchar *> &, allKey), ZFMP_IN_OUT(ZFCoreArray<ZFObject *> &, allValue))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFCallback, void, callbackTagRemove, ZFMP_IN(const zfchar *, key))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFCallback, zfautoObject, callbackTagRemoveAndGet, ZFMP_IN(const zfchar *, key))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, void, callbackTagRemoveAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, zfbool, callbackIsValid)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, ZFCallbackType, callbackType)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, ZFObject *, callbackOwnerObject)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, const ZFMethod *, callbackMethod)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, ZFFuncAddrType, callbackRawFunction)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, void, callbackClear)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, void, callbackOwnerObjectRetain)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, void, callbackOwnerObjectRelease)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFCallback, void, callbackSerializeCustomType, ZFMP_IN(const zfchar *, customType))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, const zfchar *, callbackSerializeCustomType)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFCallback, void, callbackSerializeCustomData, ZFMP_IN(const ZFSerializableData &, customData))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, const ZFSerializableData *, callbackSerializeCustomData)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFCallback, void, callbackSerializeCustomDisable, ZFMP_IN(zfbool, disable))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, zfbool, callbackSerializeCustomDisabled)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallback, const ZFPathInfo *, pathInfo)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFCallback, void, pathInfo, ZFMP_IN(const ZFPathInfo *, pathInfo))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFCallback, void, pathInfo, ZFMP_IN(const zfchar *, pathType), ZFMP_IN(const zfchar *, pathData))

// ============================================================
ZFTYPEID_ALIAS_DEFINE(ZFCallback, ZFCallback, ZFListener, ZFListener)
ZFTYPEID_ALIAS_DEFINE(ZFCallback, ZFCallback, ZFIOCallback, ZFIOCallback)
ZFTYPEID_ALIAS_DEFINE(ZFCallback, ZFCallback, ZFOutput, ZFOutput)
ZFTYPEID_ALIAS_DEFINE(ZFCallback, ZFCallback, ZFInput, ZFInput)

// ============================================================
// custom serialize logic
static zfstlmap<zfstlstringZ, _ZFP_ZFCallbackSerializeCustomCallback> &_ZFP_ZFCallbackSerializeCustomCallbackMap(void)
{
    static zfstlmap<zfstlstringZ, _ZFP_ZFCallbackSerializeCustomCallback> d;
    return d;
}
void _ZFP_ZFCallbackSerializeCustomTypeRegister(ZF_IN const zfchar *customType,
                                                ZF_IN _ZFP_ZFCallbackSerializeCustomCallback serializeCallback)
{
    zfstlmap<zfstlstringZ, _ZFP_ZFCallbackSerializeCustomCallback> &m = _ZFP_ZFCallbackSerializeCustomCallbackMap();
    zfCoreAssert(!zfsIsEmpty(customType) && serializeCallback != zfnull);
    zfCoreAssertWithMessage(m.find(customType) == m.end(), "custom callback serialize type \"%s\" already registered", customType);

    m[customType] = serializeCallback;
}
void _ZFP_ZFCallbackSerializeCustomTypeUnregister(ZF_IN const zfchar *customType)
{
    _ZFP_ZFCallbackSerializeCustomCallbackMap().erase(customType);
}
_ZFP_ZFCallbackSerializeCustomCallback _ZFP_ZFCallbackSerializeCustomTypeForName(ZF_IN const zfchar *customType)
{
    zfstlmap<zfstlstringZ, _ZFP_ZFCallbackSerializeCustomCallback> &m = _ZFP_ZFCallbackSerializeCustomCallbackMap();
    zfstlmap<zfstlstringZ, _ZFP_ZFCallbackSerializeCustomCallback>::iterator it = m.find(customType);
    if(it != m.end())
    {
        return it->second;
    }
    else
    {
        return zfnull;
    }
}

ZF_NAMESPACE_GLOBAL_END

