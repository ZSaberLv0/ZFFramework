#include "ZFUIWheelEvent.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFUIWheelEvent)

void ZFUIWheelEvent::objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
{
    ret += "wheel (";
    zfsFromFloatT(ret, this->wheelX);
    ret += ", ";
    zfsFromFloatT(ret, this->wheelY);
    ret += ")";

    if(this->eventResolved())
    {
        ret += " (resolved)";
    }
}

// ============================================================
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFUIWheelEvent, zffloat, wheelX)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFUIWheelEvent, zffloat, wheelY)

ZF_NAMESPACE_GLOBAL_END

