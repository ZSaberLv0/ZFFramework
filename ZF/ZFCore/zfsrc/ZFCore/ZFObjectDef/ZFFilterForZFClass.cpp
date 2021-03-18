#include "ZFFilterForZFClass.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFEXPORT_ENUM_DEFINE(ZFFilterForZFClassType
    , ZFFilterForZFClassTypeInclude
    , ZFFilterForZFClassTypeExclude
    , ZFFilterForZFClassTypeIncludeChildOf
    , ZFFilterForZFClassTypeIncludeParentOf
    , ZFFilterForZFClassTypeExcludeChildOf
    , ZFFilterForZFClassTypeExcludeParentOf
    )

// ============================================================
ZFFilterForZFClass::ZFFilterForZFClass(void)
: _ZFP_ZFFilterForZFClass_filters()
, _ZFP_ZFFilterForZFClass_customFilterCallbacks()
{
}
ZFFilterForZFClass::ZFFilterForZFClass(ZF_IN const ZFFilterForZFClass &ref)
: _ZFP_ZFFilterForZFClass_filters(ref._ZFP_ZFFilterForZFClass_filters)
, _ZFP_ZFFilterForZFClass_customFilterCallbacks(ref._ZFP_ZFFilterForZFClass_customFilterCallbacks)
{
}
ZFFilterForZFClass &ZFFilterForZFClass::operator = (ZF_IN const ZFFilterForZFClass &ref)
{
    this->_ZFP_ZFFilterForZFClass_filters = ref._ZFP_ZFFilterForZFClass_filters;
    this->_ZFP_ZFFilterForZFClass_customFilterCallbacks = ref._ZFP_ZFFilterForZFClass_customFilterCallbacks;
    return *this;
}
zfbool ZFFilterForZFClass::operator == (ZF_IN const ZFFilterForZFClass &ref) const
{
    return this->_ZFP_ZFFilterForZFClass_filters == ref._ZFP_ZFFilterForZFClass_filters;
}
ZFFilterForZFClass::~ZFFilterForZFClass(void)
{
}
void ZFFilterForZFClass::copyFrom(ZF_IN ZFFilterForZFClass const &ref)
{
    this->_ZFP_ZFFilterForZFClass_filters.copyFrom(ref._ZFP_ZFFilterForZFClass_filters);
    this->_ZFP_ZFFilterForZFClass_customFilterCallbacks.copyFrom(ref._ZFP_ZFFilterForZFClass_customFilterCallbacks);
}

ZFFilterForZFClass &ZFFilterForZFClass::filterAdd(ZF_IN const ZFClass * const &e,
                                                  ZF_IN_OPT ZFFilterForZFClassType filterType /* = ZFFilterForZFClassTypeExclude */)
{
    _ZFP_ZFFilterForZFClassData filterData;
    filterData.filterClass = e;
    filterData.filterType = filterType;
    this->_ZFP_ZFFilterForZFClass_filters.add(filterData);
    return *this;
}
ZFFilterForZFClass &ZFFilterForZFClass::filterRemove(ZF_IN const ZFClass * const &e,
                                                     ZF_IN_OPT ZFFilterForZFClassType filterType /* = ZFFilterForZFClassTypeExclude */)
{
    for(zfindex i = 0; i < this->_ZFP_ZFFilterForZFClass_filters.count(); ++i)
    {
        if(this->_ZFP_ZFFilterForZFClass_filters.get(i).filterClass == e && this->_ZFP_ZFFilterForZFClass_filters.get(i).filterType == filterType)
        {
            this->_ZFP_ZFFilterForZFClass_filters.remove(i);
            break;
        }
    }
    return *this;
}
ZFFilterForZFClass &ZFFilterForZFClass::filterRemoveAtIndex(ZF_IN zfindex index)
{
    this->_ZFP_ZFFilterForZFClass_filters.remove(index);
    return *this;
}
ZFFilterForZFClass &ZFFilterForZFClass::filterRemoveAll(void)
{
    this->_ZFP_ZFFilterForZFClass_filters.removeAll();
    return *this;
}

zfindex ZFFilterForZFClass::filterCount(void) const
{
    return this->_ZFP_ZFFilterForZFClass_filters.count();
}
const ZFClass *ZFFilterForZFClass::filterElementAtIndex(ZF_IN zfindex index) const
{
    return this->_ZFP_ZFFilterForZFClass_filters.get(index).filterClass;
}
ZFFilterForZFClassType ZFFilterForZFClass::filterTypeAtIndex(ZF_IN zfindex index) const
{
    return this->_ZFP_ZFFilterForZFClass_filters.get(index).filterType;
}

ZFFilterForZFClass &ZFFilterForZFClass::customFilterCallbackAdd(ZF_IN ZFFilterForZFClass::CustomFilterCallback customFilterCallback)
{
    this->_ZFP_ZFFilterForZFClass_customFilterCallbacks.add(customFilterCallback);
    return *this;
}
ZFFilterForZFClass &ZFFilterForZFClass::customFilterCallbackRemove(ZF_IN ZFFilterForZFClass::CustomFilterCallback customFilterCallback)
{
    this->_ZFP_ZFFilterForZFClass_customFilterCallbacks.removeElement(customFilterCallback);
    return *this;
}
ZFFilterForZFClass &ZFFilterForZFClass::customFilterCallbackRemove(ZF_IN zfindex index)
{
    this->_ZFP_ZFFilterForZFClass_customFilterCallbacks.remove(index);
    return *this;
}
zfindex ZFFilterForZFClass::customFilterCallbackCount(void) const
{
    return this->_ZFP_ZFFilterForZFClass_customFilterCallbacks.count();
}
ZFFilterForZFClass::CustomFilterCallback ZFFilterForZFClass::customFilterCallbackAtIndex(ZF_IN zfindex index) const
{
    return this->_ZFP_ZFFilterForZFClass_customFilterCallbacks.get(index);
}

zfbool ZFFilterForZFClass::filterCheckActive(ZF_IN const ZFClass * const &e) const
{
    for(zfindex i = 0, count = this->_ZFP_ZFFilterForZFClass_customFilterCallbacks.count(); i < count; ++i)
    {
        switch(this->_ZFP_ZFFilterForZFClass_customFilterCallbacks.get(i)(e))
        {
            case ZFFilterCallbackResultNotSpecified:
                break;
            case ZFFilterCallbackResultActive:
                return zftrue;
            case ZFFilterCallbackResultNotActive:
                return zffalse;
            default:
                zfCoreCriticalShouldNotGoHere();
                break;
        }
    }

    zfbool hasIncludeMode = zffalse;
    zfbool included = zffalse;
    for(zfindex i = 0, count = this->_ZFP_ZFFilterForZFClass_filters.count(); i < count; ++i)
    {
        const _ZFP_ZFFilterForZFClassData &filter = this->_ZFP_ZFFilterForZFClass_filters.get(i);
        switch(filter.filterType)
        {
            case ZFFilterForZFClassTypeInclude:
                hasIncludeMode = zftrue;
                if(filter.filterClass == e)
                {
                    included = zftrue;
                }
                break;
            case ZFFilterForZFClassTypeExclude:
                if(filter.filterClass == e)
                {
                    return zffalse;
                }
                break;
            case ZFFilterForZFClassTypeIncludeChildOf:
                if(e == zfnull || !e->classIsTypeOf(filter.filterClass))
                {
                    return zffalse;
                }
                break;
            case ZFFilterForZFClassTypeIncludeParentOf:
                if(e == zfnull || !filter.filterClass->classIsTypeOf(e))
                {
                    return zffalse;
                }
                break;
            case ZFFilterForZFClassTypeExcludeChildOf:
                if(e != zfnull && e->classIsTypeOf(filter.filterClass))
                {
                    return zffalse;
                }
                break;
            case ZFFilterForZFClassTypeExcludeParentOf:
                if(e != zfnull && filter.filterClass->classIsTypeOf(e))
                {
                    return zffalse;
                }
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                return zffalse;
        }
    }
    return (!hasIncludeMode || included);
}

static void _ZFP_ZFFilterForZFClass_contentInfoGetter(ZF_IN_OUT zfstring &ret, ZF_IN const _ZFP_ZFFilterForZFClassData &v)
{
    ret += '(';
    ZFFilterForZFClassTypeToString(ret, v.filterType);
    ret += ' ';
    ret += v.filterClass->classNameFull();
    ret += ')';
}
void ZFFilterForZFClass::objectInfoT(ZF_IN_OUT zfstring &ret) const
{
    this->_ZFP_ZFFilterForZFClass_filters.objectInfoOfContentT(ret,
        zfHint("max count")5,
        ZFTokenForContainerDefault(),
        _ZFP_ZFFilterForZFClass_contentInfoGetter);
}

ZF_NAMESPACE_GLOBAL_END

