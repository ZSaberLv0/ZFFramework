#include "ZFCrc32.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFEXPORT_VAR_READONLY_DEFINE(zfflags, ZFCrc32ValueZero, ((zfflags)0x0))
ZFEXPORT_VAR_READONLY_DEFINE(zfflags, ZFCrc32ValueInvalid, ((zfflags)0xFFFFFFFF))

// ============================================================
// ZFCrc32 initializer
static zfbool _ZFP_ZFCrc32_prepareCrc32Table(ZF_IN_OUT zft_zfuint32 *table)
{
    zft_zfuint32 t = 0;
    for(zft_zfuint32 i = 0; i < 256; ++i)
    {
        t = i;
        for(zft_zfuint32 j = 0; j < 8; ++j)
        {
            if(t & 1)
                t = (t >> 1) ^ 0xEDB88320L;
            else
                t >>= 1;
        }
        table[i] = t;
    }
    return zftrue;
}
static zft_zfuint32 *_ZFP_ZFCrc32TableRef(void)
{
    static zft_zfuint32 d[256] = {0};
    static zfbool dummy = _ZFP_ZFCrc32_prepareCrc32Table(d);
    (void)dummy;
    return d;
}
#define _ZFP_ZFCrc32Table (_ZFP_ZFCrc32TableRef())

// ============================================================
// ZFCrc32
zfflags zfCrc32Calc(ZF_IN const void *src,
                    ZF_IN zfindex len,
                    ZF_IN_OPT zfflags prevResult /* = ZFCrc32ValueZero() */)
{
    if(src == zfnull) {return ZFCrc32ValueInvalid();}
    const zfbyte *p = (const zfbyte *)src;
    zft_zfuint32 ret = prevResult;
    ret ^= ZFCrc32ValueInvalid();
    for(const zfbyte *pEnd = p + len; p != pEnd; ++p)
    {
        ret = _ZFP_ZFCrc32Table[(ret ^ (*p)) & 0xFF] ^ (ret >> 8);
    }
    ret ^= ZFCrc32ValueInvalid();
    return zfflags(ret);
}
ZFMETHOD_FUNC_DEFINE_2(zfflags, zfCrc32Calc,
                       ZFMP_IN(const ZFInput &, callback),
                       ZFMP_IN_OPT(zfflags, prevResult, ZFCrc32ValueZero()))
{
    if(!callback.callbackIsValid()) {return ZFCrc32ValueInvalid();}

    zfbyte buf[1024] = {0};
    zfindex readCount = 0;
    zft_zfuint32 ret = prevResult;
    ret ^= ZFCrc32ValueInvalid();
    while((readCount = callback.execute(buf, 1024)) > 0)
    {
        const zfbyte *p = buf;
        const zfbyte *pEnd = buf + readCount;
        for(; p != pEnd; ++p)
        {
            ret = _ZFP_ZFCrc32Table[(ret ^ (*p)) & 0xFF] ^ (ret >> 8);
        }
    }
    ret ^= ZFCrc32ValueInvalid();
    return zfflags(ret);
}
ZFMETHOD_FUNC_DEFINE_3(zfflags, zfCrc32Calc,
                       ZFMP_IN(const zfchar *, src),
                       ZFMP_IN_OPT(zfindex, len, zfindexMax()),
                       ZFMP_IN_OPT(zfflags, prevResult, ZFCrc32ValueZero()))
{
    return zfCrc32Calc((const void *)src, ((len == zfindexMax()) ? zfslen(src) : len), prevResult);
}

ZF_NAMESPACE_GLOBAL_END

