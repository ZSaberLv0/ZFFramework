#include "ZFObjectIO_image.h"

#include "ZFCore/ZFSTLWrapper/zfstl_map.h"
#include "ZFCore/ZFSTLWrapper/zfstl_string.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
static zfstlmap<zfstlstringZ, zfbool> &_ZFP_ZFObjectIO_image_imageExtMap(void)
{
    static zfstlmap<zfstlstringZ, zfbool> m;
    return m;
}

void ZFObjectIO_image_imageExtAdd(ZF_IN const zfchar *imageExt)
{
    zfCoreAssert(!zfsIsEmpty(imageExt));
    _ZFP_ZFObjectIO_image_imageExtMap()[imageExt] = zftrue;
}
void ZFObjectIO_image_imageExtRemove(ZF_IN const zfchar *imageExt)
{
    _ZFP_ZFObjectIO_image_imageExtMap().erase(imageExt);
}
void ZFObjectIO_image_imageExtGetAllT(ZF_IN_OUT ZFCoreArrayPOD<const zfchar *> &ret)
{
    zfstlmap<zfstlstringZ, zfbool> &m = _ZFP_ZFObjectIO_image_imageExtMap();
    for(zfstlmap<zfstlstringZ, zfbool>::iterator it = m.begin(); it != m.end(); ++it)
    {
        ret.add(it->first.c_str());
    }
}

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFObjectIO_image_imageExtDefault, ZFLevelZFFrameworkStatic)
{
    ZFObjectIO_image_imageExtAdd("png");
    ZFObjectIO_image_imageExtAdd("jpg");
    ZFObjectIO_image_imageExtAdd("jpeg");
}
ZF_GLOBAL_INITIALIZER_END(ZFObjectIO_image_imageExtDefault)

// ============================================================
ZFOBJECTIO_DEFINE(image, ZFM_EXPAND({
        zfstlmap<zfstlstringZ, zfbool> &m = _ZFP_ZFObjectIO_image_imageExtMap();
        const zfchar *fileExt = ZFObjectIOImplCheckFileExt(pathInfo);
        return (fileExt != zfnull && m.find(fileExt) != m.end());
    }), {
        ret = ZFUIImageLoadFromFile(input);
        if(ret == zfnull)
        {
            zfstringAppend(outErrorHint,
                "unable to load image from %s",
                ZFPathInfoToString(*input.pathInfo()).cString());
            return zffalse;
        }
        else
        {
            return zftrue;
        }
    }, {
        ZFUIImage *image = ZFCastZFObject(ZFUIImage *, obj);
        if(image == zfnull)
        {
            zfstringAppend(outErrorHint,
                "object %s is not type of %s",
                ZFObjectInfoOfInstance(obj).cString(),
                ZFUIImage::ClassData()->classNameFull());
            return zffalse;
        }
        if(!ZFUIImageSaveToFile(output, image))
        {
            zfstringAppend(outErrorHint,
                "unable to convert image %s to image file",
                ZFObjectInfo(image).cString());
            return zffalse;
        }
        return zftrue;
    })

ZF_NAMESPACE_GLOBAL_END

