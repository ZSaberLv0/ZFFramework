/**
 * @file ZFString.h
 * @brief string container as ZFObject
 */

#ifndef _ZFI_ZFString_h_
#define _ZFI_ZFString_h_

#include "ZFObject.h"
#include <wchar.h>
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/** @brief wchar_t wrapper */
#ifndef _ZFT_zfcharW
    ZFT_INT_WEAK(wchar_t, zfcharW);
#else
    ZFT_INT_WEAK(_ZFT_zfcharW, zfcharW);
#endif

/** @brief wstring wrapper */
#ifndef _ZFT_zfstringW
    ZFT_INT_WEAK(_zfstr<zfcharW>, zfstringW);
#else
    ZFT_INT_WEAK(_ZFT_zfstringW, zfstringW);
#endif

// ============================================================
/**
 * @brief supported string encoding
 *
 * we use internal string encoding for string processing,
 * and use native string type for string storage\n
 * \n
 * we have these encoding types:
 * -  zfchar: UTF8 encoded
 * -  zfcharW: usually used to adapt to Unicode version of Windows API,
 *   ensured no less than 2 bytes, UTF16 encoded
 *
 * we have these string types:
 * -  cosnt zfchar *: C-style strings, simple and efficient
 * -  #zfstring: wrapper for std::string
 * -  #v_zfstring: string container as a ZFObject
 *
 * \n
 * keep it in short, use zfchar (or zfstring) to process strings,
 * and use v_zfstring as ZFObject:
 * @code
 *   const zfchar *s = "my string";
 *   zfstring str = s;
 *
 *   // for Windows API, convert to zfcharW and use WCS versions:
 *   WindowsWAPI(zfstringToUTF16(s, ZFStringEncoding::e_UTF8));
 * @endcode
 *
 * for more advanced string encoding operation,
 * you should consider third-party library
 */
ZFENUM_BEGIN(ZFStringEncoding)
    ZFENUM_VALUE(UTF8 = 1)
    ZFENUM_VALUE(UTF16LE = 2)
    ZFENUM_VALUE(UTF16BE = 3)
    ZFENUM_VALUE(UTF16 = e_UTF16LE)
ZFENUM_SEPARATOR_ALLOW_DUPLICATE_VALUE(ZFStringEncoding)
    ZFENUM_VALUE_REGISTER(UTF8)
    ZFENUM_VALUE_REGISTER_WITH_NAME(UTF16LE, "UTF16")
    ZFENUM_VALUE_REGISTER_WITH_NAME(UTF16BE, "UTF16BE")
    ZFENUM_VALUE_REGISTER_WITH_NAME(UTF16, "UTF16")
ZFENUM_END(ZFStringEncoding)

// ============================================================
// conversion
/**
 * @brief convert to deisred encoding, or empty string if failed
 *
 * result would be appended to tail without clear
 */
extern ZF_ENV_EXPORT zfbool zfstringToUTF8(ZF_OUT zfstring &result,
                                           ZF_IN const void *s,
                                           ZF_IN ZFStringEncodingEnum srcEncoding);
/** @brief see #zfstringToUTF8 */
inline zfstring zfstringToUTF8(ZF_IN const void *s,
                               ZF_IN ZFStringEncodingEnum srcEncoding,
                               ZF_OUT_OPT zfbool *success = zfnull)
{
    zfstring ret;
    zfbool t = zfstringToUTF8(ret, s, srcEncoding);
    if(success != zfnull) {*success = t;}
    return ret;
}

/** @brief see #zfstringToUTF8 */
extern ZF_ENV_EXPORT zfbool zfstringToUTF16(ZF_OUT zfstringW &result,
                                            ZF_IN const void *s,
                                            ZF_IN ZFStringEncodingEnum srcEncoding);
/** @brief see #zfstringToUTF8 */
inline zfstringW zfstringToUTF16(ZF_IN const void *s,
                                 ZF_IN ZFStringEncodingEnum srcEncoding,
                                 ZF_OUT_OPT zfbool *success = zfnull)
{
    zfstringW ret;
    zfbool t = zfstringToUTF16(ret, s, srcEncoding);
    if(success != zfnull) {*success = t;}
    return ret;
}
/** @brief see #zfstringToUTF8 */
extern ZF_ENV_EXPORT zfbool zfstringToUTF16BE(ZF_OUT zfstringW &result,
                                              ZF_IN const void *s,
                                              ZF_IN ZFStringEncodingEnum srcEncoding);
/** @brief see #zfstringToUTF8 */
inline zfstringW zfstringToUTF16BE(ZF_IN const void *s,
                                   ZF_IN ZFStringEncodingEnum srcEncoding,
                                   ZF_OUT_OPT zfbool *success = zfnull)
{
    zfstringW ret;
    zfbool t = zfstringToUTF16BE(ret, s, srcEncoding);
    if(success != zfnull) {*success = t;}
    return ret;
}

/**
 * @brief get utf8 string's logical word count or -1 if invalid letter exist
 */
extern ZF_ENV_EXPORT zfindex zfstringWordCountOfUTF8(ZF_IN const zfchar *utf8String);

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFString_h_

