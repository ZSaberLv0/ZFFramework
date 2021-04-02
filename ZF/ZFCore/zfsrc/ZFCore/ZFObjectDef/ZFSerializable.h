/**
 * @file ZFSerializable.h
 * @brief serializable interface
 */

#ifndef _ZFI_ZFSerializable_h_
#define _ZFI_ZFSerializable_h_

#include "ZFSerializableData.h"
#include "ZFIOCallback.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief keyword for #ZFSerializable to hold style,
 *   see #ZFStyleSet
 */
#define ZFSerializableKeyword_styleKey "styleKey"

/**
 * @brief used for #ZFSerializable to override default constructor
 *
 * by default, serializable would be created by #ZFClass::newInstance while serializing from data,
 * you may supply this method to override:
 * -  static zfautoObject serializableNewInstance(void);
 *
 * the method should be supplied as #ZFMethod\n
 * the method should return a newly created object, or retain your existing singleton instance\n
 * typically this method is used to achieve some singleton logic
 */
#define ZFSerializableKeyword_serializableNewInstanceId serializableNewInstance
/**
 * @brief see #ZFSerializableKeyword_serializableNewInstanceId
 */
#define ZFSerializableKeyword_serializableNewInstance ZFM_TOSTRING(ZFSerializableKeyword_serializableNewInstanceId)

// ============================================================
/**
 * @brief serializable property type, see #ZFSerializable::serializableOnCheckPropertyType
 */
typedef enum {
    /**
     * @brief see #ZFSerializable::serializableOnCheckPropertyType
     */
    ZFSerializablePropertyTypeNotSerializable,
    /**
     * @brief see #ZFSerializable::serializableOnCheckPropertyType
     */
    ZFSerializablePropertyTypeSerializableProperty,
    /**
     * @brief see #ZFSerializable::serializableOnCheckPropertyType
     */
    ZFSerializablePropertyTypeEmbededProperty,
} ZFSerializablePropertyType;

// ============================================================
zfclassFwd _ZFP_I_ZFSerializablePropertyTypeHolder;
/**
 * @brief base class of call serializable object
 *
 * a serializable object can be encoded to and decoded from a string data,
 * use #ZFSerializableData to store necessary data\n
 * a ZFSerializableData can hold these datas:
 * -  serializable class:
 *   ZFObject's class name or other non-ZFObject's type name,
 *   such as "v_zfstring", "zfstring" and "zfint"
 * -  property name:
 *   used only when the serializable belongs to another serializable,
 *   it's the property name,
 *   and is ensured stored in attributes with "name" as the attribute name
 * -  property value:
 *   used only when the serializable can be converted directly to a type,
 *   and is ensured stored in attributes with "value" as the attribute name
 * -  category:
 *   used to mark the node should be resolved specially,
 *   and is ensured stored in attributes with "category" as the attribute name
 * -  attributes:
 *   used to hold extra type datas for the owner
 * -  elements:
 *   used to hold properties of the serializable,
 *   it's a ZFSerializableData too
 *
 * ZFSerializableData can be converted from and to xml elements,
 * to make it easier to understand,
 * here's a typical serializable data in xml format that shows the types:
 * @code
 *   // assume we have a object hold a ZFArray as retain property:
 *   zfclass TestClass : zfextends ZFSerializable
 *   {
 *       ZFOBJECT_DECLARE(TestClass, ZFSerializable)
 *       ZFPROPERTY_RETAIN(ZFArray *, testProperty)
 *       ...
 *   };
 *
 *   // we have a ZFSerializableData like:
 *   <TestClass test="test">
 *       <ZFArray name="testProperty">
 *           <v_zfstring value="string content" />
 *       </ZFArray>
 *       <SomeType category="CategoryName" />
 *   </TestClass>
 * @endcode
 * in this example:
 * -  the "TestClass" in "<TestClass>" is a serializable class
 * -  the "testProperty" in "<ZFArray name="testProperty">" is a property name
 * -  the "string content" in "<v_zfstring value="string content" />" is a property value
 * -  the "test="test"" in "<TestClass test="test">" is a attribute
 * -  the "category" in "<SomeType category="CategoryName" />" is a category
 *   that should be resolved by subclass during #serializableOnSerializeFromData
 *
 * we have these builtin keywords for serializable data,
 * you should not use them as attribute name:
 * -  "name":
 *   shows the serializable is it's parent's property,
 *   and the name is the property's name
 * -  "value":
 *   for basic type only, the value for the basic type
 * -  "category":
 *   if exist this attribute,
 *   ZFSerializable would ignore this node and leave it to subclass to decode
 *
 * \n
 * a simplest way to implement ZFSerializable is:
 * -  declare your object, and make it implement ZFSerializable,
 *   by default, all #ZFProperty would be auto serialized
 *
 * \n
 * if your object has extra serialize step to do, you may want to:
 * -  override #serializableOnCheck to check serializable depending on current object state
 * -  override one or more of these methods to achieve custom serialize step:
 *   -  #serializableOnSerializePropertyFromData / #serializableOnSerializePropertyToData
 *   -  #serializableOnSerializeEmbededPropertyFromData / #serializableOnSerializeEmbededPropertyToData
 *   -  #serializableOnSerializeFromData / #serializableOnSerializeToData
 * -  take care of referencedOwnerOrNull while serialize to data,
 *   where you should implement reference logic for your custom serialize step\n
 *   see #ZFStyleSet for more info\n
 *   by default, serializable property and embeded property's reference logic
 *   would be done by ZFSerializable automatically,
 *   but you should take care of category's reference logic manually
 *
 *
 * \n
 * typically you should override
 * #serializableOnSerializeFromData and #serializableOnSerializeToData
 * to supply custom serialize step\n
 * \n
 * serializable logic can also be used with style logic,
 * see #ZFStyleSet for more info\n
 * \n
 * ADVANCED:\n
 * serializable would be created by #ZFClass::newInstance while serializing from data,
 * you may supply your custom constructor,
 * see #ZFSerializableKeyword_serializableNewInstance
 */
zfinterface ZF_ENV_EXPORT ZFSerializable : zfextends ZFInterface
{
    ZFINTERFACE_DECLARE(ZFSerializable, ZFInterface)

    // ============================================================
public:
    /**
     * @brief true if object is currently serializable, see #ZFSerializable
     *
     * subclass should override #serializableOnCheck to check whether serializable\n
     * some object may be serializable or not depends on content
     * @note you must check super's state first if override
     * @see ZFSerializable
     */
    zffinal zfbool serializable(void);
    /**
     * @brief serialize from data, see #ZFSerializable
     *
     * note that for performance, this method won't check whether serializable before execute
     * @note for convenient for script,
     *   you may supply a method named "serializableOnSerializeFromData" / "serializableOnSerializeToData",
     *   if you do so, the method would be called to perform serialization after default action\n
     *   the method's proto type should match #serializableOnSerializeFromData,
     *   and can be registered by ZFMETHOD_USER_REGISTER_N series
     *   or #ZFMethodDynamicRegister series\n
     *   typically, this is useful for script language with #ZFDynamic
     */
    zffinal zfbool serializeFromData(ZF_IN const ZFSerializableData &serializableData,
                                     ZF_OUT_OPT zfstring *outErrorHint = zfnull,
                                     ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull);
    /**
     * @brief serialize to data, see #ZFSerializable
     *
     * note that for performance, this method won't check whether serializable before execute
     */
    zffinal zfbool serializeToData(ZF_OUT ZFSerializableData &serializableData,
                                   ZF_OUT_OPT zfstring *outErrorHint = zfnull,
                                   ZF_IN_OPT ZFSerializable *referencedOwnerOrNull = zfnull);

    /**
     * @brief serialize from string,
     *   return false by default
     *
     * for most case, #serializeFromData would supply serialization logic automatically,
     * however, the serialization result may be quite verbose for the data structure,
     * subclass may override #serializableOnSerializeFromString to supply custom
     * serialization logic as short data structure
     * @note for convenient for script,
     *   you may supply a method named "serializableOnSerializeFromString" / "serializableOnSerializeToString",
     *   if you do so, the method would be called to perform serialization and replace the default action\n
     *   the method's proto type should match #serializableOnSerializeToData,
     *   and can be registered by ZFMETHOD_USER_REGISTER_N series
     *   or #ZFMethodDynamicRegister series\n
     *   typically, this is useful for script language with #ZFDynamic
     */
    zffinal zfbool serializeFromString(ZF_IN const zfchar *src);
    /** @brief see #serializeFromString */
    zffinal zfbool serializeToString(ZF_IN_OUT zfstring &ret);

private:
    zffinal _ZFP_I_ZFSerializablePropertyTypeHolder *_ZFP_ZFSerializable_getPropertyTypeHolder(void);
public:
    /** @brief see #serializableGetAllSerializableProperty */
    zffinal void serializableGetAllSerializablePropertyT(ZF_IN_OUT ZFCoreArray<const ZFProperty *> &ret);
    /**
     * @brief get all serializable property, usually for debug only, see #serializableOnCheckPropertyType
     */
    inline ZFCoreArrayPOD<const ZFProperty *> serializableGetAllSerializableProperty(void)
    {
        ZFCoreArrayPOD<const ZFProperty *> ret;
        this->serializableGetAllSerializablePropertyT(ret);
        return ret;
    }
    /** @brief see #serializableGetAllSerializableEmbededProperty */
    zffinal void serializableGetAllSerializableEmbededPropertyT(ZF_IN_OUT ZFCoreArray<const ZFProperty *> &ret);
    /**
     * @brief get all serializable embeded property, usually for debug only, see #serializableOnCheckPropertyType
     */
    inline ZFCoreArrayPOD<const ZFProperty *> serializableGetAllSerializableEmbededProperty(void)
    {
        ZFCoreArrayPOD<const ZFProperty *> ret;
        this->serializableGetAllSerializableEmbededPropertyT(ret);
        return ret;
    }

protected:
    /**
     * @brief check the property type that serializable should do what while serializing
     *
     * properties declared in ZFSerializalbe have these types:
     * -  not serializable:
     *   -  the property is not serializable and should be manually serialized if necessary
     * -  normal serializable property:
     *   -  the property would be serialized automatically
     *     during #serializableOnSerializePropertyFromData and #serializableOnSerializePropertyToData
     *   -  while serializing from data,
     *     ZFSerializable will serialize a property's new instance and then set to the property
     *   -  by default, a property is treated as normal serializable property if:
     *     -  the property's setter and getter is not private
     *     -  the property is retain property and its type is ZFSerializable
     *     -  the property is assign property and its type is registered by #ZFTYPEID_DECLARE
     * -  embeded serializable property:\n
     *   -  the property would be serialized automatically
     *     during #serializableOnSerializeEmbededPropertyFromData and #serializableOnSerializeEmbededPropertyToData
     *   -  while serializing from data,
     *     ZFSerializable will directly serialize the data to property instance
     *     (do nothing if property is null)
     *   -  by default, a property is treated as normal serializable property if:
     *     -  the property is retain property and its type is ZFSerializable
     *     -  the property's setter is private and getter is not private
     *   -  if a property is an embeded property,
     *     you must ensure it's not null while serializing,
     *     otherwise, serializing would fail
     *
     * \n
     * subclass may override this method to make ZFSerializable ignore or force serialize some property,
     * but you must make sure it's logical valid\n
     * ignored property (i.e. ZFSerializablePropertyTypeNotSerializable) can be manually serialized
     * during #serializableOnSerializeFromData and #serializableOnSerializeToData
     */
    virtual ZFSerializablePropertyType serializableOnCheckPropertyType(ZF_IN const ZFProperty *property);

protected:
    /**
     * @brief see #serializable
     */
    virtual zfbool serializableOnCheck(void)
    {
        return zftrue;
    }

    /**
     * @brief for serializable data that has "category" attribute,
     *   ZFSerializable would ignore it and leave it to subclass to resolve,
     *   see #ZFSerializable
     *
     * while overriding this method, you should call super first,
     * and then check whether super has resolved the data\n
     * if subclass should resolve the category,
     * you should mark data as resolved and return whether resolve success\n
     * if not, subclass should leave the data unresoved and return true
     */
    virtual inline zfbool serializableOnSerializeFromData(ZF_IN const ZFSerializableData &serializableData,
                                                          ZF_OUT_OPT zfstring *outErrorHint = zfnull,
                                                          ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull)
    {
        return zftrue;
    }
    /**
     * @brief corresponding to #serializableOnSerializeFromData,
     *   return whether the task is success,
     *   see #ZFSerializable
     */
    virtual inline zfbool serializableOnSerializeToData(ZF_IN_OUT ZFSerializableData &serializableData,
                                                        ZF_IN ZFSerializable *referencedOwnerOrNull,
                                                        ZF_OUT_OPT zfstring *outErrorHint = zfnull)
    {
        return zftrue;
    }
    /**
     * @brief see #serializableOnCheckPropertyType, usually you have no need to override this method,
     *   see #ZFSerializable
     *
     * if subclass override this method, you should check whether it's resolved by parent,
     * and then mark data as resolved and return whether resolve success
     * @code
     *   zfbool YourType::serializableOnSerializePropertyFromData(...)
     *   {
     *       if(!SuperSerializable::serializableOnSerializePropertyFromData(...))
     *       {
     *           return zffalse;
     *       }
     *       if(propertyData.resolved())
     *       {
     *           return zftrue;
     *       }
     *
     *       // mark resolve if you have resolved
     *       // or don't mark to leave it to subclass
     *       propertyData.resolveMark();
     *
     *       return zftrue;
     *   }
     * @endcode
     */
    virtual zfbool serializableOnSerializePropertyFromData(ZF_IN const ZFSerializableData &propertyData,
                                                           ZF_IN const ZFProperty *property,
                                                           ZF_OUT_OPT zfstring *outErrorHint = zfnull,
                                                           ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull);
    /**
     * @brief see #serializableOnCheckPropertyType, usually you have no need to override this method,
     *   see #ZFSerializable
     *
     * set serializable class to null to show the property is in init value state
     * and have no need to be serialized
     */
    virtual zfbool serializableOnSerializePropertyToData(ZF_OUT ZFSerializableData &propertyData,
                                                         ZF_IN const ZFProperty *property,
                                                         ZF_IN ZFSerializable *referencedOwnerOrNull,
                                                         ZF_OUT_OPT zfstring *outErrorHint = zfnull);
    /**
     * @brief see #serializableOnCheckPropertyType, usually you have no need to override this method,
     *   see #ZFSerializable
     */
    virtual zfbool serializableOnSerializeEmbededPropertyFromData(ZF_IN const ZFSerializableData &propertyData,
                                                                  ZF_IN const ZFProperty *property,
                                                                  ZF_OUT_OPT zfstring *outErrorHint = zfnull,
                                                                  ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull);
    /**
     * @brief see #serializableOnCheckPropertyType, usually you have no need to override this method,
     *   see #ZFSerializable
     *
     * set serializable class to null to show the property is in init value state
     * and have no need to be serialized
     */
    virtual zfbool serializableOnSerializeEmbededPropertyToData(ZF_OUT ZFSerializableData &propertyData,
                                                                ZF_IN const ZFProperty *property,
                                                                ZF_IN ZFSerializable *referencedOwnerOrNull,
                                                                ZF_OUT_OPT zfstring *outErrorHint = zfnull);

protected:
    /** @brief see #serializeFromString */
    virtual zfbool serializableOnSerializeFromString(ZF_IN const zfchar *src)
    {
        return zffalse;
    }
    /** @brief see #serializeFromString */
    virtual zfbool serializableOnSerializeToString(ZF_IN_OUT zfstring &ret)
    {
        return zffalse;
    }

public:
    /**
     * @brief get info as a serializable
     */
    virtual void serializableInfoT(ZF_IN_OUT zfstring &ret);
    /** @brief see #serializableInfoT */
    virtual inline zfstring serializableInfo(void)
    {
        zfstring ret;
        this->serializableInfoT(ret);
        return ret;
    }
};

// ============================================================
/**
 * @brief true if object is serializable
 *
 * note that null is treated as serializable
 */
extern ZF_ENV_EXPORT zfbool ZFObjectIsSerializable(ZF_IN ZFObject *obj);

// ============================================================
/**
 * @brief convenient method to #ZFSerializable::serializeFromData
 *
 * @note return null object doesn't necessarily mean fail,
 *   if the input is ZFSerializableKeyword_null,
 *   which describe a null object,
 *   the result would be null
 */
extern ZF_ENV_EXPORT zfautoObject ZFObjectFromData(ZF_IN const ZFSerializableData &serializableData,
                                                   ZF_OUT_OPT zfstring *outErrorHint = zfnull,
                                                   ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull);
/** @brief see #ZFObjectFromData */
extern ZF_ENV_EXPORT zfbool ZFObjectFromData(ZF_OUT zfautoObject &result,
                                             ZF_IN const ZFSerializableData &serializableData,
                                             ZF_OUT_OPT zfstring *outErrorHint = zfnull,
                                             ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull);
/** @brief see #ZFObjectFromData */
extern ZF_ENV_EXPORT zfbool ZFObjectToData(ZF_OUT ZFSerializableData &serializableData,
                                           ZF_IN ZFObject *obj,
                                           ZF_OUT_OPT zfstring *outErrorHint = zfnull,
                                           ZF_IN_OPT ZFSerializable *referencedOwnerOrNull = zfnull);
/** @brief see #ZFObjectFromData */
extern ZF_ENV_EXPORT ZFSerializableData ZFObjectToData(ZF_IN ZFObject *obj,
                                                       ZF_OUT_OPT zfbool *outSuccess = zfnull,
                                                       ZF_OUT_OPT zfstring *outErrorHint = zfnull,
                                                       ZF_IN_OPT ZFSerializable *referencedOwnerOrNull = zfnull);

// ============================================================
/**
 * @brief convenient method to #ZFSerializable::serializeFromString
 */
extern ZF_ENV_EXPORT zfbool ZFSerializeFromString(ZF_OUT zfautoObject &result,
                                                  ZF_IN const ZFClass *cls,
                                                  ZF_IN const zfchar *src);
/** @brief see #ZFSerializeFromString */
extern ZF_ENV_EXPORT zfautoObject ZFSerializeFromString(ZF_IN const ZFClass *cls,
                                                        ZF_IN const zfchar *src);
/** @brief see #ZFSerializeFromString */
extern ZF_ENV_EXPORT zfbool ZFSerializeToString(ZF_IN_OUT zfstring &ret,
                                                ZF_IN ZFObject *obj);
/** @brief see #ZFSerializeFromString */
extern ZF_ENV_EXPORT zfstring ZFSerializeToString(ZF_IN ZFObject *obj);

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFSerializable_h_

