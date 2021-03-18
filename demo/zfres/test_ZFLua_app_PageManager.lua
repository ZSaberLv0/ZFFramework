local pm = ZFUIPageManager();

pm:observerAdd(ZFUIPageManager.EventManagerOnCreate(), function(listenerData, userData)
        zfLog("manager onCreate");
        local pm = listenerData:sender();

        local managerContainer = pm:managerContainer();

        local topView = ZFUILinearLayout();
        pm:objectTag("topView", topView);
        managerContainer:childAdd(topView, ZFUISizeParamFillWrap());
        topView:layoutOrientation(ZFUIOrientation.e_Left());

        local leftButton = ZFUIButtonBasic();
        pm:objectTag("leftButton", leftButton);
        topView:childAdd(leftButton, ZFUISizeParamFillWrap(), ZFUIAlign.e_Center());
        leftButton:layoutParam():layoutWeight(1);
        leftButton:buttonLabelText("Back");
        leftButton:viewBackgroundColor(ZFUIColorRandom());
        leftButton:layoutParam():layoutReserveSpaceWhenNotVisible(zftrue);
        leftButton:viewVisible(zffalse);
        leftButton:observerAdd(ZFUIButton.EventButtonOnClick(), function(listenerData, userData)
                local pm = userData:objectHolded();
                pm:pageAtIndex(pm:pageCount() - 1):pageDestroy();
            end, pm:objectHolder());

        local centerButton = ZFUIButtonBasic();
        pm:objectTag("centerButton", centerButton);
        topView:childAdd(centerButton, ZFUISizeParamFillWrap(), ZFUIAlign.e_Center());
        centerButton:layoutParam():layoutWeight(2);
        centerButton:viewBackgroundColor(ZFUIColorRandom());

        local rightButton = ZFUIButtonBasic();
        pm:objectTag("rightButton", rightButton);
        topView:childAdd(rightButton, ZFUISizeParamFillWrap(), ZFUIAlign.e_Center());
        rightButton:layoutParam():layoutWeight(1);
        rightButton:buttonLabelText("ExitTest");
        rightButton:viewBackgroundColor(ZFUIColorRandom());
        rightButton:observerAdd(ZFUIButton.EventButtonOnClick(), function(listenerData, userData)
                local pm = userData:objectHolded();
                pm:managerDestroy();
            end, pm:objectHolder());
    end);

return pm;

