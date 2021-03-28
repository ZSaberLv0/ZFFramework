/**
 * @file ZFUIImageIO.h
 * @brief common image load logic
 */

#ifndef _ZFI_ZFUIImageIO_h_
#define _ZFI_ZFUIImageIO_h_

#include "ZFUIImage.h"
ZF_NAMESPACE_GLOBAL_BEGIN

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
// input
/**
 * @brief see #ZFUIIMAGE_SERIALIZE_TYPE_DEFINE
 *
 * serializable data:
 * @code
 *   <ZFCallback ... />
 * @endcode
 */
#define ZFUIImageSerializeType_input "input"

/**
 * @brief load image from input, input should contain the image's binary data
 * @note this method has no cache logic
 */
ZFMETHOD_FUNC_DECLARE_1(zfautoObjectT<ZFUIImage *>, ZFUIImageLoadFromInput,
                        ZFMP_IN(const ZFInput &, input))

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

