#include "ZFTimeLineCurve.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFTimeLineCurve)

ZFMETHOD_DEFINE_1(ZFTimeLineCurve, zffloat, progressUpdate,
                  ZFMP_IN(zffloat, time))
{
    return this->progressOnUpdate(time);
}

ZF_NAMESPACE_GLOBAL_END

