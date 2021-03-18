#include "ZFCore_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZF_ENV_EXPORT _ZFP_ZFCore_ZFPropertyDynamic_test_Object : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_ZFCore_ZFPropertyDynamic_test_Object, ZFObject)

public:
    zfint myProp(void) const
    {
        return _ZFP_myProp.zfv;
    }
    void myProp(ZF_IN zfint v)
    {
        _ZFP_myProp.zfv = v;
    }
private:
    zftValue<zfint> _ZFP_myProp;
};

// ============================================================
zfclass ZFCore_ZFPropertyDynamic_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFCore_ZFPropertyDynamic_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();

        const ZFClass *cls = _ZFP_ZFCore_ZFPropertyDynamic_test_Object::ClassData();

        const ZFMethod *setterMethod = ZFMethodDynamicRegister(ZFMethodDynamicRegisterParam()
                .methodOwnerClass(cls)
                .methodGenericInvoker(_setterGI)
                .methodType(ZFMethodTypeNormal)
                .methodName("myProp")
                .methodParamAdd(ZFTypeId_zfint(), "zfint const &")
            );
        const ZFMethod *getterMethod = ZFMethodDynamicRegister(ZFMethodDynamicRegisterParam()
                .methodOwnerClass(cls)
                .methodGenericInvoker(_getterGI)
                .methodType(ZFMethodTypeNormal)
                .methodName("myProp")
                .methodReturnTypeId(ZFTypeId_zfint())
            );

        const ZFProperty *property = ZFPropertyDynamicRegister(ZFPropertyDynamicRegisterParam()
                .propertyOwnerClass(cls)
                .propertyTypeId(ZFTypeId_zfint())
                .propertyName("myProp")
                .propertyCustomImpl(setterMethod, getterMethod, _callbackIsValueAccessed, _callbackIsInitValue, _callbackValueReset)
            );

        this->testCaseOutput("property: %s", property->objectInfo().cString());

        zfblockedAlloc(_ZFP_ZFCore_ZFPropertyDynamic_test_Object, obj);
        obj->myProp(123);
        this->testCaseOutput("obj: %s", ZFClassUtil::objectInfo(obj).cString());

        ZFPropertyDynamicUnregister(property);
        ZFMethodDynamicUnregister(setterMethod);
        ZFMethodDynamicUnregister(getterMethod);
        this->testCaseStop();
    }
private:
    static const zfchar *_valueKey(void)
    {
        return "ZFCore_ZFPropertyDynamic_test_myProp";
    }
    static zfbool _setterGI(ZFMETHOD_GENERIC_INVOKER_PARAMS)
    {
        _ZFP_ZFCore_ZFPropertyDynamic_test_Object *obj = invokerObject->toAny();
        obj->myProp(paramList[0]->to<v_zfint *>()->zfv);
        obj->objectTag(_valueKey(), paramList[0]->to<ZFCopyable *>()->copy());
        return zftrue;
    }
    static zfbool _getterGI(ZFMETHOD_GENERIC_INVOKER_PARAMS)
    {
        _ZFP_ZFCore_ZFPropertyDynamic_test_Object *obj = invokerObject->toAny();
        zfautoObject tag = zflineAlloc(v_zfint, obj->myProp());
        obj->objectTag(_valueKey(), tag);
        ret = tag;
        return zftrue;
    }
    static zfbool _callbackIsValueAccessed(ZF_IN const ZFProperty *property,
                                           ZF_IN ZFObject *ownerObj)
    {
        return !_callbackIsInitValue(property, ownerObj);
    }
    static zfbool _callbackIsInitValue(ZF_IN const ZFProperty *property,
                                       ZF_IN ZFObject *ownerObj,
                                       ZF_OUT_OPT zfautoObject *outInitValue = zfnull)
    {
        _ZFP_ZFCore_ZFPropertyDynamic_test_Object *obj = ownerObj->toAny();
        if(outInitValue != zfnull)
        {
            *outInitValue = zflineAlloc(v_zfint);
        }
        return (obj->myProp() == 0);
    }
    static void _callbackValueReset(ZF_IN const ZFProperty *property,
                                    ZF_IN ZFObject *ownerObj)
    {
        _ZFP_ZFCore_ZFPropertyDynamic_test_Object *obj = ownerObj->toAny();
        obj->objectTagRemove(_valueKey());
        obj->myProp(0);
    }
};
ZFOBJECT_REGISTER(ZFCore_ZFPropertyDynamic_test)

ZF_NAMESPACE_GLOBAL_END

