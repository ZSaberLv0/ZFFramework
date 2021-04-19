#include "ZFImpl_sys_Qt_ZFUIKit_impl.h"

#if ZF_ENV_sys_Qt

#include <QLayout>
#include <QLabel>
#include <QTextEdit>

ZF_NAMESPACE_GLOBAL_BEGIN

void _ZFP_ZFImpl_sys_Qt_viewInfoT(ZF_IN_OUT zfstring &s, ZF_IN QWidget *view)
{
    // class name
    s += view->metaObject()->className();

    // instance
    zfstringAppend(s, " %p", view);

    // frame
    {
        const QRect &frame = view->geometry();
        zfstringAppend(s, " (%d, %d, %d, %d)", (zfint)frame.x(), (zfint)frame.y(), (zfint)frame.width(), (zfint)frame.height());
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

void ZFImpl_sys_Qt_viewInfoT(ZF_OUT zfstring &ret, ZF_IN QWidget *view)
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

static void _ZFP_ZFImpl_sys_Qt_viewTreePrint_recursive(ZF_IN_OUT zfstring &s, QWidget *view, zfindex depth, zfindex siblingIndex)
{
    zfstringAppend(s, "|%2d ", siblingIndex);
    for(zfindex i = 0; i < depth; ++i)
    {
        s += "| ";
    }

    _ZFP_ZFImpl_sys_Qt_viewInfoT(s, view);

    s += "\n";

    if(view->layout() != zfnull)
    {
        for(int i = 0; i < view->layout()->count(); ++i)
        {
            _ZFP_ZFImpl_sys_Qt_viewTreePrint_recursive(s, view->layout()->itemAt(i)->widget(), depth + 1, i);
        }
    }
}
void ZFImpl_sys_Qt_viewTreePrintT(ZF_OUT zfstring &ret, ZF_IN QWidget *view)
{
    ret += "==================== QWidget tree begin ====================\n";
    if(view != zfnull)
    {
        _ZFP_ZFImpl_sys_Qt_viewTreePrint_recursive(ret, view, 0, 0);
    }
    ret += "==================== QWidget tree  end  ====================\n";
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
void ZFImpl_sys_Qt_ZFUISizeToQSizeT(ZF_OUT QSize &ret, ZF_IN const ZFUISize &size)
{
    ret = QSize(size.width, size.height);
}
void ZFImpl_sys_Qt_ZFUISizeFromQSizeT(ZF_OUT ZFUISize &ret, ZF_IN const QSize &qSize)
{
    ret.width = qSize.width();
    ret.height = qSize.height();
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
#endif // #if ZF_ENV_sys_Qt

