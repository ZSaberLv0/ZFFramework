#ifndef _ZFI_ZFImpl_sys_Qt_View_h_
#define _ZFI_ZFImpl_sys_Qt_View_h_

#include "ZFImpl_sys_Qt_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIView.h"

#if ZF_ENV_sys_Qt

#include "ZFImpl/sys_Qt/ZFMainEntry_sys_Qt.h"

// ============================================================
// view
zfclassNotPOD ZF_ENV_EXPORT ZFImpl_sys_Qt_View : public ZFImpl_sys_Qt_BaseView
{
    Q_OBJECT
};

#endif // #if ZF_ENV_sys_Qt
#endif // #ifndef _ZFI_ZFImpl_sys_Qt_View_h_

