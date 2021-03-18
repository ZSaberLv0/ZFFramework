/**
 * @file ZFUIDialogForChoice.h
 * @brief basic dialog for user to choose items
 */

#ifndef _ZFI_ZFUIDialogForChoice_h_
#define _ZFI_ZFUIDialogForChoice_h_

#include "ZFUIDialogBasic.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief byte order define for ZFFramework
 */
ZFENUM_BEGIN(ZFUIDialogForChoiceMode)
    /**
     * @brief choose only one, click confirm to finish
     */
    ZFENUM_VALUE(ChoiceSingle)
    /**
     * @brief choose only one, single one tap to finish
     */
    ZFENUM_VALUE(ChoiceSingleQuickly)
    /**
     * @brief choose multiple
     */
    ZFENUM_VALUE(ChoiceMultiple)
ZFENUM_SEPARATOR(ZFUIDialogForChoiceMode)
    ZFENUM_VALUE_REGISTER(ChoiceSingle)
    ZFENUM_VALUE_REGISTER(ChoiceSingleQuickly)
    ZFENUM_VALUE_REGISTER(ChoiceMultiple)
ZFENUM_END(ZFUIDialogForChoiceMode)

// ============================================================
// ZFUIDialogForChoiceData
/**
 * @brief choice item data for #ZFUIDialogForChoice
 */
zfclass ZF_ENV_EXPORT ZFUIDialogForChoiceData : zfextends ZFObject, zfimplements ZFSerializable
{
    ZFOBJECT_DECLARE(ZFUIDialogForChoiceData, ZFObject)
    ZFIMPLEMENTS_DECLARE(ZFSerializable)

    /**
     * @brief id of the item
     */
    ZFPROPERTY_ASSIGN(zfstring, choiceId)
    /**
     * @brief name of the item
     */
    ZFPROPERTY_ASSIGN(zfstring, choiceName)
};

// ============================================================
// ZFUIDialogForChoice
zfclassFwd _ZFP_ZFUIDialogForChoicePrivate;
/**
 * @brief basic dialog for input
 */
zfclass ZF_ENV_EXPORT ZFUIDialogForChoice : zfextends ZFUIDialogBasic
{
    ZFOBJECT_DECLARE(ZFUIDialogForChoice, ZFUIDialogBasic)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIDialogForChoice)

    // ============================================================
    // events
public:
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when choice confirmed, either by:
     * -  click choice if choiceMode is #ZFUIDialogForChoiceMode::e_ChoiceSingleQuickly,
     *   implemented by default
     * -  click #ZFUIDialogContent::dialogButtonYes,
     *   implemented by default
     * -  manually notify by #choiceConfirmNotify
     */
    ZFOBSERVER_EVENT(ChoiceOnConfirm)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when choice changed
     */
    ZFOBSERVER_EVENT(ChoiceOnChange)

public:
    /**
     * @brief choice mode, #ZFUIDialogForChoiceMode::EnumDefault by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIDialogForChoiceModeEnum, choiceMode,
                                ZFUIDialogForChoiceMode::EnumDefault())
    ZFPROPERTY_OVERRIDE_ON_ATTACH_INLINE(ZFUIDialogForChoiceModeEnum, choiceMode)
    {
        if(this->choiceMode() != propertyValueOld)
        {
            this->choiceListOnChange();
            this->_ZFP_choiceStateUpdate();
        }
    }

    /**
     * @brief list of choice items, empty by default,
     *   use #choiceAdd/#choiceRemove to access the choice list
     */
    ZFPROPERTY_RETAIN_DETAIL(ZFArrayEditable *, list, zflineAlloc(ZFArrayEditable),
                             private, protected)

    /**
     * @brief list of selected items' index, empty by default,
     *   holds #v_zfindex
     */
    ZFPROPERTY_RETAIN_DETAIL(ZFArrayEditable *, selected, zflineAlloc(ZFArrayEditable),
                             private, protected)

public:
    /** @brief get choice count */
    ZFMETHOD_INLINE_0(zfindex, choiceCount)
    {
        return this->list()->count();
    }
    /** @brief get choice at index */
    ZFMETHOD_INLINE_1(ZFUIDialogForChoiceData *, choiceAtIndex,
                      ZFMP_IN(zfindex, index))
    {
        return this->list()->get<ZFUIDialogForChoiceData *>(index);
    }
    /** @brief get choice id at index */
    ZFMETHOD_INLINE_1(const zfchar *, choiceIdAtIndex,
                      ZFMP_IN(zfindex, index))
    {
        return this->list()->get<ZFUIDialogForChoiceData *>(index)->choiceId();
    }
    /** @brief get choice name at index */
    ZFMETHOD_INLINE_1(const zfchar *, choiceNameAtIndex,
                      ZFMP_IN(zfindex, index))
    {
        return this->list()->get<ZFUIDialogForChoiceData *>(index)->choiceName();
    }
    /**
     * @brief add choice data
     */
    ZFMETHOD_INLINE_1(void, choiceAdd,
                      ZFMP_IN(ZFUIDialogForChoiceData *, choiceData))
    {
        if(choiceData)
        {
            this->list()->add(choiceData);
            this->choiceListOnChange();
            this->_ZFP_choiceStateUpdate();
        }
    }
    /** @brief see #choiceAdd */
    ZFMETHOD_INLINE_2(void, choiceAdd,
                      ZFMP_IN(const zfchar *, choiceId),
                      ZFMP_IN(const zfchar *, choiceName))
    {
        zfblockedAlloc(ZFUIDialogForChoiceData, choiceData);
        choiceData->choiceId(choiceId);
        choiceData->choiceName(choiceName);
        this->choiceAdd(choiceData);
    }
    /** @brief remove choice by id */
    ZFMETHOD_INLINE_1(void, choiceRemove,
                      ZFMP_IN(const zfchar *, choiceId))
    {
        zfindex index = this->_ZFP_indexForChoiceId(choiceId);
        if(index != zfindexMax())
        {
            this->choiceRemoveAtIndex(index);
        }
    }
    /** @brief remove choice at index */
    ZFMETHOD_INLINE_1(void, choiceRemoveAtIndex,
                      ZFMP_IN(zfindex, index))
    {
        this->list()->remove(index);
        for(zfindex i = 0; i < this->selected()->count(); ++i)
        {
            zfindex t = this->selected()->get<v_zfindex *>(i)->zfv;
            if(t == index)
            {
                this->selected()->remove(i);
                --i;
            }
            else if(t > index)
            {
                this->selected()->set(i, zflineAlloc(v_zfindex, t - 1));
            }
        }
        this->choiceListOnChange();
        this->_ZFP_choiceStateUpdate();
    }
    /** @brief remove all choice data */
    ZFMETHOD_INLINE_0(void, choiceRemoveAll)
    {
        this->list()->removeAll();
        this->selected()->removeAll();
        this->choiceListOnChange();
    }

public:
    /** @brief select choice by id, automatically deselect previous for single choice mode */
    ZFMETHOD_INLINE_1(void, choiceSelect,
                      ZFMP_IN(const zfchar *, choiceId))
    {
        this->choiceSelectAtIndex(this->_ZFP_indexForChoiceId(choiceId));
    }
    /** @brief select choice at index, automatically deselect previous for single choice mode */
    ZFMETHOD_INLINE_1(void, choiceSelectAtIndex,
                      ZFMP_IN(zfindex, index))
    {
        if(index >= this->list()->count())
        {
            return ;
        }
        zfblockedAlloc(v_zfindex, indexHolder, index);
        if(!this->selected()->isContain(indexHolder))
        {
            if(this->_ZFP_singleChoiceMode())
            {
                this->selected()->removeAll();
            }
            this->selected()->add(indexHolder);
            if(this->selected()->count() > 1)
            {
                this->selected()->sort();
            }
            this->choiceSelectedListOnChange();
        }
    }
    /** @brief select all choice, or select first for single choice mode */
    ZFMETHOD_INLINE_0(void, choiceSelectAll)
    {
        if(this->_ZFP_singleChoiceMode())
        {
            return ;
        }
        this->selected()->removeAll();
        for(zfindex i = 0; i < this->list()->count(); ++i)
        {
            this->selected()->add(zflineAlloc(v_zfindex, i));
        }
        this->choiceSelectedListOnChange();
    }
    /** @brief deselect choice by id, do nothing for single choice mode */
    ZFMETHOD_INLINE_1(void, choiceDeselect,
                      ZFMP_IN(const zfchar *, choiceId))
    {
        if(this->_ZFP_singleChoiceMode())
        {
            return ;
        }
        this->choiceDeselectAtIndex(this->_ZFP_indexForChoiceId(choiceId));
    }
    /** @brief deselect choice at index, do nothing for single choice mode */
    ZFMETHOD_INLINE_1(void, choiceDeselectAtIndex,
                      ZFMP_IN(zfindex, index))
    {
        if(this->_ZFP_singleChoiceMode())
        {
            return ;
        }
        if(index >= this->list()->count())
        {
            return ;
        }
        if(this->selected()->removeElement(zflineAlloc(v_zfindex, index)))
        {
            this->choiceSelectedListOnChange();
        }
    }
    /** @brief deselect all choice, do nothing for single choice mode */
    ZFMETHOD_INLINE_0(void, choiceDeselectAll)
    {
        if(this->_ZFP_singleChoiceMode())
        {
            return ;
        }
        if(!this->selected()->isEmpty())
        {
            this->selected()->removeAll();
            this->choiceSelectedListOnChange();
        }
    }

public:
    /**
     * @brief true if choice selected
     */
    ZFMETHOD_INLINE_1(zfbool, choiceSelected,
                      ZFMP_IN(const zfchar *, choiceId))
    {
        zfindex index = this->_ZFP_indexForChoiceId(choiceId);
        if(index != zfindexMax())
        {
            return this->selected()->isContain(zflineAlloc(v_zfindex, index));
        }
        else
        {
            return zffalse;
        }
    }
    /**
     * @brief true if choice selected
     */
    ZFMETHOD_INLINE_1(zfbool, choiceSelectedAtIndex,
                      ZFMP_IN(zfindex, index))
    {
        return this->selected()->isContain(zflineAlloc(v_zfindex, index));
    }
    /**
     * @brief return all selected choice index
     */
    ZFMETHOD_INLINE_0(ZFCoreArrayPOD<zfindex>, choiceSelectedIndexList)
    {
        ZFCoreArrayPOD<zfindex> ret;
        ret.capacity(this->selected()->count());
        for(zfindex i = 0; i < this->selected()->count(); ++i)
        {
            zfindex index = this->selected()->get<v_zfindex *>(i)->zfv;
            ret.add(index);
        }
        return ret;
    }
    /**
     * @brief return all selected choice id
     */
    ZFMETHOD_INLINE_0(ZFCoreArray<zfstring>, choiceSelectedIdList)
    {
        ZFCoreArray<zfstring> ret;
        ret.capacity(this->selected()->count());
        for(zfindex i = 0; i < this->selected()->count(); ++i)
        {
            zfindex index = this->selected()->get<v_zfindex *>(i)->zfv;
            ret.add(this->choiceIdAtIndex(index));
        }
        return ret;
    }
    /**
     * @brief return all selected choice name
     */
    ZFMETHOD_INLINE_0(ZFCoreArray<zfstring>, choiceSelectedNameList)
    {
        ZFCoreArray<zfstring> ret;
        ret.capacity(this->selected()->count());
        for(zfindex i = 0; i < this->selected()->count(); ++i)
        {
            zfindex index = this->selected()->get<v_zfindex *>(i)->zfv;
            ret.add(this->choiceNameAtIndex(index));
        }
        return ret;
    }

protected:
    /**
     * @brief called when choice list changed
     */
    virtual void choiceListOnChange(void);
    /**
     * @brief called when selected index list changed
     */
    virtual void choiceSelectedListOnChange(void);

private:
    zffinal void _ZFP_choiceStateUpdate(void)
    {
        switch(this->choiceMode())
        {
            case ZFUIDialogForChoiceMode::e_ChoiceSingle:
            case ZFUIDialogForChoiceMode::e_ChoiceSingleQuickly:
                if(this->selected()->count() > 1)
                {
                    this->selected()->remove(1, zfindexMax());
                    this->choiceSelectedListOnChange();
                }
                else if(this->selected()->count() == 0 && !this->list()->isEmpty())
                {
                    this->selected()->add(zflineAlloc(v_zfindex, 0));
                    this->choiceSelectedListOnChange();
                }
                break;
            case ZFUIDialogForChoiceMode::e_ChoiceMultiple:
                this->selected()->sort();
                this->choiceSelectedListOnChange();
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                break;
        }
    }
    zffinal zfindex _ZFP_indexForChoiceId(ZF_IN const zfchar *choiceId)
    {
        for(zfindex i = 0; i < this->list()->count(); ++i)
        {
            if(zfscmpTheSame(this->list()->get<ZFUIDialogForChoiceData *>(i)->choiceId().cString(), choiceId))
            {
                return i;
            }
        }
        return zfindexMax();
    }
    zffinal zfbool _ZFP_singleChoiceMode(void)
    {
        switch(this->choiceMode())
        {
            case ZFUIDialogForChoiceMode::e_ChoiceSingle:
            case ZFUIDialogForChoiceMode::e_ChoiceSingleQuickly:
                return zftrue;
            case ZFUIDialogForChoiceMode::e_ChoiceMultiple:
                return zffalse;
            default:
                zfCoreCriticalShouldNotGoHere();
                return zffalse;
        }
    }

protected:
    /** @brief see #EventChoiceOnConfirm */
    virtual inline void choiceOnConfirm(void)
    {
        this->observerNotify(ZFUIDialogForChoice::EventChoiceOnConfirm());
    }
    /** @brief see #EventChoiceOnChange */
    virtual inline void choiceOnChange(void)
    {
        this->observerNotify(ZFUIDialogForChoice::EventChoiceOnChange());
    }

public:
    /**
     * @brief manually notify confirm the choice
     *
     * this method is useful if you have addition confirm button or action
     * to finish the choice, see #EventChoiceOnConfirm
     */
    ZFMETHOD_INLINE_0(void, choiceConfirmNotify)
    {
        this->choiceOnConfirm();
    }
    /**
     * @brief manually notify confirm the choice
     *
     * this method is useful if you have addition change button or action
     * to change the choice, see #EventChoiceOnChange
     */
    ZFMETHOD_INLINE_0(void, choiceChangeNotify)
    {
        this->choiceOnChange();
    }

public:
    zffinal void _ZFP_ZFUIDialogForChoice_choiceChange(ZF_IN zfindex index, ZF_IN zfbool selected)
    {
        this->selected()->removeElement(zflineAlloc(v_zfindex, index));
        if(selected)
        {
            this->selected()->add(zflineAlloc(v_zfindex, index));
            this->selected()->sort();
        }
    }

protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);

    zfoverride
    virtual zfbool serializableOnSerializeFromData(ZF_IN const ZFSerializableData &serializableData,
                                                   ZF_OUT_OPT zfstring *outErrorHint = zfnull,
                                                   ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull)
    {
        if(!zfsuper::serializableOnSerializeFromData(serializableData, outErrorHint, outErrorPos))
        {
            return zffalse;
        }
        this->choiceListOnChange();
        this->choiceSelectedListOnChange();
        return zftrue;
    }
    zfoverride
    virtual void styleableOnCopyFrom(ZF_IN ZFStyleable *anotherStyleable)
    {
        zfsuper::styleableOnCopyFrom(anotherStyleable);
        this->choiceListOnChange();
        this->choiceSelectedListOnChange();
    }

    zfoverride
    virtual void dialogButtonOnAdd(ZF_IN ZFUIButton *button);
    zfoverride
    virtual void dialogButtonOnRemove(ZF_IN ZFUIButton *button);

private:
    _ZFP_ZFUIDialogForChoicePrivate *d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIDialogForChoice_h_

