#include "ZFIdentityUtil.h"
#include "ZFCoreArray.h"
#include "ZFCore/ZFSTLWrapper/zfstl_map.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassNotPOD _ZFP_ZFIdentityGeneratorPrivate
{
public:
    zfuint refCount;
    zfidentity cur;
    zfstlmap<zfidentity, zfbool> used;

public:
    _ZFP_ZFIdentityGeneratorPrivate(void)
    : refCount(1)
    , cur(zfidentityInvalid())
    , used()
    {
        this->used[zfidentityInvalid()] = zftrue;
    }
};

// ============================================================
ZFIdentityGenerator::ZFIdentityGenerator(void)
: d(zfnew(_ZFP_ZFIdentityGeneratorPrivate))
{
}
ZFIdentityGenerator::ZFIdentityGenerator(ZF_IN ZFIdentityGenerator const &ref)
: d(ref.d)
{
    ++(d->refCount);
}
ZFIdentityGenerator &ZFIdentityGenerator::operator = (ZF_IN ZFIdentityGenerator const &ref)
{
    _ZFP_ZFIdentityGeneratorPrivate *dTmp = d;
    d = ref.d;
    ++(ref.d->refCount);
    --(dTmp->refCount);
    if(dTmp->refCount == 0)
    {
        zfdelete(dTmp);
    }
    return *this;
}
zfbool ZFIdentityGenerator::operator == (ZF_IN ZFIdentityGenerator const &ref) const
{
    return (d == ref.d);
}
ZFIdentityGenerator::~ZFIdentityGenerator(void)
{
    --(d->refCount);
    if(d->refCount == 0)
    {
        zfdelete(d);
    }
}

zfidentity ZFIdentityGenerator::idAcquire(void)
{
    do
    {
        ++(d->cur);
    } while(d->cur == zfidentityInvalid() || d->used.find(d->cur) != d->used.end());
    d->used[d->cur] = zftrue;
    return d->cur;
}
void ZFIdentityGenerator::idRelease(ZF_IN zfidentity identity)
{
    d->used.erase(identity);
}

zfbool ZFIdentityGenerator::idUsed(ZF_IN zfidentity identity) const
{
    return (d->used.find(identity) != d->used.end());
}
void ZFIdentityGenerator::idUsedGetAll(ZF_IN_OUT ZFCoreArray<zfidentity> &ret) const
{
    ret.capacity(ret.capacity() + (zfindex)d->used.size());
    for(zfstlmap<zfidentity, zfbool>::iterator it = d->used.begin(); it != d->used.end(); ++it)
    {
        ret.add(it->first);
    }
}

// ============================================================
zfidentity zfidentityCalcString(ZF_IN const zfchar *src, ZF_IN_OPT zfindex srcLen /* = zfindexMax() */)
{
    zfidentity hash = zfidentityZero();
    if(src)
    {
        if(srcLen == zfindexMax())
        {
            while(*src)
            {
                hash = hash * 131 + (*src++);
            }
        }
        else
        {
            const zfchar *srcEnd = src + srcLen;
            while(src < srcEnd)
            {
                hash = hash * 131 + (*src++);
            }
        }
    }
    return hash;
}
zfidentity zfidentityCalcBuf(ZF_IN const void *src, ZF_IN zfindex srcLen)
{
    const zfbyte *p = (const zfbyte *)src;
    const zfbyte *pEnd = p + srcLen;
    zfidentity hash = zfidentityZero();
    if(p)
    {
        while(p < pEnd)
        {
            hash = hash * 131 + (*p++);
        }
    }
    return hash;
}

zfidentity zfidentityCalcPointer(ZF_IN const void *p)
{
    return zfidentityCalcBuf(&p, sizeof(const void *));
}
zfidentity zfidentityCalcPointer(ZF_IN ZFFuncAddrType p)
{
    return zfidentityCalcBuf(&p, sizeof(ZFFuncAddrType));
}

ZF_NAMESPACE_GLOBAL_END

