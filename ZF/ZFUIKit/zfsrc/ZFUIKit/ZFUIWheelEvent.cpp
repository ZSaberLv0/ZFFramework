#include "ZFUIWheelEvent.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFUIWheelEvent)

void ZFUIWheelEvent::objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
{
    ret += "wheel (";
    zfsFromIntT(ret, this->wheelX);
    ret += ", ";
    zfsFromIntT(ret, this->wheelY);
    ret += ")";

    if(this->eventResolved())
    {
        ret += " (resolved)";
    }
}

// ============================================================
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFUIWheelEvent, zfint, wheelX)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFUIWheelEvent, zfint, wheelY)

ZF_NAMESPACE_GLOBAL_END

