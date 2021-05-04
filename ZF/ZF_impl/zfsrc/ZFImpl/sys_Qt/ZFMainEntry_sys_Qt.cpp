#include "ZFImpl_sys_Qt_ZF_impl.h"
#include "ZFCore/ZFMainEntry.h"
#include "ZFCore/ZFString.h"
#include "ZFCore/protocol/ZFProtocolZFMainEntry.h"

#include "ZFMainEntry_sys_Qt.h"

#if ZF_ENV_sys_Qt

#include <QApplication>
#include <QGraphicsProxyWidget>

// ============================================================
ZFImpl_sys_Qt_BaseView::ZFImpl_sys_Qt_BaseView(void)
: _forceGeometry(QRectF())
, _forceGeometryFlag(0)
{
}
void ZFImpl_sys_Qt_BaseView::ForceGeometry(QGraphicsWidget *item, const QRectF &rect)
{
    ZFImpl_sys_Qt_BaseView *tmp = qobject_cast<ZFImpl_sys_Qt_BaseView *>(item);
    if(tmp != NULL)
    {
        tmp->forceGeometry(rect);
    }
    else
    {
        QGraphicsProxyWidget *tmp2 = qobject_cast<QGraphicsProxyWidget *>(item);
        if(tmp2 != NULL && tmp2->widget() != NULL)
        {
            tmp2->widget()->setMinimumSize((int)rect.width(), (int)rect.height());
            tmp2->widget()->setMaximumSize((int)rect.width(), (int)rect.height());
            tmp2->widget()->setGeometry((int)rect.x(), (int)rect.y(), (int)rect.width(), (int)rect.height());
        }
        else
        {
            if(item->layout() != NULL)
            {
                item->layout()->setMinimumSize(rect.size());
                item->layout()->setMaximumSize(rect.size());
            }
        }
        item->setMinimumSize(rect.size());
        item->setMaximumSize(rect.size());
        item->setGeometry(rect);
    }
}
void ZFImpl_sys_Qt_BaseView::forceGeometry(const QRectF &rect)
{
    _forceGeometry = rect;
    ++_forceGeometryFlag;
    ZFImpl_sys_Qt_BaseLayout *l = (ZFImpl_sys_Qt_BaseLayout *)this->layout();
    if(l != NULL)
    {
        ++(l->_forceGeometryFlag);
        l->forceGeometry(_forceGeometry);
    }
    this->setMinimumSize(_forceGeometry.size());
    this->setMaximumSize(_forceGeometry.size());
    this->setGeometry(_forceGeometry);
    if(l != NULL)
    {
        --(l->_forceGeometryFlag);
    }
    --_forceGeometryFlag;
}
QSizeF ZFImpl_sys_Qt_BaseView::sizeHint(Qt::SizeHint which, const QSizeF &constraint /* = QSizeF() */) const
{
    return _forceGeometry.size();
}
void ZFImpl_sys_Qt_BaseView::updateGeometry(void)
{
    if(_forceGeometryFlag == 0)
    {
        QGraphicsWidget::updateGeometry();
    }
}

// ============================================================
void ZFImpl_sys_Qt_BaseLayout::setGeometry(const QRectF &rect)
{
    QGraphicsLayout::setGeometry(_forceGeometry);
    if(_forceGeometryFlag == 0)
    {
        this->onLayout(_forceGeometry);
    }
}
void ZFImpl_sys_Qt_BaseLayout::forceGeometry(const QRectF &rect)
{
    _forceGeometry = rect;
    ++_forceGeometryFlag;
    this->setMinimumSize(_forceGeometry.size());
    this->setMaximumSize(_forceGeometry.size());
    this->setGeometry(_forceGeometry);
    --_forceGeometryFlag;
}

zfindex ZFImpl_sys_Qt_BaseLayout::childCount(void) const
{
    return this->children.count();
}
QGraphicsWidget *ZFImpl_sys_Qt_BaseLayout::childAtIndex(ZF_IN zfindex index) const
{
    return this->children[index];
}
void ZFImpl_sys_Qt_BaseLayout::childAdd(ZF_IN QGraphicsWidget *item,
                                        ZF_IN_OPT zfindex index /* = zfindexMax() */)
{
    if(index >= this->children.count())
    {
        this->addChildLayoutItem(item);
        this->children.add(item);
    }
    else
    {
        this->addChildLayoutItem(item);
        item->stackBefore(this->children[index]);
        this->children.add(index, item);
    }
    this->invalidate();
}
void ZFImpl_sys_Qt_BaseLayout::childRemove(ZF_IN QGraphicsWidget *item)
{
    zfindex index = this->children.find(item);
    if(index != zfindexMax())
    {
        this->childRemoveAtIndex(index);
    }
}
void ZFImpl_sys_Qt_BaseLayout::childRemoveAtIndex(ZF_IN zfindex index)
{
    QGraphicsWidget *item = this->children.removeAndGet(index);
    if(item->scene() != NULL)
    {
        item->scene()->removeItem(item);
    }
    item->setParentLayoutItem(NULL);
    item->setParentItem(NULL);
    item->setParent(NULL);
    this->invalidate();
}
void ZFImpl_sys_Qt_BaseLayout::childRemoveAll(void)
{
    for(zfindex i = 0; i < this->children.count(); ++i)
    {
        QGraphicsWidget *item = this->children[i];
        if(item->scene() != NULL)
        {
            item->scene()->removeItem(item);
        }
        item->setParentLayoutItem(NULL);
        item->setParentItem(NULL);
        item->setParent(NULL);
    }
    this->children.removeAll();
    this->invalidate();
}

ZFImpl_sys_Qt_BaseLayout::ZFImpl_sys_Qt_BaseLayout(void)
: children()
, _forceGeometry(QRectF())
, _forceGeometryFlag(0)
{
}
int ZFImpl_sys_Qt_BaseLayout::count() const
{
    return (int)this->childCount();
}
QGraphicsLayoutItem *ZFImpl_sys_Qt_BaseLayout::itemAt(int i) const
{
    return this->childAtIndex((zfindex)i);
}
void ZFImpl_sys_Qt_BaseLayout::removeAt(int index)
{
    this->childRemoveAtIndex((zfindex)index);
}
QSizeF ZFImpl_sys_Qt_BaseLayout::sizeHint(Qt::SizeHint which, const QSizeF &constraint /* = QSizeF() */) const
{
    return _forceGeometry.size();
}
void ZFImpl_sys_Qt_BaseLayout::updateGeometry(void)
{
    if(_forceGeometryFlag == 0)
    {
        QGraphicsLayout::updateGeometry();
    }
}

// ============================================================
ZFImpl_sys_Qt_Window::ZFImpl_sys_Qt_Window(void)
: ZFImpl_sys_Qt_BaseView()
{
    this->setAutoFillBackground(true);
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, Qt::white);
    this->setPalette(palette);

    this->setLayout(new ZFImpl_sys_Qt_WindowLayout());
}

void ZFImpl_sys_Qt_WindowLayout::onLayout(const QRectF &rect)
{
    QRectF bounds(QPointF(), rect.size());
    for(zfindex i = 0; i < this->children.count(); ++i)
    {
        ZFImpl_sys_Qt_BaseView::ForceGeometry(this->children[i], bounds);
    }
}

ZF_NAMESPACE_GLOBAL_BEGIN

int ZFMainEntry_sys_Qt(int argc, char **argv)
{
    QApplication app(argc, argv);
    QGraphicsView container;
    QGraphicsScene scene;
    container.setScene(&scene);
    container.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    container.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ZFImpl_sys_Qt_Window window;
    scene.addItem(&window);
    container.show();

    int ret = ZFMainEntry_sys_Qt_attach(&window, argc, argv);
    if(ret != 0)
    {
        return ret;
    }
    ret = app.exec();
    ZFMainEntry_sys_Qt_detach();
    return ret;
}

static QGraphicsWidget *_ZFP_ZFImpl_sys_Qt_rootWindowInstance = zfnull;
QGraphicsWidget *ZFImpl_sys_Qt_rootWindow(void)
{
    return _ZFP_ZFImpl_sys_Qt_rootWindowInstance;
}

int ZFMainEntry_sys_Qt_attach(ZF_IN QGraphicsWidget *rootWindow,
                              ZF_IN_OPT int argc /* = 0 */,
                              ZF_IN_OPT char **argv /* = NULL */)
{
    _ZFP_ZFImpl_sys_Qt_rootWindowInstance = rootWindow;
    ZFFrameworkInit();

    zfCoreAssert(rootWindow != NULL && rootWindow->layout() != NULL);

    ZFCoreArray<zfstring> params;
    for(int i = 1; i < argc; ++i)
    {
        params.add(argv[i]);
    }
    int ret = ZFMainExecute(params);
    if(ret != 0)
    {
        ZFMainEntry_sys_Qt_detach();
    }
    return ret;
}
void ZFMainEntry_sys_Qt_detach(void)
{
    ZFFrameworkCleanup();
    _ZFP_ZFImpl_sys_Qt_rootWindowInstance = zfnull;
}

ZF_NAMESPACE_GLOBAL_END

#endif

