/**
 * @file ZFUIDraw.h
 * @brief global style for ui elements
 */

#ifndef _ZFI_ZFUIDraw_h_
#define _ZFI_ZFUIDraw_h_

#include "ZFUIView.h"
#include "ZFUIImage.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUIDrawableView
/**
 * @brief abstract view that can draw user content
 */
zfclass ZF_ENV_EXPORT ZFUIDrawableView : zfextends ZFUIView
{
    ZFOBJECT_DECLARE(ZFUIDrawableView, ZFUIView)

public:
    // ============================================================
    // events
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called to draw the contents,
     * see #ZFUIDrawableView
     */
    ZFOBSERVER_EVENT(OnDraw)

public:
    /**
     * @brief mark the view needs to redraw, see #ZFUIDrawableView
     */
    ZFMETHOD_DECLARE_0(void, drawRequest)

    /** @brief true if during #onDraw */
    ZFMETHOD_DECLARE_0(zfbool const &, drawing)

protected:
    /** @brief see #EventOnDraw */
    virtual inline void onDraw(void)
    {
        this->observerNotify(zfself::EventOnDraw());
    }
public:
    zffinal inline void _ZFP_ZFUIDrawableView_onDraw(void)
    {
        this->_drawing = zftrue;
        this->onDraw();
        this->_drawing = zffalse;
    }

protected:
    zfoverride
    virtual void objectOnInit(void);
private:
    zfbool _drawing;
};

ZF_NAMESPACE_BEGIN(ZFUIDraw)

// ============================================================
// context
/**
 * @brief begin a draw context
 */
ZFMETHOD_FUNC_DECLARE_1(void *, beginForView,
                        ZFMP_IN(ZFUIDrawableView *, view))
/** @brief see #ZFUIDraw::beginForView */
ZFMETHOD_FUNC_DECLARE_1(void, endForView,
                        ZFMP_IN(void *, context))

/** @brief see #ZFUIDraw::beginForView */
ZFMETHOD_FUNC_DECLARE_1(void *, beginForImage,
                        ZFMP_IN(const ZFUISize &, imageSizePixel))
/** @brief see #ZFUIDraw::beginForView */
ZFMETHOD_FUNC_DECLARE_1(zfautoObjectT<ZFUIImage *>, endForImage,
                        ZFMP_IN(void *, context))

// ============================================================
// settings
/** @brief whether antialiasing, false by default */
ZFMETHOD_FUNC_DECLARE_1(zfbool const &, antialiasing,
                        ZFMP_IN(void *, context))
/** @brief whether antialiasing, false by default */
ZFMETHOD_FUNC_DECLARE_2(void, antialiasing,
                        ZFMP_IN(void *, context),
                        ZFMP_IN(zfbool const &, value))

// ============================================================
// draw api
/** @brief see #ZFUIDraw::beginForView */
ZFMETHOD_FUNC_DECLARE_2(void, drawClear,
                        ZFMP_IN(void *, context),
                        ZFMP_IN_OPT(const ZFUIRect &, targetFramePixel, ZFUIRectZero()))

/** @brief see #ZFUIDraw::beginForView */
ZFMETHOD_FUNC_DECLARE_3(void, drawColor,
                        ZFMP_IN(void *, context),
                        ZFMP_IN(const ZFUIColor &, color),
                        ZFMP_IN_OPT(const ZFUIRect &, targetFramePixel, ZFUIRectZero()))

/** @brief see #ZFUIDraw::beginForView */
ZFMETHOD_FUNC_DECLARE_4(void, drawImage,
                        ZFMP_IN(void *, context),
                        ZFMP_IN(ZFUIImage *, image),
                        ZFMP_IN_OPT(const ZFUIRect &, imageFramePixel, ZFUIRectZero()),
                        ZFMP_IN_OPT(const ZFUIRect &, targetFramePixel, ZFUIRectZero()))

ZF_NAMESPACE_END(ZFUIDraw)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIDraw_h_

