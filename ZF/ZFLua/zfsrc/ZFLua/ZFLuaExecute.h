/**
 * @file ZFLuaExecute.h
 * @brief execute lua script
 */

#ifndef _ZFI_ZFLuaExecute_h_
#define _ZFI_ZFLuaExecute_h_

#include "ZFLuaDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief main entry for lua module, execute lua code
 *
 * thanks to powerful reflection mechanism in ZFFramework,
 * most of components are registered to lua by reflection automatically\n
 * \n
 * all types of ZFFramework are wrapped as #zfautoObject in lua world,
 * including: #ZFObject types (wrapped directly)
 * and non-ZFObject normal C++ types (wrapped by #ZFTypeIdWrapper)\n
 * then, all reflectable #ZFMethod supply #ZFMethodGenericInvoker
 * to invoke the method without knowing all actual types\n
 * \n
 * here is a list of functions available in lua to communicate with ZFFramework:
 * -  `zfAlloc("ClassName" [params...])`
 *   or `ClassName([params...])``\n
 *   alloc a ZFObject type\n
 *   "ClassName" can be #v_ZFClass, #v_zfstring, or native lua string\n
 *   if extra init param passed,
 *   your class must supplys reflectable #ZFObject::objectOnInit
 * -  invoker
 *   -  `ret = zfl_call(obj, "functionName", param0, param1, ...)`
 *     or `obj:functionName(param0, param1, ...)`\n
 *     call object's instance method, params are optional\n
 *     for "functionName", see #ZFTypeId_ZFMethod\n
 *     "functionName" can be #v_ZFMethod, #v_zfstring, or native lua string,
 *     while other types must exactly match the original types
 *   -  `ret = zfl_callStatic("methodName", param0, param1, ...)`
 *     or `ret = zfl_callStatic2("ClassOrNamespace", "methodName", param0, param1, ...)`
 *     or `ret = ClassOrNamespace.methodName(param0, param1, ...)`\n
 *     call global function or static class member method, params are optional\n
 *     for "functionName", see #ZFTypeId_ZFMethod\n
 *     "functionName" can be #v_ZFMethod, #v_zfstring, #v_ZFClass, or native lua string,
 *     while other types must exactly match the original types\n
 *     these namespace are considered as the same:
 *     -  #ZF_NAMESPACE_GLOBAL_NAME
 *     -  #ZF_NAMESPACE_GLOBAL_ABBR_NAME
 *     -  empty string
 *   -  while calling functions with lua raw string, such as `func('abc')`,
 *     we will try to convert to proper param type by string converter,
 *     if overload functions exists,
 *     conversion may take more than once,
 *     until it matches first function
 * -  ZFEnum
 *   -  `YourEnum.e_YourEnumValue()`\n
 *     access the enum value
 *   -  `YourEnumFlags('YourEnumValue1|YourEnumValue2')`
 *     or `YourEnumFlags(YourEnum.e_YourEnumValue1(), YourEnum.e_YourEnumValue2, ...)`\n
 *     create enum flags
 * -  value holder
 *   -  `value = YourTypeName("yourTypeData")`
 *     or `value = zfAlloc("YourTypeName")`
 *     or `value = YourTypeName()``\n
 *     create a non-ZFObject type registered by #ZFTYPEID_DECLARE,
 *     return the associated `YourTypeName` that holds the value\n
 *     "YourTypeName" represents the type name in #ZFTYPEID_DECLARE\n
 *     "yourTypeData" store string datas that would be decoded by YourTypeNameFromString\n
 *     "yourTypeData" can be #v_zfstring, or native lua string\n
 *     if your value holder supplys reflectable #ZFObject::objectOnInit
 *     (#ZFOBJECT_ON_INIT_DECLARE_2 series),
 *     the value holder can also be constructed by function like call:
 *     `YourTypeName(param0, param1)`
 *   -  `value:yourFunc()`
 *     or `YourTypeName.YourFunc()`\n
 *     for non-ZFObject types that wrapped by #ZFTYPEID_DECLARE,
 *     you may use #ZFMETHOD_USER_REGISTER_0 series to register methods
 *     to its wrapper type `YourTypeName`,
 *     then the methods can be invoked directly to your value type
 * -  value converter
 *   -  `zfl_luaValue(v)`\n
 *     convert a value to lua's raw value,
 *     the result lua value can be:
 *     -  lua string
 *     -  lua integer
 *     -  lua number
 *     -  lua boolean
 * -  callback
 *   -  `ZFCallbackForLua(luaFunc)`\n
 *     create a #ZFListener from lua function\n
 *     the lua function's proto type must be:
 *     @code
 *       function myLuaCallback(listenerData, userData)
 *       end
 *     @endcode
 *     \n
 *     further more, lua function can be converted to #ZFListener implicitly
 *     @code
 *       button:observerAdd(ZFUIButton.EventButtonOnClick(), function (listenerData, userData)
 *           end)
 *     @endcode
 *   -  `output:log(fmt, ...)`\n
 *     write to output callback, typically usage:\n
 *     `zfLogT():log(fmt, xxx):log(fmt, xxx)`
 * -  array
 *   -  `ZFCoreArrayCreate([a, b, c, ...])`\n
 *     create a array, params support these types:
 *     -  zfautoObject
 *     -  native lua number (stored as #v_zflongdouble)
 *     -  native lua string (stored as #v_zfstring)
 * -  param and return value
 *   -  simply use lua standard logic to process params and return values,
 *     here's some example:
 *     @code
 *       -- get all params
 *       local args = {...};
 *       local argsCount = #args;
 *       -- return value
 *       return args[1];
 *     @endcode
 *     note that, all params passed from #ZFLuaExecute are all #zfautoObject type
 * -  util
 *   -  `zfstringAppend(s, fmt, ...)`
 *     or `zfstringWithFormat(fmt, ...)`\n
 *     fmt can be #v_zfstring, or native lua string\n
 *     following va_args support:
 *     -  #ZFObject, would be converted by #ZFObject::objectInfo
 *     -  lua string type
 *     -  any lua type that supports convert to string
 *
 *     note: the va_args support params up to #ZFMETHOD_MAX_PARAM
 * -  path info
 *   -  `zfl_L()`\n
 *     lua_State of current chunk, stored as #v_VoidPointer
 *   -  `ZFLuaPathInfo()`\n
 *     return path info of current context, null if not available
 *   -  `ZFLuaImport(localFilePath [, param0, param1, ...])`
 *     or `ZFLuaImport(inputCallback [, param0, param1, ...])`\n
 *     util method for #ZFLuaExecute + #ZFInputForLocalFile
 *   -  `ZFLuaImportOnce(localFilePath [, param0, param1, ...])`
 *     or `ZFLuaImportOnce(inputCallback [, param0, param1, ...])`\n
 *     same as ZFLuaImport, but only run once for each input with same #ZFCallback::callbackId,
 *     you may also use ZFLuaImportOnceReset to reset the cache state\n
 *     this is useful to load #ZFDynamic contents from lua code
 *   -  `ZFLuaImportAll(localFilePath [, importCallback, importCallbackUserData, recursive])`
 *     or `ZFLuaImportAll(pathInfo [, importCallback, importCallbackUserData, recursive])`\n
 *     util method to import all lua files under specified path,
 *     files are looped by #ZFFilePathInfoCallbackFindFirst,
 *     and lua files are imported by ZFLuaImportOnce\n
 *     importCallback's param0 holds a #v_ZFPathInfo that points to the file
 *     which would be loaded
 *   -  `ZFLuaRes(localFilePath)`
 *     or `ZFLuaRes(inputCallback)`\n
 *     util method for #ZFObjectIOLoad + #ZFInputForLocalFile
 * -  debug helper
 *   -  `zfLog(fmt, ...)`
 *     or zfLogTrim(fmt, ...)\n
 *     use zfstringAppend then output to zfLog
 *   -  `zfLogT()`
 *     or zfLogTrimT()\n
 *     return a output callback to output, see above
 *   -  `zfl_tableInfo(v)`
 *     or `zfl_tableInfoPrint`\n
 *     return string that represents the table
 *
 *
 * note:
 * -  all types would be registered automatically,
 * -  for dynamically loaded library, all types would also be registered normally,
 *   however, won't be automatically unregistered when unloaded
 */
ZFMETHOD_FUNC_DECLARE_3(zfautoObject, ZFLuaExecute,
                        ZFMP_IN(const ZFInput &, input),
                        ZFMP_IN_OPT(const ZFCoreArray<zfautoObject> *, luaParams, zfnull),
                        ZFMP_IN_OPT(void *, L, zfnull))
/** @brief see #ZFLuaExecute */
ZFMETHOD_FUNC_DECLARE_3(zfautoObject, ZFLuaExecute,
                        ZFMP_IN(const zfchar *, buf),
                        ZFMP_IN_OPT(const ZFCoreArray<zfautoObject> *, luaParams, zfnull),
                        ZFMP_IN_OPT(void *, L, zfnull))

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFLuaExecute_h_

