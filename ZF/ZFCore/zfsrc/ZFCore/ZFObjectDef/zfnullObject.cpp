#include "zfnullObject.h"
#include "ZFObjectImpl.h"
#include "ZFCopyable.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/** @brief see #zfnullObject */
zfclass ZFNull : zfextends ZFObject, zfimplements ZFSerializable, zfimplements ZFCopyable
{
    ZFOBJECT_DECLARE(ZFNull, ZFObject)
    ZFIMPLEMENTS_DECLARE(ZFSerializable, ZFCopyable)

public:
    zfoverride
    virtual void objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
    {
        ret += "zfnullObject";
    }
    zfoverride
    virtual zfidentity objectHash(void)
    {
        return zfidentityInvalid();
    }
    zfoverride
    virtual ZFCompareResult objectCompare(ZF_IN ZFObject *anotherObj)
    {
        if(this == anotherObj || (anotherObj != zfnull && anotherObj->classData() == ZFNull::ClassData()))
        {
            return ZFCompareTheSame;
        }
        return ZFCompareUncomparable;
    }
    zfoverride
    virtual void objectTag(ZF_IN const zfchar *key, ZF_IN ZFObject *tag)
    {
        zfCoreCriticalMessageTrim("setting tag to zfnullObject is not allowed, key: %s", key);
    }

private:
    ZFMETHOD_INLINE_DETAIL_0(private, ZFMethodTypeStatic, ZF_CALLER_LINE,
                             zfautoObject, ZFSerializableKeyword_serializableNewInstanceId)
    {
        return zfnullObject();
    }

public:
    zfoverride
    virtual zfautoObject copy(void)
    {
        return zfnullObject();
    }
};

ZFOBJECT_REGISTER(ZFNull)

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(zfnullObjHolder, ZFLevelZFFrameworkEssential)
{
    this->nullObject = zfAlloc(ZFNull);
}
ZF_GLOBAL_INITIALIZER_DESTROY(zfnullObjHolder)
{
    zfRelease(this->nullObject);
    this->nullObject = zfnull;
}
public:
    ZFObject *nullObject;
ZF_GLOBAL_INITIALIZER_END(zfnullObjHolder)
ZFObject *_ZFP_zfnullObject(void)
{
    return ZF_GLOBAL_INITIALIZER_INSTANCE(zfnullObjHolder)->nullObject;
}

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_USER_REGISTER_0({return zfnullObject();}, ZFObject *, zfnullObject)

ZF_NAMESPACE_GLOBAL_END
#endif

