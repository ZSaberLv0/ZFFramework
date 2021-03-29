/**
 * @page DocTag_Tutorial_UsingContainer Using container
 *
 * ZFFramework use these container types:
 * -  all other C++ containers, such as std::vector,
 *   can be used directly
 * -  ZFCoreArray/ZFCoreMap\n
 *   C++-style container similar to std::vector / std::map,
 *   may not so efficient compared to STL containers,
 *   used to reduce dependency,
 *   use only if necessary
 *   @code
 *     ZFCoreArray<zfstring> array;
 *     array.add("element");
 *     const zfstring &e = array[0];
 *
 *     // or use POD version for performance, can hold POD types only
 *     ZFCoreArrayPOD<zfindex> arrayPOD;
 *
 *     ZFCoreMap map;
 *     // can hold string as key type and any type as value type,
 *     // while value type can only be wrapped by ZFCorePointer
 *     zfstring *p = zfnew(zfstring);
 *     map.set("key", ZFCorePointerForObject<zfstring *>(p));
 *     // see #ZFCorePointerBase and #ZFCoreMap for more info
 *   @endcode
 * -  ZFContainer/ZFKeyValueContainer\n
 *   container as ZFObject,
 *   can hold ZFObject types only\n
 *   typically we have ZFArray, ZFSet, ZFHashSet as container,
 *   and ZFMap, ZFHashMap as key value container\n
 *   all container would automatically retain elements when add,
 *   and release when remove
 *   @code
 *     zfblockedAlloc(ZFArray, array);
 *
 *     // can hold ZFObject types only
 *     array->add(zflineAlloc(v_zfstring));
 *   @endcode
 */

