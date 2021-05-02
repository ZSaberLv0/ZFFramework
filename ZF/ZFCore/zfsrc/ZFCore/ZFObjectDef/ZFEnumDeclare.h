/**
 * @file ZFEnumDeclare.h
 * @brief ZFEnum declare impl
 */

#ifndef _ZFI_ZFEnumDeclare_h_
#define _ZFI_ZFEnumDeclare_h_

#include "ZFEnumDeclarePropType.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief macros to define a key-value map for enum type
 *
 * usage:
 * @code
 *   // ============================================================
 *   // in h file
 *  / **
 *    * you can add Doxygen docs for EnumName (as a ZFObject) here
 *    * /
 *   ZFENUM_BEGIN(EnumName)
 *      / **
 *        * you can add Doxygen docs for Value1 here
 *        * /
 *       ZFENUM_VALUE(Value1)
 *       ZFENUM_VALUE(Value2) // /< you can add Doxygen docs for Value2 here
 *       ZFENUM_VALUE_WITH_INIT(Value3, 33) // you can assign the value for enum item
 *       ZFENUM_VALUE_WITH_INIT(Value4, e_Value2) // you can assign a same value with old value
 *   ZFENUM_SEPARATOR(EnumName)
 *       // you must use ZFENUM_VALUE_REGISTER to map the value and name
 *       ZFENUM_VALUE_REGISTER(Value1)
 *       ZFENUM_VALUE_REGISTER(Value2)
 *       // you can set a custom name by ZFENUM_VALUE_REGISTER_WITH_NAME
 *       ZFENUM_VALUE_REGISTER_WITH_NAME(Value3, "CustomNameValue3")
 *       // (here Value4 is equal to Value2)
 *       // when register a new name for a existing value,
 *       // old name would be overrided,
 *       // as well as the value-name map
 *       ZFENUM_VALUE_REGISTER_WITH_NAME(Value4, "Value4 override Value2")
 *   ZFENUM_END(EnumName)
 *
 *   // ============================================================
 *   // in cpp file
 *   // add this macro for necessary code expansion
 *   ZFENUM_DEFINE(EnumName)
 * @endcode
 * @note if there are more than one enum with same value,
 *   only the last registered one would be mapped
 * once defined, you can:
 * -  use EnumName to access the enum value and name
 *   @code
 *     zfuint value;
 *     const zfchar *name;
 *     value = EnumName::e_Value1;
 *     value = EnumName::EnumValueForName("Value1");
 *     name = EnumName::EnumNameForValue(value);
 *   @endcode
 * -  use EnumName to store the enum value as a ZFObject
 *   @code
 *     EnumName *e = zfAlloc(EnumName());
 *     e->enumValue(EnumName::e_Value1);
 *     zfuint value = e->enumValue();
 *     const zfchar *name = e->enumName();
 *     zfRelease(e);
 *   @endcode
 * -  use base class ZFEnum to achieve dynamic binding
 *   @code
 *     zfuint value;
 *     const zfchar *name;
 *
 *     ZFEnum *e = zfAlloc(EnumName, EnumName::e_Value1);
 *     value = e->enumValue(); // return the value stored as EnumName
 *     zfRelease(e);
 *
 *     zfautoObject tmp = ZFClass::classForName("EnumName")->newInstance(); // see #ZFOBJECT_REGISTER for more info
 *     e = tmp.to<ZFEnum *>();
 *     for(zfindex i = 0; i < e->enumCount(); ++i)
 *     { // OK, list all the value and name for e, which is EnumName type
 *         value = e->enumValueAtIndex(i);
 *         name = e->enumNameAtIndex(i);
 *     }
 *     value = e->enumValueForName("Value1"); // OK, value from EnumName
 *     name = e->enumNameForValue(value); // OK, name from EnumName
 *     zfRelease(e);
 *   @endcode
 * -  you can access the internal enum type by EnumNameEnum
 * -  usually, it's highly recommended to use the internal enum type for performance:
 *   @code
 *     // pass by EnumNameEnum value, which is actually a int value
 *     void func1(MyEnumEnum e) {...}
 *   @endcode
 */
#define ZFENUM_BEGIN(EnumName) \
    _ZFP_ZFENUM_BEGIN(EnumName)

/** @brief see #ZFENUM_BEGIN */
#define ZFENUM_VALUE(Value) \
    _ZFP_ZFENUM_VALUE(Value)
/** @brief see #ZFENUM_BEGIN */
#define ZFENUM_VALUE_WITH_INIT(Value, initValue) \
    _ZFP_ZFENUM_VALUE_WITH_INIT(Value, initValue)

/** @brief see #ZFENUM_BEGIN */
#define ZFENUM_SEPARATOR(EnumName) \
    _ZFP_ZFENUM_SEPARATOR(EnumName, zffalse)
/** @brief see #ZFENUM_BEGIN */
#define ZFENUM_SEPARATOR_ALLOW_DUPLICATE_VALUE(EnumName) \
    _ZFP_ZFENUM_SEPARATOR(EnumName, zftrue)

/** @brief see #ZFENUM_BEGIN */
#define ZFENUM_VALUE_REGISTER_WITH_NAME(Value, Name) \
    _ZFP_ZFENUM_VALUE_REGISTER_WITH_NAME(Value, Name)
/** @brief see #ZFENUM_BEGIN */
#define ZFENUM_VALUE_REGISTER(Value) \
    _ZFP_ZFENUM_VALUE_REGISTER_WITH_NAME(Value, ZFM_TOSTRING_DIRECT(Value))

/** @brief see #ZFENUM_BEGIN */
#define ZFENUM_END(EnumName) \
    _ZFP_ZFENUM_END(EnumName)
/** @brief see #ZFENUM_BEGIN */
#define ZFENUM_END_WITH_DEFAULT(EnumName, defaultEnum) \
    _ZFP_ZFENUM_END_WITH_DEFAULT(EnumName, defaultEnum)

/** @brief see #ZFENUM_BEGIN */
#define ZFENUM_END_FLAGS(EnumName, EnumFlagsName) \
    _ZFP_ZFENUM_END_FLAGS(EnumName, EnumFlagsName)
/** @brief see #ZFENUM_BEGIN */
#define ZFENUM_END_FLAGS_WITH_DEFAULT(EnumName, EnumFlagsName, defaultEnum, defaultEnumFlags) \
    _ZFP_ZFENUM_END_FLAGS_WITH_DEFAULT(EnumName, EnumFlagsName, defaultEnum, defaultEnumFlags)

/** @brief see #ZFENUM_BEGIN */
#define ZFENUM_DEFINE(EnumName) \
    _ZFP_ZFENUM_DEFINE(EnumName)

/** @brief see #ZFENUM_BEGIN */
#define ZFENUM_DEFINE_FLAGS(EnumName, EnumFlagsName) \
    _ZFP_ZFENUM_DEFINE_FLAGS(EnumName, EnumFlagsName)

// ============================================================
zfclassFwd _ZFP_ZFEnumDataPrivate;
zfclassNotPOD ZF_ENV_EXPORT _ZFP_ZFEnumData
{
public:
    _ZFP_ZFEnumData(void);
    ~_ZFP_ZFEnumData(void);
public:
    zfbool needInitFlag;
    const ZFClass *ownerClass;
    zfuint enumDefault;
    zfbool enumIsFlags;
    void add(ZF_IN zfbool isEnableDuplicateValue,
             ZF_IN zfuint value,
             ZF_IN const zfchar *name);
    zfindex enumCount(void) const;
    zfindex enumIndexForValue(ZF_IN zfuint value) const;
    zfuint enumValueAtIndex(ZF_IN zfindex index) const;
    const zfchar *enumNameAtIndex(ZF_IN zfindex index) const;
    zfbool enumContainValue(ZF_IN zfuint value) const;
    zfuint enumValueForName(ZF_IN const zfchar *name) const;
    const zfchar *enumNameForValue(ZF_IN zfuint value) const;
private:
    _ZFP_ZFEnumDataPrivate *d;
};
extern ZF_ENV_EXPORT _ZFP_ZFEnumData *_ZFP_ZFEnumDataAccess(ZF_IN const ZFClass *ownerClass);
extern ZF_ENV_EXPORT void _ZFP_ZFEnumDataCleanup(ZF_IN const ZFClass *ownerClass);
extern ZF_ENV_EXPORT void _ZFP_ZFEnumDataCleanup(ZF_IN const _ZFP_ZFEnumData *d);
zfclassNotPOD _ZFP_ZFEnumDataHolder
{
public:
    _ZFP_ZFEnumDataHolder(ZF_IN const _ZFP_ZFEnumData *d)
    : d(d)
    {
    }
    _ZFP_ZFEnumDataHolder(void)
    {
        _ZFP_ZFEnumDataCleanup(d);
    }
public:
    const _ZFP_ZFEnumData *d;
};

// ============================================================
#define _ZFP_ZFENUM_BEGIN(EnumName) \
    zfclass ZF_ENV_EXPORT EnumName : zfextends ZFEnum \
    { \
        ZFOBJECT_DECLARE(EnumName, ZFEnum) \
    public: \
        /** @brief see @ref EnumName */ \
        typedef enum \
        {

#define _ZFP_ZFENUM_VALUE(Value) /** \n */ e_##Value,
#define _ZFP_ZFENUM_VALUE_WITH_INIT(Value, initValue) /** @brief \n Value(initValue)\n */ e_##Value = initValue,

#define _ZFP_ZFENUM_SEPARATOR(EnumName, isEnableDuplicateValue_) \
            /** @brief max enum value */ \
            ZFEnumCount, \
            /* used to ensure sizeof(enum) == sizeof(zfuint) */ \
            /* required for enum value reinterpret cast (ZFTAG_TRICKS: EnumReinterpretCast) */ \
            _ZFP_ZFEnumMax = ((zfuint)-1), \
        } ZFEnumType; \
    public: \
        zfoverride \
        virtual zfidentity objectHash(void) \
        { \
            return zfidentityHash( \
                zfidentityCalcString(zfself::ClassData()->classNameFull()), \
                zfidentityCalcPOD(this->enumValue())); \
        } \
    public: \
        /** @brief get the count of enum value */ \
        static zfindex EnumCount(void) \
        { \
            return zfself::_ZFP_ZFEnumDataRef()->enumCount(); \
        } \
        /** @brief get the index of enum value, or zfindexMax() if no such enum value */ \
        static zfindex EnumIndexForValue(ZF_IN zfuint value) \
        { \
            return zfself::_ZFP_ZFEnumDataRef()->enumIndexForValue(value); \
        } \
        /** @brief get the value at index, or ZFEnumInvalid if not exist */ \
        static zfuint EnumValueAtIndex(ZF_IN zfindex index) \
        { \
            return zfself::_ZFP_ZFEnumDataRef()->enumValueAtIndex(index); \
        } \
        /** @brief get the name at index, or ZFEnumNameInvalid if not exist */ \
        static const zfchar *EnumNameAtIndex(ZF_IN zfindex index) \
        { \
            return zfself::_ZFP_ZFEnumDataRef()->enumNameAtIndex(index); \
        } \
        /** @brief return true if contain the specified value */ \
        static zfbool EnumContainValue(ZF_IN zfuint value) \
        { \
            return zfself::_ZFP_ZFEnumDataRef()->enumContainValue(value); \
        } \
        /** @brief get value with name, or ZFEnumInvalid if not exist */ \
        static zfuint EnumValueForName(ZF_IN const zfchar *name) \
        { \
            return zfself::_ZFP_ZFEnumDataRef()->enumValueForName(name); \
        } \
        /** @brief get name with value, or ZFEnumNameInvalid if not exist */ \
        static const zfchar *EnumNameForValue(ZF_IN zfuint value) \
        { \
            return zfself::_ZFP_ZFEnumDataRef()->enumNameForValue(value); \
        } \
    public: \
        /** @cond ZFPrivateDoc */ \
        zfoverride \
        virtual zfindex enumCount(void) \
        { \
            return zfself::EnumCount(); \
        } \
        zfoverride \
        virtual zfuint enumDefault(void) \
        { \
            return zfself::EnumDefault(); \
        } \
        zfoverride \
        virtual zfbool enumIsFlags(void) \
        { \
            return zfself::EnumIsFlags(); \
        } \
        zfoverride \
        virtual zfindex enumIndexForValue(ZF_IN zfuint value) \
        { \
            return zfself::EnumIndexForValue(value); \
        } \
        zfoverride \
        virtual zfuint enumValueAtIndex(ZF_IN zfindex index) \
        { \
            return zfself::EnumValueAtIndex(index); \
        } \
        zfoverride \
        virtual const zfchar *enumNameAtIndex(ZF_IN zfindex index) \
        { \
            return zfself::EnumNameAtIndex(index); \
        } \
        zfoverride \
        virtual zfbool enumContainValue(ZF_IN zfuint value) \
        { \
            return zfself::EnumContainValue(value); \
        } \
        zfoverride \
        virtual zfuint enumValueForName(ZF_IN const zfchar *name) \
        { \
            return zfself::EnumValueForName(name); \
        } \
        zfoverride \
        virtual const zfchar *enumNameForValue(ZF_IN zfuint value) \
        { \
            return zfself::EnumNameForValue(value); \
        } \
        /** @endcond */ \
    public: \
        static const _ZFP_ZFEnumData *_ZFP_ZFEnumDataRef(void) \
        { \
            static _ZFP_ZFEnumDataHolder d(_ZFP_ZFEnumDataInit()); \
            return d.d; \
        } \
        static const _ZFP_ZFEnumData *_ZFP_ZFEnumDataInit(void) \
        { \
            zfCoreMutexLocker(); \
            _ZFP_ZFEnumData *d = _ZFP_ZFEnumDataAccess(EnumName::ClassData()); \
            if(d->needInitFlag) \
            { \
                d->needInitFlag = zffalse; \
                zfbool isEnableDuplicateValue = isEnableDuplicateValue_;

#define _ZFP_ZFENUM_VALUE_REGISTER_WITH_NAME(Value, Name) \
                d->add(isEnableDuplicateValue, zfself::e_##Value, Name);

#define _ZFP_ZFENUM_END(EnumName) \
    _ZFP_ZFENUM_END_DETAIL(EnumName, zffalse, ZFEnumInvalid())
#define _ZFP_ZFENUM_END_WITH_DEFAULT(EnumName, defaultEnum) \
    _ZFP_ZFENUM_END_DETAIL(EnumName, zffalse, defaultEnum)

#define _ZFP_ZFENUM_END_FLAGS(EnumName, EnumFlagsName) \
    _ZFP_ZFENUM_END_DETAIL(EnumName, zftrue, ZFEnumInvalid()) \
    _ZFP_ZFENUM_FLAGS_DECLARE(EnumName, EnumFlagsName, EnumName::EnumDefault())
#define _ZFP_ZFENUM_END_FLAGS_WITH_DEFAULT(EnumName, EnumFlagsName, defaultEnum, defaultEnumFlags) \
    _ZFP_ZFENUM_END_DETAIL(EnumName, zftrue, defaultEnum) \
    _ZFP_ZFENUM_FLAGS_DECLARE(EnumName, EnumFlagsName, defaultEnumFlags)

#define _ZFP_ZFENUM_END_DETAIL(EnumName, IsFlags, EnumDefaultAction) \
                d->enumDefault = (zfuint)(EnumDefaultAction); \
                if(d->enumDefault == ZFEnumInvalid()) \
                { \
                    d->enumDefault = d->enumValueAtIndex(0); \
                } \
                d->enumIsFlags = (IsFlags); \
            } \
            return d; \
        } \
    public: \
        /** @brief see #ZFEnum::enumIsFlags */ \
        static inline zfbool EnumIsFlags(void) \
        { \
            return (EnumName::ZFEnumType)zfself::_ZFP_ZFEnumDataRef()->enumIsFlags; \
        } \
        /** @brief default value for @ref EnumName */ \
        static inline EnumName::ZFEnumType EnumDefault(void) \
        { \
            return (EnumName::ZFEnumType)zfself::_ZFP_ZFEnumDataRef()->enumDefault; \
        } \
    public: \
        zfoverride \
        virtual const zfchar *wrappedValueTypeId(void); \
    }; \
    /** @brief editable version of @ref EnumName */ \
    zfclass ZF_ENV_EXPORT EnumName##Editable : zfextends EnumName \
    { \
        ZFOBJECT_DECLARE(EnumName##Editable, EnumName) \
    public: \
        /** @cond ZFPrivateDoc */ \
        zfoverride \
        virtual void enumValue(ZF_IN zfuint value) \
        { \
            zfsuper::enumValue(value); \
        } \
        /** @endcond */ \
    }; \
    /** @brief same as @ref EnumName##Enum, see @ref EnumName */ \
    typedef EnumName::ZFEnumType EnumName##Enum; \
    _ZFP_ZFENUM_CONVERTER_DECLARE(EnumName) \
    _ZFP_ZFENUM_PROP_TYPE_DECLARE(EnumName)

// ============================================================
extern ZF_ENV_EXPORT const _ZFP_ZFEnumData *_ZFP_ZFEnumDataFind(ZF_IN const ZFClass *enumClass);
extern ZF_ENV_EXPORT void _ZFP_ZFEnumMethodReg(ZF_IN_OUT ZFCoreArrayPOD<const ZFMethod *> &ret,
                                               ZF_IN const _ZFP_ZFEnumData *d);
#define _ZFP_ZFENUM_DEFINE(EnumName) \
    _ZFP_ZFENUM_CONVERTER_DEFINE(EnumName) \
    ZFOBJECT_REGISTER(EnumName) \
    ZFOBJECT_REGISTER(EnumName##Editable) \
    _ZFP_ZFENUM_PROP_TYPE_DEFINE(EnumName) \
    ZF_STATIC_REGISTER_INIT(EnumReg_##EnumName) \
    { \
        for(zfindex i = 0; i < EnumName::EnumCount(); ++i) \
        { \
            ZFMethodUserRegisterDetail_0(resultMethod, { \
                    return (EnumName##Enum)EnumName::EnumValueForName(invokerMethod->methodName() + 2); \
                }, EnumName::ClassData(), public, ZFMethodTypeStatic, \
                EnumName##Enum, zfstringWithFormat("e_%s", EnumName::EnumNameAtIndex(i))); \
            _m.add(resultMethod); \
        } \
        _ZFP_ZFEnumMethodReg(_m, EnumName::_ZFP_ZFEnumDataRef()); \
    } \
    ZF_STATIC_REGISTER_DESTROY(EnumReg_##EnumName) \
    { \
        for(zfindex i = 0; i < _m.count(); ++i) \
        { \
            ZFMethodUserUnregister(_m[i]); \
        } \
    } \
    ZFCoreArrayPOD<const ZFMethod *> _m; \
    ZF_STATIC_REGISTER_END(EnumReg_##EnumName)

#define _ZFP_ZFENUM_DEFINE_FLAGS(EnumName, EnumFlagsName) \
    _ZFP_ZFENUM_DEFINE(EnumName) \
    _ZFP_ZFENUM_FLAGS_DEFINE(EnumName, EnumFlagsName)

// ============================================================
#define _ZFP_ZFENUM_CONVERTER_DECLARE(EnumName) \
    /** @brief see @ref EnumName, return empty string if error */ \
    extern ZF_ENV_EXPORT zfbool EnumName##ToString(ZF_IN_OUT zfstring &ret, ZF_IN EnumName *const &value); \
    /** @brief see @ref EnumName, return empty string if error */ \
    inline zfstring EnumName##ToString(ZF_IN EnumName *const &value) \
    { \
        zfstring ret; \
        EnumName##ToString(ret, value); \
        return ret; \
    } \
    /** @brief see @ref EnumName, return enum object if success */ \
    extern ZF_ENV_EXPORT zfbool EnumName##FromString(ZF_OUT zfautoObject &ret, \
                                                     ZF_IN const zfchar *src, \
                                                     ZF_IN_OPT zfindex srcLen = zfindexMax()); \
    ZFOUTPUT_TYPE_DECLARE(EnumName##Enum)
#define _ZFP_ZFENUM_CONVERTER_DEFINE(EnumName) \
    zfbool EnumName##ToString(ZF_IN_OUT zfstring &ret, ZF_IN EnumName *const &value) \
    { \
        ret += ((value == zfnull) ? "" : value->enumName()); \
        return zftrue; \
    } \
    zfbool EnumName##FromString(ZF_OUT zfautoObject &ret, \
                                ZF_IN const zfchar *src, \
                                ZF_IN_OPT zfindex srcLen /* = zfindexMax() */) \
    { \
        if(zfsncmp(src, ZFEnumNameInvalid(), srcLen) == 0) \
        { \
            ret = zflineAlloc(EnumName, ZFEnumInvalid()); \
            return zftrue; \
        } \
        zfuint tmpValue = EnumName::EnumValueForName( \
            (srcLen == zfindexMax()) ? src : zfstring(src, srcLen).cString()); \
        if(tmpValue == ZFEnumInvalid()) \
        { \
            return zffalse; \
        } \
        else \
        { \
            ret = zflineAlloc(EnumName, tmpValue); \
            return zftrue; \
        } \
    } \
    ZFOUTPUT_TYPE_DEFINE(EnumName##Enum, {output << EnumName::EnumNameForValue(v);})

// ============================================================
#define _ZFP_ZFENUM_FLAGS_DECLARE(EnumName, EnumFlagsName, defaultValue) \
    /** @brief EnumName as #zfflags, see @ref EnumName, @ref EnumFlagsName##ToString, @ref EnumFlagsName##FromString */ \
    zffinal zfclassPOD ZF_ENV_EXPORT EnumFlagsName \
    { /* this class should be POD-like to support enum value reinterpret cast (ZFTAG_TRICKS: EnumReinterpretCast) */ \
    public: \
        /** @brief default value for EnumFlagsName */ \
        static zfuint EnumDefault(void) \
        { \
            return defaultValue; \
        } \
    public: \
        /** @cond ZFPrivateDoc */ \
        EnumFlagsName(void) : flags(0) {} \
        EnumFlagsName(ZF_IN zfuint const &flags) : flags(flags) {} \
        EnumFlagsName(ZF_IN EnumName##Enum const &flags) : flags((zfuint)flags) {} \
        EnumFlagsName(ZF_IN EnumFlagsName const &ref) : flags(ref.flags) {} \
    public: \
        zfuint const &enumValue(void) const {return this->flags;} \
        void enumValue(ZF_IN zfuint const &flags) {this->flags = flags;} \
        void enumValue(ZF_IN EnumName##Enum const &flags) {this->flags = (zfuint)flags;} \
    public: \
        operator zfuint const & (void) const {return this->flags;} \
        EnumFlagsName &operator = (ZF_IN zfuint const &flags) {this->flags = flags; return *this;} \
        EnumFlagsName &operator = (ZF_IN EnumName##Enum const &flags) {this->flags = (zfuint)flags; return *this;} \
        EnumFlagsName &operator = (ZF_IN EnumFlagsName const &ref) {this->flags = ref.flags; return *this;} \
        zfbool operator == (ZF_IN zfuint const &flags) const {return (this->flags == flags);} \
        zfbool operator == (ZF_IN EnumName##Enum const &flags) const {return (this->flags == (zfuint)flags);} \
        zfbool operator == (ZF_IN EnumFlagsName const &ref) const {return (this->flags == ref.flags);} \
        zfbool operator != (ZF_IN zfuint const &flags) const {return (this->flags != flags);} \
        zfbool operator != (ZF_IN EnumName##Enum const &flags) const {return (this->flags != (zfuint)flags);} \
        zfbool operator != (ZF_IN EnumFlagsName const &ref) const {return (this->flags != ref.flags);} \
        EnumFlagsName &operator |= (ZF_IN zfuint const &flags) {this->flags |= flags; return *this;} \
        EnumFlagsName &operator |= (ZF_IN EnumName##Enum const &flags) {this->flags |= (zfuint)flags; return *this;} \
        EnumFlagsName &operator |= (ZF_IN EnumFlagsName const &ref) {this->flags |= ref.flags; return *this;} \
        EnumFlagsName &operator &= (ZF_IN zfuint const &flags) {this->flags &= flags; return *this;} \
        EnumFlagsName &operator &= (ZF_IN EnumName##Enum const &flags) {this->flags &= (zfuint)flags; return *this;} \
        EnumFlagsName &operator &= (ZF_IN EnumFlagsName const &ref) {this->flags &= ref.flags; return *this;} \
    public: \
        void objectInfoT(ZF_IN_OUT zfstring &ret) const; \
        inline zfstring objectInfo(void) const \
        { \
            zfstring ret; \
            this->objectInfoT(ret); \
            return ret; \
        } \
        /** @endcond */ \
    private: \
        zfuint flags; \
    }; \
    ZFOUTPUT_TYPE_DECLARE(EnumFlagsName) \
    _ZFP_ZFENUM_FLAGS_PROP_TYPE_DECLARE(EnumName, EnumFlagsName)

#define _ZFP_ZFENUM_FLAGS_DEFINE(EnumName, EnumFlagsName) \
    ZFOUTPUT_TYPE_DEFINE(EnumFlagsName, {output << v.objectInfo();}) \
    void EnumFlagsName::objectInfoT(ZF_IN_OUT zfstring &ret) const \
    { \
        zfflagsToString(ret, EnumName::ClassData(), (zfflags)this->enumValue()); \
    } \
    _ZFP_ZFENUM_FLAGS_PROP_TYPE_DEFINE(EnumName, EnumFlagsName)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFEnumDeclare_h_

