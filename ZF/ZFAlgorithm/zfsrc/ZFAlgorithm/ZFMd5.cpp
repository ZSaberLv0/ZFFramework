#include "ZFMd5.h"

ZF_NAMESPACE_GLOBAL_BEGIN

typedef struct {
    zft_zfuint32 lo, hi;
    zft_zfuint32 a, b, c, d;
    zfbyte buffer[64];
    zft_zfuint32 block[16];
} _ZFP_ZFMd5_CTX;

static void _ZFP_ZFMd5_Init(_ZFP_ZFMd5_CTX *ctx);
static void _ZFP_ZFMd5_Update(_ZFP_ZFMd5_CTX *ctx, const void *data, zfindex size);
static void _ZFP_ZFMd5_Final(zfbyte *result, _ZFP_ZFMd5_CTX *ctx);

#define _ZFP_ZFMd5_blockSize 512
void zfMd5Calc(ZF_IN_OUT zfstring &ret,
               ZF_IN const void *src,
               ZF_IN zfindex len,
               ZF_IN_OPT zfbool upperCase /* = zftrue */)
{
    _ZFP_ZFMd5_CTX md5Ctx;
    _ZFP_ZFMd5_Init(&md5Ctx);

    const zfbyte *srcTmp = (const zfbyte *)src;
    const zfbyte *srcEndTmp = srcTmp + len;
    while(srcEndTmp - srcTmp >= _ZFP_ZFMd5_blockSize)
    {
        _ZFP_ZFMd5_Update(&md5Ctx, srcTmp, _ZFP_ZFMd5_blockSize);
        srcTmp += _ZFP_ZFMd5_blockSize;
    }
    if(srcTmp < srcEndTmp)
    {
        _ZFP_ZFMd5_Update(&md5Ctx, srcTmp, srcEndTmp - srcTmp);
    }

    zfbyte result[16] = {0};
    _ZFP_ZFMd5_Final((zfbyte *)result, &md5Ctx);

    if(upperCase)
    {
        for(zfindex i = 0; i < 16; ++i)
        {
            zfstringAppend(ret, "%02X", result[i]);
        }
    }
    else
    {
        for(zfindex i = 0; i < 16; ++i)
        {
            zfstringAppend(ret, "%02x", result[i]);
        }
    }
}
ZFMETHOD_FUNC_DEFINE_3(void, zfMd5Calc,
                       ZFMP_IN_OUT(zfstring &, ret),
                       ZFMP_IN(const ZFInput &, callback),
                       ZFMP_IN_OPT(zfbool, upperCase, zftrue))
{
    if(!callback.callbackIsValid())
    {
        return ;
    }
    _ZFP_ZFMd5_CTX md5Ctx;
    _ZFP_ZFMd5_Init(&md5Ctx);

    zfbyte buf[_ZFP_ZFMd5_blockSize] = {0};
    do
    {
        zfindex read = callback.execute(buf, _ZFP_ZFMd5_blockSize);
        if(read > 0)
        {
            _ZFP_ZFMd5_Update(&md5Ctx, buf, read);
            if(read < _ZFP_ZFMd5_blockSize)
            {
                break;
            }
        }
        else
        {
            break;
        }
    } while(zftrue);

    zfbyte result[16] = {0};
    _ZFP_ZFMd5_Final((zfbyte *)result, &md5Ctx);

    if(upperCase)
    {
        for(zfindex i = 0; i < 16; ++i)
        {
            zfstringAppend(ret, "%02X", result[i]);
        }
    }
    else
    {
        for(zfindex i = 0; i < 16; ++i)
        {
            zfstringAppend(ret, "%02x", result[i]);
        }
    }
}
ZFMETHOD_FUNC_DEFINE_2(zfstring, zfMd5Calc,
                       ZFMP_IN(const ZFInput &, callback),
                       ZFMP_IN_OPT(zfbool, upperCase, zftrue))
{
    zfstring ret;
    zfMd5Calc(ret, callback, upperCase);
    return ret;
}

/*
 * The basic MD5 functions.
 *
 * F and G are optimized compared to their RFC 1321 definitions for
 * architectures that lack an AND-NOT instruction, just like in Colin Plumb's
 * implementation.
 */
#define F(x, y, z) ((z) ^ ((x) & ((y) ^ (z))))
#define G(x, y, z) ((y) ^ ((z) & ((x) ^ (y))))
#define H(x, y, z) (((x) ^ (y)) ^ (z))
#define H2(x, y, z) ((x) ^ ((y) ^ (z)))
#define I(x, y, z) ((y) ^ ((x) | ~(z)))

/*
 * The MD5 transformation for all four rounds.
 */
#define STEP(f, a, b, c, d, x, t, s) \
    (a) += f((b), (c), (d)) + (x) + (t); \
    (a) = (((a) << (s)) | (((a) & 0xffffffff) >> (32 - (s)))); \
    (a) += (b);

/*
 * SET reads 4 input bytes in little-endian byte order and stores them
 * in a properly aligned word in host byte order.
 *
 * The check for little-endian architectures that tolerate unaligned
 * memory accesses is just an optimization.  Nothing will break if it
 * doesn't work.
 */
#if defined(__i386__) || defined(__x86_64__) || defined(__vax__)
    #define SET(n) \
        (*(zft_zfuint32 *)&ptr[(n) * 4])
    #define GET(n) \
        SET(n)
#else
    #define SET(n) \
        (ctx->block[(n)] = \
        (zft_zfuint32)ptr[(n) * 4] | \
        ((zft_zfuint32)ptr[(n) * 4 + 1] << 8) | \
        ((zft_zfuint32)ptr[(n) * 4 + 2] << 16) | \
        ((zft_zfuint32)ptr[(n) * 4 + 3] << 24))
    #define GET(n) \
        (ctx->block[(n)])
#endif

/*
 * This processes one or more 64-byte data blocks, but does NOT update
 * the bit counters.  There are no alignment requirements.
 */
static const void *body(_ZFP_ZFMd5_CTX *ctx, const void *data, zfindex size)
{
    const zfbyte *ptr;
    zft_zfuint32 a, b, c, d;
    zft_zfuint32 saved_a, saved_b, saved_c, saved_d;

    ptr = (const zfbyte *)data;

    a = ctx->a;
    b = ctx->b;
    c = ctx->c;
    d = ctx->d;

    do {
        saved_a = a;
        saved_b = b;
        saved_c = c;
        saved_d = d;

        /* Round 1 */
        STEP(F, a, b, c, d, SET(0), 0xd76aa478, 7)
        STEP(F, d, a, b, c, SET(1), 0xe8c7b756, 12)
        STEP(F, c, d, a, b, SET(2), 0x242070db, 17)
        STEP(F, b, c, d, a, SET(3), 0xc1bdceee, 22)
        STEP(F, a, b, c, d, SET(4), 0xf57c0faf, 7)
        STEP(F, d, a, b, c, SET(5), 0x4787c62a, 12)
        STEP(F, c, d, a, b, SET(6), 0xa8304613, 17)
        STEP(F, b, c, d, a, SET(7), 0xfd469501, 22)
        STEP(F, a, b, c, d, SET(8), 0x698098d8, 7)
        STEP(F, d, a, b, c, SET(9), 0x8b44f7af, 12)
        STEP(F, c, d, a, b, SET(10), 0xffff5bb1, 17)
        STEP(F, b, c, d, a, SET(11), 0x895cd7be, 22)
        STEP(F, a, b, c, d, SET(12), 0x6b901122, 7)
        STEP(F, d, a, b, c, SET(13), 0xfd987193, 12)
        STEP(F, c, d, a, b, SET(14), 0xa679438e, 17)
        STEP(F, b, c, d, a, SET(15), 0x49b40821, 22)

        /* Round 2 */
        STEP(G, a, b, c, d, GET(1), 0xf61e2562, 5)
        STEP(G, d, a, b, c, GET(6), 0xc040b340, 9)
        STEP(G, c, d, a, b, GET(11), 0x265e5a51, 14)
        STEP(G, b, c, d, a, GET(0), 0xe9b6c7aa, 20)
        STEP(G, a, b, c, d, GET(5), 0xd62f105d, 5)
        STEP(G, d, a, b, c, GET(10), 0x02441453, 9)
        STEP(G, c, d, a, b, GET(15), 0xd8a1e681, 14)
        STEP(G, b, c, d, a, GET(4), 0xe7d3fbc8, 20)
        STEP(G, a, b, c, d, GET(9), 0x21e1cde6, 5)
        STEP(G, d, a, b, c, GET(14), 0xc33707d6, 9)
        STEP(G, c, d, a, b, GET(3), 0xf4d50d87, 14)
        STEP(G, b, c, d, a, GET(8), 0x455a14ed, 20)
        STEP(G, a, b, c, d, GET(13), 0xa9e3e905, 5)
        STEP(G, d, a, b, c, GET(2), 0xfcefa3f8, 9)
        STEP(G, c, d, a, b, GET(7), 0x676f02d9, 14)
        STEP(G, b, c, d, a, GET(12), 0x8d2a4c8a, 20)

        /* Round 3 */
        STEP(H, a, b, c, d, GET(5), 0xfffa3942, 4)
        STEP(H2, d, a, b, c, GET(8), 0x8771f681, 11)
        STEP(H, c, d, a, b, GET(11), 0x6d9d6122, 16)
        STEP(H2, b, c, d, a, GET(14), 0xfde5380c, 23)
        STEP(H, a, b, c, d, GET(1), 0xa4beea44, 4)
        STEP(H2, d, a, b, c, GET(4), 0x4bdecfa9, 11)
        STEP(H, c, d, a, b, GET(7), 0xf6bb4b60, 16)
        STEP(H2, b, c, d, a, GET(10), 0xbebfbc70, 23)
        STEP(H, a, b, c, d, GET(13), 0x289b7ec6, 4)
        STEP(H2, d, a, b, c, GET(0), 0xeaa127fa, 11)
        STEP(H, c, d, a, b, GET(3), 0xd4ef3085, 16)
        STEP(H2, b, c, d, a, GET(6), 0x04881d05, 23)
        STEP(H, a, b, c, d, GET(9), 0xd9d4d039, 4)
        STEP(H2, d, a, b, c, GET(12), 0xe6db99e5, 11)
        STEP(H, c, d, a, b, GET(15), 0x1fa27cf8, 16)
        STEP(H2, b, c, d, a, GET(2), 0xc4ac5665, 23)

        /* Round 4 */
        STEP(I, a, b, c, d, GET(0), 0xf4292244, 6)
        STEP(I, d, a, b, c, GET(7), 0x432aff97, 10)
        STEP(I, c, d, a, b, GET(14), 0xab9423a7, 15)
        STEP(I, b, c, d, a, GET(5), 0xfc93a039, 21)
        STEP(I, a, b, c, d, GET(12), 0x655b59c3, 6)
        STEP(I, d, a, b, c, GET(3), 0x8f0ccc92, 10)
        STEP(I, c, d, a, b, GET(10), 0xffeff47d, 15)
        STEP(I, b, c, d, a, GET(1), 0x85845dd1, 21)
        STEP(I, a, b, c, d, GET(8), 0x6fa87e4f, 6)
        STEP(I, d, a, b, c, GET(15), 0xfe2ce6e0, 10)
        STEP(I, c, d, a, b, GET(6), 0xa3014314, 15)
        STEP(I, b, c, d, a, GET(13), 0x4e0811a1, 21)
        STEP(I, a, b, c, d, GET(4), 0xf7537e82, 6)
        STEP(I, d, a, b, c, GET(11), 0xbd3af235, 10)
        STEP(I, c, d, a, b, GET(2), 0x2ad7d2bb, 15)
        STEP(I, b, c, d, a, GET(9), 0xeb86d391, 21)

        a += saved_a;
        b += saved_b;
        c += saved_c;
        d += saved_d;

        ptr += 64;
    } while (size -= 64);

    ctx->a = a;
    ctx->b = b;
    ctx->c = c;
    ctx->d = d;

    return ptr;
}

void _ZFP_ZFMd5_Init(_ZFP_ZFMd5_CTX *ctx)
{
    ctx->a = 0x67452301;
    ctx->b = 0xefcdab89;
    ctx->c = 0x98badcfe;
    ctx->d = 0x10325476;

    ctx->lo = 0;
    ctx->hi = 0;
}

void _ZFP_ZFMd5_Update(_ZFP_ZFMd5_CTX *ctx, const void *data, zfindex size)
{
    zft_zfuint32 saved_lo;
    zfindex used, available;

    saved_lo = ctx->lo;
    if ((ctx->lo = (saved_lo + size) & 0x1fffffff) < saved_lo)
        ctx->hi++;
    ctx->hi += size >> 29;

    used = saved_lo & 0x3f;

    if (used) {
        available = 64 - used;

        if (size < available) {
            memcpy(&ctx->buffer[used], data, size);
            return;
        }

        memcpy(&ctx->buffer[used], data, available);
        data = (const zfbyte *)data + available;
        size -= available;
        body(ctx, ctx->buffer, 64);
    }

    if (size >= 64) {
        data = body(ctx, data, size & ~(zfindex)0x3f);
        size &= 0x3f;
    }

    memcpy(ctx->buffer, data, size);
}

void _ZFP_ZFMd5_Final(zfbyte *result, _ZFP_ZFMd5_CTX *ctx)
{
    zfindex used, available;

    used = ctx->lo & 0x3f;

    ctx->buffer[used++] = 0x80;

    available = 64 - used;

    if (available < 8) {
        memset(&ctx->buffer[used], 0, available);
        body(ctx, ctx->buffer, 64);
        used = 0;
        available = 64;
    }

    memset(&ctx->buffer[used], 0, available - 8);

    ctx->lo <<= 3;
    ctx->buffer[56] = (zfbyte)(ctx->lo);
    ctx->buffer[57] = (zfbyte)(ctx->lo >> 8);
    ctx->buffer[58] = (zfbyte)(ctx->lo >> 16);
    ctx->buffer[59] = (zfbyte)(ctx->lo >> 24);
    ctx->buffer[60] = (zfbyte)(ctx->hi);
    ctx->buffer[61] = (zfbyte)(ctx->hi >> 8);
    ctx->buffer[62] = (zfbyte)(ctx->hi >> 16);
    ctx->buffer[63] = (zfbyte)(ctx->hi >> 24);

    body(ctx, ctx->buffer, 64);

    result[0] = (zfbyte)(ctx->a);
    result[1] = (zfbyte)(ctx->a >> 8);
    result[2] = (zfbyte)(ctx->a >> 16);
    result[3] = (zfbyte)(ctx->a >> 24);
    result[4] = (zfbyte)(ctx->b);
    result[5] = (zfbyte)(ctx->b >> 8);
    result[6] = (zfbyte)(ctx->b >> 16);
    result[7] = (zfbyte)(ctx->b >> 24);
    result[8] = (zfbyte)(ctx->c);
    result[9] = (zfbyte)(ctx->c >> 8);
    result[10] = (zfbyte)(ctx->c >> 16);
    result[11] = (zfbyte)(ctx->c >> 24);
    result[12] = (zfbyte)(ctx->d);
    result[13] = (zfbyte)(ctx->d >> 8);
    result[14] = (zfbyte)(ctx->d >> 16);
    result[15] = (zfbyte)(ctx->d >> 24);

    memset(ctx, 0, sizeof(*ctx));
}

#undef F
#undef G
#undef H
#undef H2
#undef I
#undef STEP
#undef SET
#undef GET

ZF_NAMESPACE_GLOBAL_END

