/**
 * @file ZFDynamicInvoker.h
 * @brief dynamic invoker util for advanced dynamic impl
 */

#ifndef _ZFI_ZFDynamicInvoker_h_
#define _ZFI_ZFDynamicInvoker_h_

#include "ZFObjectCore.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief wrapper for unknown types for #ZFDI_invoke
 */
zfabstract ZF_ENV_EXPORT ZFDI_WrapperBase : zfextends ZFObject
{
    ZFOBJECT_DECLARE_ABSTRACT(ZFDI_WrapperBase, ZFObject)
    ZFALLOC_CACHE_RELEASE_ABSTRACT({
        cache->zfv(zfnull);
    })

public:
    /**
     * @brief the data
     *
     * the data must store strings that can be converted by:
     * -  #ZFTypeIdWrapper::wrappedValueFromString
     * -  #ZFSerializeFromString
     */
    virtual void zfv(ZF_IN const zfchar *zfv) zfpurevirtual;
    /** @brief see #zfv */
    virtual const zfchar *zfv(void) zfpurevirtual;

public:
    zfoverride
    virtual zfidentity objectHash(void)
    {
        return zfidentityCalcString(this->zfv());
    }
    zfoverride
    virtual ZFCompareResult objectCompare(ZF_IN ZFObject *anotherObj)
    {
        zfself *ref = ZFCastZFObject(zfself *, anotherObj);
        if(ref != zfnull)
        {
            return ZFComparerDefault(this->zfv(), ref->zfv());
        }
        else
        {
            return ZFCompareUncomparable;
        }
    }
    zfoverride
    virtual inline zfbool objectIsPrivate(void) {return zftrue;}
    zfoverride
    virtual inline zfbool objectIsInternal(void) {return zftrue;}
protected:
    zfoverride
    virtual void objectOnInit(void) {zfsuper::objectOnInit();}
    /** @brief init with value */
    virtual void objectOnInit(ZF_IN const zfchar *zfv) {this->objectOnInit(); this->zfv(zfv);}
protected:
    zfoverride
    virtual void objectInfoT(ZF_IN_OUT zfstring &ret)
    {
        ret += this->zfv();
    }
};
/** @brief see #ZFDI_WrapperBase */
zfclass ZF_ENV_EXPORT ZFDI_Wrapper : zfextends ZFDI_WrapperBase
{
    ZFOBJECT_DECLARE(ZFDI_Wrapper, ZFDI_WrapperBase)
    ZFALLOC_CACHE_RELEASE({zfsuper::zfAllocCacheRelease(cache);})
public:
    zfoverride
    virtual void zfv(ZF_IN const zfchar *zfv)
    {
        this->_ZFP_zfv = zfv;
    }
    zfoverride
    virtual const zfchar *zfv(void) {return this->_ZFP_zfv;}
private:
    zfstring _ZFP_zfv;
};
/** @brief see #ZFDI_WrapperBase */
zfclass ZF_ENV_EXPORT ZFDI_WrapperRaw : zfextends ZFDI_WrapperBase
{
    ZFOBJECT_DECLARE_WITH_CUSTOM_CTOR(ZFDI_WrapperRaw, ZFDI_WrapperBase)
    ZFALLOC_CACHE_RELEASE({zfsuper::zfAllocCacheRelease(cache);})
public:
    zfoverride
    virtual void zfv(ZF_IN const zfchar *zfv)
    {
        if(zfv)
        {
            this->_ZFP_zfv = zfv;
        }
        else
        {
            this->_ZFP_zfv = "";
        }
    }
    zfoverride
    virtual const zfchar *zfv(void) {return this->_ZFP_zfv;}
private:
    const zfchar *_ZFP_zfv;
protected:
    ZFDI_WrapperRaw(void) : _ZFP_zfv("") {}
};

// ============================================================
/**
 * @brief util method to convert to string
 *
 * support these types:
 * -  #v_zfstring
 * -  #ZFDI_WrapperBase
 */
extern ZF_ENV_EXPORT const zfchar *ZFDI_toString(ZF_IN ZFObject *obj);

/**
 * @brief util method to find class as well as type id wrapper class
 *
 * we will try to find class by #ZFClass::classForName,
 * if unable to find,
 * we will append #ZFTypeIdWrapperPrefixName to find again, e.g.
 * "ClassName" to "v_ClassName" or "NS.ClassName" to "NS.v_ClassName"
 */
extern ZF_ENV_EXPORT const ZFClass *ZFDI_classForName(ZF_IN const zfchar *className,
                                                      ZF_IN const zfchar *NS);
/**
 * @brief util to print param info
 */
extern ZF_ENV_EXPORT void ZFDI_paramInfo(ZF_IN_OUT zfstring &ret
                                         , ZF_IN_OPT ZFObject *param0 = ZFMethodGenericInvokerDefaultParam()
                                         , ZF_IN_OPT ZFObject *param1 = ZFMethodGenericInvokerDefaultParam()
                                         , ZF_IN_OPT ZFObject *param2 = ZFMethodGenericInvokerDefaultParam()
                                         , ZF_IN_OPT ZFObject *param3 = ZFMethodGenericInvokerDefaultParam()
                                         , ZF_IN_OPT ZFObject *param4 = ZFMethodGenericInvokerDefaultParam()
                                         , ZF_IN_OPT ZFObject *param5 = ZFMethodGenericInvokerDefaultParam()
                                         , ZF_IN_OPT ZFObject *param6 = ZFMethodGenericInvokerDefaultParam()
                                         , ZF_IN_OPT ZFObject *param7 = ZFMethodGenericInvokerDefaultParam()
                                         );

/**
 * @brief perform advanced dynamic invoke
 *
 * note: when mentioned `string`, we means any type
 * that can be converted by #ZFDI_toString
 *
 * NS can be any string describe namespace,
 * and these values are considered in global scope:
 * -  null or empty string
 * -  #ZF_NAMESPACE_GLOBAL_NAME
 * -  #ZF_NAMESPACE_GLOBAL_ABBR_NAME
 *
 * type can be:
 * -  #v_ZFClass : same as #ZFDI_alloc
 * -  #v_ZFMethod : invoke using the method
 * -  string :
 *   -  holds method name ("methodName")
 *     or method name with namespace ("NS.methodName")
 *   -  holds #ZFClass::className or #ZFClass::classNameFull,
 *     see #ZFDI_alloc for how to find the class\n
 *     this rule only applied when obj is null
 *
 * params can be :
 * -  #ZFObject type for retain type
 * -  #ZFTypeIdWrapper for assign type
 * -  #ZFMethodGenericInvokerDefaultParam for default param
 * -  #ZFDI_WrapperBase, we will try to convert to desired type if possible
 */
extern ZF_ENV_EXPORT zfbool ZFDI_invoke(ZF_OUT zfautoObject &ret
                                        , ZF_OUT_OPT zfstring *errorHint
                                        , ZF_IN_OPT ZFObject *obj
                                        , ZF_IN_OPT const zfchar *NS
                                        , ZF_IN ZFObject *type
                                        , ZF_IN_OPT zfindex paramCount
                                        , ZF_IN_OUT zfautoObject (&paramList)[ZFMETHOD_MAX_PARAM]
                                        );

/**
 * @brief perform advanced dynamic invoke
 *
 * for string type, NS, and more info, see also #ZFDI_invoke
 *
 * type can be:
 * -  #v_ZFClass : alloc using the class
 * -  string : holds #ZFClass::className or #ZFClass::classNameFull
 *
 * when finding class using string, we would try to find by:
 * -  #ZFClass::classForName
 * -  if not able to find, we would try to add the "v_" prefix
 *   to the class name then find again, e.g.
 *   "ClassName" to "v_ClassName" or "NS.ClassName" to "NS.v_ClassName"
 *
 * for the params, see #ZFDI_invoke for more info
 */
extern ZF_ENV_EXPORT zfbool ZFDI_alloc(ZF_OUT zfautoObject &ret
                                       , ZF_OUT_OPT zfstring *errorHint
                                       , ZF_IN_OPT const zfchar *NS
                                       , ZF_IN ZFObject *type
                                       , ZF_IN_OPT zfindex paramCount
                                       , ZF_IN_OUT zfautoObject (&paramList)[ZFMETHOD_MAX_PARAM]
                                       );
/** @brief see #ZFDI_alloc */
extern ZF_ENV_EXPORT zfbool ZFDI_alloc(ZF_OUT zfautoObject &ret
                                       , ZF_OUT_OPT zfstring *errorHint
                                       , ZF_IN const ZFClass *cls
                                       , ZF_IN_OPT zfindex paramCount
                                       , ZF_IN_OUT zfautoObject (&paramList)[ZFMETHOD_MAX_PARAM]
                                       );

// ============================================================
/**
 * @brief util method to convert param type from
 */
extern ZF_ENV_EXPORT zfbool ZFDI_paramConvert(ZF_OUT zfautoObject &ret,
                                              ZF_IN const zfchar *typeId,
                                              ZF_IN ZFDI_WrapperBase *wrapper,
                                              ZF_OUT_OPT zfstring *errorHint = zfnull);

// ============================================================
/* ZFMETHOD_MAX_PARAM */
/**
 * @brief util to #ZFDI_invoke for static method or object allocation
 *
 * cases:
 * @code
 *   NS.ClassName(...)             // object allcation
 *   NS.ClassName.MethodName(...)  // static member method
 *   NS.MethodName(...)            // static method
 *
 *   // type wrapper specials
 *   NS.v_zfstring(...)            // type wrapper allcation
 *   NS.zfstring(...)              // type wrapper allcation
 *   NS.v_zfstring.MethodName(...) // static member method of type wrapper
 *   NS.zfstring.MethodName(...)   // static member method of type wrapper
 * @endcode
 */
extern ZF_ENV_EXPORT zfautoObject ZFInvoke(ZF_IN const zfchar *name
                                           , ZF_IN_OPT ZFObject *param0 = ZFMethodGenericInvokerDefaultParam()
                                           , ZF_IN_OPT ZFObject *param1 = ZFMethodGenericInvokerDefaultParam()
                                           , ZF_IN_OPT ZFObject *param2 = ZFMethodGenericInvokerDefaultParam()
                                           , ZF_IN_OPT ZFObject *param3 = ZFMethodGenericInvokerDefaultParam()
                                           , ZF_IN_OPT ZFObject *param4 = ZFMethodGenericInvokerDefaultParam()
                                           , ZF_IN_OPT ZFObject *param5 = ZFMethodGenericInvokerDefaultParam()
                                           , ZF_IN_OPT ZFObject *param6 = ZFMethodGenericInvokerDefaultParam()
                                           , ZF_IN_OPT ZFObject *param7 = ZFMethodGenericInvokerDefaultParam()
                                           , ZF_OUT_OPT zfbool *success = zfnull
                                           , ZF_OUT_OPT zfstring *errorHint = zfnull
                                           );
/** @brief see #ZFInvoke */
extern ZF_ENV_EXPORT zfautoObject ZFInvoke(ZF_IN const zfchar *name
                                           , ZF_IN const zfchar *param0
                                           , ZF_IN_OPT const zfchar *param1 = zfnull
                                           , ZF_IN_OPT const zfchar *param2 = zfnull
                                           , ZF_IN_OPT const zfchar *param3 = zfnull
                                           , ZF_IN_OPT const zfchar *param4 = zfnull
                                           , ZF_IN_OPT const zfchar *param5 = zfnull
                                           , ZF_IN_OPT const zfchar *param6 = zfnull
                                           , ZF_IN_OPT const zfchar *param7 = zfnull
                                           , ZF_OUT_OPT zfbool *success = zfnull
                                           , ZF_OUT_OPT zfstring *errorHint = zfnull
                                           );

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFDynamicInvoker_h_

