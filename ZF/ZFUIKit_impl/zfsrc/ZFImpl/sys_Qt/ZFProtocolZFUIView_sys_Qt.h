#ifndef _ZFI_ZFProtocolZFUIView_sys_Qt_h_
#define _ZFI_ZFProtocolZFUIView_sys_Qt_h_

#include "ZFImpl_sys_Qt_ZFUIKit_impl.h"

#if ZF_ENV_sys_Qt

#include "ZFImpl/sys_Qt/ZFImpl_sys_Qt_View.h"
#include "ZFImpl/sys_Qt/ZFImpl_sys_Qt_Layout.h"
#include <QGraphicsSceneMouseEvent>

extern zfbool (*ZFUIViewImpl_sys_Qt_isMouseCancel)(ZF_IN QGraphicsSceneMouseEvent *event);
extern QGraphicsSceneMouseEvent *ZFUIViewImpl_sys_Qt_mouseEventClone(ZF_IN QGraphicsSceneMouseEvent *event,
                                                                     ZF_IN QEvent::Type type,
                                                                     ZF_IN QPointF const &localPos);

#endif // #if ZF_ENV_sys_Qt
#endif // #ifndef _ZFI_ZFProtocolZFUIView_sys_Qt_h_

