/**
 * @file ZFProtocolZFRegExp.h
 * @brief protocol for ZFRegExp
 */

#ifndef _ZFI_ZFProtocolZFRegExp_h_
#define _ZFI_ZFProtocolZFRegExp_h_

#include "ZFCore/ZFProtocol.h"
#include "ZFAlgorithm/ZFRegExp.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for ZFRegExp
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFRegExp)
public:
    /**
     * @brief create a native reg exp
     */
    virtual void *nativeRegExpCreate(ZF_IN ZFRegExp *regExp) zfpurevirtual;
    /**
     * @brief destroy a native reg exp
     */
    virtual void nativeRegExpDestroy(ZF_IN ZFRegExp *regExp,
                                     ZF_IN void *nativeRegExp) zfpurevirtual;

    /** @brief see ZFRegExp::regExpCompile */
    virtual void regExpCompile(ZF_IN ZFRegExp *regExp,
                               ZF_IN const zfchar *pattern,
                               ZF_IN_OPT ZFRegExpOptionFlags flag = ZFRegExpOptionFlags::EnumDefault()) zfpurevirtual;

    /** @brief see ZFRegExp::regExpNamedGroupIndexForName */
    virtual zfindex regExpNamedGroupIndexForName(ZF_IN ZFRegExp *regExp,
                                                 ZF_IN const zfchar *name) zfpurevirtual;

    /** @brief see ZFRegExp::regExpMatch */
    virtual void regExpMatch(ZF_IN ZFRegExp *regExp,
                             ZF_OUT ZFRegExpResult &result,
                             ZF_IN const zfchar *src,
                             ZF_IN_OPT zfindex srcLength = zfindexMax()) zfpurevirtual;
    /** @brief see ZFRegExp::regExpMatchExact */
    virtual void regExpMatchExact(ZF_IN ZFRegExp *regExp,
                                  ZF_OUT ZFRegExpResult &result,
                                  ZF_IN const zfchar *src,
                                  ZF_IN_OPT zfindex srcLength = zfindexMax()) zfpurevirtual;

    /** @brief see ZFRegExp::regExpReplace */
    virtual void regExpReplace(ZF_IN ZFRegExp *regExp,
                               ZF_OUT zfstring &ret,
                               ZF_OUT ZFRegExpResult &result,
                               ZF_IN const zfchar *src,
                               ZF_IN const zfchar *replacePattern,
                               ZF_IN_OPT zfindex maxReplaceCount = zfindexMax(),
                               ZF_IN_OPT zfindex srcLength = zfindexMax()) zfpurevirtual;
ZFPROTOCOL_INTERFACE_END(ZFRegExp)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFRegExp_h_

