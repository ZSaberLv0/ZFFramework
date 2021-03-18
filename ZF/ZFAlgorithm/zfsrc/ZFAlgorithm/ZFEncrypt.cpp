#include "ZFEncrypt.h"
#include "protocol/ZFProtocolZFEncrypt.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFEncrypt,
                       ZFMP_IN_OUT(const ZFOutput &, output),
                       ZFMP_IN(const ZFInput &, input),
                       ZFMP_IN(const zfchar *, encryptKey))
{
    if(!input.callbackIsValid() || !output.callbackIsValid())
    {
        return zffalse;
    }
    return ZFPROTOCOL_ACCESS(ZFEncrypt)->encrypt(output, input, encryptKey);
}
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFDecrypt,
                       ZFMP_IN_OUT(const ZFOutput &, output),
                       ZFMP_IN(const ZFInput &, input),
                       ZFMP_IN(const zfchar *, encryptKey))
{
    if(!input.callbackIsValid() || !output.callbackIsValid())
    {
        return zffalse;
    }
    return ZFPROTOCOL_ACCESS(ZFEncrypt)->decrypt(output, input, encryptKey);
}

ZF_NAMESPACE_GLOBAL_END

