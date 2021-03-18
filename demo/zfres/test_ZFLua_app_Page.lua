local args = {...};
local argsCount = #args;

local pm = args[1];

pm:observerAdd(ZFUIPage.EventPageOnResume(), function(listenerData, userData)
        local page = listenerData:sender();
        local pm = page:pageManager();
        pm:objectTag("leftButton"):viewVisible(pm:pageCount() > 1);
        pm:objectTag("centerButton"):buttonLabelText(page:objectInfoOfInstance());
    end, pm:objectHolder());

_ZFP_ZFLua_app_test_setupPage = function(page)
        page:observerAdd(ZFUIPage.EventPageOnCreate(), function(listenerData, userData)
                zfLog("page onCreate");
                local page = listenerData:sender();

                local pageView = ZFUIButtonBasic();
                page:pageView():childAdd(pageView, ZFUISizeParamFillFill());

                pageView:buttonLabelText(page:objectInfoOfInstance());
                pageView:viewBackgroundColor(ZFUIColorRandom());
                pageView:observerAdd(ZFUIButton.EventButtonOnClick(), function(listenerData, userData)
                        local page = userData:objectHolded();
                        page:pageManager():pageCreate(_ZFP_ZFLua_app_test_setupPage(ZFUIPage()));
                    end, page:objectHolder());
            end);
        return page;
    end

pm:pageCreate(_ZFP_ZFLua_app_test_setupPage(ZFUIPage()));
pm:pageAniOverride(zfnull, zfnull);

