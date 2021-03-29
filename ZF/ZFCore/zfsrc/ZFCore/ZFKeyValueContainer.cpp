#include "ZFKeyValueContainer.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFKeyValueContainer)

ZFOBSERVER_EVENT_REGISTER(ZFKeyValueContainer, ContentOnChange)
ZFOBSERVER_EVENT_REGISTER(ZFKeyValueContainer, ContentOnAdd)
ZFOBSERVER_EVENT_REGISTER(ZFKeyValueContainer, ContentOnRemove)

ZFMETHOD_DEFINE_3(ZFKeyValueContainer, void, objectInfoOfContentT,
                  ZFMP_IN_OUT(zfstring &, ret),
                  ZFMP_IN_OPT(zfindex, maxCount, zfindexMax()),
                  ZFMP_IN_OPT(const ZFTokenForKeyValueContainer &, token, ZFTokenForKeyValueContainerDefault()))
{
    zfindex count = 0;
    ret += token.tokenLeft;
    for(zfiterator it = this->iterator(); this->iteratorValid(it) && count < maxCount; ++count, this->iteratorNext(it))
    {
        ZFKeyValuePair pair = this->iteratorPair(it);

        if(count > 0)
        {
            ret += token.tokenSeparator;
        }

        ret += token.tokenPairLeft;
        {
            ret += token.tokenKeyLeft;
            pair.key->objectInfoT(ret);
            ret += token.tokenKeyRight;
        }
        ret += token.tokenPairSeparator;
        {
            ret += token.tokenValueLeft;
            pair.value->objectInfoT(ret);
            ret += token.tokenValueRight;
        }
        ret += token.tokenPairRight;
    }
    if(count < this->count())
    {
        if(count > 0)
        {
            ret += token.tokenSeparator;
        }
        ret += token.tokenEtc;
    }
    ret += token.tokenRight;
}
ZFMETHOD_DEFINE_2(ZFKeyValueContainer, zfstring, objectInfoOfContent,
                  ZFMP_IN_OPT(zfindex, maxCount, zfindexMax()),
                  ZFMP_IN_OPT(const ZFTokenForKeyValueContainer &, token, ZFTokenForKeyValueContainerDefault()))
{
    zfstring ret;
    this->objectInfoOfContentT(ret, maxCount, token);
    return ret;
}

zfbool ZFKeyValueContainer::serializableOnCheck(void)
{
    if(!zfsuperI(ZFSerializable)::serializableOnCheck()) {return zffalse;}

    for(zfiterator it = this->iterator(); this->iteratorValid(it); this->iteratorNext(it))
    {
        if(!ZFObjectIsSerializable(this->iteratorPair(it).key)
            || !ZFObjectIsSerializable(this->iteratorValue(it)))
        {
            return zffalse;
        }
    }

    return zftrue;
}
zfbool ZFKeyValueContainer::serializableOnSerializeFromData(ZF_IN const ZFSerializableData &serializableData,
                                                            ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */,
                                                            ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */)
{
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeFromData(serializableData, outErrorHint, outErrorPos)) {return zffalse;}

    this->removeAll();

    zfautoObject key;
    zfautoObject value;
    for(zfindex i = 0; i < serializableData.elementCount(); ++i)
    {
        const ZFSerializableData &categoryData = serializableData.elementAtIndex(i);
        if(categoryData.resolved()) {continue;}
        const zfchar *category = ZFSerializableUtil::checkCategory(categoryData);
        if(category == zfnull) {continue;}

        if(zfscmpTheSame(category, ZFSerializableKeyword_ZFKeyValueContainer_key))
        {
            if(key != zfnull)
            {
                ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
                    "missing value for key %s (%s)",
                    key.toObject()->objectInfoOfInstance().cString(),
                    key.toObject()->objectInfo().cString());
                return zffalse;
            }
            if(!ZFObjectFromData(key, categoryData, outErrorHint, outErrorPos))
            {
                return zffalse;
            }
            if(key == zfnull)
            {
                ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData, "null key");
                return zffalse;
            }
        }
        else if(zfscmpTheSame(category, ZFSerializableKeyword_ZFKeyValueContainer_value))
        {
            if(key == zfnull)
            {
                ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
                    "missing key");
                return zffalse;
            }
            if(!ZFObjectFromData(value, categoryData, outErrorHint, outErrorPos))
            {
                return zffalse;
            }
            if(value == zfnull)
            {
                ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData, "null value");
                return zffalse;
            }
            this->iteratorAdd(key.toObject(), value.toObject());
            key = zfnull;
            value = zfnull;
        }
    }
    if(key != zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
            "missing value for key %s (%s)",
            key.toObject()->objectInfoOfInstance().cString(),
            key.toObject()->objectInfo().cString());
        return zffalse;
    }

    return zftrue;
}
zfbool ZFKeyValueContainer::serializableOnSerializeToData(ZF_IN_OUT ZFSerializableData &serializableData,
                                                          ZF_IN ZFSerializable *referencedOwnerOrNull,
                                                          ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */)
{
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeToData(serializableData, referencedOwnerOrNull, outErrorHint)) {return zffalse;}
    zfself *ref = ZFCastZFObject(zfself *, referencedOwnerOrNull);

    if(ref == zfnull)
    {
        for(zfiterator it = this->iterator(); this->iteratorValid(it); this->iteratorNext(it))
        {
            ZFKeyValuePair pair = this->iteratorPair(it);

            ZFSerializableData keyData;
            if(!ZFObjectToData(keyData, pair.key, outErrorHint))
            {
                return zffalse;
            }

            ZFSerializableData valueData;
            if(!ZFObjectToData(valueData, pair.value, outErrorHint))
            {
                return zffalse;
            }

            keyData.category(ZFSerializableKeyword_ZFKeyValueContainer_key);
            serializableData.elementAdd(keyData);
            valueData.category(ZFSerializableKeyword_ZFKeyValueContainer_value);
            serializableData.elementAdd(valueData);
        }
    }
    else
    {
        return this->serializableOnSerializeToDataWithRef(serializableData, ref, outErrorHint);
    }

    return zftrue;
}
zfbool ZFKeyValueContainer::serializableOnSerializeToDataWithRef(ZF_IN_OUT ZFSerializableData &serializableData,
                                                                 ZF_IN ZFSerializable *referencedOwnerOrNull,
                                                                 ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */)
{
    ZFKeyValueContainer *ref = ZFCastZFObject(ZFKeyValueContainer *, referencedOwnerOrNull);
    if(ref == zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHint,
            "%s not type of %s",
            referencedOwnerOrNull->toObject()->objectInfoOfInstance().cString(), ZFKeyValueContainer::ClassData()->classNameFull());
        return zffalse;
    }

    if(ref->count() == 0)
    {
        for(zfiterator it = this->iterator(); this->iteratorValid(it); this->iteratorNext(it))
        {
            ZFKeyValuePair pair = this->iteratorPair(it);
            ZFSerializableData keyData;
            if(!ZFObjectToData(keyData, pair.key, outErrorHint))
            {
                return zffalse;
            }
            ZFSerializableData valueData;
            if(!ZFObjectToData(valueData, pair.value, outErrorHint))
            {
                return zffalse;
            }

            keyData.category(ZFSerializableKeyword_ZFKeyValueContainer_key);
            serializableData.elementAdd(keyData);
            valueData.category(ZFSerializableKeyword_ZFKeyValueContainer_value);
            serializableData.elementAdd(valueData);
        }
        return zftrue;
    }

    ZFKeyValueContainer *tmp = this->classData()->newInstance();
    zfblockedRelease(tmp);
    tmp->addFrom(ref);
    for(zfiterator it = this->iterator(); this->iteratorValid(it); this->iteratorNext(it))
    {
        ZFKeyValuePair pair = this->iteratorPair(it);
        zfiterator itTmp = tmp->iteratorFind(pair.key);
        if(ref->iteratorValid(itTmp))
        {
            if(ZFObjectCompare(pair.value, tmp->iteratorValue(itTmp)) == ZFCompareTheSame)
            {
                tmp->iteratorRemove(itTmp);
                continue;
            }
            else
            {
                tmp->iteratorRemove(itTmp);
            }
        }

        ZFSerializableData keyData;
        if(!ZFObjectToData(keyData, pair.key, outErrorHint))
        {
            return zffalse;
        }
        ZFSerializableData valueData;
        if(!ZFObjectToData(valueData, pair.value, outErrorHint))
        {
            return zffalse;
        }

        keyData.category(ZFSerializableKeyword_ZFKeyValueContainer_key);
        serializableData.elementAdd(keyData);
        valueData.category(ZFSerializableKeyword_ZFKeyValueContainer_value);
        serializableData.elementAdd(valueData);
    }

    if(tmp->count() > 0)
    {
        ZFSerializableUtil::errorOccurred(outErrorHint,
            "missing elements from referenced container: %s", tmp->objectInfoOfContent().cString());
        return zffalse;
    }

    return zftrue;
}

void ZFKeyValueContainer::copyableOnCopyFrom(ZF_IN ZFObject *anotherObj)
{
    zfsuperI(ZFCopyable)::copyableOnCopyFrom(anotherObj);
    zfself *another = ZFCastZFObject(zfself *, anotherObj);
    if(another != zfnull && this != another)
    {
        this->removeAll();
        this->addFrom(another);
    }
}

void ZFKeyValueContainer::objectOnDeallocPrepare(void)
{
    this->removeAll();
    zfsuper::objectOnDeallocPrepare();
}

zfidentity ZFKeyValueContainer::objectHash(void)
{
    zfiterator it = this->iterator();
    if(this->iteratorValid(it))
    {
        return (zfidentity)(~((zft_zfidentity)this->count()
            | ((ZFObjectHash(this->iteratorPair(it).key) << 16) & 0x00FF0000)
            | ((ZFObjectHash(this->iteratorValue(it)) << 24) & 0xFF000000)));
    }
    else
    {
        return zfidentityZero();
    }
}
ZFCompareResult ZFKeyValueContainer::objectCompare(ZF_IN ZFObject *anotherObj)
{
    if(this == anotherObj) {return ZFCompareTheSame;}
    zfself *another = ZFCastZFObject(zfself *, anotherObj);
    if(another == zfnull) {return ZFCompareUncomparable;}

    if(this->count() != another->count()
        || this->objectHash() != another->objectHash())
    {
        return ZFCompareUncomparable;
    }

    for(zfiterator it0 = this->iterator(); this->iteratorValid(it0); this->iteratorNext(it0))
    {
        ZFKeyValuePair pair0 = this->iteratorPair(it0);
        zfiterator it1 = another->iteratorFind(pair0.key);
        if(!another->iteratorValid(it1))
        {
            return ZFCompareUncomparable;
        }
        if(ZFObjectCompare(pair0.value, another->iteratorValue(it1)) != ZFCompareTheSame)
        {
            return ZFCompareUncomparable;
        }
        another->iteratorNext(it1);
    }
    return ZFCompareTheSame;
}

ZF_NAMESPACE_GLOBAL_END

