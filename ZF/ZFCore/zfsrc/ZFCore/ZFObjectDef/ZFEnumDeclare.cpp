#include "ZFEnumDeclare.h"
#include "ZFObjectImpl.h"

#include "ZFCore/ZFSTLWrapper/zfstl_string.h"
#include "ZFCore/ZFSTLWrapper/zfstl_map.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZF_STATIC_INITIALIZER_INIT(ZFEnumDataHolder)
{
}
ZFCoreMap enumDataMap;
ZF_STATIC_INITIALIZER_END(ZFEnumDataHolder)
#define _ZFP_ZFEnumDataMap (ZF_STATIC_INITIALIZER_INSTANCE(ZFEnumDataHolder)->enumDataMap)

// ============================================================
zfclassNotPOD _ZFP_ZFEnumDataPrivate
{
public:
    typedef zfstlmap<zfuint, ZFCoreArrayPOD<zfchar *> > ValueMapType;
    ValueMapType valueMap;
    typedef zfstlmap<const zfchar *, zfuint, zfcharConst_zfstlComparer> NameMapType;
    NameMapType nameMap;
    ZFCoreArrayPOD<zfuint> vl; // ensured no duplicated value
    ZFCoreArrayPOD<zfchar *> nl; // for duplicated value, only first stored
public:
    ~_ZFP_ZFEnumDataPrivate(void)
    {
        this->nameMap.clear();
        for(ValueMapType::iterator it = this->valueMap.begin(); it != this->valueMap.end(); ++it)
        {
            for(zfindex j = 0; j < it->second.count(); ++j)
            {
                zffree(it->second[j]);
            }
        }
    }
};
_ZFP_ZFEnumData::_ZFP_ZFEnumData(void)
: needInitFlag(zftrue)
, ownerClass(zfnull)
, enumDefault(0)
, enumIsFlags(zffalse)
, d(zfnew(_ZFP_ZFEnumDataPrivate))
{
}
_ZFP_ZFEnumData::~_ZFP_ZFEnumData(void)
{
    zfdelete(d);
    d = zfnull;
}
void _ZFP_ZFEnumData::add(ZF_IN zfbool isEnableDuplicateValue,
                          ZF_IN zfuint value,
                          ZF_IN const zfchar *name)
{
    zfCoreAssert(value != ZFEnumInvalid());
    zfCoreAssert(!zfsIsEmpty(name));
    _ZFP_ZFEnumDataPrivate::ValueMapType::iterator it = d->valueMap.find(value);
    if(it != d->valueMap.end())
    {
        zfCoreAssertWithMessageTrim(isEnableDuplicateValue,
            "[ZFEnum] duplicate value %s (new: %s, old: %s) when define %s",
            zfsFromInt(value).cString(),
            name,
            it->second[0],
            this->ownerClass->classNameFull());
        it->second.add(zfsCopy(name));
    }
    else
    {
        zfchar *nameTmp = zfsCopy(name);
        d->valueMap[value].add(nameTmp);
        d->vl.add(value);
        d->nl.add(nameTmp);
    }
    if(d->nameMap.find(name) == d->nameMap.end())
    {
        d->nameMap[name] = value;
    }
}
zfindex _ZFP_ZFEnumData::enumCount(void) const
{
    return d->vl.count();
}
zfindex _ZFP_ZFEnumData::enumIndexForValue(ZF_IN zfuint value) const
{
    return d->vl.find(value);
}
zfuint _ZFP_ZFEnumData::enumValueAtIndex(ZF_IN zfindex index) const
{
    if(index >= d->vl.count())
    {
        return ZFEnumInvalid();
    }
    else
    {
        return d->vl[index];
    }
}
const zfchar *_ZFP_ZFEnumData::enumNameAtIndex(ZF_IN zfindex index) const
{
    if(index >= d->nl.count())
    {
        return zfnull;
    }
    else
    {
        return d->nl[index];
    }
}
zfbool _ZFP_ZFEnumData::enumContainValue(ZF_IN zfuint value) const
{
    return (d->valueMap.find(value) != d->valueMap.end());
}
zfuint _ZFP_ZFEnumData::enumValueForName(ZF_IN const zfchar *name) const
{
    if(!zfsIsEmpty(name))
    {
        _ZFP_ZFEnumDataPrivate::NameMapType::iterator it = d->nameMap.find(name);
        if(it != d->nameMap.end())
        {
            return it->second;
        }
    }
    return ZFEnumInvalid();
}
const zfchar *_ZFP_ZFEnumData::enumNameForValue(ZF_IN zfuint value) const
{
    _ZFP_ZFEnumDataPrivate::ValueMapType::const_iterator it = d->valueMap.find(value);
    if(it != d->valueMap.end())
    {
        return it->second[0];
    }
    else
    {
        return ZFEnumNameInvalid();
    }
}

_ZFP_ZFEnumData *_ZFP_ZFEnumDataAccess(ZF_IN const ZFClass *ownerClass)
{
    zfCoreMutexLocker();
    _ZFP_ZFEnumData *d = _ZFP_ZFEnumDataMap.get<_ZFP_ZFEnumData *>(ownerClass->classNameFull());
    if(d != zfnull)
    {
        return d;
    }
    d = zfnew(_ZFP_ZFEnumData);
    _ZFP_ZFEnumDataMap.set(ownerClass->classNameFull(), ZFCorePointerForObject<_ZFP_ZFEnumData *>(d));
    d->ownerClass = ownerClass;
    return d;
}
void _ZFP_ZFEnumDataCleanup(ZF_IN const ZFClass *ownerClass)
{
    _ZFP_ZFEnumDataMap.remove(ownerClass->classNameFull());
}

// ============================================================
const _ZFP_ZFEnumData *_ZFP_ZFEnumDataFind(ZF_IN const ZFClass *enumClass)
{
    return _ZFP_ZFEnumDataMap.get<_ZFP_ZFEnumData *>(enumClass->classNameFull());
}
void _ZFP_ZFEnumMethodReg(ZF_IN_OUT ZFCoreArrayPOD<const ZFMethod *> &ret,
                          ZF_IN const _ZFP_ZFEnumData *d)
{
    {
        ZFMethodUserRegisterDetail_0(resultMethod, {
                return _ZFP_ZFEnumDataFind(invokerMethod->methodOwnerClass())->enumIsFlags;
            }, d->ownerClass, public, ZFMethodTypeStatic,
            zfbool, "EnumIsFlags");
        ret.add(resultMethod);
    }
    {
        ZFMethodUserRegisterDetail_0(resultMethod, {
                return _ZFP_ZFEnumDataFind(invokerMethod->methodOwnerClass())->enumDefault;
            }, d->ownerClass, public, ZFMethodTypeStatic,
            zfuint, "EnumDefault");
        ret.add(resultMethod);
    }
    {
        ZFMethodUserRegisterDetail_0(resultMethod, {
                return _ZFP_ZFEnumDataFind(invokerMethod->methodOwnerClass())->enumCount();
            }, d->ownerClass, public, ZFMethodTypeStatic,
            zfindex, "EnumCount");
        ret.add(resultMethod);
    }
    {
        ZFMethodUserRegisterDetail_1(resultMethod, {
                return _ZFP_ZFEnumDataFind(invokerMethod->methodOwnerClass())->enumIndexForValue(value);
            }, d->ownerClass, public, ZFMethodTypeStatic,
            zfindex, "EnumIndexForValue",
            ZFMP_IN(zfuint, value));
        ret.add(resultMethod);
    }
    {
        ZFMethodUserRegisterDetail_1(resultMethod, {
                return _ZFP_ZFEnumDataFind(invokerMethod->methodOwnerClass())->enumValueAtIndex(index);
            }, d->ownerClass, public, ZFMethodTypeStatic,
            zfuint, "EnumValueAtIndex",
            ZFMP_IN(zfindex, index));
        ret.add(resultMethod);
    }
    {
        ZFMethodUserRegisterDetail_1(resultMethod, {
                return _ZFP_ZFEnumDataFind(invokerMethod->methodOwnerClass())->enumNameAtIndex(index);
            }, d->ownerClass, public, ZFMethodTypeStatic,
            const zfchar *, "EnumNameAtIndex",
            ZFMP_IN(zfindex, index));
        ret.add(resultMethod);
    }
    {
        ZFMethodUserRegisterDetail_1(resultMethod, {
                return _ZFP_ZFEnumDataFind(invokerMethod->methodOwnerClass())->enumContainValue(value);
            }, d->ownerClass, public, ZFMethodTypeStatic,
            zfbool, "EnumContainValue",
            ZFMP_IN(zfuint, value));
        ret.add(resultMethod);
    }
    {
        ZFMethodUserRegisterDetail_1(resultMethod, {
                return _ZFP_ZFEnumDataFind(invokerMethod->methodOwnerClass())->enumValueForName(name);
            }, d->ownerClass, public, ZFMethodTypeStatic,
            zfuint, "EnumValueForName",
            ZFMP_IN(const zfchar *, name));
        ret.add(resultMethod);
    }
    {
        ZFMethodUserRegisterDetail_1(resultMethod, {
                return _ZFP_ZFEnumDataFind(invokerMethod->methodOwnerClass())->enumNameForValue(value);
            }, d->ownerClass, public, ZFMethodTypeStatic,
            const zfchar *, "EnumNameForValue",
            ZFMP_IN(zfuint, value));
        ret.add(resultMethod);
    }
}

ZF_NAMESPACE_GLOBAL_END

