#include "ZFFilterForZFProperty.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

void ZFFilterForZFProperty::objectInfoT(ZF_IN_OUT zfstring &ret) const
{
    ret += '(';
    zfindex flagsCount = 0;
    if(this->ignorePublicSetter())
    {
        ++flagsCount;
        ret += "ignorePublicSetter, ";
    }
    if(this->ignoreProtectedSetter())
    {
        ++flagsCount;
        ret += "ignoreProtectedSetter, ";
    }
    if(this->ignorePrivateSetter())
    {
        ++flagsCount;
        ret += "ignorePrivateSetter, ";
    }
    if(this->ignorePublicGetter())
    {
        ++flagsCount;
        ret += "ignorePublicGetter, ";
    }
    if(this->ignoreProtectedGetter())
    {
        ++flagsCount;
        ret += "ignoreProtectedGetter, ";
    }
    if(this->ignorePrivateGetter())
    {
        ++flagsCount;
        ret += "ignorePrivateGetter, ";
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

zfbool ZFFilterForZFProperty::filterOnCheckActive(ZF_IN const ZFProperty * const &e) const
{
    if(!zfsuper::filterOnCheckActive(e)
       || !this->classFilter.filterCheckActive(e->propertyOwnerClass()))
    {
        return zffalse;
    }

    if((e->setterMethod()->methodPrivilegeType() == ZFMethodPrivilegeTypePrivate && this->ignorePrivateSetter())
       || (e->setterMethod()->methodPrivilegeType() == ZFMethodPrivilegeTypeProtected && this->ignoreProtectedSetter())
       || (e->setterMethod()->methodPrivilegeType() == ZFMethodPrivilegeTypePublic && this->ignorePublicSetter())
       || (e->getterMethod()->methodPrivilegeType() == ZFMethodPrivilegeTypePrivate && this->ignorePrivateGetter())
       || (e->getterMethod()->methodPrivilegeType() == ZFMethodPrivilegeTypeProtected && this->ignoreProtectedGetter())
       || (e->getterMethod()->methodPrivilegeType() == ZFMethodPrivilegeTypePublic && this->ignorePublicGetter()))
    {
        return zffalse;
    }

    return zftrue;
}

ZF_NAMESPACE_GLOBAL_END

