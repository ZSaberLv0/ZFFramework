#include "ZFLua_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass _ZFP_ZFLua_gc_test_Object : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_ZFLua_gc_test_Object, ZFObject)

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
};
ZFOBJECT_REGISTER(_ZFP_ZFLua_gc_test_Object)

zfclass ZFLua_gc_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFLua_gc_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();

        zfLogTrimT() << "normal begin";
        ZFLuaExecute(
                "local obj = _ZFP_ZFLua_gc_test_Object();\n"
            );
        zfLogTrimT() << "normal gc begin";
        ZFLuaGC();
        zfLogTrimT() << "normal gc end";
        zfLogTrimT() << "normal end";

        zfLogTrimT() << "============================================================";
        zfLogTrimT() << "note:";
        zfLogTrimT() << "    if compiled without cpp exception,";
        zfLogTrimT() << "    cpp object would leak when lua exception occurred";
        zfLogTrimT() << "============================================================";
        zfLogTrimT() << "lua exception begin";
        #if ZF_ENV_ZFLUA_USE_EXCEPTION
            ZFLuaExecute(
                    "local obj = _ZFP_ZFLua_gc_test_Object();\n"
                    "ZFObject.notExistFunc(obj);\n"
                );
            zfLogTrimT() << "lua exception gc begin";
            ZFLuaGC();
            zfLogTrimT() << "lua exception gc end";
        #else
            zfLogTrimT() << "lua exception disabled";
        #endif
        zfLogTrimT() << "lua exception end";

        this->testCaseStop();
    }
};
ZFOBJECT_REGISTER(ZFLua_gc_test)

ZF_NAMESPACE_GLOBAL_END

