#ifndef _ZFI_ZFImpl_sys_Qt_Layout_h_
#define _ZFI_ZFImpl_sys_Qt_Layout_h_

#include "ZFImpl_sys_Qt_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIView.h"

#if ZF_ENV_sys_Qt

#include "ZFImpl/sys_Qt/ZFMainEntry_sys_Qt.h"

// ============================================================
// layout
zfclassNotPOD ZF_ENV_EXPORT ZFImpl_sys_Qt_Layout : public ZFImpl_sys_Qt_BaseLayout
{
public:
    ~ZFImpl_sys_Qt_Layout(void)
    {
        if(!this->children.isEmpty())
        {
            zfstring errHint;
            errHint += "[ZFUIView] these views has not been removed when parent dealloc:";
            for(zfindex i = 0; i < this->children.count(); ++i)
            {
                QGraphicsWidget *v = this->children.get(i);
                errHint += " ";
                ZFImpl_sys_Qt_viewInfoT(errHint, v);
            }
            zfCoreCriticalMessageTrim("%s", errHint.cString());
        }
    }
};

#endif // #if ZF_ENV_sys_Qt
#endif // #ifndef _ZFI_ZFImpl_sys_Qt_Layout_h_

