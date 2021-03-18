#include "ZFCoreTypeDef_CharType.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfindex zfcharGetSize(const zfchar *p)
{
    if((zfbyte)p[0] <= 0x7F)
    {
        return 1;
    }
    else if((zfbyte)p[0] <= 0xDF && (zfbyte)p[1] <= 0xBF)
    {
        return 2;
    }
    else if((zfbyte)p[0] <= 0xEF && (zfbyte)p[1] <= 0xBF && (zfbyte)p[2] <= 0xBF)
    {
        return 3;
    }
    else if((zfbyte)p[0] <= 0xF7 && (zfbyte)p[1] <= 0xBF && (zfbyte)p[2] <= 0xBF && (zfbyte)p[3] <= 0xBF)
    {
        return 4;
    }
    else if((zfbyte)p[0] <= 0xFB && (zfbyte)p[1] <= 0xBF && (zfbyte)p[2] <= 0xBF && (zfbyte)p[3] <= 0xBF && (zfbyte)p[4] <= 0xBF)
    {
        return 5;
    }
    else if((zfbyte)p[0] <= 0xFD && (zfbyte)p[1] <= 0xBF && (zfbyte)p[2] <= 0xBF && (zfbyte)p[3] <= 0xBF && (zfbyte)p[4] <= 0xBF && (zfbyte)p[5] <= 0xBF)
    {
        return 6;
    }
    else if((zfbyte)p[0] <= 0xFE && (zfbyte)p[1] <= 0xBF && (zfbyte)p[2] <= 0xBF && (zfbyte)p[3] <= 0xBF && (zfbyte)p[4] <= 0xBF && (zfbyte)p[5] <= 0xBF && (zfbyte)p[6] <= 0xBF)
    {
        return 7;
    }
    else if((zfbyte)p[1] <= 0xBF && (zfbyte)p[2] <= 0xBF && (zfbyte)p[3] <= 0xBF && (zfbyte)p[4] <= 0xBF && (zfbyte)p[5] <= 0xBF && (zfbyte)p[6] <= 0xBF && (zfbyte)p[7] <= 0xBF)
    {
        return 8;
    }
    else
    {
        return 1;
    }
}

// ============================================================
zfint zfsicmp(const zfchar *s1, const zfchar *s2)
{
    while(*s1 && *s2 && (
        (*s1 == *s2)
        || (*s1 >= 'A' && *s1 <= 'Z' && *s1 + 32 == *s2)
        || (*s1 >= 'a' && *s1 <= 'a' && *s1 - 32 == *s2)))
    {
        ++s1;
        ++s2;
    }
    return (((zfint)(*s1)) - ((zfint)(*s2)));
}
zfint zfsnicmp(const zfchar *s1, const zfchar *s2, zfindex count)
{
    if(count == 0)
    {
        return 0;
    }
    while(--count && *s1 && *s2 && (
        (*s1 == *s2)
        || (*s1 >= 'A' && *s1 <= 'Z' && *s1 + 32 == *s2)
        || (*s1 >= 'a' && *s1 <= 'a' && *s1 - 32 == *s2)))
    {
        ++s1;
        ++s2;
    }
    return (((zfint)(*s1)) - ((zfint)(*s2)));
}

ZF_NAMESPACE_GLOBAL_END

