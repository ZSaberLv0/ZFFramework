#include "ZFTextTemplate.h"

ZF_NAMESPACE_GLOBAL_BEGIN

static const zfchar *_ZFP_ZFTextTemplate_tagL = "{ZFTT_";
static const zfindex _ZFP_ZFTextTemplate_tagLSize = zfslen(_ZFP_ZFTextTemplate_tagL);
static const zfchar _ZFP_ZFTextTemplate_tagR = '}';

// ============================================================
zfclassLikePOD ZF_ENV_EXPORT _ZFP_ZFTextTemplateIndexDataState
{
public:
    const ZFTextTemplateIndexData *indexData;
    zfindex indexCur;

public:
    _ZFP_ZFTextTemplateIndexDataState(void)
    : indexData(zfnull)
    , indexCur(0)
    {
    }
};

static void _ZFP_ZFTextTemplateApply_replaceData(ZF_IN const ZFTextTemplateParam &param,
                                                 ZF_IN_OUT ZFCoreMap &stateMap,
                                                 ZF_IN const ZFOutput &output,
                                                 ZF_IN const zfchar *pEnd,
                                                 ZF_IN_OUT const zfchar *&data,
                                                 ZF_IN_OUT const zfchar *&p,
                                                 ZF_IN_OUT zfindex &size);
static void _ZFP_ZFTextTemplateApply_enableData(ZF_IN const ZFTextTemplateParam &param,
                                                ZF_IN_OUT ZFCoreMap &stateMap,
                                                ZF_IN const ZFOutput &output,
                                                ZF_IN const zfchar *pEnd,
                                                ZF_IN_OUT const zfchar *&data,
                                                ZF_IN_OUT const zfchar *&p,
                                                ZF_IN_OUT zfindex &size,
                                                ZF_IN_OUT zfindex &condCount);
static void _ZFP_ZFTextTemplateApply_indexData(ZF_IN const ZFTextTemplateParam &param,
                                               ZF_IN_OUT ZFCoreMap &stateMap,
                                               ZF_IN const ZFOutput &output,
                                               ZF_IN const zfchar *pEnd,
                                               ZF_IN_OUT const zfchar *&data,
                                               ZF_IN_OUT const zfchar *&p,
                                               ZF_IN_OUT zfindex &size);

// ============================================================
ZFMETHOD_FUNC_DEFINE_4(zfindex, ZFTextTemplateApply,
                       ZFMP_IN(const ZFTextTemplateParam &, param),
                       ZFMP_IN(const ZFOutput &, output),
                       ZFMP_IN(const zfchar *, data),
                       ZFMP_IN_OPT(zfindex, dataSize, zfindexMax()))
{
    if(data == zfnull)
    {
        return zfindexMax();
    }

    zfindex condCount = 0;
    zfindex size = 0;
    const zfchar *p = data;
    const zfchar *pEnd = (data + ((dataSize == zfindexMax()) ? zfslen(data) : dataSize));
    ZFCoreMap stateMap;
    do
    {
        if(p >= pEnd)
        {
            if(p > data)
            {
                size += p - data;
                if(output.callbackIsValid())
                {
                    output.execute(data, p - data);
                }
            }
            break;
        }
        if(!(
                (zfindex)(pEnd - p) > _ZFP_ZFTextTemplate_tagLSize
                && zfsncmp(p, _ZFP_ZFTextTemplate_tagL, _ZFP_ZFTextTemplate_tagLSize) == 0
            ))
        {
            ++p;
            continue;
        }
        if(p > data)
        {
            size += p - data;
            if(output.callbackIsValid())
            {
                output.execute(data, p - data);
            }
            data = p;
        }

        switch(p[_ZFP_ZFTextTemplate_tagLSize])
        {
            case 'R':
                p += _ZFP_ZFTextTemplate_tagLSize + 1;
                _ZFP_ZFTextTemplateApply_replaceData(param, stateMap, output, pEnd, data, p, size);
                break;
            case 'C':
                p += _ZFP_ZFTextTemplate_tagLSize + 1;
                _ZFP_ZFTextTemplateApply_enableData(param, stateMap, output, pEnd, data, p, size, condCount);
                break;
            case 'I':
                p += _ZFP_ZFTextTemplate_tagLSize + 1;
                _ZFP_ZFTextTemplateApply_indexData(param, stateMap, output, pEnd, data, p, size);
                break;
            default:
                ++p;
                break;
        }
    } while(zftrue);
    return size;
}
ZFMETHOD_FUNC_DEFINE_3(zfindex, ZFTextTemplateApply,
                       ZFMP_IN(const ZFTextTemplateParam &, param),
                       ZFMP_IN(const ZFOutput &, output),
                       ZFMP_IN(const ZFInput &, input))
{
    ZFBuffer buffer = ZFInputReadToBuffer(input);
    if(buffer.buffer() == zfnull)
    {
        return zfindexMax();
    }
    return ZFTextTemplateApply(param, output, buffer.bufferAsString(), buffer.bufferAsStringLength());
}

// ============================================================
static zfindex _ZFP_ZFTextTemplateApply_keyLength(ZF_IN const zfchar *pEnd,
                                                  ZF_IN const zfchar *p)
{
    if(*p == ' ' || *p == '\t' || *p == _ZFP_ZFTextTemplate_tagR)
    {
        return zfindexMax();
    }
    const zfchar *t = p;
    while(t < pEnd && *t != _ZFP_ZFTextTemplate_tagR) {++t;}
    if(t >= pEnd)
    {
        return zfindexMax();
    }
    return (t - p);
}
// ============================================================
static void _ZFP_ZFTextTemplateApply_replaceData(ZF_IN const ZFTextTemplateParam &param,
                                                 ZF_IN_OUT ZFCoreMap &stateMap,
                                                 ZF_IN const ZFOutput &output,
                                                 ZF_IN const zfchar *pEnd,
                                                 ZF_IN_OUT const zfchar *&data,
                                                 ZF_IN_OUT const zfchar *&p,
                                                 ZF_IN_OUT zfindex &size)
{ // {ZFTT_R_myKey}
    if(*p != '_')
    {
        return ;
    }
    ++p;
    zfindex keySize = _ZFP_ZFTextTemplateApply_keyLength(pEnd, p);
    if(keySize == zfindexMax())
    {
        return ;
    }
    zfstring key(p, keySize);
    p += keySize + 1;

    const zfchar *value = param.replaceData(key);
    if(value == zfnull)
    {
        return ;
    }
    zfindex valueLen = zfslen(value);
    if(output.callbackIsValid())
    {
        output.execute(value, valueLen);
    }
    size += valueLen;
    data = p;
}
static void _ZFP_ZFTextTemplateApply_enableData(ZF_IN const ZFTextTemplateParam &param,
                                                ZF_IN_OUT ZFCoreMap &stateMap,
                                                ZF_IN const ZFOutput &output,
                                                ZF_IN const zfchar *pEnd,
                                                ZF_IN_OUT const zfchar *&data,
                                                ZF_IN_OUT const zfchar *&p,
                                                ZF_IN_OUT zfindex &size,
                                                ZF_IN_OUT zfindex &condCount)
{ // {ZFTT_C_myCond}...{ZFTT_CE}
    if(pEnd - p >= 2 && p[0] == 'E' && p[1] == _ZFP_ZFTextTemplate_tagR)
    {
        p += 2;
        if(condCount > 0)
        {
            --condCount;
            data = p;
        }
        return ;
    }
    if(*p != '_')
    {
        return ;
    }
    ++p;
    zfindex keySize = _ZFP_ZFTextTemplateApply_keyLength(pEnd, p);
    if(keySize == zfindexMax())
    {
        return ;
    }
    zfstring key(p, keySize);
    p += keySize + 1;

    data = p;
    const zfbool *valueTmp = param.enableData(key);
    zfbool value = (valueTmp ? *valueTmp : param.enableDataDefault());
    if(value)
    {
        ++condCount;
        return ;
    }

    zfindex condCountOffset = 0;
    do
    {
        if(p >= pEnd)
        {
            data = p;
            break;
        }
        if(!(
                (zfindex)(pEnd - p) > _ZFP_ZFTextTemplate_tagLSize
                && zfsncmp(p, _ZFP_ZFTextTemplate_tagL, _ZFP_ZFTextTemplate_tagLSize) == 0
            ))
        {
            ++p;
            continue;
        }

        p += _ZFP_ZFTextTemplate_tagLSize;
        if(*p != 'C')
        {
            continue;
        }
        ++p;
        if(pEnd - p >= 2 && p[0] == 'E' && p[1] == _ZFP_ZFTextTemplate_tagR)
        {
            p += 2;
            if(condCountOffset == 0)
            {
                data = p;
                break;
            }
            else
            {
                --condCountOffset;
                continue;
            }
        }
        if(*p != '_')
        {
            continue;
        }
        ++p;
        zfindex keySize = _ZFP_ZFTextTemplateApply_keyLength(pEnd, p);
        if(keySize == zfindexMax())
        {
            continue;
        }
        ++condCountOffset;
        p += keySize + 1;
    } while(zftrue);
}
// ============================================================
static void _ZFP_ZFTextTemplateApply_indexData_reset(ZF_IN const ZFTextTemplateParam &param,
                                                     ZF_IN_OUT ZFCoreMap &stateMap,
                                                     ZF_IN const ZFOutput &output,
                                                     ZF_IN const zfchar *pEnd,
                                                     ZF_IN_OUT const zfchar *&data,
                                                     ZF_IN_OUT const zfchar *&p,
                                                     ZF_IN_OUT zfindex &size);
static void _ZFP_ZFTextTemplateApply_indexData(ZF_IN const ZFTextTemplateParam &param,
                                               ZF_IN_OUT ZFCoreMap &stateMap,
                                               ZF_IN const ZFOutput &output,
                                               ZF_IN const zfchar *pEnd,
                                               ZF_IN_OUT const zfchar *&data,
                                               ZF_IN_OUT const zfchar *&p,
                                               ZF_IN_OUT zfindex &size)
{ // {ZFTT_I_myKey}
    if(*p == 'R')
    {
        ++p;
        _ZFP_ZFTextTemplateApply_indexData_reset(param, stateMap, output, pEnd, data, p, size);
        return ;
    }
    if(*p != '_')
    {
        return ;
    }
    ++p;
    zfindex keySize = _ZFP_ZFTextTemplateApply_keyLength(pEnd, p);
    if(keySize == zfindexMax())
    {
        return ;
    }
    zfstring key(p, keySize);
    p += keySize + 1;

    zfstring value;
    {
        zfstring indexDataStateKey = zfstringWithFormat("indexData:%s", key.cString());
        _ZFP_ZFTextTemplateIndexDataState *indexDataState = stateMap.get<_ZFP_ZFTextTemplateIndexDataState *>(indexDataStateKey);
        if(indexDataState == zfnull)
        {
            indexDataState = zfnew(_ZFP_ZFTextTemplateIndexDataState);
            indexDataState->indexData = param.indexData(key);
            if(indexDataState->indexData == zfnull)
            {
                indexDataState->indexData = &(param.indexDataDefault());
            }

            indexDataState->indexCur = indexDataState->indexData->indexStart;

            stateMap.set(indexDataStateKey, ZFCorePointerForObject<_ZFP_ZFTextTemplateIndexDataState *>(indexDataState));
        }

        zfstring fmt;
        if(indexDataState->indexData->indexWidth == 0)
        {
            fmt = "%s";
        }
        else
        {
            switch(indexDataState->indexData->indexFlag)
            {
                case ZFTextTemplateIndexFlag::e_LeadingZero:
                    zfstringAppend(fmt, "%%0%zis", indexDataState->indexData->indexWidth);
                    break;
                case ZFTextTemplateIndexFlag::e_LeadingSpace:
                    zfstringAppend(fmt, "%%%zis", indexDataState->indexData->indexWidth);
                    break;
                case ZFTextTemplateIndexFlag::e_TailSpace:
                    zfstringAppend(fmt, "%%-%zis", indexDataState->indexData->indexWidth);
                    break;
                default:
                    zfCoreCriticalShouldNotGoHere();
                    return ;
            }
        }

        zfstringAppend(value, fmt.cString(), zfsFromInt(
                indexDataState->indexCur,
                indexDataState->indexData->indexRadix,
                indexDataState->indexData->indexUpperCase
            ).cString());

        zfCoreAssertWithMessage(indexDataState->indexData->indexOffset, "indexOffset must not be 0");
        indexDataState->indexCur += indexDataState->indexData->indexOffset;
    }

    if(output.callbackIsValid())
    {
        output.execute(value.cString(), value.length());
    }
    size += value.length();
    data = p;
}
static void _ZFP_ZFTextTemplateApply_indexData_reset(ZF_IN const ZFTextTemplateParam &param,
                                                     ZF_IN_OUT ZFCoreMap &stateMap,
                                                     ZF_IN const ZFOutput &output,
                                                     ZF_IN const zfchar *pEnd,
                                                     ZF_IN_OUT const zfchar *&data,
                                                     ZF_IN_OUT const zfchar *&p,
                                                     ZF_IN_OUT zfindex &size)
{ // {ZFTT_IR_myKey}
    if(*p != '_')
    {
        return ;
    }
    ++p;
    zfindex keySize = _ZFP_ZFTextTemplateApply_keyLength(pEnd, p);
    if(keySize == zfindexMax())
    {
        return ;
    }
    zfstring key(p, keySize);
    p += keySize + 1;
    data = p;

    zfstring indexDataStateKey = zfstringWithFormat("indexData:%s", key.cString());
    stateMap.remove(indexDataStateKey);
}

ZF_NAMESPACE_GLOBAL_END

