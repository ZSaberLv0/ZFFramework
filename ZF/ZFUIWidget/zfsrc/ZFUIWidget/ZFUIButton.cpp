#include "ZFUIButton.h"
#include "ZFCore/ZFSTLWrapper/zfstl_map.h"

ZF_NAMESPACE_GLOBAL_BEGIN

#define _ZFP_ZFUIButton_DEBUG 0
#if _ZFP_ZFUIButton_DEBUG
    #define _ZFP_ZFUIButton_DEBUG_LOG(fmt, ...) \
        zfLogTrimT() << "[ZFUIButton]" << zfstringWithFormat(fmt, ##__VA_ARGS__) << ZFLOG_HEADER_STRING;
    #define _ZFP_ZFUIButton_DEBUG_EVENT(actionName) \
        zfLogTrimT() << "[ZFUIButton]" << ZFM_TOSTRING(actionName) << ZFLOG_HEADER_STRING;
#else
    #define _ZFP_ZFUIButton_DEBUG_LOG(fmt, ...)
    #define _ZFP_ZFUIButton_DEBUG_EVENT(actionName)
#endif

// ============================================================
ZFENUM_DEFINE(ZFUIButtonState)

// ============================================================
ZFSTYLE_DEFAULT_DEFINE(ZFUIButton)

// ============================================================
// _ZFP_ZFUIButtonPrivate
zfclassPOD _ZFP_ZFUIButtonMouseData
{
public:
    zfidentity mouseId;
    ZFUIMouseActionEnum mouseAction;
    ZFUIPoint mousePoint;
};
static inline _ZFP_ZFUIButtonMouseData _ZFP_ZFUIButtonMouseDataFromEvent(ZF_IN ZFUIMouseEvent *event)
{
    _ZFP_ZFUIButtonMouseData ret = {
        event->mouseId,
        event->mouseAction,
        event->mousePoint,
    };
    return ret;
}
#define _ZFP_ZFUIButtonMouseDataToEvent(event, mouseData) \
    zfblockedAllocWithCache(ZFUIMouseEvent, event); \
    event->eventResolved(zffalse); \
    event->mouseId = mouseData.mouseId; \
    event->mouseAction = mouseData.mouseAction; \
    event->mousePoint = mouseData.mousePoint; \
    event->mouseButton = ZFUIMouseButton::e_MouseButtonLeft

zfclassNotPOD _ZFP_ZFUIButtonPrivate
{
public:
    ZFUIButton *pimplOwner;
    ZFUIButtonStateEnum buttonState;
    zfidentity processingMouseId;
    ZFCoreArrayPOD<zfidentity> ignoredMouses;
    ZFCoreArrayPOD<_ZFP_ZFUIButtonMouseData> processingMouses;
    zfstlmap<zfidentity, ZFUIPoint> prevMousePointMap;
    zfbool buttonEnableFlag;
    zfbool buttonHighlightedFlag;
    zftimet buttonLastClickTimestamp;

public:
    void buttonEnable(ZF_IN zfbool buttonEnable)
    {
        if(this->buttonEnableFlag == buttonEnable)
        {
            return ;
        }
        this->buttonEnableFlag = buttonEnable;

        this->buttonStateUpdate(this->buttonHighlightedFlag);

        if(!this->processingMouses.isEmpty())
        {
            for(zfindex i = 0; i < this->processingMouses.count(); ++i)
            {
                this->ignoredMouses.add(this->processingMouses[i].mouseId);
            }
            ZFCoreArrayPOD<_ZFP_ZFUIButtonMouseData> tmp = this->processingMouses;
            this->processingMouses = ZFCoreArrayPOD<_ZFP_ZFUIButtonMouseData>();

            for(zfindex i = 0; i < tmp.count(); ++i)
            {
                const _ZFP_ZFUIButtonMouseData &mouseData = tmp[i];
                this->prevMousePointMap.erase(mouseData.mouseId);
                _ZFP_ZFUIButtonMouseDataToEvent(event, mouseData);

                _ZFP_ZFUIButton_DEBUG_EVENT(buttonMouseOnUp)
                this->pimplOwner->buttonMouseOnUp(event);
            }
        }
    }

public:
    void buttonStateUpdate(ZF_IN zfbool highlighted)
    {
        this->buttonHighlightedFlag = highlighted;
        if(this->buttonEnableFlag)
        {
            if(this->buttonHighlightedFlag)
            {
                if(this->pimplOwner->buttonChecked())
                {
                    this->buttonState = ZFUIButtonState::e_CheckedHighlighted;
                }
                else
                {
                    this->buttonState = ZFUIButtonState::e_Highlighted;
                }
                _ZFP_ZFUIButton_DEBUG_EVENT(buttonStateOnHighlighted)
                this->pimplOwner->buttonStateOnUpdate();
            }
            else
            {
                if(this->pimplOwner->buttonChecked())
                {
                    this->buttonState = ZFUIButtonState::e_Checked;
                }
                else
                {
                    this->buttonState = ZFUIButtonState::e_Normal;
                }
                _ZFP_ZFUIButton_DEBUG_EVENT(buttonStateOnNormal)
                this->pimplOwner->buttonStateOnUpdate();
            }
        }
        else
        {
            this->buttonState = ZFUIButtonState::e_Disabled;
            _ZFP_ZFUIButton_DEBUG_EVENT(buttonStateOnDisabled)
            this->pimplOwner->buttonStateOnUpdate();
        }
    }
    void viewEventOnMouseEvent(ZF_IN ZFUIMouseEvent *mouseEvent)
    {
        _ZFP_ZFUIButton_DEBUG_LOG("begin %s", ZFObjectInfo(mouseEvent).cString())
        ZFUIMouseActionEnum mouseAction = mouseEvent->mouseAction;
        switch(mouseAction)
        {
            case ZFUIMouseAction::e_MouseDown:
            {
                if(!this->buttonEnableFlag)
                {
                    _ZFP_ZFUIButton_DEBUG_LOG("      %s disabled", ZFObjectInfo(mouseEvent).cString())
                    this->ignoredMouses.add(mouseEvent->mouseId);
                    break;
                }

                if(this->processingMouseId != zfidentityInvalid())
                {
                    _ZFP_ZFUIButton_DEBUG_LOG("      %s ignored", ZFObjectInfo(mouseEvent).cString())
                    this->ignoredMouses.add(mouseEvent->mouseId);
                }
                else
                {
                    this->processingMouseId = mouseEvent->mouseId;
                    this->processingMouses.add(_ZFP_ZFUIButtonMouseDataFromEvent(mouseEvent));
                    this->processMouse(mouseEvent);
                }
            }
                break;
            case ZFUIMouseAction::e_MouseMove:
            {
                if(this->ignoredMouses.find(mouseEvent->mouseId) == zfindexMax())
                {
                    this->processMouse(mouseEvent);
                }
            }
                break;
            case ZFUIMouseAction::e_MouseUp:
            case ZFUIMouseAction::e_MouseCancel:
            {
                for(zfindex i = 0; i < this->processingMouses.count(); ++i)
                {
                    if(this->processingMouses[i].mouseId == mouseEvent->mouseId)
                    {
                        this->processingMouses.remove(i);
                        break;
                    }
                }
                zfindex ignoredIndex = this->ignoredMouses.find(mouseEvent->mouseId);
                if(ignoredIndex != zfindexMax())
                {
                    this->ignoredMouses.remove(ignoredIndex);
                }
                else
                {
                    this->processMouse(mouseEvent);
                    if(this->processingMouseId == mouseEvent->mouseId)
                    {
                        this->processingMouseId = zfidentityInvalid();
                    }
                }
            }
                break;
            case ZFUIMouseAction::e_MouseHoverEnter:
            case ZFUIMouseAction::e_MouseHover:
            case ZFUIMouseAction::e_MouseHoverExit:
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                return ;
        }
        mouseEvent->eventResolved(zftrue);
        _ZFP_ZFUIButton_DEBUG_LOG(" end  %s", ZFObjectInfo(mouseEvent).cString())
    }

private:
    void processMouse(ZF_IN ZFUIMouseEvent *mouseEvent)
    {
        _ZFP_ZFUIButton_DEBUG_LOG("process begin %s", ZFObjectInfo(mouseEvent).cString())
        switch(mouseEvent->mouseAction)
        {
            case ZFUIMouseAction::e_MouseDown:
            {
                this->prevMousePointMap[mouseEvent->mouseId] = mouseEvent->mousePoint;
                this->buttonStateUpdate(zftrue);
                _ZFP_ZFUIButton_DEBUG_EVENT(buttonMouseOnDown)
                this->pimplOwner->buttonMouseOnDown(mouseEvent);
            }
                break;
            case ZFUIMouseAction::e_MouseMove:
            {
                zfstlmap<zfidentity, ZFUIPoint>::iterator prevMousePointMapIt = this->prevMousePointMap.find(mouseEvent->mouseId);
                if(prevMousePointMapIt == this->prevMousePointMap.end())
                {
                    break;
                }
                ZFUIPoint &prevMousePoint = prevMousePointMapIt->second;
                ZFUIRect bounds = ZFUIRectGetBounds(this->pimplOwner->viewFrame());
                zfbool mouseInside = this->buttonClickedInside(bounds, mouseEvent->mousePoint);
                zfbool mouseInsidePrev = this->buttonClickedInside(bounds, prevMousePoint);
                prevMousePoint = mouseEvent->mousePoint;

                if(mouseInside != mouseInsidePrev)
                {
                    this->buttonStateUpdate(mouseInside);
                    if(mouseInside)
                    {
                        _ZFP_ZFUIButton_DEBUG_EVENT(buttonMouseOnMoveEnter)
                        this->pimplOwner->buttonMouseOnMoveEnter(mouseEvent);
                    }
                    else
                    {
                        _ZFP_ZFUIButton_DEBUG_EVENT(buttonMouseOnMoveExit)
                        this->pimplOwner->buttonMouseOnMoveExit(mouseEvent);
                    }
                }
                if(mouseInside)
                {
                    _ZFP_ZFUIButton_DEBUG_EVENT(buttonMouseOnMoveInside)
                    this->pimplOwner->buttonMouseOnMoveInside(mouseEvent);
                }
                else
                {
                    _ZFP_ZFUIButton_DEBUG_EVENT(buttonMouseOnMoveOutside)
                    this->pimplOwner->buttonMouseOnMoveOutside(mouseEvent);
                }
            }
                break;
            case ZFUIMouseAction::e_MouseUp:
            {
                zfstlmap<zfidentity, ZFUIPoint>::iterator prevMousePointMapIt = this->prevMousePointMap.find(mouseEvent->mouseId);
                if(prevMousePointMapIt == this->prevMousePointMap.end())
                {
                    break;
                }
                this->prevMousePointMap.erase(prevMousePointMapIt);

                zfbool mouseInside = this->buttonClickedInside(
                    ZFUIRectGetBounds(this->pimplOwner->viewFrame()),
                    mouseEvent->mousePoint);

                this->buttonHighlightedFlag = zffalse;
                if(mouseInside)
                {
                    _ZFP_ZFUIButton_DEBUG_EVENT(buttonMouseOnUpInside)
                    this->pimplOwner->buttonMouseOnUpInside(mouseEvent);
                    if(this->pimplOwner->buttonCheckable())
                    {
                        this->pimplOwner->buttonChecked(!this->pimplOwner->buttonChecked());
                    }
                    else
                    {
                        this->buttonStateUpdate(this->buttonHighlightedFlag);
                    }
                    this->buttonClicked(mouseEvent);
                }
                else
                {
                    _ZFP_ZFUIButton_DEBUG_EVENT(buttonMouseOnUp)
                    this->pimplOwner->buttonMouseOnUp(mouseEvent);
                    this->buttonStateUpdate(this->buttonHighlightedFlag);
                }
            }
                break;
            case ZFUIMouseAction::e_MouseCancel:
            {
                zfstlmap<zfidentity, ZFUIPoint>::iterator prevMousePointMapIt = this->prevMousePointMap.find(mouseEvent->mouseId);
                if(prevMousePointMapIt == this->prevMousePointMap.end())
                {
                    break;
                }
                this->prevMousePointMap.erase(prevMousePointMapIt);

                this->buttonStateUpdate(zffalse);
                _ZFP_ZFUIButton_DEBUG_EVENT(buttonMouseOnUp)
                this->pimplOwner->buttonMouseOnUp(mouseEvent);
            }
                break;
            case ZFUIMouseAction::e_MouseHoverEnter:
            case ZFUIMouseAction::e_MouseHover:
            case ZFUIMouseAction::e_MouseHoverExit:
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                return ;
        }
        _ZFP_ZFUIButton_DEBUG_LOG("process  end  %s", ZFObjectInfo(mouseEvent).cString())
    }
    zfbool buttonClickedInside(ZF_IN const ZFUIRect &bounds,
                               ZF_IN const ZFUIPoint &mousePoint)
    {
        return ZFUIRectIsContainPoint(bounds, mousePoint, this->pimplOwner->buttonClickTolerance());
    }
public:
    void buttonClicked(ZF_IN ZFUIEvent *event)
    {
        zftimet curTime = ZFTime::timestamp();
        if(curTime - this->buttonLastClickTimestamp >= this->pimplOwner->buttonClickInterval())
        {
            this->buttonLastClickTimestamp = curTime;

            _ZFP_ZFUIButton_DEBUG_EVENT(buttonOnClick)
            this->pimplOwner->buttonOnClick(event);
        }
        else
        {
            _ZFP_ZFUIButton_DEBUG_LOG("click ignored %s", ZFObjectInfo(event).cString())
        }
    }

public:
    _ZFP_ZFUIButtonPrivate(void)
    : pimplOwner(zfnull)
    , buttonState(ZFUIButtonState::e_Normal)
    , processingMouseId(zfidentityInvalid())
    , ignoredMouses()
    , processingMouses()
    , prevMousePointMap()
    , buttonEnableFlag(zftrue)
    , buttonHighlightedFlag(zffalse)
    , buttonLastClickTimestamp(0)
    {
    }
};

// ============================================================
// ZFUIButton
ZFOBJECT_REGISTER(ZFUIButton)

ZFOBSERVER_EVENT_REGISTER(ZFUIButton, ButtonOnClick)
ZFOBSERVER_EVENT_REGISTER(ZFUIButton, ButtonStateOnUpdate)
ZFOBSERVER_EVENT_REGISTER(ZFUIButton, ButtonMouseOnDown)
ZFOBSERVER_EVENT_REGISTER(ZFUIButton, ButtonMouseOnMoveEnter)
ZFOBSERVER_EVENT_REGISTER(ZFUIButton, ButtonMouseOnMoveExit)
ZFOBSERVER_EVENT_REGISTER(ZFUIButton, ButtonMouseOnMoveInside)
ZFOBSERVER_EVENT_REGISTER(ZFUIButton, ButtonMouseOnMoveOutside)
ZFOBSERVER_EVENT_REGISTER(ZFUIButton, ButtonMouseOnUpInside)
ZFOBSERVER_EVENT_REGISTER(ZFUIButton, ButtonMouseOnUp)

ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUIButton, zfbool, buttonEnable)
{
    d->buttonEnable(this->buttonEnable());
}
ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUIButton, zfbool, buttonCheckable)
{
    if(!this->buttonCheckable() && this->buttonChecked())
    {
        this->buttonChecked(zffalse);
    }
}
ZFPROPERTY_OVERRIDE_ON_VERIFY_DEFINE(ZFUIButton, zfbool, buttonChecked)
{
    if(!this->buttonCheckable())
    {
        propertyValue = zffalse;
    }
}
ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUIButton, zfbool, buttonChecked)
{
    d->buttonStateUpdate(d->buttonHighlightedFlag);
}

void ZFUIButton::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFUIButtonPrivate);
    d->pimplOwner = this;
}
void ZFUIButton::objectOnDealloc(void)
{
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}

ZFMETHOD_DEFINE_0(ZFUIButton, void, buttonClickIntervalReset)
{
    d->buttonLastClickTimestamp = 0;
}

ZFMETHOD_DEFINE_1(ZFUIButton, void, buttonSimulateClick,
                  ZFMP_IN_OPT(ZFUIEvent *, event, zfnull))
{
    zfCoreAssert(ZFThread::currentThread() == ZFThread::mainThread());
    d->buttonHighlightedFlag = zffalse;
    if(this->buttonCheckable())
    {
        this->buttonChecked(!this->buttonChecked());
    }
    else
    {
        d->buttonStateUpdate(d->buttonHighlightedFlag);
    }
    d->buttonClicked(event);
}

ZFMETHOD_DEFINE_5(ZFUIButton, zfidentity, onClick,
                  ZFMP_IN(const ZFListener &, observer),
                  ZFMP_IN_OPT(ZFObject *, userData, zfnull),
                  ZFMP_IN_OPT(ZFObject *, owner, zfnull),
                  ZFMP_IN_OPT(zfbool, autoRemoveAfterActivate, zffalse),
                  ZFMP_IN_OPT(ZFLevel, observerLevel, ZFLevelAppNormal))
{
    return this->observerAdd(ZFUIButton::EventButtonOnClick(),
            observer, userData, owner, autoRemoveAfterActivate, observerLevel
        );
}
ZFMETHOD_DEFINE_4(ZFUIButton, zfidentity, onClickForOnce,
                  ZFMP_IN(const ZFListener &, observer),
                  ZFMP_IN_OPT(ZFObject *, userData, zfnull),
                  ZFMP_IN_OPT(ZFObject *, owner, zfnull),
                  ZFMP_IN_OPT(ZFLevel, observerLevel, ZFLevelAppNormal))
{
    return this->observerAddForOnce(ZFUIButton::EventButtonOnClick(),
            observer, userData, owner, observerLevel
        );
}

void ZFUIButton::viewEventOnMouseEvent(ZF_IN ZFUIMouseEvent *mouseEvent)
{
    zfsuper::viewEventOnMouseEvent(mouseEvent);

    if(mouseEvent->mouseButton == ZFUIMouseButton::e_MouseButtonLeft)
    {
        d->viewEventOnMouseEvent(mouseEvent);
    }
}
void ZFUIButton::viewEventOnKeyEvent(ZF_IN ZFUIKeyEvent *keyEvent)
{
    zfsuper::viewEventOnKeyEvent(keyEvent);
    if(keyEvent->eventResolved()) {
        return ;
    }

    switch(keyEvent->keyCode)
    {
        case ZFUIKeyCode::e_kEnter:
        case ZFUIKeyCode::e_kSpace:
            switch(keyEvent->keyAction)
            {
                case ZFUIKeyAction::e_KeyDown:
                    d->buttonStateUpdate(zftrue);
                    break;
                case ZFUIKeyAction::e_KeyRepeat:
                    break;
                case ZFUIKeyAction::e_KeyUp:
                    d->buttonHighlightedFlag = zffalse;
                    if(this->buttonCheckable())
                    {
                        this->buttonChecked(!this->buttonChecked());
                    }
                    else
                    {
                        d->buttonStateUpdate(d->buttonHighlightedFlag);
                    }
                    d->buttonClicked(keyEvent);
                    break;
                case ZFUIKeyAction::e_KeyCancel:
                    d->buttonStateUpdate(zffalse);
                    break;
                default:
                    zfCoreCriticalShouldNotGoHere();
                    break;
            }
            keyEvent->eventResolved(zftrue);
            break;
        default:
            break;
    }
}

void ZFUIButton::viewPropertyOnUpdate(void)
{
    zfsuper::viewPropertyOnUpdate();
    this->buttonStateUpdate();
}

ZFMETHOD_DEFINE_0(ZFUIButton, ZFUIButtonStateEnum, buttonState)
{
    return d->buttonState;
}
ZFMETHOD_DEFINE_0(ZFUIButton, void, buttonStateUpdate)
{
    this->buttonStateOnUpdate();
}

ZF_NAMESPACE_GLOBAL_END

