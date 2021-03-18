/**
 * @page DocTag_Feature_DynamicRegister Dynamic class/method/property register
 *
 * unlike other C++ framework use static template specialization
 * to achieve reflection,
 * ZFFramework use fully dynamic way to achieve reflection,
 * this may cause less performance,
 * but are much more convenient for high level app coding\n
 * \n
 * example:
 * @code
 *   ZFLuaExecute(
 *       "ZFDynamic()"
 *       ":classBegin('MyClass', 'ZFUIView')"
 *       "    :property(ZFTypeId_zfstring(), 'myProp')"
 *       ":classEnd()"
 *       );
 * @endcode
 *
 * as you can see, we are able to register class/method/property in script language\n
 * further more, the registered contents can be combined
 * with all other features in C++ world:
 * @code
 *   zfautoObject obj = ZFClass::classForName("MyClass")->newInstance();
 *   obj->invoke("myProp", zflineAlloc(v_zfstring, "myValue"));
 *   ZFObjectToXml(ZFOutputDefault(), obj);
 * @endcode
 */

