#include "ZFUIWidget_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUIWidget_ZFUIListView_test_ListAdapter : zfextends ZFObject, zfimplements ZFUIListAdapter
{
    ZFOBJECT_DECLARE(ZFUIWidget_ZFUIListView_test_ListAdapter, ZFObject)
    ZFIMPLEMENTS_DECLARE(ZFUIListAdapter)

public:
    virtual zfindex cellCount(void)
    {
        return 2000;
    }
    virtual zfint cellSizeAtIndex(ZF_IN zfindex index,
                                  ZF_IN ZFUIListCell *cell)
    {
        #if 1
            return this->cellSizeHint() + zfmRand(100);
        #else
            return zfsuperI(ZFUIListAdapter)::cellSizeAtIndex(index, cell);
        #endif
    }
    virtual zfautoObject cellAtIndex(ZF_IN zfindex index)
    {
        zfblockedAlloc(ZFUIListCell, ret);
        ret->cellView(zflineAlloc(ZFUIListCellViewBasic));
        return ret;
    }
    virtual inline void cellOnUpdate(ZF_IN zfindex atIndex,
                                     ZF_IN ZFUIListCell *cell)
    {
        zfsuperI(ZFUIListAdapter)::cellOnUpdate(atIndex, cell);

        ZFUIListCellViewBasic *tmp = cell->cellView()->toAny();
        tmp->cellLabelMain()->text(zfstringWithFormat("main %zi", atIndex));
        if(zfmRand(3) == 0)
        {
            tmp->cellLabelSub()->text(zfstringWithFormat("sub %zi", atIndex));
        }
        if(zfmRand(3) == 0)
        {
            tmp->cellIcon()->image(ZFUIImageLoadFromColor(ZFUIColorRandom(), ZFUISizeMake(40)));
        }
        tmp->viewBackgroundColor(ZFUIColorRandom());
    }

    #if 1 // whether use cache
        virtual inline zfautoObject cellCacheOnAccess(ZF_IN zfindex index)
        {
            return this->cellCacheDefaultAccess(zfnull);
        }
        virtual inline void cellCacheOnRecycle(ZF_IN ZFUIListCell *cell)
        {
            this->cellCacheDefaultRecycle(zfnull, cell);
        }
    #endif
};

zfclass ZFUIWidget_ZFUIListView_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFUIWidget_ZFUIListView_test, ZFFramework_test_TestCase)

    ZFPROPERTY_RETAIN_WITH_INIT(ZFUIListAdapter *, listAdapter, zflineAlloc(ZFUIWidget_ZFUIListView_test_ListAdapter))

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView);
        ZFUIWindow *window = zfnull;
        ZFUIView *container = zfnull;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        zfblockedAlloc(ZFUIListView, listView);
        container->childAdd(listView);
        listView->layoutParam()->sizeParam(ZFUISizeParamFillFill());
        listView->layoutParam()->layoutMargin(ZFUIMarginMake(40));
        listView->viewBackgroundColor(ZFUIColorRed());
        listView->listAdapter(this->listAdapter());
        {
            zfblockedAlloc(ZFUIListCellUpdaterBasic, cellUpdater);
            listView->cellUpdater()->add(cellUpdater);
            cellUpdater->separatorSize(5);
            cellUpdater->separatorColor(ZFUIColorRed());
        }

        this->setupScrollListener(listView);

        this->prepareSettingButton(window, listView);
    }

private:
    void setupScrollListener(ZF_IN ZFUIListView *listView)
    {
#if 0 // output logs during scroll event may cause performance issue
        ZFLISTENER_LOCAL(onDragBegin, {
            zfLogTrimT() << "onDragBegin  " << listenerData.sender()->objectInfoOfInstance() << listenerData.sender<ZFUIListView *>()->scrollContentFrame();
        })
        listView->observerAdd(ZFUIListView::EventScrollOnDragBegin(), onDragBegin);

        ZFLISTENER_LOCAL(onDrag, {
            zfLogTrimT() << "onDrag       " << listenerData.sender()->objectInfoOfInstance() << listenerData.sender<ZFUIListView *>()->scrollContentFrame();
        })
        listView->observerAdd(ZFUIListView::EventScrollOnDrag(), onDrag);

        ZFLISTENER_LOCAL(onDragEnd, {
            zfLogTrimT() << "onDragEnd    " << listenerData.sender()->objectInfoOfInstance() << listenerData.sender<ZFUIListView *>()->scrollContentFrame();
        })
        listView->observerAdd(ZFUIListView::EventScrollOnDragEnd(), onDragEnd);

        ZFLISTENER_LOCAL(onScrollBegin, {
            zfLogTrimT() << "onScrollBegin" << listenerData.sender()->objectInfoOfInstance() << listenerData.sender<ZFUIListView *>()->scrollContentFrame();
        })
        listView->observerAdd(ZFUIListView::EventScrollOnScrollBegin(), onScrollBegin);

        ZFLISTENER_LOCAL(onScroll, {
            zfLogTrimT() << "onScroll     " << listenerData.sender()->objectInfoOfInstance() << listenerData.sender<ZFUIListView *>()->scrollContentFrame();
        })
        listView->observerAdd(ZFUIListView::EventScrollOnScroll(), onScroll);

        ZFLISTENER_LOCAL(onScrollEnd, {
            zfLogTrimT() << "onScrollEnd  " << listenerData.sender()->objectInfoOfInstance() << listenerData.sender<ZFUIListView *>()->scrollContentFrame();
        })
        listView->observerAdd(ZFUIListView::EventScrollOnScrollEnd(), onScrollEnd);
#endif
    }
    void prepareSettingButton(ZF_IN ZFUIWindow *window,
                              ZF_IN ZFUIListView *listView)
    {
        zfblockedAlloc(ZFArray, settings);
        ZFUIKit_test_prepareSettingForBoolProperty(settings, listView, ZFPropertyAccess(ZFUIListView, scrollAlignToAxis));
        ZFUIKit_test_prepareSettingForBoolProperty(settings, listView, ZFPropertyAccess(ZFUIListView, scrollBounceHorizontal));
        ZFUIKit_test_prepareSettingForBoolProperty(settings, listView, ZFPropertyAccess(ZFUIListView, scrollBounceVertical));
        ZFUIKit_test_prepareSettingForBoolProperty(settings, listView, ZFPropertyAccess(ZFUIListView, scrollBounceHorizontalAlways));
        ZFUIKit_test_prepareSettingForBoolProperty(settings, listView, ZFPropertyAccess(ZFUIListView, scrollBounceVerticalAlways));
        ZFUIKit_test_prepareSettingForNormalProperty(settings, listView, ZFUIOrientationEnum, ZFPropertyAccess(ZFUIListView, listOrientation),
            ZFCoreArrayPODCreate(ZFUIOrientationEnum
                , ZFUIOrientation::e_Top
                , ZFUIOrientation::e_Left
                , ZFUIOrientation::e_Right
                , ZFUIOrientation::e_Bottom
                ));
        ZFUIKit_test_prepareSettingForNormalProperty(settings, listView->listAdapter()->toObject(), zfint, ZFPropertyAccess(ZFUIListAdapter, cellSizeHint), ZFCoreArrayPODCreate(zfint, 56, 100, 44));

#define _ZFP_ZFUIWidget_ZFUIListView_test_autoScrollSpeed 100
        { // auto scroll x
            zfblockedAlloc(ZFUIKit_test_SettingData, setting);
            settings->add(setting);
            setting->userData(zflineAlloc(ZFObject));
            setting->userData()->objectTag("listView", listView->objectHolder());
            ZFLISTENER_LOCAL(buttonTextGetter, {
                ZFUIListView *listView = userData->objectTag("listView")->objectHolded();
                v_zfstring *text = listenerData.param0<v_zfstring *>();
                text->zfv = zfstringWithFormat("autoScrollSpeedX: %d", listView->autoScrollSpeedX());
            })
            setting->buttonTextGetter(buttonTextGetter);
            ZFLISTENER_LOCAL(buttonClickListener, {
                ZFUIListView *listView = userData->objectTag("listView")->objectHolded();
                if(listView->autoScrollSpeedX() == 0)
                {
                    listView->autoScrollStartX(_ZFP_ZFUIWidget_ZFUIListView_test_autoScrollSpeed * ((zfmRand(2) == 0) ? 1 : -1));
                }
                else
                {
                    listView->autoScrollStopX();
                }
            })
            setting->buttonClickListener(buttonClickListener);
        }
        { // auto scroll y
            zfblockedAlloc(ZFUIKit_test_SettingData, setting);
            settings->add(setting);
            setting->userData(zflineAlloc(ZFObject));
            setting->userData()->objectTag("listView", listView->objectHolder());
            ZFLISTENER_LOCAL(buttonTextGetter, {
                ZFUIListView *listView = userData->objectTag("listView")->objectHolded();
                v_zfstring *text = listenerData.param0<v_zfstring *>();
                text->zfv = zfstringWithFormat("autoScrollSpeedY: %d", listView->autoScrollSpeedY());
            })
            setting->buttonTextGetter(buttonTextGetter);
            ZFLISTENER_LOCAL(buttonClickListener, {
                ZFUIListView *listView = userData->objectTag("listView")->objectHolded();
                if(listView->autoScrollSpeedY() == 0)
                {
                    listView->autoScrollStartY(_ZFP_ZFUIWidget_ZFUIListView_test_autoScrollSpeed * ((zfmRand(2) == 0) ? 1 : -1));
                }
                else
                {
                    listView->autoScrollStopY();
                }
            })
            setting->buttonClickListener(buttonClickListener);
        }

        { // scroll random cell to random position
            zfblockedAlloc(ZFObject, userData);
            userData->objectTag("listView", listView->objectHolder());
            ZFLISTENER_LOCAL(buttonClickListener, {
                ZFUIListView *listView = userData->objectTag("listView")->objectHolded();
                zfbool toHead = (zfmRand(2) == 0);
                zfindex toIndex = zfmRand(listView->listAdapter()->cellCount());
                zfint toOffset = (ZFUIOrientationIsHorizontal(listView->listOrientation())
                    ? zfmRand(listView->viewFrame().width)
                    : zfmRand(listView->viewFrame().height));
                zfbool animated = (zfmRand(5) != 0);
                ZFUIHintShow(zfstringWithFormat("%s\nindex: %zi\noffset: %d\nanimated: %b",
                    toHead ? "scrollListCellToHead" : "scrollListCellToTail",
                    toIndex, toOffset, animated));
                if(toHead)
                {
                    listView->scrollListCellToHead(toIndex, toOffset, animated);
                }
                else
                {
                    listView->scrollListCellToTail(toIndex, toOffset, animated);
                }
            })
            zfblockedAlloc(ZFUIKit_test_Button, randomScrollButton);
            window->childAdd(randomScrollButton);
            randomScrollButton->buttonLabelText("random scroll");
            randomScrollButton->observerAdd(ZFUIButton::EventButtonOnClick(), buttonClickListener, userData);
        }

        { // scrollAreaMargin
            zfblockedAlloc(ZFUIKit_test_SettingData, setting);
            settings->add(setting);
            setting->userData(zflineAlloc(ZFObject));
            setting->userData()->objectTag("listView", listView->objectHolder());
            ZFLISTENER_LOCAL(buttonTextGetter, {
                ZFUIScrollView *listView = userData->objectTag("listView")->objectHolded();
                v_zfstring *text = listenerData.param0<v_zfstring *>();
                text->zfv = zfstringWithFormat("scrollAreaMargin: %s",
                    ZFUIMarginToString(listView->scrollAreaMargin()).cString());
            })
            setting->buttonTextGetter(buttonTextGetter);
            ZFLISTENER_LOCAL(buttonClickListener, {
                ZFUIScrollView *listView = userData->objectTag("listView")->objectHolded();
                if(listView->scrollAreaMargin().left >= 40)
                {
                    listView->scrollAreaMarginRemove(listView->scrollAreaMargin());
                }
                else
                {
                    listView->scrollAreaMarginAdd(ZFUIMarginMake(20));
                }
            })
            setting->buttonClickListener(buttonClickListener);
        }

        ZFUIKit_test_prepareSettingButtonWithTestWindow(window, settings);
    }
};
ZFOBJECT_REGISTER(ZFUIWidget_ZFUIListView_test)

ZF_NAMESPACE_GLOBAL_END

