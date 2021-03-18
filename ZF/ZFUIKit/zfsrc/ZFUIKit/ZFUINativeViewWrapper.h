/**
 * @file ZFUINativeViewWrapper.h
 * @brief native view wrapper
 */

#ifndef _ZFI_ZFUINativeViewWrapper_h_
#define _ZFI_ZFUINativeViewWrapper_h_

#include "ZFUIView.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUINativeViewWrapper
/**
 * @brief view container to hold a native view
 *
 * used to hold a native view in ZFUIKit environment,
 * what you should do is create a #ZFUINativeViewWrapper
 * and store native view by #wrappedNativeView\n
 * the embeded native view would be layouted to fill #ZFUINativeViewWrapper's frame,
 * to change it's layout logic, you should use it as a #ZFUIView\n
 * the #ZFUINativeViewWrapper itself is serializable,
 * however the native view itself can not be serialized,
 * it should be initialized by app manually
 */
zffinal zfclass ZF_ENV_EXPORT ZFUINativeViewWrapper : zfextends ZFUIView
{
    ZFOBJECT_DECLARE(ZFUINativeViewWrapper, ZFUIView)

protected:
    /**
     * @brief init with native view, see #ZFUINativeViewWrapper
     */
    ZFOBJECT_ON_INIT_INLINE_1(ZFMP_IN(void *, wrappedNativeView))
    {
        this->objectOnInit();
        zfself::wrappedNativeView(wrappedNativeView);
    }

    zfoverride
    virtual void objectOnInit(void) {zfsuper::objectOnInit();}

protected:
    zfoverride
    virtual void objectInfoOnAppend(ZF_IN_OUT zfstring &ret);

public:
    /**
     * @brief see #ZFUINativeViewWrapper
     */
    ZFMETHOD_DECLARE_1(void, wrappedNativeView,
                       ZFMP_IN(void *, wrappedNativeView))
    /**
     * @brief see #ZFUINativeViewWrapper
     */
    ZFMETHOD_DECLARE_0(void *, wrappedNativeView)

    /**
     * @brief measure the native view
     */
    ZFMETHOD_DECLARE_2(void, measureNativeView,
                       ZFMP_OUT(ZFUISize &, ret),
                       ZFMP_IN_OPT(const ZFUISize &, sizeHint, ZFUISizeInvalid()))

protected:
    zfoverride
    virtual void layoutOnMeasure(ZF_OUT ZFUISize &ret,
                                 ZF_IN const ZFUISize &sizeHint,
                                 ZF_IN const ZFUISizeParam &sizeParam);
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUINativeViewWrapper_h_

