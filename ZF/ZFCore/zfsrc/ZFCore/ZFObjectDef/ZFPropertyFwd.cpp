#include "ZFPropertyFwd.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

void ZFPropertyCallbackIsValueAccessedChange(ZF_IN const ZFProperty *property,
                                             ZF_IN ZFPropertyCallbackIsValueAccessed callback)
{
    zfCoreAssert(property != zfnull && callback != zfnull);
    property->_ZFP_ZFProperty_removeConst()->callbackIsValueAccessed = callback;
}
void ZFPropertyCallbackIsInitValueChange(ZF_IN const ZFProperty *property,
                                         ZF_IN ZFPropertyCallbackIsInitValue callback)
{
    zfCoreAssert(property != zfnull && callback != zfnull);
    property->_ZFP_ZFProperty_removeConst()->callbackIsInitValue = callback;
}
void ZFPropertyCallbackValueResetChange(ZF_IN const ZFProperty *property,
                                        ZF_IN ZFPropertyCallbackValueReset callback)
{
    zfCoreAssert(property != zfnull && callback != zfnull);
    property->_ZFP_ZFProperty_removeConst()->callbackValueReset = callback;
}

ZF_NAMESPACE_GLOBAL_END

