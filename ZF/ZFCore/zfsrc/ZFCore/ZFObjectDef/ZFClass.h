/**
 * @file ZFClass.h
 * @brief meta data for ZFObject
 */

#ifndef _ZFI_ZFClass_h_
#define _ZFI_ZFClass_h_

#include "zfautoObjectFwd.h"
#include "ZFObjectObserver.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassFwd ZFObject;
zfclassFwd ZFInterface;
zfclassFwd ZFMethod;
zfclassFwd ZFProperty;

typedef ZFObject *(*_ZFP_ZFObjectConstructor)(void);
typedef ZFObject *(*_ZFP_zfAllocWithCacheCallback)(void);
typedef void (*_ZFP_ZFObjectDestructor)(ZF_IN ZFObject *obj);
typedef void (*_ZFP_ZFObjectCheckInitImplementationListCallback)(ZF_IN_OUT ZFClass *cls);
typedef ZFInterface * (*_ZFP_ZFObjectToInterfaceCastCallback)(ZF_IN ZFObject * const &obj);

// ============================================================
/** @brief see #ZFClass::instanceObserverAdd */
zffinal zfclassLikePOD ZF_ENV_EXPORT ZFClassInstanceObserverAddParam
{
    ZFCORE_PARAM_DECLARE_SELF(ZFClassInstanceObserverAddParam)

    /** @brief see #ZFClass::instanceObserverAdd */
    ZFCORE_PARAM(ZFListener, observer)

    /** @brief see #ZFClass::instanceObserverAdd */
    ZFCORE_PARAM_WITH_INIT(ZFObject *, userData, zfnull)

    /** @brief see #ZFClass::instanceObserverAdd */
    ZFCORE_PARAM_WITH_INIT(ZFObject *, owner, zfnull)

    /** @brief see #ZFClass::instanceObserverAdd */
    ZFCORE_PARAM_WITH_INIT(ZFLevel, observerLevel, ZFLevelAppNormal)

    /** @brief see #ZFClass::instanceObserverAdd */
    ZFCORE_PARAM_WITH_INIT(zfbool, observeAllChildType, zftrue)

public:
    /** @cond ZFPrivateDoc */
    zfbool operator == (ZF_IN ZFClassInstanceObserverAddParam const &ref) const
    {
        return (zftrue
                && this->observer() == ref.observer()
                && this->userData() == ref.userData()
                && this->owner() == ref.owner()
                && this->observerLevel() == ref.observerLevel()
                && this->observeAllChildType() == ref.observeAllChildType()
            );
    }
    zfbool operator != (ZF_IN ZFClassInstanceObserverAddParam const &ref) const
    {
        return !this->operator == (ref);
    }
    /** @endcond */
};

// ============================================================
zfclassFwd _ZFP_ZFClassPrivate;
/**
 * @brief ZFObject's class info
 * @see ZFObject
 */
zffinal zfclassNotPOD ZF_ENV_EXPORT ZFClass
{
    ZFCLASS_DISALLOW_COPY_CONSTRUCTOR(ZFClass)

    // ============================================================
    // static methods
public:
    /**
     * @brief get class info by name
     * @return class info or zfnull if not found
     *
     * typical usage:
     * @code
     *   const ZFClass *cls = ZFClass::classForName("MyObject");
     *   zfautoObject objTmp = ((cls != zfnull) ? cls->newInstance() : zfautoObject());
     *   MyObject *obj = objTmp.to<MyObject *>();
     * @endcode
     * @note the class to find must:
     *   -  inherit from ZFObject or its subclass
     *   -  have #ZFOBJECT_DECLARE declared in the class
     *   -  have #ZFOBJECT_REGISTER defined,
     *      or match the situation described in #ZFOBJECT_REGISTER
     *
     * @see ZFOBJECT_DECLARE, ZFOBJECT_REGISTER
     *
     * @note the className may be simple format ("MyObject")
     *   or full format ("Scope0.Scope1.MyObject"),
     *   while in simple format,
     *   the class must be placed in global scope (#ZF_NAMESPACE_GLOBAL)
     */
    static const ZFClass *classForName(ZF_IN const zfchar *classNameOrFullName);
    /**
     * @brief find class with explicit namespace, see #classForName
     */
    static const ZFClass *classForName(ZF_IN const zfchar *className,
                                       ZF_IN const zfchar *classNamespace);

    // ============================================================
    // instance observer
public:
    /**
     * @brief add an observer which would be called if any of this class's instance created
     */
    void instanceObserverAdd(ZF_IN const ZFListener &observer,
                             ZF_IN_OPT ZFObject *userData = zfnull,
                             ZF_IN_OPT ZFObject *owner = zfnull,
                             ZF_IN_OPT ZFLevel observerLevel = ZFLevelAppNormal,
                             ZF_IN_OPT zfbool observeAllChildType = zftrue) const;
    /** @brief see #instanceObserverAdd */
    inline void instanceObserverAdd(ZF_IN const ZFClassInstanceObserverAddParam &param) const
    {
        this->instanceObserverAdd(param.observer(), param.userData(), param.owner(), param.observerLevel(), param.observeAllChildType());
    }
    /** @brief see #instanceObserverAdd */
    void instanceObserverRemove(ZF_IN const ZFListener &observer) const;
    /** @brief see #instanceObserverAdd */
    void instanceObserverRemoveByOwner(ZF_IN ZFObject *owner) const;
    /**
     * @brief see #instanceObserverAdd
     *
     * @note it's unsafe to remove all observer manually,
     *   which may break unrelated modules' observer logic,
     *   remove only if necessary\n
     *   typically, you should remove exactly the one you have added
     */
    void instanceObserverRemoveAll(void) const;
    void _ZFP_ZFClass_instanceObserverNotify(ZF_IN ZFObject *obj) const;

    // ============================================================
    // class data change observer
public:
    void _ZFP_ZFClass_classDataChangeNotify(void) const;
    /**
     * @brief auto remove #classTagRemove when class meta data changed
     *
     * class meta data would change if:
     * -  method register or unregistered
     * -  property register or unregistered
     *
     *
     * this method is useful if you cached some data by #classTag
     * according to class's method or property list,
     * and want to update it again if meta data changed\n
     * see also #ZFClassDataChangeObserver
     */
    void classDataChangeAutoRemoveTagAdd(ZF_IN const zfchar *tag) const;
    /** @brief see #classDataChangeAutoRemoveTagAdd */
    void classDataChangeAutoRemoveTagRemove(ZF_IN const zfchar *tag) const;

    // ============================================================
    // class info
public:
    /** @brief see #objectInfo */
    void objectInfoT(ZF_IN_OUT zfstring &ret) const;
    /** @brief return object info */
    zfstring objectInfo(void) const
    {
        zfstring ret;
        this->objectInfoT(ret);
        return ret;
    }

    /** @brief see #objectInfoOfInheritTree */
    void objectInfoOfInheritTreeT(ZF_IN_OUT zfstring &ret) const;
    /**
     * @brief return a string describe the class inherit tree, debug use only
     */
    zfstring objectInfoOfInheritTree(void) const
    {
        zfstring ret;
        this->objectInfoOfInheritTreeT(ret);
        return ret;
    }

public:
    /**
     * @brief true if class is subclass of cls, or implements cls
     */
    zfbool classIsTypeOf(ZF_IN const ZFClass *cls) const;

public:
    /**
     * @brief class namespace, ensured null for global scope class
     */
    inline const zfchar *classNamespace(void) const
    {
        return this->classNamespaceCache;
    }
    /**
     * @brief class name, e.g. "ZFObject"
     */
    inline const zfchar *className(void) const
    {
        return this->classNameCache;
    }
    /**
     * @brief class full name, e.g. "NS0.NS1.YourObject"
     */
    inline const zfchar *classNameFull(void) const
    {
        return this->classNameFullCache;
    }

    /**
     * @brief class's parent, zfnull if none
     */
    inline const ZFClass *classParent(void) const
    {
        return this->classParentCache;
    }

    /**
     * @brief true if class is registered by #ZFClassDynamicRegister
     */
    zfbool classIsDynamicRegister(void) const;
    /**
     * @brief see #ZFClassDynamicRegister
     */
    ZFObject *classDynamicRegisterUserData(void) const;
    /**
     * @brief true if the class is abstract class
     *
     * #newInstance would return zfnull if the class is abstract
     */
    zfbool classIsAbstract(void) const;
    /**
     * @brief true if the class is an interface
     */
    zfbool classIsInterface(void) const;
    /**
     * @brief whether the class is private class
     *
     * all class that have "_ZFP_" as prefix would be treated as private class\n
     * a private class should not be used publicly,
     * however, you may still apply alloc observer for it,
     * see #classIsInternal
     */
    zfbool classIsPrivate(void) const;
    /**
     * @brief whether the class is internal class
     *
     * all class that have "_ZFP_I_" as prefix would be treated as internal class\n
     * an internal class should not be used publicly,
     * also, it would be ignored from alloc observer,
     * see #classIsPrivate
     */
    zfbool classIsInternal(void) const;

    /**
     * @brief make a new instance of ZFObject dynamically, which is described by ZFClass
     *
     * typical usage:
     * @code
     *   ZFObject *myObj = zfAlloc(MyObject);
     *   ...
     *   // OK, newObj is a new instance of MyObject
     *   zfautoObject newObjTmp = myObj->classData()->newInstance();
     *   ZFObject *newObj = newObjTmp.toObject();
     * @endcode
     * @note object's no param version of objectOnInit would be called,
     *   if you want to construct with params,
     *   use #newInstanceGeneric instead
     */
    zfautoObject newInstance(void) const;
    /**
     * @brief for advanced reflection use only
     *
     * create object by specified params,
     * the object must have proper #ZFObject::objectOnInit
     * declared as #ZFOBJECT_ON_INIT_DECLARE_1 series,
     * and params are passed according to #ZFMethodGenericInvoker
     * @note this method do much reflection steps to find proper objectOnInit,
     *   which may cause performance issue,
     *   use with caution,
     *   or use #newInstanceGenericBegin if available:
     *   @code
     *     zfautoObject result;
     *     ZFCoreArrayPOD<const ZFMethod *> objectOnInitMethodList;
     *     cls->methodForNameGetAllT(objectOnInitMethodList, "objectOnInit");
     *     // you may do your extra method filters before actual alloc the object
     *     void *token = cls->newInstanceGenericBegin();
     *     if(token != zfnull)
     *     {
     *         for(zfindex i = 0; i < objectOnInitMethodList.count(); ++i)
     *         {
     *             if(cls->newInstanceGenericCheck(token, objectOnInitMethodList[i], params...))
     *             {
     *                 result = cls->newInstanceGenericEnd(token, zftrue);
     *                 break;
     *             }
     *         }
     *         if(result == zfnull)
     *         {
     *             cls->newInstanceGenericEnd(token, zffalse);
     *         }
     *     }
     *   @endcode
     * @note if the objectOnInit is declared by #ZFOBJECT_ON_INIT_DECLARE_1 series,
     *   it's called as is,
     *   if it's user registered (#ZFMETHOD_USER_REGISTER_1 series),
     *   #ZFObject::objectOnInit would be called before invoke the method
     *   to prevent logic error
     * @note if all params are #ZFMethodGenericInvokerDefaultParam, this method would call
     *   original #ZFObject::objectOnInit instead (same as #newInstance)
     *
     * see also #ZFObjectTagKeyword_newInstanceGenericFailed
     */
    zfautoObject newInstanceGeneric(
                                      ZF_IN_OPT ZFObject *param0 = ZFMethodGenericInvokerDefaultParam()
                                    , ZF_IN_OPT ZFObject *param1 = ZFMethodGenericInvokerDefaultParam()
                                    , ZF_IN_OPT ZFObject *param2 = ZFMethodGenericInvokerDefaultParam()
                                    , ZF_IN_OPT ZFObject *param3 = ZFMethodGenericInvokerDefaultParam()
                                    , ZF_IN_OPT ZFObject *param4 = ZFMethodGenericInvokerDefaultParam()
                                    , ZF_IN_OPT ZFObject *param5 = ZFMethodGenericInvokerDefaultParam()
                                    , ZF_IN_OPT ZFObject *param6 = ZFMethodGenericInvokerDefaultParam()
                                    , ZF_IN_OPT ZFObject *param7 = ZFMethodGenericInvokerDefaultParam()
                                    ) const; /* ZFMETHOD_MAX_PARAM */
    /** @brief see #newInstanceGeneric */
    void *newInstanceGenericBegin(void) const;
    /** @brief see #newInstanceGeneric */
    zfbool newInstanceGenericCheck(ZF_IN void *&token
                                   , ZF_IN const ZFMethod *objectOnInitMethod
                                   , ZF_IN_OUT zfautoObject (&paramList)[ZFMETHOD_MAX_PARAM]
                                   , ZF_OUT_OPT zfstring *errorHint = zfnull) const; /* ZFMETHOD_MAX_PARAM */
    /** @brief see #newInstanceGeneric */
    zfautoObject newInstanceGenericEnd(ZF_IN void *&token,
                                       ZF_IN zfbool objectOnInitMethodInvokeSuccess) const;

    /**
     * @brief get implemented interface count
     */
    zfindex implementedInterfaceCount(void) const;
    /**
     * @brief get implemented interface
     */
    const ZFClass *implementedInterfaceAtIndex(ZF_IN zfindex index) const;

    // ============================================================
    // ZFMethod
public:
    /**
     * @brief get the count of method registered to this ZFClass
     *
     * note that parent class's method won't be included\n
     * the order is ensured to be the same as ZFMethod declared
     * @see methodAtIndex
     * @note can be found only if it matches the condition described in #ZFOBJECT_REGISTER
     */
    zfindex methodCount(void) const;
    /**
     * @brief get the method at index, assert fail if out of bounds
     *
     * note that parent class's method won't be included\n
     * the order is ensured to be the same as ZFMethod declared
     * @see methodForNameIgnoreParent, methodForName
     * @note can be found only if it matches the condition described in #ZFOBJECT_REGISTER
     */
    const ZFMethod *methodAtIndex(ZF_IN zfindex index) const;

    /** @brief see #methodGetAll */
    void methodGetAllT(ZF_IN_OUT ZFCoreArray<const ZFMethod *> &ret) const;
    /**
     * @brief get all method, including methods inherited from parent
     */
    ZFCoreArrayPOD<const ZFMethod *> methodGetAll(void) const
    {
        ZFCoreArrayPOD<const ZFMethod *> ret;
        this->methodGetAllT(ret);
        return ret;
    }

    /* ZFMETHOD_MAX_PARAM */
    /**
     * @brief get the method by name within this class only, or zfnull if not exists
     *
     * the following optional params are, type id of each param type
     * described by #ZFTypeId::TypeId,
     * if more than one method with same name (and or param type id) were found,
     * the first one would be returned
     *
     * @see methodAtIndex, methodForName
     * @note can be found only if it matches the condition described in #ZFOBJECT_REGISTER
     */
    const ZFMethod *methodForNameIgnoreParent(ZF_IN const zfchar *methodName
                                              , ZF_IN const zfchar *methodParamTypeId0
                                              , ZF_IN_OPT const zfchar *methodParamTypeId1 = zfnull
                                              , ZF_IN_OPT const zfchar *methodParamTypeId2 = zfnull
                                              , ZF_IN_OPT const zfchar *methodParamTypeId3 = zfnull
                                              , ZF_IN_OPT const zfchar *methodParamTypeId4 = zfnull
                                              , ZF_IN_OPT const zfchar *methodParamTypeId5 = zfnull
                                              , ZF_IN_OPT const zfchar *methodParamTypeId6 = zfnull
                                              , ZF_IN_OPT const zfchar *methodParamTypeId7 = zfnull
                                              ) const;
    /** @brief see #methodForNameIgnoreParent */
    const ZFMethod *methodForNameIgnoreParent(ZF_IN const zfchar *methodName) const;
    /**
     * @brief get the method by name hierarchically from class inherit tree, or zfnull if not exists
     *
     * searching from subclass to parent class, return the first matched
     * @see methodAtIndex, methodForNameIgnoreParent
     * @note can be found only if it matches the condition described in #ZFOBJECT_REGISTER
     * @note ensured breadth-first
     */
    const ZFMethod *methodForName(ZF_IN const zfchar *methodName
                                  , ZF_IN const zfchar *methodParamTypeId0
                                  , ZF_IN_OPT const zfchar *methodParamTypeId1 = zfnull
                                  , ZF_IN_OPT const zfchar *methodParamTypeId2 = zfnull
                                  , ZF_IN_OPT const zfchar *methodParamTypeId3 = zfnull
                                  , ZF_IN_OPT const zfchar *methodParamTypeId4 = zfnull
                                  , ZF_IN_OPT const zfchar *methodParamTypeId5 = zfnull
                                  , ZF_IN_OPT const zfchar *methodParamTypeId6 = zfnull
                                  , ZF_IN_OPT const zfchar *methodParamTypeId7 = zfnull
                                  ) const;
    /** @brief see #methodForName */
    const ZFMethod *methodForName(ZF_IN const zfchar *methodName) const;
    /** @brief see #methodForNameGetAll */
    void methodForNameGetAllT(ZF_IN_OUT ZFCoreArray<const ZFMethod *> &ret,
                              ZF_IN const zfchar *methodName) const;
    /**
     * @brief get all method with name, ignoring method id
     */
    ZFCoreArrayPOD<const ZFMethod *> methodForNameGetAll(ZF_IN const zfchar *methodName) const
    {
        ZFCoreArrayPOD<const ZFMethod *> ret;
        this->methodForNameGetAllT(ret, methodName);
        return ret;
    }

    // ============================================================
    // ZFProperty
public:
    /**
     * @brief get ZFProperty count registered to this class
     *
     * note that parent class's property won't be included\n
     * the order is ensured to be the same as property declared
     * @see propertyAtIndex
     * @note can be found only if it matches the condition described in #ZFOBJECT_REGISTER
     */
    zfindex propertyCount(void) const;
    /**
     * @brief get ZFProperty at index
     *
     * note that parent class's property won't be included\n
     * the order is ensured to be the same as property declared
     * @see propertyForName
     * @note can be found only if it matches the condition described in #ZFOBJECT_REGISTER
     */
    const ZFProperty *propertyAtIndex(ZF_IN zfindex index) const;

    /** @brief see #propertyGetAll */
    void propertyGetAllT(ZF_IN_OUT ZFCoreArray<const ZFProperty *> &ret) const;
    /**
     * @brief get all property, including properties inherited from parent
     */
    ZFCoreArrayPOD<const ZFProperty *> propertyGetAll(void) const
    {
        ZFCoreArrayPOD<const ZFProperty *> ret;
        this->propertyGetAllT(ret);
        return ret;
    }

    /**
     * @brief get the property by name within this class only, or zfnull if not exists
     *
     * propertyName should be "Property" without "set" or "get"\n
     * note that parent class's property won't be included
     * @see propertyAtIndex, propertyForName
     * @note can be found only if it matches the condition described in #ZFOBJECT_REGISTER
     */
    const ZFProperty *propertyForNameIgnoreParent(const zfchar *propertyName) const;
    /**
     * @brief get the property by name hierarchically from class inherit tree, or zfnull if not exists
     *
     * propertyName should be "Property" without "set" or "get"\n
     * searching from subclass to parent class, return the first matched
     * @see propertyAtIndex, propertyForName
     * @note can be found only if it matches the condition described in #ZFOBJECT_REGISTER
     * @note ensured breadth-first
     */
    const ZFProperty *propertyForName(const zfchar *propertyName) const;

    /**
     * @brief try to get property setter method within this class only
     *
     * note, the property has no need to exist,
     * we only search method with proper name and param and return type
     */
    const ZFMethod *propertySetterForNameIgnoreParent(const zfchar *propertyName) const;
    /**
     * @brief try to get property setter method
     *
     * note, the property has no need to exist,
     * we only search method with proper name and param and return type
     */
    const ZFMethod *propertySetterForName(const zfchar *propertyName) const;
    /**
     * @brief try to get property getter method within this class only
     *
     * note, the property has no need to exist,
     * we only search method with proper name and param and return type
     */
    const ZFMethod *propertyGetterForNameIgnoreParent(const zfchar *propertyName) const;
    /**
     * @brief try to get property getter method
     *
     * note, the property has no need to exist,
     * we only search method with proper name and param and return type
     */
    const ZFMethod *propertyGetterForName(const zfchar *propertyName) const;

    /**
     * @brief whether the property has #ZFPROPERTY_OVERRIDE_ON_INIT_DECLARE
     */
    zfbool propertyHasOverrideInitStep(void) const;
    /**
     * @brief whether the property has #ZFPROPERTY_OVERRIDE_ON_INIT_DECLARE
     */
    zfbool propertyHasOverrideInitStep(ZF_IN const ZFProperty *property) const;

    // ============================================================
    // class instance methods
public:
    /**
     * @brief set a classTag for the class, internal use only
     *
     * @note this method is not thread-safe
     * @note all classTags would be removed during #ZFFrameworkCleanup
     *   as #ZFLevelZFFrameworkHigh and #ZFLevelZFFrameworkEssential,
     *   you must ensure the classTag is safe to be deleted at this time
     * @note usually used to store meta-data for performance use only
     */
    void classTag(ZF_IN const zfchar *key,
                  ZF_IN ZFObject *tag) const;
    /**
     * @brief see #classTag
     */
    ZFObject *classTag(ZF_IN const zfchar *key) const;
    /**
     * @brief see #classTag
     */
    template<typename T_ZFObject>
    T_ZFObject classTag(ZF_IN const zfchar *key) const
    {
        return ZFCastZFObjectUnchecked(T_ZFObject, this->classTag(key));
    }
    /** @brief see #classTag */
    zffinal void classTagGetAllKeyValue(ZF_IN_OUT ZFCoreArray<const zfchar *> &allKey,
                                        ZF_IN_OUT ZFCoreArray<ZFObject *> &allValue) const;
    /**
     * @brief remove tag, same as set tag to null
     */
    inline void classTagRemove(ZF_IN const zfchar *key) const
    {
        this->classTag(key, zfnull);
    }
    /**
     * @brief remove tag, return removed tag or null if not exist
     */
    zffinal zfautoObject classTagRemoveAndGet(ZF_IN const zfchar *key) const;
    /**
     * @brief see #classTag
     *
     * @note it's unsafe to remove all tag manually,
     *   which may break unrelated modules' additional logic,
     *   remove only if necessary\n
     *   typically, you should remove exactly the one you have added
     */
    zffinal void classTagRemoveAll(void) const;

    // ============================================================
    // private
public:
    static ZFClass *_ZFP_ZFClassRegister(ZF_IN zfbool *ZFCoreLibDestroyFlag,
                                         ZF_IN const zfchar *classNamespace,
                                         ZF_IN const zfchar *className,
                                         ZF_IN const ZFClass *parent,
                                         ZF_IN _ZFP_zfAllocWithCacheCallback objectAllocWithCacheCallback,
                                         ZF_IN _ZFP_ZFObjectConstructor constructor,
                                         ZF_IN _ZFP_ZFObjectDestructor destructor,
                                         ZF_IN _ZFP_ZFObjectCheckInitImplementationListCallback checkInitImplListCallback,
                                         ZF_IN zfbool isInterface,
                                         ZF_IN zfbool classIsDynamicRegister,
                                         ZF_IN ZFObject *classDynamicRegisterUserData);
    static void _ZFP_ZFClassUnregister(ZF_IN zfbool *ZFCoreLibDestroyFlag, ZF_IN const ZFClass *cls);
    /** @cond ZFPrivateDoc */
    ZFClass(void);
    ~ZFClass(void);
    /** @endcond */
    zfbool _ZFP_ZFClass_interfaceNeedRegister(void);
    void _ZFP_ZFClass_interfaceRegister(ZF_IN zfint dummy,
                                        ZF_IN const ZFClass *cls,
                                        ZF_IN _ZFP_ZFObjectToInterfaceCastCallback callback,
                                        ...);
    ZFInterface *_ZFP_ZFClass_interfaceCast(ZF_IN ZFObject * const &obj,
                                            ZF_IN const ZFClass *interfaceClass) const;

    static void _ZFP_ZFClassInitFinish(ZF_IN ZFClass *cls);
    static void _ZFP_ZFClassInitFinish_parentListCache(ZF_IN ZFClass *cls);
    static void _ZFP_ZFClassInitFinish_parentInterfaceListCache(ZF_IN ZFClass *cls);
    static void _ZFP_ZFClassInitFinish_interfaceCastListCache(ZF_IN ZFClass *cls);
    static void _ZFP_ZFClassInitFinish_allParentAndChildrenCache(ZF_IN ZFClass *cls);
    static void _ZFP_ZFClassInitFinish_methodAndPropertyFindCache(ZF_IN ZFClass *cls);
    static void _ZFP_ZFClassInitFinish_propertyMetaDataCache(ZF_IN ZFClass *cls);
    static void _ZFP_ZFClassInitFinish_instanceObserverCache(ZF_IN ZFClass *cls);

    void _ZFP_ZFClass_objectDesctuct(ZF_IN ZFObject *obj) const;
    ZFClass *_ZFP_ZFClass_removeConst(void) const
    {
        return const_cast<ZFClass *>(this);
    }

    void _ZFP_ZFClass_methodAndPropertyAutoRegister(void) const;
    void _ZFP_ZFClass_methodRegister(ZF_IN const ZFMethod *method) const;
    void _ZFP_ZFClass_methodUnregister(ZF_IN const ZFMethod *method) const;
    void _ZFP_ZFClass_propertyRegister(ZF_IN const ZFProperty *zfproperty) const;
    void _ZFP_ZFClass_propertyUnregister(ZF_IN const ZFProperty *zfproperty) const;
    void _ZFP_ZFClass_propertyAutoInitRegister(ZF_IN const ZFProperty *property) const;
    void _ZFP_ZFClass_propertyAutoInitAction(ZF_IN ZFObject *owner) const;
    void _ZFP_ZFClass_propertyInitStepRegister(ZF_IN const ZFProperty *property) const;
    zfbool _ZFP_ZFClass_propertyInitStepIsTheSame(ZF_IN const ZFProperty *property,
                                                  ZF_IN const ZFClass *refClass) const;
    _ZFP_zfAllocWithCacheCallback _ZFP_objectAllocWithCacheCallback(void) const;
    _ZFP_ZFObjectConstructor _ZFP_objectConstructor(void) const;
    _ZFP_ZFObjectDestructor _ZFP_objectDestructor(void) const;
    void _ZFP_classDynamicRegisterObjectInstanceDetach(ZF_IN ZFObject *obj) const;

public:
    zfbool _ZFP_ZFClassNeedInitImplementationList;
private:
    _ZFP_ZFClassPrivate *d;
    friend zfclassFwd _ZFP_ZFClassPrivate;
    const zfchar *classNamespaceCache;
    const zfchar *classNameCache;
    const zfchar *classNameFullCache;
    const ZFClass *classParentCache;
};

/**
 * @brief used to mark object allocation failed for #ZFClass::newInstanceGenericCheck
 *
 * when object created by #ZFClass::newInstanceGeneric,
 * it's possible to have mismatch param for generic invoke of `objectOnInit`,
 * you may set an error message to the allocated object by #ZFObject::objectTag
 * with this as key and the error message (#v_zfstring) as value,
 * to tell #ZFClass::newInstanceGenericCheck that the allocation failed
 */
#define ZFObjectTagKeyword_newInstanceGenericFailed "_ZFP_NIGFail"

// ============================================================
zfclassLikePOD ZF_ENV_EXPORT _ZFP_ZFClassRegisterHolder
{
public:
    _ZFP_ZFClassRegisterHolder(ZF_IN const zfchar *classNamespace,
                               ZF_IN const zfchar *className,
                               ZF_IN const ZFClass *parent,
                               ZF_IN _ZFP_zfAllocWithCacheCallback objectAllocWithCacheCallback,
                               ZF_IN _ZFP_ZFObjectConstructor constructor,
                               ZF_IN _ZFP_ZFObjectDestructor destructor,
                               ZF_IN _ZFP_ZFObjectCheckInitImplementationListCallback checkInitImplListCallback,
                               ZF_IN_OPT zfbool isInterface = zffalse,
                               ZF_IN_OPT zfbool classIsDynamicRegister = zffalse,
                               ZF_IN_OPT ZFObject *classDynamicRegisterUserData = zfnull);
    ~_ZFP_ZFClassRegisterHolder(void);
public:
    zfbool ZFCoreLibDestroyFlag;
    ZFClass *cls;
};

// ============================================================
zfclassFwd ZFFilterForZFClass;
/** @brief see #ZFClassGetAll */
extern ZF_ENV_EXPORT void ZFClassGetAllT(ZF_IN_OUT ZFCoreArray<const ZFClass *> &ret,
                                         ZF_IN_OPT const ZFFilterForZFClass *classFilter = zfnull);
/**
 * @brief get all class currently registered, for debug use only
 */
inline ZFCoreArrayPOD<const ZFClass *> ZFClassGetAll(ZF_IN_OPT const ZFFilterForZFClass *classFilter = zfnull)
{
    ZFCoreArrayPOD<const ZFClass *> ret;
    ZFClassGetAllT(ret, classFilter);
    return ret;
}

// ============================================================
ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
/**
 * @brief see #ZFObject::observerNotify
 *
 * notified when any class data changed, notified to #ZFClassDataChangeObserver,
 *   see #ZFObject::observerNotify
 *
 * param0 is a #ZFPointerHolder holds a (const #ZFClassDataChangeData *) value\n
 * called when:
 * -  class registered or unregistered
 * -  class's property registered or unregistered (including user registered property)
 * -  class's method or function method registered or unregistered (including user registered method)
 *
 *
 * \n
 * for each event call, #ZFClassDataChangeData::changedClass,
 * #ZFClassDataChangeData::changedProperty
 * and #ZFClassDataChangeData::changedMethod
 * are ensured only one not null\n
 * \n
 * \n
 * note this method would be notified only if #ZFFrameworkStateCheck with #ZFLevelZFFrameworkLow
 * returned #ZFFrameworkStateAvailable
 *
 * @warning app code can add observer to this event,
 *   but must not manually notify this event\n
 *   also, take care of performance
 */
ZFOBSERVER_EVENT_GLOBAL(ClassDataChange)
ZF_NAMESPACE_END(ZFGlobalEvent)

extern ZF_ENV_EXPORT ZFObserverHolder &_ZFP_ZFClassDataChangeObserverRef(void);
/**
 * @brief see #ZFGlobalEvent::EventClassDataChange
 *
 * @warning app code can add observer to this event,
 *   but must not manually notify this event\n
 *   also, take care of performance
 */
#define ZFClassDataChangeObserver (_ZFP_ZFClassDataChangeObserverRef())

// ============================================================
zfclassNotPOD ZF_ENV_EXPORT _ZFP_ZFCoreCriticalClassNotTypeOf
{
public:
    static zfstring classInfo(ZF_IN const ZFClass *cls)
    {
        if(cls)
        {
            return cls->classNameFull();
        }
        else
        {
            return ZFTOKEN_zfnull;
        }
    }
};
/**
 * @brief log that likes "[file function (line)] class xxx not type of xxx"
 */
#define zfCoreCriticalClassNotTypeOf(cls, desired) \
    zfCoreCriticalMessage("class %s not type of %s", \
        _ZFP_ZFCoreCriticalClassNotTypeOf::classInfo(cls).cString(), \
        _ZFP_ZFCoreCriticalClassNotTypeOf::classInfo(desired).cString())
/**
 * @brief see #zfCoreCriticalClassNotTypeOf
 */
#define zfCoreCriticalClassNotTypeOfDetail(callerInfo, cls, desired) \
    zfCoreCriticalMessageDetail(callerInfo, "class %s not type of %s", \
        _ZFP_ZFCoreCriticalClassNotTypeOf::classInfo(cls).cString(), \
        _ZFP_ZFCoreCriticalClassNotTypeOf::classInfo(desired).cString())

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFClass_h_

