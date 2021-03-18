#include "ZFFramework_test_global.h"
#include "ZFFramework_test/ZFUIKit_test/ZFUIKit_test.h"
#include "ZFCore.h"
#include "ZFUIKit.h"
#include "ZFUIWidget.h"
#include "ZFUIWebKit.h"
#include "ZFLua.h"

#include "ZFCore/protocol/ZFProtocolZFThreadTaskRequest.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIView.h"
#include "ZFUIKit/protocol/ZFProtocolZFUISysWindow.h"

static zfbool _ZFP_ZFFramework_test_protocolCheck(void);
static zfautoObject _ZFP_ZFFramework_test_containerViewPrepare(void);
static void _ZFP_ZFFramework_test_prepareTestCase(ZF_IN ZFUIView *containerView);

ZFMAIN_ENTRY()
{
    if(_ZFP_ZFFramework_test_protocolCheck())
    {
        zfautoObject containerView = _ZFP_ZFFramework_test_containerViewPrepare();
        _ZFP_ZFFramework_test_prepareTestCase(containerView);
    }
}

static zfbool _ZFP_ZFFramework_test_protocolCheck(void)
{
    {
        ZFCoreArray<ZFProtocolImplInfo> implDatas = ZFProtocolImplInfoGetAllNotImplemented();
        if(!implDatas.isEmpty())
        {
            zfclassNotPOD _ZFP_main_ZFProtocolImplInfo_sort
            {
            public:
                static ZFCompareResult action(ZF_IN ZFProtocolImplInfo const &v0, ZF_IN ZFProtocolImplInfo const &v1)
                {
                    return ZFComparerDefault(v0.protocolName, v1.protocolName);
                }
            };
            implDatas.sort(_ZFP_main_ZFProtocolImplInfo_sort::action);

            zfLogTrimT() << "note, these protocol has not been implemented:";
            for(zfindex i = 0; i < implDatas.count(); ++i)
            {
                ZFOutput output = (zfLogTrimT() << ZFLogAutoSpaceOff << ZFLogAutoEndlOff);
                output << "    ";
                ZFProtocolImplInfoPrint(implDatas[i], output);
                output << "\n";
            }
        }
    }
    if(!ZFPROTOCOL_IS_AVAILABLE(ZFUIView)
       || !ZFPROTOCOL_IS_AVAILABLE(ZFUISysWindow)
       || !ZFPROTOCOL_IS_AVAILABLE(ZFThreadTaskRequest))
    {
        ZFTestCaseRunAllStart();
        return zffalse;
    }
    return zftrue;
}
static zfautoObject _ZFP_ZFFramework_test_containerViewPrepare(void)
{
    zfblockedAlloc(ZFUIWindow, window);
    window->windowShow();

    zfblockedAlloc(ZFUIKit_test_ListView, containerView);
    window->childAdd(containerView);
    containerView->layoutParam()->sizeParam(ZFUISizeParamFillFill());

    ZFUIViewFocusNextMove(window);
    return containerView;
}

static void _ZFP_ZFFramework_test_prepareTestCaseSubModule(ZF_IN ZFUIView *containerView,
                                                           ZF_IN const zfchar *subModuleName,
                                                           ZF_IN ZFCoreArrayPOD<const ZFClass *> const &testCases);
static void _ZFP_ZFFramework_test_prepareTestCaseSubModuleTest(ZF_IN ZFUIView *containerView,
                                                               ZF_IN const zfchar *subModuleName,
                                                               ZF_IN const ZFClass *testCase);
static void _ZFP_ZFFramework_test_prepareTestCase(ZF_IN ZFUIView *containerView)
{
    ZFCoreArrayPOD<const ZFClass *> allTestCase = ZFTestCaseGetAll();
    zfclassNotPOD _ZFP_main_class_sort
    {
    public:
        static ZFCompareResult action(ZF_IN const ZFClass * const &cls0, ZF_IN const ZFClass * const &cls1)
        {
            return ZFComparerDefault(cls0->classNameFull(), cls1->classNameFull());
        }
    };
    allTestCase.sort(_ZFP_main_class_sort::action);

    while(!allTestCase.isEmpty())
    {
        ZFCoreArrayPOD<const ZFClass *> subModule;
        zfstring subModuleName = allTestCase[0]->classNameFull();
        {
            zfindex t = zfstringFind(subModuleName, '_');
            if(t == zfindexMax())
            {
                subModuleName.removeAll();
            }
            else
            {
                subModuleName = zfstring(subModuleName.cString(), t);
            }
        }
        for(zfindex i = 0; i < allTestCase.count(); ++i)
        {
            if(zfsncmp(allTestCase[i]->classNameFull(), subModuleName.cString(), subModuleName.length()) == 0)
            {
                subModule.add(allTestCase[i]);
                allTestCase.remove(i);
                --i;
            }
        }
        _ZFP_ZFFramework_test_prepareTestCaseSubModule(containerView, subModuleName, subModule);
    }
}
zfclass _ZFP_ZFFramework_test_TestCaseSubModuleData : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_ZFFramework_test_TestCaseSubModuleData, ZFObject)

    zfstring subModuleName;
    ZFCoreArrayPOD<const ZFClass *> testCases;
};
static void _ZFP_ZFFramework_test_prepareTestCaseSubModule(ZF_IN ZFUIView *containerView,
                                                           ZF_IN const zfchar *subModuleName,
                                                           ZF_IN ZFCoreArrayPOD<const ZFClass *> const &testCases)
{
    zfblockedAlloc(ZFUIKit_test_Button, button);
    containerView->childAdd(button);

    ZFLISTENER_LOCAL(onClickButton, {
        _ZFP_ZFFramework_test_TestCaseSubModuleData *subModuleData = userData->toAny();

        zfblockedAlloc(ZFUIWindow, subModuleWindow);
        subModuleWindow->viewBackgroundColor(ZFUIColorWhite());
        subModuleWindow->windowShow();
        zfblockedAlloc(ZFUIKit_test_ListView, containerView);
        subModuleWindow->childAdd(containerView);
        containerView->layoutParam()->sizeParam(ZFUISizeParamFillFill());

        {
            zfblockedAlloc(ZFUIKit_test_Button, closeButton);
            containerView->childAdd(closeButton);
            closeButton->buttonLabelText("back");
            ZFLISTENER_LOCAL(closeButtonOnClick, {
                userData->objectHolded<ZFUIWindow *>()->windowHide();
            })
            closeButton->observerAdd(ZFUIButton::EventButtonOnClick(), closeButtonOnClick, subModuleWindow->objectHolder());
            closeButton->buttonBackgroundStyle()->viewBackgroundColor(ZFUIColorRed());

            zfblockedAlloc(ZFUIView, separator);
            containerView->childAdd(separator);
            separator->viewSizeMin(ZFUISizeMake(0, 5));
            separator->viewSizeMax(ZFUISizeMake(-1, 5));
            separator->viewBackgroundColor(ZFUIColorGray());
        }

        for(zfindex i = 0; i < subModuleData->testCases.count(); ++i)
        {
            _ZFP_ZFFramework_test_prepareTestCaseSubModuleTest(containerView, subModuleData->subModuleName, subModuleData->testCases[i]);
        }
        ZFUIViewFocusNextMove(subModuleWindow);
    })
    zfblockedAlloc(_ZFP_ZFFramework_test_TestCaseSubModuleData, subModuleData);
    subModuleData->subModuleName = subModuleName;
    subModuleData->testCases = testCases;
    button->observerAdd(ZFUIButton::EventButtonOnClick(), onClickButton, subModuleData);
    button->buttonLabelText(subModuleName);
}
static void _ZFP_ZFFramework_test_prepareTestCaseSubModuleTest(ZF_IN ZFUIView *containerView,
                                                               ZF_IN const zfchar *subModuleName,
                                                               ZF_IN const ZFClass *testCase)
{
    zfblockedAlloc(ZFUIKit_test_Button, button);
    containerView->childAdd(button);

    ZFLISTENER_LOCAL(onClickButton, {
        const ZFClass *testCase = userData->objectTag<v_ZFClass *>("testCase")->zfv;
        ZFUIView *containerView = userData->objectTag("containerView")->objectHolded();
        ZFTestCase *running = zfnull;
        containerView->viewUIEnableTree(zffalse);
        ZFTestCaseRun(testCase, &running);
        if(running != zfnull)
        {
            ZFLISTENER_LOCAL(testCaseOnStop, {
                ZFUIView *containerView = userData->objectTag("containerView")->objectHolded();
                containerView->viewUIEnableTree(zftrue);
            })
            running->observerAdd(ZFTestCase::EventTestCaseOnStop(), testCaseOnStop, userData);
        }
        else
        {
            containerView->viewUIEnableTree(zftrue);
        }
    })
    zfblockedAlloc(ZFObject, userData);
    userData->objectTag("testCase", zflineAlloc(v_ZFClass, testCase));
    userData->objectTag("containerView", containerView->objectHolder());
    button->observerAdd(ZFUIButton::EventButtonOnClick(), onClickButton, userData);
    button->buttonLabelText(zfstring(testCase->classNameFull() + zfslen(subModuleName) + 1));
}

