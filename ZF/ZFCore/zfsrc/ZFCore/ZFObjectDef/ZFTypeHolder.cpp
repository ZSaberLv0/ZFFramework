#include "ZFTypeHolder.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFTypeHolder
ZFOBJECT_REGISTER(ZFTypeHolder)

void ZFTypeHolder::objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
{
    zfsuper::objectInfoOnAppend(ret);
    ret += " ";
    zfsFromPointerT(ret, this->holdedData);
}
zfidentity ZFTypeHolder::objectHash(void)
{
    return zfidentityHash(
        zfidentityCalcPointer(this->holdedData),
        zfidentityCalcPointer(ZFCastReinterpret(void *, this->holderType)));
}
ZFCompareResult ZFTypeHolder::objectCompare(ZF_IN ZFObject *anotherObj)
{
    if(this == anotherObj) {return ZFCompareTheSame;}
    zfself *another = ZFCastZFObject(zfself *, anotherObj);
    if(another == zfnull) {return ZFCompareUncomparable;}

    if(this->holdedData == another->holdedData
        && this->holderType == another->holderType)
    {
        return ZFCompareTheSame;
    }
    else
    {
        return ZFCompareUncomparable;
    }
}

ZF_NAMESPACE_GLOBAL_END

