/**
 * @file ZFAniBuilder.h
 * @brief util of #ZFAni to build animation quickly
 */

#ifndef _ZFI_ZFAniBuilder_h_
#define _ZFI_ZFAniBuilder_h_

#include "ZFAni.h"
ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd _ZFP_ZFAniBuilderPrivate;
/**
 * @brief util of #ZFAni to build animation quickly
 *
 * usage:
 * @code
 *   ZFAniBuilder(target)
 *       .to("prop0", "from0", "to0", duration, curve)
 *       .wait(waitTime)
 *       .to("groupedProp1", "from1", "to1")
 *       .to("groupedProp2", "from1", "to1")
 *       .step()
 *       .customAni(customAniCallback)
 *       .aniOnStart(callback)
 *       .aniOnStop(callback)
 *       .aniStart(onStopOrOnInvalidCallback);
 * @endcode
 */
zffinal zfclassLikePOD ZFAniBuilder
{
public:
    /** @brief see #ZFAniBuilder */
    const ZFAniBuilder &to(ZF_IN const zfchar *name,
                           ZF_IN ZFObject *from,
                           ZF_IN ZFObject *to,
                           ZF_IN_OPT zftimet aniDuration = 0,
                           ZF_IN_OPT ZFTimeLineCurve *aniCurve = zfnull) const;
    /** @brief see #ZFAniBuilder */
    const ZFAniBuilder &to(ZF_IN const zfchar *name,
                           ZF_IN const zfchar *from,
                           ZF_IN const zfchar *to,
                           ZF_IN_OPT zftimet aniDuration = 0,
                           ZF_IN_OPT ZFTimeLineCurve *aniCurve = zfnull) const;

    /**
     * @brief see #ZFAniBuilder
     *
     * customAniCallback's param0 is a #v_zffloat holds the ani progress
     */
    const ZFAniBuilder &customAni(ZF_IN const ZFListener &customAniCallback,
                                  ZF_IN_OPT ZFObject *userData = zfnull,
                                  ZF_IN_OPT zftimet aniDuration = 0,
                                  ZF_IN_OPT ZFTimeLineCurve *aniCurve = zfnull) const;

public:
    /** @brief see #ZFAniBuilder */
    const ZFAniBuilder &wait(ZF_IN zftimet waitTime) const;
    /** @brief see #ZFAniBuilder */
    const ZFAniBuilder &step(void) const;

public:
    /** @brief see #ZFAniBuilder */
    const ZFAniBuilder &aniOnInvalid(ZF_IN const ZFListener &cb, ZF_IN_OPT ZFObject *userData = zfnull) const;
    /** @brief see #ZFAniBuilder */
    const ZFAniBuilder &aniOnStart(ZF_IN const ZFListener &cb, ZF_IN_OPT ZFObject *userData = zfnull) const;
    /** @brief see #ZFAniBuilder */
    const ZFAniBuilder &aniOnStop(ZF_IN const ZFListener &cb, ZF_IN_OPT ZFObject *userData = zfnull) const;
    /** @brief see #ZFAniBuilder */
    const ZFAniBuilder &aniOnStopOrInvalid(ZF_IN const ZFListener &cb, ZF_IN_OPT ZFObject *userData = zfnull) const;

public:
    /** @brief get the impl animation */
    const ZFAniBuilder &aniStart(void) const;
    /** @brief get the impl animation */
    const ZFAniBuilder &aniStart(ZF_IN const ZFListener &onStopOrOnInvalidCallback, ZF_IN_OPT ZFObject *userData = zfnull) const;

    /** @brief get the aniTarget */
    ZFObject *aniTarget(void) const;
    /** @brief change the aniTarget */
    const ZFAniBuilder &aniTarget(ZF_IN ZFObject *aniTarget) const;
    /** @brief get the impl animation */
    zfautoObjectT<ZFAnimation *> toAnimation(void) const;

public:
    /** @brief construct with ani target */
    ZFAniBuilder(ZF_IN_OPT ZFObject *aniTarget = zfnull);
    /** @cond ZFPrivateDoc */
    ZFAniBuilder(ZF_IN const ZFAniBuilder &ref);
    ~ZFAniBuilder(void);
    ZFAniBuilder &operator = (ZF_IN const ZFAniBuilder &ref);
    zfbool operator == (ZF_IN const ZFAniBuilder &ref) const {return d == ref.d;}
    zfbool operator != (ZF_IN const ZFAniBuilder &ref) const {return d != ref.d;}
    /** @endcond */

private:
    _ZFP_ZFAniBuilderPrivate *d;
};

ZFTYPEID_ACCESS_ONLY_DECLARE(ZFAniBuilder, ZFAniBuilder)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFAniBuilder_h_

