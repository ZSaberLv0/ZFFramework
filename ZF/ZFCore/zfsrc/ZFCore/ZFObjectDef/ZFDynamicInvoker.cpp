#include "ZFDynamicInvoker.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFOBJECT_REGISTER(ZFDI_WrapperBase)
ZFOBJECT_REGISTER(ZFDI_Wrapper)
ZFOBJECT_REGISTER(ZFDI_WrapperRaw)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFDI_WrapperBase, void, zfv, ZFMP_IN(const zfchar *, zfv))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFDI_WrapperBase, const zfchar *, zfv)

// ============================================================
const zfchar *ZFDI_toString(ZF_IN ZFObject *obj)
{
    if(obj == zfnull)
    {
        return zfnull;
    }
    {
        v_zfstring *t = ZFCastZFObject(v_zfstring *, obj);
        if(t != zfnull)
        {
            return t->zfv;
        }
    }
    {
        ZFDI_WrapperBase *t = ZFCastZFObject(ZFDI_WrapperBase *, obj);
        if(t != zfnull)
        {
            return t->zfv();
        }
    }
    return zfnull;
}

const ZFClass *ZFDI_classForName(ZF_IN const zfchar *className,
                                 ZF_IN const zfchar *NS)
{
    if(className == zfnull)
    {
        return zfnull;
    }
    const ZFClass *cls = ZFClass::classForName(className, NS);
    if(cls != zfnull)
    {
        return cls;
    }
    zfindex dotPos = zfstringFindReversely(className, zfindexMax(), ZFNamespaceSeparator(), ZFNamespaceSeparatorLen());
    if(dotPos == zfindexMax())
    {
        zfstring classNameTmp;
        classNameTmp += ZFTypeIdWrapperPrefixName;
        classNameTmp += className;
        cls = ZFClass::classForName(classNameTmp, NS);
    }
    else
    {
        zfstring classNameTmp;
        classNameTmp.append(className, dotPos + ZFNamespaceSeparatorLen());
        classNameTmp += ZFTypeIdWrapperPrefixName;
        classNameTmp += className + dotPos + ZFNamespaceSeparatorLen();
        cls = ZFClass::classForName(classNameTmp, NS);
    }
    return cls;
}

static void _ZFP_ZFDI_paramInfo(ZF_IN_OUT zfstring &ret,
                                ZF_IN ZFObject *param)
{
    if(param == zfnull)
    {
        ret += ZFTOKEN_zfnull;
    }
    else
    {
        ret += "(";
        ret += param->classData()->className();
        ret += ")";
        param->objectInfoT(ret);
    }
}
void ZFDI_paramInfo(ZF_IN_OUT zfstring &ret
                    , ZF_IN_OPT ZFObject *param0 /* = ZFMethodGenericInvokerDefaultParam() */
                    , ZF_IN_OPT ZFObject *param1 /* = ZFMethodGenericInvokerDefaultParam() */
                    , ZF_IN_OPT ZFObject *param2 /* = ZFMethodGenericInvokerDefaultParam() */
                    , ZF_IN_OPT ZFObject *param3 /* = ZFMethodGenericInvokerDefaultParam() */
                    , ZF_IN_OPT ZFObject *param4 /* = ZFMethodGenericInvokerDefaultParam() */
                    , ZF_IN_OPT ZFObject *param5 /* = ZFMethodGenericInvokerDefaultParam() */
                    , ZF_IN_OPT ZFObject *param6 /* = ZFMethodGenericInvokerDefaultParam() */
                    , ZF_IN_OPT ZFObject *param7 /* = ZFMethodGenericInvokerDefaultParam() */
                    )
{
    if(param0 == zfnull || param0 == ZFMethodGenericInvokerDefaultParam()) {return ;}
    ret += '[';
    do
    {
        _ZFP_ZFDI_paramInfo(ret, param0);

        #define _ZFP_ZFDI_paramInfo_loop(N) \
            if(param##N == zfnull || param##N == ZFMethodGenericInvokerDefaultParam()) \
            { \
                break; \
            } \
            else \
            { \
                ret += ", "; \
                _ZFP_ZFDI_paramInfo(ret, param##N); \
            }
        _ZFP_ZFDI_paramInfo_loop(1)
        _ZFP_ZFDI_paramInfo_loop(2)
        _ZFP_ZFDI_paramInfo_loop(3)
        _ZFP_ZFDI_paramInfo_loop(4)
        _ZFP_ZFDI_paramInfo_loop(5)
        _ZFP_ZFDI_paramInfo_loop(6)
        _ZFP_ZFDI_paramInfo_loop(7)
    } while(zffalse);
    ret += ']';
}

// ============================================================
static void _ZFP_ZFDI_paramCount(ZF_IN_OUT zfindex &paramCount,
                                 ZF_IN_OUT zfautoObject (&paramList)[ZFMETHOD_MAX_PARAM])
{
    paramCount = 0;
    while(paramCount < ZFMETHOD_MAX_PARAM && paramList[paramCount] != ZFMethodGenericInvokerDefaultParam())
    {
        ++paramCount;
    }
}
zfbool ZFDI_invoke(ZF_OUT zfautoObject &ret
                   , ZF_OUT_OPT zfstring *errorHint
                   , ZF_IN_OPT ZFObject *obj
                   , ZF_IN_OPT const zfchar *NS
                   , ZF_IN ZFObject *type
                   , ZF_IN zfindex paramCount
                   , ZF_IN_OUT zfautoObject (&paramList)[ZFMETHOD_MAX_PARAM]
                   )
{
    if(type == zfnull)
    {
        if(errorHint != zfnull)
        {
            zfstringAppend(errorHint, "null methodName or ClassName");
        }
        return zffalse;
    }

    if(obj == zfnull)
    {
        v_ZFClass *clsWrapper = ZFCastZFObject(v_ZFClass *, type);
        if(clsWrapper != zfnull)
        {
            if(clsWrapper->zfv == zfnull)
            {
                if(errorHint != zfnull)
                {
                    zfstringAppend(errorHint, "null class");
                }
                return zffalse;
            }
            else
            {
                return ZFDI_alloc(ret, errorHint, clsWrapper->zfv, paramCount, paramList);
            }
        }
    }

    const zfchar *methodName = ZFDI_toString(type);
    zfstring NSHolder;
    if(methodName != zfnull && ZFNamespaceSkipGlobal(NS) == zfnull)
    {
        zfindex dotPos = zfstringFindReversely(methodName, zfindexMax(), ZFNamespaceSeparator(), ZFNamespaceSeparatorLen());
        if(dotPos != zfindexMax())
        {
            NSHolder.append(methodName, dotPos);
            NS = NSHolder;
            methodName = methodName + dotPos + ZFNamespaceSeparatorLen();
        }
    }
    if(methodName != zfnull && obj == zfnull)
    {
        const ZFClass *cls = ZFDI_classForName(methodName, NS);
        if(cls != zfnull)
        {
            return ZFDI_alloc(ret, errorHint, cls, paramCount, paramList);
        }
    }

    ZFCoreArrayPOD<const ZFMethod *> methodList;
    if(methodName == zfnull)
    {
        v_ZFMethod *methodWrapper = ZFCastZFObject(v_ZFMethod *, type);
        if(methodWrapper != zfnull)
        {
            methodList.add(methodWrapper->zfv);
        }
        else
        {
            if(errorHint != zfnull)
            {
                zfstringAppend(errorHint, "unable to detect methodName from: \"%s\"",
                    ZFObjectInfo(type).cString());
            }
            return zffalse;
        }
    }
    else
    {
        if(obj != zfnull)
        {
            obj->classData()->methodForNameGetAllT(methodList, methodName);
        }
        else
        {
            ZFMethodForNameGetAllT(methodList, NS, methodName);
            if(methodList.isEmpty())
            {
                const ZFClass *cls = ZFDI_classForName(NS, zfnull);
                if(cls != zfnull)
                {
                    cls->methodForNameGetAllT(methodList, methodName);
                }
            }
        }
        if(methodList.isEmpty())
        {
            if(errorHint != zfnull)
            {
                if(obj != zfnull)
                {
                    zfstringAppend(errorHint, "no such method \"%s\" for object: %s",
                        methodName, obj->objectInfoOfInstance().cString());
                }
                else
                {
                    zfstringAppend(errorHint, "no such method \"%s\" in scope \"%s\"",
                        methodName, NS);
                }
            }
            return zffalse;
        }
    }

    if(paramCount == zfindexMax())
    {
        _ZFP_ZFDI_paramCount(paramCount, paramList);
    }

    // try to invoke each method
    zfstring _errorHintTmp;
    zfstring *errorHintTmp = errorHint ? &_errorHintTmp : zfnull;
    if(methodList.count() == 1)
    {
        do {
            const ZFMethod *method = methodList[0];
            if(paramCount < method->methodParamCountMin() || paramCount > method->methodParamCount())
            {
                if(errorHintTmp != zfnull)
                {
                    zfstringAppend(_errorHintTmp, "expect %s param, got %zi",
                        ((method->methodParamCountMin() == method->methodParamCount())
                            ? zfindexToString(method->methodParamCount()).cString()
                            : zfstringWithFormat("%zi~%zi", method->methodParamCountMin(), method->methodParamCount()).cString()),
                        paramCount);
                }
                break;
            }
            zfbool paramConvertSuccess = zftrue;
            zfautoObject paramConvertCache;
            for(zfindex iParam = 0; iParam < paramCount; ++iParam)
            {
                ZFDI_WrapperBase *wrapper = ZFCastZFObject(ZFDI_WrapperBase *, paramList[iParam]);
                if(wrapper != zfnull)
                {
                    paramConvertCache.zflockfree_assign(paramList[iParam]);
                    if(!ZFDI_paramConvert(
                        paramList[iParam], method->methodParamTypeIdAtIndex(iParam), wrapper, errorHintTmp))
                    {
                        paramList[iParam].zflockfree_assign(paramConvertCache);
                        paramConvertSuccess = zffalse;
                        break;
                    }
                }
            }
            if(!paramConvertSuccess)
            {
                break;
            }
            if(method->methodGenericInvoker()(method, obj, errorHintTmp, ret, paramList))
            {
                if(zfscmpTheSame(method->methodReturnTypeId(), ZFTypeId_void()))
                {
                    ret = obj;
                }
                return zftrue;
            }
        } while(zffalse);
    }
    else
    {
        zfautoObject paramListTmp[ZFMETHOD_MAX_PARAM];
        for(zfindex iMethod = 0; iMethod < methodList.count(); ++iMethod)
        {
            const ZFMethod *method = methodList[iMethod];
            if(!_errorHintTmp.isEmpty())
            {
                _errorHintTmp += "\n    ";
            }
            if(paramCount < method->methodParamCountMin() || paramCount > method->methodParamCount())
            {
                if(errorHintTmp != zfnull)
                {
                    zfstringAppend(_errorHintTmp, "expect %s param, got %zi",
                        ((method->methodParamCountMin() == method->methodParamCount())
                            ? zfindexToString(method->methodParamCount()).cString()
                            : zfstringWithFormat("%zi~%zi", method->methodParamCountMin(), method->methodParamCount()).cString()),
                        paramCount);
                }
                continue;
            }

            for(zfindex i = 0; i < ZFMETHOD_MAX_PARAM; ++i)
            {
                paramListTmp[i].zflockfree_assign(paramList[i]);
            }

            zfbool paramConvertSuccess = zftrue;
            for(zfindex iParam = 0; iParam < paramCount; ++iParam)
            {
                ZFDI_WrapperBase *wrapper = ZFCastZFObject(ZFDI_WrapperBase *, paramListTmp[iParam]);
                if(wrapper != zfnull)
                {
                    if(!ZFDI_paramConvert(
                        paramListTmp[iParam], method->methodParamTypeIdAtIndex(iParam), wrapper, errorHintTmp))
                    {
                        paramConvertSuccess = zffalse;
                        break;
                    }
                }
            }
            if(!paramConvertSuccess)
            {
                continue;
            }
            if(method->methodGenericInvoker()(method, obj, errorHintTmp, ret, paramListTmp))
            {
                if(zfscmpTheSame(method->methodReturnTypeId(), ZFTypeId_void()))
                {
                    ret = obj;
                }
                return zftrue;
            }
        }
    }
    if(errorHint != zfnull)
    {
        *errorHint += "no matching method to call";
        *errorHint += ", error hint:\n    ";
        *errorHint += _errorHintTmp;
        *errorHint += "\n  candidate methods:";
        for(zfindex i = 0; i < methodList.count(); ++i)
        {
            *errorHint += "\n    ";
            methodList[i]->objectInfoT(*errorHint);
        }

        if(paramCount > 0)
        {
            *errorHint += "\n  with params: ";
            ZFDI_paramInfo(*errorHint
                    , paramList[0]
                    , paramList[1]
                    , paramList[2]
                    , paramList[3]
                    , paramList[4]
                    , paramList[5]
                    , paramList[6]
                    , paramList[7]
                );
        }
    }
    return zffalse;
}

zfbool ZFDI_alloc(ZF_OUT zfautoObject &ret
                  , ZF_OUT_OPT zfstring *errorHint
                  , ZF_IN_OPT const zfchar *NS
                  , ZF_IN ZFObject *type
                  , ZF_IN zfindex paramCount
                  , ZF_IN_OUT zfautoObject (&paramList)[ZFMETHOD_MAX_PARAM]
                  )
{
    if(type == zfnull)
    {
        if(errorHint != zfnull)
        {
            zfstringAppend(errorHint, "null class name");
        }
        return zffalse;
    }
    const ZFClass *cls = zfnull;
    v_ZFClass *clsWrapper = ZFCastZFObject(v_ZFClass *, type);
    if(clsWrapper != zfnull)
    {
        cls = clsWrapper->zfv;
        if(cls == zfnull)
        {
            if(errorHint != zfnull)
            {
                zfstringAppend(errorHint, "null class");
            }
            return zffalse;
        }
    }
    else
    {
        cls = ZFDI_classForName(ZFDI_toString(type), NS);
    }
    if(cls == zfnull)
    {
        if(errorHint != zfnull)
        {
            zfstringAppend(errorHint, "no such class \"%s\" in scope \"%s\"",
                ZFObjectInfo(type).cString(), NS);
        }
        return zffalse;
    }
    else
    {
        return ZFDI_alloc(ret, errorHint, cls, paramCount, paramList);
    }
}
zfbool ZFDI_alloc(ZF_OUT zfautoObject &ret
                  , ZF_OUT_OPT zfstring *errorHint
                  , ZF_IN const ZFClass *cls
                  , ZF_IN zfindex paramCount
                  , ZF_IN_OUT zfautoObject (&paramList)[ZFMETHOD_MAX_PARAM]
                  )
{
    if(cls == zfnull)
    {
        if(errorHint != zfnull)
        {
            zfstringAppend(errorHint, "null class");
        }
        return zffalse;
    }

    if(paramCount == zfindexMax())
    {
        _ZFP_ZFDI_paramCount(paramCount, paramList);
    }
    if(paramCount == 0)
    {
        ret = cls->newInstance();
        if(ret == zfnull)
        {
            if(errorHint != zfnull)
            {
                zfstringAppend(errorHint, "unable to alloc class \"%s\"", cls->classNameFull());
            }
            return zffalse;
        }
        else
        {
            return zftrue;
        }
    }
    ZFCoreArrayPOD<const ZFMethod *> methodList;
    cls->methodForNameGetAllT(methodList, "objectOnInit");
    if(methodList.isEmpty())
    {
        if(errorHint != zfnull)
        {
            zfstringAppend(errorHint, "class \"%s\" has no reflectable objectOnInit", cls->classNameFull());
        }
        return zffalse;
    }

    void *token = cls->newInstanceGenericBegin();
    if(token == zfnull)
    {
        if(errorHint != zfnull)
        {
            zfstringAppend(errorHint, "unable to alloc class \"%s\"", cls->classNameFull());
        }
        return zffalse;
    }

    zfstring _errorHintTmp;
    zfstring *errorHintTmp = errorHint ? &_errorHintTmp : zfnull;
    if(methodList.count() == 1)
    {
        do {
            const ZFMethod *method = methodList[0];
            if(paramCount < method->methodParamCountMin() || paramCount > method->methodParamCount())
            {
                if(errorHintTmp != zfnull)
                {
                    zfstringAppend(_errorHintTmp, "expect %s param, got %zi",
                        ((method->methodParamCountMin() == method->methodParamCount())
                            ? zfindexToString(method->methodParamCount()).cString()
                            : zfstringWithFormat("%zi~%zi", method->methodParamCountMin(), method->methodParamCount()).cString()),
                        paramCount);
                }
                continue;
            }
            zfbool paramConvertSuccess = zftrue;
            zfautoObject paramConvertCache;
            for(zfindex iParam = 0; iParam < paramCount; ++iParam)
            {
                ZFDI_WrapperBase *wrapper = ZFCastZFObject(ZFDI_WrapperBase *, paramList[iParam]);
                if(wrapper != zfnull)
                {
                    paramConvertCache.zflockfree_assign(paramList[iParam]);
                    if(!ZFDI_paramConvert(
                        paramList[iParam], method->methodParamTypeIdAtIndex(iParam), wrapper, errorHintTmp))
                    {
                        paramList[iParam].zflockfree_assign(paramConvertCache);
                        paramConvertSuccess = zffalse;
                        break;
                    }
                }
            }
            if(!paramConvertSuccess)
            {
                break;
            }
            if(cls->newInstanceGenericCheck(token, method, paramList, errorHintTmp))
            {
                ret = cls->newInstanceGenericEnd(token, zftrue);
                return zftrue;
            }
        } while(zffalse);
    }
    else
    {
        zfautoObject paramListTmp[ZFMETHOD_MAX_PARAM];
        for(zfindex iMethod = 0; iMethod < methodList.count(); ++iMethod)
        {
            const ZFMethod *method = methodList[iMethod];
            if(!_errorHintTmp.isEmpty())
            {
                _errorHintTmp += "\n    ";
            }
            if(paramCount < method->methodParamCountMin() || paramCount > method->methodParamCount())
            {
                if(errorHintTmp != zfnull)
                {
                    zfstringAppend(_errorHintTmp, "expect %s param, got %zi",
                        ((method->methodParamCountMin() == method->methodParamCount())
                            ? zfindexToString(method->methodParamCount()).cString()
                            : zfstringWithFormat("%zi~%zi", method->methodParamCountMin(), method->methodParamCount()).cString()),
                        paramCount);
                }
                continue;
            }

            for(zfindex i = 0; i < ZFMETHOD_MAX_PARAM; ++i)
            {
                paramListTmp[i].zflockfree_assign(paramList[i]);
            }

            zfbool paramConvertSuccess = zftrue;
            for(zfindex iParam = 0; iParam < paramCount; ++iParam)
            {
                ZFDI_WrapperBase *wrapper = ZFCastZFObject(ZFDI_WrapperBase *, paramListTmp[iParam]);
                if(wrapper != zfnull)
                {
                    if(!ZFDI_paramConvert(
                        paramListTmp[iParam], method->methodParamTypeIdAtIndex(iParam), wrapper, errorHintTmp))
                    {
                        paramConvertSuccess = zffalse;
                        break;
                    }
                }
            }
            if(!paramConvertSuccess)
            {
                continue;
            }
            if(cls->newInstanceGenericCheck(token, method, paramListTmp, errorHintTmp))
            {
                ret = cls->newInstanceGenericEnd(token, zftrue);
                return zftrue;
            }
        }
    }
    cls->newInstanceGenericEnd(token, zffalse);
    if(errorHint != zfnull)
    {
        *errorHint += "no matching objectOnInit to call for class \"";
        *errorHint += cls->className();
        *errorHint += "\"";
        *errorHint += ", error hint:\n    ";
        *errorHint += _errorHintTmp;
        *errorHint += "\n  candidate methods:";
        for(zfindex i = 0; i < methodList.count(); ++i)
        {
            *errorHint += "\n    ";
            methodList[i]->objectInfoT(*errorHint);
        }

        if(paramCount > 0)
        {
            *errorHint += "\n  with params: ";
            ZFDI_paramInfo(*errorHint
                    , paramList[0]
                    , paramList[1]
                    , paramList[2]
                    , paramList[3]
                    , paramList[4]
                    , paramList[5]
                    , paramList[6]
                    , paramList[7]
                );
        }
    }
    return zffalse;
}

zfbool ZFDI_paramConvert(ZF_OUT zfautoObject &ret,
                         ZF_IN const zfchar *typeId,
                         ZF_IN ZFDI_WrapperBase *wrapper,
                         ZF_OUT_OPT zfstring *errorHint /* = zfnull */)
{
    const ZFClass *cls = ZFClass::classForName(typeId);
    if(cls != zfnull)
    {
        if(!ZFSerializeFromString(ret, cls, wrapper->zfv()))
        {
            if(errorHint != zfnull)
            {
                zfstringAppend(errorHint, "%s unable to convert from string \"%s\"",
                    typeId, wrapper->zfv());
            }
            return zffalse;
        }
        else
        {
            return zftrue;
        }
    }

    const ZFTypeInfo *typeIdData = ZFTypeInfoForName(typeId);
    if(typeIdData == zfnull || typeIdData->typeIdClass() == zfnull || typeIdData->typeIdClass()->classIsAbstract())
    {
        if(errorHint != zfnull)
        {
            zfstringAppend(errorHint, "%s can not be converted from string automatically", typeId);
        }
        return zffalse;
    }
    ret = typeIdData->typeIdClass()->newInstance();
    if(ret != zfnull)
    {
        ZFTypeIdWrapper *typeWrapper = ret;
        if(typeWrapper != zfnull && typeWrapper->wrappedValueFromString(wrapper->zfv()))
        {
            return zftrue;
        }
    }
    if(errorHint != zfnull)
    {
        zfstringAppend(errorHint, "%s can not be converted from string \"%s\"",
                typeId,
                wrapper->zfv()
            );
    }
    return zffalse;
}

// ============================================================
zfautoObject ZFInvoke(ZF_IN const zfchar *name
                      , ZF_IN_OPT ZFObject *param0 /* = ZFMethodGenericInvokerDefaultParam() */
                      , ZF_IN_OPT ZFObject *param1 /* = ZFMethodGenericInvokerDefaultParam() */
                      , ZF_IN_OPT ZFObject *param2 /* = ZFMethodGenericInvokerDefaultParam() */
                      , ZF_IN_OPT ZFObject *param3 /* = ZFMethodGenericInvokerDefaultParam() */
                      , ZF_IN_OPT ZFObject *param4 /* = ZFMethodGenericInvokerDefaultParam() */
                      , ZF_IN_OPT ZFObject *param5 /* = ZFMethodGenericInvokerDefaultParam() */
                      , ZF_IN_OPT ZFObject *param6 /* = ZFMethodGenericInvokerDefaultParam() */
                      , ZF_IN_OPT ZFObject *param7 /* = ZFMethodGenericInvokerDefaultParam() */
                      , ZF_OUT_OPT zfbool *success /* = zfnull */
                      , ZF_OUT_OPT zfstring *errorHint /* = zfnull */
                      )
{
    zfCoreMutexLock();
    zfautoObject paramList[ZFMETHOD_MAX_PARAM];
    zfindex paramCount = 0;
    do {
        if(param0 == zfnull) {paramCount = 0; break;} else {paramList[0].zflockfree_assign(param0);}
        if(param1 == zfnull) {paramCount = 1; break;} else {paramList[1].zflockfree_assign(param1);}
        if(param2 == zfnull) {paramCount = 2; break;} else {paramList[2].zflockfree_assign(param2);}
        if(param3 == zfnull) {paramCount = 3; break;} else {paramList[3].zflockfree_assign(param3);}
        if(param4 == zfnull) {paramCount = 4; break;} else {paramList[4].zflockfree_assign(param4);}
        if(param5 == zfnull) {paramCount = 5; break;} else {paramList[5].zflockfree_assign(param5);}
        if(param6 == zfnull) {paramCount = 6; break;} else {paramList[6].zflockfree_assign(param6);}
        if(param7 == zfnull) {paramCount = 7; break;} else {paramList[7].zflockfree_assign(param7);}
    } while(zffalse);
    zflockfree_zfblockedAllocWithCache(v_zfstring, nameHolder);
    nameHolder->zfv = name;
    zfCoreMutexUnlock();
    zfautoObject ret;
    if(ZFDI_invoke(ret, errorHint, zfnull, zfnull, nameHolder, paramCount, paramList))
    {
        if(success != zfnull) {*success = zftrue;}
        return ret;
    }
    else
    {
        if(success != zfnull) {*success = zffalse;}
        return zfnull;
    }
}
zfautoObject ZFInvoke(ZF_IN const zfchar *name
                      , ZF_IN const zfchar *param0
                      , ZF_IN_OPT const zfchar *param1 /* = zfnull */
                      , ZF_IN_OPT const zfchar *param2 /* = zfnull */
                      , ZF_IN_OPT const zfchar *param3 /* = zfnull */
                      , ZF_IN_OPT const zfchar *param4 /* = zfnull */
                      , ZF_IN_OPT const zfchar *param5 /* = zfnull */
                      , ZF_IN_OPT const zfchar *param6 /* = zfnull */
                      , ZF_IN_OPT const zfchar *param7 /* = zfnull */
                      , ZF_OUT_OPT zfbool *success /* = zfnull */
                      , ZF_OUT_OPT zfstring *errorHint /* = zfnull */
                      )
{
    zfCoreMutexLock();
    zfautoObject paramList[ZFMETHOD_MAX_PARAM];
    zfindex paramCount = 0;
    do {
        if(param0 == zfnull) {paramCount = 0; break;} else {paramList[0] = zflockfree_zflineAlloc(ZFDI_Wrapper, param0);}
        if(param1 == zfnull) {paramCount = 1; break;} else {paramList[1] = zflockfree_zflineAlloc(ZFDI_Wrapper, param1);}
        if(param2 == zfnull) {paramCount = 2; break;} else {paramList[2] = zflockfree_zflineAlloc(ZFDI_Wrapper, param2);}
        if(param3 == zfnull) {paramCount = 3; break;} else {paramList[3] = zflockfree_zflineAlloc(ZFDI_Wrapper, param3);}
        if(param4 == zfnull) {paramCount = 4; break;} else {paramList[4] = zflockfree_zflineAlloc(ZFDI_Wrapper, param4);}
        if(param5 == zfnull) {paramCount = 5; break;} else {paramList[5] = zflockfree_zflineAlloc(ZFDI_Wrapper, param5);}
        if(param6 == zfnull) {paramCount = 6; break;} else {paramList[6] = zflockfree_zflineAlloc(ZFDI_Wrapper, param6);}
        if(param7 == zfnull) {paramCount = 7; break;} else {paramList[7] = zflockfree_zflineAlloc(ZFDI_Wrapper, param7);}
    } while(zffalse);
    zflockfree_zfblockedAllocWithCache(v_zfstring, nameHolder);
    nameHolder->zfv = name;
    zfCoreMutexUnlock();
    zfautoObject ret;
    if(ZFDI_invoke(ret, errorHint, zfnull, zfnull, nameHolder, paramCount, paramList))
    {
        if(success != zfnull) {*success = zftrue;}
        return ret;
    }
    else
    {
        if(success != zfnull) {*success = zffalse;}
        return zfnull;
    }
}

ZF_NAMESPACE_GLOBAL_END

