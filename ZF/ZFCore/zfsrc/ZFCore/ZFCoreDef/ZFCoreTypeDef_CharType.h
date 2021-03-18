/**
 * @file ZFCoreTypeDef_CharType.h
 * @brief types for ZFFramework
 */

#ifndef _ZFI_ZFCoreTypeDef_CharType_h_
#define _ZFI_ZFCoreTypeDef_CharType_h_

#include "ZFCoreTypeDef_CoreType.h"
#include <string.h>
#include <stdlib.h>

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/** @brief char wrapper */
ZFT_INT_WEAK(_ZFT_t_zfchar, zfchar);

// ============================================================
/**
 * @brief get logical char's size,
 *   see #zfcharMoveNext
 */
extern ZF_ENV_EXPORT zfindex zfcharGetSize(const zfchar *p);

/**
 * @brief move to next logical char
 *
 * since zfchar's encoding may take more than one char unit
 * to store a logical char, you can't simply use "++p"\n
 * this macro take one param,
 * which should be a pointer var of (zfchar *)
 * or (const zfchar *),
 * and this macro would move the pointer to next logical char
 */
template<typename T_CharString>
inline void zfcharMoveNext(T_CharString &p)
{
    p += zfcharGetSize(p);
}

/**
 * @brief append logical char from p to s,
 *   then move p to next char,
 *   see #zfcharMoveNext
 */
template<typename T_Str, typename T_CharString>
void zfcharAppendAndMoveNext(T_Str &s, T_CharString &p)
{
    switch(zfcharGetSize(p))
    {
        case 2:
            s.append(p, 2);
            p += 2;
            break;
        case 3:
            s.append(p, 3);
            p += 3;
            break;
        case 4:
            s.append(p, 4);
            p += 4;
            break;
        case 5:
            s.append(p, 5);
            p += 5;
            break;
        case 6:
            s.append(p, 6);
            p += 6;
            break;
        case 7:
            s.append(p, 7);
            p += 7;
            break;
        case 8:
            s.append(p, 8);
            p += 8;
            break;
        case 1:
        default:
            s.append(p, 1);
            p += 1;
            break;
    }
}

/**
 * @brief true if 'a' ~ 'z'
 */
#define zfcharIsLetterLowerCase(c) \
    ((c) >= 'a' && (c) <= 'z')
/**
 * @brief true if 'A' ~ 'Z'
 */
#define zfcharIsLetterUpperCase(c) \
    ((c) >= 'A' && (c) <= 'Z')
/**
 * @brief true if '0' ~ '9'
 */
#define zfcharIsNumber(c) \
    ((c) >= '0' && (c) <= '9')
/**
 * @brief true if '0' ~ '9' or 'a' ~ 'f'
 */
#define zfcharIsHexNumberLowerCase(c) \
    (zfcharIsNumber(c) || ((c) >= 'a' && (c) <= 'f'))
/**
 * @brief true if '0' ~ '9' or 'A' ~ 'F'
 */
#define zfcharIsHexNumberUpperCase(c) \
    (zfcharIsNumber(c) || ((c) >= 'A' && (c) <= 'F'))
/**
 * @brief true if '0' ~ '9' or 'a' ~ 'f' or 'A' ~ 'F'
 */
#define zfcharIsHexNumber(c) \
    (zfcharIsNumber(c) || ((c) >= 'a' && (c) <= 'f') || ((c) >= 'A' && (c) <= 'F'))
/**
 * @brief true if char is space or tab
 */
#define zfcharIsSpace(c) \
    ((c) == ' ' || (c) == '\t')
/**
 * @brief true if char is '\\r' or '\\n'
 */
#define zfcharIsEndl(c) \
    ((c) == '\r' || (c) == '\n')
/**
 * @brief move p to skip spaces and tabs, up to end
 */
#define zfcharSkipSpace(p, end) \
    while(p < end && zfcharIsSpace(*p)) ++p
/**
 * @brief move p to skip spaces and tabs, up to end
 */
#define zfcharSkipEndl(p, end) \
    while(p < end && zfcharIsEndl(*p)) ++p
/**
 * @brief move p to skip spaces and tabs, up to end
 */
#define zfcharSkipSpaceAndEndl(p, end) \
    while(p < end && (zfcharIsSpace(*p) || zfcharIsEndl(*p))) ++p

// ============================================================
// zfchar types
/** @brief strlen wrapper as zfchar type */
inline zfindex zfslen(const zfchar *s)
{
    return (zfindex)strlen(s);
}
/** @brief strcpy wrapper as zfchar type */
inline zfchar *zfscpy(zfchar *dst, const zfchar *src)
{
    return strcpy(dst, src);
}
/** @brief strcmp wrapper as zfchar type */
inline zfint zfscmp(const zfchar *s1, const zfchar *s2)
{
    return (zfint)strcmp(s1, s2);
}
/** @brief stricmp wrapper as zfchar type, no locale supported */
extern ZF_ENV_EXPORT zfint zfsicmp(const zfchar *s1, const zfchar *s2);
/** @brief strncmp wrapper as zfchar type */
inline zfint zfsncmp(const zfchar *s1, const zfchar *s2, zfindex count)
{
    return ((count == zfindexMax()) ? strcmp(s1, s2) : strncmp(s1, s2, (size_t)count));
}
/** @brief strnicmp wrapper as zfchar type, no locale supported */
extern ZF_ENV_EXPORT zfint zfsnicmp(const zfchar *s1, const zfchar *s2, zfindex count);

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreTypeDef_CharType_h_

