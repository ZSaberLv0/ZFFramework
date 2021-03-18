#include "UTFCodeUtil.h"
#include <stdlib.h>
#include <string.h>

namespace UTFCodeUtil {

size_t UTF8GetWordCount(const char *src)
{
    size_t size = 0;
    const unsigned char *p = (const unsigned char *)src;
    while(*p != '\0')
    {
        if(*p <= 0x7F)
        {
            ++p;
        }
        else if(*p <= 0xDF && p[1] <= 0xBF)
        {
            p += 2;
        }
        else if(*p <= 0xEF && p[1] <= 0xBF && p[2] <= 0xBF)
        {
            p += 3;
        }
        else if(*p <= 0xF7 && p[1] <= 0xBF && p[2] <= 0xBF && p[3] <= 0xBF)
        {
            p += 4;
        }
        else if(*p <= 0xFB && p[1] <= 0xBF && p[2] <= 0xBF && p[3] <= 0xBF && p[4] <= 0xBF)
        {
            p += 5;
        }
        else if(*p <= 0xFD && p[1] <= 0xBF && p[2] <= 0xBF && p[3] <= 0xBF && p[4] <= 0xBF && p[5] <= 0xBF)
        {
            p += 6;
        }
        else if(*p <= 0xFE && p[1] <= 0xBF && p[2] <= 0xBF && p[3] <= 0xBF && p[4] <= 0xBF && p[5] <= 0xBF && p[6] <= 0xBF)
        {
            p += 7;
        }
        else if(p[1] <= 0xBF && p[2] <= 0xBF && p[3] <= 0xBF && p[4] <= 0xBF && p[5] <= 0xBF && p[6] <= 0xBF && p[7] <= 0xBF)
        {
            p += 8;
        }
        else
        {
            return (size_t)-1;
        }
        ++size;
    }
    return size;
}

char *UTF16ToUTF8(const wchar_t *src)
{
    if(src == NULL) {return NULL;}

    size_t size = 0;
    const wchar_t *p = src;
    while(*p != '\0')
    {
        if (*p <= 0x007F)
        {
            ++size;
        }
        else if(*p <= 0x07FF)
        {
            size += 2;
        }
        else
        {
            size += 3;
        }
        ++p;
    }

    char *result = (char *)malloc(sizeof(char) * (size + 1));
    p = src;
    char *p2 = result;
    while(*p != '\0')
    {
        if (*p <= 0x007F)
        {
            *p2++ = (unsigned char)(*p);
        }
        else if(*p <= 0x07FF)
        {
            *p2++ = (unsigned char)(((*p >> 6) & 0x1F) | 0xC0);
            *p2++ = (unsigned char)((*p & 0x3F) | 0x80);
        }
        else
        {
            *p2++ = (unsigned char)(((*p >> 12) & 0x0F) | 0xE0);
            *p2++ = (unsigned char)(((*p >> 6) & 0x3F) | 0x80);
            *p2++ = (unsigned char)((*p & 0x3F) | 0x80);
        }
        ++p;
    }
    *p2 = '\0';

    return result;
}
wchar_t *UTF8ToUTF16(const char *src)
{
    if(src == NULL) {return NULL;}

    size_t size = UTF8GetWordCount(src);
    const unsigned char *p = (const unsigned char *)src;

    wchar_t *result = (wchar_t *)malloc(sizeof(wchar_t) * (size + 1));
    p = (const unsigned char *)src;
    wchar_t *p2 = result;
    while(*p != '\0')
    {
        if(*p <= 0x7F)
        {
            *p2 = *p;
            ++p;
        }
        else if(*p <= 0xDF && p[1] <= 0xBF)
        {
            *p2 = ((*p++ & 0x1F) << 6);
            *p2 |= (*p++ & 0x3F);
        }
        else if(*p <= 0xEF && p[1] <= 0xBF && p[2] <= 0xBF)
        {
            *p2 = ((*p++ & 0x0F) << 12);
            *p2 |= ((*p++ & 0x3F) << 6);
            *p2 |= (*p++ & 0x3F);
        }
        else if(*p <= 0xF7 && p[1] <= 0xBF && p[2] <= 0xBF && p[3] <= 0xBF)
        {
            *p2 = ' ';
            p += 4;
        }
        else if(*p <= 0xFB && p[1] <= 0xBF && p[2] <= 0xBF && p[3] <= 0xBF && p[4] <= 0xBF)
        {
            *p2 = ' ';
            p += 5;
        }
        else if(*p <= 0xFD && p[1] <= 0xBF && p[2] <= 0xBF && p[3] <= 0xBF && p[4] <= 0xBF && p[5] <= 0xBF)
        {
            *p2 = ' ';
            p += 6;
        }
        else if(*p <= 0xFE && p[1] <= 0xBF && p[2] <= 0xBF && p[3] <= 0xBF && p[4] <= 0xBF && p[5] <= 0xBF && p[6] <= 0xBF)
        {
            *p2 = ' ';
            p += 7;
        }
        else if(p[1] <= 0xBF && p[2] <= 0xBF && p[3] <= 0xBF && p[4] <= 0xBF && p[5] <= 0xBF && p[6] <= 0xBF && p[7] <= 0xBF)
        {
            *p2 = ' ';
            p += 8;
        }
        else
        {
            free(result);
            return NULL;
        }
        ++p2;
    }
    *p2 = '\0';

    return result;
}
wchar_t *UTF16ToUTF16BE(const wchar_t *src)
{
    if(src == NULL) {return NULL;}
    wchar_t *result = (wchar_t *)malloc(sizeof(wchar_t) * (wcslen(src) + 1));
    wchar_t *p = result;
    while(*src != '\0')
    {
        *p = (((*src) << 8) & 0xFF00) | (((*src) >> 8) & 0xFF);
        ++p;
        ++src;
    }
    *p = '\0';
    return result;
}
wchar_t *UTF16BEToUTF16(const wchar_t *src)
{
    return UTFCodeUtil::UTF16ToUTF16BE(src);
}

} // namespace UTFCodeUtil
