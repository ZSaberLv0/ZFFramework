#include "ZFUIImageIO.h"
#include "protocol/ZFProtocolZFUIImage.h"
#include "protocol/ZFProtocolZFUIImageIO.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUIImageScale
ZFMETHOD_FUNC_DEFINE_2(zfautoObjectT<ZFUIImage *>, ZFUIImageScale,
                       ZFMP_IN(ZFUIImage *, image),
                       ZFMP_IN(const ZFUISize &, newSize))
{
    if(image == zfnull || image->nativeImage() == zfnull)
    {
        return zfnull;
    }
    zffloat scale = image->imageScaleFixed();
    void *nativeImage = ZFPROTOCOL_ACCESS(ZFUIImageIO)->imageApplyScale(
        scale,
        image->nativeImage(),
        ZFUISizeApplyScale(newSize, scale),
        ZFUIMarginApplyScale(image->imageNinePatch(), scale));
    return ZFUIImageLoadFromNativeImage(nativeImage);
}

// ============================================================
// ZFUIImageLoadInFrame
ZFMETHOD_FUNC_DEFINE_2(zfautoObjectT<ZFUIImage *>, ZFUIImageLoadInFrame,
                       ZFMP_IN(ZFUIImage *, image),
                       ZFMP_IN(const ZFUIRect &, frameInImage))
{
    if(image == zfnull
        || frameInImage.x < 0 || frameInImage.x >= image->imageSize().width
        || frameInImage.y < 0 || frameInImage.y >= image->imageSize().height
    ) {
        return zfnull;
    }
    ZFUIRect frame = frameInImage;
    if(frame.width <= 0 || frame.x + frame.width > image->imageSize().width)
    {
        frame.width = image->imageSize().width - frame.x;
    }
    if(frame.height <= 0 || frame.y + frame.height > image->imageSize().height)
    {
        frame.height = image->imageSize().height - frame.y;
    }
    zffloat scale = image->imageScaleFixed();
    void *nativeImage = ZFPROTOCOL_ACCESS(ZFUIImageIO)->imageLoadInFrame(
        scale,
        image->nativeImage(),
        ZFUIRectApplyScale(frameInImage, scale));
    zfautoObjectT<ZFUIImage *> ret = ZFUIImageLoadFromNativeImage(nativeImage);
    if(ret == zfnull
        || image->imageSerializableType() == zfnull // only store custom type for performance
    ) {
        return ret;
    }

    ZFSerializableData data;
    data.itemClass(ZFSerializableKeyword_node);
    ZFSerializableData refData;
    ZFSerializableData frameData;
    if(!image->serializeToData(refData) || !ZFUIRectToData(frameData, frame))
    {
        return ret;
    }
    refData.category(ZFSerializableKeyword_ZFUIImageIO_ref);
    frameData.category(ZFSerializableKeyword_ZFUIImageIO_ref_frame);
    data.elementAdd(refData);
    data.elementAdd(frameData);
    ret->imageSerializableType(ZFUIImageSerializeType_ref);
    ret->imageSerializableData(&data);

    return ret;
}

// ============================================================
// ZFUIImageLoadFromNativeImage
ZFMETHOD_FUNC_DEFINE_1(zfautoObjectT<ZFUIImage *>, ZFUIImageLoadFromNativeImage,
                       ZFMP_IN(void *, nativeImage))
{
    if(nativeImage == zfnull)
    {
        return zfnull;
    }
    zfautoObjectT<ZFUIImage *> ret = ZFUIImage::ClassData()->newInstance();
    ret.to<ZFUIImage *>()->nativeImage(nativeImage);
    ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageRelease(nativeImage);
    return ret;
}

// ============================================================
// input
ZFUIIMAGE_SERIALIZE_TYPE_DEFINE(input, ZFUIImageSerializeType_input)
{
    ZFCallback input;
    if(!ZFCallbackFromData(input, serializableData, outErrorHint, outErrorPos))
    {
        return zffalse;
    }
    if(!input.callbackIsValid())
    {
        ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
            "invalid callback");
        return zffalse;
    }
    if(!ZFUIImageEncodeFromFile(ret, input))
    {
        ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
            "load image failed");
        return zffalse;
    }
    return zftrue;
}

ZFMETHOD_FUNC_DEFINE_1(zfautoObjectT<ZFUIImage *>, ZFUIImageLoadFromInput,
                       ZFMP_IN(const ZFInput &, input))
{
    zfautoObjectT<ZFUIImage *> ret = ZFUIImage::ClassData()->newInstance();
    ZFUIImage *image = ret;
    if(!ZFUIImageEncodeFromFile(image, input))
    {
        return zfnull;
    }

    if(!input.callbackSerializeCustomDisabled())
    {
        ZFSerializableData inputData;
        if(ZFCallbackToData(inputData, input))
        {
            image->imageSerializableType(ZFUIImageSerializeType_input);
            image->imageSerializableData(&inputData);
        }
    }

    return ret;
}

// ============================================================
// color
ZFUIIMAGE_SERIALIZE_TYPE_DEFINE(color, ZFUIImageSerializeType_color)
{
    ZFUIColor color = ZFUIColorZero();
    { // color
        const ZFSerializableData *categoryData = ZFSerializableUtil::checkElementByCategory(serializableData, ZFSerializableKeyword_ZFUIImageIO_color);
        if(categoryData != zfnull)
        {
            if(!ZFUIColorFromData(color, *categoryData, outErrorHint, outErrorPos))
            {
                return zffalse;
            }
        }
    }

    ZFUISize size = ZFUISizeMake(1);
    { // size
        const ZFSerializableData *categoryData = ZFSerializableUtil::checkElementByCategory(serializableData, ZFSerializableKeyword_ZFUIImageIO_color_size);
        if(categoryData != zfnull)
        {
            if(!ZFUISizeFromData(size, *categoryData, outErrorHint, outErrorPos))
            {
                return zffalse;
            }
            if(size.width <= 0 || size.height <= 0)
            {
                ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, *categoryData,
                    "invalid size: %s", ZFUISizeToString(size).cString());
                return zffalse;
            }
        }
    }

    ZFSerializableData imageData;
    {
        if(!ZFUIColorIsEqual(color, ZFUIColorZero()))
        {
            ZFSerializableData categoryData;
            if(!ZFUIColorToData(categoryData, color, outErrorHint))
            {
                return zffalse;
            }
            categoryData.category(ZFSerializableKeyword_ZFUIImageIO_color);
            imageData.elementAdd(categoryData);
        }
        if(!ZFUISizeIsEqual(size, ZFUISizeMake(1, 1)))
        {
            ZFSerializableData categoryData;
            if(!ZFUISizeToData(categoryData, size, outErrorHint))
            {
                return zffalse;
            }
            categoryData.category(ZFSerializableKeyword_ZFUIImageIO_color_size);
            imageData.elementAdd(categoryData);
        }
    }

    void *nativeImage = ZFPROTOCOL_ACCESS(ZFUIImageIO)->imageLoadFromColor(
        ZFUIGlobalStyle::DefaultStyle()->imageScale(),
        color,
        ZFUISizeApplyScale(size, ZFUIGlobalStyle::DefaultStyle()->imageScale()));
    ret->nativeImage(nativeImage);
    ret->imageSerializableData(&imageData);
    ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageRelease(nativeImage);

    return zftrue;
}
ZFMETHOD_FUNC_DEFINE_2(zfautoObjectT<ZFUIImage *>, ZFUIImageLoadFromColor,
                       ZFMP_IN(const ZFUIColor &, color),
                       ZFMP_IN_OPT(const ZFUISize &, size, ZFUISizeZero()))
{
    ZFUISize sizeTmp = ZFUISizeMake(zfmMax(1, size.width), zfmMax(1, size.height));
    void *nativeImage = ZFPROTOCOL_ACCESS(ZFUIImageIO)->imageLoadFromColor(
        ZFUIGlobalStyle::DefaultStyle()->imageScale(),
        color,
        ZFUISizeApplyScale(sizeTmp, ZFUIGlobalStyle::DefaultStyle()->imageScale()));
    zfautoObjectT<ZFUIImage *> ret = ZFUIImageLoadFromNativeImage(nativeImage);
    ZFUIImage *image = ret;
    if(image == zfnull)
    {
        return zfnull;
    }

    ZFSerializableData imageData;
    do
    {
        // color
        if(color != ZFUIColorZero())
        {
            ZFSerializableData categoryData;
            if(!ZFUIColorToData(categoryData, color))
            {
                break;
            }
            categoryData.category(ZFSerializableKeyword_ZFUIImageIO_color);
            imageData.elementAdd(categoryData);
        }

        // size
        if(!ZFUISizeIsEqual(sizeTmp, ZFUISizeMake(1, 1)))
        {
            ZFSerializableData categoryData;
            if(!ZFUISizeToData(categoryData, sizeTmp))
            {
                break;
            }
            categoryData.category(ZFSerializableKeyword_ZFUIImageIO_color_size);
            imageData.elementAdd(categoryData);
        }

        imageData.itemClass(ZFSerializableKeyword_node);
    } while(zffalse);

    if(imageData.itemClass() != zfnull)
    {
        image->imageSerializableType(ZFUIImageSerializeType_color);
        image->imageSerializableData(&imageData);
    }

    return ret;
}

ZF_NAMESPACE_GLOBAL_END

