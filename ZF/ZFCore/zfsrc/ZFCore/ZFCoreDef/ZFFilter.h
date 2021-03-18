/**
 * @file ZFFilter.h
 * @brief filters in ZFFramework
 */

#ifndef _ZFI_ZFFilter_h_
#define _ZFI_ZFFilter_h_

#include "ZFCoreArray.h"
#include "ZFCoreSPrintf.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief filter type
 */
typedef enum {
    ZFFilterTypeInclude = 1,
    ZFFilterTypeExclude = 2,
} ZFFilterType;
/** @brief string tokens */
#define ZFTOKEN_ZFFilterTypeInclude "Include"
/** @brief string tokens */
#define ZFTOKEN_ZFFilterTypeExclude "Exclude"

/**
 * @brief filter result for custom filter callback
 */
typedef enum {
    ZFFilterCallbackResultNotSpecified,
    ZFFilterCallbackResultActive,
    ZFFilterCallbackResultNotActive,
} ZFFilterCallbackResult;
/** @brief string tokens */
#define ZFTOKEN_ZFFilterCallbackResultNotSpecified "NotSpecified"
/** @brief string tokens */
#define ZFTOKEN_ZFFilterCallbackResultActive "Active"
/** @brief string tokens */
#define ZFTOKEN_ZFFilterCallbackResultNotActive "NotActive"

/**
 * @brief declare a filter class
 *
 * since filter usually a template class,
 * it needs some trick to declare a filter
 * @code
 *   ZFFILTER_DECLARE(
 *       ZFM_EXPAND(ChildClass<T_Public, T_Internal>),
 *       ZFM_EXPAND(SuperClass<T_Public, T_Internal>))
 * @endcode
 */
#define ZFFILTER_DECLARE(ChildClass, SuperClass) \
    protected: \
        /** @brief class ref to super */ \
        typedef SuperClass zfsuper; \
        /** @brief class ref to self */ \
        typedef ChildClass zfself; \
    public:

// ============================================================
/** @brief filter data for #ZFFilterBase */
template<typename T_Element>
zfclassLikePOD ZF_ENV_EXPORT _ZFP_ZFFilterData
{
public:
    /** @brief filter data for #ZFFilterBase */
    T_Element element;
    /** @brief filter data for #ZFFilterBase */
    ZFFilterType filterType;
};

/**
 * @brief base class of filter
 */
template<typename T_Public, typename T_Internal = T_Public>
zfclassLikePOD ZF_ENV_EXPORT ZFFilterBase
{
protected:
    /** @brief typedef for self */
    typedef ZFFilterBase<T_Public, T_Internal> zfself;

public:
    /**
     * @brief custom filter callback for ZFFilter
     */
    typedef ZFFilterCallbackResult (*CustomFilterCallback)(ZF_IN T_Public const &e);

public:
    /**
     * @brief main constructor
     */
    ZFFilterBase(void)
    : _filters()
    , _customFilters(zfnull)
    {
    }
    /**
     * @brief construct from another filter, retain only, you should use #copyFrom to copy contents
     */
    ZFFilterBase(ZF_IN const ZFFilterBase<T_Public, T_Internal> &ref)
    : _filters(ref._filters)
    , _customFilters(zfnull)
    {
        if(ref._customFilters != zfnull)
        {
            this->_customFilters = zfnew(
                _CustomFilterCallbacksType,
                *ref._customFilters);
        }
    }
    /**
     * @brief retain only
     */
    virtual ZFFilterBase<T_Public, T_Internal> &operator = (ZF_IN const ZFFilterBase<T_Public, T_Internal> &ref)
    {
        if(this != &ref)
        {
            this->_filters = ref._filters;
            if(this->_customFilters != zfnull)
            {
                zfdelete(this->_customFilters);
                this->_customFilters = zfnull;
            }
            if(ref._customFilters != zfnull)
            {
                this->_customFilters = zfnew(
                    _CustomFilterCallbacksType,
                    *ref._customFilters);
            }
        }
        return *this;
    }
    /** @cond ZFPrivateDoc */
    zfbool operator == (ZF_IN const ZFFilterBase<T_Public, T_Internal> &ref) const
    {
        return this->_filters == ref._filters;
    }
    inline zfbool operator != (ZF_IN const ZFFilterBase<T_Public, T_Internal> &ref) const {return !this->operator == (ref);}
    /** @endcond */
    virtual ~ZFFilterBase(void)
    {
    }
public:
    /**
     * @brief copy from another filter
     */
    virtual void copyFrom(ZF_IN ZFFilterBase<T_Public, T_Internal> const &ref)
    {
        this->_filters.copyFrom(ref._filters);
        if(ref._customFilters != zfnull)
        {
            if(this->_customFilters == zfnull)
            {
                this->_customFilters = zfnew(_CustomFilterCallbacksType);
            }
            this->_customFilters->copyFrom(*ref._customFilters);
        }
        else
        {
            if(this->_customFilters != zfnull)
            {
                zfdelete(this->_customFilters);
            }
        }
    }

public:
    /**
     * @brief add a element
     *
     * note this method won't check duplicated data for performance
     */
    virtual ZFFilterBase<T_Public, T_Internal> &filterAdd(ZF_IN T_Public const &e,
                                                          ZF_IN_OPT ZFFilterType filterType = ZFFilterTypeExclude)
    {
        if(this->filterOnCheckValid(e))
        {
            _ZFP_ZFFilterData<T_Internal> filterData;
            this->filterOnStore(filterData.element, e);
            filterData.filterType = filterType;
            this->_filters.add(filterData);
        }
        return *this;
    }
    /**
     * @brief remove a element
     */
    virtual ZFFilterBase<T_Public, T_Internal> &filterRemove(ZF_IN T_Public const &e,
                                                             ZF_IN_OPT ZFFilterType filterType = ZFFilterTypeExclude)
    {
        if(this->filterOnCheckValid(e))
        {
            for(zfindex i = 0; i < this->_filters.count(); ++i)
            {
                if(this->filterOnCheckEqual(this->_filters.get(i).element, e)
                    && this->_filters.get(i).filterType == filterType)
                {
                    this->filterOnRemove(this->_filters.get(i).element);
                    this->_filters.remove(i);
                    break;
                }
            }
        }
        return *this;
    }
    /**
     * @brief remove a element
     */
    virtual ZFFilterBase<T_Public, T_Internal> &filterRemoveAtIndex(ZF_IN zfindex index)
    {
        this->filterOnRemove(this->_filters.get(index).element);
        this->_filters.remove(index);
        return *this;
    }
    /**
     * @brief remove all contents of this filter
     */
    virtual void filterRemoveAll(void)
    {
        for(zfindex i = 0; i < this->_filters.count(); ++i)
        {
            this->filterOnRemove(this->_filters.get(i).element);
        }
        this->_filters.removeAll();
        zfdelete(this->_customFilters);
        this->_customFilters = zfnull;
    }
    /**
     * @brief get count of element
     */
    virtual zfindex filterCount(void) const
    {
        return this->_filters.count();
    }
    /**
     * @brief get element at index
     */
    virtual T_Public filterElementAtIndex(ZF_IN zfindex index) const
    {
        T_Public t;
        this->filterOnAccess(t, this->_filters.get(index).element);
        return t;
    }
    /**
     * @brief get filter data at index
     */
    virtual T_Internal const &filterInternalAtIndex(ZF_IN zfindex index) const
    {
        return this->_filters.get(index).element;
    }
    /**
     * @brief get filter type for filter data at index
     */
    virtual ZFFilterType filterTypeAtIndex(ZF_IN zfindex index) const
    {
        return this->_filters.get(index).filterType;
    }
    /**
     * @brief add a custom filter callback
     */
    virtual ZFFilterBase<T_Public, T_Internal> &customFilterCallbackAdd(ZF_IN typename ZFFilterBase<T_Public, T_Internal>::CustomFilterCallback customFilterCallback)
    {
        if(this->_customFilters == zfnull)
        {
            this->_customFilters = zfnew(_CustomFilterCallbacksType);
        }
        this->_customFilters->add(customFilterCallback);
        return *this;
    }
    /**
     * @brief remove custom filter callback
     */
    virtual ZFFilterBase<T_Public, T_Internal> &customFilterCallbackRemove(ZF_IN typename ZFFilterBase<T_Public, T_Internal>::CustomFilterCallback customFilterCallback)
    {
        if(this->_customFilters != zfnull)
        {
            for(zfindex i = 0; i < this->_customFilters->count(); ++i)
            {
                if(this->_customFilters->get(i) == customFilterCallback)
                {
                    this->_customFilters->remove(i);
                    break;
                }
            }
        }
        return *this;
    }
    /**
     * @brief remove custom filter callback
     */
    virtual ZFFilterBase<T_Public, T_Internal> &customFilterCallbackRemove(ZF_IN zfindex index)
    {
        if(this->_customFilters != zfnull)
        {
            this->_customFilters->remove(index);
        }
        return *this;
    }
    /**
     * @brief get custom filter callback count
     */
    virtual zfindex customFilterCallbackCount(void) const
    {
        if(this->_customFilters != zfnull)
        {
            return this->_customFilters->count();
        }
        return 0;
    }
    /**
     * @brief get custom filter callback
     */
    virtual typename ZFFilterBase<T_Public, T_Internal>::CustomFilterCallback customFilterCallbackAtIndex(ZF_IN zfindex index) const
    {
        return this->_customFilters->get(index);
    }
    /**
     * @brief return true if e is not filtered by this filter, see #ZFFilterType
     */
    virtual zfbool filterCheckActive(ZF_IN T_Public const &e) const
    {
        if(!this->filterOnCheckValid(e))
        {
            return zffalse;
        }
        if(this->_customFilters != zfnull)
        {
            for(zfindex i = 0; i < this->_customFilters->count(); ++i)
            {
                switch(this->_customFilters->get(i)(e))
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
        }
        return this->filterOnCheckActive(e);
    }

private:
    static void _ZFP_ZFFilterBase_contentInfoGetter(ZF_IN_OUT zfstring &ret, ZF_IN const _ZFP_ZFFilterData<T_Internal> &v)
    {
        ret += '(';
        switch(v.filterType)
        {
            case ZFFilterTypeInclude:
                ret += "include ";
                break;
            case ZFFilterTypeExclude:
                ret += "exclude ";
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                return ;
        }
        zfstringAppend(ret, "%p", &(v.element));
        ret += ')';
    }
public:
    /** @brief see #objectInfo */
    virtual void objectInfoT(ZF_IN_OUT zfstring &ret) const
    {
        this->_filters.objectInfoOfContentT(ret,
            5, // max count
            ZFTokenForContainerDefault(),
            _ZFP_ZFFilterBase_contentInfoGetter);
    }
    /** @brief return object info */
    virtual inline zfstring objectInfo(void) const
    {
        zfstring ret;
        this->objectInfoT(ret);
        return ret;
    }

protected:
    /**
     * @brief return true if element is valid for a filter
     *
     * if false, won't be added to content by #filterAdd or compared by #filterOnCheckEqual
     */
    virtual zfbool filterOnCheckValid(ZF_IN T_Public const &e) const
    {
        return zftrue;
    }
    /**
     * @brief store public type to internal storage type
     */
    virtual void filterOnStore(ZF_IN_OUT T_Internal &to,
                               ZF_IN T_Public const &from) const zfpurevirtual;
    /**
     * @brief called before value removed from filter
     */
    virtual void filterOnRemove(ZF_IN_OUT T_Internal &value) const {}
    /**
     * @brief access public type from internal storage type
     */
    virtual void filterOnAccess(ZF_IN_OUT T_Public &to,
                                ZF_IN T_Internal const &from) const zfpurevirtual;
    /**
     * @brief true if e1 is regarded as equal to e2
     *
     * subclass may override this to supply custom comparation
     */
    virtual zfbool filterOnCheckEqual(ZF_IN T_Internal const &e1,
                                      ZF_IN T_Public const &e2) const zfpurevirtual;
    /**
     * @brief called by #filterCheckActive to check whether the element is filtered out
     */
    virtual zfbool filterOnCheckActive(ZF_IN T_Public const &e) const
    {
        zfbool hasIncludeMode = zffalse;
        zfbool included = zffalse;
        for(zfindex i = 0; i < this->_filters.count(); ++i)
        {
            const _ZFP_ZFFilterData<T_Internal> &filter = this->_filters.get(i);
            switch(filter.filterType)
            {
                case ZFFilterTypeInclude:
                    hasIncludeMode = zftrue;
                    if(this->filterOnCheckEqual(filter.element, e))
                    {
                        included = zftrue;
                    }
                    break;
                case ZFFilterTypeExclude:
                    if(this->filterOnCheckEqual(filter.element, e))
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

private:
    typedef ZFCoreArray<_ZFP_ZFFilterData<T_Internal> > _FiltersType;
    typedef ZFCoreArrayPOD<typename ZFFilterBase<T_Public, T_Internal>::CustomFilterCallback> _CustomFilterCallbacksType;
    _FiltersType _filters;
    _CustomFilterCallbacksType *_customFilters;
};

// ============================================================
/**
 * @brief basic filter
 *
 * a basic filter is:
 * -  support operator = to assign value between public type and internal storage type
 * -  support operator == to compare whether public type is equal to internal storage type
 */
template<typename T_Public, typename T_Internal = T_Public>
zfclassLikePOD ZF_ENV_EXPORT ZFFilterBasic : zfextendsLikePOD ZFFilterBase<T_Public, T_Internal>
{
    ZFFILTER_DECLARE(
        ZFM_EXPAND(ZFFilterBasic<T_Public, T_Internal>),
        ZFM_EXPAND(ZFFilterBase<T_Public, T_Internal>))

public:
    /**
     * @brief see #ZFFilterBasic
     */
    ZFFilterBasic(void)
    : zfsuper()
    {
    }
    /**
     * @brief see #ZFFilterBasic
     */
    ZFFilterBasic(ZF_IN const ZFFilterBasic<T_Public, T_Internal> &ref)
    : zfsuper(ref)
    {
    }
    /** @cond ZFPrivateDoc */
    virtual ZFFilterBasic<T_Public, T_Internal> &operator = (ZF_IN const ZFFilterBasic<T_Public, T_Internal> &ref)
    {
        zfsuper::operator = (ref);
        return *this;
    }
    /** @endcond */
    virtual ~ZFFilterBasic(void) {}

protected:
    /** @cond ZFPrivateDoc */
    zfoverride
    virtual void filterOnStore(ZF_IN_OUT T_Internal &to,
                               ZF_IN T_Public const &from) const
    {
        to = from;
    }
    zfoverride
    virtual void filterOnAccess(ZF_IN_OUT T_Public &to,
                                ZF_IN T_Internal const &from) const
    {
        to = from;
    }
    zfoverride
    virtual zfbool filterOnCheckEqual(ZF_IN T_Internal const &e1,
                                      ZF_IN T_Public const &e2) const
    {
        return (e1 == e2);
    }
    /** @endcond */
};

/**
 * @brief number filter with zfint as filter type
 */
typedef ZFFilterBasic<zfint> ZFFilterForNumber;

/**
 * @brief number filter with zfindex as filter type
 */
typedef ZFFilterBasic<zfindex> ZFFilterForIndex;

/**
 * @brief number filter with zfidentity as filter type
 */
typedef ZFFilterBasic<zfidentity> ZFFilterForIdentity;

// ============================================================
/**
 * @brief base class of string filter
 */
template<typename T_Public, typename T_Internal>
zfclassLikePOD ZF_ENV_EXPORT ZFFilterForStringBase : zfextendsLikePOD ZFFilterBase<T_Public, T_Internal>
{
    ZFFILTER_DECLARE(
        ZFM_EXPAND(ZFFilterForStringBase<T_Public, T_Internal>),
        ZFM_EXPAND(ZFFilterBase<T_Public, T_Internal>))

public:
    /**
     * @brief main constructor
     */
    ZFFilterForStringBase(void)
    : zfsuper()
    {
    }
    /**
     * @brief see #ZFFilterBase
     */
    ZFFilterForStringBase(ZF_IN const ZFFilterForStringBase<T_Public, T_Internal> &ref)
    : zfsuper(ref)
    {
    }
    /** @cond ZFPrivateDoc */
    virtual ZFFilterForStringBase<T_Public, T_Internal> &operator = (ZF_IN const ZFFilterForStringBase<T_Public, T_Internal> &ref)
    {
        zfsuper::operator = (ref);
        return *this;
    }
    /** @endcond */
    virtual ~ZFFilterForStringBase(void) {}

protected:
    zfoverride
    virtual zfbool filterOnCheckValid(ZF_IN T_Public const &e) const
    {
        return (e != zfnull);
    }
    zfoverride
    virtual void filterOnStore(ZF_IN_OUT T_Internal &to,
                               ZF_IN T_Public const &from) const
    {
        to = from;
    }
    zfoverride
    virtual void filterOnAccess(ZF_IN_OUT T_Public &to,
                                ZF_IN T_Internal const &from) const
    {
        to = from.cString();
    }
    zfoverride
    virtual zfbool filterOnCheckEqual(ZF_IN T_Internal const &e1,
                                      ZF_IN T_Public const &e2) const
    {
        return (e1.compare(e2) == 0);
    }
};

/**
 * @brief string filter with const zfchar * as public type and zfstring as internal storage type
 */
typedef ZFFilterForStringBase<const zfchar *, zfstring> ZFFilterForString;

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFFilter_h_

