#include "ZFFilterForZFMethod.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

void ZFFilterForZFMethod::objectInfoT(ZF_IN_OUT zfstring &ret) const
{
    ret += '(';
    zfindex flagsCount = 0;
    if(this->ignorePublic())
    {
        ++flagsCount;
        ret += "ignorePublic, ";
    }
    if(this->ignoreProtected())
    {
        ++flagsCount;
        ret += "ignoreProtected, ";
    }
    if(this->ignorePrivate())
    {
        ++flagsCount;
        ret += "ignorePrivate, ";
    }

    if(flagsCount == 0)
    {
        ret.remove(ret.length() - 1);
    }
    else
    {
        ret.remove(ret.length() - 2);
        ret += ')';
    }

    zfsuper::objectInfoT(ret);
}

zfbool ZFFilterForZFMethod::filterOnCheckActive(ZF_IN const ZFMethod * const &e) const
{
    if(!zfsuper::filterOnCheckActive(e)
       || !this->classFilter.filterCheckActive(e->methodOwnerClass()))
    {
        return zffalse;
    }

    if((e->methodPrivilegeType() == ZFMethodPrivilegeTypePrivate && this->ignorePrivate())
       || (e->methodPrivilegeType() == ZFMethodPrivilegeTypeProtected && this->ignoreProtected())
       || (e->methodPrivilegeType() == ZFMethodPrivilegeTypePublic && this->ignorePublic()))
    {
        return zffalse;
    }

    return zftrue;
}

ZF_NAMESPACE_GLOBAL_END

