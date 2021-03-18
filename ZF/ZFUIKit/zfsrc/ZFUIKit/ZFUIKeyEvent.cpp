#include "ZFUIKeyEvent.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFUIKeyEvent)

void ZFUIKeyEvent::objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
{
    ret += ZFUIKeyAction::EnumNameForValue(this->keyAction);
    ret += " ";
    ret += ZFUIKeyCode::EnumNameForValue(this->keyCode);

    if(this->eventResolved())
    {
        ret += " (resolved)";
    }
}

// ============================================================
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFUIKeyEvent, zfidentity, keyId)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFUIKeyEvent, ZFUIKeyActionEnum, keyAction)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFUIKeyEvent, ZFUIKeyCodeEnum, keyCode)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFUIKeyEvent, zfflags, keyCodeRaw)

ZF_NAMESPACE_GLOBAL_END

