#include "ZFFile_impl.cpp"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// cwd
ZFMETHOD_FUNC_DEFINE_0(const zfchar *, ZFFilePathForCwd)
{
    ZFPROTOCOL_INTERFACE_CLASS(ZFFileCwd) *impl = ZFPROTOCOL_TRY_ACCESS(ZFFileCwd);
    if(impl != zfnull)
    {
        return impl->pathForCwd();
    }
    else
    {
        return zfnull;
    }
}
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFFilePathForCwdChange,
                       ZFMP_IN(const zfchar *, path))
{
    ZFPROTOCOL_INTERFACE_CLASS(ZFFileCwd) *impl = ZFPROTOCOL_TRY_ACCESS(ZFFileCwd);
    if(impl != zfnull)
    {
        return impl->pathForCwdChange(path);
    }
    else
    {
        return zffalse;
    }
}
ZFMETHOD_FUNC_DEFINE_0(const zfchar *, ZFFilePathForCwdOrModule)
{
    const zfchar *ret = ZFFilePathForCwd();
    if(ret != zfnull)
    {
        return ret;
    }
    else
    {
        return ZFFilePathForModule();
    }
}

// ============================================================
// module
ZFMETHOD_FUNC_DEFINE_0(const zfchar *, ZFFilePathForModule)
{
    return _ZFP_ZFFilePathImpl->pathForModule();
}
ZFMETHOD_FUNC_DEFINE_0(const zfchar *, ZFFilePathForModuleFile)
{
    return _ZFP_ZFFilePathImpl->pathForModuleFile();
}

// ============================================================
// setting
ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
ZFOBSERVER_EVENT_GLOBAL_REGISTER(ZFFilePathForSettingOnChange)
ZF_NAMESPACE_END(ZFGlobalEvent)

ZFMETHOD_FUNC_DEFINE_0(const zfchar *, ZFFilePathForSetting)
{
    return _ZFP_ZFFilePathImpl->pathForSetting();
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFFilePathForSetting,
                       ZFMP_IN_OPT(const zfchar *, path, zfnull))
{
    if(ZFGlobalEventCenter::instance()->observerHasAdd(ZFGlobalEvent::EventZFFilePathForSettingOnChange()))
    {
        zfblockedAlloc(v_zfstring, old, _ZFP_ZFFilePathImpl->pathForSetting());
        _ZFP_ZFFilePathImpl->pathForSetting(path);
        ZFGlobalEventCenter::instance()->observerNotify(ZFGlobalEvent::EventZFFilePathForSettingOnChange(), old);
    }
    else
    {
        _ZFP_ZFFilePathImpl->pathForSetting(path);
    }
}

// ============================================================
// storage
ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
ZFOBSERVER_EVENT_GLOBAL_REGISTER(ZFFilePathForStorageOnChange)
ZF_NAMESPACE_END(ZFGlobalEvent)

ZFMETHOD_FUNC_DEFINE_0(const zfchar *, ZFFilePathForStorage)
{
    return _ZFP_ZFFilePathImpl->pathForStorage();
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFFilePathForStorage,
                       ZFMP_IN_OPT(const zfchar *, path, zfnull))
{
    if(ZFGlobalEventCenter::instance()->observerHasAdd(ZFGlobalEvent::EventZFFilePathForStorageOnChange()))
    {
        zfblockedAlloc(v_zfstring, old, _ZFP_ZFFilePathImpl->pathForStorage());
        _ZFP_ZFFilePathImpl->pathForStorage(path);
        ZFGlobalEventCenter::instance()->observerNotify(ZFGlobalEvent::EventZFFilePathForStorageOnChange(), old);
    }
    else
    {
        _ZFP_ZFFilePathImpl->pathForStorage(path);
    }
}

// ============================================================
// storage shared
ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
ZFOBSERVER_EVENT_GLOBAL_REGISTER(ZFFilePathForStorageSharedOnChange)
ZF_NAMESPACE_END(ZFGlobalEvent)

ZFMETHOD_FUNC_DEFINE_0(const zfchar *, ZFFilePathForStorageShared)
{
    return _ZFP_ZFFilePathImpl->pathForStorageShared();
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFFilePathForStorageShared,
                       ZFMP_IN_OPT(const zfchar *, path, zfnull))
{
    if(ZFGlobalEventCenter::instance()->observerHasAdd(ZFGlobalEvent::EventZFFilePathForStorageSharedOnChange()))
    {
        zfblockedAlloc(v_zfstring, old, _ZFP_ZFFilePathImpl->pathForStorageShared());
        _ZFP_ZFFilePathImpl->pathForStorageShared(path);
        ZFGlobalEventCenter::instance()->observerNotify(ZFGlobalEvent::EventZFFilePathForStorageSharedOnChange(), old);
    }
    else
    {
        _ZFP_ZFFilePathImpl->pathForStorageShared(path);
    }
}

// ============================================================
// cache
ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
ZFOBSERVER_EVENT_GLOBAL_REGISTER(ZFFilePathForCacheOnChange)
ZFOBSERVER_EVENT_GLOBAL_REGISTER(ZFFilePathForCacheBeforeClear)
ZFOBSERVER_EVENT_GLOBAL_REGISTER(ZFFilePathForCacheAfterClear)
ZF_NAMESPACE_END(ZFGlobalEvent)

ZFMETHOD_FUNC_DEFINE_0(const zfchar *, ZFFilePathForCache)
{
    return _ZFP_ZFFilePathImpl->pathForCache();
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFFilePathForCache,
                       ZFMP_IN_OPT(const zfchar *, path, zfnull))
{
    if(ZFGlobalEventCenter::instance()->observerHasAdd(ZFGlobalEvent::EventZFFilePathForCacheOnChange()))
    {
        zfblockedAlloc(v_zfstring, old, _ZFP_ZFFilePathImpl->pathForCache());
        _ZFP_ZFFilePathImpl->pathForCache(path);
        ZFGlobalEventCenter::instance()->observerNotify(ZFGlobalEvent::EventZFFilePathForCacheOnChange(), old);
    }
    else
    {
        _ZFP_ZFFilePathImpl->pathForCache(path);
    }
}

ZFMETHOD_FUNC_DEFINE_0(void, ZFFilePathForCacheClear)
{
    if(ZFPROTOCOL_IS_AVAILABLE(ZFFilePath))
    {
        ZFGlobalEventCenter::instance()->observerNotify(ZFGlobalEvent::EventZFFilePathForCacheBeforeClear());
        _ZFP_ZFFilePathImpl->pathForCacheClear();
        ZFGlobalEventCenter::instance()->observerNotify(ZFGlobalEvent::EventZFFilePathForCacheAfterClear());
    }
}

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFFilePathForCacheAutoClear, ZFLevelZFFrameworkLow)
{
    ZFFilePathForCacheClear();
}
ZF_GLOBAL_INITIALIZER_END(ZFFilePathForCacheAutoClear)

ZF_NAMESPACE_GLOBAL_END

