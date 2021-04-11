#include "ZFUIImage.h"
#include "protocol/ZFProtocolZFUIImage.h"

#include "ZFCore/ZFSTLWrapper/zfstl_string.h"
#include "ZFCore/ZFSTLWrapper/zfstl_map.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// serializabel data
static zfstlmap<zfstlstringZ, _ZFP_ZFUIImageSerializeFromCallback> &_ZFP_ZFUIImageSerializeDataMap(void)
{
    static zfstlmap<zfstlstringZ, _ZFP_ZFUIImageSerializeFromCallback> d;
    return d;
}
void _ZFP_ZFUIImageSerializeTypeRegister(ZF_IN const zfchar *name,
                                         ZF_IN _ZFP_ZFUIImageSerializeFromCallback fromCallback)
{
    zfCoreMutexLocker();
    zfstlmap<zfstlstringZ, _ZFP_ZFUIImageSerializeFromCallback> &m = _ZFP_ZFUIImageSerializeDataMap();
    zfCoreAssert(name != zfnull && fromCallback != zfnull);
    zfCoreAssertWithMessageTrim(m.find(name) == m.end(),
        "[ZFUIIMAGE_SERIALIZE_TYPE_DEFINE] %s already registered",
        name);
    m[name] = fromCallback;
}
void _ZFP_ZFUIImageSerializeTypeUnregister(ZF_IN const zfchar *name)
{
    zfCoreMutexLocker();
    zfstlmap<zfstlstringZ, _ZFP_ZFUIImageSerializeFromCallback> &m = _ZFP_ZFUIImageSerializeDataMap();
    m.erase(name);
}

void ZFUIImageSerializeTypeGetAllT(ZF_IN_OUT ZFCoreArray<const zfchar *> &ret)
{
    zfCoreMutexLocker();
    zfstlmap<zfstlstringZ, _ZFP_ZFUIImageSerializeFromCallback> &m = _ZFP_ZFUIImageSerializeDataMap();
    for(zfstlmap<zfstlstringZ, _ZFP_ZFUIImageSerializeFromCallback>::iterator it = m.begin(); it != m.end(); ++it)
    {
        ret.add(it->first.c_str());
    }
}

// ============================================================
// _ZFP_ZFUIImagePrivate
zfclassNotPOD _ZFP_ZFUIImagePrivate
{
public:
    ZFUIImage *pimplOwner;
    void *nativeImage;
    zffloat imageScaleFixed;
    ZFUISize imageSizeFixed;
    ZFUISize imageSize;

public:
    zfchar *serializableType;
    const ZFSerializableData *serializableData;

public:
    void imageSizeUpdate(void)
    {
        this->imageScaleFixed = this->pimplOwner->imageScale() * ZFUIGlobalStyle::DefaultStyle()->imageScale();
        if(this->nativeImage != zfnull)
        {
            this->imageSizeFixed = ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageSize(this->nativeImage);
            this->imageSize = ZFUISizeApplyScaleReversely(this->imageSizeFixed, this->imageScaleFixed);
            if(this->imageSize.width <= 0)
            {
                this->imageSize.width = 1;
            }
            if(this->imageSize.height <= 0)
            {
                this->imageSize.height = 1;
            }
        }
        else
        {
            this->imageSizeFixed = ZFUISizeZero();
            this->imageSize = ZFUISizeZero();
        }
    }
    void copyFrom(ZF_IN _ZFP_ZFUIImagePrivate *another)
    {
        if(this->nativeImage != zfnull)
        {
            ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageRelease(this->nativeImage);
            this->nativeImage = zfnull;
        }
        this->imageSizeFixed = ZFUISizeZero();
        this->imageSize = ZFUISizeZero();
        zfsChange(this->serializableType, (const zfchar *)zfnull);
        if(this->serializableData != zfnull)
        {
            zfdelete(this->serializableData);
            this->serializableData = zfnull;
        }

        if(another->nativeImage != zfnull)
        {
            this->nativeImage = ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageRetain(another->nativeImage);
        }
        this->imageSizeUpdate();
        zfsChange(this->serializableType, another->serializableType);
        if(another->serializableData != zfnull)
        {
            this->serializableData = zfnew(ZFSerializableData, *(another->serializableData));
        }
    }

public:
    _ZFP_ZFUIImagePrivate(void)
    : pimplOwner(zfnull)
    , nativeImage(zfnull)
    , imageScaleFixed(ZFUIGlobalStyle::DefaultStyle()->imageScale())
    , imageSizeFixed(ZFUISizeZero())
    , imageSize(ZFUISizeZero())
    , serializableType(zfnull)
    , serializableData(zfnull)
    {
    }
    ~_ZFP_ZFUIImagePrivate(void)
    {
        zffree(this->serializableType);
        zfdelete(this->serializableData);
    }
};

// ============================================================
// ZFUIImage
ZFOBJECT_REGISTER(ZFUIImage)

zfbool ZFUIImage::serializableOnSerializeFromData(ZF_IN const ZFSerializableData &serializableData,
                                                  ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */,
                                                  ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */)
{
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeFromData(serializableData, outErrorHint, outErrorPos)) {return zffalse;}

    // style
    if(this->styleKey() != zfnull)
    {
        if(d->nativeImage == zfnull)
        {
            ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
                "unable to load image from style \"%s\"",
                this->styleKey());
            return zffalse;
        }
        else
        {
            return zftrue;
        }
    }

    // imageBin
    const zfchar *imageBin = zfnull;
    ZFSerializableUtilSerializeCategoryFromData(serializableData, outErrorHint, outErrorPos,
        check, ZFSerializableKeyword_ZFUIImage_imageBin, zfstring, imageBin);
    if(imageBin != zfnull)
    {
        zfblockedAlloc(ZFIOBufferByCacheFile, io);
        if(!ZFBase64Decode(io->output(), ZFInputForBufferUnsafe(imageBin)))
        {
            ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
                "invalid base64 data: \"%s\"", imageBin);
            return zffalse;
        }
        void *nativeImage = ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageFromInput(io->input());
        if(nativeImage == zfnull)
        {
            ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
                "fail to load image from base64 data: \"%s\"", imageBin);
            return zffalse;
        }
        this->nativeImage(nativeImage);
        ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageRelease(nativeImage);
        return zftrue;
    }

    // imageType
    const zfchar *typeName = zfnull;
    ZFSerializableUtilSerializeAttributeFromData(serializableData, outErrorHint, outErrorPos,
        require, ZFSerializableKeyword_ZFUIImage_imageType, zfstring, typeName);
    _ZFP_ZFUIImageSerializeFromCallback fromCallback = zfnull;
    {
        zfCoreMutexLocker();
        zfstlmap<zfstlstringZ, _ZFP_ZFUIImageSerializeFromCallback> &m = _ZFP_ZFUIImageSerializeDataMap();
        zfstlmap<zfstlstringZ, _ZFP_ZFUIImageSerializeFromCallback>::iterator it = m.find(typeName);
        if(it != m.end())
        {
            fromCallback = it->second;
        }
    }
    if(fromCallback == zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
            "no such image serializable type registered: \"%s\"", typeName);
        return zffalse;
    }

    // imageData
    ZFSerializableData imageData;
    {
        const ZFSerializableData *categoryData = ZFSerializableUtil::requireElementByCategory(serializableData, ZFSerializableKeyword_ZFUIImage_imageData);
        if(categoryData == zfnull)
        {
            return zffalse;
        }
        if(!fromCallback(this, *categoryData, outErrorHint, outErrorPos))
        {
            return zffalse;
        }

        imageData = categoryData->copy();
        imageData.category(zfnull);

        categoryData->resolveMark();
    }

    // check
    if(d->nativeImage == zfnull)
    {
        d->imageSizeFixed = ZFUISizeZero();
        d->imageSize = ZFUISizeZero();
        ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData, "nativeImage not set");
        return zffalse;
    }

    this->imageSerializableType(typeName);
    this->imageSerializableData(&imageData);

    return zftrue;
}
zfbool ZFUIImage::serializableOnSerializeToData(ZF_IN_OUT ZFSerializableData &serializableData,
                                                ZF_IN ZFSerializable *referencedOwnerOrNull,
                                                ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */)
{
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeToData(serializableData, referencedOwnerOrNull, outErrorHint)) {return zffalse;}
    zfself *ref = ZFCastZFObject(zfself *, referencedOwnerOrNull);

    // check
    if(d->nativeImage == zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHint, "serialize an image whose nativeImage not set");
        return zffalse;
    }

    if(this->imageSerializableType() != zfnull)
    { // custom serialize
        if(ref != zfnull
            && zfscmpTheSame(this->imageSerializableType(), ref->imageSerializableType())
            && this->imageSerializableData() != zfnull && ref->imageSerializableData() != zfnull
            && this->imageSerializableData()->objectCompare(*(ref->imageSerializableData())))
        {
            // all match, skip
        }
        else
        {
            // imageType
            ZFSerializableUtilSerializeAttributeToData(serializableData, outErrorHint, ref,
                ZFSerializableKeyword_ZFUIImage_imageType, zfstring, this->imageSerializableType(), ref->imageSerializableType(), "");

            { // data
                if(this->imageSerializableData() == zfnull)
                {
                    ZFSerializableUtil::errorOccurred(outErrorHint,
                        "missing image data for type: \"%s\"", this->imageSerializableType());
                    return zffalse;
                }

                ZFSerializableData categoryData = this->imageSerializableData()->copy();
                categoryData.category(ZFSerializableKeyword_ZFUIImage_imageData);
                serializableData.elementAdd(categoryData);
            }
        }
    }
    else
    { // imageBin
        zfstring imageBin;
        if(!ZFUIImageSaveToBase64(ZFOutputForString(imageBin), this))
        {
            ZFSerializableUtil::errorOccurred(outErrorHint, "save image to base64 failed");
            return zffalse;
        }
        zfstring imageBinRef;
        if(ref != zfnull)
        {
            ZFUIImageSaveToBase64(ZFOutputForString(imageBinRef), ref);
        }
        ZFSerializableUtilSerializeCategoryToData(serializableData, outErrorHint, ref,
            ZFSerializableKeyword_ZFUIImage_imageBin, zfstring, imageBin, imageBinRef, zfstring());
    }

    return zftrue;
}

void ZFUIImage::styleableOnCopyFrom(ZF_IN ZFStyleable *anotherStyleable)
{
    zfsuperI(ZFCopyable)::styleableOnCopyFrom(anotherStyleable);
    d->copyFrom(ZFCastZFObjectUnchecked(zfself *, anotherStyleable)->d);
}

ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUIImage, zffloat, imageScale)
{
    d->imageSizeUpdate();
    this->imageScaleOnChange();
}
void ZFUIImage::_ZFP_ZFUIImage_imageScaleOnChange(void)
{
    d->imageSizeUpdate();
    this->imageScaleOnChange();
}
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIImageScaleChangeListenerHolder, ZFLevelZFFrameworkEssential)
{
    this->globalImageScaleOnChangeListener = ZFCallbackForFunc(zfself::globalImageScaleOnChange);
}
ZFListener globalImageScaleOnChangeListener;
static void globalImageScaleOnChange(ZF_IN const ZFListenerData &listenerData, ZF_IN ZFObject *userData)
{
    const ZFProperty *property = listenerData.param0<v_ZFProperty *>()->zfv;
    if(property == ZFPropertyAccess(ZFUIGlobalStyle, imageScale))
    {
        ZFUIImage *image = userData->objectHolded();
        image->_ZFP_ZFUIImage_imageScaleOnChange();
    }
}
ZF_GLOBAL_INITIALIZER_END(ZFUIImageScaleChangeListenerHolder)

ZFMETHOD_DEFINE_0(ZFUIImage, zffloat const &, imageScaleFixed)
{
    return d->imageScaleFixed;
}

ZFMETHOD_DEFINE_0(ZFUIImage, const ZFUISize &, imageSize)
{
    return d->imageSize;
}
ZFMETHOD_DEFINE_0(ZFUIImage, const ZFUISize &, imageSizeFixed)
{
    return d->imageSizeFixed;
}

void ZFUIImage::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFUIImagePrivate);
    d->pimplOwner = this;
}
void ZFUIImage::objectOnDealloc(void)
{
    if(d->nativeImage != zfnull)
    {
        ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageRelease(d->nativeImage);
        d->nativeImage = zfnull;
    }
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}
void ZFUIImage::objectOnInitFinish(void)
{
    zfsuper::objectOnInitFinish();
    ZFUIGlobalStyle::DefaultStyle()->observerAdd(
        ZFObject::EventObjectPropertyValueOnUpdate(),
        ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIImageScaleChangeListenerHolder)->globalImageScaleOnChangeListener,
        this->objectHolder());
}
void ZFUIImage::objectOnDeallocPrepare(void)
{
    ZFUIGlobalStyle::DefaultStyle()->observerRemove(
        ZFObject::EventObjectPropertyValueOnUpdate(),
        ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIImageScaleChangeListenerHolder)->globalImageScaleOnChangeListener);
    zfsuper::objectOnDeallocPrepare();
}

zfidentity ZFUIImage::objectHash(void)
{
    return zfidentityCalcPointer(d->nativeImage);
}
ZFCompareResult ZFUIImage::objectCompare(ZF_IN ZFObject *anotherObj)
{
    if(this == anotherObj) {return ZFCompareTheSame;}
    zfself *another = ZFCastZFObject(zfself *, anotherObj);
    if(another == zfnull) {return ZFCompareUncomparable;}

    if(d->nativeImage == another->d->nativeImage
        && this->imageNinePatch() == another->imageNinePatch()
        && zfscmpTheSame(d->serializableType, another->d->serializableType)
        && ((d->serializableData == zfnull && another->d->serializableData == zfnull)
            || (d->serializableData != zfnull && another->d->serializableData != zfnull
                && d->serializableData->objectCompare(*(another->d->serializableData)) == ZFCompareTheSame)))
    {
        return ZFCompareTheSame;
    }
    return ZFCompareUncomparable;
}
void ZFUIImage::objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
{
    zfsuper::objectInfoOnAppend(ret);
    ret += " ";
    ZFUISizeToString(ret, this->imageSize());
}

ZFMETHOD_DEFINE_0(ZFUIImage, void *, nativeImage)
{
    return d->nativeImage;
}

void ZFUIImage::nativeImage(ZF_IN void *nativeImage)
{
    void *toRelease = d->nativeImage;

    if(nativeImage != zfnull)
    {
        d->nativeImage = ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageRetain(nativeImage);
    }
    else
    {
        d->nativeImage = zfnull;
    }
    d->imageSizeUpdate();

    if(toRelease != zfnull)
    {
        ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageRelease(toRelease);
    }
}

void ZFUIImage::imageSerializableType(ZF_IN const zfchar *typeName)
{
    zfsChange(d->serializableType, typeName);
}
const zfchar *ZFUIImage::imageSerializableType(void)
{
    return d->serializableType;
}
void ZFUIImage::imageSerializableData(ZF_IN const ZFSerializableData *serializableData)
{
    const ZFSerializableData *old = d->serializableData;
    if(serializableData != zfnull)
    {
        d->serializableData = zfnew(ZFSerializableData, *serializableData);
    }
    else
    {
        d->serializableData = zfnull;
    }
    if(old != zfnull)
    {
        zfdelete(old);
    }
}
const ZFSerializableData *ZFUIImage::imageSerializableData(void)
{
    return d->serializableData;
}

ZF_NAMESPACE_GLOBAL_END

