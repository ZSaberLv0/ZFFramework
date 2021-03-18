#include "ZFObjectIO_zfsd.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECTIO_DEFINE(zfsd, {
        return ZFObjectIOImplCheck(pathInfo, "zfsd");
    }, {
        return ZFObjectFromZfsd(ret, input, outErrorHint);
    }, {
        return ZFObjectToZfsd(output, obj, outErrorHint);
    })

ZF_NAMESPACE_GLOBAL_END

