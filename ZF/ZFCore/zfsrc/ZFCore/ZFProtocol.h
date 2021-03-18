/**
 * @file ZFProtocol.h
 * @brief protocol definitions for ZFFramework
 */

#ifndef _ZFI_ZFProtocol_h_
#define _ZFI_ZFProtocol_h_

#include "ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief level type for ZFProtocol, see #ZFPROTOCOL_IMPLEMENTATION_BEGIN
 */
ZFENUM_BEGIN(ZFProtocolLevel)
    ZFENUM_VALUE(Default)
    ZFENUM_VALUE(SystemLow)
    ZFENUM_VALUE(SystemNormal)
    ZFENUM_VALUE(SystemHigh)
    ZFENUM_VALUE(AppLow)
    ZFENUM_VALUE(AppNormal)
    ZFENUM_VALUE(AppHigh)
ZFENUM_SEPARATOR(ZFProtocolLevel)
    ZFENUM_VALUE_REGISTER(Default)
    ZFENUM_VALUE_REGISTER(SystemLow)
    ZFENUM_VALUE_REGISTER(SystemNormal)
    ZFENUM_VALUE_REGISTER(SystemHigh)
    ZFENUM_VALUE_REGISTER(AppLow)
    ZFENUM_VALUE_REGISTER(AppNormal)
    ZFENUM_VALUE_REGISTER(AppHigh)
ZFENUM_END(ZFProtocolLevel)

// ============================================================
/**
 * @brief base class of all protocol of ZFFramework
 */
zfclassNotPOD ZF_ENV_EXPORT ZFProtocol
{
    ZFCLASS_DISALLOW_COPY_CONSTRUCTOR(ZFProtocol)

public:
    /**
     * @brief called to init the protocol
     *
     * similar to #ZFObject::objectOnInit, we have these functions for protocol to do custom init steps (ordered by life cycle):
     * -# normal object constructor
     * -# #protocolOnInit : called when first time accessed
     * -# #protocolOnInitFinish : called when first time accessed
     * -# #protocolOnDeallocPrepare : called during #ZFFrameworkCleanup as level #ZFLevelZFFrameworkEssential,
     *   or called when changing impl manually
     * -# #protocolOnDealloc : called during #ZFFrameworkCleanup as level #ZFLevelZFFrameworkStatic,
     *   or called when changing impl manually
     * -# normal object destructor
     */
    virtual void protocolOnInit(void)
    {
    }
    /** @brief see #protocolOnInit */
    virtual void protocolOnInitFinish(void)
    {
    }
    /** @brief see #protocolOnInit */
    virtual void protocolOnDeallocPrepare(void)
    {
    }
    /** @brief see #protocolOnInit */
    virtual void protocolOnDealloc(void)
    {
    }

public:
    /**
     * @brief protocol state
     *
     * note, it's ensured that:
     * @code
     *   ProtocolInstanceState state0 = ProtocolInstanceStateOnInitFinish;
     *   zfbool t0 = ZFBitTest(state0, ProtocolInstanceStateOnInit); // ensured true
     *
     *   ProtocolInstanceState state1 = ProtocolInstanceStateOnDeallocPrepare;
     *   zfbool t1 = ZFBitTest(state1, ProtocolInstanceStateOnDealloc); // ensured true
     * @endcode
     */
    typedef enum {
        ProtocolInstanceStateOnInit = 0x01, /**< @brief protocol is under #protocolOnInit */
        ProtocolInstanceStateOnInitFinish = 0x03, /**< @brief protocol is under #protocolOnInitFinish */
        ProtocolInstanceStateIdle = 0, /**< @brief protocol is constructed successfully */
        ProtocolInstanceStateOnDeallocPrepare = 0x30, /**< @brief protocol is under #protocolOnDeallocPrepare */
        ProtocolInstanceStateOnDealloc = 0x10, /**< @brief protocol is under #protocolOnDealloc */
    } ProtocolInstanceState;
    /**
     * @brief protocol instance's state
     */
    virtual ZFProtocol::ProtocolInstanceState protocolInstanceState(void)
    {
        return _ZFP_ZFProtocol_protocolInstanceState;
    }
    ZFProtocol::ProtocolInstanceState _ZFP_ZFProtocol_protocolInstanceState;

public:
    /**
     * @brief get the name of the protocol module, e.g. "ZFObject"
     */
    virtual const zfchar *protocolName(void) const
    {
        return zfnull;
    }
    /**
     * @brief get the name of the protocol implementation, e.g. "ZFObject_default"
     */
    virtual const zfchar *protocolImplementationName(void) const
    {
        return zfnull;
    }
    /**
     * @brief get the level of the protocol implementation
     */
    virtual ZFProtocolLevelEnum protocolImplementationLevel(void) const
    {
        return ZFProtocolLevel::e_Default;
    }
    /**
     * @brief get type hint of the implementation or empty string if not defined, e.g. "iOS:NSString" for iOS's string impl,
     *   use #ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT to register
     * @note usually for debug use only, do not abuse it\n
     *   for some implementation that depends on other packaged implementations,
     *   you may check this value to be your expected value and assert false if not
     *   (for example, view's impl usually depends on string's impl)
     */
    virtual const zfchar *protocolImplementationPlatformHint(void) const
    {
        return "";
    }

public:
    /** @brief see #objectInfo */
    virtual void objectInfoT(ZF_IN_OUT zfstring &ret) const
    {
        zfstringAppend(ret, "<[%s] %s%s>",
            this->protocolName(),
            this->protocolImplementationName(),
            (!zfsIsEmpty(this->protocolImplementationPlatformHint()) ? zfstringWithFormat(" (%s)", this->protocolImplementationPlatformHint()).cString() : "")
            );
    }
    /**
     * @brief get a short info about this object
     */
    virtual inline zfstring objectInfo(void) const
    {
        zfstring ret;
        this->objectInfoT(ret);
        return ret;
    }

public:
    virtual zfbool _ZFP_ZFProtocol_checkImplDependency(ZF_OUT const zfchar *&desiredImplPlatformHint,
                                                       ZF_OUT const zfchar *&mismatchProtocolName,
                                                       ZF_OUT ZFProtocol *&mismatchImpl) const
    {
        return zftrue;
    }
protected:
    static zfbool _ZFP_ZFProtocolOptional(void)
    {
        return zffalse;
    }

public:
    /** @cond ZFPrivateDoc */
    ZFProtocol(void) : _ZFP_ZFProtocol_protocolInstanceState(ZFProtocol::ProtocolInstanceStateOnInit) {}
    virtual ~ZFProtocol(void) {}
    /** @endcond */
};

// ============================================================
ZFOUTPUT_TYPE_DECLARE(const ZFProtocol *)
ZFOUTPUT_TYPE(ZFProtocol *, {output << (const ZFProtocol *)v;})
ZFOUTPUT_TYPE(ZFProtocol, {output << (const ZFProtocol *)&v;})

// ============================================================
zfclassFwd _ZFP_ZFProtocolData;
typedef ZFProtocol *(*_ZFP_ZFProtocolTryAccessCallback)(void);
typedef ZFProtocol *(*_ZFP_ZFProtocolConstructor)(void);
typedef void (*_ZFP_ZFProtocolCleanupCallback)(ZF_IN _ZFP_ZFProtocolData *implData);
zfclassLikePOD ZF_ENV_EXPORT _ZFP_ZFProtocolData
{
public:
    zfstring protocolName;
    zfbool protocolOptional;
    _ZFP_ZFProtocolTryAccessCallback implTryAccessCallback;
    _ZFP_ZFProtocolConstructor implConstructor;
    zfstring implName;
    ZFProtocolLevelEnum implLevel;
    _ZFP_ZFProtocolCleanupCallback implCleanupCallback;
    ZFProtocol *implInstance;
};
extern ZF_ENV_EXPORT _ZFP_ZFProtocolData &_ZFP_ZFProtocolImplDataRegister(zfbool *ZFCoreLibDestroyFlag,
                                                                          const zfchar *protocolName,
                                                                          _ZFP_ZFProtocolTryAccessCallback implTryAccessCallback,
                                                                          zfbool protocolOptional);
extern ZF_ENV_EXPORT void _ZFP_ZFProtocolImplDataUnregister(zfbool *ZFCoreLibDestroyFlag, const zfchar *protocolName);
extern ZF_ENV_EXPORT void _ZFP_ZFProtocolImplAccess(void);

// ============================================================
/**
 * @brief access the class name of ZFProtocol for ModuleName
 *
 * usually no use, unless the protocol module needs a constructor
 */
#define ZFPROTOCOL_INTERFACE_CLASS(ModuleName) \
    ZFPInterface_##ModuleName

#define _ZFP_ZFPROTOCOL_INTERFACE_BEGIN(ModuleName) \
    zfclassNotPOD ZF_ENV_EXPORT ZFPROTOCOL_INTERFACE_CLASS(ModuleName) : zfextendsNotPOD ZFProtocol \
    { \
        protected: \
            /** @brief class ref to super */ \
            typedef ZFProtocol zfsuper; \
            /** @brief class ref to self */ \
            typedef ZFPROTOCOL_INTERFACE_CLASS(ModuleName) zfself; \
        public: \
            zfoverride \
            virtual const zfchar *protocolName(void) const \
            { \
                return ZFM_TOSTRING_DIRECT(ModuleName); \
            } \
        public: \
            static zfbool *_ZFP_ZFProtocolZFCoreLibDestroyFlag(void) \
            { \
                static zfbool ZFCoreLibDestroyFlag = zffalse; \
                return &ZFCoreLibDestroyFlag; \
            } \
            static _ZFP_ZFProtocolData &_ZFP_ZFProtocolDataRef(void) \
            { \
                static _ZFP_ZFProtocolData &_d = _ZFP_ZFProtocolImplDataRegister( \
                    zfself::_ZFP_ZFProtocolZFCoreLibDestroyFlag(), \
                    ZFM_TOSTRING_DIRECT(ModuleName), \
                    zfself::_ZFP_ZFProtocolTryAccessCallbackFunc, \
                    zfself::_ZFP_ZFProtocolOptional()); \
                return _d; \
            } \
            static void _ZFP_ZFProtocolDataRegister(void) \
            { \
                (void)zfself::_ZFP_ZFProtocolDataRef(); \
            } \
            static ZFProtocol *_ZFP_ZFProtocolTryAccessCallbackFunc(void) \
            { \
                return ZFPROTOCOL_TRY_ACCESS(ModuleName); \
            } \
            static void _ZFP_ZFProtocolInstanceCleanupCallback(ZF_IN _ZFP_ZFProtocolData *implData) \
            { \
                if(implData->implInstance != zfnull) \
                { \
                    zfdelete(implData->implInstance); \
                    implData->implInstance = zfnull; \
                } \
            } \
            static void _ZFP_ZFProtocolImplementationRegister(ZF_IN _ZFP_ZFProtocolConstructor implConstructor, \
                                                              ZF_IN const zfchar *implName, \
                                                              ZF_IN ZFProtocolLevelEnum implLevel) \
            { \
                _ZFP_ZFProtocolData &_d = zfself::_ZFP_ZFProtocolDataRef(); \
                if(_d.implConstructor != zfnull) \
                { \
                    if(_d.implLevel == implLevel) \
                    { \
                        zfCoreCriticalMessageTrim("only one implementation is allowed for protocol \"%s\", while we have: \"%s\", \"%s\"", \
                            #ModuleName, \
                            _d.implName.cString(), \
                            implName); \
                        return ; \
                    } \
                    else if(_d.implLevel > implLevel) \
                    { \
                        /* \
                        zfCoreLogTrim("ignore implementation \"%s\" while having \"%s\" for protocol \"%s\"", \
                            implName, _d.implName.cString(), #ModuleName); \
                         */ \
                        return ; \
                    } \
                    else \
                    { \
                        /* \
                        zfCoreLogTrim("reset implementation for protocol \"%s\", old: \"%s\", new: \"%s\"", \
                            #ModuleName, _d.implName.cString(), implName); \
                         */ \
                    } \
                } \
                _d.implConstructor = implConstructor; \
                _d.implName = implName; \
                _d.implLevel = implLevel; \
                _d.implCleanupCallback = zfself::_ZFP_ZFProtocolInstanceCleanupCallback; \
            } \
            static void _ZFP_ZFProtocolImplementationChange(ZF_IN _ZFP_ZFProtocolConstructor implConstructor, \
                                                            ZF_IN const zfchar *implName, \
                                                            ZF_IN ZFProtocolLevelEnum implLevel) \
            { \
                _ZFP_ZFProtocolData &_d = zfself::_ZFP_ZFProtocolDataRef(); \
                if(_d.implInstance != zfnull) \
                { \
                    _d.implInstance->_ZFP_ZFProtocol_protocolInstanceState = ZFProtocol::ProtocolInstanceStateOnDeallocPrepare; \
                    _d.implInstance->protocolOnDeallocPrepare(); \
                    _d.implInstance->_ZFP_ZFProtocol_protocolInstanceState = ZFProtocol::ProtocolInstanceStateOnDealloc; \
                    _d.implInstance->protocolOnDealloc(); \
                    _d.implInstance->_ZFP_ZFProtocol_protocolInstanceState = ZFProtocol::ProtocolInstanceStateOnInit; \
                } \
                zfself::_ZFP_ZFProtocolInstanceCleanupCallback(&_d); \
                if(implConstructor != zfnull && !zfsIsEmpty(implName)) \
                { \
                    _d.implConstructor = implConstructor; \
                    _d.implName = implName; \
                    _d.implLevel = implLevel; \
                } \
                else \
                { \
                    _d.implConstructor = zfnull; \
                    _d.implName.removeAll(); \
                    _d.implLevel = ZFProtocolLevel::e_Default; \
                } \
            } \
            static inline zfbool _ZFP_ZFProtocolIsAvailable(void) \
            { \
                return (zfself::_ZFP_ZFProtocolDataRef().implConstructor != zfnull); \
            } \
            static zfself *_ZFP_ZFProtocolNewInstance(void) \
            { \
                _ZFP_ZFProtocolData &_d = zfself::_ZFP_ZFProtocolDataRef(); \
                _ZFP_ZFProtocolImplAccess(); \
                zfself *retVal = zfnull; \
                if(_d.implConstructor != zfnull) \
                { \
                    retVal = ZFCastStatic(zfself *, _d.implConstructor()); \
                    if(retVal != zfnull) \
                    { \
                        retVal->_ZFP_ZFProtocol_protocolInstanceState = ZFProtocol::ProtocolInstanceStateOnInit; \
                        retVal->protocolOnInit(); \
                        retVal->_ZFP_ZFProtocol_protocolInstanceState = ZFProtocol::ProtocolInstanceStateOnInitFinish; \
                        retVal->protocolOnInitFinish(); \
                        retVal->_ZFP_ZFProtocol_protocolInstanceState = ZFProtocol::ProtocolInstanceStateIdle; \
                    } \
                } \
                return retVal; \
            } \
            static zfself *_ZFP_ZFProtocolAccess(void) \
            { \
                static ZFProtocol *&impl = zfself::_ZFP_ZFProtocolDataRef().implInstance; \
                if(impl == zfnull) \
                { \
                    _ZFP_ZFProtocolData &_d = zfself::_ZFP_ZFProtocolDataRef(); \
                    _d.implInstance = zfself::_ZFP_ZFProtocolNewInstance(); \
                    if(_d.implInstance == zfnull) \
                    { \
                        zfCoreCriticalMessageTrim("no implementation for protocol \"%s\"", \
                            ZFM_TOSTRING(ModuleName)); \
                        return zfnull; \
                    } \
                    const zfchar *desiredImplPlatformHint = zfnull; \
                    const zfchar *mismatchProtocolName = zfnull; \
                    ZFProtocol *mismatchImpl = zfnull; \
                    if(!_d.implInstance->_ZFP_ZFProtocol_checkImplDependency(desiredImplPlatformHint, mismatchProtocolName, mismatchImpl)) \
                    { \
                        zfCoreCriticalErrorPrepare(); \
                        zfCoreLogTrimNoEndl("(%s)%s depends on (%s)\"%s\"", \
                            _d.implInstance->protocolName(), \
                            _d.implInstance->protocolImplementationName(), \
                            mismatchProtocolName, \
                            desiredImplPlatformHint); \
                        if(mismatchImpl == zfnull) \
                        { \
                            zfCoreLogTrim(" but it's not implemented"); \
                        } \
                        else \
                        { \
                            zfCoreLogTrim(" but it's implementation \"%s\" mismatch", \
                                mismatchImpl->protocolImplementationPlatformHint()); \
                        } \
                        zfCoreCriticalError(); \
                        zfdelete(_d.implInstance); \
                        _d.implInstance = zfnull; \
                        return zfnull; \
                    } \
                } \
                return ZFCastStatic(zfself *, impl); \
            } \
            static zfself *_ZFP_ZFProtocolTryAccess(void) \
            { \
                static ZFProtocol *&impl = zfself::_ZFP_ZFProtocolDataRef().implInstance; \
                if(impl == zfnull) \
                { \
                    _ZFP_ZFProtocolData &_d = zfself::_ZFP_ZFProtocolDataRef(); \
                    _d.implInstance = zfself::_ZFP_ZFProtocolNewInstance(); \
                    const zfchar *desiredImplPlatformHint = zfnull; \
                    const zfchar *mismatchProtocolName = zfnull; \
                    ZFProtocol *mismatchImpl = zfnull; \
                    if(_d.implInstance != zfnull \
                        && !_d.implInstance->_ZFP_ZFProtocol_checkImplDependency(desiredImplPlatformHint, mismatchProtocolName, mismatchImpl)) \
                    { \
                        zfdelete(_d.implInstance); \
                        _d.implInstance = zfnull; \
                    } \
                } \
                return ZFCastStatic(zfself *, impl); \
            }
#define _ZFP_ZFPROTOCOL_INTERFACE_END(ModuleName) \
    };

/**
 * @brief declare a protocol interface for ZFFramework
 *
 * a protocol interface is a part that must be implemented,
 * usually it's under Module/protocol such as ZFCore/protocol,
 * meaning you should implement all protocols under ZFCore/protocol if you
 * want to use ZFCore module\n
 * this macro is used internally to declare a protocol\n
 * typical usage:
 * @code
 *   // in a *.h file
 *   ZFPROTOCOL_INTERFACE_BEGIN(SomeModule)
 *       public: // you can add constructors, but with no arguments only
 *           ZFPROTOCOL_INTERFACE_CLASS(SomeModule)(void) {...}
 *           virtual ~ZFPROTOCOL_INTERFACE_CLASS(SomeModule)(void) {...}
 *       public: // declare protocols here
 *           virtual void moduleFunc(void) zfpurevirtual;
 *       private:
 *           SomeType member; // you can add member here
 *   ZFPROTOCOL_INTERFACE_END(SomeModule)
 * @endcode
 * for the difference between them, see #ZFPROTOCOL_TRY_ACCESS\n
 * for how to implement it, see #ZFPROTOCOL_IMPLEMENTATION_BEGIN\n
 * for how to access the implementation, see #ZFPROTOCOL_TRY_ACCESS
 * @note all protocol is singleton mode
 */
#define ZFPROTOCOL_INTERFACE_BEGIN(ModuleName) \
    _ZFP_ZFPROTOCOL_INTERFACE_BEGIN(ModuleName)

/**
 * @brief for more information, please refer to #ZFPROTOCOL_INTERFACE_BEGIN
 */
#define ZFPROTOCOL_INTERFACE_END(ModuleName) \
    _ZFP_ZFPROTOCOL_INTERFACE_END(ModuleName)

/**
 * @brief macro to show that the protocol is optional
 */
#define ZFPROTOCOL_INTERFACE_OPTIONAL \
    protected: \
        static zfbool _ZFP_ZFProtocolOptional(void) \
        { \
            return zftrue; \
        } \
    public:

/**
 * @brief statically register the protocol interface, must be placed in source file
 */
#define ZFPROTOCOL_INTERFACE_REGISTER(ModuleName) \
    ZF_STATIC_REGISTER_INIT(ZFProtocolInterfaceRegister_##ModuleName) \
    { \
        ZFPROTOCOL_INTERFACE_CLASS(ModuleName)::_ZFP_ZFProtocolDataRegister(); \
    } \
    ZF_STATIC_REGISTER_END(ZFProtocolInterfaceRegister_##ModuleName)

// ============================================================
/**
 * @brief access the protocol implementation
 *
 * return protocol implementation or null if not implemented\n
 * typical usage:
 * @code
 *   ZFPROTOCOL_INTERFACE_CLASS(SomeModule) *impl = zfnull;
 *   // access the implementation of the protocol
 *   impl = ZFPROTOCOL_TRY_ACCESS(SomeModule);
 *   if(impl == zfnull)
 *   {
 *       // error, have no implementation
 *       exit(); // then you can handle it here, exit or ignore, whatever you like
 *   }
 *   ...
 *   // use it
 *   impl->someFunc();
 * @endcode
 * all protocol is singleton protocol,
 * ZFPROTOCOL_TRY_ACCESS would create new instance only for the first time,
 * and the instance would be destroyed at a certain proper time\n
 * \n
 * when using ZFPROTOCOL_TRY_ACCESS, you take full responsibility to check whether
 * the implmentation is available
 */
#define ZFPROTOCOL_TRY_ACCESS(ModuleName) \
    (ZFPROTOCOL_INTERFACE_CLASS(ModuleName)::_ZFP_ZFProtocolTryAccess())

/**
 * @brief similar to #ZFPROTOCOL_TRY_ACCESS, but assert fail if no implementation
 */
#define ZFPROTOCOL_ACCESS(ModuleName) \
    (ZFPROTOCOL_INTERFACE_CLASS(ModuleName)::_ZFP_ZFProtocolAccess())

/**
 * @brief test whether the protocol is available
 */
#define ZFPROTOCOL_IS_AVAILABLE(ModuleName) \
    ZFPROTOCOL_INTERFACE_CLASS(ModuleName)::_ZFP_ZFProtocolIsAvailable()

typedef void (*_ZFP_ZFProtocolImplRegisterHolderClearCallback)(void);
zfclassLikePOD ZF_ENV_EXPORT _ZFP_ZFProtocolImplRegisterHolder
{
public:
    _ZFP_ZFProtocolImplRegisterHolder(ZF_IN zfbool *ZFCoreLibDestroyFlag,
                                      ZF_IN _ZFP_ZFProtocolImplRegisterHolderClearCallback callback)
    : ZFCoreLibDestroyFlag(ZFCoreLibDestroyFlag)
    , callback(callback)
    {
    }
    ~_ZFP_ZFProtocolImplRegisterHolder(void)
    {
        if(!*(this->ZFCoreLibDestroyFlag))
        {
            this->callback();
        }
    }
private:
    zfbool *ZFCoreLibDestroyFlag;
    _ZFP_ZFProtocolImplRegisterHolderClearCallback callback;
};
// ============================================================
#define _ZFP_ZFPROTOCOL_IMPLEMENTATION_BEGIN_(ImplementationName, ImplementationClass, ModuleName, implLevel) \
    zfclassNotPOD ImplementationClass : zfextendsNotPOD ZFPROTOCOL_INTERFACE_CLASS(ModuleName) \
    { \
        protected: \
            /** @brief class ref to super */ \
            typedef ZFPROTOCOL_INTERFACE_CLASS(ModuleName) zfsuper; \
            /** @brief class ref to self */ \
            typedef ImplementationClass zfself; \
        public: \
            zfoverride \
            virtual const zfchar *protocolImplementationName(void) const \
            { \
                return ZFM_TOSTRING_DIRECT(ImplementationName); \
            } \
            zfoverride \
            virtual ZFProtocolLevelEnum protocolImplementationLevel(void) const \
            { \
                return implLevel; \
            } \
        public: \
            static ZFProtocol *_ZFP_##ImplementationClass##_ctor(void) \
            { \
                return zfnew(ImplementationClass); \
            } \
            static void _ZFP_ZFProtocolUnregister(void) \
            { \
                if(ZFPROTOCOL_INTERFACE_CLASS(ModuleName)::_ZFP_ZFProtocolDataRef().implConstructor == &ImplementationClass::_ZFP_##ImplementationClass##_ctor) \
                { \
                    ZFPROTOCOL_INTERFACE_CLASS(ModuleName)::_ZFP_ZFProtocolImplementationChange(zfnull, zfnull, ZFProtocolLevel::e_Default); \
                } \
                _ZFP_ZFProtocolImplDataUnregister(zfself::_ZFP_ZFProtocolZFCoreLibDestroyFlag(), ZFM_TOSTRING_DIRECT(ModuleName)); \
            } \
            static zfbool _ZFP_ZFProtocolRegister(void) \
            { \
                ZFPROTOCOL_INTERFACE_CLASS(ModuleName)::_ZFP_ZFProtocolImplementationRegister( \
                    &ImplementationClass::_ZFP_##ImplementationClass##_ctor, \
                    ZFM_TOSTRING_DIRECT(ImplementationName), \
                    implLevel); \
                static _ZFP_ZFProtocolImplRegisterHolder _holder( \
                    zfself::_ZFP_ZFProtocolZFCoreLibDestroyFlag(), \
                    &ImplementationClass::_ZFP_ZFProtocolUnregister); \
                return zftrue; \
            } \
            static zfbool _ZFP_ZFProtocolChange(void) \
            { \
                ZFPROTOCOL_INTERFACE_CLASS(ModuleName)::_ZFP_ZFProtocolImplementationChange( \
                    &ImplementationClass::_ZFP_##ImplementationClass##_ctor, \
                    ZFM_TOSTRING_DIRECT(ImplementationName), \
                    implLevel); \
                return zftrue; \
            }
#define _ZFP_ZFPROTOCOL_IMPLEMENTATION_BEGIN(ImplementationName, ImplementationClass, ModuleName, implLevel) \
    _ZFP_ZFPROTOCOL_IMPLEMENTATION_BEGIN_(ImplementationName, ImplementationClass, ModuleName, implLevel)

#define _ZFP_ZFPROTOCOL_IMPLEMENTATION_END_(ImplementationName, ImplementationClass) \
    }; \
    ZFOUTPUT_TYPE(ImplementationClass, {output << (const ZFProtocol *)&v;}) \
    ZFOUTPUT_TYPE(const ImplementationClass *, {output << (const ZFProtocol *)v;}) \
    ZFOUTPUT_TYPE(ImplementationClass *, {output << (const ZFProtocol *)v;})
#define _ZFP_ZFPROTOCOL_IMPLEMENTATION_END(ImplementationName, ImplementationClass) \
    _ZFP_ZFPROTOCOL_IMPLEMENTATION_END_(ImplementationName, ImplementationClass)

#define _ZFP_ZFPROTOCOL_IMPLEMENTATION_REGISTER_(ImplementationName, ImplementationClass) \
    ZF_STATIC_REGISTER_INIT(ZFPImplReg_##ImplementationClass) \
    { \
        ImplementationClass::_ZFP_ZFProtocolRegister(); \
    } \
    ZF_STATIC_REGISTER_END(ZFPImplReg_##ImplementationClass)
#define _ZFP_ZFPROTOCOL_IMPLEMENTATION_REGISTER(ImplementationName, ImplementationClass) \
    _ZFP_ZFPROTOCOL_IMPLEMENTATION_REGISTER_(ImplementationName, ImplementationClass)

/**
 * @brief register platform hint for an implementation,
 *   put it between #ZFPROTOCOL_IMPLEMENTATION_BEGIN and #ZFPROTOCOL_IMPLEMENTATION_END
 *
 * while implementing protocol, you may use ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_XXX
 * to explicitly show it require other implementation\n
 * the PlatformHint can be any string,
 * but you should make sure it's unique from others,
 * and it's recommended to follow these rules (not necessary):
 * -  for common case, use\n
 *   "framework or system or environment name" + ":" + "hint"\n
 *   such as "iOS:NSString"
 * -  if you need some native wrapper to hold native types,
 *   it's recommended to supply in this format:\n
 *   "framework or system or environment name" + ":" + "wrapper name" + "_" + "hint"\n
 *   such as "iOS:NSObject_NSString"\n
 *   and in your wrapper object, supply a native method named "zfnativeImpl"
 *   to return the wrapped type,
 *   so that native code can use native code to reflect and access the internal types
 *
 * \n
 * if you are able to access an implementation,
 * you may check its platform hint by #ZFProtocol::protocolImplementationPlatformHint
 */
#define ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT(PlatformHint) \
    public: \
        virtual const zfchar *protocolImplementationPlatformHint(void) const \
        { \
            return PlatformHint; \
        }

/**
 * @brief declare a dependency
 *
 * @code
 *   ZFPROTOCOL_IMPLEMENTATION_BEGIN(...)
 *       ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
 *           ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ProtocolName, "DependencyPlatformHint")
 *       ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()
 *   ZFPROTOCOL_IMPLEMENTATION_END(...)
 * @endcode
 * the DependencyPlatformHint is the string declared by #ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT
 */
#define ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN() \
    public: \
        virtual zfbool _ZFP_ZFProtocol_checkImplDependency(ZF_OUT const zfchar *&desiredImplPlatformHint, \
                                                           ZF_OUT const zfchar *&mismatchProtocolName, \
                                                           ZF_OUT ZFProtocol *&mismatchImpl) const \
        { \
            if(!zfsuper::_ZFP_ZFProtocol_checkImplDependency(desiredImplPlatformHint, mismatchProtocolName, mismatchImpl)) \
            { \
                return zffalse; \
            }
/** @brief see #ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN */
#define ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(Protocol, DependencyPlatformHint) \
            if(ZFPROTOCOL_TRY_ACCESS(Protocol) == zfnull \
                || !zfscmpTheSame(ZFPROTOCOL_ACCESS(Protocol)->protocolImplementationPlatformHint(), DependencyPlatformHint)) \
            { \
                desiredImplPlatformHint = DependencyPlatformHint; \
                mismatchProtocolName = ZFM_TOSTRING(Protocol); \
                mismatchImpl = ZFPROTOCOL_TRY_ACCESS(Protocol); \
                return zffalse; \
            }
/** @brief see #ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN */
#define ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END() \
            return zftrue; \
        } \
    public:

/**
 * @brief the actual class name used by ZFPROTOCOL_IMPLEMENTATION_BEGIN
 *
 * usually used to declare a constructor for protocol's implementation
 */
#define ZFPROTOCOL_IMPLEMENTATION_CLASS(Implementation) \
    ZFPImpl_##Implementation

/**
 * @brief declare a implementation for specified protocol interface named ModuleName
 *
 * for what ZFProtocol is, please refer to #ZFPROTOCOL_INTERFACE_BEGIN\n
 * typical usage:
 * @code
 *   // in a *.cpp file
 *   ZFPROTOCOL_IMPLEMENTATION_BEGIN(YourImplementation, ModuleName, implLevel)
 *       public: // you can add your own init steps
 *           zfoverride
 *           virtual void protocolOnInit(void) {...}
 *           zfoverride
 *           virtual void protocolOnInitFinish(void) {...}
 *           zfoverride
 *           virtual void protocolOnDeallocPrepare(void) {...}
 *           zfoverride
 *           virtual void protocolOnDealloc(void) {...}
 *       private: // also, add other member if you like
 *           SomeType member;
 *       public: // (necessary) implement all protocols here
 *           virtual void implementedFunc(void)
 *           {...}
 *   ZFPROTOCOL_IMPLEMENTATION_END(YourImplementation)
 *   ZFPROTOCOL_IMPLEMENTATION_REGISTER(YourImplementation)
 * @endcode
 * you must have exactly one implementation for each protocol interface
 * if you want to use the specific module\n
 * to achieve this, you may want to use the \#ifdef
 * @code
 *   // in ModuleWindows.cpp
 *   #ifdef _FLAG_WINDOWS
 *       ZFPROTOCOL_IMPLEMENTATION_BEGIN(YourImplementation, ...)
 *       ...
 *       ZFPROTOCOL_IMPLEMENTATION_END(YourImplementation)
 *       ZFPROTOCOL_IMPLEMENTATION_REGISTER(YourImplementation)
 *   #endif
 *
 *   // in ModuleLinux.cpp
 *   #ifdef _FLAG_LINUX
 *       ZFPROTOCOL_IMPLEMENTATION_BEGIN(YourImplementation, ...)
 *       ...
 *       ZFPROTOCOL_IMPLEMENTATION_END(YourImplementation)
 *       ZFPROTOCOL_IMPLEMENTATION_REGISTER(YourImplementation)
 *   #endif
 * @endcode
 * the implementation's registration is automatically done by the BEGIN and END macro,
 * what you should do, is to put it in a cpp file and make it active\n
 * \n
 * the third param "implLevel", which has:
 * -  ZFProtocolLevel::e_Default
 * -  ZFProtocolLevel::e_SystemLow
 * -  ZFProtocolLevel::e_SystemNormal
 * -  ZFProtocolLevel::e_SystemHigh
 * -  ZFProtocolLevel::e_AppLow
 * -  ZFProtocolLevel::e_AppNormal
 * -  ZFProtocolLevel::e_AppHigh
 *
 * is used to specify the implementation level, ordered from low to high priority\n
 * for example, if you have a Default and a System level's implementation,
 * the Default one would be overrided and the System one would be used.\n
 * however, if you have two same level's implementations,
 * you would get duplicated implementation errors\n
 * \n
 * for how to declare a protocol, see #ZFPROTOCOL_INTERFACE_BEGIN\n
 * \n
 * advanced: \n
 * you may change implementation at runtime, see #ZFProtocolChangeImplementation
 */
#define ZFPROTOCOL_IMPLEMENTATION_BEGIN(ImplementationName, ModuleName, implLevel) \
    _ZFP_ZFPROTOCOL_IMPLEMENTATION_BEGIN(ImplementationName, ZFPROTOCOL_IMPLEMENTATION_CLASS(ImplementationName), ModuleName, implLevel)
/**
 * @brief for more information, please refer to #ZFPROTOCOL_IMPLEMENTATION_BEGIN
 */
#define ZFPROTOCOL_IMPLEMENTATION_END(ImplementationName) \
    _ZFP_ZFPROTOCOL_IMPLEMENTATION_END(ImplementationName, ZFPROTOCOL_IMPLEMENTATION_CLASS(ImplementationName))
/**
 * @brief for more information, please refer to #ZFPROTOCOL_IMPLEMENTATION_BEGIN
 */
#define ZFPROTOCOL_IMPLEMENTATION_REGISTER(ImplementationName) \
    _ZFP_ZFPROTOCOL_IMPLEMENTATION_REGISTER(ImplementationName, ZFPROTOCOL_IMPLEMENTATION_CLASS(ImplementationName))

// ============================================================
#define _ZFP_ZFProtocolChangeImplementation(ImplementationName, ImplementationClass) \
    ImplementationClass::_ZFP_ZFProtocolChange()

/**
 * @brief change the implementation at runtime
 *
 * old implementation's level is ignored and forced to be replaced\n
 * it's your responsibility to make sure old implementation isn't in use,
 * otherwise, app may crash\n
 * usually, you should change before #ZFFrameworkInit
 */
#define ZFProtocolChangeImplementation(ImplementationName) \
    _ZFP_ZFProtocolChangeImplementation(ImplementationName, ZFPROTOCOL_IMPLEMENTATION_CLASS(ImplementationName))

// ============================================================
/**
 * @brief get protocol with name or null if no such protocol
 *
 * first param is the protocol's name (e.g. "ZFXml")\n
 * second param desiredImpl can be assigned to check whether the protocol matches what you want,
 * see #ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT
 */
extern ZF_ENV_EXPORT ZFProtocol *ZFProtocolForName(ZF_IN const zfchar *name,
                                                   ZF_IN_OPT const zfchar *desiredImpl = zfnull);
/**
 * @brief check whether protocol is available
 *
 * first param is the protocol's name (e.g. "ZFXml")\n
 * second param desiredImpl can be assigned to check whether the protocol matches what you want,
 * see #ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT
 */
ZFMETHOD_FUNC_DECLARE_2(zfbool, ZFProtocolIsAvailable,
                        ZFMP_IN(const zfchar *, name),
                        ZFMP_IN_OPT(const zfchar *, desiredImpl, zfnull))

// ============================================================
/**
 * @brief see #ZFProtocolImplInfoGetAll
 */
zfclassLikePOD ZF_ENV_EXPORT ZFProtocolImplInfo
{
public:
    /**
     * @brief protocol's name
     */
    zfstring protocolName;
    /**
     * @brief whether protocol is optional
     */
    zfbool protocolOptional;
    /**
     * @brief protocol's implementation, or null if currently not implemented
     */
    ZFProtocol *protocolImpl;
};

/**
 * @brief for debug use only, get all protocol's info
 */
extern ZF_ENV_EXPORT ZFCoreArray<ZFProtocolImplInfo> ZFProtocolImplInfoGetAll(void);
/**
 * @brief util method to #ZFProtocolImplInfoGetAll
 */
extern ZF_ENV_EXPORT ZFCoreArray<ZFProtocolImplInfo> ZFProtocolImplInfoGetAllImplemented(void);
/**
 * @brief util method to #ZFProtocolImplInfoGetAll
 */
extern ZF_ENV_EXPORT ZFCoreArray<ZFProtocolImplInfo> ZFProtocolImplInfoGetAllNotImplemented(void);
/**
 * @brief util method to #ZFProtocolImplInfoGetAll
 */
ZFMETHOD_FUNC_DECLARE_1(void, ZFProtocolImplInfoPrint,
                        ZFMP_IN_OPT(const ZFOutput &, callback, ZFOutputDefault()))
/**
 * @brief util method to #ZFProtocolImplInfoGetAll
 */
extern ZF_ENV_EXPORT void ZFProtocolImplInfoPrint(ZF_IN const ZFProtocolImplInfo &data,
                                                  ZF_IN_OPT const ZFOutput &callback = ZFOutputDefault());

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocol_h_

