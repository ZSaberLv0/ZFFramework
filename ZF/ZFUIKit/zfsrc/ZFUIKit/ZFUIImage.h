/**
 * @file ZFUIImage.h
 * @brief image
 */

#ifndef _ZFI_ZFUIImage_h_
#define _ZFI_ZFUIImage_h_

#include "ZFUIGlobalStyle.h"
ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd ZFUIImage;

// ============================================================
typedef zfbool (*_ZFP_ZFUIImageSerializeFromCallback)(ZF_IN_OUT ZFUIImage *image,
                                                      ZF_IN const ZFSerializableData &imageData,
                                                      ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */,
                                                      ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */);

extern ZF_ENV_EXPORT void _ZFP_ZFUIImageSerializeTypeRegister(ZF_IN const zfchar *name,
                                                              ZF_IN _ZFP_ZFUIImageSerializeFromCallback fromCallback);
extern ZF_ENV_EXPORT void _ZFP_ZFUIImageSerializeTypeUnregister(ZF_IN const zfchar *name);
/**
 * @brief register a serializable logic for ZFUIImage
 *
 * ZFUIImage can be loaded from many sources,
 * so we supply a dynamic serializable logic depending on image's load logic,
 * use this macro to register your own serialize logic,
 * such as load from file or load from color\n
 * \n
 * example
 * @code
 *   // in cpp file
 *   ZFUIIMAGE_SERIALIZE_TYPE_DEFINE(registerSig, yourTypeName)
 *   {
 *       // serialize callback from data
 *       // proto type:
 *       //   zfbool action(ZF_IN_OUT ZFUIImage *ret,
 *       //                 ZF_IN const ZFSerializableData &serializableData,
 *       //                 ZF_OUT_OPT zfstring *outErrorHint,
 *       //                 ZF_OUT_OPT ZFSerializableData *outErrorPos);
 *       return zftrue;
 *   }
 * @endcode
 */
#define ZFUIIMAGE_SERIALIZE_TYPE_DEFINE(registerSig, typeName) \
    static zfbool _ZFP_ZFUIImageSerializeFromCallback_##registerSig(ZF_IN_OUT ZFUIImage *ret, \
                                                                    ZF_IN const ZFSerializableData &serializableData, \
                                                                    ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */, \
                                                                    ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */); \
    ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIImageSerializeReg_##registerSig, ZFLevelZFFrameworkHigh) \
    { \
        _ZFP_ZFUIImageSerializeTypeRegister(typeName, _ZFP_ZFUIImageSerializeFromCallback_##registerSig); \
    } \
    ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIImageSerializeReg_##registerSig) \
    { \
        _ZFP_ZFUIImageSerializeTypeUnregister(typeName); \
    } \
    ZF_GLOBAL_INITIALIZER_END(ZFUIImageSerializeReg_##registerSig) \
    static zfbool _ZFP_ZFUIImageSerializeFromCallback_##registerSig(ZF_IN_OUT ZFUIImage *ret, \
                                                                    ZF_IN const ZFSerializableData &serializableData, \
                                                                    ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */, \
                                                                    ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */)

/** @brief see #ZFUIImageSerializeTypeGetAll */
extern ZF_ENV_EXPORT void ZFUIImageSerializeTypeGetAllT(ZF_IN_OUT ZFCoreArray<const zfchar *> &ret);
/**
 * @brief usually for debug use only
 */
inline ZFCoreArrayPOD<const zfchar *> ZFUIImageSerializeTypeGetAll(void)
{
    ZFCoreArrayPOD<const zfchar *> ret;
    ZFUIImageSerializeTypeGetAllT(ret);
    return ret;
}

/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUIImage_imageType "imageType"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUIImage_imageData "imageData"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUIImage_imageBin "imageBin"

// ============================================================
// ZFUIImage
zfclassFwd _ZFP_ZFUIImagePrivate;
/**
 * @brief image storage
 * @note #ZFUIImage always use custom scale value, see #ZFUIImage::imageScale
 *
 * \n
 * serializable data:
 * @code
 *   <ZFUIImage imageType="type name for ZFUIIMAGE_SERIALIZE_TYPE_DEFINE" >
 *       <anything category="imageData" ... /> // depends on load logic, see #ZFUIIMAGE_SERIALIZE_TYPE_DEFINE
 *
 *       // if nothing supplied, we will serialize by binary data
 *       <zfstring category="imageBin" value="image binary data, base64 encoded" />
 *   </ZFUIImage>
 * @endcode
 */
zffinal zfclass ZF_ENV_EXPORT ZFUIImage : zfextends ZFStyleableObject
{
    ZFOBJECT_DECLARE(ZFUIImage, ZFStyleableObject)

    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when #imageScale or #ZFUIImage::imageScale changed
     */
    ZFOBSERVER_EVENT(ImageScaleOnChange)

protected:
    zfoverride
    virtual zfbool serializableOnSerializeFromData(ZF_IN const ZFSerializableData &serializableData,
                                                   ZF_OUT_OPT zfstring *outErrorHint = zfnull,
                                                   ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull);
    zfoverride
    virtual zfbool serializableOnSerializeToData(ZF_IN_OUT ZFSerializableData &serializableData,
                                                 ZF_IN ZFSerializable *referencedOwnerOrNull,
                                                 ZF_OUT_OPT zfstring *outErrorHint = zfnull);

protected:
    zfoverride
    virtual void styleableOnCopyFrom(ZF_IN ZFStyleable *anotherStyleable);

    // ============================================================
    // property
public:
    /**
     * @brief image's scale, 1 by default
     *
     * we have these image scale settings:
     * -  #ZFUIImage::imageScale, scale setting, serializable
     * -  #ZFUIGlobalStyle::imageScale, global scale for all images
     * -  #ZFUIImage::imageScaleFixed, util method to access actual scale
     *
     * to access pixel size, use #imageSizeFixed instead
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zffloat, imageScale, 1)
    ZFPROPERTY_ON_ATTACH_DECLARE(zffloat, imageScale)
    /**
     * @brief nine patch described by a margin value, disabled if zero margin or margin exceeds image size
     * @note #ZFUIImage always use custom scale value, see #ZFUIImage::imageScale
     */
    ZFPROPERTY_ASSIGN(ZFUIMargin, imageNinePatch)

    /**
     * @brief util method to access actual scale
     *
     * ensured to be (#imageScale * #ZFUIGlobalStyle::imageScale)
     */
    ZFMETHOD_DECLARE_0(zffloat const &, imageScaleFixed)

    /**
     * @brief get size of the image
     * @note #ZFUIImage always use custom scale value, see #ZFUIImage::imageScale
     */
    ZFMETHOD_DECLARE_0(const ZFUISize &, imageSize)
    /**
     * @brief get raw size of the image without scale (in pixel unit)
     *
     * this is the pixel size of the image,
     * not affected by #imageScale
     */
    ZFMETHOD_DECLARE_0(const ZFUISize &, imageSizeFixed)

    // ============================================================
    // other
    ZFCLASS_PRIVATE_ALLOC("can only be created by ZFUIImageLoadXxx")
protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);
    zfoverride
    virtual void objectOnInitFinish(void);
    zfoverride
    virtual void objectOnDeallocPrepare(void);

    zfoverride
    virtual zfidentity objectHash(void);
    zfoverride
    virtual ZFCompareResult objectCompare(ZF_IN ZFObject *anotherObj);
protected:
    zfoverride
    virtual void objectInfoOnAppend(ZF_IN_OUT zfstring &ret);

public:
    /**
     * @brief get native image object
     */
    ZFMETHOD_DECLARE_0(void *, nativeImage)

public:
    zffinal void _ZFP_ZFUIImage_imageScaleOnChange(void);
protected:
    /** @brief see #EventImageScaleOnChange */
    virtual void imageScaleOnChange(void)
    {
        this->observerNotify(ZFUIImage::EventImageScaleOnChange());
    }

    // ============================================================
    // internal methods
public:
    /**
     * @brief internal use only, set the native image
     *
     * the image would be retained, actual retain logic is depending on the implementation
     */
    virtual void nativeImage(ZF_IN void *nativeImage,
                             ZF_IN_OPT zfbool retainNativeImage = zftrue);

    /**
     * @brief see #ZFUIIMAGE_SERIALIZE_TYPE_DEFINE
     */
    virtual void imageSerializableType(ZF_IN const zfchar *typeName);
    /**
     * @brief see #imageSerializableType
     */
    virtual const zfchar *imageSerializableType(void);
    /**
     * @brief see #ZFUIIMAGE_SERIALIZE_TYPE_DEFINE
     */
    virtual void imageSerializableData(ZF_IN const ZFSerializableData *serializableData);
    /**
     * @brief see #imageSerializableData
     */
    virtual const ZFSerializableData *imageSerializableData(void);

private:
    _ZFP_ZFUIImagePrivate *d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIImage_h_

#include "ZFUIImageIO.h"

