#include "ZFSerializableUtil.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN
ZF_NAMESPACE_BEGIN(ZFSerializableUtil)

#define _ZFP_ZFSerializableUtil_DEBUG 0

#if _ZFP_ZFSerializableUtil_DEBUG
static void _ZFP_ZFSerializableUtilDebugAction(void)
{
    zfCoreCriticalMessageTrim("[ZFSerializableUtil] errorOccurred");
}
#endif

void errorOccurred(ZF_OUT_OPT zfstring *outErrorHint,
                   ZF_OUT_OPT ZFSerializableData *outErrorPos,
                   ZF_IN const ZFSerializableData &errorPos,
                   ZF_IN const zfchar *fmt,
                   ...)
{
    if(outErrorPos != zfnull)
    {
        *outErrorPos = errorPos;
    }

    if(outErrorHint != zfnull)
    {
        va_list vaList;
        va_start(vaList, fmt);
        zfstringAppendV(*outErrorHint, fmt, vaList);
        va_end(vaList);
        zfstringAppend(*outErrorHint, ", at:\n    %s", errorPos.objectInfo().cString());
    }

#if _ZFP_ZFSerializableUtil_DEBUG
    _ZFP_ZFSerializableUtilDebugAction();
#endif
}

void errorOccurred(ZF_OUT_OPT zfstring *outErrorHint,
                   ZF_IN const zfchar *fmt,
                   ...)
{
    if(outErrorHint != zfnull)
    {
        va_list vaList;
        va_start(vaList, fmt);
        zfstringAppendV(*outErrorHint, fmt, vaList);
        va_end(vaList);
    }

#if _ZFP_ZFSerializableUtil_DEBUG
    _ZFP_ZFSerializableUtilDebugAction();
#endif
}

void errorOccurredWhile(ZF_OUT_OPT zfstring *outErrorHint,
                        ZF_OUT_OPT ZFSerializableData *outErrorPos,
                        ZF_IN const ZFSerializableData &errorPos,
                        ZF_IN const zfchar *serializingName,
                        ZF_IN const zfchar *errorValue)
{
    if(outErrorPos != zfnull)
    {
        *outErrorPos = errorPos;
    }

    if(outErrorHint != zfnull)
    {
        zfstringAppend(*outErrorHint, "failed to serialize \"%s\" with value \"%s\"",
            serializingName, errorValue);
        zfstringAppend(*outErrorHint, ", at:\n    %s", errorPos.objectInfo().cString());
    }

#if _ZFP_ZFSerializableUtil_DEBUG
    _ZFP_ZFSerializableUtilDebugAction();
#endif
}
void errorOccurredWhile(ZF_OUT_OPT zfstring *outErrorHint,
                        ZF_IN const zfchar *serializingName,
                        ZF_IN const zfchar *errorValue)
{
    if(outErrorHint != zfnull)
    {
        zfstringAppend(*outErrorHint, "failed to serialize \"%s\" with value \"%s\"",
            serializingName, errorValue);
    }

#if _ZFP_ZFSerializableUtil_DEBUG
    _ZFP_ZFSerializableUtilDebugAction();
#endif
}

const zfchar *checkItemClass(ZF_IN const ZFSerializableData &serializableData,
                             ZF_IN const zfchar *desiredClass)
{
    const zfchar *serializableClass = serializableData.itemClass();
    if(zfscmpTheSame(desiredClass, ZFTypeId_none()))
    {
        return serializableClass;
    }
    else
    {
        if(zfscmpTheSame(serializableClass, desiredClass))
        {
            return serializableClass;
        }
        else
        {
            return zfnull;
        }
    }
}
const zfchar *requireItemClass(ZF_IN const ZFSerializableData &serializableData,
                               ZF_IN const zfchar *desiredClass,
                               ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */,
                               ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */)
{
    const zfchar *ret = checkItemClass(serializableData, desiredClass);
    if(ret == zfnull)
    {
        if(desiredClass == zfnull || *desiredClass == '\0')
        {
            ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
                "missing serializable class");
        }
        else
        {
            ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
                "serializable class must be \"%s\"", desiredClass);
        }
    }
    return ret;
}

const zfchar *checkAttribute(ZF_IN const ZFSerializableData &serializableData,
                             ZF_IN const zfchar *desiredAttribute)
{
    zfiterator it = serializableData.attributeIteratorForName(desiredAttribute);
    if(!serializableData.attributeIteratorIsValid(it))
    {
        return zfnull;
    }
    serializableData.attributeIteratorResolveMark(it);
    return serializableData.attributeIteratorValue(it);
}
const zfchar *requireAttribute(ZF_IN const ZFSerializableData &serializableData,
                               ZF_IN const zfchar *desiredAttribute,
                               ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */,
                               ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */)
{
    const zfchar *ret = checkAttribute(serializableData, desiredAttribute);
    if(ret == zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
            "missing attribute \"%s\"", desiredAttribute);
    }
    return ret;
}

const ZFSerializableData *checkElementByName(ZF_IN const ZFSerializableData &serializableData,
                                             ZF_IN const zfchar *desiredElementName)
{
    zfindex index = serializableData.elementFindByName(desiredElementName);
    if(index == zfindexMax())
    {
        return zfnull;
    }
    serializableData.elementAtIndex(index).resolvePropertyNameMark();
    return &(serializableData.elementAtIndex(index));
}
const ZFSerializableData *requireElementByName(ZF_IN const ZFSerializableData &serializableData,
                                               ZF_IN const zfchar *desiredElementName,
                                               ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */,
                                               ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */)
{
    const ZFSerializableData *ret = checkElementByName(serializableData, desiredElementName);
    if(ret == zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
            "missing element with name \"%s\"",
            desiredElementName);
    }
    return ret;
}

const ZFSerializableData *checkElementByCategory(ZF_IN const ZFSerializableData &serializableData,
                                                 ZF_IN const zfchar *desiredElementCategory)
{
    zfindex index = serializableData.elementFindByCategory(desiredElementCategory);
    if(index == zfindexMax())
    {
        return zfnull;
    }
    serializableData.elementAtIndex(index).resolveCategoryMark();
    return &(serializableData.elementAtIndex(index));
}
const ZFSerializableData *requireElementByCategory(ZF_IN const ZFSerializableData &serializableData,
                                                   ZF_IN const zfchar *desiredElementCategory,
                                                   ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */,
                                                   ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */)
{
    const ZFSerializableData *ret = checkElementByCategory(serializableData, desiredElementCategory);
    if(ret == zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
            "missing element with category \"%s\"",
            desiredElementCategory);
    }
    return ret;
}

static zfbool _ZFP_ZFSerializableUtilPrintResolveStatus(ZF_IN const ZFSerializableData &serializableData,
                                                        ZF_IN const ZFOutput &outputCallback,
                                                        ZF_IN zfindex level)
{
    if(!serializableData.resolvedAll())
    {
        for(zfindex i = 0; i < level; ++i)
        {
            outputCallback << "    ";
        }
        outputCallback << serializableData.itemClass();
        if(!serializableData.resolved())
        {
            outputCallback << "(unresolved)";
        }

        zfbool hasUnresolvedAttribute = zffalse;
        for(zfiterator it = serializableData.attributeIterator();
            serializableData.attributeIteratorIsValid(it);
            serializableData.attributeIteratorNextValue(it))
        {
            if(!serializableData.attributeIteratorResolved(it))
            {
                if(!hasUnresolvedAttribute)
                {
                    hasUnresolvedAttribute = zftrue;
                    outputCallback << " < ";
                }
                else
                {
                    outputCallback << "; ";
                }
                outputCallback
                    << serializableData.attributeIteratorKey(it)
                    << "="
                    << serializableData.attributeIteratorValue(it);
            }
        }
        if(hasUnresolvedAttribute)
        {
            outputCallback << " >";
        }

        outputCallback << "\n";

        for(zfindex i = 0; i < serializableData.elementCount(); ++i)
        {
            _ZFP_ZFSerializableUtilPrintResolveStatus(serializableData.elementAtIndex(i), outputCallback, level + 1);
        }
        return zftrue;
    }
    return zffalse;
}
zfbool printResolveStatus(ZF_IN const ZFSerializableData &serializableData,
                          ZF_IN_OPT const ZFOutput &outputCallback /* = ZFOutputDefault() */)
{
    if(!ZFSerializableDataResolveCheckEnable)
    {
        return zffalse;
    }
    zfstring tmp = "[ZFSerializable] not all resolved:\n";
    zfbool ret = _ZFP_ZFSerializableUtilPrintResolveStatus(serializableData, ZFOutputForString(tmp), 1);
    if(ret)
    {
        outputCallback.execute(tmp.cString());
    }
    return ret;
}

ZF_NAMESPACE_END_WITH_REGISTER(ZFSerializableUtil, ZF_NAMESPACE_GLOBAL)
ZF_NAMESPACE_GLOBAL_END

