#include "ZFUIViewTreePrint.h"

#include "ZFUIKit/protocol/ZFProtocolZFUIView.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIViewTreeNative.h"

ZF_NAMESPACE_GLOBAL_BEGIN

static void _ZFP_ZFUIViewTreePrintAfterDelayAction(ZF_IN const ZFListenerData &listenerData, ZF_IN ZFObject *userData)
{
    ZFListenerHolder *data = ZFCastZFObjectUnchecked(ZFListenerHolder *, userData);
    ZFUIViewTreePrint(data->listenerData.param0<ZFUIView *>(), data->runnable);
}

ZFMETHOD_FUNC_DEFINE_3(void, ZFUIViewTreePrintAfterDelay,
                       ZFMP_IN(zftimet, delay),
                       ZFMP_IN(ZFUIView *, view),
                       ZFMP_IN_OPT(const ZFOutput &, outputCallback, ZFOutputDefault()))
{
    ZFExecuteAfterDelay(
        delay,
        ZFCallbackForFunc(_ZFP_ZFUIViewTreePrintAfterDelayAction),
        zflineAlloc(ZFListenerHolder, outputCallback, ZFListenerData().param0(view)));
}

// ============================================================
zfclassPOD _ZFP_ZFUIViewTreePrintData
{
public:
    const ZFClass *viewClass;
    ZFUIViewTreePrintInfoGetter viewInfoGetter;
};

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIViewTreePrintDataHolder, ZFLevelZFFrameworkEssential)
{
}
public:
    // ordered by:
    // ZFUIView, classBaseA, classChildA, classBaseB, classChildB
    ZFCoreArrayPOD<_ZFP_ZFUIViewTreePrintData> datas;
ZF_GLOBAL_INITIALIZER_END(ZFUIViewTreePrintDataHolder)

zfclassPOD _ZFP_ZFUIViewTreePrintPrintData
{
public:
    ZFUIView *view;
    zfindex depth;
    zfindex siblingIndex;
    ZFUIViewChildLayerEnum layer;
};
static ZFObject *_ZFP_ZFUIViewTreePrintSyncObject = zfnull;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIViewTreePrintSyncObjectHolder, ZFLevelZFFrameworkHigh)
{
    _ZFP_ZFUIViewTreePrintSyncObject = zfAlloc(ZFObject);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIViewTreePrintSyncObjectHolder)
{
    zfRelease(_ZFP_ZFUIViewTreePrintSyncObject);
    _ZFP_ZFUIViewTreePrintSyncObject = zfnull;
}
ZF_GLOBAL_INITIALIZER_END(ZFUIViewTreePrintSyncObjectHolder);

zfclass _ZFP_I_ZFUIViewTreePrintFormat : zfextends ZFObject, zfimplements ZFOutputFormat
{
    ZFOBJECT_DECLARE(_ZFP_I_ZFUIViewTreePrintFormat, ZFObject)
    ZFIMPLEMENTS_DECLARE(ZFOutputFormat)

protected:
    virtual void format(ZF_IN_OUT zfstring &ret,
                        ZF_IN ZFOutputFormatStepEnum outputStep,
                        ZF_IN const zfchar *src,
                        ZF_IN zfindex srcLen,
                        ZF_IN zfindex outputCount,
                        ZF_IN zfindex writtenLen,
                        ZF_IN_OUT_OPT void *&state)
    {
        const zfchar *srcEnd = src + srcLen;
        while(src < srcEnd)
        {
            if(*src == '\r')
            {
                ret += "\\r";
                ++src;
            }
            else if(*src == '\n')
            {
                ret += "\\n";
                ++src;
            }
            else
            {
                zfcharAppendAndMoveNext(ret, src);
            }
        }
    }
};

ZFMETHOD_FUNC_DEFINE_2(void, ZFUIViewTreePrint,
                       ZFMP_IN(ZFUIView *, view),
                       ZFMP_IN_OPT(const ZFOutput &, outputCallback, ZFOutputDefault()))
{
    if(view == zfnull || !outputCallback.callbackIsValid())
    {
        return ;
    }

    zfsynchronize(_ZFP_ZFUIViewTreePrintSyncObject);

    ZFOutput outputCallbackNoEndl = ZFOutputForFormat(outputCallback, zflineAlloc(_ZFP_I_ZFUIViewTreePrintFormat));

    ZFCoreArrayPOD<_ZFP_ZFUIViewTreePrintPrintData> printDatas;
    _ZFP_ZFUIViewTreePrintPrintData rootPrintData;
    rootPrintData.view = view;
    rootPrintData.depth = 0;
    rootPrintData.siblingIndex = 0;
    rootPrintData.layer = ZFUIViewChildLayer::e_Normal;
    printDatas.add(rootPrintData);

    outputCallback.execute("====================== view tree begin =====================\n");
    do
    {
        _ZFP_ZFUIViewTreePrintPrintData printData = printDatas.getLast();
        printDatas.removeLast();

        // all children
        ZFCoreArrayPOD<ZFUIView *> implViews = printData.view->internalImplViewArray();
        ZFCoreArrayPOD<ZFUIView *> bgViews = printData.view->internalBgViewArray();
        ZFCoreArrayPOD<ZFUIView *> normalViews = printData.view->childArray();
        ZFCoreArrayPOD<ZFUIView *> fgViews = printData.view->internalFgViewArray();
        for(zfindex i = fgViews.count() - 1; i != zfindexMax(); --i)
        {
            _ZFP_ZFUIViewTreePrintPrintData printDataTmp;
            printDataTmp.view = fgViews.get(i)->to<ZFUIView *>();
            printDataTmp.depth = printData.depth + 1;
            printDataTmp.siblingIndex = i;
            printDataTmp.layer = ZFUIViewChildLayer::e_InternalFg;
            printDatas.add(printDataTmp);
        }
        for(zfindex i = normalViews.count() - 1; i != zfindexMax(); --i)
        {
            _ZFP_ZFUIViewTreePrintPrintData printDataTmp;
            printDataTmp.view = normalViews.get(i)->to<ZFUIView *>();
            printDataTmp.depth = printData.depth + 1;
            printDataTmp.siblingIndex = i;
            printDataTmp.layer = ZFUIViewChildLayer::e_Normal;
            printDatas.add(printDataTmp);
        }
        for(zfindex i = bgViews.count() - 1; i != zfindexMax(); --i)
        {
            _ZFP_ZFUIViewTreePrintPrintData printDataTmp;
            printDataTmp.view = bgViews.get(i)->to<ZFUIView *>();
            printDataTmp.depth = printData.depth + 1;
            printDataTmp.siblingIndex = i;
            printDataTmp.layer = ZFUIViewChildLayer::e_InternalBg;
            printDatas.add(printDataTmp);
        }
        for(zfindex i = implViews.count() - 1; i != zfindexMax(); --i)
        {
            _ZFP_ZFUIViewTreePrintPrintData printDataTmp;
            printDataTmp.view = implViews.get(i)->to<ZFUIView *>();
            printDataTmp.depth = printData.depth + 1;
            printDataTmp.siblingIndex = i;
            printDataTmp.layer = ZFUIViewChildLayer::e_InternalImpl;
            printDatas.add(printDataTmp);
        }

        outputCallback.execute("|");
        outputCallback.execute(zfstringWithFormat("%2zi", printData.siblingIndex).cString());
        for(zfindex i = printData.depth - 1; i != zfindexMax(); --i)
        {
            outputCallback.execute(" |");
        }

        switch(printData.layer)
        {
            case ZFUIViewChildLayer::e_Normal:
                outputCallback.execute(" ");
                break;
            case ZFUIViewChildLayer::e_InternalImpl:
                outputCallback.execute(" impl ");
                break;
            case ZFUIViewChildLayer::e_InternalBg:
                outputCallback.execute(" bg ");
                break;
            case ZFUIViewChildLayer::e_InternalFg:
                outputCallback.execute(" fg ");
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                return ;
        }

        ZFCoreArrayPOD<_ZFP_ZFUIViewTreePrintData> &datas = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewTreePrintDataHolder)->datas;
        zfbool exist = zffalse;
        for(zfindex i = datas.count() - 1; i != zfindexMax(); --i)
        {
            if(printData.view->classData()->classIsTypeOf(datas[i].viewClass))
            {
                datas[i].viewInfoGetter(printData.view, outputCallbackNoEndl);
                exist = zftrue;
                break;
            }
        }
        if(!exist)
        {
            zfstring tmp;
            printData.view->objectInfoT(tmp);
            outputCallbackNoEndl.execute(tmp.cString(), tmp.length());
        }
        outputCallback.execute("\n");
    } while(!printDatas.isEmpty());
    outputCallback.execute("====================== view tree  end  =====================\n");
}

void ZFUIViewTreePrintInfoGetterForClass(ZF_IN const ZFClass *viewClass,
                                         ZF_IN ZFUIViewTreePrintInfoGetter viewInfoGetter)
{
    zfCoreAssert(viewClass != zfnull && viewClass->classIsTypeOf(ZFUIView::ClassData()));
    ZFCoreArrayPOD<_ZFP_ZFUIViewTreePrintData> &datas = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewTreePrintDataHolder)->datas;
    if(viewInfoGetter == zfnull)
    {
        for(zfindex i = 0; i < datas.count(); ++i)
        {
            const ZFClass *cls = datas[i].viewClass;
            if(viewClass == cls)
            {
                datas.remove(i);
                break;
            }
        }
        return ;
    }

    zfindex indexAddTo = zfindexMax();
    for(zfindex i = 0; i < datas.count(); ++i)
    {
        const ZFClass *cls = datas[i].viewClass;
        if(viewClass == cls)
        {
            datas[i].viewInfoGetter = viewInfoGetter;
            break;
        }
        else if(cls->classIsTypeOf(viewClass))
        {
            indexAddTo = i;
            break;
        }
        else if(viewClass->classIsTypeOf(cls))
        {
            indexAddTo = i + 1;
        }
    }

    _ZFP_ZFUIViewTreePrintData data;
    data.viewClass = viewClass;
    data.viewInfoGetter = viewInfoGetter;
    if(indexAddTo == zfindexMax())
    {
        datas.add(data);
    }
    else
    {
        datas.add(indexAddTo, data);
    }
}
ZFUIViewTreePrintInfoGetter ZFUIViewTreePrintInfoGetterForClass(ZF_IN const ZFClass *viewClass)
{
    ZFCoreArrayPOD<_ZFP_ZFUIViewTreePrintData> &datas = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewTreePrintDataHolder)->datas;
    for(zfindex i = datas.count() - 1; i != zfindexMax(); --i)
    {
        if(datas[i].viewClass == viewClass)
        {
            return datas[i].viewInfoGetter;
        }
    }
    return zfnull;
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_2(void, ZFUIViewTreeNativePrint,
                       ZFMP_IN(ZFUIView *, view),
                       ZFMP_IN_OPT(const ZFOutput &, outputCallback, ZFOutputDefault()))
{
    if(!outputCallback.callbackIsValid())
    {
        return;
    }
    ZFPROTOCOL_INTERFACE_CLASS(ZFUIViewTreeNative) *impl = ZFPROTOCOL_TRY_ACCESS(ZFUIViewTreeNative);
    if(impl != zfnull)
    {
        impl->viewTreeNative(view, outputCallback);
    }
    else
    {
        outputCallback
            << "========== ZFUIViewTreeNativePrint begin ==========\n"
            << "| not available\n"
            << "========== ZFUIViewTreeNativePrint  end  ==========\n"
            ;
    }
}

static void _ZFP_ZFUIViewTreeNativePrintAfterDelayAction(ZF_IN const ZFListenerData &listenerData, ZF_IN ZFObject *userData)
{
    ZFListenerHolder *data = ZFCastZFObjectUnchecked(ZFListenerHolder *, userData);
    ZFUIViewTreeNativePrint(data->listenerData.param0<ZFUIView *>(), data->runnable);
}

ZFMETHOD_FUNC_DEFINE_3(void, ZFUIViewTreeNativePrintAfterDelay,
                       ZFMP_IN(zftimet, delay),
                       ZFMP_IN(ZFUIView *, view),
                       ZFMP_IN_OPT(const ZFOutput &, outputCallback, ZFOutputDefault()))
{
    ZFExecuteAfterDelay(
        delay,
        ZFCallbackForFunc(_ZFP_ZFUIViewTreeNativePrintAfterDelayAction),
        zflineAlloc(ZFListenerHolder, outputCallback, ZFListenerData().param0(view)));
}

ZF_NAMESPACE_GLOBAL_END

