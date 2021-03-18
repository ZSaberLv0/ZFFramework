#include "ZFClassSerializable.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFTYPEID_ACCESS_ONLY_DEFINE(ZFClassInstanceObserverAddParam, ZFClassInstanceObserverAddParam)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_SETTER_GETTER(v_ZFClassInstanceObserverAddParam, ZFListener, observer)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_SETTER_GETTER(v_ZFClassInstanceObserverAddParam, ZFObject *, userData)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_SETTER_GETTER(v_ZFClassInstanceObserverAddParam, ZFObject *, owner)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_SETTER_GETTER(v_ZFClassInstanceObserverAddParam, ZFLevel, observerLevel)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_SETTER_GETTER(v_ZFClassInstanceObserverAddParam, zfbool , observeAllChildType)

ZFTYPEID_DEFINE_BY_STRING_CONVERTER(ZFClass, const ZFClass *, {
        v = ZFClass::classForName((srcLen == zfindexMax()) ? src : zfstring(src, srcLen).cString());
        return (v != zfnull);
    }, {
        if(v)
        {
            s += v->classNameFull();
        }
        return zftrue;
    })
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_STATIC_1(ZFClass, v_ZFClass, const ZFClass *, classForName, ZFMP_IN(const zfchar *, className))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_STATIC_2(ZFClass, v_ZFClass, const ZFClass *, classForName, ZFMP_IN(const zfchar *, className), ZFMP_IN(const zfchar *, classNamespace))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_5(v_ZFClass, void, instanceObserverAdd,
    ZFMP_IN(const ZFListener &, observer), ZFMP_IN_OPT(ZFObject *, userData, zfnull), ZFMP_IN_OPT(ZFObject *, owner, zfnull), ZFMP_IN_OPT(ZFLevel, observerLevel, ZFLevelAppNormal), ZFMP_IN_OPT(zfbool, observeAllChildType, zftrue))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFClass, void, instanceObserverAdd, ZFMP_IN(const ZFClassInstanceObserverAddParam &, param))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFClass, void, instanceObserverRemove, ZFMP_IN(const ZFListener &, observer))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFClass, void, instanceObserverRemoveByOwner, ZFMP_IN(ZFObject *, owner))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFClass, void, instanceObserverRemoveAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFClass, void, classDataChangeAutoRemoveTagAdd, ZFMP_IN(const zfchar *, tag))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFClass, void, classDataChangeAutoRemoveTagRemove, ZFMP_IN(const zfchar *, tag))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFClass, void, objectInfoOfInheritTreeT, ZFMP_IN_OUT(zfstring &, ret))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFClass, zfstring, objectInfoOfInheritTree)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFClass, zfbool, classIsTypeOf, ZFMP_IN(const ZFClass *, cls))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFClass, const zfchar *, classNamespace)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFClass, const zfchar *, className)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFClass, const zfchar *, classNameFull)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFClass, const ZFClass *, classParent)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFClass, zfbool, classIsAbstract)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFClass, zfbool, classIsInterface)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFClass, zfbool, classIsPrivate)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFClass, zfbool, classIsInternal)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFClass, zfautoObject, newInstance)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_8(v_ZFClass, zfautoObject, newInstanceGeneric
    , ZFMP_IN_OPT(ZFObject *, param0, ZFMethodGenericInvokerDefaultParam())
    , ZFMP_IN_OPT(ZFObject *, param1, ZFMethodGenericInvokerDefaultParam())
    , ZFMP_IN_OPT(ZFObject *, param2, ZFMethodGenericInvokerDefaultParam())
    , ZFMP_IN_OPT(ZFObject *, param3, ZFMethodGenericInvokerDefaultParam())
    , ZFMP_IN_OPT(ZFObject *, param4, ZFMethodGenericInvokerDefaultParam())
    , ZFMP_IN_OPT(ZFObject *, param5, ZFMethodGenericInvokerDefaultParam())
    , ZFMP_IN_OPT(ZFObject *, param6, ZFMethodGenericInvokerDefaultParam())
    , ZFMP_IN_OPT(ZFObject *, param7, ZFMethodGenericInvokerDefaultParam())
    ) /* ZFMETHOD_MAX_PARAM */
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFClass, zfindex, implementedInterfaceCount)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFClass, const ZFClass *, implementedInterfaceAtIndex, ZFMP_IN(zfindex, index))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFClass, zfindex, methodCount)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFClass, const ZFMethod *, methodAtIndex, ZFMP_IN(zfindex, index))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFClass, void, methodGetAllT, ZFMP_IN_OUT(ZFCoreArray<const ZFMethod *> &, ret))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFClass, ZFCoreArrayPOD<const ZFMethod *>, methodGetAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_8(v_ZFClass, const ZFMethod *, methodForNameIgnoreParent, ZFMP_IN(const zfchar *, methodName)
    , ZFMP_IN_OPT(const zfchar *, methodParamTypeId0, zfnull)
    , ZFMP_IN_OPT(const zfchar *, methodParamTypeId1, zfnull)
    , ZFMP_IN_OPT(const zfchar *, methodParamTypeId2, zfnull)
    , ZFMP_IN_OPT(const zfchar *, methodParamTypeId3, zfnull)
    , ZFMP_IN_OPT(const zfchar *, methodParamTypeId4, zfnull)
    , ZFMP_IN_OPT(const zfchar *, methodParamTypeId5, zfnull)
    , ZFMP_IN_OPT(const zfchar *, methodParamTypeId6, zfnull)
    // , ZFMP_IN_OPT(const zfchar *, methodParamTypeId7, zfnull)
    ) /* ZFMETHOD_MAX_PARAM */
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_8(v_ZFClass, const ZFMethod *, methodForName, ZFMP_IN(const zfchar *, methodName)
    , ZFMP_IN_OPT(const zfchar *, methodParamTypeId0, zfnull)
    , ZFMP_IN_OPT(const zfchar *, methodParamTypeId1, zfnull)
    , ZFMP_IN_OPT(const zfchar *, methodParamTypeId2, zfnull)
    , ZFMP_IN_OPT(const zfchar *, methodParamTypeId3, zfnull)
    , ZFMP_IN_OPT(const zfchar *, methodParamTypeId4, zfnull)
    , ZFMP_IN_OPT(const zfchar *, methodParamTypeId5, zfnull)
    , ZFMP_IN_OPT(const zfchar *, methodParamTypeId6, zfnull)
    // , ZFMP_IN_OPT(const zfchar *, methodParamTypeId7, zfnull)
    ) /* ZFMETHOD_MAX_PARAM */
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFClass, void, methodForNameGetAllT, ZFMP_IN_OUT(ZFCoreArray<const ZFMethod *> &, ret), ZFMP_IN(const zfchar *, methodName))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFClass, ZFCoreArrayPOD<const ZFMethod *>, methodForNameGetAll, ZFMP_IN(const zfchar *, methodName))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFClass, zfindex, propertyCount)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFClass, const ZFProperty *, propertyAtIndex, ZFMP_IN(zfindex, index))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFClass, void, propertyGetAllT, ZFMP_IN_OUT(ZFCoreArray<const ZFProperty *> &, ret))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFClass, ZFCoreArrayPOD<const ZFProperty *>, propertyGetAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFClass, const ZFProperty *, propertyForNameIgnoreParent, ZFMP_IN(const zfchar *, propertyName))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFClass, const ZFProperty *, propertyForName, ZFMP_IN(const zfchar *, propertyName))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFClass, zfbool, propertyHasOverrideInitStep)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFClass, zfbool, propertyHasOverrideInitStep, ZFMP_IN(const ZFProperty *, property))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFClass, void, classTag, ZFMP_IN(const zfchar *, key), ZFMP_IN(ZFObject *, tag))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFClass, ZFObject *, classTag, ZFMP_IN(const zfchar *, key))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFClass, void, classTagGetAllKeyValue, ZFMP_IN_OUT(ZFCoreArray<const zfchar *> &, allKey), ZFMP_IN_OUT(ZFCoreArray<ZFObject *> &, allValue))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFClass, void, classTagRemove, ZFMP_IN(const zfchar *, key))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFClass, zfautoObject, classTagRemoveAndGet, ZFMP_IN(const zfchar *, key))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFClass, void, classTagRemoveAll)

ZF_NAMESPACE_GLOBAL_END

