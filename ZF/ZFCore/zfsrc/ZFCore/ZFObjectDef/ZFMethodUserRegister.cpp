#include "ZFMethodUserRegister.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

void ZFMethodUserUnregister(ZF_IN const ZFMethod *method)
{
    if(method == zfnull)
    {
        return ;
    }
    zfCoreMutexLocker();
    zfCoreAssertWithMessageTrim(method->methodIsUserRegister(),
            "[ZFMethodUserUnregister] method %s is not user registered",
            method->objectInfo().cString()
        );
    zfCoreAssertWithMessageTrim(!method->methodIsFunctionType(),
            "[ZFMethodUserUnregister] method %s is not class member type",
            method->objectInfo().cString()
        );

    _ZFP_ZFMethodUnregister(method);
}

ZF_NAMESPACE_GLOBAL_END

