#include "ZFString.h"
#include "protocol/ZFProtocolZFString.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFENUM_DEFINE(ZFStringEncoding)

// ============================================================
// conversion
zfbool zfstringToUTF8(ZF_OUT zfstring &result,
                      ZF_IN const void *s,
                      ZF_IN ZFStringEncodingEnum srcEncoding)
{
    if(s == zfnull) {return zftrue;}
    return ZFPROTOCOL_ACCESS(ZFString)->toUTF8(result, s, srcEncoding);
}

zfbool zfstringToUTF16(ZF_OUT zfstringW &result,
                       ZF_IN const void *s,
                       ZF_IN ZFStringEncodingEnum srcEncoding)
{
    if(s == zfnull) {return zftrue;}
    return ZFPROTOCOL_ACCESS(ZFString)->toUTF16(result, s, srcEncoding);
}
zfbool zfstringToUTF16BE(ZF_OUT zfstringW &result,
                         ZF_IN const void *s,
                         ZF_IN ZFStringEncodingEnum srcEncoding)
{
    if(s == zfnull) {return zftrue;}
    return ZFPROTOCOL_ACCESS(ZFString)->toUTF16BE(result, s, srcEncoding);
}

zfindex zfstringWordCountOfUTF8(ZF_IN const zfchar *utf8String)
{
    if(utf8String == zfnull) {return 0;}
    return ZFPROTOCOL_ACCESS(ZFString)->wordCountOfUTF8(utf8String);
}

ZF_NAMESPACE_GLOBAL_END

