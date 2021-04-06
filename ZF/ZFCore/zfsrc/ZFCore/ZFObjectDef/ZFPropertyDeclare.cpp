#include "ZFPropertyDeclare.h"
#include "ZFObjectImpl.h"
#include "ZFListenerDeclare.h"

ZF_NAMESPACE_GLOBAL_BEGIN

const ZFProperty *ZFPropertyForName(ZF_IN const zfchar *classNameOrFullName,
                                    ZF_IN const zfchar *propertyName)
{
    const ZFClass *cls = ZFClass::classForName(classNameOrFullName);
    if(cls != zfnull)
    {
        return cls->propertyForName(propertyName);
    }
    return zfnull;
}

// ============================================================
ZFCoreArrayPOD<_ZFP_PropLifeCycleData> &_ZFP_ZFPropertyLifeCycleDataRef(ZF_IN const zfchar *lifeCycleName,
                                                                        ZF_IN const ZFProperty *property)
{
    if(zffalse)
    {
    }
    else if(zfscmpTheSame(lifeCycleName, "OnInit"))
    {
        return property->_ZFP_ZFProperty_removeConst()->_ZFP_ZFPropertyLifeCycle_OnInit;
    }
    else if(zfscmpTheSame(lifeCycleName, "OnDealloc"))
    {
        return property->_ZFP_ZFProperty_removeConst()->_ZFP_ZFPropertyLifeCycle_OnDealloc;
    }
    else if(zfscmpTheSame(lifeCycleName, "OnVerify"))
    {
        return property->_ZFP_ZFProperty_removeConst()->_ZFP_ZFPropertyLifeCycle_OnVerify;
    }
    else if(zfscmpTheSame(lifeCycleName, "OnAttach"))
    {
        return property->_ZFP_ZFProperty_removeConst()->_ZFP_ZFPropertyLifeCycle_OnAttach;
    }
    else if(zfscmpTheSame(lifeCycleName, "OnDetach"))
    {
        return property->_ZFP_ZFProperty_removeConst()->_ZFP_ZFPropertyLifeCycle_OnDetach;
    }
    else if(zfscmpTheSame(lifeCycleName, "OnUpdate"))
    {
        return property->_ZFP_ZFProperty_removeConst()->_ZFP_ZFPropertyLifeCycle_OnUpdate;
    }

    zfCoreCriticalShouldNotGoHere();
    return property->_ZFP_ZFProperty_removeConst()->_ZFP_ZFPropertyLifeCycle_OnInit;
}
void _ZFP_ZFPropertyLifeCycleRegister(ZF_IN const zfchar *lifeCycleName,
                                      ZF_IN const ZFProperty *property,
                                      ZF_IN const ZFClass *propertyOwnerClass,
                                      ZF_IN _ZFP_PropLifeCycleWrapper propertyLifeCycleWrapper)
{
    ZFCoreArrayPOD<_ZFP_PropLifeCycleData> &d = _ZFP_ZFPropertyLifeCycleDataRef(lifeCycleName, property);
    for(zfindex i = 0; i < d.count(); ++i)
    {
        if(d[i].propertyOwnerClass == propertyOwnerClass)
        {
            d[i].propertyLifeCycleWrapper = propertyLifeCycleWrapper;
            return ;
        }
    }
    _ZFP_PropLifeCycleData data;
    data.propertyOwnerClass = propertyOwnerClass;
    data.propertyLifeCycleWrapper = propertyLifeCycleWrapper;
    d.add(data);
}
void _ZFP_ZFPropertyLifeCycleUnregister(ZF_IN const zfchar *lifeCycleName,
                                        ZF_IN const ZFProperty *property,
                                        ZF_IN const ZFClass *propertyOwnerClass)
{
    ZFCoreArrayPOD<_ZFP_PropLifeCycleData> &d = _ZFP_ZFPropertyLifeCycleDataRef(lifeCycleName, property);
    for(zfindex i = 0; i < d.count(); ++i)
    {
        if(d[i].propertyOwnerClass == propertyOwnerClass)
        {
            d.remove(i);
            break;
        }
    }
}

static void _ZFP_ZFPropertyLifeCycleCallAction(ZF_IN ZFCoreArrayPOD<_ZFP_PropLifeCycleData> const &d,
                                               ZF_IN ZFObject *propertyOwnerObject,
                                               ZF_IN void *propertyValue,
                                               ZF_IN const void *propertyValueOld,
                                               ZF_IN_OPT zfbool subclassFirst = zffalse)
{
    if(subclassFirst)
    {
        for(const _ZFP_PropLifeCycleData *p = d.arrayBuf() + d.count() - 1, *pEnd = d.arrayBuf() - 1; p != pEnd; --p)
        {
            if(propertyOwnerObject->classData()->classIsTypeOf(p->propertyOwnerClass))
            {
                p->propertyLifeCycleWrapper(propertyOwnerObject, propertyValue, propertyValueOld);
            }
        }
    }
    else
    {
        for(const _ZFP_PropLifeCycleData *p = d.arrayBuf(), *pEnd = d.arrayBuf() + d.count(); p != pEnd; ++p)
        {
            if(propertyOwnerObject->classData()->classIsTypeOf(p->propertyOwnerClass))
            {
                p->propertyLifeCycleWrapper(propertyOwnerObject, propertyValue, propertyValueOld);
            }
        }
    }
}

static void _ZFP_ZFPropertyWeakAttach(ZF_IN ZFObject *propertyOwnerObject,
                                      ZF_IN const ZFProperty *property,
                                      ZF_IN ZFObject *valueNew);
static void _ZFP_ZFPropertyWeakDetach(ZF_IN ZFObject *propertyOwnerObject,
                                      ZF_IN const ZFProperty *property,
                                      ZF_IN ZFObject *valueOld);

void _ZFP_ZFPropertyLifeCycleCall_init_retain(ZF_IN const ZFProperty *property,
                                              ZF_IN ZFObject *propertyOwnerObject,
                                              ZF_IN ZFObject *value,
                                              ZF_IN zfbool needNotifyOwner,
                                              ZF_IN void (*rawValueStoreCallback)(ZF_IN void *rawValueStoreToken, ZF_IN ZFObject *value),
                                              ZF_IN void *rawValueStoreToken)
{
    zfautoObject valueHolder = value;
    _ZFP_ZFPropertyLifeCycleCallAction(
        property->_ZFP_ZFPropertyLifeCycle_OnInit,
        propertyOwnerObject,
        &valueHolder,
        &valueHolder);
    rawValueStoreCallback(rawValueStoreToken, valueHolder);

    if(needNotifyOwner)
    {
        _ZFP_ZFPropertyLifeCycleCallAction(
            property->_ZFP_ZFPropertyLifeCycle_OnAttach,
            propertyOwnerObject,
            &valueHolder,
            &valueHolder);
        propertyOwnerObject->_ZFP_ZFObject_objectPropertyValueAttach(property);
        _ZFP_ZFPropertyLifeCycleCallAction(
            property->_ZFP_ZFPropertyLifeCycle_OnUpdate,
            propertyOwnerObject,
            &valueHolder,
            &valueHolder);
        propertyOwnerObject->_ZFP_ZFObject_objectPropertyValueOnUpdate(property, zfnull);
    }
}
void _ZFP_ZFPropertyLifeCycleCall_init_assign(ZF_IN const ZFProperty *property,
                                              ZF_IN ZFObject *propertyOwnerObject,
                                              ZF_IN void *value,
                                              ZF_IN zfbool needNotifyOwner,
                                              ZF_IN ZFObject *weakProp)
{
    _ZFP_ZFPropertyLifeCycleCallAction(
        property->_ZFP_ZFPropertyLifeCycle_OnInit,
        propertyOwnerObject,
        value,
        value);

    if(needNotifyOwner)
    {
        if(weakProp) {_ZFP_ZFPropertyWeakAttach(propertyOwnerObject, property, weakProp);}
        _ZFP_ZFPropertyLifeCycleCallAction(
            property->_ZFP_ZFPropertyLifeCycle_OnAttach,
            propertyOwnerObject,
            value,
            value);
        propertyOwnerObject->_ZFP_ZFObject_objectPropertyValueAttach(property);
        _ZFP_ZFPropertyLifeCycleCallAction(
            property->_ZFP_ZFPropertyLifeCycle_OnUpdate,
            propertyOwnerObject,
            value,
            value);
        propertyOwnerObject->_ZFP_ZFObject_objectPropertyValueOnUpdate(property, zfnull);
    }
}
void _ZFP_ZFPropertyLifeCycleCall_dealloc_retain(ZF_IN const ZFProperty *property,
                                                 ZF_IN ZFObject *propertyOwnerObject,
                                                 ZF_IN ZFObject *value,
                                                 ZF_IN zfbool completeDetach)
{
    zfautoObject valueHolder = value;
    _ZFP_ZFPropertyLifeCycleCallAction(
        property->_ZFP_ZFPropertyLifeCycle_OnDetach,
        propertyOwnerObject,
        &valueHolder,
        &valueHolder,
        zftrue);
    if(completeDetach) {propertyOwnerObject->_ZFP_ZFObject_objectPropertyValueDetach(property);}
    _ZFP_ZFPropertyLifeCycleCallAction(
        property->_ZFP_ZFPropertyLifeCycle_OnDealloc,
        propertyOwnerObject,
        &valueHolder,
        &valueHolder,
        zftrue);
}
void _ZFP_ZFPropertyLifeCycleCall_dealloc_assign(ZF_IN const ZFProperty *property,
                                                 ZF_IN ZFObject *propertyOwnerObject,
                                                 ZF_IN void *value,
                                                 ZF_IN zfbool completeDetach,
                                                 ZF_IN ZFObject *weakProp)
{
    if(weakProp) {_ZFP_ZFPropertyWeakDetach(propertyOwnerObject, property, weakProp);}
    _ZFP_ZFPropertyLifeCycleCallAction(
        property->_ZFP_ZFPropertyLifeCycle_OnDetach,
        propertyOwnerObject,
        value,
        value,
        zftrue);
    if(completeDetach) {propertyOwnerObject->_ZFP_ZFObject_objectPropertyValueDetach(property);}
    _ZFP_ZFPropertyLifeCycleCallAction(
        property->_ZFP_ZFPropertyLifeCycle_OnDealloc,
        propertyOwnerObject,
        value,
        value,
        zftrue);
}
void _ZFP_ZFPropertyLifeCycleCall_setter_retain(ZF_IN const ZFProperty *property,
                                                ZF_IN ZFObject *propertyOwnerObject,
                                                ZF_IN zfbool accessed,
                                                ZF_IN ZFObject *propertyValueOld,
                                                ZF_IN ZFObject *propertyValueNew,
                                                ZF_IN void (*rawValueStoreCallback)(ZF_IN void *rawValueStoreToken, ZF_IN ZFObject *value),
                                                ZF_IN void *rawValueStoreToken)
{
    zfautoObject valueOld = propertyValueOld;
    zfautoObject valueNew = propertyValueNew;
    if(accessed)
    {
        _ZFP_ZFPropertyLifeCycleCallAction(
            property->_ZFP_ZFPropertyLifeCycle_OnDetach,
            propertyOwnerObject,
            &valueNew,
            &valueOld,
            zftrue);

        _ZFP_ZFPropertyLifeCycleCallAction(
            property->_ZFP_ZFPropertyLifeCycle_OnDealloc,
            propertyOwnerObject,
            &valueNew,
            &valueOld,
            zftrue);
    }

    rawValueStoreCallback(rawValueStoreToken, valueNew);
    _ZFP_ZFPropertyLifeCycleCallAction(
        property->_ZFP_ZFPropertyLifeCycle_OnVerify,
        propertyOwnerObject,
        &valueNew,
        &valueOld);
    rawValueStoreCallback(rawValueStoreToken, valueNew);

    _ZFP_ZFPropertyLifeCycleCallAction(
        property->_ZFP_ZFPropertyLifeCycle_OnAttach,
        propertyOwnerObject,
        &valueNew,
        &valueOld);
    if(!accessed) {propertyOwnerObject->_ZFP_ZFObject_objectPropertyValueAttach(property);}

    _ZFP_ZFPropertyLifeCycleCallAction(
        property->_ZFP_ZFPropertyLifeCycle_OnUpdate,
        propertyOwnerObject,
        &valueNew,
        &valueOld);
    propertyOwnerObject->_ZFP_ZFObject_objectPropertyValueOnUpdate(property, &propertyValueOld);
}
void _ZFP_ZFPropertyLifeCycleCall_setter_assign(ZF_IN const ZFProperty *property,
                                                ZF_IN ZFObject *propertyOwnerObject,
                                                ZF_IN zfbool accessed,
                                                ZF_IN void *propertyValueOld,
                                                ZF_IN const void *propertyValueNew,
                                                ZF_IN void *(*rawValueStoreCallback)(ZF_IN void *rawValueStoreToken, ZF_IN const void *value),
                                                ZF_IN void *rawValueStoreToken,
                                                ZF_IN ZFObject *weakPropOld,
                                                ZF_IN ZFObject *weakPropNew)
{
    if(accessed)
    {
        if(weakPropOld) {_ZFP_ZFPropertyWeakDetach(propertyOwnerObject, property, weakPropOld);}
        _ZFP_ZFPropertyLifeCycleCallAction(
            property->_ZFP_ZFPropertyLifeCycle_OnDetach,
            propertyOwnerObject,
            propertyValueOld,
            propertyValueOld,
            zftrue);
        _ZFP_ZFPropertyLifeCycleCallAction(
            property->_ZFP_ZFPropertyLifeCycle_OnDealloc,
            propertyOwnerObject,
            propertyValueOld,
            propertyValueOld,
            zftrue);
    }
    void *valueRef = rawValueStoreCallback(rawValueStoreToken, propertyValueNew);
    _ZFP_ZFPropertyLifeCycleCallAction(
        property->_ZFP_ZFPropertyLifeCycle_OnVerify,
        propertyOwnerObject,
        valueRef,
        propertyValueOld);

    if(weakPropNew) {_ZFP_ZFPropertyWeakAttach(propertyOwnerObject, property, weakPropNew);}
    _ZFP_ZFPropertyLifeCycleCallAction(
        property->_ZFP_ZFPropertyLifeCycle_OnAttach,
        propertyOwnerObject,
        valueRef,
        propertyValueOld);
    if(!accessed) {propertyOwnerObject->_ZFP_ZFObject_objectPropertyValueAttach(property);}
    _ZFP_ZFPropertyLifeCycleCallAction(
        property->_ZFP_ZFPropertyLifeCycle_OnUpdate,
        propertyOwnerObject,
        valueRef,
        propertyValueOld);
    propertyOwnerObject->_ZFP_ZFObject_objectPropertyValueOnUpdate(property, propertyValueOld);
}

// ============================================================
zfclass _ZFP_I_ZFPropertyWeakTaskData : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_I_ZFPropertyWeakTaskData, ZFObject)

public:
    ZFObject *propertyOwnerObject;
    const ZFProperty *property;
};
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFPropertyWeakDataHolder, ZFLevelZFFrameworkEssential)
{
    this->propertyOnDeallocListener = ZFCallbackForFunc(zfself::propertyOnDealloc);
}
public:
    ZFListener propertyOnDeallocListener;
    static void propertyOnDealloc(ZF_IN const ZFListenerData &listenerData, ZF_IN ZFObject *userData)
    {
        _ZFP_I_ZFPropertyWeakTaskData *taskData = userData->to<_ZFP_I_ZFPropertyWeakTaskData *>();
        taskData->property->callbackValueReset(
            taskData->property,
            taskData->propertyOwnerObject);
    }
ZF_GLOBAL_INITIALIZER_END(ZFPropertyWeakDataHolder)
static ZFCompareResult _ZFP_ZFPropertyWeakUserDataComparer(ZF_IN ZFObject *const &v0, ZF_IN ZFObject *const &v1)
{
    _ZFP_I_ZFPropertyWeakTaskData *v0_ = ZFCastZFObject(_ZFP_I_ZFPropertyWeakTaskData *, v0);
    _ZFP_I_ZFPropertyWeakTaskData *v1_ = ZFCastZFObject(_ZFP_I_ZFPropertyWeakTaskData *, v1);
    if(v0_ != zfnull && v1_ != zfnull
        && v0_->propertyOwnerObject == v1_->propertyOwnerObject
        && v0_->property == v1_->property
        )
    {
        return ZFCompareTheSame;
    }
    else
    {
        return ZFCompareUncomparable;
    }
}
static void _ZFP_ZFPropertyWeakAttach(ZF_IN ZFObject *propertyOwnerObject,
                                      ZF_IN const ZFProperty *property,
                                      ZF_IN ZFObject *valueNew)
{
    zfblockedAlloc(_ZFP_I_ZFPropertyWeakTaskData, taskData);
    taskData->propertyOwnerObject = propertyOwnerObject;
    taskData->property = property;
    valueNew->observerAdd(
        ZFObject::EventObjectBeforeDealloc(),
        ZF_GLOBAL_INITIALIZER_INSTANCE(ZFPropertyWeakDataHolder)->propertyOnDeallocListener,
        taskData);
}
static void _ZFP_ZFPropertyWeakDetach(ZF_IN ZFObject *propertyOwnerObject,
                                      ZF_IN const ZFProperty *property,
                                      ZF_IN ZFObject *valueOld)
{
    zfblockedAlloc(_ZFP_I_ZFPropertyWeakTaskData, taskData);
    taskData->propertyOwnerObject = propertyOwnerObject;
    taskData->property = property;
    valueOld->observerRemove(
        ZFObject::EventObjectBeforeDealloc(),
        ZF_GLOBAL_INITIALIZER_INSTANCE(ZFPropertyWeakDataHolder)->propertyOnDeallocListener,
        taskData,
        _ZFP_ZFPropertyWeakUserDataComparer);
}

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(const ZFProperty *, ZFPropertyForName, ZFMP_IN(const zfchar *, classNameOrFullName), ZFMP_IN(const zfchar *, propertyName))

ZF_NAMESPACE_GLOBAL_END
#endif

