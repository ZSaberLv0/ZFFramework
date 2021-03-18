#include "ZFObjectSmartPointer.h"
#include "ZFObjectImpl.h"
#include "zfsynchronize.h"
#include "ZFDynamicInvoker.h"

#include "ZFCore/ZFSTLWrapper/zfstl_string.h"
#include "ZFCore/ZFSTLWrapper/zfstl_map.h"
#include "ZFCore/ZFSTLWrapper/zfstl_vector.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// _ZFP_ZFObjectPrivate
typedef zfstlmap<zfstlstringZ, zfautoObject> _ZFP_ZFObjectTagMapType;
zfclassNotPOD _ZFP_ZFObjectPrivate
{
public:
    zfuint objectRetainCount;
    ZFObjectInstanceState objectInstanceState;
    ZFObjectHolder *objectHolder;
    void *mutexImpl;
    _ZFP_ZFObjectTagMapType objectTagMap;
    zfstlvector<const ZFProperty *> propertyAccessed;
    enum {
        stateFlag_objectIsPrivate = 1 << 0,
        stateFlag_objectIsInternal = 1 << 1,
        stateFlag_observerHasAddFlag_objectAfterAlloc = 1 << 2,
        stateFlag_observerHasAddFlag_objectBeforeDealloc = 1 << 3,
        stateFlag_observerHasAddFlag_objectPropertyValueOnUpdate = 1 << 4,
    };
    zfuint stateFlags;

public:
    _ZFP_ZFObjectPrivate(ZF_IN const ZFClass *cls)
    : objectRetainCount(1)
    , objectInstanceState(ZFObjectInstanceStateOnInit)
    , objectHolder(zfnull)
    , mutexImpl(zfnull)
    , objectTagMap()
    , propertyAccessed()
    , stateFlags(0)
    {
        if(cls->classIsPrivate())
        {
            ZFBitSet(this->stateFlags, _ZFP_ZFObjectPrivate::stateFlag_objectIsPrivate);
        }
        if(cls->classIsInternal())
        {
            ZFBitSet(this->stateFlags, _ZFP_ZFObjectPrivate::stateFlag_objectIsInternal);
        }
    }
};

// ============================================================
// ZFObject
ZFOBJECT_REGISTER(ZFObject)

ZFOBSERVER_EVENT_REGISTER(ZFObject, ObjectAfterAlloc)
ZFOBSERVER_EVENT_REGISTER(ZFObject, ObjectBeforeDealloc)
ZFOBSERVER_EVENT_REGISTER(ZFObject, ObjectPropertyValueOnUpdate)

static zfuint _ZFP_ZFObject_stateFlags = 0;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFObject_stateFlags, ZFLevelZFFrameworkStatic)
{
    ZFObjectGlobalEventObserver().observerHasAddStateAttach(ZFObject::EventObjectAfterAlloc(), &_ZFP_ZFObject_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectAfterAlloc);
    ZFObjectGlobalEventObserver().observerHasAddStateAttach(ZFObject::EventObjectBeforeDealloc(), &_ZFP_ZFObject_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectBeforeDealloc);
    ZFObjectGlobalEventObserver().observerHasAddStateAttach(ZFObject::EventObjectPropertyValueOnUpdate(), &_ZFP_ZFObject_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectPropertyValueOnUpdate);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFObject_stateFlags)
{
    ZFObjectGlobalEventObserver().observerHasAddStateDetach(ZFObject::EventObjectAfterAlloc(), &_ZFP_ZFObject_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectAfterAlloc);
    ZFObjectGlobalEventObserver().observerHasAddStateDetach(ZFObject::EventObjectBeforeDealloc(), &_ZFP_ZFObject_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectBeforeDealloc);
    ZFObjectGlobalEventObserver().observerHasAddStateDetach(ZFObject::EventObjectPropertyValueOnUpdate(), &_ZFP_ZFObject_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectPropertyValueOnUpdate);
}
ZF_GLOBAL_INITIALIZER_END(ZFObject_stateFlags)

zfindex ZFObject::objectRetainCount(void)
{
    return (zfindex)d->objectRetainCount;
}

ZFObjectHolder *ZFObject::objectHolder(void)
{
    if(d->objectHolder == zfnull)
    {
        zfCoreMutexLocker();
        if(d->objectHolder == zfnull)
        {
            d->objectHolder = zflockfree_zfAllocWithCache(ZFObjectHolder);
            d->objectHolder->objectHolded(this);
        }
    }
    return d->objectHolder;
}
ZFAny ZFObject::objectHolded(void)
{
    return this->to<ZFObjectHolder *>()->_ZFP_objectHolded;
}

void ZFObject::objectInfoOfInstanceT(ZF_IN_OUT zfstring &ret)
{
    ret += this->classData()->classNameFull();
    ret += "(";
    zfsFromPointerT(ret, this);
    ret += ")";
}
void ZFObject::objectInfoT(ZF_IN_OUT zfstring &ret)
{
    this->objectInfoOnAppendTokenLeft(ret);
    this->objectInfoOnAppend(ret);
    this->objectInfoOnAppendTokenRight(ret);
}
void ZFObject::objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
{
    this->objectInfoOfInstanceT(ret);
}
zfidentity ZFObject::objectHash(void)
{
    return zfidentityCalcPointer(this);
}
ZFCompareResult ZFObject::objectCompare(ZF_IN ZFObject *anotherObj)
{
    return ((this == anotherObj) ? ZFCompareTheSame : ZFCompareUncomparable);
}

/* ZFMETHOD_MAX_PARAM */
zfautoObject ZFObject::invoke(ZF_IN const zfchar *methodName
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
                              )
{
    zfCoreMutexLock();
    zfautoObject paramList[ZFMETHOD_MAX_PARAM];
    zfindex paramCount = 0;
    do {
        if(param0 == zfnull) {paramCount = 0; break;} else {paramList[0].zflockfree_assign(param0);}
        if(param1 == zfnull) {paramCount = 1; break;} else {paramList[1].zflockfree_assign(param1);}
        if(param2 == zfnull) {paramCount = 2; break;} else {paramList[2].zflockfree_assign(param2);}
        if(param3 == zfnull) {paramCount = 3; break;} else {paramList[3].zflockfree_assign(param3);}
        if(param4 == zfnull) {paramCount = 4; break;} else {paramList[4].zflockfree_assign(param4);}
        if(param5 == zfnull) {paramCount = 5; break;} else {paramList[5].zflockfree_assign(param5);}
        if(param6 == zfnull) {paramCount = 6; break;} else {paramList[6].zflockfree_assign(param6);}
        if(param7 == zfnull) {paramCount = 7; break;} else {paramList[7].zflockfree_assign(param7);}
    } while(zffalse);
    zflockfree_zfblockedAllocWithCache(v_zfstring, methodNameHolder);
    methodNameHolder->zfv = methodName;
    zfCoreMutexUnlock();

    zfautoObject ret;
    if(ZFDI_invoke(
        ret
        , errorHint
        , this
        , zfnull
        , methodNameHolder
        , paramCount
        , paramList
    )) {
        if(success != zfnull) {*success = zftrue;}
        return ret;
    }
    else
    {
        if(success != zfnull) {*success = zffalse;}
        return zfnull;
    }
}
zfautoObject ZFObject::invoke(ZF_IN const zfchar *methodName
                              , ZF_IN_OPT const zfchar *param0 /* = zfnull */
                              , ZF_IN_OPT const zfchar *param1 /* = zfnull */
                              , ZF_IN_OPT const zfchar *param2 /* = zfnull */
                              , ZF_IN_OPT const zfchar *param3 /* = zfnull */
                              , ZF_IN_OPT const zfchar *param4 /* = zfnull */
                              , ZF_IN_OPT const zfchar *param5 /* = zfnull */
                              , ZF_IN_OPT const zfchar *param6 /* = zfnull */
                              , ZF_IN_OPT const zfchar *param7 /* = zfnull */
                              , ZF_OUT_OPT zfbool *success /* = zfnull */
                              , ZF_OUT_OPT zfstring *errorHint /* = zfnull */
                              )
{
    zfCoreMutexLock();
    zfautoObject paramList[ZFMETHOD_MAX_PARAM];
    zfindex paramCount = 0;
    do {
        if(param0 == zfnull) {paramCount = 0; break;} else {paramList[0] = zflockfree_zflineAlloc(ZFDI_Wrapper, param0);}
        if(param1 == zfnull) {paramCount = 1; break;} else {paramList[1] = zflockfree_zflineAlloc(ZFDI_Wrapper, param1);}
        if(param2 == zfnull) {paramCount = 2; break;} else {paramList[2] = zflockfree_zflineAlloc(ZFDI_Wrapper, param2);}
        if(param3 == zfnull) {paramCount = 3; break;} else {paramList[3] = zflockfree_zflineAlloc(ZFDI_Wrapper, param3);}
        if(param4 == zfnull) {paramCount = 4; break;} else {paramList[4] = zflockfree_zflineAlloc(ZFDI_Wrapper, param4);}
        if(param5 == zfnull) {paramCount = 5; break;} else {paramList[5] = zflockfree_zflineAlloc(ZFDI_Wrapper, param5);}
        if(param6 == zfnull) {paramCount = 6; break;} else {paramList[6] = zflockfree_zflineAlloc(ZFDI_Wrapper, param6);}
        if(param7 == zfnull) {paramCount = 7; break;} else {paramList[7] = zflockfree_zflineAlloc(ZFDI_Wrapper, param7);}
    } while(zffalse);
    zflockfree_zfblockedAllocWithCache(v_zfstring, methodNameHolder);
    methodNameHolder->zfv = methodName;
    zfCoreMutexUnlock();

    zfautoObject ret;
    if(ZFDI_invoke(
        ret
        , errorHint
        , this
        , zfnull
        , methodNameHolder
        , paramCount
        , paramList
    )) {
        if(success != zfnull) {*success = zftrue;}
        return ret;
    }
    else
    {
        if(success != zfnull) {*success = zffalse;}
        return zfnull;
    }
}

zfbool ZFObject::objectTagExist(void)
{
    return !(d->objectTagMap.empty());
}
void ZFObject::objectTag(ZF_IN const zfchar *key,
                         ZF_IN ZFObject *tag)
{
    zfCoreMutexLocker();
    if(ZFBitTest(d->objectInstanceState, ZFObjectInstanceStateOnDealloc) && tag != zfnull)
    {
        zfCoreCriticalMessageTrim("[ZFObject] you must not set tag while object is deallocating, class: %s, tag: %s",
            this->classData()->classNameFull(),
            key);
        return ;
    }
    if(key == zfnull)
    {
        return ;
    }

    _ZFP_ZFObjectTagMapType &m = d->objectTagMap;
    _ZFP_ZFObjectTagMapType::iterator it = m.find(key);
    if(it == m.end())
    {
        if(tag != zfnull)
        {
            m[key].zflockfree_assign(tag);
        }
    }
    else
    {
        ZFObject *obj = zflockfree_zfRetain(it->second);
        if(tag == zfnull)
        {
            m.erase(it);
        }
        else
        {
            it->second.zflockfree_assign(tag);
        }
        zflockfree_zfRelease(obj);
    }
}
ZFObject *ZFObject::objectTag(ZF_IN const zfchar *key)
{
    if(key != zfnull)
    {
        zfCoreMutexLocker();
        _ZFP_ZFObjectTagMapType::iterator it = d->objectTagMap.find(key);
        if(it != d->objectTagMap.end())
        {
            return it->second.toObject();
        }
    }
    return zfnull;
}
void ZFObject::objectTagGetAllKeyValue(ZF_IN_OUT ZFCoreArray<const zfchar *> &allKey,
                                       ZF_IN_OUT ZFCoreArray<ZFObject *> &allValue)
{
    zfCoreMutexLocker();
    _ZFP_ZFObjectTagMapType &m = d->objectTagMap;
    allKey.capacity(allKey.count() + m.size());
    allValue.capacity(allValue.count() + m.size());
    for(_ZFP_ZFObjectTagMapType::iterator it = m.begin(); it != m.end(); ++it)
    {
        allKey.add(it->first.c_str());
        allValue.add(it->second.toObject());
    }
}
zfautoObject ZFObject::objectTagRemoveAndGet(ZF_IN const zfchar *key)
{
    if(key != zfnull)
    {
        zfCoreMutexLocker();
        _ZFP_ZFObjectTagMapType::iterator it = d->objectTagMap.find(key);
        if(it != d->objectTagMap.end())
        {
            zfautoObject ret;
            ret.zflockfree_assign(it->second);
            d->objectTagMap.erase(it);
            return ret;
        }
    }
    return zfnull;
}
void ZFObject::objectTagRemoveAll(void)
{
    if(!d->objectTagMap.empty())
    {
        zfCoreMutexLocker();
        _ZFP_ZFObjectTagMapType tmp;
        tmp.swap(d->objectTagMap);
    }
}


void ZFObject::observerOnAdd(ZF_IN zfidentity eventId)
{
    if(zffalse)
    {
    }
    else if(eventId == ZFObject::EventObjectAfterAlloc())
    {
        ZFBitSet(d->stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectAfterAlloc);
    }
    else if(eventId == ZFObject::EventObjectBeforeDealloc())
    {
        ZFBitSet(d->stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectBeforeDealloc);
    }
    else if(eventId == ZFObject::EventObjectPropertyValueOnUpdate())
    {
        ZFBitSet(d->stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectPropertyValueOnUpdate);
    }
}
void ZFObject::observerOnRemove(ZF_IN zfidentity eventId)
{
    if(zffalse)
    {
    }
    else if(eventId == ZFObject::EventObjectAfterAlloc())
    {
        ZFBitUnset(d->stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectAfterAlloc);
    }
    else if(eventId == ZFObject::EventObjectBeforeDealloc())
    {
        ZFBitUnset(d->stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectBeforeDealloc);
    }
    else if(eventId == ZFObject::EventObjectPropertyValueOnUpdate())
    {
        ZFBitUnset(d->stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectPropertyValueOnUpdate);
    }
}

void ZFObject::_ZFP_ZFObjectLock(void)
{
    if(d->mutexImpl)
    {
        _ZFP_ZFObjectMutexImplLock(d->mutexImpl);
    }
    else
    {
        zfCoreMutexLock();
        if(_ZFP_ZFObjectMutexImplInit)
        {
            if(d->mutexImpl == zfnull)
            {
                d->mutexImpl = _ZFP_ZFObjectMutexImplInit();
            }
        }
        zfCoreMutexUnlock();
        if(d->mutexImpl)
        {
            _ZFP_ZFObjectMutexImplLock(d->mutexImpl);
        }
    }
}
void ZFObject::_ZFP_ZFObjectUnlock(void)
{
    if(d->mutexImpl)
    {
        _ZFP_ZFObjectMutexImplUnlock(d->mutexImpl);
    }
}
zfbool ZFObject::_ZFP_ZFObjectTryLock(void)
{
    if(d->mutexImpl)
    {
        return _ZFP_ZFObjectMutexImplTryLock(d->mutexImpl);
    }
    else
    {
        zfCoreMutexLock();
        if(_ZFP_ZFObjectMutexImplInit)
        {
            if(d->mutexImpl == zfnull)
            {
                d->mutexImpl = _ZFP_ZFObjectMutexImplInit();
            }
        }
        zfCoreMutexUnlock();
        if(d->mutexImpl)
        {
            return _ZFP_ZFObjectMutexImplTryLock(d->mutexImpl);
        }
        else
        {
            return zffalse;
        }
    }
}

ZFObject *ZFObject::_ZFP_ZFObjectCheckOnInit(void)
{
    if(d == zfnull)
    {
        zfCoreCriticalMessageTrim("[ZFObject] ZFObject::objectOnInit() not called");
        return zfnull;
    }

    d->objectInstanceState = ZFObjectInstanceStateOnInitFinish;
    this->classData()->_ZFP_ZFClass_propertyAutoInitAction(this);
    this->_ZFP_ObjI_onInitIvk();
    this->objectOnInitFinish();
    d->objectInstanceState = ZFObjectInstanceStateIdle;

    if(!this->objectIsInternal())
    {
        this->classData()->_ZFP_ZFClass_instanceObserverNotify(this);
        if(ZFBitTest(d->stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectAfterAlloc)
            || ZFBitTest(_ZFP_ZFObject_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectAfterAlloc))
        {
            this->observerNotify(ZFObject::EventObjectAfterAlloc());
        }
    }

    return this;
}
void ZFObject::_ZFP_ZFObjectCheckRelease(void)
{
    if(ZFBitTest(d->stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectBeforeDealloc)
        || ZFBitTest(_ZFP_ZFObject_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectBeforeDealloc))
    {
        if(d->objectRetainCount == 1)
        {
            this->observerNotify(ZFObject::EventObjectBeforeDealloc());
            if(d->objectRetainCount > 1)
            {
                this->objectOnRelease();
                this->observerRemoveAll(ZFObject::EventObjectBeforeDealloc());
                return ;
            }
        }
    }

    this->objectOnRelease();
    if(d->objectRetainCount > 0)
    {
        // check to save cache
        if(this->_ZFP_ZFObject_zfAllocCacheRelease && d->objectRetainCount == 1)
        {
            if(ZFBitTest(d->stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectBeforeDealloc)
                || ZFBitTest(_ZFP_ZFObject_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectBeforeDealloc))
            {
                this->observerNotify(ZFObject::EventObjectBeforeDealloc());
                if(d->objectRetainCount > 1)
                {
                    this->objectOnRelease();
                    this->observerRemoveAll(ZFObject::EventObjectBeforeDealloc());
                    return ;
                }
                this->observerRemoveAll();
            }
            this->_ZFP_ZFObject_zfAllocCacheRelease(this);
        }
        return ;
    }

    d->objectInstanceState = ZFObjectInstanceStateOnDeallocPrepare;
    this->objectOnDeallocPrepare();
    this->_ZFP_ObjI_onDeallocIvk();
    for(zfstlsize i = d->propertyAccessed.size() - 1; i != (zfstlsize)-1; --i)
    {
        const ZFProperty *property = d->propertyAccessed[i];
        property->_ZFP_ZFProperty_callbackDealloc(property, this);
    }
    d->objectInstanceState = ZFObjectInstanceStateOnDealloc;
    this->objectOnDealloc();
    if(d != zfnull)
    {
        zfCoreCriticalMessageTrim("[ZFObject] ZFObject::objectOnDealloc() not called");
        return ;
    }
    this->classData()->_ZFP_ZFClass_objectDesctuct(this);
}

void ZFObject::objectOnInit(void)
{
    if(d != zfnull)
    {
        zfCoreCriticalMessageTrim("[ZFObject] ZFObject::objectOnInit() called twice");
        return ;
    }

    // note that (d != zfnull) is also used to check whether ZFObject::objectOnInit() is called
    d = zfpoolNew(_ZFP_ZFObjectPrivate, this->classData());
    this->observerHolder()._ZFP_ZFObserverHolder_observerOwner(this);
}
void ZFObject::objectOnDealloc(void)
{
    if(d == zfnull)
    {
        zfCoreCriticalMessageTrim("[ZFObject] ZFObject::objectOnDealloc() called twice");
        return ;
    }

    if(this->_ZFP_ZFObject_classData)
    {
        this->_ZFP_ZFObject_classData->_ZFP_classDynamicRegisterObjectInstanceDetach(this);
    }

    if(d->mutexImpl)
    {
        _ZFP_ZFObjectMutexImplDealloc(d->mutexImpl);
        d->mutexImpl = zfnull;
    }

    if(d->objectHolder)
    {
        d->objectHolder->objectHolded(zfnull);
        zfRelease(d->objectHolder);
    }

    zfpoolDelete(d);
    // note that (d == zfnull) is also used to check whether ZFObject::objectOnDealloc() is called
    d = zfnull;
}
void ZFObject::objectOnInitFinish(void)
{
}
void ZFObject::objectOnDeallocPrepare(void)
{
    this->objectTagRemoveAll();
    this->observerRemoveAll();
}
void ZFObject::objectOnRetain(void)
{
    zfCoreAssertWithMessageTrim(d->objectRetainCount > 0,
        "[ZFObject] retain an object while deallocating: %s", this->objectInfoOfInstance().cString());
    ++(d->objectRetainCount);
}
void ZFObject::objectOnRelease(void)
{
    --(d->objectRetainCount);
}

ZFObjectInstanceState ZFObject::objectInstanceState(void)
{
    return d->objectInstanceState;
}

zfbool ZFObject::objectIsPrivate(void)
{
    return ZFBitTest(d->stateFlags, _ZFP_ZFObjectPrivate::stateFlag_objectIsPrivate)
        || ZFBitTest(d->stateFlags, _ZFP_ZFObjectPrivate::stateFlag_objectIsInternal);
}
zfbool ZFObject::objectIsInternal(void)
{
    return ZFBitTest(d->stateFlags, _ZFP_ZFObjectPrivate::stateFlag_objectIsInternal);
}

void ZFObject::_ZFP_ZFObject_objectPropertyValueAttach(ZF_IN const ZFProperty *property)
{
    d->propertyAccessed.push_back(property);
}
void ZFObject::_ZFP_ZFObject_objectPropertyValueDetach(ZF_IN const ZFProperty *property)
{
    for(zfstlsize i = d->propertyAccessed.size() - 1; i != (zfstlsize)-1; --i)
    {
        if(d->propertyAccessed[i] == property)
        {
            d->propertyAccessed.erase(d->propertyAccessed.begin() + i);
        }
    }
}

void ZFObject::objectPropertyValueOnUpdate(ZF_IN const ZFProperty *property, ZF_IN const void *oldValue)
{
    if(!this->objectIsInternal()
        && (ZFBitTest(d->stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectPropertyValueOnUpdate)
            || ZFBitTest(_ZFP_ZFObject_stateFlags, _ZFP_ZFObjectPrivate::stateFlag_observerHasAddFlag_objectPropertyValueOnUpdate)))
    {
        v_ZFProperty *param0 = zflockfree_zfAllocWithCache(v_ZFProperty);
        param0->zfv = property;
        v_VoidPointerConst *param1 = zflockfree_zfAllocWithCache(v_VoidPointerConst);
        param1->zfv = oldValue;
        this->observerNotify(ZFObject::EventObjectPropertyValueOnUpdate(), param0, param1);
        zflockfree_zfRelease(param0);
        zflockfree_zfRelease(param1);
    }
}

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFEXPORT_ENUM_DEFINE(ZFObjectInstanceState, ZFObjectInstanceStateOnInit, ZFObjectInstanceStateOnInitFinish, ZFObjectInstanceStateIdle, ZFObjectInstanceStateOnDeallocPrepare, ZFObjectInstanceStateOnDealloc)

ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFObject, const ZFClass *, classData)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFObject, ZFObjectHolder *, objectHolder)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFObject, ZFAny, objectHolded)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFObject, void, objectInfoOfInstanceT, ZFMP_IN_OUT(zfstring &, ret))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFObject, zfstring, objectInfoOfInstance)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFObject, void, objectInfoT, ZFMP_IN_OUT(zfstring &, ret))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFObject, zfstring, objectInfo)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFObject, zfidentity, objectHash)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFObject, ZFCompareResult, objectCompare, ZFMP_IN(ZFObject *, anotherObj))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_8(ZFObject, zfautoObject, invoke
        , ZFMP_IN(const zfchar *, methodName)
        , ZFMP_IN_OPT(ZFObject *, param0, ZFMethodGenericInvokerDefaultParam())
        , ZFMP_IN_OPT(ZFObject *, param1, ZFMethodGenericInvokerDefaultParam())
        , ZFMP_IN_OPT(ZFObject *, param2, ZFMethodGenericInvokerDefaultParam())
        , ZFMP_IN_OPT(ZFObject *, param3, ZFMethodGenericInvokerDefaultParam())
        , ZFMP_IN_OPT(ZFObject *, param4, ZFMethodGenericInvokerDefaultParam())
        , ZFMP_IN_OPT(ZFObject *, param5, ZFMethodGenericInvokerDefaultParam())
        , ZFMP_IN_OPT(ZFObject *, param6, ZFMethodGenericInvokerDefaultParam())
        /* ZFMETHOD_MAX_PARAM , ZFMP_IN_OPT(ZFObject *, param7, ZFMethodGenericInvokerDefaultParam()) */
        /* ZFMETHOD_MAX_PARAM , ZFMP_OUT_OPT(zfbool *, success, zfnull) */
        /* ZFMETHOD_MAX_PARAM , ZFMP_OUT_OPT(zfstring *, errorHint, zfnull) */
    )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFObject, zfbool, objectTagExist)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(ZFObject, void, objectTag, ZFMP_IN(const zfchar *, key), ZFMP_IN(ZFObject *, tag))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFObject, ZFObject *, objectTag, ZFMP_IN(const zfchar *, key))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(ZFObject, void, objectTagGetAllKeyValue, ZFMP_IN_OUT(ZFCoreArray<const zfchar *> &, allKey), ZFMP_IN_OUT(ZFCoreArray<ZFObject *> &, allValue))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFObject, void, objectTagRemove, ZFMP_IN(const zfchar *, key))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFObject, zfautoObject, objectTagRemoveAndGet, ZFMP_IN(const zfchar *, key))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFObject, void, objectTagRemoveAll)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFObject, const ZFObserverHolder &, observerHolder)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_6(ZFObject, zfidentity, observerAdd, ZFMP_IN(zfidentity, eventId), ZFMP_IN(const ZFListener &, observer), ZFMP_IN_OPT(ZFObject *, userData, zfnull), ZFMP_IN_OPT(ZFObject *, owner, zfnull), ZFMP_IN_OPT(zfbool, autoRemoveAfterActivate, zffalse), ZFMP_IN_OPT(ZFLevel, observerLevel, ZFLevelAppNormal))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFObject, zfidentity, observerAdd, ZFMP_IN(const ZFObserverAddParam &, param))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_5(ZFObject, zfidentity, observerAddForOnce, ZFMP_IN(zfidentity, eventId), ZFMP_IN(const ZFListener &, observer), ZFMP_IN_OPT(ZFObject *, userData, zfnull), ZFMP_IN_OPT(ZFObject *, owner, zfnull), ZFMP_IN_OPT(ZFLevel, observerLevel, ZFLevelAppNormal))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFObject, void, observerMoveToFirst, ZFMP_IN(zfidentity, taskId))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_2(ZFObject, void, observerRemove, ZFMP_IN(zfidentity, eventId), ZFMP_IN(const ZFListener &, callback))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_4(ZFObject, void, observerRemove, ZFMP_IN(zfidentity, eventId), ZFMP_IN(const ZFListener &, callback), ZFMP_IN(ZFObject *, userData), ZFMP_IN_OPT(ZFComparer<ZFObject *>::Comparer, userDataComparer, ZFComparerCheckEqual))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFObject, void, observerRemoveByTaskId, ZFMP_IN(zfidentity, taskId))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFObject, void, observerRemoveByOwner, ZFMP_IN(ZFObject *, owner))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFObject, void, observerRemoveAll, ZFMP_IN(zfidentity, eventId))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFObject, void, observerRemoveAll)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFObject, zfbool, observerHasAdd)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFObject, zfbool, observerHasAdd, ZFMP_IN(zfidentity, eventId))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_3(ZFObject, void, observerNotify, ZFMP_IN(zfidentity, eventId), ZFMP_IN_OPT(ZFObject *, param0, zfnull), ZFMP_IN_OPT(ZFObject *, param1, zfnull))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_4(ZFObject, void, observerNotifyWithCustomSender, ZFMP_IN(ZFObject *, customSender), ZFMP_IN(zfidentity, eventId), ZFMP_IN_OPT(ZFObject *, param0, zfnull), ZFMP_IN_OPT(ZFObject *, param1, zfnull))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFObject, ZFObjectInstanceState, objectInstanceState)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFObject, zfbool, objectIsPrivate)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFObject, zfbool, objectIsInternal)

ZF_NAMESPACE_GLOBAL_END
#endif

