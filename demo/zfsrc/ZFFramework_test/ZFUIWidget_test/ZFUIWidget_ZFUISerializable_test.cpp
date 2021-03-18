#include "ZFUIWidget_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUIWidget_ZFUISerializable_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFUIWidget_ZFUISerializable_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();

        zfautoObject testObject = this->prepareTestObject();

        {
            zfstring s;
            this->testCaseOutputSeparator();
            this->testCaseOutput("xml:");
            ZFObjectToXml(ZFOutputForString(s), testObject);
            ZFOutputDefault() << s;
            this->testCaseOutput("xml re-serialized:");
            ZFObjectToXml(ZFOutputDefault(), ZFObjectFromXml(ZFInputForBufferUnsafe(s)));
        }

        {
            zfstring s;
            this->testCaseOutputSeparator();
            this->testCaseOutput("json:");
            ZFObjectToJson(ZFOutputForString(s), testObject);
            ZFOutputDefault() << s;
            this->testCaseOutput("json re-serialized:");
            ZFObjectToJson(ZFOutputDefault(), ZFObjectFromJson(ZFInputForBufferUnsafe(s)));
        }

        {
            zfstring s;
            this->testCaseOutputSeparator();
            this->testCaseOutput("zfsd:");
            ZFObjectToZfsd(ZFOutputForString(s), testObject);
            ZFOutputDefault() << s;
            this->testCaseOutput("zfsd re-serialized:");
            ZFObjectToZfsd(ZFOutputDefault(), ZFObjectFromZfsd(ZFInputForBufferUnsafe(s)));
        }

        this->testCaseStop();
    }

private:
    zfautoObject prepareTestObject(void)
    {
        zfblockedAlloc(ZFUIAutoLayout, parent);
        parent->viewAlpha(0.5f);

        zfblockedAlloc(ZFUIView, child0);
        child0->viewUIEnableTree(zffalse);
        zfal_maker(child0, parent).left().toParentLeft();

        zfblockedAlloc(ZFUITextView, child1);
        child1->text("special chars: \r\n\t\"'-_=+<>()[]{}");
        zfal_maker(child1, parent).right().toParentRight();

        return parent;
    }
};
ZFOBJECT_REGISTER(ZFUIWidget_ZFUISerializable_test)

ZF_NAMESPACE_GLOBAL_END

