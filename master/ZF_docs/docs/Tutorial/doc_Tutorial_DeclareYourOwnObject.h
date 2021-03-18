/**
 * @page DocTag_Tutorial_DeclareYourOwnObject Declare your own object
 *
 * to use ZFFramework's object,
 * you must follow some rules:
 * @code
 *   zfclass MyObject : zfextends ParentObject
 *   {
 *       ZFOBJECT_DECLARE(MyObject, ParentObject)
 *   };
 *
 *   // in cpp files
 *   ZFOBJECT_REGISTER(MyObject)
 * @endcode
 *
 * -  use zfclass to show it's an ZFObject type
 * -  use zfextends to extends from a ZFObject
 *   or any other ZFObject type
 * -  use ZFOBJECT_DECLARE for necessary macro expansion
 * -  use ZFOBJECT_REGISTER to register your object,
 *   this is required if you want your object reflectable
 */

