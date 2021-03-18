#include "ZFCoreUtilMath.h"
#include "ZFCoreStaticInitializer.h"
#include <time.h>
#include <stdlib.h>

ZF_NAMESPACE_GLOBAL_BEGIN

const zffloat zffloatEpsilonT<zffloat>::v = zffloatEpsilon;
const zfdouble zffloatEpsilonT<zfdouble>::v = zfdoubleEpsilon;
const zflongdouble zffloatEpsilonT<zflongdouble>::v = zflongdoubleEpsilon;

// ============================================================
ZF_STATIC_REGISTER_INIT(ZFCoreUtilMathDataHolder)
{
    srand((zfuint)time(zfnull));
}
ZF_STATIC_REGISTER_END(ZFCoreUtilMathDataHolder)

zfuint zfmRand(void)
{
    return (zfuint)rand();
}

ZF_NAMESPACE_GLOBAL_END

