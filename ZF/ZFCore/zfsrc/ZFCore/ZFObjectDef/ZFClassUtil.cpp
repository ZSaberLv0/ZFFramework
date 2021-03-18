#include "ZFClassUtil.h"
#include "ZFObjectImpl.h"
#include "ZFFilterForZFClass.h"
#include "ZFFilterForZFMethod.h"
#include "ZFFilterForZFProperty.h"
#include "ZFPropertyUtil.h"

ZF_NAMESPACE_GLOBAL_BEGIN
ZF_NAMESPACE_BEGIN(ZFClassUtil)

void allClassParentT(ZF_IN_OUT ZFCoreArray<const ZFClass *> &ret,
                     ZF_IN const ZFClass *cls,
                     ZF_IN_OPT const ZFFilterForZFClass *filter /* = zfnull */)
{
    if(filter == zfnull)
    {
        while(cls != zfnull)
        {
            ret.add(cls);
            for(zfindex i = 0; i < cls->implementedInterfaceCount(); ++i)
            {
                ret.add(cls->implementedInterfaceAtIndex(i));
            }
            cls = cls->classParent();
        }
    }
    else
    {
        while(cls != zfnull)
        {
            if(filter->filterCheckActive(cls))
            {
                ret.add(cls);
            }
            for(zfindex i = 0; i < cls->implementedInterfaceCount(); ++i)
            {
                if(filter->filterCheckActive(cls->implementedInterfaceAtIndex(i)))
                {
                    ret.add(cls->implementedInterfaceAtIndex(i));
                }
            }
            cls = cls->classParent();
        }
    }
}

void allMethodT(ZF_IN_OUT ZFCoreArray<const ZFMethod *> &ret,
                ZF_IN const ZFClass *cls,
                ZF_IN_OPT const ZFFilterForZFMethod *filter /* = zfnull */)
{
    ZFCoreArrayPOD<const ZFClass *> allClassParent = ZFClassUtil::allClassParent(cls, (filter == zfnull) ? zfnull : &(filter->classFilter));
    for(zfindex i = 0; i < allClassParent.count(); ++i)
    {
        const ZFClass *cls = allClassParent.get(i);
        for(zfindex j = 0; j < cls->methodCount(); ++j)
        {
            if(filter == zfnull || filter->filterCheckActive(cls->methodAtIndex(j)))
            {
                ret.add(cls->methodAtIndex(j));
            }
        }
    }
}

void allPropertyT(ZF_IN_OUT ZFCoreArray<const ZFProperty *> &ret,
                  ZF_IN const ZFClass *cls,
                  ZF_IN_OPT const ZFFilterForZFProperty *filter /* = zfnull */)
{
    ZFCoreArrayPOD<const ZFClass *> allClassParent = ZFClassUtil::allClassParent(cls, (filter == zfnull) ? zfnull : &(filter->classFilter));
    for(zfindex i = 0; i < allClassParent.count(); ++i)
    {
        const ZFClass *cls = allClassParent.get(i);
        for(zfindex j = 0; j < cls->propertyCount(); ++j)
        {
            if(filter == zfnull || filter->filterCheckActive(cls->propertyAtIndex(j)))
            {
                ret.add(cls->propertyAtIndex(j));
            }
        }
    }
}

zfbool allPropertyIsEqual(ZF_IN ZFObject *obj0,
                          ZF_IN ZFObject *obj1,
                          ZF_IN_OPT const ZFFilterForZFProperty *filter /* = zfnull */)
{
    if(obj0 == obj1)
    {
        return zftrue;
    }
    if(obj0 == zfnull || obj1 == zfnull)
    {
        return zffalse;
    }
    const ZFClass *cls0 = obj0->classData();
    const ZFClass *cls1 = obj1->classData();

    ZFCoreArrayPOD<const ZFProperty *> allProperty = ZFClassUtil::allProperty(cls0, filter);
    for(zfindex i = allProperty.count() - 1; i != zfindexMax(); --i)
    {
        if(cls1->classIsTypeOf(allProperty[i]->propertyOwnerClass())
            && ZFPropertyCompare(allProperty[i], obj0, obj1) != ZFCompareTheSame)
        {
            return zffalse;
        }
    }

    return zftrue;
}

void objectPropertyInfo(ZF_IN_OUT zfstring &ret,
                        ZF_IN ZFObject *obj,
                        ZF_IN_OPT zfindex maxCount /* = zfindexMax() */,
                        ZF_IN_OPT const ZFTokenForKeyValueContainer &token /* = ZFTokenForKeyValueContainerDefault() */)
{
    if(obj == zfnull)
    {
        return ;
    }

    ZFCoreArrayPOD<const ZFProperty *> allProperty = ZFClassUtil::allProperty(obj->classData());
    zfindex count = 0;
    zfindex index = 0;
    for( ; index < allProperty.count() && count < maxCount; ++index)
    {
        if(ZFPropertyIsInitValue(allProperty[index], obj))
        {
            continue;
        }

        if(count == 0)
        {
            ret += token.tokenLeft;
        }
        else
        {
            ret += token.tokenSeparator;
        }
        ++count;

        ret += token.tokenPairLeft;
        {
            ret += token.tokenKeyLeft;
            ret += allProperty[index]->propertyName();
            ret += token.tokenKeyRight;
        }
        ret += token.tokenPairSeparator;
        {
            ret += token.tokenValueLeft;
            ZFPropertyGetInfo(ret, allProperty[index], obj);
            ret += token.tokenValueRight;
        }
        ret += token.tokenPairRight;
    }
    if(index < allProperty.count())
    {
        if(count > 0)
        {
            ret += token.tokenSeparator;
        }
        ret += token.tokenEtc;
    }
    if(count > 0)
    {
        ret += token.tokenRight;
    }
}

void objectInfoT(ZF_IN_OUT zfstring &ret,
                 ZF_IN ZFObject *obj)
{
    if(obj == zfnull)
    {
        ret += ZFTOKEN_zfnull;
        return ;
    }

    ret += ZFTOKEN_ZFObjectInfoLeft;
    ret += obj->classData()->classNameFull();
    ret += "(";
    zfsFromPointerT(ret, obj);
    ret += ")";

    ZFClassUtil::objectPropertyInfo(ret, obj);

    ret += ZFTOKEN_ZFObjectInfoRight;
}

ZF_NAMESPACE_END_WITH_REGISTER(ZFClassUtil, ZF_NAMESPACE_GLOBAL)

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN
ZF_NAMESPACE_BEGIN(ZFClassUtil)

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(void, allClassParentT, ZFMP_IN_OUT(ZFCoreArray<const ZFClass *> &, ret), ZFMP_IN(const ZFClass *, cls), ZFMP_IN_OPT(const ZFFilterForZFClass *, filter, zfnull))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(ZFCoreArrayPOD<const ZFClass *>, allClassParent, ZFMP_IN(const ZFClass *, cls), ZFMP_IN_OPT(const ZFFilterForZFClass *, filter, zfnull))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(void, allMethodT, ZFMP_IN_OUT(ZFCoreArray<const ZFMethod *> &, ret), ZFMP_IN(const ZFClass *, cls), ZFMP_IN_OPT(const ZFFilterForZFMethod *, filter, zfnull))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(ZFCoreArrayPOD<const ZFMethod *>, allMethod, ZFMP_IN(const ZFClass *, cls), ZFMP_IN_OPT(const ZFFilterForZFMethod *, filter, zfnull))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(void, allPropertyT, ZFMP_IN_OUT(ZFCoreArray<const ZFProperty *> &, ret), ZFMP_IN(const ZFClass *, cls), ZFMP_IN_OPT(const ZFFilterForZFProperty *, filter, zfnull))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(ZFCoreArrayPOD<const ZFProperty *>, allProperty, ZFMP_IN(const ZFClass *, cls), ZFMP_IN_OPT(const ZFFilterForZFProperty *, filter, zfnull))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zfbool, allPropertyIsEqual, ZFMP_IN(ZFObject *, obj0), ZFMP_IN(ZFObject *, obj1), ZFMP_IN_OPT(const ZFFilterForZFProperty *, filter, zfnull))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_4(void, objectPropertyInfo, ZFMP_IN_OUT(zfstring &, ret), ZFMP_IN(ZFObject *, obj), ZFMP_IN_OPT(zfindex, maxCount, zfindexMax()), ZFMP_IN_OPT(const ZFTokenForKeyValueContainer &, token, ZFTokenForKeyValueContainerDefault()))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zfstring, objectPropertyInfo, ZFMP_IN(ZFObject *, obj), ZFMP_IN_OPT(zfindex, maxCount, zfindexMax()), ZFMP_IN_OPT(const ZFTokenForKeyValueContainer &, token, ZFTokenForKeyValueContainerDefault()))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(void, objectInfoT, ZFMP_IN_OUT(zfstring &, ret), ZFMP_IN(ZFObject *, obj))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(zfstring, objectInfo, ZFMP_IN(ZFObject *, obj))

ZF_NAMESPACE_END(ZFClassUtil)
ZF_NAMESPACE_GLOBAL_END
#endif

