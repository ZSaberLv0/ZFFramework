/**
 * @file ZFCoreTypeDef_CoreIntType.h
 * @brief types for ZFFramework
 */

#ifndef _ZFI_ZFCoreTypeDef_CoreIntType_h_
#define _ZFI_ZFCoreTypeDef_CoreIntType_h_

#include "ZFCoreTypeDef_CorePrimitiveType.h"

/**
 * @brief whether we have stdint, true by default if under C++11
 */
#ifndef ZF_ENV_STDINT
    #if defined(__cplusplus) && (__cplusplus >= 201103L)
        #define ZF_ENV_STDINT 1
    #else
        #define ZF_ENV_STDINT 0
    #endif
#endif

#if ZF_ENV_STDINT
    #include <stdint.h>
#endif

ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @typedef zft_zfint8
 * @brief primitive number type, use only if necessary, see #zfindex
 * @typedef zft_zfuint8
 * @brief primitive number type, use only if necessary, see #zfindex
 *
 * @typedef zft_zfint16
 * @brief primitive number type, use only if necessary, see #zfindex
 * @typedef zft_zfuint16
 * @brief primitive number type, use only if necessary, see #zfindex
 *
 * @typedef zft_zfint32
 * @brief primitive number type, use only if necessary, see #zfindex
 * @typedef zft_zfuint32
 * @brief primitive number type, use only if necessary, see #zfindex
 *
 * @typedef zft_zfint64
 * @brief primitive number type, use only if necessary, see #zfindex
 * @typedef zft_zfuint64
 * @brief primitive number type, use only if necessary, see #zfindex
 * */
#if ZF_ENV_STDINT
    typedef int8_t zft_zfint8;
    typedef uint8_t zft_zfuint8;

    typedef int16_t zft_zfint16;
    typedef uint16_t zft_zfuint16;

    typedef int32_t zft_zfint32;
    typedef uint32_t zft_zfuint32;

    typedef int64_t zft_zfint64;
    typedef uint64_t zft_zfuint64;
#else
    typedef signed char zft_zfint8;
    typedef unsigned char zft_zfuint8;

    typedef signed short int zft_zfint16;
    typedef unsigned short int zft_zfuint16;

    typedef signed int zft_zfint32;
    typedef unsigned int zft_zfuint32;

    typedef signed long long int zft_zfint64;
    typedef unsigned long long int zft_zfuint64;
#endif // #if ZF_ENV_STDINT

// ============================================================
/**
 * @brief see #ZFT_INT_STRONG
 */
#define ZFT_INT_WEAK(T, D) \
    typedef T D; \
    /** @brief primitive number type, use only if necessary, for app, use @ref D instead */ \
    typedef T zft_##D;

// ============================================================
/**
 * @brief declare a strong wrapper type for integer types, similar to boost's BOOST_STRONG_TYPEDEF
 */
#define ZFT_INT_STRONG(T, D) _ZFP_ZFT_INT_STRONG(T, D)
#define _ZFP_ZFT_INT_STRONG(T, D) \
    /** @cond ZFPrivateDoc */ \
    class _zft_##D \
    { \
    protected: \
        typedef _zft_##D D; \
    public: \
        T t; \
        _zft_##D(void) : t() {} \
        _zft_##D(const D &t_) : t(t_.t) {} \
        _zft_##D(const T t_) : t(t_) {} \
        inline D &operator = (const D &t_) {t = t_.t; return *this;} \
        template<typename T2> inline D &operator = (const T2 &t_) {t = t_; return *this;} \
        inline operator const T & (void) const {return t;} \
        inline operator T & (void) {return t;} \
        inline bool operator ! (void) const {return (t == 0);} \
        inline bool operator == (const D &t_) const {return (t == t_.t);} \
        inline bool operator != (const D &t_) const {return (t != t_.t);} \
        inline bool operator <  (const D &t_) const {return (t <  t_.t);} \
        inline bool operator <= (const D &t_) const {return (t <= t_.t);} \
        inline bool operator >  (const D &t_) const {return (t >  t_.t);} \
        inline bool operator >= (const D &t_) const {return (t >= t_.t);} \
        template<typename T2> inline bool operator == (const T2 &t_) const {return (t == t_);} \
        template<typename T2> inline bool operator != (const T2 &t_) const {return (t != t_);} \
        template<typename T2> inline bool operator <  (const T2 &t_) const {return (t <  t_);} \
        template<typename T2> inline bool operator <= (const T2 &t_) const {return (t <= t_);} \
        template<typename T2> inline bool operator >  (const T2 &t_) const {return (t >  t_);} \
        template<typename T2> inline bool operator >= (const T2 &t_) const {return (t >= t_);} \
    }; \
    /** @endcond */ \
    typedef _zft_##D D; \
    /** @brief primitive number type, use only if necessary, for app, use @ref D instead */ \
    typedef T zft_##D;

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreTypeDef_CoreIntType_h_

