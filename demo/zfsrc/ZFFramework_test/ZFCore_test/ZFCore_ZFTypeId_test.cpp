#include "ZFCore_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclass ZFCore_ZFTypeId_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFCore_ZFTypeId_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();

        zfautoObject v;

        zfblockedAlloc(v_zfstring, test_zfstring, "string type");
        v = test_zfstring;

        // for aliased type, when accessed, a new instance would be created and stored as tag in holder object
        typedef const zfchar * _ZFP_AliasedChar;
        zfLogTrimT() << "============================================================";
        zfLogTrimT() << "const zfchar *";
        zfLogTrimT() << ZFTypeId<_ZFP_AliasedChar>::Value<_ZFP_AliasedChar>::zfvAccess(v);
        zfLogTrimT() << ZFTypeId<_ZFP_AliasedChar>::Value<const _ZFP_AliasedChar &>::zfvAccess(v);
        zfLogTrimT() << ZFTypeId<_ZFP_AliasedChar>::Value<_ZFP_AliasedChar &>::zfvAccess(v);
        zfLogTrimT() << ZFTypeId<_ZFP_AliasedChar>::Value<_ZFP_AliasedChar *>::zfvAccess(v);
        zfLogTrimT() << ZFTypeId<_ZFP_AliasedChar>::Value<const _ZFP_AliasedChar *>::zfvAccess(v);
        zfLogTrimT() << ZFTypeId<_ZFP_AliasedChar>::Value<_ZFP_AliasedChar * &>::zfvAccess(v);
        zfLogTrimT() << ZFTypeId<_ZFP_AliasedChar>::Value<_ZFP_AliasedChar * const &>::zfvAccess(v);
        zfLogTrimT() << ZFTypeId<_ZFP_AliasedChar>::Value<const _ZFP_AliasedChar * &>::zfvAccess(v);
        zfLogTrimT() << ZFTypeId<_ZFP_AliasedChar>::Value<const _ZFP_AliasedChar * const &>::zfvAccess(v);

        // for wrapped type, all should be able to access
        zfLogTrimT() << "============================================================";
        zfLogTrimT() << "zfstring";
        zfLogTrimT() << ZFTypeId<zfstring>::Value<zfstring>::zfvAccess(v);
        zfLogTrimT() << ZFTypeId<zfstring>::Value<const zfstring &>::zfvAccess(v);
        zfLogTrimT() << ZFTypeId<zfstring>::Value<zfstring &>::zfvAccess(v);
        zfLogTrimT() << ZFTypeId<zfstring>::Value<zfstring *>::zfvAccess(v);
        zfLogTrimT() << ZFTypeId<zfstring>::Value<const zfstring *>::zfvAccess(v);
        zfLogTrimT() << ZFTypeId<zfstring>::Value<zfstring * &>::zfvAccess(v);
        zfLogTrimT() << ZFTypeId<zfstring>::Value<zfstring * const &>::zfvAccess(v);
        zfLogTrimT() << ZFTypeId<zfstring>::Value<const zfstring * &>::zfvAccess(v);
        zfLogTrimT() << ZFTypeId<zfstring>::Value<const zfstring * const &>::zfvAccess(v);

        // zfautoObject processed as normal raw type
        zfblockedAlloc(v_zfstring, test_String, "zfautoObject");
        v = test_String;
        zfLogTrimT() << "============================================================";
        zfLogTrimT() << "zfautoObject";
        zfLogTrimT() << ZFTypeId<zfautoObject>::Value<zfautoObject>::zfvAccess(v);
        zfLogTrimT() << ZFTypeId<zfautoObject>::Value<const zfautoObject &>::zfvAccess(v);
        zfLogTrimT() << ZFTypeId<zfautoObject>::Value<zfautoObject &>::zfvAccess(v);
        zfLogTrimT() << ZFTypeId<zfautoObject>::Value<zfautoObject *>::zfvAccess(v);
        zfLogTrimT() << ZFTypeId<zfautoObject>::Value<const zfautoObject *>::zfvAccess(v);
        zfLogTrimT() << ZFTypeId<zfautoObject>::Value<zfautoObject * &>::zfvAccess(v);
        zfLogTrimT() << ZFTypeId<zfautoObject>::Value<zfautoObject * const &>::zfvAccess(v);
        zfLogTrimT() << ZFTypeId<zfautoObject>::Value<const zfautoObject * &>::zfvAccess(v);
        zfLogTrimT() << ZFTypeId<zfautoObject>::Value<const zfautoObject * const &>::zfvAccess(v);

        // ZFAny processed as aligned type
        zfblockedAlloc(v_zfstring, test_ZFAny, "ZFAny");
        v = test_ZFAny;
        zfLogTrimT() << "============================================================";
        zfLogTrimT() << "ZFAny";
        zfLogTrimT() << ZFTypeId<ZFAny>::Value<ZFAny>::zfvAccess(v);
        zfLogTrimT() << ZFTypeId<ZFAny>::Value<const ZFAny &>::zfvAccess(v);
        zfLogTrimT() << ZFTypeId<ZFAny>::Value<ZFAny &>::zfvAccess(v);
        zfLogTrimT() << ZFTypeId<ZFAny>::Value<ZFAny *>::zfvAccess(v);
        zfLogTrimT() << ZFTypeId<ZFAny>::Value<const ZFAny *>::zfvAccess(v);
        zfLogTrimT() << ZFTypeId<ZFAny>::Value<ZFAny * &>::zfvAccess(v);
        zfLogTrimT() << ZFTypeId<ZFAny>::Value<ZFAny * const &>::zfvAccess(v);
        zfLogTrimT() << ZFTypeId<ZFAny>::Value<const ZFAny * &>::zfvAccess(v);
        zfLogTrimT() << ZFTypeId<ZFAny>::Value<const ZFAny * const &>::zfvAccess(v);

        this->testCaseStop();
    }
};
ZFOBJECT_REGISTER(ZFCore_ZFTypeId_test)

ZF_NAMESPACE_GLOBAL_END

