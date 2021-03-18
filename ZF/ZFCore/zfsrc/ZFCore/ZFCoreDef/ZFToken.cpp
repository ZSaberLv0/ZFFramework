#include "ZFToken.h"

ZF_NAMESPACE_GLOBAL_BEGIN

const ZFTokenForContainer _ZFP_ZFTokenForContainerTrim;

const ZFTokenForContainer _ZFP_ZFTokenForContainerDetail(
          "[\n    " // tokenLeft
        , "\n]"     // tokenRight
        , ",\n    " // tokenSeparator
        , ""        // tokenValueLeft
        , ""        // tokenValueRight
        , "..."     // tokenEtc
    );

const ZFTokenForContainer _ZFP_ZFTokenForContainerPlainList(
          ""        // tokenLeft
        , ""        // tokenRight
        , "\n"      // tokenSeparator
        , ""        // tokenValueLeft
        , ""        // tokenValueRight
        , "..."     // tokenEtc
    );

ZFTokenForContainer _ZFP_ZFTokenForContainerDefault = ZFTokenForContainerTrim();

// ============================================================
const ZFTokenForKeyValueContainer _ZFP_ZFTokenForKeyValueContainerTrim;
const ZFTokenForKeyValueContainer _ZFP_ZFTokenForKeyValueContainerDetail(
          "{\n    " // tokenLeft
        , "\n}"     // tokenRight
        , ",\n    " // tokenSeparator
        , ""        // tokenPairLeft
        , ""        // tokenPairRight
        , " = "     // tokenPairSeparator
        , ""        // tokenKeyLeft
        , ""        // tokenKeyRight
        , ""        // tokenValueLeft
        , ""        // tokenValueRight
        , "..."     // tokenEtc
    );
ZFTokenForKeyValueContainer _ZFP_ZFTokenForKeyValueContainerDefault = ZFTokenForKeyValueContainerTrim();

ZF_NAMESPACE_GLOBAL_END

