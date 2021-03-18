#include "ZFImpl_sys_Qt_ZF_impl.h"
#include "ZFCore/ZFMainEntry.h"
#include "ZFCore/ZFString.h"
#include "ZFCore/protocol/ZFProtocolZFMainEntry.h"

#include "ZFMainEntry_sys_Qt.h"

#if ZF_ENV_sys_Qt

#ifdef QT_WIDGETS_LIB
#include <QApplication>
#include <QWidget>
#include <QLayout>
#else
#include <QCoreApplication>
#endif

#ifdef QT_WIDGETS_LIB
class _ZFP_ZFMainEntry_sys_Qt_WindowLayout : public QLayout
{
    Q_OBJECT

public:
    QWidget *_ZFP_owner;
    ZFCoreArrayPOD<QLayoutItem *> _ZFP_children;

public:
    ~_ZFP_ZFMainEntry_sys_Qt_WindowLayout(void)
    {
        QLayoutItem *item = zfnull;
        while((item = this->takeAt(0)) != zfnull)
        {
            delete item;
        }
    }

public:
    virtual void addItem(QLayoutItem *layoutItem)
    {
        _ZFP_children.add(layoutItem);
    }
    virtual QSize sizeHint(void) const
    {
        return this->_ZFP_owner->geometry().size();
    }
    virtual QSize minimumSize(void) const
    {
        return QSize(0, 0);
    }
    virtual void setGeometry(const QRect &rect)
    {
        for(zfindex i = 0; i < this->_ZFP_children.count(); ++i)
        {
            this->_ZFP_children[i]->setGeometry(rect);
        }
    }

    virtual QLayoutItem *itemAt(int index) const
    {
        if(index < (int)_ZFP_children.count())
        {
            return _ZFP_children[index];
        }
        return zfnull;
    }
    virtual QLayoutItem *takeAt(int index)
    {
        if(index < (int)_ZFP_children.count())
        {
            QLayoutItem *ret = _ZFP_children[index];
            _ZFP_children.remove(index);
            return ret;
        }
        return zfnull;
    }
    virtual int indexOf(QWidget *widget) const
    {
        for(zfindex i = 0; i < _ZFP_children.count(); ++i)
        {
            if(_ZFP_children[i]->widget() == widget)
            {
                return (int)i;
            }
        }
        return -1;
    }
    virtual int count() const
    {
        return (int)_ZFP_children.count();
    }
};

ZFImpl_sys_Qt_Window::ZFImpl_sys_Qt_Window(void)
: QWidget()
{
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, Qt::white);
    this->setPalette(palette);

    _ZFP_ZFMainEntry_sys_Qt_WindowLayout *layout = new _ZFP_ZFMainEntry_sys_Qt_WindowLayout();
    layout->_ZFP_owner = this;
    this->setLayout(layout);
}
#endif

ZF_NAMESPACE_GLOBAL_BEGIN

int ZFMainEntry_sys_Qt(int argc, char **argv)
{
#ifdef QT_WIDGETS_LIB
    QApplication app(argc, argv);
    ZFImpl_sys_Qt_Window window;
    window.show();

    int ret = ZFMainEntry_sys_Qt_attach(&window, argc, argv);
    if(ret != 0)
    {
        return ret;
    }
    ret = app.exec();
    ZFMainEntry_sys_Qt_detach();

    window.close();
    return ret;
#else
    QCoreApplication app(argc, argv);
    int ret = ZFMainEntry_sys_Qt_attach(NULL, argc, argv);
    if(ret != 0)
    {
        return ret;
    }
    ZFMainEntry_sys_Qt_detach();
    return 0;
#endif
}

static QWidget *_ZFP_ZFImpl_sys_Qt_rootWindowInstance = zfnull;
QWidget *ZFImpl_sys_Qt_rootWindow(void)
{
    return _ZFP_ZFImpl_sys_Qt_rootWindowInstance;
}

int ZFMainEntry_sys_Qt_attach(ZF_IN QWidget *rootWindow,
                              ZF_IN_OPT int argc /* = 0 */,
                              ZF_IN_OPT char **argv /* = NULL */)
{
    _ZFP_ZFImpl_sys_Qt_rootWindowInstance = rootWindow;
    ZFFrameworkInit();

#ifdef QT_WIDGETS_LIB
    zfCoreAssert(rootWindow != NULL && rootWindow->layout() != NULL);
#endif

    ZFCoreArray<zfstring> params;
    for(int i = 0; i < argc; ++i)
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

#include "ZFMainEntry_sys_Qt.moc"
#endif

