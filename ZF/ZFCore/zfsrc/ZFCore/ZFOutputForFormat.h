/**
 * @file ZFOutputForFormat.h
 * @brief output callback util
 */

#ifndef _ZFI_ZFOutputForFormat_h_
#define _ZFI_ZFOutputForFormat_h_

#include "ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/** @brief see #ZFOutputForFormat */
ZFENUM_BEGIN(ZFOutputFormatStep)
    /**
     * @brief before anything would be outputed,
     *   ensured called only once for each output
     */
    ZFENUM_VALUE(OnInit)
    /**
     * @brief called each time before any output would be outputed
     */
    ZFENUM_VALUE(OnOutput)
    /**
     * @brief called only once just before finish/destroy the output
     */
    ZFENUM_VALUE(OnOutputEnd)
    /**
     * @brief called only once just before finish/destroy the output
     */
    ZFENUM_VALUE(OnDealloc)
ZFENUM_SEPARATOR(ZFOutputFormatStep)
    ZFENUM_VALUE_REGISTER(OnInit)
    ZFENUM_VALUE_REGISTER(OnOutput)
    ZFENUM_VALUE_REGISTER(OnOutputEnd)
    ZFENUM_VALUE_REGISTER(OnDealloc)
ZFENUM_END(ZFOutputFormatStep)

// ============================================================
/** @brief see #ZFOutputForFormat */
zfinterface ZFOutputFormat : zfextends ZFInterface
{
    ZFINTERFACE_DECLARE(ZFOutputFormat, ZFInterface)

protected:
    /**
     * @brief see #ZFOutputForFormat
     *
     * during output, it's ensured:
     * -  src is valid
     * -  srcLen is the src's length
     * -  outputCount is the number of times that the output callback was called
     * -  writtenLen is the number of chars that actually written to original output
     */
    virtual void format(ZF_IN_OUT zfstring &ret,
                        ZF_IN ZFOutputFormatStepEnum outputStep,
                        ZF_IN const zfchar *src,
                        ZF_IN zfindex srcLen,
                        ZF_IN zfindex outputCount,
                        ZF_IN zfindex writtenLen,
                        ZF_IN_OUT_OPT void *&state) zfpurevirtual;
public:
    inline void _ZFP_format(ZF_IN_OUT zfstring &ret,
                            ZF_IN ZFOutputFormatStepEnum outputStep,
                            ZF_IN const zfchar *src,
                            ZF_IN zfindex srcLen,
                            ZF_IN zfindex outputCount,
                            ZF_IN zfindex writtenLen,
                            ZF_IN_OUT_OPT void *&state)
    {
        this->format(ret, outputStep, src, srcLen, outputCount, writtenLen, state);
    }
};

// ============================================================
/**
 * @brief see #ZFCALLBACK_SERIALIZE_CUSTOM_TYPE_DEFINE
 *
 * serializable data:
 * @code
 *   <node>
 *       <ZFCallback category="output" ... /> // the output
 *       <YourOutputFormat category="format" ... /> // the format object
 *   </node>
 * @endcode
 */
#define ZFCallbackSerializeCustomType_ZFOutputForFormat "ZFOutputForFormat"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFOutputForFormat_output "output"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFOutputForFormat_format "format"

// ============================================================
/** @brief see #ZFOutputForFormat */
ZFMETHOD_FUNC_DECLARE_3(zfbool, ZFOutputForFormatT,
                        ZFMP_OUT(ZFCallback &, ret),
                        ZFMP_IN(const ZFOutput &, output),
                        ZFMP_IN(ZFOutputFormat *, format))

/**
 * @brief create an output callback with specified format rule
 *
 * each time before anything would be outputed to the original output,
 * it would be processed by the format object to determin the final output\n
 * \n
 * note:
 * -  the original output and the format object would be retained by the result output
 *   until it's destroyed
 * -  the result output is serializable if both the
 *   original output and the format object are serializable
 * -  serialization may cause performance issue,
 *   you may use #ZFOutputForFormatT and ##ZFCallbackSerializeCustomTypeDisable
 *   to disable the default serialization step,
 *   see #ZFCallbackSerializeCustomType_ZFInputForPathInfo for more info
 * -  if output success, the original output size would be returned,
 *   instead of the formated size
 */
ZFMETHOD_FUNC_DECLARE_2(ZFOutput, ZFOutputForFormat,
                        ZFMP_IN(const ZFOutput &, output),
                        ZFMP_IN(ZFOutputFormat *, format))

// ============================================================
/**
 * @brief try access the output format passed to #ZFOutputForFormat,
 *   return null if not available
 */
ZFMETHOD_FUNC_DECLARE_1(ZFOutputFormat *, ZFOutputForFormatGetFormat,
                        ZFMP_IN(const ZFCallback &, callback))

/**
 * @brief try access the output passed to #ZFOutputForFormat,
 *   return null callback if not available
 */
ZFMETHOD_FUNC_DECLARE_1(ZFOutput, ZFOutputForFormatGetOutput,
                        ZFMP_IN(const ZFCallback &, callback))

// ============================================================
/**
 * @brief basic output format
 */
zfclass ZF_ENV_EXPORT ZFOutputFormatBasic : zfextends ZFStyleableObject, zfimplements ZFOutputFormat
{
    ZFOBJECT_DECLARE(ZFOutputFormatBasic, ZFStyleableObject)
    ZFIMPLEMENTS_DECLARE(ZFOutputFormat)

public:
    /** @brief contents write before entire output */
    ZFPROPERTY_ASSIGN(zfstring, outputPrefix)
    /** @brief contents write before each line */
    ZFPROPERTY_ASSIGN(zfstring, linePrefix)
    /** @brief contents write after each line */
    ZFPROPERTY_ASSIGN(zfstring, linePostfix)
    /** @brief contents write after entire output */
    ZFPROPERTY_ASSIGN(zfstring, outputPostfix)

protected:
    zfoverride
    virtual void format(ZF_IN_OUT zfstring &ret,
                        ZF_IN ZFOutputFormatStepEnum outputStep,
                        ZF_IN const zfchar *src,
                        ZF_IN zfindex srcLen,
                        ZF_IN zfindex outputCount,
                        ZF_IN zfindex writtenLen,
                        ZF_IN_OUT_OPT void *&state);
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFOutputForFormat_h_

