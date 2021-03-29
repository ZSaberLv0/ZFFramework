#include "ZFUIKit_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUIKit_ZFUIScrollView_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFUIKit_ZFUIScrollView_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView);
        ZFUIWindow *window = zfnull;
        ZFUIView *container = zfnull;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        zfblockedAlloc(ZFUIScrollView, scrollView);
        container->childAdd(scrollView);
        scrollView->layoutParam()->sizeParam(ZFUISizeParamFillFill());
        scrollView->layoutParam()->layoutMargin(ZFUIMarginMake(40));
        scrollView->viewBackgroundColor(ZFUIColorRed());

        this->setupScrollListener(scrollView);
        this->setupScrollContent(scrollView, ZFUISizeMake(120), ZFUISizeMake(60), ZFUISizeMake(10));

#if 1 // test embeded scroll view
        zfblockedAlloc(ZFUIScrollView, embededScrollView);
        scrollView->childAdd(embededScrollView);
        embededScrollView->layoutParam()->layoutMargin(ZFUIMarginMake(80));
        embededScrollView->layoutParam()->sizeHint(ZFUISizeMake(200, 100));
        embededScrollView->layoutParam()->sizeParam(ZFUISizeParamFillFill());
        embededScrollView->viewBackgroundColor(ZFUIColorMake(0, 0, 1, 0.75f));

        this->setupScrollListener(embededScrollView);
        this->setupScrollContent(embededScrollView, ZFUISizeMake(60), ZFUISizeMake(30), ZFUISizeMake(10));
#endif

        this->prepareSettingButton(window, scrollView);
    }

private:
    void setupScrollListener(ZF_IN ZFUIScrollView *scrollView)
    {
#if 1 // output logs during scroll event may cause performance issue
        ZFLISTENER_LOCAL(onDragBegin, {
            zfLogTrimT() << "onDragBegin  " << listenerData.sender()->objectInfoOfInstance() << listenerData.sender<ZFUIScrollView *>()->scrollContentFrame();
        })
        scrollView->observerAdd(ZFUIScrollView::EventScrollOnDragBegin(), onDragBegin);

        ZFLISTENER_LOCAL(onDrag, {
            zfLogTrimT() << "onDrag       " << listenerData.sender()->objectInfoOfInstance() << listenerData.sender<ZFUIScrollView *>()->scrollContentFrame();
        })
        scrollView->observerAdd(ZFUIScrollView::EventScrollOnDrag(), onDrag);

        ZFLISTENER_LOCAL(onDragEnd, {
            zfLogTrimT() << "onDragEnd    " << listenerData.sender()->objectInfoOfInstance() << listenerData.sender<ZFUIScrollView *>()->scrollContentFrame();
        })
        scrollView->observerAdd(ZFUIScrollView::EventScrollOnDragEnd(), onDragEnd);

        ZFLISTENER_LOCAL(onScrollBegin, {
            zfLogTrimT() << "onScrollBegin" << listenerData.sender()->objectInfoOfInstance() << listenerData.sender<ZFUIScrollView *>()->scrollContentFrame();
        })
        scrollView->observerAdd(ZFUIScrollView::EventScrollOnScrollBegin(), onScrollBegin);

        ZFLISTENER_LOCAL(onScroll, {
            zfLogTrimT() << "onScroll     " << listenerData.sender()->objectInfoOfInstance() << listenerData.sender<ZFUIScrollView *>()->scrollContentFrame();
        })
        scrollView->observerAdd(ZFUIScrollView::EventScrollOnScroll(), onScroll);

        ZFLISTENER_LOCAL(onScrollEnd, {
            zfLogTrimT() << "onScrollEnd  " << listenerData.sender()->objectInfoOfInstance() << listenerData.sender<ZFUIScrollView *>()->scrollContentFrame();
        })
        scrollView->observerAdd(ZFUIScrollView::EventScrollOnScrollEnd(), onScrollEnd);
#endif
    }
    void setupScrollContent(ZF_IN ZFUIScrollView *scrollView,
                            ZF_IN const ZFUISize &itemSize,
                            ZF_IN const ZFUISize &itemSpace,
                            ZF_IN const ZFUISize &itemCount,
                            ZF_IN_OPT const ZFUIMargin &contentMargin = ZFUIMarginZero())
    {
        scrollView->scrollContentFrame(ZFUIRectMake(
            0,
            0,
            ZFUIMarginGetWidth(contentMargin) + itemSize.width + (itemSize.width + itemSpace.width) * itemCount.width,
            ZFUIMarginGetHeight(contentMargin) + itemSize.height + (itemSize.height + itemSpace.height) * itemCount.height));

        for(zfint y = contentMargin.top;
            y <= scrollView->scrollContentFrame().height - itemSize.height - contentMargin.bottom;
            y += itemSize.height + itemSpace.height)
        {
            for(zfint x = contentMargin.left;
                x <= scrollView->scrollContentFrame().height - itemSize.width - contentMargin.right;
                x += itemSize.width + itemSpace.width)
            {
                zfblockedAlloc(ZFUIButton, btn);
                scrollView->childAdd(btn);
                btn->layoutParam()->sizeParam(ZFUISizeParamFillFill());
                btn->layoutParam()->sizeHint(itemSize);
                btn->layoutParam()->layoutMargin(ZFUIMarginMake(x, y, 0, 0));
                btn->viewBackgroundColor(ZFUIColorRandom(0.75f));

                ZFLISTENER_LOCAL(onClick, {
                    zfLogTrimT() << "clicked" << listenerData.sender();
                })
                btn->observerAdd(ZFUIButton::EventButtonOnClick(), onClick);
            }
        }
    }
    void prepareSettingButton(ZF_IN ZFUIWindow *window,
                              ZF_IN ZFUIScrollView *scrollView)
    {
        zfblockedAlloc(ZFArray, settings);
        ZFUIKit_test_prepareSettingForBoolProperty(settings, scrollView, ZFPropertyAccess(ZFUIScrollView, scrollAlignToAxis));
        ZFUIKit_test_prepareSettingForBoolProperty(settings, scrollView, ZFPropertyAccess(ZFUIScrollView, scrollBounceHorizontal));
        ZFUIKit_test_prepareSettingForBoolProperty(settings, scrollView, ZFPropertyAccess(ZFUIScrollView, scrollBounceVertical));
        ZFUIKit_test_prepareSettingForBoolProperty(settings, scrollView, ZFPropertyAccess(ZFUIScrollView, scrollBounceHorizontalAlways));
        ZFUIKit_test_prepareSettingForBoolProperty(settings, scrollView, ZFPropertyAccess(ZFUIScrollView, scrollBounceVerticalAlways));
        ZFUIKit_test_prepareSettingForBoolProperty(settings, scrollView, ZFPropertyAccess(ZFUIScrollView, scrollAlignToPageHorizontal));
        ZFUIKit_test_prepareSettingForBoolProperty(settings, scrollView, ZFPropertyAccess(ZFUIScrollView, scrollAlignToPageVertical));

#define _ZFP_ZFUIKit_ZFUIScrollView_test_autoScrollSpeed 100
        { // auto scroll x
            zfblockedAlloc(ZFUIKit_test_SettingData, setting);
            settings->add(setting);
            setting->userData(zflineAlloc(ZFObject));
            setting->userData()->objectTag("scrollView", scrollView->objectHolder());
            ZFLISTENER_LOCAL(buttonTextGetter, {
                ZFUIScrollView *scrollView = userData->objectTag("scrollView")->objectHolded();
                v_zfstring *text = listenerData.param0<v_zfstring *>();
                text->zfv = zfstringWithFormat("autoScrollSpeedX: %d", scrollView->autoScrollSpeedX());
            })
            setting->buttonTextGetter(buttonTextGetter);
            ZFLISTENER_LOCAL(buttonClickListener, {
                ZFUIScrollView *scrollView = userData->objectTag("scrollView")->objectHolded();
                if(scrollView->autoScrollSpeedX() == 0)
                {
                    scrollView->autoScrollStartX(_ZFP_ZFUIKit_ZFUIScrollView_test_autoScrollSpeed * ((zfmRand(2) == 0) ? 1 : -1));
                }
                else
                {
                    scrollView->autoScrollStopX();
                }
            })
            setting->buttonClickListener(buttonClickListener);
        }
        { // auto scroll y
            zfblockedAlloc(ZFUIKit_test_SettingData, setting);
            settings->add(setting);
            setting->userData(zflineAlloc(ZFObject));
            setting->userData()->objectTag("scrollView", scrollView->objectHolder());
            ZFLISTENER_LOCAL(buttonTextGetter, {
                ZFUIScrollView *scrollView = userData->objectTag("scrollView")->objectHolded();
                v_zfstring *text = listenerData.param0<v_zfstring *>();
                text->zfv = zfstringWithFormat("autoScrollSpeedY: %d", scrollView->autoScrollSpeedY());
            })
            setting->buttonTextGetter(buttonTextGetter);
            ZFLISTENER_LOCAL(buttonClickListener, {
                ZFUIScrollView *scrollView = userData->objectTag("scrollView")->objectHolded();
                if(scrollView->autoScrollSpeedY() == 0)
                {
                    scrollView->autoScrollStartY(_ZFP_ZFUIKit_ZFUIScrollView_test_autoScrollSpeed * ((zfmRand(2) == 0) ? 1 : -1));
                }
                else
                {
                    scrollView->autoScrollStopY();
                }
            })
            setting->buttonClickListener(buttonClickListener);
        }

        { // scroll child to visible
            zfblockedAlloc(ZFUIKit_test_SettingData, setting);
            settings->add(setting);
            setting->userData(zflineAlloc(ZFObject));
            setting->userData()->objectTag("scrollView", scrollView->objectHolder());
            ZFLISTENER_LOCAL(buttonTextGetter, {
                v_zfstring *text = listenerData.param0<v_zfstring *>();
                text->zfv = "scrollChildToVisible";
            })
            setting->buttonTextGetter(buttonTextGetter);
            ZFLISTENER_LOCAL(buttonClickListener, {
                ZFUIScrollView *scrollView = userData->objectTag("scrollView")->objectHolded();
                zfindex index = scrollView->childCount() / 2;
                scrollView->scrollChildToVisible(scrollView->childAtIndex(index));
            })
            setting->buttonClickListener(buttonClickListener);
        }

        { // scrollAreaMargin
            zfblockedAlloc(ZFUIKit_test_SettingData, setting);
            settings->add(setting);
            setting->userData(zflineAlloc(ZFObject));
            setting->userData()->objectTag("scrollView", scrollView->objectHolder());
            ZFLISTENER_LOCAL(buttonTextGetter, {
                ZFUIScrollView *scrollView = userData->objectTag("scrollView")->objectHolded();
                v_zfstring *text = listenerData.param0<v_zfstring *>();
                text->zfv = zfstringWithFormat("scrollAreaMargin: %s",
                    ZFUIMarginToString(scrollView->scrollAreaMargin()).cString());
            })
            setting->buttonTextGetter(buttonTextGetter);
            ZFLISTENER_LOCAL(buttonClickListener, {
                ZFUIScrollView *scrollView = userData->objectTag("scrollView")->objectHolded();
                if(scrollView->scrollAreaMargin().left >= 40)
                {
                    scrollView->scrollAreaMarginRemove(scrollView->scrollAreaMargin());
                }
                else
                {
                    scrollView->scrollAreaMarginAdd(ZFUIMarginMake(20));
                }
            })
            setting->buttonClickListener(buttonClickListener);
        }

        ZFUIKit_test_prepareSettingButtonWithTestWindow(window, settings);
    }
};
ZFOBJECT_REGISTER(ZFUIKit_ZFUIScrollView_test)

ZF_NAMESPACE_GLOBAL_END

