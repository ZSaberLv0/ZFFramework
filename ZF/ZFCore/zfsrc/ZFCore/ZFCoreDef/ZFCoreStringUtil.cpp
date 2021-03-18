#include "ZFCoreStringUtil.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfindex zfstringFind(ZF_IN const zfchar *src, ZF_IN zfindex srcLen, ZF_IN const zfchar *find, ZF_IN_OPT zfindex findLen /* = zfindexMax() */)
{
    if(src && find)
    {
        if(srcLen == zfindexMax())
        {
            srcLen = zfslen(src);
        }
        if(findLen == zfindexMax())
        {
            findLen = zfslen(find);
        }
        if(srcLen >= findLen)
        {
            const zfchar *p = src;
            const zfchar *pEnd = src + srcLen - findLen;
            do
            {
                if(zfsncmp(p, find, findLen) == 0)
                {
                    return (p - src);
                }
                zfcharMoveNext(p);
            } while(p <= pEnd);
        }
    }
    return zfindexMax();
}
zfindex zfstringFindReversely(ZF_IN const zfchar *src, ZF_IN zfindex srcLen, ZF_IN const zfchar *find, ZF_IN_OPT zfindex findLen /* = zfindexMax() */)
{
    if(src && find)
    {
        if(srcLen == zfindexMax())
        {
            srcLen = zfslen(src);
        }
        if(findLen == zfindexMax())
        {
            findLen = zfslen(find);
        }
        if(srcLen >= findLen)
        {
            const zfchar *p = src + srcLen - findLen;
            const zfchar *pEnd = src;
            do
            {
                if(zfsncmp(p, find, findLen) == 0)
                {
                    return (p - src);
                }
                --p;
            } while(p >= pEnd);
        }
    }
    return zfindexMax();
}
zfindex zfstringFindCaseInsensitive(ZF_IN const zfchar *src, ZF_IN zfindex srcLen, ZF_IN const zfchar *find, ZF_IN_OPT zfindex findLen /* = zfindexMax() */)
{
    if(src && find)
    {
        if(srcLen == zfindexMax())
        {
            srcLen = zfslen(src);
        }
        if(findLen == zfindexMax())
        {
            findLen = zfslen(find);
        }
        if(srcLen >= findLen)
        {
            const zfchar *p = src;
            const zfchar *pEnd = src + srcLen - findLen;
            do
            {
                if(zfsnicmp(p, find, findLen) == 0)
                {
                    return (p - src);
                }
                zfcharMoveNext(p);
            } while(p <= pEnd);
        }
    }
    return zfindexMax();
}
zfindex zfstringFindCaseInsensitiveReversely(ZF_IN const zfchar *src, ZF_IN zfindex srcLen, ZF_IN const zfchar *find, ZF_IN_OPT zfindex findLen /* = zfindexMax() */)
{
    if(src && find)
    {
        if(srcLen == zfindexMax())
        {
            srcLen = zfslen(src);
        }
        if(findLen == zfindexMax())
        {
            findLen = zfslen(find);
        }
        if(srcLen >= findLen)
        {
            const zfchar *p = src + srcLen - findLen;
            const zfchar *pEnd = src;
            do
            {
                if(zfsnicmp(p, find, findLen) == 0)
                {
                    return (p - src);
                }
                --p;
            } while(p >= pEnd);
        }
    }
    return zfindexMax();
}
zfindex zfstringFindFirstOf(ZF_IN const zfchar *src, ZF_IN zfindex srcLen, ZF_IN const zfchar *find, ZF_IN_OPT zfindex findLen /* = zfindexMax() */)
{
    if(src && find)
    {
        if(srcLen == zfindexMax())
        {
            srcLen = zfslen(src);
        }
        if(findLen == zfindexMax())
        {
            findLen = zfslen(find);
        }
        const zfchar *p = src;
        const zfchar *pEnd = src + srcLen;
        while(p < pEnd)
        {
            for(zfindex i = 0; i < findLen; ++i)
            {
                if(*p == find[i])
                {
                    return (p - src);
                }
            }
            zfcharMoveNext(p);
        }
    }
    return zfindexMax();
}
zfindex zfstringFindFirstNotOf(ZF_IN const zfchar *src, ZF_IN zfindex srcLen, ZF_IN const zfchar *find, ZF_IN_OPT zfindex findLen /* = zfindexMax() */)
{
    if(src && find)
    {
        if(srcLen == zfindexMax())
        {
            srcLen = zfslen(src);
        }
        if(findLen == zfindexMax())
        {
            findLen = zfslen(find);
        }
        const zfchar *p = src;
        const zfchar *pEnd = src + srcLen;
        while(p < pEnd)
        {
            zfindex i = 0;
            for( ; i < findLen; ++i)
            {
                if(*p == find[i])
                {
                    break;
                }
            }
            if(i >= findLen)
            {
                return (p - src);
            }
            zfcharMoveNext(p);
        }
    }
    return zfindexMax();
}
zfindex zfstringFindLastOf(ZF_IN const zfchar *src, ZF_IN zfindex srcLen, ZF_IN const zfchar *find, ZF_IN_OPT zfindex findLen /* = zfindexMax() */)
{
    if(src && find)
    {
        if(srcLen == zfindexMax())
        {
            srcLen = zfslen(src);
        }
        if(findLen == zfindexMax())
        {
            findLen = zfslen(find);
        }
        const zfchar *p = src + srcLen - 1;
        const zfchar *pEnd = src - 1;
        while(p > pEnd)
        {
            for(zfindex i = 0; i < findLen; ++i)
            {
                if(*p == find[i])
                {
                    return (p - src);
                }
            }
            --p;
        }
    }
    return zfindexMax();
}
zfindex zfstringFindLastNotOf(ZF_IN const zfchar *src, ZF_IN zfindex srcLen, ZF_IN const zfchar *find, ZF_IN_OPT zfindex findLen /* = zfindexMax() */)
{
    if(src && find)
    {
        if(srcLen == zfindexMax())
        {
            srcLen = zfslen(src);
        }
        if(findLen == zfindexMax())
        {
            findLen = zfslen(find);
        }
        const zfchar *p = src + srcLen - 1;
        const zfchar *pEnd = src - 1;
        while(p > pEnd)
        {
            zfindex i = 0;
            for( ; i < findLen; ++i)
            {
                if(*p == find[i])
                {
                    break;
                }
            }
            if(i >= findLen)
            {
                return (p - src);
            }
            --p;
        }
    }
    return zfindexMax();
}
zfindex zfstringReplace(ZF_IN_OUT zfstring &s, ZF_IN const zfchar *replaceFrom, ZF_IN const zfchar *replaceTo, ZF_IN_OPT zfindex maxCount /* = zfindexMax() */)
{
    zfindex replacedCount = 0;
    zfindex replaceFromLen = zfslen(replaceFrom);
    zfindex replaceToLen = zfslen(replaceTo);
    zfindex pos = 0;
    while(maxCount == zfindexMax() || replacedCount < maxCount)
    {
        pos = zfstringFind(s.cString() + pos, s.length() - pos, replaceFrom, replaceFromLen);
        if(pos == zfindexMax()) {break;}
        s.replace(pos, replaceFromLen, replaceTo);
        pos += replaceToLen;
        ++replacedCount;
    }
    return replacedCount;
}
zfindex zfstringReplaceReversely(ZF_IN_OUT zfstring &s, ZF_IN const zfchar *replaceFrom, ZF_IN const zfchar *replaceTo, ZF_IN_OPT zfindex maxCount /* = zfindexMax() */)
{
    zfindex replacedCount = 0;
    zfindex replaceFromLen = zfslen(replaceFrom);
    zfindex pos = s.length();
    while(maxCount == zfindexMax() || replacedCount < maxCount)
    {
        pos = zfstringFindReversely(s.cString(), pos, replaceFrom, replaceFromLen);
        if(pos == zfindexMax()) {break;}
        s.replace(pos, replaceFromLen, replaceTo);
        ++replacedCount;
    }
    return replacedCount;
}

ZF_NAMESPACE_GLOBAL_END

