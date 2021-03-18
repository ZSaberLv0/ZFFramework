#include "ZFCore_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass _ZFP_ZFCore_ZFSerializable_test_TestClass : zfextends ZFObject, zfimplements ZFSerializable
{
    ZFOBJECT_DECLARE(_ZFP_ZFCore_ZFSerializable_test_TestClass, ZFObject)
    ZFIMPLEMENTS_DECLARE(ZFSerializable)

public:
    ZFPROPERTY_ASSIGN(zfstring, stringInParent)

protected:
    zfoverride
    virtual void objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
    {
        zfsuper::objectInfoOnAppend(ret);
        ZFClassUtil::objectPropertyInfo(ret, this);
    }
};

zfclass _ZFP_ZFCore_ZFSerializable_test_TestClassChild : zfextends _ZFP_ZFCore_ZFSerializable_test_TestClass
{
    ZFOBJECT_DECLARE(_ZFP_ZFCore_ZFSerializable_test_TestClassChild, _ZFP_ZFCore_ZFSerializable_test_TestClass)

    ZFPROPERTY_ASSIGN(zfstring, stringInChild)
};

zfclass _ZFP_ZFCore_ZFSerializable_test_TestClassContainer : zfextends ZFObject, zfimplements ZFSerializable
{
    ZFOBJECT_DECLARE(_ZFP_ZFCore_ZFSerializable_test_TestClassContainer, ZFObject)
    ZFIMPLEMENTS_DECLARE(ZFSerializable)

public:
    ZFPROPERTY_RETAIN(ZFObject *, serializableMember)

protected:
    zfoverride
    virtual void objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
    {
        ret += this->classData()->classNameFull();
        ZFClassUtil::objectPropertyInfo(ret, this);
    }
};

// ============================================================
zfclass ZFCore_ZFSerializable_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFCore_ZFSerializable_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();

        this->testCaseOutputSeparator();
        this->testCaseOutput("ZFSerializable: normal serializable object");
        this->test(this->obj);

        this->testCaseOutputSeparator();
        this->testCaseOutput("ZFSerializable: inherit serializable object");
        this->test(this->objChild);

        this->testCaseOutputSeparator();
        this->testCaseOutput("ZFSerializable: serializable object that contains another serializable object");
        this->test(this->objContainer);

        this->testCaseStop();
    }
protected:
    virtual void objectOnInit(void)
    {
        zfsuper::objectOnInit();
        this->memberPrepare();
    }
    virtual void objectOnDealloc(void)
    {
        this->memberDestroy();
        zfsuper::objectOnDealloc();
    }
private:
    _ZFP_ZFCore_ZFSerializable_test_TestClass *obj;
    _ZFP_ZFCore_ZFSerializable_test_TestClassChild *objChild;
    _ZFP_ZFCore_ZFSerializable_test_TestClassContainer *objContainer;
    void memberPrepare(void)
    {
        this->obj = zfAlloc(_ZFP_ZFCore_ZFSerializable_test_TestClass);
        this->obj->stringInParent("base's string, with unicode chars: \"啊哦\"");

        this->objChild = zfAlloc(_ZFP_ZFCore_ZFSerializable_test_TestClassChild);
        this->objChild->stringInParent("child's string");
        this->objChild->stringInChild("child's string in child");

        this->objContainer = zfAlloc(_ZFP_ZFCore_ZFSerializable_test_TestClassContainer);
        _ZFP_ZFCore_ZFSerializable_test_TestClassChild *objTmp = zfAlloc(_ZFP_ZFCore_ZFSerializable_test_TestClassChild);
        objTmp->stringInParent("container's string");
        objTmp->stringInChild("container's string");
        this->objContainer->serializableMember(objTmp);
        zfRelease(objTmp);
    }
    void memberDestroy(void)
    {
        zfRelease(this->obj);
        this->obj = zfnull;
        zfRelease(this->objChild);
        this->objChild = zfnull;
        zfRelease(this->objContainer);
        this->objContainer = zfnull;
    }
    void test(ZFSerializable *serializableObj)
    {
        zfstring encodedData;

        this->testCaseOutput("object:\n%s\n", serializableObj->toObject()->objectInfo().cString());

        {
            ZFSerializableData serializableData;
            ZFObjectToData(serializableData, serializableObj->toObject());
            ZFSerializableDataToZfsd(encodedData, serializableData);
            this->testCaseOutput("encodedData:\n%s\n", encodedData.cString());
        }

        {
            ZFSerializableData serializableData;
            serializableObj->serializeToData(serializableData);
            this->testCaseOutput("serializableData:\n%s\n", serializableData.objectInfo().cString());
        }

        {
            ZFSerializableData serializableData;
            ZFSerializableDataFromZfsd(serializableData, encodedData);
            zfautoObject newSerializableObj = ZFObjectFromData(serializableData);
            this->testCaseOutput("re-serialize from encodedData, result:\n%s\n", newSerializableObj.toObject()->objectInfo().cString());
        }
    }
};
ZFOBJECT_REGISTER(ZFCore_ZFSerializable_test)

ZF_NAMESPACE_GLOBAL_END

