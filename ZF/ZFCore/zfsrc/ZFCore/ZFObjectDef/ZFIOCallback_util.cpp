#include "ZFIOCallback_util.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfindex ZFInputReadToOutput(ZF_IN_OUT const ZFOutput &output,
                            ZF_IN_OUT const ZFInput &input)
{
    zfindex size = 0;
    if(input.callbackIsValid() && output.callbackIsValid())
    {
        #define _ZFP_ZFInputReadToOutput_blockSize 256
        zfchar buf[_ZFP_ZFInputReadToOutput_blockSize] = {0};
        zfindex readCount = 0;
        zfindex writeCount = 0;
        do
        {
            readCount = input.execute(buf, _ZFP_ZFInputReadToOutput_blockSize);
            writeCount = output.execute(buf, readCount);
            size += writeCount;
            if(readCount < _ZFP_ZFInputReadToOutput_blockSize || writeCount < readCount)
            {
                break;
            }
        } while(zftrue);
        #undef _ZFP_ZFInputReadToOutput_blockSize
    }
    return size;
}

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfindex, ZFInputReadToOutput, ZFMP_IN_OUT(const ZFOutput &, output), ZFMP_IN_OUT(const ZFInput &, input))

ZF_NAMESPACE_GLOBAL_END
#endif

