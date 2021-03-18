/**
 * @file ZFUIButtonGroup.h
 * @brief abstract button group management
 */

#ifndef _ZFI_ZFUIButtonGroup_h_
#define _ZFI_ZFUIButtonGroup_h_

#include "ZFUIButton.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUIButtonGroupType
/**
 * @brief button group type
 */
ZFENUM_BEGIN(ZFUIButtonGroupType)
    /**
     * @brief normal, all buttons are added without any modification
     */
    ZFENUM_VALUE(Normal)
    /**
     * @brief act like tab management, all buttons are changed to checkable,
     *   and only one button can be checked at one time
     *
     * this type can be used as tab buttons and ratio buttons
     */
    ZFENUM_VALUE(Tab)
ZFENUM_SEPARATOR(ZFUIButtonGroupType)
    ZFENUM_VALUE_REGISTER(Normal)
    ZFENUM_VALUE_REGISTER(Tab)
ZFENUM_END(ZFUIButtonGroupType)

// ============================================================
// ZFUIButtonGroup
/**
 * @brief abstract button group management
 *
 * you have two basic ways to use a button group to manage button views:
 * -  implements from #ZFUIButtonGroup,
 *   override #ZFUIButtonGroup::buttonOnAdd and #ZFUIButtonGroup::buttonOnRemove
 *   to manage button views
 * -  use #ZFUIButtonGroupArray to manage buttons,
 *   and observe #ZFUIButtonGroup::EventButtonOnAdd and #ZFUIButtonGroup::EventButtonOnRemove
 *   to manage button views
 *
 * after buttons added, you may observe #ZFUIButton's button event to achieve your logic\n
 * see #ZFUIButtonGroup::buttonGroupOnEvent for more info
 */
zfinterface ZF_ENV_EXPORT ZFUIButtonGroup : zfextends ZFInterface
{
    ZFINTERFACE_DECLARE(ZFUIButtonGroup, ZFInterface)

public:
    // ============================================================
    // events
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when button added,
     * sender is the button,
     * param0 is this button group itself,
     * param1 is a #v_zfindex which shows the button's index
     */
    ZFOBSERVER_EVENT(ButtonOnAdd)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when button removed,
     * sender is the button,
     * param0 is this button group itself,
     * param1 is a #v_zfindex which shows the button's index
     */
    ZFOBSERVER_EVENT(ButtonOnRemove)

public:
    /**
     * @brief button group's type, see #ZFUIButtonGroupType::e_Normal for more info
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIButtonGroupTypeEnum, buttonGroupType,
                                ZFUIButtonGroupType::EnumDefault())
    ZFPROPERTY_OVERRIDE_ON_ATTACH_DECLARE(ZFUIButtonGroupTypeEnum, buttonGroupType)

private:
    ZFPROPERTY_RETAIN_WITH_INIT(ZFArrayEditable *, _ZFP_ZFUIButtonGroup_buttons, zflineAlloc(ZFArrayEditable))
public:
    /**
     * @brief button count
     */
    ZFMETHOD_DECLARE_0(zfindex, buttonCount)
    /**
     * @brief find the button's index or return zfindexMax() if not exist
     */
    ZFMETHOD_DECLARE_1(zfindex, buttonFind,
                       ZFMP_IN(ZFUIButton *, button))
    /**
     * @brief get button at index
     */
    ZFMETHOD_DECLARE_1(ZFUIButton *, buttonAtIndex,
                       ZFMP_IN(zfindex, buttonIndex))
    /**
     * @brief add button
     *
     * newly added button's setting would be changed according #buttonGroupType,
     * before #buttonOnAdd
     */
    ZFMETHOD_DECLARE_2(void, buttonAdd,
                       ZFMP_IN(ZFUIButton *, button),
                       ZFMP_IN_OPT(zfindex, atIndex, zfindexMax()))
    /**
     * @brief remove button or do nothing if not in this button group
     *
     * removed button's setting won't be reset by this method
     */
    ZFMETHOD_DECLARE_1(void, buttonRemove,
                       ZFMP_IN(ZFUIButton *, button))
    /**
     * @brief remove button at index
     *
     * removed button's setting won't be reset by this method
     */
    ZFMETHOD_DECLARE_1(void, buttonRemoveAtIndex,
                       ZFMP_IN(zfindex, buttonIndex))
    /**
     * @brief remove all button
     */
    ZFMETHOD_DECLARE_0(void, buttonRemoveAll)

public:
    zffinal inline void _ZFP_ZFUIButtonGroup_buttonGroupOnEvent(ZF_IN ZFUIButton *button,
                                                                ZF_IN zfindex buttonIndex,
                                                                ZF_IN zfidentity eventId)
    {
        this->buttonGroupOnEvent(button, buttonIndex, eventId);
    }
protected:
    /**
     * @brief called when any of button event are notified
     *   to the buttons managed by this group
     *
     * here's a list of button event that would be fired:
     * -  #ZFUIButton::EventButtonOnClick
     * -  #ZFUIButton::EventButtonStateOnUpdate
     * -  #ZFUIButton::EventButtonMouseOnDown
     * -  #ZFUIButton::EventButtonMouseOnMoveEnter
     * -  #ZFUIButton::EventButtonMouseOnMoveExit
     * -  #ZFUIButton::EventButtonMouseOnMoveInside
     * -  #ZFUIButton::EventButtonMouseOnMoveOutside
     * -  #ZFUIButton::EventButtonMouseOnUpInside
     * -  #ZFUIButton::EventButtonMouseOnUp
     *
     * for example:
     * @code
     *   ZFUIButtonGroup *buttonGroup = ...;
     *   buttonGroup->toObject()->observerAdd(ZFUIButton::EventButtonOnClick(), myObserver);
     * @endcode
     * note:
     * sender is the button that fired the button event,
     * param0 is the button group itself,
     * param1 is a #v_zfindex which shows the button's index
     */
    virtual inline void buttonGroupOnEvent(ZF_IN ZFUIButton *button,
                                           ZF_IN zfindex buttonIndex,
                                           ZF_IN zfidentity eventId)
    {
        this->toObject()->observerNotifyWithCustomSender(
            button,
            eventId,
            this->toObject(),
            zflineAlloc(v_zfindex, buttonIndex));
    }
    /** @brief see #EventButtonOnAdd */
    virtual inline void buttonOnAdd(ZF_IN ZFUIButton *button,
                                    ZF_IN zfindex buttonIndex)
    {
        this->toObject()->observerNotifyWithCustomSender(
            button,
            ZFUIButtonGroup::EventButtonOnAdd(),
            this->toObject(),
            zflineAlloc(v_zfindex, buttonIndex));
    }
    /** @brief see #EventButtonOnRemove */
    virtual inline void buttonOnRemove(ZF_IN ZFUIButton *button,
                                       ZF_IN zfindex buttonIndex)
    {
        this->toObject()->observerNotifyWithCustomSender(
            button,
            ZFUIButtonGroup::EventButtonOnRemove(),
            this->toObject(),
            zflineAlloc(v_zfindex, buttonIndex));
    }

    // ============================================================
    // for ZFUIButtonGroupType::e_Tab type
public:
    /**
     * @brief see #ZFObject::observerNotify
     *
     * for #ZFUIButtonGroupType::e_Tab type only\n
     * called when #buttonTabChecked changed,
     * sender is the newly clicked button,
     * param0 is the button group itself,
     * param1 is a #v_zfindex which shows the previous checked button index
     * (may be #zfindexMax if nothing checked previously)
     */
    ZFOBSERVER_EVENT(ButtonTabOnChange)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * for #ZFUIButtonGroupType::e_Tab type only\n
     * called when checked button clicked,
     * sender is the clicked button,
     * param0 is the button group itself,
     * param1 is a #v_zfindex which shows the button's index
     */
    ZFOBSERVER_EVENT(ButtonTabOnClickChecked)

    /**
     * @brief for #ZFUIButtonGroupType::e_Tab type only, whether allow uncheck all button, false by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfbool, buttonTabAllowUnchecked, zffalse)
    ZFPROPERTY_OVERRIDE_ON_ATTACH_DECLARE(zfbool, buttonTabAllowUnchecked)

    /**
     * @brief for #ZFUIButtonGroupType::e_Tab type only, the checked tab index, zfindexMax() by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfindex, buttonTabChecked, zfindexMax())
    ZFPROPERTY_OVERRIDE_ON_VERIFY_DECLARE(zfindex, buttonTabChecked)
    ZFPROPERTY_OVERRIDE_ON_ATTACH_DECLARE(zfindex, buttonTabChecked)

public:
    zffinal inline void _ZFP_ZFUIButtonGroup_buttonTabOnChange(ZF_IN ZFUIButton *button,
                                                               ZF_IN zfindex buttonIndexPrev)
    {
        this->buttonTabOnChange(button, buttonIndexPrev);
    }
    zffinal inline void _ZFP_ZFUIButtonGroup_buttonTabOnClickChecked(ZF_IN ZFUIButton *button,
                                                                     ZF_IN zfindex buttonIndex)
    {
        this->buttonTabOnClickChecked(button, buttonIndex);
    }
protected:
    /** @brief see #EventButtonTabOnChange */
    virtual inline void buttonTabOnChange(ZF_IN ZFUIButton *button,
                                          ZF_IN zfindex buttonIndexPrev)
    {
        this->toObject()->observerNotifyWithCustomSender(
            button,
            ZFUIButtonGroup::EventButtonTabOnChange(),
            this->toObject(),
            zflineAlloc(v_zfindex, buttonIndexPrev));
    }
    /** @brief see #EventButtonTabOnClickChecked */
    virtual inline void buttonTabOnClickChecked(ZF_IN ZFUIButton *button,
                                                ZF_IN zfindex buttonIndex)
    {
        this->toObject()->observerNotifyWithCustomSender(
            button,
            ZFUIButtonGroup::EventButtonTabOnClickChecked(),
            this->toObject(),
            zflineAlloc(v_zfindex, buttonIndex));
    }
};

// ============================================================
// ZFUIButtonGroupArray
/**
 * @brief simply button group that only holds buttons
 */
zfclass ZF_ENV_EXPORT ZFUIButtonGroupArray : zfextends ZFObject, zfimplements ZFUIButtonGroup
{
    ZFOBJECT_DECLARE(ZFUIButtonGroupArray, ZFObject)
    ZFIMPLEMENTS_DECLARE(ZFUIButtonGroup)
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIButtonGroup_h_

