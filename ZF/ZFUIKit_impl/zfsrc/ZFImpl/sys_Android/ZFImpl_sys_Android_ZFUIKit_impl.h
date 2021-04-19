#ifndef _ZFI_ZFImpl_sys_Android_ZFUIKit_impl_h_
#define _ZFI_ZFImpl_sys_Android_ZFUIKit_impl_h_

#include "ZFImpl.h"

#include "ZFImpl/sys_Android/ZFImpl_sys_Android_ZF_impl.h"
#include "ZFUIKit/ZFUITypeDef.h"

#if ZF_ENV_sys_Android
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// Point
#define ZFImpl_sys_Android_JNI_ID_ZFAndroidPoint ZFImpl_sys_Android_JNI_ID(NativeUtil_ZFAndroidPoint)
#define ZFImpl_sys_Android_JNI_NAME_ZFAndroidPoint ZFImpl_sys_Android_JNI_NAME(NativeUtil.ZFAndroidPoint)
extern ZF_ENV_EXPORT jclass ZFImpl_sys_Android_jclassZFAndroidPoint(void);
extern ZF_ENV_EXPORT jobject ZFImpl_sys_Android_ZFUIPointToZFAndroidPoint(ZF_IN const ZFUIPoint &point, ZF_IN_OUT jobject jobjPoint);
// note returned value must be released by DeleteLocalRef
// note conversion may have performance issue
extern ZF_ENV_EXPORT jobject ZFImpl_sys_Android_ZFUIPointToZFAndroidPoint(ZF_IN const ZFUIPoint &point);
extern ZF_ENV_EXPORT void ZFImpl_sys_Android_ZFUIPointFromZFAndroidPointT(ZF_OUT ZFUIPoint &ret, ZF_IN jobject jobjPoint);
inline ZFUIPoint ZFImpl_sys_Android_ZFUIPointFromZFAndroidPoint(ZF_IN jobject jobjPoint)
{
    ZFUIPoint ret = ZFUIPointZero();
    ZFImpl_sys_Android_ZFUIPointFromZFAndroidPointT(ret, jobjPoint);
    return ret;
}

// ============================================================
// Size
#define ZFImpl_sys_Android_JNI_ID_ZFAndroidSize ZFImpl_sys_Android_JNI_ID(NativeUtil_ZFAndroidSize)
#define ZFImpl_sys_Android_JNI_NAME_ZFAndroidSize ZFImpl_sys_Android_JNI_NAME(NativeUtil.ZFAndroidSize)
extern ZF_ENV_EXPORT jclass ZFImpl_sys_Android_jclassZFAndroidSize(void);
extern ZF_ENV_EXPORT jobject ZFImpl_sys_Android_ZFUISizeToZFAndroidSize(ZF_IN const ZFUISize &size, ZF_IN_OUT jobject jobjSize);
// note returned value must be released by DeleteLocalRef
// note conversion may have performance issue
extern ZF_ENV_EXPORT jobject ZFImpl_sys_Android_ZFUISizeToZFAndroidSize(ZF_IN const ZFUISize &size);
extern ZF_ENV_EXPORT void ZFImpl_sys_Android_ZFUISizeFromZFAndroidSizeT(ZF_OUT ZFUISize &ret, ZF_IN jobject jobjSize);
inline ZFUISize ZFImpl_sys_Android_ZFUISizeFromZFAndroidSize(ZF_IN jobject jobjSize)
{
    ZFUISize ret = ZFUISizeZero();
    ZFImpl_sys_Android_ZFUISizeFromZFAndroidSizeT(ret, jobjSize);
    return ret;
}

// ============================================================
// Margin
#define ZFImpl_sys_Android_JNI_ID_ZFAndroidMargin ZFImpl_sys_Android_JNI_ID(NativeUtil_ZFAndroidMargin)
#define ZFImpl_sys_Android_JNI_NAME_ZFAndroidMargin ZFImpl_sys_Android_JNI_NAME(NativeUtil.ZFAndroidMargin)
extern ZF_ENV_EXPORT jclass ZFImpl_sys_Android_jclassZFAndroidMargin(void);
extern ZF_ENV_EXPORT jobject ZFImpl_sys_Android_ZFUIMarginToZFAndroidMargin(ZF_IN const ZFUIMargin &margin, ZF_IN_OUT jobject jobjMargin);
// note returned value must be released by DeleteLocalRef
// note conversion may have performance issue
extern ZF_ENV_EXPORT jobject ZFImpl_sys_Android_ZFUIMarginToZFAndroidMargin(ZF_IN const ZFUIMargin &margin);
extern ZF_ENV_EXPORT void ZFImpl_sys_Android_ZFUIMarginFromZFAndroidMarginT(ZF_OUT ZFUIMargin &ret, ZF_IN jobject jobjMargin);
inline ZFUIMargin ZFImpl_sys_Android_ZFUIMarginFromZFAndroidMargin(ZF_IN jobject jobjMargin)
{
    ZFUIMargin ret = ZFUIMarginZero();
    ZFImpl_sys_Android_ZFUIMarginFromZFAndroidMarginT(ret, jobjMargin);
    return ret;
}

// ============================================================
// Rect
#define ZFImpl_sys_Android_JNI_ID_ZFAndroidRect ZFImpl_sys_Android_JNI_ID(NativeUtil_ZFAndroidRect)
#define ZFImpl_sys_Android_JNI_NAME_ZFAndroidRect ZFImpl_sys_Android_JNI_NAME(NativeUtil.ZFAndroidRect)
extern ZF_ENV_EXPORT jclass ZFImpl_sys_Android_jclassZFAndroidRect(void);
extern ZF_ENV_EXPORT jobject ZFImpl_sys_Android_ZFUIRectToZFAndroidRect(ZF_IN const ZFUIRect &rect, ZF_IN_OUT jobject jobjRect);
// note returned value must be released by DeleteLocalRef
// note conversion may have performance issue
extern ZF_ENV_EXPORT jobject ZFImpl_sys_Android_ZFUIRectToZFAndroidRect(ZF_IN const ZFUIRect &rect);
extern ZF_ENV_EXPORT void ZFImpl_sys_Android_ZFUIRectFromZFAndroidRectT(ZF_OUT ZFUIRect &ret, ZF_IN jobject jobjRect);
inline ZFUIRect ZFImpl_sys_Android_ZFUIRectFromZFAndroidRect(ZF_IN jobject jobjRect)
{
    ZFUIRect ret = ZFUIRectZero();
    ZFImpl_sys_Android_ZFUIRectFromZFAndroidRectT(ret, jobjRect);
    return ret;
}

// ============================================================
// Color
extern ZF_ENV_EXPORT jint ZFImpl_sys_Android_ZFUIColorToColor(ZF_IN const ZFUIColor &color);
extern ZF_ENV_EXPORT void ZFImpl_sys_Android_ZFUIColorFromColorT(ZF_OUT ZFUIColor &ret, ZF_IN jint jColor);
inline ZFUIColor ZFImpl_sys_Android_ZFUIColorFromColor(ZF_IN jint jColor)
{
    ZFUIColor ret = ZFUIColorZero();
    ZFImpl_sys_Android_ZFUIColorFromColorT(ret, jColor);
    return ret;
}

// ============================================================
// ZFAndroidUI
#define ZFImpl_sys_Android_JNI_ID_ZFAndroidUI ZFImpl_sys_Android_JNI_ID(NativeUtil_ZFAndroidUI)
#define ZFImpl_sys_Android_JNI_NAME_ZFAndroidUI ZFImpl_sys_Android_JNI_NAME(NativeUtil.ZFAndroidUI)

extern ZF_ENV_EXPORT void ZFImpl_sys_Android_viewTreePrintT(ZF_OUT zfstring &ret, ZF_IN jobject nativeView);
inline zfstring ZFImpl_sys_Android_viewTreePrint(ZF_IN jobject nativeView)
{
    zfstring ret;
    ZFImpl_sys_Android_viewTreePrintT(ret, nativeView);
    return ret;
}

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_Android
#endif // #ifndef _ZFI_ZFImpl_sys_Android_ZFUIKit_impl_h_

