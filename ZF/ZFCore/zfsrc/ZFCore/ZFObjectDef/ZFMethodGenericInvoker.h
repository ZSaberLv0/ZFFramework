/**
 * @file ZFMethodGenericInvoker.h
 * @brief generic invoker declare for #ZFMethod::methodGenericInvoker
 */

#ifndef _ZFI_ZFMethodGenericInvoker_h_
#define _ZFI_ZFMethodGenericInvoker_h_

#include "ZFTypeIdFwd.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief max param supported by ZFMethod
 */
#define ZFMETHOD_MAX_PARAM 8

// ============================================================
zfclassFwd ZFMethod;
/* ZFMETHOD_MAX_PARAM */
/**
 * @brief generic invoker for advanced reflection, see #ZFMethod::methodGenericInvoker
 */
typedef zfbool (*ZFMethodGenericInvoker)(ZF_IN const ZFMethod *invokerMethod
                                         , ZF_IN ZFObject *invokerObject
                                         , ZF_OUT_OPT zfstring *errorHint
                                         , ZF_OUT zfautoObject &ret
                                         , ZF_IN_OUT zfautoObject (&paramList)[ZFMETHOD_MAX_PARAM]
                                         );

/**
 * @brief util macro to expand params for #ZFMethodGenericInvoker
 */
#define ZFMETHOD_GENERIC_INVOKER_PARAMS \
    ZF_IN const ZFMethod *invokerMethod \
    , ZF_IN ZFObject *invokerObject \
    , ZF_OUT_OPT zfstring *errorHint \
    , ZF_OUT zfautoObject &ret \
    , ZF_IN_OUT zfautoObject (&paramList)[ZFMETHOD_MAX_PARAM]

extern ZF_ENV_EXPORT ZFObject *_ZFP_ZFMethodGenericInvokerDefaultParamRef;
/**
 * @brief mark the object as default param
 */
#define ZFMethodGenericInvokerDefaultParam() ((ZFObject * const &)_ZFP_ZFMethodGenericInvokerDefaultParamRef)

extern ZF_ENV_EXPORT zfautoObject _ZFP_ZFMethodGenericInvokerDefaultParamHolderRef;
/**
 * @brief holder of #ZFMethodGenericInvokerDefaultParam
 */
#define ZFMethodGenericInvokerDefaultParamHolder() ((zfautoObject const &)_ZFP_ZFMethodGenericInvokerDefaultParamHolderRef)

// ============================================================
template<typename T_Dummy, int n>
zfclassNotPOD _ZFP_MtdGICk
{
};
template<typename T_Dummy>
zfclassNotPOD _ZFP_MtdGICk<T_Dummy, 1>
{
public:
    typedef int TypeMustRegisterByZFPROPERTY;
};
extern ZF_ENV_EXPORT zfbool _ZFP_MtdGIParamCheck(ZF_OUT_OPT zfstring *errorHint,
                                                 ZF_IN zfbool zfvAccessAvailable,
                                                 ZF_IN const ZFMethod *invokerMethod,
                                                 ZF_IN zfindex paramIndex,
                                                 ZF_IN const zfchar *paramType,
                                                 ZF_IN ZFObject *param);
#define _ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_DECLARE_EXPAND(N, ParamType) \
    typedef ParamType _T##N; \
    typedef zftTraits<ParamType>::TrNoRef _TR##N; \
    typedef _ZFP_MtdGICk< \
            _TR##N, \
            ZFTypeId<_TR##N>::TypeIdRegistered \
        >::TypeMustRegisterByZFPROPERTY _Ck##N;
#define _ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_PREPARE_EXPAND(N, DefaultExpandOrEmpty, ParamType, param) \
    _ZFP_MtdGIParamCheck( \
        errorHint, \
        ZFTypeId<_TR##N>::Value<_T##N>::zfvAccessAvailable(param), \
        invokerMethod, \
        (_Ck##N)N, \
        ZFM_TOSTRING(ParamType), \
        param)
template<typename T_Type, typename T_Access>
zfclassNotPOD _ZFP_MtdGIPA
{
public:
    zfautoObject &obj;
public:
    explicit _ZFP_MtdGIPA(ZF_IN_OUT zfautoObject &obj) : obj(obj) {}
    T_Access a(ZF_IN_OPT const zfautoObject &pDef = zfnull)
    {
        if(this->obj == ZFMethodGenericInvokerDefaultParam())
        {
            this->obj = pDef;
        }
        return ZFTypeId<T_Type>::template Value<T_Access>::zfvAccess(this->obj);
    }
public:
    ~_ZFP_MtdGIPA(void)
    {
        ZFTypeId<T_Type>::template Value<T_Access>::zfvAccessFinish(this->obj);
    }
};
#define _ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_ACCESS_EXPAND(N, DefaultExpandOrEmpty, ParamType, param) \
    _ZFP_MtdGIPA<_TR##N, _T##N>(param).a(DefaultExpandOrEmpty(pDef##N()))
#define _ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS(N, DefaultExpandOrEmpty, ParamType, DefaultValueFix) \
    DefaultExpandOrEmpty( \
        static zfautoObject pDef##N(void) \
        { \
            zftValue<zftTraits<ParamType>::TrNoRef> paramDefault; \
            zfautoObject ret; \
            ZFTypeId<zftTraits<ParamType>::TrNoRef>::ValueStore( \
                ret, \
                (paramDefault.zfv DefaultValueFix())); \
            ZFTypeIdWrapper::markConst(ret); \
            return ret; \
        } \
    )

// ============================================================
extern ZF_ENV_EXPORT void _ZFP_MtdGIRetError(ZF_OUT_OPT zfstring *errorHint,
                                             ZF_IN const zfchar *returnTypeId,
                                             ZF_IN const zfchar *returnValueInfo);

template<typename T_ReturnType>
zfclassNotPOD _ZFP_MtdGIARefA
{
public:
    static zfbool R(T_ReturnType retTmp, ZFObject *invokerObject, zfautoObject &ret)
    {
        zfautoObject invokerObjectHolder = invokerObject;
        typedef typename zftTraits<T_ReturnType>::TrNoRef T_ReturnTypeTmp;
        if(ZFTypeId<T_ReturnTypeTmp>::template Value<T_ReturnType>::zfvAccessAvailable(invokerObjectHolder)
            && (&retTmp == &(ZFTypeId<T_ReturnTypeTmp>::template Value<T_ReturnType>::zfvAccess(invokerObjectHolder))))
        {
            ret = invokerObject;
            return zftrue;
        }
        else
        {
            return zffalse;
        }
    }
};
template<typename T_ReturnType> zfclassNotPOD _ZFP_MtdGIARef {public:
    static zfbool C(void) {return zffalse;}
    static zfbool R(T_ReturnType retTmp, ZFObject *invokerObject, zfautoObject &ret) {return zffalse;}
};
template<typename T_ReturnType> zfclassNotPOD _ZFP_MtdGIARef<T_ReturnType &> {public:
    static zfbool C(void) {return zftrue;}
    static zfbool R(T_ReturnType &retTmp, ZFObject *invokerObject, zfautoObject &ret)
    {return _ZFP_MtdGIARefA<T_ReturnType &>::R(retTmp, invokerObject, ret);}
};
template<typename T_ReturnType> zfclassNotPOD _ZFP_MtdGIARef<T_ReturnType const &> {public:
    static zfbool C(void) {return zftrue;}
    static zfbool R(T_ReturnType const &retTmp, ZFObject *invokerObject, zfautoObject &ret)
    {return _ZFP_MtdGIARefA<T_ReturnType const &>::R(retTmp, invokerObject, ret);}
};
template<typename T_ReturnType> zfclassNotPOD _ZFP_MtdGIARef<T_ReturnType *> {public:
    static zfbool C(void) {return zffalse;}
    static zfbool R(T_ReturnType *retTmp, ZFObject *invokerObject, zfautoObject &ret) {return zffalse;}
};
template<typename T_ReturnType> zfclassNotPOD _ZFP_MtdGIARef<T_ReturnType * &> {public:
    static zfbool C(void) {return zffalse;}
    static zfbool R(T_ReturnType * &retTmp, ZFObject *invokerObject, zfautoObject &ret) {return zffalse;}
};
template<typename T_ReturnType> zfclassNotPOD _ZFP_MtdGIARef<T_ReturnType * const &> {public:
    static zfbool C(void) {return zffalse;}
    static zfbool R(T_ReturnType * const &retTmp, ZFObject *invokerObject, zfautoObject &ret) {return zffalse;}
};
template<typename T_ReturnType> zfclassNotPOD _ZFP_MtdGIARef<const T_ReturnType *> {public:
    static zfbool C(void) {return zffalse;}
    static zfbool R(const T_ReturnType *retTmp, ZFObject *invokerObject, zfautoObject &ret) {return zffalse;}
};
template<typename T_ReturnType> zfclassNotPOD _ZFP_MtdGIARef<const T_ReturnType * &> {public:
    static zfbool C(void) {return zffalse;}
    static zfbool R(const T_ReturnType * &retTmp, ZFObject *invokerObject, zfautoObject &ret) {return zffalse;}
};
template<typename T_ReturnType> zfclassNotPOD _ZFP_MtdGIARef<const T_ReturnType * const &> {public:
    static zfbool C(void) {return zffalse;}
    static zfbool R(const T_ReturnType * const &retTmp, ZFObject *invokerObject, zfautoObject &ret) {return zffalse;}
};

template<typename T_ReturnType>
zfclassNotPOD _ZFP_MtdGIA
{
public:
    typedef T_ReturnType (*Ivk)(ZF_IN const ZFMethod *invokerMethod
                                , ZF_IN ZFObject *invokerObject
                                , ZF_IN_OUT zfautoObject (&paramList)[ZFMETHOD_MAX_PARAM]
                                );
public:
    static zfbool A(ZF_IN Ivk invoke
                    , ZF_IN const ZFMethod *invokerMethod
                    , ZF_IN ZFObject *invokerObject
                    , ZF_OUT_OPT zfstring *errorHint
                    , ZF_OUT_OPT zfautoObject &ret
                    , ZF_IN_OUT zfautoObject (&paramList)[ZFMETHOD_MAX_PARAM]
                    )
    {
        T_ReturnType retTmp = invoke(invokerMethod, invokerObject, paramList);
        zfCoreMutexLocker();
        typedef typename zftTraits<T_ReturnType>::TrNoRef T_ReturnTypeTmp;
        if(_ZFP_MtdGIARef<T_ReturnType>::C() && _ZFP_MtdGIARef<T_ReturnType>::R(retTmp, invokerObject, ret))
        {
            return zftrue;
        }
        if(ZFTypeId<T_ReturnTypeTmp>::ValueStore(ret, retTmp))
        {
            _ZFP_ZFTypeIdWrapperMarkConstCheck<T_ReturnType>::a(ret);
            return zftrue;
        }
        else
        {
            zfstring info;
            ZFCoreElementInfoGetter<T_ReturnTypeTmp>::elementInfoGetter(info, retTmp);
            _ZFP_MtdGIRetError(errorHint,
                ZFTypeId<T_ReturnTypeTmp>::TypeId(),
                info);
            return zffalse;
        }
    }
};
template<>
zfclassNotPOD _ZFP_MtdGIA<void>
{
public:
    typedef void (*Ivk)(ZF_IN const ZFMethod *invokerMethod
                        , ZF_IN ZFObject *invokerObject
                        , ZF_IN_OUT zfautoObject (&paramList)[ZFMETHOD_MAX_PARAM]
                        );
public:
    static zfbool A(ZF_IN Ivk invoke
                    , ZF_IN const ZFMethod *invokerMethod
                    , ZF_IN ZFObject *invokerObject
                    , ZF_OUT_OPT zfstring *errorHint
                    , ZF_OUT_OPT zfautoObject &ret
                    , ZF_IN_OUT zfautoObject (&paramList)[ZFMETHOD_MAX_PARAM]
                    )
    {
        invoke(invokerMethod, invokerObject, paramList);
        return zftrue;
    }
};

// ============================================================
#define _ZFP_ZFMETHOD_GENERIC_INVOKER_DECLARE( \
        ReturnType \
        , ParamExpandOrEmpty0, ParamType0, param0_, DefaultExpandOrEmpty0, DefaultValueFix0 \
        , ParamExpandOrEmpty1, ParamType1, param1_, DefaultExpandOrEmpty1, DefaultValueFix1 \
        , ParamExpandOrEmpty2, ParamType2, param2_, DefaultExpandOrEmpty2, DefaultValueFix2 \
        , ParamExpandOrEmpty3, ParamType3, param3_, DefaultExpandOrEmpty3, DefaultValueFix3 \
        , ParamExpandOrEmpty4, ParamType4, param4_, DefaultExpandOrEmpty4, DefaultValueFix4 \
        , ParamExpandOrEmpty5, ParamType5, param5_, DefaultExpandOrEmpty5, DefaultValueFix5 \
        , ParamExpandOrEmpty6, ParamType6, param6_, DefaultExpandOrEmpty6, DefaultValueFix6 \
        , ParamExpandOrEmpty7, ParamType7, param7_, DefaultExpandOrEmpty7, DefaultValueFix7 \
    ) \
    private: \
        ParamExpandOrEmpty0(_ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_DECLARE_EXPAND(0, ParamType0)) \
        ParamExpandOrEmpty1(_ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_DECLARE_EXPAND(1, ParamType1)) \
        ParamExpandOrEmpty2(_ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_DECLARE_EXPAND(2, ParamType2)) \
        ParamExpandOrEmpty3(_ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_DECLARE_EXPAND(3, ParamType3)) \
        ParamExpandOrEmpty4(_ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_DECLARE_EXPAND(4, ParamType4)) \
        ParamExpandOrEmpty5(_ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_DECLARE_EXPAND(5, ParamType5)) \
        ParamExpandOrEmpty6(_ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_DECLARE_EXPAND(6, ParamType6)) \
        ParamExpandOrEmpty7(_ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_DECLARE_EXPAND(7, ParamType7)) \
    public: \
        ParamExpandOrEmpty0(_ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS(0, DefaultExpandOrEmpty0, ParamType0, DefaultValueFix0)) \
        ParamExpandOrEmpty1(_ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS(1, DefaultExpandOrEmpty1, ParamType1, DefaultValueFix1)) \
        ParamExpandOrEmpty2(_ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS(2, DefaultExpandOrEmpty2, ParamType2, DefaultValueFix2)) \
        ParamExpandOrEmpty3(_ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS(3, DefaultExpandOrEmpty3, ParamType3, DefaultValueFix3)) \
        ParamExpandOrEmpty4(_ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS(4, DefaultExpandOrEmpty4, ParamType4, DefaultValueFix4)) \
        ParamExpandOrEmpty5(_ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS(5, DefaultExpandOrEmpty5, ParamType5, DefaultValueFix5)) \
        ParamExpandOrEmpty6(_ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS(6, DefaultExpandOrEmpty6, ParamType6, DefaultValueFix6)) \
        ParamExpandOrEmpty7(_ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS(7, DefaultExpandOrEmpty7, ParamType7, DefaultValueFix7)) \
    public: \
        static zfbool GI(ZF_IN const ZFMethod *invokerMethod \
                         , ZF_IN ZFObject *invokerObject \
                         , ZF_OUT_OPT zfstring *errorHint \
                         , ZF_OUT_OPT zfautoObject &ret \
                         , ZF_IN_OUT zfautoObject (&paramList)[ZFMETHOD_MAX_PARAM] \
                         ) \
        { \
            ParamExpandOrEmpty0( \
                if( \
                                           !_ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_PREPARE_EXPAND(0, DefaultExpandOrEmpty0, ParamType0, paramList[0]) \
                    ParamExpandOrEmpty1(|| !_ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_PREPARE_EXPAND(1, DefaultExpandOrEmpty1, ParamType1, paramList[1])) \
                    ParamExpandOrEmpty2(|| !_ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_PREPARE_EXPAND(2, DefaultExpandOrEmpty2, ParamType2, paramList[2])) \
                    ParamExpandOrEmpty3(|| !_ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_PREPARE_EXPAND(3, DefaultExpandOrEmpty3, ParamType3, paramList[3])) \
                    ParamExpandOrEmpty4(|| !_ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_PREPARE_EXPAND(4, DefaultExpandOrEmpty4, ParamType4, paramList[4])) \
                    ParamExpandOrEmpty5(|| !_ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_PREPARE_EXPAND(5, DefaultExpandOrEmpty5, ParamType5, paramList[5])) \
                    ParamExpandOrEmpty6(|| !_ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_PREPARE_EXPAND(6, DefaultExpandOrEmpty6, ParamType6, paramList[6])) \
                    ParamExpandOrEmpty7(|| !_ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_PREPARE_EXPAND(7, DefaultExpandOrEmpty7, ParamType7, paramList[7])) \
                    ) \
                { \
                    return zffalse; \
                } \
            ) \
            return _ZFP_MtdGIA<ReturnType>::A(I, invokerMethod, invokerObject, errorHint, ret, paramList); \
        } \
    private: \
        static ReturnType I(ZF_IN const ZFMethod *invokerMethod \
                            , ZF_IN ZFObject *invokerObject \
                            , ZF_IN_OUT zfautoObject (&paramList)[ZFMETHOD_MAX_PARAM] \
                            ) \
        { \
            return invokerMethod->_ZFP_execute<ReturnType \
                    ParamExpandOrEmpty0(ZFM_COMMA() ParamType0) \
                    ParamExpandOrEmpty1(ZFM_COMMA() ParamType1) \
                    ParamExpandOrEmpty2(ZFM_COMMA() ParamType2) \
                    ParamExpandOrEmpty3(ZFM_COMMA() ParamType3) \
                    ParamExpandOrEmpty4(ZFM_COMMA() ParamType4) \
                    ParamExpandOrEmpty5(ZFM_COMMA() ParamType5) \
                    ParamExpandOrEmpty6(ZFM_COMMA() ParamType6) \
                    ParamExpandOrEmpty7(ZFM_COMMA() ParamType7) \
                >(invokerObject \
                    ParamExpandOrEmpty0(ZFM_COMMA() _ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_ACCESS_EXPAND(0, DefaultExpandOrEmpty0, ParamType0, paramList[0])) \
                    ParamExpandOrEmpty1(ZFM_COMMA() _ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_ACCESS_EXPAND(1, DefaultExpandOrEmpty1, ParamType1, paramList[1])) \
                    ParamExpandOrEmpty2(ZFM_COMMA() _ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_ACCESS_EXPAND(2, DefaultExpandOrEmpty2, ParamType2, paramList[2])) \
                    ParamExpandOrEmpty3(ZFM_COMMA() _ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_ACCESS_EXPAND(3, DefaultExpandOrEmpty3, ParamType3, paramList[3])) \
                    ParamExpandOrEmpty4(ZFM_COMMA() _ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_ACCESS_EXPAND(4, DefaultExpandOrEmpty4, ParamType4, paramList[4])) \
                    ParamExpandOrEmpty5(ZFM_COMMA() _ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_ACCESS_EXPAND(5, DefaultExpandOrEmpty5, ParamType5, paramList[5])) \
                    ParamExpandOrEmpty6(ZFM_COMMA() _ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_ACCESS_EXPAND(6, DefaultExpandOrEmpty6, ParamType6, paramList[6])) \
                    ParamExpandOrEmpty7(ZFM_COMMA() _ZFP_ZFMETHOD_GENERIC_INVOKER_PARAM_ACCESS_EXPAND(7, DefaultExpandOrEmpty7, ParamType7, paramList[7])) \
                ); \
        } \
    public:
#define _ZFP_ZFMETHOD_GENERIC_INVOKER_ADDR(owner) \
    owner::GI
#define _ZFP_ZFMETHOD_GENERIC_PARAM_DEFAULT_ACCESS_ADDR(owner, DefaultExpandOrEmpty, N) \
    (zfnull DefaultExpandOrEmpty(ZFM_EMPTY(), owner::pDef##N))

// ============================================================
extern ZF_ENV_EXPORT zfbool _ZFP_ZFMethodGenericInvoke(ZF_IN const ZFMethod *invokerMethod
                                                       , ZF_IN ZFObject *invokerObject
                                                       , ZF_OUT_OPT zfstring *errorHint
                                                       , ZF_OUT zfautoObject &ret
                                                       , ZF_IN_OUT zfautoObject (&paramList)[ZFMETHOD_MAX_PARAM]
                                                       );
extern ZF_ENV_EXPORT void _ZFP_ZFMethodGenericInvokeError(ZF_IN const ZFMethod *method,
                                                          ZF_IN ZFObject *obj,
                                                          ZF_IN zfint pos,
                                                          ZF_IN_OPT const zfchar *errorHint = zfnull);

#define _ZFP_ZFMethodGenericInvoke_REPEAT1(N) \
        typedef typename zftTraits<Type##N>::TrNoRef _Type##N; \
        if(!ZFTypeId<_Type##N>::ValueStore(_p[N], param##N)) \
        { \
            _ZFP_ZFMethodGenericInvokeError(method, obj, N); \
        } \
        _ZFP_ZFTypeIdWrapperMarkConstCheck<Type##N>::a(_p[N]);
#define _ZFP_ZFMethodGenericInvoke_REPEAT2(N) \
        _ZFP_MtdGII_P<Type##N>::p(param##N, _p[N]);

#define _ZFP_ZFMethodGenericInvoke_DECLARE(N) \
    template<typename T_ReturnType ZFM_REPEAT(N, ZFM_REPEAT_TEMPLATE, ZFM_COMMA, ZFM_COMMA)> \
    T_ReturnType _ZFP_MtdGII(ZF_IN const ZFMethod *method, ZF_IN ZFObject *obj ZFM_REPEAT(N, ZFM_REPEAT_PARAM, ZFM_COMMA, ZFM_COMMA)) \
    { \
        zfautoObject _p[ZFMETHOD_MAX_PARAM]; \
        for(zfindex i = 0; i < ZFMETHOD_MAX_PARAM; ++i) \
        { \
            _p[i] = ZFMethodGenericInvokerDefaultParamHolder(); \
        } \
        ZFM_REPEAT(N, _ZFP_ZFMethodGenericInvoke_REPEAT1, ZFM_EMPTY, ZFM_EMPTY) \
        zfautoObject _ret; \
        zfstring errorHint; \
        if(!_ZFP_ZFMethodGenericInvoke(method, obj, &errorHint, _ret, _p)) \
        { \
            _ZFP_ZFMethodGenericInvokeError(method, obj, -1, errorHint); \
        } \
        ZFM_REPEAT(N, _ZFP_ZFMethodGenericInvoke_REPEAT2, ZFM_EMPTY, ZFM_EMPTY) \
        return _ZFP_MtdGII_R<T_ReturnType>::r(method, obj, _ret); \
    }

template<typename T_ParamType, typename T_Fix = void>
zfclassNotPOD _ZFP_MtdGII_P
{
public:
    static inline void p(ZF_IN_OUT T_ParamType p, ZF_IN_OUT zfautoObject &h)
    {
    }
};
template<typename T_ParamType>
zfclassNotPOD _ZFP_MtdGII_P<T_ParamType const &>
{
public:
    static inline void p(ZF_IN_OUT T_ParamType const &p, ZF_IN_OUT zfautoObject &h)
    {
    }
};
template<typename T_ParamType>
zfclassNotPOD _ZFP_MtdGII_P<const T_ParamType *>
{
public:
    static inline void p(ZF_IN_OUT const T_ParamType *p, ZF_IN_OUT zfautoObject &h)
    {
    }
};
template<typename T_ParamType>
zfclassNotPOD _ZFP_MtdGII_P<T_ParamType &>
{
public:
    static void p(ZF_IN_OUT T_ParamType &p, ZF_IN_OUT zfautoObject &h)
    {
        typedef typename zftTraits<T_ParamType &>::TrNoRef _Type;
        p = ZFTypeId<_Type>::template Value<T_ParamType &>::zfvAccess(h);
        ZFTypeId<_Type>::template Value<T_ParamType &>::zfvAccessFinish(h);
    }
};
template<typename T_ParamType>
zfclassNotPOD _ZFP_MtdGII_P<T_ParamType *, zftEnableIf<!zftIsZFObject(T_ParamType)> >
{
public:
    static void p(ZF_IN_OUT T_ParamType *p, ZF_IN_OUT zfautoObject &h)
    {
        typedef typename zftTraits<T_ParamType *>::TrNoRef _Type;
        if(p)
        {
            *p = ZFTypeId<_Type>::template Value<T_ParamType const &>::zfvAccess(h);
            ZFTypeId<_Type>::template Value<T_ParamType const &>::zfvAccessFinish(h);
        }
    }
};

template<typename T_ReturnType>
zfclassNotPOD _ZFP_MtdGII_R
{
public:
    static T_ReturnType r(ZF_IN const ZFMethod *method, ZF_IN ZFObject *obj, ZF_IN_OUT zfautoObject &ret)
    {
        typedef typename zftTraits<T_ReturnType>::TrNoRef _T_ReturnType;
        if(!ZFTypeId<_T_ReturnType>::template Value<T_ReturnType>::zfvAccessAvailable(ret))
        {
            _ZFP_ZFMethodGenericInvokeError(method, obj, -2);
        }
        return ZFTypeId<_T_ReturnType>::template Value<T_ReturnType>::zfvAccess(ret);
    }
};
template<>
zfclassNotPOD _ZFP_MtdGII_R<void>
{
public:
    static void r(ZF_IN const ZFMethod *method, ZF_IN ZFObject *obj, ZF_IN_OUT zfautoObject &ret)
    {
    }
};

_ZFP_ZFMethodGenericInvoke_DECLARE(0)
_ZFP_ZFMethodGenericInvoke_DECLARE(1)
_ZFP_ZFMethodGenericInvoke_DECLARE(2)
_ZFP_ZFMethodGenericInvoke_DECLARE(3)
_ZFP_ZFMethodGenericInvoke_DECLARE(4)
_ZFP_ZFMethodGenericInvoke_DECLARE(5)
_ZFP_ZFMethodGenericInvoke_DECLARE(6)
_ZFP_ZFMethodGenericInvoke_DECLARE(7)
_ZFP_ZFMethodGenericInvoke_DECLARE(8)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFMethodGenericInvoker_h_

