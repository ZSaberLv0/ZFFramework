#include "ZFUIDialogBasic.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFENUM_DEFINE(ZFUIDialogButtonType)

ZFOBJECT_REGISTER(ZFUIDialogContent)

ZFOBSERVER_EVENT_REGISTER(ZFUIDialogContent, DialogButtonOnAdd)
ZFOBSERVER_EVENT_REGISTER(ZFUIDialogContent, DialogButtonOnRemove)

static const ZFClass *_ZFP_ZFUIDialogContentClass = zfnull;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIDialogContentClassInit, ZFLevelZFFrameworkEssential)
{
    _ZFP_ZFUIDialogContentClass = ZFUIDialogContentBasic::ClassData();
}
ZF_GLOBAL_INITIALIZER_END(ZFUIDialogContentClassInit)

ZFMETHOD_FUNC_DEFINE_1(void, ZFUIDialogContentClass,
                       ZFMP_IN(const ZFClass *, cls))
{
    if(cls == zfnull)
    {
        _ZFP_ZFUIDialogContentClass = ZFUIDialogContentBasic::ClassData();
    }
    else if(!cls->classIsTypeOf(ZFUIDialogContent::ClassData()))
    {
        zfCoreCriticalClassNotTypeOf(cls, ZFUIDialogContent::ClassData());
        return ;
    }
    else
    {
        _ZFP_ZFUIDialogContentClass = cls;
    }
}
ZFMETHOD_FUNC_DEFINE_0(const ZFClass *, ZFUIDialogContentClass)
{
    return _ZFP_ZFUIDialogContentClass;
}

ZFSTYLE_DEFAULT_DEFINE(ZFUIDialogBasic)

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIDialogBasicDataHolder, ZFLevelZFFrameworkEssential)
{
    this->dialogButtonOnAddListener = ZFCallbackForFunc(zfself::dialogButtonOnAdd);
    this->dialogButtonOnRemoveListener = ZFCallbackForFunc(zfself::dialogButtonOnRemove);
}
public:
    ZFListener dialogButtonOnAddListener;
    ZFListener dialogButtonOnRemoveListener;
    static ZFLISTENER_PROTOTYPE_EXPAND(dialogButtonOnAdd)
    {
        userData->objectHolded<ZFUIDialogBasic *>()
            ->_ZFP_ZFUIDialogBasic_dialogButtonOnAdd(listenerData.param0<ZFUIButton *>());
    }
    static ZFLISTENER_PROTOTYPE_EXPAND(dialogButtonOnRemove)
    {
        userData->objectHolded<ZFUIDialogBasic *>()
            ->_ZFP_ZFUIDialogBasic_dialogButtonOnRemove(listenerData.param0<ZFUIButton *>());
    }
ZF_GLOBAL_INITIALIZER_END(ZFUIDialogBasicDataHolder)

ZFPROPERTY_OVERRIDE_ON_VERIFY_DEFINE(ZFUIDialogBasic, ZFUIDialogContent *, dialogContent)
{
    if(propertyValueOld == zfnull)
    {
        zfCoreCriticalMessage("change dialogContent to null is not allowed");
        return ;
    }
    else if(!propertyValueOld.toObject()->classData()->classIsTypeOf(ZFUIDialogContent::ClassData()))
    {
        zfCoreCriticalClassNotTypeOf(propertyValue.toObject()->classData(), ZFUIDialogContent::ClassData());
        return ;
    }
}
ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUIDialogBasic, ZFUIDialogContent *, dialogContent)
{
    this->dialogInternalContainer()->childAdd(this->dialogContent()->to<ZFUIView *>());
    this->dialogContent()->to<ZFUIView *>()->layoutParam()->layoutAlign(ZFUIAlign::e_Center);

    this->dialogContent()->toObject()->observerAdd(
        ZFUIDialogContent::EventDialogButtonOnAdd(),
        ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIDialogBasicDataHolder)->dialogButtonOnAddListener,
        this->objectHolder());
    this->dialogContent()->toObject()->observerAdd(
        ZFUIDialogContent::EventDialogButtonOnRemove(),
        ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIDialogBasicDataHolder)->dialogButtonOnRemoveListener,
        this->objectHolder());
}
ZFPROPERTY_OVERRIDE_ON_DETACH_DEFINE(ZFUIDialogBasic, ZFUIDialogContent *, dialogContent)
{
    if(this->dialogContent() != zfnull)
    {
        this->dialogContent()->toObject()->observerRemove(
            ZFUIDialogContent::EventDialogButtonOnAdd(),
            ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIDialogBasicDataHolder)->dialogButtonOnAddListener);
        this->dialogContent()->toObject()->observerRemove(
            ZFUIDialogContent::EventDialogButtonOnRemove(),
            ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIDialogBasicDataHolder)->dialogButtonOnRemoveListener);
        this->dialogContent()->to<ZFUIView *>()->viewRemoveFromParent();
    }
}

void ZFUIDialogBasic::objectOnInit(void)
{
    zfsuper::objectOnInit();
    this->dialogContent(this->dialogContent());
}
void ZFUIDialogBasic::objectOnDeallocPrepare(void)
{
    this->dialogContent()->dialogButtonRemoveAll();
    this->dialogContent()->dialogButtonRemove(ZFUIDialogButtonType::e_Yes);
    this->dialogContent()->dialogButtonRemove(ZFUIDialogButtonType::e_No);
    this->dialogContent()->dialogButtonRemove(ZFUIDialogButtonType::e_Cancel);
    this->dialogContent()->dialogButtonRemove(ZFUIDialogButtonType::e_Destructive);

    zfsuper::objectOnDeallocPrepare();
}
void ZFUIDialogBasic::objectOnDealloc(void)
{
    this->dialogContent()->toObject()->observerRemove(
        ZFUIDialogContent::EventDialogButtonOnAdd(),
        ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIDialogBasicDataHolder)->dialogButtonOnAddListener);
    this->dialogContent()->toObject()->observerRemove(
        ZFUIDialogContent::EventDialogButtonOnRemove(),
        ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIDialogBasicDataHolder)->dialogButtonOnRemoveListener);

    zfsuper::objectOnDealloc();
}

ZF_NAMESPACE_GLOBAL_END

