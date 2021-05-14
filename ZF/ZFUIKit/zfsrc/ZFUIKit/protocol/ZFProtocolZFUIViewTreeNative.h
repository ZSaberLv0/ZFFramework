/**
 * @file ZFProtocolZFUIViewTreeNative.h
 * @brief protocol for ZFUIView
 */

#ifndef _ZFI_ZFProtocolZFUIViewTreeNative_h_
#define _ZFI_ZFProtocolZFUIViewTreeNative_h_

#include "ZFCore/ZFProtocol.h"
#include "ZFUIKit/ZFUIView.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for ZFUIView
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFUIViewTreeNative)
ZFPROTOCOL_INTERFACE_OPTIONAL
public:
    /**
     * @brief return view tree hint of native view
     */
    virtual void viewTreeNative(ZF_IN ZFUIView *view,
                                ZF_IN_OUT const ZFOutput &outputCallback) zfpurevirtual;
ZFPROTOCOL_INTERFACE_END(ZFUIViewTreeNative)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFUIViewTreeNative_h_

