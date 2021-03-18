#include "ZFTextTemplateRun.h"

#include "ZFRegExp.h"

ZF_NAMESPACE_GLOBAL_BEGIN

#define _ZFP_ZFTextTemplateRunParamDefault_DECLARE(typeName) \
    static ZFRegExp *_ZFP_ZFTextTemplateRunParamDefault_##typeName##FilterRule = zfnull; \
    static ZFFilterCallbackResult _ZFP_ZFTextTemplateRunParamDefault_##typeName##Filter(ZF_IN const zfchar *const &value) \
    { \
        if(_ZFP_ZFTextTemplateRunParamDefault_##typeName##FilterRule != zfnull) \
        { \
            ZFRegExpResult regExpResult; \
            _ZFP_ZFTextTemplateRunParamDefault_##typeName##FilterRule->regExpMatch(regExpResult, value); \
            if(regExpResult.matched) \
            { \
                return ZFFilterCallbackResultNotActive; \
            } \
        } \
        return ZFFilterCallbackResultNotSpecified; \
    }

_ZFP_ZFTextTemplateRunParamDefault_DECLARE(dirName)
_ZFP_ZFTextTemplateRunParamDefault_DECLARE(dirContent)
_ZFP_ZFTextTemplateRunParamDefault_DECLARE(fileName)
_ZFP_ZFTextTemplateRunParamDefault_DECLARE(fileContent)

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFTextTemplateRunParamDefaultSetting, ZFLevelZFFrameworkHigh)
{
    #if 0
    _ZFP_ZFTextTemplateRunParamDefault_dirNameFilterRule = zfAlloc(ZFRegExp);
    ZFTextTemplateRunParamDefault().dirNameFilter.customFilterCallbackAdd(_ZFP_ZFTextTemplateRunParamDefault_dirNameFilter);
    _ZFP_ZFTextTemplateRunParamDefault_dirNameFilterRule->regExpCompile(
            ""
        );
    #endif

    _ZFP_ZFTextTemplateRunParamDefault_dirContentFilterRule = zfAlloc(ZFRegExp);
    ZFTextTemplateRunParamDefault().dirContentFilter.customFilterCallbackAdd(_ZFP_ZFTextTemplateRunParamDefault_dirContentFilter);
    _ZFP_ZFTextTemplateRunParamDefault_dirContentFilterRule->regExpCompile(
            "(^\\.git$)"
            "|(^\\.svn$)"
            "|(^\\.hg$)"
        );

    #if 0
    _ZFP_ZFTextTemplateRunParamDefault_fileNameFilterRule = zfAlloc(ZFRegExp);
    ZFTextTemplateRunParamDefault().fileNameFilter.customFilterCallbackAdd(_ZFP_ZFTextTemplateRunParamDefault_fileNameFilter);
    _ZFP_ZFTextTemplateRunParamDefault_fileNameFilterRule->regExpCompile(
            ""
        );
    #endif

    _ZFP_ZFTextTemplateRunParamDefault_fileContentFilterRule = zfAlloc(ZFRegExp);
    ZFTextTemplateRunParamDefault().fileContentFilter.customFilterCallbackAdd(_ZFP_ZFTextTemplateRunParamDefault_fileContentFilter);
    _ZFP_ZFTextTemplateRunParamDefault_fileContentFilterRule->regExpCompile(
            "(\\.ico$)"
            "|(\\.icns$)"

            "|(\\.png$)"
            "|(\\.jpg$)"
            "|(\\.jpeg$)"
            "|(\\.gif$)"

            "|(\\.mp2$)"
            "|(\\.mp3$)"
            "|(\\.mp4$)"
            "|(\\.wav$)"
            "|(\\.ogg$)"

            "|(\\.db$)"
            "|(\\.rar$)"
            "|(\\.zip$)"
            "|(\\.7z$)"

            "|(\\.a(\\..*)*$)"
            "|(\\.o(\\..*)*$)"
            "|(\\.so(\\..*)*$)"
            "|(\\.dll(\\..*)*$)"
            "|(\\.dylib(\\..*)*$)"
        );
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFTextTemplateRunParamDefaultSetting)
{
    ZFTextTemplateRunParamDefault().dirNameFilter.customFilterCallbackRemove(_ZFP_ZFTextTemplateRunParamDefault_dirNameFilter);
    zfRelease(_ZFP_ZFTextTemplateRunParamDefault_dirNameFilterRule);
    _ZFP_ZFTextTemplateRunParamDefault_dirNameFilterRule = zfnull;

    ZFTextTemplateRunParamDefault().dirContentFilter.customFilterCallbackRemove(_ZFP_ZFTextTemplateRunParamDefault_dirContentFilter);
    zfRelease(_ZFP_ZFTextTemplateRunParamDefault_dirContentFilterRule);
    _ZFP_ZFTextTemplateRunParamDefault_dirContentFilterRule = zfnull;

    ZFTextTemplateRunParamDefault().fileNameFilter.customFilterCallbackRemove(_ZFP_ZFTextTemplateRunParamDefault_fileNameFilter);
    zfRelease(_ZFP_ZFTextTemplateRunParamDefault_fileNameFilterRule);
    _ZFP_ZFTextTemplateRunParamDefault_fileNameFilterRule = zfnull;

    ZFTextTemplateRunParamDefault().fileContentFilter.customFilterCallbackRemove(_ZFP_ZFTextTemplateRunParamDefault_fileContentFilter);
    zfRelease(_ZFP_ZFTextTemplateRunParamDefault_fileContentFilterRule);
    _ZFP_ZFTextTemplateRunParamDefault_fileContentFilterRule = zfnull;
}
ZF_GLOBAL_INITIALIZER_END(ZFTextTemplateRunParamDefaultSetting)

ZF_NAMESPACE_GLOBAL_END

