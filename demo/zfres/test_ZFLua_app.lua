local pm = ZFLuaImport("test_ZFLua_app_PageManager.lua");

pm:managerCreateForWindow();
pm:managerContainer():layoutParam():layoutMargin(ZFUIMarginMake(20));

ZFLuaImport("test_ZFLua_app_Page.lua", pm);

return pm;

