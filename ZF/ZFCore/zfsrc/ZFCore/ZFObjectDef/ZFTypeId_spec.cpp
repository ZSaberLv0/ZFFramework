#include "ZFTypeId_spec.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// void
ZFTYPEID_ID_DATA_REGISTER(void, void)

// ============================================================
// zfautoObject
ZFTYPEID_DEFINE_BY_SERIALIZABLE_CONVERTER_WITH_CUSTOM_WRAPPER(zfautoObject, zfautoObject, {
        return ZFObjectFromData(v, serializableData, outErrorHint, outErrorPos);
    }, {
        return ZFObjectToData(serializableData, v, outErrorHint);
    })
ZFOUTPUT_TYPE_DEFINE(zfautoObject, {
        output.execute(
            (v == zfnull)
            ? ZFTOKEN_zfnull
            : v.toObject()->objectInfo()
        );
    })

// ============================================================
// ZFCallerInfo
ZFTYPEID_DEFINE_BY_STRING_CONVERTER(ZFCallerInfo, ZFCallerInfo, {
        ZFIndexRange pos[3];
        zfmemset(pos, 0, sizeof(pos));
        const zfchar *p = src;
        const zfchar *pEnd = src + ((srcLen == zfindexMax()) ? zfslen(src) : srcLen);

        zfcharSkipSpace(p, pEnd);
        if(p >= pEnd)
        {
            v.callerInfo();
            return zftrue;
        }
        if(*p != '[')
        {
            return zffalse;
        }
        ++p;

        if(*p != ' ')
        {
            pos[0].start = p - src;
            while(*p != ' ' && p < pEnd) {++p;}
            pos[0].count = p - src - pos[0].start;
        }
        ++p;

        if(*p != ' ')
        {
            pos[1].start = p - src;
            while(*p != ' ' && p < pEnd) {++p;}
            pos[1].count = p - src - pos[1].start;
        }
        ++p;

        if(*p != '(')
        {
            return zffalse;
        }
        ++p;
        pos[2].start = p - src;
        while(*p != ')' && p < pEnd) {++p;}
        pos[2].count = p - src - pos[2].start;

        if(p + 2 > pEnd || *p != ')' || *(p + 1) != ']')
        {
            return zffalse;
        }

        v.callerInfo(
                (pos[0].count > 0)
                    ? zfstring(src + pos[0].start, pos[0].count).cString()
                    : zfnull,
                (pos[1].count > 0)
                    ? zfstring(src + pos[1].start, pos[1].count).cString()
                    : zfnull,
                (zfuint)zfsToInt(zfstring(src + pos[2].start, pos[2].count).cString())
            );
        return zftrue;
    }, {
        v.callerInfoT(s);
        return zftrue;
    })
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFCallerInfo, void, callerInfoT, ZFMP_IN_OUT(zfstring &, ret))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallerInfo, zfstring, callerInfo)

ZF_NAMESPACE_GLOBAL_END

