#include "ZFBase64.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFEXPORT_VAR_READONLY_DEFINE(const zfchar *, ZFBase64TableDefault, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/")
ZFEXPORT_VAR_READONLY_DEFINE(zfchar, ZFBase64PadDefault, '=')
ZFEXPORT_VAR_READONLY_DEFINE(zfindex, ZFBase64LineBreakPosStandard, 76)
ZFEXPORT_VAR_READONLY_DEFINE(zfindex, ZFBase64LineBreakPosNone, zfindexMax())

// ============================================================
// encode
ZFMETHOD_FUNC_DEFINE_2(zfindex, ZFBase64EncodeCalcSize,
                       ZFMP_IN(zfindex, srcLen),
                       ZFMP_IN_OPT(zfindex, lineBreakPos, ZFBase64LineBreakPosNone()))
{
    if(srcLen == zfindexMax())
    {
        return zfindexMax();
    }
    srcLen = ((srcLen * 4) + 2) / 3;
    if(lineBreakPos != ZFBase64LineBreakPosNone() && srcLen > 0)
    {
        srcLen += (srcLen - 1) / lineBreakPos;
    }
    return (srcLen + 1);
}

zfbool ZFBase64Encode(ZF_OUT zfchar *buf,
                      ZF_IN const void *src,
                      ZF_IN_OPT zfindex srcLen /* = zfindexMax() */,
                      ZF_OUT_OPT zfindex *outResultSize /* = zfnull */,
                      ZF_IN_OPT const zfchar *table /* = ZFBase64TableDefault() */,
                      ZF_IN_OPT zfchar pad /* = ZFBase64PadDefault() */,
                      ZF_IN_OPT zfindex lineBreakPos /* = ZFBase64LineBreakPosNone() */)
{
    return ZFBase64Encode(ZFOutputForBufferUnsafe(buf), ZFInputForBufferUnsafe(src, srcLen), outResultSize, table, pad, lineBreakPos);
}
ZFMETHOD_FUNC_DEFINE_6(zfbool, ZFBase64Encode,
                       ZFMP_IN_OUT(const ZFOutput &, outputCallback),
                       ZFMP_IN(const ZFInput &, inputCallback),
                       ZFMP_OUT_OPT(zfindex *, outResultSize, zfnull),
                       ZFMP_IN_OPT(const zfchar *, table, ZFBase64TableDefault()),
                       ZFMP_IN_OPT(zfchar, pad, ZFBase64PadDefault()),
                       ZFMP_IN_OPT(zfindex, lineBreakPos, ZFBase64LineBreakPosNone()))
{
    if(!outputCallback.callbackIsValid() || !inputCallback.callbackIsValid())
    {
        return zffalse;
    }

    static const zfindex OUTPUT_BUF_SIZE = 32;
    zfchar outputBuf[OUTPUT_BUF_SIZE] = {0};
    zfchar *pOutput = outputBuf;
    zfchar *pOutputEnd = pOutput + (OUTPUT_BUF_SIZE - 5); // should at least be able to hold 4 bytes + 1 line break
    zfindex writtenCount = 0;
    zfchar *pOutputLineBegin = pOutput;

    static const zfindex INPUT_BUF_SIZE = 30; // must be multiple of 3 bytes (3 bytes data converted to 4 bytes base64)
    zfbyte inputBuf[INPUT_BUF_SIZE] = {0};
    zfbyte *pInput = inputBuf;
    zfbyte *pInputEnd = inputBuf + inputCallback.execute(inputBuf, INPUT_BUF_SIZE);
    do
    {
        if(pInputEnd - pInput < 3) {break;}

        *pOutput++ = (zfchar)table[(pInput[0] >> 2) & 0x3F];
        *pOutput++ = (zfchar)table[((pInput[0] & 0x03) << 4) | ((pInput[1] & 0xF0) >> 4)];
        *pOutput++ = (zfchar)table[((pInput[1] & 0x0F) << 2) | ((pInput[2] & 0xC0) >> 6)];
        *pOutput++ = (zfchar)table[pInput[2] & 0x3F];

        pInput += 3;
        if(pInput == pInputEnd)
        {
            pInputEnd = inputBuf + inputCallback.execute(inputBuf, INPUT_BUF_SIZE);
            pInput = inputBuf;
        }
        if((zfindex)(pOutput - pOutputLineBegin) >= lineBreakPos && pInput < pInputEnd)
        {
            *pOutput++ = '\n';
            pOutputLineBegin = pOutput;
        }
        if(pOutput >= pOutputEnd)
        {
            outputCallback.execute(outputBuf, (pOutput - outputBuf) * sizeof(zfchar));
            writtenCount += pOutput - outputBuf;
            pOutput = outputBuf;
        }
    } while(zftrue);
    if(pInput < pInputEnd)
    {
        *pOutput++ = (zfchar)table[(pInput[0] >> 2) & 0x3F];
        if(pInputEnd - pInput == 2)
        {
            *pOutput++ = (zfchar)table[((pInput[0] & 0x03) << 4) | ((pInput[1] & 0xF0) >> 4)];
            *pOutput++ = (zfchar)table[((pInput[1] & 0x0F) << 2)];
            *pOutput++ = pad;
        }
        else
        {
            *pOutput++ = (zfchar)table[((pInput[0] & 0x03) << 4)];
            *pOutput++ = pad;
            *pOutput++ = pad;
        }
    }
    *pOutput = '\0';
    outputCallback.execute(outputBuf, (pOutput - outputBuf) * sizeof(zfchar));
    writtenCount += pOutput - outputBuf;
    if(outResultSize != zfnull)
    {
        *outResultSize = writtenCount;
    }
    return zftrue;
}

// ============================================================
// decode
ZFMETHOD_FUNC_DEFINE_2(zfindex, ZFBase64DecodeCalcSize,
                       ZFMP_IN(zfindex, srcLen),
                       ZFMP_IN_OPT(zfindex, lineBreakPos, ZFBase64LineBreakPosNone()))
{
    if(srcLen == zfindexMax())
    {
        return zfindexMax();
    }
    return (srcLen * 3 / 4);
}
zfbool ZFBase64Decode(ZF_OUT void *buf,
                      ZF_IN const zfchar *src,
                      ZF_IN_OPT zfindex srcLen /* = zfindexMax() */,
                      ZF_OUT_OPT zfindex *outResultSize /* = zfnull */,
                      ZF_IN_OPT const zfchar *table /* = ZFBase64TableDefault() */,
                      ZF_IN_OPT zfchar pad /* = ZFBase64PadDefault() */)
{
    return ZFBase64Decode(ZFOutputForBufferUnsafe(buf), ZFInputForBufferUnsafe(src, srcLen), outResultSize, table, pad);
}
ZFMETHOD_FUNC_DEFINE_5(zfbool, ZFBase64Decode,
                       ZFMP_IN_OUT(const ZFOutput &, outputCallback),
                       ZFMP_IN(const ZFInput &, inputCallback),
                       ZFMP_OUT_OPT(zfindex *, outResultSize, zfnull),
                       ZFMP_IN_OPT(const zfchar *, table, ZFBase64TableDefault()),
                       ZFMP_IN_OPT(zfchar, pad, ZFBase64PadDefault()))
{
    if(!outputCallback.callbackIsValid() || !inputCallback.callbackIsValid())
    {
        return zffalse;
    }

    zfbyte asciiTable[256];
    zfmemset(asciiTable, 64, 256);
    for(zfindex i = 0; i < 64; ++i) {asciiTable[(zfint)table[i]] = (zfbyte)i;}

    static const zfindex OUTPUT_BUF_SIZE = 32;
    zfbyte outputBuf[OUTPUT_BUF_SIZE] = {0};
    zfbyte *pOutput = (zfbyte *)outputBuf;
    zfbyte *pOutputEnd = pOutput + (OUTPUT_BUF_SIZE - 3); // should at least be able to hold 3 bytes
    zfindex writtenCount = 0;
    zfbool success = zftrue;

    zfchar inputToken[4] = {0};
    static const zfindex INPUT_BUF_SIZE = 32; // must be multiple of 1 bytes (read 1 bytes each time from base64)
    zfchar inputBuf[32] = {0};
    zfchar *pInput = inputBuf;
    zfchar *pInputEnd = inputBuf + inputCallback.execute(inputBuf, INPUT_BUF_SIZE);
    do
    {
        memset(inputToken, 0, 4);

        for(zfindex i = 0; i != 4; )
        {
            while(pInput < pInputEnd && (*pInput == ' ' || *pInput == '\t' || *pInput == '\n' || *pInput == '\r')) {++pInput;}
            if(pInput == pInputEnd)
            {
                pInputEnd = inputBuf + inputCallback.execute(inputBuf, INPUT_BUF_SIZE);
                pInput = inputBuf;
                if(pInput == pInputEnd)
                {
                    break;
                }
                else
                {
                    continue;
                }
            }
            inputToken[i] = *pInput;
            ++pInput;
            if(pInput == pInputEnd)
            {
                pInputEnd = inputBuf + inputCallback.execute(inputBuf, INPUT_BUF_SIZE);
                pInput = inputBuf;
            }
            ++i;
        }

        if(inputToken[0] != '\0')
        {
            for(zfindex i = 0; i < 4; ++i)
            {
                if(inputToken[i] == '\0' || inputToken[i] == pad)
                {
                    inputToken[i] = '\0';
                }
                else if(asciiTable[(zfint)inputToken[i]] >= 64)
                {
                    success = zffalse;
                    break;
                }
            }
            if(!success) {break;}

            *pOutput++ = (zfbyte)((asciiTable[(zfint)inputToken[0]] << 2) | asciiTable[(zfint)inputToken[1]] >> 4);
            if(inputToken[2])
            {
                *pOutput++ = (zfbyte)((asciiTable[(zfint)inputToken[1]] << 4) | asciiTable[(zfint)inputToken[2]] >> 2);
                if(inputToken[3])
                {
                    *pOutput++ = (zfbyte)((asciiTable[(zfint)inputToken[2]] << 6) | asciiTable[(zfint)inputToken[3]]);
                }
            }
        }
        if(pOutput >= pOutputEnd)
        {
            outputCallback.execute(outputBuf, pOutput - outputBuf);
            writtenCount += pOutput - outputBuf;
            pOutput = outputBuf;
        }
    } while(pInput < pInputEnd);
    *pOutput = '\0';
    outputCallback.execute(outputBuf, pOutput - outputBuf);
    writtenCount += pOutput - outputBuf;
    if(outResultSize != zfnull)
    {
        *outResultSize = writtenCount;
    }
    return success;
}

// ============================================================
ZFOBJECT_REGISTER(ZFBase64)

ZFMETHOD_DEFINE_1(ZFBase64, zfindex, base64EncodeCalcSize, ZFMP_IN(zfindex, srcLen))
{
    return ZFBase64EncodeCalcSize(srcLen, this->base64LineBreakPos());
}
ZFMETHOD_DEFINE_3(ZFBase64, zfbool, base64Encode,
                  ZFMP_IN_OUT(const ZFOutput &, output),
                  ZFMP_IN(const ZFInput &, input),
                  ZFMP_OUT_OPT(zfindex *, outResultSize, zfnull))
{
    return ZFBase64Encode(output, input, outResultSize, this->base64Table(), this->base64Pad(), this->base64LineBreakPos());
}
ZFMETHOD_DEFINE_1(ZFBase64, zfindex, base64DecodeCalcSize, ZFMP_IN(zfindex, srcLen))
{
    return ZFBase64DecodeCalcSize(srcLen, this->base64LineBreakPos());
}
ZFMETHOD_DEFINE_3(ZFBase64, zfbool, base64Decode,
                  ZFMP_IN_OUT(const ZFOutput &, output),
                  ZFMP_IN(const ZFInput &, input),
                  ZFMP_OUT_OPT(zfindex *, outResultSize, zfnull))
{
    return ZFBase64Decode(output, input, outResultSize, this->base64Table(), this->base64Pad());
}

ZF_NAMESPACE_GLOBAL_END

