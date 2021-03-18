/**
 * @file ZFCoreUtilTemplate.h
 * @brief template utils
 */

#ifndef _ZFI_ZFCoreUtilTemplate_h_
#define _ZFI_ZFCoreUtilTemplate_h_

#include "ZFCoreTypeDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// zftValue
/**
 * @brief util wrapper class to hold a type
 */
template<typename T_Type>
zfclassLikePOD ZF_ENV_EXPORT zftValue
{
public:
    /** @brief the wrapped value */
    T_Type zfv;
public:
    /** @cond ZFPrivateDoc */
    zftValue(void) : zfv() {}
    zftValue(ZF_IN T_Type const &ref) : zfv(ref) {}
    zftValue(ZF_IN zftValue<T_Type> const &ref) : zfv(ref.zfv) {}
    template<typename T_Ref> zftValue(ZF_IN T_Ref const &ref) : zfv(ref) {}
    inline zftValue<T_Type> &operator = (ZF_IN T_Type const &ref) {zfv = ref; return *this;}
    inline zftValue<T_Type> &operator = (ZF_IN zftValue<T_Type> const &ref) {zfv = ref.zfv; return *this;}
    template<typename T_Ref> inline zftValue<T_Type> &operator = (ZF_IN T_Ref const &ref) {zfv = ref; return *this;}
    inline operator T_Type const & (void) const {return zfv;}
    inline operator T_Type & (void) {return zfv;}
    /** @endcond */
};

// ============================================================
// zftTraits
/** @brief see #zftTraits::ModifierName */
typedef enum {
    zftTraitsModifier_N, /**< @brief see #zftTraits::ModifierName */
    zftTraitsModifier_R, /**< @brief see #zftTraits::ModifierName */
    zftTraitsModifier_CR, /**< @brief see #zftTraits::ModifierName */
    zftTraitsModifier_P, /**< @brief see #zftTraits::ModifierName */
    zftTraitsModifier_PR, /**< @brief see #zftTraits::ModifierName */
    zftTraitsModifier_PCR, /**< @brief see #zftTraits::ModifierName */
    zftTraitsModifier_CP, /**< @brief see #zftTraits::ModifierName */
    zftTraitsModifier_CPR, /**< @brief see #zftTraits::ModifierName */
    zftTraitsModifier_CPCR, /**< @brief see #zftTraits::ModifierName */
} zftTraitsModifier;
/** @brief see #zftTraits::ModifierName */
#define zftTraitsModifierName_N "N"
/** @brief see #zftTraits::ModifierName */
#define zftTraitsModifierName_R "R"
/** @brief see #zftTraits::ModifierName */
#define zftTraitsModifierName_CR "CR"
/** @brief see #zftTraits::ModifierName */
#define zftTraitsModifierName_P "P"
/** @brief see #zftTraits::ModifierName */
#define zftTraitsModifierName_PR "PR"
/** @brief see #zftTraits::ModifierName */
#define zftTraitsModifierName_PCR "PCR"
/** @brief see #zftTraits::ModifierName */
#define zftTraitsModifierName_CP "CP"
/** @brief see #zftTraits::ModifierName */
#define zftTraitsModifierName_CPR "CPR"
/** @brief see #zftTraits::ModifierName */
#define zftTraitsModifierName_CPCR "CPCR"
/**
 * @brief type traits similar to STL's traits
 *
 * @note this is a walkaround to reduce dependency of STL,
 *   no rvalue reference support
 */
template<typename T_Type>
zfclassNotPOD ZF_ENV_EXPORT zftTraits
{
public:
    enum {
        TrIsPtr = 0, /** @brief true if the type is pointer type */
        TrIsRef = 0, /** @brief true if the type is reference type */
        TrModifier = (int)zftTraitsModifier_N, /** @brief the modifier type */
    };
    typedef T_Type                  TrType;         /**< @brief normal type */
    typedef T_Type &                TrRef;          /**< @brief reference type */
    typedef T_Type const &          TrConstRef;     /**< @brief const reference type */
    typedef T_Type *                TrPtr;          /**< @brief pointer type */
    typedef const T_Type *          TrConstPtr;     /**< @brief const pointer type */
    typedef T_Type                  TrNoRef;        /**< @brief remove reference */
    /**
     * @brief type modifier name
     *
     *   -  'N' : Type
     *   -  'R' : Type &
     *   -  'CR' : Type const &
     *   -  'P' : Type *
     *   -  'PR' : Type *&
     *   -  'PCR' : Type * const &
     *   -  'CP' : const Type *
     *   -  'CPR' : const Type *&
     *   -  'CPCR' : const Type * const &
     */
    static const zfchar *ModifierName(void) {return zftTraitsModifierName_N;}
};
/** @cond ZFPrivateDoc */
/*
 * here is a list
 *   Type
 *   Type &
 *   Type const &
 *   Type *
 *   Type *&
 *   Type * const &
 *   const Type *
 *   const Type *&
 *   const Type * const &
 *
 *   void
 *   void *
 *   const void *
 *   void *&
 *   void * const &
 *   const void *&
 *   const void * const &
 */
template<typename T_Type>
zfclassNotPOD ZF_ENV_EXPORT zftTraits<T_Type &>
{
public:
    enum {
        TrIsPtr = 0,
        TrIsRef = 1,
        TrModifier = (int)zftTraitsModifier_R,
    };
    typedef T_Type                  TrType;
    typedef T_Type &                TrRef;
    typedef T_Type const &          TrConstRef;
    typedef T_Type *                TrPtr;
    typedef const T_Type *          TrConstPtr;
    typedef T_Type                  TrNoRef;
    static const zfchar *ModifierName(void) {return zftTraitsModifierName_R;}
};
template<typename T_Type>
zfclassNotPOD ZF_ENV_EXPORT zftTraits<T_Type const &>
{
public:
    enum {
        TrIsPtr = 0,
        TrIsRef = 1,
        TrModifier = (int)zftTraitsModifier_CR,
    };
    typedef T_Type                  TrType;
    typedef T_Type &                TrRef;
    typedef T_Type const &          TrConstRef;
    typedef T_Type *                TrPtr;
    typedef const T_Type *          TrConstPtr;
    typedef T_Type                  TrNoRef;
    static const zfchar *ModifierName(void) {return zftTraitsModifierName_CR;}
};
template<typename T_Type>
zfclassNotPOD ZF_ENV_EXPORT zftTraits<T_Type *>
{
public:
    enum {
        TrIsPtr = 1,
        TrIsRef = 0,
        TrModifier = (int)zftTraitsModifier_P,
    };
    typedef T_Type                  TrType;
    typedef T_Type &                TrRef;
    typedef T_Type const &          TrConstRef;
    typedef T_Type *                TrPtr;
    typedef const T_Type *          TrConstPtr;
    typedef T_Type *                TrNoRef;
    static const zfchar *ModifierName(void) {return zftTraitsModifierName_P;}
};
template<typename T_Type>
zfclassNotPOD ZF_ENV_EXPORT zftTraits<T_Type *&>
{
public:
    enum {
        TrIsPtr = 1,
        TrIsRef = 1,
        TrModifier = (int)zftTraitsModifier_PR,
    };
    typedef T_Type                  TrType;
    typedef T_Type &                TrRef;
    typedef T_Type const &          TrConstRef;
    typedef T_Type *                TrPtr;
    typedef const T_Type *          TrConstPtr;
    typedef T_Type *                TrNoRef;
    static const zfchar *ModifierName(void) {return zftTraitsModifierName_PR;}
};
template<typename T_Type>
zfclassNotPOD ZF_ENV_EXPORT zftTraits<T_Type * const &>
{
public:
    enum {
        TrIsPtr = 1,
        TrIsRef = 1,
        TrModifier = (int)zftTraitsModifier_PCR,
    };
    typedef T_Type                  TrType;
    typedef T_Type &                TrRef;
    typedef T_Type const &          TrConstRef;
    typedef T_Type *                TrPtr;
    typedef const T_Type *          TrConstPtr;
    typedef T_Type *                TrNoRef;
    static const zfchar *ModifierName(void) {return zftTraitsModifierName_PCR;}
};
template<typename T_Type>
zfclassNotPOD ZF_ENV_EXPORT zftTraits<const T_Type *>
{
public:
    enum {
        TrIsPtr = 1,
        TrIsRef = 0,
        TrModifier = (int)zftTraitsModifier_CP,
    };
    typedef T_Type                  TrType;
    typedef T_Type &                TrRef;
    typedef T_Type const &          TrConstRef;
    typedef T_Type *                TrPtr;
    typedef const T_Type *          TrConstPtr;
    typedef const T_Type *          TrNoRef;
    static const zfchar *ModifierName(void) {return zftTraitsModifierName_CP;}
};
template<typename T_Type>
zfclassNotPOD ZF_ENV_EXPORT zftTraits<const T_Type *&>
{
public:
    enum {
        TrIsPtr = 1,
        TrIsRef = 1,
        TrModifier = (int)zftTraitsModifier_CPR,
    };
    typedef T_Type                  TrType;
    typedef T_Type &                TrRef;
    typedef T_Type const &          TrConstRef;
    typedef T_Type *                TrPtr;
    typedef const T_Type *          TrConstPtr;
    typedef const T_Type *          TrNoRef;
    static const zfchar *ModifierName(void) {return zftTraitsModifierName_CPR;}
};
template<typename T_Type>
zfclassNotPOD ZF_ENV_EXPORT zftTraits<const T_Type * const &>
{
public:
    enum {
        TrIsPtr = 1,
        TrIsRef = 1,
        TrModifier = (int)zftTraitsModifier_CPCR,
    };
    typedef T_Type                  TrType;
    typedef T_Type &                TrRef;
    typedef T_Type const &          TrConstRef;
    typedef T_Type *                TrPtr;
    typedef const T_Type *          TrConstPtr;
    typedef const T_Type *          TrNoRef;
    static const zfchar *ModifierName(void) {return zftTraitsModifierName_CPCR;}
};

template<>
zfclassNotPOD ZF_ENV_EXPORT zftTraits<void>
{
public:
    enum {
        TrIsPtr = 0,
        TrIsRef = 0,
        TrModifier = (int)zftTraitsModifier_N,
    };
    typedef void                    TrType;
    typedef void                    TrRef;
    typedef void                    TrConstRef;
    typedef void *                  TrPtr;
    typedef const void *            TrConstPtr;
    typedef void                    TrNoRef;
    static const zfchar *ModifierName(void) {return zftTraitsModifierName_N;}
};
template<>
zfclassNotPOD ZF_ENV_EXPORT zftTraits<void *>
{
public:
    enum {
        TrIsPtr = 1,
        TrIsRef = 0,
        TrModifier = (int)zftTraitsModifier_P,
    };
    typedef void                    TrType;
    typedef void                    TrRef;
    typedef void * const &          TrConstRef;
    typedef void *                  TrPtr;
    typedef const void *            TrConstPtr;
    typedef void *                  TrNoRef;
    static const zfchar *ModifierName(void) {return zftTraitsModifierName_P;}
};
template<>
zfclassNotPOD ZF_ENV_EXPORT zftTraits<const void *>
{
public:
    enum {
        TrIsPtr = 1,
        TrIsRef = 0,
        TrModifier = (int)zftTraitsModifier_CP,
    };
    typedef void                    TrType;
    typedef void                    TrRef;
    typedef const void * const &    TrConstRef;
    typedef void *                  TrPtr;
    typedef const void *            TrConstPtr;
    typedef const void *            TrNoRef;
    static const zfchar *ModifierName(void) {return zftTraitsModifierName_CP;}
};
template<>
zfclassNotPOD ZF_ENV_EXPORT zftTraits<void *&>
{
public:
    enum {
        TrIsPtr = 1,
        TrIsRef = 1,
        TrModifier = (int)zftTraitsModifier_PR,
    };
    typedef void                    TrType;
    typedef void                    TrRef;
    typedef void * const &          TrConstRef;
    typedef void *                  TrPtr;
    typedef const void *            TrConstPtr;
    typedef void *                  TrNoRef;
    static const zfchar *ModifierName(void) {return zftTraitsModifierName_PR;}
};
template<>
zfclassNotPOD ZF_ENV_EXPORT zftTraits<void * const &>
{
public:
    enum {
        TrIsPtr = 1,
        TrIsRef = 1,
        TrModifier = (int)zftTraitsModifier_PCR,
    };
    typedef void                    TrType;
    typedef void                    TrRef;
    typedef void * const &          TrConstRef;
    typedef void *                  TrPtr;
    typedef const void *            TrConstPtr;
    typedef void *                  TrNoRef;
    static const zfchar *ModifierName(void) {return zftTraitsModifierName_PCR;}
};
template<>
zfclassNotPOD ZF_ENV_EXPORT zftTraits<const void *&>
{
public:
    enum {
        TrIsPtr = 1,
        TrIsRef = 1,
        TrModifier = (int)zftTraitsModifier_CPR,
    };
    typedef void                    TrType;
    typedef void                    TrRef;
    typedef void * const &          TrConstRef;
    typedef void *                  TrPtr;
    typedef const void *            TrConstPtr;
    typedef const void *            TrNoRef;
    static const zfchar *ModifierName(void) {return zftTraitsModifierName_CPR;}
};
template<>
zfclassNotPOD ZF_ENV_EXPORT zftTraits<const void * const &>
{
public:
    enum {
        TrIsPtr = 1,
        TrIsRef = 1,
        TrModifier = (int)zftTraitsModifier_CPCR,
    };
    typedef void                    TrType;
    typedef void                    TrRef;
    typedef void * const &          TrConstRef;
    typedef void *                  TrPtr;
    typedef const void *            TrConstPtr;
    typedef const void *            TrNoRef;
    static const zfchar *ModifierName(void) {return zftTraitsModifierName_CPCR;}
};
/** @endcond */

// ============================================================
/** @brief std::enable_if wrapper */
template<bool cond = false>
zfclassNotPOD ZF_ENV_EXPORT zftEnableIf
{
};
/** @brief std::enable_if wrapper */
template<>
zfclassNotPOD ZF_ENV_EXPORT zftEnableIf<true>
{
public:
    /** @brief std::enable_if wrapper */
    typedef void EnableIf;
};

// ============================================================
/** @brief std::is_same wrapper */
template<typename T0, typename T1>
zfclassNotPOD ZF_ENV_EXPORT zftTypeIsSame
{
public:
    enum {
        /** @brief std::is_same wrapper */
        TypeIsSame = 0
    };
};
/** @brief std::is_same wrapper */
template<typename T>
zfclassNotPOD ZF_ENV_EXPORT zftTypeIsSame<T, T>
{
public:
    enum {
        /** @brief std::is_same wrapper */
        TypeIsSame = 1
    };
};

// ============================================================
/** @brief std::is_class wrapper */
template<typename T>
zfclassNotPOD ZF_ENV_EXPORT zftTypeIsClass
{
private:
    template<typename U>
    static char _test(int U::*);
    template<typename U>
    static int _test(...);
public:
    enum {
        /** @brief std::is_class wrapper */
        TypeIsClass = ((sizeof(_test<T>(0)) == sizeof(char)) ? 1 : 0)
    };
};

// ============================================================
/** @brief std::is_base_of wrapper */
template<typename TChild, typename TBase,
    bool _IsClass = (zftTypeIsClass<TChild>::TypeIsClass && zftTypeIsClass<TBase>::TypeIsClass)>
zfclassNotPOD ZF_ENV_EXPORT zftTypeIsTypeOf
{
private:
    template<typename T_Dummy>
    static char _test(TChild const &, T_Dummy);
    static int _test(TBase const &, int);
    zfclassNotPOD _Conv
    {
    public:
        operator TChild const & (void);
        operator TBase const & (void) const;
    };
public:
    enum {
        /** @brief std::is_base_of wrapper */
        TypeIsTypeOf = ((sizeof(_test(_Conv(), 0)) == sizeof(char)) ? 1 : 0)
    };
};
/** @brief std::is_base_of wrapper */
template<typename TChild, typename TBase>
zfclassNotPOD ZF_ENV_EXPORT zftTypeIsTypeOf<TChild, TBase, false>
{
public:
    enum {
        /** @brief std::is_base_of wrapper */
        TypeIsTypeOf = zftTypeIsSame<TChild, TBase>::TypeIsSame
    };
};
/** @brief std::is_base_of wrapper */
template<typename T>
zfclassNotPOD ZF_ENV_EXPORT zftTypeIsTypeOf<T, T, true>
{
public:
    enum {
        /** @brief std::is_base_of wrapper */
        TypeIsTypeOf = 1
    };
};

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreUtilTemplate_h_

