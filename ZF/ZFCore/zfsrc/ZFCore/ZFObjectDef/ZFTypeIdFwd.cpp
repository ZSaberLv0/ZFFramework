#include "ZFTypeIdFwd.h"
#include "ZFObjectImpl.h"
#include "ZFListenerDeclare.h"

#include "ZFCore/ZFSTLWrapper/zfstl_string.h"
#include "ZFCore/ZFSTLWrapper/zfstl_map.h"
#include "ZFCore/ZFSTLWrapper/zfstl_vector.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZF_STATIC_INITIALIZER_INIT(ZFTypeInfoHolder)
{
}
ZF_STATIC_INITIALIZER_DESTROY(ZFTypeInfoHolder)
{
    zfCoreAssert(m.empty());
}
zfstlmap<zfstlstringZ, ZFTypeInfo *> m;
ZF_STATIC_INITIALIZER_END(ZFTypeInfoHolder)

void _ZFP_ZFTypeInfoRegister(ZF_IN const zfchar *typeId,
                             ZF_IN ZFTypeInfo *typeIdData)
{
    zfCoreMutexLocker();
    zfstlmap<zfstlstringZ, ZFTypeInfo *> &m = ZF_STATIC_INITIALIZER_INSTANCE(ZFTypeInfoHolder)->m;
    zfCoreAssert(m.find(typeId) == m.end());
    m[typeId] = typeIdData;
}
ZFTypeInfo *_ZFP_ZFTypeInfoUnregister(ZF_IN const zfchar *typeId)
{
    zfCoreMutexLocker();
    zfstlmap<zfstlstringZ, ZFTypeInfo *> &m = ZF_STATIC_INITIALIZER_INSTANCE(ZFTypeInfoHolder)->m;
    zfstlmap<zfstlstringZ, ZFTypeInfo *>::iterator it = m.find(typeId);
    zfCoreAssert(it != m.end());
    ZFTypeInfo *t = it->second;
    m.erase(it);
    return t;
}
const ZFTypeInfo *ZFTypeInfoForName(ZF_IN const zfchar *typeId)
{
    zfCoreMutexLocker();
    zfstlmap<zfstlstringZ, ZFTypeInfo *> &m = ZF_STATIC_INITIALIZER_INSTANCE(ZFTypeInfoHolder)->m;
    zfstlmap<zfstlstringZ, ZFTypeInfo *>::iterator it = m.find(typeId);
    if(it != m.end())
    {
        return it->second;
    }
    else
    {
        return zfnull;
    }
}
void ZFTypeInfoGetAllT(ZF_IN_OUT ZFCoreArray<const ZFTypeInfo *> &ret)
{
    zfCoreMutexLocker();
    zfstlmap<zfstlstringZ, ZFTypeInfo *> &m = ZF_STATIC_INITIALIZER_INSTANCE(ZFTypeInfoHolder)->m;
    for(zfstlmap<zfstlstringZ, ZFTypeInfo *>::iterator it = m.begin(); it != m.end(); ++it)
    {
        ret.add(it->second);
    }
}

// ============================================================
// special alias implicit convert
zfclass _ZFP_I_PropAliasHolder : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_I_PropAliasHolder, ZFObject)
public:
    ZFObject *obj;
    void *v;
    _ZFP_PropAliasDetachCallback detachCallback;
protected:
    zfoverride
    virtual void objectOnDeallocPrepare(void)
    {
        this->cleanup();
        zfsuper::objectOnDeallocPrepare();
    }
private:
    void cleanup(void)
    {
        if(this->detachCallback != zfnull)
        {
            _ZFP_PropAliasDetachCallback detachCallbackTmp = this->detachCallback;
            this->detachCallback = zfnull;
            detachCallbackTmp(this->obj, this->v);
        }
    }
};

void _ZFP_PropAliasAttach(ZF_IN ZFObject *obj,
                          ZF_IN void *v,
                          ZF_IN const zfchar *typeName,
                          ZF_IN _ZFP_PropAliasDetachCallback detachCallback)
{
    zfstring key = "_ZFP_PropTypeAlias_";
    key += typeName;
    _ZFP_I_PropAliasHolder *d = zfAlloc(_ZFP_I_PropAliasHolder);
    d->obj = obj;
    d->v = v;
    d->detachCallback = detachCallback;
    obj->objectTag(key, d);
    zfRelease(d);
}
void _ZFP_PropAliasDetach(ZF_IN ZFObject *obj,
                          ZF_IN const zfchar *typeName)
{
    zfstring key = "_ZFP_PropTypeAlias_";
    key += typeName;
    obj->objectTagRemove(key);
}

void _ZFP_ZFTypeIdWrapperMarkConst(ZF_IN_OUT_OPT ZFObject *zfv)
{
    ZFTypeIdWrapper::markConst(zfv);
}

ZF_NAMESPACE_GLOBAL_END

