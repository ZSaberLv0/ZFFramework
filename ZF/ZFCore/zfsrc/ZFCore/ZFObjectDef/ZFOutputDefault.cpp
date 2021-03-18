#include "ZFOutputDefault.h"
#include "ZFObjectImpl.h"
#include "ZFGlobalEventCenter.h"

ZF_NAMESPACE_GLOBAL_BEGIN

static ZFCoreArray<ZFOutput> &_ZFP_ZFOutputDefault_list(void)
{
    static ZFCoreArray<ZFOutput> d;
    return d;
}
static zfindex _ZFP_ZFOutputDefault_callback(ZF_IN const void *src, ZF_IN_OPT zfindex count = zfindexMax())
{
    ZFCoreArray<ZFOutput> &l = _ZFP_ZFOutputDefault_list();
    for(zfindex i = 0; i < l.count(); ++i)
    {
        if(l[i].callbackIsValid())
        {
            l[i].execute(src, count);
        }
    }
    return count;
}
const ZFOutput &_ZFP_ZFOutputDefault(void)
{
    static ZFOutput d = ZFCallbackForFunc(_ZFP_ZFOutputDefault_callback);
    return d;
}

void ZFOutputDefaultAdd(ZF_IN const ZFOutput &v)
{
    ZFCoreArray<ZFOutput> &l = _ZFP_ZFOutputDefault_list();
    l.add(v);
}
void ZFOutputDefaultRemove(ZF_IN const ZFOutput &v)
{
    ZFCoreArray<ZFOutput> &l = _ZFP_ZFOutputDefault_list();
    l.removeElement(v);
}

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFOutputDefaultCleanup, ZFLevelZFFrameworkEssential)
{
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFOutputDefaultCleanup)
{
    ZFCoreArray<ZFOutput> &l = _ZFP_ZFOutputDefault_list();
    l.removeAll();
}
ZF_GLOBAL_INITIALIZER_END(ZFOutputDefaultCleanup)

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_0(const ZFOutput &, ZFOutputDefault)
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(void, ZFOutputDefaultAdd, ZFMP_IN(const ZFOutput &, v))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(void, ZFOutputDefaultRemove, ZFMP_IN(const ZFOutput &, v))

ZF_NAMESPACE_GLOBAL_END
#endif

