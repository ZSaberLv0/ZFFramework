/**
 * @file ZFCoreStaticInitializer.h
 * @brief static initializer step
 */
#ifndef _ZFI_ZFCoreStaticInitializer_h_
#define _ZFI_ZFCoreStaticInitializer_h_

#include "ZFCoreStaticRegister.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
typedef void *(*_ZFP_SI_Constructor)(void);
typedef void (*_ZFP_SI_Destructor)(ZF_IN void *p);
zfclassLikePOD ZF_ENV_EXPORT _ZFP_SI_Holder
{
public:
    _ZFP_SI_Holder(ZF_IN const zfchar *name,
                   ZF_IN _ZFP_SI_Constructor constructor,
                   ZF_IN _ZFP_SI_Destructor destructor);
    ~_ZFP_SI_Holder(void);
public:
    void *instance;
};

// ============================================================
/**
 * @brief static initializer
 *
 * called before any other active code,
 * must be used in a cpp file\n
 * typical usage:
 * @code
 *   // in a cpp file only
 *   ZF_STATIC_INITIALIZER_INIT(YourInitName)
 *   : privateMember() // you may add member init here
 *   {
 *       // add your own init phase here
 *       // note: never access ZFFramework member here
 *   }
 *   ZF_STATIC_INITIALIZER_DESTROY(YourInitName)
 *   {
 *       // add your own cleanup phase here
 *       // note: never access ZFFramework member here
 *   }
 *   private:
 *       zfbool privateMember; // you may add member declaration here
 *       void func(void) {} // you may add private function here
 *   ZF_STATIC_INITIALIZER_END(YourInitName)
 * @endcode
 * \n
 * @note if more than one initializer exists,
 *   the order of the initialization is not ensured
 * @note use #ZF_GLOBAL_INITIALIZER_INIT is recommended at most of case
 * @warning you should not access any of ZFFramework members
 *   in the static initializer,
 *   you may check it by #ZFFrameworkStateCheck
 * @note see #ZF_STATIC_REGISTER_INIT for recommended usage
 */
#define ZF_STATIC_INITIALIZER_INIT(Name) \
    zfclassNotPOD _ZFP_SI_##Name \
    { \
    protected: \
        typedef _ZFP_SI_##Name zfself; \
    public: \
        static void *_ZFP_SI_ctor_##Name(void) \
        { \
            return (void *)zfnew(_ZFP_SI_##Name); \
        } \
        static void _ZFP_SI_dtor_##Name(ZF_IN void *p) \
        { \
            zfdelete(ZFCastStatic(_ZFP_SI_##Name *, p)); \
        } \
        static _ZFP_SI_##Name *_ZFP_SI_instanceAccess(void) \
        { \
            static _ZFP_SI_Holder d(ZFM_TOSTRING_DIRECT(Name), \
                _ZFP_SI_##Name::_ZFP_SI_ctor_##Name, \
                _ZFP_SI_##Name::_ZFP_SI_dtor_##Name); \
            return ZFCastStatic(_ZFP_SI_##Name *, d.instance); \
        } \
    public: \
        _ZFP_SI_##Name(void)
/**
 * @brief see #ZF_STATIC_INITIALIZER_INIT
 */
#define ZF_STATIC_INITIALIZER_DESTROY(Name) \
        ~_ZFP_SI_##Name(void)
/**
 * @brief see #ZF_STATIC_INITIALIZER_INIT
 */
#define ZF_STATIC_INITIALIZER_END(Name) \
    }; \
    ZF_STATIC_REGISTER_INIT(SI_##Name) \
    { \
        _ZFP_SI_##Name::_ZFP_SI_instanceAccess(); \
    } \
    ZF_STATIC_REGISTER_END(SI_##Name)
/**
 * @brief access the instance of the initializer
 */
#define ZF_STATIC_INITIALIZER_INSTANCE(Name) \
    (_ZFP_SI_##Name::_ZFP_SI_instanceAccess())

#define _ZFP_ZF_STATIC_INITIALIZER_CLASS(Name) \
    _ZFP_SI_##Name
/**
 * @brief the class holder of the static initializer
 */
#define ZF_STATIC_INITIALIZER_CLASS(Name) \
    _ZFP_ZF_STATIC_INITIALIZER_CLASS(Name)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFCoreStaticInitializer_h_

