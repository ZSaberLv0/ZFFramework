#include "ZFLua_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass _ZFP_ZFLua_common_test_Object : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_ZFLua_common_test_Object, ZFObject)

protected:
    zfoverride
    virtual void objectOnInit(void)
    {
        zfLogT() << (void *)this;
        zfsuper::objectOnInit();
    }
    zfoverride
    virtual void objectOnDealloc(void)
    {
        zfLogT() << (void *)this;
        zfsuper::objectOnDealloc();
    }
    zfoverride
    virtual void objectOnRetain(void)
    {
        zfsuper::objectOnRetain();
        zfLogT() << (void *)this << this->objectRetainCount();
    }
    zfoverride
    virtual void objectOnRelease(void)
    {
        zfsuper::objectOnRelease();
        zfLogT() << (void *)this << this->objectRetainCount();
    }

public:
    ZFMETHOD_INLINE_1(zfstring, myMethod, ZFMP_IN(const zfchar *, param0))
    {
        zfLogT() << param0;
        zfstring ret = param0;
        ret += "(modified)";
        return ret;
    }
    ZFMETHOD_INLINE_STATIC_1(zfstring &, MyMethod, ZFMP_IN(zfstring &, param0))
    {
        zfLogT() << param0;
        param0 += "(modified)";
        return param0;
    }

    ZFMETHOD_INLINE_1(void, myMethodOverload, ZFMP_IN(const zfchar *, param0))
    {
        zfLogT() << param0;
    }
    ZFMETHOD_INLINE_1(void, myMethodOverload, ZFMP_IN(zfbool, param0))
    {
        zfLogT() << param0;
    }
    ZFMETHOD_INLINE_STATIC_1(void, MyMethodOverload, ZFMP_IN(const zfchar *, param0))
    {
        zfLogT() << param0;
    }
    ZFMETHOD_INLINE_STATIC_1(void, MyMethodOverload, ZFMP_IN(zfbool, param0))
    {
        zfLogT() << param0;
    }
};
ZFOBJECT_REGISTER(_ZFP_ZFLua_common_test_Object)

zfclass ZFLua_common_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFLua_common_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();

        {
            ZFCoreStatisticInvokeTimeLoggerOneTime("run lua code");
            ZFLuaExecute(
                    "local obj = _ZFP_ZFLua_common_test_Object()\n"
                    "print('========================================')\n"
                    "print('print by print():')\n"
                    "print(obj)\n"
                    "print('print by zfLog():')\n"
                    "zfLog('%s', obj)\n"
                    "print('========================================')\n"
                    "print('enum convert:')\n"
                    "print(ZFFileOpenOption.e_Create())\n"
                    "print(ZFFileOpenOptionFlags('Create|Read'))\n"
                    "print('========================================')\n"
                    "print('call member method:')\n"
                    "obj:myMethod('param')\n"
                    "print('========================================')\n"
                    "print('call static member method:')\n"
                    "local param0 = zfstring('param')\n"
                    "_ZFP_ZFLua_common_test_Object.MyMethod(param0)\n"
                    "print(param0)\n"
                    "print('========================================')\n"
                    "print('call overload method:')\n"
                    "obj:myMethodOverload(zfstring('param'))\n"
                    "obj:myMethodOverload(zfbool('true'))\n"
                    "_ZFP_ZFLua_common_test_Object.MyMethodOverload(zfstring('param'))\n"
                    "_ZFP_ZFLua_common_test_Object.MyMethodOverload(zfbool('true'))\n"
                    "print('========================================')\n"
                    "print('when not explicitly specified, overload method may dispatch unexpectly:')\n"
                    "obj:myMethodOverload('param')\n"
                    "obj:myMethodOverload('true')\n"
                );
        }

        {
            ZFCoreStatisticInvokeTimeLoggerOneTime("lua gc");
            ZFLuaGC();
        }

        this->testCaseStop();
    }
};
ZFOBJECT_REGISTER(ZFLua_common_test)

ZF_NAMESPACE_GLOBAL_END

