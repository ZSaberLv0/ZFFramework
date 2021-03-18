#include "../ZFCoreDef.h"

#include "../ZFSTLWrapper/zfstl_map.h"
#include "../ZFSTLWrapper/zfstl_string.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
const zfchar *ZFNamespaceSkipGlobal(ZF_IN const zfchar *ns)
{
    static zfstring namePrefix(ZF_NAMESPACE_GLOBAL_NAME);
    static zfstring abbrNamePrefix(ZF_NAMESPACE_GLOBAL_ABBR_NAME);
    if(zfsIsEmpty(ns))
    {
        return zfnull;
    }
    else if(zfsncmp(ns, ZFNamespaceSeparator(), ZFNamespaceSeparatorLen()) == 0)
    {
        if(ns[ZFNamespaceSeparatorLen()] == '\0')
        {
            return zfnull;
        }
        else
        {
            return ns + ZFNamespaceSeparatorLen();
        }
    }
    else if(zfsncmp(ns, namePrefix.cString(), namePrefix.length()) == 0)
    {
        if(ns[namePrefix.length()] == '\0')
        {
            return zfnull;
        }
        else if(zfsncmp(ns + namePrefix.length(), ZFNamespaceSeparator(), ZFNamespaceSeparatorLen()) == 0)
        {
            if(ns[namePrefix.length() + ZFNamespaceSeparatorLen()] == '\0')
            {
                return zfnull;
            }
            else
            {
                return ns + namePrefix.length() + ZFNamespaceSeparatorLen();
            }
        }
        else
        {
            return ns;
        }
    }
    else if(zfsncmp(ns, abbrNamePrefix.cString(), abbrNamePrefix.length()) == 0)
    {
        if(ns[abbrNamePrefix.length()] == '\0')
        {
            return zfnull;
        }
        else if(zfsncmp(ns + abbrNamePrefix.length(), ZFNamespaceSeparator(), ZFNamespaceSeparatorLen()) == 0)
        {
            if(ns[abbrNamePrefix.length() + ZFNamespaceSeparatorLen()] == '\0')
            {
                return zfnull;
            }
            else
            {
                return ns + abbrNamePrefix.length() + ZFNamespaceSeparatorLen();
            }
        }
        else
        {
            return ns;
        }
    }
    else
    {
        return ns;
    }
}

zfbool ZFNamespaceSplit(ZF_IN_OUT ZFCoreArray<ZFIndexRange> &ret,
                        ZF_IN const zfchar *src,
                        ZF_IN_OPT zfindex srcLen /* = zfindexMax() */)
{
    if(src == zfnull)
    {
        ret.add(ZFIndexRangeZero());
        return zftrue;
    }
    const zfchar *pL = src;
    const zfchar *p = src;
    const zfchar *pEnd = src + (srcLen == zfindexMax() ? zfslen(src) : srcLen);
    zfbool hasAdd = zffalse;
    while(p < pEnd)
    {
        if(zfsncmp(pL, ZFNamespaceSeparator(), ZFNamespaceSeparatorLen()) == 0)
        {
            if(p > pL)
            {
                ret.add(ZFIndexRangeMake(pL - src, p - pL));
            }
            p = pL = p + ZFNamespaceSeparatorLen();
            hasAdd = zftrue;
            continue;
        }
        else
        {
            ++p;
        }
    }
    if(p > pL)
    {
        ret.add(ZFIndexRangeMake(pL - src, p - pL));
    }
    else
    {
        if(!hasAdd)
        {
            ret.add(ZFIndexRangeZero());
        }
    }
    return zftrue;
}

// ============================================================
zfclassLikePOD _ZFP_ZFNamespaceMapType
{
public:
    zfstring ns;
    zfstlmap<zfstlstringZ, ZFCorePointerForObject<_ZFP_ZFNamespaceMapType *> > d;
};

static _ZFP_ZFNamespaceMapType &_ZFP_ZFNamespaceMap(void)
{
    static _ZFP_ZFNamespaceMapType d;
    return d;
}
zfstring _ZFP_ZFNamespaceRegister(ZF_IN const char *parent,
                                  ZF_IN const char *child)
{
    zfCoreMutexLocker();
    zfstring ns;
    if(parent != zfnull)
    {
        ns += parent;
        ns += ZFNamespaceSeparator();
    }
    ns += child;
    ZFCoreArrayPOD<ZFIndexRange> pos;
    ZFNamespaceSplit(pos, ns.cString(), ns.length());
    _ZFP_ZFNamespaceMapType *t = &_ZFP_ZFNamespaceMap();
    for(zfindex i = 0; i < pos.count(); ++i)
    {
        zfstlstringZ key(ns.cString() + pos[i].start, pos[i].count);
        zfstlmap<zfstlstringZ, ZFCorePointerForObject<_ZFP_ZFNamespaceMapType *> >::iterator it = t->d.find(key);
        if(it == t->d.end())
        {
            _ZFP_ZFNamespaceMapType *tNew = zfnew(_ZFP_ZFNamespaceMapType);
            t->d[key] = ZFCorePointerForObject<_ZFP_ZFNamespaceMapType *>(tNew);
            t = tNew;
            t->ns.append(ns, pos[i].start + pos[i].count);
        }
        else
        {
            t = it->second.pointerValue();
        }
    }
    return ns;
}
void _ZFP_ZFNamespaceUnregister(ZF_IN const char *ns)
{
    zfCoreMutexLocker();
    zfstring nsTmp = ns;
    ZFCoreArrayPOD<ZFIndexRange> pos;
    ZFNamespaceSplit(pos, nsTmp.cString(), nsTmp.length());
    if(pos.count() < 1)
    {
        return ;
    }
    _ZFP_ZFNamespaceMapType *t = &_ZFP_ZFNamespaceMap();
    for(zfindex i = 0; i < pos.count() - 1; ++i)
    {
        zfstlstringZ key(nsTmp.cString() + pos[i].start, pos[i].count);
        zfstlmap<zfstlstringZ, ZFCorePointerForObject<_ZFP_ZFNamespaceMapType *> >::iterator it = t->d.find(key);
        if(it == t->d.end())
        {
            t = zfnull;
            break;
        }
        else
        {
            t = it->second.pointerValue();
        }
    }
    if(t != zfnull)
    {
        t->d.erase(zfstlstringZ(nsTmp.cString() + pos[pos.count() - 1].start));
    }
}

void ZFNamespaceGetAllT(ZF_IN_OUT ZFCoreArray<const zfchar *> &ret)
{
    zfCoreMutexLocker();
    ZFCoreQueuePOD<_ZFP_ZFNamespaceMapType *> toCheck;
    toCheck.queuePut(&_ZFP_ZFNamespaceMap());
    do
    {
        _ZFP_ZFNamespaceMapType *t = toCheck.queueTake();
        if(!t->ns.isEmpty())
        {
            ret.add(t->ns);
        }
        for(zfstlmap<zfstlstringZ, ZFCorePointerForObject<_ZFP_ZFNamespaceMapType *> >::iterator it = t->d.begin(); it != t->d.end(); ++it)
        {
            toCheck.queuePut(it->second.pointerValue());
        }
    } while(!toCheck.isEmpty());
}

void ZFNamespaceGetAllT(ZF_IN_OUT ZFCoreArray<const zfchar *> &ret,
                        ZF_IN const zfchar *parent,
                        ZF_IN_OPT zfbool recursive /* = zffalse */)
{
    zfCoreMutexLocker();
    parent = ZFNamespaceSkipGlobal(parent);
    _ZFP_ZFNamespaceMapType *t = &_ZFP_ZFNamespaceMap();
    if(parent != zfnull)
    {
        ZFCoreArrayPOD<ZFIndexRange> pos;
        ZFNamespaceSplit(pos, parent);
        for(zfindex i = 0; i < pos.count(); ++i)
        {
            zfstlstringZ key(parent + pos[i].start, pos[i].count);
            zfstlmap<zfstlstringZ, ZFCorePointerForObject<_ZFP_ZFNamespaceMapType *> >::iterator it = t->d.find(key);
            if(it == t->d.end())
            {
                return ;
            }
            else
            {
                t = it->second.pointerValue();
            }
        }
    }

    ZFCoreQueuePOD<_ZFP_ZFNamespaceMapType *> toCheck;
    for(zfstlmap<zfstlstringZ, ZFCorePointerForObject<_ZFP_ZFNamespaceMapType *> >::iterator it = t->d.begin(); it != t->d.end(); ++it)
    {
        toCheck.queuePut(it->second.pointerValue());
    }
    do
    {
        _ZFP_ZFNamespaceMapType *t = toCheck.queueTake();
        if(!t->ns.isEmpty())
        {
            ret.add(t->ns);
        }
        if(recursive)
        {
            for(zfstlmap<zfstlstringZ, ZFCorePointerForObject<_ZFP_ZFNamespaceMapType *> >::iterator it = t->d.begin(); it != t->d.end(); ++it)
            {
                toCheck.queuePut(it->second.pointerValue());
            }
        }
    } while(!toCheck.isEmpty());
}

ZF_NAMESPACE_GLOBAL_END

