#include "ZFKeyValuePair.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFEXPORT_VAR_READONLY_DEFINE(ZFKeyValuePair, ZFKeyValuePairZero, ZFKeyValuePairMake(zfnull, zfnull))

// ============================================================
ZFTYPEID_DEFINE_BY_SERIALIZABLE_CONVERTER(ZFKeyValuePairHolder, ZFKeyValuePairHolder, {
        const ZFSerializableData *data = zfnull;
        data = ZFSerializableUtil::checkElementByCategory(serializableData, ZFSerializableKeyword_ZFKeyValuePair_key);
        if(data != zfnull)
        {
            if(!ZFObjectFromData(v.key, *data, outErrorHint, outErrorPos))
            {
                return zffalse;
            }
        }
        else
        {
            v.key = zfnull;
        }
        data = ZFSerializableUtil::checkElementByCategory(serializableData, ZFSerializableKeyword_ZFKeyValuePair_value);
        if(data != zfnull)
        {
            if(!ZFObjectFromData(v.value, *data, outErrorHint, outErrorPos))
            {
                return zffalse;
            }
        }
        else
        {
            v.value = zfnull;
        }
        return zftrue;
    }, {
        if(v.key != zfnull)
        {
            ZFSerializableData data;
            if(!ZFObjectToData(data, v.key, outErrorHint))
            {
                return zffalse;
            }
            data.category(ZFSerializableKeyword_ZFKeyValuePair_key);
            serializableData.elementAdd(data);
        }
        if(v.value != zfnull)
        {
            ZFSerializableData data;
            if(!ZFObjectToData(data, v.value, outErrorHint))
            {
                return zffalse;
            }
            data.category(ZFSerializableKeyword_ZFKeyValuePair_value);
            serializableData.elementAdd(data);
        }
        return zftrue;
    })
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFKeyValuePairHolder, zfautoObject, key)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFKeyValuePairHolder, zfautoObject, value)

ZFTYPEID_ALIAS_DEFINE(ZFKeyValuePairHolder, ZFKeyValuePairHolder, ZFKeyValuePair, ZFKeyValuePair)

ZF_NAMESPACE_GLOBAL_END

