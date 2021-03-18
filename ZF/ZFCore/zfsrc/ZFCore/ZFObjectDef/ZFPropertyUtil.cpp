#include "ZFPropertyUtil.h"
#include "ZFObjectImpl.h"
#include "ZFClassUtil.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFPropertyCopyAll
void ZFPropertyCopyAll(ZF_IN ZFObject *dstObj,
                       ZF_IN ZFObject *srcObj,
                       ZF_IN_OPT const ZFFilterForZFProperty *filter /* = zfnull */,
                       ZF_OUT_OPT ZFCoreArrayPOD<const ZFProperty *> *copiedProperties /* = zfnull */)
{
    if(dstObj == zfnull || srcObj == zfnull)
    {
        return ;
    }

    ZFCoreArrayPOD<const ZFProperty *> allProperty = ZFClassUtil::allProperty(dstObj->classData(), filter);
    const ZFClass *srcClass = srcObj->classData();
    for(zfindex i = 0; i < allProperty.count(); ++i)
    {
        const ZFProperty *property = allProperty.get(i);
        if(!srcClass->classIsTypeOf(property->propertyOwnerClass()))
        {
            continue;
        }
        ZFPropertyCopy(property, dstObj, srcObj);
        if(copiedProperties != zfnull)
        {
            copiedProperties->add(property);
        }
    }
}

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfbool, ZFPropertyIsValueAccessed, ZFMP_IN(const ZFProperty *, propertyInfo), ZFMP_IN(ZFObject *, ownerObj))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfbool, ZFPropertyIsInitValue, ZFMP_IN(const ZFProperty *, propertyInfo), ZFMP_IN(ZFObject *, ownerObj))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(ZFCompareResult, ZFPropertyCompare, ZFMP_IN(const ZFProperty *, propertyInfo), ZFMP_IN(ZFObject *, obj0), ZFMP_IN(ZFObject *, obj1))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(void, ZFPropertyCopy, ZFMP_IN(const ZFProperty *, propertyInfo), ZFMP_IN(ZFObject *, dstObj), ZFMP_IN(ZFObject *, srcObj))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_4(void, ZFPropertyCopyAll, ZFMP_IN(ZFObject *, dstObj), ZFMP_IN(ZFObject *, srcObj), ZFMP_IN_OPT(const ZFFilterForZFProperty *, filter, zfnull), ZFMP_OUT_OPT(ZFCoreArrayPOD<const ZFProperty *> *, copiedProperties, zfnull))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(void, ZFPropertyGetInfo, ZFMP_IN_OUT(zfstring &, ret), ZFMP_IN(const ZFProperty *, propertyInfo), ZFMP_IN(ZFObject *, ownerObject))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfstring, ZFPropertyGetInfo, ZFMP_IN(const ZFProperty *, propertyInfo), ZFMP_IN(ZFObject *, ownerObject))

ZF_NAMESPACE_GLOBAL_END
#endif

