#include "ZFIdMap.h"
#include "ZFObjectImpl.h"

#include "ZFCore/ZFSTLWrapper/zfstl_map.h"
#include "ZFCore/ZFSTLWrapper/zfstl_string.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
_ZFP_ZFIdMapHolder::_ZFP_ZFIdMapHolder(ZF_IN const zfchar *idName)
: ZFCoreLibDestroyFlag(zffalse)
, idName(idName)
, idValue(_ZFP_ZFIdMapRegister(&ZFCoreLibDestroyFlag, idName))
{
}
_ZFP_ZFIdMapHolder::~_ZFP_ZFIdMapHolder(void)
{
    _ZFP_ZFIdMapUnregister(&ZFCoreLibDestroyFlag, *idValue);
}

// ============================================================
zfclassLikePOD _ZFP_ZFIdMapData
{
public:
    zfuint refCount;
    zfidentity idValue;
    zfstring idName;
    zfbool isDynamicRegister;
    ZFCoreArrayPOD<zfbool *> ZFCoreLibDestroyFlag;

public:
    _ZFP_ZFIdMapData(void)
    : refCount(1)
    , idValue(zfidentityInvalid())
    , idName()
    , isDynamicRegister(zffalse)
    , ZFCoreLibDestroyFlag()
    {
    }
    ~_ZFP_ZFIdMapData(void)
    {
        for(zfindex i = 0; i < this->ZFCoreLibDestroyFlag.count(); ++i)
        {
            *(this->ZFCoreLibDestroyFlag[i]) = zftrue;
        }
    }
};
typedef zfstlmap<zfidentity, _ZFP_ZFIdMapData *> _ZFP_ZFIdMapDataIdMapType;
typedef zfstlmap<const zfchar *, _ZFP_ZFIdMapData *, zfcharConst_zfstlComparer> _ZFP_ZFIdMapDataNameMapType;

zfclassLikePOD _ZFP_ZFIdMapModuleData
{
public:
    _ZFP_ZFIdMapDataIdMapType dataIdMap;
    _ZFP_ZFIdMapDataNameMapType dataNameMap;
    ZFIdentityGenerator idValueGenerator;
};

static _ZFP_ZFIdMapModuleData &_ZFP_ZFIdMapModuleDataRef(void)
{
    static _ZFP_ZFIdMapModuleData d;
    return d;
}

const zfidentity *_ZFP_ZFIdMapRegister(ZF_IN zfbool *ZFCoreLibDestroyFlag,
                                       ZF_IN const zfchar *idName,
                                       ZF_IN_OPT zfbool isDynamicRegister /* = zffalse */)
{
    if(zfsIsEmpty(idName))
    {
        zfCoreCriticalMessageTrim("[ZFIdMapDynamicRegister] empty name");
    }

    zfCoreMutexLocker();
    _ZFP_ZFIdMapModuleData &moduleData = _ZFP_ZFIdMapModuleDataRef();
    _ZFP_ZFIdMapDataIdMapType &dataIdMap = moduleData.dataIdMap;
    _ZFP_ZFIdMapDataNameMapType &dataNameMap = moduleData.dataNameMap;

    _ZFP_ZFIdMapData *data = zfnull;
    _ZFP_ZFIdMapDataNameMapType::iterator itName = dataNameMap.find(idName);
    if(itName != dataNameMap.end())
    {
        data = itName->second;
    }
    if(data != zfnull)
    {
        if(isDynamicRegister)
        {
            zfCoreCriticalMessageTrim("[ZFIdMapDynamicRegister] already registered: %s", idName);
        }
        ++(data->refCount);
    }
    else
    {
        data = zfnew(_ZFP_ZFIdMapData);
        data->idValue = moduleData.idValueGenerator.idAcquire();
        data->idName = idName;
        data->isDynamicRegister = isDynamicRegister;

        dataIdMap[data->idValue] = data;
        dataNameMap[data->idName.cString()] = data;
    }
    if(ZFCoreLibDestroyFlag != zfnull)
    {
        data->ZFCoreLibDestroyFlag.add(ZFCoreLibDestroyFlag);
    }

    return &(data->idValue);
}
void _ZFP_ZFIdMapUnregister(ZF_IN zfbool *ZFCoreLibDestroyFlag,
                            ZF_IN zfidentity idValue,
                            ZF_IN_OPT zfbool isDynamicRegister /* = zffalse */)
{
    if(ZFCoreLibDestroyFlag != zfnull && *ZFCoreLibDestroyFlag)
    {
        return ;
    }
    zfCoreMutexLocker();
    _ZFP_ZFIdMapModuleData &moduleData = _ZFP_ZFIdMapModuleDataRef();
    _ZFP_ZFIdMapDataIdMapType &dataIdMap = moduleData.dataIdMap;
    _ZFP_ZFIdMapDataNameMapType &dataNameMap = moduleData.dataNameMap;

    _ZFP_ZFIdMapDataIdMapType::iterator it = dataIdMap.find(idValue);
    if(it == dataIdMap.end())
    {
        if(!isDynamicRegister)
        {
            zfCoreCriticalShouldNotGoHere();
        }
        return ;
    }
    _ZFP_ZFIdMapData *data = it->second;
    if(!data->isDynamicRegister && isDynamicRegister)
    {
        zfCoreCriticalMessageTrim(
            "[ZFIdMapDynamicUnregister] unregister %s(%s) which is not dynamic registered",
            zfsFromInt(data->idValue).cString(),
            data->idName.cString());
    }
    data->ZFCoreLibDestroyFlag.removeElement(ZFCoreLibDestroyFlag);
    --(data->refCount);
    if(data->refCount == 0)
    {
        dataIdMap.erase(data->idValue);
        dataNameMap.erase(data->idName.cString());
        zfdelete(data);
    }
}
const zfchar *ZFIdMapNameForId(ZF_IN zfidentity idValue)
{
    zfCoreMutexLocker();
    _ZFP_ZFIdMapModuleData &moduleData = _ZFP_ZFIdMapModuleDataRef();
    _ZFP_ZFIdMapDataIdMapType &dataIdMap = moduleData.dataIdMap;

    _ZFP_ZFIdMapDataIdMapType::const_iterator it = dataIdMap.find(idValue);
    if(it != dataIdMap.end())
    {
        return it->second->idName.cString();
    }
    return zfnull;
}
zfidentity ZFIdMapIdForName(ZF_IN const zfchar *idName)
{
    zfCoreMutexLocker();
    _ZFP_ZFIdMapModuleData &moduleData = _ZFP_ZFIdMapModuleDataRef();
    _ZFP_ZFIdMapDataNameMapType &dataNameMap = moduleData.dataNameMap;

    _ZFP_ZFIdMapDataNameMapType::const_iterator it = dataNameMap.find(idName);
    if(it != dataNameMap.end())
    {
        return it->second->idValue;
    }
    return zfidentityInvalid();
}
void ZFIdMapGetAll(ZF_IN_OUT ZFCoreArrayPOD<zfidentity> &idValues, ZF_IN_OUT ZFCoreArrayPOD<const zfchar *> &idNames)
{
    zfCoreMutexLocker();
    _ZFP_ZFIdMapModuleData &moduleData = _ZFP_ZFIdMapModuleDataRef();

    idValues.capacity(idValues.count() + moduleData.dataIdMap.size());
    idNames.capacity(idNames.count() + moduleData.dataIdMap.size());
    for(_ZFP_ZFIdMapDataIdMapType::iterator it = moduleData.dataIdMap.begin(); it != moduleData.dataIdMap.end(); ++it)
    {
        idValues.add(it->second->idValue);
        idNames.add(it->second->idName);
    }
}

zfidentity ZFIdMapDynamicRegister(ZF_IN const zfchar *idName)
{
    return *_ZFP_ZFIdMapRegister(zfnull, idName, zftrue);
}
void ZFIdMapDynamicUnregister(ZF_IN zfidentity idValue)
{
    _ZFP_ZFIdMapUnregister(zfnull, idValue, zftrue);
}

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(const zfchar *, ZFIdMapNameForId, ZFMP_IN(zfidentity, idValue))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(zfidentity, ZFIdMapIdForName, ZFMP_IN(const zfchar *, idName))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(void, ZFIdMapGetAll, ZFMP_IN_OUT(ZFCoreArrayPOD<zfidentity> &, idValues), ZFMP_IN_OUT(ZFCoreArrayPOD<const zfchar *> &, idNames))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(zfidentity, ZFIdMapDynamicRegister, ZFMP_IN(const zfchar *, idName))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(void, ZFIdMapDynamicUnregister, ZFMP_IN(zfidentity, idValue))

ZF_NAMESPACE_GLOBAL_END
#endif

