#include "ZFFilterForZFObject.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFFilterForZFObject::ZFFilterForZFObject(void)
: zfsuper()
, classFilter()
{
}
ZFFilterForZFObject::ZFFilterForZFObject(ZF_IN const ZFFilterForZFObject &ref)
: zfsuper(ref)
, classFilter(ref.classFilter)
{
}
ZFFilterForZFObject &ZFFilterForZFObject::operator = (ZF_IN const ZFFilterForZFObject &ref)
{
    zfsuper::operator = (ref);
    this->classFilter = ref.classFilter;
    return *this;
}
ZFFilterForZFObject::~ZFFilterForZFObject(void)
{
    for(zfindex i = 0; i < this->filterCount(); ++i)
    {
        zfRelease(this->filterInternalAtIndex(i));
    }
}

void ZFFilterForZFObject::copyFrom(ZF_IN ZFFilterBase<ZFObject *, ZFObject *> const &ref)
{
    zfsuper::copyFrom(ref);
    for(zfindex i = 0; i < this->filterCount(); ++i)
    {
        zfRetain(this->filterInternalAtIndex(i));
    }
    this->classFilter.copyFrom(((ZFFilterForZFObject const &)ref).classFilter);
}

void ZFFilterForZFObject::filterOnStore(ZF_IN_OUT ZFObject * &to,
                                        ZF_IN ZFObject * const &from) const
{
    to = zfRetain(from);
}
void ZFFilterForZFObject::filterOnRemove(ZF_IN_OUT ZFObject * &value) const
{
    zfRelease(value);
}
zfbool ZFFilterForZFObject::filterOnCheckEqual(ZF_IN ZFObject * const &e1,
                                               ZF_IN ZFObject * const &e2) const
{
    return (ZFObjectCompare(e1, e2) == ZFCompareTheSame);
}
zfbool ZFFilterForZFObject::filterOnCheckActive(ZF_IN ZFObject * const &e) const
{
    if(!zfsuper::filterOnCheckActive(e))
    {
        return zffalse;
    }
    return this->classFilter.filterCheckActive(e ? e->classData() : zfnull);
}

ZF_NAMESPACE_GLOBAL_END

