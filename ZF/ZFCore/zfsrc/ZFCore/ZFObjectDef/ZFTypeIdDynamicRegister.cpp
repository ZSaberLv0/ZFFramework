#include "ZFTypeIdDynamicRegister.h"
#include "ZFObjectImpl.h"

#include "../ZFSTLWrapper/zfstl_string.h"
#include "../ZFSTLWrapper/zfstl_map.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFTypeIdDynamicRegisterDataHolder, ZFLevelZFFrameworkStatic)
{
}
zfstlmap<zfstlstringZ, ZFCorePointerForObject<ZFTypeInfo *> > m;
ZF_GLOBAL_INITIALIZER_END(ZFTypeIdDynamicRegisterDataHolder)

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFTypeIdDynamicRegisterAutoRemove, ZFLevelZFFrameworkHigh)
{
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFTypeIdDynamicRegisterAutoRemove)
{
    zfstlmap<zfstlstringZ, ZFCorePointerForObject<ZFTypeInfo *> > &m = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFTypeIdDynamicRegisterDataHolder)->m;
    if(!m.empty())
    {
        zfstlmap<zfstlstringZ, ZFCorePointerForObject<ZFTypeInfo *> > t;
        t.swap(m);
        for(zfstlmap<zfstlstringZ, ZFCorePointerForObject<ZFTypeInfo *> >::iterator it = t.begin(); it != t.end(); ++it)
        {
            _ZFP_ZFTypeInfoUnregister(it->first.c_str());
        }
    }
}
ZF_GLOBAL_INITIALIZER_END(ZFTypeIdDynamicRegisterAutoRemove)

// ============================================================
static zfbool _ZFP_ZFTypeIdGI(ZFMETHOD_GENERIC_INVOKER_PARAMS)
{
    ret = zflineAlloc(v_zfstring, invokerMethod->methodName() + zfslen("ZFTypeId_"));
    return zftrue;
}
zfbool ZFTypeIdDynamicRegister(ZF_IN const zfchar *typeIdName,
                               ZF_IN const ZFCorePointerForObject<ZFTypeInfo *> &typeIdData,
                               ZF_OUT_OPT zfstring *errorHint /* = zfnull */)
{
    if(zfsIsEmpty(typeIdName))
    {
        zfstringAppend(errorHint, "empty typeIdName");
        return zffalse;
    }
    if(typeIdData == zfnull)
    {
        zfstringAppend(errorHint, "null typeIdData");
        return zffalse;
    }
    ZF_GLOBAL_INITIALIZER_CLASS(ZFTypeIdDynamicRegisterDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFTypeIdDynamicRegisterDataHolder);
    if(d->m.find(typeIdName) != d->m.end())
    {
        zfstringAppend(errorHint, "type id %s already registered", typeIdName);
        return zffalse;
    }
    if(!ZFMethodDynamicRegister(ZFMethodDynamicRegisterParam()
            .methodGenericInvoker(_ZFP_ZFTypeIdGI)
            .methodReturnTypeId(ZFTypeId_zfstring())
            .methodName(zfstringWithFormat("ZFTypeId_%s", typeIdName))
        , errorHint))
    {
        return zffalse;
    }
    d->m[typeIdName] = typeIdData;
    _ZFP_ZFTypeInfoRegister(typeIdName, typeIdData);
    return zftrue;
}
void ZFTypeIdDynamicUnregister(ZF_IN const zfchar *typeIdName)
{
    if(!zfsIsEmpty(typeIdName))
    {
        ZF_GLOBAL_INITIALIZER_CLASS(ZFTypeIdDynamicRegisterDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFTypeIdDynamicRegisterDataHolder);
        zfstlmap<zfstlstringZ, ZFCorePointerForObject<ZFTypeInfo *> >::iterator it = d->m.find(typeIdName);
        if(it != d->m.end())
        {
            ZFMethodDynamicUnregister(ZFMethodForName(ZF_NAMESPACE_GLOBAL_NAME,
                zfstringWithFormat("ZFTypeId_%s", typeIdName)));
            _ZFP_ZFTypeInfoUnregister(typeIdName);
            d->m.erase(it);
        }
    }
}

ZF_NAMESPACE_GLOBAL_END

