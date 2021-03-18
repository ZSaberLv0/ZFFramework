#include "ZFObjectGlobalInstance.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
#define _ZFP_ZFObjectGlobalInstanceDefine_(Name, Level) \
    ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(Name, Level) \
    { \
    } \
    ZF_GLOBAL_INITIALIZER_DESTROY(Name) \
    { \
        if(!this->instances.isEmpty()) \
        { \
            ZFCoreArrayPOD<ZFCorePointerBase *> tmp; \
            tmp.copyFrom(this->instances); \
            this->instances.removeAll(); \
            for(zfindex i = 0; i < tmp.count(); ++i) \
            { \
                tmp[i]->refDelete(); \
            } \
        } \
    } \
    public: \
        ZFCoreArrayPOD<ZFCorePointerBase *> instances; \
    ZF_GLOBAL_INITIALIZER_END(Name)
#define _ZFP_ZFObjectGlobalInstanceDefine(Name, Level) \
    _ZFP_ZFObjectGlobalInstanceDefine_(Name, Level)

_ZFP_ZFObjectGlobalInstanceDefine(ZFM_CAT(ZFObjectGlobalInstanceLevel, ZFFrameworkEssential), ZFLevelZFFrameworkEssential)
_ZFP_ZFObjectGlobalInstanceDefine(ZFM_CAT(ZFObjectGlobalInstanceLevel, ZFFrameworkHigh), ZFLevelZFFrameworkHigh)
_ZFP_ZFObjectGlobalInstanceDefine(ZFM_CAT(ZFObjectGlobalInstanceLevel, ZFFrameworkNormal), ZFLevelZFFrameworkNormal)
_ZFP_ZFObjectGlobalInstanceDefine(ZFM_CAT(ZFObjectGlobalInstanceLevel, ZFFrameworkLow), ZFLevelZFFrameworkLow)

_ZFP_ZFObjectGlobalInstanceDefine(ZFM_CAT(ZFObjectGlobalInstanceLevel, AppEssential), ZFLevelAppEssential)
_ZFP_ZFObjectGlobalInstanceDefine(ZFM_CAT(ZFObjectGlobalInstanceLevel, AppHigh), ZFLevelAppHigh)
_ZFP_ZFObjectGlobalInstanceDefine(ZFM_CAT(ZFObjectGlobalInstanceLevel, AppNormal), ZFLevelAppNormal)
_ZFP_ZFObjectGlobalInstanceDefine(ZFM_CAT(ZFObjectGlobalInstanceLevel, AppLow), ZFLevelAppLow)

_ZFP_ZFObjectGlobalInstanceDefine(ZFM_CAT(ZFObjectGlobalInstanceLevel, ZFFrameworkPostLow), ZFLevelZFFrameworkPostLow)
_ZFP_ZFObjectGlobalInstanceDefine(ZFM_CAT(ZFObjectGlobalInstanceLevel, ZFFrameworkPostNormal), ZFLevelZFFrameworkPostNormal)
_ZFP_ZFObjectGlobalInstanceDefine(ZFM_CAT(ZFObjectGlobalInstanceLevel, ZFFrameworkPostHigh), ZFLevelZFFrameworkPostHigh)
_ZFP_ZFObjectGlobalInstanceDefine(ZFM_CAT(ZFObjectGlobalInstanceLevel, ZFFrameworkPostEssential), ZFLevelZFFrameworkPostEssential)

static ZFCoreArrayPOD<ZFCorePointerBase *> &_ZFP_ZFObjectGlobalInstanceRef(ZF_IN ZFLevel level)
{
    switch(level)
    {
        case ZFLevelZFFrameworkEssential:
            return ZF_GLOBAL_INITIALIZER_INSTANCE(ZFObjectGlobalInstanceLevelZFFrameworkEssential)->instances;
        case ZFLevelZFFrameworkHigh:
            return ZF_GLOBAL_INITIALIZER_INSTANCE(ZFObjectGlobalInstanceLevelZFFrameworkHigh)->instances;
        case ZFLevelZFFrameworkNormal:
            return ZF_GLOBAL_INITIALIZER_INSTANCE(ZFObjectGlobalInstanceLevelZFFrameworkNormal)->instances;
        case ZFLevelZFFrameworkLow:
            return ZF_GLOBAL_INITIALIZER_INSTANCE(ZFObjectGlobalInstanceLevelZFFrameworkLow)->instances;

        case ZFLevelAppEssential:
            return ZF_GLOBAL_INITIALIZER_INSTANCE(ZFObjectGlobalInstanceLevelAppEssential)->instances;
        case ZFLevelAppHigh:
            return ZF_GLOBAL_INITIALIZER_INSTANCE(ZFObjectGlobalInstanceLevelAppHigh)->instances;
        case ZFLevelAppNormal:
            return ZF_GLOBAL_INITIALIZER_INSTANCE(ZFObjectGlobalInstanceLevelAppNormal)->instances;
        case ZFLevelAppLow:
            return ZF_GLOBAL_INITIALIZER_INSTANCE(ZFObjectGlobalInstanceLevelAppLow)->instances;

        case ZFLevelZFFrameworkPostLow:
            return ZF_GLOBAL_INITIALIZER_INSTANCE(ZFObjectGlobalInstanceLevelZFFrameworkPostLow)->instances;
        case ZFLevelZFFrameworkPostNormal:
            return ZF_GLOBAL_INITIALIZER_INSTANCE(ZFObjectGlobalInstanceLevelZFFrameworkPostNormal)->instances;
        case ZFLevelZFFrameworkPostHigh:
            return ZF_GLOBAL_INITIALIZER_INSTANCE(ZFObjectGlobalInstanceLevelZFFrameworkPostHigh)->instances;
        case ZFLevelZFFrameworkPostEssential:
            return ZF_GLOBAL_INITIALIZER_INSTANCE(ZFObjectGlobalInstanceLevelZFFrameworkPostEssential)->instances;

        default:
            zfCoreCriticalShouldNotGoHere();
            return ZF_GLOBAL_INITIALIZER_INSTANCE(ZFObjectGlobalInstanceLevelAppLow)->instances;
    }
}

ZFCorePointerBase *ZFObjectGlobalInstanceAdd(ZF_IN const ZFCorePointerBase &sp,
                                             ZF_IN_OPT ZFLevel level /* = ZFLevelAppNormal */)
{
    ZFCorePointerBase *t = sp.refNew();
    _ZFP_ZFObjectGlobalInstanceRef(level).add(t);
    return t;
}

ZFCorePointerBase *ZFObjectGlobalInstanceAdd(ZF_IN ZFObject *obj,
                                             ZF_IN_OPT ZFLevel level /* = ZFLevelAppNormal */)
{
    if(obj)
    {
        return ZFObjectGlobalInstanceAdd(ZFCorePointerForZFObject<ZFObject *>(obj), level);
    }
    else
    {
        return zfnull;
    }
}

void ZFObjectGlobalInstanceRemove(ZF_IN ZFCorePointerBase *sp,
                                  ZF_IN ZFLevel level)
{
    ZFCoreArrayPOD<ZFCorePointerBase *> &instances = _ZFP_ZFObjectGlobalInstanceRef(level);
    zfindex index = instances.find(sp);
    if(index != zfindexMax())
    {
        instances.remove(index);
        sp->refDelete();
    }
}

// ============================================================
ZF_STATIC_INITIALIZER_INIT(ZFClassSingletonInstanceRefHolder)
{
}
ZFCoreMap singletonInstanceMap; // _ZFP_ZFClassSingletonPointerHolder *
ZF_STATIC_INITIALIZER_END(ZFClassSingletonInstanceRefHolder)

_ZFP_ZFClassSingletonPointerHolder *_ZFP_ZFClassSingletonInstanceRefAccess(ZF_IN const zfchar *sig)
{
    zfCoreMutexLocker();
    ZFCoreMap &m = ZF_STATIC_INITIALIZER_INSTANCE(ZFClassSingletonInstanceRefHolder)->singletonInstanceMap;
    ZFCorePointerBase *v = m.get(sig);
    if(v == zfnull)
    {
        m.set(sig, ZFCorePointerForObject<_ZFP_ZFClassSingletonPointerHolder *>(zfnew(_ZFP_ZFClassSingletonPointerHolder)));
        v = m.get(sig);
    }
    return v->pointerValueT<_ZFP_ZFClassSingletonPointerHolder *>();
}

ZF_NAMESPACE_GLOBAL_END

