/**
 * @file ZFExport.h
 * @brief util to export global variable
 */

#ifndef _ZFI_ZFExport_h_
#define _ZFI_ZFExport_h_

#include "ZFMethodFuncDeclare.h"
#include "ZFMethodFuncUserRegister.h"
ZF_NAMESPACE_GLOBAL_BEGIN

#define _ZFP_ZFEXPORT_VAR_INIT_VALUE(Type, Name, initValue) \
    ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ExpV_##Name, ZFLevelZFFrameworkEssential) \
    { \
        _ZFP_v_##Name = (initValue); \
    } \
    ZF_GLOBAL_INITIALIZER_END(ExpV_##Name)

// ============================================================
/**
 * @brief util to export global variable
 *
 * the variable are exported as #ZFMETHOD_FUNC_DECLARE_0\n
 * usage:
 * @code
 *   // in header file
 *   ZFEXPORT_VAR_DECLARE(YourType, yourName)
 *
 *   // in source file
 *   ZFEXPORT_VAR_DEFINE(YourType, yourName, yourInitValue)
 *
 *   // access
 *   YourType &v = yourName();
 *   // modify
 *   yourName(v);
 * @endcode
 * \n
 * for readonly var, use #ZFEXPORT_VAR_READONLY_DECLARE\n
 * to alias existing var, use #ZFEXPORT_VAR_ALIAS_DECLARE\n
 * for existing raw var, use #ZFEXPORT_VAR_VALUEREF_DECLARE\n
 *
 * @note the init value would be applied during #ZFFrameworkInit as level #ZFLevelZFFrameworkEssential,
 *   if you want to ensure the init value applied,
 *   you can use ZFExportVarEnsureInit_YourVarName
 */
#define ZFEXPORT_VAR_DECLARE(Type, Name) \
    /** @cond ZFPrivateDoc */ \
    extern ZF_ENV_EXPORT Type _ZFP_v_##Name; \
    /** @endcond */ \
    ZFMETHOD_FUNC_INLINE_DECLARE_DETAIL_0(G, \
        Type &, Name \
        ) \
    { \
        return _ZFP_v_##Name; \
    } \
    /** @brief see @ref Name */ \
    ZFMETHOD_FUNC_INLINE_DECLARE_DETAIL_1(S, \
        void, Name \
        , ZFMP_IN(Type const &, v) \
        ) \
    { \
        _ZFP_v_##Name = v; \
    } \
    /** @brief see #ZFEXPORT_VAR_DECLARE */ \
    extern ZF_ENV_EXPORT void ZFExportVarEnsureInit_##Name(void);

/** @brief see #ZFEXPORT_VAR_DECLARE */
#define ZFEXPORT_VAR_DEFINE(Type, Name, initValue) \
    Type _ZFP_v_##Name; \
    ZFMETHOD_FUNC_INLINE_DEFINE_DETAIL_0(G, \
        Type &, Name \
        ) \
    ZFMETHOD_FUNC_INLINE_DEFINE_DETAIL_1(S, \
        void, Name \
        , ZFMP_IN(Type const &, v) \
        ) \
    _ZFP_ZFEXPORT_VAR_INIT_VALUE(Type, Name, initValue) \
    void ZFExportVarEnsureInit_##Name(void) \
    { \
        (void)ZF_GLOBAL_INITIALIZER_INSTANCE(ExpV_##Name); \
    }

/** @brief see #ZFEXPORT_VAR_DECLARE */
#define ZFEXPORT_VAR_ALIAS_DECLARE(Type, Name, AliasName) \
    ZFMETHOD_FUNC_INLINE_DECLARE_DETAIL_0(G, \
        Type &, Name \
        ) \
    { \
        return (AliasName)(); \
    } \
    /** @brief see @ref Name */ \
    ZFMETHOD_FUNC_INLINE_DECLARE_DETAIL_1(S, \
        void, Name \
        , ZFMP_IN(Type const &, v) \
        ) \
    { \
        AliasName(v); \
    } \
    /** @brief see #ZFEXPORT_VAR_DECLARE */ \
    extern ZF_ENV_EXPORT void ZFExportVarEnsureInit_##Name(void);

/** @brief see #ZFEXPORT_VAR_DECLARE */
#define ZFEXPORT_VAR_ALIAS_DEFINE(Type, Name, AliasName) \
    ZFMETHOD_FUNC_INLINE_DEFINE_DETAIL_0(G, \
        Type &, Name \
        ) \
    ZFMETHOD_FUNC_INLINE_DEFINE_DETAIL_1(S, \
        void, Name \
        , ZFMP_IN(Type const &, v) \
        ) \
    void ZFExportVarEnsureInit_##Name(void) \
    { \
        ZFExportVarEnsureInit_##AliasName(); \
    }

/** @brief see #ZFEXPORT_VAR_DECLARE */
#define ZFEXPORT_VAR_VALUEREF_DECLARE(Type, Name, ValueRef) \
    ZFMETHOD_FUNC_INLINE_DECLARE_DETAIL_0(G, \
        Type &, Name \
        ) \
    { \
        return ValueRef; \
    } \
    /** @brief see @ref Name */ \
    ZFMETHOD_FUNC_INLINE_DECLARE_DETAIL_1(S, \
        void, Name \
        , ZFMP_IN(Type const &, v) \
        ) \
    { \
        ValueRef = v; \
    } \
    /** @brief see #ZFEXPORT_VAR_DECLARE */ \
    inline void ZFExportVarEnsureInit_##Name(void) \
    { \
    }

/** @brief see #ZFEXPORT_VAR_DECLARE */
#define ZFEXPORT_VAR_VALUEREF_DEFINE(Type, Name, ValueRef) \
    ZFMETHOD_FUNC_INLINE_DEFINE_DETAIL_0(G, \
        Type &, Name \
        ) \
    ZFMETHOD_FUNC_INLINE_DEFINE_DETAIL_1(S, \
        void, Name \
        , ZFMP_IN(Type const &, v) \
        )

// ============================================================
/** @brief see #ZFEXPORT_VAR_DECLARE */
#define ZFEXPORT_VAR_READONLY_DECLARE(Type, Name) \
    /** @cond ZFPrivateDoc */ \
    extern ZF_ENV_EXPORT Type _ZFP_v_##Name; \
    /** @endcond */ \
    ZFMETHOD_FUNC_INLINE_DECLARE_DETAIL_0(G, \
        Type const &, Name \
        ) \
    { \
        return _ZFP_v_##Name; \
    } \
    /** @brief see #ZFEXPORT_VAR_DECLARE */ \
    extern ZF_ENV_EXPORT void ZFExportVarEnsureInit_##Name(void);

/** @brief see #ZFEXPORT_VAR_DECLARE */
#define ZFEXPORT_VAR_READONLY_DEFINE(Type, Name, initValue) \
    Type _ZFP_v_##Name; \
    ZFMETHOD_FUNC_INLINE_DEFINE_DETAIL_0(G, \
        Type const &, Name \
        ) \
    _ZFP_ZFEXPORT_VAR_INIT_VALUE(Type, Name, initValue) \
    void ZFExportVarEnsureInit_##Name(void) \
    { \
        (void)ZF_GLOBAL_INITIALIZER_INSTANCE(ExpV_##Name); \
    }

/** @brief see #ZFEXPORT_VAR_DECLARE */
#define ZFEXPORT_VAR_READONLY_ALIAS_DECLARE(Type, Name, AliasName) \
    ZFMETHOD_FUNC_INLINE_DECLARE_DETAIL_0(G, \
        Type const &, Name \
        ) \
    { \
        return (AliasName)(); \
    } \
    /** @brief see #ZFEXPORT_VAR_DECLARE */ \
    extern ZF_ENV_EXPORT void ZFExportVarEnsureInit_##Name(void);

/** @brief see #ZFEXPORT_VAR_DECLARE */
#define ZFEXPORT_VAR_READONLY_ALIAS_DEFINE(Type, Name, AliasName) \
    ZFMETHOD_FUNC_INLINE_DEFINE_DETAIL_0(G, \
        Type const &, Name \
        ) \
    void ZFExportVarEnsureInit_##Name(void) \
    { \
        ZFExportVarEnsureInit_##AliasName(); \
    }

/** @brief see #ZFEXPORT_VAR_DECLARE */
#define ZFEXPORT_VAR_READONLY_VALUEREF_DECLARE(Type, Name, ValueRef) \
    ZFMETHOD_FUNC_INLINE_DECLARE_DETAIL_0(G, \
        Type const &, Name \
        ) \
    { \
        return ValueRef; \
    } \
    /** @brief see #ZFEXPORT_VAR_DECLARE */ \
    inline void ZFExportVarEnsureInit_##Name(void) \
    { \
    }

/** @brief see #ZFEXPORT_VAR_DECLARE */
#define ZFEXPORT_VAR_READONLY_VALUEREF_DEFINE(Type, Name, ValueRef) \
    ZFMETHOD_FUNC_INLINE_DEFINE_DETAIL_0(G, \
        Type const &, Name \
        )

// ============================================================
/**
 * @brief util macro to export raw enum value as zfint
 *
 * for normal enum, use #ZFENUM_BEGIN is recommended\n
 * this macro is used to export low level enum values by #ZFMETHOD_FUNC_USER_REGISTER_0,
 * with this proto type:
 * @code
 *   zfint YourRawEnumValueName(void);
 * @endcode
 */
#define ZFEXPORT_ENUM_DEFINE(EnumName, enumValues, ...) \
    _ZFP_ZFEXPORT_ENUM_DEFINE(ZF_CALLER_LINE, EnumName, enumValues, ##__VA_ARGS__)

#define _ZFP_ZFEXPORT_ENUM_DEFINE_EXPAND(...) __VA_ARGS__
#define _ZFP_ZFEXPORT_ENUM_DEFINE(...) \
    _ZFP_ZFEXPORT_ENUM_DEFINE_EXPAND(_ZFP_ZFEXPORT_ENUM_DEFINE_(__VA_ARGS__))
#define _ZFP_ZFEXPORT_ENUM_DEFINE_(RegSig, EnumName, enumValues, ...) \
    ZF_STATIC_REGISTER_INIT(ZFEXPORT_ENUM_##EnumName##_##RegSig) \
    { \
        typedef EnumName _EnumName; \
        ZFM_FIX_PARAM(_ZFP_ZFEXPORT_ENUM_EXPAND, ZFM_EMPTY, enumValues, ##__VA_ARGS__) \
    } \
    ZF_STATIC_REGISTER_DESTROY(ZFEXPORT_ENUM_##EnumName##_##RegSig) \
    { \
        for(zfindex i = 0; i < m.count(); ++i) \
        { \
            ZFMethodFuncUserUnregister(m[i]); \
        } \
    } \
    ZFCoreArrayPOD<const ZFMethod *> m; \
    ZF_STATIC_REGISTER_END(ZFEXPORT_ENUM_##EnumName##_##RegSig)
#define _ZFP_ZFEXPORT_ENUM_EXPAND(v) \
    { \
        ZFMethodFuncUserRegister_0(resultMethod, {return v;}, ZF_NAMESPACE_CURRENT(), _EnumName, ZFM_TOSTRING(v)); \
        m.add(resultMethod); \
    }

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFExport_h_

