/**
 * @file ZFEnum.h
 * @brief enum utility
 */

#ifndef _ZFI_ZFEnum_h_
#define _ZFI_ZFEnum_h_

#include "ZFTypeId.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief invalid value for ZFEnum
 */
#define ZFEnumInvalid() ((zfuint)-1)
/**
 * @brief invalid value's name for ZFEnum
 */
#define ZFEnumNameInvalid() "ZFEnumInvalid"

// ============================================================
/**
 * @brief abstract base class of all enum type of ZFFramework
 *
 * please refer to #ZFENUM_BEGIN to see how to use enum type\n
 * \n
 * serializable data:
 * @code
 *   <EnumClass
 *       value="EmunValueName" // optional, #ZFEnumNameInvalid by default
 *   />
 * @endcode
 */
zfabstract ZF_ENV_EXPORT ZFEnum : zfextends ZFTypeIdWrapper
{
    ZFOBJECT_DECLARE_ABSTRACT_WITH_CUSTOM_CTOR(ZFEnum, ZFTypeIdWrapper)

protected:
    /** @cond ZFPrivateDoc */
    ZFEnum(void) : _ZFP_ZFEnum_value(ZFEnumInvalid()) {}
    /** @endcond */

protected:
    zfoverride
    virtual zfbool serializableOnSerializeFromData(ZF_IN const ZFSerializableData &serializableData,
                                                   ZF_OUT_OPT zfstring *outErrorHint = zfnull,
                                                   ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull);
    zfoverride
    virtual zfbool serializableOnSerializeToData(ZF_IN_OUT ZFSerializableData &serializableData,
                                                 ZF_IN ZFSerializable *referencedOwnerOrNull,
                                                 ZF_OUT_OPT zfstring *outErrorHint = zfnull);

protected:
    zfoverride
    virtual void copyableOnCopyFrom(ZF_IN ZFObject *anotherObj)
    {
        zfsuperI(ZFCopyable)::copyableOnCopyFrom(anotherObj);
        this->enumValue(ZFCastZFObjectUnchecked(zfself *, anotherObj)->enumValue());
    }

protected:
    /**
     * @brief init with value or ZFEnumInvalid if invalid
     */
    virtual void objectOnInit(ZF_IN zfuint value);
    /**
     * @brief init with ZFEnum
     */
    virtual void objectOnInit(ZF_IN ZFEnum *another);

    zfoverride
    virtual void objectOnInit(void) {zfsuper::objectOnInit();}

    zfoverride
    virtual void objectInfoT(ZF_IN_OUT zfstring &ret);

public:
    /**
     * @brief compare result of two enum values as int,
     *   or return ZFCompareUncomparable if any of them are ZFEnumInvalid
     */
    zfoverride
    virtual ZFCompareResult objectCompare(ZF_IN ZFObject *anotherObj);

public:
    /* use ZFObject version instead of ZFTypeIdWrapper version */
    zfoverride
    virtual zfbool objectIsPrivate(void)
    {
        return ZFObject::objectIsPrivate();
    }
    zfoverride
    virtual zfbool objectIsInternal(void)
    {
        return ZFObject::objectIsInternal();
    }

public:
    zfoverride
    virtual void *wrappedValue(void);
    zfoverride
    virtual void wrappedValue(ZF_IN const void *v);
    zfoverride
    virtual void wrappedValueCopy(ZF_IN void *v);
public:
    zfoverride
    virtual void wrappedValueReset(void);
    zfoverride
    virtual zfbool wrappedValueIsInit(void);
public:
    zfoverride
    virtual zfbool wrappedValueFromData(ZF_IN const ZFSerializableData &serializableData,
                                        ZF_OUT_OPT zfstring *outErrorHint = zfnull,
                                        ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull);
    zfoverride
    virtual zfbool wrappedValueToData(ZF_OUT ZFSerializableData &serializableData,
                                      ZF_OUT_OPT zfstring *outErrorHint = zfnull);
    zfoverride
    virtual zfbool wrappedValueFromString(ZF_IN const zfchar *src,
                                          ZF_IN_OPT zfindex srcLen = zfindexMax());
    zfoverride
    virtual zfbool wrappedValueToString(ZF_IN_OUT zfstring &s);

public:
    /**
     * @brief get value num
     */
    virtual zfindex enumCount(void) zfpurevirtual;
    /**
     * @brief get the index of enum value, or zfindexMax() if no such enum value
     */
    virtual zfindex enumIndexForValue(ZF_IN zfuint value) zfpurevirtual;
    /**
     * @brief get the value at index, or ZFEnumInvalid if not exist
     */
    virtual zfuint enumValueAtIndex(ZF_IN zfindex index) zfpurevirtual;
    /**
     * @brief get the name at index, or ZFEnumNameInvalid if not exist
     */
    virtual const zfchar *enumNameAtIndex(ZF_IN zfindex index) zfpurevirtual;
    /**
     * @brief return true if contain the specified value
     */
    virtual zfbool enumContainValue(ZF_IN zfuint value) zfpurevirtual;

    /**
     * @brief get the value with specified name, or ZFEnumInvalid if not exist
     */
    virtual zfuint enumValueForName(ZF_IN const zfchar *name) zfpurevirtual;
    /**
     * @brief get the name with specified value, or ZFEnumNameInvalid if not exist
     */
    virtual const zfchar *enumNameForValue(ZF_IN zfuint value) zfpurevirtual;

    /**
     * @brief true if this enum type is flags type
     *   (which support bit operation)
     */
    virtual zfbool enumIsFlags(void) zfpurevirtual;

    /**
     * @brief default value of this enum
     */
    virtual zfuint enumDefault(void) zfpurevirtual;

    /**
     * @brief get the enum value, or #ZFEnumInvalid if invalid
     */
    virtual zfuint enumValue(void)
    {
        return _ZFP_ZFEnum_value;
    }
    /**
     * @brief get the enum value and cast to desired type,
     *   you must ensure it's not #ZFEnumInvalid
     */
    template<typename T_EnumType>
    inline T_EnumType enumValue(void)
    {
        return ZFCastStatic(T_EnumType, _ZFP_ZFEnum_value);
    }
    /**
     * @brief get current value's name, or ZFEnumNameInvalid if invalid
     *
     * return invalid if holds flags value,
     * use #zfflagsToString instead
     */
    virtual const zfchar *enumName(void)
    {
        return this->enumNameForValue(_ZFP_ZFEnum_value);
    }

protected:
    /**
     * @brief set the enum value
     *
     * must be the enum's value or ZFEnumInvalid
     */
    virtual void enumValue(ZF_IN zfuint value)
    {
        if(!this->enumIsFlags() && !this->enumContainValue(value))
        {
            _ZFP_ZFEnum_value = ZFEnumInvalid();
            return ;
        }
        _ZFP_ZFEnum_value = value;
    }

public:
    void _ZFP_enumValue(ZF_IN zfuint value)
    {
        this->enumValue(value);
    }
public:
    zfuint _ZFP_ZFEnum_value;
public:
    zfoverride
    virtual void wrappedValueOnAssign(ZF_IN ZFTypeIdWrapper *ref)
    {
        zfself *refTmp = ZFCastZFObject(zfself *, ref);
        if(refTmp != zfnull)
        {
            this->_ZFP_ZFEnum_value = refTmp->_ZFP_ZFEnum_value;
        }
    }
};

// ============================================================
// zfflags conversion
/**
 * @brief zfflags conversion using ZFEnum as source, see #zfflagsToString
 */
extern ZF_ENV_EXPORT zfbool zfflagsToString(ZF_IN_OUT zfstring &ret,
                                            ZF_IN const ZFClass *enumClass,
                                            ZF_IN zfflags const &value,
                                            ZF_IN_OPT zfbool includeNotConverted = zftrue,
                                            ZF_IN_OPT zfbool exclusiveMode = zffalse,
                                            ZF_OUT_OPT zfflags *notConverted = zfnull,
                                            ZF_IN_OPT zfchar separatorToken = '|');
/** @brief see #zfflagsToString */
inline zfstring zfflagsToString(ZF_IN const ZFClass *enumClass,
                                ZF_IN zfflags const &value,
                                ZF_IN_OPT zfbool includeNotConverted = zftrue,
                                ZF_IN_OPT zfbool exclusiveMode = zffalse,
                                ZF_OUT_OPT zfflags *notConverted = zfnull,
                                ZF_IN_OPT zfchar separatorToken = '|')
{
    zfstring ret;
    zfflagsToString(ret, enumClass, value, includeNotConverted, exclusiveMode, notConverted, separatorToken);
    return ret;
}
/**
 * @brief zfflags conversion using ZFEnum as source, see #zfflagsFromString
 */
extern ZF_ENV_EXPORT zfbool zfflagsFromString(ZF_OUT zfflags &ret,
                                              ZF_IN const ZFClass *enumClass,
                                              ZF_IN const zfchar *src,
                                              ZF_IN_OPT zfindex srcLen = zfindexMax(),
                                              ZF_IN_OPT zfchar separatorToken = '|',
                                              ZF_OUT_OPT const zfchar **outErrorPos = zfnull);

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFEnum_h_

