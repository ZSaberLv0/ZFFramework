/**
 * @file ZFObjectGlobalInstance.h
 * @brief global instance holder
 */
#ifndef _ZFI_ZFObjectGlobalInstance_h_
#define _ZFI_ZFObjectGlobalInstance_h_

#include "ZFTypeId.h"
#include "ZFMethodDeclare.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// global instance cleaner
/**
 * @brief add a global instance that would be auto deleted while #ZFFrameworkCleanup
 *
 * you must ensure the smart pointer is safe to be cleanup
 * during the assigned level,
 * if necessary,
 * use #ZFObjectGlobalInstanceRemove to remove manually before #ZFFrameworkCleanup
 */
extern ZF_ENV_EXPORT ZFCorePointerBase *ZFObjectGlobalInstanceAdd(ZF_IN const ZFCorePointerBase &sp,
                                                                  ZF_IN_OPT ZFLevel level = ZFLevelAppNormal);
/** @brief see #ZFObjectGlobalInstanceAdd */
extern ZF_ENV_EXPORT ZFCorePointerBase *ZFObjectGlobalInstanceAdd(ZF_IN ZFObject *obj,
                                                                  ZF_IN_OPT ZFLevel level = ZFLevelAppNormal);
/** @brief see #ZFObjectGlobalInstanceAdd */
extern ZF_ENV_EXPORT void ZFObjectGlobalInstanceRemove(ZF_IN ZFCorePointerBase *sp,
                                                       ZF_IN ZFLevel level);

// ============================================================
// private
zfclassLikePOD ZF_ENV_EXPORT _ZFP_ZFClassSingletonPointerHolder
{
public:
    void *d;
public:
    _ZFP_ZFClassSingletonPointerHolder(void) : d(zfnull) {}
};
typedef void (*_ZFP_ZFClassSingletonDeleteCallback)(ZF_IN void *instance);
extern ZF_ENV_EXPORT _ZFP_ZFClassSingletonPointerHolder *_ZFP_ZFClassSingletonInstanceRefAccess(ZF_IN const zfchar *sig);

zffinal zfclassNotPOD ZF_ENV_EXPORT _ZFP_ZFClassSingletonDeleteCallbackHolder
{
public:
    _ZFP_ZFClassSingletonDeleteCallback deleteCallback;
    void *instance;
public:
    _ZFP_ZFClassSingletonDeleteCallbackHolder(ZF_IN _ZFP_ZFClassSingletonDeleteCallback deleteCallback,
                                              ZF_IN void *instance)
    : deleteCallback(deleteCallback)
    , instance(instance)
    {
    }
    ~_ZFP_ZFClassSingletonDeleteCallbackHolder(void)
    {
        if(this->deleteCallback && this->instance)
        {
            this->deleteCallback(this->instance);
        }
    }
};

// ============================================================
// singleton for normal class type
#define _ZFP_ZFCLASS_SINGLETON_DECLARE(SetterAccessType, GetterAccessType, \
                                       AccessTypeName, accessMethodName) \
    GetterAccessType: \
        /** \n access singleton instance */ \
        static AccessTypeName *accessMethodName(void); \
    SetterAccessType: \
        /** @brief see @ref accessMethodName */ \
        static void accessMethodName(ZF_IN AccessTypeName *newInstance); \
    public: \
        static ZFCorePointerBase *&_ZFP_ZFClassSingletonCleaner_##accessMethodName(void); \
        static void _ZFP_ZFClassSingletonOnDelete_##accessMethodName(ZF_IN void *instance); \
    public:
#define _ZFP_ZFCLASS_SINGLETON_DEFINE(OwnerClass, AccessTypeName, ObjectTypeName, sig, accessMethodName, \
                                      ZFLevel_, \
                                      newAction, deleteAction, retainAction, releaseAction) \
    AccessTypeName *OwnerClass::accessMethodName(void) \
    { \
        static _ZFP_ZFClassSingletonPointerHolder *holder = _ZFP_ZFClassSingletonInstanceRefAccess(sig); \
        if(holder->d == zfnull) \
        { \
            if(ZFFrameworkStateCheck(ZFLevel_) == ZFFrameworkStateNotAvailable) \
            { \
                return zfnull; \
            } \
            AccessTypeName *t = newAction(ObjectTypeName); \
            OwnerClass::accessMethodName(t); \
            releaseAction(t); \
        } \
        return ZFCastStatic(AccessTypeName *, holder->d); \
    } \
    void OwnerClass::accessMethodName(ZF_IN AccessTypeName *newInstance) \
    { \
        if(ZFFrameworkStateCheck(ZFLevel_) == ZFFrameworkStateNotAvailable) \
        { \
            return ; \
        } \
        _ZFP_ZFClassSingletonPointerHolder *holder = _ZFP_ZFClassSingletonInstanceRefAccess(sig); \
        if(holder->d == newInstance) \
        { \
            return ; \
        } \
        ZFCorePointerBase *&cleanerRef = OwnerClass::_ZFP_ZFClassSingletonCleaner_##accessMethodName(); \
        ZFCorePointerBase *cleanerOld = cleanerRef; \
        cleanerRef = zfnull; \
        if(newInstance != zfnull) \
        { \
            holder->d = retainAction(newInstance); \
            cleanerRef = ZFObjectGlobalInstanceAdd(ZFCorePointerForObject<_ZFP_ZFClassSingletonDeleteCallbackHolder *>( \
                zfnew(_ZFP_ZFClassSingletonDeleteCallbackHolder, OwnerClass::_ZFP_ZFClassSingletonOnDelete_##accessMethodName, holder->d)), \
                ZFLevel_); \
        } \
        if(cleanerOld != zfnull) \
        { \
            ZFObjectGlobalInstanceRemove(cleanerOld, ZFLevel_); \
        } \
    } \
    ZFCorePointerBase *&OwnerClass::_ZFP_ZFClassSingletonCleaner_##accessMethodName(void) \
    { \
        static ZFCorePointerBase *_cleaner = zfnull; \
        return _cleaner; \
    } \
    void OwnerClass::_ZFP_ZFClassSingletonOnDelete_##accessMethodName(ZF_IN void *instance) \
    { \
        _ZFP_ZFClassSingletonPointerHolder *holder = _ZFP_ZFClassSingletonInstanceRefAccess(sig); \
        if(holder->d == instance) \
        { \
            holder->d = zfnull; \
        } \
        deleteAction(ZFCastStatic(AccessTypeName *, instance)); \
    }
/**
 * @brief util macro to declare a singleton instance access for normal object type
 *
 * usage:
 * @code
 *   // in *.h file
 *   zfclass YourObject
 *   {
 *       / **
 *        * comment here
 *        * /
 *       ZFCLASS_SINGLETON_DECLARE(YourObject, instance)
 *   };
 *   // in *.cpp file
 *   ZFCLASS_SINGLETON_DEFINE(YourObject, YourObject, instance)
 *
 *   // then you can access the instance by
 *   YourObject::instance();
 * @endcode
 * you may change the internal singleton instance by:
 * @code
 *   newInstance = zfnew(YourType); // must be created by zfnew
 *   YourObject::instance(newInstance); // set
 *   newInstance = YourObject::instance(); // must re-check new pointer value
 * @endcode
 */
#define ZFCLASS_SINGLETON_DECLARE(AccessTypeName, accessMethodName) \
    ZFCLASS_SINGLETON_DECLARE_DETAIL(protected, public, AccessTypeName, accessMethodName)
/**
 * @brief see #ZFCLASS_SINGLETON_DECLARE
 */
#define ZFCLASS_SINGLETON_DECLARE_DETAIL(SetterAccessType, GetterAccessType, AccessTypeName, accessMethodName) \
    _ZFP_ZFCLASS_SINGLETON_DECLARE(SetterAccessType, GetterAccessType, AccessTypeName, accessMethodName)
/**
 * @brief see #ZFCLASS_SINGLETON_DECLARE
 */
#define ZFCLASS_SINGLETON_DEFINE(OwnerClass, AccessTypeName, ObjectTypeName, accessMethodName) \
    ZFCLASS_SINGLETON_DEFINE_DETAIL(OwnerClass, AccessTypeName, ObjectTypeName, accessMethodName, ZFLevelAppNormal)
/**
 * @brief see #ZFCLASS_SINGLETON_DECLARE
 */
#define ZFCLASS_SINGLETON_DEFINE_DETAIL(OwnerClass, AccessTypeName, ObjectTypeName, accessMethodName, ZFLevel_) \
    _ZFP_ZFCLASS_SINGLETON_DEFINE(OwnerClass, AccessTypeName, ObjectTypeName, \
                                  zfsConnectLineFree("_ZFP_ZFClass_", #AccessTypeName, "_", #accessMethodName), \
                                  accessMethodName, ZFLevel_, \
                                  zfnew, zfdelete, ZFM_EXPAND, ZFUNUSED)

// ============================================================
// singleton for ZFObject
#define _ZFP_ZFOBJECT_SINGLETON_DECLARE(SetterAccessType, GetterAccessType, \
                                        AccessTypeName, accessMethodName) \
    GetterAccessType: \
        /** \n access singleton instance */ \
        ZFMETHOD_DECLARE_DETAIL_0( \
            GetterAccessType, ZFMethodTypeStatic, G, \
            AccessTypeName *, accessMethodName \
            ) \
    SetterAccessType: \
        /** @brief see @ref accessMethodName */ \
        ZFMETHOD_DECLARE_DETAIL_1( \
            SetterAccessType, ZFMethodTypeStatic, S, \
            void, accessMethodName \
            , ZFMP_IN(AccessTypeName *, param0) \
            ) \
        /** @cond ZFPrivateDoc */ \
    private: \
        _ZFP_ZFCLASS_SINGLETON_DECLARE(private, private, \
                                       AccessTypeName, _ZFP_ZFObjectSingleton_##accessMethodName) \
        /** @endcond */ \
    public:
#define _ZFP_ZFOBJECT_SINGLETON_DEFINE(OwnerClass, \
                                       AccessTypeName, ObjectTypeName, sig, accessMethodName, \
                                       ZFLevel_) \
    _ZFP_ZFCLASS_SINGLETON_DEFINE(OwnerClass, \
                                  AccessTypeName, ObjectTypeName, sig, _ZFP_ZFObjectSingleton_##accessMethodName, \
                                  ZFLevel_, \
                                  zfAlloc, zfRelease, zfRetain, zfRelease) \
    ZFMETHOD_DEFINE_DETAIL_0(OwnerClass, G, \
        AccessTypeName *, accessMethodName \
        ) \
    { \
        return zfself::_ZFP_ZFObjectSingleton_##accessMethodName(); \
    } \
    ZFMETHOD_DEFINE_DETAIL_1(OwnerClass, S, \
        void, accessMethodName \
        , ZFMP_IN(AccessTypeName *, param0) \
        ) \
    { \
        if(ZFFrameworkStateCheck(ZFLevel_) == ZFFrameworkStateNotAvailable) \
        { \
            return ; \
        } \
        zfself::_ZFP_ZFObjectSingleton_##accessMethodName(param0); \
    }

/**
 * @brief util macro to declare a singleton instance access for ZFObject type
 *
 * usage:
 * @code
 *   // in *.h file
 *   zfclass YourObject : zfextends ZFObject
 *   {
 *       / **
 *        * comment here (optional)
 *        * /
 *       ZFOBJECT_SINGLETON_DECLARE(YourObject, instance)
 *   };
 *   // in *.cpp file
 *   ZFOBJECT_SINGLETON_DEFINE(YourObject, YourObject, instance)
 *
 *   // then you can access the instance by
 *   YourObject::instance();
 *   // or manually change the instance by
 *   YourObject::instance(yourInstance);
 * @endcode
 * @note this macro would generate these reflectable ZFMethod for you,
 *   with this proto type:
 *   -  YourObject *instance(void);
 *   -  void instance(ZF_IN YourObject *);
 */
#define ZFOBJECT_SINGLETON_DECLARE(AccessTypeName, accessMethodName) \
    ZFOBJECT_SINGLETON_DECLARE_DETAIL(protected, public, AccessTypeName, accessMethodName)
/**
 * @brief see #ZFOBJECT_SINGLETON_DECLARE
 */
#define ZFOBJECT_SINGLETON_DECLARE_DETAIL(SetterAccessType, GetterAccessType, AccessTypeName, accessMethodName) \
    _ZFP_ZFOBJECT_SINGLETON_DECLARE(SetterAccessType, GetterAccessType, AccessTypeName, accessMethodName)
/**
 * @brief see #ZFOBJECT_SINGLETON_DECLARE
 */
#define ZFOBJECT_SINGLETON_DEFINE(OwnerClass, AccessTypeName, ObjectTypeName, accessMethodName) \
    ZFOBJECT_SINGLETON_DEFINE_DETAIL(OwnerClass, AccessTypeName, ObjectTypeName, accessMethodName, ZFLevelAppNormal)
/**
 * @brief see #ZFOBJECT_SINGLETON_DECLARE
 */
#define ZFOBJECT_SINGLETON_DEFINE_DETAIL(OwnerClass, \
                                         AccessTypeName, ObjectTypeName, accessMethodName, \
                                         ZFLevel_) \
    _ZFP_ZFOBJECT_SINGLETON_DEFINE(OwnerClass, \
                                   AccessTypeName, ObjectTypeName, \
                                   zfsConnectLineFree("_ZFP_ZFObject_", zfself::ClassData()->classNameFull(), "_", #AccessTypeName, #accessMethodName), \
                                   accessMethodName, \
                                   ZFLevel_)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFObjectGlobalInstance_h_

