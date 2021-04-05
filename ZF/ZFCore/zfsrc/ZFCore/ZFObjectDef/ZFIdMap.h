/**
 * @file ZFIdMap.h
 * @brief id to string map util, useful to define extendable string enum
 */

#ifndef _ZFI_ZFIdMap_h_
#define _ZFI_ZFIdMap_h_

#include "../ZFCoreDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
extern ZF_ENV_EXPORT const zfidentity *_ZFP_ZFIdMapRegister(ZF_IN zfbool *ZFCoreLibDestroyFlag,
                                                            ZF_IN const zfchar *idName,
                                                            ZF_IN_OPT zfbool isDynamicRegister = zffalse);
extern ZF_ENV_EXPORT void _ZFP_ZFIdMapUnregister(ZF_IN zfbool *ZFCoreLibDestroyFlag,
                                                 ZF_IN zfidentity idValue,
                                                 ZF_IN_OPT zfbool isDynamicRegister = zffalse);
/**
 * @brief see #ZFIDMAP
 *
 * get id name from id value, or null if no such id
 * @note can be found only if accessed or registered by #ZFIDMAP_REGISTER
 */
extern ZF_ENV_EXPORT const zfchar *ZFIdMapNameForId(ZF_IN zfidentity idValue);
/**
 * @brief see #ZFIDMAP
 *
 * get id value from id name, or #zfidentityInvalid if no such id name
 * @note can be found only if accessed or registered by #ZFIDMAP_REGISTER
 */
extern ZF_ENV_EXPORT zfidentity ZFIdMapIdForName(ZF_IN const zfchar *idName);

/**
 * @brief see #ZFIDMAP
 *
 * get all registered id data, for debug use only
 * @note can be found only if accessed or registered by #ZFIDMAP_REGISTER
 */
extern ZF_ENV_EXPORT void ZFIdMapGetAll(ZF_IN_OUT ZFCoreArrayPOD<zfidentity> &idValues, ZF_IN_OUT ZFCoreArrayPOD<const zfchar *> &idNames);

/**
 * @brief dynamically register your own id
 *
 * assert fail if already registered
 */
extern ZF_ENV_EXPORT zfidentity ZFIdMapDynamicRegister(ZF_IN const zfchar *idName);
/**
 * @brief unregister id that was registered by #ZFIdMapDynamicRegister
 *
 * do nothing if no such id,
 * assert fail if the id is not dynamically registered
 */
extern ZF_ENV_EXPORT void ZFIdMapDynamicUnregister(ZF_IN zfidentity idValue);

zfclassLikePOD ZF_ENV_EXPORT _ZFP_ZFIdMapHolder
{
public:
    _ZFP_ZFIdMapHolder(ZF_IN const zfchar *idName);
    ~_ZFP_ZFIdMapHolder(void);

public:
    zfbool ZFCoreLibDestroyFlag;
    zfstring idName;
    const zfidentity *idValue;
};
/**
 * @brief declare an id within ZFObject class scope
 *
 * usage:
 * @code
 *   zfclass YourClass ...
 *   {
 *       ZFIDMAP(YourSth)
 *   };
 *
 *   ZFIDMAP_REGISTER(YourClass, YourSth)
 * @endcode
 * declared id can be accessed by:
 * @code
 *   zfidentity idValue = YourClass::IdYourSth();
 *   const zfchar *idName = ZFIdMapNameForId(idValue);
 * @endcode
 * note that subclass may declare an id same as parent,
 * while the final id name is different:\n
 *   ParentClass::IdYourSth() => "ParentClass::IdYourSth"\n
 *   ChildClass::IdYourSth() => "ChildClass::IdYourSth"\n
 * \n
 * @note we declare the id as int types for performance,
 *   it's ensured each id has different value,
 *   but it's only ensured while app is running,
 *   after relaunching the app,
 *   the id value is not ensured the same,
 *   you should use the name of the id to store or pass between apps,
 *   and you can use #ZFIdMapIdForName or #ZFIdMapNameForId
 *   to convert them easily
 */
#define ZFIDMAP(YourIdName) \
    ZFIDMAP_DETAIL(Id, YourIdName)
/** @brief see #ZFIDMAP */
#define ZFIDMAP_DETAIL(prefix, YourIdName) \
    public: \
        /** \n */ \
        static zfidentity prefix##YourIdName(void) \
        { \
            return *(_ZFP_IM_##prefix##YourIdName().idValue); \
        } \
        static _ZFP_ZFIdMapHolder &_ZFP_IM_##prefix##YourIdName(void) \
        { \
            static _ZFP_ZFIdMapHolder d( \
                    zfsConnectLineFree(zfself::ClassData()->classNameFull(), "::", ZFM_TOSTRING_DIRECT(prefix), ZFM_TOSTRING_DIRECT(YourIdName)) \
                ); \
            return d; \
        }

/**
 * @brief declare an id in global scope, see #ZFIDMAP
 *
 * usage:
 * @code
 *   // in header files
 *   ZF_NAMESPACE_BEGIN(YourNamespace)
 *   / ** @brief you can add doxygen docs here * /
 *   ZFIDMAP_GLOBAL(YourSth)
 *   ZF_NAMESPACE_END(YourNamespace)
 *
 *   ZFIDMAP_GLOBAL_REGISTER(YourSth)
 *
 *   // use the id
 *   zfidentity idValue = YourNamespace::IdYourSth();
 * @endcode
 * unlike #ZFIDMAP, this macro would declare id outside of class scope
 */
#define ZFIDMAP_GLOBAL(YourIdName) \
    ZFIDMAP_GLOBAL_DETAIL(Id, YourIdName)
/** @brief see #ZFIDMAP_GLOBAL */
#define ZFIDMAP_GLOBAL_DETAIL(prefix, YourIdName) \
    /** @cond ZFPrivateDoc */ \
    zfclass ZF_ENV_EXPORT _ZFP_ZFIdMapHolder_##prefix##_##YourIdName \
    { \
    public: \
        static _ZFP_ZFIdMapHolder &h(void) \
        { \
            static _ZFP_ZFIdMapHolder d( \
                    zfsConnectLineFree(ZF_NAMESPACE_CURRENT(), "::", ZFM_TOSTRING_DIRECT(prefix), ZFM_TOSTRING_DIRECT(YourIdName)) \
                ); \
            return d; \
        } \
    }; \
    /** @endcond */ \
    /** \n see #ZFIDMAP_GLOBAL */ \
    inline zfidentity prefix##YourIdName(void) \
    { \
        return *(_ZFP_ZFIdMapHolder_##prefix##_##YourIdName::h().idValue); \
    }

/** @brief see #ZFIDMAP */
#define ZFIDMAP_REGISTER(Scope, YourIdName) \
    ZFIDMAP_REGISTER_DETAIL(Scope, Id, YourIdName)
/** @brief see #ZFIDMAP */
#define ZFIDMAP_REGISTER_DETAIL(Scope, prefix, YourIdName) \
    ZF_STATIC_REGISTER_INIT(ZFIdMap_##Scope##_##YourIdName) \
    { \
        (void)Scope::prefix##YourIdName(); \
        \
        { \
            ZFMethodUserRegisterDetail_0(resultMethod, { \
                    return Scope::prefix##YourIdName(); \
                }, Scope::ClassData(), public, ZFMethodTypeStatic, \
                zfidentity, ZFM_TOSTRING(prefix##YourIdName)); \
            this->m_id = resultMethod; \
        } \
    } \
    const ZFMethod *m_id; \
    const ZFMethod *m_idName; \
    ZF_STATIC_REGISTER_DESTROY(ZFIdMap_##Scope##_##YourIdName) \
    { \
        ZFMethodUserUnregister(this->m_id); \
        ZFMethodUserUnregister(this->m_idName); \
    } \
    ZF_STATIC_REGISTER_END(ZFIdMap_##Scope##_##YourIdName)

/** @brief see #ZFIDMAP */
#define ZFIDMAP_GLOBAL_REGISTER(YourIdName) \
    ZFIDMAP_GLOBAL_REGISTER_DETAIL(Id, YourIdName)
/** @brief see #ZFIDMAP */
#define ZFIDMAP_GLOBAL_REGISTER_DETAIL(prefix, YourIdName) \
    ZF_STATIC_REGISTER_INIT(ZFIdMap_##YourIdName) \
    { \
        (void)prefix##YourIdName(); \
        \
        { \
            ZFMethodFuncUserRegisterDetail_0(resultMethod, { \
                    return prefix##YourIdName(); \
                }, ZF_NAMESPACE_CURRENT(), Id, zfidentity, ZFM_TOSTRING(prefix##YourIdName)); \
            this->m_id = resultMethod; \
        } \
    } \
    ZF_STATIC_REGISTER_DESTROY(ZFIdMap_##YourIdName) \
    { \
        ZFMethodFuncUserUnregister(this->m_id); \
        ZFMethodFuncUserUnregister(this->m_idName); \
    } \
    const ZFMethod *m_id; \
    const ZFMethod *m_idName; \
    ZF_STATIC_REGISTER_END(ZFIdMap_##YourIdName) \

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFIdMap_h_

