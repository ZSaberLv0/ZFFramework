#include "ZFUIDialogForChoice.h"

#include "ZFUIListView.h"
#include "ZFUIListAdapterArray.h"
#include "ZFUIButtonGroup.h"
#include "ZFUIButtonCheckBox.h"
#include "ZFUIButtonRatio.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFENUM_DEFINE(ZFUIDialogForChoiceMode)
ZFSTYLE_DEFAULT_DEFINE(ZFUIDialogForChoice)

ZFOBSERVER_EVENT_REGISTER(ZFUIDialogForChoice, ChoiceOnConfirm)
ZFOBSERVER_EVENT_REGISTER(ZFUIDialogForChoice, ChoiceOnChange)

// ============================================================
zfclass _ZFP_ZFUIDialogForChoicePrivate : zfextends ZFUIListView
{
    ZFOBJECT_DECLARE(_ZFP_ZFUIDialogForChoicePrivate, ZFUIListView)

public:
    ZFUIDialogForChoice *pimplOwner;
    ZFUIListAdapterArray *listAdapterValue;
    ZFListener updateTaskListener;
    zfidentity updateTaskId;
    ZFListener buttonEventListener;
    ZFUIButtonGroupArray *buttonGroup;
    ZFUISize measuredSize;

public:
    ZFMETHOD_INLINE_2(void, updateTask,
                      ZFMP_IN(const ZFListenerData &, listenerData),
                      ZFMP_IN(ZFObject *, userData))
    {
        this->buttonGroup->observerRemove(ZFUIButton::EventButtonOnClick(), this->buttonEventListener);
        this->updateTaskId = zfidentityInvalid();
        this->listAdapterValue->cellRemoveAll();
        this->buttonGroup->buttonRemoveAll();
        switch(this->pimplOwner->choiceMode())
        {
            case ZFUIDialogForChoiceMode::e_ChoiceSingle:
                this->updateForChoiceSingle();
                break;
            case ZFUIDialogForChoiceMode::e_ChoiceSingleQuickly:
                this->updateForChoiceSingle();
                break;
            case ZFUIDialogForChoiceMode::e_ChoiceMultiple:
                this->updateForChoiceMultiple();
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                break;
        }
        this->buttonGroup->observerAdd(ZFUIButton::EventButtonOnClick(), this->buttonEventListener);
        this->updateListSize();
        this->listReload();
    }


protected:
    virtual void layoutOnMeasure(ZF_OUT ZFUISize &ret,
                                 ZF_IN const ZFUISize &sizeHint,
                                 ZF_IN const ZFUISizeParam &sizeParam)
    {
        ret = this->measuredSize;
    }

private:
    void updateListSize(void)
    {
        zffloat cellSizeHint = this->listAdapterValue->cellSizeHint();
        ZFUISize sizeHint = ZFUISizeMake(ZFUIGlobalStyle::DefaultStyle()->itemSizeDialogWidth(), cellSizeHint);
        ZFUISize size = sizeHint;
        size.height = 0;
        for(zfindex i = 0; i < this->buttonGroup->buttonCount(); ++i)
        {
            ZFUIButton *button = this->buttonGroup->buttonAtIndex(i);
            button->layoutMeasure(sizeHint, ZFUISizeParamWrapWrap());
            if(size.width < button->layoutMeasuredSize().width)
            {
                size.width = button->layoutMeasuredSize().width;
            }
            size.height += zfmMax(cellSizeHint, button->layoutMeasuredSize().height);
        }
        if(size.height == 0)
        {
            size.height = sizeHint.height;
        }
        this->measuredSize = size;
    }

private:
    void updateForChoiceSingle(void)
    {
        this->buttonGroup->buttonGroupType(ZFUIButtonGroupType::e_Tab);
        for(zfindex i = 0; i < this->pimplOwner->choiceCount(); ++i)
        {
            zfblockedAlloc(ZFUIListCell, cell);
            this->listAdapterValue->cellAdd(cell);

            zfblockedAlloc(ZFUIButtonRatio, button);
            cell->cellView(button);
            this->buttonGroup->buttonAdd(button);

            button->buttonLabelText(this->pimplOwner->choiceNameAtIndex(i));
        }
        this->buttonGroup->buttonTabChecked(this->pimplOwner->choiceSelectedIndexList().getFirst());
    }
    void updateForChoiceMultiple(void)
    {
        this->buttonGroup->buttonGroupType(ZFUIButtonGroupType::e_Normal);
        for(zfindex i = 0; i < this->pimplOwner->choiceCount(); ++i)
        {
            zfblockedAlloc(ZFUIListCell, cell);
            this->listAdapterValue->cellAdd(cell);

            zfblockedAlloc(ZFUIButtonCheckBox, button);
            cell->cellView(button);
            this->buttonGroup->buttonAdd(button);

            button->buttonLabelText(this->pimplOwner->choiceNameAtIndex(i));
        }
        ZFCoreArrayPOD<zfindex> selectedIndexList = this->pimplOwner->choiceSelectedIndexList();
        for(zfindex i = 0; i < selectedIndexList.count(); ++i)
        {
            this->buttonGroup->buttonAtIndex(selectedIndexList[i])->buttonChecked(zftrue);
        }
    }

private:
    ZFMETHOD_INLINE_2(void, buttonEvent,
                      ZFMP_IN(const ZFListenerData &, listenerData),
                      ZFMP_IN(ZFObject *, userData))
    {
        zfindex index = listenerData.param1<v_zfindex *>()->zfv;
        switch(this->pimplOwner->choiceMode())
        {
            case ZFUIDialogForChoiceMode::e_ChoiceSingle:
                this->pimplOwner->_ZFP_ZFUIDialogForChoice_choiceChange(this->pimplOwner->choiceSelectedIndexList().getFirst(), zffalse);
                this->pimplOwner->_ZFP_ZFUIDialogForChoice_choiceChange(index, zftrue);
                this->pimplOwner->choiceChangeNotify();
                break;
            case ZFUIDialogForChoiceMode::e_ChoiceSingleQuickly:
                this->pimplOwner->_ZFP_ZFUIDialogForChoice_choiceChange(this->pimplOwner->choiceSelectedIndexList().getFirst(), zffalse);
                this->pimplOwner->_ZFP_ZFUIDialogForChoice_choiceChange(index, zftrue);
                this->pimplOwner->choiceChangeNotify();
                this->pimplOwner->choiceConfirmNotify();
                break;
            case ZFUIDialogForChoiceMode::e_ChoiceMultiple:
            {
                ZFUIButton *button = listenerData.sender<ZFUIButton *>();
                this->pimplOwner->_ZFP_ZFUIDialogForChoice_choiceChange(index, button->buttonChecked());
                this->pimplOwner->choiceChangeNotify();
            }
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                break;
        }
    }

protected:
    zfoverride
    virtual void objectOnInit(void)
    {
        zfsuper::objectOnInit();

        this->listBounce(zffalse);

        this->listAdapterValue = zfAlloc(ZFUIListAdapterArray);
        this->listAdapter(this->listAdapterValue);

        this->updateTaskListener = ZFCallbackForMemberMethod(this, ZFMethodAccess(zfself, updateTask));
        this->updateTaskId = zfidentityInvalid();

        this->buttonEventListener = ZFCallbackForMemberMethod(this, ZFMethodAccess(zfself, buttonEvent));
        this->buttonGroup = zfAlloc(ZFUIButtonGroupArray);
        this->buttonGroup->observerAdd(ZFUIButton::EventButtonOnClick(), this->buttonEventListener);

        this->updateListSize();
    }
    zfoverride
    virtual void objectOnDealloc(void)
    {
        this->listAdapter(zfnull);
        zfRelease(this->listAdapterValue);
        this->listAdapterValue = zfnull;

        zfRelease(this->buttonGroup);
        this->buttonGroup = zfnull;

        zfsuper::objectOnDealloc();
    }
};

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIDialogForChoiceDataHolder, ZFLevelZFFrameworkEssential)
{
    this->dialogOnConfirmListener = ZFCallbackForFunc(zfself::dialogOnConfirm);
}
public:
    ZFListener dialogOnConfirmListener;
    static void dialogOnConfirm(ZF_IN const ZFListenerData &listenerData, ZF_IN ZFObject *userData)
    {
        userData->objectHolded<ZFUIDialogForChoice *>()->choiceConfirmNotify();
    }
ZF_GLOBAL_INITIALIZER_END(ZFUIDialogForChoiceDataHolder)

void ZFUIDialogForChoice::objectOnInit(void)
{
    zfsuper::objectOnInit();

    d = zfAlloc(_ZFP_ZFUIDialogForChoicePrivate);
    d->pimplOwner = this;
    this->dialogContentContainer()->childAdd(d);
}
void ZFUIDialogForChoice::objectOnDealloc(void)
{
    ZFThreadTaskCancel(d->updateTaskId);
    d->viewRemoveFromParent();
    zfRelease(d);
    zfsuper::objectOnDealloc();
}

void ZFUIDialogForChoice::dialogButtonOnAdd(ZF_IN ZFUIButton *button)
{
    zfsuper::dialogButtonOnAdd(button);
    if(button == this->dialogButtonYes(zfHint("autoCreateIfNotExist")zffalse))
    {
        button->observerAdd(
            ZFUIButton::EventButtonOnClick(),
            ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIDialogForChoiceDataHolder)->dialogOnConfirmListener,
            this->objectHolder());
    }
}
void ZFUIDialogForChoice::dialogButtonOnRemove(ZF_IN ZFUIButton *button)
{
    if(button == this->dialogButtonYes(zfHint("autoCreateIfNotExist")zffalse))
    {
        button->observerRemove(
            ZFUIButton::EventButtonOnClick(),
            ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIDialogForChoiceDataHolder)->dialogOnConfirmListener);
    }
    zfsuper::dialogButtonOnRemove(button);
}

void ZFUIDialogForChoice::choiceListOnChange(void)
{
    zfblockedAlloc(ZFThreadTaskRequestData, taskRequestData);
    taskRequestData->taskCallback(d->updateTaskListener);
    d->updateTaskId = ZFThreadTaskRequest(taskRequestData, ZFThreadTaskRequestMergeCallbackIgnoreOldTask());
}
void ZFUIDialogForChoice::choiceSelectedListOnChange(void)
{
    zfblockedAlloc(ZFThreadTaskRequestData, taskRequestData);
    taskRequestData->taskCallback(d->updateTaskListener);
    d->updateTaskId = ZFThreadTaskRequest(taskRequestData, ZFThreadTaskRequestMergeCallbackIgnoreOldTask());

    this->choiceChangeNotify();
}

ZF_NAMESPACE_GLOBAL_END

