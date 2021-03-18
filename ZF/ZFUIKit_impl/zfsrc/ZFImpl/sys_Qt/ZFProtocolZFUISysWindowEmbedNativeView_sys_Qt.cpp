#include "ZFImpl_sys_Qt_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUISysWindowEmbedNativeView.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIView.h"

#if ZF_ENV_sys_Qt

#include <QWidget>
#include <QVBoxLayout>

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUISysWindowEmbedNativeViewImpl_sys_Qt, ZFUISysWindowEmbedNativeView, ZFProtocolLevel::e_SystemHigh)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIView, "Qt:QWidget")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()

public:
    virtual void nativeViewAdd(ZF_IN void *parent, ZF_IN void *child)
    {
        QWidget *nativeParent = (QWidget *)parent;
        QWidget *nativeChild = (QWidget *)child;
        if(nativeParent->layout() != NULL)
        {
            nativeParent->layout()->addWidget(nativeChild);
        }
        else
        {
            QVBoxLayout *layout = new QVBoxLayout();
            layout->setSpacing(0);
            layout->setContentsMargins(0, 0, 0, 0);
            nativeParent->setLayout(layout);
            layout->addWidget(nativeChild);
        }
    }
    virtual void nativeViewRemote(ZF_IN void *parent, ZF_IN void *child)
    {
        QWidget *nativeParent = (QWidget *)parent;
        QWidget *nativeChild = (QWidget *)child;
        nativeParent->layout()->removeWidget(nativeChild);
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUISysWindowEmbedNativeViewImpl_sys_Qt)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUISysWindowEmbedNativeViewImpl_sys_Qt)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_Qt

