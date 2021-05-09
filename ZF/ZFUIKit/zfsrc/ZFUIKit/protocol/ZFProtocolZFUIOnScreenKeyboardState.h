/**
 * @file ZFProtocolZFUIOnScreenKeyboardState.h
 * @brief protocol for #ZFUIOnScreenKeyboardState
 */

#ifndef _ZFI_ZFProtocolZFUIOnScreenKeyboardState_h_
#define _ZFI_ZFProtocolZFUIOnScreenKeyboardState_h_

#include "ZFCore/ZFProtocol.h"
#include "ZFUIKit/ZFUIOnScreenKeyboardState.h"
#include "ZFUIKit/ZFUIViewPositionOnScreen.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for ZFUIOnScreenKeyboardState
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFUIOnScreenKeyboardState)
ZFPROTOCOL_INTERFACE_OPTIONAL
public:
    /**
     * @brief true if the keyboard currently showing
     */
    virtual zfbool keyboardShowing(ZF_IN ZFUIOnScreenKeyboardState *keyboardState) zfpurevirtual;
    /**
     * @brief current key board frame, must return #ZFUIRectZero if not showing
     */
    virtual ZFUIRect keyboardFrame(ZF_IN ZFUIOnScreenKeyboardState *keyboardState) zfpurevirtual;

    /**
     * @brief see #ZFUIOnScreenKeyboardState::keyboardFixClientFrame
     */
    virtual void keyboardFixClientFrame(ZF_IN ZFUIOnScreenKeyboardState *keyboardState,
                                        ZF_IN const ZFUIRect &keyboardFrame,
                                        ZF_OUT ZFUIRect &clientFrame)
    {
        ZFUIViewPositionOnScreen(clientFrame, keyboardState->keyboardOwnerSysWindow()->rootView());
        ZFUIRectApplyScale(clientFrame, clientFrame, keyboardState->keyboardOwnerSysWindow()->rootView()->UIScaleFixed());
        if(ZFUIRectGetBottom(clientFrame) > ZFUIRectGetTop(keyboardFrame))
        {
            clientFrame.height -= ZFUIRectGetBottom(clientFrame) - ZFUIRectGetTop(keyboardFrame);
        }
    }

    // ============================================================
    // callbacks that implementations must notify
public:
    /**
     * @brief implementations must notify when keyboard's showing state or frame changed
     */
    zffinal void notifyKeyboardStateOnChange(ZF_IN ZFUIOnScreenKeyboardState *keyboardState)
    {
        zffloat scale = keyboardState->keyboardOwnerSysWindow()->rootView()->UIScaleFixed();
        zfbool keyboardShowingNew = this->keyboardShowing(keyboardState);
        ZFUIRect keyboardFrameNew = ZFUIRectApplyScaleReversely(this->keyboardFrame(keyboardState), scale);
        if(keyboardShowingNew == keyboardState->_ZFP_ZFUIOnScreenKeyboardState_keyboardShowing
            && keyboardFrameNew == keyboardState->_ZFP_ZFUIOnScreenKeyboardState_keyboardFrame)
        {
            return ;
        }
        keyboardState->_ZFP_ZFUIOnScreenKeyboardState_keyboardFramePrev = keyboardState->_ZFP_ZFUIOnScreenKeyboardState_keyboardFrame;
        keyboardState->_ZFP_ZFUIOnScreenKeyboardState_keyboardFrame = keyboardFrameNew;
        keyboardState->_ZFP_ZFUIOnScreenKeyboardState_keyboardShowing = this->keyboardShowing(keyboardState);
        keyboardState->observerNotify(ZFUIOnScreenKeyboardState::EventKeyboardStateOnChange());
    }
ZFPROTOCOL_INTERFACE_END(ZFUIOnScreenKeyboardState)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFUIOnScreenKeyboardState_h_

