/**
 * @file ZFObjectDeclare.h
 * @brief ZFObject declaration
 */

#ifndef _ZFI_ZFObjectDeclare_h_
#define _ZFI_ZFObjectDeclare_h_

#include "ZFAny.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
template<typename T_zfsuper, typename T_superInterface, int superImplementsInterface>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_zfsuperI
{
public:
    typedef T_superInterface SuperType;
};
template<typename T_zfsuper, typename T_superInterface>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_zfsuperI<T_zfsuper, T_superInterface, 1>
{
public:
    typedef T_zfsuper SuperType;
};
/**
 * @brief class ref to proper super type, see #ZFObject for more info
 */
#define zfsuperI(T_SuperType) \
    _ZFP_zfsuperI<zfsuper, T_SuperType, zftTypeIsTypeOf<zfsuper, T_SuperType>::TypeIsTypeOf>::SuperType

// ============================================================
#define _ZFP_ZFOBJECT_DECLARE_PROTECTED_CONSTRUCTOR(ChildClass, SuperClass) \
    protected: \
        /** @cond ZFPrivateDoc */ \
        ChildClass(void) : SuperClass() {} \
        virtual ~ChildClass(void) {} \
        /** @endcond */ \
    public:
#define _ZFP_ZFOBJECT_DECLARE(ChildClass, SuperClass) \
    ZFCLASS_DISALLOW_COPY_CONSTRUCTOR(ChildClass) \
    public: \
        static void _ZFP_zftIsZFObject(void) {} \
    protected: \
        /** @brief class ref to super */ \
        typedef SuperClass zfsuper; \
        /** @brief class ref to self */ \
        typedef ChildClass zfself; \
    public: \
        /** @cond ZFPrivateDoc */ \
        template<typename T_ZFObject> \
        inline T_ZFObject to(void) \
        { \
            return ZFCastZFObjectUnchecked(T_ZFObject, this); \
        } \
        virtual inline ZFObject *toObject(void) \
        { \
            return this; \
        } \
        inline ZFAny toAny(void) \
        { \
            return ZFAny(this); \
        } \
        /** @endcond */ \
    public: \
        /** @brief get instance's class info */ \
        virtual const ZFClass *classData(void) \
        { \
            if(this->_ZFP_ZFObject_classData) \
            { \
                return this->_ZFP_ZFObject_classData; \
            } \
            else \
            { \
                return zfself::ClassData(); \
            } \
        } \
    private: \
        static void _ZFP_Obj_initImplCk(ZF_IN ZFClass *cls) \
        { \
            if(cls->_ZFP_ZFClassNeedInitImplementationList) \
            { \
                cls->_ZFP_ZFClassNeedInitImplementationList = zffalse; \
                if(zfself::_ZFP_Obj_initImpl != zfsuper::_ZFP_Obj_initImpl) \
                { \
                    zfself::_ZFP_Obj_initImpl(cls); \
                } \
            } \
        } \
    public:
#define _ZFP_ZFOBJECT_DECLARE_OBJECT(ChildClass, SuperClass) \
    public: \
        enum {_ZFP_ZFObjectCanAlloc = 1}; \
    public: \
        zfpoolDeclareFriend() \
        friend zfclassFwd _ZFP_Obj_AllocCk<zfself, 0>; \
        friend zfclassFwd _ZFP_Obj_AllocCk<zfself, 1>; \
        static ZFObject *_ZFP_Obj_ctor(void) \
        { \
            return zfpoolNew(zfself); \
        } \
        static void _ZFP_Obj_dtor(ZF_IN ZFObject *obj) \
        { \
            zfpoolDelete(_ZFP_ObjCastInternal(zfself *, obj)); \
        } \
        /** @brief get class info */ \
        static const ZFClass *ClassData(void) \
        { \
            static _ZFP_ZFClassRegisterHolder _holder( \
                    ZF_NAMESPACE_CURRENT(), \
                    ZFM_TOSTRING_DIRECT(ChildClass), \
                    zfsuper::ClassData(), \
                    (&zfself::_ZFP_zfAllocWithCache == &zfsuper::_ZFP_zfAllocWithCache \
                        || &zfself::_ZFP_zfAllocWithCache == &ZFObject::_ZFP_zfAllocWithCache) \
                            ? zfnull \
                            : &zfself::_ZFP_zfAllocWithCache, \
                    &zfself::_ZFP_Obj_ctor, \
                    &zfself::_ZFP_Obj_dtor, \
                    &zfself::_ZFP_Obj_initImplCk \
                ); \
            return _holder.cls; \
        }
#define _ZFP_ZFOBJECT_DECLARE_ABSTRACT(ChildClass, SuperClass) \
    public: \
        typedef enum {_ZFP_ZFObjectCanAlloc = 0} _ZFP_ZFObjectCanAllocChecker; \
    public: \
        /** @brief get class info */ \
        static const ZFClass *ClassData(void) \
        { \
            static _ZFP_ZFClassRegisterHolder _holder( \
                    ZF_NAMESPACE_CURRENT(), \
                    ZFM_TOSTRING_DIRECT(ChildClass), \
                    zfsuper::ClassData(), \
                    zfnull, \
                    zfnull, \
                    zfnull, \
                    &zfself::_ZFP_Obj_initImplCk \
                ); \
            return _holder.cls; \
        }
/**
 * @brief necessary for every class inherit from ZFObject
 *
 * for more information, please refer to #ZFObject
 * @see ZFObject, ZFOBJECT_REGISTER, ZFOBJECT_DECLARE_ABSTRACT
 */
#define ZFOBJECT_DECLARE(ChildClass, SuperClass) \
    _ZFP_ZFOBJECT_DECLARE(ChildClass, SuperClass) \
    _ZFP_ZFOBJECT_DECLARE_OBJECT(ChildClass, SuperClass) \
    _ZFP_ZFOBJECT_DECLARE_PROTECTED_CONSTRUCTOR(ChildClass, SuperClass) \
    public:
/**
 * @brief declare object which allow custom constructor
 *
 * by default, ZFObject declare constructor as protected
 * so that app can't construct ZFObject on stack,
 * you may use this macro instead of #ZFOBJECT_DECLARE
 * to allow user defined constructor\n
 * \n
 * NOTE: never abuse this macro,
 * and strongly recommended not to allow ZFObject create on stack (i.e. public constructors)
 */
#define ZFOBJECT_DECLARE_WITH_CUSTOM_CTOR(ChildClass, SuperClass) \
    _ZFP_ZFOBJECT_DECLARE(ChildClass, SuperClass) \
    _ZFP_ZFOBJECT_DECLARE_OBJECT(ChildClass, SuperClass) \
    public:
/**
 * @brief necessary for every abstract class inherit from ZFObject
 *
 * for more information, please refer to #ZFObject
 * @see ZFObject, ZFOBJECT_REGISTER, ZFOBJECT_DECLARE
 */
#define ZFOBJECT_DECLARE_ABSTRACT(ChildClass, SuperClass) \
    _ZFP_ZFOBJECT_DECLARE(ChildClass, SuperClass) \
    _ZFP_ZFOBJECT_DECLARE_ABSTRACT(ChildClass, SuperClass) \
    _ZFP_ZFOBJECT_DECLARE_PROTECTED_CONSTRUCTOR(ChildClass, SuperClass) \
    public:
/**
 * @brief see #ZFOBJECT_DECLARE_WITH_CUSTOM_CTOR
 */
#define ZFOBJECT_DECLARE_ABSTRACT_WITH_CUSTOM_CTOR(ChildClass, SuperClass) \
    _ZFP_ZFOBJECT_DECLARE(ChildClass, SuperClass) \
    _ZFP_ZFOBJECT_DECLARE_ABSTRACT(ChildClass, SuperClass) \
    public:

/**
 * @brief register ZFObject to ZFClass map
 *
 * usage:
 * @code
 *   // in something.h:
 *   zfclass YourClass : zfextends BaseClass
 *   {
 *       ZFOBJECT_DECLARE(YourClass, BaseClass)
 *   };
 *
 *   // in something.cpp:
 *   #include "something.h"
 *   ZFOBJECT_REGISTER(YourClass)
 * @endcode
 * you only need this if you want to use reflection,
 * such as ZFClass::classForName\n
 * \n
 * detailed:\n
 * ZFClass map is set up when YourClass::ClassData is called, which may occurred when:
 * -  YourClass::ClassData or subclass's ClassData, is explicitly called
 * -  ever created a instance of YourClass or subclass
 * -  ZFOBJECT_REGISTER(YourClass) is added to a certain h or cpp file
 *
 * this macro actually do only one thing:
 * declare a static global variable to make sure YourClass::ClassData is called\n
 * so if you add it to cpp file, only one dummy global variable is defined\n
 * on the other hand, if you add it to h file, multiple dummy variables are defined,
 * as more as the cpp files which include the h file
 * @see ZFOBJECT_DECLARE, ZFClass
 *
 * \n
 * \n
 * all #ZFMethod and #ZFProperty would also be registered automatically
 * when it's owner class registered,
 * for only one exception:
 * -  the method or property's owner class is abstract and no other class inherit the class,
 *   typically this has no other side effect,
 *   since you can't alloc abstract class by reflection
 */
#define ZFOBJECT_REGISTER(T_ZFObject) \
    ZF_STATIC_REGISTER_INIT(ObjR_##T_ZFObject) \
    { \
        (void)T_ZFObject::ClassData()->_ZFP_ZFClass_methodAndPropertyAutoRegister(); \
    } \
    ZF_STATIC_REGISTER_END(ObjR_##T_ZFObject)

/**
 * @brief mark this object can not be allocated directly
 *
 * usage:
 * @code
 *   zfclass MyClass : zfextends ZFObject
 *   {
 *       ZFOBJECT_DECLARE(MyClass, ZFObject)
 *       ZFCLASS_PRIVATE_ALLOC("hint about why it's private")
 *   };
 * @endcode
 *
 * when declared, the object can not be allocated by #zfAlloc series directly,
 * and can only be allocated by reflection (#ZFClass::newInstance)
 */
#define ZFCLASS_PRIVATE_ALLOC(...) \
    public: \
        enum {_ZFP_ZFObjectCanAllocPublic = 0}; \
    protected:

// ============================================================
/** @brief util macro for subclass to declare #ZFObject::objectOnInit as #ZFMethod */
#define ZFOBJECT_ON_INIT_INLINE_1( \
      ZFMP_0 \
    ) \
    ZFMETHOD_INLINE_DETAIL_1( \
        protected, ZFMethodTypeVirtual, ZF_CALLER_LINE, \
        void, objectOnInit \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        )
/** @brief see #ZFOBJECT_ON_INIT_INLINE_1 */
#define ZFOBJECT_ON_INIT_DECLARE_1( \
      ZFMP_0 \
    ) \
    ZFMETHOD_DECLARE_DETAIL_1( \
        protected, ZFMethodTypeVirtual, ZF_CALLER_LINE, \
        void, objectOnInit \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        )
/** @brief see #ZFOBJECT_ON_INIT_INLINE_1 */
#define ZFOBJECT_ON_INIT_DEFINE_1(OwnerClass \
    , ZFMP_0 \
    ) \
    ZFMETHOD_DEFINE_1(OwnerClass, void, objectOnInit \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        )
/** @brief see #ZFOBJECT_ON_INIT_INLINE_1 */
#define ZFOBJECT_ON_INIT_INLINE_2( \
      ZFMP_0 \
    , ZFMP_1 \
    ) \
    ZFMETHOD_INLINE_DETAIL_2( \
        protected, ZFMethodTypeVirtual, ZF_CALLER_LINE, \
        void, objectOnInit \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        )
/** @brief see #ZFOBJECT_ON_INIT_INLINE_1 */
#define ZFOBJECT_ON_INIT_DECLARE_2( \
      ZFMP_0 \
    , ZFMP_1 \
    ) \
    ZFMETHOD_DECLARE_DETAIL_2( \
        protected, ZFMethodTypeVirtual, ZF_CALLER_LINE, \
        void, objectOnInit \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        )
/** @brief see #ZFOBJECT_ON_INIT_INLINE_1 */
#define ZFOBJECT_ON_INIT_DEFINE_2(OwnerClass \
    , ZFMP_0 \
    , ZFMP_1 \
    ) \
    ZFMETHOD_DEFINE_2(OwnerClass, void, objectOnInit \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        )
/** @brief see #ZFOBJECT_ON_INIT_INLINE_1 */
#define ZFOBJECT_ON_INIT_INLINE_3( \
      ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    ) \
    ZFMETHOD_INLINE_DETAIL_3( \
        protected, ZFMethodTypeVirtual, ZF_CALLER_LINE, \
        void, objectOnInit \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        )
/** @brief see #ZFOBJECT_ON_INIT_INLINE_1 */
#define ZFOBJECT_ON_INIT_DECLARE_3( \
      ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    ) \
    ZFMETHOD_DECLARE_DETAIL_3( \
        protected, ZFMethodTypeVirtual, ZF_CALLER_LINE, \
        void, objectOnInit \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        )
/** @brief see #ZFOBJECT_ON_INIT_INLINE_1 */
#define ZFOBJECT_ON_INIT_DEFINE_3(OwnerClass \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    ) \
    ZFMETHOD_DEFINE_3(OwnerClass, void, objectOnInit \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        )
/** @brief see #ZFOBJECT_ON_INIT_INLINE_1 */
#define ZFOBJECT_ON_INIT_INLINE_4( \
      ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    ) \
    ZFMETHOD_INLINE_DETAIL_4( \
        protected, ZFMethodTypeVirtual, ZF_CALLER_LINE, \
        void, objectOnInit \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        )
/** @brief see #ZFOBJECT_ON_INIT_INLINE_1 */
#define ZFOBJECT_ON_INIT_DECLARE_4( \
      ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    ) \
    ZFMETHOD_DECLARE_DETAIL_4( \
        protected, ZFMethodTypeVirtual, ZF_CALLER_LINE, \
        void, objectOnInit \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        )
/** @brief see #ZFOBJECT_ON_INIT_INLINE_1 */
#define ZFOBJECT_ON_INIT_DEFINE_4(OwnerClass \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    ) \
    ZFMETHOD_DEFINE_4(OwnerClass, void, objectOnInit \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        )
/** @brief see #ZFOBJECT_ON_INIT_INLINE_1 */
#define ZFOBJECT_ON_INIT_INLINE_5( \
      ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    ) \
    ZFMETHOD_INLINE_DETAIL_5( \
        protected, ZFMethodTypeVirtual, ZF_CALLER_LINE, \
        void, objectOnInit \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        )
/** @brief see #ZFOBJECT_ON_INIT_INLINE_1 */
#define ZFOBJECT_ON_INIT_DECLARE_5( \
      ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    ) \
    ZFMETHOD_DECLARE_DETAIL_5( \
        protected, ZFMethodTypeVirtual, ZF_CALLER_LINE, \
        void, objectOnInit \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        )
/** @brief see #ZFOBJECT_ON_INIT_INLINE_1 */
#define ZFOBJECT_ON_INIT_DEFINE_5(OwnerClass \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    ) \
    ZFMETHOD_DEFINE_5(OwnerClass, void, objectOnInit \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        )
/** @brief see #ZFOBJECT_ON_INIT_INLINE_1 */
#define ZFOBJECT_ON_INIT_INLINE_6( \
      ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    ) \
    ZFMETHOD_INLINE_DETAIL_6( \
        protected, ZFMethodTypeVirtual, ZF_CALLER_LINE, \
        void, objectOnInit \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        )
/** @brief see #ZFOBJECT_ON_INIT_INLINE_1 */
#define ZFOBJECT_ON_INIT_DECLARE_6( \
      ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    ) \
    ZFMETHOD_DECLARE_DETAIL_6( \
        protected, ZFMethodTypeVirtual, ZF_CALLER_LINE, \
        void, objectOnInit \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        )
/** @brief see #ZFOBJECT_ON_INIT_INLINE_1 */
#define ZFOBJECT_ON_INIT_DEFINE_6(OwnerClass \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    ) \
    ZFMETHOD_DEFINE_6(OwnerClass, void, objectOnInit \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        )
/** @brief see #ZFOBJECT_ON_INIT_INLINE_1 */
#define ZFOBJECT_ON_INIT_INLINE_7( \
      ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    ) \
    ZFMETHOD_INLINE_DETAIL_7( \
        protected, ZFMethodTypeVirtual, ZF_CALLER_LINE, \
        void, objectOnInit \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        , _ZFP_MtdP_EXPAND(ZFMP_6) \
        )
/** @brief see #ZFOBJECT_ON_INIT_INLINE_1 */
#define ZFOBJECT_ON_INIT_DECLARE_7( \
      ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    ) \
    ZFMETHOD_DECLARE_DETAIL_7( \
        protected, ZFMethodTypeVirtual, ZF_CALLER_LINE, \
        void, objectOnInit \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        , _ZFP_MtdP_EXPAND(ZFMP_6) \
        )
/** @brief see #ZFOBJECT_ON_INIT_INLINE_1 */
#define ZFOBJECT_ON_INIT_DEFINE_7(OwnerClass \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    ) \
    ZFMETHOD_DEFINE_7(OwnerClass, void, objectOnInit \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        , _ZFP_MtdP_EXPAND(ZFMP_6) \
        )
/** @brief see #ZFOBJECT_ON_INIT_INLINE_1 */
#define ZFOBJECT_ON_INIT_INLINE_8( \
      ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    , ZFMP_7 \
    ) \
    ZFMETHOD_INLINE_DETAIL_8( \
        protected, ZFMethodTypeVirtual, ZF_CALLER_LINE, \
        void, objectOnInit \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        , _ZFP_MtdP_EXPAND(ZFMP_6) \
        , _ZFP_MtdP_EXPAND(ZFMP_7) \
        )
/** @brief see #ZFOBJECT_ON_INIT_INLINE_1 */
#define ZFOBJECT_ON_INIT_DECLARE_8( \
      ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    , ZFMP_7 \
    ) \
    ZFMETHOD_DECLARE_DETAIL_8( \
        protected, ZFMethodTypeVirtual, ZF_CALLER_LINE, \
        void, objectOnInit \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        , _ZFP_MtdP_EXPAND(ZFMP_6) \
        , _ZFP_MtdP_EXPAND(ZFMP_7) \
        )
/** @brief see #ZFOBJECT_ON_INIT_INLINE_1 */
#define ZFOBJECT_ON_INIT_DEFINE_8(OwnerClass \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    , ZFMP_7 \
    ) \
    ZFMETHOD_DEFINE_8(OwnerClass, void, objectOnInit \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        , _ZFP_MtdP_EXPAND(ZFMP_6) \
        , _ZFP_MtdP_EXPAND(ZFMP_7) \
        )

// ============================================================
/** @brief util macro to declare #ZFObject::objectOnInit as #ZFMETHOD_USER_REGISTER_1 */
#define ZFOBJECT_ON_INIT_USER_REGISTER_1(methodInvokerAction, ownerClassSig \
    , ZFMP_0 \
    ) \
    ZFMETHOD_USER_REGISTER_DETAIL_1( \
        {invokerObject->_ZFP_ZFObject_objectOnInit(); methodInvokerAction}, ownerClassSig, \
        protected, ZFMethodTypeVirtual, ZF_CALLER_LINE, \
        void, objectOnInit \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        )
/** @brief see #ZFOBJECT_ON_INIT_USER_REGISTER_1 */
#define ZFOBJECT_ON_INIT_USER_REGISTER_2(methodInvokerAction, ownerClassSig \
    , ZFMP_0 \
    , ZFMP_1 \
    ) \
    ZFMETHOD_USER_REGISTER_DETAIL_2( \
        {invokerObject->_ZFP_ZFObject_objectOnInit(); methodInvokerAction}, ownerClassSig, \
        protected, ZFMethodTypeVirtual, ZF_CALLER_LINE, \
        void, objectOnInit \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        )
/** @brief see #ZFOBJECT_ON_INIT_USER_REGISTER_1 */
#define ZFOBJECT_ON_INIT_USER_REGISTER_3(methodInvokerAction, ownerClassSig \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    ) \
    ZFMETHOD_USER_REGISTER_DETAIL_3( \
        {invokerObject->_ZFP_ZFObject_objectOnInit(); methodInvokerAction}, ownerClassSig, \
        protected, ZFMethodTypeVirtual, ZF_CALLER_LINE, \
        void, objectOnInit \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        )
/** @brief see #ZFOBJECT_ON_INIT_USER_REGISTER_1 */
#define ZFOBJECT_ON_INIT_USER_REGISTER_4(methodInvokerAction, ownerClassSig \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    ) \
    ZFMETHOD_USER_REGISTER_DETAIL_4( \
        {invokerObject->_ZFP_ZFObject_objectOnInit(); methodInvokerAction}, ownerClassSig, \
        protected, ZFMethodTypeVirtual, ZF_CALLER_LINE, \
        void, objectOnInit \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        )
/** @brief see #ZFOBJECT_ON_INIT_USER_REGISTER_1 */
#define ZFOBJECT_ON_INIT_USER_REGISTER_5(methodInvokerAction, ownerClassSig \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    ) \
    ZFMETHOD_USER_REGISTER_DETAIL_5( \
        {invokerObject->_ZFP_ZFObject_objectOnInit(); methodInvokerAction}, ownerClassSig, \
        protected, ZFMethodTypeVirtual, ZF_CALLER_LINE, \
        void, objectOnInit \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        )
/** @brief see #ZFOBJECT_ON_INIT_USER_REGISTER_1 */
#define ZFOBJECT_ON_INIT_USER_REGISTER_6(methodInvokerAction, ownerClassSig \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    ) \
    ZFMETHOD_USER_REGISTER_DETAIL_6( \
        {invokerObject->_ZFP_ZFObject_objectOnInit(); methodInvokerAction}, ownerClassSig, \
        protected, ZFMethodTypeVirtual, ZF_CALLER_LINE, \
        void, objectOnInit \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        )
/** @brief see #ZFOBJECT_ON_INIT_USER_REGISTER_1 */
#define ZFOBJECT_ON_INIT_USER_REGISTER_7(methodInvokerAction, ownerClassSig \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    ) \
    ZFMETHOD_USER_REGISTER_DETAIL_7( \
        {invokerObject->_ZFP_ZFObject_objectOnInit(); methodInvokerAction}, ownerClassSig, \
        protected, ZFMethodTypeVirtual, ZF_CALLER_LINE, \
        void, objectOnInit \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        , _ZFP_MtdP_EXPAND(ZFMP_6) \
        )
/** @brief see #ZFOBJECT_ON_INIT_USER_REGISTER_1 */
#define ZFOBJECT_ON_INIT_USER_REGISTER_8(methodInvokerAction, ownerClassSig \
    , ZFMP_0 \
    , ZFMP_1 \
    , ZFMP_2 \
    , ZFMP_3 \
    , ZFMP_4 \
    , ZFMP_5 \
    , ZFMP_6 \
    , ZFMP_7 \
    ) \
    ZFMETHOD_USER_REGISTER_DETAIL_8( \
        {invokerObject->_ZFP_ZFObject_objectOnInit(); methodInvokerAction}, ownerClassSig, \
        protected, ZFMethodTypeVirtual, ZF_CALLER_LINE, \
        void, objectOnInit \
        , _ZFP_MtdP_EXPAND(ZFMP_0) \
        , _ZFP_MtdP_EXPAND(ZFMP_1) \
        , _ZFP_MtdP_EXPAND(ZFMP_2) \
        , _ZFP_MtdP_EXPAND(ZFMP_3) \
        , _ZFP_MtdP_EXPAND(ZFMP_4) \
        , _ZFP_MtdP_EXPAND(ZFMP_5) \
        , _ZFP_MtdP_EXPAND(ZFMP_6) \
        , _ZFP_MtdP_EXPAND(ZFMP_7) \
        )

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFObjectDeclare_h_

