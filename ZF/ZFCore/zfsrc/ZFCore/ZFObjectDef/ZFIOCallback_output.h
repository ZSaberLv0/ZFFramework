/**
 * @file ZFIOCallback_output.h
 * @brief common callback defines
 */

#ifndef _ZFI_ZFIOCallback_output_h_
#define _ZFI_ZFIOCallback_output_h_

#include "ZFIOCallback_fwd.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFOutput
/**
 * @brief general output callback
 *
 * proto type:
 * @code
 *   zfindex output(ZF_IN const void *src,
 *                  ZF_IN_OPT zfindex count = zfindexMax());
 * @endcode
 *
 * params:
 * -  (const void *) output buffer
 * -  (zfindex) byte size to write, or zfindexMax() to calculate by src
 *  (at this case, src must be NULL-terminated zfchar string)
 *
 * return:
 * -  char count written to output,
 *   return a value less than count indicates error state
 *
 * ADVANCED:\n
 * for implementations, see #ZFCallbackTagKeyword_ioOwner if need support seek
 */
ZFCALLBACK_DECLARE_BEGIN(ZFOutput, ZFIOCallback)
public:
    /** @brief see #ZFOutput */
    inline zfindex execute(ZF_IN const void *src,
                           ZF_IN_OPT zfindex count = zfindexMax()) const
    {
        return ZFCallback::executeExact<zfindex, const void *, zfindex>(src, count);
    }
    /** @brief see #ZFOutput */
    inline zfindex operator () (ZF_IN const void *src,
                                ZF_IN_OPT zfindex count = zfindexMax()) const
    {
        return ZFCallback::executeExact<zfindex, const void *, zfindex>(src, count);
    }
    /** @brief see #ZFOutput */
    inline zfindex output(ZF_IN const zfchar *src,
                          ZF_IN_OPT zfindex count = zfindexMax()) const
    {
        return this->execute(src, count);
    }
_ZFP_ZFCALLBACK_DECLARE_END_NO_ALIAS(ZFOutput, ZFIOCallback)

// ============================================================
// custom type output
#define _ZFP_ZFOUTPUT_EXPAND(T_Type) \
    template<> \
    zfclassNotPOD ZFCoreElementInfoGetter<T_Type> \
    { \
    public: \
        static void elementInfoGetter(ZF_IN_OUT zfstring &ret, ZF_IN T_Type const &v) \
        { \
            ZFCoreElementTokenGetter<T_Type>::elementTokenGetterLeft(ret, v); \
            { \
                ZFOutputForString(ret) << v; \
            } \
            ZFCoreElementTokenGetter<T_Type>::elementTokenGetterRight(ret, v); \
        } \
    };
/**
 * @brief declare your custom type to output to #ZFOutput
 *
 * proto type:
 * @code
 *   const ZFOutput &operator << (ZF_IN_OUT const ZFOutput &output, ZF_IN YourType const &v);
 * @endcode
 * usage:
 * @code
 *   ZFOUTPUT_TYPE(YourType, {output.execute(v.toString());})
 * @endcode
 *
 * once declared, you may output your type to #ZFOutput by:
 * @code
 *   output << yourObject;
 * @endcode
 */
#define ZFOUTPUT_TYPE(T_Type, outputAction) \
    /** @cond ZFPrivateDoc */ \
    inline const ZFOutput &operator << (const ZFOutput &output, T_Type const &v) \
    { \
        if(output.callbackIsValid()) \
        outputAction \
        return output; \
    } \
    _ZFP_ZFOUTPUT_EXPAND(T_Type) \
    /** @endcond */
/**
 * @brief see #ZFOUTPUT_TYPE
 *
 * usage:
 * @code
 *   ZFOUTPUT_TYPE_TEMPLATE(ZFM_EXPAND(typename T0, typename T1),
 *                          ZFM_EXPAND(YourType<T0, T1>),
 *                          {output.execute(v.toString())})
 * @endcode
 */
#define ZFOUTPUT_TYPE_TEMPLATE(templateList, T_Type, outputAction) \
    /** @cond ZFPrivateDoc */ \
    template<templateList> \
    inline const ZFOutput &operator << (const ZFOutput &output, T_Type const &v) \
    { \
        outputAction \
        return output; \
    } \
    template<templateList> \
    zfclassNotPOD ZFCoreElementInfoGetter<T_Type> \
    { \
    public: \
        static void elementInfoGetter(ZF_IN_OUT zfstring &ret, ZF_IN T_Type const &v) \
        { \
            ZFCoreElementTokenGetter<T_Type>::elementTokenGetterLeft(ret, v); \
            { \
                ZFOutputForString(ret) << v; \
            } \
            ZFCoreElementTokenGetter<T_Type>::elementTokenGetterRight(ret, v); \
        } \
    }; \
    /** @endcond */

/**
 * @brief see #ZFOUTPUT_TYPE
 *
 * usage:
 * @code
 *   // in header file
 *   ZFOUTPUT_TYPE_DECLARE(YourType)
 *   // in source file
 *   ZFOUTPUT_TYPE_DEFINE(YourType, {doYourStuff();})
 *
 *   // or, the inline version
 *   ZFOUTPUT_TYPE(YourType, {doYourStuff();})
 * @endcode
 */
#define ZFOUTPUT_TYPE_DECLARE(T_Type) \
    /** @cond ZFPrivateDoc */ \
    extern ZF_ENV_EXPORT const ZFOutput &operator << (const ZFOutput &output, T_Type const &v); \
    _ZFP_ZFOUTPUT_EXPAND(T_Type) \
    /** @endcond */
/** @brief see #ZFOUTPUT_TYPE_DECLARE */
#define ZFOUTPUT_TYPE_DEFINE(T_Type, outputAction) \
    /** @cond ZFPrivateDoc */ \
    const ZFOutput &operator << (const ZFOutput &output, T_Type const &v) \
    { \
        outputAction \
        return output; \
    } \
    /** @endcond */

// ============================================================
// common output callbacks
/**
 * @brief see #ZFCALLBACK_SERIALIZE_CUSTOM_TYPE_DEFINE
 *
 * serializable data:
 * @code
 *   <node>
 *       // dummy input has no contents
 *   </node>
 * @endcode
 */
#define ZFCallbackSerializeCustomType_ZFOutputDummy "ZFOutputDummy"
/**
 * @brief a dummy output that always output nothing with success state
 */
extern ZF_ENV_EXPORT ZFOutput ZFOutputDummy(void);

/**
 * @brief create a output callback to output to a zfstring
 *
 * params:
 * -  (zfstring &) a zfstring to output
 *
 * @note you must ensure the string to output is alive while the callback is still under use
 */
extern ZF_ENV_EXPORT ZFOutput ZFOutputForString(ZF_IN_OUT zfstring &s);
/**
 * @brief create a output callback to output to a buffer
 *
 * params:
 * -  (void *) buffer to write to
 * -  (zfindex) max buffer size or zfindexMax() for no limit (you must make sure buffer is enough),
 *   if autoAppendNullToken, maxCount should contain the extra '\0' size
 * -  (zfbool) whether auto append '\0' to tail each time write
 */
extern ZF_ENV_EXPORT ZFOutput ZFOutputForBufferUnsafe(ZF_IN void *buf,
                                                      ZF_IN_OPT zfindex maxCount = zfindexMax(),
                                                      ZF_IN_OPT zfbool autoAppendNullToken = zftrue);

// ============================================================
// basic output
ZFOUTPUT_TYPE(const zfchar *, {output.execute(v ? v : ZFTOKEN_zfnull);})
ZFOUTPUT_TYPE(zfchar *, {output << (const zfchar *)v;})

ZFOUTPUT_TYPE(const void *, {output.execute(v ? zfsFromPointer(v).cString() : ZFTOKEN_zfnull);})
ZFOUTPUT_TYPE(void *, {output << (const void *)v;})

/** @cond ZFPrivateDoc */
template<typename T_Type, int T_ZFObject = 0>
zfclassNotPOD _ZFP_ZFOutputOutputWrapper
{
public:
    static void outputAction(ZF_IN_OUT const ZFOutput &output, T_Type const &v)
    {
        output << (*v);
    }
};
template<typename T_Type>
zfclassNotPOD _ZFP_ZFOutputOutputWrapper<T_Type, 1>
{
public:
    static void outputAction(ZF_IN_OUT const ZFOutput &output, T_Type const &v)
    {
        output.execute(v->toObject()->objectInfo());
    }
};

template<typename T_Type>
const ZFOutput &operator << (const ZFOutput &output, const T_Type * const &v)
{
    if(v == zfnull)
    {
        output.execute(ZFTOKEN_zfnull);
    }
    else
    {
        output << (*v);
    }
    return output;
}
template<typename T_Type>
const ZFOutput &operator << (const ZFOutput &output, T_Type * const &v)
{
    if(v == zfnull)
    {
        output.execute(ZFTOKEN_zfnull);
    }
    else
    {
        _ZFP_ZFOutputOutputWrapper<T_Type *,
                zftIsZFObject(typename zftTraits<T_Type>::TrType)
            >::outputAction(output, v);
    }
    return output;
}
/** @endcond */

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFIOCallback_output_h_

