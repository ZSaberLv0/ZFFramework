#include "ZFUtility_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass _ZFP_ZFUtility_ZFAni_test_Object : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_ZFUtility_ZFAni_test_Object, ZFObject)

    ZFPROPERTY_ASSIGN(zfint, testProp)
    ZFPROPERTY_OVERRIDE_ON_ATTACH_INLINE(zfint, testProp)
    {
        zfLogTrimT() << "[ZFAni_test] testProp:" << this->testProp();
    }

protected:
    zfoverride
    virtual void objectOnInit(void)
    {
        zfsuper::objectOnInit();
        zfLogTrimT() << "[ZFAni_test] objectOnInit" << this->objectHash();
    }
    zfoverride
    virtual void objectOnDealloc(void)
    {
        zfLogTrimT() << "[ZFAni_test] objectOnDealloc" << this->objectHash();
        zfsuper::objectOnDealloc();
    }
    zfoverride
    virtual void objectOnRetain(void)
    {
        zfsuper::objectOnRetain();
        zfLogTrimT() << "[ZFAni_test] objectOnRetain" << this->objectHash() << this->objectRetainCount();
    }
    zfoverride
    virtual void objectOnRelease(void)
    {
        zfLogTrimT() << "[ZFAni_test] objectOnRelease" << this->objectHash() << this->objectRetainCount();
        zfsuper::objectOnRelease();
    }
};

zfclass ZFUtility_ZFAni_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFUtility_ZFAni_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();

        zfblockedAlloc(_ZFP_ZFUtility_ZFAni_test_Object, target);
        zfautoObject aniHolder = ZFAni(target, "testProp", "-100", "200");
        ZFAnimation *ani = aniHolder;
        ani->aniDuration(2000);

        ZFLISTENER_LOCAL(aniOnStop, {
                userData->objectHolded<ZFTestCase *>()->testCaseStop();
            })
        ani->observerAdd(ZFAnimation::EventAniOnStop(), aniOnStop, this->objectHolder());

        ZFLISTENER_LOCAL(aniOnDealloc, {
                zfLogTrimT() << "[ZFAni_test] aniOnDealloc" << listenerData.sender()->objectHash();
            })
        ani->observerAdd(ZFObject::EventObjectBeforeDealloc(), aniOnDealloc);

        ani->aniStart();
    }
};
ZFOBJECT_REGISTER(ZFUtility_ZFAni_test)

ZF_NAMESPACE_GLOBAL_END

