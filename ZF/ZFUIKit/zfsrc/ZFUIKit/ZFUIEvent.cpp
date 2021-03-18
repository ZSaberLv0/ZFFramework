#include "ZFUIEvent.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFUIEvent)

ZFMETHOD_DEFINE_1(ZFUIEvent, void, eventResolved,
                  ZFMP_IN(zfbool const &, value))
{
    _ZFP_ZFUIEvent_eventResolved = value;
}
ZFMETHOD_DEFINE_0(ZFUIEvent, zfbool, eventResolved)
{
    return _ZFP_ZFUIEvent_eventResolved;
}

void ZFUIEvent::objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
{
    zfsuper::objectInfoOnAppend(ret);
    if(this->eventResolved())
    {
        ret += " (resolved)";
    }
    ZFClassUtil::objectPropertyInfo(ret, this);
}

ZF_NAMESPACE_GLOBAL_END

