#include "ZFStyleable.h"
#include "ZFObjectImpl.h"
#include "ZFClassUtil.h"
#include "ZFPropertyUtil.h"
#include "ZFListenerDeclare.h"

#include "../ZFSTLWrapper/zfstl_string.h"
#include "../ZFSTLWrapper/zfstl_map.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFStyleable *ZFStyleable::defaultStyle(void)
{
    const ZFMethod *method = this->classData()->methodForName("DefaultStyle");
    if(method != zfnull)
    {
        return method->methodGenericInvoke(zfnull);
    }
    else
    {
        return zfnull;
    }
}

// ============================================================
zfclass _ZFP_I_ZFStyleable_PropertyTypeHolder : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_I_ZFStyleable_PropertyTypeHolder, ZFObject)

public:
    ZFCoreArrayPOD<const ZFProperty *> normalProperty;
    ZFCoreArrayPOD<const ZFProperty *> styleableProperty;
    ZFCoreArrayPOD<const ZFProperty *> copyableProperty;
};

// ============================================================
void ZFStyleable::styleableCopyFrom(ZF_IN ZFStyleable *anotherStyleable)
{
    if(anotherStyleable == zfnull)
    {
        return ;
    }
    if(this->styleKey() == zfnull)
    {
        this->styleKey(anotherStyleable->styleKey());
    }

    _ZFP_I_ZFStyleable_PropertyTypeHolder *holderTmp = this->_ZFP_ZFStyleable_getPropertyTypeHolder();
    const ZFClass *thisCls = this->classData();
    const ZFClass *anotherCls = anotherStyleable->classData();
    ZFObject *anotherStyleableObject = anotherStyleable->toObject();;
    const ZFProperty *property = zfnull;

    for(zfindex i = holderTmp->normalProperty.count() - 1; i != zfindexMax(); --i)
    {
        property = holderTmp->normalProperty[i];
        if(!anotherCls->classIsTypeOf(property->propertyOwnerClass()))
        {
            continue;
        }
        if(!thisCls->_ZFP_ZFClass_propertyInitStepIsTheSame(property, anotherCls)
            || property->callbackIsValueAccessed(property, anotherStyleableObject))
        {
            this->styleableOnCopyPropertyFrom(anotherStyleable, property, ZFStyleable::PropertyTypeNormal);
        }
    }
    for(zfindex i = holderTmp->styleableProperty.count() - 1; i != zfindexMax(); --i)
    {
        property = holderTmp->styleableProperty[i];
        if(!anotherCls->classIsTypeOf(property->propertyOwnerClass()))
        {
            continue;
        }
        if(!thisCls->_ZFP_ZFClass_propertyInitStepIsTheSame(property, anotherCls)
            || property->callbackIsValueAccessed(property, anotherStyleableObject))
        {
            this->styleableOnCopyPropertyFrom(anotherStyleable, property, ZFStyleable::PropertyTypeStyleable);
        }
    }
    for(zfindex i = holderTmp->copyableProperty.count() - 1; i != zfindexMax(); --i)
    {
        property = holderTmp->copyableProperty[i];
        if(!anotherCls->classIsTypeOf(property->propertyOwnerClass()))
        {
            continue;
        }
        if(!thisCls->_ZFP_ZFClass_propertyInitStepIsTheSame(property, anotherCls)
            || property->callbackIsValueAccessed(property, anotherStyleableObject))
        {
            this->styleableOnCopyPropertyFrom(anotherStyleable, property, ZFStyleable::PropertyTypeCopyable);
        }
    }

    this->styleableOnCopyFrom(anotherStyleable);
}
ZFStyleable::PropertyType ZFStyleable::styleableOnCheckPropertyType(ZF_IN const ZFProperty *property)
{
    if(property->propertyIsRetainProperty()
        && property->setterMethod()->methodPrivilegeType() == ZFMethodPrivilegeTypePrivate
        && property->getterMethod()->methodPrivilegeType() != ZFMethodPrivilegeTypePrivate)
    {
        if(property->propertyClassOfRetainProperty()->classIsTypeOf(ZFStyleable::ClassData()))
        {
            return ZFStyleable::PropertyTypeStyleable;
        }
        else if(property->propertyClassOfRetainProperty()->classIsTypeOf(ZFCopyable::ClassData()))
        {
            return ZFStyleable::PropertyTypeCopyable;
        }
    }
    if(property->setterMethod()->methodPrivilegeType() == ZFMethodPrivilegeTypePrivate
        || property->getterMethod()->methodPrivilegeType() == ZFMethodPrivilegeTypePrivate)
    {
        return ZFStyleable::PropertyTypeNotStyleable;
    }
    return ZFStyleable::PropertyTypeNormal;
}

void ZFStyleable::styleableOnCopyPropertyFrom(ZF_IN ZFStyleable *anotherStyleable,
                                              ZF_IN const ZFProperty *property,
                                              ZF_IN ZFStyleable::PropertyType propertyType)
{
    if(this->styleKeyForProperty(property) == zfnull)
    {
        this->styleKeyForProperty(property, anotherStyleable->styleKeyForProperty(property));
    }
    switch(propertyType)
    {
        case ZFStyleable::PropertyTypeNormal:
        {
            ZFPropertyCopy(property, this->toObject(), anotherStyleable->toObject());
        }
            break;
        case ZFStyleable::PropertyTypeStyleable:
        {
            ZFStyleable *selfPropertyValue = property->getterMethod()->methodGenericInvoke(this->toObject());
            ZFStyleable *anotherPropertyValue = property->getterMethod()->methodGenericInvoke(anotherStyleable->toObject());
            if(selfPropertyValue != zfnull && anotherPropertyValue != zfnull)
            {
                selfPropertyValue->styleableCopyFrom(anotherPropertyValue);
            }
        }
            break;
        case ZFStyleable::PropertyTypeCopyable:
        {
            ZFCopyable *selfPropertyValue = property->getterMethod()->methodGenericInvoke(this->toObject());
            ZFObject *anotherPropertyValue = property->getterMethod()->methodGenericInvoke(anotherStyleable->toObject());
            if(selfPropertyValue != zfnull && anotherPropertyValue != zfnull)
            {
                selfPropertyValue->copyFrom(anotherPropertyValue);
            }
        }
            break;
        case ZFStyleable::PropertyTypeNotStyleable:
        default:
            zfCoreCriticalShouldNotGoHere();
            return ;
    }
}

_ZFP_I_ZFStyleable_PropertyTypeHolder *ZFStyleable::_ZFP_ZFStyleable_getPropertyTypeHolder(void)
{
    zfCoreMutexLocker();
    _ZFP_I_ZFStyleable_PropertyTypeHolder *holder = this->classData()->classTag<_ZFP_I_ZFStyleable_PropertyTypeHolder *>(
        _ZFP_I_ZFStyleable_PropertyTypeHolder::ClassData()->classNameFull());
    if(holder == zfnull)
    {
        zflockfree_zfblockedAlloc(_ZFP_I_ZFStyleable_PropertyTypeHolder, holderTmp);

        const ZFCoreArrayPOD<const ZFProperty *> allProperty = ZFClassUtil::allProperty(this->classData());
        const ZFProperty *propertyTmp = zfnull;
        for(zfindex i = 0; i < allProperty.count(); ++i)
        {
            propertyTmp = allProperty[i];
            switch(this->styleableOnCheckPropertyType(propertyTmp))
            {
                case ZFStyleable::PropertyTypeNotStyleable:
                    break;
                case ZFStyleable::PropertyTypeNormal:
                    holderTmp->normalProperty.add(propertyTmp);
                    break;
                case ZFStyleable::PropertyTypeStyleable:
                    holderTmp->styleableProperty.add(propertyTmp);
                    break;
                case ZFStyleable::PropertyTypeCopyable:
                    holderTmp->copyableProperty.add(propertyTmp);
                    break;
            }
        }

        holder = holderTmp;
        this->classData()->classTag(
            _ZFP_I_ZFStyleable_PropertyTypeHolder::ClassData()->classNameFull(),
            holderTmp);
        this->ClassData()->classDataChangeAutoRemoveTagAdd(_ZFP_I_ZFStyleable_PropertyTypeHolder::ClassData()->classNameFull());
    }
    return holder;
}

// ============================================================
ZFOBJECT_REGISTER(ZFStyleableObject)

// ============================================================
ZF_STATIC_INITIALIZER_INIT(ZFStyleableDefaultStyleDataHolder)
{
}
ZFCoreMap instanceDataMap; // _ZFP_ZFStyleableDefaultPointerHolder *
ZF_STATIC_INITIALIZER_END(ZFStyleableDefaultStyleDataHolder)

_ZFP_ZFStyleableDefaultPointerHolder *_ZFP_ZFStyleableDefaultRefAccess(ZF_IN const zfchar *name)
{
    zfCoreMutexLocker();
    ZFCoreMap &m = ZF_STATIC_INITIALIZER_INSTANCE(ZFStyleableDefaultStyleDataHolder)->instanceDataMap;
    ZFCorePointerBase *v = m.get(name);
    if(v == zfnull)
    {
        m.set(name, ZFCorePointerForObject<_ZFP_ZFStyleableDefaultPointerHolder *>(zfnew(_ZFP_ZFStyleableDefaultPointerHolder)));
        v = m.get(name);
    }
    return v->pointerValueT<_ZFP_ZFStyleableDefaultPointerHolder *>();
}

// ============================================================
zfclass _ZFP_I_ZFStyleDefaultApplyAutoCopyTaskData : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_I_ZFStyleDefaultApplyAutoCopyTaskData, ZFObject)

public:
    ZFCoreArrayPOD<ZFObjectHolder *> styles;
};
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFStyleDefaultApplyAutoCopyDataHolder, ZFLevelZFFrameworkEssential)
{
    this->styleOnDeallocListener = ZFCallbackForFunc(zfself::styleOnDealloc);
    this->defaultStyleOnChangeListener = ZFCallbackForFunc(zfself::defaultStyleOnChange);
}
public:
    ZFListener styleOnDeallocListener;
    ZFListener defaultStyleOnChangeListener;
public:
    static ZFLISTENER_PROTOTYPE_EXPAND(styleOnDealloc)
    {
        zfCoreMutexLocker();

        ZFStyleable *defaultStyle = listenerData.sender<ZFStyleable *>()->defaultStyle();
        zfCoreAssert(defaultStyle != zfnull);
        _ZFP_I_ZFStyleDefaultApplyAutoCopyTaskData *taskData = defaultStyle->toObject()
            ->objectTag<_ZFP_I_ZFStyleDefaultApplyAutoCopyTaskData *>(_ZFP_I_ZFStyleDefaultApplyAutoCopyTaskData::ClassData()->classNameFull());
        zfCoreAssert(taskData != zfnull);

        taskData->styles.removeElement(listenerData.sender()->objectHolder());

        if(taskData->styles.isEmpty())
        {
            defaultStyle->toObject()->objectTagRemove(_ZFP_I_ZFStyleDefaultApplyAutoCopyTaskData::ClassData()->classNameFull());
            defaultStyle->toObject()->observerRemove(ZFObject::EventObjectPropertyValueOnUpdate(),
                ZF_GLOBAL_INITIALIZER_INSTANCE(ZFStyleDefaultApplyAutoCopyDataHolder)->defaultStyleOnChangeListener);
        }
    }
    static ZFLISTENER_PROTOTYPE_EXPAND(defaultStyleOnChange)
    {
        zfCoreMutexLocker();

        const ZFProperty *property = listenerData.param0<v_ZFProperty *>()->zfv;
        ZFStyleable *defaultStyle = listenerData.sender<ZFStyleable *>();
        _ZFP_I_ZFStyleDefaultApplyAutoCopyTaskData *taskData = defaultStyle->toObject()
            ->objectTag<_ZFP_I_ZFStyleDefaultApplyAutoCopyTaskData *>(_ZFP_I_ZFStyleDefaultApplyAutoCopyTaskData::ClassData()->classNameFull());

        ZFCoreArrayPOD<ZFObjectHolder *> styles;
        styles.copyFrom(taskData->styles);
        for(zfindex i = 0; i < styles.count(); ++i)
        {
            ZFPropertyCopy(property, styles[i]->objectHolded(), listenerData.sender());
        }
    }
ZF_GLOBAL_INITIALIZER_END(ZFStyleDefaultApplyAutoCopyDataHolder)

void ZFStyleDefaultApplyAutoCopy(ZF_IN ZFStyleable *style)
{
    zfCoreMutexLocker();
    if(style != zfnull && !style->styleableIsDefaultStyle())
    {
        ZFStyleable *defaultStyle = style->defaultStyle();
        if(defaultStyle != zfnull)
        {
            _ZFP_I_ZFStyleDefaultApplyAutoCopyTaskData *taskData = defaultStyle->toObject()
                ->objectTag<_ZFP_I_ZFStyleDefaultApplyAutoCopyTaskData *>(_ZFP_I_ZFStyleDefaultApplyAutoCopyTaskData::ClassData()->classNameFull());
            if(taskData == zfnull)
            {
                taskData = zfAlloc(_ZFP_I_ZFStyleDefaultApplyAutoCopyTaskData);
                defaultStyle->toObject()->objectTag(_ZFP_I_ZFStyleDefaultApplyAutoCopyTaskData::ClassData()->classNameFull(), taskData);
                zfRelease(taskData);

                defaultStyle->toObject()->observerAdd(ZFObject::EventObjectPropertyValueOnUpdate(),
                    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFStyleDefaultApplyAutoCopyDataHolder)->defaultStyleOnChangeListener);
            }
            taskData->styles.add(style->toObject()->objectHolder());
            style->toObject()->observerAdd(ZFObject::EventObjectBeforeDealloc(),
                ZF_GLOBAL_INITIALIZER_INSTANCE(ZFStyleDefaultApplyAutoCopyDataHolder)->styleOnDeallocListener);
        }
    }
}

// ============================================================
// style holder
static zfbool _ZFP_ZFStyleChangeFlag = zffalse;
static zfstlmap<zfstlstringZ, zfautoObject> &_ZFP_ZFStyleHolder(void)
{
    static zfstlmap<zfstlstringZ, zfautoObject> d;
    return d;
}
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFStyleCleanup, ZFLevelZFFrameworkNormal)
{
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFStyleCleanup)
{
    _ZFP_ZFStyleHolder().clear();
}
ZF_GLOBAL_INITIALIZER_END(ZFStyleCleanup)

static zfstlmap<zfstlstringZ, _ZFP_ZFStyleDecoder> &_ZFP_ZFStyleDecoderMap(void)
{
    static zfstlmap<zfstlstringZ, _ZFP_ZFStyleDecoder> d;
    return d;
}
void _ZFP_ZFStyleDecoderRegister(ZF_IN const zfchar *registerSig,
                                 ZF_IN _ZFP_ZFStyleDecoder decoder)
{
    zfCoreMutexLocker();
    zfstlmap<zfstlstringZ, _ZFP_ZFStyleDecoder> &m = _ZFP_ZFStyleDecoderMap();
    zfCoreAssert(registerSig != zfnull && decoder != zfnull);
    zfCoreAssertWithMessageTrim(m.find(registerSig) == m.end(),
        "[ZFSTYLE_DECODER_DEFINE] %s already registered",
        registerSig);
    m[registerSig] = decoder;
}
void _ZFP_ZFStyleDecoderUnregister(ZF_IN const zfchar *registerSig)
{
    zfCoreMutexLocker();
    zfstlmap<zfstlstringZ, _ZFP_ZFStyleDecoder> &m = _ZFP_ZFStyleDecoderMap();
    m.erase(registerSig);
}

void ZFStyleSet(ZF_IN const zfchar *styleKey, ZF_IN ZFStyleable *styleValue)
{
    if(styleKey && styleValue)
    {
        zfCoreMutexLock();
        _ZFP_ZFStyleChangeFlag = zftrue;
        _ZFP_ZFStyleHolder()[styleKey] = styleValue;
        zfCoreMutexUnlock();
    }
}
zfautoObject ZFStyleGet(ZF_IN const zfchar *styleKey)
{
    if(styleKey == zfnull)
    {
        return zfnull;
    }

    zfCoreMutexLocker();
    zfautoObject ret;
    zfstlmap<zfstlstringZ, _ZFP_ZFStyleDecoder> &m = _ZFP_ZFStyleDecoderMap();
    for(zfstlmap<zfstlstringZ, _ZFP_ZFStyleDecoder>::iterator it = m.begin(); it != m.end(); ++it)
    {
        if(it->second(ret, styleKey))
        {
            return ret;
        }
    }

    zfstlmap<zfstlstringZ, zfautoObject> &d = _ZFP_ZFStyleHolder();
    zfstlmap<zfstlstringZ, zfautoObject>::iterator it = d.find(styleKey);
    if(it != d.end())
    {
        return it->second;
    }
    else
    {
        return zfnull;
    }
}
void ZFStyleGetAll(ZF_IN_OUT ZFCoreArrayPOD<const zfchar *> &styleKey,
                   ZF_IN_OUT ZFCoreArrayPOD<ZFStyleable *> &styleValue)
{
    zfCoreMutexLocker();
    zfstlmap<zfstlstringZ, zfautoObject> &d = _ZFP_ZFStyleHolder();
    for(zfstlmap<zfstlstringZ, zfautoObject>::iterator it = d.begin(); it != d.end(); ++it)
    {
        styleKey.add(it->first.c_str());
        styleValue.add(it->second);
    }
}
void ZFStyleRemoveAll(void)
{
    zfCoreMutexLock();
    zfstlmap<zfstlstringZ, zfautoObject> d;
    _ZFP_ZFStyleChangeFlag = zftrue;
    d.swap(_ZFP_ZFStyleHolder());
    zfCoreMutexUnlock();
}

static zfint _ZFP_ZFStyleChangeBeginFlag = 0;
void ZFStyleChangeBegin()
{
    zfCoreMutexLock();
    ++_ZFP_ZFStyleChangeBeginFlag;
    if(_ZFP_ZFStyleChangeBeginFlag == 1)
    {
        _ZFP_ZFStyleChangeFlag = zffalse;
    }
    zfCoreMutexUnlock();
}
void ZFStyleChangeEnd()
{
    zfCoreAssertWithMessageTrim(_ZFP_ZFStyleChangeBeginFlag != 0,
        "ZFStyleChangeBegin/ZFStyleChangeEnd not paired");

    zfCoreMutexLock();
    --_ZFP_ZFStyleChangeBeginFlag;
    zfbool needNotify = (_ZFP_ZFStyleChangeBeginFlag == 0 && _ZFP_ZFStyleChangeFlag);
    zfCoreMutexUnlock();

    if(needNotify)
    {
        ZFGlobalObserver().observerNotify(ZFGlobalEvent::EventZFStyleOnChange());
    }
}

ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
ZFOBSERVER_EVENT_GLOBAL_REGISTER(ZFStyleOnChange)
ZFOBSERVER_EVENT_GLOBAL_REGISTER(ZFStyleOnInvalid)
ZF_NAMESPACE_END(ZFGlobalEvent)

static zfbool _ZFP_ZFStyleInvalidCheckDisableFlag = zffalse;
void ZFStyleInvalidCheckDisable(ZF_IN zfbool enable)
{
    _ZFP_ZFStyleInvalidCheckDisableFlag = zftrue;
}
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFStyleInvalidAssert, ZFLevelZFFrameworkHigh)
{
    ZFLISTENER_LOCAL(action, {
            if(_ZFP_ZFStyleInvalidCheckDisableFlag) {return ;}
            const zfchar *propertyName = listenerData.param0<v_zfstring *>()->zfv;
            const zfchar *styleKey = listenerData.param1<v_zfstring *>()->zfv;
            if(zfsIsEmpty(propertyName))
            {
                zfCoreCriticalMessageTrim(
                    "[ZFStyle] %s unable to apply style \"%s\"",
                    listenerData.sender()->objectInfoOfInstance().cString(),
                    styleKey);
            }
            else
            {
                zfCoreCriticalMessageTrim(
                    "[ZFStyle] %s unable to apply style \"%s\" for property \"%s\"",
                    listenerData.sender()->objectInfoOfInstance().cString(),
                    styleKey,
                    propertyName);
            }
        })
    this->taskId = ZFGlobalObserver().observerAdd(
        ZFGlobalEvent::EventZFStyleOnInvalid(),
        action);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFStyleInvalidAssert)
{
    _ZFP_ZFStyleInvalidCheckDisableFlag = zffalse;
    ZFGlobalObserver().observerRemoveByTaskId(this->taskId);
}
zfidentity taskId;
ZF_GLOBAL_INITIALIZER_END(ZFStyleInvalidAssert)

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFStyleable, ZFStyleable *, defaultStyle)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFStyleable, void, styleableCopyFrom, ZFMP_IN(ZFStyleable *, anotherStyleable))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFStyleable, zfbool, styleableIsDefaultStyle)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFStyleable, void, styleKey, ZFMP_IN(const zfchar *, styleKey))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFStyleable, const zfchar *, styleKey)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(ZFStyleable, void, styleKeyForProperty, ZFMP_IN(const zfchar *, propertyName), ZFMP_IN(const zfchar *, styleKey))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFStyleable, const zfchar *, styleKeyForProperty, ZFMP_IN(const zfchar *, propertyName))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(ZFStyleable, void, styleKeyForProperty, ZFMP_IN(const ZFProperty *, property), ZFMP_IN(const zfchar *, styleKey))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFStyleable, const zfchar *, styleKeyForProperty, ZFMP_IN(const ZFProperty *, property))

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(void, ZFStyleDefaultApplyAutoCopy, ZFMP_IN(ZFStyleable *, styleValue))

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(void, ZFStyleSet, ZFMP_IN(const zfchar *, styleKey), ZFMP_IN(ZFStyleable *, styleValue))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(zfautoObject, ZFStyleGet, ZFMP_IN(const zfchar *, styleKey))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(void, ZFStyleGetAll, ZFMP_IN_OUT(ZFCoreArrayPOD<const zfchar *> &, styleKey), ZFMP_IN_OUT(ZFCoreArrayPOD<ZFStyleable *>, styleValue))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_0(void, ZFStyleRemoveAll)
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_0(void, ZFStyleChangeBegin)
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_0(void, ZFStyleChangeEnd)

ZF_NAMESPACE_GLOBAL_END
#endif

