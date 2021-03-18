/**
 * @file ZFProtocolZFUIViewPositionOnScreen.h
 * @brief protocol for #ZFUIViewPositionOnScreen
 */

#ifndef _ZFI_ZFProtocolZFUIViewPositionOnScreen_h_
#define _ZFI_ZFProtocolZFUIViewPositionOnScreen_h_

#include "ZFCore/ZFProtocol.h"
#include "ZFUIKit/ZFUIView.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for ZFUIViewPositionOnScreen
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFUIViewPositionOnScreen)
public:
    /**
     * @brief see #ZFUIViewPositionOnScreen
     */
    virtual void viewPositionOnScreen(ZF_IN ZFUIView *view,
                                      ZF_OUT ZFUIRect &rect) zfpurevirtual;
ZFPROTOCOL_INTERFACE_END(ZFUIViewPositionOnScreen)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFUIViewPositionOnScreen_h_

