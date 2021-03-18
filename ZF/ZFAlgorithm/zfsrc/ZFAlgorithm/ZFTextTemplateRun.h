/**
 * @file ZFTextTemplateRun.h
 * @brief text template util
 */

#ifndef _ZFI_ZFTextTemplateRun_h_
#define _ZFI_ZFTextTemplateRun_h_

#include "ZFTextTemplate.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief param for #ZFTextTemplateRun
 *
 * while running the #ZFTextTemplateRun,
 * these things can be applied with #ZFTextTemplateApply:
 * -  dir name
 * -  dir content, i.e. all the files and sub dirs within it
 * -  file name
 * -  file content
 *
 * if dir or file name matches,
 * they would be renamed\n
 * if dir or file content matches,
 * the content would be applied with #ZFTextTemplateApply\n
 * \n
 * the filter param can be used to filter out something you don't want to be
 * applied with #ZFTextTemplateApply\n
 * \n
 * by default, all setting would be copied from #ZFTextTemplateRunParamDefault
 */
zffinal zfclassLikePOD ZF_ENV_EXPORT ZFTextTemplateRunParam
{
public:
    ZFFilterForString dirNameFilter; /**< @brief see #ZFTextTemplateRunParam */
    ZFFilterForString dirContentFilter; /**< @brief see #ZFTextTemplateRunParam */
    ZFFilterForString fileNameFilter; /**< @brief see #ZFTextTemplateRunParam */
    ZFFilterForString fileContentFilter; /**< @brief see #ZFTextTemplateRunParam */

public:
    /**
     * @brief copy from another param
     */
    void copyFrom(ZF_IN const ZFTextTemplateRunParam &ref)
    {
        this->dirNameFilter.copyFrom(ref.dirNameFilter);
        this->dirContentFilter.copyFrom(ref.dirContentFilter);
        this->fileNameFilter.copyFrom(ref.fileNameFilter);
        this->fileContentFilter.copyFrom(ref.fileContentFilter);
    }

public:
    /** @cond ZFPrivateDoc */
    ZFTextTemplateRunParam(void) {}
    ZFTextTemplateRunParam(ZF_IN const ZFTextTemplateRunParam &ref)
    {
        this->copyFrom(ref);
    }
    ZFTextTemplateRunParam &operator = (ZF_IN const ZFTextTemplateRunParam &ref)
    {
        this->copyFrom(ref);
        return *this;
    }
    zfbool operator == (ZF_IN const ZFTextTemplateRunParam &ref) const
    {
        return (this->dirNameFilter == ref.dirNameFilter
            && this->dirContentFilter == ref.dirContentFilter
            && this->fileNameFilter == ref.fileNameFilter
            && this->fileContentFilter == ref.fileContentFilter);
    }
    zfbool operator != (ZF_IN const ZFTextTemplateRunParam &ref) const
    {
        return !this->operator == (ref);
    }
    /** @endcond */
};
ZFTYPEID_ACCESS_ONLY_DECLARE(ZFTextTemplateRunParam, ZFTextTemplateRunParam)

extern ZF_ENV_EXPORT ZFTextTemplateRunParam &_ZFP_ZFTextTemplateRunParamDefault(void);
/**
 * @brief default param for #ZFTextTemplateRun
 *
 * this setting can be changed at runtime,
 * however, it would affect the entire runtime,
 * change with caution
 * @note this setting would be cleared during #ZFFrameworkCleanup as #ZFLevelZFFrameworkEssential
 */
#define ZFTextTemplateRunParamDefault() _ZFP_ZFTextTemplateRunParamDefault()

/**
 * @brief util method to apply #ZFTextTemplateApply to a given path recursively
 */
ZFMETHOD_FUNC_DECLARE_4(zfbool, ZFTextTemplateRun,
                        ZFMP_IN(const zfchar *, path),
                        ZFMP_IN(const ZFTextTemplateParam &, textTemplateParam),
                        ZFMP_IN_OPT(const ZFTextTemplateRunParam &, runParam, ZFTextTemplateRunParamDefault()),
                        ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull))

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFTextTemplateRun_h_

