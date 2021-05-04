#include "ZFImpl_sys_Qt_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUISysWindowEmbedNativeView.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIView.h"

#if ZF_ENV_sys_Qt

#include "ZFImpl/sys_Qt/ZFMainEntry_sys_Qt.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUISysWindowEmbedNativeViewImpl_sys_Qt, ZFUISysWindowEmbedNativeView, ZFProtocolLevel::e_SystemHigh)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIView, "Qt:QGraphicsWidget")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()

public:
    virtual void nativeViewAdd(ZF_IN void *parent, ZF_IN void *child)
    {
        QGraphicsWidget *nativeParent = (QGraphicsWidget *)parent;
        QGraphicsWidget *nativeChild = (QGraphicsWidget *)child;
        if(nativeParent->layout() != NULL)
        {
            ((ZFImpl_sys_Qt_BaseLayout *)nativeParent->layout())->childAdd(nativeChild);
        }
        else
        {
            ZFImpl_sys_Qt_BaseLayout *layout = new ZFImpl_sys_Qt_BaseLayout();
            nativeParent->setLayout(layout);
            layout->childAdd(nativeChild);
        }
    }
    virtual void nativeViewRemove(ZF_IN void *parent, ZF_IN void *child)
    {
        QGraphicsWidget *nativeParent = (QGraphicsWidget *)parent;
        QGraphicsWidget *nativeChild = (QGraphicsWidget *)child;
        ((ZFImpl_sys_Qt_BaseLayout *)nativeParent->layout())->childRemove(nativeChild);
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUISysWindowEmbedNativeViewImpl_sys_Qt)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUISysWindowEmbedNativeViewImpl_sys_Qt)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_Qt

