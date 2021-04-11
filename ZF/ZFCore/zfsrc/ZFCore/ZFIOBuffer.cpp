#include "ZFIOBuffer.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFIOBuffer)

ZFMETHOD_DEFINE_0(ZFIOBuffer, ZFInput, input)
{
    ZFInput ret = this->implInput();
    if(ret.callbackIsValid())
    {
        ret.callbackTag(ZFCallbackTagKeyword_ZFIOBuffer, this);
    }
    return ret;
}
ZFMETHOD_DEFINE_0(ZFIOBuffer, ZFOutput, output)
{
    ZFInput ret = this->implOutput();
    if(ret.callbackIsValid())
    {
        ret.callbackTag(ZFCallbackTagKeyword_ZFIOBuffer, this);
    }
    return ret;
}
ZFMETHOD_DEFINE_0(ZFIOBuffer, void, removeAll)
{
    this->implRemoveAll();
}

ZF_NAMESPACE_GLOBAL_END

