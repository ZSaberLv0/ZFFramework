/**
 * @file ZFMemPool.h
 * @brief memory pool
 */
#ifndef _ZFI_ZFMemPool_h_
#define _ZFI_ZFMemPool_h_

#include "ZFCoreTypeDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief whether enable memory pool, true by default
 */
#ifndef ZF_ENV_ZFMEMPOOL_ENABLE
    #if _ZFP_ZFMEM_LOG
        #define ZF_ENV_ZFMEMPOOL_ENABLE 0
    #else
        #define ZF_ENV_ZFMEMPOOL_ENABLE 1
    #endif
#endif

// ============================================================
/**
 * @def zfpoolNew
 * @brief internal use only, for allocating internal types for performance
 *
 * @warning no thread-safe
 * @warning pointers passed to zfpoolDelete must be the same with the type you create
 * @def zfpoolDelete
 * @brief see #zfpoolNew
 * @def zfpoolDeclareFriend
 * @brief use to declare friend if your type has non-public constructors
 */
#if ZF_ENV_ZFMEMPOOL_ENABLE
    #define zfpoolNew(T_Type, ...) zfnewPlacement((_ZFP_zfpoolObjectHolder<T_Type>::poolMalloc()), T_Type, ##__VA_ARGS__)
    #define zfpoolDelete(obj) _ZFP_zfpoolDelete(obj)
    #define zfpoolDeclareFriend() \
        friend zfclassFwd _ZFP_zfpoolObjectHolder<zfself>;
#else
    #define zfpoolNew(T_Type, ...) zfnew(T_Type, ##__VA_ARGS__)
    #define zfpoolDelete(obj) zfdelete(obj)
    #define zfpoolDeclareFriend()
#endif

// ============================================================
// impl
#define _ZFP_zfpoolSizeAlignMin (sizeof(void *) * 2)
#define _ZFP_zfpoolSizeAlign(size) \
    ( \
        (((size) % _ZFP_zfpoolSizeAlignMin) == 0) \
            ? (size) \
            : ((((size) / _ZFP_zfpoolSizeAlignMin) + 1) * _ZFP_zfpoolSizeAlignMin) \
    )
template<int N>
union ZF_ENV_EXPORT _ZFP_zfpoolObjectBlock
{
public:
    zfbyte buf[N];
    _ZFP_zfpoolObjectBlock<N> *next;
};
template<int N>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_zfpoolObject
{
public:
    void *poolMalloc(void)
    {
        if(_available)
        {
            _ZFP_zfpoolObjectBlock<N> *t = _available;
            _available = _available->next;
            return t;
        }
        else
        {
            return zfmalloc(sizeof(_ZFP_zfpoolObjectBlock<N>));
        }
    }
    void poolFree(ZF_IN void *obj)
    {
        _ZFP_zfpoolObjectBlock<N> *t = _available;
        _available = (_ZFP_zfpoolObjectBlock<N> *)obj;
        _available->next = t;
    }
public:
    _ZFP_zfpoolObject(void)
    : _available(zfnull)
    {
    }
    ~_ZFP_zfpoolObject(void)
    {
        while(_available)
        {
            _ZFP_zfpoolObjectBlock<N> *t = _available;
            _available = _available->next;
            zffree(t);
        }
    }
    static _ZFP_zfpoolObject<N> &instance(void)
    {
        static _ZFP_zfpoolObject<N> d;
        return d;
    }
private:
    _ZFP_zfpoolObjectBlock<N> *_available;
};

template<typename T_Type>
zfclassNotPOD _ZFP_zfpoolObjectHolder
{
public:
    static void *poolMalloc(void)
    {
        return _ZFP_zfpoolObject<_ZFP_zfpoolSizeAlign(sizeof(T_Type))>::instance().poolMalloc();
    }
    static void poolDelete(ZF_IN T_Type *obj)
    {
        obj->~T_Type();
        _ZFP_zfpoolObject<_ZFP_zfpoolSizeAlign(sizeof(T_Type))>::instance().poolFree(obj);
    }
};
template<typename T_Type>
inline void _ZFP_zfpoolDelete(ZF_IN T_Type *obj)
{
    if(obj)
    {
        _ZFP_zfpoolObjectHolder<T_Type>::poolDelete(obj);
    }
}

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFMemPool_h_

