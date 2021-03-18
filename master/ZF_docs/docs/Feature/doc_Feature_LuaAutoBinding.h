/**
 * @page DocTag_Feature_LuaAutoBinding Automatically lua binding
 *
 * thanks to powerful reflection logic of ZFFramework
 * (see also @ref DocTag_Tutorial_Reflection),
 * we are able to achieve automatically lua binding without any other code\n
 * all reflectable #ZFClass/#ZFMethod/#ZFProperty
 * would be binded to lua automatically,
 * even including anything that are dynamically loaded from library\n
 * \n
 * example:
 * @code
 *   // in C++
 *   ZFMETHOD_FUNC_DECLARE_1(zfstring, myFunc, ZFMP_IN(const zfchar *, param))
 *   ZFMETHOD_FUNC_DEFINE_1(zfstring, myFunc, ZFMP_IN(const zfchar *, param))
 *   {
 *       return param;
 *   }
 *
 *   // in lua, just use the function directly
 *   local ret = myFunc(param);
 * @endcode
 *
 * example:
 * @code
 *   // in C++
 *   zfclass MyClass : zfextends ZFObject
 *   {
 *       ZFOBJECT_DECLARE(MyClass, ZFObject)
 *       ZFPROPERTY_ASSIGN(ZFUISize, myProp)
 *   };
 *
 *   // in lua
 *   local obj = MyClass();
 *   local value = obj:myProp();
 * @endcode
 *
 * param and return supports any type that are registered by #ZFTYPEID_DECLARE
 * @code
 *   // header file
 *   class YourType
 *   {
 *   public:
 *       int yourProp;
 *   };
 *
 *   ZFTYPEID_DECLARE(YourType, YourType)
 *
 *   // source file
 *   ZFTYPEID_DEFINE_BY_STRING_CONVERTER(YourType, YourType, {
 *           v.yourProp = yourParser(src, srcLen);
 *           return zftrue;
 *       }, {
 *           yourPrinter(s, v);
 *           return zftrue;
 *       })
 *   ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_YourType, zfint, yourProp)
 *
 *   // in lua
 *   local obj = YourType();
 *   local yourProp = obj:yourProp();
 * @endcode
 */

