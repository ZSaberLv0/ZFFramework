#include "ZFImpl_sys_Qt_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIView.h"

#if ZF_ENV_sys_Qt

#include <QWidget>
#include <QLayout>

// ============================================================
// layout
zfclassPOD ZF_ENV_EXPORT ZFImpl_sys_Qt_QLayoutItemHolder
{
public:
    QWidget *widget;
    QLayoutItem *layoutItem;
};
zfclassNotPOD ZF_ENV_EXPORT ZFImpl_sys_Qt_QLayout : public QLayout
{
    Q_OBJECT

public:
    ZFCoreArrayPOD<ZFImpl_sys_Qt_QLayoutItemHolder> layoutItemList;

public:
    ZFImpl_sys_Qt_QLayout(void)
    : layoutItemList()
    {
    }
    ~ZFImpl_sys_Qt_QLayout(void)
    {
        if(!this->layoutItemList.isEmpty())
        {
            zfstring errHint;
            errHint += "[ZFUIView] these views has not been removed when parent dealloc:";
            for(zfindex i = 0; i < this->layoutItemList.count(); ++i)
            {
                QWidget *v = this->layoutItemList.get(i).widget;
                errHint += " ";
                ZFImpl_sys_Qt_viewInfoT(errHint, v);
            }
            zfCoreCriticalMessageTrim("%s", errHint.cString());
        }
    }

public:
    virtual zfindex childCount(void) const
    {
        return this->layoutItemList.count();
    }
    virtual QWidget *childAtIndex(ZF_IN zfindex virtualIndex) const
    {
        return this->layoutItemList[virtualIndex].widget;
    }
    virtual void childAdd(ZF_IN ZFUIView *ownerZFUIViewOrNull,
                          ZF_IN QWidget *widget,
                          ZF_IN zfindex virtualIndex)
    {
        zfindex oldCount = this->layoutItemList.count();
        this->addWidget(widget);
        if(ownerZFUIViewOrNull != zfnull && ownerZFUIViewOrNull->viewVisible() != widget->isVisible())
        {
            widget->setVisible(ownerZFUIViewOrNull->viewVisible());
        }
        widget->update();
        if(virtualIndex < oldCount)
        {
            this->layoutItemList.move(oldCount, virtualIndex);
            for(zfindex i = virtualIndex + 1; i < this->layoutItemList.count(); ++i)
            {
                this->layoutItemList[i].widget->raise();
            }
        }
    }
    virtual void childRemove(ZF_IN zfindex virtualIndex)
    {
        QWidget *t = this->layoutItemList[virtualIndex].widget;
        this->removeWidget(t);
        t->setParent(NULL);
    }
    virtual void childRemoveAll(void)
    {
        while(!this->layoutItemList.isEmpty())
        {
            QWidget *t = this->layoutItemList.getLast().widget;
            this->removeWidget(t);
            t->setParent(NULL);
        }
    }

public:
    virtual void addItem(QLayoutItem *layoutItem)
    {
        ZFImpl_sys_Qt_QLayoutItemHolder item;
        item.widget = layoutItem->widget();
        item.layoutItem = layoutItem;
        this->layoutItemList.add(item);
    }
    virtual QSize sizeHint(void) const
    {
        return QSize(1, 1);
    }
    virtual QSize minimumSize(void) const
    {
        return QSize(0, 0);
    }
    virtual void setGeometry(const QRect &rect)
    {
        QLayout::setGeometry(rect);
    }

    virtual QLayoutItem *itemAt(int index) const
    {
        if(index < (int)this->layoutItemList.count())
        {
            return this->layoutItemList[index].layoutItem;
        }
        return zfnull;
    }
    virtual QLayoutItem *takeAt(int index)
    {
        if(index < (int)this->layoutItemList.count())
        {
            QLayoutItem *ret = this->layoutItemList[index].layoutItem;
            this->layoutItemList.remove(index);
            return ret;
        }
        return zfnull;
    }
    virtual int indexOf(QWidget *widget) const
    {
        for(zfindex i = 0; i < this->layoutItemList.count(); ++i)
        {
            if(this->layoutItemList[i].widget == widget)
            {
                return (int)i;
            }
        }
        return -1;
    }
    virtual int count() const
    {
        return (int)this->layoutItemList.count();
    }
};

#endif // #if ZF_ENV_sys_Qt

