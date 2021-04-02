#include "ZFCopyable.h"
#include "ZFObjectImpl.h"
#include "ZFSerializable.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFCopyable)

zfautoObjectT<ZFCopyable *> ZFCopyable::copy(void)
{
    zfautoObjectT<ZFCopyable *> ret = this->classData()->newInstance();
    if(ret != zfnull)
    {
        ZFCastZFObjectUnchecked(zfself *, ret.toObject())->copyableOnCopyFrom(this->toObject());
    }
    return ret;
}

void ZFCopyable::copyFrom(ZF_IN ZFObject *anotherObj)
{
    if(anotherObj != zfnull && anotherObj != this->toObject() && anotherObj->classData() == this->classData())
    {
        this->copyableOnCopyFrom(anotherObj);
    }
}

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFCopyable, zfautoObjectT<ZFCopyable *>, copy)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFCopyable, void, copyFrom, ZFMP_IN(ZFObject *, anotherObj))

ZF_NAMESPACE_GLOBAL_END
#endif

