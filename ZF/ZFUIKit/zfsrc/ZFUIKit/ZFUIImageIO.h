/**
 * @file ZFUIImageIO.h
 * @brief common image load logic
 */

#ifndef _ZFI_ZFUIImageIO_h_
#define _ZFI_ZFUIImageIO_h_

#include "ZFUIImage.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// raw image io
/**
 * @brief load image from base64 data
 * @note this method has no cache logic
 */
ZFMETHOD_FUNC_DECLARE_1(zfautoObjectT<ZFUIImage *>, ZFUIImageLoadFromBase64,
                        ZFMP_IN(const ZFInput &, inputCallback))
/**
 * @brief save image to base64 data
 */
ZFMETHOD_FUNC_DECLARE_2(zfbool, ZFUIImageSaveToBase64,
                        ZFMP_OUT(const ZFOutput &, outputCallback),
                        ZFMP_IN(ZFUIImage *, image))
/**
 * @brief load image from file
 * @note this method has no cache logic
 */
ZFMETHOD_FUNC_DECLARE_1(zfautoObjectT<ZFUIImage *>, ZFUIImageLoadFromFile,
                        ZFMP_IN(const ZFInput &, inputCallback))
/**
 * @brief save image to file
 */
ZFMETHOD_FUNC_DECLARE_2(zfbool, ZFUIImageSaveToFile,
                        ZFMP_OUT(const ZFOutput &, outputCallback),
                        ZFMP_IN(ZFUIImage *, image))

/**
 * @brief see #ZFUIIMAGE_SERIALIZE_TYPE_DEFINE
 *
 * serializable data:
 * @code
 *   <ZFCallback ... />
 * @endcode
 */
#define ZFUIImageSerializeType_input "input"

// ============================================================
// ZFUIImageScale
/**
 * @brief scale and return the newly created scaled image
 *
 * if the source image have nine patch,
 * scale would use the nine patch setting\n
 * return null if source image or newSize invalid\n
 * this method would create a new image even if size not changed
 */
ZFMETHOD_FUNC_DECLARE_2(zfautoObjectT<ZFUIImage *>, ZFUIImageScale,
                        ZFMP_IN(ZFUIImage *, image),
                        ZFMP_IN(const ZFUISize &, newSize))

// ============================================================
// ZFUIImageLoadInFrame
/**
 * @brief see #ZFUIIMAGE_SERIALIZE_TYPE_DEFINE
 *
 * serializable data:
 * @code
 *   <node ...>
 *       <ZFUIImage category="ref" ... />
 *       <ZFUIRect category="frame" ... />
 *   </node>
 * @endcode
 */
#define ZFUIImageSerializeType_ref "ref"

/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUIImageIO_ref "ref"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUIImageIO_ref_frame "frame"

/**
 * @brief clip an exist image and sharing low level data if possible
 * @note this method has no cache logic
 */
ZFMETHOD_FUNC_DECLARE_2(zfautoObjectT<ZFUIImage *>, ZFUIImageLoadInFrame,
                        ZFMP_IN(ZFUIImage *, image),
                        ZFMP_IN(const ZFUIRect &, frameInImage))

// ============================================================
// ZFUIImageLoadFromNativeImage
/**
 * @brief create image from native image
 *
 * the native image would be retained by the #ZFUIImage
 */
ZFMETHOD_FUNC_DECLARE_1(zfautoObjectT<ZFUIImage *>, ZFUIImageLoadFromNativeImage,
                        ZFMP_IN(void *, nativeImage))

// ============================================================
// color
/**
 * @brief see #ZFUIIMAGE_SERIALIZE_TYPE_DEFINE
 *
 * serializable data:
 * @code
 *   <node>
 *       <ZFUIColor category="color" /> // optional, transparent by default
 *       <ZFUISize category="size" /> // optional, (1, 1) by default
 *   </node>
 * @endcode
 */
#define ZFUIImageSerializeType_color "color"

/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUIImageIO_color "color"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUIImageIO_color_size "size"

/**
 * @brief load image from color
 *
 * size is applied with #ZFUIGlobalStyle::imageScale
 * @note this method has no cache logic
 */
ZFMETHOD_FUNC_DECLARE_2(zfautoObjectT<ZFUIImage *>, ZFUIImageLoadFromColor,
                        ZFMP_IN(const ZFUIColor &, color),
                        ZFMP_IN_OPT(const ZFUISize &, size, ZFUISizeZero()))

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIImageIO_h_

