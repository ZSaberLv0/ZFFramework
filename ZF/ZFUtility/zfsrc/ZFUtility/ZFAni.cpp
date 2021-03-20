#include "ZFAni.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFMETHOD_FUNC_DEFINE_3(zfautoObject, ZFAni,
                       ZFMP_IN(ZFObject *, target),
                       ZFMP_IN(const ZFListener &, aniImpl),
                       ZFMP_IN_OPT(ZFObject *, userData, zfnull))
{
    if(!aniImpl.callbackIsValid())
    {
        return zfnull;
    }
    zfblockedAlloc(ZFAniForCustomAni, ani);
    ani->customAniCallback(aniImpl);
    if(userData != zfnull)
    {
        ani->customAniUserData(userData);
    }
    return ani;
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_4(zfautoObject, ZFAni,
                       ZFMP_IN(ZFObject *, target),
                       ZFMP_IN(const zfchar *, name),
                       ZFMP_IN(const zfchar *, from),
                       ZFMP_IN(const zfchar *, to))
{
    zfblockedAlloc(ZFAniForGeneric, ani);
    ani->aniTarget(target);
    ani->name(name);
    ani->fromValue(from);
    ani->toValue(to);
    return ani;
}
ZFMETHOD_FUNC_DEFINE_4(zfautoObject, ZFAni,
                       ZFMP_IN(ZFObject *, target),
                       ZFMP_IN(const zfchar *, name),
                       ZFMP_IN(ZFObject *, from),
                       ZFMP_IN(ZFObject *, to))
{
    zfblockedAlloc(ZFAniForObject, ani);
    ani->aniTarget(target);
    ani->name(name);
    ani->fromValue(from);
    ani->toValue(to);
    return ani;
}

// ============================================================
zfclassNotPOD _ZFP_ZFAniForGenericPrivate
{
public:
    zfbool needUpdate;
private:
    zfautoObject _aniTarget;
    const ZFMethod *_setterMethod;
    const ZFMethod *_getterMethod;
    zfautoObject _from;
    zfautoObject _to;
    const ZFTypeInfo *_typeInfo; // null for retain property, non-null for assignable property
public:
    _ZFP_ZFAniForGenericPrivate(void)
    : needUpdate(zftrue)
    , _aniTarget(zfnull)
    , _setterMethod(zfnull)
    , _getterMethod(zfnull)
    , _from()
    , _to(zfnull)
    , _typeInfo(zfnull)
    {
    }
public:
    void cleanup(void)
    {
        _aniTarget = zfnull;
        _setterMethod = zfnull;
        _getterMethod = zfnull;
        _from = zfnull;
        _to = zfnull;
        _typeInfo = zfnull;
    }
    zfbool checkSetup(ZF_IN ZFObject *aniTarget,
                      ZF_IN const zfchar *name,
                      ZF_IN ZFObject *from,
                      ZF_IN ZFObject *to)
    {
        if(!this->needUpdate)
        {
            return zftrue;
        }
        if(!this->_prepare(aniTarget, name))
        {
            return zffalse;
        }
        _from = from;
        _to = to;
        return this->_setup();
    }
    zfbool checkSetup(ZF_IN ZFObject *aniTarget,
                      ZF_IN const zfchar *name,
                      ZF_IN const zfchar *from,
                      ZF_IN const zfchar *to)
    {
        if(!this->needUpdate)
        {
            return zftrue;
        }
        if(!this->_prepare(aniTarget, name))
        {
            return zffalse;
        }
        const zfchar *typeId = _getterMethod->methodReturnTypeId();
        const ZFClass *typeClass = ZFClass::classForName(typeId);
        if(typeClass != zfnull)
        {
            if(!typeClass->classIsTypeOf(ZFSerializable::ClassData()))
            {
                return zffalse;
            }
            if(!zfsIsEmpty(from))
            {
                _from = typeClass->newInstance();
                if(!_from.to<ZFSerializable *>()->serializeFromString(from))
                {
                    return zffalse;
                }
            }
            if(!zfsIsEmpty(to))
            {
                _to = typeClass->newInstance();
                if(!_to.to<ZFSerializable *>()->serializeFromString(to))
                {
                    return zffalse;
                }
            }
        }
        else
        {
            const ZFTypeInfo *typeInfo = ZFTypeInfoForName(typeId);
            if(typeInfo == zfnull || typeInfo->typeIdClass() == zfnull || typeInfo->typeIdClass()->classIsAbstract())
            {
                return zffalse;
            }
            if(!zfsIsEmpty(from))
            {
                _from = typeInfo->typeIdClass()->newInstance();
                if(!_from.to<ZFTypeIdWrapper *>()->wrappedValueFromString(from))
                {
                    return zffalse;
                }
            }
            if(!zfsIsEmpty(to))
            {
                _to = typeInfo->typeIdClass()->newInstance();
                if(!_to.to<ZFTypeIdWrapper *>()->wrappedValueFromString(to))
                {
                    return zffalse;
                }
            }
        }
        return this->_setup();
    }
    void update(ZF_IN zffloat progress)
    {
        if(_from == zfnull)
        {
            _from = _getterMethod->methodGenericInvoke(_aniTarget);
            if(_from == zfnull)
            {
                return ;
            }
            if(_from->classData()->classIsTypeOf(ZFCopyable::ClassData()))
            {
                _from = _from.to<ZFCopyable *>()->copy();
            }
        }
        if(_to == zfnull)
        {
            _to = _getterMethod->methodGenericInvoke(_aniTarget);
            if(_to == zfnull)
            {
                return ;
            }
            if(_to->classData()->classIsTypeOf(ZFCopyable::ClassData()))
            {
                _to = _to.to<ZFCopyable *>()->copy();
            }
        }

        if(_typeInfo != zfnull)
        {
            zfautoObject valueHolder = _typeInfo->typeIdClass()->newInstance();
            ZFProgressable *value = valueHolder;
            if(value != zfnull && value->progressUpdate(_from, _to, progress))
            {
                _setterMethod->methodGenericInvoke(_aniTarget, valueHolder);
            }
        }
        else
        {
            zfautoObject valueHolder = _getterMethod->methodGenericInvoke(_aniTarget);
            ZFProgressable *value = valueHolder;
            if(value != zfnull)
            {
                value->progressUpdate(_from, _to, progress);
            }
        }
    }
private:
    zfbool _prepare(ZF_IN ZFObject *aniTarget,
                    ZF_IN const zfchar *name)
    {
        if(aniTarget == zfnull)
        {
            return zffalse;
        }
        const ZFProperty *prop = aniTarget->classData()->propertyForName(name);
        if(prop != zfnull)
        {
            _setterMethod = prop->setterMethod();
            _getterMethod = prop->getterMethod();
        }
        else
        {
            _setterMethod = aniTarget->classData()->propertySetterForName(name);
            _getterMethod = aniTarget->classData()->propertyGetterForName(name);
        }
        if(_setterMethod != zfnull && _setterMethod->methodPrivilegeType() != ZFMethodPrivilegeTypePublic)
        {
            _setterMethod = zfnull;
        }
        if(_getterMethod != zfnull && _getterMethod->methodPrivilegeType() == ZFMethodPrivilegeTypePrivate)
        {
            _getterMethod = zfnull;
        }
        _aniTarget = aniTarget;
        return (_getterMethod != zfnull);
    }
    zfbool _setup(void)
    {
        if(_from == zfnull && _to == zfnull)
        {
            return zffalse;
        }
        if(_getterMethod->methodGenericInvoke(_aniTarget) == zfnull)
        {
            return zffalse;
        }

        if(_setterMethod == zfnull)
        { // ani by getter only
            if(ZFClass::classForName(_getterMethod->methodReturnTypeId()) == zfnull
                || ZFCastZFObject(ZFProgressable *, _getterMethod->methodGenericInvoke(_aniTarget)) == zfnull)
            { // ani by getter only valid for retain property
                return zffalse;
            }
        }
        else
        { // ani by getter>change>setter
            _typeInfo = ZFTypeInfoForName(_getterMethod->methodReturnTypeId());
            if(_typeInfo == zfnull
                || _typeInfo->typeIdClass() == zfnull
                || _typeInfo->typeIdClass()->classIsAbstract()
                || !_typeInfo->typeIdClass()->classIsTypeOf(ZFProgressable::ClassData())
            ) {
                return zffalse;
            }
        }
        return zftrue;
    }
};

// ============================================================
ZFOBJECT_REGISTER(ZFAniForCustomAni)

ZFOBJECT_REGISTER(ZFAniForObject)
ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFAniForObject, zfstring, name) {d->needUpdate = true;}
ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFAniForObject, ZFObject *, fromValue) {d->needUpdate = true;}
ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFAniForObject, ZFObject *, toValue) {d->needUpdate = true;}
void ZFAniForObject::aniImplTargetOnChange(ZF_IN ZFObject *aniTargetOld)
{
    zfsuper::aniImplTargetOnChange(aniTargetOld);
    d->needUpdate = zftrue;
}
zfbool ZFAniForObject::aniImplCheckValid(void)
{
    if(!zfsuper::aniImplCheckValid()) {return zffalse;}
    return d->checkSetup(this->aniTarget(), this->name(), this->fromValue(), this->toValue());
}
void ZFAniForObject::aniOnStopOrInvalid(ZF_IN zfbool aniValid)
{
    d->cleanup();
    zfsuper::aniOnStopOrInvalid(aniValid);
}
void ZFAniForObject::aniTimeLineOnUpdate(ZF_IN zffloat progress)
{
    d->update(progress);
}
void ZFAniForObject::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFAniForGenericPrivate);
}
void ZFAniForObject::objectOnDealloc(void)
{
    zfpoolDelete(d);
    zfsuper::objectOnDealloc();
}

ZFOBJECT_REGISTER(ZFAniForGeneric)
ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFAniForGeneric, zfstring, name) {d->needUpdate = true;}
ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFAniForGeneric, zfstring, fromValue) {d->needUpdate = true;}
ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFAniForGeneric, zfstring, toValue) {d->needUpdate = true;}
void ZFAniForGeneric::aniImplTargetOnChange(ZF_IN ZFObject *aniTargetOld)
{
    zfsuper::aniImplTargetOnChange(aniTargetOld);
    d->needUpdate = zftrue;
}
zfbool ZFAniForGeneric::aniImplCheckValid(void)
{
    if(!zfsuper::aniImplCheckValid()) {return zffalse;}
    return d->checkSetup(this->aniTarget(), this->name(), this->fromValue(), this->toValue());
}
void ZFAniForGeneric::aniOnStopOrInvalid(ZF_IN zfbool aniValid)
{
    d->cleanup();
    zfsuper::aniOnStopOrInvalid(aniValid);
}
void ZFAniForGeneric::aniTimeLineOnUpdate(ZF_IN zffloat progress)
{
    d->update(progress);
}
void ZFAniForGeneric::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFAniForGenericPrivate);
}
void ZFAniForGeneric::objectOnDealloc(void)
{
    zfpoolDelete(d);
    zfsuper::objectOnDealloc();
}

ZF_NAMESPACE_GLOBAL_END

