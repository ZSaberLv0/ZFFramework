/**
 * @file ZFCoreGlobalInitializer.h
 * @brief global initializer step
 */
#ifndef _ZFI_ZFCoreGlobalInitializer_h_
#define _ZFI_ZFCoreGlobalInitializer_h_

#include "ZFLevel.h"
#include "ZFCoreStaticRegister.h"
#include "ZFCoreArray.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/** @brief see #ZFFrameworkInit */
typedef void (*ZFFrameworkStateChangeCallback)(void);
extern ZF_ENV_EXPORT ZFCoreArrayPOD<ZFFrameworkStateChangeCallback> &_ZFP_ZFFrameworkInitFinishCallbacks(void);
extern ZF_ENV_EXPORT ZFCoreArrayPOD<ZFFrameworkStateChangeCallback> &_ZFP_ZFFrameworkCleanupPrepareCallbacks(void);
/** @brief called when #ZFFrameworkInit finished */
#define ZFFrameworkInitFinishCallbacks _ZFP_ZFFrameworkInitFinishCallbacks()
/** @brief called when #ZFFrameworkInit finished */
#define ZFFrameworkCleanupPrepareCallbacks _ZFP_ZFFrameworkCleanupPrepareCallbacks()

// ============================================================
/**
 * @brief must be called in main thread before any access of ZFFramework member
 *
 * usually, this would been done by implementation of ZFFramework\n
 * \n
 * ADVANCED:\n
 * if you want to dynamically load other libs written in ZFFramework
 * after ZFFramework's main lib initialized:
 * -  you must manually call #ZFFrameworkInit to initialize your newly loaded lib
 * -  when unload the additional lib,
 *   the #ZF_GLOBAL_INITIALIZER_DESTROY's order is not ensured
 * -  #ZFFrameworkInitFinishCallbacks and #ZFFrameworkCleanupPrepareCallbacks
 *   won't be called when loading the additional lib
 */
extern ZF_ENV_EXPORT void ZFFrameworkInit(void);
/**
 * @brief see #ZFFrameworkInit
 */
extern ZF_ENV_EXPORT void ZFFrameworkCleanup(void);

/**
 * @brief state for ZFFramework
 */
typedef enum {
    ZFFrameworkStateNotAvailable, /**< @brief not available */
    ZFFrameworkStateInitProcessing, /**< @brief during init step (#ZFFrameworkInit) */
    ZFFrameworkStateAvailable, /**< @brief available, init finished */
    ZFFrameworkStateCleanupProcessing, /**< @brief during cleanup step (#ZFFrameworkCleanup) */
} ZFFrameworkState;

/** @brief string tokens */
#define ZFTOKEN_ZFFrameworkStateNotAvailable "StateNotAvailable"
/** @brief string tokens */
#define ZFTOKEN_ZFFrameworkStateInitProcessing "StateInitProcessing"
/** @brief string tokens */
#define ZFTOKEN_ZFFrameworkStateAvailable "StateAvailable"
/** @brief string tokens */
#define ZFTOKEN_ZFFrameworkStateCleanupProcessing "StateCleanupProcessing"

/**
 * @brief assert fail and output log if ZFFramework not initialized
 */
extern ZF_ENV_EXPORT void ZFFrameworkAssertInit(void);
/**
 * @brief get current state of ZFFramework
 */
extern ZF_ENV_EXPORT ZFFrameworkState ZFFrameworkStateCheck(void);
/**
 * @brief get current state of ZFFramework of specified level
 */
extern ZF_ENV_EXPORT ZFFrameworkState ZFFrameworkStateCheck(ZF_IN ZFLevel level);

// ============================================================
typedef void *(*_ZFP_GI_Constructor)(void);
typedef void (*_ZFP_GI_Destructor)(ZF_IN void *p);
zfclassFwd _ZFP_GI_RegPrivate;
zfclassNotPOD ZF_ENV_EXPORT _ZFP_GI_Reg
{
public:
    _ZFP_GI_Reg(ZF_IN const zfchar *name,
                ZF_IN ZFLevel level,
                ZF_IN _ZFP_GI_Constructor constructor,
                ZF_IN _ZFP_GI_Destructor destructor);
    ~_ZFP_GI_Reg(void);
public:
    void *instanceAccess(void);
private:
    _ZFP_GI_RegPrivate *d;
};

#define _ZFP_ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(Name, ZFLevel_) \
    static void *_ZFP_GI_ctor_##Name(void); \
    static void _ZFP_GI_dtor_##Name(ZF_IN void *p); \
    static _ZFP_GI_Reg _ZFP_GI_reg_##Name(ZFM_TOSTRING(Name), \
            ZFLevel_, \
            _ZFP_GI_ctor_##Name, \
            _ZFP_GI_dtor_##Name \
        ); \
    zfclassNotPOD _ZFP_GI_##Name \
    { \
    protected: \
        typedef _ZFP_GI_##Name zfself; \
    public: \
        static _ZFP_GI_##Name *_ZFP_GI_instance(void) \
        { \
            return ZFCastStatic(_ZFP_GI_##Name *, _ZFP_GI_reg_##Name.instanceAccess()); \
        } \
        _ZFP_GI_##Name(void)
/**
 * @brief code block that will be executed when app start and stop
 *
 * usage:
 * @code
 *   // in a *.cpp file
 *   ZF_GLOBAL_INITIALIZER_INIT(SomeName)
 *   : data('c') // you can init member here
 *   {
 *       // code that will be executed when app start
 *   }
 *   ZF_GLOBAL_INITIALIZER_DESTROY(SomeName)
 *   {
 *       // code that will be executed when app stop
 *   }
 *   private:
 *       zfchar data; // you can add member here
 *   private:
 *       void func() {} // you can add function here
 *   ZF_GLOBAL_INITIALIZER_END(SomeName)
 * @endcode
 * \n
 * @warning if you have more than one initializer with same level,
 *   the execute order of the code block is not ensured
 * @warning init and destroy step is not one time,
 *   they'll be called each time ZFFrameworkInit/ZFFrameworkCleanup is called
 * @note see #ZF_STATIC_REGISTER_INIT for recommended usage
 */
#define ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(Name, ZFLevel_) \
    _ZFP_ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(Name, ZFLevel_)

/**
 * @brief see #ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL
 */
#define ZF_GLOBAL_INITIALIZER_INIT(Name) \
    _ZFP_ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(Name, ZFLevelAppNormal)

#define _ZFP_ZF_GLOBAL_INITIALIZER_DESTROY(Name) \
    public: \
        ~_ZFP_GI_##Name(void)
/**
 * @brief see #ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL
 */
#define ZF_GLOBAL_INITIALIZER_DESTROY(Name) \
    _ZFP_ZF_GLOBAL_INITIALIZER_DESTROY(Name)

#define _ZFP_ZF_GLOBAL_INITIALIZER_END(Name) \
    }; \
    static void *_ZFP_GI_ctor_##Name(void) \
    { \
        return (void *)zfnew(_ZFP_GI_##Name); \
    } \
    static void _ZFP_GI_dtor_##Name(ZF_IN void *p) \
    { \
        zfdelete((_ZFP_GI_##Name *)p); \
    }
/**
 * @brief see #ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL
 */
#define ZF_GLOBAL_INITIALIZER_END(Name) \
    _ZFP_ZF_GLOBAL_INITIALIZER_END(Name)

#define _ZFP_ZF_GLOBAL_INITIALIZER_INSTANCE(Name) \
    (_ZFP_GI_##Name::_ZFP_GI_instance())
/**
 * @brief access the instance of the initializer
 */
#define ZF_GLOBAL_INITIALIZER_INSTANCE(Name) \
    _ZFP_ZF_GLOBAL_INITIALIZER_INSTANCE(Name)

#define _ZFP_ZF_GLOBAL_INITIALIZER_CLASS(Name) \
    _ZFP_GI_##Name
/**
 * @brief the class holder of the global initializer
 */
#define ZF_GLOBAL_INITIALIZER_CLASS(Name) \
    _ZFP_ZF_GLOBAL_INITIALIZER_CLASS(Name)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFCoreGlobalInitializer_h_

