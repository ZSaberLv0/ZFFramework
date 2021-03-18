#include "ZFCoreStringConvert.h"
#include "ZFCoreArray.h"
#include "ZFCoreDataPairSplit.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfindex zfsCheckMatch(ZF_IN const zfchar **tokens,
                      ZF_IN zfindex tokenCount,
                      ZF_IN const zfchar *toCompare,
                      ZF_IN_OPT zfindex toCompareLength /* = zfindexMax() */)
{
    if(toCompare == zfnull)
    {
        return zfindexMax();
    }

    zfindex tmpLen = 0;
    for(zfindex i = 0; i < tokenCount; ++i)
    {
        tmpLen = zfslen(tokens[i]);
        if(tmpLen <= toCompareLength && zfsncmp(tokens[i], toCompare, tmpLen) == 0)
        {
            return i;
        }
    }
    return zfindexMax();
}

ZF_NAMESPACE_GLOBAL_END

