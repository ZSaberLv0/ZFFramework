#include "ZFSerializableDataSerializableConverter.h"
#include "ZFObjectImpl.h"
#include "ZFSerializableUtil.h"

ZF_NAMESPACE_GLOBAL_BEGIN


// ============================================================
/*
 * // '-' for empty ClassName
 *
 * <ClassNameEncoded AttrKey1="AttrValue1" AttrKey2="AttrValue2" [
 *     <ChildElement0>
 *     <ChildElement1 AttrKey3="">
 *     <ChildElement2 []>
 * ]>
 */
#define _ZFP_ZFSD_NullClass '-'
#define _ZFP_ZFSD_IsSpace(c) (zfcharIsSpace(c) || zfcharIsEndl(c))
#define _ZFP_ZFSD_Space ' '
#define _ZFP_ZFSD_ObjBegin '<'
#define _ZFP_ZFSD_ObjEnd '>'
#define _ZFP_ZFSD_AttrAssign '='
#define _ZFP_ZFSD_AttrValuePair '"'
#define _ZFP_ZFSD_AttrValuePairEscape '\\'
#define _ZFP_ZFSD_ChildBegin '['
#define _ZFP_ZFSD_ChildEnd ']'

// ============================================================
// escape char map for serializable data's class name and attribute key name
// everything would be escaped except:
//   0~9
//   a-z
//   A-Z
//   @#$_
static const zfchar _ZFP_ZFSerializableEscapeCharMap[256] = {
    // 0x00 ~ 0x0F
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // 0x10 ~ 0x1F
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // 0x20 ~ 0x2F
    0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // 0x30 ~ 0x3F
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
    // 0x40 ~ 0x4F
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    // 0x50 ~ 0x5F
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1,
    // 0x60 ~ 0x6F
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    // 0x70 ~ 0x7F
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
    // 0x80 ~ 0x8F
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // 0x90 ~ 0x9F
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // 0xA0 ~ 0xAF
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // 0xB0 ~ 0xBF
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // 0xC0 ~ 0xCF
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // 0xD0 ~ 0xDF
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // 0xE0 ~ 0xEF
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // 0xF0 ~ 0xFF
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    //
};

// ============================================================
zfbool ZFSerializableDataFromZfsd(ZF_OUT ZFSerializableData &serializableData,
                                  ZF_IN const ZFInput &input,
                                  ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */)
{
    if(!input.callbackIsValid())
    {
        ZFSerializableUtil::errorOccurred(outErrorHint, "invalid input callback");
        return zffalse;
    }
    ZFBuffer buf = ZFInputReadToBuffer(input);
    if(buf.buffer() == zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHint, "unable to load data from input");
        return zffalse;
    }
    zfbool ret = ZFSerializableDataFromZfsd(serializableData, buf.bufferAsString(), buf.bufferAsStringLength(), outErrorHint);
    if(ret)
    {
        serializableData.pathInfo(input.pathInfo());
    }
    return ret;
}
ZFSerializableData ZFSerializableDataFromZfsd(ZF_IN const ZFInput &input,
                                              ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */)
{
    ZFSerializableData ret;
    if(ZFSerializableDataFromZfsd(ret, input, outErrorHint))
    {
        return ret;
    }
    else
    {
        return ZFSerializableData();
    }
}
zfbool ZFSerializableDataToZfsd(ZF_IN_OUT const ZFOutput &output,
                                ZF_IN const ZFSerializableData &serializableData,
                                ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */,
                                ZF_IN_OPT zfbool prettyPrint /* = zftrue */)
{
    if(!output.callbackIsValid())
    {
        ZFSerializableUtil::errorOccurred(outErrorHint, "invalid output callback");
        return zffalse;
    }
    zfstring tmp;
    if(!ZFSerializableDataToZfsd(tmp, serializableData, outErrorHint, prettyPrint))
    {
        return zffalse;
    }
    tmp += "\n";
    output.execute(tmp.cString(), tmp.length());
    return zftrue;
}

// ============================================================
static zfbool _ZFP_ZFSD_AttrValueDecode(ZF_IN_OUT zfstring &ret,
                                        ZF_IN_OUT const zfchar *&encodedData,
                                        ZF_IN const zfchar *srcEnd)
{
    if(*encodedData != _ZFP_ZFSD_AttrValuePair)
    {
        return zffalse;
    }
    ++encodedData;
    const zfchar *pLeft = encodedData;
    while(encodedData < srcEnd && *encodedData != _ZFP_ZFSD_AttrValuePair)
    {
        if(*encodedData == _ZFP_ZFSD_AttrValuePairEscape && encodedData + 1 < srcEnd)
        {
            ++encodedData;
            if(*encodedData == _ZFP_ZFSD_AttrValuePair)
            {
                ret.append(pLeft, encodedData - 1 - pLeft);
                ret += _ZFP_ZFSD_AttrValuePair;
                ++encodedData;
                pLeft = encodedData;
            }
            else if(*encodedData == _ZFP_ZFSD_AttrValuePairEscape)
            {
                ret.append(pLeft, encodedData - 1 - pLeft);
                ret += _ZFP_ZFSD_AttrValuePairEscape;
                ++encodedData;
                pLeft = encodedData;
            }
            else if(*encodedData == 'r')
            {
                ret.append(pLeft, encodedData - 1 - pLeft);
                ret += '\r';
                ++encodedData;
                pLeft = encodedData;
            }
            else if(*encodedData == 'n')
            {
                ret.append(pLeft, encodedData - 1 - pLeft);
                ret += '\n';
                ++encodedData;
                pLeft = encodedData;
            }
            else if(*encodedData == 't')
            {
                ret.append(pLeft, encodedData - 1 - pLeft);
                ret += '\t';
                ++encodedData;
                pLeft = encodedData;
            }
        }
        else
        {
            zfcharMoveNext(encodedData);
        }
    }
    ret.append(pLeft, encodedData - pLeft);

    if(*encodedData == _ZFP_ZFSD_AttrValuePair)
    {
        ++encodedData;
        return zftrue;
    }
    else
    {
        return zffalse;
    }
}
static void _ZFP_ZFSD_AttrValueEncode(ZF_IN_OUT zfstring &ret,
                                      ZF_IN const zfchar *value)
{
    ret += _ZFP_ZFSD_AttrValuePair;
    const zfchar *pL = value;
    while(*value != '\0')
    {
        if(*value == _ZFP_ZFSD_AttrValuePair)
        {
            ret.append(pL, value - pL);
            ++value;
            pL = value;
            ret += _ZFP_ZFSD_AttrValuePairEscape;
            ret += _ZFP_ZFSD_AttrValuePair;
        }
        else if(*value == _ZFP_ZFSD_AttrValuePairEscape)
        {
            ret.append(pL, value - pL);
            ++value;
            pL = value;
            ret += _ZFP_ZFSD_AttrValuePairEscape;
            ret += _ZFP_ZFSD_AttrValuePairEscape;
        }
        else if(*value == '\r')
        {
            ret.append(pL, value - pL);
            ++value;
            pL = value;
            ret += _ZFP_ZFSD_AttrValuePairEscape;
            ret += 'r';
        }
        else if(*value == '\n')
        {
            ret.append(pL, value - pL);
            ++value;
            pL = value;
            ret += _ZFP_ZFSD_AttrValuePairEscape;
            ret += 'n';
        }
        else if(*value == '\t')
        {
            ret.append(pL, value - pL);
            ++value;
            pL = value;
            ret += _ZFP_ZFSD_AttrValuePairEscape;
            ret += 't';
        }
        else
        {
            zfcharMoveNext(value);
        }
    }
    ret.append(pL, value - pL);
    ret += _ZFP_ZFSD_AttrValuePair;
}
zfbool _ZFP_ZFSerializableDataFromZfsd(ZF_OUT ZFSerializableData &serializableData,
                                       ZF_IN_OUT const zfchar *&encodedData,
                                       ZF_IN zfindex encodedDataLen,
                                       ZF_OUT zfstring *outErrorHint,
                                       ZF_IN_OPT zfbool validateTail = zffalse)
{
    if(encodedData == zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHint,
            "invalid param");
        return zffalse;
    }
    const zfchar *srcEnd = (encodedData + ((encodedDataLen == zfindexMax()) ? zfslen(encodedData) : encodedDataLen));
    zfbool ret = zffalse;
    do
    {
        const zfchar *pLeft = zfnull;
        const zfchar *pRight = zfnull;
        zfstring decodedTmp;

        // begin
        zfcharSkipSpaceAndEndl(encodedData, srcEnd);
        if(encodedData >= srcEnd || *encodedData != _ZFP_ZFSD_ObjBegin) {break;}
        ++encodedData;

        // class name
        zfcharSkipSpaceAndEndl(encodedData, srcEnd);
        pLeft = encodedData;
        while(encodedData < srcEnd
            && !_ZFP_ZFSD_IsSpace(*encodedData)
            && *encodedData != _ZFP_ZFSD_ChildBegin
            && *encodedData != _ZFP_ZFSD_ObjEnd
            )
        {
            zfcharMoveNext(encodedData);
        }
        pRight = encodedData;
        zfcharSkipSpaceAndEndl(encodedData, srcEnd);
        if(encodedData >= srcEnd) {break;}

        if(pRight == pLeft + 1 && *pLeft == _ZFP_ZFSD_NullClass)
        {
            serializableData.itemClass(zfnull);
        }
        else
        {
            zfCoreDataDecode(decodedTmp, pLeft, pRight - pLeft);
            serializableData.itemClass(decodedTmp.cString());
            decodedTmp.removeAll();
        }

        // attributes
        if(*encodedData != _ZFP_ZFSD_ChildBegin && *encodedData != _ZFP_ZFSD_ObjEnd)
        {
            while(encodedData < srcEnd)
            {
                if(*encodedData == _ZFP_ZFSD_ChildBegin || *encodedData == _ZFP_ZFSD_ObjEnd)
                {
                    ret = zftrue;
                    break;
                }

                // name
                pLeft = encodedData;
                while(encodedData < srcEnd
                    && !_ZFP_ZFSD_IsSpace(*encodedData)
                    && *encodedData != _ZFP_ZFSD_AttrAssign
                    )
                {
                    zfcharMoveNext(encodedData);
                }
                if(encodedData >= srcEnd) {break;}
                pRight = encodedData;
                if(*encodedData != _ZFP_ZFSD_AttrAssign)
                {
                    zfcharSkipSpaceAndEndl(encodedData, srcEnd);
                }
                if(*encodedData != _ZFP_ZFSD_AttrAssign) {break;}
                ++encodedData;
                zfcharSkipSpaceAndEndl(encodedData, srcEnd);

                zfstring attributeName;
                zfCoreDataDecode(attributeName, pLeft, pRight - pLeft);

                // value
                if(!_ZFP_ZFSD_AttrValueDecode(decodedTmp, encodedData, srcEnd)) {break;}

                // save
                if(!attributeName.isEmpty() && !decodedTmp.isEmpty())
                {
                    serializableData.attributeForName(attributeName.cString(), decodedTmp.cString());
                }
                decodedTmp.removeAll();

                zfcharSkipSpaceAndEndl(encodedData, srcEnd);
            }
            if(!ret) {break;}
            ret = zffalse;
        } // if(*encodedData == _ZFP_ZFSD_AttrBegin)

        // elements
        if(*encodedData == _ZFP_ZFSD_ChildBegin)
        {
            ++encodedData;
            while(encodedData < srcEnd)
            {
                zfcharSkipSpaceAndEndl(encodedData, srcEnd);
                if(*encodedData == _ZFP_ZFSD_ChildEnd)
                {
                    ++encodedData;
                    zfcharSkipSpaceAndEndl(encodedData, srcEnd);
                    ret = zftrue;
                    break;
                }

                ZFSerializableData element;
                if(!_ZFP_ZFSerializableDataFromZfsd(element, encodedData, srcEnd - encodedData, outErrorHint))
                {
                    return zffalse;
                }
                serializableData.elementAdd(element);
            }
            if(!ret) {break;}
            ret = zffalse;
        }

        // tail
        if(encodedData >= srcEnd || *encodedData != _ZFP_ZFSD_ObjEnd) {break;}
        ++encodedData;
        if(validateTail)
        {
            zfcharSkipSpaceAndEndl(encodedData, srcEnd);
            if(encodedData < srcEnd) {break;}
        }

        ret = zftrue;
    } while(zffalse);
    if(!ret)
    {
        ZFSerializableUtil::errorOccurred(outErrorHint,
            "wrong serializable string format at position: \"%s\"",
            zfstring(encodedData, srcEnd - encodedData).cString());
    }
    return ret;
}

// ============================================================
zfbool ZFSerializableDataFromZfsd(ZF_OUT ZFSerializableData &serializableData,
                                  ZF_IN const zfchar *encodedData,
                                  ZF_IN_OPT zfindex encodedDataLen /* = zfindexMax() */,
                                  ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */)
{
    return _ZFP_ZFSerializableDataFromZfsd(serializableData, encodedData, encodedDataLen, outErrorHint, zfHint("validateTail")zftrue);
}
ZFSerializableData ZFSerializableDataFromZfsd(ZF_IN const zfchar *encodedData,
                                              ZF_IN zfindex encodedDataLen,
                                              ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */)
{
    ZFSerializableData ret;
    if(ZFSerializableDataFromZfsd(ret, encodedData, encodedDataLen, outErrorHint))
    {
        return ret;
    }
    else
    {
        return ZFSerializableData();
    }
}

static zfbool _ZFP_ZFSerializableDataToZfsdPretty(ZF_OUT zfstring &result,
                                                  ZF_IN const ZFSerializableData &serializableData,
                                                  ZF_OUT zfstring *outErrorHint,
                                                  ZF_IN zfindex indentLevel);
zfbool ZFSerializableDataToZfsd(ZF_OUT zfstring &result,
                                ZF_IN const ZFSerializableData &serializableData,
                                ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */,
                                ZF_IN_OPT zfbool prettyPrint /* = zftrue */)
{
    if(prettyPrint)
    {
        return _ZFP_ZFSerializableDataToZfsdPretty(result, serializableData, outErrorHint, 0);
    }

    result += _ZFP_ZFSD_ObjBegin;

    // serializable class
    if(serializableData.itemClass() == zfnull)
    {
        result += _ZFP_ZFSD_NullClass;
    }
    else
    {
        zfCoreDataEncode(result, serializableData.itemClass(), zfindexMax(), _ZFP_ZFSerializableEscapeCharMap);
    }

    // attributes
    if(serializableData.attributeCount() > 0)
    {
        for(zfiterator it = serializableData.attributeIterator();
            serializableData.attributeIteratorIsValid(it);
            serializableData.attributeIteratorNextValue(it))
        {
            result += _ZFP_ZFSD_Space;
            zfCoreDataEncode(result, serializableData.attributeIteratorKey(it), zfindexMax(), _ZFP_ZFSerializableEscapeCharMap);
            result += _ZFP_ZFSD_AttrAssign;
            _ZFP_ZFSD_AttrValueEncode(result, serializableData.attributeIteratorValue(it));
        }
    }

    // elements
    if(serializableData.elementCount() > 0)
    {
        result += _ZFP_ZFSD_ChildBegin;
        for(zfindex i = 0; i < serializableData.elementCount(); ++i)
        {
            if(!ZFSerializableDataToZfsd(result, serializableData.elementAtIndex(i), outErrorHint))
            {
                return zffalse;
            }
        }
        result += _ZFP_ZFSD_ChildEnd;
    }

    result += _ZFP_ZFSD_ObjEnd;
    return zftrue;
}
zfstring ZFSerializableDataToZfsd(ZF_IN const ZFSerializableData &serializableData,
                                  ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */,
                                  ZF_IN_OPT zfbool prettyPrint /* = zftrue */)
{
    zfstring tmp;
    ZFSerializableDataToZfsd(tmp, serializableData, outErrorHint);
    return tmp;
}

// ============================================================
static void _ZFP_ZFSerializableDataToZfsdPrettyIndent(ZF_OUT zfstring &result,
                                                      ZF_IN zfindex indentLevel)
{
    for(zfindex i = 0; i < indentLevel; ++i)
    {
        result += "    ";
    }
}
static zfbool _ZFP_ZFSerializableDataToZfsdPretty(ZF_OUT zfstring &result,
                                                  ZF_IN const ZFSerializableData &serializableData,
                                                  ZF_OUT zfstring *outErrorHint,
                                                  ZF_IN zfindex indentLevel)
{
    _ZFP_ZFSerializableDataToZfsdPrettyIndent(result, indentLevel);
    result += _ZFP_ZFSD_ObjBegin;

    // serializable class
    if(serializableData.itemClass() == zfnull)
    {
        result += _ZFP_ZFSD_NullClass;
    }
    else
    {
        zfCoreDataEncode(result, serializableData.itemClass(), zfindexMax(), _ZFP_ZFSerializableEscapeCharMap);
    }

    zfbool needBreak = (serializableData.attributeCount() > 3);

    // attributes
    if(serializableData.attributeCount() > 0)
    {
        for(zfiterator it = serializableData.attributeIterator();
            serializableData.attributeIteratorIsValid(it);
            serializableData.attributeIteratorNextValue(it))
        {
            if(needBreak)
            {
                result += '\n';
                _ZFP_ZFSerializableDataToZfsdPrettyIndent(result, indentLevel + 1);
            }
            result += _ZFP_ZFSD_Space;
            zfCoreDataEncode(result, serializableData.attributeIteratorKey(it), zfindexMax(), _ZFP_ZFSerializableEscapeCharMap);
            result += _ZFP_ZFSD_AttrAssign;
            _ZFP_ZFSD_AttrValueEncode(result, serializableData.attributeIteratorValue(it));
        }
    }

    // elements
    if(serializableData.elementCount() > 0)
    {
        if(needBreak)
        {
            result += '\n';
            _ZFP_ZFSerializableDataToZfsdPrettyIndent(result, indentLevel + 1);
        }
        else
        {
            result += ' ';
        }
        result += _ZFP_ZFSD_ChildBegin;
        result += '\n';
        for(zfindex i = 0; i < serializableData.elementCount(); ++i)
        {
            if(!_ZFP_ZFSerializableDataToZfsdPretty(result, serializableData.elementAtIndex(i), outErrorHint, indentLevel + 1))
            {
                return zffalse;
            }
            result += '\n';
        }
        _ZFP_ZFSerializableDataToZfsdPrettyIndent(result, indentLevel);
        result += _ZFP_ZFSD_ChildEnd;
    }

    if(needBreak)
    {
        result += ' ';
    }
    result += _ZFP_ZFSD_ObjEnd;
    return zftrue;
}

zfbool ZFObjectFromZfsd(ZF_OUT zfautoObject &ret,
                        ZF_IN const ZFInput &input,
                        ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */)
{
    ZFSerializableData data;
    if(ZFSerializableDataFromZfsd(data, input, outErrorHint))
    {
        return ZFObjectFromData(ret, data, outErrorHint);
    }
    else
    {
        return zffalse;
    }
}
zfautoObject ZFObjectFromZfsd(ZF_IN const ZFInput &input,
                              ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */)
{
    zfautoObject ret;
    ZFObjectFromZfsd(ret, input, outErrorHint);
    return ret;
}

zfbool ZFObjectToZfsd(ZF_IN_OUT const ZFOutput &output,
                      ZF_IN ZFObject *obj,
                      ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */,
                      ZF_IN_OPT zfbool prettyPrint /* = zftrue */)
{
    ZFSerializableData serializableData;
    if(!ZFObjectToData(serializableData, obj, outErrorHint))
    {
        return zffalse;
    }
    else
    {
        return ZFSerializableDataToZfsd(output, serializableData, outErrorHint, prettyPrint);
    }
}

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zfbool, ZFSerializableDataFromZfsd, ZFMP_OUT(ZFSerializableData &, serializableData), ZFMP_IN(const ZFInput &, input), ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(ZFSerializableData, ZFSerializableDataFromZfsd, ZFMP_IN(const ZFInput &, input), ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zfbool, ZFSerializableDataToZfsd, ZFMP_IN_OUT(const ZFOutput &, output), ZFMP_IN(const ZFSerializableData &, serializableData), ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull))

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_4(zfbool, ZFSerializableDataFromZfsd, ZFMP_OUT(ZFSerializableData &, serializableData), ZFMP_IN(const zfchar *, encodedData), ZFMP_IN_OPT(zfindex, encodedDataLen, zfindexMax()), ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(ZFSerializableData, ZFSerializableDataFromZfsd, ZFMP_IN(const zfchar *, encodedData), ZFMP_IN_OPT(zfindex, encodedDataLen, zfindexMax()), ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zfbool, ZFSerializableDataToZfsd, ZFMP_OUT(zfstring &, result), ZFMP_IN(const ZFSerializableData &, serializableData), ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfstring, ZFSerializableDataToZfsd, ZFMP_IN(const ZFSerializableData &, serializableData), ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull))

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zfbool, ZFObjectFromZfsd, ZFMP_OUT(zfautoObject &, ret), ZFMP_IN(const ZFInput &, input), ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfautoObject, ZFObjectFromZfsd, ZFMP_IN(const ZFInput &, input), ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zfbool, ZFObjectToZfsd, ZFMP_IN_OUT(const ZFOutput &, output), ZFMP_IN(ZFObject *, obj), ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull))

ZF_NAMESPACE_GLOBAL_END
#endif

