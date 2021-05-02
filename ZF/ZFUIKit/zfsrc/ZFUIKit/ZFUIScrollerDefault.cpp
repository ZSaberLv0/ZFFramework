#include "ZFUIScrollerDefault.h"

#include <math.h> // for sqrt

ZF_NAMESPACE_GLOBAL_BEGIN

#define _ZFP_ZFUIScrollerDefault_scrollAniBounceMax 40
#define _ZFP_ZFUIScrollerDefault_scrollAniBounceDragMax 800
#define _ZFP_ZFUIScrollerDefault_scrollAniStartTolerance 500

#define _ZFP_ZFUIScrollerDefault_aniByPointDurationMin ((zftimet)250)
#define _ZFP_ZFUIScrollerDefault_aniByPointDurationMax ((zftimet)1500)

#define _ZFP_ZFUIScrollerDefault_aniBySpeedSpeedMaxNormal 3000 // pixels per second
#define _ZFP_ZFUIScrollerDefault_aniBySpeedSpeedMaxBounce 500 // pixels per second
#define _ZFP_ZFUIScrollerDefault_aniBySpeedGravityNormal 500 // reduce how much speed per second
#define _ZFP_ZFUIScrollerDefault_aniBySpeedGravityBounce 2000 // reduce how much speed per second

// ============================================================
typedef enum {
    _ZFP_ZFUIScrollerDefaultPrivateAniStateIdle,
    _ZFP_ZFUIScrollerDefaultPrivateAniStateByPoint,
    _ZFP_ZFUIScrollerDefaultPrivateAniStateBySpeed,
} _ZFP_ZFUIScrollerDefaultPrivateAniState;
zfclassNotPOD _ZFP_ZFUIScrollerDefaultPrivate
{
public:
    _ZFP_ZFUIScrollerDefaultPrivate(void)
    : pimplOwner(zfnull)
    , contentOffset(0)
    , contentSize(0)
    , ownerSize(0)
    , scrollBounce(zftrue)
    , scrollBounceAlways(zffalse)
    , scrollAlignToPage(zffalse)
    , scrollAlignToPageOverrideScrollFlag(zffalse)
    , scrollRequireFocus(zffalse)
    , contentBounceTailFix(0)
    , scrollDragPrevPrevPos(0)
    , scrollDragPrevPrevTime(0)
    , scrollDragPrevPos(0)
    , scrollDragPrevTime(0)
    , scrollDragCurPos(0)
    , scrollDragCurTime(0)
    , aniState(_ZFP_ZFUIScrollerDefaultPrivateAniStateIdle)
    , aniLastTime(0)
    , aniByPointStartPos(0)
    , aniByPointStartTime(0)
    , aniByPointStopPos(0)
    , aniByPointStopTime(0)
    , aniBySpeedCurSpeed(0)
    , aniBySpeedLastTime(0)
    {
    }

public:
    ZFUIScrollerDefault *pimplOwner;

    zfint contentOffset;
    zfint contentSize;
    zfint ownerSize;
    zfbool scrollBounce;
    zfbool scrollBounceAlways;

    zfbool scrollAlignToPage;
    zfbool scrollAlignToPageOverrideScrollFlag;

    zfbool scrollRequireFocus;

    // contentOffset + fix should be 0 if no bounce at tail,
    // and should be < 0 if bounce,
    // this value would be 0 if (contentSize < ownerSize),
    // and should be (contentSize - ownerSize) if (contentSize > ownerSize)
    zfint contentBounceTailFix;

    zfint scrollDragPrevPrevPos;
    zftimet scrollDragPrevPrevTime;
    zfint scrollDragPrevPos;
    zftimet scrollDragPrevTime;
    zfint scrollDragCurPos;
    zftimet scrollDragCurTime;

    _ZFP_ZFUIScrollerDefaultPrivateAniState aniState;
    zftimet aniLastTime;

    zfint aniByPointStartPos;
    zftimet aniByPointStartTime;
    zfint aniByPointStopPos;
    zftimet aniByPointStopTime;

    // when not bouncing, speed would keep decreased until reach 0 or start bouncing
    // when bouncing out, speed would be decreased at higher speed until reach 0
    // when bouncing back, speed would be increased at higher speed until end bouncing
    zfint aniBySpeedCurSpeed;
    zftimet aniBySpeedLastTime;

public:
    void contentBounceTailFixUpdate(void)
    {
        if(this->contentSize < this->ownerSize)
        {
            this->contentBounceTailFix = 0;
        }
        else
        {
            this->contentBounceTailFix = this->contentSize - this->ownerSize;
        }
    }
    inline zfbool scrollBounceEnable(void)
    {
        return (this->scrollBounceAlways || (this->scrollBounce && this->contentSize > this->ownerSize));
    }
    inline zfint calcBounceFromDrag(ZF_IN zfint dragOffset)
    {
        if(dragOffset >= _ZFP_ZFUIScrollerDefault_scrollAniBounceDragMax)
        {
            return _ZFP_ZFUIScrollerDefault_scrollAniBounceMax;
        }
        else
        {
            zfint ret = (zfint)(ZFBezierEaseOut().y_by_x((zffloat)dragOffset / _ZFP_ZFUIScrollerDefault_scrollAniBounceDragMax) * _ZFP_ZFUIScrollerDefault_scrollAniBounceMax);
            return zfmMin(ret, dragOffset);
        }
    }
    inline zfint calcDragFromBounce(ZF_IN zfint scrollBounce)
    {
        if(scrollBounce >= _ZFP_ZFUIScrollerDefault_scrollAniBounceMax)
        {
            return _ZFP_ZFUIScrollerDefault_scrollAniBounceDragMax;
        }
        else
        {
            zfint ret = (zfint)(ZFBezierEaseOut().x_by_y((zffloat)scrollBounce / _ZFP_ZFUIScrollerDefault_scrollAniBounceMax) * _ZFP_ZFUIScrollerDefault_scrollAniBounceDragMax);
            return zfmMax(ret, scrollBounce);
        }
    }
    inline zfint calcBounceHeadFromContentOffset(ZF_IN zfint contentOffset)
    {
        if(contentOffset > 0)
        {
            return contentOffset;
        }
        else
        {
            return 0;
        }
    }
    inline zfint calcContentOffsetFromBounceHead(ZF_IN zfint scrollBounceHead)
    {
        return scrollBounceHead;
    }
    inline zfint calcBounceTailFromContentOffset(ZF_IN zfint contentOffset)
    {
        if(contentOffset + this->contentBounceTailFix < 0)
        {
            return -(contentOffset + this->contentBounceTailFix);
        }
        else
        {
            return 0;
        }
    }
    inline zfint calcContentOffsetFromBounceTail(ZF_IN zfint scrollBounceTail)
    {
        return (-this->contentBounceTailFix - scrollBounceTail);
    }
    inline zfint calcContentOffsetFromBounce(ZF_IN zfint scrollBounce,
                                             ZF_IN zfbool bounceAtHead)
    {
        return (bounceAtHead
                ? this->calcContentOffsetFromBounceHead(scrollBounce)
                : this->calcContentOffsetFromBounceTail(scrollBounce));
    }
    inline zfint calcBounceFromContentOffset(ZF_IN zfint contentOffset,
                                             ZF_IN zfbool bounceAtHead)
    {
        return (bounceAtHead
                ? this->calcBounceHeadFromContentOffset(contentOffset)
                : this->calcBounceTailFromContentOffset(contentOffset));
    }

    zfint calcScrollOffset(ZF_IN zfint dragOffset, ZF_IN zftimet timeOffset)
    {
        zfint offset = 0;
        if(timeOffset > 0)
        {
            zfdouble speed = (zfdouble)((zft_zfdouble)dragOffset * 1000 / timeOffset);
            zfdouble speedFix = zfmAbs(speed);
            zfdouble g = (zfdouble)200;
            zfdouble t = (zfdouble)(speedFix / g);
            offset = (zfint)(speedFix * t - (g * t * t) / 2);
            if(speed < 0)
            {
                offset = -offset;
            }
        }
        return offset;
    }

    void scrollAniNotifyStart(void)
    {
        this->aniLastTime = this->pimplOwner->scrollAniStartCallback.execute();
    }
    void scrollAniNotifyStop(void)
    {
        this->scrollRequireFocus = zffalse;

        this->aniState = _ZFP_ZFUIScrollerDefaultPrivateAniStateIdle;
        this->pimplOwner->scrollAniStopCallback.execute();
    }
    void scrollToFitRange(void)
    {
        if(this->calcBounceHeadFromContentOffset(this->contentOffset) > 0
            || this->calcBounceTailFromContentOffset(this->contentOffset) > 0)
        {
            this->aniBySpeedCurSpeed = 0;
            this->aniState = _ZFP_ZFUIScrollerDefaultPrivateAniStateBySpeed;
            this->scrollAniNotifyStart();
            this->aniBySpeedLastTime = this->aniLastTime;
            this->scrollAlignToPageCheckUpdate();
        }
        else
        {
            if(!this->scrollAlignToPageCheckUpdate())
            {
                this->scrollAniNotifyStop();
            }
        }
    }

    // ============================================================
    // aniByPoint logic
    void aniByPointUpdate(ZF_IN zftimet curTime)
    {
        if(curTime >= this->aniByPointStopTime)
        {
            this->contentOffset = aniByPointStopPos;
            this->scrollToFitRange();
            return ;
        }

        zffloat progress = (zffloat)(curTime - this->aniByPointStartTime) / (this->aniByPointStopTime - this->aniByPointStartTime);
        zffloat offsetProgress = ZFBezierEaseOut().y_by_x(progress);

        this->contentOffset = this->aniByPointStartPos + (zfint)zfmRound((this->aniByPointStopPos - this->aniByPointStartPos) * offsetProgress);
        if(zfmAbs(this->contentOffset - this->aniByPointStopPos) <= 1)
        {
            this->contentOffset = aniByPointStopPos;
            this->scrollToFitRange();
            return ;
        }
    }
    zftimet aniByPointDurationForOffset(ZF_IN zfint offset)
    {
        offset = zfmAbs(offset);
        if(offset <= 2)
        {
            return zftimetZero();
        }

        if(offset >= _ZFP_ZFUIScrollerDefault_scrollAniBounceDragMax)
        {
            return _ZFP_ZFUIScrollerDefault_aniByPointDurationMax;
        }
        zftimet ret = (zft_zftimet)(ZFBezierLinear().y_by_x((zft_zffloat)offset / _ZFP_ZFUIScrollerDefault_scrollAniBounceDragMax)
            * _ZFP_ZFUIScrollerDefault_aniByPointDurationMax);
        if(ret < _ZFP_ZFUIScrollerDefault_aniByPointDurationMin)
        {
            return _ZFP_ZFUIScrollerDefault_aniByPointDurationMin;
        }
        else
        {
            return ret;
        }
    }
    void aniByPointStart(ZF_IN zfint stopPos, ZF_IN_OPT zftimet duration = zftimetZero())
    {
        // fix stopPos in bounce range
        if(this->calcBounceHeadFromContentOffset(stopPos) > _ZFP_ZFUIScrollerDefault_scrollAniBounceMax)
        {
            stopPos = this->calcContentOffsetFromBounceHead(_ZFP_ZFUIScrollerDefault_scrollAniBounceMax);
        }
        else if(this->calcBounceTailFromContentOffset(stopPos) > _ZFP_ZFUIScrollerDefault_scrollAniBounceMax)
        {
            stopPos = this->calcContentOffsetFromBounceTail(_ZFP_ZFUIScrollerDefault_scrollAniBounceMax);
        }

        if(duration <= 0)
        {
            duration = this->aniByPointDurationForOffset(stopPos - this->contentOffset);
        }
        if(duration <= 0)
        {
            this->contentOffset = stopPos;
            this->aniByPointStopPos = stopPos;
            this->scrollToFitRange();
            return ;
        }

        this->aniByPointStartPos = this->contentOffset;
        this->aniByPointStopPos = stopPos;
        this->aniState = _ZFP_ZFUIScrollerDefaultPrivateAniStateByPoint;
        this->scrollAniNotifyStart();
        this->aniByPointStartTime = this->aniLastTime;
        this->aniByPointStopTime = this->aniByPointStartTime + duration;
    }
    void aniByPointScrollOwnerSizeChanged(void)
    {
        if(this->calcBounceHeadFromContentOffset(this->aniByPointStartPos) > _ZFP_ZFUIScrollerDefault_scrollAniBounceMax)
        {
            this->aniByPointStartPos = this->calcContentOffsetFromBounceHead(_ZFP_ZFUIScrollerDefault_scrollAniBounceMax);
        }
        else if(this->calcBounceTailFromContentOffset(this->aniByPointStartPos) > _ZFP_ZFUIScrollerDefault_scrollAniBounceMax)
        {
            this->aniByPointStartPos = this->calcContentOffsetFromBounceTail(_ZFP_ZFUIScrollerDefault_scrollAniBounceMax);
        }

        if(this->calcBounceHeadFromContentOffset(this->aniByPointStopPos) > _ZFP_ZFUIScrollerDefault_scrollAniBounceMax)
        {
            this->aniByPointStopPos = this->calcContentOffsetFromBounceHead(_ZFP_ZFUIScrollerDefault_scrollAniBounceMax);
        }
        else if(this->calcBounceTailFromContentOffset(this->aniByPointStopPos) > _ZFP_ZFUIScrollerDefault_scrollAniBounceMax)
        {
            this->aniByPointStopPos = this->calcContentOffsetFromBounceTail(_ZFP_ZFUIScrollerDefault_scrollAniBounceMax);
        }
    }

    // ============================================================
    // aniBySpeed logic
    void aniBySpeedScrollOwnerSizeChanged(void)
    {
        // nothing to do
    }
    void aniBySpeedCalcSpeed(void)
    {
        zfint prevSpeed = 0;
        zfint curSpeed = 0;
        if(this->scrollDragPrevTime > this->scrollDragPrevPrevTime)
        {
            prevSpeed = (zfint)((this->scrollDragPrevPos - this->scrollDragPrevPrevPos) * 1000 / (this->scrollDragPrevTime - this->scrollDragPrevPrevTime));
        }
        if(this->scrollDragCurTime > this->scrollDragPrevTime)
        {
            curSpeed = (zfint)((this->scrollDragCurPos - this->scrollDragPrevPos) * 1000 / (this->scrollDragCurTime - this->scrollDragPrevTime));
        }
        if((prevSpeed > 0 && curSpeed > 0) || (prevSpeed < 0 && curSpeed < 0))
        {
            this->aniBySpeedCurSpeed = (prevSpeed + curSpeed) / 2;
        }
        else
        {
            this->aniBySpeedCurSpeed = curSpeed / 2;
        }
        if(zfmAbs(this->aniBySpeedCurSpeed) > _ZFP_ZFUIScrollerDefault_aniBySpeedSpeedMaxNormal)
        {
            if(this->aniBySpeedCurSpeed > 0)
            {
                this->aniBySpeedCurSpeed = _ZFP_ZFUIScrollerDefault_aniBySpeedSpeedMaxNormal;
            }
            else
            {
                this->aniBySpeedCurSpeed = -_ZFP_ZFUIScrollerDefault_aniBySpeedSpeedMaxNormal;
            }
        }
    }
    void aniBySpeedStart(void)
    {
        if(this->aniBySpeedShouldStop())
        {
            this->scrollToFitRange();
            return ;
        }
        this->aniState = _ZFP_ZFUIScrollerDefaultPrivateAniStateBySpeed;
        this->scrollAniNotifyStart();
        this->aniBySpeedLastTime = this->aniLastTime;
    }
    void aniBySpeedUpdate(ZF_IN zftimet curTime)
    {
        zfint v = zfmAbs(this->aniBySpeedCurSpeed);
        zftimet t = (curTime - this->aniBySpeedLastTime);

        zfbool bounceAtHead = (this->calcBounceHeadFromContentOffset(this->contentOffset) > 0);
        zfbool bounceAtTail = (this->calcBounceTailFromContentOffset(this->contentOffset) > 0);
        if(bounceAtHead || bounceAtTail)
        {
            zfbool bounceAway = ((bounceAtHead && this->aniBySpeedCurSpeed > 0) || (bounceAtTail && this->aniBySpeedCurSpeed < 0));
            zfint a = (bounceAway ? -_ZFP_ZFUIScrollerDefault_aniBySpeedSpeedMaxNormal : _ZFP_ZFUIScrollerDefault_aniBySpeedSpeedMaxNormal);
            zfint bounceOffset = this->calcBounceFromContentOffset(this->contentOffset, bounceAtHead);
            if(bounceAway)
            {
                v = zfmMin(v, _ZFP_ZFUIScrollerDefault_aniBySpeedSpeedMaxBounce);
                zftimet tDelta = (zftimet)((long)-v * 1000 / a);
                zfbool reachEnd = (t - 10 >= tDelta);
                this->contentOffset = this->calcContentOffsetFromBounce(
                    bounceOffset + this->aniBySpeed_calcOffset(v, reachEnd ? tDelta : t, a),
                    bounceAtHead);
                if(reachEnd)
                {
                    // next time, we'll bounce back
                    this->aniBySpeedCurSpeed = 0;
                }
                else
                {
                    // reduce speed
                    this->aniBySpeedCurSpeed = (zfint)(v + a * t / 1000);
                    if(bounceAtTail)
                    {
                        this->aniBySpeedCurSpeed = -this->aniBySpeedCurSpeed;
                    }
                }
            }
            else
            { // bounce back
                zfint offset = this->aniBySpeed_calcOffset(v, t, a);
                if(offset >= bounceOffset - offset / 4)
                {
                    this->contentOffset = this->calcContentOffsetFromBounce(0, bounceAtHead);
                    this->scrollAniNotifyStop();
                }
                else
                {
                    this->contentOffset = this->calcContentOffsetFromBounce(
                        bounceOffset - offset,
                        bounceAtHead);
                    this->aniBySpeedCurSpeed = (zfint)(v + a * t / 1000);
                    if(bounceAtHead)
                    {
                        this->aniBySpeedCurSpeed = -this->aniBySpeedCurSpeed;
                    }
                }
            }
            this->aniBySpeedLastTime = curTime;
            return ;
        }

        // no bounce
        zfint a = -_ZFP_ZFUIScrollerDefault_aniBySpeedGravityNormal;
        zfint offset = this->aniBySpeed_calcOffset(v, t, a);
        zfbool scrollToHead = (this->aniBySpeedCurSpeed > 0);
        if(this->calcBounceFromContentOffset(
            scrollToHead ? this->contentOffset + offset : this->contentOffset - offset,
            scrollToHead) > 0)
        { // begin to bounce
            zfint offsetDelta = (scrollToHead
                                 ? -this->contentOffset
                                 : this->contentOffset + this->contentBounceTailFix - this->ownerSize);
            zftimet tDelta = this->aniBySpeed_calcTime(v, offsetDelta, a);
            this->contentOffset = this->calcContentOffsetFromBounce(1, scrollToHead);
            this->aniBySpeedLastTime += tDelta;
            this->aniBySpeedCurSpeed = (zfint)(v + a * t / 1000);
            if(!scrollToHead)
            {
                this->aniBySpeedCurSpeed = -this->aniBySpeedCurSpeed;
            }
            if(this->aniBySpeedShouldStop())
            {
                this->contentOffset = this->calcContentOffsetFromBounce(0, scrollToHead);
                this->scrollAniNotifyStop();
            }
            else
            {
                this->aniBySpeedUpdate(curTime);
            }
        }
        else
        { // not begin to bounce
            if(scrollToHead)
            {
                this->contentOffset += offset;
            }
            else
            {
                this->contentOffset -= offset;
            }
            this->aniBySpeedLastTime = curTime;
            this->aniBySpeedCurSpeed = (zfint)(v + a * t / 1000);
            if(!scrollToHead)
            {
                this->aniBySpeedCurSpeed = -this->aniBySpeedCurSpeed;
            }
            if(this->aniBySpeedShouldStop())
            {
                this->scrollAniNotifyStop();
            }
        }
    }
    zfint anibySpeedEndPointPredicted(void)
    {
        zftimet t = (zftimet)((long)zfmAbs(this->aniBySpeedCurSpeed) * 1000 / _ZFP_ZFUIScrollerDefault_aniBySpeedGravityNormal);
        zfint offset = this->aniBySpeed_calcOffset(zfmAbs(this->aniBySpeedCurSpeed), t, -_ZFP_ZFUIScrollerDefault_aniBySpeedGravityNormal);
        if(this->aniBySpeedCurSpeed > 0)
        {
            return this->contentOffset + offset;
        }
        else
        {
            return this->contentOffset - offset;
        }
    }

private:
    zfbool aniBySpeedShouldStop(void)
    {
        return (zfmAbs(this->aniBySpeedCurSpeed) <= _ZFP_ZFUIScrollerDefault_aniBySpeedGravityNormal * 20 / 1000);
    }
    zfint aniBySpeed_calcOffset(ZF_IN zfint v, ZF_IN zftimet t, ZF_IN zfint a)
    {
        return (zfint)((long)v * t / 1000 + (long)a * t * t / 1000 / 1000 / 2);
    }
    zftimet aniBySpeed_calcTime(ZF_IN zfint v, ZF_IN zfint offset, ZF_IN zfint a)
    {
        zftimet t = (zft_zftimet)((-v + sqrt((double)((long)v * v + (long)2 * a * offset))) * 1000 / a);
        return zfmMax((zftimet)0, t);
    }

    // ============================================================
    // align to page logic
public:
    zfbool scrollAlignToPageCheckUpdate(void)
    {
        if(!this->scrollAlignToPage || this->scrollAlignToPageOverrideScrollFlag)
        {
            return zffalse;
        }

        zfint endPos = this->pimplOwner->scrollEndPointPredicted();
        zfint endPosAligned = this->scrollAlignToPageEndPosAligned(endPos);
        if(endPos != endPosAligned)
        {
            this->scrollRequireFocus = zftrue;
            this->scrollAlignToPageOverrideScrollFlag = zftrue;
            this->pimplOwner->scrollByPoint(endPosAligned);
            this->scrollAlignToPageOverrideScrollFlag = zffalse;
            return zftrue;
        }
        return zffalse;
    }
private:
    zfint scrollAlignToPageEndPosAligned(ZF_IN zfint endPos)
    {
        if(this->ownerSize == 0)
        {
            return endPos;
        }
        if(this->calcBounceHeadFromContentOffset(endPos) > 0)
        {
            return this->calcContentOffsetFromBounceHead(0);
        }
        else if(this->calcBounceTailFromContentOffset(endPos) > 0)
        {
            return this->calcContentOffsetFromBounceTail(0);
        }
        else
        {
            zfint tmp = (((zfint)((endPos - this->ownerSize / 2) / this->ownerSize)) * this->ownerSize);
            if(this->contentSize >= this->ownerSize && tmp + this->contentSize < this->ownerSize)
            {
                return this->calcContentOffsetFromBounceTail(0);
            }
            return tmp;
        }
    }
};

// ============================================================
ZFOBJECT_REGISTER(ZFUIScrollerDefault)

void ZFUIScrollerDefault::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFUIScrollerDefaultPrivate);
    d->pimplOwner = this;
}
void ZFUIScrollerDefault::objectOnDealloc(void)
{
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}

void ZFUIScrollerDefault::scrollOwnerSizeChanged(ZF_IN zfint ownerSize)
{
    d->ownerSize = ownerSize;
    d->contentBounceTailFixUpdate();

    if(d->aniState == _ZFP_ZFUIScrollerDefaultPrivateAniStateByPoint)
    {
        d->aniByPointScrollOwnerSizeChanged();
    }
    else if(d->aniState == _ZFP_ZFUIScrollerDefaultPrivateAniStateBySpeed)
    {
        d->aniBySpeedScrollOwnerSizeChanged();
    }
    else
    {
        if(d->calcBounceHeadFromContentOffset(d->contentOffset) > 0)
        {
            d->contentOffset = d->calcContentOffsetFromBounceHead(0);
        }
        else if(d->calcBounceTailFromContentOffset(d->contentOffset) > 0)
        {
            d->contentOffset = d->calcContentOffsetFromBounceTail(0);
        }
    }
    d->scrollAlignToPageCheckUpdate();
}
void ZFUIScrollerDefault::scrollBounceChanged(ZF_IN zfbool scrollBounce,
                                              ZF_IN zfbool scrollBounceAlways)
{
    d->scrollBounce = scrollBounce;
    d->scrollBounceAlways = scrollBounceAlways;
}

void ZFUIScrollerDefault::scrollAlignToPageChanged(ZF_IN zfbool scrollAlignToPage)
{
    d->scrollAlignToPage = scrollAlignToPage;
    d->scrollAlignToPageCheckUpdate();
}

void ZFUIScrollerDefault::scrollContentChanged(ZF_IN zfint contentOffset, ZF_IN zfint contentSize)
{
    d->contentOffset = contentOffset;
    d->contentSize = contentSize;
    d->contentBounceTailFixUpdate();

    // scrollBySpeed have no need to update, simply keep current speed
    // scrollByPoint need update src and dst position
    if(d->aniState == _ZFP_ZFUIScrollerDefaultPrivateAniStateByPoint)
    {
        this->scrollByPoint(d->aniByPointStopPos);
    }
}

void ZFUIScrollerDefault::scrollToFitRange(void)
{
    d->scrollToFitRange();
}
void ZFUIScrollerDefault::scrollWithoutAnimation(ZF_IN zfint contentOffset)
{
    d->scrollAniNotifyStop();
    d->contentOffset = contentOffset;
}
void ZFUIScrollerDefault::scrollByPoint(ZF_IN zfint point)
{
    d->aniByPointStart(point);
    d->scrollAlignToPageCheckUpdate();
}
zfint ZFUIScrollerDefault::scrollByPointEndPoint(void)
{
    return (d->aniState == _ZFP_ZFUIScrollerDefaultPrivateAniStateByPoint ? d->aniByPointStopPos : d->contentOffset);
}

zfint ZFUIScrollerDefault::scrollEndPointPredicted(void)
{
    switch(d->aniState)
    {
        case _ZFP_ZFUIScrollerDefaultPrivateAniStateIdle:
            return d->contentOffset;
        case _ZFP_ZFUIScrollerDefaultPrivateAniStateByPoint:
            return d->aniByPointStopPos;
        case _ZFP_ZFUIScrollerDefaultPrivateAniStateBySpeed:
            return d->anibySpeedEndPointPredicted();
        default:
            zfCoreCriticalShouldNotGoHere();
            return 0;
    }
}

void ZFUIScrollerDefault::scrollBySpeed(ZF_IN zfint speed)
{
    d->aniBySpeedCurSpeed = speed;
    d->aniBySpeedStart();
    d->scrollAlignToPageCheckUpdate();
}
zfint ZFUIScrollerDefault::scrollBySpeedCurrentSpeed(void)
{
    return (d->aniState == _ZFP_ZFUIScrollerDefaultPrivateAniStateBySpeed ? d->aniBySpeedCurSpeed : 0);
}
zfint ZFUIScrollerDefault::scrollBySpeedEndPointPredicted(void)
{
    return (d->aniState == _ZFP_ZFUIScrollerDefaultPrivateAniStateBySpeed
        ? d->anibySpeedEndPointPredicted()
        : 0);
}

zfbool ZFUIScrollerDefault::scrollRequireFocus(void)
{
    return d->scrollRequireFocus;
}

zfint ZFUIScrollerDefault::scrollOwnerSize(void)
{
    return d->ownerSize;
}
zfint ZFUIScrollerDefault::scrollContentOffset(void)
{
    return d->contentOffset;
}
zfint ZFUIScrollerDefault::scrollContentSize(void)
{
    return d->contentSize;
}

void ZFUIScrollerDefault::scrollOnDragBegin(ZF_IN zfint mousePos,
                                            ZF_IN zftimet mouseTime)
{
    d->scrollAniNotifyStop();

    d->scrollDragPrevPrevPos = mousePos;
    d->scrollDragPrevPrevTime = mouseTime;
    d->scrollDragPrevPos = mousePos;
    d->scrollDragPrevTime = mouseTime;
    d->scrollDragCurPos = mousePos;
    d->scrollDragCurTime = mouseTime;

    d->aniByPointStopPos = mousePos;
}
void ZFUIScrollerDefault::scrollOnDrag(ZF_IN zfint mousePos,
                                       ZF_IN zftimet mouseTime)
{
    d->scrollDragPrevPrevPos = d->scrollDragPrevPos;
    d->scrollDragPrevPrevTime = d->scrollDragPrevTime;
    d->scrollDragPrevPos = d->scrollDragCurPos;
    d->scrollDragPrevTime = d->scrollDragCurTime;
    d->scrollDragCurPos = mousePos;
    d->scrollDragCurTime = mouseTime;

    d->aniByPointStopPos = mousePos;

    zfint dragOffset = d->scrollDragCurPos - d->scrollDragPrevPos;
    zfint offset = 0;
    if(d->calcBounceHeadFromContentOffset(d->contentOffset) > 0)
    {
        offset = d->calcContentOffsetFromBounceHead(d->calcDragFromBounce(d->calcBounceHeadFromContentOffset(d->contentOffset)));
    }
    else if(d->calcBounceTailFromContentOffset(d->contentOffset) > 0)
    {
        offset = d->calcContentOffsetFromBounceTail(d->calcDragFromBounce(d->calcBounceTailFromContentOffset(d->contentOffset)));
    }
    else
    {
        offset = d->contentOffset;
    }
    offset += dragOffset;
    if(d->calcBounceHeadFromContentOffset(offset) > 0)
    {
        if(!d->scrollBounceEnable())
        {
            offset = d->calcContentOffsetFromBounceHead(0);
        }
        d->contentOffset = d->calcContentOffsetFromBounceHead(d->calcBounceFromDrag(d->calcBounceHeadFromContentOffset(offset)));
    }
    else if(d->calcBounceTailFromContentOffset(offset) > 0)
    {
        if(!d->scrollBounceEnable())
        {
            offset = d->calcContentOffsetFromBounceTail(0);
        }
        d->contentOffset = d->calcContentOffsetFromBounceTail(d->calcBounceFromDrag(d->calcBounceTailFromContentOffset(offset)));
    }
    else
    {
        d->contentOffset = offset;
    }
}
void ZFUIScrollerDefault::scrollOnDragEnd(ZF_IN zftimet mouseTime,
                                          ZF_IN zfbool needScrollAni)
{
    if(!needScrollAni)
    {
        d->scrollToFitRange();
        return ;
    }

    if(d->scrollDragCurTime - d->scrollDragPrevTime <= 5
        || mouseTime - d->scrollDragPrevTime >= _ZFP_ZFUIScrollerDefault_scrollAniStartTolerance)
    { // no need animation
        d->scrollToFitRange();
        return ;
    }

    d->aniBySpeedCalcSpeed();
    d->aniBySpeedStart();
    d->scrollAlignToPageCheckUpdate();
}

void ZFUIScrollerDefault::scrollAniOnUpdate(ZF_IN zftimet time)
{
    d->aniLastTime = time;
    switch(d->aniState)
    {
        case _ZFP_ZFUIScrollerDefaultPrivateAniStateByPoint:
            d->aniByPointUpdate(time);
            break;
        case _ZFP_ZFUIScrollerDefaultPrivateAniStateBySpeed:
            d->aniBySpeedUpdate(time);
            break;
        case _ZFP_ZFUIScrollerDefaultPrivateAniStateIdle:
            zfCoreCriticalShouldNotGoHere();
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            break;
    }
}

ZF_NAMESPACE_GLOBAL_END

