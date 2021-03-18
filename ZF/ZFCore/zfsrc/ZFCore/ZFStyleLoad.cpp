#include "ZFStyleLoad.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
static void _ZFP_ZFStyleLoad_ZFStyleSet(ZF_IN const zfchar *styleKey, ZF_IN ZFStyleable *styleValue)
{
    if(styleValue != zfnull && styleValue->classData()->classIsTypeOf(ZFStyleList::ClassData()))
    {
        ZFStyleLoad(ZFCastZFObjectUnchecked(ZFStyleList *, styleValue));
    }
    else
    {
        ZFStyleSet(styleKey, styleValue);
    }
}
static void _ZFP_ZFStyleLoadImpl(ZF_IN const ZFFilePathInfoData &fileImpl,
                                 ZF_IN const zfchar *pathType,
                                 ZF_IN const zfchar *pathData,
                                 ZF_IN const ZFFilterForString *fileNameFilter,
                                 ZF_IN const ZFFilterForString *dirNameFilter,
                                 ZF_IN const zfchar *relativePath)
{
    ZFFileFindData fd;
    if(fileImpl.callbackFindFirst(fd, pathData))
    {
        do
        {
            if(*fd.fileName() == '.' || *fd.fileName() == '_'
                || zfstringFind(fd.fileName(), zfindexMax(), "._.") != zfindexMax())
            {
                continue;
            }

            zfstring relativePathTmp = relativePath;
            if(!relativePathTmp.isEmpty())
            {
                relativePathTmp += ZFFileSeparator();
            }
            relativePathTmp += fd.fileName();

            zfstring pathDataChild;
            fileImpl.callbackToChild(pathData, pathDataChild, fd.fileName());
            if(fd.fileIsDir())
            {
                if(dirNameFilter != zfnull && !dirNameFilter->filterCheckActive(fd.fileName()))
                {
                    continue;
                }
                _ZFP_ZFStyleLoadImpl(fileImpl, pathType, pathDataChild, fileNameFilter, dirNameFilter, relativePathTmp);
            }
            else
            {
                if(fileNameFilter != zfnull && !fileNameFilter->filterCheckActive(fd.fileName()))
                {
                    continue;
                }
                zfautoObject styleValue;
                if(ZFObjectIOLoadT(styleValue, ZFInputForPathInfo(ZFPathInfo(pathType, pathDataChild))))
                {
                    ZFFilePathOfWithoutExt(relativePathTmp, relativePathTmp);
                    _ZFP_ZFStyleLoad_ZFStyleSet(relativePathTmp, styleValue);
                }
            }
        } while(fileImpl.callbackFindNext(fd));
        fileImpl.callbackFindClose(fd);
    }
}

ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFStyleLoad,
                       ZFMP_IN(const ZFPathInfo &, pathInfo),
                       ZFMP_IN_OPT(const ZFFilterForString *, fileNameFilter, zfnull),
                       ZFMP_IN_OPT(const ZFFilterForString *, dirNameFilter, zfnull))
{
    const ZFFilePathInfoData *fileImpl = ZFFilePathInfoDataForPathType(pathInfo.pathType);
    if(fileImpl == zfnull)
    {
        return zffalse;
    }
    ZFStyleChangeBlock();

    if(!fileImpl->callbackIsDir(pathInfo.pathData))
    {
        zfstring fileName;
        if(!fileImpl->callbackToFileName(pathInfo.pathData, fileName))
        {
            return zffalse;
        }
        if(fileNameFilter != zfnull && !fileNameFilter->filterCheckActive(fileName))
        {
            return zftrue;
        }
        zfautoObject styleValue;
        if(!ZFObjectIOLoadT(styleValue, ZFInputForPathInfo(pathInfo)))
        {
            return zffalse;
        }
        ZFFilePathOfWithoutExt(fileName, fileName);
        _ZFP_ZFStyleLoad_ZFStyleSet(fileName, styleValue);
        return zftrue;
    }

    _ZFP_ZFStyleLoadImpl(*fileImpl, pathInfo.pathType, pathInfo.pathData, fileNameFilter, dirNameFilter, zfnull);
    return zftrue;
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFStyleLoad,
                       ZFMP_IN(const ZFSerializableData &, serializableData))
{
    if(serializableData.elementCount() > 0)
    {
        ZFStyleChangeBlock();
        for(zfindex i = 0; i < serializableData.elementCount(); ++i)
        {
            const ZFSerializableData &child = serializableData.elementAtIndex(i);
            const zfchar *styleKey = ZFSerializableUtil::checkPropertyName(child);
            if(styleKey != zfnull)
            {
                zfautoObject styleValueHolder = ZFObjectFromData(child);
                ZFStyleable *styleValue = styleValueHolder;
                if(styleValue != zfnull)
                {
                    _ZFP_ZFStyleLoad_ZFStyleSet(styleKey, styleValue);
                }
            }
        }
    }
    return zftrue;
}
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFStyleLoad,
                       ZFMP_IN(ZFStyleList *, styleList))
{
    if(styleList != zfnull && styleList->itemCount() > 0)
    {
        ZFStyleChangeBlock();
        for(zfindex i = 0; i < styleList->itemCount(); ++i)
        {
            _ZFP_ZFStyleLoad_ZFStyleSet(styleList->itemKeyAtIndex(i), styleList->itemValueAtIndex(i));
        }
    }
    return zftrue;
}

// ============================================================
zfclassNotPOD _ZFP_ZFStyleListPrivate
{
public:
    ZFCoreArrayPOD<zfchar *> keyList;
    ZFCoreArrayPOD<ZFStyleable *> valueList;
};

ZFMETHOD_DEFINE_0(ZFStyleList, zfindex, itemCount)
{
    return d->keyList.count();
}
ZFMETHOD_DEFINE_1(ZFStyleList, const zfchar *, itemKeyAtIndex,
                  ZFMP_IN(zfindex, index))
{
    return d->keyList.get(index);
}
ZFMETHOD_DEFINE_1(ZFStyleList, ZFStyleable *, itemValueAtIndex,
                  ZFMP_IN(zfindex, index))
{
    return d->valueList.get(index);
}
ZFMETHOD_DEFINE_2(ZFStyleList, void, itemForKey,
                  ZFMP_IN(const zfchar *, key),
                  ZFMP_IN(ZFStyleable *, value))
{
    for(zfindex i = 0; i < d->keyList.count(); ++i)
    {
        if(zfscmpTheSame(d->keyList[i], key))
        {
            if(value == zfnull)
            {
                zffree(d->keyList.removeAndGet(i));
                zfRelease(d->valueList.removeAndGet(i));
            }
            else
            {
                zfRetainChange(d->valueList[i], value);
            }
            return ;
        }
    }

    if(value != zfnull)
    {
        d->keyList.add(zfsCopy(key));
        d->valueList.add(zfRetain(value));
    }
}

void ZFStyleList::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFStyleListPrivate);
}
void ZFStyleList::objectOnDealloc(void)
{
    if(!d->keyList.isEmpty())
    {
        ZFCoreArrayPOD<zfchar *> keyListTmp = d->keyList;
        d->keyList = ZFCoreArrayPOD<zfchar *>();
        ZFCoreArrayPOD<ZFStyleable *> valueListTmp = d->valueList;
        d->valueList = ZFCoreArrayPOD<ZFStyleable *>();
        for(zfindex i = 0; i < keyListTmp.count(); ++i)
        {
            zffree(keyListTmp[i]);
            zfRelease(valueListTmp[i]);
        }
    }
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}

zfbool ZFStyleList::serializableOnSerializeFromData(ZF_IN const ZFSerializableData &serializableData,
                                                    ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */,
                                                    ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */)
{
    if(!zfsuper::serializableOnSerializeFromData(serializableData, outErrorHint, outErrorPos))
    {
        return zffalse;
    }
    for(zfindex i = 0; i < serializableData.elementCount(); ++i)
    {
        const ZFSerializableData &elementData = serializableData.elementAtIndex(i);
        if(elementData.resolved())
        {
            continue;
        }
        const zfchar *key = elementData.propertyName();
        if(key == zfnull)
        {
            continue;
        }
        zfautoObject value;
        if(!ZFObjectFromData(value, elementData, outErrorHint, outErrorPos))
        {
            return zffalse;
        }
        if(ZFCastZFObject(ZFStyleable *, value) == zfnull)
        {
            ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, elementData,
                "%s not type of %s",
                ZFObjectInfo(value.toObject()).cString(),
                ZFStyleable::ClassData()->className());
            return zffalse;
        }
        this->itemForKey(key, value);
    }
    return zftrue;
}
zfbool ZFStyleList::serializableOnSerializeToData(ZF_IN_OUT ZFSerializableData &serializableData,
                                                  ZF_IN ZFSerializable *referencedOwnerOrNull,
                                                  ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */)
{
    if(!zfsuper::serializableOnSerializeToData(serializableData, referencedOwnerOrNull, outErrorHint))
    {
        return zffalse;
    }
    if(referencedOwnerOrNull != zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHint, "reference not supported");
        return zffalse;
    }
    for(zfindex i = 0; i < d->keyList.count(); ++i)
    {
        ZFSerializableData elementData;
        if(!ZFObjectToData(elementData, d->valueList[i]->toObject(), outErrorHint))
        {
            return zffalse;
        }
        elementData.propertyName(d->keyList[i]);
        serializableData.elementAdd(elementData);
    }
    return zftrue;
}

ZF_NAMESPACE_GLOBAL_END

