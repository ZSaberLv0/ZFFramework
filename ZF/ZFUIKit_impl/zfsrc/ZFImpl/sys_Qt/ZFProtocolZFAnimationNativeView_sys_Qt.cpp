#include "ZFImpl_sys_Qt_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFAnimationNativeView.h"

#if ZF_ENV_sys_Qt

#include <QAbstractAnimation>
#include <QGraphicsOpacityEffect>
#include <QGraphicsWidget>
#include <QTransform>

class _ZFP_ZFAnimationNativeViewImpl_sys_Qt_Ani : public QAbstractAnimation
{
    Q_OBJECT

public:
    typedef zffloat (*CurveFunc)(ZF_IN zffloat progress);

public:
    ZFAnimationNativeView *ownerZFAnimation;

    // for ani impl
    ZFUIView *aniTarget;
    QGraphicsWidget *nativeAniTarget;
    zffloat nativeAniScale;
    zffloat aniProgress;
    CurveFunc aniCurveFunc;
    zfbool aniRunning;

    // for ani transform
    QGraphicsOpacityEffect *alphaEffect;
    qreal alphaOrig;

public:
    _ZFP_ZFAnimationNativeViewImpl_sys_Qt_Ani(ZF_IN ZFAnimationNativeView *ownerZFAnimation)
    : QAbstractAnimation()
    , ownerZFAnimation(ownerZFAnimation)
    , aniTarget(NULL)
    , nativeAniTarget(NULL)
    , nativeAniScale(1)
    , aniProgress(0)
    , aniCurveFunc(zfnull)
    , aniRunning(zffalse)
    , alphaEffect(NULL)
    , alphaOrig(1)
    {
        this->connect(this, SIGNAL(finished()), this, SLOT(nativeAniOnStop()));
    }
    ~_ZFP_ZFAnimationNativeViewImpl_sys_Qt_Ani(void)
    {
        this->disconnect(this, SIGNAL(finished()), this, SLOT(nativeAniOnStop()));
    }

public slots:
    void nativeAniOnStop(void)
    {
        if(this->ownerZFAnimation->aniRunning())
        {
            ZFPROTOCOL_ACCESS(ZFAnimationNativeView)->notifyAniStop(this->ownerZFAnimation);
        }
    }

public:
    virtual int duration(void) const
    {
        return this->ownerZFAnimation->aniDurationFixed();
    }
protected:
    virtual void updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState)
    {
        QAbstractAnimation::updateState(newState, oldState);
    }
    virtual void updateDirection(QAbstractAnimation::Direction direction)
    {
        QAbstractAnimation::updateDirection(direction);
    }
    virtual void updateCurrentTime(int currentTime)
    {
        if(!this->aniRunning)
        {
            return ;
        }

        if(currentTime >= this->duration())
        {
            this->aniProgress = 1;
            this->aniOnProgress();
            this->nativeAniCleanup();
            this->aniRunning = zffalse;
            this->stop();
        }
        else
        {
            this->aniProgress = this->aniCurveFunc((zffloat)currentTime / this->duration());
            this->aniOnProgress();
        }
    }

public:
    void aniOnProgress(void)
    {
        ZFAnimationNativeView *ani = this->ownerZFAnimation;
        zffloat progress = this->aniProgress;

        if(this->alphaEffect != NULL)
        {
            if(ani->aniAlphaFrom() != 1 || ani->aniAlphaTo() != 1)
            {
                this->alphaEffect->setOpacity((qreal)(
                        zfmApplyProgress(ani->aniAlphaFrom() * this->alphaOrig, ani->aniAlphaTo() * this->alphaOrig, progress)
                    ));
            }
        }

        QTransform transform;
        qreal width = this->nativeAniTarget->geometry().width();
        qreal height = this->nativeAniTarget->geometry().height();

        transform.translate(width / 2, height / 2);

        if(this->aniTarget->viewRotate() != 0)
        {
            transform.rotate(this->aniTarget->viewRotate());
        }
        if(ani->aniRotateZFrom() != 0 || ani->aniRotateZTo() != 0)
        {
            transform.rotate(zfmApplyProgress(ani->aniRotateZFrom(), ani->aniRotateZTo(), progress));
        }

        if(this->aniTarget->viewScaleX() != 1 || this->aniTarget->viewScaleY() != 1)
        {
            transform.scale(this->aniTarget->viewScaleX(), this->aniTarget->viewScaleY());
        }
        if(ani->aniScaleXFrom() != 1 || ani->aniScaleXTo() != 1
            || ani->aniScaleYFrom() != 1 || ani->aniScaleYTo() != 1)
        {
            transform.scale(
                zfmApplyProgress(ani->aniScaleXFrom(), ani->aniScaleXTo(), progress),
                zfmApplyProgress(ani->aniScaleYFrom(), ani->aniScaleYTo(), progress)
                );
        }

        if(ani->aniTranslateXFrom() != 0 || ani->aniTranslateXTo() != 0
            || ani->aniTranslateYFrom() != 0 || ani->aniTranslateYTo() != 0)
        {
            transform.translate(
                zfmApplyProgress(ani->aniTranslateXFrom(), ani->aniTranslateXTo(), progress) * width * this->nativeAniScale,
                zfmApplyProgress(ani->aniTranslateYFrom(), ani->aniTranslateYTo(), progress) * height * this->nativeAniScale
                );
        }
        if(ani->aniTranslatePixelXFrom() != 0 || ani->aniTranslatePixelXTo() != 0
            || ani->aniTranslatePixelYFrom() != 0 || ani->aniTranslatePixelYTo() != 0)
        {
            transform.translate(
                zfmApplyProgress(ani->aniTranslatePixelXFrom(), ani->aniTranslatePixelXTo(), progress) * this->nativeAniScale,
                zfmApplyProgress(ani->aniTranslatePixelYFrom(), ani->aniTranslatePixelYTo(), progress) * this->nativeAniScale
                );
        }

        transform.translate(-width / 2, -height / 2);

        this->nativeAniTarget->setTransform(transform);
    }

public:
    static zffloat _aniCurveFuncLinear(ZF_IN zffloat progress)
    {
        return progress;
    }
    static zffloat _aniCurveFuncEaseIn(ZF_IN zffloat progress)
    {
        return ZFBezierEaseIn().y_by_x(progress);
    }
    static zffloat _aniCurveFuncEaseOut(ZF_IN zffloat progress)
    {
        return ZFBezierEaseOut().y_by_x(progress);
    }
    static zffloat _aniCurveFuncEaseInOut(ZF_IN zffloat progress)
    {
        return ZFBezierEaseInOut().y_by_x(progress);
    }
    void nativeAniStart(void)
    {
        this->aniProgress = 0;
        switch(this->ownerZFAnimation->aniCurve())
        {
            case ZFAnimationNativeViewCurve::e_Linear:
                this->aniCurveFunc = _aniCurveFuncLinear;
                break;
            case ZFAnimationNativeViewCurve::e_EaseIn:
                this->aniCurveFunc = _aniCurveFuncEaseIn;
                break;
            case ZFAnimationNativeViewCurve::e_EaseOut:
                this->aniCurveFunc = _aniCurveFuncEaseOut;
                break;
            case ZFAnimationNativeViewCurve::e_EaseInOut:
                this->aniCurveFunc = _aniCurveFuncEaseInOut;
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                return ;
        }
        this->aniTarget = this->ownerZFAnimation->aniTarget()->to<ZFUIView *>();
        this->nativeAniTarget = (QGraphicsWidget *)this->aniTarget->nativeView();
        this->alphaOrig = 1;
        this->alphaEffect = qobject_cast<QGraphicsOpacityEffect *>(this->nativeAniTarget->graphicsEffect());
        if(this->alphaEffect != NULL)
        {
            this->alphaOrig = this->alphaEffect->opacity();
        }
        else if(this->ownerZFAnimation->aniAlphaFrom() != 1 || this->ownerZFAnimation->aniAlphaTo() != 1)
        {
            this->alphaEffect = new QGraphicsOpacityEffect(this->nativeAniTarget);
            this->nativeAniTarget->setGraphicsEffect(this->alphaEffect);
        }

        this->aniRunning = zftrue;
        this->start();
    }
    void nativeAniStop(void)
    {
        this->nativeAniCleanup();
        this->aniRunning = zffalse;
        this->stop();
    }
private:
    void nativeAniCleanup(void)
    {
        if(this->alphaOrig == 1)
        {
            this->nativeAniTarget->setGraphicsEffect(NULL);
        }
        else
        {
            QGraphicsOpacityEffect *effect = qobject_cast<QGraphicsOpacityEffect *>(this->nativeAniTarget->graphicsEffect());
            if(effect == zfnull)
            {
                effect = new QGraphicsOpacityEffect(this->nativeAniTarget);
                this->nativeAniTarget->setGraphicsEffect(effect);
            }
            effect->setOpacity(this->alphaOrig);
        }

        {
            QTransform t;
            t.translate(this->nativeAniTarget->geometry().width() / 2, this->nativeAniTarget->geometry().height() / 2);
            if(this->aniTarget->viewRotate() != 0)
            {
                t.rotate(this->aniTarget->viewRotate());
            }
            if(this->aniTarget->viewScaleX() != 1 || this->aniTarget->viewScaleY() != 1)
            {
                t.scale(this->aniTarget->viewScaleX(), this->aniTarget->viewScaleY());
            }
            t.translate(-this->nativeAniTarget->geometry().width() / 2, -this->nativeAniTarget->geometry().height() / 2);
            this->nativeAniTarget->setTransform(t);
        }

        this->alphaEffect = NULL;
        this->nativeAniTarget = NULL;
        this->aniCurveFunc = zfnull;
    }
};

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFAnimationNativeViewImpl_sys_Qt, ZFAnimationNativeView, ZFProtocolLevel::e_SystemHigh)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Qt:QAbstractAnimation")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIView, "Qt:QGraphicsWidget")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()
public:
    virtual void *nativeAniCreate(ZF_IN ZFAnimationNativeView *ani)
    {
        return new _ZFP_ZFAnimationNativeViewImpl_sys_Qt_Ani(ani);
    }
    virtual void nativeAniDestroy(ZF_IN ZFAnimationNativeView *ani,
                                  ZF_IN void *nativeAni)
    {
        _ZFP_ZFAnimationNativeViewImpl_sys_Qt_Ani *nativeAniTmp = ZFCastStatic(_ZFP_ZFAnimationNativeViewImpl_sys_Qt_Ani *, nativeAni);
        nativeAniTmp->deleteLater();
    }

    virtual void nativeAniStart(ZF_IN ZFAnimationNativeView *ani,
                                ZF_IN zffloat nativeAniScale)
    {
        _ZFP_ZFAnimationNativeViewImpl_sys_Qt_Ani *nativeAni = ZFCastStatic(_ZFP_ZFAnimationNativeViewImpl_sys_Qt_Ani *, ani->nativeAnimation());
        nativeAni->nativeAniScale = nativeAniScale;
        nativeAni->nativeAniStart();
    }
    virtual void nativeAniStop(ZF_IN ZFAnimationNativeView *ani)
    {
        _ZFP_ZFAnimationNativeViewImpl_sys_Qt_Ani *nativeAni = ZFCastStatic(_ZFP_ZFAnimationNativeViewImpl_sys_Qt_Ani *, ani->nativeAnimation());
        nativeAni->nativeAniStop();
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFAnimationNativeViewImpl_sys_Qt)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFAnimationNativeViewImpl_sys_Qt)

ZF_NAMESPACE_GLOBAL_END

#include "ZFProtocolZFAnimationNativeView_sys_Qt.moc"
#endif // #if ZF_ENV_sys_Qt

