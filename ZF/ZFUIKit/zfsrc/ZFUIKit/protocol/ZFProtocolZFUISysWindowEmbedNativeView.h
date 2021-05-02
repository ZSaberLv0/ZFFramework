/**
 * @file ZFProtocolZFUISysWindowEmbedNativeView.h
 * @brief protocol for ZFUISysWindow
 */

#ifndef _ZFI_ZFProtocolZFUISysWindowEmbedNativeView_h_
#define _ZFI_ZFProtocolZFUISysWindowEmbedNativeView_h_

#include "ZFCore/ZFProtocol.h"
#include "ZFUIKit/ZFUISysWindow.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for ZFUISysWindow
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFUISysWindowEmbedNativeView)
public:
    /**
     * @brief used to attach window's root view to native container
     *
     * the attached child must fill parent,
     * and change frame according to parent's size automatically
     */
    virtual void nativeViewAdd(ZF_IN void *parent, ZF_IN void *child) zfpurevirtual;
    /**
     * @brief remove native view, see #nativeViewAdd
     */
    virtual void nativeViewRemove(ZF_IN void *parent, ZF_IN void *child) zfpurevirtual;
ZFPROTOCOL_INTERFACE_END(ZFUISysWindowEmbedNativeView)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFUISysWindowEmbedNativeView_h_

