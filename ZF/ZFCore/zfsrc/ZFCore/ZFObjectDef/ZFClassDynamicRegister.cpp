#include "ZFClassDynamicRegister.h"
#include "ZFObjectImpl.h"

#include "../ZFSTLWrapper/zfstl_map.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFClassDynamicRegisterDataHolder, ZFLevelZFFrameworkStatic)
{
}
zfstlmap<const ZFClass *, zfbool> m;
ZF_GLOBAL_INITIALIZER_END(ZFClassDynamicRegisterDataHolder)

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFClassDynamicRegisterAutoRemove, ZFLevelZFFrameworkHigh)
{
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFClassDynamicRegisterAutoRemove)
{
    zfstlmap<const ZFClass *, zfbool> &m = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFClassDynamicRegisterDataHolder)->m;
    if(!m.empty())
    {
        zfstlmap<const ZFClass *, zfbool> t;
        t.swap(m);
        for(zfstlmap<const ZFClass *, zfbool>::iterator it = t.begin(); it != t.end(); ++it)
        {
            ZFClass::_ZFP_ZFClassUnregister(zfnull, it->first);
        }
    }
}
ZF_GLOBAL_INITIALIZER_END(ZFClassDynamicRegisterAutoRemove)

// ============================================================
const ZFClass *ZFClassDynamicRegister(ZF_IN const zfchar *classNameFull,
                                      ZF_IN_OPT const ZFClass *parent /* = zfnull */,
                                      ZF_IN_OPT ZFObject *classDynamicRegisterUserData /* = zfnull */,
                                      ZF_OUT_OPT zfstring *errorHint /* = zfnull */)
{
    if(parent == zfnull)
    {
        parent = ZFObject::ClassData();
    }
    if(parent->classIsAbstract())
    {
        zfstringAppend(errorHint,
            "parent must not be abstract: %s",
            parent->objectInfo().cString());
        return zfnull;
    }
    if(zfsIsEmpty(classNameFull))
    {
        zfstringAppend(errorHint, "null classNameFull");
        return zfnull;
    }
    const ZFClass *cls = ZFClass::classForName(classNameFull);
    if(cls != zfnull)
    {
        zfstringAppend(errorHint,
            "class %s already exist while registering to parent: %s",
            cls->objectInfo().cString(),
            parent->objectInfo().cString());
        return zfnull;
    }
    zfindex dotPos = zfstringFindReversely(classNameFull, zfindexMax(), ZFNamespaceSeparator());
    cls = ZFClass::_ZFP_ZFClassRegister(
        zfnull,
        dotPos == zfindexMax() ? zfnull : zfstring(classNameFull, dotPos).cString(),
        dotPos == zfindexMax() ? classNameFull : classNameFull + dotPos + ZFNamespaceSeparatorLen(),
        parent,
        parent->_ZFP_objectAllocWithCacheCallback(),
        parent->_ZFP_objectConstructor(),
        parent->_ZFP_objectDestructor(),
        zfnull,
        zffalse,
        zftrue,
        classDynamicRegisterUserData);
    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFClassDynamicRegisterDataHolder)->m[cls] = zftrue;
    return cls;
}
void ZFClassDynamicUnregister(ZF_IN const ZFClass *cls)
{
    if(cls == zfnull)
    {
        return ;
    }
    if(!cls->classIsDynamicRegister())
    {
        zfCoreCriticalMessageTrim(
            "[ZFClassDynamicRegister] unregistering class %s that is not dyanmiac registered",
            cls->objectInfo().cString());
    }
    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFClassDynamicRegisterDataHolder)->m.erase(cls);
    cls->classTagRemoveAll();
    ZFClass::_ZFP_ZFClassUnregister(zfnull, cls);
}

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_4(const ZFClass *, ZFClassDynamicRegister, ZFMP_IN(const zfchar *, classNameFull), ZFMP_IN_OPT(const ZFClass *, parent, zfnull), ZFMP_IN_OPT(ZFObject *, classDynamicRegisterUserData, zfnull), ZFMP_OUT_OPT(zfstring *, errorHint, zfnull))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(void, ZFClassDynamicUnregister, ZFMP_IN(const ZFClass *, cls))

ZF_NAMESPACE_GLOBAL_END
#endif

