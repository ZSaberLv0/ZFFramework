#include "ZFUIImageIO.h"
#include "protocol/ZFProtocolZFUIImage.h"
#include "protocol/ZFProtocolZFUIImageIO.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// raw image io
ZFMETHOD_FUNC_DEFINE_1(zfautoObjectT<ZFUIImage *>, ZFUIImageLoadFromBase64,
                       ZFMP_IN(const ZFInput &, inputCallback))
{
    ZFIOBufferedCallbackUsingTmpFile io;
    zfautoObjectT<ZFUIImage *> ret = ZFUIImage::ClassData()->newInstance();
    ZFUIImage *image = ret;
    if(image != zfnull && ZFBase64Decode(io.outputCallback(), inputCallback))
    {
        void *nativeImage = ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageFromInput(io.inputCallback());
        if(nativeImage != zfnull)
        {
            image->nativeImage(nativeImage);
            ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageRelease(nativeImage);
            return ret;
        }
    }
    return zfnull;
}
ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFUIImageSaveToBase64,
                       ZFMP_OUT(const ZFOutput &, outputCallback),
                       ZFMP_IN(ZFUIImage *, image))
{
    if(image != zfnull && image->nativeImage() != zfnull && outputCallback.callbackIsValid())
    {
        ZFIOBufferedCallbackUsingTmpFile io;
        if(!ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageToOutput(image->nativeImage(), io.outputCallback()))
        {
            return zffalse;
        }
        return ZFBase64Encode(outputCallback, io.inputCallback());
    }
    return zffalse;
}
ZFMETHOD_FUNC_DEFINE_1(zfautoObjectT<ZFUIImage *>, ZFUIImageLoadFromFile,
                       ZFMP_IN(const ZFInput &, inputCallback))
{
    zfautoObjectT<ZFUIImage *> ret = ZFUIImage::ClassData()->newInstance();
    ZFUIImage *image = ret;
    if(image == zfnull || !inputCallback.callbackIsValid())
    {
        return zfnull;
    }
    void *nativeImage = ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageFromInput(inputCallback);
    if(nativeImage == zfnull)
    {
        return zfnull;
    }
    image->nativeImage(nativeImage);
    ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageRelease(nativeImage);

    if(!inputCallback.callbackSerializeCustomDisabled())
    {
        ZFSerializableData inputData;
        if(ZFCallbackToData(inputData, inputCallback))
        {
            image->imageSerializableType(ZFUIImageSerializeType_input);
            image->imageSerializableData(&inputData);
        }
    }
    return ret;
}
ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFUIImageSaveToFile,
                       ZFMP_OUT(const ZFOutput &, outputCallback),
                       ZFMP_IN(ZFUIImage *, image))
{
    if(image != zfnull && image->nativeImage() != zfnull && outputCallback.callbackIsValid())
    {
        return ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageToOutput(image->nativeImage(), outputCallback);
    }
    return zffalse;
}

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
    void *nativeImage = ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageFromInput(input);
    if(nativeImage == zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
            "load image failed");
        return zffalse;
    }
    ret->nativeImage(nativeImage);
    ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageRelease(nativeImage);
    return zftrue;
}

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
    zfautoObjectT<ZFUIImage *> ret = ZFUIImageLoadFromNativeImage(nativeImage);
    ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageRelease(nativeImage);
    return ret;
}

// ============================================================
// ZFUIImageLoadInFrame
static zfbool _ZFP_ZFUIImageLoadInFrame(ZF_IN_OUT ZFUIImage *ret,
                                        ZF_IN ZFUIImage *image,
                                        ZF_IN const ZFUIRect &framePixel)
{
    if(ret == zfnull || image == zfnull)
    {
        return zffalse;
    }
    const ZFUISize &imageSizeFixed = image->imageSizeFixed();
    if(framePixel.x < 0 || framePixel.x >= imageSizeFixed.width
        || framePixel.y < 0 || framePixel.y >= imageSizeFixed.height)
    {
        return zffalse;
    }
    ZFUIRect frame = framePixel;
    if(frame.width <= 0 || frame.x + frame.width > imageSizeFixed.width)
    {
        frame.width = imageSizeFixed.width - frame.x;
    }
    if(frame.height <= 0 || frame.y + frame.height > imageSizeFixed.height)
    {
        frame.height = imageSizeFixed.height - frame.y;
    }
    void *nativeImage = ZFPROTOCOL_ACCESS(ZFUIImageIO)->imageLoadInFrame(
        image->imageScaleFixed(),
        image->nativeImage(),
        framePixel);
    ret->nativeImage(nativeImage);
    ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageRelease(nativeImage);

    // only store custom type for performance
    if(image->imageSerializableType() != zfnull)
    {
        ZFSerializableData data;
        data.itemClass(ZFSerializableKeyword_node);
        ZFSerializableData refData;
        ZFSerializableData frameData;
        if(image->serializeToData(refData) && ZFUIRectToData(frameData, frame))
        {
            refData.category(ZFSerializableKeyword_ZFUIImageIO_ref);
            frameData.category(ZFSerializableKeyword_ZFUIImageIO_ref_frame);
            data.elementAdd(refData);
            data.elementAdd(frameData);
            ret->imageSerializableType(ZFUIImageSerializeType_ref);
            ret->imageSerializableData(&data);
        }
    }

    return zftrue;
}
ZFUIIMAGE_SERIALIZE_TYPE_DEFINE(ref, ZFUIImageSerializeType_ref)
{
    serializableData.resolveMark();

    zfautoObjectT<ZFUIImage *> ref;
    const ZFSerializableData *refData = ZFSerializableUtil::requireElementByCategory(
        serializableData, ZFSerializableKeyword_ZFUIImageIO_ref, outErrorHint, outErrorPos);
    if(refData == zfnull || !ZFObjectFromData(ref, *refData, outErrorHint, outErrorPos) || ref == zfnull)
    {
        return zffalse;
    }

    ZFUIRect frame = ZFUIRectMake(ZFUIPointZero(), ref->imageSizeFixed());
    const ZFSerializableData *frameData = ZFSerializableUtil::checkElementByCategory(
        serializableData, ZFSerializableKeyword_ZFUIImageIO_ref_frame);
    if(frameData == zfnull || !ZFUIRectFromData(frame, *frameData, outErrorHint, outErrorPos))
    {
        return zffalse;
    }

    return _ZFP_ZFUIImageLoadInFrame(ret, ref, frame);;
}

ZFMETHOD_FUNC_DEFINE_2(zfautoObjectT<ZFUIImage *>, ZFUIImageLoadInFrame,
                       ZFMP_IN(ZFUIImage *, image),
                       ZFMP_IN(const ZFUIRect &, framePixel))
{
    zfautoObjectT<ZFUIImage *> ret = ZFUIImage::ClassData()->newInstance();
    if(_ZFP_ZFUIImageLoadInFrame(ret, image, framePixel))
    {
        return ret;
    }
    else
    {
        return zfnull;
    }
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
                       ZFMP_IN_OPT(const ZFUISize &, sizePixel, ZFUISizeZero()))
{
    ZFUISize sizeTmp = ZFUISizeMake(zfmMax(1, sizePixel.width), zfmMax(1, sizePixel.height));
    void *nativeImage = ZFPROTOCOL_ACCESS(ZFUIImageIO)->imageLoadFromColor(
        ZFUIGlobalStyle::DefaultStyle()->imageScale(),
        color,
        sizeTmp);
    zfautoObjectT<ZFUIImage *> ret = ZFUIImageLoadFromNativeImage(nativeImage);
    ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageRelease(nativeImage);
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

