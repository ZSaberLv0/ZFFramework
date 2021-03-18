#include "ZFEnumDynamicRegister.h"
#include "ZFObjectImpl.h"

#include "ZFClassDynamicRegister.h"
#include "ZFMethodUserRegister.h"
#include "ZFTypeIdDynamicRegister.h"

#include "../ZFSTLWrapper/zfstl_map.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclass _ZFP_I_ZFEnumDynamicHolder : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_I_ZFEnumDynamicHolder, ZFObject)
public:
    _ZFP_ZFEnumData *d;
    const ZFClass *enumEditableClass;
    ZFCoreArrayPOD<const ZFMethod *> userRegMethods;
};
zfclass _ZFP_I_ZFEnumDynamic : zfextends ZFEnum
{
    ZFOBJECT_DECLARE(_ZFP_I_ZFEnumDynamic, ZFEnum)

public:
    zfoverride
    virtual zfidentity objectHash(void)
    {
        return zfidentityHash(
            zfidentityCalcString(zfself::ClassData()->classNameFull()),
            zfidentityCalcPOD(this->enumValue()));
    }
public:
    zfoverride
    virtual zfindex enumCount(void)
    {
        return _ZFP_ZFEnumDataRef()->enumCount();
    }
    zfoverride
    virtual zfuint enumDefault(void)
    {
        return _ZFP_ZFEnumDataRef()->enumDefault;
    }
    zfoverride
    virtual zfbool enumIsFlags(void)
    {
        return _ZFP_ZFEnumDataRef()->enumIsFlags;
    }
    zfoverride
    virtual zfindex enumIndexForValue(ZF_IN zfuint value)
    {
        return _ZFP_ZFEnumDataRef()->enumIndexForValue(value);
    }
    zfoverride
    virtual zfuint enumValueAtIndex(ZF_IN zfindex index)
    {
        return _ZFP_ZFEnumDataRef()->enumValueAtIndex(index);
    }
    zfoverride
    virtual const zfchar *enumNameAtIndex(ZF_IN zfindex index)
    {
        return _ZFP_ZFEnumDataRef()->enumNameAtIndex(index);
    }
    zfoverride
    virtual zfbool enumContainValue(ZF_IN zfuint value)
    {
        return _ZFP_ZFEnumDataRef()->enumContainValue(value);
    }
    zfoverride
    virtual zfuint enumValueForName(ZF_IN const zfchar *name)
    {
        return _ZFP_ZFEnumDataRef()->enumValueForName(name);
    }
    zfoverride
    virtual const zfchar *enumNameForValue(ZF_IN zfuint value)
    {
        return _ZFP_ZFEnumDataRef()->enumNameForValue(value);
    }
public:
    zfoverride
    virtual const zfchar *wrappedValueTypeId(void)
    {
        return _ZFP_ZFEnumDataRef()->ownerClass->classNameFull();
    }
private:
    const _ZFP_ZFEnumData *_ZFP_ZFEnumDataRef(void)
    {
        return this->classData()->classDynamicRegisterUserData()->to<_ZFP_I_ZFEnumDynamicHolder *>()->d;
    }
};
zfclassNotPOD _ZFP_ZFEnumTypeId : zfextendsNotPOD ZFTypeInfo
{
public:
    const ZFClass *enumClass;

public:
    enum {
        TypeIdRegistered = 1,
        TypeIdSerializable = 1,
    };
    zfoverride
    virtual zfbool typeIdSerializable(void) const
    {
        return zftrue;
    }
    zfoverride
    virtual const zfchar *typeId(void) const
    {
        return this->enumClass->classNameFull();
    }
    zfoverride
    virtual const ZFClass *typeIdClass(void) const
    {
        return this->enumClass;
    }
};

// ============================================================
static ZFEnum *_ZFP_ZFEnumDynamic_e(ZF_IN const ZFMethod *invokerMethod, ZF_IN ZFObject *invokerObject)
{
    const zfchar *enumName = invokerMethod->methodName() + zfslen("e_");
    zfstring tagKey = zfstringWithFormat("_ZFP_EnumDyn_%s", enumName);
    ZFEnum *ret = invokerMethod->methodOwnerClass()->classTag<ZFEnum *>(tagKey);
    if(ret != zfnull)
    {
        return ret;
    }
    const _ZFP_ZFEnumData *d = _ZFP_ZFEnumDataFind(invokerMethod->methodOwnerClass());
    zfautoObject retHolder = invokerMethod->methodOwnerClass()->newInstance();
    ret = retHolder;
    ret->_ZFP_enumValue(d->enumValueForName(enumName));
    invokerMethod->methodOwnerClass()->classTag(tagKey, retHolder);
    return ret;
}
const ZFClass *ZFEnumDynamicRegister(ZF_IN const zfchar *enumClassName,
                                     ZF_IN const ZFCoreArrayPOD<zfuint> &enumValues,
                                     ZF_IN const ZFCoreArray<zfstring> &enumNames,
                                     ZF_IN zfuint enumDefault /* = ZFEnumInvalid() */,
                                     ZF_IN zfbool enumIsFlags /* = zffalse */,
                                     ZF_OUT_OPT zfstring *errorHint /* = zfnull */)
{
    if(enumValues.count() != enumNames.count())
    {
        zfstringAppend(errorHint, "enum values and names mismatch");
        return zfnull;
    }
    if(enumValues.isEmpty())
    {
        zfstringAppend(errorHint, "empty enum value");
        return zfnull;
    }

    if(enumDefault == ZFEnumInvalid())
    {
        enumDefault = enumValues[0];
    }
    else
    {
        if(enumValues.find(enumDefault) == zfindexMax())
        {
            zfstringAppend(errorHint,
                "default enum value %s doesn't exists in enum value list: %s",
                zfuintToString(enumDefault).cString(),
                enumValues.objectInfoOfContent().cString());
            return zfnull;
        }
    }

    // class
    zfblockedAlloc(_ZFP_I_ZFEnumDynamicHolder, d);
    const ZFClass *enumClass = ZFClassDynamicRegister(
        enumClassName, _ZFP_I_ZFEnumDynamic::ClassData(),
        d, errorHint);
    if(enumClass == zfnull)
    {
        return zfnull;
    }
    d->enumEditableClass = ZFClassDynamicRegister(
        zfstringWithFormat("%sEditable", enumClassName), enumClass,
        d, errorHint);
    if(d->enumEditableClass == zfnull)
    {
        ZFClassDynamicUnregister(enumClass);
        return zfnull;
    }

    // internal data
    d->d = _ZFP_ZFEnumDataAccess(enumClass);
    for(zfindex i = 0; i < enumValues.count(); ++i)
    {
        d->d->add(zftrue, enumValues[i], enumNames[i]);
    }
    d->d->needInitFlag = zffalse;
    d->d->enumDefault = enumDefault;
    d->d->enumIsFlags = enumIsFlags;

    // typeid
    _ZFP_ZFEnumTypeId *typeIdData = zfnew(_ZFP_ZFEnumTypeId);
    ZFCorePointerForObject<ZFTypeInfo *> typeIdDataHolder(typeIdData);
    typeIdData->enumClass = enumClass;
    if(!ZFTypeIdDynamicRegister(enumClassName, typeIdDataHolder, errorHint))
    {
        ZFClassDynamicUnregister(enumClass);
        ZFClassDynamicUnregister(d->enumEditableClass);
        return zfnull;
    }

    // method
    _ZFP_ZFEnumMethodReg(d->userRegMethods, d->d);
    for(zfindex i = 0; i < d->d->enumCount(); ++i)
    {
        ZFMethodUserRegisterDetail_0(resultMethod, {
                return _ZFP_ZFEnumDynamic_e(invokerMethod, invokerObject);
            }, enumClass, public, ZFMethodTypeStatic,
            ZFEnum *, zfstringWithFormat("e_%s", d->d->enumNameAtIndex(i)));
        d->userRegMethods.add(resultMethod);
    }

    return d->d->ownerClass;
}
void ZFEnumDynamicUnregister(ZF_IN const ZFClass *enumClass)
{
    if(enumClass == zfnull)
    {
        return ;
    }

    _ZFP_I_ZFEnumDynamicHolder *d = enumClass->classDynamicRegisterUserData()->toAny();
    if(d == zfnull)
    {
        return ;
    }
    for(zfindex i = 0; i < d->userRegMethods.count(); ++i)
    {
        ZFMethodUserUnregister(d->userRegMethods[i]);
    }
    const ZFClass *enumEditableClass = d->enumEditableClass;
    ZFTypeIdDynamicUnregister(enumClass->classNameFull());
    _ZFP_ZFEnumDataCleanup(enumClass);
    ZFClassDynamicUnregister(enumEditableClass);
    ZFClassDynamicUnregister(enumClass);
}

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_6(const ZFClass *, ZFEnumDynamicRegister, ZFMP_IN(const zfchar *, enumClassName), ZFMP_IN(const ZFCoreArrayPOD<zfuint> &, enumValues), ZFMP_IN(const ZFCoreArray<zfstring> &, enumNames), ZFMP_IN_OPT(zfuint, enumDefault, ZFEnumInvalid()), ZFMP_IN_OPT(zfbool, enumIsFlags, zffalse), ZFMP_OUT_OPT(zfstring *, errorHint, zfnull))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(void, ZFEnumDynamicUnregister, ZFMP_IN(const ZFClass *, enumClass))

ZF_NAMESPACE_GLOBAL_END
#endif

