/**
 * @page DocTag_Tutorial_DeclareMethodAndProperty Declare method and property
 *
 * aside from normal method of C++ world,
 * ZFFramework supplys enhanced method and property logic:
 * @code
 *   zfclass MyObject : zfextends ZFObject
 *   {
 *       ZFOBJECT_DECLARE(MyObject)
 *
 *       // declare method
 *       ZFMETHOD_INLINE_0(void, myMethod)
 *       {
 *           // method contents here
 *       }
 *
 *       // declare property
 *       ZFPROPERTY_ASSIGN(zfstring, myProperty)
 *   };
 * @endcode
 *
 * once declared,
 * you may use them as normal methods:
 * @code
 *   // call as normal method
 *   obj->myMethod();
 *
 *   // get property value by normal method
 *   zfstring value = obj->myProperty();
 *   // set property value by normal method
 *   obj->myProperty(value);
 * @endcode
 *
 * for advanced usage of method and property,
 * see @ref DocTag_Tutorial_Reflection for more info
 */

