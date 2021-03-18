#include "ZFAlgorithm_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFAlgorithm_ZFRegExp_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFAlgorithm_ZFRegExp_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();

        zfblockedAlloc(ZFRegExp, regexp);
        const zfchar *patternFrom = "(ab*c)(?<n0>ab*c)";
        const zfchar *patternTo = "{$1}";
        const zfchar *stringFrom = "zacabcabbcabbbcz";

        regexp->regExpCompile(patternFrom);

        {
            ZFRegExpResult result;
            this->testCaseOutputSeparator();
            this->testCaseOutput("find:");
            regexp->regExpMatch(result, stringFrom);
            this->testCaseOutput("  pattern    : %s", patternFrom);
            this->testCaseOutput("  string     : %s", stringFrom);
            this->testCaseOutput("  result     : %s", result.objectInfo().cString());
            this->testCaseOutput("  named group: %zi", regexp->regExpNamedGroupIndexForName("n0"));
        }

        {
            ZFRegExpResult result;
            zfstring stringTo;
            zfindex maxCount = zfindexMax();
            this->testCaseOutputSeparator();
            this->testCaseOutput("replace:");
            regexp->regExpReplace(stringTo, result, stringFrom, patternTo, maxCount);
            this->testCaseOutput("  pattern from: %s", patternFrom);
            this->testCaseOutput("  pattern to  : %s", patternTo);
            this->testCaseOutput("  string  from: %s", stringFrom);
            this->testCaseOutput("  string  to  : %s", stringTo.cString());
            this->testCaseOutput("  match result: %s", result.objectInfo().cString());
            this->testCaseOutput("  named group: %zi", regexp->regExpNamedGroupIndexForName("n0"));
        }
        this->testCaseStop();
    }
};
ZFOBJECT_REGISTER(ZFAlgorithm_ZFRegExp_test)

ZF_NAMESPACE_GLOBAL_END

