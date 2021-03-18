/**
 * @page DocTag_Tutorial_Serializable Serializable
 *
 * to make an object serializable is easy:
 * @code
 *   // extends from any ZFObject type, and implements from ZFSerializable
 *   zfclass MyObject : zfextends MyParent, zfimplements ZFSerializable
 *   {
 *       ZFOBJECT_DECLARE(MyObject, MyParent)
 *       ZFIMPLEMENTS_DECLARE(ZFSerializable)
 *
 *       // normal property value would be serialized automatically
 *       ZFPROPERTY_ASSIGN(zfindex, myProperty)
 *
 *       // object type property can be serialized automatically
 *       // if it's serializable
 *       ZFPROPERTY_RETAIN(SomeSerializable *, mySerializableProperty)
 *   };
 * @endcode
 *
 * once declared, the object can be serialized by:
 * @code
 *   zfblockedAlloc(MyObject, obj);
 *
 *   // serialize to an data holder,
 *   // which can be converted to raw data and write to file,
 *   // you may also convert it to xml or json or other data formats
 *   ZFSerializableData data = ZFObjectToData(obj);
 *
 *   // serialize object from existing data
 *   zfautoObject serializedObject = ZFObjectFromData(data);
 * @endcode
 *
 * @section DocTag_Tutorial_Serializable_Advanced Advanced
 * if any of contents of your object can't be serialized automatically,
 * you may override some methods to supply your own serialize logic:
 * @code
 *   zfclass MyObject : zfextends ZFObject, zfimplements ZFSerializable
 *   {
 *       ZFOBJECT_DECLARE(MyObject, ZFObject)
 *       ZFIMPLEMENTS_DECLARE(ZFSerializable)
 *
 *   protected:
 *       virtual zfbool serializableOnSerializeFromData(ZF_IN const ZFSerializableData &serializableData,
 *                                                      ZF_OUT_OPT zfstring *outErrorHint = zfnull,
 *                                                      ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull)
 *       {
 *           if(!zfsuperI(ZFSerializable)::serializableOnSerializeFromData(serializableData, outErrorHint, outErrorPos)) {return zffalse;}
 *
 *           // serialize your type from serializableData
 *           // recommended to use ZFSerializableUtilSerializeAttributeFromData series
 *           return zftrue;
 *       }
 *       virtual zfbool serializableOnSerializeToData(ZF_IN_OUT ZFSerializableData &serializableData,
 *                                                    ZF_IN ZFSerializable *referencedOwnerOrNull,
 *                                                    ZF_OUT_OPT zfstring *outErrorHint = zfnull)
 *       {
 *           if(!zfsuperI(ZFSerializable)::serializableOnSerializeToData(serializableData, outErrorHint, outErrorPos)) {return zffalse;}
 *
 *           // serialize your type to serializableData
 *           // recommended to use ZFSerializableUtilSerializeAttributeFromData series
 *           return zftrue;
 *       }
 *   private:
 *       YourNotSerializableType data;
 *   };
 * @endcode
 * for detailed usage, please refer to #ZFSerializable
 */

