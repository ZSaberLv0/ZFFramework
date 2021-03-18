/**
 * @file ZFCoreSPrintf.h
 * @brief string utilities for ZFFramework
 */

#ifndef _ZFI_ZFCoreSPrintf_h_
#define _ZFI_ZFCoreSPrintf_h_

#include "ZFCoreTypeDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/** @brief see #zfstringAppend */
extern ZF_ENV_EXPORT void zfstringAppendV(ZF_OUT zfstring &s, ZF_IN const zfchar *fmt, va_list vaList);
/** @brief see #zfstringAppend */
inline void zfstringAppendV(ZF_OUT_OPT zfstring *s, ZF_IN const zfchar *fmt, va_list vaList)
{
    if(s)
    {
        zfstringAppendV(*s, fmt, vaList);
    }
}

// ============================================================
/**
 * @brief a light-weight sprintf-like string formatter
 *
 * s to append could be any types that support:
 * -  operator += (CharType)
 * -  operator += (const CharType *)
 *
 * supported format tokens:
 * -  "b": zfbool (e.g. true)
 * -  "zi": zfindex (e.g. 13579)
 * -  "d" or "i": zfint (e.g. -13579)
 * -  "u": zfuint (e.g. 13579)
 * -  "o": zfuint, in octal (e.g. 01234567)
 * -  "x": zfuint, in heximal, lowercase (e.g. 0x1234abcd)
 * -  "X": zfuint, in heximal, uppercase (e.g. 0x1234ABCD)
 * -  "f": zffloat, accuracy is not ensured (e.g. 13.579)
 * -  "p": pointer value (e.g. 0x1234ABCD)
 * -  "c" or "C": char
 * -  "s" or "S": zfchar string
 * -  "%": % itself
 *
 * extra format tokens can be added before any of those listed above (except "%"),
 * similar to printf, they are:
 * -  "-": left justify
 * -  "0": leading zero fill
 * -  "+": print plus if positive number
 * -  " ": print space if positive number
 * -  "m": minimum width
 * -  "*": same as "m" but scaned from vaList as a zfindex
 * -  ".n": decimal width for float types or max width for string types
 * -  ".*": same as ".n" but scaned from vaList as a zfindex
 *
 * note:
 * -  for special types such as zfflags or zftimet,
 *   you should use zfsFromInt series to make sure type size
 *   doesn't affect the print result
 * -  zfindex may or may not have the same size with zfint/zfuint,
 *   you must take care of it when printing zfuint and zfindex types,
 *   for example, this may cause trouble:
 *   @code
 *     zfindex n = 1;
 *     const zfchar *s = "1";
 *     zfstring str;
 *     // since zfuint is usually 32 bit and zfindex may be 64 bit,
 *     // when scanning %u from a zfindex,
 *     // the left 32 bit would be passed and scaned as %s,
 *     // the app may crash at runtime
 *     zfstringAppend(str, "%u %s", n, s);
 *   @endcode
 */
inline void zfstringAppend(ZF_OUT zfstring &s,
                           ZF_IN const zfchar *fmt,
                           ...)
{
    va_list vaList;
    va_start(vaList, fmt);
    zfstringAppendV(s, fmt, vaList);
    va_end(vaList);
}
/** @brief see #zfstringAppend */
inline void zfstringAppend(ZF_OUT zfstring *s,
                           ZF_IN const zfchar *fmt,
                           ...)
{
    if(s)
    {
        va_list vaList;
        va_start(vaList, fmt);
        zfstringAppendV(*s, fmt, vaList);
        va_end(vaList);
    }
}

// ============================================================
/**
 * @brief util to create a zfstring from format
 */
inline zfstring zfstringWithFormat(ZF_IN const zfchar *fmt, ...)
{
    zfstring ret;
    va_list vaList;
    va_start(vaList, fmt);
    zfstringAppendV(ret, fmt, vaList);
    va_end(vaList);
    return ret;
}
/**
 * @brief util to create a zfstring from format
 */
inline zfstring zfstringWithFormatV(ZF_IN const zfchar *fmt, ZF_IN va_list vaList)
{
    zfstring ret;
    zfstringAppendV(ret, fmt, vaList);
    return ret;
}

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreSPrintf_h_

