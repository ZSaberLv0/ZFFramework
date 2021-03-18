/**
 * @file UTFCodeUtil.h
 * @brief simple UTF code convertor
 */

#ifndef _UTFCodeUtil_h_
#define _UTFCodeUtil_h_

#include "ZFCore.h"
#include <wchar.h>

namespace UTFCodeUtil {

    /**
     * @brief get logic word count of UTF8 string instead of byte size
     *
     * return (size_t)-1 if invalid utf8 chars exist
     */
    extern ZF_ENV_EXPORT size_t UTF8GetWordCount(const char *src);

    /**
     * @brief convert UTF16 to UTF8
     */
    extern ZF_ENV_EXPORT char *UTF16ToUTF8(const wchar_t *src);
    /**
     * @brief convert UTF8 to UTF16
     *
     * use space if letter out of UTF16's range
     */
    extern ZF_ENV_EXPORT wchar_t *UTF8ToUTF16(const char *src);

    /**
     * @brief convert UTF16 to UTF16 big endian
     */
    extern ZF_ENV_EXPORT wchar_t *UTF16ToUTF16BE(const wchar_t *src);
    /**
     * @brief convert UTF16 big endian to UTF16
     */
    extern ZF_ENV_EXPORT wchar_t *UTF16BEToUTF16(const wchar_t *src);

} // namespace UTFCodeUtil

#endif // #ifndef _UTFCodeUtil_h_

