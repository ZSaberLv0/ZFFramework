#include "ZFSerializable.h"
#include "ZFObjectImpl.h"
#include "ZFSerializableDataSerializableConverter.h"
#include "ZFPropertyUtil.h"

#include "ZFCore/ZFSTLWrapper/zfstl_string.h"
#include "ZFCore/ZFSTLWrapper/zfstl_map.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// cache in class's meta data for performance
zfclassNotPOD _ZFP_ZFSerializable_PropertyTypeData
{
public:
    const ZFProperty *property;
    ZFSerializablePropertyType propertyType;
};
zfclass _ZFP_I_ZFSerializablePropertyTypeHolder : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_I_ZFSerializablePropertyTypeHolder, ZFObject)
public:
    ZFCoreArrayPOD<_ZFP_ZFSerializable_PropertyTypeData *> serializableProperty;
    ZFCoreMap serializablePropertyMap; // _ZFP_ZFSerializable_PropertyTypeData *

public:
    void addData(ZF_IN const ZFProperty *property,
                 ZF_IN ZFSerializablePropertyType propertyType)
    {
        _ZFP_ZFSerializable_PropertyTypeData *typeData = zfnew(_ZFP_ZFSerializable_PropertyTypeData);
        typeData->property = property;
        typeData->propertyType = propertyType;

        this->serializableProperty.add(typeData);
        this->serializablePropertyMap.set(property->propertyName(),
            ZFCorePointerForObject<_ZFP_ZFSerializable_PropertyTypeData *>(typeData));
    }
};

// ============================================================
// ZFSerializable
zfbool ZFSerializable::serializable(void)
{
    return this->serializableOnCheck();
}
zfbool ZFSerializable::serializeFromData(ZF_IN const ZFSerializableData &serializableData,
                                         ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */,
                                         ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */)
{
    // style logic
    ZFStyleable *styleable = ZFCastZFObject(ZFStyleable *, this);
    {
        const zfchar *styleKey = ZFSerializableUtil::checkAttribute(serializableData, ZFSerializableKeyword_styleKey);
        if(styleKey != zfnull)
        {
            if(styleable == zfnull)
            {
                ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
                    "styleKey exists but the object is not ZFStyleable: %s",
                    this->toObject()->objectInfoOfInstance().cString());
                return zffalse;
            }
            styleable->styleKey(styleKey);
        }
    }

    // property
    {
        const ZFCoreMap &propertyMap = this->_ZFP_ZFSerializable_getPropertyTypeHolder()->serializablePropertyMap;
        for(zfindex i = 0; i < serializableData.elementCount(); ++i)
        {
            const ZFSerializableData &element = serializableData.elementAtIndex(i);
            if(element.resolved() || element.category() != zfnull)
            {
                continue;
            }
            const zfchar *propertyName = ZFSerializableUtil::checkPropertyName(element);
            if(propertyName == zfnull)
            {
                continue;
            }
            _ZFP_ZFSerializable_PropertyTypeData *data = propertyMap.get<_ZFP_ZFSerializable_PropertyTypeData *>(propertyName);
            if(data == zfnull)
            {
                continue;
            }

            // serialize the property
            switch(data->propertyType)
            {
                case ZFSerializablePropertyTypeSerializableProperty:
                    {
                        const zfchar *styleKey = ZFSerializableUtil::checkAttribute(element, ZFSerializableKeyword_styleKey);
                        if(styleKey != zfnull)
                        {
                            if(styleable == zfnull)
                            {
                                ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
                                    "styleKey exists in property %s but the object is not ZFStyleable: %s",
                                    data->property->objectInfo().cString(),
                                    this->toObject()->objectInfoOfInstance().cString());
                                return zffalse;
                            }
                            styleable->styleKeyForProperty(data->property, styleKey);
                            break;
                        }
                    }
                    if(!this->serializableOnSerializePropertyFromData(
                        element,
                        data->property,
                        outErrorHint,
                        outErrorPos))
                    {
                        return zffalse;
                    }
                    break;
                case ZFSerializablePropertyTypeEmbededProperty:
                    if(!this->serializableOnSerializeEmbededPropertyFromData(
                        element,
                        data->property,
                        outErrorHint,
                        outErrorPos))
                    {
                        return zffalse;
                    }
                    break;
                case ZFSerializablePropertyTypeNotSerializable:
                default:
                    zfCoreCriticalShouldNotGoHere();
                    return zffalse;
            }
        }
    }

    // subclass
    if(!this->serializableOnSerializeFromData(serializableData, outErrorHint, outErrorPos))
    {
        return zffalse;
    }

    // mark self as resolved
    serializableData.resolveMark();

    // dynamic
    const ZFMethod *dynamicMethod = this->classData()->methodForName("serializableOnSerializeFromData");
    if(dynamicMethod != zfnull
        && !dynamicMethod->execute<zfbool, const ZFSerializableData &, zfstring *, ZFSerializableData *>(
            this->toObject(), serializableData, outErrorHint, outErrorPos))
    {
        return zffalse;
    }

    // check whether all resoved
    if(ZFSerializableDataResolveCheckEnable)
    {
        zfstring tmp;
        if(ZFSerializableUtil::printResolveStatus(serializableData, ZFOutputForString(tmp)))
        {
            #if ZF_ENV_DEBUG
                zfCoreCriticalMessageTrim("%s", tmp.cString());
            #endif
        }
    }

    return zftrue;
}
zfbool ZFSerializable::serializeToData(ZF_OUT ZFSerializableData &serializableData,
                                       ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */,
                                       ZF_IN_OPT ZFSerializable *referencedOwnerOrNull /* = zfnull */)
{
    zfautoObject referencedObjectHolder;
    ZFSerializable *referencedObject = zfnull;
    ZFStyleable *styleable = ZFCastZFObject(ZFStyleable *, this);
    if(styleable != zfnull && styleable->styleKey() != zfnull)
    {
        referencedObjectHolder = ZFStyleGet(styleable->styleKey());
        referencedObject = referencedObjectHolder;
        serializableData.attributeForName(ZFSerializableKeyword_styleKey, styleable->styleKey());
    }
    if(referencedObject == zfnull)
    {
        referencedObject = referencedOwnerOrNull;
    }

    if(referencedObject != zfnull
        && !referencedObject->classData()->classIsTypeOf(this->classData())
        && !this->classData()->classIsTypeOf(referencedObject->classData()))
    {
        ZFSerializableUtil::errorOccurred(outErrorHint,
            "serialize with a reference style object whose type mismatch, self: %s, style: %s",
            this->classData()->classNameFull(), referencedObject->classData()->classNameFull());
        return zffalse;
    }

    // property
    {
        const ZFCoreArrayPOD<_ZFP_ZFSerializable_PropertyTypeData *> &allProperty = this->_ZFP_ZFSerializable_getPropertyTypeHolder()->serializableProperty;
        for(zfindex i = 0; i < allProperty.count(); ++i)
        {
            _ZFP_ZFSerializable_PropertyTypeData *data = allProperty[i];
            ZFSerializableData propertyData;

            switch(data->propertyType)
            {
                case ZFSerializablePropertyTypeSerializableProperty:
                    if(styleable != zfnull)
                    {
                        const zfchar *styleKey = styleable->styleKeyForProperty(data->property);
                        if(styleKey != zfnull)
                        {
                            propertyData.itemClass(data->property->propertyTypeId());
                            propertyData.attributeForName(ZFSerializableKeyword_styleKey, styleKey);
                            break;
                        }
                    }
                    if(!this->serializableOnSerializePropertyToData(propertyData,
                                                                    data->property,
                                                                    referencedObject,
                                                                    outErrorHint))
                    {
                        return zffalse;
                    }
                    break;
                case ZFSerializablePropertyTypeEmbededProperty:
                    if(!this->serializableOnSerializeEmbededPropertyToData(propertyData,
                                                                           data->property,
                                                                           referencedObject,
                                                                           outErrorHint))
                    {
                        return zffalse;
                    }
                    break;
                case ZFSerializablePropertyTypeNotSerializable:
                default:
                    zfCoreCriticalShouldNotGoHere();
                    return zffalse;
            }

            if(propertyData.itemClass() != zfnull)
            {
                propertyData.propertyName(data->property->propertyName());
                serializableData.elementAdd(propertyData);
            }
        }
    }

    // subclass
    if(!this->serializableOnSerializeToData(serializableData, referencedObject, outErrorHint))
    {
        return zffalse;
    }

    serializableData.itemClass(this->classData()->classNameFull());

    // dynamic
    const ZFMethod *dynamicMethod = this->classData()->methodForName("serializableOnSerializeToData");
    if(dynamicMethod != zfnull
        && !dynamicMethod->execute<zfbool, ZFSerializableData &, ZFSerializable *, zfstring *>(
            this->toObject(), serializableData, referencedObject, outErrorHint))
    {
        return zffalse;
    }

    return zftrue;
}

zffinal zfbool ZFSerializable::serializeFromString(ZF_IN const zfchar *src)
{
    // dynamic
    const ZFMethod *dynamicMethod = this->classData()->methodForName("serializableOnSerializeFromString");
    if(dynamicMethod != zfnull
        && !dynamicMethod->execute<zfbool, const zfchar *>(this->toObject(), src))
    {
        return zffalse;
    }
    return this->serializableOnSerializeFromString(src);
}
zffinal zfbool ZFSerializable::serializeToString(ZF_IN_OUT zfstring &ret)
{
    // dynamic
    const ZFMethod *dynamicMethod = this->classData()->methodForName("serializableOnSerializeToString");
    if(dynamicMethod != zfnull
        && !dynamicMethod->execute<zfbool, zfstring &>(this->toObject(), ret))
    {
        return zffalse;
    }
    return this->serializableOnSerializeToString(ret);
}

_ZFP_I_ZFSerializablePropertyTypeHolder *ZFSerializable::_ZFP_ZFSerializable_getPropertyTypeHolder(void)
{
    zfCoreMutexLocker();
    _ZFP_I_ZFSerializablePropertyTypeHolder *holder = this->classData()->classTag<_ZFP_I_ZFSerializablePropertyTypeHolder *>(_ZFP_I_ZFSerializablePropertyTypeHolder::ClassData()->classNameFull());
    if(holder == zfnull)
    {
        zflockfree_zfblockedAlloc(_ZFP_I_ZFSerializablePropertyTypeHolder, holderTmp);
        ZFCoreArrayPOD<const ZFProperty *> allProperty;
        {
            // the order affects the serialization step's order,
            // sort parent's property first for human-readable
            ZFCoreArrayPOD<const ZFClass *> allClass;
            const ZFClass *tmpCls = this->classData();
            allClass.add(tmpCls);
            do
            {
                for(zfindex i = tmpCls->implementedInterfaceCount() - 1; i != zfindexMax(); --i)
                {
                    allClass.add(tmpCls->implementedInterfaceAtIndex(i));
                }
                if(tmpCls->classParent() != zfnull)
                {
                    allClass.add(tmpCls->classParent());
                }
                tmpCls = tmpCls->classParent();
            } while(tmpCls != zfnull);
            for(zfindex i = allClass.count() - 1; i != zfindexMax(); --i)
            {
                tmpCls = allClass[i];
                for(zfindex iProperty = 0; iProperty < tmpCls->propertyCount(); ++iProperty)
                {
                    allProperty.add(tmpCls->propertyAtIndex(iProperty));
                }
            }
        }

        for(zfindex i = 0; i < allProperty.count(); ++i)
        {
            const ZFProperty *property = allProperty[i];
            switch(this->serializableOnCheckPropertyType(property))
            {
                case ZFSerializablePropertyTypeNotSerializable:
                    break;
                case ZFSerializablePropertyTypeSerializableProperty:
                    holderTmp->addData(property, ZFSerializablePropertyTypeSerializableProperty);
                    break;
                case ZFSerializablePropertyTypeEmbededProperty:
                    holderTmp->addData(property, ZFSerializablePropertyTypeEmbededProperty);
                    break;
                default:
                    zfCoreCriticalShouldNotGoHere();
                    return zfnull;
            }
        }

        holder = holderTmp;
        this->classData()->classTag(
            _ZFP_I_ZFSerializablePropertyTypeHolder::ClassData()->classNameFull(),
            holderTmp);
        this->classData()->classDataChangeAutoRemoveTagAdd(_ZFP_I_ZFSerializablePropertyTypeHolder::ClassData()->classNameFull());
    }
    return holder;
}
void ZFSerializable::serializableGetAllSerializablePropertyT(ZF_IN_OUT ZFCoreArray<const ZFProperty *> &ret)
{
    const ZFCoreArrayPOD<_ZFP_ZFSerializable_PropertyTypeData *> &tmp = this->_ZFP_ZFSerializable_getPropertyTypeHolder()->serializableProperty;
    for(zfindex i = 0; i < tmp.count(); ++i)
    {
        if(tmp[i]->propertyType == ZFSerializablePropertyTypeSerializableProperty)
        {
            ret.add(tmp[i]->property);
        }
    }
}
void ZFSerializable::serializableGetAllSerializableEmbededPropertyT(ZF_IN_OUT ZFCoreArray<const ZFProperty *> &ret)
{
    const ZFCoreArrayPOD<_ZFP_ZFSerializable_PropertyTypeData *> &tmp = this->_ZFP_ZFSerializable_getPropertyTypeHolder()->serializableProperty;
    for(zfindex i = 0; i < tmp.count(); ++i)
    {
        if(tmp[i]->propertyType == ZFSerializablePropertyTypeEmbededProperty)
        {
            ret.add(tmp[i]->property);
        }
    }
}

ZFSerializablePropertyType ZFSerializable::serializableOnCheckPropertyType(ZF_IN const ZFProperty *property)
{
    if(property->propertyIsRetainProperty())
    {
        if(property->setterMethod()->methodPrivilegeType() == ZFMethodPrivilegeTypePrivate)
        {
            if(property->getterMethod()->methodPrivilegeType() == ZFMethodPrivilegeTypePrivate
                || !property->propertyClassOfRetainProperty()->classIsTypeOf(ZFSerializable::ClassData()))
            {
                return ZFSerializablePropertyTypeNotSerializable;
            }
            else
            {
                return ZFSerializablePropertyTypeEmbededProperty;
            }
        }
        else
        {
            if(property->getterMethod()->methodPrivilegeType() == ZFMethodPrivilegeTypePrivate)
            {
                return ZFSerializablePropertyTypeNotSerializable;
            }
            else
            {
                return ZFSerializablePropertyTypeSerializableProperty;
            }
        }
    }
    else
    {
        if(!property->propertySerializable()
            || property->getterMethod()->methodPrivilegeType() == ZFMethodPrivilegeTypePrivate)
        {
            return ZFSerializablePropertyTypeNotSerializable;
        }
        else
        {
            return ZFSerializablePropertyTypeSerializableProperty;
        }
    }
}

zfbool ZFSerializable::serializableOnSerializePropertyFromData(ZF_IN const ZFSerializableData &propertyData,
                                                               ZF_IN const ZFProperty *property,
                                                               ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */,
                                                               ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */)
{
    if(property->propertyIsRetainProperty())
    {
        zfautoObject obj;
        if(!ZFObjectFromData(obj, propertyData, outErrorHint, outErrorPos))
        {
            return zffalse;
        }
        if(obj != zfnull && !obj.toObject()->classData()->classIsTypeOf(property->propertyClassOfRetainProperty()))
        {
            ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, propertyData,
                "object %s not type of %s",
                obj.toObject()->objectInfoOfInstance().cString(), property->propertyClassOfRetainProperty()->classNameFull());
            return zffalse;
        }
        property->setterMethod()->methodGenericInvoke(this->toObject(), obj);
        return zftrue;
    }
    else
    {
        const ZFTypeInfo *typeId = ZFTypeInfoForName(property->propertyTypeId());
        if(typeId == zfnull)
        {
            ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, propertyData,
                "property %s is not serializable",
                property->objectInfo().cString());
            return zffalse;
        }

        const ZFClass *propertyClass = typeId->typeIdClass();
        if(propertyClass == zfnull || propertyClass->classIsAbstract())
        {
            ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, propertyData,
                "property %s is not serializable",
                property->propertyName());
            return zffalse;
        }
        zfautoObject propertyValue = propertyClass->newInstance();
        if(!ZFCastZFObject(ZFTypeIdWrapper *, propertyValue)->wrappedValueFromData(propertyData, outErrorHint, outErrorPos))
        {
            return zffalse;
        }
        property->setterMethod()->methodGenericInvoke(this->toObject(), propertyValue);
        return zftrue;
    }
}
zfbool ZFSerializable::serializableOnSerializePropertyToData(ZF_OUT ZFSerializableData &propertyData,
                                                             ZF_IN const ZFProperty *property,
                                                             ZF_IN ZFSerializable *referencedOwnerOrNull,
                                                             ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */)
{
    if(referencedOwnerOrNull != zfnull
        && ZFPropertyCompare(property, this->toObject(), referencedOwnerOrNull->toObject()) == ZFCompareTheSame)
    {
        return zftrue;
    }
    if(property->callbackIsInitValue(property, this->toObject(), zfnull))
    {
        return zftrue;
    }

    if(property->propertyIsRetainProperty())
    {
        return ZFObjectToData(propertyData, property->getterMethod()->methodGenericInvoke(this->toObject()), outErrorHint);
    }
    else
    {
        zfautoObject propertyValue = property->getterMethod()->methodGenericInvoke(this->toObject());
        if(propertyValue == zfnull)
        {
            ZFSerializableUtil::errorOccurred(outErrorHint,
                "property %s is not serializable",
                property->propertyName());
            return zffalse;
        }
        return ZFCastZFObject(ZFTypeIdWrapper *, propertyValue)->wrappedValueToData(propertyData, outErrorHint);
    }
}
zfbool ZFSerializable::serializableOnSerializeEmbededPropertyFromData(ZF_IN const ZFSerializableData &propertyData,
                                                                      ZF_IN const ZFProperty *property,
                                                                      ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */,
                                                                      ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */)
{
    zfautoObject obj = property->getterMethod()->methodGenericInvoke(this->toObject());
    if(obj == zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHint,
            "embeded property %s is null while serializing \"%s\"",
            property->propertyName(),
            this->classData()->classNameFull());
        return zffalse;
    }
    else if(!ZFObjectIsSerializable(obj))
    {
        ZFSerializableUtil::errorOccurred(outErrorHint,
            "not serializable object %s while serializing \"%s\"",
            obj->objectInfoOfInstance().cString(),
            this->classData()->classNameFull());
        return zffalse;
    }
    else
    {
        const ZFClass *cls = ZFClass::classForName(propertyData.itemClass());
        if(cls == zfnull)
        {
            ZFSerializableUtil::errorOccurred(outErrorHint,
                "no class named %s while serializing \"%s\"'s property %s",
                propertyData.itemClass(),
                this->classData()->classNameFull(),
                property->propertyName());
            return zffalse;
        }
        else if(!cls->classIsTypeOf(property->propertyClassOfRetainProperty()))
        {
            ZFSerializableUtil::errorOccurred(outErrorHint,
                "node %s is not type of %s while serializing \"%s\"'s property %s",
                propertyData.itemClass(),
                property->propertyClassOfRetainProperty()->classNameFull(),
                this->classData()->classNameFull(),
                property->propertyName());
            return zffalse;
        }
    }
    return ZFCastZFObjectUnchecked(zfself *, obj)->serializeFromData(propertyData, outErrorHint, outErrorPos);
}
zfbool ZFSerializable::serializableOnSerializeEmbededPropertyToData(ZF_OUT ZFSerializableData &propertyData,
                                                                    ZF_IN const ZFProperty *property,
                                                                    ZF_IN ZFSerializable *referencedOwnerOrNull,
                                                                    ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */)
{
    if(referencedOwnerOrNull != zfnull
        && ZFPropertyCompare(property, this->toObject(), referencedOwnerOrNull->toObject()) == ZFCompareTheSame)
    {
        return zftrue;
    }
    zfautoObject initValue;
    if(property->callbackIsInitValue(property, this->toObject(), &initValue))
    {
        return zftrue;
    }

    zfautoObject obj = property->getterMethod()->methodGenericInvoke(this->toObject());
    if(obj == zfnull || !ZFObjectIsSerializable(obj))
    {
        return zftrue;
    }

    ZFSerializable *propertyRef = zfnull;
    if(referencedOwnerOrNull != zfnull)
    {
        zfautoObject t = property->getterMethod()->methodGenericInvoke(referencedOwnerOrNull->toObject());
        if(t != zfnull)
        {
            propertyRef = t->to<ZFSerializable *>();
        }
    }
    if(propertyRef == zfnull)
    {
        propertyRef = initValue;
    }

    if(!obj->to<zfself *>()->serializeToData(propertyData, outErrorHint, propertyRef))
    {
        return zffalse;
    }

    if(propertyData.elementCount() == 0
        && propertyData.attributeCount() == 0)
    {
        propertyData.itemClass(zfnull);
    }
    return zftrue;
}

void ZFSerializable::serializableInfoT(ZF_IN_OUT zfstring &ret)
{
    ZFSerializableData serializableData;
    if(!this->serializable()
       || !this->serializeToData(serializableData))
    {
        ret += ZFTOKEN_ZFObjectInfoLeft;
        this->toObject()->objectInfoOfInstanceT(ret);
        ret += ": not serializable";
        ret += ZFTOKEN_ZFObjectInfoRight;
    }
    else
    {
        serializableData.objectInfoT(ret);
    }
}

// ============================================================
zfbool ZFObjectIsSerializable(ZF_IN ZFObject *obj)
{
    if(obj == zfnull)
    {
        return zftrue;
    }
    else
    {
        ZFSerializable *tmp = ZFCastZFObject(ZFSerializable *, obj);
        return (tmp != zfnull && tmp->serializable());
    }
}

// ============================================================
zfbool ZFObjectFromData(ZF_OUT zfautoObject &result,
                        ZF_IN const ZFSerializableData &serializableData,
                        ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */,
                        ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */)
{
    result = zfnull;

    const zfchar *serializableClass = ZFSerializableUtil::requireItemClass(serializableData, ZFTypeId_none(), outErrorHint, outErrorPos);
    if(serializableClass == zfnull)
    {
        return zffalse;
    }
    if(zfscmpTheSame(serializableClass, ZFSerializableKeyword_null))
    {
        serializableData.resolveMark();
        return zftrue;
    }

    const ZFClass *cls = ZFClass::classForName(serializableClass);
    if(cls == zfnull)
    {
        if(cls == zfnull)
        {
            ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
                "no class named \"%s\"",
                serializableData.itemClass());
            return zffalse;
        }
    }
    zfautoObject obj;
    {
        const ZFMethod *overridedCreateMethod = cls->methodForNameIgnoreParent(ZFSerializableKeyword_serializableNewInstance);
        if(overridedCreateMethod != zfnull)
        {
            obj = overridedCreateMethod->execute<zfautoObject>(zfnull);
        }
        else
        {
            obj = cls->newInstance();
        }
    }
    ZFSerializable *tmp = ZFCastZFObject(ZFSerializable *, obj.toObject());
    if(tmp == zfnull || !tmp->serializable())
    {
        ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
            "object %s not serializable",
            ZFObjectInfoOfInstance(obj.toObject()).cString());
        return zffalse;
    }
    if(!tmp->serializeFromData(serializableData, outErrorHint, outErrorPos))
    {
        return zffalse;
    }
    result = obj;
    return zftrue;
}
zfautoObject ZFObjectFromData(ZF_IN const ZFSerializableData &serializableData,
                              ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */,
                              ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */)
{
    zfautoObject result;
    ZFObjectFromData(result, serializableData, outErrorHint, outErrorPos);
    return result;
}
zfbool ZFObjectToData(ZF_OUT ZFSerializableData &serializableData,
                      ZF_IN ZFObject *obj,
                      ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */,
                      ZF_IN_OPT ZFSerializable *referencedOwnerOrNull /* = zfnull */)
{
    if(obj == zfnull)
    {
        serializableData.itemClass(ZFSerializableKeyword_null);
        return zftrue;
    }
    ZFSerializable *tmp = ZFCastZFObject(ZFSerializable *, obj);
    if(tmp == zfnull || !tmp->serializable())
    {
        ZFSerializableUtil::errorOccurred(outErrorHint,
            "object %s not serializable",
            obj->objectInfoOfInstance().cString());
        return zffalse;
    }
    return tmp->serializeToData(serializableData, outErrorHint, referencedOwnerOrNull);
}
ZFSerializableData ZFObjectToData(ZF_IN ZFObject *obj,
                                  ZF_OUT_OPT zfbool *outSuccess /* = zfnull */,
                                  ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */,
                                  ZF_IN_OPT ZFSerializable *referencedOwnerOrNull /* = zfnull */)
{
    ZFSerializableData serializableData;
    zfbool success = ZFObjectToData(serializableData, obj, outErrorHint, referencedOwnerOrNull);
    if(outSuccess != zfnull)
    {
        *outSuccess = success;
    }
    if(success)
    {
        return serializableData;
    }
    return ZFSerializableData();
}

zfbool ZFSerializeFromString(ZF_OUT zfautoObject &result,
                             ZF_IN const ZFClass *cls,
                             ZF_IN const zfchar *src)
{
    if(cls == zfnull || !cls->classIsTypeOf(ZFSerializable::ClassData()))
    {
        return zffalse;
    }
    const ZFMethod *createMethod = cls->methodForName(ZFSerializableKeyword_serializableNewInstance);
    if(createMethod == zfnull)
    {
        result = cls->newInstance();
    }
    else
    {
        result = createMethod->execute<zfautoObject>(zfnull);
    }
    ZFSerializable *serializable = result;
    if(serializable == zfnull)
    {
        result = zfnull;
        return zffalse;
    }
    if(!serializable->serializeFromString(src))
    {
        result = zfnull;
        return zffalse;
    }
    else
    {
        return zftrue;
    }
}
zfautoObject ZFSerializeFromString(ZF_IN const ZFClass *cls,
                                   ZF_IN const zfchar *src)
{
    zfautoObject ret;
    ZFSerializeFromString(ret, cls, src);
    return ret;
}
zfbool ZFSerializeToString(ZF_IN_OUT zfstring &ret,
                           ZF_IN ZFObject *obj)
{
    if(obj == zfnull)
    {
        return zftrue;
    }
    ZFSerializable *t = ZFCastZFObject(ZFSerializable *, obj);
    if(t == zfnull)
    {
        return zffalse;
    }
    return t->serializeToString(ret);
}
zfstring ZFSerializeToString(ZF_IN ZFObject *obj)
{
    zfstring ret;
    ZFSerializeToString(ret, obj);
    return ret;
}

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFSerializable, zfbool, serializable)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_3(ZFSerializable, zfbool, serializeFromData, ZFMP_IN(const ZFSerializableData &, serializableData), ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull), ZFMP_OUT_OPT(ZFSerializableData *, outErrorPos, zfnull))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_3(ZFSerializable, zfbool, serializeToData, ZFMP_OUT(ZFSerializableData &, serializableData), ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull), ZFMP_IN_OPT(ZFSerializable *, referencedOwnerOrNull, zfnull))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFSerializable, zfbool, serializeFromString, ZFMP_IN(const zfchar *, src))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFSerializable, zfbool, serializeToString, ZFMP_IN_OUT(zfstring &, ret))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFSerializable, void, serializableGetAllSerializablePropertyT, ZFMP_IN_OUT(ZFCoreArray<const ZFProperty *> &, ret))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFSerializable, ZFCoreArrayPOD<const ZFProperty *>, serializableGetAllSerializableProperty)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFSerializable, void, serializableGetAllSerializableEmbededPropertyT, ZFMP_IN_OUT(ZFCoreArray<const ZFProperty *> &, ret))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFSerializable, ZFCoreArrayPOD<const ZFProperty *>, serializableGetAllSerializableEmbededProperty)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFSerializable, void, serializableInfoT, ZFMP_IN_OUT(zfstring &, ret))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFSerializable, zfstring, serializableInfo)

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(zfbool, ZFObjectIsSerializable, ZFMP_IN(ZFObject *, obj))

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zfautoObject, ZFObjectFromData, ZFMP_IN(const ZFSerializableData &, serializableData), ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull), ZFMP_OUT_OPT(ZFSerializableData *, outErrorPos, zfnull))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_4(zfbool, ZFObjectFromData, ZFMP_OUT(zfautoObject &, result), ZFMP_IN(const ZFSerializableData &, serializableData), ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull), ZFMP_OUT_OPT(ZFSerializableData *, outErrorPos, zfnull))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_4(zfbool, ZFObjectToData, ZFMP_OUT(ZFSerializableData &, serializableData), ZFMP_IN(ZFObject *, obj), ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull), ZFMP_IN_OPT(ZFSerializable *, referencedOwnerOrNull, zfnull))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_4(ZFSerializableData, ZFObjectToData, ZFMP_IN(ZFObject *, obj), ZFMP_OUT_OPT(zfbool *, outSuccess, zfnull), ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull), ZFMP_IN_OPT(ZFSerializable *, referencedOwnerOrNull, zfnull))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zfbool, ZFSerializeFromString, ZFMP_OUT(zfautoObject &, result), ZFMP_IN(const ZFClass *, cls), ZFMP_IN(const zfchar *, src))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfautoObject, ZFSerializeFromString, ZFMP_IN(const ZFClass *, cls), ZFMP_IN(const zfchar *, src))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfbool, ZFSerializeToString, ZFMP_IN_OUT(zfstring &, ret), ZFMP_IN(ZFObject *, obj))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(zfstring, ZFSerializeToString, ZFMP_IN(ZFObject *, obj))

ZF_NAMESPACE_GLOBAL_END
#endif

