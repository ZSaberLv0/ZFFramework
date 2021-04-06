/**
 * @file ZFCoreTypeDef_ClassType.h
 * @brief types for ZFFramework
 */

#ifndef _ZFI_ZFCoreTypeDef_ClassType_h_
#define _ZFI_ZFCoreTypeDef_ClassType_h_

#include "ZFCoreTypeDef_CoreType.h"
#include <stdlib.h>
#include <memory.h>
#include <string.h>

// for internal debug use only
#define _ZFP_ZFMEM_LOG 0
#define _ZFP_ZFMEM_LOG_VERBOSE 0

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief forward declaration of a class type
 */
#define zfclassFwd class
/**
 * @brief shows the class is a POD type
 */
#define zfclassPOD class
/**
 * @brief shows the class is not a POD type,
 *   but you may use it like a POD
 *   except memset it to 0
 */
#define zfclassLikePOD class
/**
 * @brief shows the class is not a POD type,
 *   you should not memset it
 *   or declare it in stack
 *   or copy value by copy constructor or operator =
 */
#define zfclassNotPOD class

/**
 * @brief extends for #zfclassLikePOD
 */
#define zfextendsLikePOD public
/**
 * @brief extends for #zfclassPOD
 */
#define zfextendsPOD public
/**
 * @brief extends for #zfclassNotPOD
 */
#define zfextendsNotPOD public

/**
 * @brief dummy macro shows that method override parent's method
 */
#define zfoverride

/**
 * @brief dummy macro shows that a method or class is designed must not to be overrided
 */
#define zffinal

/**
 * @brief dummy macro shows that a method is pure virtual method
 */
#define zfpurevirtual = 0

// ============================================================
/**
 * @brief same as static_cast<Type>(obj)
 */
#define ZFCastStatic(Type, obj) (static_cast<Type>(obj))
/**
 * @brief same as reinterpret_cast<Type>(obj)
 */
#define ZFCastReinterpret(Type, obj) (reinterpret_cast<Type>(obj))

// ============================================================
/**
 * @brief a macro to show that a class doesn't allow copy constructor
 *
 * usage:
 * @code
 *   class YourClass
 *   {
 *       ZFCLASS_DISALLOW_COPY_CONSTRUCTOR(YourClass)
 *   };
 * @endcode
 */
#define ZFCLASS_DISALLOW_COPY_CONSTRUCTOR(ClassName) \
    private: \
        ClassName(const ClassName &); \
        ClassName &operator = (const ClassName &); \
    public:

// ============================================================
/** @brief same as new defined for future use */
#define zfnew(Type, ...) (new Type(__VA_ARGS__))
/** @brief same as delete defined for future use */
#define zfdelete(instance) delete(instance)

/** @brief placement new defined for future use */
#define zfnewPlacement(buf, Type, ...) (new (buf) Type(__VA_ARGS__))
template<typename Type> inline void _ZFP_zfdeletePlacement(Type *obj) {obj->~Type();}
/** @brief placement delete (instance->~Type()) defined for future use */
#define zfdeletePlacement(instance) _ZFP_zfdeletePlacement(instance)

/** @brief same as malloc defined for future use */
#define zfmalloc(size) malloc((size_t)(size))
extern ZF_ENV_EXPORT void *_ZFP_zfmallocZero(zfindex size);
/** @brief util method to #zfmalloc and #zfmemset to zero */
#define zfmallocZero(size) _ZFP_zfmallocZero(size)

/** @brief same as realloc defined for future use */
#define zfrealloc(oldPtr, newSize) realloc((oldPtr), (size_t)(newSize))
extern ZF_ENV_EXPORT void *_ZFP_zfreallocZero(void *oldPtr, zfindex newSize, zfindex oldSize);
/** @brief util method to #zfrealloc and #zfmemset the increased memory to zero */
#define zfreallocZero(oldPtr, newSize, oldSize) _ZFP_zfreallocZero((oldPtr), (newSize), (oldSize))

/** @brief same as free defined for future use, do nothing if ptr is NULL */
#define zffree(ptr) free((ptr))

// ============================================================
/** @brief wrapper to memset */
inline void *zfmemset(void *p, zfint ch, zfindex size) {return memset(p, ch, (size_t)size);}
/** @brief wrapper to memcpy */
inline void *zfmemcpy(void *dst, const void *src, zfindex size) {return memcpy(dst, src, (size_t)size);}
/** @brief wrapper to memmove */
inline void *zfmemmove(void *dst, const void *src, zfindex size) {return memmove(dst, src, (size_t)size);}
/** @brief wrapper to memcmp */
inline zfint zfmemcmp(const void *p1, const void *p2, zfindex size) {return (zfint)memcmp(p1, p2, (size_t)size);}

/**
 * @brief memmove for common object type, object must support operator =
 *
 * it's all right if dst and src share same memory
 */
template<typename T_Element>
T_Element *zfmemmoveObject(T_Element *dst, const T_Element *src, zfindex count)
{
    if(dst && src && count != 0 && count != zfindexMax())
    {
        if(dst > src && src + count > dst)
        {
            const T_Element *srcEnd = src;
            src += count;
            dst += count;
            while(src > srcEnd)
            {
                --dst;
                --src;
                *dst = *src;
            }
        }
        else
        {
            const T_Element *srcEnd = src + count;
            while(src < srcEnd)
            {
                *dst = *src;
                ++dst;
                ++src;
            }
        }
    }
    return dst;
}

// ============================================================
/**
 * @brief dummy macro that shows the param used as required input
 */
#define ZF_IN
/**
 * @brief dummy macro that shows the param used as optional input
 */
#define ZF_IN_OPT
/**
 * @brief dummy macro that shows the param used as required output
 */
#define ZF_OUT
/**
 * @brief dummy macro that shows the param used as optional output
 */
#define ZF_OUT_OPT
/**
 * @brief dummy macro that shows the param used as required input and output
 */
#define ZF_IN_OUT
/**
 * @brief dummy macro that shows the param used as optional input and output
 */
#define ZF_IN_OUT_OPT

// ============================================================
#if _ZFP_ZFMEM_LOG
    extern void _ZFP_ZFMEM_logNew(void *p, const char *action, const char *file, const char *func, int line);
    extern void _ZFP_ZFMEM_logDelete(void *p, const char *action, const char *file, const char *func, int line);
    extern void _ZFP_ZFMEM_printStatus(int threshold = 10);
    template<typename T>
    T _ZFP_ZFMEM_new_action(T p, const char *action, const char *file, const char *func, int line)
    {
        _ZFP_ZFMEM_logNew((void *)p, action, file, func, line);
        return p;
    }
    template<typename T>
    T _ZFP_ZFMEM_delete_action(T p, const char *action, const char *file, const char *func, int line)
    {
        _ZFP_ZFMEM_logDelete((void *)p, action, file, func, line);
        return p;
    }
    #define _ZFP_ZFMEM_new(p, action) _ZFP_ZFMEM_new_action((p), action, __FILE__, __FUNCTION__, __LINE__)
    #define _ZFP_ZFMEM_delete(p, action) _ZFP_ZFMEM_delete_action((p), action, __FILE__, __FUNCTION__, __LINE__)

    #undef zfnew
    #define zfnew(Type, ...) _ZFP_ZFMEM_new((new Type(__VA_ARGS__)), "zfnew            ")
    #undef zfdelete
    #define zfdelete(instance) delete(_ZFP_ZFMEM_delete((instance), "zfdelete         "))

    #undef zfnewPlacement
    #define zfnewPlacement(buf, Type, ...) _ZFP_ZFMEM_new((new (buf) Type(__VA_ARGS__)), "zfnewPlacement   ")
    #undef zfdeletePlacement
    #define zfdeletePlacement(instance) _ZFP_zfdeletePlacement(_ZFP_ZFMEM_delete((instance), "zfdeletePlacement"))

    #undef zfmalloc
    #define zfmalloc(size) _ZFP_ZFMEM_new((malloc((size_t)(size))), "zfmalloc         ")
    #undef zfmallocZero
    #define zfmallocZero(size) _ZFP_ZFMEM_new(_ZFP_zfmallocZero((size_t)(size)), "zfmallocZero     ")

    inline void *_ZFP_ZFMEM_zfrealloc(void *oldPtr, zfindex newSize, const char *file, const char *func, int line)
    {
        _ZFP_ZFMEM_logDelete(oldPtr, "zfreallocD       ", file, func, line);
        void *ret = realloc(oldPtr, (size_t)newSize);
        _ZFP_ZFMEM_logNew(ret, "zfrealloc        ", file, func, line);
        return ret;
    }
    #undef zfrealloc
    #define zfrealloc(oldPtr, newSize) _ZFP_ZFMEM_zfrealloc((oldPtr), (newSize), __FILE__, __FUNCTION__, __LINE__)
    inline void *_ZFP_ZFMEM_zfreallocZero(void *oldPtr, zfindex newSize, zfindex oldSize, const char *file, const char *func, int line)
    {
        _ZFP_ZFMEM_logDelete(oldPtr, "zfreallocZeroD   ", file, func, line);
        void *ret = _ZFP_zfreallocZero(oldPtr, newSize, oldSize);
        _ZFP_ZFMEM_logNew(ret, "zfreallocZero    ", file, func, line);
        return ret;
    }
    #undef zfreallocZero
    #define zfreallocZero(oldPtr, newSize, oldSize) _ZFP_ZFMEM_zfreallocZero((oldPtr), (newSize), (oldSize), __FILE__, __FUNCTION__, __LINE__)

    #undef zffree
    #define zffree(ptr) free(_ZFP_ZFMEM_delete((ptr), "zffree           "))
#endif

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreTypeDef_ClassType_h_

