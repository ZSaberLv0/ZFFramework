#include "ZFUINativeViewWrapper.h"
#include "protocol/ZFProtocolZFUIView.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFUINativeViewWrapper)

ZFOBJECT_ON_INIT_DEFINE_1(ZFUINativeViewWrapper,
                          ZFMP_IN(void *, wrappedNativeView))
{
    this->objectOnInit();
    zfself::wrappedNativeView(wrappedNativeView);
}

void ZFUINativeViewWrapper::objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
{
    zfsuper::objectInfoOnAppend(ret);
    if(this->wrappedNativeView() != zfnull)
    {
        ret += " ";
        zfsFromPointerT(ret, this->wrappedNativeView());
    }
}

ZFMETHOD_DEFINE_1(ZFUINativeViewWrapper, void, wrappedNativeView,
                  ZFMP_IN(void *, wrappedNativeView))
{
    zfclassNotPOD _ZFP_ZFUINativeViewWrapper_nativeImplViewDestroy
    {
    public:
        static void action(ZF_IN ZFUIView *view,
                           ZF_IN void *nativeImplView)
        {
            // nothing to do
        }
    };
    this->nativeImplView(wrappedNativeView, _ZFP_ZFUINativeViewWrapper_nativeImplViewDestroy::action);
}
ZFMETHOD_DEFINE_0(ZFUINativeViewWrapper, void *, wrappedNativeView)
{
    return this->nativeImplView();
}

ZFMETHOD_DEFINE_2(ZFUINativeViewWrapper, void, measureNativeView,
                  ZFMP_OUT(ZFUISize &, ret),
                  ZFMP_IN_OPT(const ZFUISize &, sizeHint, ZFUISizeInvalid()))
{
    const ZFUIMargin &nativeImplViewMargin = this->nativeImplViewMargin();
    ZFPROTOCOL_ACCESS(ZFUIView)->measureNativeView(
        ret,
        this->wrappedNativeView(),
        ZFUISizeApplyScale(
            ZFUILayoutParam::sizeHintOffset(sizeHint, ZFUISizeMake(
                    0 - ZFUIMarginGetWidth(nativeImplViewMargin),
                    0 - ZFUIMarginGetHeight(nativeImplViewMargin)
                )),
            this->scaleFixed()));
    ZFUISizeApplyScaleReversely(ret, ret, this->scaleFixed());
    ZFUISizeApplyMarginReversely(ret, ret, nativeImplViewMargin);
}

void ZFUINativeViewWrapper::layoutOnMeasure(ZF_OUT ZFUISize &ret,
                                            ZF_IN const ZFUISize &sizeHint,
                                            ZF_IN const ZFUISizeParam &sizeParam)
{
    this->measureNativeView(ret, sizeHint);
}

ZF_NAMESPACE_GLOBAL_END

