#include "ZFObjectCore.h"
#include "ZFObjectImpl.h"

#include "ZFCore/ZFSTLWrapper/zfstl_string.h"
#include "ZFCore/ZFSTLWrapper/zfstl_vector.h"
#include "ZFCore/ZFSTLWrapper/zfstl_deque.h"
#include "ZFCore/ZFSTLWrapper/zfstl_map.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFClass's global data
ZF_STATIC_INITIALIZER_INIT(ZFClassDataHolder)
{
}
ZF_STATIC_INITIALIZER_DESTROY(ZFClassDataHolder)
{
    ZFCoreMap classMapTmp = this->classMap;
    ZFCoreMap delayDeleteMapTmp = this->delayDeleteMap;
    this->classMap = ZFCoreMap();
    this->delayDeleteMap = ZFCoreMap();

    delayDeleteMapTmp.removeAll();
    classMapTmp.removeAll();
}
ZFCoreMap classMap; // <classNameFull, ZFClass *>
ZFCoreMap delayDeleteMap; // <classNameFull, ZFClass *>
/*
 * delay delete a class
 * ZFClass may be registered by different module,
 * while unloading a module,
 * _ZFP_ZFClassUnregister would be called to unload the class,
 * however, it may be called earlier than object instance's dealloc step,
 * so we cache the ZFClass object,
 * remove from the activating classMap,
 * and delete them all during ZFClassDataHolder's dealloc step
 *
 * this can't resolve the problem perfectly,
 * but did resolve most cases
 */
ZF_STATIC_INITIALIZER_END(ZFClassDataHolder)
#define _ZFP_ZFClassMap (ZF_STATIC_INITIALIZER_INSTANCE(ZFClassDataHolder)->classMap)
#define _ZFP_ZFClassDelayDeleteMap (ZF_STATIC_INITIALIZER_INSTANCE(ZFClassDataHolder)->delayDeleteMap)

// ============================================================
// _ZFP_ZFClassPrivate
typedef zfstlmap<zfstlstringZ, zfautoObject> _ZFP_ZFClassTagMapType;
typedef zfstlmap<const ZFProperty *, zfstlmap<const ZFClass *, zfbool> > _ZFP_ZFClassPropertyInitStepMapType;
zfclassNotPOD _ZFP_ZFClassPrivate
{
public:
    zfuint refCount;
    /*
     * here's a memo for ZFCoreLibDestroyFlag:
     *
     * ZFFramework is designed as plugin type framework,
     * which can be load dynamically
     *
     * it's all right for most case if use
     * ZF_GLOBAL_INITIALIZER_INIT/ZF_STATIC_INITIALIZER_INIT,
     * however, for core meta-object types (class map, method map, etc),
     * it require additional register and unregister steps
     * to ensure initialize and destroy order
     *
     * here's a typical case of the questions mentioned above:
     *
     * * libraries that depends on ZFFramework needs to be load and unload at runtime
     *
     *   at this case, we need some static holder object, to unregister class map from ZFFramework,
     *   during the library being unloaded
     *
     * * ZFFramework would be unloaded before app's destruction,
     *   this is typical if under Qt using QLibrary
     *
     *   at this case, simply use static holder object would cause crash during app's destruction,
     *   since ZFFramework already unloaded when the holder object try to unregister class map
     */
    ZFCoreArrayPOD<zfbool *> ZFCoreLibDestroyFlag;
    ZFClass *pimplOwner;
    zfbool classIsDynamicRegister;
    zfautoObject classDynamicRegisterUserData;
    zfstlmap<ZFObject *, zfbool> classDynamicRegisterObjectInstanceMap;
    _ZFP_zfAllocWithCacheCallback objectAllocWithCacheCallback;
    _ZFP_ZFObjectConstructor constructor;
    _ZFP_ZFObjectDestructor destructor;
    zfstring classNamespace;
    zfstring className;
    zfstring classNameFull;
    const ZFClass *classParent;
    zfbool isInterface;
    zfbool isPrivateClass;
    zfbool isInternalClass;

public:
    zfbool needRegisterImplementedInterface;
    zfbool internalTypesNeedAutoRegister; // used to register ZFMethod and ZFProperty
    zfbool needFinalInit;
    ZFCoreArrayPOD<const ZFClass *> implementedInterface;
    zfstlmap<zfstlstringZ, zfstlvector<const ZFMethod *> > methodMap;
    ZFCoreArrayPOD<const ZFMethod *> methodList;
    ZFCoreArrayPOD<const ZFProperty *> propertyList;
    ZFCoreMap propertyMap; // map to const ZFProperty *
    /*
     * store all property that has override parent's OnInit step by #ZFPROPERTY_OVERRIDE_ON_INIT_DECLARE
     * including self and all parent
     *
     * note, only stored when subclass override parent's OnInit,
     * not applied when the property's direct owner declared custom OnInit
     *
     * during owner object's allocation,
     * all of these property would have their getter being called
     * to suit most logic case
     */
    zfstlmap<const ZFProperty *, zfbool> propertyAutoInitMap;
    /*
     * see propertyAutoInitMap,
     * used to check whether two class has same property init step
     */
    _ZFP_ZFClassPropertyInitStepMapType propertyInitStepMap;

public:
    _ZFP_ZFClassTagMapType classTagMap;

public:
    zfstlmap<zfstlstringZ, zfbool> classDataChangeAutoRemoveTagList;

public:
    const ZFClass **parentListCache; // all parent including self
    const ZFClass **parentInterfaceListCache; // all parent interface, count may differ from interfaceCastListCache
    const ZFClass **interfaceCastListCache; // all parent implemented interface, for interface cast
    _ZFP_ZFObjectToInterfaceCastCallback *interfaceCastCallbackListCache;

public:
    /*
     * cache all parent and children for future use
     */
    zfstlmap<const ZFClass *, zfbool> allParent; // all parent and interface excluding self
    zfstlmap<const ZFClass *, zfbool> allChildren; // all children excluding self

public:
    /*
     * cache all parent to search method and property for performance
     *
     * unlike allParent, this array ensured ordered by: self > parent interface > parent
     */
    zfstlvector<const ZFClass *> methodAndPropertyFindCache;
    /*
     * cache for propertyForName, recursively from child to parent
     */
    zfstlmap<zfstlstringZ, const ZFProperty *> propertyMapCache;
    /*
     * cache for methodForName, child one would override parent
     */
    zfstlmap<zfstlstringZ, zfstlvector<const ZFMethod *> > methodMapCache;
    zfbool methodMapCacheNeedUpdate;

public:
    zfclassLikePOD InstanceObserverData
    {
    public:
        ZFListener observer;
        zfautoObject userData;
        ZFObject *owner;
        ZFLevel observerLevel;
    };
    zfstldeque<ZFCorePointerForObject<_ZFP_ZFClassPrivate::InstanceObserverData *> > instanceObserver;
    zfstldeque<_ZFP_ZFClassPrivate::InstanceObserverData *> instanceObserverCached;

public:
    void _instanceObserverDoAdd(ZF_IN_OUT zfstldeque<_ZFP_ZFClassPrivate::InstanceObserverData *> &buf,
                                ZF_IN _ZFP_ZFClassPrivate::InstanceObserverData *data)
    {
        zfstlsize index = buf.size();
        while(index > 0 && buf[index - 1]->observerLevel > data->observerLevel)
        {
            --index;
        }
        buf.insert(buf.begin() + index, data);
    }
    void instanceObserverDoAdd(ZF_IN _ZFP_ZFClassPrivate::InstanceObserverData *data,
                               ZF_IN zfbool observeAllChildType)
    {
        this->_instanceObserverDoAdd(this->instanceObserverCached, data);
        if(!observeAllChildType)
        {
            return ;
        }

        for(zfstlmap<const ZFClass *, zfbool>::iterator childIt = this->allChildren.begin(); childIt != this->allChildren.end(); ++childIt)
        {
            if(!childIt->first->classIsInternal())
            {
                this->_instanceObserverDoAdd(childIt->first->d->instanceObserverCached, data);
            }
        }
    }
    void instanceObserverDoRemove(ZF_IN _ZFP_ZFClassPrivate::InstanceObserverData *data)
    {
        const ZFClass *cls = this->pimplOwner;
        zfstlmap<const ZFClass *, zfbool>::iterator childIt = this->allChildren.begin();
        do
        {
            for(zfstlsize i = 0; i < cls->d->instanceObserverCached.size(); ++i)
            {
                if(cls->d->instanceObserverCached[i] == data)
                {
                    cls->d->instanceObserverCached.erase(cls->d->instanceObserverCached.begin() + i);
                    break;
                }
            }

            if(childIt != this->allChildren.end())
            {
                cls = childIt->first;
                ++childIt;
            }
            else
            {
                break;
            }
        } while(zftrue);
    }

public:
    ZFObject *objectConstruct(void)
    {
        if(this->constructor)
        {
            ZFObject *obj = this->constructor();
            if(this->pimplOwner->classIsDynamicRegister())
            {
                obj->_ZFP_ZFObject_classData = this->pimplOwner;
                this->classDynamicRegisterObjectInstanceMap[obj] = zftrue;
            }
            return obj;
        }
        else
        {
            return zfnull;
        }
    }

public:
    _ZFP_ZFClassPrivate(void)
    : refCount(1)
    , ZFCoreLibDestroyFlag()
    , pimplOwner(zfnull)
    , classIsDynamicRegister(zffalse)
    , classDynamicRegisterUserData()
    , classDynamicRegisterObjectInstanceMap()
    , constructor(zfnull)
    , destructor(zfnull)
    , classNamespace()
    , className()
    , classNameFull()
    , classParent(zfnull)
    , isInterface(zffalse)
    , isPrivateClass(zffalse)
    , isInternalClass(zffalse)
    , needRegisterImplementedInterface(zftrue)
    , internalTypesNeedAutoRegister(zftrue)
    , needFinalInit(zftrue)
    , implementedInterface()
    , methodMap()
    , methodList()
    , propertyList()
    , propertyMap()
    , propertyAutoInitMap()
    , propertyInitStepMap()
    , classTagMap()
    , classDataChangeAutoRemoveTagList()
    , parentListCache(zfnull)
    , parentInterfaceListCache(zfnull)
    , interfaceCastListCache(zfnull)
    , interfaceCastCallbackListCache(zfnull)
    , allParent()
    , allChildren()
    , methodAndPropertyFindCache()
    , propertyMapCache()
    , methodMapCache()
    , methodMapCacheNeedUpdate(zftrue)
    , instanceObserver()
    , instanceObserverCached()
    {
    }
    ~_ZFP_ZFClassPrivate(void)
    {
        for(zfindex i = 0; i < this->ZFCoreLibDestroyFlag.count(); ++i)
        {
            *(this->ZFCoreLibDestroyFlag[i]) = zftrue;
        }
        zffree(this->parentListCache);
        this->parentListCache = zfnull;
        zffree(this->parentInterfaceListCache);
        this->parentInterfaceListCache = zfnull;
        zffree(this->interfaceCastListCache);
        this->interfaceCastListCache = zfnull;
        zffree(this->interfaceCastCallbackListCache);
        this->interfaceCastCallbackListCache = zfnull;
    }
};

// ============================================================
// clear class tag both in ZFLevelZFFrameworkEssential and ZFLevelZFFrameworkHigh
static void _ZFP_ZFClass_classTagClear(void)
{
    ZFCoreArrayPOD<const ZFClass *> allClass;
    _ZFP_ZFClassMap.allValueT(allClass);
    for(zfindex i = 0; i < allClass.count(); ++i)
    {
        allClass.get(i)->classTagRemoveAll();
    }
}
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFClassTagClearLevelEssential, ZFLevelZFFrameworkEssential)
{
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFClassTagClearLevelEssential)
{
    _ZFP_ZFClass_classTagClear();
}
ZF_GLOBAL_INITIALIZER_END(ZFClassTagClearLevelEssential)
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFClassTagClearLevelHigh, ZFLevelZFFrameworkHigh)
{
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFClassTagClearLevelHigh)
{
    _ZFP_ZFClass_classTagClear();
}
ZF_GLOBAL_INITIALIZER_END(ZFClassTagClearLevelHigh)

// ============================================================
// static methods
const ZFClass *ZFClass::classForName(ZF_IN const zfchar *className)
{
    return _ZFP_ZFClassMap.get<const ZFClass *>(ZFNamespaceSkipGlobal(className));
}
const ZFClass *ZFClass::classForName(ZF_IN const zfchar *className,
                                     ZF_IN const zfchar *classNamespace)
{
    classNamespace = ZFNamespaceSkipGlobal(classNamespace);
    if(classNamespace != zfnull)
    {
        zfstring classNameFull;
        classNameFull += classNamespace;
        classNameFull += ZFNamespaceSeparator();
        classNameFull += className;
        return _ZFP_ZFClassMap.get<const ZFClass *>(classNameFull);
    }
    else
    {
        return _ZFP_ZFClassMap.get<const ZFClass *>(ZFNamespaceSkipGlobal(className));
    }
}

// ============================================================
// instance observer
void ZFClass::instanceObserverAdd(ZF_IN const ZFListener &observer,
                                  ZF_IN_OPT ZFObject *userData /* = zfnull */,
                                  ZF_IN_OPT ZFObject *owner /* = zfnull */,
                                  ZF_IN_OPT ZFLevel observerLevel /* = ZFLevelAppNormal */,
                                  ZF_IN_OPT zfbool observeAllChildType /* = zftrue */) const
{
    if(this->classIsInternal())
    {
        return ;
    }

    _ZFP_ZFClassPrivate::InstanceObserverData *data = zfnew(_ZFP_ZFClassPrivate::InstanceObserverData);
    data->observer = observer;
    data->userData = userData;
    data->owner = owner;
    data->observerLevel = observerLevel;
    d->instanceObserver.push_back(ZFCorePointerForObject<_ZFP_ZFClassPrivate::InstanceObserverData *>(data));
    d->instanceObserverDoAdd(data, observeAllChildType);
}
void ZFClass::instanceObserverRemove(ZF_IN const ZFListener &observer) const
{
    zfCoreMutexLocker();
    for(zfstlsize i = 0; i < d->instanceObserver.size(); ++i)
    {
        _ZFP_ZFClassPrivate::InstanceObserverData *data = d->instanceObserver[i];
        if(data->observer == observer)
        {
            d->instanceObserverDoRemove(data);
            d->instanceObserver.erase(d->instanceObserver.begin() + i);
            break;
        }
    }
}
void ZFClass::instanceObserverRemoveByOwner(ZF_IN ZFObject *owner) const
{
    zfCoreMutexLocker();
    for(zfstlsize i = 0; i < d->instanceObserver.size(); ++i)
    {
        _ZFP_ZFClassPrivate::InstanceObserverData *data = d->instanceObserver[i];
        if(data->owner == owner)
        {
            d->instanceObserverDoRemove(data);
            d->instanceObserver.erase(d->instanceObserver.begin() + i);
            --i;
        }
    }
}
void ZFClass::instanceObserverRemoveAll(void) const
{
    zfCoreMutexLocker();
    while(!d->instanceObserver.empty())
    {
        _ZFP_ZFClassPrivate::InstanceObserverData *data = d->instanceObserver[0];
        d->instanceObserverDoRemove(data);
        d->instanceObserver.erase(d->instanceObserver.begin());
    }
}
void ZFClass::_ZFP_ZFClass_instanceObserverNotify(ZF_IN ZFObject *obj) const
{
    if(!d->instanceObserverCached.empty())
    {
        ZFListenerData listenerData(ZFObject::EventObjectAfterAlloc(), obj);
        for(zfstlsize i = 0; i < d->instanceObserverCached.size() && !listenerData.eventFiltered(); ++i)
        {
            _ZFP_ZFClassPrivate::InstanceObserverData &data = *(d->instanceObserverCached[i]);
            data.observer.execute(listenerData, data.userData.toObject());
        }
    }
}

// ============================================================
// class data change observer
void ZFClass::_ZFP_ZFClass_classDataChangeNotify(void) const
{
    if(!d->classDataChangeAutoRemoveTagList.empty())
    {
        zfstlmap<zfstlstringZ, zfbool> t = d->classDataChangeAutoRemoveTagList;
        for(zfstlmap<zfstlstringZ, zfbool>::iterator it = t.begin(); it != t.end(); ++it)
        {
            this->classTagRemove(it->first.c_str());
        }
    }
}
void ZFClass::classDataChangeAutoRemoveTagAdd(ZF_IN const zfchar *tag) const
{
    zfCoreAssert(tag != zfnull);
    d->classDataChangeAutoRemoveTagList[tag] = zftrue;
}
void ZFClass::classDataChangeAutoRemoveTagRemove(ZF_IN const zfchar *tag) const
{
    zfCoreAssert(tag != zfnull);
    d->classDataChangeAutoRemoveTagList.erase(tag);
}

// ============================================================
// class info
static void _ZFP_ZFClassInfo(ZF_IN_OUT zfstring &s,
                             ZF_IN const ZFClass *cls)
{
    if(cls->classIsAbstract() && !cls->classIsInterface())
    {
        s += "(abstract)";
    }
    s += cls->classNameFull();
}
void ZFClass::objectInfoT(ZF_IN_OUT zfstring &ret) const
{
    _ZFP_ZFClassInfo(ret, this);
    if(this->implementedInterfaceCount() > 0)
    {
        ret += '<';
        for(zfindex i = 0; i < this->implementedInterfaceCount(); ++i)
        {
            if(i != 0)
            {
                ret += ", ";
            }
            ret += this->implementedInterfaceAtIndex(i)->classNameFull();
        }
        ret += '>';
    }
}
void ZFClass::objectInfoOfInheritTreeT(ZF_IN_OUT zfstring &ret) const
{
    const ZFClass *cls = this;
    while(cls != zfnull)
    {
        if(cls != this)
        {
            if(cls == ZFInterface::ClassData())
            {
                break;
            }
            ret += " : ";
        }

        _ZFP_ZFClassInfo(ret, cls);

        if(cls->implementedInterfaceCount() > 0)
        {
            zfbool first = zftrue;
            for(zfindex i = 0; i < cls->implementedInterfaceCount(); ++i)
            {
                if(cls->implementedInterfaceAtIndex(i) == ZFInterface::ClassData())
                {
                    continue;
                }
                if(first)
                {
                    first = zffalse;
                    ret += '<';
                }
                else
                {
                    ret += ", ";
                }
                cls->implementedInterfaceAtIndex(i)->objectInfoOfInheritTreeT(ret);
            }
            if(!first)
            {
                ret += '>';
            }
        }

        cls = cls->classParent();
    }
}

zfbool ZFClass::classIsTypeOf(ZF_IN const ZFClass *cls) const
{
    const ZFClass **p = (cls->d->isInterface ? d->parentInterfaceListCache : d->parentListCache);
    do
    {
        if(*p == cls)
        {
            return zftrue;
        }
        ++p;
    } while(*p);
    return zffalse;
}

zfbool ZFClass::classIsDynamicRegister(void) const
{
    return d->classIsDynamicRegister;
}
ZFObject *ZFClass::classDynamicRegisterUserData(void) const
{
    return d->classDynamicRegisterUserData;
}

zfbool ZFClass::classIsAbstract(void) const
{
    return (d->constructor == zfnull);
}

zfbool ZFClass::classIsInterface(void) const
{
    return d->isInterface;
}

zfbool ZFClass::classIsPrivate(void) const
{
    return d->isPrivateClass;
}
zfbool ZFClass::classIsInternal(void) const
{
    return d->isInternalClass;
}

zfautoObject ZFClass::newInstance(void) const
{
    zfCoreMutexLocker();
    ZFObject *obj = zfnull;
    if(d->objectAllocWithCacheCallback)
    {
        obj = d->objectAllocWithCacheCallback();
    }
    else
    {
        obj = d->objectConstruct();
        if(obj != zfnull)
        {
            obj->objectOnInit();
            obj->_ZFP_ZFObjectCheckOnInit();
        }
    }
    zfautoObject ret;
    ret.zflockfree_assign(obj);
    zflockfree_zfRelease(obj);
    return ret;
}

zfautoObject ZFClass::newInstanceGeneric(
                                           ZF_IN_OPT ZFObject *param0 /* = ZFMethodGenericInvokerDefaultParam() */
                                         , ZF_IN_OPT ZFObject *param1 /* = ZFMethodGenericInvokerDefaultParam() */
                                         , ZF_IN_OPT ZFObject *param2 /* = ZFMethodGenericInvokerDefaultParam() */
                                         , ZF_IN_OPT ZFObject *param3 /* = ZFMethodGenericInvokerDefaultParam() */
                                         , ZF_IN_OPT ZFObject *param4 /* = ZFMethodGenericInvokerDefaultParam() */
                                         , ZF_IN_OPT ZFObject *param5 /* = ZFMethodGenericInvokerDefaultParam() */
                                         , ZF_IN_OPT ZFObject *param6 /* = ZFMethodGenericInvokerDefaultParam() */
                                         , ZF_IN_OPT ZFObject *param7 /* = ZFMethodGenericInvokerDefaultParam() */
                                         ) const /* ZFMETHOD_MAX_PARAM */
{
    if(d->constructor == zfnull)
    {
        return zfnull;
    }
    if(zftrue
        && param0 == ZFMethodGenericInvokerDefaultParam()
        && param1 == ZFMethodGenericInvokerDefaultParam()
        && param2 == ZFMethodGenericInvokerDefaultParam()
        && param3 == ZFMethodGenericInvokerDefaultParam()
        && param4 == ZFMethodGenericInvokerDefaultParam()
        && param5 == ZFMethodGenericInvokerDefaultParam()
        && param6 == ZFMethodGenericInvokerDefaultParam()
        && param7 == ZFMethodGenericInvokerDefaultParam()
        )
    {
        return this->newInstance();
    }

    ZFCoreArrayPOD<const ZFMethod *> objectOnInitMethodList;
    this->methodForNameGetAllT(objectOnInitMethodList, "objectOnInit");
    ZFToken token = this->newInstanceGenericBegin();
    if(token != zfnull)
    {
        zfautoObject paramList[ZFMETHOD_MAX_PARAM];
        paramList[0].zflockfree_assign(param0);
        paramList[1].zflockfree_assign(param1);
        paramList[2].zflockfree_assign(param2);
        paramList[3].zflockfree_assign(param3);
        paramList[4].zflockfree_assign(param4);
        paramList[5].zflockfree_assign(param5);
        paramList[6].zflockfree_assign(param6);
        paramList[7].zflockfree_assign(param7);

        for(zfindex i = 0; i < objectOnInitMethodList.count(); ++i)
        {
            if(this->newInstanceGenericCheck(token, objectOnInitMethodList[i], paramList))
            {
                return this->newInstanceGenericEnd(token, zftrue);
            }
        }
        this->newInstanceGenericEnd(token, zffalse);
    }
    return zfnull;
}
ZFToken ZFClass::newInstanceGenericBegin(void) const
{
    zfCoreMutexLocker();
    return (ZFToken)d->objectConstruct();
}
zfbool ZFClass::newInstanceGenericCheck(ZF_IN ZFToken token
                                        , ZF_IN const ZFMethod *objectOnInitMethod
                                        , ZF_IN_OUT zfautoObject (&paramList)[ZFMETHOD_MAX_PARAM]
                                        ) const
{
    if(objectOnInitMethod == zfnull
        || !this->classIsTypeOf(objectOnInitMethod->methodOwnerClass())
        || !zfscmpTheSame(objectOnInitMethod->methodName(), "objectOnInit")
        )
    {
        return zffalse;
    }
    ZFObject *obj = (ZFObject *)token;
    zfautoObject methodRetDummy;
    return objectOnInitMethod->methodGenericInvoker()(objectOnInitMethod, obj, zfnull, methodRetDummy, paramList);
}
zfautoObject ZFClass::newInstanceGenericEnd(ZF_IN ZFToken token,
                                            ZF_IN zfbool objectOnInitMethodInvokeSuccess) const
{
    ZFObject *obj = (ZFObject *)token;
    if(objectOnInitMethodInvokeSuccess)
    {
        obj->_ZFP_ZFObjectCheckOnInit();
        zflockfree_zfblockedRelease(obj);
        return obj;
    }
    else
    {
        d->destructor(obj);
        return zfnull;
    }
}

zfindex ZFClass::implementedInterfaceCount(void) const
{
    return d->implementedInterface.count();
}
const ZFClass *ZFClass::implementedInterfaceAtIndex(ZF_IN zfindex index) const
{
    return d->implementedInterface.get(index);
}

// ============================================================
// ZFMethod
zfindex ZFClass::methodCount(void) const
{
    this->_ZFP_ZFClass_methodAndPropertyAutoRegister();
    return d->methodList.count();
}
const ZFMethod *ZFClass::methodAtIndex(ZF_IN zfindex index) const
{
    this->_ZFP_ZFClass_methodAndPropertyAutoRegister();
    if(index >= d->methodList.count())
    {
        zfCoreCriticalIndexOutOfRange(index, this->methodCount());
        return zfnull;
    }
    return d->methodList[index];
}

void ZFClass::methodGetAllT(ZF_IN_OUT ZFCoreArray<const ZFMethod *> &ret) const
{
    this->_ZFP_ZFClass_methodAndPropertyAutoRegister();
    for(zfstlmap<zfstlstringZ, zfstlvector<const ZFMethod *> >::iterator it = d->methodMapCache.begin(); it != d->methodMapCache.end(); ++it)
    {
        for(zfstlsize i = 0; i < it->second.size(); ++i)
        {
            ret.add(it->second[i]);
        }
    }
}

/* ZFMETHOD_MAX_PARAM */
const ZFMethod *ZFClass::methodForNameIgnoreParent(ZF_IN const zfchar *methodName
                                                   , ZF_IN const zfchar *methodParamTypeId0
                                                   , ZF_IN_OPT const zfchar *methodParamTypeId1 /* = zfnull */
                                                   , ZF_IN_OPT const zfchar *methodParamTypeId2 /* = zfnull */
                                                   , ZF_IN_OPT const zfchar *methodParamTypeId3 /* = zfnull */
                                                   , ZF_IN_OPT const zfchar *methodParamTypeId4 /* = zfnull */
                                                   , ZF_IN_OPT const zfchar *methodParamTypeId5 /* = zfnull */
                                                   , ZF_IN_OPT const zfchar *methodParamTypeId6 /* = zfnull */
                                                   , ZF_IN_OPT const zfchar *methodParamTypeId7 /* = zfnull */
                                                   ) const
{
    this->_ZFP_ZFClass_methodAndPropertyAutoRegister();

    if(!d->methodMap.empty())
    {
        zfstlmap<zfstlstringZ, zfstlvector<const ZFMethod *> >::iterator itName = d->methodMap.find(methodName);
        if(itName != d->methodMap.end())
        {
            zfstlvector<const ZFMethod *> &l = itName->second;
            for(zfstlsize i = 0; i < l.size(); ++i)
            {
                const ZFMethod *m = l[i];
                if(m->methodParamTypeIdIsMatch(
                          methodParamTypeId0
                        , methodParamTypeId1
                        , methodParamTypeId2
                        , methodParamTypeId3
                        , methodParamTypeId4
                        , methodParamTypeId5
                        , methodParamTypeId6
                        , methodParamTypeId7
                    ))
                {
                    return m;
                }
            }
        }
    }
    return zfnull;
}
const ZFMethod *ZFClass::methodForNameIgnoreParent(ZF_IN const zfchar *methodName) const
{
    this->_ZFP_ZFClass_methodAndPropertyAutoRegister();

    if(!d->methodMap.empty())
    {
        zfstlmap<zfstlstringZ, zfstlvector<const ZFMethod *> >::iterator itName = d->methodMap.find(methodName);
        if(itName != d->methodMap.end())
        {
            return itName->second[0];
        }
    }
    return zfnull;
}
const ZFMethod *ZFClass::methodForName(ZF_IN const zfchar *methodName
                                       , ZF_IN const zfchar *methodParamTypeId0
                                       , ZF_IN_OPT const zfchar *methodParamTypeId1 /* = zfnull */
                                       , ZF_IN_OPT const zfchar *methodParamTypeId2 /* = zfnull */
                                       , ZF_IN_OPT const zfchar *methodParamTypeId3 /* = zfnull */
                                       , ZF_IN_OPT const zfchar *methodParamTypeId4 /* = zfnull */
                                       , ZF_IN_OPT const zfchar *methodParamTypeId5 /* = zfnull */
                                       , ZF_IN_OPT const zfchar *methodParamTypeId6 /* = zfnull */
                                       , ZF_IN_OPT const zfchar *methodParamTypeId7 /* = zfnull */
                                       ) const
{
    if(methodName == zfnull)
    {
        return zfnull;
    }
    this->_ZFP_ZFClass_methodAndPropertyAutoRegister();
    this->_ZFP_ZFClass_methodMapCacheUpdate();
    zfstlmap<zfstlstringZ, zfstlvector<const ZFMethod *> >::iterator it = d->methodMapCache.find(methodName);
    if(it == d->methodMapCache.end())
    {
        return zfnull;
    }
    for(zfindex i = 0; i < it->second.size(); ++i)
    {
        if(it->second[i]->methodParamTypeIdIsMatch(
                methodParamTypeId0
                , methodParamTypeId1
                , methodParamTypeId2
                , methodParamTypeId3
                , methodParamTypeId4
                , methodParamTypeId5
                , methodParamTypeId6
                , methodParamTypeId7
            ))
        {
            return it->second[i];
        }
    }
    return zfnull;
}
const ZFMethod *ZFClass::methodForName(ZF_IN const zfchar *methodName) const
{
    if(methodName == zfnull)
    {
        return zfnull;
    }
    this->_ZFP_ZFClass_methodAndPropertyAutoRegister();
    this->_ZFP_ZFClass_methodMapCacheUpdate();
    zfstlmap<zfstlstringZ, zfstlvector<const ZFMethod *> >::iterator it = d->methodMapCache.find(methodName);
    if(it == d->methodMapCache.end())
    {
        return zfnull;
    }
    else
    {
        return it->second[0];
    }
}
void ZFClass::methodForNameGetAllT(ZF_IN_OUT ZFCoreArray<const ZFMethod *> &ret,
                                   ZF_IN const zfchar *methodName) const
{
    if(methodName == zfnull)
    {
        return ;
    }
    this->_ZFP_ZFClass_methodAndPropertyAutoRegister();
    this->_ZFP_ZFClass_methodMapCacheUpdate();
    zfstlmap<zfstlstringZ, zfstlvector<const ZFMethod *> >::iterator it = d->methodMapCache.find(methodName);
    if(it != d->methodMapCache.end())
    {
        ret.addFrom(&(it->second[0]), (zfindex)it->second.size());
    }
}

// ============================================================
// ZFProperty
zfindex ZFClass::propertyCount(void) const
{
    this->_ZFP_ZFClass_methodAndPropertyAutoRegister();
    return d->propertyList.count();
}
const ZFProperty *ZFClass::propertyAtIndex(ZF_IN zfindex index) const
{
    this->_ZFP_ZFClass_methodAndPropertyAutoRegister();
    if(index >= d->propertyList.count())
    {
        zfCoreCriticalIndexOutOfRange(index, this->propertyCount());
        return zfnull;
    }
    return d->propertyList.get(index);
}

void ZFClass::propertyGetAllT(ZF_IN_OUT ZFCoreArray<const ZFProperty *> &ret) const
{
    this->_ZFP_ZFClass_methodAndPropertyAutoRegister();
    for(zfstlmap<zfstlstringZ, const ZFProperty *>::iterator it = d->propertyMapCache.begin(); it != d->propertyMapCache.end(); ++it)
    {
        ret.add(it->second);
    }
}

const ZFProperty *ZFClass::propertyForNameIgnoreParent(const zfchar *propertyName) const
{
    this->_ZFP_ZFClass_methodAndPropertyAutoRegister();
    if(!d->propertyMap.isEmpty())
    {
        const ZFProperty *t = d->propertyMap.get<const ZFProperty *>(propertyName);
        return t;
    }
    return zfnull;
}
const ZFProperty *ZFClass::propertyForName(const zfchar *propertyName) const
{
    if(propertyName == zfnull)
    {
        return zfnull;
    }
    this->_ZFP_ZFClass_methodAndPropertyAutoRegister();
    zfstlmap<zfstlstringZ, const ZFProperty *>::iterator it = d->propertyMapCache.find(propertyName);
    if(it == d->propertyMapCache.end())
    {
        return zfnull;
    }
    else
    {
        return it->second;
    }
}

const ZFMethod *ZFClass::propertySetterForNameIgnoreParent(const zfchar *propertyName) const
{
    this->_ZFP_ZFClass_methodAndPropertyAutoRegister();

    if(!d->methodMap.empty())
    {
        zfstlmap<zfstlstringZ, zfstlvector<const ZFMethod *> >::iterator itName = d->methodMap.find(propertyName);
        if(itName != d->methodMap.end())
        {
            zfstlvector<const ZFMethod *> &l = itName->second;
            for(zfstlsize i = 0; i < l.size(); ++i)
            {
                const ZFMethod *m = l[i];
                if(m->methodParamCountMin() == 1)
                {
                    return m;
                }
            }
        }
    }
    return zfnull;
}
const ZFMethod *ZFClass::propertySetterForName(const zfchar *propertyName) const
{
    if(propertyName == zfnull)
    {
        return zfnull;
    }
    this->_ZFP_ZFClass_methodAndPropertyAutoRegister();
    this->_ZFP_ZFClass_methodMapCacheUpdate();
    zfstlmap<zfstlstringZ, zfstlvector<const ZFMethod *> >::iterator it = d->methodMapCache.find(propertyName);
    if(it == d->methodMapCache.end())
    {
        return zfnull;
    }
    for(zfindex i = 0; i < it->second.size(); ++i)
    {
        if(it->second[i]->methodParamCountMin() == 1)
        {
            return it->second[i];
        }
    }
    return zfnull;
}
const ZFMethod *ZFClass::propertyGetterForNameIgnoreParent(const zfchar *propertyName) const
{
    this->_ZFP_ZFClass_methodAndPropertyAutoRegister();

    if(!d->methodMap.empty())
    {
        zfstlmap<zfstlstringZ, zfstlvector<const ZFMethod *> >::iterator itName = d->methodMap.find(propertyName);
        if(itName != d->methodMap.end())
        {
            zfstlvector<const ZFMethod *> &l = itName->second;
            for(zfstlsize i = 0; i < l.size(); ++i)
            {
                const ZFMethod *m = l[i];
                if(m->methodParamCountMin() == 0)
                {
                    return m;
                }
            }
        }
    }
    return zfnull;
}
const ZFMethod *ZFClass::propertyGetterForName(const zfchar *propertyName) const
{
    if(propertyName == zfnull)
    {
        return zfnull;
    }
    this->_ZFP_ZFClass_methodAndPropertyAutoRegister();
    this->_ZFP_ZFClass_methodMapCacheUpdate();
    zfstlmap<zfstlstringZ, zfstlvector<const ZFMethod *> >::iterator it = d->methodMapCache.find(propertyName);
    if(it == d->methodMapCache.end())
    {
        return zfnull;
    }
    for(zfindex i = 0; i < it->second.size(); ++i)
    {
        if(it->second[i]->methodParamCountMin() == 0)
        {
            return it->second[i];
        }
    }
    return zfnull;
}

zfbool ZFClass::propertyHasOverrideInitStep(void) const
{
    return !(d->propertyInitStepMap.empty());
}
zfbool ZFClass::propertyHasOverrideInitStep(ZF_IN const ZFProperty *property) const
{
    return (d->propertyInitStepMap.find(property) != d->propertyInitStepMap.end());
}

// ============================================================
// class instance methods
void ZFClass::classTag(ZF_IN const zfchar *key,
                       ZF_IN ZFObject *tag) const
{
    if(key == zfnull)
    {
        return ;
    }
    if(tag != zfnull && ZFFrameworkStateCheck(ZFLevelZFFrameworkEssential) != ZFFrameworkStateAvailable)
    {
        // must not store any new tags during cleanup
        abort();
    }
    zfCoreMutexLocker();
    _ZFP_ZFClassTagMapType &m = d->classTagMap;
    _ZFP_ZFClassTagMapType::iterator it = m.find(key);
    if(it == m.end())
    {
        if(tag != zfnull)
        {
            m[key] = tag;
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
ZFObject *ZFClass::classTag(ZF_IN const zfchar *key) const
{
    if(key != zfnull)
    {
        zfCoreMutexLocker();
        _ZFP_ZFClassTagMapType::iterator it = d->classTagMap.find(key);
        if(it != d->classTagMap.end())
        {
            return it->second.toObject();
        }
    }
    return zfnull;
}
void ZFClass::classTagGetAllKeyValue(ZF_IN_OUT ZFCoreArray<const zfchar *> &allKey,
                                     ZF_IN_OUT ZFCoreArray<ZFObject *> &allValue) const
{
    _ZFP_ZFClassTagMapType &m = d->classTagMap;
    allKey.capacity(allKey.count() + m.size());
    allValue.capacity(allValue.count() + m.size());
    for(_ZFP_ZFClassTagMapType::iterator it = m.begin(); it != m.end(); ++it)
    {
        allKey.add(it->first.c_str());
        allValue.add(it->second.toObject());
    }
}
zfautoObject ZFClass::classTagRemoveAndGet(ZF_IN const zfchar *key) const
{
    if(key != zfnull)
    {
        zfCoreMutexLocker();
        _ZFP_ZFClassTagMapType &m = d->classTagMap;
        _ZFP_ZFClassTagMapType::iterator it = m.find(key);
        if(it != m.end())
        {
            zfautoObject ret;
            ret.zflockfree_assign(it->second);
            m.erase(it);
            return ret;
        }
    }
    return zfnull;
}
void ZFClass::classTagRemoveAll(void) const
{
    if(!d->classTagMap.empty())
    {
        _ZFP_ZFClassTagMapType tmp;
        tmp.swap(d->classTagMap);
    }
}

// ============================================================
// private
ZFClass::ZFClass(void)
: d(zfnull)
{
    d = zfnew(_ZFP_ZFClassPrivate);
    d->pimplOwner = this;
    this->_ZFP_ZFClassNeedInitImplementationList = zftrue;
}
ZFClass::~ZFClass(void)
{
    if(!d->classTagMap.empty())
    {
        // class tags must be removed before destroying a ZFClass
        abort();
    }

    zfdelete(d);
    d = zfnull;
}

ZFClass *ZFClass::_ZFP_ZFClassRegister(ZF_IN zfbool *ZFCoreLibDestroyFlag,
                                       ZF_IN const zfchar *classNamespace,
                                       ZF_IN const zfchar *className,
                                       ZF_IN const ZFClass *parent,
                                       ZF_IN _ZFP_zfAllocWithCacheCallback objectAllocWithCacheCallback,
                                       ZF_IN _ZFP_ZFObjectConstructor constructor,
                                       ZF_IN _ZFP_ZFObjectDestructor destructor,
                                       ZF_IN _ZFP_ZFObjectCheckInitImplementationListCallback checkInitImplListCallback,
                                       ZF_IN zfbool isInterface,
                                       ZF_IN zfbool classIsDynamicRegister,
                                       ZF_IN ZFObject *classDynamicRegisterUserData)
{
    zfCoreMutexLocker();

    // method data holder is required during _ZFP_ZFClassUnregister,
    // access here to ensure init order
    _ZFP_ZFMethodDataHolderInit();

    classNamespace = ZFNamespaceSkipGlobal(classNamespace);
    zfstring classNameFull;
    if(classNamespace != zfnull)
    {
        classNameFull += classNamespace;
        classNameFull += ZFNamespaceSeparator();
    }
    classNameFull += className;

    ZFCorePointerBase *d = _ZFP_ZFClassMap.get(classNameFull);
    ZFClass *cls = zfnull;
    if(d != zfnull)
    {
        cls = d->pointerValueT<ZFClass *>();
        if(cls->d->isInterface != isInterface || cls->d->classParent != parent)
        {
            zfCoreCriticalMessageTrim("[ZFClass] register a class that already registered: %s", className);
            return zfnull;
        }
        ++(cls->d->refCount);
    }
    else
    {
        cls = zfnew(ZFClass);
        _ZFP_ZFClassMap.set(classNameFull, ZFCorePointerForObject<ZFClass *>(cls));

        if(ZFCoreLibDestroyFlag)
        {
            cls->d->ZFCoreLibDestroyFlag.add(ZFCoreLibDestroyFlag);
        }

        cls->d->classIsDynamicRegister = classIsDynamicRegister;
        cls->d->classDynamicRegisterUserData = classDynamicRegisterUserData;
        cls->d->objectAllocWithCacheCallback = objectAllocWithCacheCallback;
        cls->d->constructor = constructor;
        cls->d->destructor = destructor;

        cls->d->classNamespace = classNamespace;
        cls->d->className = className;
        cls->d->classNameFull = classNameFull;
        cls->classNamespaceCache = (cls->d->classNamespace.isEmpty() ? zfnull : cls->d->classNamespace.cString());
        cls->classNameCache = cls->d->className.cString();
        cls->classNameFullCache = cls->d->classNameFull.cString();

        cls->d->classParent = parent;
        cls->classParentCache = parent;

        cls->d->isInterface = isInterface;

        {
            const zfchar *filter = "_ZFP_";
            const zfindex filterLen = zfslen(filter);
            cls->d->isPrivateClass = (zfsncmp(className, filter, filterLen) == 0);
        }
        {
            const zfchar *filter = "_ZFP_I_";
            const zfindex filterLen = zfslen(filter);
            cls->d->isInternalClass = (zfsncmp(className, filter, filterLen) == 0);
        }
    }

    if(checkInitImplListCallback)
    {
        checkInitImplListCallback(cls);
    }
    ZFClass::_ZFP_ZFClassInitFinish(cls);

    return cls;
}
void ZFClass::_ZFP_ZFClassUnregister(ZF_IN zfbool *ZFCoreLibDestroyFlag, ZF_IN const ZFClass *cls)
{
    if(ZFCoreLibDestroyFlag && *ZFCoreLibDestroyFlag)
    {
        return ;
    }
    zfCoreMutexLocker();

    if(cls->classIsDynamicRegister())
    {
        for(zfstlmap<ZFObject *, zfbool>::iterator it = cls->d->classDynamicRegisterObjectInstanceMap.begin(); it != cls->d->classDynamicRegisterObjectInstanceMap.end(); ++it)
        {
            it->first->_ZFP_ZFObject_classData = zfnull;
        }
        cls->d->classDynamicRegisterObjectInstanceMap.clear();
    }

    zfiterator itClass = _ZFP_ZFClassMap.iteratorFind(cls->classNameFull());
    if(!_ZFP_ZFClassMap.iteratorValid(itClass))
    {
        zfCoreCriticalShouldNotGoHere();
        return ;
    }

    _ZFP_ZFClassDataChangeNotify(ZFClassDataChangeTypeDetach, cls, zfnull, zfnull);

    _ZFP_ZFClassPrivate *d = cls->d;
    d->ZFCoreLibDestroyFlag.removeElement(ZFCoreLibDestroyFlag);
    --(d->refCount);
    if(d->refCount != 0)
    {
        return ;
    }

    _ZFP_ZFClassDelayDeleteMap.set(cls->classNameFull(),
        *_ZFP_ZFClassMap.iteratorValue(itClass));
    _ZFP_ZFClassMap.iteratorRemove(itClass);

    if(!d->internalTypesNeedAutoRegister)
    {
        ZFMethodUserUnregister(cls->methodForNameIgnoreParent("ClassData"));
    }

    d->classDynamicRegisterUserData = zfnull;
    cls->classTagRemoveAll();
    cls->instanceObserverRemoveAll();

    for(zfstlmap<const ZFClass *, zfbool>::iterator it = d->allChildren.begin(); it != d->allChildren.end(); ++it)
    {
        it->first->d->allParent.erase(cls);
        if(it->first->d->classParent == cls)
        {
            it->first->d->classParent = zfnull;
        }
    }
    for(zfstlmap<const ZFClass *, zfbool>::iterator it = d->allParent.begin(); it != d->allParent.end(); ++it)
    {
        it->first->d->allChildren.erase(cls);
    }
}

zfbool ZFClass::_ZFP_ZFClass_interfaceNeedRegister(void)
{
    return d->needRegisterImplementedInterface;
}
void ZFClass::_ZFP_ZFClass_interfaceRegister(ZF_IN zfint dummy,
                                             ZF_IN const ZFClass *cls,
                                             ZF_IN _ZFP_ZFObjectToInterfaceCastCallback callback,
                                             ...)
{
    ZFCoreArrayPOD<const ZFClass *> clsList;
    ZFCoreArrayPOD<_ZFP_ZFObjectToInterfaceCastCallback> callbackList;
    {
        va_list vaList;
        va_start(vaList, callback);
        const ZFClass *clsTmp = cls;
        _ZFP_ZFObjectToInterfaceCastCallback callbackTmp = callback;
        do
        {
            if(clsTmp != ZFInterface::ClassData())
            {
                d->implementedInterface.add(clsTmp);
                clsList.add(clsTmp);
                callbackList.add(callbackTmp);
            }
            clsTmp = va_arg(vaList, const ZFClass *);
            if(clsTmp == zfnull)
            {
                break;
            }
            callbackTmp = va_arg(vaList, _ZFP_ZFObjectToInterfaceCastCallback);
        } while(zftrue);
        va_end(vaList);
    }

    d->interfaceCastListCache = (const ZFClass **)zfrealloc(d->interfaceCastListCache, sizeof(const ZFClass *) * (clsList.count() + 1));
    zfmemcpy(d->interfaceCastListCache, clsList.arrayBuf(), sizeof(const ZFClass *) * clsList.count());
    d->interfaceCastListCache[clsList.count()] = zfnull;

    d->interfaceCastCallbackListCache = (_ZFP_ZFObjectToInterfaceCastCallback *)zfrealloc(
        d->interfaceCastCallbackListCache, sizeof(_ZFP_ZFObjectToInterfaceCastCallback) * (callbackList.count() + 1));
    zfmemcpy(d->interfaceCastCallbackListCache, callbackList.arrayBuf(), sizeof(_ZFP_ZFObjectToInterfaceCastCallback) * callbackList.count());
    d->interfaceCastCallbackListCache[callbackList.count()] = zfnull;
}
ZFInterface *ZFClass::_ZFP_ZFClass_interfaceCast(ZF_IN ZFObject * const &obj,
                                                 ZF_IN const ZFClass *interfaceClass) const
{
    if(obj)
    {
        const ZFClass **parentInterfaceListTmp = d->interfaceCastListCache;
        _ZFP_ZFObjectToInterfaceCastCallback *parentInterfaceCastListTmp = d->interfaceCastCallbackListCache;
        do
        {
            if(*parentInterfaceListTmp == interfaceClass)
            {
                return (*parentInterfaceCastListTmp)(obj);
            }
            ++parentInterfaceListTmp;
            ++parentInterfaceCastListTmp;
        } while(*parentInterfaceListTmp);
    }
    return zfnull;
}

void ZFClass::_ZFP_ZFClassInitFinish(ZF_IN ZFClass *cls)
{
    if(cls->d->needFinalInit)
    {
        cls->d->needFinalInit = zffalse;

        ZFClass::_ZFP_ZFClassInitFinish_parentListCache(cls);
        ZFClass::_ZFP_ZFClassInitFinish_parentInterfaceListCache(cls);
        ZFClass::_ZFP_ZFClassInitFinish_interfaceCastListCache(cls);
        ZFClass::_ZFP_ZFClassInitFinish_allParentAndChildrenCache(cls);
        ZFClass::_ZFP_ZFClassInitFinish_methodAndPropertyFindCache(cls);
        ZFClass::_ZFP_ZFClassInitFinish_propertyMetaDataCache(cls);
        ZFClass::_ZFP_ZFClassInitFinish_instanceObserverCache(cls);

        _ZFP_ZFClassDataChangeNotify(ZFClassDataChangeTypeAttach, cls, zfnull, zfnull);
    }
}
void ZFClass::_ZFP_ZFClassInitFinish_parentListCache(ZF_IN ZFClass *cls)
{ // init parent list for better search performance
    ZFCoreArrayPOD<const ZFClass *> parentList;
    {
        parentList.add(cls);
        const ZFClass *parentTmp = cls->classParent();
        while(parentTmp != zfnull)
        {
            parentList.add(parentTmp);
            parentTmp = parentTmp->classParent();
        }
    }
    cls->d->parentListCache = (const ZFClass **)zfmalloc(sizeof(const ZFClass *) * (parentList.count() + 1));
    zfmemcpy(cls->d->parentListCache, parentList.arrayBuf(), sizeof(const ZFClass *) * parentList.count());
    cls->d->parentListCache[parentList.count()] = zfnull;
}
void ZFClass::_ZFP_ZFClassInitFinish_parentInterfaceListCache(ZF_IN ZFClass *cls)
{ // init parent type list for better search performance
    ZFCoreArrayPOD<const ZFClass *> parentList;
    {
        ZFCoreArrayPOD<const ZFClass *> clsToCheck;
        clsToCheck.add(cls);
        do
        {
            const ZFClass *clsTmp = clsToCheck.getFirst();
            clsToCheck.removeFirst();
            if(parentList.find(clsTmp) == zfindexMax())
            {
                parentList.add(clsTmp);

                if(clsTmp->classParent() != zfnull)
                {
                    clsToCheck.add(clsTmp->classParent());
                }
                clsToCheck.addFrom(clsTmp->d->implementedInterface);
            }
        } while(!clsToCheck.isEmpty());
        for(zfindex i = 0; i < parentList.count(); ++i)
        {
            if(!parentList[i]->classIsInterface())
            {
                parentList.remove(i);
                --i;
            }
        }
    }
    cls->d->parentInterfaceListCache = (const ZFClass **)zfmalloc(sizeof(const ZFClass *) * (parentList.count() + 1));
    zfmemcpy(cls->d->parentInterfaceListCache, parentList.arrayBuf(), sizeof(const ZFClass *) * parentList.count());
    cls->d->parentInterfaceListCache[parentList.count()] = zfnull;
}
void ZFClass::_ZFP_ZFClassInitFinish_interfaceCastListCache(ZF_IN ZFClass *cls)
{ // copy parent's interface cast datas
    ZFCoreArrayPOD<const ZFClass *> parentInterfaceList;
    if(cls->classParent() != zfnull)
    {
        for(const ZFClass **p = cls->classParent()->d->interfaceCastListCache; *p != zfnull; ++p)
        {
            if(parentInterfaceList.find(*p, ZFComparerCheckEqual) == zfindexMax())
            {
                parentInterfaceList.add(*p);
            }
        }
    }
    if(cls->d->interfaceCastListCache != zfnull)
    {
        for(const ZFClass **p = cls->d->interfaceCastListCache; *p != zfnull; ++p)
        {
            if(parentInterfaceList.find(*p, ZFComparerCheckEqual) == zfindexMax())
            {
                parentInterfaceList.add(*p);
            }
        }
    }
    cls->d->interfaceCastListCache = (const ZFClass **)zfrealloc(cls->d->interfaceCastListCache, sizeof(const ZFClass *) * (parentInterfaceList.count() + 1));
    zfmemcpy(cls->d->interfaceCastListCache, parentInterfaceList.arrayBuf(), sizeof(const ZFClass *) * parentInterfaceList.count());
    cls->d->interfaceCastListCache[parentInterfaceList.count()] = zfnull;

    ZFCoreArrayPOD<_ZFP_ZFObjectToInterfaceCastCallback> parentInterfaceCastList;
    if(cls->classParent() != zfnull)
    {
        for(_ZFP_ZFObjectToInterfaceCastCallback *p = cls->classParent()->d->interfaceCastCallbackListCache; *p != zfnull; ++p)
        {
            if(parentInterfaceCastList.find(*p, ZFComparerCheckEqual) == zfindexMax())
            {
                parentInterfaceCastList.add(*p);
            }
        }
    }
    if(cls->d->interfaceCastCallbackListCache != zfnull)
    {
        for(_ZFP_ZFObjectToInterfaceCastCallback *p = cls->d->interfaceCastCallbackListCache; *p != zfnull; ++p)
        {
            if(parentInterfaceCastList.find(*p, ZFComparerCheckEqual) == zfindexMax())
            {
                parentInterfaceCastList.add(*p);
            }
        }
    }
    cls->d->interfaceCastCallbackListCache = (_ZFP_ZFObjectToInterfaceCastCallback *)zfrealloc(
        cls->d->interfaceCastCallbackListCache, sizeof(_ZFP_ZFObjectToInterfaceCastCallback) * (parentInterfaceCastList.count() + 1));
    zfmemcpy(cls->d->interfaceCastCallbackListCache, parentInterfaceCastList.arrayBuf(), sizeof(_ZFP_ZFObjectToInterfaceCastCallback) * parentInterfaceCastList.count());
    cls->d->interfaceCastCallbackListCache[parentInterfaceCastList.count()] = zfnull;
}
void ZFClass::_ZFP_ZFClassInitFinish_allParentAndChildrenCache(ZF_IN ZFClass *cls)
{ // all parent and children
    ZFCoreArrayPOD<const ZFClass *> clsToCheck;
    clsToCheck.add(cls);
    do
    {
        const ZFClass *clsTmp = clsToCheck.getFirst();
        clsToCheck.removeFirst();
        if(cls->d->allParent.find(clsTmp) == cls->d->allParent.end())
        {
            if(clsTmp != cls)
            {
                cls->d->allParent[clsTmp] = zftrue;
                clsTmp->d->allChildren[cls] = zftrue;
            }

            if(clsTmp->classParent() != zfnull)
            {
                clsToCheck.add(clsTmp->classParent());
            }
            clsToCheck.addFrom(clsTmp->d->implementedInterface);
        }
    } while(!clsToCheck.isEmpty());
}
void ZFClass::_ZFP_ZFClassInitFinish_methodAndPropertyFindCache(ZF_IN ZFClass *cls)
{ // all class to find property and method
    zfstldeque<const ZFClass *> clsToCheck;
    zfstlmap<const ZFClass *, zfbool> alreadyChecked;
    clsToCheck.push_back(cls);
    do
    {
        const ZFClass *clsTmp = clsToCheck.front();
        clsToCheck.pop_front();
        if(alreadyChecked.find(clsTmp) != alreadyChecked.end())
        {
            continue;
        }
        alreadyChecked[clsTmp] = zftrue;

        cls->d->methodAndPropertyFindCache.push_back(clsTmp);
        for(zfiterator it = clsTmp->d->propertyMap.iterator();
            clsTmp->d->propertyMap.iteratorValid(it);
            clsTmp->d->propertyMap.iteratorNext(it))
        {
            const ZFProperty *zfproperty = clsTmp->d->propertyMap.iteratorValue<const ZFProperty *>(it);
            cls->d->propertyMapCache[zfproperty->propertyName()] = zfproperty;
        }

        for(zfindex i = clsTmp->implementedInterfaceCount() - 1; i != zfindexMax(); --i)
        {
            clsToCheck.push_back(clsTmp->implementedInterfaceAtIndex(i));
        }
        if(clsTmp->classParent())
        {
            clsToCheck.push_back(clsTmp->classParent());
        }
    } while(!clsToCheck.empty());
}
void ZFClass::_ZFP_ZFClassInitFinish_propertyMetaDataCache(ZF_IN ZFClass *cls)
{ // property init step
    for(zfstlmap<const ZFClass *, zfbool>::iterator itParent = cls->d->allParent.begin();
        itParent != cls->d->allParent.end();
        ++itParent)
    {
        cls->d->propertyAutoInitMap.insert(itParent->first->d->propertyAutoInitMap.begin(), itParent->first->d->propertyAutoInitMap.end());

        for(_ZFP_ZFClassPropertyInitStepMapType::iterator itProperty = itParent->first->d->propertyInitStepMap.begin();
            itProperty != itParent->first->d->propertyInitStepMap.end();
            ++itProperty)
        {
            cls->d->propertyInitStepMap[itProperty->first].insert(itProperty->second.begin(), itProperty->second.end());
        }
    }
}
void ZFClass::_ZFP_ZFClassInitFinish_instanceObserverCache(ZF_IN ZFClass *cls)
{ // copy all parent's instance observer to self
    for(zfstlmap<const ZFClass *, zfbool>::iterator it = cls->d->allParent.begin(); it != cls->d->allParent.end(); ++it)
    {
        zfstldeque<ZFCorePointerForObject<_ZFP_ZFClassPrivate::InstanceObserverData *> > &parentInstanceObserver = it->first->d->instanceObserver;
        for(zfstlsize i = 0; i < parentInstanceObserver.size(); ++i)
        {
            cls->d->instanceObserverCached.push_back(parentInstanceObserver[i]);
        }
    }
}

void ZFClass::_ZFP_ZFClass_objectDesctuct(ZF_IN ZFObject *obj) const
{
    d->destructor(obj);
}

void ZFClass::_ZFP_ZFClass_methodAndPropertyAutoRegister(void) const
{
    if(d->internalTypesNeedAutoRegister)
    {
        zfCoreMutexLocker();
        if(d->internalTypesNeedAutoRegister)
        {
            d->internalTypesNeedAutoRegister = zffalse;

            // ClassData() registered here instead of ZFOBJECT_REGISTER,
            // to reduce output executable size and runtime memory usage,
            // unregistered during class unregister
            ZFMethodUserRegisterDetail_0(resultMethod, {
                    return invokerMethod->methodOwnerClass();
                }, this, public, ZFMethodTypeStatic,
                const ZFClass *, "ClassData");

            {
                // create dummy instance to ensure static init of the object would take effect
                // including method and property register
                if(d->constructor != zfnull)
                {
                    d->destructor(d->constructor());
                }
            }
        }
    }
}

void ZFClass::_ZFP_ZFClass_methodRegister(ZF_IN const ZFMethod *method) const
{
    zfstlvector<const ZFMethod *> &methodList = d->methodMap[method->methodName()];
    methodList.push_back(method);
    d->methodList.add(method);

    if(!d->methodMapCacheNeedUpdate)
    {
        d->methodMapCache[method->methodName()].push_back(method);
        for(zfstlmap<const ZFClass *, zfbool>::iterator itChild = d->allChildren.begin(); itChild != d->allChildren.end(); ++itChild)
        {
            itChild->first->d->methodMapCacheNeedUpdate = zftrue;
        }
    }
}
void ZFClass::_ZFP_ZFClass_methodUnregister(ZF_IN const ZFMethod *method) const
{
    zfstlmap<zfstlstringZ, zfstlvector<const ZFMethod *> >::iterator itName = d->methodMap.find(method->methodName());
    if(itName != d->methodMap.end())
    {
        zfstlvector<const ZFMethod *> &l = itName->second;
        for(zfstlsize i = 0; i < l.size(); ++i)
        {
            if(method == l[i])
            {
                l.erase(l.begin() + i);
                if(l.empty())
                {
                    d->methodMap.erase(itName);
                    break;
                }
                d->methodList.removeElement(method);
            }
        }
    }

    if(!d->methodMapCacheNeedUpdate)
    {
        zfstlmap<zfstlstringZ, zfstlvector<const ZFMethod *> >::iterator it = d->methodMapCache.find(method->methodName());
        if(it != d->methodMapCache.end())
        {
            for(zfstlvector<const ZFMethod *>::iterator itTmp = it->second.begin(); itTmp != it->second.end(); ++itTmp)
            {
                if(*itTmp == method)
                {
                    it->second.erase(itTmp);
                    break;
                }
            }
        }
        for(zfstlmap<const ZFClass *, zfbool>::iterator itChild = d->allChildren.begin(); itChild != d->allChildren.end(); ++itChild)
        {
            itChild->first->d->methodMapCacheNeedUpdate = zftrue;
        }
    }
}

void ZFClass::_ZFP_ZFClass_methodMapCacheUpdate(void) const
{
    if(d->methodMapCacheNeedUpdate)
    {
        zfCoreMutexLocker();
        d->methodMapCacheNeedUpdate = zffalse;

        d->methodMapCache.clear();
        for(zfstlsize iCls = 0; iCls < d->methodAndPropertyFindCache.size(); ++iCls)
        {
            const ZFCoreArrayPOD<const ZFMethod *> &methodList = d->methodAndPropertyFindCache[iCls]->d->methodList;
            for(zfindex i = 0; i < methodList.count(); ++i)
            {
                const ZFMethod *method = methodList[i];
                d->methodMapCache[method->methodName()].push_back(method);
            }
        }
    }
}

void ZFClass::_ZFP_ZFClass_propertyRegister(ZF_IN const ZFProperty *zfproperty) const
{
    zfCoreAssertWithMessageTrim(d->propertyMapCache.find(zfproperty->propertyName()) == d->propertyMapCache.end(),
        "class %s already has property named %s",
        this->className(),
        zfproperty->propertyName());

    d->propertyMap.set(zfproperty->propertyName(), ZFCorePointerForPointerRef<const ZFProperty *>(zfproperty));
    d->propertyList.add(zfproperty);
    d->propertyMapCache[zfproperty->propertyName()] = zfproperty;

    for(zfstlmap<const ZFClass *, zfbool>::iterator it = d->allChildren.begin(); it != d->allChildren.end(); ++it)
    {
        it->first->d->propertyMapCache[zfproperty->propertyName()] = zfproperty;
    }
}
void ZFClass::_ZFP_ZFClass_propertyUnregister(ZF_IN const ZFProperty *zfproperty) const
{
    d->propertyMapCache.erase(zfproperty->propertyName());
    d->propertyMap.remove(zfproperty->propertyName());
    d->propertyList.removeElement(zfproperty);

    for(zfstlmap<const ZFClass *, zfbool>::iterator it = d->allChildren.begin(); it != d->allChildren.end(); ++it)
    {
        it->first->d->propertyMapCache.erase(zfproperty->propertyName());
    }
}

void ZFClass::_ZFP_ZFClass_propertyAutoInitRegister(ZF_IN const ZFProperty *property) const
{
    if(property->propertyOwnerClass() == this)
    {
        return ;
    }

    d->propertyAutoInitMap[property] = zftrue;

    for(zfstlmap<const ZFClass *, zfbool>::iterator it = d->allChildren.begin(); it != d->allChildren.end(); ++it)
    {
        it->first->d->propertyAutoInitMap[property] = zftrue;
    }
}
void ZFClass::_ZFP_ZFClass_propertyAutoInitAction(ZF_IN ZFObject *owner) const
{
    // access getter to ensure property value created
    for(zfstlmap<const ZFProperty *, zfbool>::iterator it = d->propertyAutoInitMap.begin(); it != d->propertyAutoInitMap.end(); ++it)
    {
        const ZFProperty *property = it->first;
        property->_ZFP_ZFProperty_callbackEnsureInit(property, owner);
    }
}
void ZFClass::_ZFP_ZFClass_propertyInitStepRegister(ZF_IN const ZFProperty *property) const
{
    if(property->propertyOwnerClass() == this)
    {
        return ;
    }

    d->propertyInitStepMap[property][this] = zftrue;

    for(zfstlmap<const ZFClass *, zfbool>::iterator it = d->allChildren.begin(); it != d->allChildren.end(); ++it)
    {
        it->first->d->propertyInitStepMap[property][this] = zftrue;
    }
}
zfbool ZFClass::_ZFP_ZFClass_propertyInitStepIsTheSame(ZF_IN const ZFProperty *property,
                                                       ZF_IN const ZFClass *refClass) const
{
    if(this == refClass)
    {
        return zftrue;
    }
    _ZFP_ZFClassPropertyInitStepMapType::iterator data = d->propertyInitStepMap.find(property);
    _ZFP_ZFClassPropertyInitStepMapType::iterator dataRef = refClass->d->propertyInitStepMap.find(property);
    if(data == d->propertyInitStepMap.end())
    {
        return (dataRef == refClass->d->propertyInitStepMap.end());
    }
    if(dataRef == refClass->d->propertyInitStepMap.end())
    {
        return (data == d->propertyInitStepMap.end());
    }

    if(data->second.size() != dataRef->second.size())
    {
        return zffalse;
    }
    if(data->second.size() == 0)
    {
        return zftrue;
    }
    if(this->classIsTypeOf(refClass) || refClass->classIsTypeOf(this))
    {
        return zftrue;
    }

    for(zfstlmap<const ZFClass *, zfbool>::iterator it = data->second.begin(), itRef = dataRef->second.begin();
        it != data->second.end();
        ++it, ++itRef)
    {
        if(it->first != itRef->first)
        {
            return zffalse;
        }
    }
    return zftrue;
}

_ZFP_zfAllocWithCacheCallback ZFClass::_ZFP_objectAllocWithCacheCallback(void) const
{
    return d->objectAllocWithCacheCallback;
}
_ZFP_ZFObjectConstructor ZFClass::_ZFP_objectConstructor(void) const
{
    return d->constructor;
}
_ZFP_ZFObjectDestructor ZFClass::_ZFP_objectDestructor(void) const
{
    return d->destructor;
}
void ZFClass::_ZFP_classDynamicRegisterObjectInstanceDetach(ZF_IN ZFObject *obj) const
{
    d->classDynamicRegisterObjectInstanceMap.erase(obj);
}

// ============================================================
_ZFP_ZFClassRegisterHolder::_ZFP_ZFClassRegisterHolder(ZF_IN const zfchar *classNamespace,
                                                       ZF_IN const zfchar *className,
                                                       ZF_IN const ZFClass *parent,
                                                       ZF_IN _ZFP_zfAllocWithCacheCallback objectAllocWithCacheCallback,
                                                       ZF_IN _ZFP_ZFObjectConstructor constructor,
                                                       ZF_IN _ZFP_ZFObjectDestructor destructor,
                                                       ZF_IN _ZFP_ZFObjectCheckInitImplementationListCallback checkInitImplListCallback,
                                                       ZF_IN_OPT zfbool isInterface /* = zffalse */,
                                                       ZF_IN_OPT zfbool classIsDynamicRegister /* = zffalse */,
                                                       ZF_IN_OPT ZFObject *classDynamicRegisterUserData /* = zfnull */)
: ZFCoreLibDestroyFlag(zffalse)
, cls(zfnull)
{
    cls = ZFClass::_ZFP_ZFClassRegister(
        &ZFCoreLibDestroyFlag,
        classNamespace,
        className,
        parent,
        objectAllocWithCacheCallback,
        constructor,
        destructor,
        checkInitImplListCallback,
        isInterface,
        classIsDynamicRegister,
        classDynamicRegisterUserData);
}
_ZFP_ZFClassRegisterHolder::~_ZFP_ZFClassRegisterHolder(void)
{
    ZFClass::_ZFP_ZFClassUnregister(&ZFCoreLibDestroyFlag, this->cls);
}

// ============================================================
void ZFClassGetAllT(ZF_IN_OUT ZFCoreArray<const ZFClass *> &ret,
                    ZF_IN_OPT const ZFFilterForZFClass *classFilter /* = zfnull */)
{
    zfCoreMutexLocker();
    if(classFilter == zfnull)
    {
        _ZFP_ZFClassMap.allValueT(ret);
    }
    else
    {
        const ZFCoreMap &m = _ZFP_ZFClassMap;
        for(zfiterator it = m.iterator(); m.iteratorValid(it); m.iteratorNext(it))
        {
            ZFClass *v = m.iteratorValue<ZFClass *>(it);
            if(classFilter->filterCheckActive(v))
            {
                ret.add(v);
            }
        }
    }
}

// ============================================================
ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
ZFOBSERVER_EVENT_GLOBAL_REGISTER(ClassDataChange)
ZF_NAMESPACE_END(ZFGlobalEvent)

ZFObserverHolder &_ZFP_ZFClassDataChangeObserverRef(void)
{
    static ZFObserverHolder d;
    return d;
}
void _ZFP_ZFClassDataChangeNotify(ZF_IN ZFClassDataChangeType changeType,
                                  ZF_IN const ZFClass *changedClass,
                                  ZF_IN const ZFProperty *changedProperty,
                                  ZF_IN const ZFMethod *changedMethod)
{
    zfCoreMutexLocker();
    if(ZFFrameworkStateCheck(ZFLevelZFFrameworkLow) == ZFFrameworkStateAvailable)
    {
        if(changedProperty != zfnull)
        {
            changedProperty->propertyOwnerClass()->_ZFP_ZFClass_classDataChangeNotify();
        }
        else if(changedMethod != zfnull)
        {
            if(changedMethod->methodOwnerClass() != zfnull)
            {
                changedMethod->methodOwnerClass()->_ZFP_ZFClass_classDataChangeNotify();
            }
        }

        ZFClassDataChangeData data;
        data.changeType = changeType;
        data.changedClass = changedClass;
        data.changedProperty = changedProperty;
        data.changedMethod = changedMethod;

        ZFPointerHolder *holder = zflockfree_zfAllocWithCache(ZFPointerHolder);
        holder->holdedData = &data;
        ZFClassDataChangeObserver.observerNotify(ZFGlobalEvent::EventClassDataChange(), holder);
        zflockfree_zfRelease(holder);
    }
}

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(void, ZFClassGetAllT, ZFMP_IN_OUT(ZFCoreArray<const ZFClass *> &, ret), ZFMP_IN_OPT(const ZFFilterForZFClass *, classFilter, zfnull))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(ZFCoreArrayPOD<const ZFClass *>, ZFClassGetAll, ZFMP_IN_OPT(const ZFFilterForZFClass *, classFilter, zfnull))

ZF_NAMESPACE_GLOBAL_END
#endif

