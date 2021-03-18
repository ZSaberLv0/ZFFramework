#include "ZFUIButtonGroup.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFENUM_DEFINE(ZFUIButtonGroupType)

ZFOBJECT_REGISTER(ZFUIButtonGroup)

ZFOBSERVER_EVENT_REGISTER(ZFUIButtonGroup, ButtonOnAdd)
ZFOBSERVER_EVENT_REGISTER(ZFUIButtonGroup, ButtonOnRemove)
ZFOBSERVER_EVENT_REGISTER(ZFUIButtonGroup, ButtonTabOnChange)
ZFOBSERVER_EVENT_REGISTER(ZFUIButtonGroup, ButtonTabOnClickChecked)

// ============================================================
// common
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIButtonGroupListenerHolder, ZFLevelZFFrameworkEssential)
{
    this->buttonEventListener = ZFCallbackForFunc(zfself::buttonEvent);
}
public:
    ZFListener buttonEventListener;
    static ZFLISTENER_PROTOTYPE_EXPAND(buttonEvent)
    {
        ZFUIButtonGroup *buttonGroup = userData->objectHolded();
        ZFUIButton *button = listenerData.sender<ZFUIButton *>();
        zfindex buttonIndex = buttonGroup->buttonFind(button);
        zfCoreAssert(buttonIndex != zfindexMax());
        buttonGroup->_ZFP_ZFUIButtonGroup_buttonGroupOnEvent(button, buttonIndex, listenerData.eventId());
    }
ZF_GLOBAL_INITIALIZER_END(ZFUIButtonGroupListenerHolder)
static void _ZFP_ZFUIButtonGroup_setup_common(ZF_IN ZFUIButtonGroup *buttonGroup,
                                              ZF_IN ZFUIButton *button,
                                              ZF_IN zfindex buttonIndex)
{
    const ZFListener &buttonEventListener = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIButtonGroupListenerHolder)->buttonEventListener;
    ZFObject *buttonGroupHolder = buttonGroup->toObject()->objectHolder();
    button->observerAdd(ZFUIButton::EventButtonOnClick(), buttonEventListener, buttonGroupHolder);
    button->observerAdd(ZFUIButton::EventButtonStateOnUpdate(), buttonEventListener, buttonGroupHolder);
    button->observerAdd(ZFUIButton::EventButtonMouseOnDown(), buttonEventListener, buttonGroupHolder);
    button->observerAdd(ZFUIButton::EventButtonMouseOnMoveEnter(), buttonEventListener, buttonGroupHolder);
    button->observerAdd(ZFUIButton::EventButtonMouseOnMoveExit(), buttonEventListener, buttonGroupHolder);
    button->observerAdd(ZFUIButton::EventButtonMouseOnMoveInside(), buttonEventListener, buttonGroupHolder);
    button->observerAdd(ZFUIButton::EventButtonMouseOnMoveOutside(), buttonEventListener, buttonGroupHolder);
    button->observerAdd(ZFUIButton::EventButtonMouseOnUpInside(), buttonEventListener, buttonGroupHolder);
    button->observerAdd(ZFUIButton::EventButtonMouseOnUp(), buttonEventListener, buttonGroupHolder);
}
static void _ZFP_ZFUIButtonGroup_cleanup_common(ZF_IN ZFUIButtonGroup *buttonGroup,
                                                ZF_IN ZFUIButton *button)
{
    const ZFListener &buttonEventListener = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIButtonGroupListenerHolder)->buttonEventListener;
    button->observerRemove(ZFUIButton::EventButtonOnClick(), buttonEventListener);
    button->observerRemove(ZFUIButton::EventButtonStateOnUpdate(), buttonEventListener);
    button->observerRemove(ZFUIButton::EventButtonMouseOnDown(), buttonEventListener);
    button->observerRemove(ZFUIButton::EventButtonMouseOnMoveEnter(), buttonEventListener);
    button->observerRemove(ZFUIButton::EventButtonMouseOnMoveExit(), buttonEventListener);
    button->observerRemove(ZFUIButton::EventButtonMouseOnMoveInside(), buttonEventListener);
    button->observerRemove(ZFUIButton::EventButtonMouseOnMoveOutside(), buttonEventListener);
    button->observerRemove(ZFUIButton::EventButtonMouseOnUpInside(), buttonEventListener);
    button->observerRemove(ZFUIButton::EventButtonMouseOnUp(), buttonEventListener);
}

// ============================================================
// ZFUIButtonGroupType::e_Normal
static void _ZFP_ZFUIButtonGroup_setup_Normal(ZF_IN ZFUIButtonGroup *buttonGroup,
                                              ZF_IN ZFUIButton *button,
                                              ZF_IN zfindex buttonIndex)
{
    _ZFP_ZFUIButtonGroup_setup_common(buttonGroup, button, buttonIndex);
}
static void _ZFP_ZFUIButtonGroup_cleanup_Normal(ZF_IN ZFUIButtonGroup *buttonGroup,
                                                ZF_IN ZFUIButton *button)
{
    _ZFP_ZFUIButtonGroup_cleanup_common(buttonGroup, button);
}

// ============================================================
// ZFUIButtonGroupType::e_Tab
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIButtonGroupListenerHolder_Tab, ZFLevelZFFrameworkEssential)
{
    this->buttonOnClickListener = ZFCallbackForFunc(zfself::buttonOnClick);
}
public:
    ZFListener buttonOnClickListener;
    static ZFLISTENER_PROTOTYPE_EXPAND(buttonOnClick)
    {
        ZFUIButtonGroup *buttonGroup = userData->objectHolded();
        ZFUIButton *button = listenerData.sender<ZFUIButton *>();
        zfindex buttonIndex = buttonGroup->buttonFind(button);
        zfCoreAssert(buttonIndex != zfindexMax());
        if(buttonIndex == buttonGroup->buttonTabChecked())
        {
            if(buttonGroup->buttonTabAllowUnchecked())
            {
                buttonGroup->buttonTabChecked(zfindexMax());
                buttonGroup->_ZFP_ZFUIButtonGroup_buttonTabOnChange(button, buttonIndex);
            }
            else
            {
                button->buttonChecked(zftrue);
            }
            buttonGroup->_ZFP_ZFUIButtonGroup_buttonTabOnClickChecked(button, buttonIndex);
        }
        else
        {
            zfindex buttonIndexPrev = buttonGroup->buttonTabChecked();
            buttonGroup->buttonTabChecked(buttonIndex);
            buttonGroup->_ZFP_ZFUIButtonGroup_buttonTabOnChange(button, buttonIndexPrev);
        }
    }
ZF_GLOBAL_INITIALIZER_END(ZFUIButtonGroupListenerHolder_Tab)
static void _ZFP_ZFUIButtonGroup_setup_Tab(ZF_IN ZFUIButtonGroup *buttonGroup,
                                           ZF_IN ZFUIButton *button,
                                           ZF_IN zfindex buttonIndex)
{
    _ZFP_ZFUIButtonGroup_setup_common(buttonGroup, button, buttonIndex);

    button->buttonCheckable(zftrue);
    button->buttonChecked(zffalse);

    button->observerAdd(
        ZFUIButton::EventButtonOnClick(),
        ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIButtonGroupListenerHolder_Tab)->buttonOnClickListener,
        buttonGroup->toObject()->objectHolder());

    if(!buttonGroup->buttonTabAllowUnchecked() && buttonGroup->buttonTabChecked() == zfindexMax())
    {
        buttonGroup->buttonTabChecked(buttonIndex);
    }
}
static void _ZFP_ZFUIButtonGroup_cleanup_Tab(ZF_IN ZFUIButtonGroup *buttonGroup,
                                             ZF_IN ZFUIButton *button)
{
    _ZFP_ZFUIButtonGroup_cleanup_common(buttonGroup, button);

    button->observerRemove(
        ZFUIButton::EventButtonOnClick(),
        ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIButtonGroupListenerHolder_Tab)->buttonOnClickListener);
}
ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUIButtonGroup, zfbool, buttonTabAllowUnchecked)
{
    if(!this->buttonTabAllowUnchecked() && this->buttonCount() > 0 && this->buttonTabChecked() == zfindexMax())
    {
        this->buttonTabChecked(0);
    }
}
ZFPROPERTY_OVERRIDE_ON_VERIFY_DEFINE(ZFUIButtonGroup, zfindex, buttonTabChecked)
{
    propertyValue = ((propertyValue >= this->buttonCount()) ? zfindexMax() : propertyValue);
}
ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUIButtonGroup, zfindex, buttonTabChecked)
{
    if(this->buttonTabChecked() != propertyValueOld)
    {
        if(!this->buttonTabAllowUnchecked() && this->buttonTabChecked() >= this->buttonCount())
        {
            return ;
        }
        if(propertyValueOld != zfindexMax())
        {
            this->buttonAtIndex(propertyValueOld)->buttonChecked(zffalse);
        }
        if(this->buttonTabChecked() != zfindexMax())
        {
            this->buttonAtIndex(this->buttonTabChecked())->buttonChecked(zftrue);
        }
    }
}

// ============================================================
static void _ZFP_ZFUIButtonGroup_setup(ZF_IN ZFUIButtonGroup *buttonGroup,
                                       ZF_IN ZFUIButton *button,
                                       ZF_IN zfindex buttonIndex)
{
    switch(buttonGroup->buttonGroupType())
    {
        case ZFUIButtonGroupType::e_Normal:
            _ZFP_ZFUIButtonGroup_setup_Normal(buttonGroup, button, buttonIndex);
            break;
        case ZFUIButtonGroupType::e_Tab:
            _ZFP_ZFUIButtonGroup_setup_Tab(buttonGroup, button, buttonIndex);
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            return ;
    }
}
static void _ZFP_ZFUIButtonGroup_cleanup(ZF_IN ZFUIButtonGroup *buttonGroup,
                                         ZF_IN ZFUIButton *button)
{
    switch(buttonGroup->buttonGroupType())
    {
        case ZFUIButtonGroupType::e_Normal:
            _ZFP_ZFUIButtonGroup_cleanup_Normal(buttonGroup, button);
            break;
        case ZFUIButtonGroupType::e_Tab:
            _ZFP_ZFUIButtonGroup_cleanup_Tab(buttonGroup, button);
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            return ;
    }
}

ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUIButtonGroup, ZFUIButtonGroupTypeEnum, buttonGroupType)
{
    if(this->buttonGroupType() == propertyValueOld)
    {
        return ;
    }
    for(zfindex i = 0; i < this->buttonCount(); ++i)
    {
        _ZFP_ZFUIButtonGroup_cleanup(this, this->buttonAtIndex(i));
    }
    for(zfindex i = 0; i < this->buttonCount(); ++i)
    {
        _ZFP_ZFUIButtonGroup_setup(this, this->buttonAtIndex(i), i);
    }
}

ZFMETHOD_DEFINE_0(ZFUIButtonGroup, zfindex, buttonCount)
{
    return this->_ZFP_ZFUIButtonGroup_buttons()->count();
}
ZFMETHOD_DEFINE_1(ZFUIButtonGroup, zfindex, buttonFind,
                  ZFMP_IN(ZFUIButton *, button))
{
    return this->_ZFP_ZFUIButtonGroup_buttons()->find(button);
}
ZFMETHOD_DEFINE_1(ZFUIButtonGroup, ZFUIButton *, buttonAtIndex,
                  ZFMP_IN(zfindex, buttonIndex))
{
    return this->_ZFP_ZFUIButtonGroup_buttons()->get<ZFUIButton *>(buttonIndex);
}
ZFMETHOD_DEFINE_2(ZFUIButtonGroup, void, buttonAdd,
                  ZFMP_IN(ZFUIButton *, button),
                  ZFMP_IN_OPT(zfindex, atIndex, zfindexMax()))
{
    if(atIndex == zfindexMax())
    {
        atIndex = this->buttonCount();
    }
    else if(atIndex >= this->buttonCount())
    {
        zfCoreCriticalIndexOutOfRange(atIndex, this->buttonCount());
        return ;
    }
    this->_ZFP_ZFUIButtonGroup_buttons()->add(atIndex, button);
    _ZFP_ZFUIButtonGroup_setup(this, button, atIndex);
    this->buttonOnAdd(button, atIndex);
}
ZFMETHOD_DEFINE_1(ZFUIButtonGroup, void, buttonRemove,
                  ZFMP_IN(ZFUIButton *, button))
{
    this->buttonRemoveAtIndex(this->_ZFP_ZFUIButtonGroup_buttons()->find(button));
}
ZFMETHOD_DEFINE_1(ZFUIButtonGroup, void, buttonRemoveAtIndex,
                  ZFMP_IN(zfindex, buttonIndex))
{
    if(buttonIndex == zfindexMax())
    {
        return ;
    }
    if(buttonIndex >= this->buttonCount())
    {
        zfCoreCriticalIndexOutOfRange(buttonIndex, this->buttonCount());
        return ;
    }
    ZFUIButton *button = this->buttonAtIndex(buttonIndex);
    zfRetain(button);
    _ZFP_ZFUIButtonGroup_cleanup(this, button);
    this->_ZFP_ZFUIButtonGroup_buttons()->remove(buttonIndex);
    this->buttonOnRemove(button, buttonIndex);
    zfRelease(button);
}
ZFMETHOD_DEFINE_0(ZFUIButtonGroup, void, buttonRemoveAll)
{
    while(this->buttonCount() > 0)
    {
        this->buttonRemoveAtIndex(this->buttonCount() - 1);
    }
}

// ============================================================
// ZFUIButtonGroupArray
ZFOBJECT_REGISTER(ZFUIButtonGroupArray)

ZF_NAMESPACE_GLOBAL_END

