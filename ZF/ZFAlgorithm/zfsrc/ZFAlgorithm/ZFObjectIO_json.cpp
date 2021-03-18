#include "ZFObjectIO_json.h"
#include "ZFJsonSerializableConverter.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECTIO_DEFINE(json, {
        return ZFObjectIOImplCheck(pathInfo, "json");
    }, {
        return ZFObjectFromJson(ret, input, outErrorHint);
    }, {
        return ZFObjectToJson(output, obj, outErrorHint);
    })

ZF_NAMESPACE_GLOBAL_END

