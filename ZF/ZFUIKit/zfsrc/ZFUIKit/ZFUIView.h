/**
 * @file ZFUIView.h
 * @brief base class of all UI views
 */

#ifndef _ZFI_ZFUIView_h_
#define _ZFI_ZFUIView_h_

#include "ZFUIViewType.h"
#include "ZFUIMouseEvent.h"
#include "ZFUIKeyEvent.h"
#include "ZFUIWheelEvent.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUIView_internalImplView "internalImplView"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUIView_internalBgView "internalBgView"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUIView_internalFgView "internalFgView"

/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUIView_child "child"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUIView_layoutParam "layoutParam"

// ============================================================
zfclassFwd ZFUIView;
/**
 * @brief see #ZFUIView::nativeImplView
 */
typedef void (*ZFUIViewNativeImplViewDeleteCallback)(ZF_IN ZFUIView *view, ZF_IN void *nativeImplView);

// ============================================================
// ZFUIView
zfclassFwd _ZFP_ZFUIViewPrivate;
/**
 * @brief base class of all UI views
 *
 * ZFUIView has these layer of views:
 * -  internal impl view:
 *   for subclass to add views behind native impl view
 * -  internal native view:
 *   for impl to achieve different functions, internal use only
 * -  internal background view:
 *   for subclass to add background views
 * -  normal view:
 *   common children views
 * -  internal foreground view:
 *   for subclass to add foreground views
 *
 * all view layer is implemented internally by simple view management\n
 * \n
 * ZFUIView is serializable and styleable, see #ZFSerializable and #ZFStyleable for more info,
 * all property and normal children views would be serialized and styled automatically,
 * but internal views must be processed by subclass manually\n
 * \n
 * serializable data:
 * @code
 *   <ViewClass>
 *       // optional, see #internalViewAutoSerializeTagAdd
 *       <ChildClass category="internalImplView" >
 *       // optional, see #internalViewAutoSerializeTagAdd
 *       <ChildClass category="internalBgView" >
 *       </ChildClass>
 *       // optional, see #internalViewAutoSerializeTagAdd
 *       <ChildClass category="internalFgView" >
 *       </ChildClass>
 *
 *       <ChildClass category="child" >
 *           // layout param for parent, optional
 *           <LayoutParamClass category="layoutParam" ... />
 *       </ChildClass>
 *       ... // all children
 *   </ViewClass>
 * @endcode
 * by default, internal views won't be serialized automatically,
 * except matches these condition:
 * -  have the viewId of your internal view been set
 * -  have the same viewId registered by #internalViewAutoSerializeTagAdd
 *
 * \n
 * ADVANCED:\n
 * we allow add native view to ZFUIView environment,
 * for how to, refer to #ZFUINativeViewWrapper\n
 * we also allow add ZFUIView to native view,
 * for how to, refer to #ZFUISysWindow::nativeWindowEmbed
 */
zfclass ZF_ENV_EXPORT ZFUIView : zfextends ZFStyleableObject
{
    ZFOBJECT_DECLARE(ZFUIView, ZFStyleableObject)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIView)

public:
    // ============================================================
    // events
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when child added or removed or order changed,
     * may be normal child or internal child
     */
    ZFOBSERVER_EVENT(ViewChildOnChange)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when child added to this view,
     * param0 is the child,
     * param1 is #ZFUIViewChildLayer
     */
    ZFOBSERVER_EVENT(ViewChildOnAdd)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when child removed from this view,
     * param0 is the child,
     * param1 is #ZFUIViewChildLayer
     */
    ZFOBSERVER_EVENT(ViewChildOnRemove)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when this view added to parent,
     * param0 is the parent added to
     */
    ZFOBSERVER_EVENT(ViewOnAddToParent)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * param0 is the parent removed from
     */
    ZFOBSERVER_EVENT(ViewOnRemoveFromParent)
    /**
     * @brief see #ZFObject::observerNotify
     */
    ZFOBSERVER_EVENT(ViewScaleOnChange)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when view's focus state changed, both obtain or resign
     */
    ZFOBSERVER_EVENT(ViewFocusOnChange)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * param0 is the #ZFUIEvent
     */
    ZFOBSERVER_EVENT(ViewOnEvent)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when #layoutRequest called
     */
    ZFOBSERVER_EVENT(ViewLayoutOnLayoutRequest)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * param0 is a #ZFUIViewMeasureResult,
     * you may change the measured size to override the measure result
     */
    ZFOBSERVER_EVENT(ViewLayoutOnMeasureFinish)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * #viewFrame would be updated before this method,
     * use #viewFramePrev if necessary,
     * you may safely modify children's #layoutParam during this method
     */
    ZFOBSERVER_EVENT(ViewLayoutOnLayoutPrepare)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called to do actual layout steps
     */
    ZFOBSERVER_EVENT(ViewLayoutOnLayout)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when layout finished,
     * typically you should not modify #layoutParam during this event
     */
    ZFOBSERVER_EVENT(ViewLayoutOnLayoutFinish)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when #nativeImplViewMarginUpdate and value differs from old
     */
    ZFOBSERVER_EVENT(NativeImplViewMarginOnUpdate)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * see #viewPropertyUpdateRequest
     */
    ZFOBSERVER_EVENT(ViewPropertyOnUpdate)

    // ============================================================
    // serialize
public:
    /**
     * @brief store ref layout param for this view for reducing serialization output size
     *
     * if set, while serializing this view's layout param,
     * the ref one would be used as reference object to filter out contents that didn't change
     * (see #ZFSerializable::serializeToData)\n
     * by default, all children would have it's parent's default layout param (#layoutParamCreate)
     * as the ref layout param, during adding to parent
     */
    virtual void serializableRefLayoutParam(ZF_IN ZFUILayoutParam *serializableRefLayoutParam);
    /** @brief see #serializableRefLayoutParam */
    virtual ZFUILayoutParam *serializableRefLayoutParam(void);
protected:
    zfoverride
    virtual zfbool serializableOnSerializeFromData(ZF_IN const ZFSerializableData &serializableData,
                                                   ZF_OUT_OPT zfstring *outErrorHint = zfnull,
                                                   ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull);
    zfoverride
    virtual zfbool serializableOnSerializeToData(ZF_IN_OUT ZFSerializableData &serializableData,
                                                 ZF_IN ZFSerializable *referencedOwnerOrNull,
                                                 ZF_OUT_OPT zfstring *outErrorHint = zfnull);
    /**
     * @brief whether we should serialize all children
     *
     * by default, #ZFUIView would serialize all normal child views,
     * for some adapter view it may be not necessary,
     * you may override this method to disable the auto serialization of child views
     */
    virtual inline zfbool serializableOnCheckNeedSerializeChildren(void)
    {
        return zftrue;
    }

public:
    // ============================================================
    // properties
    /**
     * @brief used to identify a view, empty by default
     *
     * this is useful when you want to find a view from a complicated view tree,
     * see #ZFUIView::childFindById
     * @note it's OK that two view have same view id,
     *   however it's recommended to make it unique
     */
    ZFPROPERTY_ASSIGN(zfstring, viewId)

    /**
     * @brief visible or not, zftrue by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfbool, viewVisible,
                                zftrue)
    ZFPROPERTY_OVERRIDE_ON_ATTACH_DECLARE(zfbool, viewVisible)

    /**
     * @brief view's alpha, 1 by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zffloat, viewAlpha, 1)
    ZFPROPERTY_OVERRIDE_ON_VERIFY_DECLARE(zffloat, viewAlpha)
    ZFPROPERTY_OVERRIDE_ON_ATTACH_DECLARE(zffloat, viewAlpha)

    /**
     * @brief whether the view should receive user interaction
     *   (doesn't affect children, see #viewUIEnableTree)
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfbool, viewUIEnable,
                                zftrue)
    ZFPROPERTY_OVERRIDE_ON_ATTACH_DECLARE(zfbool, viewUIEnable)

    /**
     * @brief whether the view as well as all its children should receive user interaction,
     *   see #viewUIEnable
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfbool, viewUIEnableTree,
                                zftrue)
    ZFPROPERTY_OVERRIDE_ON_ATTACH_DECLARE(zfbool, viewUIEnableTree)

    /**
     * @brief whether enable mouse hover event, see #ZFUIView::viewEventOnMouseEvent, false by default
     */
    ZFPROPERTY_ASSIGN(zfbool, viewMouseHoverEventEnable)
    ZFPROPERTY_OVERRIDE_ON_ATTACH_DECLARE(zfbool, viewMouseHoverEventEnable)

    /**
     * @brief whether the view can be focused, false by default
     */
    ZFPROPERTY_ASSIGN(zfbool, viewFocusable)
    ZFPROPERTY_OVERRIDE_ON_ATTACH_DECLARE(zfbool, viewFocusable)
    /**
     * @brief whether try to obtain focus when clicked down, true by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfbool, viewFocusObtainWhenClick,
                                zftrue)

    /**
     * @brief the view's frame
     *
     * typicall, this property would be updated automatically by parent's #layoutOnLayout,
     * but you may also change this property manually,
     * for example, to achieve custom animation logic,
     * if you do so, the frame would be kept
     * and parent's layout logic would be ignored,
     * until you call #viewFrameReset
     */
    ZFMETHOD_DECLARE_0(ZFUIRect const &, viewFrame)
    /** @brief see #viewFrame */
    ZFMETHOD_DECLARE_1(void, viewFrame,
                       ZFMP_IN(ZFUIRect const &, viewFrame))

    /** @brief previous #viewFrame */
    ZFMETHOD_DECLARE_0(const ZFUIRect &, viewFramePrev)

    /** @brief see #viewFrame */
    ZFMETHOD_DECLARE_0(void, viewFrameReset)

    /** @brief see #viewFrame */
    ZFMETHOD_DECLARE_0(zfint const &, viewX)
    /** @brief see #viewFrame */
    ZFMETHOD_DECLARE_1(void, viewX, ZFMP_IN(zfint const &, propertyValue))
    /** @brief see #viewFrame */
    ZFMETHOD_DECLARE_0(zfint const &, viewY)
    /** @brief see #viewFrame */
    ZFMETHOD_DECLARE_1(void, viewY, ZFMP_IN(zfint const &, propertyValue))
    /** @brief see #viewFrame */
    ZFMETHOD_DECLARE_0(zfint const &, viewWidth)
    /** @brief see #viewFrame */
    ZFMETHOD_DECLARE_1(void, viewWidth, ZFMP_IN(zfint const &, propertyValue))
    /** @brief see #viewFrame */
    ZFMETHOD_DECLARE_0(zfint const &, viewHeight)
    /** @brief see #viewFrame */
    ZFMETHOD_DECLARE_1(void, viewHeight, ZFMP_IN(zfint const &, propertyValue))
    /** @brief see #viewFrame */
    ZFMETHOD_DECLARE_0(zfint const &, viewCenterX)
    /** @brief see #viewFrame */
    ZFMETHOD_DECLARE_1(void, viewCenterX, ZFMP_IN(zfint const &, propertyValue))
    /** @brief see #viewFrame */
    ZFMETHOD_DECLARE_0(zfint const &, viewCenterY)
    /** @brief see #viewFrame */
    ZFMETHOD_DECLARE_1(void, viewCenterY, ZFMP_IN(zfint const &, propertyValue))

    /**
     * @brief prefered size, #ZFUISizeInvalid by default
     *
     * if no actual rule to measure the view,
     * the prefered size would be used to measure the view,
     * if prefered size not set, size hint would be used
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUISize, viewSizePrefer, ZFUISizeInvalid())
    ZFPROPERTY_OVERRIDE_ON_ATTACH_DECLARE(ZFUISize, viewSizePrefer)
    /**
     * @brief min size, #ZFUISizeZero by default
     */
    ZFPROPERTY_ASSIGN(ZFUISize, viewSizeMin)
    ZFPROPERTY_OVERRIDE_ON_ATTACH_DECLARE(ZFUISize, viewSizeMin)
    /**
     * @brief max size, negative value means not set, #ZFUISizeInvalid by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUISize, viewSizeMax,
                                ZFUISizeInvalid())
    ZFPROPERTY_OVERRIDE_ON_ATTACH_DECLARE(ZFUISize, viewSizeMax)

    /**
     * @brief background color, #ZFUIColorZero by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIColor, viewBackgroundColor,
                                ZFUIColorZero())
    ZFPROPERTY_OVERRIDE_ON_ATTACH_DECLARE(ZFUIColor, viewBackgroundColor)

    // ============================================================
    // init and dealloc
protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);
    zfoverride
    virtual void objectOnInitFinish(void);
    zfoverride
    virtual void objectOnDeallocPrepare(void);

public:
    zfoverride
    virtual zfidentity objectHash(void);
    zfoverride
    virtual ZFCompareResult objectCompare(ZF_IN ZFObject *anotherObj);

protected:
    zfoverride
    virtual void objectInfoOnAppend(ZF_IN_OUT zfstring &ret);

    // ============================================================
public:
    /**
     * @brief native implementation view
     *
     * first child of #nativeView, layout below all other child views,
     * for internal implementation use only,
     * for example, a ImageView's implementation may use native view
     * and set it as internalNativeView\n
     * subclass must not override parent,
     * if really necessary, use #internalBgViewAdd\n
     * use with caution
     * @see nativeView
     */
    ZFMETHOD_DECLARE_0(void *, nativeImplView)
    /**
     * @brief inner margin between #ZFUIView and the internal #nativeImplView
     *
     * use #nativeImplViewMarginUpdate to update this value,
     * and it's ensured to be called during #ZFObject::objectOnInitFinish\n
     * subclass should override #nativeImplViewMarginImplUpdate to implement custom margin,
     * and manually call #nativeImplViewMarginUpdate if necessary\n
     * this value can also be controlled by app level code by #nativeImplViewMarginCustom
     */
    ZFMETHOD_DECLARE_0(const ZFUIMargin &, nativeImplViewMargin)
    /**
     * @brief see #nativeImplViewMargin,
     *   #layoutRequest if the final value actually changed
     */
    ZFMETHOD_DECLARE_0(void, nativeImplViewMarginUpdate)
    /**
     * @brief frame of #nativeImplView
     */
    ZFMETHOD_DECLARE_0(const ZFUIRect &, nativeImplViewFrame)

    /**
     * @brief see #nativeImplViewMargin, #ZFUIMarginZero by default
     */
    ZFPROPERTY_ASSIGN(ZFUIMargin, nativeImplViewMarginCustom)
    ZFPROPERTY_OVERRIDE_ON_ATTACH_DECLARE(ZFUIMargin, nativeImplViewMarginCustom)

protected:
    /**
     * @brief see #nativeImplView
     */
    zffinal void nativeImplView(ZF_IN void *nativeImplView,
                                ZF_IN ZFUIViewNativeImplViewDeleteCallback nativeImplViewDeleteCallback);
    /**
     * @brief see #nativeImplViewMargin,
     *   subclass must call super and "append" to existing margin
     */
    virtual inline void nativeImplViewMarginImplUpdate(ZF_IN_OUT ZFUIMargin &nativeImplViewMargin)
    {
    }
    /**
     * @brief see #EventNativeImplViewMarginOnUpdate
     */
    virtual inline void nativeImplViewMarginOnUpdate(void)
    {
        this->observerNotify(ZFUIView::EventNativeImplViewMarginOnUpdate());
    }
    /**
     * @brief called to layout #nativeImplView
     */
    virtual void nativeImplViewOnLayout(ZF_OUT ZFUIRect &ret,
                                        ZF_IN const ZFUIRect &bounds,
                                        ZF_IN const ZFUIMargin &nativeImplViewMargin)
    {
        ZFUIRectApplyMargin(ret, bounds, nativeImplViewMargin);
    }

    // ============================================================
protected:
    /**
     * @brief called to add or remove view to impl
     *
     * subclass may override this method to add child to other container\n
     * to implement this, you must implement all these methods:
     * -  #implChildOnAdd : called to add to impl
     * -  #implChildOnRemove : called to remove from impl
     * -  #implChildOnRemoveAllForDealloc : called to remove from impl during parent dealloc
     */
    virtual void implChildOnAdd(ZF_IN ZFUIView *child,
                                ZF_IN zfindex virtualIndex,
                                ZF_IN ZFUIViewChildLayerEnum childLayer,
                                ZF_IN zfindex childLayerIndex);
    /**
     * @brief see #implChildOnAdd, #implChildOnRemoveAllForDealloc
     */
    virtual void implChildOnRemove(ZF_IN ZFUIView *child,
                                   ZF_IN zfindex virtualIndex,
                                   ZF_IN ZFUIViewChildLayerEnum childLayer,
                                   ZF_IN zfindex childLayerIndex);
    /**
     * @brief called to remove all children during parent dealloc for performance
     *
     * for normal children management,
     * each child would be removed one by one and fire child change event when #childRemoveAll,
     * which is not necessary during parent dealloc
     * (which may cause performance issue),
     * so we use this method to remove all children directly during parent dealloc
     * to improve performance
     */
    virtual void implChildOnRemoveAllForDealloc(void);

public:
    /**
     * @brief native container view
     *
     * the actual type of this is defined by implementation,
     * and it's not recommended to use in your application\n
     * \n
     * for how to add ZFUIView to native view, please refer to #ZFUIView\n
     * for how to add native view to ZFUIView, please refer to #ZFUINativeViewWrapper\n
     * for how to access native implementation, please refer to #nativeImplView
     */
    ZFMETHOD_DECLARE_0(void *, nativeView)
    static void _ZFP_ZFUIView_nativeViewNotifyAdd(ZF_IN ZFUIView *view, ZF_IN void *nativeParentView);
    static void _ZFP_ZFUIView_nativeViewNotifyRemove(ZF_IN ZFUIView *view);

    // ============================================================
    // view focus
public:
    /**
     * @brief whether the view currently focused
     */
    ZFMETHOD_DECLARE_0(zfbool, viewFocused)
    /**
     * @brief request to obtain or resign focus,
     *   result can be checked by #viewFocused
     *
     * only focused view can receive key events
     */
    ZFMETHOD_DECLARE_1(void, viewFocusRequest,
                       ZFMP_IN(zfbool, viewFocus))
    zffinal void _ZFP_ZFUIView_viewFocusOnChange(void)
    {
        this->viewFocusOnChange();
    }
    /**
     * @brief recursively to find focused child, take care of performance
     */
    ZFMETHOD_DECLARE_0(ZFUIView *, viewFocusFind)
protected:
    /** @brief see #EventViewFocusOnChange */
    virtual inline void viewFocusOnChange(void)
    {
        this->observerNotify(ZFUIView::EventViewFocusOnChange());
    }

    // ============================================================
    // parent
public:
    zffinal void _ZFP_ZFUIView_parentChanged(ZF_IN ZFUIView *parentView,
                                             ZF_IN ZFUILayoutParam *layoutParam,
                                             ZF_IN ZFUIViewChildLayerEnum viewLayer);
    /**
     * @brief parent view or null if none
     */
    ZFMETHOD_DECLARE_0(ZFUIView *, viewParent)

    /**
     * @brief remove this view from parent or do nothing if no parent
     *
     * can remove normal child view or internal view
     */
    ZFMETHOD_DECLARE_0(void, viewRemoveFromParent)

    // ============================================================
    // scale settings
public:
    /**
     * @brief scale for view
     *
     * ZFUIView use a special scale logic to adapt various screen sizes,
     * which contain these scale values:
     * -  app scale:\n
     *   accessed by #scaleForApp, 1 by default\n
     *   automatically changed while adding a ZFUIView to another,
     *   and can only be changed by app by using #ZFUIRootView::scaleForApp of #ZFUISysWindow
     * -  impl scale:\n
     *   accessed by #scaleForImpl, it's value depends on impl\n
     *   automatically changed while adding a ZFUIView to native view,
     *   can not be changed by app
     * -  fixed scale:\n
     *   accessed by #scaleFixed, always equal to (#scaleForApp * #scaleForImpl)\n
     *   all size unit would be applied with this value
     *   before passing to implementations,
     *   can not be changed by app
     * -  impl physical scale:\n
     *   accessedb y #scaleForImplPhysicalPixel, it's value depends on impl\n
     *   corresponding to #scaleForImpl,
     *   used only if you want to verify physical pixel
     *   (since impl may have it's own scale logic)
     *
     * in general:
     * -  for app:\n
     *   usually you have no need to worry about scales,
     *   all elements can be assigned as fixed size,
     *   such as 48 for a button's height and 21 for a small textView's height,
     *   it will be scaled automatically while rendering to different size's or DPI's devices\n
     *   if you really want to change scale setting,
     *   you should use #ZFUIRootView::scaleForApp of #ZFUISysWindow
     * -  for implementation:\n
     *   you have no need to worry about element's logical size,
     *   everything would be scaled to desired size unit
     *   depending on the scale value that impl returned
     *
     * \n
     * since scale may affect impl's pixel size,
     * size-related property should be flushed manually while scale changed,
     * subclass should override #scaleOnChange to update them,
     * which would be called if #scaleFixed really changed
     */
    ZFMETHOD_DECLARE_0(zffloat, scaleForApp)
    /**
     * @brief see #scaleForApp
     */
    ZFMETHOD_DECLARE_0(zffloat, scaleForImpl)
    /**
     * @brief see #scaleForApp
     */
    ZFMETHOD_DECLARE_0(zffloat, scaleForImplPhysicalPixel)
    /**
     * @brief see #scaleForApp
     */
    ZFMETHOD_DECLARE_0(zffloat, scaleFixed)
protected:
    /**
     * @brief see #scaleForApp, ensured called only when scale value actually changed
     *
     * after this method, #EventViewScaleOnChange would be fired
     */
    virtual void scaleOnChange(void);
public:
    void _ZFP_ZFUIView_scaleSetRecursively(ZF_IN zffloat scaleFixed,
                                           ZF_IN zffloat scaleForImpl);

    // ============================================================
    // layout logic
public:
    /**
     * @brief create layout param,
     *   calling #layoutParamClass to create instance
     *   and #layoutParamOnUpdate to update
     */
    ZFMETHOD_DECLARE_0(zfautoObjectT<ZFUILayoutParam *>, layoutParamCreate)
protected:
    /**
     * @brief see #layoutParamCreate
     *
     * you should override this method to declare your layout param class
     */
    virtual const ZFClass *layoutParamClass(void);
    /**
     * @brief see #layoutParamCreate
     */
    virtual inline void layoutParamOnUpdate(ZF_IN ZFUILayoutParam *layoutParam)
    {
    }

public:
    /**
     * @brief manually set layout param
     *
     * this method can be called even if this view has no parent,
     * the layout param would be serialized while serializing the view itself\n
     * while adding to another container view with different layout param type,
     * a new layout param would be created and applied style from the existing one
     */
    ZFMETHOD_DECLARE_1(void, layoutParam,
                       ZFMP_IN(ZFUILayoutParam *, layoutParam))
    /**
     * @brief get self's layout param, valid only while the view has parent
     *
     * return null if the view has no parent,
     * automatically invoke the view's #layoutRequest if the layout param's property changed
     */
    ZFMETHOD_DECLARE_0(ZFUILayoutParam *, layoutParam)
    /**
     * @brief see #layoutParam
     */
    template<typename T_LayoutParam>
    T_LayoutParam layoutParam(void)
    {
        return ZFCastZFObjectUnchecked(T_LayoutParam, this->layoutParam());
    }

public:
    /**
     * @brief set need layout
     */
    ZFMETHOD_DECLARE_0(void, layoutRequest)
    /**
     * @brief true if need layout
     */
    ZFMETHOD_DECLARE_0(zfbool, layoutRequested)
    /**
     * @brief true if currently being layouted
     */
    ZFMETHOD_DECLARE_0(zfbool, layouting)
    /**
     * @brief measure the view
     *
     * call #layoutOnMeasure to see the needed size for this view\n
     * note that internal views won't be measured
     * @see layoutMeasuredSize
     */
    ZFMETHOD_DECLARE_2(const ZFUISize &, layoutMeasure,
                       ZFMP_IN(const ZFUISize &, sizeHint),
                       ZFMP_IN(const ZFUISizeParam &, sizeParam))
    /**
     * @brief get measured size, invalid if not measured
     */
    ZFMETHOD_DECLARE_0(const ZFUISize &, layoutMeasuredSize)
    /**
     * @brief force to layout if need
     */
    ZFMETHOD_DECLARE_0(void, layoutIfNeed)
    /**
     * @brief get child offset to this view
     *
     * for views that have offset logic (typically scroll views),
     * use this method to access the offset to its parent,
     * child's #viewFrame plus this offset should be the actual
     * offset to parent's edge\n
     * subclass should override #layoutChildOffsetOnUpdate
     * to supply this value
     */
    ZFMETHOD_DECLARE_0(ZFUIPoint, layoutChildOffset)

public:
    void _ZFP_ZFUIView_notifyLayoutView(ZF_IN const ZFUIRect &viewFrame);
protected:
    /**
     * @brief called during #layoutRequest
     */
    virtual void layoutOnLayoutRequest(void);
    /**
     * @brief called by #layoutMeasure to decide the view's size
     *
     * you may override without call super to supply your own layout logic\n
     * \n
     * note that we doesn't ensure layoutOnMeasure would be called during layout steps,
     * only layout that has wrap content features may call #layoutMeasure
     * to calculate children's size\n
     * \n
     * sizeHint means the max size child should reach, see #ZFUISizeType for more info\n
     * \n
     * return a negative value means the view doesn't care about size,
     * let #ZFUIView::viewSizePrefer to decide final result size\n
     * \n
     * @note there's some repeatly work that #layoutMeasure would have done for you,
     *   you should not repeat it again for performance:
     *   -  filter out the case that both sizeParam are fixed or fill parent
     *   -  apply sizeHint by #ZFUILayoutParam::sizeHintApply
     *   -  fix result size in range [viewSizeMin, viewSizeMax]
     * @note for impl views (such as text view),
     *   you should manually take care of #nativeImplViewMargin
     *   during measure and layout steps
     */
    virtual inline void layoutOnMeasure(ZF_OUT ZFUISize &ret,
                                        ZF_IN const ZFUISize &sizeHint,
                                        ZF_IN const ZFUISizeParam &sizeParam)
    {
    }
    /** @brief see #EventViewLayoutOnMeasureFinish */
    virtual inline void layoutOnMeasureFinish(ZF_IN_OUT ZFUISize &measuredSize,
                                              ZF_IN const ZFUISize &sizeHint,
                                              ZF_IN const ZFUISizeParam &sizeParam)
    {
    }
    /** @brief see #EventViewLayoutOnLayoutPrepare */
    virtual inline void layoutOnLayoutPrepare(ZF_IN const ZFUIRect &bounds)
    {
    }
    /**
     * @brief called by #viewFrame to layout the view and children
     *
     * it's valid for subclass to override without calling zfsuper::layoutOnLayout,
     * which means subclass would override all layout steps in parent\n
     * \n
     * note that we doesn't ensure layoutOnMeasure would be called before layoutOnLayout
     */
    virtual void layoutOnLayout(ZF_IN const ZFUIRect &bounds);
    /** @brief see #EventViewLayoutOnLayoutFinish */
    virtual inline void layoutOnLayoutFinish(ZF_IN const ZFUIRect &bounds)
    {
    }
    /** @brief see #layoutChildOffset */
    virtual inline void layoutChildOffsetOnUpdate(ZF_IN_OUT ZFUIPoint &ret)
    {
    }

    // ============================================================
    // children management
public:
    /**
     * @brief find view by viewId, return the view or null if not found
     */
    ZFMETHOD_DECLARE_3(ZFUIView *, childFindById,
                       ZFMP_IN(const zfchar *, viewId),
                       ZFMP_IN_OPT(zfbool, findRecursively, zftrue),
                       ZFMP_IN_OPT(zfbool, includeInternalViews, zffalse))

public:
    /**
     * @brief add view with layout param, param must be created by #layoutParamCreate
     *
     * if layoutParam is null (by default), create new one by #layoutParamCreate\n
     * if layoutParam is type of #layoutParamClass and not null, it would be used directly,
     * otherwise, a new layout param would be created
     * and source layout param would be copied to the newly created layout param
     */
    ZFMETHOD_DECLARE_3(void, childAdd,
                       ZFMP_IN(ZFUIView *, view),
                       ZFMP_IN_OPT(ZFUILayoutParam *, layoutParam, zfnull),
                       ZFMP_IN_OPT(zfindex, atIndex, zfindexMax()))
    /**
     * @brief util method for #childAdd
     */
    ZFMETHOD_DECLARE_3(void, childAdd,
                       ZFMP_IN(ZFUIView *, view),
                       ZFMP_IN(const ZFUISizeParam &, sizeParam),
                       ZFMP_IN_OPT(ZFUIAlignFlags const &, layoutAlign, ZFUIAlign::e_LeftInner | ZFUIAlign::e_TopInner))
    /**
     * @brief remove view or do nothing if view isn't added to this view
     */
    ZFMETHOD_DECLARE_1(void, childRemove,
                       ZFMP_IN(ZFUIView *, view))
    /**
     * @brief remove view at index or assert fail if index out of range
     */
    ZFMETHOD_DECLARE_1(void, childRemoveAtIndex,
                       ZFMP_IN(zfindex, index))
    /**
     * @brief remove all child view
     */
    ZFMETHOD_DECLARE_0(void, childRemoveAll)
    /**
     * @brief move view, make toIndexOrIndexMax as zfindexMax() to move to top most,
     *   and 0 to bottom most, do nothing if index invalid or have no change
     *
     * assert fail if fromIndex out of range, or toIndexOrIndexMax isn't zfindexMax() and out of range\n
     * moving a view would cause #layoutRequest being called
     */
    ZFMETHOD_DECLARE_2(void, childMove,
                       ZFMP_IN(zfindex, fromIndex),
                       ZFMP_IN(zfindex, toIndexOrIndexMax))
    /**
     * @brief see #childMove
     */
    ZFMETHOD_DECLARE_2(void, childMove,
                       ZFMP_IN(ZFUIView *, child),
                       ZFMP_IN(zfindex, toIndexOrIndexMax))
    /**
     * @brief replace child at index, assert fail if index out of range
     *   or view to replace already has parent
     */
    ZFMETHOD_DECLARE_2(void, childReplaceAtIndex,
                       ZFMP_IN(zfindex, atIndex),
                       ZFMP_IN(ZFUIView *, toReplace))
    /**
     * @brief get child view count
     */
    ZFMETHOD_DECLARE_0(zfindex, childCount)
    /**
     * @brief get child view at index or assert fail if out of range
     */
    ZFMETHOD_DECLARE_1(ZFUIView *, childAtIndex,
                       ZFMP_IN(zfindex, index))
    /**
     * @brief return index of view or zfindexMax() if not child of this view
     */
    ZFMETHOD_DECLARE_1(zfindex, childFind,
                       ZFMP_IN(ZFUIView *, view))
    /**
     * @brief get the child view array
     */
    ZFMETHOD_DECLARE_0(ZFCoreArrayPOD<ZFUIView *>, childArray)

    /**
     * @brief this view belongs to which layer of parent,
     *   valid only if #viewParent is not null
     *
     * would be #ZFUIViewChildLayer::e_Normal if no parent
     */
    ZFMETHOD_DECLARE_0(ZFUIViewChildLayerEnum, viewLayer)
    /**
     * @brief return all children including internal views, see #childArray
     *
     * children are ensured ordered by (impl, bg, normal, fg) views
     */
    ZFMETHOD_DECLARE_0(ZFCoreArrayPOD<ZFUIView *>, childRawArray)

    // ============================================================
    // events
protected:
    /** @brief see #EventViewChildOnChange */
    virtual void viewChildOnChange(void);
    /** @brief see #EventViewChildOnAdd */
    virtual void viewChildOnAdd(ZF_IN ZFUIView *child,
                                ZF_IN ZFUIViewChildLayerEnum layer);
    /** @brief see #EventViewChildOnRemove */
    virtual void viewChildOnRemove(ZF_IN ZFUIView *child,
                                   ZF_IN ZFUIViewChildLayerEnum layer);
    /** @brief see #EventViewOnAddToParent */
    virtual void viewOnAddToParent(ZF_IN ZFUIView *parent);
    /** @brief see #EventViewOnRemoveFromParent */
    virtual void viewOnRemoveFromParent(ZF_IN ZFUIView *parent);

    // ============================================================
    // internal impl views
public:
    /** @brief see #internalBgViewAdd */
    ZFMETHOD_DECLARE_3(void, internalImplViewAdd,
                       ZFMP_IN(ZFUIView *, view),
                       ZFMP_IN_OPT(ZFUILayoutParam *, layoutParam, zfnull),
                       ZFMP_IN_OPT(zfbool, addAsTopMost, zftrue))
    /** @brief see #internalBgViewAdd */
    ZFMETHOD_DECLARE_1(void, internalImplViewRemove,
                       ZFMP_IN(ZFUIView *, view))
    /** @brief see #internalBgViewAdd */
    ZFMETHOD_DECLARE_0(ZFCoreArrayPOD<ZFUIView *>, internalImplViewArray)
protected:
    /** @brief see #internalBgViewAdd */
    virtual void internalImplViewOnLayout(ZF_IN const ZFUIRect &bounds);

    // ============================================================
    // internal background views
public:
    /**
     * @brief internal view which is independent from normal view
     *
     * we have these layers in ZFUIView module:
     * -  internal impl view
     * -  internal background view
     * -  normal view
     * -  internal foreground view
     *
     * each layer is independent and has the same interface to add or remove view\n
     * to make the interfaces cleaner, the internal ones are named with
     * "internalBgView" and "internalFgView" as pre-fix,
     * such as #internalBgViewAdd and #internalBgViewRemove
     * (considering the #childAdd and #childRemove)\n
     * \n
     * internal views has no measure steps, its size always depends on parent's size\n
     * \n
     * for safety, internal views has no access methods related to index
     * subclass should save internal views as member and access manually\n
     * also, subclass must call super's method when override internalXXX
     */
    ZFMETHOD_DECLARE_3(void, internalBgViewAdd,
                       ZFMP_IN(ZFUIView *, view),
                       ZFMP_IN_OPT(ZFUILayoutParam *, layoutParam, zfnull),
                       ZFMP_IN_OPT(zfbool, addAsTopMost, zftrue))
    /** @brief see #internalBgViewAdd */
    ZFMETHOD_DECLARE_1(void, internalBgViewRemove,
                       ZFMP_IN(ZFUIView *, view))
    /**
     * @brief usually for debug use only, try to avoid use this in your app for other purpose
     */
    ZFMETHOD_DECLARE_0(ZFCoreArrayPOD<ZFUIView *>, internalBgViewArray)
protected:
    /** @brief see #internalBgViewAdd */
    virtual void internalBgViewOnLayout(ZF_IN const ZFUIRect &bounds);

    // ============================================================
    // internal foreground views
public:
    /** @brief see #internalBgViewAdd */
    ZFMETHOD_DECLARE_3(void, internalFgViewAdd,
                       ZFMP_IN(ZFUIView *, view),
                       ZFMP_IN_OPT(ZFUILayoutParam *, layoutParam, zfnull),
                       ZFMP_IN_OPT(zfbool, addAsTopMost, zftrue))
    /** @brief see #internalBgViewAdd */
    ZFMETHOD_DECLARE_1(void, internalFgViewRemove,
                       ZFMP_IN(ZFUIView *, view))
    /** @brief see #internalBgViewAdd */
    ZFMETHOD_DECLARE_0(ZFCoreArrayPOD<ZFUIView *>, internalFgViewArray)
    /** @brief see #internalBgViewAdd */
    virtual void internalFgViewOnLayout(ZF_IN const ZFUIRect &bounds);

    // ============================================================
    // other internal view logic
public:
    /**
     * @brief used to apply auto serialize logic to internal views
     *
     * by default, internal views won't be serialized automatically,
     * you may make it available by:
     * -  have internal view's viewId set
     * -  have same id set by this method
     *
     * while serializing, if an internal view with same id already exists,
     * we would copy style from serialize data to the existing one instead
     */
    ZFMETHOD_DECLARE_1(void, internalViewAutoSerializeTagAdd,
                       ZFMP_IN(const zfchar *, tag))
    /** @brief see #internalViewAutoSerializeTagAdd */
    ZFMETHOD_DECLARE_1(void, internalViewAutoSerializeTagRemove,
                       ZFMP_IN(const zfchar *, tag))
    /** @brief see #internalViewAutoSerializeTagAdd */
    ZFMETHOD_DECLARE_0(void, internalViewAutoSerializeTagRemoveAll)
    /** @brief see #internalViewAutoSerializeTagAdd */
    ZFMETHOD_DECLARE_1(void, internalViewAutoSerializeTagGetAllT,
                       ZFMP_IN_OUT(ZFCoreArray<zfstring> &, ret))
    /** @brief see #internalViewAutoSerializeTagAdd */
    ZFMETHOD_DECLARE_0(ZFCoreArray<zfstring>, internalViewAutoSerializeTagGetAll)

protected:
    /**
     * @brief called to check whether the internal view should be layouted using default layout logic,
     *   return true by default
     */
    virtual inline zfbool internalViewShouldLayout(ZF_IN ZFUIView *internalView)
    {
        return zftrue;
    }

    // ============================================================
    // UI events
public:
    /**
     * @brief directly send a event, use with caution
     */
    ZFMETHOD_DECLARE_1(void, viewEventSend,
                       ZFMP_IN(ZFUIEvent *, event))
protected:
    /**
     * @brief notified when a ZFUIEvent occurred
     *
     * default behavior is to dispatch event depends on event type\n
     * you may override without call super's method, to override the event\n
     * you should update #ZFUIEvent::eventResolved if resolved
     */
    virtual void viewEventOnEvent(ZF_IN ZFUIEvent *event);

    // ============================================================
    // events
protected:
    /**
     * @brief called when mouse event occurred
     *
     * due to some limitations, we doesn't support intercept mouse event,
     * event dispatch logic depends on implementation,
     * you may use native view to achieve if necessary\n
     * \n
     * mouse hover event would only be fired if #viewMouseHoverEventEnable\n
     * \n
     * by default, this method would simply resolve the event if this view is enabled,
     * you may override without call super's method, to override the event
     */
    virtual void viewEventOnMouseEvent(ZF_IN ZFUIMouseEvent *mouseEvent);

    /**
     * @brief called when key occurred
     *
     * due to some limitations, we doesn't support intercept key event,
     * event dispatch logic depends on implementation,
     * you may use native view to achieve if necessary\n
     * \n
     * by default, this method would call #viewEventOnKeyEventResolveFocus to achieve focus move,
     * you may override without call super's method, to override the event\n
     * the event would be dispatched from child to parent,
     * util it's resolved
     */
    virtual void viewEventOnKeyEvent(ZF_IN ZFUIKeyEvent *keyEvent);
    /**
     * @brief called by #viewEventOnKeyEvent to resolve focus move key event
     *
     * this method would call #ZFUIViewFocusResolveKeyEvent to achieve focus move,
     * you may override without call super's method, to override the event
     */
    virtual void viewEventOnKeyEventResolveFocus(ZF_IN ZFUIKeyEvent *keyEvent);

    /**
     * @brief called when wheel event occurred
     *
     * due to some limitations, we doesn't support intercept wheel event,
     * event dispatch logic depends on implementation,
     * you may use native view to achieve if necessary\n
     * \n
     * the event would be dispatched from child to parent,
     * util it's resolved
     */
    virtual void viewEventOnWheelEvent(ZF_IN ZFUIWheelEvent *wheelEvent);

    // ============================================================
    // view property async update
public:
    /**
     * @brief schedule a update task that #viewPropertyOnUpdate would be called after a proper time,
     *   used to update view's property
     *
     * #viewPropertyOnUpdate ensured called once after a view created
     */
    ZFMETHOD_DECLARE_0(void, viewPropertyUpdateRequest)
    zffinal inline void _ZFP_ZFUIView_viewPropertyNotifyUpdate(void)
    {
        this->viewPropertyOnUpdate();
    }
protected:
    /** @brief see #viewPropertyUpdateRequest */
    virtual void viewPropertyOnUpdate(void);

    // ============================================================
    // override
protected:
    /**
     * @brief for a view, copy style would also copy all of it's children
     */
    zfoverride
    virtual void styleableOnCopyFrom(ZF_IN ZFStyleable *anotherStyleable);

protected:
    zfoverride
    virtual void observerOnAdd(ZF_IN zfidentity eventId);
    zfoverride
    virtual void observerOnRemove(ZF_IN zfidentity eventId);

private:
    _ZFP_ZFUIViewPrivate *d;
    friend zfclassFwd _ZFP_ZFUIViewPrivate;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIView_h_

