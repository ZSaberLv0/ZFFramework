#include "ZFImpl_sys_Qt_ZFUIKit_impl.h"

#if ZF_ENV_sys_Qt

#include <QGraphicsLayout>
#include <QGraphicsWidget>
#include <QGraphicsProxyWidget>
#include <QLayout>
#include <QWidget>
#include <QLabel>
#include <QTextEdit>

ZF_NAMESPACE_GLOBAL_BEGIN

void _ZFP_ZFImpl_sys_Qt_viewInfoT(ZF_IN_OUT zfstring &s, ZF_IN QObject *view)
{
    // class name
    s += view->metaObject()->className();

    // instance
    zfstringAppend(s, " %p", view);

    // basic widget info
    if(qobject_cast<QWidget *>(view) != NULL)
    {
        QWidget *tmp = qobject_cast<QWidget *>(view);

        const QRect &frame = tmp->geometry();
        zfstringAppend(s, " (%d, %d, %d, %d)"
                , (zfint)zfmRound(frame.x())
                , (zfint)zfmRound(frame.y())
                , (zfint)zfmRound(frame.width())
                , (zfint)zfmRound(frame.height())
                );

        if(tmp->focusPolicy() != Qt::NoFocus)
        {
            zfstringAppend(s, " (focusable:%d)", (int)tmp->focusPolicy());
        }
        if(!tmp->isVisible())
        {
            s += " (hidden)";
        }
        if(!tmp->isEnabled())
        {
            s += " (disabled)";
        }
    }
    else if(qobject_cast<QGraphicsLayoutItem *>(view) != NULL)
    {
        QGraphicsLayoutItem *tmp = qobject_cast<QGraphicsLayoutItem *>(view);

        QRectF frame = tmp->geometry();
        zfstringAppend(s, " (%d, %d, %d, %d)"
                , (zfint)zfmRound(frame.x())
                , (zfint)zfmRound(frame.y())
                , (zfint)zfmRound(frame.width())
                , (zfint)zfmRound(frame.height())
                );

        QGraphicsWidget *tmpWidget = qobject_cast<QGraphicsWidget *>(view);
        if(tmpWidget != NULL && tmpWidget->focusPolicy() != Qt::NoFocus)
        {
            zfstringAppend(s, " (focusable:%d)", (int)tmpWidget->focusPolicy());
        }

        if(tmp->graphicsItem() != NULL)
        {
            if(!tmp->graphicsItem()->isVisible())
            {
                s += " (hidden)";
            }
            if(!tmp->graphicsItem()->isEnabled())
            {
                s += " (all disabled)";
            }
            else if(!tmp->graphicsItem()->acceptTouchEvents())
            {
                s += " (disabled)";
            }
        }
    }

    // text
    {
        QLabel *tmp = qobject_cast<QLabel *>(view);
        if(tmp != NULL)
        {
            zfstringAppend(s, " %s", tmp->text().toStdString().c_str());
        }
    }
    {
        QTextEdit *tmp = qobject_cast<QTextEdit *>(view);
        if(tmp != NULL)
        {
            zfstringAppend(s, " %s", tmp->toPlainText().toStdString().c_str());
        }
    }
}

void ZFImpl_sys_Qt_viewInfoT(ZF_OUT zfstring &ret, ZF_IN QObject *view)
{
    if(view == zfnull)
    {
        ret = ZFTOKEN_zfnull;
    }
    else
    {
        _ZFP_ZFImpl_sys_Qt_viewInfoT(ret, view);
    }
}

static void _ZFP_ZFImpl_sys_Qt_viewTreePrint_recursive(ZF_IN_OUT zfstring &s, QObject *view, zfindex depth, zfindex siblingIndex)
{
    zfstringAppend(s, "|%2d ", siblingIndex);
    for(zfindex i = 0; i < depth; ++i)
    {
        s += "| ";
    }

    _ZFP_ZFImpl_sys_Qt_viewInfoT(s, view);

    s += "\n";

    if(qobject_cast<QWidget *>(view) != NULL)
    {
        QLayout *l = qobject_cast<QWidget *>(view)->layout();
        if(l != zfnull)
        {
            for(int i = 0; i < l->count(); ++i)
            {
                _ZFP_ZFImpl_sys_Qt_viewTreePrint_recursive(s, l->itemAt(i)->widget(), depth + 1, i);
            }
        }
    }
    else if(qobject_cast<QGraphicsWidget *>(view) != NULL)
    {
        QGraphicsLayout *l = qobject_cast<QGraphicsWidget *>(view)->layout();
        if(l != zfnull)
        {
            for(int i = 0; i < l->count(); ++i)
            {
                _ZFP_ZFImpl_sys_Qt_viewTreePrint_recursive(s, l->itemAt(i)->graphicsItem()->toGraphicsObject(), depth + 1, i);
            }
        }
        else if(qobject_cast<QGraphicsProxyWidget *>(view) != NULL)
        {
            QGraphicsProxyWidget *l = qobject_cast<QGraphicsProxyWidget *>(view);
            if(l->widget() != NULL)
            {
                _ZFP_ZFImpl_sys_Qt_viewTreePrint_recursive(s, l->widget(), depth + 1, 0);
            }
        }
    }
}
void ZFImpl_sys_Qt_viewTreePrintT(ZF_OUT zfstring &ret, ZF_IN QObject *view)
{
    ret += "==================== Qt view tree begin ====================\n";
    if(view != zfnull)
    {
        _ZFP_ZFImpl_sys_Qt_viewTreePrint_recursive(ret, view, 0, 0);
    }
    ret += "==================== Qt view tree  end  ====================\n";
}

// convert utility
void ZFImpl_sys_Qt_ZFUIPointToQPointT(ZF_OUT QPoint &ret, ZF_IN const ZFUIPoint &point)
{
    ret = QPoint(point.x, point.y);
}
void ZFImpl_sys_Qt_ZFUIPointFromQPointT(ZF_OUT ZFUIPoint &ret, ZF_IN const QPoint &qPoint)
{
    ret.x = qPoint.x();
    ret.y = qPoint.y();
}

void ZFImpl_sys_Qt_ZFUIPointToQPointFT(ZF_OUT QPointF &ret, ZF_IN const ZFUIPoint &point)
{
    ret = QPointF(point.x, point.y);
}
void ZFImpl_sys_Qt_ZFUIPointFromQPointFT(ZF_OUT ZFUIPoint &ret, ZF_IN const QPointF &qPoint)
{
    ret.x = (zffloat)qPoint.x();
    ret.y = (zffloat)qPoint.y();
}

void ZFImpl_sys_Qt_ZFUISizeToQSizeT(ZF_OUT QSize &ret, ZF_IN const ZFUISize &size)
{
    ret = QSize(size.width, size.height);
}
void ZFImpl_sys_Qt_ZFUISizeFromQSizeT(ZF_OUT ZFUISize &ret, ZF_IN const QSize &qSize)
{
    ret.width = qSize.width();
    ret.height = qSize.height();
}

void ZFImpl_sys_Qt_ZFUISizeToQSizeFT(ZF_OUT QSizeF &ret, ZF_IN const ZFUISize &size)
{
    ret = QSizeF(size.width, size.height);
}
void ZFImpl_sys_Qt_ZFUISizeFromQSizeFT(ZF_OUT ZFUISize &ret, ZF_IN const QSizeF &qSize)
{
    ret.width = (zffloat)qSize.width();
    ret.height = (zffloat)qSize.height();
}

void ZFImpl_sys_Qt_ZFUIRectToQRectT(ZF_OUT QRect &ret, ZF_IN const ZFUIRect &rect)
{
    ret = QRect(rect.x, rect.y, rect.width, rect.height);
}
void ZFImpl_sys_Qt_ZFUIRectFromQRectT(ZF_OUT ZFUIRect &ret, ZF_IN const QRect &qRect)
{
    ret.x = qRect.x();
    ret.y = qRect.y();
    ret.width = qRect.width();
    ret.height = qRect.height();
}

void ZFImpl_sys_Qt_ZFUIRectToQRectFT(ZF_OUT QRectF &ret, ZF_IN const ZFUIRect &rect)
{
    ret = QRectF(rect.x, rect.y, rect.width, rect.height);
}
void ZFImpl_sys_Qt_ZFUIRectFromQRectFT(ZF_OUT ZFUIRect &ret, ZF_IN const QRectF &qRect)
{
    ret.x = (zffloat)qRect.x();
    ret.y = (zffloat)qRect.y();
    ret.width = (zffloat)qRect.width();
    ret.height = (zffloat)qRect.height();
}

void ZFImpl_sys_Qt_ZFUIColorToQColor(ZF_OUT QColor &ret, ZF_IN const ZFUIColor &color)
{
    ret.setRgbF(
            (qreal)ZFUIColorGetR(color),
            (qreal)ZFUIColorGetG(color),
            (qreal)ZFUIColorGetB(color),
            (qreal)ZFUIColorGetA(color)
        );
}
void ZFImpl_sys_Qt_ZFUIColorFromQColorT(ZF_OUT ZFUIColor &ret, ZF_IN const QColor &qColor)
{
    ret = ZFUIColorMake(
            (zffloat)qColor.redF(),
            (zffloat)qColor.greenF(),
            (zffloat)qColor.blueF(),
            (zffloat)qColor.alphaF()
        );
}

void ZFImpl_sys_Qt_ZFUIAlignFlagsToQAlignmentT(ZF_OUT Qt::Alignment &ret, ZF_IN const ZFUIAlignFlags &align)
{
    ret = (Qt::Alignment)0;

    if(ZFBitTest(align, ZFUIAlign::e_LeftInner))
    {
        ZFBitSet(ret, Qt::AlignLeft);
    }
    else if(ZFBitTest(align, ZFUIAlign::e_RightInner))
    {
        ZFBitSet(ret, Qt::AlignRight);
    }
    else
    {
        ZFBitSet(ret, Qt::AlignHCenter);
    }

    if(ZFBitTest(align, ZFUIAlign::e_TopInner))
    {
        ZFBitSet(ret, Qt::AlignTop);
    }
    else if(ZFBitTest(align, ZFUIAlign::e_BottomInner))
    {
        ZFBitSet(ret, Qt::AlignBottom);
    }
    else
    {
        ZFBitSet(ret, Qt::AlignVCenter);
    }
}
void ZFImpl_sys_Qt_ZFUIAlignFlagsFromQAlignmentT(ZF_OUT ZFUIAlignFlags &ret, ZF_IN const Qt::Alignment qAlign)
{
    ret = ZFUIAlign::e_Center;

    if(ZFBitTest(qAlign, Qt::AlignLeft))
    {
        ZFBitSet(ret, ZFUIAlign::e_LeftInner);
    }
    else if(ZFBitTest(qAlign, Qt::AlignRight))
    {
        ZFBitSet(ret, ZFUIAlign::e_RightInner);
    }

    if(ZFBitTest(qAlign, Qt::AlignTop))
    {
        ZFBitSet(ret, ZFUIAlign::e_TopInner);
    }
    else if(ZFBitTest(qAlign, Qt::AlignBottom))
    {
        ZFBitSet(ret, ZFUIAlign::e_BottomInner);
    }
}

ZF_NAMESPACE_GLOBAL_END

#if 0
    #include "ZFUIKit/ZFUISysWindow.h"
    ZF_GLOBAL_INITIALIZER_INIT(ZFImpl_sys_Qt_autoPrintViewTree)
    {
        if(!ZFProtocolIsAvailable("ZFUIView"))
        {
            return ;
        }
        ZFLISTENER_LOCAL(windowOnPause, {
            ZFUISysWindow *sysWindow = listenerData.sender<ZFUISysWindow *>();
            zfstring s;
            ZFImpl_sys_Qt_viewTreePrintT(s, (QGraphicsWidget *)sysWindow->rootView()->nativeView());
            zfLogTrimT() << s;
        })
        this->windowOnPauseListener = windowOnPause;
        ZFGlobalObserver().observerAdd(
            ZFUISysWindow::EventSysWindowOnPause(), this->windowOnPauseListener);
    }
    ZF_GLOBAL_INITIALIZER_DESTROY(ZFImpl_sys_Qt_autoPrintViewTree)
    {
        ZFGlobalObserver().observerRemove(
            ZFUISysWindow::EventSysWindowOnPause(), this->windowOnPauseListener);
    }
    private:
        ZFListener windowOnPauseListener;
    ZF_GLOBAL_INITIALIZER_END(ZFImpl_sys_Qt_autoPrintViewTree)
#endif

#endif // #if ZF_ENV_sys_Qt

