/**
 * @file ZFObjectRetain.h
 * @brief retain count logic for ZFFramework
 */

#ifndef _ZFI_ZFObjectRetain_h_
#define _ZFI_ZFObjectRetain_h_

#include "ZFObjectCore.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// zfAlloc
/* ZFMETHOD_MAX_PARAM */
template<typename T_ZFObject, int valid>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_Obj_AllocCk;
template<typename T_ZFObject>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_Obj_AllocCk<T_ZFObject, 1>
{
public:
    static inline T_ZFObject *CanAlloc(T_ZFObject *obj)
    {
        obj->objectOnInit();
        obj->_ZFP_ZFObjectCheckOnInit();
        return obj;
    }
    template<typename P0>
    static inline T_ZFObject *CanAlloc(T_ZFObject *obj, P0 p0)
    {
        obj->objectOnInit(p0);
        obj->_ZFP_ZFObjectCheckOnInit();
        return obj;
    }
    template<typename P0, typename P1>
    static inline T_ZFObject *CanAlloc(T_ZFObject *obj, P0 p0, P1 p1)
    {
        obj->objectOnInit(p0, p1);
        obj->_ZFP_ZFObjectCheckOnInit();
        return obj;
    }
    template<typename P0, typename P1, typename P2>
    static inline T_ZFObject *CanAlloc(T_ZFObject *obj, P0 p0, P1 p1, P2 p2)
    {
        obj->objectOnInit(p0, p1, p2);
        obj->_ZFP_ZFObjectCheckOnInit();
        return obj;
    }
    template<typename P0, typename P1, typename P2, typename P3>
    static inline T_ZFObject *CanAlloc(T_ZFObject *obj, P0 p0, P1 p1, P2 p2, P3 p3)
    {
        obj->objectOnInit(p0, p1, p2, p3);
        obj->_ZFP_ZFObjectCheckOnInit();
        return obj;
    }
    template<typename P0, typename P1, typename P2, typename P3, typename P4>
    static inline T_ZFObject *CanAlloc(T_ZFObject *obj, P0 p0, P1 p1, P2 p2, P3 p3, P4 p4)
    {
        obj->objectOnInit(p0, p1, p2, p3, p4);
        obj->_ZFP_ZFObjectCheckOnInit();
        return obj;
    }
    template<typename P0, typename P1, typename P2, typename P3, typename P4, typename P5>
    static inline T_ZFObject *CanAlloc(T_ZFObject *obj, P0 p0, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)
    {
        obj->objectOnInit(p0, p1, p2, p3, p4, p5);
        obj->_ZFP_ZFObjectCheckOnInit();
        return obj;
    }
    template<typename P0, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
    static inline T_ZFObject *CanAlloc(T_ZFObject *obj, P0 p0, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6)
    {
        obj->objectOnInit(p0, p1, p2, p3, p4, p5, p6);
        obj->_ZFP_ZFObjectCheckOnInit();
        return obj;
    }
    template<typename P0, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
    static inline T_ZFObject *CanAlloc(T_ZFObject *obj, P0 p0, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7)
    {
        obj->objectOnInit(p0, p1, p2, p3, p4, p5, p6, p7);
        obj->_ZFP_ZFObjectCheckOnInit();
        return obj;
    }
};
template<typename T_ZFObject>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_Obj_AllocCk<T_ZFObject, 0>
{
};
#define _ZFP_zfAlloc(T_ZFObject, ...) \
    _ZFP_Obj_AllocCk<T_ZFObject, T_ZFObject::_ZFP_ZFObjectCanAlloc && T_ZFObject::_ZFP_ZFObjectCanAllocPublic>::CanAlloc( \
        _ZFP_ObjCastInternal(T_ZFObject *, T_ZFObject::_ZFP_Obj_ctor()) \
        , ##__VA_ARGS__)
/**
 * @brief alloc an object, see #ZFObject
 */
#define zfAlloc(T_ZFObject, ...) \
    (zfCoreMutexLockerHolder(), zflockfree_zfAlloc(T_ZFObject, ##__VA_ARGS__))
/** @brief no lock version of #zfAlloc, use with caution */
#define zflockfree_zfAlloc(T_ZFObject, ...) \
    _ZFP_zfAlloc(T_ZFObject, ##__VA_ARGS__)

// ============================================================
// zfRetain
inline void _ZFP_zfRetainAction(ZF_IN ZFObject *obj)
{
    if(obj)
    {
        obj->objectOnRetain();
    }
}
template<typename T_ZFObject>
inline T_ZFObject _ZFP_zfRetain(ZF_IN T_ZFObject obj)
{
    _ZFP_zfRetainAction(ZFCastZFObjectUnchecked(ZFObject *, obj));
    return obj;
}
/**
 * @brief retain an object, see #ZFObject
 */
#define zfRetain(obj) \
    (zfCoreMutexLockerHolder(), zflockfree_zfRetain(obj))
/** @brief no lock version of #zfRetain, use with caution */
#define zflockfree_zfRetain(obj) \
    _ZFP_zfRetain(obj)

// ============================================================
// zfRelease
inline void _ZFP_zfReleaseAction(ZF_IN ZFObject *obj)
{
    if(obj)
    {
        obj->_ZFP_ZFObjectCheckRelease();
    }
}
template<typename T_ZFObject>
inline void _ZFP_zfRelease(ZF_IN T_ZFObject obj)
{
    _ZFP_zfReleaseAction(ZFCastZFObjectUnchecked(ZFObject *, obj));
}
/**
 * @brief release an object, see #ZFObject
 */
#define zfRelease(obj) \
    (zfCoreMutexLockerHolder(), zflockfree_zfRelease(obj))
/** @brief no lock version of #zfRelease, use with caution */
#define zflockfree_zfRelease(obj) \
    _ZFP_zfRelease(obj)

// ============================================================
// zflockfree_zfAllocWithCache
/**
 * @brief alloc object with cache
 *
 * use the allocated object normally,
 * it would be cached automatically when no other reference to it\n
 * \n
 * note:
 * -  you are unable to change the max cache size,
 *   this is a very low level cache impl for performance,
 *   supply your own custom cache logic if necessary
 * -  you are unable to check all cached objects,
 *   but you may clear all cache by #zfAllocCacheRemoveAll
 * -  the cached object should be stateless,
 *   the #ZFObject::objectTagRemoveAll and #ZFObject::observerRemoveAll
 *   would be called to clear the cached object's state
 * -  the cached object type must supply #ZFALLOC_CACHE_RELEASE
 *   to reset the cache state
 *   @code
 *     zfclass YourObject : zfextends ZFObject
 *     {
 *         ZFALLOC_CACHE_RELEASE({
 *             // call super if necessary
 *             zfsuper::zfAllocCacheRelease(obj);
 *             cache->xxx.clear();
 *         })
 *     };
 *     ZFObject *obj = zfAllocWithCache(YourObject);
 *   @endcode
 *   -  if you are unable to supply #ZFALLOC_CACHE_RELEASE for existing class,
 *     you may also supply custom cache release callback by:
 *     @code
 *       class YourCacheRelease
 *       {
 *       public:
 *           static void zfAllocCacheRelease(ZF_IN ZFObject *obj)
 *           {
 *           }
 *       };
 *       ZFObject *obj = zfAllocWithCache(YourObject, YourCacheRelease);
 *     @endcode
 *   -  especially take care of these things when supply cache release action:
 *     -  properties, they won't be cleared since the object would be cached,
 *       especially for retain properties and callbacks
 *     -  #ZFObject::objectOnDealloc won't be called when owner cached,
 *       if your object needs to perform additional cleanup steps,
 *       you may need to perform these cleanup steps manually
 */
#define zfAllocWithCache(T_ZFObject, ...) \
    (zfCoreMutexLockerHolder(), zflockfree_zfAllocWithCache(T_ZFObject, ##__VA_ARGS__))
/** @brief no lock version of #zfAllocWithCache, use with caution */
#define zflockfree_zfAllocWithCache(T_ZFObject, ...) \
    _ZFP_Obj_AllocCache<T_ZFObject, ##__VA_ARGS__>::Alloc()

/** @brief see #zfAllocWithCache */
#define ZFALLOC_CACHE_RELEASE(action) \
    public: \
        /** @cond ZFPrivateDoc */ \
        static ZFObject *_ZFP_zfAllocWithCache(void) \
        { \
            return zfAllocWithCache(zfself); \
        } \
        static void zfAllocCacheRelease(ZF_IN ZFObject *_obj) \
        { \
            zfself *cache = ZFCastZFObjectUnchecked(zfself *, _obj); \
            ZFUNUSED(cache); \
            action \
        } \
        /** @endcond */

/** @brief #ZFALLOC_CACHE_RELEASE for abstract class */
#define ZFALLOC_CACHE_RELEASE_ABSTRACT(action) \
    public: \
        /** @cond ZFPrivateDoc */ \
        static void zfAllocCacheRelease(ZF_IN ZFObject *_obj) \
        { \
            zfself *cache = ZFCastZFObjectUnchecked(zfself *, _obj); \
            ZFUNUSED(cache); \
            action \
        } \
        /** @endcond */

/** @brief dummy class for #zfAllocWithCache */
zfclassNotPOD ZF_ENV_EXPORT zfAllocCacheNoReleaseAction
{
    /** @cond ZFPrivateDoc */
public:
    static void zfAllocCacheRelease(ZF_IN ZFObject *obj)
    {
    }
};

/**
 * @brief remove all cache created by #zfAllocWithCache
 */
extern ZF_ENV_EXPORT void zfAllocCacheRemoveAll(void);

extern ZF_ENV_EXPORT void _ZFP_zfAllocWithCache_register(ZF_IN_OUT zfbool &enableFlag,
                                                         ZF_IN_OUT ZFObject **cache,
                                                         ZF_IN_OUT zfindex &cacheCount);
extern ZF_ENV_EXPORT void _ZFP_zfAllocWithCache_unregister(ZF_IN_OUT zfbool &enableFlag);
template<typename T_ZFObject, typename T_Cleanup = T_ZFObject, int T_MaxCache = 16>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_Obj_AllocCache
{
public:
    static T_ZFObject *Alloc(void)
    {
        static RegHolder _dummy;
        ZFUNUSED(_dummy);
        if(enableFlag())
        {
            if(cacheCount() > 0)
            {
                ZFObject *ret = zflockfree_zfRetain(cache()[--(cacheCount())]);
                ret->_ZFP_ZFObject_zfAllocCacheRelease = zfAllocCacheRelease;
                return ZFCastZFObjectUnchecked(T_ZFObject *, ret);
            }
            else
            {
                T_ZFObject *ret = zflockfree_zfRetain(zflockfree_zfAlloc(T_ZFObject));
                ret->_ZFP_ZFObject_zfAllocCacheRelease = zfAllocCacheRelease;
                return ret;
            }
        }
        else
        {
            return zflockfree_zfRetain(zflockfree_zfAlloc(T_ZFObject));
        }
    }
    static void zfAllocCacheRelease(ZF_IN ZFObject *obj)
    {
        obj->_ZFP_ZFObject_zfAllocCacheRelease = zfnull;
        if(enableFlag() && cacheCount() < T_MaxCache)
        {
            T_Cleanup::zfAllocCacheRelease(obj);
            cache()[cacheCount()++] = obj;
        }
        else
        {
            zflockfree_zfRelease(obj);
        }
    }
    static zfbool &enableFlag(void)
    {
        static zfbool enableFlag = zffalse;
        return enableFlag;
    }
    static ZFObject **cache(void)
    {
        static ZFObject *cache[T_MaxCache];
        return cache;
    }
    static zfindex &cacheCount(void)
    {
        static zfindex cacheCount = 0;
        return cacheCount;
    }
private:
    zfclassNotPOD RegHolder
    {
    public:
        RegHolder(void)
        {
            _ZFP_zfAllocWithCache_register(enableFlag(), cache(), cacheCount());
        }
        ~RegHolder(void)
        {
            _ZFP_zfAllocWithCache_unregister(enableFlag());
        }
    };
};

// ============================================================
/**
 * @brief util to release property's old value, retain new value, then set to property
 *
 * this macro is similar to the retain property in Object-C\n
 * typical usage:
 * @code
 *   ZFObject *property = ...;
 *   ZFObject *newProperty = ...;
 *
 *   // OK, release property, retain newProperty, then set to property
 *   zfRetainChange(property, newProperty);
 *
 *   // OK, use return value of a function as new value
 *   // but keep it in mind, that the new value will be retained
 *   zfRetainChange(property, funcThatReturnZFObject());
 *
 *   // OK, same as release old property and set it to zfnull
 *   zfRetainChange(property, zfnull);
 *
 *   // error, new value must be ZFObject
 *   // zfRetainChange(property, 123);
 *
 *   // error, property must be a variable contains a (ZFObject *)
 *   // zfRetainChange(zfnull, newProperty);
 *   // zfRetainChange(funcThatReturnZFObject(), newProperty);
 * @endcode
 * @see zfRetain, zfRelease, ZFPROPERTY_RETAIN
 */
#define zfRetainChange(property, propertyValue) \
    do \
    { \
        zfCoreMutexLock(); \
        ZFAny _ZFP_zfRetainChangeTmpValue = property; \
        zflockfree_zfRetain(property = propertyValue); \
        zflockfree_zfRelease(_ZFP_zfRetainChangeTmpValue.toObject()); \
        zfCoreMutexUnlock(); \
    } while(zffalse)
/** @brief no lock version of #zfRetainChange, use with caution */
#define zflockfree_zfRetainChange(property, propertyValue) \
    do \
    { \
        ZFAny _ZFP_zfRetainChangeTmpValue = property; \
        zflockfree_zfRetain(property = propertyValue); \
        zflockfree_zfRelease(_ZFP_zfRetainChangeTmpValue.toObject()); \
    } while(zffalse)
/** @see zfRetainChange */
#define zfRetainChange(property, propertyValue) \
    do \
    { \
        zfCoreMutexLock(); \
        ZFAny _ZFP_zfRetainChangeTmpValue = property; \
        zflockfree_zfRetain(property = propertyValue); \
        zflockfree_zfRelease(_ZFP_zfRetainChangeTmpValue.toObject()); \
        zfCoreMutexUnlock(); \
    } while(zffalse)
/** @brief no lock version of #zfRetainChange, use with caution */
#define zflockfree_zfRetainChange(property, propertyValue) \
    do \
    { \
        ZFAny _ZFP_zfRetainChangeTmpValue = property; \
        zflockfree_zfRetain(property = propertyValue); \
        zflockfree_zfRelease(_ZFP_zfRetainChangeTmpValue.toObject()); \
    } while(zffalse)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFObjectRetain_h_

