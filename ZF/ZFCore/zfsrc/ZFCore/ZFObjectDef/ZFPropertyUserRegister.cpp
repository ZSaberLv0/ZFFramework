#include "ZFPropertyUserRegister.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfautoObject _ZFP_I_PropURDIVH::create(ZF_IN void *v,
                                       ZF_IN DeleteCallback deleteCallback,
                                       ZF_IN_OPT ZFObject *retainValue /* = zfnull */)
{
    zfblockedAlloc(_ZFP_I_PropURDIVH, holder);
    holder->v = v;
    holder->deleteCallback = deleteCallback;
    holder->retainValueHolder = retainValue;
    return holder;
}

void ZFPropertyUserUnregister(ZF_IN const ZFProperty *zfproperty)
{
    if(zfproperty == zfnull)
    {
        return ;
    }
    zfCoreMutexLocker();
    zfCoreAssertWithMessageTrim(zfproperty->propertyIsUserRegister(),
            "[ZFPropertyUserUnregister] property %s is not user registered",
            zfproperty->objectInfo().cString()
        );

    _ZFP_ZFPropertyUnregister(zfproperty);
}

void ZFPropertyUserRegisterNotifyUpdate(ZF_IN ZFObject *ownerObject,
                                        ZF_IN const ZFProperty *property,
                                        ZF_IN const void *propertyValueOld)
{
    ownerObject->_ZFP_ZFObject_objectPropertyValueOnUpdate(property, propertyValueOld);
}

ZF_NAMESPACE_GLOBAL_END

