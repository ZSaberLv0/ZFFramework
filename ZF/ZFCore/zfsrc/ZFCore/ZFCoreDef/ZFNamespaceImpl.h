/**
 * @file ZFNamespaceImpl.h
 * @brief namespace impl
 */
#ifndef _ZFI_ZFNamespaceImpl_h_
#define _ZFI_ZFNamespaceImpl_h_

#include "ZFCoreTypeDef.h"
#include "ZFCoreArray.h"
#include "ZFCoreStaticRegister.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief separator token for #ZF_NAMESPACE_CURRENT
 */
#define ZFNamespaceSeparator() "."
/**
 * @brief separator token for #ZF_NAMESPACE_CURRENT
 */
#define ZFNamespaceSeparatorLen() 1 // zfslen(ZFNamespaceSeparator())

/**
 * @brief register namespace
 */
#define ZF_NAMESPACE_REGISTER(NameSpace, ParentNameSpace) \
    _ZFP_ZF_NAMESPACE_REGISTER(NameSpace, ParentNameSpace)
#define _ZFP_ZF_NAMESPACE_REGISTER(NameSpace, ParentNameSpace) \
    const char *_ZFP_ZF_NAMESPACE_NOT_DECLARED(void) \
    { \
        static _ZFP_ZFNamespaceHolder d(ParentNameSpace::_ZFP_ZF_NAMESPACE_NOT_DECLARED(), #NameSpace); \
        return d.ns.cString(); \
    } \
    ZF_STATIC_REGISTER_INIT(_ZFP_NSReg) \
    { \
        _ZFP_ZF_NAMESPACE_NOT_DECLARED(); \
    } \
    ZF_STATIC_REGISTER_END(_ZFP_NSReg)
extern ZF_ENV_EXPORT zfstring _ZFP_ZFNamespaceRegister(ZF_IN const char *parent,
                                                       ZF_IN const char *child);
extern ZF_ENV_EXPORT void _ZFP_ZFNamespaceUnregister(ZF_IN const char *ns);
class _ZFP_ZFNamespaceHolder
{
public:
    _ZFP_ZFNamespaceHolder(const char *parent,
                           const char *child)
    : ns(_ZFP_ZFNamespaceRegister(parent, child))
    {
    }
    ~_ZFP_ZFNamespaceHolder(void)
    {
        _ZFP_ZFNamespaceUnregister(this->ns.cString());
    }
public:
    zfstring ns;
};

/**
 * @brief get current namespace name
 *
 * note:
 * -  global namespace and ZFFramework's main namespace (#ZF_NAMESPACE_GLOBAL) are ensured null
 * -  embeded namespace are supported,
 *   however, you must ensure it's valid declared by #ZF_NAMESPACE_REGISTER or #ZF_NAMESPACE_END_WITH_REGISTER
 * -  for embeded namespace, the namespace text are something looks like "GrandParent.Parent.Child",
 *   the separator token can be accessed by #ZFNamespaceSeparator
 */
#define ZF_NAMESPACE_CURRENT() \
    _ZFP_ZF_NAMESPACE_NOT_DECLARED()

// ============================================================
/**
 * @brief skip "zf." in "zf.NS0.NS1.funcName"
 *
 * things to skip:
 * -  #ZFNamespaceSeparator at head ("." in ".NS0")
 * -  #ZF_NAMESPACE_GLOBAL_NAME
 * -  #ZF_NAMESPACE_GLOBAL_ABBR_NAME
 *
 * return null if everything are skipped
 */
extern ZF_ENV_EXPORT const zfchar *ZFNamespaceSkipGlobal(ZF_IN const zfchar *ns);
/**
 * @brief util to split namespace components, similar to #zfCoreDataPairSplitString
 *
 * if success, ret ensured to have at least one element even if the namespace is empty,
 * at this case, the only one element would be #ZFIndexRangeZero
 */
extern ZF_ENV_EXPORT zfbool ZFNamespaceSplit(ZF_IN_OUT ZFCoreArray<ZFIndexRange> &ret,
                                             ZF_IN const zfchar *src,
                                             ZF_IN_OPT zfindex srcLen = zfindexMax());

// ============================================================
/**
 * @brief get all namespace
 */
extern ZF_ENV_EXPORT void ZFNamespaceGetAllT(ZF_IN_OUT ZFCoreArray<const zfchar *> &ret);
/** @brief see #ZFNamespaceGetAllT */
inline ZFCoreArrayPOD<const zfchar *> ZFNamespaceGetAll(void)
{
    ZFCoreArrayPOD<const zfchar *> ret;
    ZFNamespaceGetAllT(ret);
    return ret;
}

/** @brief see #ZFNamespaceGetAllT */
extern ZF_ENV_EXPORT void ZFNamespaceGetAllT(ZF_IN_OUT ZFCoreArray<const zfchar *> &ret,
                                             ZF_IN const zfchar *parent,
                                             ZF_IN_OPT zfbool recursive = zffalse);
/** @brief see #ZFNamespaceGetAllT */
inline ZFCoreArrayPOD<const zfchar *> ZFNamespaceGetAll(ZF_IN const zfchar *parent,
                                                        ZF_IN_OPT zfbool recursive = zffalse)
{
    ZFCoreArrayPOD<const zfchar *> ret;
    ZFNamespaceGetAllT(ret, parent, recursive);
    return ret;
}

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFNamespaceImpl_h_

