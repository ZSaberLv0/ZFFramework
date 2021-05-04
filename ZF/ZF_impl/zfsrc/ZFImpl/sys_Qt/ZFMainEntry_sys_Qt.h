#ifndef _ZFI_ZFMainEntry_sys_Qt_h_
#define _ZFI_ZFMainEntry_sys_Qt_h_

#include "ZFCore.h"
#include "ZFImpl_sys_Qt_ZF_impl.h"

#if ZF_ENV_sys_Qt

#include <QGraphicsLayout>
#include <QGraphicsView>
#include <QGraphicsWidget>
#include <QLibrary>

zfclassNotPOD ZF_ENV_EXPORT ZFImpl_sys_Qt_BaseView : public QGraphicsWidget
{
    Q_OBJECT

public:
    static void ForceGeometry(QGraphicsWidget *item, const QRectF &rect);
    void forceGeometry(const QRectF &rect);

protected:
    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;
    virtual void updateGeometry(void);
private:
    QRectF _forceGeometry;
    int _forceGeometryFlag;
public:
    ZFImpl_sys_Qt_BaseView(void);
};

zfclassNotPOD ZF_ENV_EXPORT ZFImpl_sys_Qt_BaseLayout : public QGraphicsLayout
{
protected:
    ZFCoreArrayPOD<QGraphicsWidget *> children;
protected:
    virtual void onLayout(const QRectF &rect) {}
public:
    void forceGeometry(const QRectF &rect);

public:
    virtual zfindex childCount(void) const;
    virtual QGraphicsWidget *childAtIndex(ZF_IN zfindex index) const;
    virtual void childAdd(ZF_IN QGraphicsWidget *item,
                          ZF_IN_OPT zfindex index = zfindexMax());
    virtual void childRemove(ZF_IN QGraphicsWidget *item);
    virtual void childRemoveAtIndex(ZF_IN zfindex index);
    virtual void childRemoveAll(void);

public:
    ZFImpl_sys_Qt_BaseLayout(void);
public:
    virtual int count() const;
    virtual QGraphicsLayoutItem *itemAt(int i) const;
    virtual void removeAt(int index);
    virtual void setGeometry(const QRectF &rect);
protected:
    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;
    virtual void updateGeometry(void);
private:
    QRectF _forceGeometry;
    int _forceGeometryFlag;
    friend zfclassFwd ZFImpl_sys_Qt_BaseView;
};

// ============================================================
zfclassNotPOD ZF_ENV_EXPORT ZFImpl_sys_Qt_Window : public ZFImpl_sys_Qt_BaseView
{
    Q_OBJECT

public:
    ZFImpl_sys_Qt_Window(void);
};
zfclassNotPOD ZF_ENV_EXPORT ZFImpl_sys_Qt_WindowLayout : public ZFImpl_sys_Qt_BaseLayout
{
protected:
    virtual void onLayout(const QRectF &rect);
};

ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief main entry for Qt
 *
 * main entry steps:
 * -# load libraries by ZFImpl_sys_Qt_requireLib
 * -# call ZFMainEntry_sys_Qt and return
 *
 * @code
 *   int main(int argc, char **argv)
 *   {
 *       // load your lib
 *       ZFImpl_sys_Qt_requireLib(YourLibName)
 *
 *       // common entry, setup root window and start ZF
 *       return ZFMainEntry_sys_Qt(argc, argv);
 *   }
 * @endcode
 * \n
 * if you want to attach to your existing UI:
 * @code
 *   int main(int argc, char **argv)
 *   {
 *       // your own app and window
 *       QApplication app(argc, argv);
 *       QGraphicsView container;
 *       QGraphicsScene scene;
 *       container.setScene(&scene);
 *       ZFImpl_sys_Qt_Window window;
 *       scene.addItem(&window);
 *       container.show();
 *       ...
 *
 *       // attach
 *       // note, during attach, a root view would be added to the rootWindow,
 *       // by QGraphicsWidget::layout::addItem,
 *       // you must ensure:
 *       // -  the QGraphicsWidget::layout has been set properly
 *       //   and is type of ZFImpl_sys_Qt_BaseLayout
 *       // -  the root view would be layouted properly (typically to match full size of the window)
 *       int ret = ZFMainEntry_sys_Qt_attach(&window, argc, argv);
 *       if(ret != 0)
 *       {
 *           return ret;
 *       }
 *       // app exe
 *       ret = app.exec();
 *       // detach
 *       ZFMainEntry_sys_Qt_detach();
 *       // return
 *       return ret;
 *   }
 * @endcode
 */
extern ZF_ENV_EXPORT int ZFMainEntry_sys_Qt(int argc, char **argv);
#define ZFImpl_sys_Qt_requireLib(libName) \
    QLibrary _lib##libName(#libName); \
    _lib##libName.load();

extern ZF_ENV_EXPORT QGraphicsWidget *ZFImpl_sys_Qt_rootWindow(void);

extern ZF_ENV_EXPORT int ZFMainEntry_sys_Qt_attach(ZF_IN QGraphicsWidget *rootWindow,
                                                   ZF_IN_OPT int argc = 0,
                                                   ZF_IN_OPT char **argv = NULL);
extern ZF_ENV_EXPORT void ZFMainEntry_sys_Qt_detach(void);

ZF_NAMESPACE_GLOBAL_END
#endif
#endif // #ifndef _ZFI_ZFMainEntry_sys_Qt_h_

