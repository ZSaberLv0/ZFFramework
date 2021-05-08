/**
 * @file ZFCoreStringUtil.h
 * @brief string utilities for ZFFramework
 */

#ifndef _ZFI_ZFCoreStringUtil_h_
#define _ZFI_ZFCoreStringUtil_h_

#include "ZFCoreTypeDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief convert integer number (int, long, etc) to string
 *
 * integer number can be signed or unsigned,
 * whatever your compiler support\n
 * typical usage:
 * @code
 *   zfstring s = ...;
 *   zfint n = ...;
 *
 *   // append result to s and return it as result
 *   zfsFromIntT(s, n);
 *
 *   // return a new zfstring as result
 *   s = zfsFromInt(n);
 * @endcode
 */
template<typename T_Int>
ZF_ENV_EXPORT zfbool zfsFromIntT(ZF_OUT zfstring &s,
                                 ZF_IN T_Int n,
                                 ZF_IN_OPT zfindex radix = 10,
                                 ZF_IN_OPT zfbool useUpperCase = zftrue)
{
    if(radix < 2 || radix > 36) {return zffalse;}

    if(n <= (T_Int)0)
    {
        // tricks to solve the unsigned type warnings
        if(n != (T_Int)0)
        {
            s += '-';
            n = (T_Int)0 - n;
        }
        else
        {
            s += '0';
            return zftrue;
        }
    }
    else if(n == (T_Int)-1)
    {
        s += '-';
        s += '1';
        return zftrue;
    }

    unsigned long long e = 1;
    while(n / e) {e *= radix;}

    const zfchar *token = useUpperCase
        ? "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        : "0123456789abcdefghijklmnopqrstuvwxyz";
    while(e /= radix)
    {
        s += token[n / e];
        n %= e;
    }
    return zftrue;
}
/**
 * @brief see #zfsFromIntT
 */
template<typename T_Int>
ZF_ENV_EXPORT zfstring zfsFromInt(ZF_IN T_Int n,
                                  ZF_IN_OPT zfindex radix = 10,
                                  ZF_IN_OPT zfbool useUpperCase = zftrue)
{
    zfstring s;
    zfsFromIntT(s, n, radix, useUpperCase);
    return s;
}

// ============================================================
/**
 * @brief convert string to int, return error position if failed,
 *   or null if success
 *
 * it's your responsibility to make sure the template int type is correct,
 * otherwise, the result may be unexpected
 * (for example, specify a short int type while numbers in src is too long)
 */
template<typename T_Int>
ZF_ENV_EXPORT zfbool zfsToIntT(ZF_OUT T_Int &ret,
                               ZF_IN const zfchar *src,
                               ZF_IN_OPT zfindex srcLen = zfindexMax(),
                               ZF_IN_OPT zfindex radix = 10,
                               ZF_IN_OPT zfbool allowNegative = zftrue,
                               ZF_OUT_OPT const zfchar **outErrorPos = zfnull)
{
    if(src == zfnull || srcLen == 0 || radix < 2 || radix > 36)
    {
        if(outErrorPos != zfnull)
        {
            *outErrorPos = src;
        }
        return zffalse;
    }

    ret = 0;
    const zfchar *p = src;
    const zfchar *pEnd = ((srcLen == zfindexMax()) ? (p - 1) : (p + srcLen));
    zfbool negative = zffalse;
    if(*p == '-')
    {
        if(!allowNegative)
        {
            if(outErrorPos != zfnull)
            {
                *outErrorPos = p;
            }
            return zffalse;
        }
        negative = zftrue;
        ++p;
    }
    while(*p != '\0' && p != pEnd)
    {
        zfuint tmp = 0;
        if(*p >= '0' && *p <= '9') {tmp = *p - '0';}
        else if(*p >= 'a' && *p <= 'z') {tmp = 10 + *p - 'a';}
        else if(*p >= 'A' && *p <= 'Z') {tmp = 10 + *p - 'A';}
        else {break;}
        if(tmp >= radix) {break;}
        ret = (T_Int)((ret * (zfuint)radix) + tmp);
        ++p;
    }
    if(negative)
    {
        ret = (T_Int)0 - ret;
    }

    if(*p == '\0' || p == pEnd)
    {
        return zftrue;
    }
    else
    {
        if(outErrorPos != zfnull)
        {
            *outErrorPos = p;
        }
        return zffalse;
    }
}
/**
 * @brief see #zfsToIntT
 */
template<typename T_Int>
ZF_ENV_EXPORT T_Int zfsToInt(ZF_IN const zfchar *src,
                             ZF_IN_OPT zfindex srcLen = zfindexMax(),
                             ZF_IN_OPT zfindex radix = 10,
                             ZF_IN_OPT zfbool allowNegative = zftrue,
                             ZF_OUT_OPT const zfchar **outErrorPos = zfnull)
{
    T_Int ret = 0;
    zfsToIntT(ret, src, srcLen, radix, allowNegative, outErrorPos);
    return ret;
}
/**
 * @brief see #zfsToIntT
 */
inline zfint zfsToInt(ZF_IN const zfchar *src,
                      ZF_IN_OPT zfindex srcLen = zfindexMax(),
                      ZF_IN_OPT zfindex radix = 10,
                      ZF_IN_OPT zfbool allowNegative = zftrue,
                      ZF_OUT_OPT const zfchar **outErrorPos = zfnull)
{
    return zfsToInt<zfint>(src, srcLen, radix, allowNegative, outErrorPos);
}

// ============================================================
/**
 * @brief convert float number (int, long, etc) to string
 *
 * float number can be any type such as float, double, long double,
 * whatever your compiler support\n
 * for typical usage, see #zfsFromIntT
 */
template<typename T_Float>
ZF_ENV_EXPORT zfbool zfsFromFloatT(ZF_OUT zfstring &s,
                                   ZF_IN T_Float n)
{
    zfchar buf[64] = {0};
    sprintf(buf, "%lf", (double)n);
    const zfchar *p = buf;
    while(*p && *p != '.') {++p;}
    if(*p == '\0')
    {
        s += buf;
        return zftrue;
    }
    const zfchar *pEnd = p + zfslen(p) - 1;
    if(*pEnd == '0')
    {
        do {--pEnd;} while(*pEnd == '0');
        if(pEnd == p)
        {
            s.append(buf, p - buf);
        }
        else
        {
            s.append(buf, pEnd + 1 - buf);
        }
    }
    else
    {
        s += buf;
    }
    return zftrue;
}
/**
 * @brief see #zfsFromFloatT
 */
template<typename T_Float>
ZF_ENV_EXPORT zfstring zfsFromFloat(ZF_IN T_Float n)
{
    zfstring s;
    zfsFromFloatT(s, n);
    return s;
}

// ============================================================
/**
 * @brief convert string to float
 *
 * it's your responsibility to make sure int type is correct,
 * otherwise, the result may be unexpected\n
 * for typical usage, see #zfsToIntT
 * @note if src is null, pErrPos would be null
 */
template<typename T_Float>
ZF_ENV_EXPORT zfbool zfsToFloatT(ZF_OUT T_Float &ret,
                                 ZF_IN const zfchar *src,
                                 ZF_IN_OPT zfindex srcLen = zfindexMax(),
                                 ZF_OUT_OPT const zfchar **outErrorPos = zfnull)
{
    if(src == zfnull || srcLen == 0)
    {
        if(outErrorPos != zfnull)
        {
            *outErrorPos = src;
        }
        return zffalse;
    }

    ret = 0;
    const zfchar *p = src;
    const zfchar *pEnd = ((srcLen == zfindexMax()) ? (p - 1) : (p + srcLen));
    zfbool negative = zffalse;
    if(*p == '-')
    {
        negative = zftrue;
        ++p;
    }

    while(*p != '\0' && *p != '.' && p != pEnd)
    {
        if(*p < '0' || *p > '9') {break;}
        ret = ret * 10 + (*p - '0');
        ++p;
    }
    if(*p != '\0' && *p != '.' && p != pEnd)
    {
        if(outErrorPos != zfnull)
        {
            *outErrorPos = p;
        }
        return zffalse;
    }

    if(*p == '.')
    {
        ++p;

        unsigned long e = 10;
        while(*p != '\0' && p != pEnd
            && *p >= '0' && *p <= '9')
        {
            ret += (T_Float)(*p - '0') / e;
            e *= 10;
            ++p;
        }
        if(*p != '\0' && p != pEnd)
        {
            if(outErrorPos != zfnull)
            {
                *outErrorPos = p;
            }
            return zffalse;
        }
    }

    if(negative)
    {
        ret = 0 - ret;
    }

    if(*p == '\0' || p == pEnd)
    {
        return zftrue;
    }
    else
    {
        if(outErrorPos != zfnull)
        {
            *outErrorPos = p;
        }
        return zffalse;
    }
}
/**
 * @brief see #zfsToFloatT
 */
template<typename T_Float>
ZF_ENV_EXPORT T_Float zfsToFloat(ZF_IN const zfchar *src,
                                 ZF_IN_OPT zfindex srcLen = zfindexMax(),
                                 ZF_OUT_OPT const zfchar **outErrorPos = zfnull)
{
    T_Float ret = 0;
    zfsToFloatT(ret, src, srcLen, outErrorPos);
    return ret;
}
/**
 * @brief see #zfsToFloatT
 */
inline zffloat zfsToFloat(ZF_IN const zfchar *src,
                          ZF_IN_OPT zfindex srcLen = zfindexMax(),
                          ZF_OUT_OPT const zfchar **outErrorPos = zfnull)
{
    return zfsToFloat<zffloat>(src, srcLen, outErrorPos);
}


// ============================================================
/**
 * @brief convert pointer value to string
 */
inline zfbool zfsFromPointerT(ZF_OUT zfstring &s,
                              ZF_IN const void *p)
{
    zfchar buf[32] = {0};
    if(p == zfnull)
    {
        sprintf(buf, "<null>");
    }
    else
    {
        sprintf(buf, "%p", p);
    }
    const zfchar *t = buf;
    while(*t)
    {
        s += *t++;
    }
    return zftrue;
}
/**
 * @brief see #zfsFromPointerT
 */
inline zfstring zfsFromPointer(ZF_IN const void *p)
{
    zfstring s;
    zfsFromPointerT(s, p);
    return s;
}

// ============================================================
/**
 * @brief util to compare two c strings,
 *   while null is regarded as equal to empty string
 */
inline zfbool zfscmpTheSame(ZF_IN const zfchar *s1, ZF_IN const zfchar *s2)
{
    if(s1 == s2)
    {
        return zftrue;
    }
    else if(!s1)
    {
        return (*s2 == '\0');
    }
    else if(!s2)
    {
        return (*s1 == '\0');
    }
    else
    {
        return (zfscmp(s1, s2) == 0);
    }
}
/**
 * @brief util to check whether string is empty (null or empty string)
 */
inline zfbool zfsIsEmpty(ZF_IN const zfchar *s1)
{
    return (s1 == zfnull || *s1 == '\0');
}

// ============================================================
/**
 * @brief util method to copy the contents of src,
 *   return null if src null, or empty string if src is empty string
 */
inline zfchar *zfsCopy(ZF_IN const zfchar *src)
{
    zfchar *ret = zfnull;
    if(src)
    {
        zfindex size = sizeof(zfchar) * (zfslen(src) + 1);
        ret = (zfchar *)zfmalloc(size);
        zfmemcpy(ret, src, size);
    }
    return ret;
}
/**
 * @brief util method to copy the contents of src,
 *   return null if src null, or empty string if src is empty string
 */
inline zfchar *zfsCopy(ZF_IN const zfchar *src,
                       ZF_IN zfindex size)
{
    zfchar *ret = zfnull;
    if(src)
    {
        if(size == zfindexMax())
        {
            size = zfslen(src);
        }
        ret = (zfchar *)zfmalloc(sizeof(zfchar) * (size + 1));
        zfmemcpy(ret, src, sizeof(zfchar) * size);
        ret[size] = '\0';
    }
    return ret;
}
/**
 * @brief util method to free old string then copy and change to new string,
 *   free and set dst to null if src is null,
 *   otherwise, realloc and copy contents to dst
 */
inline zfchar *zfsChange(ZF_IN_OUT zfchar *&dst,
                         ZF_IN const zfchar *src)
{
    if(!src)
    {
        zffree(dst);
        dst = zfnull;
        return dst;
    }

    zfindex size = sizeof(zfchar) * (zfslen(src) + 1);
    dst = (zfchar *)zfrealloc(dst, size);
    zfmemcpy(dst, src, size);
    return dst;
}
/**
 * @brief util method to free old string then copy and change to new string,
 *   free and set dst to null if src is null,
 *   otherwise, realloc and copy contents to dst
 */
inline zfchar *zfsChange(ZF_IN_OUT zfchar *&dst,
                         ZF_IN const zfchar *src,
                         ZF_IN zfindex size)
{
    if(!src)
    {
        zffree(dst);
        dst = zfnull;
        return dst;
    }

    if(size == zfindexMax())
    {
        size = zfslen(src);
    }
    dst = (zfchar *)zfrealloc(dst, sizeof(zfchar) * (size + 1));
    zfmemcpy(dst, src, sizeof(zfchar) * size);
    dst[size] = '\0';
    return dst;
}
/**
 * @brief util method to append string to existing string buffer, realloc if need
 */
inline zfchar *zfsAppend(ZF_IN_OUT zfchar *&old,
                         ZF_IN const zfchar *append)
{
    if(!old)
    {
        old = zfsCopy(append);
        return old;
    }

    zfindex oldSize = zfslen(old);
    zfindex size = sizeof(zfchar) * (oldSize + zfslen(append) + 1);
    old = (zfchar *)zfrealloc(old, size);
    zfmemcpy(old + oldSize, append, size);
    return old;
}
/**
 * @brief util method to append string to existing string buffer, realloc if need
 */
inline zfchar *zfsAppend(ZF_IN_OUT zfchar *&old,
                         ZF_IN const zfchar *append,
                         ZF_IN zfindex size)
{
    if(!old)
    {
        old = zfsCopy(append, size);
        return old;
    }

    zfindex oldSize = zfslen(old);
    if(size == zfindexMax())
    {
        size = zfslen(append);
    }
    old = (zfchar *)zfrealloc(old, sizeof(zfchar) * (oldSize + size + 1));
    zfmemcpy(old + oldSize, append, sizeof(zfchar) * size);
    old[oldSize + size] = '\0';
    return old;
}

#define _ZFP_zfsConnectEndPtr ((void *)-1)
inline zfchar *_ZFP_zfsConnect(ZF_IN const zfchar *src,
                               ...)
{
    zfindex len = 0;
    va_list vaList;
    const zfchar *tmp = src;

    va_start(vaList, src);
    do
    {
        if(tmp != zfnull)
        {
            len += zfslen(tmp);
        }
        tmp = va_arg(vaList, const zfchar *);
    } while(tmp != _ZFP_zfsConnectEndPtr);
    va_end(vaList);

    zfchar *ret = (zfchar *)zfmalloc(sizeof(zfchar) * (len + 1));
    tmp = src;
    len = 0;
    zfindex tmpLen = 0;
    va_start(vaList, src);
    do
    {
        if(tmp != zfnull)
        {
            tmpLen = zfslen(tmp);
            zfmemcpy(ret + len, tmp, sizeof(zfchar) * tmpLen);
            len += tmpLen;
        }
        tmp = va_arg(vaList, const zfchar *);
    } while(tmp != _ZFP_zfsConnectEndPtr);
    va_end(vaList);
    ret[len] = '\0';

    return ret;
}
/**
 * @brief connect multiple strings and return a newly created string,
 *   result must be deleted by #zffree
 *
 * null string would be treated as empty string
 */
#define zfsConnect(src, ...) _ZFP_zfsConnect(src, ##__VA_ARGS__, _ZFP_zfsConnectEndPtr)

// ============================================================
// zfstringFind
/** @brief find string */
extern ZF_ENV_EXPORT zfindex zfstringFind(ZF_IN const zfchar *src, ZF_IN zfindex srcLen, ZF_IN const zfchar *find, ZF_IN_OPT zfindex findLen = zfindexMax());
/** @brief find string */
inline zfindex zfstringFind(ZF_IN const zfstring &src, ZF_IN const zfstring &find)
{
    return zfstringFind(src.cString(), src.length(), find.cString(), find.length());
}
/** @brief find string */
inline zfindex zfstringFind(ZF_IN const zfstring &src, ZF_IN const zfchar *find, ZF_IN_OPT zfindex findLen = zfindexMax())
{
    return zfstringFind(src.cString(), src.length(), find, findLen);
}
/** @brief find string */
inline zfindex zfstringFind(ZF_IN const zfstring &src, ZF_IN zfchar find)
{
    return zfstringFind(src.cString(), src.length(), &find, 1);
}
// ============================================================
// zfstringFindReversely
/** @brief find string */
extern ZF_ENV_EXPORT zfindex zfstringFindReversely(ZF_IN const zfchar *src, ZF_IN zfindex srcLen, ZF_IN const zfchar *find, ZF_IN_OPT zfindex findLen = zfindexMax());
/** @brief find string */
inline zfindex zfstringFindReversely(ZF_IN const zfstring &src, ZF_IN const zfstring &find)
{
    return zfstringFindReversely(src.cString(), src.length(), find.cString(), find.length());
}
/** @brief find string */
inline zfindex zfstringFindReversely(ZF_IN const zfstring &src, ZF_IN const zfchar *find, ZF_IN_OPT zfindex findLen = zfindexMax())
{
    return zfstringFindReversely(src.cString(), src.length(), find, findLen);
}
/** @brief find string */
inline zfindex zfstringFindReversely(ZF_IN const zfstring &src, ZF_IN zfchar find)
{
    return zfstringFindReversely(src.cString(), src.length(), &find, 1);
}
// ============================================================
// zfstringFindCaseInsensitive
/** @brief find string */
extern ZF_ENV_EXPORT zfindex zfstringFindCaseInsensitive(ZF_IN const zfchar *src, ZF_IN zfindex srcLen, ZF_IN const zfchar *find, ZF_IN_OPT zfindex findLen = zfindexMax());
/** @brief find string */
inline zfindex zfstringFindCaseInsensitive(ZF_IN const zfstring &src, ZF_IN const zfstring &find)
{
    return zfstringFindCaseInsensitive(src.cString(), src.length(), find.cString(), find.length());
}
/** @brief find string */
inline zfindex zfstringFindCaseInsensitive(ZF_IN const zfstring &src, ZF_IN const zfchar *find, ZF_IN_OPT zfindex findLen = zfindexMax())
{
    return zfstringFindCaseInsensitive(src.cString(), src.length(), find, findLen);
}
/** @brief find string */
inline zfindex zfstringFindCaseInsensitive(ZF_IN const zfstring &src, ZF_IN zfchar find)
{
    return zfstringFindCaseInsensitive(src.cString(), src.length(), &find, 1);
}
// ============================================================
// zfstringFindCaseInsensitiveReversely
/** @brief find string */
extern ZF_ENV_EXPORT zfindex zfstringFindCaseInsensitiveReversely(ZF_IN const zfchar *src, ZF_IN zfindex srcLen, ZF_IN const zfchar *find, ZF_IN_OPT zfindex findLen = zfindexMax());
/** @brief find string */
inline zfindex zfstringFindCaseInsensitiveReversely(ZF_IN const zfstring &src, ZF_IN const zfstring &find)
{
    return zfstringFindCaseInsensitiveReversely(src.cString(), src.length(), find.cString(), find.length());
}
/** @brief find string */
inline zfindex zfstringFindCaseInsensitiveReversely(ZF_IN const zfstring &src, ZF_IN const zfchar *find, ZF_IN_OPT zfindex findLen = zfindexMax())
{
    return zfstringFindCaseInsensitiveReversely(src.cString(), src.length(), find, findLen);
}
/** @brief find string */
inline zfindex zfstringFindCaseInsensitiveReversely(ZF_IN const zfstring &src, ZF_IN zfchar find)
{
    return zfstringFindCaseInsensitiveReversely(src.cString(), src.length(), &find, 1);
}
// ============================================================
// zfstringFindFirstOf
/** @brief find string */
extern ZF_ENV_EXPORT zfindex zfstringFindFirstOf(ZF_IN const zfchar *src, ZF_IN zfindex srcLen, ZF_IN const zfchar *find, ZF_IN_OPT zfindex findLen = zfindexMax());
/** @brief find string */
inline zfindex zfstringFindFirstOf(ZF_IN const zfstring &src, ZF_IN const zfstring &find)
{
    return zfstringFindFirstOf(src.cString(), src.length(), find.cString(), find.length());
}
/** @brief find string */
inline zfindex zfstringFindFirstOf(ZF_IN const zfstring &src, ZF_IN const zfchar *find, ZF_IN_OPT zfindex findLen = zfindexMax())
{
    return zfstringFindFirstOf(src.cString(), src.length(), find, findLen);
}
/** @brief find string */
inline zfindex zfstringFindFirstOf(ZF_IN const zfstring &src, ZF_IN zfchar find)
{
    return zfstringFindFirstOf(src.cString(), src.length(), &find, 1);
}
// ============================================================
// zfstringFindFirstNotOf
/** @brief find string */
extern ZF_ENV_EXPORT zfindex zfstringFindFirstNotOf(ZF_IN const zfchar *src, ZF_IN zfindex srcLen, ZF_IN const zfchar *find, ZF_IN_OPT zfindex findLen = zfindexMax());
/** @brief find string */
inline zfindex zfstringFindFirstNotOf(ZF_IN const zfstring &src, ZF_IN const zfstring &find)
{
    return zfstringFindFirstNotOf(src.cString(), src.length(), find.cString(), find.length());
}
/** @brief find string */
inline zfindex zfstringFindFirstNotOf(ZF_IN const zfstring &src, ZF_IN const zfchar *find, ZF_IN_OPT zfindex findLen = zfindexMax())
{
    return zfstringFindFirstNotOf(src.cString(), src.length(), find, findLen);
}
/** @brief find string */
inline zfindex zfstringFindFirstNotOf(ZF_IN const zfstring &src, ZF_IN zfchar find)
{
    return zfstringFindFirstNotOf(src.cString(), src.length(), &find, 1);
}
// ============================================================
// zfstringFindLastOf
/** @brief find string */
extern ZF_ENV_EXPORT zfindex zfstringFindLastOf(ZF_IN const zfchar *src, ZF_IN zfindex srcLen, ZF_IN const zfchar *find, ZF_IN_OPT zfindex findLen = zfindexMax());
/** @brief find string */
inline zfindex zfstringFindLastOf(ZF_IN const zfstring &src, ZF_IN const zfstring &find)
{
    return zfstringFindLastOf(src.cString(), src.length(), find.cString(), find.length());
}
/** @brief find string */
inline zfindex zfstringFindLastOf(ZF_IN const zfstring &src, ZF_IN const zfchar *find, ZF_IN_OPT zfindex findLen = zfindexMax())
{
    return zfstringFindLastOf(src.cString(), src.length(), find, findLen);
}
/** @brief find string */
inline zfindex zfstringFindLastOf(ZF_IN const zfstring &src, ZF_IN zfchar find)
{
    return zfstringFindLastOf(src.cString(), src.length(), &find, 1);
}
// ============================================================
// zfstringFindLastNotOf
/** @brief find string */
extern ZF_ENV_EXPORT zfindex zfstringFindLastNotOf(ZF_IN const zfchar *src, ZF_IN zfindex srcLen, ZF_IN const zfchar *find, ZF_IN_OPT zfindex findLen = zfindexMax());
/** @brief find string */
inline zfindex zfstringFindLastNotOf(ZF_IN const zfstring &src, ZF_IN const zfstring &find)
{
    return zfstringFindLastNotOf(src.cString(), src.length(), find.cString(), find.length());
}
/** @brief find string */
inline zfindex zfstringFindLastNotOf(ZF_IN const zfstring &src, ZF_IN const zfchar *find, ZF_IN_OPT zfindex findLen = zfindexMax())
{
    return zfstringFindLastNotOf(src.cString(), src.length(), find, findLen);
}
/** @brief find string */
inline zfindex zfstringFindLastNotOf(ZF_IN const zfstring &src, ZF_IN zfchar find)
{
    return zfstringFindLastNotOf(src.cString(), src.length(), &find, 1);
}
// ============================================================
// zfstringReplace
/** @brief replace string, return replaced count */
extern ZF_ENV_EXPORT zfindex zfstringReplace(ZF_IN_OUT zfstring &s, ZF_IN const zfchar *replaceFrom, ZF_IN const zfchar *replaceTo, ZF_IN_OPT zfindex maxCount = zfindexMax());
/** @brief replace string, return replaced count */
extern ZF_ENV_EXPORT zfindex zfstringReplaceReversely(ZF_IN_OUT zfstring &s, ZF_IN const zfchar *replaceFrom, ZF_IN const zfchar *replaceTo, ZF_IN_OPT zfindex maxCount = zfindexMax());

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreStringUtil_h_

