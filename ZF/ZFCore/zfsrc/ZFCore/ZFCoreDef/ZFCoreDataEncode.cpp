#include "ZFCoreDataEncode.h"

ZF_NAMESPACE_GLOBAL_BEGIN

const zfchar _ZFP_zfCoreDataEncodeCharMapDefault[256] = {
    // 0x00 ~ 0x0F
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // 0x10 ~ 0x1F
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // 0x20 ~ 0x2F
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // 0x30 ~ 0x3F
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
    // 0x40 ~ 0x4F
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    // 0x50 ~ 0x5F
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
    // 0x60 ~ 0x6F
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    // 0x70 ~ 0x7F
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
    // 0x80 ~ 0x8F
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // 0x90 ~ 0x9F
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // 0xA0 ~ 0xAF
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // 0xB0 ~ 0xBF
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // 0xC0 ~ 0xCF
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // 0xD0 ~ 0xDF
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // 0xE0 ~ 0xEF
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // 0xF0 ~ 0xFF
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

void zfCoreDataEncode(ZF_OUT zfstring &result,
                      ZF_IN const zfchar *src,
                      ZF_IN_OPT zfindex srcLen /* = zfindexMax() */,
                      ZF_IN_OPT const zfchar *charMap /* = zfCoreDataEncodeCharMapDefault() */,
                      ZF_IN_OPT zfchar escapeToken /* = '%' */)
{
    if(src == zfnull) {return;}
    const zfchar *srcEnd = (src + ((srcLen == zfindexMax()) ? zfslen(src) : srcLen));
    while(src < srcEnd)
    {
        if(*src == escapeToken)
        {
            result += escapeToken;
            result += escapeToken;
        }
        else if(charMap[(zfbyte)(*src)])
        {
            result += *src;
        }
        else
        {
            result += escapeToken;
            zfbyte t = ((*src >> 4) & 0x0F);
            result += ((t <= 9) ? ('0' + t) : ('A' + (t - 10)));
            t = ((*src) & 0x0F);
            result += ((t <= 9) ? ('0' + t) : ('A' + (t - 10)));
        }
        ++src;
    }
}
void zfCoreDataDecode(ZF_OUT zfstring &result,
                      ZF_IN const zfchar *src,
                      ZF_IN_OPT zfindex srcLen /* = zfindexMax() */,
                      ZF_IN_OPT zfchar escapeToken /* = '%' */)
{
    if(src == zfnull) {return ;}
    const zfchar *srcEnd = (src + ((srcLen == zfindexMax()) ? zfslen(src) : srcLen));
    while(src < srcEnd)
    {
        if(*src == escapeToken)
        {
            if(src[1] == escapeToken)
            {
                result += escapeToken;
                src += 2;
            }
            else if(((src[1] >= '0' && src[1] <= '9') || (src[1] >= 'A' && src[1] <= 'F'))
                && ((src[1] >= '0' && src[1] <= '9') || (src[1] >= 'A' && src[1] <= 'F')))
            {
                result += (zfchar)(
                        ((((src[1] <= '9') ? (src[1] - '0') : (src[1] - 'A' + 10)) << 4) & 0xF0)
                        | (((src[2] <= '9') ? (src[2] - '0') : (src[2] - 'A' + 10))& 0x0F)
                    );
                src += 3;
            }
            else
            {
                result += escapeToken;
                ++src;
            }
        }
        else
        {
            result += *src;
            ++src;
        }
    }
}

ZF_NAMESPACE_GLOBAL_END

