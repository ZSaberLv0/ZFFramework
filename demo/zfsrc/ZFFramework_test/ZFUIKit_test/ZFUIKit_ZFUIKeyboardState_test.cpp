#include "ZFUIKit_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_GLOBAL_INITIALIZER_INIT(ZFUIKeyboardState_test)
{
    this->keyPressedOnChangeListener = ZFCallbackForFunc(zfself::keyPressedOnChange);
    ZFObjectGlobalEventObserver().observerAdd(ZFUIKeyboardState::EventKeyPressedOnChange(), this->keyPressedOnChangeListener);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIKeyboardState_test)
{
    ZFObjectGlobalEventObserver().observerRemove(ZFUIKeyboardState::EventKeyPressedOnChange(), this->keyPressedOnChangeListener);
}
private:
    ZFListener keyPressedOnChangeListener;
    static ZFLISTENER_PROTOTYPE_EXPAND(keyPressedOnChange)
    {
        zfLogTrimT() << "[ZFUIKeyboardState]" << listenerData.sender();
    }
ZF_GLOBAL_INITIALIZER_END(ZFUIKeyboardState_test)

ZF_NAMESPACE_GLOBAL_END

