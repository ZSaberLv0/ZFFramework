#include "ZFTypeId_CoreType.h"
#include "ZFObjectImpl.h"
#include "ZFSerializableDataSerializableConverter.h"
#include "ZFIdMap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// utils
#define _ZFP_ZFTYPEID_DEFINE_int_allow_negative(TypeName, Type) \
    ZFTYPEID_DEFINE_BY_STRING_CONVERTER(TypeName, Type, { \
            return zfsToIntT(v, src, srcLen, 10, zftrue); \
        }, { \
            return zfsFromIntT(s, v); \
        })
#define _ZFP_ZFTYPEID_DEFINE_int_disallow_negative(TypeName, Type) \
    ZFTYPEID_DEFINE_BY_STRING_CONVERTER(TypeName, Type, { \
            if(src != zfnull && zfsncmp(src, "-1", 2) == 0) \
            { \
                v = (Type)-1; \
                return zftrue; \
            } \
            else \
            { \
                return zfsToIntT(v, src, srcLen, 10, zffalse); \
            } \
        }, { \
            if(v == ((Type)-1)) \
            { \
                s += "-1"; \
                return zftrue; \
            } \
            else \
            { \
                return zfsFromIntT(s, v); \
            } \
        })

// ============================================================
_ZFP_ZFTYPEID_DEFINE_int_allow_negative(zfint, zfint)
ZFTYPEID_PROGRESS_DEFINE_BY_VALUE(zfint, zfint)
// ============================================================
_ZFP_ZFTYPEID_DEFINE_int_disallow_negative(zfuint, zfuint)
ZFTYPEID_PROGRESS_DEFINE_BY_VALUE(zfuint, zfuint)

// ============================================================
_ZFP_ZFTYPEID_DEFINE_int_disallow_negative(zfindex, zfindex)
ZFTYPEID_PROGRESS_DEFINE_BY_VALUE(zfindex, zfindex)

// ============================================================
_ZFP_ZFTYPEID_DEFINE_int_allow_negative(zftimet, zftimet)
ZFTYPEID_PROGRESS_DEFINE(zftimet, zftimet, {
        ret = (zft_zftimet)(from + (zft_zftimet)((to - from) * progress));
    })

// ============================================================
ZFTYPEID_DEFINE(zfidentity, zfidentity, {
        if(ZFSerializableUtil::requireItemClass(serializableData, ZFTypeId_zfidentity(), outErrorHint, outErrorPos) == zfnull)
        {
            return zffalse;
        }
        const zfchar *valueString = ZFSerializableUtil::checkPropertyValue(serializableData);
        if(valueString == zfnull)
        {
            v = zfidentityInvalid();
            return zftrue;
        }
        if(!zfidentityFromString(v, valueString))
        {
            ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
                "invalid value: \"%s\"", valueString);
            return zffalse;
        }
        serializableData.resolveMark();
        return zftrue;
    }, {
        serializableData.itemClass(ZFTypeId_zfidentity());
        zfstring s;
        if(!zfidentityToString(s, v))
        {
            ZFSerializableUtil::errorOccurred(outErrorHint,
                "unable to convert value to string");
            return zffalse;
        }
        serializableData.propertyValue(s.isEmpty() ? zfnull : s.cString());
        return zftrue;
    }, {
        if(src == zfnull
            || srcLen == 0
            || zfsncmp(src, ZFTOKEN_zfidentityInvalid, srcLen) == 0)
        {
            v = zfidentityInvalid();
            return zftrue;
        }
        v = ZFIdMapIdForName(srcLen == zfindexMax() || src[srcLen] == '\0' ? src : zfstring(src, srcLen).cString());
        return (v != zfidentityInvalid());
    }, {
        if(v == zfidentityInvalid())
        {
            s += ZFTOKEN_zfidentityInvalid;
            return zftrue;
        }
        else
        {
            const zfchar *name = ZFIdMapNameForId(v);
            if(name == zfnull)
            {
                return zffalse;
            }
            else
            {
                s += name;
                return zftrue;
            }
        }
    })

// ============================================================
_ZFP_ZFTYPEID_DEFINE_int_disallow_negative(zfflags, zfflags)

ZF_NAMESPACE_GLOBAL_END

