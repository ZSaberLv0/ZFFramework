/**
 * @page DocTag_Feature_AutoSerialization Serialization without extra code
 *
 * unlike other serialization framework,
 * when you want to serialize your own subclass in ZFFramework,
 * typically there's no need to write any other serialization code,
 * all property can be serialized if it's type is serializable,
 * even for your custom type
 * \n
 * example:
 * @code
 *   zfclassNotPOD MyCppType
 *   {
 *   public:
 *       zfint myValue;
 *   };
 *   ZFTYPEID_DECLARE(MyCppType, MyCppType)
 *   zfclass MyClass : zfextends ZFStyleableObject
 *   {
 *       ZFOBJECT_DECLARE(MyClass, ZFStyleableObject)
 *       ZFPROPERTY_ASSIGN(zfstring, myProp0)
 *       ZFPROPERTY_ASSIGN(MyCppType, myProp1)
 *   };
 *
 *   ZFTYPEID_DEFINE_BY_STRING_CONVERTER(MyCppType, MyCppType, {
 *           // supply MyCppType's string conversion
 *           return zfintFromString(v.myValue, src, srcLen);
 *       }, {
 *           // supply MyCppType's string conversion
 *           return zfintToString(s, v.myValue);
 *       })
 * @endcode
 *
 * that's all, MyClass are serializable now,
 * as you can see, you have no need to write any code
 * to implement MyClass's serialization logic,
 * all you need to do is to ensure all your property's type are serializable\n
 * also, like MyCppType, all plain cpp type can be embeded to ZFFramework's
 * powerful serialization logic
 */

