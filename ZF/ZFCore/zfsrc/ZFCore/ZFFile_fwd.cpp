#include "ZFFile_impl.cpp"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFFileSeparator
const zfchar _ZFP_ZFFileSeparator = '/';
const zfchar *_ZFP_ZFFileSeparatorString = "/";
ZFEXPORT_VAR_READONLY_VALUEREF_DEFINE(zfchar, ZFFileSeparator, _ZFP_ZFFileSeparator)
ZFEXPORT_VAR_READONLY_VALUEREF_DEFINE(const zfchar *, ZFFileSeparatorString, _ZFP_ZFFileSeparatorString)

// ============================================================
// ZFFileOpenOption
ZFENUM_DEFINE_FLAGS(ZFFileOpenOption, ZFFileOpenOptionFlags)

// ============================================================
// ZFFileFindData
zfclassNotPOD _ZFP_ZFFileFindDataPrivate
{
public:
    zfuint refCount;
    zfstring implName; // empty shows find not started
    void *implUserData;
    ZFFileFindData::Impl impl;
public:
    _ZFP_ZFFileFindDataPrivate(void)
    : refCount(1)
    , implName()
    , implUserData(zfnull)
    , impl()
    {
    }
    ~_ZFP_ZFFileFindDataPrivate(void)
    {
        zfCoreAssertWithMessage(this->implName.isEmpty(),
            "have you forgot to close find? module: %s",
            this->implName.cString());
    }
};

ZFFileFindData::ZFFileFindData(void)
: d(zfnew(_ZFP_ZFFileFindDataPrivate))
{
}
ZFFileFindData::ZFFileFindData(ZF_IN ZFFileFindData const &ref)
: d(ref.d)
{
    ++(d->refCount);
}
ZFFileFindData &ZFFileFindData::operator = (ZF_IN ZFFileFindData const &ref)
{
    ++(ref.d->refCount);
    --(d->refCount);
    if(d->refCount == 0)
    {
        zfdelete(d);
    }
    d = ref.d;
    return *this;
}
zfbool ZFFileFindData::operator == (ZF_IN ZFFileFindData const &ref) const
{
    return (d->impl.nativeFd == ref.d->impl.nativeFd);
}
ZFFileFindData::~ZFFileFindData(void)
{
    --(d->refCount);
    if(d->refCount == 0)
    {
        zfdelete(d);
    }
}

void ZFFileFindData::objectInfoT(ZF_IN_OUT zfstring &ret) const
{
    ret += ZFTOKEN_ZFObjectInfoLeft;
    if(this->fileIsDir())
    {
        ret += "(dir)";
    }
    ret += this->fileName();
    ret += ZFTOKEN_ZFObjectInfoRight;
}

ZFFileFindData::Impl &ZFFileFindData::impl(void) const
{
    return d->impl;
}
const zfchar *ZFFileFindData::implName(void) const
{
    return (d->implName.isEmpty() ? zfnull : d->implName.cString());
}
void *ZFFileFindData::implUserData(void) const
{
    return d->implUserData;
}
void ZFFileFindData::implAttach(ZF_IN const zfchar *implName,
                                ZF_IN_OPT void *implUserData /* = zfnull */)
{
    zfCoreAssertWithMessage(d->implName.isEmpty(),
        "have you forgot to close find? current module: %s",
        d->implName.cString());
    zfCoreAssert(!zfsIsEmpty(implName));
    d->implName = implName;
    d->implUserData = implUserData;
}
void ZFFileFindData::implDetach(void)
{
    d->implName.removeAll();
    d->implUserData = zfnull;
}
void *ZFFileFindData::implCheck(ZF_IN const zfchar *implName) const
{
    zfCoreAssertWithMessage(!d->implName.isEmpty(), "find not started");
    zfCoreAssertWithMessage(zfscmpTheSame(d->implName.cString(), implName),
        "have you forgot to close find? desired, %s, got: %s",
        implName, d->implName.cString());
    return d->implUserData;
}

// ============================================================
ZFTYPEID_ACCESS_ONLY_DEFINE(ZFFileFindData, ZFFileFindData)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFFileFindData, const zfchar *, fileName)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFFileFindData, zfbool, fileIsDir)

ZF_NAMESPACE_GLOBAL_END

