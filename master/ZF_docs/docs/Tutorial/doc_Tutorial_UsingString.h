/**
 * @page DocTag_Tutorial_UsingString Using string
 *
 * ZFFramework use these string types:
 * -  const zfchar *\n
 *   C-style string, use just like "const char *"
 * -  zfstring\n
 *   C++-style string, use just like std::string
 * -  v_zfstring\n
 *   string wrapped as #ZFObject
 *
 * @code
 *   // common C-style string, best performance
 *   const zfchar *cStyle = "C-style string";
 *   zfchar cStyle2[32] = {0};
 *   zfscpy(cString2, "copied C-style string");
 *   zfLogT() << cStyle << cStyle2;
 *
 *   // common C++-style string, high performance yet easy to use
 *   zfstring cppStyle("C++-style string");
 *   zfstring cppStyle2;
 *   cppStyle2 = "copied C++-style string"
 *   zfLogT() << cppStyle << cppStyle2.cString();
 *
 *   // string container as object type
 *   zfblockedAlloc(v_zfstring, zfStyle, "object-style string");
 *   zfblockedAlloc(v_zfstring, zfStyle2);
 *   zfStyle2->zfv = "copied object-style string";
 *   zfLogT() << zfStyle << zfStyle2->zfv;
 * @endcode
 * \n
 * by default, all strings in ZFFramework use UTF-8 as string encoding
 */

