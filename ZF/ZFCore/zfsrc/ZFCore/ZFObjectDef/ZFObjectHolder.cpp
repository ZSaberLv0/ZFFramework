#include "ZFObjectHolder.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFOBJECT_REGISTER(ZFObjectHolder)

void ZFObjectHolder::objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
{
    zfsuper::objectInfoOnAppend(ret);
    ret += " ";
    zfsFromPointerT(ret, this->_ZFP_objectHolded.toObject());
}

zfidentity ZFObjectHolder::objectHash(void)
{
    return ZFObjectHash(this->_ZFP_objectHolded.toObject());
}
ZFCompareResult ZFObjectHolder::objectCompare(ZF_IN ZFObject *anotherObj)
{
    if(this == anotherObj) {return ZFCompareTheSame;}
    zfself *another = ZFCastZFObject(zfself *, anotherObj);
    if(another == zfnull) {return ZFCompareUncomparable;}

    return ZFObjectCompare(this->_ZFP_objectHolded.toObject(), another->_ZFP_objectHolded.toObject());
}

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFObjectHolder, void, objectHolded, ZFMP_IN(ZFObject *, obj))

ZF_NAMESPACE_GLOBAL_END
#endif

