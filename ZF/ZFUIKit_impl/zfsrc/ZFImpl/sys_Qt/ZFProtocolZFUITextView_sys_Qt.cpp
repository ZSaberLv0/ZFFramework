#include "ZFImpl_sys_Qt_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUITextView.h"

#if ZF_ENV_sys_Qt

#include "ZFImpl/sys_Qt/ZFMainEntry_sys_Qt.h"
#include <QLabel>
#include <QFontMetrics>
#include <QGraphicsDropShadowEffect>
#include <QGraphicsProxyWidget>

class _ZFP_ZFUITextViewImpl_sys_Qt_TextView : public QLabel
{
    Q_OBJECT

public:
    _ZFP_ZFUITextViewImpl_sys_Qt_TextView(void)
    : QLabel()
    {
        this->_ZFP_textColor(ZFUIColorBlack());
        this->_ZFP_textSize(14);
        this->setWordWrap(false);
        this->setGraphicsEffect(zfnull);

        QPalette palette = this->palette();
        palette.setColor(QPalette::Window, Qt::transparent);
        this->setPalette(palette);
    }

public:
    void _ZFP_textSize(ZF_IN zffloat v)
    {
        QFont font = this->font();
        font.setPixelSize((int)v);
        this->setFont(font);
    }
    void _ZFP_textColor(ZF_IN ZFUIColor v)
    {
        QPalette palette = this->palette();
        palette.setColor(QPalette::WindowText, ZFImpl_sys_Qt_ZFUIColorToQColor(v));
        this->setPalette(palette);
    }
    void _ZFP_textShadowUpdate(ZF_IN const ZFUIColor &textShadowColor, ZF_IN const ZFUISize &textShadowOffset)
    {
        if(textShadowColor == ZFUIColorZero())
        {
            this->setGraphicsEffect(zfnull);
        }
        else
        {
            QGraphicsDropShadowEffect *effect = qobject_cast<QGraphicsDropShadowEffect *>(this->graphicsEffect());
            if(effect == zfnull)
            {
                effect = new QGraphicsDropShadowEffect(this);
                this->setGraphicsEffect(effect);
            }
            effect->setBlurRadius(0);
            effect->setColor(ZFImpl_sys_Qt_ZFUIColorToQColor(textShadowColor));
            effect->setOffset(textShadowOffset.width, textShadowOffset.height);
        }
    }
};

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUITextViewImpl_sys_Qt, ZFUITextView, ZFProtocolLevel::e_SystemHigh)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Qt:QGraphicsProxyWidget:QLabel")

public:
    virtual void *nativeTextViewCreate(ZF_IN ZFUITextView *textView)
    {
        QGraphicsProxyWidget *proxy = new QGraphicsProxyWidget();
        proxy->setWidget(new _ZFP_ZFUITextViewImpl_sys_Qt_TextView());
        return proxy;
    }
    virtual void nativeTextViewDestroy(ZF_IN ZFUITextView *textView,
                                       ZF_IN void *nativeTextView)
    {
        QGraphicsProxyWidget *proxy = ZFCastStatic(QGraphicsProxyWidget *, nativeTextView);
        delete proxy;
    }
private:
    _ZFP_ZFUITextViewImpl_sys_Qt_TextView *getNativeImplView(ZF_IN ZFUITextView *textView)
    {
        QGraphicsProxyWidget *proxy = ZFCastStatic(QGraphicsProxyWidget *, textView->nativeImplView());
        return qobject_cast<_ZFP_ZFUITextViewImpl_sys_Qt_TextView *>(proxy->widget());
    }

// ============================================================
// properties
public:
    virtual void text(ZF_IN ZFUITextView *textView,
                      ZF_IN const zfchar *text)
    {
        _ZFP_ZFUITextViewImpl_sys_Qt_TextView *nativeImplView = getNativeImplView(textView);
        nativeImplView->setText(ZFImpl_sys_Qt_zfstringToQString(text));
    }
    virtual void textAppearance(ZF_IN ZFUITextView *textView,
                                ZF_IN ZFUITextAppearanceEnum const &textAppearance)
    {
        _ZFP_ZFUITextViewImpl_sys_Qt_TextView *nativeImplView = getNativeImplView(textView);
        QFont font = nativeImplView->font();
        switch(textAppearance)
        {
            case ZFUITextAppearance::e_Normal:
                font.setBold(false);
                font.setItalic(false);
                break;
            case ZFUITextAppearance::e_Bold:
                font.setBold(true);
                font.setItalic(false);
                break;
            case ZFUITextAppearance::e_Italic:
                font.setBold(false);
                font.setItalic(true);
                break;
            case ZFUITextAppearance::e_BoldItalic:
                font.setBold(true);
                font.setItalic(true);
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                return ;
        }
        nativeImplView->setFont(font);
    }
    virtual void textAlign(ZF_IN ZFUITextView *textView,
                           ZF_IN ZFUIAlignFlags const &textAlign)
    {
        _ZFP_ZFUITextViewImpl_sys_Qt_TextView *nativeImplView = getNativeImplView(textView);
        nativeImplView->setAlignment(ZFImpl_sys_Qt_ZFUIAlignFlagsToQAlignment(textAlign));
    }
    virtual void textColor(ZF_IN ZFUITextView *textView,
                           ZF_IN ZFUIColor const &textColor)
    {
        _ZFP_ZFUITextViewImpl_sys_Qt_TextView *nativeImplView = getNativeImplView(textView);
        nativeImplView->_ZFP_textColor(textColor);
    }
    virtual void textShadowColor(ZF_IN ZFUITextView *textView,
                                 ZF_IN ZFUIColor const &textShadowColor)
    {
        _ZFP_ZFUITextViewImpl_sys_Qt_TextView *nativeImplView = getNativeImplView(textView);
        nativeImplView->_ZFP_textShadowUpdate(textShadowColor, textView->textShadowOffset());
    }
    virtual void textShadowOffset(ZF_IN ZFUITextView *textView,
                                  ZF_IN ZFUISize const &textShadowOffset)
    {
        _ZFP_ZFUITextViewImpl_sys_Qt_TextView *nativeImplView = getNativeImplView(textView);
        nativeImplView->_ZFP_textShadowUpdate(textView->textShadowColor(), textShadowOffset);
    }
    virtual void textSize(ZF_IN ZFUITextView *textView,
                          ZF_IN zffloat textSize)
    {
        // changed during layoutNativeTextView
    }
    virtual void textSizeAutoChangeMinSize(ZF_IN ZFUITextView *textView,
                                           ZF_IN zffloat textSizeAutoChangeMinSize)
    {
        // changed during layoutNativeTextView
    }
    virtual void textSizeAutoChangeMaxSize(ZF_IN ZFUITextView *textView,
                                           ZF_IN zffloat textSizeAutoChangeMaxSize)
    {
        // changed during layoutNativeTextView
    }
    virtual void textSingleLine(ZF_IN ZFUITextView *textView,
                                ZF_IN zfbool textSingleLine)
    {
        _ZFP_ZFUITextViewImpl_sys_Qt_TextView *nativeImplView = getNativeImplView(textView);
        nativeImplView->setWordWrap(!textSingleLine);
    }
    virtual void textTruncateMode(ZF_IN ZFUITextView *textView,
                                  ZF_IN ZFUITextTruncateModeEnum const &textTruncateMode)
    {
#if 0 // not supported for now
        _ZFP_ZFUITextViewImpl_sys_Qt_TextView *nativeImplView = ZFCastStatic(_ZFP_ZFUITextViewImpl_sys_Qt_TextView *, textView->nativeImplView());
        switch(textTruncateMode)
        {
            case ZFUITextTruncateMode::e_Disable:
                break;
            case ZFUITextTruncateMode::e_Head:
                break;
            case ZFUITextTruncateMode::e_Middle:
                break;
            case ZFUITextTruncateMode::e_Tail:
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                return ;
        }
#endif
    }

    // ============================================================
    // layout
public:
    virtual ZFUISize measureNativeTextView(ZF_IN ZFUITextView *textView,
                                           ZF_IN const ZFUISize &sizeHint,
                                           ZF_IN zffloat textSize)
    {
        _ZFP_ZFUITextViewImpl_sys_Qt_TextView *nativeImplView = getNativeImplView(textView);
        QRect geometrySaved = nativeImplView->geometry();
        int textSizeSaved = nativeImplView->font().pixelSize();
        if(textSizeSaved == -1)
        {
            textSizeSaved = QFontMetrics(nativeImplView->font()).height();
        }
        QSize minimumSizeSaved = nativeImplView->minimumSize();
        QSize maximumSizeSaved = nativeImplView->maximumSize();

        nativeImplView->_ZFP_textSize(textSize);
        nativeImplView->setMinimumSize(0, 0);
        nativeImplView->setMaximumSize(
            (sizeHint.width <= 0) ? 30000 : sizeHint.width,
            (sizeHint.height <= 0) ? 30000 : sizeHint.height);
        nativeImplView->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        QSize ret = nativeImplView->minimumSizeHint();

        nativeImplView->_ZFP_textSize(textSizeSaved);
        nativeImplView->setMinimumSize(minimumSizeSaved);
        nativeImplView->setMaximumSize(maximumSizeSaved);
        nativeImplView->setGeometry(geometrySaved);

        return ZFUISizeMake(ret.width(), ret.height());
    }

    virtual zffloat textSizeCurrent(ZF_IN ZFUITextView *textView)
    {
        _ZFP_ZFUITextViewImpl_sys_Qt_TextView *nativeImplView = getNativeImplView(textView);
        return (zffloat)nativeImplView->font().pixelSize();
    }

    virtual void layoutNativeTextView(ZF_IN ZFUITextView *textView,
                                      ZF_IN const ZFUISize &viewSize)
    {
        _ZFP_ZFUITextViewImpl_sys_Qt_TextView *nativeImplView = getNativeImplView(textView);
        zffloat fixedTextSize = this->calcTextSizeAutoChange(textView, viewSize);
        nativeImplView->_ZFP_textSize(fixedTextSize);
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUITextViewImpl_sys_Qt)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUITextViewImpl_sys_Qt)

ZF_NAMESPACE_GLOBAL_END

#include "ZFProtocolZFUITextView_sys_Qt.moc"
#endif // #if ZF_ENV_sys_Qt

